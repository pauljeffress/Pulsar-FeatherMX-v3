/*
 * power_fns.h
 * 
 * Header for power_fns.cpp
 * 
 */
#ifndef CASE_CHECK_POWER_H
#define CASE_CHECK_POWER_H

#include <arduino.h>


/* defines */
#define FMX_BATT_LOW_V  (3.3*100)   // voltage in V * 10^2 i.e 3.3v = 3300

/* define any enums */


/* define any struct's */


/* extern my global vars */


/* function pre defines */
void check_power(bool silent);

#endif
