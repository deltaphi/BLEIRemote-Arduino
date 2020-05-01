/**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 */

#include <Arduino.h>

#include "BLEDevice.h"
//#include "BLEScan.h"

#include "Sensors/IrRemote.h"

int scanTime = 5; //In seconds

static esp32weatherstation::Sensors::IrRemote irRemote;

bool scanNeeded() {
  return irRemote.isScanNeeded(); 
}

/*
static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    Serial.println((char*)pData);
}
*/

/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    if (irRemote.matchesDevice(advertisedDevice)) {
      irRemote.connect(advertisedDevice);
    }

  } // onResult
}; // MyAdvertisedDeviceCallbacks


void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
  pBLEScan->setActiveScan(true);
  //pBLEScan->start(scanTime, false);
} // End of setup.


// This is the Arduino main loop function.
void loop() {

  irRemote.loop();

  if (scanNeeded()) {
    BLEDevice::getScan()->start(scanTime, false);  // this is just eample to start scan after disconnect, most likely there is better way to do it in arduino
  } else {
    BLEDevice::getScan()->stop();
  }
  
  delay(1000); // Delay a second between loops.
} // End of loop
