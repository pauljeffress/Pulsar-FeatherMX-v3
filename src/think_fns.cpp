/*
 * think_fns.cpp
 *
 * This is where we make the big decisions about what to do.
 *
 *
 */

#include "global.h"
#include "think_fns.h"

bool justBootedFlag = false;
bool checkPowerFlag = false;
bool readSensorsFlag = false;
bool readGPSFlag = false;     // when set case_read_gps() will try for a fix.
bool contactGroundFlag = false;  // when set ....... will try to notify ground
bool gotMsgFromGroundFlag = false; 
bool txMsgToApFlag = false;
bool isbdTxSucceededFlag = false;


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
    /*
     *  Flag any actions needed due to having just booted/reset.
     */
    if (justBootedFlag)
    {
        justBootedFlag = false; // clear the flag as we are actioning it.

        checkPowerFlag = true;
        readSensorsFlag = true;
        readGPSFlag = true;
    }

    /*
     *  Should we CONTACT_GROUND?  
     */
    if ((seconds_since_last_contact_ground_attempt/60) > myFmxSettings.FMX_CONTACT_GND_INT_M)
    {
        contactGroundFlag = true;
        seconds_since_last_contact_ground_attempt = 0;    // Reset this timer.
    }

    /*
     *  Should we check power?
     */
    if (seconds_since_last_check_power > FMX_CHECKPOWER_PERIOD_S)
    {
        checkPowerFlag = true;
        seconds_since_last_check_power = 0;    // Reset this timer.
    }

    /*
     *  Should we read sensors?
     */
    if (seconds_since_last_sensors_read > FMX_READSENSORS_PERIOD_S)
    {
        readSensorsFlag = true;
        seconds_since_last_sensors_read = 0;    // Reset this timer.
    }

    if (seconds_since_last_gps_read > FMX_READGPS_PERIOD_S)
    {
        readGPSFlag = true;
        seconds_since_last_gps_read = 0;    // Reset this timer.
    }



} // END - case_think()

// END of file think_fns.cpp
