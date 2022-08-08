/*
 * sensor_fns.cpp
 */

#include "global.h" // My main header file for this project itself

#include "sensor_fns.h" // Header for this subset of globals and functions etc.

Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();
sensors_event_t shtc3_humidity, shtc3_temp;
bool sensor_shtc3_status;

Adafruit_VEML7700 veml = Adafruit_VEML7700();
bool sensor_veml7700_status; 

void sensorsSetup()
{
    debugPrintln("sensorsSetup() - Starting.");

    // Setup the SHTC3 sensor
    debugPrintln("sensorsSetup() - SHTC3 initialising");
    if (!shtc3.begin())
    {
        debugPrintln("sensorsSetup() - Couldn't find SHTC3");
        sensor_shtc3_status = BAD;
    }
    else
    {
        debugPrintln("sensorsSetup() - Found SHTC3 sensor");
        sensor_shtc3_status = GOOD;
    }

    // Setup the VEML7700 sensor
    debugPrintln("sensorsSetup() - VEML7700 initialising");
    if (!veml.begin())
    {
        debugPrintln("sensorsSetup() - Couldn't find VEML7700");
        sensor_veml7700_status = BAD;
    }
    else
    {
        debugPrintln("sensorsSetup() - Found VEML7700 sensor");
        sensor_veml7700_status = GOOD;
    }


}
