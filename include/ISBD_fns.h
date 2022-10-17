/*
 * ISBD_fns.h
 * 
 */

#ifndef ISBD_FNS_H
#define ISBD_FNS_H

#include <Arduino.h>
#include "IridiumSBD.h"


/* defines */
#define IRIDIUM_PWR_FET_PIN     17          // Pin the FMX uses to enable power to the 9603 via a FET
#define IRIDIUM_SLEEP_PIN   -1              // -1 means unused
#define IRIDIUM_RING_INDICATOR_PIN   -1     // -1 means unused

//#define BYPASS_IRIDIUM                  // Uncomment this line to completely bypass using the Iridium modem (i.e don't charge supercaps, don't ISBD TX/RX etc) and use the test Serial connection insted.
//#define SKIP_IRIDIUM_TX                 // Uncomment this line to only skip the Iridium SBD TX/RX cmds, for example if you want to test the code that charges the supercaps, but without using ISBD message credits
#define ENABLE_ISBD_DIAGNOSTICS         // Uncomment to see IridiumSBD library specific diagnostics on Serial Monitor.

#define ISBD_RX_ARRAY_MAX_MSGS 4        // The maximum number of ISBD Mobile Terminated (i.e. inbound to boat) messages
                                        // that we will temporarily store if there is more than one available from ISBD
                                        // during one ISBD session.

#define MAX_ISBD_TX_TRYS  3   // Max num times we try to TX an MO message before giving up for until next TXINT.
#define MAX_ISBD_LAPS     6   // Max number of ISBD TX/RX's in one ISBD session. Looking to stop bizzare problems running away spending ISBD credits!



/* define any enums */


/* define any struct's */


/* extern global variables */
extern IridiumSBD isbdModem;


/* function pre defines */
void ISBDSetup();
bool ISBDCallback();
void ISBDConsoleCallback(IridiumSBD *device, char c);
void ISBDDiagsCallback(IridiumSBD *device, char c);
void iridiumON();
void iridiumOFF();
boolean prep_iridium_modem();
void zero_inBufferNewLatest();
void zero_inBufferNew();
boolean isbdCheckMtMsg();
void store_received_MT_msg();
void clear_mo_buffer();
void update_waiting_msgs_iridium();
void print_iridium_endstate();
void sleep_iridium_modem();


#endif
// END - ISBD_fns.h



