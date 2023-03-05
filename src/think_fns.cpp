/*
 * think_fns.cpp
 *
 * This is where we make the big decisions about what to do.
 *
 *
 */

#include "global.h"
#include "think_fns.h"

bool justBootedFlag = false;                // Ensures any activities that need to happen due to recent boot get triggered.
bool contactGroundFirstTimeFlag = false;    // Ensures that not long after boot the boat does an one initial contact_ground();
bool checkPowerFlag = false;                // When set will cause check_power() to be executed during next State Machine cycle.
bool readSensorsFlag = false;               // When set will cause read_sensors() to be executed during next State Machine cycle.
bool readGPSFlag = false;                   // When set will cause read_gps() to be executed during next State Machine cycle.
bool contactGroundFlag = false;             // When set will cause contact_ground() to be executed during next State Machine cycle.
bool gotMsgFromGroundFlag = false;          // When set will cause process_isbd_rx() to be executed during next State Machine cycle.
bool txMsgToApFlag = false;                 // When set will cause tx_to_autopilot() to be executed during next State Machine cycle.
bool isbdTxSucceededFlag = false;           // xxx - doesn't look like we act on this anywhere....consider removing.


/*
 * case_think() - this is where we do our global decision making.
 * ============
 * We look at things like timers and other state information and make decisions
 * about what to do next. We then set global flags to make those things happen as the 
 * state machine cycles through.
 * 
 * Logic Notes
 * -----------
 * 1. We generally only ever set flags here, and they are cleared when actioned
 * in other states. 
 * 2. If a flag is already set when this state is entered it will 
 * generally remain set on exit of this state.
 * 3. Keep in mind that the global flags can also be set by other states, for example
 * if CHECK_POWER thinks we should contact ground, then it too can set that flag.
 * 
 */
void case_think()
{
    DIPswCheck();   // Always check and see if any switches have changed.

    /*
     *  Flag any actions needed due to having just booted/reset.
     */
    if (justBootedFlag)
    {
        debugPrintln("case_think() - We have just booted");
        justBootedFlag = false;     // clear the flag as we are actioning it.
        checkPowerFlag = true;      // Always check power after boot/power-up.
        readSensorsFlag = true;     // Always read sensors after boot/power-up.
        readGPSFlag = true;         // Always read GPS after boot/power-up. (xxx - consider removal before going to see??? or use a #define to manage that.)
        contactGroundFirstTimeFlag = true;  // Always contact ground shortly after boot/power-up. (xxx - consider removal before going to see??? or use a #define to manage that.)
    }

    /*
     *  Should we CONTACT_GROUND due periodic interval timer or manual 5min (300 SECONDS) interval?
     */
    if (((seconds_since_last_contact_ground_attempt/60) > myFmxSettings.FMX_CONTACT_GND_INT_M) ||
        (flag_contact_ground_every_5mins && ((seconds_since_last_contact_ground_attempt) > 300)))
    {
        debugPrintln("case_think() - timer says Contact Ground");
        debugPrint("case_think() - seconds_since_last_contact_ground_attempt=");debugPrintlnInt(seconds_since_last_contact_ground_attempt);
        debugPrint("case_think() - myFmxSettings.FMX_CONTACT_GND_INT_M=");debugPrintlnInt(myFmxSettings.FMX_CONTACT_GND_INT_M);
        contactGroundFlag = true;
        seconds_since_last_contact_ground_attempt = 0;    // Reset this 'attempt' timer.
    }

    /*
     * Should we CONTACT_GROUND due to recent boot? 
     * i.e. do once not long after reboot/power-up and then settle into 
     * correct periodic interval after that.
     */
    if ((contactGroundFirstTimeFlag) and ((seconds_since_reset_or_powercycle/60) >= FMX_POSTBOOT_CONTACT_GROUND_PERIOD_M))
    {
        debugPrintln("case_think() - decided its first time to Contact Ground");
        contactGroundFlag = true;
        contactGroundFirstTimeFlag = false;
        seconds_since_last_contact_ground_attempt = 0;    // Reset this 'attempt' timer.
    }

    /*
     *  Should we check power on way through next pass?
     */
    if ((seconds_since_last_check_power > FMX_CHECKPOWER_PERIOD_S) or (contactGroundFlag))
    {
        debugPrintln("case_think() - timer (or contactGroundFlag) says Check Power");
        checkPowerFlag = true;
        seconds_since_last_check_power = 0;    // Reset this timer.
    }

    /*
     *  Should we read sensors on way through next pass?
     */
    if ((seconds_since_last_sensors_read > FMX_READSENSORS_PERIOD_S) or (contactGroundFlag))
    {
        debugPrintln("case_think() - timer (or contactGroundFlag) says Check Sensors");
        readSensorsFlag = true;
        seconds_since_last_sensors_read = 0;    // Reset this timer.
    }

    /*
     * Should we read GPS on way through next pass?
     */
    if ((seconds_since_last_gps_read > FMX_READGPS_PERIOD_S) or (contactGroundFlag))
    {
        debugPrintln("case_think() - timer (or contactGroundFlag) says Read GPS");
        readGPSFlag = true;
        seconds_since_last_gps_read = 0;    // Reset this timer.
    }




} // END - case_think()

// END of file think_fns.cpp
