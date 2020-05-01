#ifndef __SENSORS__IRREMOTE_H__
#define __SENSORS__IRREMOTE_H__

#include <Arduino.h>

#include <memory>

#include "BLESensorDevice.h"

namespace esp32weatherstation {
namespace Sensors {

class IrRemote : public BLESensorDevice {
 public:
  using Temp_t = uint16_t;  // Temperature in 100th degrees centigrade.
  using Pressure_t = uint16_t;
  using Humidity_t = uint16_t;
  using BatteryLevel_t = uint8_t;

  // The remote service we wish to connect to.
  static BLEUUID batteryServiceUUID;
  static BLEUUID environmentalSensingServiceUUID;

  // The characteristic of the remote service we are interested in.
  static BLEUUID batteryChargeCharUUID;
  static BLEUUID temperatureCharUUID;
  static BLEUUID humidityCharUUID;
  static BLEUUID pressureCharUUID;

  void loop();

  void onDisconnect(BLEClient* pclient) override;

  bool matchesDevice(BLEAdvertisedDevice& advertisedDevice) const;

 private:
  void setState(DeviceState state) override;

  void doSensing();
  void doConnecting();

  /* Measurement values */
  BatteryLevel_t batteryLevel;
  Temp_t temperature;
  Humidity_t humidity;
  Pressure_t pressure;

  ServicePtr batteryService;
  ServicePtr environmentalSensingService;

  CharacteristicPtr batteryChar;
  CharacteristicPtr tempChar;
  CharacteristicPtr humidChar;
  CharacteristicPtr pressChar;
};

}  // namespace Sensors
}  // namespace esp32weatherstation

#endif  // __SENSORS__IRREMOTE_H__
