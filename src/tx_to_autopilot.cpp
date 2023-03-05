/*
 * tx_to_autopilot.cpp
 * 
 */

#include "global.h"
#include "tx_to_autopilot.h"

// Globals


// Functions

/*============================
 * tx_to_autopilot()
 *
 * Do a periodic TX to the AP, via MAVLink messages. 
 * AND/OR
 * Do a specifically flagged TX to the AP (usually due to a newly received msg from Ground).
 * 
 *============================*/
void tx_to_autopilot()
{
    bool result = false;

    // if its time to do a TX to the AP due to a recent message from the Ground?
    if (txMsgToApFlag)
    {
        debugPrintln("tx_to_autopilot() - Starting for (txMsgToApFlag == true) section");
        oled.println("\ntx_to_autopilot()");
        txMsgToApFlag = false;  // Clear flag as we don't need it after this.
        seconds_since_last_ap_tx = 0;   // reset timer.
        
        /*
         * Check and action ARM/DISARM commands.
         * =====================================
         */
        if (pulsar_mt_msg.set_ap_arm_state_b && pulsar_mt_msg.set_ap_disarm_state_b)    // if both ARM and DISARM commands are set then thats a problem
                                                                                        // and we will not execute either of them!
        {
            debugPrintln("tx_to_autopilot() - ERROR - both ARM and DISARM are requested, so not doing either!");
        }
        else    // i.e. only one, or none of the two ARM/DISARM commands are being requested.
        {
            if (pulsar_mt_msg.set_ap_arm_state_b) 
            {
                debugPrintln("tx_to_autopilot() - ARMing the AP");
                mavlink_set_arm_ap(&Serial2);           // xxx - set_arm_ap() needs improvement, see notes in its comments.
            }
            if (pulsar_mt_msg.set_ap_disarm_state_b)
            {
                debugPrintln("tx_to_autopilot() - DISarming the AP");
                mavlink_set_disarm_ap(&Serial2);     // xxx - set_disarm_ap() needs improvement, see notes in its comments.
            }
        }


        /*
         * Check and action SET_AP_PARAM commands from Ground.
         * ===================================================
         */
        if ((pulsar_mt_msg.set_ap_param_paramvaluetype1 != 0) && (pulsar_mt_msg.set_ap_param_paramname1 != ""))      
                                        // If set_ap_param_paramvaluetype == 0 that means this the Ground
                                        // has not populated this request, se we take no action.
                                        // Likewise, we double check pulsar_mt_msg.set_ap_param_paramname1 != ""
                                        // and also abort if it is empty.
        {
             debugPrintln("tx_to_autopilot() - setting ap param 1");
             result = mavlink_set_one_param_on_ap(&Serial2, pulsar_mt_msg.set_ap_param_paramname1,
                                     pulsar_mt_msg.set_ap_param_paramvalue_int1,
                                     pulsar_mt_msg.set_ap_param_paramvalue_float1,
                                     pulsar_mt_msg.set_ap_param_paramvaluetype1);  
        
            if (result) // i.e we successfully set the param on the AP and verified it.
            {
                debugPrintln("tx_to_autopilot() - setting ap param 1 - SUCCESS");
                myFmxSettings.AP_SET_PARAM1_RESULT = result;   // set response from Boat to Ground accordingly
            }
            else
            {
                debugPrintln("tx_to_autopilot() - setting ap param 1 - FAILED");
                myFmxSettings.AP_SET_PARAM1_RESULT = result;   // set response from Boat to Ground accordingly
            }
        }

        // do the same as above but for PARAM2
        if ((pulsar_mt_msg.set_ap_param_paramvaluetype2 != 0) && (pulsar_mt_msg.set_ap_param_paramname2 != ""))      
                                        // If set_ap_param_paramvaluetype == 0 that means this the Ground
                                        // has not populated this request, se we take no action.
                                        // Likewise, we double check pulsar_mt_msg.set_ap_param_paramname1 != ""
                                        // and also abort if it is empty.
        {
             debugPrintln("tx_to_autopilot() - setting ap param 2");
             result = mavlink_set_one_param_on_ap(&Serial2, pulsar_mt_msg.set_ap_param_paramname2,
                                     pulsar_mt_msg.set_ap_param_paramvalue_int2,
                                     pulsar_mt_msg.set_ap_param_paramvalue_float2,
                                     pulsar_mt_msg.set_ap_param_paramvaluetype2);  
        
            if (result) // i.e we successfully set the param on the AP and verified it.
            {
                debugPrintln("tx_to_autopilot() - setting ap param 2 - SUCCESS");
                myFmxSettings.AP_SET_PARAM2_RESULT = result;   // set response from Boat to Ground accordingly
            }
            else
            {
                debugPrintln("tx_to_autopilot() - setting ap param 2 - FAILED");
                myFmxSettings.AP_SET_PARAM2_RESULT = result;   // set response from Boat to Ground accordingly
            }
        }


        /*
         * Check and action any REQUEST_AP_PARAM commands from Ground.
         * ===========================================================
         */
        int32_t tempInt = 0;
        float   tempFloat = 0;

        if (pulsar_mt_msg.request_ap_param_paramname1 != "")      // If not empty then go ahead and REQUEST the param,
                                                                        // as empty indicates the Ground has not populated
                                                                        // this REQUEST_AP_PARAM command.
        {
             debugPrint("tx_to_autopilot() - requesting paramname1:");Serial.println(pulsar_mt_msg.request_ap_param_paramname1);
             result = mavlink_get_one_param_from_ap(&Serial2, pulsar_mt_msg.request_ap_param_paramname1,
                                                    &tempInt,
                                                    &tempFloat,
                                                    pulsar_mt_msg.request_ap_param_paramvaluetype1);  
            if (result) // i.e we requested and received the desired param from the  AP
            {
                debugPrintln("tx_to_autopilot() - request successful");
                // copy the received results into data structure ready to send back to ground.
                myFmxSettings.AP_REQ_PARAM_VALUETYPE1 = pulsar_mt_msg.request_ap_param_paramvaluetype1;
                myFmxSettings.AP_REQ_PARAM_VALUEINT1 = tempInt;
                myFmxSettings.AP_REQ_PARAM_VALUEFLOAT1 = tempFloat;
            }
            else
            {
                debugPrintln("tx_to_autopilot() - request FAILED");
                // flag the failure into the data structure ready to send back to ground.
                myFmxSettings.AP_REQ_PARAM_VALUETYPE1 = MAV_REQ_PARAM_FAIL;    // signal to ground that we did not get the param value that was requested.
                myFmxSettings.AP_REQ_PARAM_VALUEINT1 = 66;    // just to ensure there is a value stored, and its obviously a placeholder only
                myFmxSettings.AP_REQ_PARAM_VALUEFLOAT1 = 66.66;    // just to ensure there is a value stored, and its obviously a placeholder only
            }

            debugPrint("tx_to_autopilot() - AP_REQ_PARAM_VALUETYPE1:"); debugPrintlnInt(myFmxSettings.AP_REQ_PARAM_VALUETYPE1);
            debugPrint("tx_to_autopilot() - AP_REQ_PARAM_VALUEINT1:"); debugPrintlnInt(myFmxSettings.AP_REQ_PARAM_VALUEINT1);
            debugPrint("tx_to_autopilot() - AP_REQ_PARAM_VALUEFLOAT1:"); debugPrintlnFlt(myFmxSettings.AP_REQ_PARAM_VALUEFLOAT1);

            contactGroundFlag = true;  // regardless of success or failure, flag that we have something to send to Ground.
        }


        // do the same as above but for PARAM2
        tempInt = 0;
        tempFloat = 0;
        if (pulsar_mt_msg.request_ap_param_paramname2 != "")      // If not empty then go ahead and REQUEST the param,
                                                                        // as empty indicates the Ground has not populated
                                                                        // this REQUEST_AP_PARAM command.
        {
             debugPrint("tx_to_autopilot() - requesting paramname2:");Serial.println(pulsar_mt_msg.request_ap_param_paramname2);
             result = mavlink_get_one_param_from_ap(&Serial2, pulsar_mt_msg.request_ap_param_paramname2,
                                                    &tempInt,
                                                    &tempFloat,
                                                    pulsar_mt_msg.request_ap_param_paramvaluetype2);  
            if (result) // i.e we requested and received the desired param from the  AP
            {
                debugPrintln("tx_to_autopilot() - request successful");
                // copy the received results into data structure ready to send back to ground.
                myFmxSettings.AP_REQ_PARAM_VALUETYPE2 = pulsar_mt_msg.request_ap_param_paramvaluetype2;
                myFmxSettings.AP_REQ_PARAM_VALUEINT2 = tempInt;
                myFmxSettings.AP_REQ_PARAM_VALUEFLOAT2 = tempFloat;
            }
            else
            {
                debugPrintln("tx_to_autopilot() - request FAILED");
                // flag the failure into the data structure ready to send back to ground.
                myFmxSettings.AP_REQ_PARAM_VALUETYPE2 = MAV_REQ_PARAM_FAIL;    // signal to ground that we did not get the param value that was requested.
                myFmxSettings.AP_REQ_PARAM_VALUEINT2 = 66;    // just to ensure there is a value stored, and its obviously a placeholder only
                myFmxSettings.AP_REQ_PARAM_VALUEFLOAT2 = 66.66;    // just to ensure there is a value stored, and its obviously a placeholder only
            }

            debugPrint("tx_to_autopilot() - AP_REQ_PARAM_VALUETYPE2:"); debugPrintlnInt(myFmxSettings.AP_REQ_PARAM_VALUETYPE2);
            debugPrint("tx_to_autopilot() - AP_REQ_PARAM_VALUEINT2:"); debugPrintlnInt(myFmxSettings.AP_REQ_PARAM_VALUEINT2);
            debugPrint("tx_to_autopilot() - AP_REQ_PARAM_VALUEFLOAT2:"); debugPrintlnFlt(myFmxSettings.AP_REQ_PARAM_VALUEFLOAT2);

            contactGroundFlag = true;  // regardless of success or failure, flag that we have something to send to Ground.
        }


        /*
         *  Check and action mission data.
         */
        if (flag_got_msg_from_ground_with_mission)     // the mission data was pre validated and flag 
                                                    // would have been set if it looked like we had
                                                    // received a valid mission from Ground. 
        {
            debugPrintln("tx_to_autopilot() - flag_got_msg_from_ground_with_mission = TRUE, so sending mission to AP - xxx NOT CODED YET");
            // xxx Do the mission tx to AP stuff.

                //mission_clear_all_ap();
                //global_mission_dummy_load();
                //global_mission_print();


                //PulsarMavMissionResult_t  pmmr;   // local struct to hold results ready for return to callee.
                //pmmr.pr = NOT_SET;
                //pmmr.mmr = MAV_MISSION_ACCEPTED;
                //pmmr.u32value = 0;
                //pmmr = mission_upload_to_ap();
                //debugPrint("mavlink_get_MISSION_REQUEST_INT() - pmmr.pr:"); Serial.println(pmmr.pr);
                //debugPrint("mavlink_get_MISSION_REQUEST_INT() - pmmr.mmr(UNUSED):"); Serial.println(pmmr.mmr);
                //debugPrint("mavlink_get_MISSION_REQUEST_INT() - pmmr.u32value:"); Serial.println(pmmr.u32value);

                // #ifdef MAVLINK_DEBUG 
                //     debugPrintln("tx_to_autopilot() - Periodic TX - do immediate mavlink_receive() to check for response");
                // #endif

                // // do an immediate mavlink_receive() to see if the AP responded to me setting the param above.
                // uint32_t start = millis();  // xxx - need to review how I'm timing this loop...seems clunky. Also need to take any contants and set the as #defines.
                // while ((millis() < (start + 3000)) && (millis() > 4000))  // For 3 seconds, see if we can assemble received msgs and if so process them.
                //     mavlink_receive(); 

            myFmxSettings.AP_MISSION_RESULT = 66; // xxx - set to proper pulsar mav mission result 
            contactGroundFlag = true;  // regardless of success or failure, flag that we have something to send to AGT/Ground.
        }

        /*
         * Set the AP FlightMode via MAV_CMD_DO_SET_MODE (#176) https://mavlink.io/en/messages/common.html#MAV_CMD_DO_SET_MODE
         * =======================================
         */
        if (pulsar_mt_msg.set_ap_flightmode != AP_FLIGHTMODE_NC)   // Is ground asking us to change flightmode?
        {
            debugPrintln("tx_to_autopilot() - Set Flightmode - xxx NOT FULLY CODED YET!");
            debugPrintln("tx_to_autopilot() - Set Flightmode - xxx NEED TO TEST with different ARM/Disarm ETC");
            
            debugPrint("tx_to_autopilot() - desired Flightmode:");debugPrintlnInt(pulsar_mt_msg.set_ap_flightmode);
            
            // final bounds check - 0 = MANUAL, 4 = HOLD, 10 = AUTO, 11 = RTL
            if ((pulsar_mt_msg.set_ap_flightmode == 0) ||
                (pulsar_mt_msg.set_ap_flightmode == 4) ||
                (pulsar_mt_msg.set_ap_flightmode == 10) ||
                (pulsar_mt_msg.set_ap_flightmode == 11))
            {
                mavlink_set_flightmode_ap(&Serial2, pulsar_mt_msg.set_ap_flightmode);
                debugPrintln("tx_to_autopilot() - Set Flightmode - xxx NEED CODE to look for AP response output");
                // Even though success will be apparent from the  
                // later HEARTBEATS we capture in RX_FROM_AP, we need to check if the AP spits out an error
                // when this fails and if so, catch it and send it back to ground.
            }
            else
                debugPrintln("tx_to_autopilot() - Set Flightmode - WARNING - Value out of bounds, taking no action!");

            contactGroundFlag = true;  // regardless of the outcome we still want to trigger a send to ground
                                        // as it will have the latest FLIGHTMODE in it, as captured from a HEARTBEAT.
        }


        /*
         * Reboot the AP via MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN command.
         * =======================================
         */
        if (pulsar_mt_msg.set_ap_reboot_m != 0)     // If anything but zero then go ahead and REBOOT the AP,
                                                        // after "set_ap_reboot" mins.
        {
            debugPrintln("tx_to_autopilot() - Reboot AP - xxx NOT FULLY CODED YET!");
            debugPrintln("tx_to_autopilot() - Reboot AP - xxx NEED TO TEST WHEN IN AUTO MODE ETC");
            
            debugPrint("tx_to_autopilot() - Reboot AP - wait time (mins):");debugPrintlnInt(pulsar_mt_msg.set_ap_reboot_m);
            
            // final bounds check on allowed times in minutes (to protect a bit against just a random number reaching here!)
            if ((pulsar_mt_msg.set_ap_flightmode == 1) ||
                (pulsar_mt_msg.set_ap_flightmode == 5) ||
                (pulsar_mt_msg.set_ap_flightmode == 10) ||
                (pulsar_mt_msg.set_ap_flightmode == 15) ||
                (pulsar_mt_msg.set_ap_flightmode == 30) ||
                (pulsar_mt_msg.set_ap_flightmode == 60) ||
                (pulsar_mt_msg.set_ap_flightmode == 90) ||
                (pulsar_mt_msg.set_ap_flightmode == 120) ||
                (pulsar_mt_msg.set_ap_flightmode == 180) ||
                (pulsar_mt_msg.set_ap_flightmode == 240)
                )
            {
                delay(1000 * 60 * pulsar_mt_msg.set_ap_reboot_m);   // wait before issuing the reboot
                debugPrintln("tx_to_autopilot() - Reboot AP - executing command now!");
                mavlink_cmd_preflight_reboot_ap(&Serial2);
                
                
                debugPrintln("tx_to_autopilot() - Reboot AP - xxx NEED CODE to look for AP reboot output");
                myFmxSettings.AP_REBOOT_RESULT = 33; // xxx - temp mark it was ok, but we need to look for proof!!!
            }
            else
            {
                debugPrintln("tx_to_autopilot() - Reboot AP - WARNING - Value out of bounds, taking no action!");
                myFmxSettings.AP_REBOOT_RESULT = 66; // mark that it was a problem.
            }

            contactGroundFlag = true;  // regardless of success or failure, flag that we have something to send to AGT/Ground.
        }

        debugPrintln("tx_to_autopilot() - Completed for (txMsgToApFlag == true) section");   
        //oled.println("tx_to_autopilot() - Complete");
    }   // END - if (txMsgToApFlag)



    // I have commented out the below as I no longer see a need to do this.
    // // Is it time to do a periodic TX to the AutoPilot?
    // // Note: If we did a TX to the AP above due to new AP data from the AGT, then this periodic TX 
    // //       will get skipped as seconds_since_last_ap_tx will have just been reset.
    // if (seconds_since_last_ap_tx > TX_TO_AP_PERIOD_SECONDS)
    // {
    //     debugPrintln("\ntx_to_autopilot() - Starting periodic TX to AP section - xxx YET TO CODE THIS!");
    //     seconds_since_last_ap_tx = 0;   // reset timer.
    //     //do_periodic_tx_to_ap();   // xxx - yet to code
    //     debugPrintln("tx_to_autopilot() - Completed periodic TX to AP section");
    // }
    
    
}   // END - tx_to_autopilot()
