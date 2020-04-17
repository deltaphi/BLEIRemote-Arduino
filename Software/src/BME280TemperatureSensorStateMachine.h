#ifndef __BME280TEMPERATURESENSORSTATEMACHINE_H__
#define __BME280TEMPERATURESENSORSTATEMACHINE_H__

#include <Arduino.h>
#include "SensorStateMachine.h"
#include "SparkFunBME280.h"

class BME280TemperatureSensorStateMachine
    : public SensorStateMachine<uint16_t> {
 public:
  using SensorStateMachine<uint16_t>::SensorStateMachine;

  void init();

  void startSampling();

 private:
  BME280 bme280;
};

#endif  //  __BME280TEMPERATURESENSORSTATEMACHINE_H__