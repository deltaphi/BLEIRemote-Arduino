#ifndef __BATTERYSENSORSTATEMACHINE_H__
#define __BATTERYSENSORSTATEMACHINE_H__

#include <Arduino.h>

#include "SensorStateMachine.h"

class BatterySensorStateMachine : public SensorStateMachine<uint8_t> {
 public:
  using SensorStateMachine<uint8_t>::SensorStateMachine;

  void startSampling();
};

#endif  //  __BATTERYSENSORSTATEMACHINE_H__