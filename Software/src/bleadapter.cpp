#include "bleadapter.h"

volatile bool ble_ready;
bool setup_required = false;


#ifdef SERVICES_PIPE_TYPE_MAPPING_CONTENT
static services_pipe_type_mapping_t
services_pipe_type_mapping[NUMBER_OF_PIPES] = SERVICES_PIPE_TYPE_MAPPING_CONTENT;
#else
#define NUMBER_OF_PIPES 0
static services_pipe_type_mapping_t * services_pipe_type_mapping = NULL;
#endif
static const hal_aci_data_t setup_msgs[NB_SETUP_MESSAGES] PROGMEM = SETUP_MESSAGES_CONTENT;

// aci_struct that will contain
// total initial credits
// current credit
// current state of the aci (setup/standby/active/sleep)
// open remote pipe pending
// close remote pipe pending
// Current pipe available bitmap
// Current pipe closed bitmap
// Current connection interval, slave latency and link supervision timeout
// Current State of the the GATT client (Service Discovery)
// Status of the bond (R) Peer address
static struct aci_state_t aci_state;
static hal_aci_evt_t aci_data;
static hal_aci_data_t aci_cmd;

bool ble_available() {
  return ble_ready || setup_required;
}

void ble_setWorkAvailable() {
  ble_ready = true;
}

void ble_setup() {

  ble_ready = false;

  if (NULL != services_pipe_type_mapping)
  {
    aci_state.aci_setup_info.services_pipe_type_mapping = &services_pipe_type_mapping[0];
  }
  else
  {
    aci_state.aci_setup_info.services_pipe_type_mapping = NULL;
  }
  aci_state.aci_setup_info.number_of_pipes    = NUMBER_OF_PIPES;
  aci_state.aci_setup_info.setup_msgs         = (hal_aci_data_t*) setup_msgs;
  aci_state.aci_setup_info.num_setup_msgs     = NB_SETUP_MESSAGES;

  /*
    Tell the ACI library, the MCU to nRF8001 pin connections.
    The Active pin is optional and can be marked UNUSED
  */
  aci_state.aci_pins.board_name = BOARD_DEFAULT; //See board.h for details
  aci_state.aci_pins.reqn_pin   = 10;
  aci_state.aci_pins.rdyn_pin   = 2; // Changed to INT0
  aci_state.aci_pins.mosi_pin   = MOSI;
  aci_state.aci_pins.miso_pin   = MISO;
  aci_state.aci_pins.sck_pin    = SCK;

  aci_state.aci_pins.spi_clock_divider      = SPI_CLOCK_DIV8;//SPI_CLOCK_DIV8  = 2MHz SPI speed
  //SPI_CLOCK_DIV16 = 1MHz SPI speed

  aci_state.aci_pins.reset_pin              = 5;
  aci_state.aci_pins.active_pin             = UNUSED;
  aci_state.aci_pins.optional_chip_sel_pin  = UNUSED;

  aci_state.aci_pins.interface_is_interrupt = false;
  aci_state.aci_pins.interrupt_number       = 1;

  //We reset the nRF8001 here by toggling the RESET line connected to the nRF8001
  //and initialize the data structures required to setup the nRF8001
  lib_aci_init(&aci_state, false);
}

void ble_loop() {

  // We enter the if statement only when there is a ACI event available to be processed
  if (lib_aci_event_get(&aci_state, &aci_data))
  {
    Serial.println(F("ACI Event available"));
    aci_evt_t * aci_evt;
    aci_evt = &aci_data.evt;
    switch (aci_evt->evt_opcode)
    {
      /**
        As soon as you reset the nRF8001 you will get an ACI Device Started Event
      */
      case ACI_EVT_DEVICE_STARTED:
        {
          aci_state.data_credit_available = aci_evt->params.device_started.credit_available;
          switch (aci_evt->params.device_started.device_mode)
          {
            case ACI_DEVICE_SETUP:
              /**
                When the device is in the setup mode
              */
              Serial.println(F("Evt Device Started: Setup"));
              setup_required = true;
              break;

            case ACI_DEVICE_STANDBY:
              Serial.println(F("Evt Device Started: Standby"));
              //See ACI Broadcast in the data sheet of the nRF8001
              //lib_aci_broadcast(0/* in seconds */, 0x0100 /* advertising interval 100ms */);
              lib_aci_connect(0, 0x0100);
              //While broadcasting (non_connectable) interval of 100ms is the minimum possible
              Serial.println(F("Broadcasting started"));
              //To stop the broadcasting before the timeout use the
              //lib_aci_radio_reset to soft reset the radio
              //See ACI RadioReset in the datasheet of the nRF8001
              break;
          }
        }
        break; //ACI Device Started Event

      case ACI_EVT_CMD_RSP:
        //If an ACI command response event comes with an error -> stop
        if (ACI_STATUS_SUCCESS != aci_evt->params.cmd_rsp.cmd_status)
        {
          //ACI ReadDynamicData and ACI WriteDynamicData will have status codes of
          //TRANSACTION_CONTINUE and TRANSACTION_COMPLETE
          //all other ACI commands will have status code of ACI_STATUS_SCUCCESS for a successful command
          Serial.print(F("ACI Command "));
          Serial.println(aci_evt->params.cmd_rsp.cmd_opcode, HEX);
          Serial.println(F("Evt Cmd respone: Error. Arduino is in an while(1); loop"));
          while (1);
        }
        break;

      case ACI_EVT_CONNECTED:
        Serial.println(F("Evt Connected"));
        break;

      case ACI_EVT_PIPE_STATUS:
        Serial.println(F("Evt Pipe Status"));
        break;

      case ACI_EVT_DISCONNECTED:
        if (ACI_STATUS_ERROR_ADVT_TIMEOUT == aci_evt->params.disconnected.aci_status)
        {
          Serial.println(F("Broadcasting timed out"));
        }
        else
        {
          Serial.println(F("Evt Disconnected. Link Loss"));
        }
        Serial.println(F("Restarting Broadcast."));
        lib_aci_connect(0, 0x0100);
        break;

      case ACI_EVT_DATA_RECEIVED:
        Serial.print(F("Data received on Pipe #: 0x"));
        Serial.println(aci_evt->params.data_received.rx_data.pipe_number, HEX);
        Serial.print(F("Length of data received: 0x"));
        Serial.println(aci_evt->len - 2, HEX);

        if (PIPE_IRSND_IRMP_PACKET_RX_ACK_AUTO == aci_evt->params.data_received.rx_data.pipe_number) {
          /* Received an IRMP packet */
          ble_dataReceived_Cbk(aci_evt->params.data_received.rx_data.pipe_number, &(aci_evt->params.data_received.rx_data.aci_data[0]), aci_evt->len - 2);
        }

        break;

      case ACI_EVT_HW_ERROR:
        Serial.println(F("HW error: "));
        Serial.println(aci_evt->params.hw_error.line_num, DEC);
        for (uint8_t counter = 0; counter <= (aci_evt->len - 3); counter++)
        {
          Serial.write(aci_evt->params.hw_error.file_name[counter]); //uint8_t file_name[20];
        }
        Serial.println();
        break;
    }
  }
  else
  {
    if (!setup_required) {
      Serial.println(F("No ACI Events available, no setup required."));
      ble_ready = false;
    } else {
    /* setup_required is set to true when the device starts up and enters setup mode.
       It indicates that do_aci_setup() should be called. The flag should be cleared if
       do_aci_setup() returns ACI_STATUS_TRANSACTION_COMPLETE.
    */
      Serial.println(F("No ACI Events available, setup required"));
      Serial.println(F("Doing nRF8001 Setup"));
      if (SETUP_SUCCESS == do_aci_setup(&aci_state))
      {
        setup_required = false;
      }
    }
  }
}

/* Define how assert should function in the BLE library */
void __ble_assert(const char *file, uint16_t line)
{
  Serial.print("ERROR ");
  Serial.print(file);
  Serial.print(": ");
  Serial.print(line);
  Serial.print("\n");
  while (1);
}
