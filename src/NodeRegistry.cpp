#include "NodeRegistry.hpp"
#include "HardwareSerial.h"
#include <cstdint>
#include <ctime>
#include <limits>
#include <mutex>
#include "config.hpp"
#include "Protocol.hpp"


void NodeRegistry::updateNode(const uint8_t mac[6], time_t t) {
  std::lock_guard<std::mutex> lock(this->mtx);
  // USED FOR TESTING ONLY
  const uint8_t banned_mac[] = {0x20, 0xE7, 0xC8, 0x59, 0xE9, 0x18};
  if (memcmp(mac, banned_mac, 6) == 0) {
    return;
  }
  std::array<uint8_t, 6> key;
  std::copy(mac, mac+6, key.begin());
  if (this->nodes.find(key) == this->nodes.end()){
    establishPeer(mac, PEER_CHANNEL, PEER_ENCRYPT);
  }
  if(this->nodes.size() < NODE_REGISTRY_SIZE){
    this->nodes[key] = t;
  }
}

std::array<uint8_t, 6> NodeRegistry::getMostRecentNode() const{
  std::lock_guard<std::mutex> lock(mtx);
  if (this->nodes.empty()) return {0,0,0,0,0,0};
  std::array<uint8_t, 6> result = {0,0,0,0,0,0};
  time_t min_score = std::numeric_limits<time_t>::max();
  time_t score = 0;
  time_t now = std::time(nullptr);
  for (const auto& [mac, lastSeen] :this->nodes){
    score = std::difftime(now, lastSeen);
    if(score < min_score){
      result = mac;
      min_score = score;
    }
  }
  return result;
}

bool NodeRegistry::peerExists(const uint8_t mac[6]){
  std::lock_guard<std::mutex> lock(mtx);
  std::array<uint8_t, 6> key;
  std::copy(mac, mac+6, key.begin());
  return (this->nodes.find(key) != this->nodes.end());
}


void NodeRegistry::debug() const {
    std::lock_guard<std::mutex> lock(mtx);

    for (const auto& [mac, lastSeen] : nodes) {

        for (size_t i = 0; i < mac.size(); ++i) {
            if (mac[i] < 0x10)
                Serial.print('0');

            Serial.print(mac[i], HEX);

            if (i < mac.size() - 1)
                Serial.print(':');
        }

        Serial.print("  lastSeen: ");
        Serial.println(static_cast<long>(lastSeen));
    }
}


void NodeRegistry::debugMostRecentNode(){
    std::array<uint8_t, 6> mostRecent = this->getMostRecentNode();

    bool empty = true;
    for (auto b : mostRecent) {
        if (b != 0) {
            empty = false;
            break;
        }
    }

    if (empty) {
        Serial.println("NodeRegistry is empty!");
        return;
    }

    Serial.print("Most recent node: ");
    for (size_t i = 0; i < mostRecent.size(); ++i) {
        if (mostRecent[i] < 0x10) Serial.print('0');
        Serial.print(static_cast<unsigned int>(mostRecent[i]), HEX);
        if (i < mostRecent.size() - 1) Serial.print(':');
    }
    Serial.println();
}
