/*
 * case_read_sensors.cpp
 * 
 */

#include "global.h"
#include "sensor_fns.h"

void case_read_sensors()
{
    // is it time to do a routine sensor read?
    if (seconds_since_last_sensors_read > FMX_CHECKSENSORSPERIODSECONDS)
    {
        seconds_since_last_sensors_read = 0;    // Reset this timer.
        
        debugPrintln("case_read_sensors() - time to execute");
        mp3.playFile(4); //Play F004.mp3 "Read sensors"
        //oled.println("read_sensors()");

        if (sensor_shtc3_status)    // don't try to read the sensor once its marked bad.  The flag will be reset when Feather is reset/power-cycled.
        {
            /*
             * ----------------------------------
             * Operate the SHTC3 Temp & Humidity Sensor 
             * ----------------------------------
             */

            // TODO - are we supposed to turn on the SHT31's heater before taking a reading????

            debugPrintln("case_read_sensors() - SHTC3");

            shtc3.getEvent(&shtc3_humidity, &shtc3_temp);// populate temp and humidity objects with fresh data
  
            Serial.print("Temperature: "); Serial.print(shtc3_temp.temperature); Serial.println(" degrees C");
            Serial.print("Humidity: "); Serial.print(shtc3_humidity.relative_humidity); Serial.println("% rH");

            myFmxSettings.FMX_TEMP = shtc3_temp.temperature * 100; // * 100 to store as correct units in Pulsar project.


            myFmxSettings.FMX_RH = shtc3_humidity.relative_humidity * 100; // * 100 to store as correct units in Pulsar project.


        }   // END - if (sensor_shtc3_status)



        if (sensor_veml7700_status)    // don't try to read the sensor once its marked bad.  The flag will be reset when Feather is reset/power-cycled.
        {
            /*
             * ----------------------------------
             * Operate the Ambient Light Sensor 
             * ----------------------------------
             */
            //debugPrintln("case_read_sensors() - AmbientLight");
            uint16_t veml_als = veml.readALS();
            Serial.print("raw ALS: "); Serial.println(veml_als);

            uint16_t veml_white = veml.readWhite();
            Serial.print("raw white: "); Serial.println(veml_white);

            float veml_lux = veml.readLux();
            Serial.print("lux: "); Serial.println(veml_lux);

        }   // END - if (sensor_ambientlight_status)        
        
        debugPrintln("case_read_sensors() - finishing");
        //oled.println("case_read_sensors() - finishing");
    
    }
    else  // is it time to read sensors?
    {
        //debugPrintln(" - NOT NOW");
    }

    //debugPrintln("case_read_sensors() - done");
}