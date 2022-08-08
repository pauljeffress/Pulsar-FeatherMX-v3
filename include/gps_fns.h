/*
 * gps_fns.h
 * 
 */

#ifndef GPS_FNS_H
#define GPS_FNS_H

#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>   //  https://github.com/sparkfun/SparkFun_u-blox_GNSS_Arduino_Library 
                                                    // Include the u-blox library first so the message fields know about the dynModel enum

/* defines */


/* define any enums */


/* define any struct's */


/* extern global variables */
extern SFE_UBLOX_GNSS myGNSS;

/* function pre defines */
void print_gps_data();
void gpsSetup();
void case_read_gps();

#endif
// END - gps_fns.h



