/*
 * iridium_bypass_do_firstRXTX.ino
 */

#include "global.h"

void iridium_bypass_do_firstRXTX()
{
    uint16_t packetTempLen = 0; // temporary length of a MT packet as we start to receive one in. Includes all bytes in buffer so far.

    debugPrintln("iridium_bypass_do_firstRXTX() - BYPASS_IRIDIUM - using Serial bypass link!");

    // TX
    // TX over the bypass Serial link to my GSE32 box.
    // Note, outBufferNew[] was populated when prep_binary_MO_message_locarb() was called not long before now.
    for (int i = 0; i < outBufferNewSize; i++)
        Serial1.write(outBufferNew[i]);
    debugPrintln("iridium_bypass_do_firstRXTX() - BYPASS_IRIDIUM - TX - message has been sent out Serial port to GSE32");

    debugPrintln("iridium_bypass_do_firstRXTX() - BYPASS_IRIDIUM - TX - Fake/set the following flags...");
    debugPrintln("iridium_bypass_do_firstRXTX() - BYPASS_IRIDIUM - TX - isbdTxSucceededFlag = true");
    debugPrintln("iridium_bypass_do_firstRXTX() - BYPASS_IRIDIUM - TX - contactGroundFlag = false");
    // fake the below params to manage behaviour further below.
    isbdTxSucceededFlag = true; // set the flag as we succeeded.
    contactGroundFlag = false; // clear the flag now that we have successfully done it.
    // err = ISBD_SUCCESS; // Fake successful transmit
    send_successful = true;
    seconds_since_last_iridium_tx = 0; // reset timer as iridium tx was "successful"

    // Flash pixel Blue (for ISBD) and Green (for success) slowly for 10 secs to indicate successful ISBD send
    for (int i = 0; i < 5; i++)
    {
        pixelBlue();
        delay(1000);
        pixelGreen();
        delay(1000);
    }
    pixelOff();

    // RX
    // Check the BYPASS serial port for a message
    // Read Serial chars from GSE32 - Check for and gather Serial data arriving from GSE32, try to build a packet.
    debugPrintln("iridium_bypass_do_firstRXTX() - BYPASS_IRIDIUM - RX - see if we have received a message from GSE32");
    bool timedOut = false; // track when we have timed out.

    uint32_t start = 0;
    if (Serial1.available())
    {
        Serial.println("iridium_bypass_do_firstRXTX() - BYPASS_IRIDIUM - RX - Inbound buffer from GSE to AGT has something :)");
        start = millis(); // take a millis() timestamp when we start
        while (!timedOut && Serial1.available() && (packetTempLen == 0))
        {
            uint8_t tempChar = Serial1.read();
            packetTempLen = recPulsarParse(tempChar, BOAT_IN_BUFFER_NEW_MAX_BYTES, inBufferNew); // returns 0 until/unless we have received a full Pulsar packet,
                                                                                                 // as in recPulsarParse() has found headers and trailers etc.

            // Check if we have timed out, which will be evaluated at next pass through the while()
            if ((millis() - start) >= (RX_FROM_GSE32_TIMEOUT_S * 1000))
            {
                timedOut = true;
                debugPrintln("iridium_bypass_do_firstRXTX() - BYPASS_IRIDIUM - RX - WARNING we timed out before receiving a msg from the GSE32!");
            }
        }
    }

    // Have we RX'd a valid MT message?
    if (packetTempLen)
    {
        debugPrint("iridium_bypass_do_firstRXTX() - RX - GOT ONE, we must have a full packet as packetTempLen=");
        debugPrintlnInt(packetTempLen);
        inBufferNewSize = packetTempLen; // set the number of bytes stored in the buffer in its global size var.

        // Now, even though we know the message is valid as the recPulsarParse() code we called above,
        // here in Iridium BYPASS code, only returns non 0 for a fully recieved and checked on the way
        // in MT message. The problem is the corresponding real ISBD receive code does not validate the
        // MT message it just fills a buffer with bytes, so they have to be validated later as on of our
        // properly formatted MT messages.  So in order to test that validation code that I am marking
        // "unvalidatedRX = true" here because I want to test that validation code EVEN when I have
        // received and pre validated an MT message vis the BYPASS link.
        unvalidatedRX = true; // We have an unvalidated MT msg in inBufferNew.
    }
    else
    {
        debugPrintln("iridium_bypass_do_firstRXTX() - BYPASS IRIDIUM - RX - FAIL - we did not get a full packet. Reseting byte counters etc to start clean look for new packet next time.");
        // Because we have not been able to read in a whole MT packet in the above code,
        // we don't want to keep the bits we may have got and then keep building on them later.
        // Insted we want to discard any partial packet we may have received and reset everything
        // so that when we are called again we are starting fresh and looking for a new whole MT packet.
        // Letting the code gather a packet over multiple calls to this function proved problematic
        // in that it was hard to manage the timeout. So I decided we would work to the above principle of
        // get a whole MT packet in the above code OR if we didn't then reset etc.
        recPulsarParseReset();

        unvalidatedRX = false; // There is nothin in inBufferNew of use, to work on after here.
    }
    firstTX = false; // clear the flag that marked this as the first time through and hence first TX.

    debugPrintln("iridium_bypass_do_firstRXTX() - BYPASS_IRIDIUM - Complete");
} // END - iridium_bypass_do_firstRXTX()
