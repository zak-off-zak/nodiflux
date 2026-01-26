#ifndef BLOOM_FILTER
#define BLOOM_FILTER

#include "config.hpp"
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
  uint8_t bloom[BLOOM_FILTER / 8];

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
};

#endif // !BLOOM_FILTER
