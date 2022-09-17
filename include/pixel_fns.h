/*
 * pixel_fns.h
 *
 */

#ifndef PIXEL_FNS_H
#define PIXEL_FNS_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h> // https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-installation

/* defines */
#define PIXEL_PIN 8     // according to https://learn.adafruit.com/adafruit-feather-m4-express-atsamd51?view=all
#define PIXEL_COUNT 1   // we only have one NeoPixel in this "strip".
#define PIXEL_INDEX 0   // the index for the first (and only in this case) pixel in a strip.
/* define any enums */

/* define any struct's */

/* extern global variables */
//extern Adafruit_NeoPixel pixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);


/* function pre defines */
void pixelSetup();
void pixelOff();
void pixelRed();
void pixelGreen();
void pixelBlue();
void pixelMagenta();
void pixelYellow();


#endif
// END - pixel_fns.h
