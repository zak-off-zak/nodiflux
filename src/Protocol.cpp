#include "Protocol.hpp"

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
  if(checksum(data) == data.chs){
    Serial.print("ALL GOOD!");
  } else {
    Serial.print("ALL BAD!");
  }
  Serial.println();
}


void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status){
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void SendDiscoveryPacket(const uint8_t* broadcastAddress){
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

