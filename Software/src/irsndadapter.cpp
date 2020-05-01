// This file instantiates irsnd with the given configuration.

#include "irsndadapter.h"

#include "irsnd.c.h"
#include "irsnd.h"

/* use TimerOne http://arduino.cc/playground/Code/Timer1 for interrupts */
#include <TimerOne.h>

IRMP_DATA irmp_data;

void IRSND_adapter_init() {
  /*irmp_init();
    Timer1.initialize(US);
    Timer1.attachInterrupt(timerinterrupt);
    Serial.println(F("IRMP Detector ready.\n\n"));
  */
  irsnd_init();
  Timer1.initialize(US);
  Timer1.attachInterrupt(IRSND_timerinterrupt);
}

/* helper function: attachInterrupt wants void(), but irmp_ISR is uint8_t() */
void IRSND_timerinterrupt() {
  irsnd_ISR();  // call irsnd ISR
}

void receivedIRMPPacket(const uint8_t* data) {
  Serial.print(F("IRMP Packet received\n"));
  memcpy(&irmp_data, data, sizeof(IRMP_DATA));
  Serial.println(F("Received Frame: "));
  printFrame(irmp_data);

  if (!irsnd_is_busy()) {
    Serial.println(F("IRSND idle. Sending IR frame."));
    irsnd_send_data(&irmp_data, TRUE);
  } else {
    Serial.println(F("IRSND busy. Dropping IR frame."));
  }
}

void printFrame(IRMP_DATA& frame) {
  Serial.print(F("P:"));
  Serial.print(frame.protocol, HEX);
  Serial.print(F(" A:"));
  Serial.print(frame.address, HEX);
  Serial.print(F(" C:"));
  Serial.print(frame.command, HEX);
  Serial.print(F(" "));
  Serial.println(frame.flags, HEX);
}
