#include "gtest/gtest.h"
#include <algorithm>
#include "Packet.hpp"

TEST(PacketTest, Checksum){
  // Creating a packet
  Packet pkt{};
  pkt.src = 1;
  pkt.dest = 2;
  pkt.type = PacketType::DAT;
  pkt.ttl = 11;
  pkt.pkt_id = 1;
  for(int i = 0; i < 20; i++){
    pkt.msg[i] = i;
  }
  pkt.chs =   checksum(pkt);

  EXPECT_EQ(checksum(pkt), 10);
}

TEST(PacketTest, SerializeDeserialize){
  // Creating a packet
  Packet pkt{};
  pkt.src = 1;
  pkt.dest = 2;
  pkt.type = PacketType::DAT;
  pkt.ttl = 11;
  pkt.pkt_id = 1;
  for(int i = 0; i < 20; i++){
    pkt.msg[i] = i;
  }
  pkt.chs =   checksum(pkt);

  uint8_t buffer[sizeof(Packet)] = {0};
  serialize(pkt, buffer);

  Packet recieved{};
  bool valid = deserialize(buffer, recieved);

  EXPECT_TRUE(valid);
  EXPECT_EQ(recieved.src, pkt.src);
  EXPECT_EQ(recieved.dest, pkt.dest);
  EXPECT_EQ(recieved.type, pkt.type);
  EXPECT_EQ(recieved.ttl, pkt.ttl);
  EXPECT_EQ(recieved.pkt_id, pkt.pkt_id);
  EXPECT_TRUE(std::equal(std::begin(recieved.msg), std::end(recieved.msg), std::begin(pkt.msg)));
  EXPECT_EQ(recieved.chs, pkt.chs);
}
