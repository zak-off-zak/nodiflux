#include "BloomFilter.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <mutex>

void BloomFilter::addEntry(uint16_t pkt_id) {
  std::lock_guard<std::mutex> lock(this->mtx);
  this->bloom.set(this->h1(pkt_id) % this->bloom.size());
  this->bloom.set(this->h2(pkt_id) % this->bloom.size());
  this->bloom.set(this->h3(pkt_id) % this->bloom.size());
}

bool BloomFilter::isSeen(uint16_t pkt_id) {
  std::lock_guard<std::mutex> lock(this->mtx);
  return this->bloom.test(this->h1(pkt_id) % this->bloom.size()) &&
         this->bloom.test(this->h2(pkt_id) % this->bloom.size()) &&
         this->bloom.test(this->h3(pkt_id) % this->bloom.size());
}

uint8_t BloomFilter::mix1(uint8_t message_block, uint8_t internal_state) {
  return ((message_block << 4) * (internal_state >> 3));
}

uint16_t BloomFilter::h1(const uint16_t data) {
  uint8_t internal_state = 0xA5;

  const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&data);
  for (size_t i = 0; i < sizeof(data); i++) {
    internal_state = this->mix1(bytes[i], internal_state);
  }

  return (static_cast<uint16_t>(internal_state) << 8) |
         (static_cast<uint16_t>(this->mix1(internal_state, internal_state)));
}

uint16_t BloomFilter::h2(const uint16_t data) {
  uint8_t internal_state = 0xA6;

  const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&data);
  for (size_t i = 0; i < sizeof(data); i++) {
    internal_state = this->pearson_permutations[internal_state % bytes[i]];
  }

  return (static_cast<uint16_t>(internal_state) << 8) |
         (static_cast<uint16_t>(
             this->pearson_permutations[internal_state % 0xA6]));
}

uint16_t BloomFilter::h3(const uint16_t data) {
  uint8_t internal_state = 0xA7;

  const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&data);
  for (size_t i = 0; i < sizeof(data); i++) {
    internal_state =
        this->crc_8_autostar_lookup_table
            [internal_state % sizeof(this->crc_8_autostar_lookup_table)] *
        internal_state;
  }

  return (static_cast<uint16_t>(internal_state) << 8) |
         (static_cast<uint16_t>(
             this->crc_8_autostar_lookup_table
                 [internal_state % sizeof(this->crc_8_autostar_lookup_table)]));
}
