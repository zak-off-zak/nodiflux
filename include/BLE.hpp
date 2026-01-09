#ifndef BLE
#define BLE

#include "NimBLECharacteristic.h"
#include "config.hpp"


void initBLE();

/**
 * @class BLEController
 * @brief The class used to createa singelton for BLE communication
 *
 */
class BLEController {
  private:
    NimBLECharacteristic* tx = nullptr;
    NimBLECharacteristic* rx = nullptr;
  public:

    /**
     * @brief Default constructor
     */
    BLEController () = default;

    /**
     * @brief Delete the copy constructor
     */
    BLEController(const BLEController&) = delete;

    /**
     * @brief Detele copy assignment operator
     *
     * @return BLEController
     */
    BLEController& operator=(const BLEController&) = delete;

    /**
     * @brief Method to get the singelton instance
     *
     * @return Intance of the BLE controller
     */
    static BLEController& instance() {
      static BLEController instance;
      return instance;
    }

    /**
     * @brief Initilialize the BLE
     */
    void init();

    /**
     * @brief Trasmit a message using BLE
     *
     * @param msg Message to tarnsmit
     * @return True, if trasmission is successful
     */
    bool transmit(const std::string& msg);
};

/**
 * @class RxCallbacks
 * @brief A class to define characteristics call backs
 *
 */
class RxCallbacks : public NimBLECharacteristicCallbacks{
  /**
   * @brief A callback to be triggered once a message is recieved
   *
   * @param ch characteristic to be monitored
   * @param connInfo Connection Information
   */
  void onWrite(NimBLECharacteristic* ch, NimBLEConnInfo& connInfo) override;
};



#endif // !BLE
