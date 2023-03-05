/*
 * DIPsw_fns.cpp
 * 
 * All functions to do with the DIP Switches attached to the FMX.
 * 
 * 
 */

#include "global.h" // My main header file for this project itself
#include "DIPsw_fns.h"

// Globals
bool DIPsw1_status = false;     // setting them all to a known initial value, as they will be read in before any real use.
bool DIPsw2_status = false;
bool DIPsw3_status = false;
bool DIPsw4_status = false;
bool DIPsw5_status = false;

bool DIPsw1_last_checked_status = false;    // Assumption is all switches are off, anything else will get printed in debug prints.
bool DIPsw2_last_checked_status = false; 
bool DIPsw3_last_checked_status = false; 
bool DIPsw4_last_checked_status = false; 
bool DIPsw5_last_checked_status = false; 

bool flag_bypass_iridium = false;   // Controlled by DIP SW 1. If set, we will not use the 9603 Iridium modem, we wil use Serial bypass via GSE32.
bool flag_reset_eeprom = false;     // Controlled by DIP SW 2. If set, on power-up/reset we will reset EEPROM values to defaults.
bool flag_contact_ground_every_5mins = false;   // Controlled by DIP SW 3. If set, manually overides myFmxSettings.FMX_CONTACT_GND_INT_M to 5 mins

// Functions


void DIPswSetup()
{
    // setup DIP Switch input pins
    pinMode(DIPSW1_PIN, INPUT);
    pinMode(DIPSW2_PIN, INPUT);
    pinMode(DIPSW3_PIN, INPUT);
    pinMode(DIPSW4_PIN, INPUT);
    pinMode(DIPSW5_PIN, INPUT);

}   // END - DIPswSetup()


/*
 * Check status of all DIP switches and set any boolean flags accordingly.
 */
void DIPswCheck()
{
    // backup last state of all switches before we read them again
    DIPsw1_last_checked_status = DIPsw1_status;
    DIPsw2_last_checked_status = DIPsw2_status;
    DIPsw3_last_checked_status = DIPsw3_status;
    DIPsw4_last_checked_status = DIPsw4_status;
    DIPsw5_last_checked_status = DIPsw5_status;
    
    // Sample the switches
    DIPsw1_status = digitalRead(DIPSW1_PIN);
    DIPsw2_status = digitalRead(DIPSW2_PIN);
    DIPsw3_status = digitalRead(DIPSW3_PIN);
    DIPsw4_status = digitalRead(DIPSW4_PIN);
    DIPsw5_status = digitalRead(DIPSW5_PIN);

    // Action/set flags for all switches, doesn't mater if changed state or not.
    //sw1
    flag_bypass_iridium = DIPsw1_status;    // set the flag to match switch status.
    //sw2
    flag_reset_eeprom = DIPsw2_status;      // set the flag to match switch status.
    //sw3
    flag_contact_ground_every_5mins = DIPsw3_status;    // set the flag to match switch status.
    //sw4

    //sw5

    // Check if they have changed since last check and if so, print it.
    if (DIPsw1_status != DIPsw1_last_checked_status)   // i.e. its changed since last check
    {
        debugPrint("DIPswCheck() - DIPsw1 changed to ");
        if (DIPsw1_status) debugPrintln("ON"); else  debugPrintln("OFF");
        debugPrint("DIPswCheck() - So flag_bypass_iridium = ");
        if (DIPsw1_status) debugPrintln("TRUE now."); else  debugPrintln("FALSE now.");
    }

    if (DIPsw2_status != DIPsw2_last_checked_status)   // i.e. its changed since last check
    {
        debugPrint("DIPswCheck() - DIPsw2 changed to ");
        if (DIPsw2_status) debugPrintln("ON"); else  debugPrintln("OFF");
        debugPrint("DIPswCheck() - So flag_reset_eeprom = ");
        if (DIPsw2_status) debugPrintln("TRUE now."); else  debugPrintln("FALSE now.");
    }

    if (DIPsw3_status != DIPsw3_last_checked_status)   // i.e. its changed since last check
    {
        debugPrint("DIPswCheck() - DIPsw3 changed to ");
        if (DIPsw3_status) debugPrintln("ON"); else  debugPrintln("OFF");
        debugPrint("DIPswCheck() - So flag_contact_ground_every_5mins = ");
        if (DIPsw3_status) debugPrintln("TRUE now."); else  debugPrintln("FALSE now.");
    }

    if (DIPsw4_status != DIPsw4_last_checked_status)   // i.e. its changed since last check
    {
        debugPrint("DIPswCheck() - DIPsw4 changed to ");
        if (DIPsw4_status) debugPrintln("ON"); else  debugPrintln("OFF");
    }

    if (DIPsw5_status != DIPsw5_last_checked_status)   // i.e. its changed since last check
    {
        debugPrint("DIPswCheck() - DIPsw5 changed to ");
        if (DIPsw5_status) debugPrintln("ON"); else  debugPrintln("OFF");
    }
}   // END - DIPswSetup()




// END - DIPsw_fns.cpp