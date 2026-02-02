#include "BloomFilter.hpp"
#include <mutex>

void BloomFilter::addEntry(uint16_t pkt_id) {
  std::lock_guard<std::mutex> lock(this->mtx);
}

bool BloomFilter::isSeen(uint16_t pkt_id) { return false; }
