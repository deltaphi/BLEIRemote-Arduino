#ifndef __SENSORSTATEMACHINE_H__
#define __SENSORSTATEMACHINE_H__

#include <Arduino.h>

#include "bleadapter.h"

enum class SensorValueStatus {
    kOff = 0, ///< Sensor is not being read.
    kIdle,  ///< Sensor is being read but no reading is in progress.
    kSampling, ///< Sensor is making a reading, value not yet available.
    kSampleAvailable ///< Sensor is not making a reading, value is available for transmission.
};

template<typename SensorValueType>
class SensorStateMachine {
 public:
  SensorStateMachine(uint8_t pipe): pipe(pipe) {}

  SensorValueType getValue() {
    return value;
  }
  SensorValueType getState() {
    return state;
  }


  void enable() {
      if (!isEnabled()) {
          state = SensorValueStatus::kIdle;
      }
  }
  void disable() {
      state = SensorValueStatus::kOff;
  }
  bool isEnabled() {
      return state != SensorValueStatus::kOff;
  }

  void startSampling() {
      state = SensorValueStatus::kSampling;
  }

  void transmitSample() {
    if (state == SensorValueStatus::kSampleAvailable) {
      Serial.print(F("Transmitting sample for Pipe "));
      Serial.print(pipe, DEC);
      if (lib_aci_get_nb_available_credits(&aci_state) > 0) {
        bool sendSuccess = lib_aci_send_data(pipe, (uint8_t*)&value, sizeof(value));
        if (sendSuccess) {
            Serial.println(F(": Success."));
        } else {
            Serial.println(F(": Failed."));
        }
      } else {
        Serial.println(F(": No credits available."));
      }
      state = SensorValueStatus::kIdle;
    }
  }


 protected:
  uint8_t pipe;
  SensorValueStatus state = SensorValueStatus::kOff;
  SensorValueType value;
};

#endif //  __SENSORSTATEMACHINE_H__