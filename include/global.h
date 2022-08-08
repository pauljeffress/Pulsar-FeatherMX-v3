/*
 * global.h
 * 
 * Overall header file for this project
 * 
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <Arduino.h>
#include "wiring_private.h"    // for "pinPeripheral()". Only needed in PlatformIO, not required in Arduino IDE???
#include <Wire.h>       // Lots of I2C devices in this project
#include <SPI.h>        // CAN txcvr uses SPI


// Below mavlink includes draw from the c_library_v1 library I added to this PlatformIO project.
//#include <common/mavlink.h> // The Mavlink library for the "common" dialect  // Must remove this line now that I have the
                                                                               // ardupilotmega dialect below, as it pulls 
                                                                               // in common itself.
#include <ardupilotmega/mavlink.h> // The Mavlink library for the "ardupilotmega" dialect

#include <TimeLib.h> // The PJRC TimeLib library to help me deal with Unix Epoch time from GPS.

// Includes from my various files here in this project
#include "serial_fns.h"
#include "loop.h"
#include "misc_fns.h"
#include "RTC_Adafruit_fns.h"
#include "OLED_mini_fns.h"
#include "sensor_fns.h"
#include "ola_fns.h"
#include "actuator_fns.h"
#include "mp3_fns.h"
#include "gps_fns.h"

// Includes from various stuff I need from Pulsar_Shared_Source
#include "CBP.h"    // my CAN Boat Protocol
#include "can_fns.h"  
#include "debug_fns.h"
#include "timer_fns.h"
#include "Unions.h"
#include "pulsar_packer_fns.h"
#include "pulsar_buffer_fns.h"
#include "case_check_CANbus.h"
#include "case_check_power.h"
#include "mavlink_fns.h"
#include "PowerFeatherSettings_fns.h"
#include "TFTFeatherInternalSettings_fns.h"
#include "FmxSettings_fns.h"
#include "pulsar_boat_tofrom_ground_fns.h"


/*=======================*/
/* #DEFINES              */
/*=======================*/
#define GOOD true
#define BAD false

#define HOST_IS_FEATHERMX    // used to select what CAN/CBP packets to decode

// 12v switched power outputs
#define PI_PWR_PIN              23 // The pin the FMX controls the power supply to the Pi on.
#define STROBE_LIGHT_PWR_PIN    24 // The pin the FMX controls the power supply to the Strobe light on.
#define POWER_FEATHER_PWR_PIN   25 // The pin the FMX controls the power supply to the Power Feather on.

/*=======================*/
/* define any enums      */
/*=======================*/



/*=======================*/
/* define any struct's   */
/*=======================*/

/*=======================*/
/* extern my global vars */
/*=======================*/

extern Uart Serial2;
extern Uart Serial3;


extern bool flag_tx_msg_to_agt;
extern bool flag_got_msg_from_agt;
extern bool flag_got_msg_from_agt_with_mission;
extern bool flag_tx_msg_to_ap;
extern bool flag_do_first_agt_tx;
extern bool flag_first_agt_tx;

extern GlobalMission_t global_mission;

extern uint8_t fmx_tx_to_agt_numtries;

extern long lastsec1;

/*============================*/
/* Function Predefines        */
/*============================*/
void case_loop_init();
void case_zzz();
void case_wake();
void case_assess_situation();
void case_read_sensors();
void case_tx_to_CANbus();
void case_write_to_tft();
void case_heartbeat_to_autopilot();
void case_rx_from_autopilot();
void case_process_autopilot();
void case_tx_to_autopilot();
void case_rx_from_agt();
void case_process_agt();
void case_process_agt_for_ap();
void case_tx_to_agt();
void case_tx_to_logger();



int16_t int32_to_int16_a(int32_t input_int32);
uint16_t int32_to_int16_b(int32_t input_int32);
uint16_t int32_to_int16_c(int32_t input_int32);




#endif