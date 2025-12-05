#include "Packet.hpp"

uint8_t checksum(const DataPacket& pkt){
  uint8_t cs = 0;
  const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&pkt);
  for(size_t i = 0; i < sizeof(pkt) - 1; i++){
    cs ^= bytes[i];
  }
  return cs;
}

uint8_t checksum(const DiscoveryPacket& pkt){
  uint8_t cs = 0;
  const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&pkt);
  for(size_t i = 0; i < sizeof(pkt) - 1; i++){
    cs ^= bytes[i];
  }
  return cs;
}


size_t serialize(const DataPacket& pkt, uint8_t* buffer){
  std::memcpy(buffer, &pkt, sizeof(pkt));
  return sizeof(pkt);
}

size_t deserialize(const uint8_t* buffer, DataPacket& pkt){
  std::memcpy(&pkt, buffer, sizeof(pkt));
  return checksum(pkt) == pkt.chs;
}

