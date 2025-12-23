#include "AcknowledgePacket.hpp"
#include "NodeRegistry.hpp"
#include "Protocol.hpp"
#include "RetryJournal.hpp"
#include "config.hpp"
#include "utils.hpp"
#include "RRTesting.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <WiFi.h>

// Little-Endian
size_t AcknowledgePacket::serialize(uint8_t* buffer, size_t buffer_size) const {
  // PacketType type;
  // uint8_t src[6];
  // uint16_t pkt_id;
  // uint16_t ack_pkt_id;
  // uint8_t dest[6];
  // uint8_t ttl;
  // uint8_t chs;
  
  size_t offset = 0;

  if(offset + 1 > buffer_size) return offset;
  buffer[offset++] = static_cast<uint8_t>(this->type);

  if(offset + 6 > buffer_size) return offset;
  memcpy(buffer + offset, this->src, 6);
  offset += 6;

  if(offset + 2 > buffer_size) return offset;
  writeLE16(buffer, offset, this->pkt_id);

  if(offset + 2 > buffer_size) return offset;
  writeLE16(buffer, offset, this->ack_pkt_id);

  if(offset + 6 > buffer_size) return offset;
  memcpy(buffer + offset, this->dest, 6);
  offset += 6;

  if(offset + 1 > buffer_size) return offset;
  buffer[offset++] = this->ttl;

  if(offset + 1 > buffer_size) return offset;
  buffer[offset++] = this->chs;

  return offset;
}

uint8_t AcknowledgePacket::checksum() const{
  // Could be done diretly on field to save memory
  // Size of the packet is 19
  uint8_t cs = 0;
  uint8_t bytes[19];
  size_t len = this->serialize(bytes, sizeof(bytes));
  for(size_t i = 0; i < len - 1; i++){
    cs ^= bytes[i];
  }
  return cs;
}

bool AcknowledgePacket::deserializeFields(const uint8_t* buffer, size_t len){
  if(len < 19) return false;

  size_t offset = 0;

  if(offset + 1 > len) return false;
  this->type = static_cast<PacketType>(buffer[offset]);
  offset++;

  if(offset + 6 > len) return false;
  memcpy(this->src, buffer + offset, 6);
  offset += 6;

  if(offset + 2 > len) return false;
  this->pkt_id = readLE16(buffer, offset);
  offset += 2;

  if(offset + 2 > len) return false;
  this->ack_pkt_id = readLE16(buffer, offset);
  offset += 2;

  if(offset + 6 > len) return false;
  memcpy(this->dest, buffer + offset, 6);
  offset += 6;

  if(offset + 1 > len) return false;
  this->ttl = buffer[offset++];

  if(offset + 1 > len) return false;
  this->chs = buffer[offset++];

  return true;
}

AcknowledgePacket::AcknowledgePacket(const uint8_t dest[6], const uint16_t packet_id){
    this->type = PacketType::ACK;

    uint8_t mac_bytes[6];
    macStringToBytes(WiFi.macAddress(), mac_bytes);
    memcpy(this->src, mac_bytes, 6);
    memcpy(this->dest, dest, 6);

    this->ttl = ACK_TTL;
    this->pkt_id = generatePktId(this->src);
    this->ack_pkt_id = packet_id;
    this->chs = this->checksum();
}

AcknowledgePacket::AcknowledgePacket(){}

void AcknowledgePacket::handle() {
  // Serial.printf("AcknowledgePacket handling here\n");
  // Serial.println(macBytesToString(this->src));
  this->ttl = this->ttl - 1;
  this->chs = this->checksum();
  if(this->ttl > 0){
    uint8_t mac_bytes[6];
    macStringToBytes(WiFi.macAddress(), mac_bytes);
    bool equal = true;
    for (size_t i = 0; i < 6; ++i) {
        if (this->dest[i] != mac_bytes[i]) {
            equal = false;
            break;
        }
    }
    if(equal){
      if(TESTING_ENABLED){
        onTestAckReceived(this);
      } else {
        Serial.print("ACK FROM: ");
        Serial.println(macBytesToString(this->src));
      }
      RetryJournal::instance().deleteEntry(this->ack_pkt_id);
    } else {
      if(NodeRegistry::instance().peerExists(this->dest)){
        sendPacket(this->dest, *this);
      }else{
        sendPacket(NodeRegistry::instance().getMostRecentNode().data(), *this);
      }
    }
  } else {
    // DEBUG ONLY
    Serial.println("TTL 0 -> Dropping the packet");
  }
}
