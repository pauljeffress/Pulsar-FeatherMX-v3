/*
 * sensor_fns.h
 * 
 */

#ifndef SENSOR_FNS_H
#define SENSOR_FNS_H

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHTC3.h"     // Adafruit SHTC3 temp & humidity sensor - https://learn.adafruit.com/adafruit-sensirion-shtc3-temperature-humidity-sensor?view=all
#include "Adafruit_VEML7700.h"  // Adafruit VEML7700 ambient light sensor - https://learn.adafruit.com/adafruit-veml7700?view=all

/* defines */

/* define any enums */

/* define any struct's */

/* extern global variables */
extern Adafruit_SHTC3 shtc3;
extern sensors_event_t shtc3_humidity, shtc3_temp;
extern bool sensor_shtc3_status; 

extern Adafruit_VEML7700 veml;
extern bool sensor_veml7700_status; 

/* function pre defines */
void sensorsSetup();
void case_read_sensors();

#endif
// END - sensor_fns.h



