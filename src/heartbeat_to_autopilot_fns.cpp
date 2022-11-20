/*
 * heartbeat_to_autopilot_fns.cpp
 * 
 */

#include "global.h"

void case_heartbeat_to_autopilot()
{
    // if its time to send a MAVLink HEARTBEAT to the AutoPilot
    if (seconds_since_last_mavlink_heartbeat_tx > FMX_TX_HB_AP_S)
    {
        debugPrintln("case_heartbeat_to_autopilot() - time to execute");
        //oled.println("heartbeat_to_autopilot()");

        mavlink_fmx_send_heartbeat_to_ap(&Serial2);

        seconds_since_last_mavlink_heartbeat_tx = 0;  // reset the counter

        debugPrintln("case_heartbeat_to_autopilot() - finishing");
        //oled.println("case_heartbeat_to_autopilot() - finishing");
    }
    else
    {
        #ifdef MAVLINK_DEBUG 
            //debugPrintln("case_heartbeat_to_autopilot() - NOT NOW");
        #endif
    }

}