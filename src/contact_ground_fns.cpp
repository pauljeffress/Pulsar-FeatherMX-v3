/*
 * contact_ground_fns.cpp
 *
 * Code to execute when communicating with the Ground Station
 *
 *
 */

#include "global.h"
#include "contact_ground_fns.h"

// Globals

uint8_t isbdNumWaitingMessages  = 1;        // How many inbound ISBD MT messages are waiting to be RX'd by the boat.
uint8_t isbdCountTxRxLaps       = 0;        // Count how many times in this current session we have ISBD TX/RX'ed so we can break out if we hit limit. i.e. potential problem.
bool    isbdSendSuccessful      = false;    // did ISBD TX succeed?
uint8_t isbdNumTxAttempts        = 0;        // how many ISBD TX attempts have we had this time?
bool    isbdFirstTx             = true;     // Send the text/binary message the first time, by on TX NULL messages thereafter when RX'ing in any remaining mesages waiting for us.
bool    isbdValidRx             = false;    // Is the received Iridium message valid?
bool    isbdUnvalidatedRx       = false;    // Have we received an Iridium message, that we haven't validated yet?

// Functions

uint16_t getNextISBDMoNum()
{
    uint16_t magicNum = myFmxSettings.FMX_ISBD_MO_MAGICNUMBER;
    myFmxSettings.FMX_ISBD_MO_MAGICNUMBER++;    // increment it ready for next use.  Will rollover to 0 automatically
    putFmxSettings(); // Save newly incremented number
    return (magicNum);
}   // END - getNextISBDMoNum()




void case_contact_ground()
{
    if (contactGroundFlag)
    {
        debugPrintln("\nX X X X X X X X X X X X X X");
        debugPrintln("contact_ground() - Starting");
        debugPrintln("X X X X X X X X X X X X X X");
        oled.print("contact_ground()");
        if(flag_bypass_iridium) // print debugs/oled for BYPASS mode
        {
            debugPrintln("contact_ground() - Iridium BYPASS mode (using SERIAL TEST LINK)");
            oled.println("w/BYPASS");
        }
        else // print debugs/oled for using Iridium mode
        {
            debugPrintln("contact_ground() - Iridium real TX/RX mode");
            oled.println("w/ISBD");
        }
        
        // prep/update any additional variables, just before we send a message to ground.
        myFmxSettings.FMX_UPTIME_S = seconds_since_reset_or_powercycle;


        do_iridium_locarb();    // do/attempt the whole ISBD TX/RX's

        seconds_since_last_iridium_tx = 0; // reset the counter.       xxx - should we have two counters, one for tx_attemps and one for tx_success

        pixelOff(); // cleanup pixel at end of state. It may have been left on by the ISBDCallback functions.

        debugPrintln("X X X X X X X X X X X X X X");
        debugPrintln("contact_ground() - Complete");
        debugPrintln("X X X X X X X X X X X X X X");
    }
} // END - case_contact_ground()

// END - contact_ground_fns.cpp
