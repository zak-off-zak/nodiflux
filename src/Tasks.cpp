#include "Tasks.hpp"
#include "RetryJournal.hpp"
#include "config.hpp"
#include "Protocol.hpp"
#include "RRTesting.hpp"
#include "HardwareSerial.h"
#include "freertos/portmacro.h"

void discoveryTask(void *param){
  uint8_t* addr = (uint8_t*) param;
  while(true){
    sendDiscoveryPacket(addr);
    vTaskDelay(DIS_BROADCAST_SPEED / portTICK_PERIOD_MS);
  }
}

void dataTask(void *param){
  uint8_t* addr = (uint8_t*) param;
  while(true){
    sendDataPacket(addr);
    vTaskDelay(DIS_BROADCAST_SPEED / portTICK_PERIOD_MS);
  }
}

void retryTask(void *param){
  while(true){
    RetryJournal::instance().executeRetries();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void rrTestTask(void *param) {
    Serial.println("[RR] RRTest task started");

    RRTest();

    Serial.println("[RR] RRTest task finished");
    vTaskDelete(NULL);
}

void serialCommandTask(void *param) {
  RRTestTrigger();
}

