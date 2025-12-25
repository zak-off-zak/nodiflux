#include <Arduino.h>
#include <WiFi.h>
#include <cstdint>
#include <esp_now.h>
#include "Protocol.hpp"
#include "WiFiAPI.hpp"
#include "config.hpp"
#include "Tasks.hpp"
#include "BLE.hpp"

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8_t mac_addr[] = {0x20, 0xE7, 0xC8, 0x59, 0xE9, 0x18};


void setup() {
  Serial.begin(115200);

  // Setting up WiFi
  WiFi.mode(WIFI_AP_STA);
  bool ok = WiFi.softAP(WSS_SSID, WSS_PASSWORD, PEER_CHANNEL);
  Serial.printf("SoftAP start: %s\n", ok ? "OK" : "FAILED");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  // Setting up ESP-NOW
  if(esp_now_init() != ESP_OK){
    Serial.print("\r\nError initializing ESP-NOW\n");
    return; // TODO: Error handling or retry
  }

  esp_now_register_recv_cb(onDataReceive);
  // esp_now_register_send_cb(onDataSent);

  establishPeer(broadcastAddress, PEER_CHANNEL, PEER_ENCRYPT);

  xTaskCreatePinnedToCore(discoveryTask, "discoveryTask", 4096, (void*)broadcastAddress, 1, NULL, 1);
  xTaskCreatePinnedToCore(retryTask, "retryTask", 4096, NULL, 2, NULL, 1);
  if(TESTING_ENABLED){
    xTaskCreatePinnedToCore(serialCommandTask, "serialCommandTask", 4096, NULL, 2, NULL, 1);
  }
  // xTaskCreatePinnedToCore(dataTask, "dataTask", 4096, (void*)mac_addr, 1, NULL, 1);

  BLEController::instance().init();
  initWS();
}


void loop() {
}

