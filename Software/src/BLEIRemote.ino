/* Copyright (c) 2014, Nordic Semiconductor ASA

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

/**
   My project template
*/

/** @defgroup my_project my_project
  @{
  @ingroup projects
  @brief Empty project that can be used as a template for new projects.

  @details
  This project is a firmware template for new projects.
  The project will run correctly in its current state, but does nothing.
  With this project you have a starting point for adding your own application functionality.

  The following instructions describe the steps to be made on the Windows PC:

  -# Install the Master Control Panel on your computer. Connect the Master Emulator
    (nRF2739) and make sure the hardware drivers are installed.

*/

/* first include Arduino.h, the IDE includes it after irmp*.h ... */
#include "Arduino.h"
/* ... and then chokes on uintX_t ... */
#include <avr/sleep.h>
#include <avr/power.h>

// Glue Code to IRSND
#include "irsndadapter.h"

// Glue Code and State Machine to nRF8001
#include "bleadapter.h"

// Interrupt PIN used for Wakeup from nrf8001
#define RDYN_INTR_NO 0
volatile unsigned long wakeupCounter = 0;

/** 
 * ISR for RDYN low events
 */
void rdyn_isr()
{
    // This is a level interrupt that would fire again while the
    // signal is low. Thus, we need to detach the interrupt.
    detachInterrupt(RDYN_INTR_NO);
    ble_ready = true;  
    ++wakeupCounter;
}

void ble_dataReceived_Cbk(uint8_t pipe, uint8_t * data, uint8_t len) {
  if (PIPE_IRSND_IRMP_PACKET_RX_ACK_AUTO == pipe) {
    receivedIRMPPacket(data);
  }
}


/**
 * Put device into power-down mode to save as much energy as possible.
 * 
 * The device will wake up again by interrupt, either the watch dog timer 
 * or RDYN going low (= nRF8001 sends an event to device).
 */
void do_sleep()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    // Disable interrupts until we sleep to avoid race conditions
    // (interrupt firing before going to sleep would prevent MCU from
    // waking by interrupt).
    cli();
    if (ble_ready) {
        // Last chance to stay awake.
        sei();
    } else {
        sleep_enable();
        #ifdef DISABLE_BOD_WHILE_SLEEPING
        // Disabling brown-out detection while sleeping 
        // Saves about 25 uA.
        // BODS: Brown-out Detection Sleep
        // BODSE: Brown-out Detection Sleep Enable
        // This is a timed sequence:
        // First, BODS and BODSE must me set to one.
        // Then, BODS must be set to one and BODSE to zero
        // within four clock cycles. Then, BODS stays active three
        // clock cycles, so sleep_cpu() must be called within
        // three cycles after setting BODS.
        MCUCR = bit(BODS) | bit(BODSE);
        MCUCR = bit(BODS);
        #endif 
        // Enable interrupts again. It is guranteed that the next
        // command (entering sleep mode) is executed *before* an 
        // interrupt is fired (no race condition). From the data sheet:
        // "When using the SEI instruction to enable interrupts, 
        // the instruction following SEI will be executed
        // before any pending interrupts."
        sei();
        sleep_cpu();
        // Wake again after interrupt.
        sleep_disable();
    }
}

void setup(void)
{
  Serial.begin(115200);
  //Wait until the serial port is available (useful only for the Leonardo)
  //As the Leonardo board is not reseted every time you open the Serial Monitor
#if defined (__AVR_ATmega32U4__)
  while (!Serial)
  {}
  delay(5000);  //5 seconds delay for enabling to see the start up comments on the serial board
#elif defined(__PIC32MX__)
  delay(1000);
#endif
  Serial.println(F("Arduino setup"));
  power_adc_disable();

  IRSND_adapter_init();

  ble_setup();

  // Install interrupt for RDYN line of nRF8001 for event handling.
  // We use a level-interrupt that can also fire in sleep mode to
  // wake up the Arduino when an event is received.
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(RDYN_INTR_NO, rdyn_isr, LOW);
}

void loop()
{
  bool workAvailable = false;

  // Loop the BLE state machine. Returns true when there is additional work to be done.
  workAvailable |= ble_loop();

  // Returns true when irsnd is currently transmitting - avoid sleeping during that time.
  workAvailable |= irsnd_is_busy();

  if (!workAvailable) {
    attachInterrupt(RDYN_INTR_NO, rdyn_isr, LOW);
    // No event in the ACI Event queue
    // Arduino can go to sleep now
    Serial.println(F("Arduino going to sleep."));
    delay(100); // Allow for completion of serial transmission
    do_sleep();
    Serial.print(F("Arduino woke up. Wakeup-Counter: "));
    Serial.println(wakeupCounter, DEC);
    // Wakeup from sleep from the RDYN line
  }
}

