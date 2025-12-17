#include "Packet.hpp"
#include "DataPacket.hpp"
#include "DiscoveryPacket.hpp"
#include "AcknowledgePacket.hpp"
#include "HardwareSerial.h"
#include "NodeRegistry.hpp"
#include "Protocol.hpp"
#include "config.hpp"
#include "utils.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <WiFi.h>

Packet* Packet::deserializeFactory(const uint8_t* buffer, size_t len){
  if(len < 1) return nullptr;
  PacketType type = static_cast<PacketType>(buffer[0]);
  Packet* pkt = nullptr;
  switch (type) {
    case PacketType::DIS: pkt = new DiscoveryPacket(); break;
    case PacketType::ACK: pkt = new AcknowledgePacket(); break;
    case PacketType::DAT: pkt = new DataPacket(); break;
  }

  // TODO: Avoid double checking
  if(!pkt->deserializeFields(buffer, len)){
    delete pkt;
    Serial.println("Unknown packet type: deserializeFactory failed\n");
    return nullptr;
  }

  if (pkt->getChecksum() != pkt->checksum()){
    delete pkt;
    Serial.println("Unknown packet type: checksum mismatch\n");
    return nullptr;
  }

  return pkt;
}

