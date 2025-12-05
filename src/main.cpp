#include "Packet.hpp"
#include "Protocol.hpp"
#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

esp_now_peer_info_t peerInfo;


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);

  if(esp_now_init() != ESP_OK){
    Serial.print("\r\nError initializing ESP-NOW\n");
    return; // TODO: Error handling or retry
  }

  esp_now_register_recv_cb(OnDataReceive);
  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 1;
  peerInfo.encrypt = false;

  if(esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  SendDiscoveryPacket(broadcastAddress);
}
