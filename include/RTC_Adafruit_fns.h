/*
 * RTC_fns.h
 * 
 */

#ifndef RTC_FNS_H
#define RTC_FNS_H

#include <Arduino.h>
#include "RTClib.h"


/* defines */


/* define any enums */


/* define any struct's */


/* extern global variables */
extern bool RTC_status;
extern RTC_DS3231 rtc; 

/* function pre defines */
void RTCSetup(TwoWire *wireInstance);
void RTCPrintCurrentTime();

#endif
// END - RTC_fns.h



