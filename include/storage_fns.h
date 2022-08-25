/*
 * storage_fns.h
 * 
 */

#ifndef STORAGE_FNS_H
#define STORAGE_FNS_H

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_External_EEPROM.h" // Click here to get the library: http://librarymanager/All#SparkFun_External_EEPROM


/* defines */

// Uncomment the below to reset myFMXSettings in the EEPROM to defaults.
#define RESET_EEPROM_TO_DEFAULTS

#define LOCATION_SETTINGS   0 //Position in EEPROM to store the setting structure
#define LOCATION_FMX_STX                0  
#define LOCATION_FMX_EEPROM_MAGIC_NUM_A 1
#define LOCATION_FMX_EEPROM_MAGIC_NUM_B 2
#define LOCATION_FMX_EEPROM_MAGIC_NUM_C 3
#define LOCATION_FMX_EEPROM_MAGIC_NUM_D 4
#define LOCATION_FMX_EEPROM_MAGIC_NUM_E 5
#define LOCATION_FMX_EEPROM_MAGIC_NUM_F 6


/* define any enums */


/* define any struct's */


/* extern global variables */
extern ExternalEEPROM myStorage;
extern bool myStorage_status;

/* function pre defines */
bool checkEEPROM();
void putFmxSettings();
void getFmxSettings();
void setupFmxSettings();

#endif
// END - storage_fns.h