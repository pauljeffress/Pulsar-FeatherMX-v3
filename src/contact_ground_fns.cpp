/*
 * contact_ground_fns.cpp
 *
 * Code to execute when communicating with the Ground Station
 *
 *
 */

#include "global.h"
#include "contact_ground_fns.h"


uint8_t num_waiting_messages = 1;     // How many inbound ISBD MT messages are waiting to be RX'd by the boat.
uint8_t countTXRXlaps = 0;            // Count how many times in this current session we have ISBD TX/RX'ed so we can break out if we hit limit. i.e. potential problem.
bool send_successful = false;         // did ISBD TX succeed?
uint8_t num_iridium_tx_attempts = 0;  // how many ISBD TX attempts have we had this time?
bool firstTX = true;                  // Send the text/binary message the first time, by on TX NULL messages thereafter when RX'ing in any remaining mesages waiting for us.
bool validRX = false;                 // Is the received Iridium message valid?
bool unvalidatedRX = false;           // Have we received an Iridium message, that we haven't validated yet?






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
        debugPrintln("\ncontact_ground() - Starting");
        oled.print("contact_ground()");
#ifdef BYPASS_IRIDIUM // print debugs/oled for BYPASS mode
        debugPrintln("contact_ground() - Iridium BYPASS mode (using SERIAL TEST LINK)");
        oled.println("w/BYPASS");
#else // print debugs/oled for using Iridium mode
        debugPrintln("contact_ground() - Iridium real TX/RX mode");
        oled.println("w/ISBD");
#endif
        do_iridium_locarb();    // do/attempt the whole ISBD TX/RX's

        seconds_since_last_iridium_tx = 0; // reset the counter.       xxx - should we have two counters, one for tx_attemps and one for tx_success

        debugPrintln("contact_ground() - Complete");
    }
} // END - case_contact_ground()

// END - contact_ground_fns.cpp
