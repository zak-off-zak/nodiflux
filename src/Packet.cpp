#include "Packet.hpp"
#include "AcknowledgePacket.hpp"
#include "BloomFilter.hpp"
#include "DataPacket.hpp"
#include "DiscoveryPacket.hpp"
#include "HardwareSerial.h"

#include <WiFi.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

Packet *Packet::deserializeFactory(const uint8_t *buffer, size_t len) {
  if (len < 1)
    return nullptr;
  PacketType type = static_cast<PacketType>(buffer[0]);
  Packet *pkt = nullptr;
  switch (type) {
  case PacketType::DIS:
    pkt = new DiscoveryPacket();
    break;
  case PacketType::ACK:
    pkt = new AcknowledgePacket();
    break;
  case PacketType::DAT:
    pkt = new DataPacket();
    break;
  }

  if (!pkt->deserializeFields(buffer, len)) {
    delete pkt;
    Serial.println("Unknown packet type: deserializeFactory failed\n");
    return nullptr;
  }

  if (pkt->getChecksum() != pkt->checksum()) {
    delete pkt;
    Serial.println("Unknown packet type: checksum mismatch\n");
    return nullptr;
  }

  BloomFilter &bf = BloomFilter::instance();
  if (bf.isSeen(pkt->getPacketId())) {
    delete pkt;
    Serial.println("Packet with this packet id has already been seen: dropping "
                   "the packet\n");
    return nullptr;
  }

  bf.addEntry(pkt->getPacketId());
  return pkt;
}
