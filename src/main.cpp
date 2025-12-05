#include "Packet.hpp"
#include <Arduino.h>
#include <WiFi.h>
#include <cstdint>
#include <esp_now.h>


void OnDataReceive(const uint8_t* mac_addr, const uint8_t *incomingData, int len){
  DiscoveryPacket data;
  memcpy(&data, incomingData, sizeof(DiscoveryPacket));
  Serial.print("LEN: ");
  Serial.println(len);
  Serial.print("src: ");
  Serial.println(data.src);
  // Serial.print("type: ");
  // Serial.println(data.type);
  Serial.print("pkt_id: ");
  Serial.println(data.pkt_id);
  Serial.print("chs: ");
  Serial.println(data.chs);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);

  if(esp_now_init() != ESP_OK){
    Serial.print("\r\nError initializing ESP-NOW\n");
    return; // TODO: Error handling or retry
  }

  esp_now_register_recv_cb(OnDataReceive);
}

void loop() {
}
