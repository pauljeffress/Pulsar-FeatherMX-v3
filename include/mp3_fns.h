/*
 * mp3_fns.h
 *
 */

#ifndef MP3_FNS_H
#define MP3_FNS_H

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_Qwiic_MP3_Trigger_Arduino_Library.h" // https://github.com/sparkfun/SparkFun_Qwiic_MP3_Trigger_Arduino_Library

/* defines */

/* define any enums */

/* define any struct's */

/* extern global variables */
extern MP3TRIGGER mp3;
extern bool mp3_status; 

/* function pre defines */
void MP3Setup();

#endif
// END - mp3_fns.h
