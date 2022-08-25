/*
 * misc_fns.ino
 *
 * Assorted functions
 *
 *
 */

#include "global.h"

long lastsec1 = 0;    // counter for my every Xsec debug prints.


void setupPins()
{
    debugPrintln("setupPins()");

    pinMode(LED_BUILTIN, OUTPUT); // Make the LED pin an output

    // setup the 12v switched power output GPIO control pins.
    pinMode(PI_PWR_PIN, OUTPUT);
    digitalWrite(PI_PWR_PIN, LOW);
    pinMode(STROBE_LIGHT_PWR_PIN, OUTPUT);
    digitalWrite(STROBE_LIGHT_PWR_PIN, LOW);
    pinMode(POWER_FEATHER_PWR_PIN, OUTPUT);
    digitalWrite(POWER_FEATHER_PWR_PIN, LOW);

    gpsOFF();
}

// Because Arduino print can't handle uint64_t I found this here https://forum.arduino.cc/t/printing-uint64_t/364646
// and it works well.
// Use it as follows;
//    Serial.print(my64toString(<your uint64_t variable>));
String my64toString(uint64_t x)
{
    boolean flag = false; // For preventing string return like this 0000123, with a lot of zeros in front.
    String str = "";      // Start with an empty string.
    uint64_t y = 10000000000000000000;
    int res;
    if (x == 0) // if x = 0 and this is not testet, then function return a empty string.
    {
        str = "0";
        return str; // or return "0";
    }
    while (y > 0)
    {
        res = (int)(x / y);
        if (res > 0) // Wait for res > 0, then start adding to string.
            flag = true;
        if (flag == true)
            str = str + String(res);
        x = x - (y * (uint64_t)res); // Subtract res times * y from x
        y = y / 10;                  // Reducer y with 10
    }
    return str;
}

void loopHousekeeping()
{
    // Toggle LED to show we are running.
    digitalWrite(LED_BUILTIN, millis() % 500 > 250);

    // temporarily required until my 1sec Timer ISR is working
    // determines if 1sec has passed (roughly) and if so increments my seconds counters
    oneSecCounter = millis();
    if (oneSecCounter > (oneSecCounter_last + 1000))
    {
        unsigned long numSecsPassed = (oneSecCounter - oneSecCounter_last) / 1000; // how many whole seconds have passed
        oneSecCounter_last = oneSecCounter;
        for (unsigned long i = 0; i < numSecsPassed; i++) // call my incrementer once for each second that has passed.
            timerCounterIncrementer();
    }

    // print a seconds count every X secs when idling around this while loop....makes visual troubleshooting easier :)
    if (seconds_since_reset_or_powercycle > (lastsec1 + 59))
    {
        lastsec1 = seconds_since_reset_or_powercycle;
        Serial.print("loop() - seconds_since_reset_or_powercycle=");
        Serial.println(seconds_since_reset_or_powercycle);
        oled.print(" ");
        oled.print(seconds_since_reset_or_powercycle);
        oled.print("s ");
    }
}