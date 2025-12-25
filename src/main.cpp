#include <Arduino.h>
#include <WiFi.h>
#include <cstdint>
#include <esp_now.h>
#include "Protocol.hpp"
#include "WiFiAPI.hpp"
#include "config.hpp"
#include "env.hpp"
#include "Tasks.hpp"
#include "BLE.hpp"

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8_t mac_addr[] = {0x20, 0xE7, 0xC8, 0x59, 0xE9, 0x18};


void setup() {
  Serial.begin(115200);

  // Setting up WiFi
  WiFi.mode(WIFI_AP_STA);
  bool ok = WiFi.softAP(WSS_SSID_AP, WSS_PASSWORD_AP, PEER_CHANNEL);
  Serial.printf("SoftAP start: %s\n", ok ? "OK" : "FAILED");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
  
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(WSS_SSID, WSS_PASSWORD);
  //
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("Connected! IP address: ");
  // Serial.println(WiFi.localIP());
  // Serial.print("Connected Wi-Fi channel: ");
  // Serial.println(WiFi.channel());

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

