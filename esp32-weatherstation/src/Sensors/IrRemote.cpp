#include "IrRemote.h"

namespace esp32weatherstation {
namespace Sensors {

// The remote service we wish to connect to.
BLEUUID IrRemote::batteryServiceUUID{"0000180f-0000-1000-8000-00805f9b34fb"};
BLEUUID IrRemote::environmentalSensingServiceUUID{
    "0000181a-0000-1000-8000-00805f9b34fb"};

// The characteristic of the remote service we are interested in.
BLEUUID IrRemote::batteryChargeCharUUID{"00002a19-0000-1000-8000-00805f9b34fb"};
BLEUUID IrRemote::temperatureCharUUID{"00002a1c-0000-1000-8000-00805f9b34fb"};
BLEUUID IrRemote::humidityCharUUID{"00002a6f-0000-1000-8000-00805f9b34fb"};
BLEUUID IrRemote::pressureCharUUID{"00002a6d-0000-1000-8000-00805f9b34fb"};

bool IrRemote::matchesDevice(BLEAdvertisedDevice& advertisedDevice) const {
  return advertisedDevice.haveServiceUUID() &&
         advertisedDevice.isAdvertisingService(environmentalSensingServiceUUID);
}

void IrRemote::setState(DeviceState state) {
  BLESensorDevice::setState(state);
  if (isScanNeeded()) {
    batteryChar.reset();
    tempChar.reset();
    humidChar.reset();
    pressChar.reset();

    batteryService.reset();
    environmentalSensingService.reset();
  }
}

void IrRemote::doConnecting() {
  BLESensorDevice::doConnecting();

  // Obtain a reference to the service we are after in the remote BLE server.
  environmentalSensingService =
      findServiceHelper(environmentalSensingServiceUUID);

  // Obtain a reference to the characteristic in the service of the remote BLE
  // server.
  tempChar = findCharacteristicHelper(temperatureCharUUID,
                                      environmentalSensingService);
  humidChar =
      findCharacteristicHelper(humidityCharUUID, environmentalSensingService);
  pressChar =
      findCharacteristicHelper(pressureCharUUID, environmentalSensingService);

  batteryService = findServiceHelper(batteryServiceUUID);
  batteryChar = findCharacteristicHelper(batteryChargeCharUUID, batteryService);

  // Obtain initial values
  doSensing();

  /*
      if(pRemoteCharacteristic->canNotify())
        pRemoteCharacteristic->registerForNotify(notifyCallback);
  */
  setState(kConnected);
}

void IrRemote::doSensing() {
  if (tempChar && tempChar->canRead()) {
    Serial.print("Temperature: ");
    Temp_t value = tempChar->readUInt16();
    // Value is in 100th degrees centigrade
    Serial.print(value / 100, DEC);
    Serial.print(".");
    Serial.print(value % 100, DEC);
    Serial.print("C ");
  }

  if (humidChar && humidChar->canRead()) {
    Serial.print("Humidity: ");
    Humidity_t value = humidChar->readUInt16();
    // Value is in ...?
    Serial.print(value, DEC);
    Serial.print("% ");
  }

  if (pressChar && pressChar->canRead()) {
    Serial.print("Pressure: ");
    Pressure_t value = pressChar->readUInt16();
    // Value is in ...?
    Serial.print(value, DEC);
    Serial.print("hPa ");
  }
  if (batteryChar && batteryChar->canRead()) {
    Serial.print("Battery: ");
    BatteryLevel_t value = batteryChar->readUInt8();
    // Value is in percent of charge.
    Serial.print(value, DEC);
    Serial.print("% ");
  }

  Serial.println();
}

void IrRemote::onDisconnect(BLEClient* pclient) {
  BLESensorDevice::onDisconnect(pclient);
  batteryChar.reset();
  tempChar.reset();
  humidChar.reset();
  pressChar.reset();
}

void IrRemote::loop() {
  switch (getState()) {
    case kNotFound:
    case kDisconnected:
    case kConnectionloss:
      return;
    case kConnecting:
      doConnecting();
      break;
    case kConnected:
      doSensing();
      break;
  }
}

}  // namespace Sensors
}  // namespace esp32weatherstation