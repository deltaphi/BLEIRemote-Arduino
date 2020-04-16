
#ifndef __BLEADAPTER_H__
#define __BLEADAPTER_H__

#include <SPI.h>
#include <EEPROM.h>
#include <lib_aci.h>
#include <aci_setup.h>

/**
  Put the nRF8001 setup in the RAM of the nRF8001.
*/
#include "services.h"
/**
  Include the services_lock.h to put the setup in the OTP memory of the nRF8001.
  This would mean that the setup cannot be changed once put in.
  However this removes the need to do the setup of the nRF8001 on every reset.
*/

extern volatile bool ble_ready;

void ble_setup();
/**
 * \brief Performs the event loop of the ACI adapter.
 * \returns true if there is work pending, false otherwise.
 */
bool ble_loop();

extern void ble_dataReceived_Cbk(uint8_t pipe, uint8_t * data, uint8_t len);

#endif // __BLEADAPTER_H__