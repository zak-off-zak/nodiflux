#include "Packet.hpp"
#include <Arduino.h>
#include <WiFi.h>
#include <cstdint>
#include <esp_now.h>


uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status){
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);

  if(esp_now_init() != ESP_OK){
    Serial.print("\r\nError initializing ESP-NOW\n");
    return; // TODO: Error handling or retry
  }

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
  DiscoveryPacket dsk_pkt{};
  WiFi.macAddress().toCharArray(dsk_pkt.src, sizeof(dsk_pkt.src));
  dsk_pkt.type = PacketType::DIS;
  dsk_pkt.pkt_id = random(1,20); 
  dsk_pkt.chs = checksum(dsk_pkt);

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &dsk_pkt, sizeof(dsk_pkt));

  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
  }
  delay(2000);
}
