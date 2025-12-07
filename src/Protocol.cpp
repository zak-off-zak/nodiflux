#include "Protocol.hpp"
#include "HardwareSerial.h"
#include "Packet.hpp"
#include "utils.hpp"

void OnDataReceive(const uint8_t* mac_addr, const uint8_t *incomingData, int len){
  if(len < 10){
    Serial.print("TOO LITTLE DATA RECIVED!\n");
    return;
  }

  Packet* pkt = Packet::deserializeFactory(incomingData, len);

  if(pkt == nullptr){
    Serial.println("Unknown packet type!\n");
    return;
  }else {
    if(pkt->checksum() == pkt->getChecksum()){
      pkt->handle();
    } else {
      Serial.print("Corrupted Packet: Checksums do not match\n");
    }
  }

  delete pkt;
}

void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status){
  Serial.print("\r\nLast Packet Send Status:");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}


void SendDiscoveryPacket(const uint8_t* broadcastAddress){
  DiscoveryPacket dsk_pkt;
  uint8_t buffer[32];
  size_t len = dsk_pkt.serialize(buffer, sizeof(buffer));
  esp_err_t result = esp_now_send(broadcastAddress, buffer, len);

  if (result != ESP_OK) Serial.println("Sending error");
  delay(2000);
}

