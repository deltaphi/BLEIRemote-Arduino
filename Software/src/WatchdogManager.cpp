#include <Arduino.h>

#include <avr/wdt.h>

#include "WatchdogManager.h"

static uint8_t watchdogEnableCount = 0;

volatile bool wdgFlag;

void wdg_start() {
  // Disable Interrupts & Reset Watchdog
  cli();
  wdt_reset();

  MCUSR &= ~(1 << WDRF);  // Configure WDG to not reset the system.
  WDTCSR = (1 << WDCE) | (1 << WDE);  // Watchdog Change Enable.
  WDTCSR = (1 << WDP3) | (1 << WDP0);  // Set Watchdog cycle.
  WDTCSR |= (1 << WDIE);  // Enabe Watchdog Interrupt.
  
  // Reenable interrupts.
  sei();
}

void wdg_init() {
  wdg_reset_expiry();
  watchdogEnableCount = 0;
}

ISR(WDT_vect) {
    wdgFlag = true;
}

void incrementWatchdogEnableCount() {
  if (watchdogEnableCount == 0xFF) {
    // hit the maximum value. This will cause errors, but not overflows.
  } else if (watchdogEnableCount == 0) {
      Serial.print(F("Enabling Watchdog."));
      wdg_start();
      ++watchdogEnableCount;
      // Simulate immediate expiry
      wdgFlag = true;
  }
  Serial.print(F("Increment Watchdog Count to "));
  Serial.println(watchdogEnableCount, DEC);
}

void decrementWatchdogEnableCount() {
  if (watchdogEnableCount > 1) {
    --watchdogEnableCount;
  } else if (watchdogEnableCount == 1) {
    Serial.print(F("Disabling Watchdog."));
    wdt_disable();
    --watchdogEnableCount;
    // Do not execute last ISR
    wdg_reset_expiry();
  }
  Serial.print(F("Decrement Watchdog Count to "));
  Serial.println(watchdogEnableCount, DEC);
}
 