#ifndef PROTOCOL
#define PROTOCOL

#include <cstdint>
#include <esp_now.h>
#include "Packet.hpp"
#include <Arduino.h>
#include <WiFi.h>

void OnDataReceive(const uint8_t* mac_addr, const uint8_t *incomingData, int len);
void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status);
void SendDiscoveryPacket(const uint8_t* broadcastAddress);

#endif // !PROTOCOL
