#ifndef UTILS
#define UTILS

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <Arduino.h>
#include <cstdio>

/**
 * @brief Converts the value to a Little-Endian and wrtites it to the buffer
 *
 * @param buffer Byte buffer to write the result to
 * @param offset Offset in the buffer to write from
 * @param value Value to be converted
 */
inline void writeLE16(uint8_t* buffer, size_t& offset, uint16_t value){
  buffer[offset++] = value & 0xFF;
  buffer[offset++] = (value >> 8) & 0xFF;
}

/**
 * @brief Convert the Little-Endian bytes from the buffer to the 2 byte value
 *
 * @param buffer Byte buffer to read from
 * @param offset Offset to read from in the buffer
 * @return Two bytes value
 */
inline uint16_t readLE16(const uint8_t* buffer, size_t& offset) {
  return static_cast<uint16_t>(buffer[offset]) | (static_cast<uint16_t>(buffer[offset + 1]) << 8);
}

/**
 * @brief Generates a packet ID from mac and a random number
 *
 * @param mac MAC address of the sender
 * @return Packet ID
 */
inline uint16_t generatePktId(const uint8_t mac[6]) {
  uint8_t mac_hash = mac[4] ^ mac[5];
  uint8_t rand_part = rand() & 0xFF;
  return (static_cast<uint16_t>(mac_hash) << 8) | rand_part;
}

/**
 * @brief Convertes a string containing a MAC address to an array of bytes
 *
 * @param macStr MAC string to be converted
 * @param mac Array of bytes to be written to
 */
inline void macStringToBytes(const String& macStr, uint8_t mac[6]) {
  int values[6];
  sscanf(macStr.c_str(), "%02x:%02x:%02x:%02x:%02x:%02x",
          &values[0], &values[1], &values[2],
          &values[3], &values[4], &values[5]);
  for (int i = 0; i < 6; i++) mac[i] = static_cast<uint8_t>(values[i]);
}

/**
 * @brief Convertes a byte array to a MAC string
 *
 * @param mac_bytes Array of MAC bytes
 * @return String containinf a MAC address
 */
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

/**
 * @brief Checks if two MAC addresses are equal
 *
 * @param a_mac First MAC address
 * @param b_mac Second MAC address
 * @return True, if both of the MAC addresses are identical
 */
inline bool isMACEqual(const uint8_t a_mac[6], const uint8_t b_mac[6]){
  for (size_t i = 0; i < 6; ++i) {
    if (a_mac[i] != b_mac[i]) {
      return false;
    }
  }
  return true;
}

#endif // !UTILS
