#include "NodeRegistry.hpp"
#include "HardwareSerial.h"
#include <cstdint>
#include <ctime>
#include <limits>
#include <mutex>


void NodeRegistry::updateNode(const uint8_t mac[6], time_t t) {
  std::lock_guard<std::mutex> lock(this->mtx);
  std::array<uint8_t, 6> key;
  std::copy(mac, mac+6, key.begin());
  nodes[key] = t;
}

std::array<uint8_t, 6> NodeRegistry::getMostRecentNode() const{
  std::lock_guard<std::mutex> lock(mtx);
  if (this->nodes.empty()) return {0,0,0,0,0,0};
  std::array<uint8_t, 6> result = {0,0,0,0,0,0};
  time_t min_score = std::numeric_limits<time_t>::max();
  time_t score = 0;
  time_t now = std::time(nullptr);
  for (const auto& [mac, lastSeen] : nodes){
    score = std::difftime(now, lastSeen);
    if(score < min_score){
      result = mac;
      min_score = score;
    }
  }
  return result;
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


    auto mac = this->getMostRecentNode();
    Serial.print("Most recent node: ");
    for (size_t i = 0; i < mac.size(); ++i) {
      if (mac[i] < 0x10) Serial.print('0');
      Serial.print(static_cast<unsigned int>(mac[i]), HEX);
      if (i < mac.size()-1) Serial.print(':');
    }
    Serial.println();
}
