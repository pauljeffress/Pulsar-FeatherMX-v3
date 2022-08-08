/*
 * RTC_Adafruit_fns.cpp
 * 
 * Functions that use the external DS3231 RTC module over I2C.
 * 
 * 
 */


#include "RTC_Adafruit_fns.h"

// Globals
RTC_DS3231 rtc;
bool RTC_status = false;  
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


// Functions

/*
 * Attempt to initialise the rtc object, and also check if it has lost power previously.
 * Then set global var RTC_status accordingly.
 */
void RTCSetup(TwoWire *wireInstance)
{
    // Not sure if we are assuming that the requested Wire port has been "Wire.begin()" already, perhaps in setup()
    
    Serial.println("RTCSetup() - Starting.");
    if (! rtc.begin(wireInstance)) 
    {
        Serial.println("RTCSetup() - ERROR - Couldn't find RTC");
        RTC_status = false;
        delay(5000);
    }
    else
    {
        Serial.println("RTCSetup() - RTC found ok");
        if (rtc.lostPower()) 
        {
            Serial.println("RTCSetup() - ERROR - RTC lost power!");
            RTC_status = false;
            delay(5000);
        }
        else
        {
            Serial.println("RTCSetup() - RTC time ok");
            RTCPrintCurrentTime();
            RTC_status = true;
        }
    }
    Serial.println("RTCSetup() - Complete.");
}   // END - RTCsetup()

/*
 * Read time from RTC and print it to Serial.
 * 
 */
void RTCPrintCurrentTime()
{
    Serial.print("RTCPrintCurrentTime() - ");
    
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    
    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print(" seconds OR ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println(" days");

}   // END - RTCPrintCurrentTime()


