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

// Used for debugging, when TESTING_ENABLED is true
const uint8_t mac_addr[] = {0x20, 0xE7, 0xC8, 0x59, 0xE9, 0x18};


void setup() {
  Serial.begin(115200);

  // Setting up WiFi
  WiFi.mode(WIFI_AP_STA);
  bool ok = WiFi.softAP(WSS_SSID_AP, WSS_PASSWORD_AP, PEER_CHANNEL);
  Serial.printf("SoftAP start: %s\n", ok ? "OK" : "FAILED");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
  

  // Setting up ESP-NOW
  if(esp_now_init() != ESP_OK){
    Serial.print("\r\nError initializing ESP-NOW\n");
    return;
  }

  // Regetring the receive callback
  esp_now_register_recv_cb(onDataReceive);

  // Establishing the peer connection for braodcasting
  establishPeer(broadcastAddress, PEER_CHANNEL, PEER_ENCRYPT);

  // Assigning tasks to cores
  xTaskCreatePinnedToCore(discoveryTask, "discoveryTask", 4096, (void*)broadcastAddress, 1, NULL, 1);
  xTaskCreatePinnedToCore(retryTask, "retryTask", 4096, NULL, 2, NULL, 1);
  if(TESTING_ENABLED){
    xTaskCreatePinnedToCore(serialCommandTask, "serialCommandTask", 4096, NULL, 2, NULL, 1);
  }
  if(SERIAL_MESSAGE_SENDING_ENABLED){
    xTaskCreatePinnedToCore(dataTask, "dataTask", 4096, (void*)mac_addr, 1, NULL, 1);
  }

  // initializing BLE, if BLE_ENABLED is true
  if(BLE_ENABLED){
    BLEController::instance().init();
  }

  // initializing the WebSocket
  initWS();
}

void loop() {
}

