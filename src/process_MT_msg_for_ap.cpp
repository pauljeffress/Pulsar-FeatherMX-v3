/*
 * process_MT_msg_for_ap.cpp
 *
 * Called when we recently got a message from Ground.
 * If it contains valid mission data then copy it into
 * global_mission_subset and set flag to say its populated
 * and ready for use in case_tx_to_autopilot();
 *
 * We are not assessing the other AP related params in
 * myFmxSharedSettings such as myFmxSharedSettings.set_ap_arm_state_b
 * as we will do that shortly in tx_to_autopilot();
 *
 */

#include "global.h"

bool flag_tx_msg_to_ap = false; // If set, then case_tx_to_autopilot() will execute fully.

void process_MT_msg_for_ap()
{
    bool missionValid = true; // gets set to false if for any reason I don't think the mission data is valid.
    uint16_t tempCount = 0;

    debugPrintln("\ncase_process_agt_for_ap() - starting as flag_got_msg_from_agt == true");
    oled.println("process_agt_for_ap()");
    flag_tx_msg_to_ap = true; // everytime we get a message from the Ground we set this flag,
                              // because we want the subsequent tx to ap function to look at the
                              // info we received from the Ground and determine if it needs to action
                              // it with the ap (read params, set params, set mission etc)

    // Check the received mission data
    // Check if the 'count' param is out bounds.
    if ((myFmxSettings.global_mission_subset.count < 1) || (myAgtSharedSettings.global_mission_subset.count > PULSAR_MISSION_MAX_ITEMS))
        missionValid = false;
    else
        tempCount = myAgtSharedSettings.global_mission_subset.count; // so I can use it in subsequent checks.
    // Check that the sequence_numbers in the first and last PulsarMissionItem in the mission are as expected.
    // I do this just as a bit of an extra check that the mission data has not been mangled at some point.
    if (myAgtSharedSettings.global_mission_subset.pulsar_mission_item_subset[0].sequence_number != 0)
        missionValid = false;
    else if (myAgtSharedSettings.global_mission_subset.pulsar_mission_item_subset[tempCount - 1].sequence_number != (tempCount - 1))
        missionValid = false;

    // If no issues identified above, then process the mission data
    if (missionValid)
    {
        // All we need to do to process it is copy the mission data to our global_mission_subset.
        memcpy(&myAgtSharedSettings.global_mission_subset, &global_mission_subset, sizeof(global_mission_subset));
        debugPrintln("case_process_agt_for_ap() - Mission from AGT looks valid, getting it ready for subsequent tx to AP.");
        flag_got_msg_from_agt_with_mission = true; // flag this so it will be actioned in subsequent tx to ap.
    }
    else
    {
        debugPrintln("case_process_agt_for_ap() - ERROR - RX'd msg failed some checks, so NOT processing it");
        flag_got_msg_from_agt_with_mission = false; // clear this flag so it will NOT be actioned in subsequent tx to ap.
    }

    flag_got_msg_from_agt = false; // no longer needed, so clear it.
    debugPrintln("case_process_agt_for_ap() - Complete");
    // oled.println("case_process_agt_for_ap() - Complete");

} // END - process_MT_msg_for_ap()

// END - process_MT_msg_for_ap.cpp