/*
 * misc_fns.h
 * 
 */

#ifndef MISC_FNS_H
#define MISC_FNS_H

#include <Arduino.h>

/* defines */


/* define any enums */


/* define any struct's */


/* extern global variables */
extern long lastsec1;


/* function pre defines */

void setupPins();
String my64toString(uint64_t x);
void loopHousekeeping();

#endif
// END - misc_fns.h



