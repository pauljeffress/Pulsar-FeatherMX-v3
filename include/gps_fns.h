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
//  DEBUGGING NOTES
// #define SKIP_GPS_FIX         // Uncomment to skip actually powering up and using the GPS
// #define ENABLE_GPS_DEBUG     // Uncomment to ENABLE the GNSS library internal debugs.

#define GPS_PWR_ENA_PIN 16          // (a.k.a A2) Pin that enables the GPS' dedicated 3.3v regulator sub module
#define GPS_POWERUP_TIMEOUT_S 5UL   // seconds - Timeout after this many _seconds_ when waiting
                                    //            for GPS to power up after power is enabled.
#define GPS_FIX_TIMEOUT_S 60UL      // seconds - Timeout after this many _minutes_ when waiting for a 3D GPS fix
#define GPS_MAX_TRIES 3             // When we try to get a fix, how many times do we try before failing and moving on.
//#define GPS_LAST_READ_PERIOD 3UL    // minutes - How recently do we consider it ok not to do another fresh read?
#define GPS_MIN_FIX_TYPE 3          // Minimum Fix Type (2D, 3D etc) for AGT onboard GPS to consider it got a usable fix.

#define FMX_GPS_STATUS_UNKNOWN          1
#define FMX_GPS_STATUS_INIT_OK          2
#define FMX_GPS_STATUS_INIT_FAILED      3
#define FMX_GPS_STATUS_FIX_OK           4
#define FMX_GPS_STATUS_FIX_FAILED       5

/* define any enums */



/* define any struct's */


/* extern global variables */
extern SFE_UBLOX_GNSS myGNSS;

/* function pre defines */
void gpsON();
void gpsOFF();
void print_gps_data();
void gpsSetup();
void case_read_gps();

#endif
// END - gps_fns.h



