
#ifndef __BLEADAPTER_H__
#define __BLEADAPTER_H__

#include <EEPROM.h>
#include <SPI.h>
#include <aci_setup.h>
#include <lib_aci.h>

/**
  Put the nRF8001 setup in the RAM of the nRF8001.
*/
#include "services.h"
/**
  Include the services_lock.h to put the setup in the OTP memory of the nRF8001.
  This would mean that the setup cannot be changed once put in.
  However this removes the need to do the setup of the nRF8001 on every reset.
*/

extern struct aci_state_t aci_state;

void ble_setup();
/**
 * \brief Performs the event loop of the ACI adapter.
 * \returns true if there is work pending, false otherwise.
 */
void ble_loop();

bool ble_available();
void ble_setWorkAvailable();

extern void ble_dataReceived_Cbk(uint8_t pipe, uint8_t* data, uint8_t len);
extern void ble_pipeEvent_Cbk();
extern void ble_temperature_Cbk(uint16_t temperature);

#endif  // __BLEADAPTER_H__