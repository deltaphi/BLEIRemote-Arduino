#ifndef __SENSORSTATEMACHINE_H__
#define __SENSORSTATEMACHINE_H__

#include <Arduino.h>

#include "bleadapter.h"

bool do_transmit_single(uint8_t pipe, uint8_t* value, uint8_t valueLen);

enum class SensorValueStatus {
  kOff = 0,         ///< Sensor is not being read.
  kIdle,            ///< Sensor is being read but no reading is in progress.
  kSampling,        ///< Sensor is making a reading, value not yet available.
  kSampleAvailable  ///< Sensor is not making a reading, value is available for
                    ///< transmission.
};

template <typename SensorValueType>
class SensorStateMachine {
 public:
  SensorStateMachine(uint8_t pipe) : pipe(pipe) {}

  SensorValueType getValue() { return value; }
  SensorValueType getState() { return state; }

  void enable() {
    if (!isEnabled()) {
      state = SensorValueStatus::kIdle;
    }
  }
  void disable() { state = SensorValueStatus::kOff; }
  bool isEnabled() const { return state != SensorValueStatus::kOff; }
  bool isDataPending() const {
    return state == SensorValueStatus::kSampleAvailable;
  }

  void startSampling() { state = SensorValueStatus::kSampling; }

  void transmitSample() {
    if (state == SensorValueStatus::kSampleAvailable) {
      bool sendSucces =
          do_transmit_single(pipe, (uint8_t*)&value, sizeof(value));
      if (sendSucces) {
        state = SensorValueStatus::kIdle;
      }
    }
  }

 protected:
  uint8_t pipe;
  SensorValueStatus state = SensorValueStatus::kOff;
  SensorValueType value;
};

#endif  //  __SENSORSTATEMACHINE_H__