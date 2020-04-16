#ifndef __IRSNDADAPTER_H__
#define __IRSNDADAPTER_H__

#include <Arduino.h>

extern "C" {
//#include <irmp.h>
#include <irsnd.h>
}

/* F_INTERRUPTS is the interrupt frequency defined in irmpconfig.h */
#define US (1000000 / F_INTERRUPTS)

// typical protocols, disable here!             Enable  Remarks                 F_INTERRUPTS            Program Space
#define IRSND_SUPPORT_SIRCS_PROTOCOL            1       // Sony SIRCS           >= 10000                 ~200 bytes
#define IRSND_SUPPORT_NEC_PROTOCOL              1       // NEC + APPLE          >= 10000                 ~100 bytes
#define IRSND_SUPPORT_SAMSUNG_PROTOCOL          1       // Samsung + Samsung32  >= 10000                 ~300 bytes
#define IRSND_SUPPORT_MATSUSHITA_PROTOCOL       1       // Matsushita           >= 10000                 ~200 bytes
#define IRSND_SUPPORT_KASEIKYO_PROTOCOL         1       // Kaseikyo             >= 10000                 ~300 bytes

// more protocols, enable here!                 Enable  Remarks                 F_INTERRUPTS            Program Space
#define IRSND_SUPPORT_DENON_PROTOCOL            0       // DENON, Sharp         >= 10000                 ~200 bytes
#define IRSND_SUPPORT_RC5_PROTOCOL              0       // RC5                  >= 10000                 ~150 bytes
#define IRSND_SUPPORT_RC6_PROTOCOL              0       // RC6                  >= 10000                 ~250 bytes
#define IRSND_SUPPORT_RC6A_PROTOCOL             0       // RC6A                 >= 10000                 ~250 bytes
#define IRSND_SUPPORT_JVC_PROTOCOL              0       // JVC                  >= 10000                 ~150 bytes
#define IRSND_SUPPORT_NEC16_PROTOCOL            0       // NEC16                >= 10000                 ~150 bytes
#define IRSND_SUPPORT_NEC42_PROTOCOL            0       // NEC42                >= 10000                 ~150 bytes
#define IRSND_SUPPORT_IR60_PROTOCOL             0       // IR60 (SDA2008)       >= 10000                 ~250 bytes
#define IRSND_SUPPORT_GRUNDIG_PROTOCOL          0       // Grundig              >= 10000                 ~300 bytes
#define IRSND_SUPPORT_SIEMENS_PROTOCOL          0       // Siemens, Gigaset     >= 15000                 ~150 bytes
#define IRSND_SUPPORT_NOKIA_PROTOCOL            0       // Nokia                >= 10000                 ~400 bytes

// exotic protocols, enable here!               Enable  Remarks                 F_INTERRUPTS            Program Space
#define IRSND_SUPPORT_KATHREIN_PROTOCOL         0       // Kathrein             >= 10000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_NUBERT_PROTOCOL           0       // NUBERT               >= 10000                 ~100 bytes
#define IRSND_SUPPORT_FAN_PROTOCOL              1       // FAN (ventilator)     >= 10000                 ~100 bytes
#define IRSND_SUPPORT_SPEAKER_PROTOCOL          0       // SPEAKER              >= 10000                 ~100 bytes
#define IRSND_SUPPORT_BANG_OLUFSEN_PROTOCOL     0       // Bang&Olufsen         >= 10000                 ~250 bytes
#define IRSND_SUPPORT_RECS80_PROTOCOL           0       // RECS80               >= 15000                 ~100 bytes
#define IRSND_SUPPORT_RECS80EXT_PROTOCOL        0       // RECS80EXT            >= 15000                 ~100 bytes
#define IRSND_SUPPORT_THOMSON_PROTOCOL          0       // Thomson              >= 10000                 ~250 bytes
#define IRSND_SUPPORT_NIKON_PROTOCOL            0       // NIKON                >= 10000                 ~150 bytes
#define IRSND_SUPPORT_NETBOX_PROTOCOL           0       // Netbox keyboard      >= 10000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_ORTEK_PROTOCOL            0       // ORTEK (Hama)         >= 10000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_TELEFUNKEN_PROTOCOL       0       // Telefunken 1560      >= 10000                 ~150 bytes
#define IRSND_SUPPORT_FDC_PROTOCOL              0       // FDC IR keyboard      >= 10000 (better 15000)  ~150 bytes
#define IRSND_SUPPORT_RCCAR_PROTOCOL            0       // RC CAR               >= 10000 (better 15000)  ~150 bytes
#define IRSND_SUPPORT_ROOMBA_PROTOCOL           0       // iRobot Roomba        >= 10000                 ~150 bytes
#define IRSND_SUPPORT_RUWIDO_PROTOCOL           0       // RUWIDO, T-Home       >= 15000                 ~250 bytes
#define IRSND_SUPPORT_A1TVBOX_PROTOCOL          0       // A1 TV BOX            >= 15000 (better 20000)  ~200 bytes
#define IRSND_SUPPORT_LEGO_PROTOCOL             0       // LEGO Power RC        >= 20000                 ~150 bytes
#define IRSND_SUPPORT_RCMM_PROTOCOL             0       // RCMM 12,24, or 32    >= 20000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_LGAIR_PROTOCOL            0       // LG Air Condition     >= 10000                 ~150 bytes.
#define IRSND_SUPPORT_SAMSUNG48_PROTOCOL        0       // Samsung48            >= 10000                 ~100 bytes
#define IRSND_SUPPORT_PENTAX_PROTOCOL           0       // Pentax               >= 10000                 ~150 bytes
#define IRSND_SUPPORT_S100_PROTOCOL             0       // S100                 >= 10000                 ~150 bytes
#define IRSND_SUPPORT_ACP24_PROTOCOL            0       // ACP24                >= 10000                 ~150 bytes
#define IRSND_SUPPORT_TECHNICS_PROTOCOL         1       // TECHNICS             >= 10000                 ~150 bytes

void printFrame(IRMP_DATA & frame);

void IRSND_adapter_init();
void IRSND_timerinterrupt();
/**
 * \brief
 * \param data Assumed to contain sizeof(IRMP_DATA) bytes.
 */
void receivedIRMPPacket(const uint8_t * data);

#endif // __IRSNDADAPTER_H__
