/*
 * OLED_mini_fns.h
 * 
 */

#ifndef OLED_MINI_FNS_H
#define OLED_MINI_FNS_H

#include <Arduino.h>
#include <Wire.h>               // for the little SSD1306 type I2C OLEDs
#include "SSD1306Ascii.h"       // for the little SSD1306 type I2C OLEDs
#include "SSD1306AsciiWire.h"   // for the little SSD1306 type I2C OLEDs

/* defines */

#define OLED_MINI_ADDR 0x3C    // 0X3C+SA0 - 0x3C or 0x3D
#define OLED_MINI_RST_PIN -1    // Define proper RST_PIN if required.

/* define any enums */

/* define any struct's */

/* extern global variables */
extern SSD1306AsciiWire oled;
extern bool OLED_mini_status; 

/* function pre defines */
void OLEDMiniSetup();

#endif
// END - OLED_mini_fns.h



