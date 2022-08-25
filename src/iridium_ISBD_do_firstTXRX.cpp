/*
 * iridium_ISBD_do_firstRXTX.ino 
 */

 #include "global.h"

void iridium_ISBD_do_firstRXTX()
{
    int err = 0;                    // general error tracking within this function
    int signalQuality = -1;     // ISBD signal quality - xxx - what???

    debugPrintln("iridium_ISBD_do_firstRXTX() - Starting.");
 
    //SIGNAL QUALITY - This returns a number between 0 and 5. 2 or better is preferred.
    err = isbdModem.getSignalQuality(signalQuality);
    if (err != ISBD_SUCCESS)
    {
        Serial.print("iridium_ISBD_do_firstRXTX() - ISBD getSignalQuality() - FAILED with error: ");Serial.println(err);
        // xxx - if this fails shouldn't we take some action? Or what if the returned signal quality is too low???
    }      
    else
    {
        Serial.print("iridium_ISBD_do_firstRXTX() - ISBD getSignalQuality() - SUCCEEDED - On a scale of 0 to 5, signal quality is currently ");Serial.println(signalQuality);
    }

    // TX binary formatted message and potentially simultaneously RX a message over ISBD
    num_iridium_tx_attempts++;  // increment the TX attempts counter each time we try a TX.
    #ifndef SKIP_IRIDIUM_TX
        // We are NOT skipping the Iridium TX/RX
        debugPrintln("iridium_ISBD_do_firstRXTX() - doing modem.sendReceiveSBDBinary(......)");
        err = isbdModem.sendReceiveSBDBinary(outBufferNew, outBufferNewSize, inBufferNew, inBufferNewSize); // This could take many seconds to complete and will call ISBDCallback() periodically
    #else
        // We ARE skipping the Iridium TX/RX
        debugPrintln("iridium_ISBD_do_firstRXTX() - SKIP_IRIDIUM_TX is defined, so not actually ISBD TX/RX'ing, just fake failing that.");
        err = 66; // a made up error code only used here with the SKIP_IRIDIUM_TX
        inBufferNewSize = 0;  // set as if we did not receive a message...which we didn't because we faked the Iridium TX/RX.
    #endif
    
    // Evaluate error/success of Iridium TX/RX attempt
    if (err == 0)
    {
        debugPrintln("\niridium_ISBD_do_firstRXTX() - ISBD TX/RX Succeeded");
        debugPrintln("iridium_ISBD_do_firstRXTX() -  so setting the following flags...");
        debugPrintln("iridium_ISBD_do_firstRXTX() - isbdTxSucceededFlag = true");
        debugPrintln("iridium_ISBD_do_firstRXTX() - contactGroundFlag = false");
        isbdTxSucceededFlag = true;  // set the flag as we succeeded. 
        contactGroundFlag = false;  // clear the flag now that we have successfully done it.
        seconds_since_last_iridium_tx = 0;  // reset timer as iridium tx was successful
        send_successful = true;
        firstTX = false;  // clear the flag that marked this as the first time through and hence first TX.
        
        // Flash pixel Blue (for ISBD) and Green (for success) slowly for 10 secs to indicate successful ISBD send
        for (int i = 0; i < 5; i++)
        {
            pixelBlue();
            delay(1000);
            pixelGreen();
            delay(1000);
        }
        pixelOff();

        if (inBufferNewSize != 0)
            unvalidatedRX = true;       // set flag as we got a buffer in, but we don't know yet if its a valid MT message.
        else
            unvalidatedRX = false;      // explicitly clear flag to be tidy.
    }
    else
    {
        Serial.print("iridium_ISBD_do_firstRXTX() - ISBD TX/RX FAILED with status code:");Serial.println(err);
        Serial.print("iridium_ISBD_do_firstRXTX() - num_iridium_tx_attempts so far:");Serial.println(num_iridium_tx_attempts);
        isbdTxSucceededFlag = false;   // ensure the flag is cleared as we FAILED. 
        contactGroundFlag = false;  // clear the flag now that we have successfully done it.
        unvalidatedRX = false;          // explicitly clear flag to be tidy.
        
        // Flash pixel Blue (for ISBD) and Red (for failure) slowly for 10 secs to indicate failed ISBD send
        for (int i = 0; i < 5; i++)
        {
            pixelBlue();
            delay(1000);
            pixelRed();
            delay(1000);
        }
        pixelOff();

        seconds_since_last_iridium_tx = 0;  // reset timer as iridium tx's attempted but have failed.
    }  
    
    debugPrintln("iridium_ISBD_do_firstRXTX() - Finished.");

}   // END - iridium_ISBD_do_firstRXTX()