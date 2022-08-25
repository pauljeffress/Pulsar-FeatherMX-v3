/*
 * iridium_ISBD_do_additionalRXs.cpp
 */

 #include "global.h"

void iridium_ISBD_do_additionalRXs()
{
    int err;                    // general error tracking within this function

    debugPrintln("iridium_ISBD_do_additionalRX() - Starting.");
 
    // Send NULL message to allow as us to continue to Receive over ISBD
    #ifndef SKIP_IRIDIUM_TX
        debugPrintln("iridium_ISBD_do_additionalRX() - doing modem.sendReceiveSBDText(NULL, inBufferNew, inBufferNewSize)");
        err = isbdModem.sendReceiveSBDText(NULL, inBufferNew, inBufferNewSize); // Simultaneously TX a NULL message and try to RX a message. 
                                                                            // This could take many seconds to complete and will call ISBDCallback() periodically                                                                  
    #else
        debugPrintln("iridium_ISBD_do_additionalRX() - SKIP_IRIDIUM_TX is defined, so not actually ISBD TX/RX'ing, just fake failing that.");
        err = 66; // a made up error code only used here with the SKIP_IRIDIUM_TX
        inBufferNewSize = 0;  // set as if we did not receive a message...which we didn't because we faked the Iridium TX/RX.
    #endif

    if (inBufferNewSize != 0)
        unvalidatedRX = true;       // set flag as we got a buffer in, but we don't know yet if its a valid MT message.
    else
        unvalidatedRX = false;      // explicitly clear flag to be tidy.  

    if (err == 0)
        Serial.println("\niridium_ISBD_do_additionalRX() - ISBD TX(null)/RX Succeeded");
    else
    {
        Serial.print("\niridium_ISBD_do_additionalRX()) - ISBD TX(null)/RX FAILED with status code:");Serial.println(err);
        Serial.print("iridium_ISBD_do_additionalRX() - countTXRXlaps so far:");Serial.println(countTXRXlaps);
    } 

    debugPrintln("iridium_ISBD_do_additionalRX() - Finished.");

}   // END - iridium_ISBD_do_additionalRXs()