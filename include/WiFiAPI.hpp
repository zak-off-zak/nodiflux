#ifndef WIFI_API
#define WIFI_API

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <cstddef>
#include <cstdint>

/**
 * @brief The asynchronous web server to host the WebSocket
 */
extern AsyncWebServer server;

/**
 * @brief The WebSocket
 */
extern AsyncWebSocket ws;

/**
 * @brief The handler for events happening on the WebSocket
 *
 * @param server The async web server
 * @param client The async WebSocket client
 * @param type The event type
 * @param arg Additional arguments
 * @param data The recieved data
 * @param len The length of the recieved data
 */
void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type,  void *arg, uint8_t  *data, size_t len);

/**
 * @brief Initializes the WebSocket
 */
void initWS();

#endif // !WIFI_API
