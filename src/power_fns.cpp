/*
 * power_fns.cpp
 *
 */

#include "global.h"
#include "power_fns.h"

#ifndef A6
#define A6 20 // This is a complete hack because this file gets compiled no matter what I do, when I compile the AGT code.
              // And as the AGT does not have an A6 it fails to compile...even though we don't even call this function
              // from the AGT.
              // So given that on a Feather A6 = D20, I am just defining it here and it will still work on a Feather,
              // and at least compile on teh AGT but not be used.
#endif
// Power measurement stuff
#define FEATHER_VBAT_PIN A6  // Pin the battery monitor voltage divider circuit is connected to
#define FEATHER_VBAT_LOW 3.3 // When running on the Feathers battery, what do we consider low battery.

void check_power(bool silent)
{
    if (checkPowerFlag) // we do this here because checkPower is in my Pulsar Shared Source
    {

        if (!silent)
        {
            debugPrintln("case_check_power() - time to execute");
            mp3.playFile(5); // "Checking Power"
            delay(2000);
        }

        checkPowerFlag = false; // reset the flag now that we are actioning it.

        // measure it as per https://learn.adafruit.com/adafruit-feather-m4-express-atsamd51/power-management
        float measuredvbat = analogRead(FEATHER_VBAT_PIN);
        measuredvbat *= 2;    // we divided by 2 with the resistor divider circuit, so multiply back
        measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
        measuredvbat /= 1024; // convert to voltage

        if (!silent)
        {
            debugPrint("case_check_power() - ");
            debugPrintlnFlt(measuredvbat);
        }

        myFmxSettings.FMX_BATT_V = measuredvbat * 100; // we store it as an int of voltage * 100, e.g. 3.8v = 380
    }
} // END - check_power()

    // END - power_fns.h
