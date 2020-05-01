#include "BLESensorDevice.h"

namespace esp32weatherstation {

void BLESensorDevice::setState(DeviceState newState) {
  state = newState;
  if (isScanNeeded()) {
    device.reset();
    client.reset();
  }
}

void BLESensorDevice::connect(BLEAdvertisedDevice& advertisedDevice) {
  device = std::unique_ptr<BLEAdvertisedDevice>(
      new BLEAdvertisedDevice(advertisedDevice));
  setState(kConnecting);
}

void BLESensorDevice::doConnecting() {
  if (!device) {
    Serial.println("Error while connecting BLESensorDevice: No Device set.");
    return;
  }
  Serial.print("Forming a connection to ");
  Serial.println(device->getAddress().toString().c_str());

  // Connect to the remove BLE Server.
  client = std::unique_ptr<BLEClient>(BLEDevice::createClient());
  Serial.println(" - Created client");

  client->setClientCallbacks(this);

  // Connect to the remove BLE Server.
  client->connect(device.get());  // if you pass BLEAdvertisedDevice instead of
                                  // address, it will be recognized type of peer
                                  // device address (public or private)
  Serial.println(" - Connected to server");
}

void BLESensorDevice::onConnect(BLEClient* pclient) { setState(kConnected); }

void BLESensorDevice::onDisconnect(BLEClient* pclient) { setState(kNotFound); }

BLESensorDevice::ServicePtr BLESensorDevice::findServiceHelper(BLEUUID& uuid) {
  ServicePtr service = ServicePtr(getClient().getService(uuid));
  if (!service) {
    Serial.print("Failed to find service UUID: ");
    Serial.println(uuid.toString().c_str());
    // getClient()->disconnect();
    // setState(kDisconnected);
    // return;
  }
  Serial.println(" - Found service.");
  return service;
}

BLESensorDevice::CharacteristicPtr BLESensorDevice::findCharacteristicHelper(
    BLEUUID& uuid, ServicePtr& service) {
  CharacteristicPtr characteristic;

  if (service) {
    CharacteristicPtr characteristic =
        CharacteristicPtr(service->getCharacteristic(uuid));
    if (characteristic) {
      Serial.print("Failed to find characteristic UUID: ");
      Serial.println(uuid.toString().c_str());
      // getClient()->disconnect();
      // setState(kDisconnected);
      // return false;
    }
    Serial.println(" - Found characteristic.");
  }

  return characteristic;
}

}  // namespace esp32weatherstation
