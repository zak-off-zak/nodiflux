#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "Protocol.hpp"
#include "config.hpp"

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8_t mac_addr[] = {0x20, 0xE7, 0xC8, 0x59, 0x57, 0x64};

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);

  if(esp_now_init() != ESP_OK){
    Serial.print("\r\nError initializing ESP-NOW\n");
    return; // TODO: Error handling or retry
  }

  esp_now_register_recv_cb(OnDataReceive);
  // esp_now_register_send_cb(OnDataSent);

  establishPeer(broadcastAddress, PEER_CHANNEL, PEER_ENCRYPT);
}

void loop() {
  SendDiscoveryPacket(broadcastAddress);
  // SendDataPacket(mac_addr);
  // NodeRegistry::instance().debug();
  // NodeRegistry::instance().debugMostRecentNode();
}

