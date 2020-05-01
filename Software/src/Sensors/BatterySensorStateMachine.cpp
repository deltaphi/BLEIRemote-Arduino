#include "BatterySensorStateMachine.h"

void BatterySensorStateMachine::startSampling() {
  this->SensorStateMachine<uint8_t>::startSampling();
  power_adc_enable();
  // 0 = 0V; 1023 = 3.3V
  // Voltage divider gives half the value -> reading 3.3V is a completely full
  // battery
  int sample = analogRead(A3);
  power_adc_disable();

  value = map(sample, 0, 1023, 0, 100);
  Serial.print(F("Battery power: "));
  Serial.print(value, DEC);
  Serial.println("%");
  state = SensorValueStatus::kSampleAvailable;
}