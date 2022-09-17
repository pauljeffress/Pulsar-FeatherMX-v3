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
    pixelSetup();   // prep the NeoPixel on the Feather.
    pixelYellow();

    delay(5000); // Give me time to get Serial Monitor open if need be.     xxx
    
    Wire.begin();   // get I2C port ready for subsequent users (OLED, RTC etc)

    myCANid = CBP_CANID_FEATHERMX; // Set myCANid based on defines in CBP.h

    setupPins(); // initialise all GPIOs    
    
    OLEDMiniSetup();
    oled.println("    ---  FMX   ---");
    oled.println("setup() - underway");

    for (int x = 0; x < 5; x++)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
    }
    
    disableDebugging(); // Make sure the serial debug messages are disabled until the Serial port is open ( see loop_init() )!
    disableLogging(); // Make sure the serial logging messages (to OLA) are disabled until the Serial port is open ( see loop_init() )! 

    serialSetup();  // Setup all Serial ports   

    enableDebugging(Serial); // THIS LINE IS RIGHT HERE FOR A REASON. Because we re issue Serial.begin() for the console/debug port
                             // just a few lines earlier here in serialSetup(), you need to enable the debug stuff that uses it
                             // after you have done that.  For more info see case_zzz(), and you will see it explicitly does a
                             // Serial.end(); before putting the processor to sleep.
                             // Uncomment this line to enable extra debug messages to Serial    


    MP3Setup();     // Needs to be after Serial and debugs are enabled, and Wire.begin()

    debugPrintln("================================================");
    debugPrintln("************************************************");
    debugPrintln("================================================");
    debugPrintln("        Pulsar FeatherMx Reset/PowerOn          ");   
    enableLogging(Serial3); // THIS LINE IS RIGHT HERE FOR A REASON. See explanation above for enableDebugging(Serial);

    
    logPrintln("================================================");
    logPrintln("************************************************");
    logPrintln("================================================");
    logPrintln("        Pulsar FeatherMx Reset/PowerOn          "); 
    
    
    setupFmxSettings();     // initialise FmxSetting from defaults or EEPROM stored values if present.



    initTFTFeatherInternalSettings();
    initPowerFeatherSettings();
    initMAVLinkSettings();  

    RTCSetup(&Wire);
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
} // END - setup()