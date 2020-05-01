#ifndef __NRF8001TEMPERATURESENSORSTATEMACHINE_H__
#define __NRF8001TEMPERATURESENSORSTATEMACHINE_H__

#include <Arduino.h>

#include "SensorStateMachine.h"

class nrf8001TemperatureSensorStateMachine
    : public SensorStateMachine<uint16_t> {
 public:
  using SensorStateMachine<uint16_t>::SensorStateMachine;

  void startSampling();
  void handleTemperatureEvent(uint16_t receivedValue);
};

#endif  //  __NRF8001TEMPERATURESENSORSTATEMACHINE_H__