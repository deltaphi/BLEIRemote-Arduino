#include "SensorStateMachine.h"

bool do_transmit_single(uint8_t pipe, uint8_t* value, uint8_t valueLen) {
  Serial.print(F("Transmitting sample for Pipe "));
  Serial.print(pipe, DEC);
  if (lib_aci_get_nb_available_credits(&aci_state) > 0) {
    bool sendSuccess = lib_aci_send_data(pipe, value, valueLen);
    if (sendSuccess) {
      Serial.println(F(": Success."));
    } else {
      Serial.println(F(": Failed."));
    }
    return sendSuccess;
  } else {
    Serial.println(F(": No credits available."));
    return false;
  }
}