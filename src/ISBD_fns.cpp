/*
 * ISBD_fns.cpp
 *
 * ISBD support functions
 *
 *
 */

#include "global.h"


// Globals
IridiumSBD isbdModem(Serial1, IRIDIUM_SLEEP_PIN, IRIDIUM_RING_INDICATOR_PIN); // Declare the IridiumSBD object (including the sleep (ON/OFF) and Ring Indicator pins)

// Functions

/*
 * At FMX boot/reset we fire up the ISBD 9603 modem and make sure
 * we can talk to it etc.
 * Then we shut it down (remove power), as it will be powered up agin
 * when its needed.
 */
void ISBDSetup()
{
    // Note, Serial1.begin(19200) MUST already be executed in serialSetup() prior to this function running.
    debugPrintln("ISBDSetup() - Starting");
    prep_iridium_modem();
    iridiumOFF();
    // Note: We are not setting any flags as to the state of the modem, because even if it fails here,
    //       we want subsequent code to try an initialise it again when its needed, as we have nothing
    //       to loose by trying again.
    debugPrintln("ISBDSetup() - Complete");
} // END - ISBDSetup()

/*
 * IridiumSBD Callback - this code is called often by the ISBD library
 * while the 9603 is doing things...
 * i.e during the execution of functions like modem.sendReceiveSBDBinary()
 * So we can see whats happening and take action (flash LED etc) if need be.
 * If we want the ISBD library to stop doing whatever its doing, we just need to
 * return FALSE, and it will stop shortly thereafter. Otherwise we should always
 * return TRUE.
 */
bool ISBDCallback()
{
    // Flash the onboard NeoPixel at 4 Hz
    if ((millis() / 250) % 2 == 1)
        pixelMagenta();
    else
        pixelOff();

    // Write alternating chars to Serial Monitor every second so we know things are still happening
    if ((millis() / 1000) % 2 == 1)
        debugPrint("+");
    else
        debugPrint("-");

    // Check the battery voltage now we are drawing current for the 9603
    check_power(true);                             // Read the 9603N bus voltage, and ask function to be silent
    if (myFmxSettings.FMX_BATT_V < FMX_BATT_LOW_V) // If voltage is low, stop Iridium send
    {
        Serial.print("***!!! LOW VOLTAGE (ISBDCallback): ");
        Serial.print(((float)myFmxSettings.FMX_BATT_V/100.0),2);
        Serial.println("v !!!***");
        // xxx - we should log this to the OLA.
        return false; // Returning false causes IridiumSBD to terminate
    }
    else
        return true;
} // End of ISBDCallback()

/*
 * The ISBD libraries diagnostics functions.
 */
#ifdef ENABLE_ISBD_DIAGNOSTICS
void ISBDConsoleCallback(IridiumSBD *device, char c)
{
    Serial.write(c);
}
void ISBDDiagsCallback(IridiumSBD *device, char c)
{
    Serial.write(c);
}
#endif

/*
 *  Enable the Iridium 9603N modem power
 */
void iridiumON()
{
    debugPrintln("iridiumON() - executed");
    digitalWrite(IRIDIUM_PWR_FET_PIN, HIGH);
}

/*
 *  Disable the Iridium 9603N modem power
 */
void iridiumOFF()
{
    debugPrintln("iridiumOFF() - executed");
    digitalWrite(IRIDIUM_PWR_FET_PIN, LOW);
}

/*
 * Turn on power to 9603N, enable the iridiumSerial (FMX to 9603N) port, set modem settings etc.
 * Return boolean for success or failure
 */
boolean prep_iridium_modem()
{
    debugPrintln("prep_iridium_modem() - Starting");

    boolean prepResult = false; // track status of this preparation work.
    uint8_t err;                // general error tracking within this function

    debugPrintln("prep_iridium_modem() - Enabling Iridium Modem power...and waiting 5 seconds");
    iridiumON(); // Turn on power to the Iridium modem.
    delay(5000); // Give the iridium modem a few seconds to power up.

    debugPrintln("prep_iridium_modem() - Start the serial port connected to the Iridium Modem");
    Serial1.begin(19200);
    delay(1000); // Allow time for the serial port to come up.

    /* Relax timing constraints waiting for the supercap to recharge.
     * USB_POWER_PROFILE means assume we can't charge the supercaps fast as the whole project
     * is running off USB power, so the 9603 and it's supercaps can prob only pull a few 100 mA
     * so they will need plenty of time to charge up between transmissions etc.
     * See https://github.com/sparkfun/SparkFun_IridiumSBD_I2C_Arduino_Library/tree/main/documentation#power-considerations
     */
    isbdModem.setPowerProfile(IridiumSBD::USB_POWER_PROFILE);

    // Begin satellite modem operation
    debugPrintln("prep_iridium_modem() - Starting Iridium Modem...");
    err = isbdModem.begin();
    // Check if the modem started correctly
    if (err != ISBD_SUCCESS)
    {
        // If the modem failed to start...   xxx - should we take more action here?
        debugPrintln("prep_iridium_modem() - ERROR isbdModem.begin FAILED with error ");
        debugPrintlnInt(err);
        debugPrintln("!!!***");
        iridiumOFF(); // Turn off power to the Iridium modem.
        prepResult = false;
    }
    else
    {
        debugPrintln("prep_iridium_modem() - isbdModem.begin SUCCEEDED");
        prepResult = true;
    }
    debugPrintln("prep_iridium_modem() - Complete");
    return prepResult;

} // END - prep_iridium_modem()

/*
 * prep an empty inBufferNewLatest.
 */
void zero_inBufferNewLatest()
{
    // debugPrintln("zero_inBufferNewLatest() - zero out inBufferNewLatest[]");
    for (int i = 0; i <= (GENERIC_BUFFER_NEW_MAX_BYTES - 1); i++) // zero out the buffer
        inBufferNewLatest[i] = 0;
    inBufferNewLatestSize = 0; // reset size variable to zero to indicate an empty buffer.
}

/*
 * prep an empty inBufferNew.
 */
void zero_inBufferNew()
{
    // debugPrintln("zero_inBufferNew() - zero out inBufferNew[]");
    for (int i = 0; i <= (GENERIC_BUFFER_NEW_MAX_BYTES - 1); i++) // zero out the buffer
        inBufferNew[i] = 0;
    inBufferNewSize = 0; // reset size variable to zero to indicate an empty buffer.
}

/*
 *  check and validate a received MT msg in inBufferNew[]
 */
boolean check_MT_msg_iridium()
{
    boolean prepResult = false; // track status of this preparation work.
    uint16_t len = 0;

    debugPrintln("check_MT_msg_iridium() - Starting - validation checking of inBufferNew[]");

    // Note: At this point we have inBufferNew[] supposedly containing a full MT message
    //       AND we have inBufferNewSize = num bytes received. (should be 6Byte header + Paload Bytes + 4Byte trailer)

    // 1st test
    // check the start delimiter, length, end delimiters and checksum.
    if ((inBufferNew[0] == BUFFER_START_DELIM_1ST) && (inBufferNew[1] == BUFFER_START_DELIM_2ND)) // are start delimiters correct?
        prepResult = true;                                                                        // so far so good.
    else
    {
        prepResult = false; // we failed this test.
        debugPrintln("check_MT_msg_iridium() - FAILED 1st test");
    }
    // 2nd test
    if (prepResult) // i.e. we passed the previous test, so execute the next test.
    {
        // check the length is suitable.
        len = inBufferNew[2] + (256 * inBufferNew[3]);                                             // recreate length from received msg
        if ((len <= (BOAT_IN_BUFFER_NEW_MAX_BYTES - 4 - 6)) && ((len + 6 + 4) == inBufferNewSize)) // do two tests on it here.
                                                                                                   // Test 1.
                                                                                                   // Is the length value stored in the packet at most the max size we ever expect,
                                                                                                   // which is IN_BUFFER_NEW_MAX_BYTES minus 6 header bytes and minus 4 trailer bytes.
                                                                                                   // Test 2.
                                                                                                   // Is the payload len what we expect based on the number of bytes we have received in the message?
                                                                                                   // i.e. it should be 10 less than inBufferNewSize.

            prepResult = true; // so far so good.
        else
        {
            prepResult = false; // we failed this test.
            debugPrintln("check_MT_msg_iridium() - FAILED 2nd test");
        }
    }

    // 3rd test
    if (prepResult) // i.e. we passed the previous test, so execute the next test.
    {
        // check the two end delimiters are in place.
        if ((inBufferNew[inBufferNewSize - 2] == BUFFER_END_DELIM_1ST) && (inBufferNew[inBufferNewSize - 1] == BUFFER_END_DELIM_2ND)) // are end delimiters correct and where we expect them?
            prepResult = true;                                                                                                        // so far so good.
        else
        {
            prepResult = false; // we failed this test.
            debugPrintln("check_MT_msg_iridium() - FAILED 3rd test");
            Serial.print("inBufferNewSize = ");
            Serial.println(inBufferNewSize);
            Serial.print("inBufferNew[inBufferNewSize-2] = ");
            Serial.println(inBufferNew[inBufferNewSize - 2], HEX);
            Serial.print("inBufferNew[inBufferNewSize-1] = ");
            Serial.println(inBufferNew[inBufferNewSize - 1], HEX);
        }
    }

    // 4th test
    if (prepResult) // i.e. we passed the previous test, so execute the next test.
    {
        // check the checksum is correct.
        // we do this by calc'ing csum of buffer from 1st start delim byte to last payload byte
        // Last payload byte is at byte inBufferNewSize - 5
        uint32_t calcCsumA = 0;
        uint32_t calcCsumB = 0;
        for (int i = 0; i < (inBufferNewSize - 4); i++)
        {
            calcCsumA = calcCsumA + inBufferNew[i];
            calcCsumB = calcCsumB + calcCsumA;
        }
        // Convert calculated checksums into bytes ready for comparison
        uint8_t tempCsumAByte = (byte)(calcCsumA & 0x000000ff);
        uint8_t tempCsumBByte = (byte)(calcCsumB & 0x000000ff);
        // Compare the checksum bytes in the packet itself with my calculated checksum bytes
        if ((inBufferNew[inBufferNewSize - 4] == tempCsumAByte) && (inBufferNew[inBufferNewSize - 3] == tempCsumBByte))
            prepResult = true; // so far so good.
        else
        {
            prepResult = false; // we failed this test.
            debugPrintln("check_MT_msg_iridium() - FAILED 4th test");
        }
    }

    if (prepResult)
        Serial.println("check_MT_msg_iridium() - inBufferNew[] contains a valid MT message.");
    else
    {
        debugPrintln("check_MT_msg_iridium() - ERROR - inBufferNew[] IS NOT a valid MT message.");
        gotMsgFromGroundFlag = false; // clear the flag as we did NOT get a valid message from ground.
    }

    debugPrintln("check_MT_msg_iridium() - Complete");

    return prepResult;

} // END - check_MT_msg_iridium()

/*
 *  Store the successfully received and basically validated new MT msg we received
 */
void store_received_MT_msg()
{
    debugPrintln("store_received_MT_msg() - executing - RX'd inBufferNew copied to inBufferNewLatest");
    gotMsgFromGroundFlag = true;   // set the flag as we did get a valid message from ground.
    seconds_since_last_iridium_rx = 0; // reset timer as iridium rx was successful
    // Note: at this point the most recently received MT message is copied into inBufferNewLatest
    //       to ensure it is not zero'd out if/when we do another attempted RX. The idea being
    //       that we only want to keep the last successfully received and validated MT message.
    for (int i = 0; i <= (BOAT_IN_BUFFER_NEW_MAX_BYTES - 1); i++) // copy the buffer byte by byte.
        inBufferNewLatest[i] = inBufferNew[i];
    inBufferNewLatestSize = inBufferNewSize; // set size variable to match the original we just copied from.
} // END - store_received_MT_msg()

/*
 * Clear the Mobile Originated message buffer to avoid re-sending the message during subsequent loops
 */
void clear_mo_buffer()
{
#ifndef BYPASS_IRIDIUM // i.e. we are using the Iridium
    int err;           // general error tracking within this function
    debugPrintln("clear_mo_buffer() - Clearing the MO buffer.");
    err = isbdModem.clearBuffers(ISBD_CLEAR_MO); // Clear MO buffer
    if (err != ISBD_SUCCESS)
    {
        debugPrintln("clear_mo_buffer() - ***!!! modem.clearBuffers failed with error ");
        debugPrintlnInt(err);
        debugPrintln("!!!***");
    }
#endif
}

/*
 * Update the global num_waiting_messages, which counts how many MT messages are still waiting to be RX'd by boat.
 */
void update_waiting_msgs_iridium()
{
#ifdef BYPASS_IRIDIUM // Bypassing Iridium
    debugPrintln("update_waiting_msgs_iridium() - BYPASS_IRIDIUM - so setting num waiting msgs to 0");
    num_waiting_messages = 0; // Fake the remaining message count
#else                         // Using Iridium
    num_waiting_messages = isbdModem.getWaitingMessageCount();
#endif
    debugPrint("update_waiting_msgs_iridium() - The number of messages in the MT queue is:");
    debugPrintlnInt(num_waiting_messages);
    if (num_waiting_messages == 255)
    {
        debugPrint("update_waiting_msgs_iridium() - As it is 255, that means none, setting to 0");
        num_waiting_messages == 0;
    }
} // END - update_waiting_msgs_iridium()

/*
 *  Called after an ISBD TX/RX session, to display variables endstate.
 */
void print_iridium_endstate()
{
    // once we get to here, we are finishing up, we will not be ISBD TX/RX'ing any more for now.
    debugPrintln("print_iridium_endstate() - exited the ISBD TX/RX while() loop");
    debugPrintln("print_iridium_endstate() - Values at exit are...");
    debugPrint("print_iridium_endstate() - num_waiting_messages=");
    debugPrintlnInt(num_waiting_messages);
    debugPrint("print_iridium_endstate() - send_successful=");
    debugPrintlnInt(send_successful);
    debugPrint("print_iridium_endstate() - num_iridium_tx_attempts=");
    debugPrintlnInt(num_iridium_tx_attempts);
    debugPrint("print_iridium_endstate() - countTXRXlaps=");
    debugPrintlnInt(countTXRXlaps);
    if (gotMsgFromGroundFlag)
    {
        debugPrint("print_iridium_endstate() - WE RECV'D VALID MT Message - inBufferNewLatestSize=");
        debugPrintlnInt(inBufferNewLatestSize);
    }
} // END - print_iridium_endstate()

/*
 *  Put the 9603N into sleep mode
 */
void sleep_iridium_modem()
{
#ifndef BYPASS_IRIDIUM
    // xxx - do we need to do this given we are about to turn power off to the Iridium Modem????
    int err; // general error tracking within this function
    debugPrintln("sleep_iridium_modem() - Putting the Iridium modem to sleep.");
    err = isbdModem.sleep();
    if (err != ISBD_SUCCESS)
    {
        debugPrint("sleep_iridium_modem() - ***!!! modem.sleep failed with error:");
        debugPrintlnInt(err);
        debugPrintln("!!!***");
    }
#endif
} // END - sleep_iridium_modem()