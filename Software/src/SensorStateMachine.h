#ifndef __SENSORSTATEMACHINE_H__
#define __SENSORSTATEMACHINE_H__

#include <Arduino.h>

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

class BatterySensorStateMachine: public SensorStateMachine<uint8_t> {
 public:
  using SensorStateMachine<uint8_t>::SensorStateMachine;

  void startSampling() {
    this->SensorStateMachine<uint8_t>::startSampling();
    power_adc_enable();
    // 0 = 0V; 1023 = 3.3V
    // Voltage divider gives half the value -> reading 3.3V is a completely full battery  
    int sample = analogRead(A3);
    power_adc_disable();
    
    value = map(sample, 0, 1023, 0, 100);
    Serial.print(F("Battery power: "));
    Serial.print(value, DEC);
    Serial.println("%");
    state = SensorValueStatus::kSampleAvailable;
  }
};

#endif //  __SENSORSTATEMACHINE_H__