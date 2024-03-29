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

    loopHousekeeping(); // Execute any simple/quick housekeeping code that I want to run every time
                        // we loop or change state.

    // Run next state
    switch (main_state)
    {
    // ************************************************************************************************
    // Check Feather's power source status.
    case THINK:
        case_think();
        main_state = CHECK_POWER;
        break;

    // ************************************************************************************************
    // Check Feather's power source status.
    case CHECK_POWER:
        check_power(false); // execute check_power but not silent.
        main_state = READ_SENSORS;
        break;

    // ************************************************************************************************
    // Read the various sensors attached to the Feather.
    case READ_SENSORS:
        case_read_sensors();
        main_state = READ_GPS;
        break;

    // ************************************************************************************************
    // Read the GPS attached to the Feather.
    case READ_GPS:
        case_read_gps();
        main_state = CONTACT_GROUND;
        break;

    // ************************************************************************************************
    // Contact Ground Station - use the ISBD link (or Serial bypass test code) to communicate with my Ground Station.
    case CONTACT_GROUND:
        case_contact_ground();
        main_state = PROCESS_ISBD_RX;
    break;

    // ************************************************************************************************
    // Properly store MT msg here on FMX, 
    // then action anything we received from the ground that is for the FMX itself,
    // then do any TX to the AP based on the MT msg we received.
    case PROCESS_ISBD_RX:
        process_isbd_rx();
        main_state = RX_FROM_AP;
    break;

    // ************************************************************************************************
    // Read any streamed params from the Autopilot that I need.
    // Note: this procedure also IMPORTANTLY receives & records HEARTBEATS from AP.
    case RX_FROM_AP:
        rx_from_autopilot();
        main_state = TX_TO_CANBUS; // Set next state
        break;

    // ************************************************************************************************
    // If we need to, send info onto the CAN bus
    case TX_TO_CANBUS:
        case_tx_to_CANbus();
        main_state = CHECK_CANBUS; // Set next state
        break;

    // ************************************************************************************************
    // check if any packets received via CAN bus.
    case CHECK_CANBUS:
        case_check_CANbus();
        main_state = HEARTBEAT_TO_AP; // Set next state
        break;

    // ************************************************************************************************
    // Send a MAVLink HEARTBEAT to the Autopilot.
    case HEARTBEAT_TO_AP:
        case_heartbeat_to_autopilot();
        main_state = THINK; // Set next state
        break;






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

} // END - loop()