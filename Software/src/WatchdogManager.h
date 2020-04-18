#ifndef __WATCHDOGMANAGER_H__
#define __WATCHDOGMANAGER_H__

extern volatile bool wdgFlag;

/**
 * \brief Setup Data Structures for the Watchdog.
 */
void wdg_init();

/**
 * \brief Determine whether the WDG has fired.
 */
inline bool wdg_expired() { return wdgFlag; };

/**
 * \brief Consume an expiry of the WDG.
 */
inline void wdg_reset_expiry() { wdgFlag = false; };

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