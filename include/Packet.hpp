#ifndef PACKET
#define PACKET

#include <cstdint>
#include <cstring>

enum class PacketType : uint8_t {
  DIS = 0x01,
  ACK = 0x02,
  DAT = 0x03,
  KAL = 0x04,
};

#pragma pack(push, 1)
struct Packet {
  uint16_t src;
  uint16_t dest;
  PacketType type;
  uint8_t ttl;
  uint16_t pkt_id;
  uint8_t msg[20];
  uint8_t chs;
};
#pragma pack(pop)

uint8_t checksum(const Packet& pkt);
size_t serialize(const Packet& pkt, uint8_t* buffer);
size_t deserialize(const uint8_t* buffer, Packet& pkt);

#endif // !PACKET
