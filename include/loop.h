/*
 * loop.h
 * 
 */

#ifndef LOOP_H
#define LOOP_H

#include <Arduino.h>


/* defines */


/* define any enums */

typedef enum PULSAR_MAIN_SM_STATE     // State machine states for the main state machine running in loop()
{
    THINK,
    CHECK_POWER,
    READ_SENSORS,
    READ_GPS,
    CONTACT_GROUND,
    PROCESS_ISBD_RX,
    TX_TO_CANBUS,
    CHECK_CANBUS,
    HEARTBEAT_TO_AP,
    RX_FROM_AP,
    PROCESS_AP,
    TX_TO_AP,
    RX_FROM_AGT,
    PROCESS_AGT,
    PROCESS_AGT_FOR_AP,
    TX_TO_AGT,
    TX_TO_LOGGER,
} PULSAR_MAIN_SM_STATE;

/* define any struct's */


/* extern global variables */
extern PULSAR_MAIN_SM_STATE main_state;


/* function pre defines */



#endif
// END - loop.h



