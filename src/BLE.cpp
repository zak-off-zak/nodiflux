#include "BLE.hpp"
#include "DataPacket.hpp"
#include "HardwareSerial.h"
#include "NimBLEAdvertising.h"
#include "NimBLECharacteristic.h"
#include "NimBLELocalValueAttribute.h"
#include "NimBLEServer.h"
#include "NimBLEService.h"
#include "Protocol.hpp"
#include "config.hpp"
#include <NimBLEDevice.h>
#include <cstdint>

void RxCallbacks::onWrite(NimBLECharacteristic* ch, NimBLEConnInfo& connInfo) {
  std::string value = ch->getValue();

  if (value.size() < 6) {
    Serial.println("Error: message too short to include a mac address");
    return;
  }

  uint8_t dest[6];
  std::memcpy(dest, value.data(), 6);

  size_t msg_len = value.size() - 6;
  uint8_t msg[DATA_MESSAGE_SIZE] = {0};
  std::memcpy(msg, value.data() + 6, msg_len);
  DataPacket data_pkt(std::string(reinterpret_cast<char*>(msg), msg_len), dest);
  sendPacket(dest, data_pkt);
}

void BLEController::init(){
  NimBLEDevice::init("ESP-NODE");

  NimBLEServer* server = NimBLEDevice::createServer();
  NimBLEService* service = server->createService(SERVICE_UUID);

  this->rx = service->createCharacteristic(RX_CHARACTERISTIC_UUID, NIMBLE_PROPERTY::WRITE);
  this->tx = service->createCharacteristic(TX_CHARACTERISTIC_UUID, NIMBLE_PROPERTY::NOTIFY);
  this->rx->setCallbacks(new RxCallbacks());

  service->start();

  NimBLEAdvertising* adv =  NimBLEDevice::getAdvertising();
  adv->setName("ESP-NODE");
  adv->addServiceUUID(SERVICE_UUID);
  adv->start();

  Serial.println("BLE STARTED");
}

bool BLEController::transmit(const std::string& msg){
  if (!this->tx) return false;
  this->tx->setValue(msg);
  this->tx->notify();
  return true;
}
