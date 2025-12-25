#ifndef WIFI_API
#define WIFI_API

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <cstddef>
#include <cstdint>

extern AsyncWebServer server;
extern AsyncWebSocket ws;

void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type,  void *arg, uint8_t  *data, size_t len);
void initWS();

#endif // !WIFI_API
