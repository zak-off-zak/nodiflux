#ifndef PROTOCOL
#define PROTOCOL

#include <cstdint>
#include <esp_now.h>
#include "Packet.hpp"
#include "NodeRegistry.hpp"
#include "utils.hpp"
#include <Arduino.h>
#include <WiFi.h>

void onDataReceive(const uint8_t* mac_addr, const uint8_t *incomingData, int len);
void onDataSent(const uint8_t* mac_addr, esp_now_send_status_t status);

void sendPacket(const uint8_t* mac_addr, const Packet& packet);

void sendDiscoveryPacket(const uint8_t* broadcastAddress);
void sendDataPacket(const uint8_t* dest);
void establishPeer(const uint8_t* mac_addr, uint8_t channel, bool encrypt);

template<typename PktT, typename LambdaCallback>
void commonRouting(PktT& pkt, LambdaCallback lambdaCallback){
  pkt.ttl--;
  pkt.chs = pkt.checksum();

  if(pkt.ttl <= 0){
    Serial.println("TTL <= 0 -> Dropping the packet");
    return;
  }

  uint8_t mac_bytes[6];
  macStringToBytes(WiFi.macAddress(), mac_bytes);
  if(isMACEqual(pkt.dest, mac_bytes)){
    lambdaCallback();
  } else {
    if(NodeRegistry::instance().peerExists(pkt.dest)){
      sendPacket(pkt.dest, pkt);
    }else{
      sendPacket(NodeRegistry::instance().getMostRecentNode().data(), pkt);
    }
  }

}

#endif // !PROTOCOL
