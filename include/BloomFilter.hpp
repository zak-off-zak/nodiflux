#ifndef BLOOM_FILTER
#define BLOOM_FILTER

#include "config.hpp"
#include <bitset>
#include <cstdint>
#include <mutex>

/**
 * @class BloomFilter
 * @brief The class responsible for storing a representation of the seen packets
 *
 */
class BloomFilter {
private:
  mutable std::mutex mtx;
  std::bitset<BLOOM_FILTER_SIZE> bloom;

  /**
   * @brief A hash function to be used as a fist stage hash function in the
   * BloomFilter
   *
   * @param data Data to be hashed
   * @return A 16-bit hash of the data
   */
  uint16_t h1(const uint16_t data);

  /**
   * @brief A hash function to be used as a second stage hash function in the
   * BloomFilter
   *
   * @param data Data to be hashed
   * @return A 16-bit hash of the data
   */
  uint16_t h2(const uint16_t data);

  /**
   * @brief A hash function to be used as a third stage hash function in the
   * BloomFilter
   *
   * @param data Data to be hashed
   * @return A 16-bit hash of the data
   */
  uint16_t h3(const uint16_t data);

  /**
   * @brief The mixing function to be used in the h1
   *
   * @param message_block A block of the message to be mixed
   * @param internal_state The internal sate of the hash function
   * @return New combined internal state of the hash function
   */
  uint8_t mix1(uint8_t message_block, uint8_t internal_state);

  /**
   * @brief A collection of random permutations taken from the RFC 3074
   */
  uint8_t pearson_permutations[256] = {
      251, 175, 119, 215, 81,  14,  79,  191, 103, 49,  181, 143, 186, 157, 0,
      232, 31,  32,  55,  60,  152, 58,  17,  237, 174, 70,  160, 144, 220, 90,
      57,  223, 59,  3,   18,  140, 111, 166, 203, 196, 134, 243, 124, 95,  222,
      179, 197, 65,  180, 48,  36,  15,  107, 46,  233, 130, 165, 30,  123, 161,
      209, 23,  97,  16,  40,  91,  219, 61,  100, 10,  210, 109, 250, 127, 22,
      138, 29,  108, 244, 67,  207, 9,   178, 204, 74,  98,  126, 249, 167, 116,
      34,  77,  193, 200, 121, 5,   20,  113, 71,  35,  128, 13,  182, 94,  25,
      226, 227, 199, 75,  27,  41,  245, 230, 224, 43,  225, 177, 26,  155, 150,
      212, 142, 218, 115, 241, 73,  88,  105, 39,  114, 62,  255, 192, 201, 145,
      214, 168, 158, 221, 148, 154, 122, 12,  84,  82,  163, 44,  139, 228, 236,
      205, 242, 217, 11,  187, 146, 159, 64,  86,  239, 195, 42,  106, 198, 118,
      112, 184, 172, 87,  2,   173, 117, 176, 229, 247, 253, 137, 185, 99,  164,
      102, 147, 45,  66,  231, 52,  141, 211, 194, 206, 246, 238, 56,  110, 78,
      248, 63,  240, 189, 93,  92,  51,  53,  183, 19,  171, 72,  50,  33,  104,
      101, 69,  8,   252, 83,  120, 76,  135, 85,  54,  202, 125, 188, 213, 96,
      235, 136, 208, 162, 129, 190, 132, 156, 38,  47,  1,   7,   254, 24,  4,
      216, 131, 89,  21,  28,  133, 37,  153, 149, 80,  170, 68,  6,   169, 234,
      151};

  uint8_t crc_8_autostar_lookup_table[256] = {
      0x00, 0x2F, 0x5E, 0x71, 0xBC, 0x93, 0xE2, 0xCD, 0x57, 0x78, 0x09, 0x26,
      0xEB, 0xC4, 0xB5, 0x9A, 0xAE, 0x81, 0xF0, 0xDF, 0x12, 0x3D, 0x4C, 0x63,
      0xF9, 0xD6, 0xA7, 0x88, 0x45, 0x6A, 0x1B, 0x34, 0x73, 0x5C, 0x2D, 0x02,
      0xCF, 0xE0, 0x91, 0xBE, 0x24, 0x0B, 0x7A, 0x55, 0x98, 0xB7, 0xC6, 0xE9,
      0xDD, 0xF2, 0x83, 0xAC, 0x61, 0x4E, 0x3F, 0x10, 0x8A, 0xA5, 0xD4, 0xFB,
      0x36, 0x19, 0x68, 0x47, 0xE6, 0xC9, 0xB8, 0x97, 0x5A, 0x75, 0x04, 0x2B,
      0xB1, 0x9E, 0xEF, 0xC0, 0x0D, 0x22, 0x53, 0x7C, 0x48, 0x67, 0x16, 0x39,
      0xF4, 0xDB, 0xAA, 0x85, 0x1F, 0x30, 0x41, 0x6E, 0xA3, 0x8C, 0xFD, 0xD2,
      0x95, 0xBA, 0xCB, 0xE4, 0x29, 0x06, 0x77, 0x58, 0xC2, 0xED, 0x9C, 0xB3,
      0x7E, 0x51, 0x20, 0x0F, 0x3B, 0x14, 0x65, 0x4A, 0x87, 0xA8, 0xD9, 0xF6,
      0x6C, 0x43, 0x32, 0x1D, 0xD0, 0xFF, 0x8E, 0xA1, 0xE3, 0xCC, 0xBD, 0x92,
      0x5F, 0x70, 0x01, 0x2E, 0xB4, 0x9B, 0xEA, 0xC5, 0x08, 0x27, 0x56, 0x79,
      0x4D, 0x62, 0x13, 0x3C, 0xF1, 0xDE, 0xAF, 0x80, 0x1A, 0x35, 0x44, 0x6B,
      0xA6, 0x89, 0xF8, 0xD7, 0x90, 0xBF, 0xCE, 0xE1, 0x2C, 0x03, 0x72, 0x5D,
      0xC7, 0xE8, 0x99, 0xB6, 0x7B, 0x54, 0x25, 0x0A, 0x3E, 0x11, 0x60, 0x4F,
      0x82, 0xAD, 0xDC, 0xF3, 0x69, 0x46, 0x37, 0x18, 0xD5, 0xFA, 0x8B, 0xA4,
      0x05, 0x2A, 0x5B, 0x74, 0xB9, 0x96, 0xE7, 0xC8, 0x52, 0x7D, 0x0C, 0x23,
      0xEE, 0xC1, 0xB0, 0x9F, 0xAB, 0x84, 0xF5, 0xDA, 0x17, 0x38, 0x49, 0x66,
      0xFC, 0xD3, 0xA2, 0x8D, 0x40, 0x6F, 0x1E, 0x31, 0x76, 0x59, 0x28, 0x07,
      0xCA, 0xE5, 0x94, 0xBB, 0x21, 0x0E, 0x7F, 0x50, 0x9D, 0xB2, 0xC3, 0xEC,
      0xD8, 0xF7, 0x86, 0xA9, 0x64, 0x4B, 0x3A, 0x15, 0x8F, 0xA0, 0xD1, 0xFE,
      0x33, 0x1C, 0x6D, 0x42};

public:
  /**
   * @brief Default constructor
   */
  BloomFilter() = default;

  /**
   * @brief Delete the copy constructor
   */
  BloomFilter(const BloomFilter &) = delete;

  /**
   * @brief Detele copy assignment operator
   *
   * @return BloomFilter
   */
  BloomFilter &operator=(const BloomFilter &) = delete;

  /**
   * @brief Method to get the singelton instance of the BloomFilter
   *
   * @return Singelton instance of the BloomFilter
   */
  static BloomFilter &instance() {
    static BloomFilter instance;
    return instance;
  }

  /**
   * @brief The method to add new entries to the BloomFilter
   *
   * @param pkt_id The id of the packet to be stored in the BloomFilter
   */
  void addEntry(uint16_t pkt_id);

  /**
   * @brief The function to determine whether a packet id has been seen alredy
   *
   * @param pkt_id The id of the packet to check
   * @return True, if the item is likley to be seen, flase, if the item is for
   * sure has not been seen before
   */
  bool isSeen(uint16_t pkt_id);
};

#endif // !BLOOM_FILTER
