#include "Tasks.hpp"
#include "config.hpp"
#include "Protocol.hpp"
#include "freertos/portmacro.h"

void discoveryTask(void *param){
  uint8_t* addr = (uint8_t*) param;
  while(true){
    SendDiscoveryPacket(addr);
    vTaskDelay(DIS_BROADCAST_SPEED / portTICK_PERIOD_MS);
  }
}

void dataTask(void *param){
  uint8_t* addr = (uint8_t*) param;
  while(true){
    SendDataPacket(addr);
    vTaskDelay(DIS_BROADCAST_SPEED / portTICK_PERIOD_MS);
  }
}
