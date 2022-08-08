/*
 * ola_fns.h
 * 
 */

#ifndef OLA_FNS_H
#define OLA_FNS_H

#include <Arduino.h>

/* defines */

/* define any enums */

/* define any struct's */

/* extern global variables */

extern bool _printLog;
extern Stream *_logSerial;

/* function pre defines */
void enableLogging(Stream &logPort);
void disableLogging(void);
void logPrintStamp(void);
void logPrint(const char *message);
void logPrintln(const char *message);
void logPrintInt(int32_t number);
void logPrintlnInt(int32_t number);
void logPrintFlt(float number);
void logPrintlnFlt(float number);

#endif
// END - ola_fns.h



