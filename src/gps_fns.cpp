/*
 * gps_fns.cpp
 * 
 * All GPS related code
 * 
 * 
 */

#include "global.h"
#include "gps_fns.h"

// ************************************************************************************************
// Operate the onboard GPS
/*
 *  case_read_GPS() - enables power to the GPS module tries to initialise it and then get a fix.
 *  It has a set number of attemps, GPS_MAX_TRIES and timeouts.
 *  
 *  If successful, it places the fix data into myAgtSettings.XXX fields.
 *  
 *  If it fails to initialise the GPS OR get a suitable quality fix before
 *  timing out, it leaves all GPS parameters as their defaults.  This can be easily identified
 *  on the ground, as an unsuccessful attempt to get a fix.
 */

//  DEBUGGING NOTES
// #define SKIP_GPS_FIX         // Uncomment to skip actually powering up and using the GPS
// #define ENABLE_GPS_DEBUG     // Uncomment to ENABLE the GNSS library internal debugs.

#define GPS_PWR_ENA_PIN 16          // Pin that enables the GPS' dedicated 3.3v regulator sub module
#define GPS_POWERUP_TIMEOUT_S 5UL   // seconds - Timeout after this many _seconds_ when waiting
                                    //            for GPS to power up after power is enabled.
#define GPS_FIX_TIMEOUT_S 60UL      // seconds - Timeout after this many _minutes_ when waiting for a 3D GPS fix
#define GPS_MAX_TRIES 3             // When we try to get a fix, how many times do we try before failing and moving on.
//#define GPS_LAST_READ_PERIOD 3UL    // minutes - How recently do we consider it ok not to do another fresh read?
#define GPS_MIN_FIX_TYPE 3          // Minimum Fix Type (2D, 3D etc) for AGT onboard GPS to consider it got a usable fix.


SFE_UBLOX_GNSS myGNSS; // Declare the uBlox GPS object



void gpsON() // Enable power for the gps
{
    pinMode(GPS_PWR_ENA_PIN, OUTPUT);
    digitalWrite(GPS_PWR_ENA_PIN, HIGH); // Enable gps power (HIGH = enable; LOW = disable)
}

void gpsOFF() // Disable power for the gps
{
    pinMode(GPS_PWR_ENA_PIN, OUTPUT);
    digitalWrite(GPS_PWR_ENA_PIN, LOW);   // Disable gps power (HIGH = enable; LOW = disable)
}

void print_gps_data()
{
    debugPrintln("print_gps_data() - Here is whats currently in myAgtSettings");
    debugPrint("Time: ");
    // debugPrintInt(myAgtSettings.HOUR);debugPrint("h");
    // debugPrintInt(myAgtSettings.MIN);debugPrint("m");
    // debugPrintInt(myAgtSettings.SEC);debugPrintln("s");
    // oled.print("Time: ");
    // oled.print(myAgtSettings.HOUR);oled.print("h");
    // oled.print(myAgtSettings.MIN);oled.print("m");
    // oled.print(myAgtSettings.SEC);oled.println("s");

    // debugPrint("  Date: ");
    // debugPrintInt(myAgtSettings.DAY);debugPrint("/");
    // debugPrintInt(myAgtSettings.MONTH);debugPrint("/");
    // debugPrintlnInt(myAgtSettings.YEAR.the_data);
    // oled.print("  Date: ");
    // oled.print(myAgtSettings.DAY);oled.print("/");
    // oled.print(myAgtSettings.MONTH);oled.print("/");
    // oled.println(myAgtSettings.YEAR.the_data);

    // debugPrint("Lat: ");debugPrintlnInt(myAgtSettings.LAT.the_data);
    // debugPrint("Lon: ");debugPrintlnInt(myAgtSettings.LON.the_data);
    // debugPrint("Alt: ");debugPrintInt(myAgtSettings.ALT.the_data);debugPrintln("mm");
    // debugPrint("Gnd Spd: ");debugPrintInt(myAgtSettings.SPEED.the_data);debugPrintln("mm/s");
    // debugPrint("Num sats in fix: ");debugPrintInt(myAgtSettings.SATS);
    // debugPrint("Heading: ");debugPrintInt(myAgtSettings.HEAD.the_data);
    // oled.print("Spd:");oled.print(myAgtSettings.SPEED.the_data);oled.print("mm/s");
    // oled.print(" Sats:");oled.print(myAgtSettings.SATS);
    // oled.print(" Dir: ");oled.println(myAgtSettings.HEAD.the_data);

    // debugPrint("PDOP: ");debugPrintInt(myAgtSettings.PDOP.the_data);debugPrintln("cm");
}

void gpsSetup()
{
    debugPrintln("gpsSetup() - NOT CODED YET!!!!!");
    delay(5000);

    // xxx - we should be preparing the gps data with probably the defalt values??????

}

void case_read_gps()
{
    uint8_t numGPSattempts = 0;             // count attempts to get a GPS reading.
    bool    gpsResult = false;              // controls execution
    uint8_t fixType = 0;                   // type of fix the GPS is able to get.  
    uint8_t sivCount = 0;                  // number of Satellites In View (SIV)
    bool    timedOut = false;               // track when we have timed out.
    uint32_t start_ms = 0;                  // milliseconds - store start time from millis() at start of timeout period.
    bool    flag_gps_fix_succeeded = false; // clear the flag before we attempt to get a fix below.

    #ifdef SKIP_GPS_FIX  // i.e we want to skip getting a GPS fix
        debugPrintln("case_read_GPS() - SKIP_GPS_FIX is defined - so we are skipping over it.");
        oled.println("SKIP_GPS_FIX set");
    
    #else //i.e. don't skip, we want to get a GPS fix 
        debugPrintln("\ncase_read_GPS() - Starting");
        oled.println("read_GPS()");
        
        /*
        *  Power up the GPS module
        */
        gpsON(); // Enable power for the gps
        //debugPrintln("case_read_GPS() - Powering up the DONE.");
        
        delay(GPS_POWERUP_TIMEOUT_S * 1000UL); // Give the GPS time to power up.
        //debugPrintln("case_read_GPS() - Powering up delay has passed.");

        while((!flag_gps_fix_succeeded) && (numGPSattempts < GPS_MAX_TRIES)) // control how many times we try to get a reading before moving on.
        {
            numGPSattempts++; // increment attempts counter.
            debugPrint("case_read_GPS() - Try: ");debugPrintInt(numGPSattempts);debugPrint(" of ");debugPrintlnInt(GPS_MAX_TRIES);
            oled.print("Try:");oled.print(numGPSattempts);
            /*
            *  Initialise the GPS module (remember we just powered it up)
            */
            gpsResult = myGNSS.begin();  // Connect to the Ublox module using Wire port, remember we have just powered up the GPS unit.
                                         // This command executed once per "try" so its explicitly inside the while() so it gets tried multiple times.

            if (gpsResult == false) // If we were unable to connect to the GPS over I2C...
            {
                // Send a warning message
                debugPrintln("case_read_GPS() - ERROR ***!!! Ublox GPS not detected at default I2C address !!!***");
                oled.print(" I2C err");
                //myAgtSettings.FIX = DEF_FIX;  // tag the FIX as DEF_FIX so we know a fix was not achieved.
            }
            else // If the GPS started up OK
            {
                debugPrintln("case_read_GPS() - GPS started up OK");
        
                #ifdef ENABLE_GPS_DEBUG
                    debugPrintln("case_read_GPS() - enabling GPS debugging");
                    myGNSS.enableDebugging(); // Enable debug messages
                    // xxx - do I want to turn this debugging off at some point?
                #endif

                /*
                * Configure GPS (remember we just powered it up)
                */
                myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
                myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR
                myGNSS.setDynamicModel(dynModel(5));    // xxx - change this to say SEA not dynModel(5)
                // Set the dynamic platform model.
                // Possible values are: 0(PORTABLE),2(STATIONARY),3(PEDESTRIAN),4(AUTOMOTIVE),5(SEA),6(AIRBORNE1g),7(AIRBORNE2g),8(AIRBORNE4g),9(WRIST),10(BIKE)
                // Setting it correctly improves the receivers interpretation of measurements.
                // https://www.u-blox.com/sites/default/files/products/documents/u-blox8-M8_ReceiverDescrProtSpec_UBX-13003221.pdf
                // In our case we are setting it to SEA, which assumes zero vertical velocity and the device is at sea level.

                

                /*
                * Try to get a fix from the GPS until either;
                *                       1. we get one of suitable quality
                *                       OR
                *                       2. we time out for this particular try.
                */
                debugPrint("case_read_GPS() - Waiting for a GPS fix. Timeout = ");debugPrintInt(GPS_FIX_TIMEOUT_S);debugPrintln("s");
                timedOut = false;
                start_ms = millis();   // record start time.
                //debugPrint("case_read_GPS() - start(mS)=");debugPrintlnInt(start);
                while(!timedOut && (fixType < GPS_MIN_FIX_TYPE))  // do this until we either get a suitable quality fix or we time out.
                {
                    // Flash the BUILTIN LED at 1Hz inside this for loop
                    if ((millis() / 1000) % 2 == 1) digitalWrite(LED_BUILTIN, HIGH);
                    else digitalWrite(LED_BUILTIN, LOW);
                    
                    /*
                     *  Attempt to get a GPS fix.
                     */
                    sivCount = myGNSS.getSIV();    // How many Satelites In View (SIV) at the moment?
                    debugPrint("case_read_gps() - SIV: ");
                    debugPrintInt(sivCount);
                    oled.print("SIV:");
                    oled.print(sivCount);

                    fixType = myGNSS.getFixType(); // What type of fix is available from the GPS at the moment?
                    debugPrint(" Fix: ");
                    if(fixType == 0) Serial.println(F("No fix"));
                    else if(fixType == 1) Serial.println(F("Dead reckoning"));
                    else if(fixType == 2) Serial.println(F("2D"));
                    else if(fixType == 3) Serial.println(F("3D"));
                    else if(fixType == 4) Serial.println(F("GNSS + Dead reckoning"));
                    else if(fixType == 5) Serial.println(F("Time only"));
                    oled.print("Fix:");
                    if(fixType == 0) oled.println("No fix");
                    else if(fixType == 1) oled.println("Dead reckoning");
                    else if(fixType == 2) oled.println("2D");
                    else if(fixType == 3) oled.println("3D");
                    else if(fixType == 4) oled.println("GNSS + Dead reckoning");
                    else if(fixType == 5) oled.println("Time only");


                    //Check if we have timed out, which will be evaluated at next pass through the while()
                    if ((millis() - start_ms) >= (GPS_FIX_TIMEOUT_S * 1000UL))
                    {
                        timedOut = true;
                        debugPrintln("case_read_GPS() - WARNING we timed out before receiving a suitable quality fix");
                        oled.print(" timeout.");
                    }

                    delay(1000); // Delay for Xmsec so we don't pound the I2C bus too hard!
                }

                /*
                 *  The above GPS read try has either timed out or got a decent fixType.
                 */


                /*
                *  If we got a suitable fix type above then quickly use the GPS to get all the time and position data from the GNSS constellation.
                */
                if (fixType >= GPS_MIN_FIX_TYPE) 
                {
                    debugPrintln("case_read_GPS() - A suitable quality fix was found!");
                    oled.print("FIX ok ");

                    flag_gps_fix_succeeded = true;  // set flag accordingly for use in subsequent states.

                    // myAgtSettings.FIX = fixType;    // Store this data
                    // myAgtSettings.SATS = sivCount;  // Store this data
                    // // Get the time and position etc.
                    // // Get the time first to hopefully avoid second roll-over problems
                    // // myAgtSettings.MILLIS.the_data = myGNSS.getMillisecond();
                    // myAgtSettings.SEC = myGNSS.getSecond();
                    // myAgtSettings.MIN = myGNSS.getMinute();
                    // myAgtSettings.HOUR = myGNSS.getHour();
                    // myAgtSettings.DAY = myGNSS.getDay();
                    // myAgtSettings.MONTH = myGNSS.getMonth();
                    // myAgtSettings.YEAR.the_data = myGNSS.getYear(); // Get the year
                    // myAgtSettings.LAT.the_data = myGNSS.getLatitude(); // Get the latitude in degrees * 10^-7
                    // myAgtSettings.LON.the_data = myGNSS.getLongitude(); // Get the longitude in degrees * 10^-7
                    // myAgtSettings.ALT.the_data = myGNSS.getAltitudeMSL(); // Get the altitude above Mean Sea Level in mm
                    // myAgtSettings.SPEED.the_data = myGNSS.getGroundSpeed(); // Get the ground speed in mm/s
                    // myAgtSettings.HEAD.the_data = myGNSS.getHeading(); // Get the heading in degrees * 10^-7
                    // myAgtSettings.PDOP.the_data = myGNSS.getPDOP(); // Get the PDOP in cm

                    print_gps_data();
                
                    seconds_since_last_gps_read = 0;  // Read was successful so reset the timer.
                }
                else  // We didn't get a suitable quality fix.
                {
                    debugPrintln("case_read_GPS() - WARNING A suitable fix was NOT found! Leaving values at last reading");
                    oled.println(" Fix:No/too low.");
                    flag_gps_fix_succeeded = false;  // clear flag accordingly for use in subsequent states.
                    seconds_since_last_gps_read = 0; // xxx - I should not nesc be setting this back to zero in the event I could not get a position from GPS.  
                                                    // Perhaps I should have a method to control multiple attempts, then give up til next major interval (don't want to consume battery on a failing GPS...) 
                }
          
            }  // END of the "else // If the GPS started up OK"
        
        }  // END - while((!flag_gps_fix_succeeded) && (numGPSattempts < GPS_MAX_TRIES))

        /*
        * Power down the gps
        */
        debugPrintln("case_read_GPS() - Powering down the GPS");
        gpsOFF(); // Disable power for the gps

        seconds_since_last_gps_read = 0;  // Read was ATTEMPTED so reset the timer.

        debugPrintln("case_read_GPS() - Complete");

    #endif  // of the #else of #ifdef SKIP_GPS_FIX

} // END - case_read_GPS()





// END of file gps_fns.cpp
