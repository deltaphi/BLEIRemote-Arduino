#ifndef __MYBLESENSORDEVICE_H__
#define __MYBLESENSORDEVICE_H__

#include <Arduino.h>

#include <memory>

#include "BLEDevice.h"

namespace esp32weatherstation {

class BLESensorDevice : public BLEClientCallbacks {
 public:
  using DevicePtr = std::unique_ptr<BLEAdvertisedDevice>;
  using ClientPtr = std::unique_ptr<BLEClient>;

  using ServicePtr = std::unique_ptr<BLERemoteService>;
  using CharacteristicPtr = std::unique_ptr<BLERemoteCharacteristic>;

  enum DeviceState {
    kNotFound = 0,
    kDisconnected,
    kConnecting,
    kConnected,
    kConnectionloss
  };

  DeviceState getState() const { return state; }
  bool isScanNeeded() const {
    return state == kNotFound || state == kDisconnected;
  }

  void connect(BLEAdvertisedDevice& advertisedDevice);

  void onConnect(BLEClient* pclient) override;
  void onDisconnect(BLEClient* pclient) override;

 protected:
  virtual void setState(DeviceState state);
  void doConnecting();
  BLEAdvertisedDevice& getDevice() { return *device; };
  const BLEAdvertisedDevice& getDevice() const { return *device; };
  BLEClient& getClient() { return *client; };
  const BLEClient& getClient() const { return *client; };

  ServicePtr findServiceHelper(BLEUUID&);
  static CharacteristicPtr findCharacteristicHelper(BLEUUID&, ServicePtr&);

 private:
  DeviceState state = kNotFound;
  DevicePtr device;
  ClientPtr client;
};

}  // namespace esp32weatherstation

#endif  // __MYBLESENSORDEVICE_H__
