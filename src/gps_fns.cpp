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
 *  If successful, it places the fix data into myFmxSettings.XXX fields.
 *
 *  If it fails to initialise the GPS OR get a suitable quality fix before
 *  timing out, it leaves all GPS parameters as their defaults.  This can be easily identified
 *  on the ground, as an unsuccessful attempt to get a fix.
 */

// Global Variables
SFE_UBLOX_GNSS myGNSS; // Declare the uBlox GPS object

void gpsON() // Enable power for the gps
{
    pinMode(GPS_PWR_ENA_PIN, OUTPUT);
    digitalWrite(GPS_PWR_ENA_PIN, HIGH); // Enable gps power (HIGH = enable; LOW = disable)
}

void gpsOFF() // Disable power for the gps
{
    pinMode(GPS_PWR_ENA_PIN, OUTPUT);
    digitalWrite(GPS_PWR_ENA_PIN, LOW); // Disable gps power (HIGH = enable; LOW = disable)
}

void print_gps_data()
{
    debugPrintln("print_gps_data() - Here is whats currently in myFmxSettings");
    debugPrintln("============================================================");
    debugPrintln("=              myFmxSettings all GPS data                  =");
    debugPrintln("============================================================");
    Serial.print("myFmxSettings.FMX_GPS_STATUS:");
    Serial.println(myFmxSettings.FMX_GPS_STATUS);
    Serial.print("myFmxSettings.FMX_GPS_FIX:");
    Serial.println(myFmxSettings.FMX_GPS_FIX);
    Serial.print("myFmxSettings.FMX_GPS_SIV:");
    Serial.println(myFmxSettings.FMX_GPS_SIV);
    Serial.print("myFmxSettings.FMX_GPS_POSITIONTIMESTAMP:");
    Serial.println(myFmxSettings.FMX_GPS_POSITIONTIMESTAMP);
    Serial.print("myFmxSettings.FMX_GPS_LAT:");
    Serial.println(myFmxSettings.FMX_GPS_LAT);
    Serial.print("myFmxSettings.FMX_GPS_LON:");
    Serial.println(myFmxSettings.FMX_GPS_LON);
    Serial.print("myFmxSettings.FMX_GPS_ALT:");
    Serial.println(myFmxSettings.FMX_GPS_ALT);
    Serial.print("myFmxSettings.FMX_GPS_GSPEED:");
    Serial.println(myFmxSettings.FMX_GPS_GSPEED);
    Serial.print("myFmxSettings.FMX_GPS_HEAD_MOTION:");
    Serial.println(myFmxSettings.FMX_GPS_HEAD_MOTION);
    Serial.print("myFmxSettings.FMX_GPS_PDOP:");
    Serial.println(myFmxSettings.FMX_GPS_PDOP);
    debugPrintln("============================================================");
}

void gpsSetup()
{
    debugPrintln("gpsSetup() - NOT CODED YET!!!!!");
    gpsOFF();
    delay(5000);

    // xxx - we should be preparing the gps data with probably the default values??????
}

void case_read_gps()
{
    uint8_t numGPSattempts = 0;          // count attempts to get a GPS reading.
    bool gpsResult = false;              // controls execution
    uint8_t fixType = 0;                 // type of fix the GPS is able to get.
    uint8_t sivCount = 0;                // number of Satellites In View (SIV)
    bool timedOut = false;               // track when we have timed out.
    uint32_t start_ms = 0;               // milliseconds - store start time from millis() at start of timeout period.
    bool flag_gps_fix_succeeded = false; // clear the flag before we attempt to get a fix below.

    if (readGPSFlag)
    {
        readGPSFlag = false; // clear the flag as we are actioning it now.
        myFmxSettings.FMX_GPS_STATUS = FMX_GPS_STATUS_UNKNOWN;
        mp3.playFile(6); // Play F006.mp3 "Read GPS"

#ifdef SKIP_GPS_FIX // i.e we want to skip getting a GPS fix
        debugPrintln("case_read_GPS() - SKIP_GPS_FIX is defined - so we are skipping over it.");
        oled.println("SKIP_GPS_FIX set");

#else // i.e. don't skip, we want to get a GPS fix
        debugPrintln("\ncase_read_GPS() - Starting");
        oled.println("read_GPS()");
        pixelBlue(); // set Pixel to Blue for GPS activity.

        /*
         *  Power up the GPS module
         */
        gpsON(); // Enable power for the gps
        // debugPrintln("case_read_GPS() - Powering up the DONE.");

        delay(GPS_POWERUP_TIMEOUT_S * 1000UL); // Give the GPS time to power up.
        // debugPrintln("case_read_GPS() - Powering up delay has passed.");

        while ((!flag_gps_fix_succeeded) && (numGPSattempts < GPS_MAX_TRIES)) // control how many times we try to get a reading before moving on.
        {
            numGPSattempts++; // increment attempts counter.
            fixType = 0;    // reset it for this attempt.
            sivCount = 0;   // reset it for this attempt.
            debugPrint("case_read_GPS() - Try: ");
            debugPrintInt(numGPSattempts);
            debugPrint(" of ");
            debugPrintlnInt(GPS_MAX_TRIES);
            oled.print("Try:");
            oled.print(numGPSattempts);
            /*
             *  Initialise the GPS module (remember we just powered it up)
             */
            gpsResult = myGNSS.begin(); // Connect to the Ublox module using Wire port, remember we have just powered up the GPS unit.
                                        // This command executed once per "try" so its explicitly inside the while() so it gets tried multiple times.

            if (gpsResult == false) // If we were unable to connect to the GPS over I2C...
            {
                // Send a warning message
                debugPrintln("case_read_GPS() - ERROR ***!!! Ublox GPS not detected at default I2C address !!!***");
                oled.print(" I2C err");
                myFmxSettings.FMX_GPS_STATUS = FMX_GPS_STATUS_INIT_FAILED;
                myFmxSettings.FMX_GPS_FIX = DEF_FMX_GPS_FIX; // tag the FIX as DEF_FMX_GPS_FIX so we know a fix was not achieved when we see this value at Ground Station.
            }
            else // If the GPS started up OK
            {
                debugPrintln("case_read_GPS() - GPS started up OK");
                myFmxSettings.FMX_GPS_STATUS = FMX_GPS_STATUS_INIT_OK;

#ifdef ENABLE_GPS_DEBUG
                debugPrintln("case_read_GPS() - enabling GPS debugging");
                myGNSS.enableDebugging(); // Enable debug messages
                // xxx - do I want to turn this debugging off at some point?
#endif

                /*
                 * Configure GPS (remember we just powered it up)
                 */
                myGNSS.setI2COutput(COM_TYPE_UBX);                 // Set the I2C port to output UBX only (turn off NMEA noise)
                myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); // Save (only) the communications port settings to flash and BBR
                myGNSS.setDynamicModel(dynModel(5));               // xxx - change this to say SEA not dynModel(5)
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
                debugPrint("case_read_GPS() - Waiting for a GPS fix. Timeout = ");
                debugPrintInt(GPS_FIX_TIMEOUT_S);
                debugPrintln("s");
                timedOut = false;
                start_ms = millis(); // record start time.
                // debugPrint("case_read_GPS() - start(mS)=");debugPrintlnInt(start);
                while (!timedOut && (fixType != GPS_MIN_FIX_TYPE)) // do this until we either get a suitable quality fix or we time out.
                {
                    // Flash the onboard NeoPixel at 1 Hz
                    if ((millis() / 2000) % 2 == 1)
                        pixelBlue();
                    else
                        pixelOff();

                    /*
                     *  Attempt to get a GPS fix.
                     */
                    sivCount = myGNSS.getSIV(); // How many Satellites In View (SIV) at the moment?
                    debugPrint("case_read_gps() - SIV: ");
                    debugPrintInt(sivCount);
                    oled.print("SIV:");
                    oled.print(sivCount);

                    fixType = myGNSS.getFixType(); // What type of fix is available from the GPS at the moment?
                    debugPrint(" fixType: ");
                    debugPrintInt(fixType);
                    debugPrint(", ");
                    if (fixType == 0)
                        Serial.println(F("No fix"));
                    else if (fixType == 1)
                        Serial.println(F("Dead reckoning"));
                    else if (fixType == 2)
                        Serial.println(F("2D"));
                    else if (fixType == 3)
                        Serial.println(F("3D"));
                    else if (fixType == 4)
                        Serial.println(F("GNSS + Dead reckoning"));
                    else if (fixType == 5)
                        Serial.println(F("Time only"));
                    else
                        Serial.println(F("WARNING NOT EXPECTED FIX TYPE"));

                    oled.print("Fix:");
                    if (fixType == 0)
                        oled.println("No fix");
                    else if (fixType == 1)
                        oled.println("Dead reckoning");
                    else if (fixType == 2)
                        oled.println("2D");
                    else if (fixType == 3)
                        oled.println("3D");
                    else if (fixType == 4)
                        oled.println("GNSS + Dead reckoning");
                    else if (fixType == 5)
                        oled.println("Time only");
                    else
                        oled.println("BAD NUM");

                    // Check if we have timed out, which will be evaluated at next pass through the while()
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
                if (fixType == GPS_MIN_FIX_TYPE)
                {
                    debugPrintln("case_read_GPS() - A suitable quality fix was found!");
                    myFmxSettings.FMX_GPS_STATUS = FMX_GPS_STATUS_FIX_OK;
                    oled.print("FIX ok ");
                    mp3.playFile(8); // "Received a GPS fix"
                    // Flash pixel Blue (for ISBD) and Green (for success) slowly for 10 secs to indicate successful GPS Fix
                    for (int i = 0; i < 5; i++)
                    {
                        pixelBlue();
                        delay(1000);
                        pixelGreen();
                        delay(1000);
                    }
                    pixelOff();
                    flag_gps_fix_succeeded = true; // set flag accordingly for use in subsequent states.

                    myFmxSettings.FMX_GPS_FIX = fixType;  // Store this data
                    myFmxSettings.FMX_GPS_SIV = sivCount; // Store this data
                    myFmxSettings.FMX_GPS_POSITIONTIMESTAMP = myGNSS.getUnixEpoch();
                    myFmxSettings.FMX_GPS_LAT = myGNSS.getLatitude();        // Get the latitude in degrees * 10^-7
                    myFmxSettings.FMX_GPS_LON = myGNSS.getLongitude();       // Get the longitude in degrees * 10^-7
                    myFmxSettings.FMX_GPS_ALT = myGNSS.getAltitudeMSL();     // Get the altitude above Mean Sea Level in mm
                    myFmxSettings.FMX_GPS_GSPEED = myGNSS.getGroundSpeed();  // Get the ground speed in mm/s
                    myFmxSettings.FMX_GPS_HEAD_MOTION = myGNSS.getHeading(); // Get the heading in degrees * 10^-7
                    myFmxSettings.FMX_GPS_PDOP = myGNSS.getPDOP();           // Get the PDOP in cm

                    print_gps_data();
                    seconds_since_last_gps_read = 0; // Read was successful so reset the timer.
                }
                else // We didn't get a suitable quality fix.
                {
                    debugPrintln("case_read_GPS() - WARNING A suitable fix was NOT found! Leaving values at last reading");
                    myFmxSettings.FMX_GPS_STATUS = FMX_GPS_STATUS_FIX_FAILED;
                    oled.println(" Fix:No/too low.");
                    mp3.playFile(7);                 // "WARNING, no GPS fix"
                    // Flash pixel Blue (for ISBD) and Red (for fail) slowly for 10 secs to indicate failure to get GPS Fix
                    for (int i = 0; i < 5; i++)
                    {
                        pixelBlue();
                        delay(1000);
                        pixelRed();
                        delay(1000);
                    }
                    pixelOff();
                    flag_gps_fix_succeeded = false;  // clear flag accordingly for use in subsequent states.
                    seconds_since_last_gps_read = 0; // xxx - I should not nesc be setting this back to zero in the event I could not get a position from GPS.
                                                     // Perhaps I should have a method to control multiple attempts, then give up til next major interval (don't want to consume battery on a failing GPS...)
                }

            } // END of the "else // If the GPS started up OK"

        } // END - while((!flag_gps_fix_succeeded) && (numGPSattempts < GPS_MAX_TRIES))

        /*
         * Power down the gps
         */
        debugPrintln("case_read_GPS() - Powering down the GPS");
        gpsOFF(); // Disable power for the gps

        pixelOff(); // cleanup pixel at end of state. It may have been left on during cycles above.

        seconds_since_last_gps_read = 0; // Read was ATTEMPTED so reset the timer.

        debugPrintln("case_read_GPS() - Complete");

#endif // of the #else of #ifdef SKIP_GPS_FIX

    } // END - if (readGPSFlag)

} // END - case_read_GPS()

// END of file gps_fns.cpp
