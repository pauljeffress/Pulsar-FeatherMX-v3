/*
 * pixel_fns.cpp
 * 
 * Functions that use Feathers onboard NeoPixel
 * https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-installation
 * 
 * 
 */

#include "global.h" // My main header file for this project itself
#include "pixel_fns.h"

// Globals
Adafruit_NeoPixel pixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
 

// Functions
void pixelSetup()
{
    debugPrintln("pixelSetup() - initialising onboard NeoPixel");
    pixel.begin();
    pixel.show(); // Initialize all pixels to 'off'
    pixel.setPixelColor(PIXEL_INDEX, 255,0,0);  // RED
    pixel.show();
    delay(1000);
    pixel.setPixelColor(PIXEL_INDEX, 0,255,0);  // GREEN
    pixel.show();
    delay(1000);
    pixel.setPixelColor(PIXEL_INDEX, 0,0,255);  // BLUE
    pixel.show();
    delay(1000);
    pixel.setPixelColor(PIXEL_INDEX, 0,0,0);  // OFF
    pixel.show();
}   // END - pixelSetup()

void pixelOff()
{
    pixel.setPixelColor(PIXEL_INDEX, 0,0,0);  // OFF
    pixel.show();
}

void pixelRed()
{
    pixel.setPixelColor(PIXEL_INDEX, 255,0,0);  // RED
    pixel.show();
}

void pixelGreen()
{
    pixel.setPixelColor(PIXEL_INDEX, 0,255,0);  // GREEN
    pixel.show();
}

void pixelBlue()
{
    pixel.setPixelColor(PIXEL_INDEX, 0,0,255);  // BLUE
    pixel.show();
}

void pixelMagenta()
{
    pixel.setPixelColor(PIXEL_INDEX, 255,0,255);  // Magenta
    pixel.show();
}

// END - pixel_fns.cpp

