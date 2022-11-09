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
bool DIPsw1_last_checked_status = false;    // Assumption is all switches are off, anything else will get printed in debug prints.
bool DIPsw2_last_checked_status = false; 
bool DIPsw3_last_checked_status = false; 
bool DIPsw4_last_checked_status = false; 
bool DIPsw5_last_checked_status = false; 

bool flag_bypass_iridium = false;   // Controlled by DIP SW 1. If set, we will not use the 9603 Iridium modem, we wil use Serial bypass via GSE32.


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
    // Sample the switches
    bool DIPsw1_status = digitalRead(DIPSW1_PIN);
    bool DIPsw2_status = digitalRead(DIPSW2_PIN);
    bool DIPsw3_status = digitalRead(DIPSW3_PIN);
    bool DIPsw4_status = digitalRead(DIPSW4_PIN);
    bool DIPsw5_status = digitalRead(DIPSW5_PIN);

    // Check if they have changed since last check and if so, action it.
    if (DIPsw1_status != DIPsw1_last_checked_status)   // i.e. its changed since last check
    {
        DIPsw1_last_checked_status = DIPsw1_status;
        debugPrint("DIPswCheck() - DIPsw1 changed to ");
        if (DIPsw1_status) debugPrintln("ON"); else  debugPrintln("OFF");
        // Action the change
        flag_bypass_iridium = DIPsw1_status;    // set the flag ato match switch status.
        debugPrint("DIPswCheck() - So flag_bypass_iridium = ");
        if (DIPsw1_status) debugPrintln("TRUE now."); else  debugPrintln("FALSE now.");
    }

    if (DIPsw2_status != DIPsw2_last_checked_status)   // i.e. its changed since last check
    {
        DIPsw2_last_checked_status = DIPsw2_status;
        debugPrint("DIPswCheck() - DIPsw2 changed to ");
        if (DIPsw2_status) debugPrintln("ON"); else  debugPrintln("OFF");
    }

    if (DIPsw3_status != DIPsw3_last_checked_status)   // i.e. its changed since last check
    {
        DIPsw3_last_checked_status = DIPsw3_status;
        debugPrint("DIPswCheck() - DIPsw3 changed to ");
        if (DIPsw3_status) debugPrintln("ON"); else  debugPrintln("OFF");
    }

    if (DIPsw4_status != DIPsw4_last_checked_status)   // i.e. its changed since last check
    {
        DIPsw4_last_checked_status = DIPsw4_status;
        debugPrint("DIPswCheck() - DIPsw4 changed to ");
        if (DIPsw4_status) debugPrintln("ON"); else  debugPrintln("OFF");
    }

    if (DIPsw5_status != DIPsw5_last_checked_status)   // i.e. its changed since last check
    {
        DIPsw5_last_checked_status = DIPsw5_status;
        debugPrint("DIPswCheck() - DIPsw5 changed to ");
        if (DIPsw5_status) debugPrintln("ON"); else  debugPrintln("OFF");
    }
}   // END - DIPswSetup()




// END - DIPsw_fns.cpp