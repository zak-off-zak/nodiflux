#ifndef BLE
#define BLE

#include "NimBLECharacteristic.h"

#define SERVICE_UUID "40c8ef9d-8aad-401e-b951-9005e8768dcb"
#define RX_CHARACTERISTIC_UUID "aaacb24b-f61d-4e31-9c25-e2aeb24055bc"
#define TX_CHARACTERISTIC_UUID "bbb6580a-0a39-4adf-a62f-f9dede706480"

void initBLE();
class RxCallbacks : public NimBLECharacteristicCallbacks{
  void onWrite(NimBLECharacteristic* ch, NimBLEConnInfo& connInfo) override;
};



#endif // !BLE
