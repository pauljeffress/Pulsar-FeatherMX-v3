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
extern uint8_t num_waiting_messages;
extern uint8_t countTXRXlaps;
extern bool send_successful;
extern uint8_t num_iridium_tx_attempts;
extern bool firstTX;
extern bool validRX;
extern bool unvalidatedRX;

/* function pre defines */
uint16_t getNextISBDMoNum();
void case_contact_ground();
void do_iridium_locarb();
void prep_binary_MO_message_locarb();
void iridium_bypass_do_firstRXTX();     // lives in local file - iridium_bypass_do_firstRXTX.cpp
void iridium_ISBD_do_firstRXTX();       // lives in local file - iridium_ISBD_do_firstRXTX.cpp
void iridium_ISBD_do_additionalRXs();   // lives in local file - iridium_ISBD_do_additionalRXs.cpp


#endif
// END - contact_ground_fns.h



