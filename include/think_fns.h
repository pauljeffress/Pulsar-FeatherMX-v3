/*
 * think_fns.h
 * 
 */

#ifndef THINK_FNS_H
#define THINK_FNS_H

#include <Arduino.h>

/* defines */

#define FMX_READGPS_PERIOD_S            300


/* define any enums */


/* define any struct's */


/* extern global variables */
extern bool justBootedFlag;
extern bool checkPowerFlag;
extern bool readSensorsFlag;
extern bool readGPSFlag;
extern bool contactGroundFlag; 
extern bool gotMsgFromGroundFlag;
extern bool txMsgToApFlag;
extern bool isbdTxSucceededFlag;

/* function pre defines */

void case_think();

#endif
// END - think_fns.h



