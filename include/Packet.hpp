#ifndef PACKET
#define PACKET

#include <cstdint>

enum class PacketType : uint8_t {
  DIS = 0x01,
  ACK = 0x02,
  DAT = 0x03,
  KAL = 0x04,
};

struct Packet {
  uint16_t src;
  uint16_t dest;
  uint8_t hop_lmt;
  uint8_t hop_cnt;
  uint16_t msg_id;
  uint8_t msg[20];
  uint8_t chs;
};

#endif // !PACKET
