#ifndef __BME280TEMPERATURESENSORSTATEMACHINE_H__
#define __BME280TEMPERATURESENSORSTATEMACHINE_H__

#include <Arduino.h>

#include "SensorStateMachine.h"
#include "bme280.h"

class BME280TemperatureSensorStateMachine : public SensorStateMachine<int16_t> {
 public:
  using SensorStateMachine<int16_t>::SensorStateMachine;

  void init();

  void startSampling();

 private:
  struct bme280_dev dev;
};

#endif  //  __BME280TEMPERATURESENSORSTATEMACHINE_H__