#include "NodeRegistry.hpp"
#include "HardwareSerial.h"
#include <mutex>


void NodeRegistry::updateNode(const uint8_t mac[6], time_t t) {
  std::lock_guard<std::mutex> lock(this->mtx);
  std::array<uint8_t, 6> key;
  std::copy(mac, mac+6, key.begin());
  nodes[key] = t;
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
