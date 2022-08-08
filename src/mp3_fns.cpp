/*
 * mp3_fns.cpp
 * 
 * Functions that use I2C SparkFun Qwiic MP3 Trigger DEV-19030
 * https://learn.sparkfun.com/tutorials/qwiic-mp3-trigger-hookup-guide
 * 
 * 
 */

#include "global.h" // My main header file for this project itself
#include "mp3_fns.h"

// Globals
MP3TRIGGER mp3;
bool mp3_status = false;  

// Functions


void MP3Setup()
{
    // We are assuming "Wire.begin()"" has already been run.

    // Setup the MP3 player
    debugPrintln("MP3Setup() - MP3 Player initialising");
    if (!mp3.begin())
    {
        debugPrintln("MP3Setup() - Couldn't find MP3 Player");
        mp3_status = BAD;
    }
    else
    {
        debugPrintln("MP3Setup() - Found MP3 Player");
        mp3_status = GOOD;
        mp3.setVolume(20); //Volume can be 0 (off) to 31 (max)
        mp3.playFile(1); //Play F001.mp3 "Pulsar Initialisation"
    }

}   // END - MP3Setup()

