/*
 * OLED_mini_fns.cpp
 * 
 * Functions that use SSD1306 based OLED I2C displays
 * 
 * 
 */


#include "OLED_mini_fns.h"

// Globals
SSD1306AsciiWire oled;  
bool OLED_mini_status = false;  

// Functions

/*
 * Attempt to initialise the OLED object,
 * Then set global var OLED_mini_status accordingly.
 */
void OLEDMiniSetup()
{
    Serial.println("OLEDMiniSetup() - Starting.");

    // We are assuming "Wire.begin()"" has already been run.

    // Initalize the OLED device and related graphics system
    oled.begin(&Adafruit128x64, OLED_MINI_ADDR);

    OLED_mini_status = true;

    oled.setScrollMode(SCROLL_MODE_AUTO);   // depends on INCLUDE_SCROLLING must be non-zero.  Edit SSD1306Ascii.h

    oled.setFont(Adafruit5x7);

    oled.clear();

    // Initialisation text
    oled.println("    --- Pulsar ---");

    Serial.println("OLEDMiniSetup() - Complete.");
}   // END - OLEDMiniSetup()

