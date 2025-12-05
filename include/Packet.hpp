#ifndef PACKET
#define PACKET

#include <cstdint>
#include <cstring>
#include "config.hpp"

enum class PacketType : uint8_t {
  DIS = 0x01,
  ACK = 0x02,
  DAT = 0x03,
};

#pragma pack(push, 1)
struct DataPacket {
  uint16_t src;
  uint16_t dest;
  PacketType type;
  uint8_t ttl;
  uint16_t pkt_id;
  uint8_t msg[DATA_PACKET_SIZE];
  uint8_t chs;
};
#pragma pack(pop)

struct DiscoveryPacket {
  uint16_t src;
  PacketType type;
  uint16_t pkt_id;
  uint8_t chs;
};

struct AcknowledgePacket {
  uint16_t src;
  uint16_t dest;
  PacketType type;
  uint16_t pkt_id;
  uint8_t chs;
};

uint8_t checksum(const DataPacket& pkt);
size_t serialize(const DataPacket& pkt, uint8_t* buffer);
size_t deserialize(const uint8_t* buffer, DataPacket& pkt);

#endif // !PACKET
