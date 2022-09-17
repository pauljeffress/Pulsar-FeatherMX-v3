/*
 * isbdDoAdditionalRxs.cpp
 */

#include "global.h"

/*
 * May be called a number of times by do_iridium_locarb() to get all remaining
 * MT messages (after the first TX/RX by isbdDoFirstRxTx()) that are waiting
 * in the ISBD network. Remember you need to TX in order to RX, so in this
 * instance we TX a null msg, so we can still RX.
 */
void isbdDoAdditionalRxs()
{
    int err = 0; // general error tracking within this function

    debugPrintln("isbdDoAdditionalRxs() - Starting.");
    oled.println("ISBD next TXRX Start");

    // Send NULL message to allow as us to continue to Receive over ISBD
#ifndef SKIP_IRIDIUM_TX
    // We are NOT skipping the Iridium TX/RX
    debugPrintln("isbdDoAdditionalRxs() - doing modem.sendReceiveSBDText(NULL, inBufferNew, inBufferNewSize)");
    err = isbdModem.sendReceiveSBDText(NULL, inBufferNew, inBufferNewSize); // Simultaneously TX a NULL message and try to RX a message.
                                                                            // This could take many seconds to complete and will call ISBDCallback() periodically
#else
    // We ARE skipping the Iridium TX/RX
    debugPrintln("isbdDoAdditionalRxs() - SKIP_IRIDIUM_TX is defined, so not actually ISBD TX/RX'ing, just fake failing that.");
    oled.println("BYPASSing ISBD");
    err = 66;            // a made up error code only used here with the SKIP_IRIDIUM_TX
    inBufferNewSize = 0; // set as if we did not receive a message...which we didn't because we faked the Iridium TX/RX.
#endif

    // Evaluate error/success of Iridium TX/RX attempt
    if (err == 0)
    {
        debugPrintln("\nisbdDoAdditionalRxs() - ISBD TX/RX Succeeded");
        debugPrintln("isbdDoAdditionalRxs() - so setting the following flags...");
        debugPrintln("isbdDoAdditionalRxs() - isbdTxSucceededFlag = true");
        debugPrintln("isbdDoAdditionalRxs() - contactGroundFlag = false");
        oled.println("ISBD Success");
        // I have commented out the below, as we may need to TX/RX again and should not waste time
        // because the current sat in view may disappear.
        // // Flash pixel Magenta (for ISBD) and Green (for success) slowly for 10 secs to indicate successful ISBD send
        // for (int i = 0; i < 5; i++)
        // {
        //     pixelMagenta();
        //     delay(1000);
        //     pixelGreen();
        //     delay(1000);
        // }
        // pixelOff();

        if (inBufferNewSize != 0)
            isbdUnvalidatedRx = true; // set flag as we got a buffer in, but we don't know yet if its a valid MT message.
        else
            isbdUnvalidatedRx = false; // explicitly clear flag to be tidy.
    }
    else
    {
        Serial.print("isbdDoAdditionalRxs() - ISBD TX/RX FAILED with status code:");
        Serial.println(err);
        oled.print("ISBD Fail err:");
        oled.println(err);
        Serial.print("isbdDoAdditionalRxs() - isbdNumTxAttempts so far:");
        Serial.println(isbdNumTxAttempts);
        oled.print("ISBD Trys: ");
        oled.println(isbdNumTxAttempts);
        Serial.print("isbdDoFirstRxTx() - isbdCountTxRxLaps so far:");
        Serial.println(isbdCountTxRxLaps);
        oled.print("ISBD Laps: ");
        oled.println(isbdCountTxRxLaps);
        isbdUnvalidatedRx = false;   // explicitly clear flag to be tidy.
        // Flash pixel Magenta (for ISBD) and Red (for failure) slowly for 10 secs to indicate failed ISBD send
        for (int i = 0; i < 5; i++)
        {
            pixelMagenta();
            delay(1000);
            pixelRed();
            delay(1000);
        }
        pixelOff();

        seconds_since_last_iridium_tx = 0; // reset timer as iridium tx's attempted but have failed.
    }

    debugPrintln("isbdDoAdditionalRxs() - Finished.");

} // END - isbdDoAdditionalRxs()