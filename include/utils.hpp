#ifndef UTILS
#define UTILS

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <Arduino.h>
#include <cstdio>
#include <string>

inline void writeLE16(uint8_t* buffer, size_t& offset, uint16_t value){
  buffer[offset++] = value & 0xFF;
  buffer[offset++] = (value >> 8) & 0xFF;
}

inline uint16_t readLE16(const uint8_t* buffer, size_t& offset) {
  return static_cast<uint16_t>(buffer[offset]) | (static_cast<uint16_t>(buffer[offset + 1]) << 8);
}

inline uint16_t generatePktId(const uint8_t mac[6]) {
  uint8_t mac_hash = mac[4] ^ mac[5];
  uint8_t rand_part = rand() & 0xFF;
  return (static_cast<uint16_t>(mac_hash) << 8) | rand_part;
}

inline void macStringToBytes(const String& macStr, uint8_t mac[6]) {
  int values[6];
  sscanf(macStr.c_str(), "%02x:%02x:%02x:%02x:%02x:%02x",
          &values[0], &values[1], &values[2],
          &values[3], &values[4], &values[5]);
  for (int i = 0; i < 6; i++) mac[i] = static_cast<uint8_t>(values[i]);
}

inline String macBytesToString(const uint8_t mac_bytes[6]) {
    char buffer[18];
    std::snprintf(
        buffer, sizeof(buffer),
        "%02X:%02X:%02X:%02X:%02X:%02X",
        mac_bytes[0], mac_bytes[1], mac_bytes[2],
        mac_bytes[3], mac_bytes[4], mac_bytes[5]
    );
    return String(buffer);
}

inline bool isMACEqual(const uint8_t a_mac[6], const uint8_t b_mac[6]){
  for (size_t i = 0; i < 6; ++i) {
    if (a_mac[i] != b_mac[i]) {
      return false;
    }
  }
  return true;
}

#endif // !UTILS
