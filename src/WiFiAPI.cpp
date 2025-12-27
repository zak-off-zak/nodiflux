#include "WiFiAPI.hpp"
#include <ESPAsyncWebServer.h>
#include "AsyncWebSocket.h"
#include "DataPacket.hpp"
#include "HardwareSerial.h"
#include "Protocol.hpp"
#include "config.hpp"
#include "utils.hpp"
#include <cstddef>
#include <string>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type,  void *arg, uint8_t  *data, size_t len){
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client %u connected\n", client->id());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client %u disconnected\n", client->id());
      break;
    case WS_EVT_DATA: {
      AwsFrameInfo* info = (AwsFrameInfo*) arg;

      if(info->final && info->opcode == WS_BINARY){
        uint8_t dest[6];
        std::memcpy(dest, data, 6);

        size_t msg_len = len - 6;
        uint8_t msg[DATA_MESSAGE_SIZE] = {0};
        std::memcpy(msg, data + 6, msg_len);
        DataPacket data_pkt(std::string(reinterpret_cast<char*>(msg), msg_len), dest);
        Serial.println(macBytesToString(data_pkt.getDest()));
        sendPacket(dest, data_pkt);
      }
      break;
    }
    default:
      break;
  }
}

void initWS(){
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.begin();
}

