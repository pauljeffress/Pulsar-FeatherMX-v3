/*
 * rx_from_autopilot.cpp
 * 
 */

#include "global.h"

#define FMX_RXAPINT_M       300  // How often we want the FMX to read values from the AP.



/*============================
 * rx_from_autopilot()
 *
 * Do a periodic read of the various params I want from the AP, via MAVLink messages.
 * OR
 * Do it when a flag says so.  I.e. when we know we are about to TX to GROUND, we should
 *      do it to get most current values from the AP, as we may have recently issued some 
 *      commands to it.
 * 
 * Note: We just look at the MAVLink streams and pull out what we want. There is no specific
 *       Q&A that takes place here.  It's simply passive looking at the stream from the AP
 *       and catching/keeping what we are interested in.  Same stuff, every time.
 * 
 *============================*/
void rx_from_autopilot()
{
    // if its time to do a routine or on demand read of streamed AutoPilot MAVlink data?
    if ((seconds_since_last_ap_rx > FMX_RXAPINT_M) || contactGroundFlag)
    {
        debugPrint("rx_from_autopilot() - Starting");
        debugPrint(" - due to:");
        if (contactGroundFlag) debugPrintln("contactGroundFlag=TRUE");
        else debugPrintln("seconds_since_last_ap_rx > FMX_RXAPINT_M");

        oled.println("\nrx_from_autopilot()");


        // Before trying to read from the streams we just ensure the 
        // AP is still streaming the messages we are going to look for.
        // Streaming may have been turned off for some reason (via GCS, FMX etc, 
        // or AP may have reset/power-cycled), so we just make sure it is still/back on.
        mavlink_request_streaming_params_from_ap(&Serial2);

        // Collect the messages I am interested in as per requests above
        // =============================================================
        debugPrintln("rx_from_autopilot() - WARNING - CODE NOT WRITTEN HERE YET!!!!!");

        // Initialise temp holding spots and put a magic number in them so I can know when they have been updated successfully

        // xxx - I have not done this "Initialise temp holding..."

        // Do a bulk collection of data from the streamed messages

        debugPrint("rx_from_autopilot() - ATTEMPTING RX - starting at Millis:"); Serial.println(millis());
        // Explanation of the following while() loop
        // I need to ensure that the mavlink_receive() function gets a solid couple of contiguous
        // seconds to ensure it gets a good chance to snap up lots of mavlink data and form packets.
        // If I give it just a few milliseconds each time its not enough as it seems that the other
        // states in assess_situation() take up too much time and I think that serial chars from 
        // the autopilot were getting dropped and so I wasn't getting full packets reliably.
        // The other key thing to consider here is that the mavlink_request_streaming_params_from_ap() function
        // has asked the autopilot to send data once a second, so waiting here for a few seconds gives
        // me multiple chances to catch what I need in the mavlink stream.
        // Also, as I am concerned about this while loop playing up when millis() rolls over to zero, 
        // I am using the extra "&& (millis() > 4000)" in the while() to jump that bit close to 0.
        // xxx - AS PER https://techexplorations.com/guides/arduino/programming/millis-rollover/ I 
        // can simplify the below.
        uint32_t start = millis();  // xxx - need to review how I'm timing this loop...seems clunky. Also need to take any constants and set the as #defines.
        while ((millis() < (start + 3000)) && (millis() > 4000))  // For 3 seconds, see if we can assemble received msgs and if so process them.
            mavlink_receive(&Serial2); 
        
        debugPrint("rx_from_autopilot() - Done RX - ending at Millis:"); Serial.println(millis());

        // At this point I should have gathered all of the data I needed from the streaming params, 
        // so we should turn them all back off and leave the MAVLink serial connection relatively quiet.
        mavlink_unrequest_streaming_params_from_ap(&Serial2);

        // xxx - see what we got (any magic nums still in place?) and maybe timestamp it.

        // xxx - process/store whatever we got.
        debugPrintln("rx_from_autopilot() - xxx - MORE CODE NEEDED.");
        
        seconds_since_last_ap_rx = 0;    // reset counter here at the end of the process.
        debugPrintln("rx_from_autopilot() - Complete");
        //oled.println("rx_from_autopilot() - Complete");
    }
}   // END - rx_from_autopilot()