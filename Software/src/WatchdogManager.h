#ifndef __WATCHDOGMANAGER_H__
#define __WATCHDOGMANAGER_H__

#include <Arduino.h>

extern volatile uint8_t expiryCounter;
#define NUM_EXPIRIES 7;  // Slightly under 1 minute

/**
 * \brief Setup Data Structures for the Watchdog.
 */
void wdg_init();

/**
 * \brief Determine whether the WDG has fired.
 */
inline bool wdg_expired() { return expiryCounter >= NUM_EXPIRIES; };

/**
 * \brief Consume an expiry of the WDG.
 */
inline void wdg_reset_expiry() {
  expiryCounter = 0;
};

/**
 * \brief Increment the reference count on watchdog activations.
 *
 * When the count is > 0, the watchdog interrupt will be enabled.
 * The device will be periodically woken by the watchdog.
 */
void incrementWatchdogEnableCount();

/**
 * \brief Decrement the reference count on watchdog activations.
 *
 * When the count is == 0, the watchdog interrupt will be disabled.
 * The Watchdog will then no longer wake the device.
 */
void decrementWatchdogEnableCount();

#endif  //  __WATCHDOGMANAGER_H__