#ifndef __BME280TEMPERATURESENSORSTATEMACHINE_H__
#define __BME280TEMPERATURESENSORSTATEMACHINE_H__

#include <Arduino.h>

#include "SensorStateMachine.h"
#include "bme280.h"

class BME280TemperatureSensorStateMachine
    : public SensorStateMachine<bme280_data> {
 public:
  using ParentType = SensorStateMachine<bme280_data>;

  BME280TemperatureSensorStateMachine(const uint8_t* const pipes)
      : ParentType(pipes[0]), pipes(pipes){};

  void enable();

  void init();

  void startSampling();
  void transmitSample();

 private:
  const uint8_t* const pipes;
  constexpr static const uint8_t numSensors = 3;
  uint8_t nextSensorForTX;
  struct bme280_dev dev;
  uint32_t measurementDelay;
};

#endif  //  __BME280TEMPERATURESENSORSTATEMACHINE_H__