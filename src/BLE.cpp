#include "BLE.hpp"
#include "HardwareSerial.h"
#include "NimBLEAdvertising.h"
#include "NimBLECharacteristic.h"
#include "NimBLELocalValueAttribute.h"
#include "NimBLEServer.h"
#include "NimBLEService.h"
#include <NimBLEDevice.h>
#include <string>

void RxCallbacks::onWrite(NimBLECharacteristic* ch, NimBLEConnInfo& connInfo) {
  std::string value = ch->getValue();
  Serial.print("Value Recieved: ");
  Serial.println(value.c_str());
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
