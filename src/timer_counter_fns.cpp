/*
 * timer_counter_fns.cpp
 *
 * All timer/counter related globals and functions
 * 
 * My routines that use the SAMD_TimerInterrupt Library - https://github.com/khoih-prog/SAMD_TimerInterrupt
 * Based on the libraries example code in "TimerInterruptLEDDemo.ino"
 */

#include "timer_counter_fns.h"

#include "debug_fns.h"

/*============================*/
/* Global Variables           */
/*============================*/
// variables used when testing my 1sec ISR Timer.
volatile uint32_t mytimercounter = 0;
uint32_t mytimercounter_last = 0;

// temporarily required intil my 1sec Timer ISR is working
unsigned long oneSecCounter = 0;
unsigned long oneSecCounter_last = 0;

// iterationCounter is incremented each time a transmission is attempted.
// It helps keep track of whether messages are being sent successfully.
// It also indicates if the device has been reset (the count will go back to zero).
long iterationCounter = 0;

// Period counters (seconds) - They are incremented in the RTC ISR (hence volitile type) that fires every second.
// Note, this bit of code only gets executed at Power on or HW reset so all counters have to be reset.
volatile unsigned long seconds_since_reset_or_powercycle = 0;
volatile unsigned long seconds_since_last_wake = 0;
volatile unsigned long seconds_since_last_CAN_tx = 0;
volatile unsigned long seconds_since_last_ap_tx = 0;
volatile unsigned long seconds_since_last_ap_rx = 0;
//volatile unsigned long seconds_since_last_agt_tx = 0;
//volatile unsigned long seconds_since_last_agt_rx = 0;
//volatile unsigned long seconds_since_last_fmx_tx = 0;
//volatile unsigned long seconds_since_last_fmx_rx = 0;
volatile unsigned long seconds_since_last_sensors_read = 0;
//volatile unsigned long seconds_since_last_page_update = 0;
//volatile unsigned long seconds_since_last_page_change = 0;
volatile unsigned long seconds_since_last_mavlink_heartbeat_tx = 0;
volatile unsigned long seconds_since_last_mavlink_heartbeat_rx = 0;
volatile unsigned long seconds_since_last_mavlink_req = 0;
volatile unsigned long seconds_since_last_gps_read = 0;
volatile unsigned long seconds_since_last_contact_ground_attempt = 0;
volatile unsigned long seconds_since_last_iridium_tx = 0;
volatile unsigned long seconds_since_last_iridium_rx = 0;
volatile unsigned long seconds_since_last_alarmtx = 0;
volatile unsigned long seconds_since_last_check_power = 0;

uint32_t assess_iterations_counter = 0;      // Useful while debugging my assess_situation state machine.
uint32_t assess_iterations_counter_last = 0; // Useful while debugging my assess_situation state machine.

// void TimerHandler0()
// {
//   mytimercounter++;
// }


void timerCounterSetup(void)
{
    // Gets called from case_loop_init as we need to I am following the same model in the code as I have in the
    // AGT, where it accomodates the MCU going to sleep and certain things needing to be setup post sleep
    // rather than in setup() which only gets called post HW reset.

    debugPrintln("timerSetup() - executing");

    seconds_since_reset_or_powercycle = 0;
    seconds_since_last_wake = 0;
    seconds_since_last_CAN_tx = 0;
    seconds_since_last_ap_tx = 0;
    seconds_since_last_ap_rx = 0;
    // seconds_since_last_agt_tx = 0;
    // seconds_since_last_agt_rx = 0;
    // seconds_since_last_fmx_tx = 0;
    // seconds_since_last_fmx_rx = 0;
    seconds_since_last_sensors_read = 0;
    // seconds_since_last_page_update = 0;
    // seconds_since_last_page_change = 0;
    seconds_since_last_mavlink_heartbeat_tx = 0;
    seconds_since_last_mavlink_heartbeat_rx = 0;
    seconds_since_last_mavlink_req = 0;
    seconds_since_last_gps_read = 0;
    seconds_since_last_contact_ground_attempt = 0;
    seconds_since_last_iridium_tx = 0;
    seconds_since_last_iridium_rx = 0;
    seconds_since_last_alarmtx = 0;
    seconds_since_last_check_power = 0;

    // Proper code below to reinstate when I fix Linker error
    // // Interval in microsecs
    // if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
    // {
    //   Serial.println("Timer set");
    // }
    // else
    //   Serial.println(F("Can't set ITimer0. Select another freq. or timer"));
}

void timerCounterIncrementer(void)
{
    // debugPrint("timerIncrementer() - executing at ");
    // Serial.print(millis());
    seconds_since_reset_or_powercycle++;
    seconds_since_last_wake++;
    seconds_since_last_CAN_tx++;
    seconds_since_last_ap_tx++;
    seconds_since_last_ap_rx++;
    // seconds_since_last_agt_tx++;
    // seconds_since_last_agt_rx++;
    // seconds_since_last_fmx_tx++;
    // seconds_since_last_fmx_rx++;
    seconds_since_last_sensors_read++;
    // seconds_since_last_page_update++;
    // seconds_since_last_page_change++;
    seconds_since_last_mavlink_heartbeat_tx++;
    seconds_since_last_mavlink_heartbeat_rx++;
    seconds_since_last_mavlink_req++;
    seconds_since_last_gps_read++;
    seconds_since_last_contact_ground_attempt++;
    seconds_since_last_iridium_tx++;
    seconds_since_last_iridium_rx++;
    seconds_since_last_alarmtx++;
    seconds_since_last_check_power++;

    // debugPrint(" seconds_since_last_wake=");
    // Serial.println(seconds_since_last_wake);
}


// END - timer_counter_fns.cpp