#ifndef UTILS
#define UTILS

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <Arduino.h>
#include <cstdio>

inline void writeLE16(uint8_t* buffer, size_t& offset, uint16_t value){
    buffer[offset++] = value & 0xFF;
    buffer[offset++] = (value >> 8) & 0xFF;
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

#endif // !UTILS
