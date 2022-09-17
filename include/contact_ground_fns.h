/*
 * contact_ground_fns.h
 * 
 */

#ifndef CONTACT_GROUND_FNS_H
#define CONTACT_GROUND_FNS_H

#include <Arduino.h>

/* defines */


/* define any enums */


/* define any struct's */


/* extern global variables */
extern uint8_t isbdNumWaitingMessages;
extern uint8_t isbdCountTxRxLaps     ;
extern bool    isbdSendSuccessful    ;
extern uint8_t isbdNumTxAttempts      ;
extern bool    isbdFirstTx           ;
extern bool    isbdValidRx           ;
extern bool    isbdUnvalidatedRx     ;

/* function pre defines */
uint16_t getNextISBDMoNum();
void case_contact_ground();
void do_iridium_locarb();
void prep_binary_MO_message_locarb();
void iridium_bypass_do_firstRXTX();     // lives in local file - iridium_bypass_do_firstRXTX.cpp
void isbdDoFirstRxTx();                 // lives in local file - isbdDoFirstRxTx.cpp
void isbdDoAdditionalRxs();             // lives in local file - isbdDoAdditionalRxs.cpp
void case_process_isbd_rx();            // lives in localfile - process_isbd_rx.cpp
void setFmxContactGroundInt(uint16_t txint_m);   // lives in setFmx_fns.cpp
#endif
// END - contact_ground_fns.h



