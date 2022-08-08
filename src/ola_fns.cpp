/*
 * ola_fns.cpp
 *
 * Supporting functions for logging to the OpenLog Artemis (OLA).
 * 
 */

#include "global.h"        // My main header file for this project itself
#include "ola_fns.h"

// Based on Debug functions taken from the SparkFun u-blox library:
// https://github.com/sparkfun/SparkFun_Ublox_Arduino_Library

// see also my debug_fns.cpp as I based this on that.

// stuff for my printDebug functionality (most code is in debug_fns.cpp)
bool _printLog = false; // Flag to show if message field log printing is enabled.
Stream *_logSerial;     // The stream to send log messages to (if enabled)


void enableLogging(Stream &logPort)
{
  _logSerial = &logPort; //Grab which port the user wants us to use for logging

  _printLog = true; //Should we print the commands we send? Good for Logging
}

void disableLogging(void)
{
  _printLog = false; //Turn off extra print statements
}

void logPrintStamp(void)
{
    _logSerial->print(millis());
    _logSerial->print(":");
}

void logPrint(const char *message)
//Safely print messages
{
  if (_printLog == true)
  {
    logPrintStamp();
    _logSerial->print(message);
  }
}

void logPrintln(const char *message)
//Safely print messages
{
  if (_printLog == true)
  {
    logPrintStamp();
    _logSerial->println(message);
  }
}

void logPrintInt(int32_t number)
//Safely print a number
{
  if (_printLog == true)
  {
    logPrintStamp();
    _logSerial->print(number);
  }
}

void logPrintlnInt(int32_t number)
//Safely print a number
{
  if (_printLog == true)
  {
    logPrintStamp();
    _logSerial->println(number);
  }
}

void logPrintFlt(float number)
//Safely print a number
{
  if (_printLog == true)
  {
    logPrintStamp();
    _logSerial->print(number);
  }
}

void logPrintlnFlt(float number)
//Safely print a number
{
  if (_printLog == true)
  {
    logPrintStamp();
    _logSerial->println(number);
  }
}