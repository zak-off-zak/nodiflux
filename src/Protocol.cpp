#include "Protocol.hpp"
#include "HardwareSerial.h"
#include "NodeRegistry.hpp"
#include "Packet.hpp"
#include "config.hpp"
#include "utils.hpp"
#include <cstdint>
#include <esp_now.h>

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
  Serial.print("Sending DIS\t");
  sendPacket(broadcastAddress, dsk_pkt);
  delay(DIS_BROADCAST_SPEED);
}

void sendPacket(const uint8_t* mac_addr, const Packet& packet){
  uint8_t buffer[DATA_MESSAGE_SIZE + 17];
  size_t len = packet.serialize(buffer, sizeof(buffer));
  esp_err_t result = esp_now_send(mac_addr, buffer, len);

  if (result != ESP_OK){
    Serial.println("No direct connection possible, rerouting!");
    esp_err_t rerout_result = esp_now_send(NodeRegistry::instance().getMostRecentNode().data(), buffer, len);
    if(rerout_result != ESP_OK) Serial.println("Rerouting error");

  }
}

void SendDataPacket(const uint8_t* dest) {
    while (!Serial.available()) {
        delay(1);
    }

    String input = Serial.readStringUntil('\n');
    std::string message = input.c_str();

    DataPacket data_pkt(message, dest);

    Serial.print("Sending DATA\t");
    sendPacket(dest, data_pkt);
}

// TODO: Possible reafctor to bool
void establishPeer(const uint8_t* mac_addr, uint8_t channel, bool encrypt){
  if (esp_now_is_peer_exist(mac_addr)) {
    return;
  }
  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, mac_addr, 6);
  peer.channel = channel;
  peer.encrypt = encrypt;

  Serial.print("Peer Established:");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02x", mac_addr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();

  if (esp_now_add_peer(&peer) != ESP_OK) {
    Serial.println("Failed to add peer");
  }
}

