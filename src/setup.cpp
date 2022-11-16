/*
 * setup.ino
 */

#include "global.h"

#include "setup.h"

/*
 * setupPins() for the FMX
 */
void setupPins()
{
    debugPrintln("setupPins()");

    pinMode(LED_BUILTIN, OUTPUT); // Make the LED pin an output

    DIPswSetup();

    // setup Iridium 9603N Modem power control pin
    pinMode(IRIDIUM_PWR_FET_PIN, OUTPUT);
    digitalWrite(IRIDIUM_PWR_FET_PIN, LOW);  // start with it off.

    // setup the 12v switched power output GPIO control pins.
    pinMode(PI_PWR_PIN, OUTPUT);
    digitalWrite(PI_PWR_PIN, LOW);
    pinMode(STROBE_LIGHT_PWR_PIN, OUTPUT);
    digitalWrite(STROBE_LIGHT_PWR_PIN, LOW);
    pinMode(POWER_FEATHER_PWR_PIN, OUTPUT);
    digitalWrite(POWER_FEATHER_PWR_PIN, LOW);

    gpsOFF();
}


/*============================
 * setup()
 *
 * Note: setup() code only runs when CPU has done a real powerup (or HW RESET), NOT a wake from sleep.
 ============================*/
void setup()
{
    setupPins(); // initialise all GPIOs

    Wire.begin();   // get I2C port ready for subsequent users (OLED, RTC etc)

    disableDebugging(); // Make sure the serial debug messages are disabled until the Serial port is open!
    disableLogging();   // Make sure the serial logging messages (to OLA) are disabled until the Serial port is open! 

    for (int x = 0; x < 5; x++) // Flash LED & give me time to get Serial Monitor open if need be.
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
    }

    serialSetup();      // Setup all Serial ports   
    enableDebugging(Serial);
    enableLogging(Serial3);

    debugPrintln("================================================");
    debugPrintln("************************************************");
    debugPrintln("================================================");
    debugPrintln("        Pulsar FeatherMx Reset/PowerOn          ");   

    logPrintln("================================================");
    logPrintln("************************************************");
    logPrintln("================================================");
    logPrintln("        Pulsar FeatherMx Reset/PowerOn          "); 

    DIPswCheck();    // check status of DIP switches and set appropriate flags.

    pixelSetup();   // prep the NeoPixel on the Feather.
    pixelYellow();  // set onboard NeoPixel to Yellow

    OLEDMiniSetup();
    oled.println("    ---  FMX   ---");
    oled.println("setup() - underway");

    MP3Setup();     // Needs to be after Serial and debugs are enabled, and Wire.begin()

    setupFmxSettings();     // initialise FmxSetting from defaults or EEPROM stored values if present.

    initTFTFeatherInternalSettings();
    initPowerFeatherSettings();
    initMAVLinkSettings();  

    RTCSetup(&Wire);

    myCANid = CBP_CANID_FEATHERMX; // Set myCANid based on defines in CBP.h
    CANStatus = CANSetup();  

    timerCounterSetup();  

    actuatorsSetup();  

    sensorsSetup(); 

    gpsSetup();

    ISBDSetup();

    mavlink_unrequest_streaming_params_from_ap(&Serial2); // I am trying to initially hush the AutoPilot (see my other mavlink stuff for explanation)   
    
    justBootedFlag = true;
    main_state = THINK;     // Ensure main state machine starts at correct first step.

    mp3.playFile(3); delay(2000); //Play F003.mp3 "Pulsar Initialisation Complete"
    oled.println("setup() - Complete");
    pixelOff();

    debugPrintln("setup() - Complete");
    logPrintln("setup() - Complete");
} // END - setup()