#include "Packet.hpp"
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

  if(!pkt->deserializeFields(buffer, len)){
    delete pkt;
    return nullptr;
  }

  if (pkt->getChecksum() != pkt->checksum()){
    delete pkt;
    return nullptr;
  }

  return pkt;
}


// Little-Endian
size_t DataPacket::serialize(uint8_t* buffer, size_t buffer_size) const {
  // PacketType type;
  // uint8_t src[6];
  // uint16_t pkt_id;
  // uint8_t dest[6];
  // uint8_t ttl;
  // uint8_t msg[20];
  // uint8_t chs;
  
  size_t offset = 0;

  if(offset + 1 > buffer_size) return offset;
  buffer[offset++] = static_cast<uint8_t>(this->type);

  if(offset + 6 > buffer_size) return offset;
  memcpy(buffer + offset, this->src, 6);
  offset += 6;

  if(offset + 2 > buffer_size) return offset;
  writeLE16(buffer, offset, this->pkt_id);

  if(offset + 6 > buffer_size) return offset;
  memcpy(buffer + offset, this->dest, 6);
  offset += 6;

  if(offset + 1 > buffer_size) return offset;
  buffer[offset++] = this->ttl;

  if(offset + DATA_PACKET_SIZE > buffer_size) return offset;
  memcpy(buffer + offset, this->msg, DATA_PACKET_SIZE);
  offset += DATA_PACKET_SIZE;

  if(offset + 1 > buffer_size) return offset;
  buffer[offset++] = this->chs;

  return offset;
}

uint8_t DataPacket::checksum() const{
  // Could be done diretly on field to save memory
  // Size of the packet is 37
  uint8_t cs = 0;
  uint8_t bytes[37];
  size_t len = this->serialize(bytes, sizeof(bytes));
  for(size_t i = 0; i < len - 1; i++){
    cs ^= bytes[i];
  }
  return cs;
}

bool DataPacket::deserializeFields(const uint8_t* buffer, size_t len){
  if(len < 37) return false;

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

  if(offset + 6 > len) return false;
  memcpy(this->dest, buffer + offset, 6);
  offset += 6;

  if(offset + 1 > len) return false;
  this->ttl = buffer[offset++];

  if(offset + DATA_PACKET_SIZE > len) return false;
  memcpy(this->msg, buffer + offset, DATA_PACKET_SIZE);
  offset += DATA_PACKET_SIZE;

  if(offset + 1 > len) return false;
  this->chs = buffer[offset++];

  return true;
}

void DataPacket::handle() const{
  //TODO: hadling logic here
  // Serial.printf("DataPacket handling here\n");
}

DiscoveryPacket::DiscoveryPacket(){
  this->type = PacketType::DIS;
  uint8_t mac_bytes[6];
  macStringToBytes(WiFi.macAddress(), mac_bytes);
  memcpy(this->src, mac_bytes, 6);
  this->pkt_id = generatePktId(mac_bytes);
  this->chs = this->checksum();
}

// Little-Endian
size_t DiscoveryPacket::serialize(uint8_t* buffer, size_t buffer_size) const {
  // PacketType type;
  // uint8_t src[6];
  // uint16_t pkt_id;
  // uint8_t chs;
  
  size_t offset = 0;

  if(offset + 1 > buffer_size) return offset;
  buffer[offset++] = static_cast<uint8_t>(this->type);

  if(offset + 6 > buffer_size) return offset;
  memcpy(buffer + offset, this->src, 6);
  offset += 6;

  if(offset + 2 > buffer_size) return offset;
  writeLE16(buffer, offset, this->pkt_id);

  if(offset + 1 > buffer_size) return offset;
  buffer[offset++] = this->chs;

  return offset;
}

uint8_t DiscoveryPacket::checksum() const{
  // Could be done diretly on field to save memory
  // Size of the packet is 10
  uint8_t cs = 0;
  uint8_t bytes[10];
  size_t len = this->serialize(bytes, sizeof(bytes));
  for(size_t i = 0; i < len - 1; i++){
    cs ^= bytes[i];
  }
  return cs;
}

void DiscoveryPacket::handle() const{
  Serial.println("Handling DiscoveryPacket:");

  Serial.print("src: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", src[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();

  Serial.print("pkt_id: ");
  Serial.println(pkt_id);

  Serial.print("chs: ");
  Serial.println(chs);
}

bool DiscoveryPacket::deserializeFields(const uint8_t* buffer, size_t len){
  if(len < 10) return false;

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

  if(offset + 1 > len) return false;
  this->chs = buffer[offset++];

  return true;
}

// Little-Endian
size_t AcknowledgePacket::serialize(uint8_t* buffer, size_t buffer_size) const {
  // PacketType type;
  // uint8_t src[6];
  // uint16_t pkt_id;
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
  // Size of the packet is 17
  uint8_t cs = 0;
  uint8_t bytes[17];
  size_t len = this->serialize(bytes, sizeof(bytes));
  for(size_t i = 0; i < len - 1; i++){
    cs ^= bytes[i];
  }
  return cs;
}

bool AcknowledgePacket::deserializeFields(const uint8_t* buffer, size_t len){
  if(len < 37) return false;

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

  if(offset + 6 > len) return false;
  memcpy(this->dest, buffer + offset, 6);
  offset += 6;

  if(offset + 1 > len) return false;
  this->ttl = buffer[offset++];

  if(offset + 1 > len) return false;
  this->chs = buffer[offset++];

  return true;
}

void AcknowledgePacket::handle() const{
  //TODO: hadling logic here
  // Serial.printf("AcknowledgePacket handling here\n");
}
