#include <Arduino.h>
#include <WiFi.h>
#include <cstdint>
#include <esp_now.h>
#include "Protocol.hpp"
#include "config.hpp"
#include "freertos/portmacro.h"

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8_t mac_addr[] = {0x20, 0xE7, 0xC8, 0x59, 0xE9, 0x18};

void discoveryTask(void *param){
  uint8_t* addr = (uint8_t*) param;
  while(true){
    SendDiscoveryPacket(addr);
    vTaskDelay(DIS_BROADCAST_SPEED / portTICK_PERIOD_MS);
  }
}

void dataTask(void *param){
  uint8_t* addr = (uint8_t*) param;
  while(true){
    SendDataPacket(addr);
    vTaskDelay(DIS_BROADCAST_SPEED / portTICK_PERIOD_MS);
  }
}

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

  xTaskCreatePinnedToCore(discoveryTask, "discoveryTask", 4096, (void*)broadcastAddress, 1, NULL, 0);
  xTaskCreatePinnedToCore(dataTask, "dataTask", 4096, (void*)mac_addr, 1, NULL, 1);
}


void loop() {
  // SendDiscoveryPacket(broadcastAddress);
  // SendDataPacket(mac_addr);
  // NodeRegistry::instance().debug();
  // NodeRegistry::instance().debugMostRecentNode();
}

