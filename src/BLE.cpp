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

void initBLE(){
  NimBLEDevice::init("ESP-NODE");

  NimBLEServer* server = NimBLEDevice::createServer();
  NimBLEService* service = server->createService(SERVICE_UUID);

  NimBLECharacteristic* rxCharacteristic = service->createCharacteristic(RX_CHARACTERISTIC_UUID, NIMBLE_PROPERTY::WRITE);
  NimBLECharacteristic* txCharacteristic = service->createCharacteristic(TX_CHARACTERISTIC_UUID, NIMBLE_PROPERTY::NOTIFY);
  rxCharacteristic->setCallbacks(new RxCallbacks());

  service->start();

  NimBLEAdvertising* adv =  NimBLEDevice::getAdvertising();
  adv->setName("ESP-NODE");
  adv->addServiceUUID(SERVICE_UUID);
  adv->start();

  Serial.println("BLE STARTED");
}

