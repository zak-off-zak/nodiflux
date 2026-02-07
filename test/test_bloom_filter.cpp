#include "../src/BloomFilter.cpp"
#include "gtest/gtest.h"
#include <cstdint>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

TEST(TestBloomFilter, IsSeen) {
  BloomFilter &bf = BloomFilter::instance();

  uint16_t pkt_id = 0xAAAA;

  EXPECT_FALSE(bf.isSeen(pkt_id));

  bf.addEntry(pkt_id);

  EXPECT_TRUE(bf.isSeen(pkt_id));
}

TEST(TestBloomFilter, MultipleEntries) {
  BloomFilter &bf = BloomFilter::instance();

  uint16_t pkt_id1 = 0x1235;
  uint16_t pkt_id2 = 0xABCD;
  uint16_t pkt_id3 = 0xA5A5;

  EXPECT_FALSE(bf.isSeen(pkt_id1));
  EXPECT_FALSE(bf.isSeen(pkt_id2));
  EXPECT_FALSE(bf.isSeen(pkt_id3));

  bf.addEntry(pkt_id1);
  bf.addEntry(pkt_id2);
  bf.addEntry(pkt_id3);

  EXPECT_TRUE(bf.isSeen(pkt_id1));
  EXPECT_TRUE(bf.isSeen(pkt_id2));
  EXPECT_TRUE(bf.isSeen(pkt_id3));
}

TEST(TestBloomFilter, RepeatedEntry) {
  BloomFilter &bf = BloomFilter::instance();

  uint16_t pkt_id = 0x123A;

  EXPECT_FALSE(bf.isSeen(pkt_id));

  bf.addEntry(pkt_id);
  bf.addEntry(pkt_id);

  EXPECT_TRUE(bf.isSeen(pkt_id));
}

TEST(TestBloomFilter, HashCollisions) {
  BloomFilter &bf = BloomFilter::instance();

  for (uint16_t i = 0; i < 50; i++) {
    bf.addEntry(i);
  }

  for (uint16_t i = 0; i < 50; i++) {
    EXPECT_TRUE(bf.isSeen(i));
  }
}

TEST(TestBloomFilter, ThreadSafety) {
  BloomFilter &bf = BloomFilter::instance();

  std::function<void(uint16_t)> addEntry_fn = [&bf](uint16_t id) {
    bf.addEntry(id);
  };

  std::vector<std::thread> threads;
  for (uint16_t i = 200; i < 250; i++) {
    threads.emplace_back(addEntry_fn, i);
  }

  for (std::thread &t : threads) {
    t.join();
  }

  for (uint16_t i = 200; i < 250; i++) {
    EXPECT_TRUE(bf.isSeen(i));
  }
}
