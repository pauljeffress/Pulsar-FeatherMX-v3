/*
 * loop.cpp
 */

#include "global.h" // My main header file for this project itself

// Create and initialise main state machine vars.
PULSAR_MAIN_SM_STATE main_state = CHECK_POWER; // we always start at this state.

/*============================
 * loop()
 *
 *============================*/
void loop(void)
{
    // ==================
    // Main State Machine
    // ==================
    while (true) // Stay in this state machine forever.
    {
        loopHousekeeping(); // Execute any simple/quick housekeeping code that I want to run every time we change state

        // Run next state
        switch (main_state)
        {
        // ************************************************************************************************
        // Check Feather's power source status.
        case CHECK_POWER:
            case_check_power();
            main_state = READ_SENSORS; // Set next state
            break;

        // ************************************************************************************************
        // Read the various sensors attached to the Feather.
        case READ_SENSORS:
            case_read_sensors();
            //main_state = TX_TO_CANBUS; // Set next state
            main_state = CHECK_POWER; // XXX - TEMPORARY
            break;

        // // ************************************************************************************************
        // // If we need to, send info onto the CAN bus
        // case TX_TO_CANBUS:
        //     case_tx_to_CANbus();
        //     main_state = CHECK_CANBUS; // Set next state
        //     break;

        // // ************************************************************************************************
        // // check if any packets received via CAN bus.
        // case CHECK_CANBUS:
        //     case_check_CANbus();
        //     main_state = HEARTBEAT_TO_AP; // Set next state
        //     break;

        // // ************************************************************************************************
        // // Send a MAVLink HEARTBEAT to the Autopilot.
        // case HEARTBEAT_TO_AP:
        //     case_heartbeat_to_autopilot();
        //     main_state = RX_FROM_AGT; // Set next state
        //     break;

        // // ************************************************************************************************
        // // Check if the AGT has sent us a datum
        // case RX_FROM_AGT:
        //     case_rx_from_agt();
        //     main_state = PROCESS_AGT; // Set next state
        //     break;

        // // ************************************************************************************************
        // // process it if it has and set appropriate flags
        // case PROCESS_AGT:
        //     case_process_agt();
        //     main_state = PROCESS_AGT_FOR_AP; // Set next state
        //     break;

        // // ************************************************************************************************
        // // process it if it has and set appropriate flags
        // case PROCESS_AGT_FOR_AP:
        //     case_process_agt_for_ap();
        //     main_state = TX_TO_AP; // Set next state
        //     break;

        // // ************************************************************************************************
        // // Send Mavlink data to the autopilot.
        // case TX_TO_AP:
        //     case_tx_to_autopilot();
        //     main_state = RX_FROM_AP; // Set next state
        //     break;

        // // ************************************************************************************************
        // // Read any streamed params from the Autopilot that I need.
        // // Note: this procedure also IMPORTANTLY receives & records HEARTBEATS from AP.
        // case RX_FROM_AP:
        //     case_rx_from_autopilot();
        //     main_state = PROCESS_AP; // Set next state
        //     break;

        // // ************************************************************************************************
        // // Review/action the recently received Mavlink data from the Autopilot.
        // case PROCESS_AP:
        //     case_process_autopilot();
        //     main_state = TX_TO_AGT; // Set next state
        //     break;

        // // ************************************************************************************************
        // // If we need to, send a datum to the AGT
        // case TX_TO_AGT:
        //     case_tx_to_agt();
        //     main_state = TX_TO_LOGGER; // Set next state
        //     break;

        // // ************************************************************************************************
        // // Decide and write to the Logger.
        // case TX_TO_LOGGER:
        //     case_tx_to_logger();
        //     main_state = CHECK_POWER; // Set next state
        //     break;

        // ************************************************************************************************
        // DEFAULT - should not happen, but programing it defensively
        default:
            debugPrintln("ERROR - we hit the default: in main state machine");
            main_state = CHECK_POWER; // get back on track by starting again with check_power state.
            break;

        } // End of switch(main_state)

    } // End of while(true)

} // END - loop()