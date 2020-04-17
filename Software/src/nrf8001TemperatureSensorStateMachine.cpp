#include "nrf8001TemperatureSensorStateMachine.h"

#include "lib_aci.h"

void nrf8001TemperatureSensorStateMachine::startSampling() {
  Serial.println(F("Requesting Temperature from nRF8001."));
  if (lib_aci_get_temperature()) {
    this->SensorStateMachine<uint16_t>::startSampling();
  } else {
    state = SensorValueStatus::kIdle;
  }
}

void nrf8001TemperatureSensorStateMachine::handleTemperatureEvent(
    uint16_t receivedValue) {
  Serial.print(F("nRF8001 Temperature: "));
  Serial.println(receivedValue / 4, DEC);

  value = receivedValue;
  state = SensorValueStatus::kSampleAvailable;
}