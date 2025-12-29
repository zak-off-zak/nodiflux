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

  if(TESTING_ENABLED){
    const uint8_t banned_mac[] = {0x20, 0xE7, 0xC8, 0x5A, 0x2D, 0x18};
    if (memcmp(mac, banned_mac, 6) == 0) {
      return;
    }
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
