/*
 * isbdDoFirstRxTx.ino
 */

#include "global.h"

/*
 * Is used once per "contact_ground()" to do a TX and possible RX.
 * If there are any further MT messages waiting in the ISBD network
 * then we use isbdDoAdditionalRxs() to get them.
 */
void isbdDoFirstRxTx()
{
    int err = 0;            // general error tracking within this function
    int signalQuality = -1; // ISBD signal quality - xxx - what???

    debugPrintln("isbdDoFirstRxTx() - Starting.");
    oled.println("ISBD 1st TXRX Start");

    // SIGNAL QUALITY - This returns a number between 0 and 5. 2 or better is preferred.
    err = isbdModem.getSignalQuality(signalQuality);
    if (err != ISBD_SUCCESS)
    {
        Serial.print("isbdDoFirstRxTx() - ISBD getSignalQuality() - FAILED with error: ");
        Serial.println(err);
        oled.println("getSigQual FAILed");
        // xxx - if this fails shouldn't we take some action? Or what if the returned signal quality is too low???
    }
    else
    {
        Serial.print("isbdDoFirstRxTx() - ISBD getSignalQuality() - SUCCEEDED - On a scale of 0 to 5, signal quality is currently ");
        Serial.println(signalQuality);
        oled.print("getSigQual=");
        oled.println(signalQuality);
    }

    // TX a binary formatted message and potentially simultaneously RX a message over ISBD
    isbdNumTxAttempts++; // increment the TX attempts counter each time we try a TX.
#ifndef SKIP_IRIDIUM_TX
    // We are NOT skipping the Iridium TX/RX
    debugPrintln("isbdDoFirstRxTx() - doing modem.sendReceiveSBDBinary(......)");
    err = isbdModem.sendReceiveSBDBinary(outBufferNew, outBufferNewSize, inBufferNew, inBufferNewSize); // This could take many seconds to complete and will call ISBDCallback() periodically
#else
    // We ARE skipping the Iridium TX/RX
    debugPrintln("isbdDoFirstRxTx() - SKIP_IRIDIUM_TX is defined, so not actually ISBD TX/RX'ing, just fake failing that.");
    oled.println("BYPASSing ISBD");
    err = 66;            // a made up error code only used here with the SKIP_IRIDIUM_TX
    inBufferNewSize = 0; // set as if we did not receive a message...which we didn't because we faked the Iridium TX/RX.
#endif

    // Evaluate error/success of Iridium TX/RX attempt
    if (err == 0)
    {
        debugPrintln("\nisbdDoFirstRxTx() - ISBD TX/RX Succeeded");
        debugPrintln("isbdDoFirstRxTx() -  so setting the following flags...");
        debugPrintln("isbdDoFirstRxTx() - isbdTxSucceededFlag = true");
        debugPrintln("isbdDoFirstRxTx() - contactGroundFlag = false");
        isbdTxSucceededFlag = true;        // set the flag as we succeeded.
        contactGroundFlag = false;         // clear the flag now that we have successfully done it.
        seconds_since_last_iridium_tx = 0; // reset timer as iridium tx was successful
        isbdSendSuccessful = true;
        isbdFirstTx = false; // clear the flag that marked this as the first time through and hence first TX.
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
        Serial.print("isbdDoFirstRxTx() - ISBD TX/RX FAILED with status code:");
        Serial.println(err);
        oled.print("ISBD Fail err:");
        oled.println(err);
        Serial.print("isbdDoFirstRxTx() - isbdNumTxAttempts so far:");
        Serial.println(isbdNumTxAttempts);
        oled.print("ISBD Trys: ");
        oled.println(isbdNumTxAttempts);
        Serial.print("isbdDoFirstRxTx() - isbdCountTxRxLaps so far:");
        Serial.println(isbdCountTxRxLaps);
        oled.print("ISBD Laps: ");
        oled.println(isbdCountTxRxLaps);
        isbdTxSucceededFlag = false; // ensure the flag is cleared as we FAILED.
        contactGroundFlag = false;   // clear the flag now that we have successfully done it.
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

    debugPrintln("isbdDoFirstRxTx() - Finished.");

} // END - isbdDoFirstRxTx()