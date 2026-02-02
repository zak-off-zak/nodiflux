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

  // TODO: private hash functions here

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
