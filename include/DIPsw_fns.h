/*
 * DIPsw_fns.h
 *
 */

#ifndef DIPSW_FNS_H
#define DIPSW_FNS_H

#include <Arduino.h>
#include <Wire.h>

/* defines */

// DIP Switch input pins
#define DIPSW1_PIN              5   // INPUT pins connected to DIP Switch bank.  ON = HIGH. External 10kR to gnd.
#define DIPSW2_PIN              6   // INPUT pins connected to DIP Switch bank.  ON = HIGH. External 10kR to gnd.
#define DIPSW3_PIN              9   // INPUT pins connected to DIP Switch bank.  ON = HIGH. External 10kR to gnd.
#define DIPSW4_PIN              10  // INPUT pins connected to DIP Switch bank.  ON = HIGH. External 10kR to gnd.
#define DIPSW5_PIN              11  // INPUT pins connected to DIP Switch bank.  ON = HIGH. External 10kR to gnd.


/* define any enums */

/* define any struct's */

/* extern global variables */
extern bool DIPsw1_status;
extern bool DIPsw2_status;
extern bool DIPsw3_status;
extern bool DIPsw4_status;
extern bool DIPsw5_status;
extern bool DIPsw1_last_checked_status;
extern bool DIPsw2_last_checked_status;
extern bool DIPsw3_last_checked_status;
extern bool DIPsw4_last_checked_status;
extern bool DIPsw5_last_checked_status;

extern bool flag_bypass_iridium;
extern bool flag_reset_eeprom;
extern bool flag_contact_ground_every_5mins;

/* function pre defines */
void DIPswSetup();
void DIPswCheck();

#endif
// END - DIPsw_fns.h
