/*
 * timer_counter_fns.h
 * 
 * 
 * 
 */
#ifndef TIMER_COUNTER_FNS_H
#define TIMER_COUNTER_FNS_H

#include <arduino.h>

/* defines */
// NOTE - these are only for time periods that NEVER CHANGE (i.e cannot be set by Ground), they are very local timers.
#define FMX_CHECKPOWER_PERIOD_S         120     // seconds - how often should we run CHECK_POWER?
#define FMX_READSENSORS_PERIOD_S        180     // seconds - how often should we read the sensors?
#define FMX_MAVLINKHEARTBEAT_PERIOD_S   20      // seconds - how often should we send a MAVLink HEARTBEAT to the AutoPilot


/* define any enums */


/* define any struct's */


/* extern my global vars */
extern volatile uint32_t mytimercounter;
extern uint32_t mytimercounter_last;

extern unsigned long oneSecCounter;
extern unsigned long oneSecCounter_last;

extern volatile int loop_step;
extern int assess_step;
extern uint32_t assess_iterations_counter;
extern uint32_t assess_iterations_counter_last;

extern long iterationCounter;

extern volatile unsigned long seconds_since_reset_or_powercycle;
extern volatile unsigned long seconds_since_last_wake;
extern volatile unsigned long seconds_since_last_CAN_tx;
extern volatile unsigned long seconds_since_last_ap_tx;
extern volatile unsigned long seconds_since_last_ap_rx;
// extern volatile unsigned long seconds_since_last_agt_tx;
// extern volatile unsigned long seconds_since_last_agt_rx;
// extern volatile unsigned long seconds_since_last_fmx_tx;
// extern volatile unsigned long seconds_since_last_fmx_rx;
extern volatile unsigned long seconds_since_last_sensors_read;
// extern volatile unsigned long seconds_since_last_page_update;
// extern volatile unsigned long seconds_since_last_page_change;
extern volatile unsigned long seconds_since_last_mavlink_heartbeat_tx;
extern volatile unsigned long seconds_since_last_mavlink_heartbeat_rx;
extern volatile unsigned long seconds_since_last_mavlink_req;
extern volatile unsigned long seconds_since_last_gps_read;
extern volatile unsigned long seconds_since_last_contact_ground_attempt;
extern volatile unsigned long seconds_since_last_iridium_tx;
extern volatile unsigned long seconds_since_last_iridium_rx;
extern volatile unsigned long seconds_since_last_alarmtx;
extern volatile unsigned long seconds_since_last_check_power;


/* function pre defines */
void timerCounterSetup(void);
void timerCounterIncrementer(void);

#endif
