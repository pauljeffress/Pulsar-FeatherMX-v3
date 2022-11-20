/*
 * CANbus_tx_fns.cpp
 * 
 */

#include "global.h"
#include <CBP_fns.h>    // from Shared Source
#include <CAN_fns.h>    // from Shared Source - xxx - only required here because I call CANloopbackenable() whilst debugging

void case_tx_to_CANbus()
{
    // if its time to do a routine send (worst case sync method) OR something else has flagged that we should do it then
    if ((CANStatus)  // If the CAN bus is functioning ok and...
            && ((seconds_since_last_CAN_tx > FMX_TXCANINT_S) || (flag_do_CAN_tx)))
    {
        debugPrintln("\ncase_tx_to_CANbus() - time to execute");
        oled.println("\ntx_to_CANbus()");
        //Serial.println(myCANid);
        //delay(1000);     
        //CANloopbackEnable();        // ONLY FOR TESTING!!!!!!!
        #ifdef CAN_DEBUG
            debugPrintln("case_tx_to_CANbus() - About to send a Hello");
        #endif
        // Start by sending a CBP_HELLO....just because :)
        CBPsendHello(myCANid);
        #ifdef CAN_DEBUG 
            debugPrintln("case_tx_to_CANbus() - About to send bunch of params");
        #endif
        CBPsend_uint16_t(myCANid, CBP_FMX_BATT_V, myFmxSettings.FMX_BATT_V);
        CBPsend_int16_t(myCANid, CBP_FMX_TEMP, myFmxSettings.FMX_TEMP);
        CBPsend_uint16_t(myCANid, CBP_FMX_RH, myFmxSettings.FMX_RH);        
        CBPsend_uint32_t(myCANid, CBP_FMX_UPTIME_S, seconds());
        #ifdef CAN_DEBUG 
            debugPrint("case_tx_to_CANbus() - seconds()=");Serial.println(seconds());
        #endif
        // Send all of the parameters I have read from the chargers, even if they are still at defaults.
        CBPsend_uint16_t(myCANid, CBP_FMX_PRESS, myFmxSettings.FMX_PRESS);        
        CBPsend_int16_t(myCANid, CBP_FMX_WATERTEMP, myFmxSettings.FMX_WATERTEMP);
        CBPsend_int16_t(myCANid, CBP_FMX_AMBIENTLIGHT, myFmxSettings.FMX_AMBIENTLIGHT);
        
        flag_do_CAN_tx = false; // reset the flag
        seconds_since_last_CAN_tx = 0;  // reset the counter

        debugPrintln("case_tx_to_CANbus() - Finishing");
        //oled.println("case_tx_to_CANbus() - Finishing");
    }
    else
    {
        //#ifdef CAN_DEBUG 
        //    debugPrintln("case_tx_to_CANbus() - NOT NOW");
        //#endif
    }
}