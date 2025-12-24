#include "DataPacket.hpp"
#include "HardwareSerial.h"
#include "NodeRegistry.hpp"
#include "AcknowledgePacket.hpp"
#include "Protocol.hpp"
#include "config.hpp"
#include "utils.hpp"
#include "BLE.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <WiFi.h>

DataPacket::DataPacket(){
}


DataPacket::DataPacket(const std::string& message, const uint8_t dest[6]){
    this->type = PacketType::DAT;

    uint8_t mac_bytes[6];
    macStringToBytes(WiFi.macAddress(), mac_bytes);
    memcpy(this->src, mac_bytes, 6);
    memcpy(this->dest, dest, 6);

    size_t len = std::min(message.size(), (size_t)DATA_MESSAGE_SIZE);
    memcpy(this->msg, message.data(), len);
    if (len < DATA_MESSAGE_SIZE) {
        memset(this->msg + len, 0, DATA_MESSAGE_SIZE - len);
    }

    this->ttl = DATA_TTL;
    this->pkt_id = generatePktId(mac_bytes);
    this->chs = this->checksum();
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

  if(offset + DATA_MESSAGE_SIZE > buffer_size) return offset;
  memcpy(buffer + offset, this->msg, DATA_MESSAGE_SIZE);
  offset += DATA_MESSAGE_SIZE;

  if(offset + 1 > buffer_size) return offset;
  buffer[offset++] = this->chs;

  return offset;
}

uint8_t DataPacket::checksum() const{
  // Could be done diretly on field to save memory
  // Size of the packet is 37
  uint8_t cs = 0;
  uint8_t bytes[DATA_MESSAGE_SIZE + 17];
  size_t len = this->serialize(bytes, sizeof(bytes));
  for(size_t i = 0; i < len - 1; i++){
    cs ^= bytes[i];
  }
  return cs;
}

bool DataPacket::deserializeFields(const uint8_t* buffer, size_t len){
  if(len < DATA_MESSAGE_SIZE + 17) return false;

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

  if(offset + DATA_MESSAGE_SIZE > len) return false;
  memcpy(this->msg, buffer + offset, DATA_MESSAGE_SIZE);
  offset += DATA_MESSAGE_SIZE;

  if(offset + 1 > len) return false;
  this->chs = buffer[offset++];

  return true;
}

void DataPacket::handle() {
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
      Serial.print("Message: ");
      Serial.println((char*)this->msg);
      // TODO: Double check the correctness of reinterpret_cast
      BLEController::instance().transmit(std::string(reinterpret_cast<char*>(this->msg), DATA_MESSAGE_SIZE));
      if(TESTING_ENABLED){
        uint16_t seq = static_cast<uint16_t>(atoi(reinterpret_cast<char*>(this->msg)));
        AcknowledgePacket ack_pkt(this->src, seq);
        sendPacket(this->src, ack_pkt);
      } else {
        AcknowledgePacket ack_pkt(this->src, this->pkt_id);
        sendPacket(this->src, ack_pkt);
      }
    } else {
      if(NodeRegistry::instance().peerExists(this->dest)){
        Serial.println("Sending to dest!");
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
