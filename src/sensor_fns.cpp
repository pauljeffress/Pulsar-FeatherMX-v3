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

void case_read_sensors()
{
    // is it time to do a routine sensor read?
    if (readSensorsFlag)
    {
        debugPrintln("\ncase_read_sensors() - time to execute");

        readSensorsFlag = false; // reset the flag now that we are actioning it.

        mp3.playFile(4); delay(2000);// Play F004.mp3 "Read sensors"
        // oled.println("read_sensors()");

        if (sensor_shtc3_status) // don't try to read the sensor once its marked bad.  The flag will be reset when Feather is reset/power-cycled.
        {
            /*
             * ----------------------------------
             * Operate the SHTC3 Temp & Humidity Sensor
             * ----------------------------------
             */

            // TODO - are we supposed to turn on the SHT31's heater before taking a reading????

            debugPrintln("case_read_sensors() - SHTC3");

            shtc3.getEvent(&shtc3_humidity, &shtc3_temp); // populate temp and humidity objects with fresh data

            Serial.print("Temperature: ");
            Serial.print(shtc3_temp.temperature);
            Serial.println(" degrees C");
            Serial.print("Humidity: ");
            Serial.print(shtc3_humidity.relative_humidity);
            Serial.println("% rH");

            myFmxSettings.FMX_TEMP = shtc3_temp.temperature * 100; // * 100 to store as correct units in Pulsar project.

            myFmxSettings.FMX_RH = shtc3_humidity.relative_humidity * 100; // * 100 to store as correct units in Pulsar project.

        } // END - if (sensor_shtc3_status)

        if (sensor_veml7700_status) // don't try to read the sensor once its marked bad.  The flag will be reset when Feather is reset/power-cycled.
        {
            /*
             * ----------------------------------
             * Operate the Ambient Light Sensor
             * ----------------------------------
             */
            // debugPrintln("case_read_sensors() - AmbientLight");
            uint16_t veml_als = veml.readALS();
            Serial.print("raw ALS: ");
            Serial.println(veml_als);

            uint16_t veml_white = veml.readWhite();
            Serial.print("raw white: ");
            Serial.println(veml_white);

            float veml_lux = veml.readLux();
            Serial.print("lux: ");
            Serial.println(veml_lux);
            myFmxSettings.FMX_AMBIENTLIGHT = veml_lux; // value in lux


        } // END - if (sensor_ambientlight_status)

        debugPrintln("case_read_sensors() - done");

    } // END -  if (readSenorsFlag)
}