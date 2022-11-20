/*
 * heartbeat_to_autopilot_fns.h
 * 
 */

#ifndef HEARTBEAT_TO_AUTOPILOT_FNS_H
#define HEARTBEAT_TO_AUTOPILOT_FNS_H

#include <Arduino.h>


/* defines */
#define FMX_TXCANINT_S    60    // seconds - how often the FMX should TX info about itself onto the CANbus.
#define FMX_TX_HB_AP_S    180   // seconds - how often the FMX should send a MAVLink Heartbeat to teh AP.

/* define any enums */


/* define any struct's */


/* extern global variables */

/* function pre defines */
void case_heartbeat_to_autopilot();

#endif
// END - heartbeat_to_autopilot_fns.h



