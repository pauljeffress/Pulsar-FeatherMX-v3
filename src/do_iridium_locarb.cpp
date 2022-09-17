/*
 * do_iridium_locarb.cpp
 *
 * Code to execute when doing Iridium TX/RX
 *
 * This code is based on both the original Sparkfun Artemis Global Tracker example code
 * AND the locarb (https://www.locarbftw.com) way of packing and TX/RX'ing data over ISBD.
 *
 * See do_iridium_oldway.ino for how I used to do it all the Sparkfun example way before brining in locarb stuff.
 *
 */

#include "global.h"

// Notes
// 1. The #define BYPASS_IRIDIUM in global.h has big involvement here. If it's defined we reroute the data we would
//    normally TX/RX with the Ground Station via ISBD, to be sent via the test Serial link to the GSE32 instead.
//    The supercap charging and the ISBD radio are not turned on at all.
//
// 2. The #define SKIP_IRIDIUM_TX in global.h has a similar role but it only causes the ISBD radio not to actually
//    TX/RX, everything else (eg. supercap charging etc) still execute.

/*
 * Code to actually do the Iridium TX/RX process.
 *
 * Remember, we only TX one MO message to ground, but there may be more than one MT message
 * waiting in the ISBD system for us to collect.
 *
 * So, first (and only once) we...
 * Attempt to send the Mobile Originated (MO) message thats in outBufferNew[] to Ground.
 *
 * And then (potentially multiple times) we...
 * Attempt to receive any and all Mobile Terminated (MT) message(s) from Ground into inBufferNew[].
 * If multiple MT messages are received, only the last one is kept as it is consider to be most
 * recently sent from Ground and hence contain the most current instructions.  We do not allow for
 * the action of multiple MT messages here.
 */
void do_iridium_locarb()
{
    bool iridiumOk = false; // flag used inside this function to manage errors along the way.

    // Initialise relevant globals for the start of this function.
    isbdNumWaitingMessages = 1;     // xxxx - why do we set this to 1 to start?
    isbdCountTxRxLaps = 0;          // reset counter
    isbdSendSuccessful = false;     // assume false until explicitly set to true.
    isbdNumTxAttempts = 0;          // reset counter
    isbdFirstTx = true;             // we have only just entered this function so we must be about to do first TX.
    isbdValidRx = false;            // assume false until explicitly set to true.
    isbdUnvalidatedRx = false;      // assume false until explicitly set to true.

    debugPrintln("do_iridium_locarb() - Starting");

    prep_binary_MO_message_locarb(); // Construct the binary message (in outBufferNew[]) that we are going to send to Ground.

    printoutBufferNew();

#ifdef BYPASS_IRIDIUM
    iridiumOk = true; // as we are not actually using the Iridium 9603N modem, we can just fake this.
#else
    iridiumOk = prep_iridium_modem(); // Get serial link to 9603N and the 9603N itself ready
#endif

    if (!iridiumOk)
    {
        debugPrintln("do_iridium_locarb() - ERROR - we were unable to successfully prep the Iridium modem!");
    }
    else // i.e. iridiumOK = true
    {
        /* ========================================================
         * Send the prepared message & Receive any waiting message(s)
         * Remember the ISBD protocol flow is Boat sends a message, even a NULL message (so as to not spend a credit)
         * and then you can check and Receive an ISBD message (only one). You need to keep sending NULL messages so
         * you can keep checking if there are any more messages to receive.
         * ========================================================*/

        debugPrintln("do_iridium_locarb() - entering the ISBD TX/RX while() loop");

        zero_inBufferNewLatest();

        /*
         * The following while() loop has a number of controlling conditions.
         * 1. (Keep going around until there are no more inbound ISBD MT messages available to RX.
         *   OR
         * 2. When doing the TX, if it does not succeed, try the TX again up to MAX_ISBD_TX_TRYS times.)
         *   AND
         * 3. As an over riding rule, only ever go around the ISBD TX/RX loop a maximum of MAX_ISBD_LAPS times. This is a bit of a failsafe hack.
         */
        while (((isbdNumWaitingMessages > 0) || (!isbdSendSuccessful && isbdNumTxAttempts <= MAX_ISBD_TX_TRYS)) && (isbdCountTxRxLaps < MAX_ISBD_LAPS))
        {
            debugPrintln("do_iridium_locarb() - >>>>>>>>>>> TOP OF WHILE LOOP <<<<<<<<<<");
            isbdCountTxRxLaps++;       // increment lap counter each time around the while() loop.
            isbdUnvalidatedRx = false; // clear flag ready for use soon on each lap through this while()

            zero_inBufferNew();

            /*
             * The first time around this while() we TX and RX, after that we RX only.
             */
            if (isbdFirstTx == true)
            {
                debugPrintln("do_iridium_locarb() - isbdFirstTx=true");
                isbdFirstTx = false; // we only wanted to use this flag once, so clear it straight after use
#ifdef BYPASS_IRIDIUM
                iridium_bypass_do_firstRXTX();
#else
                isbdDoFirstRxTx();
#endif
            }

            /*
             * This is only called to do 2nd, 3rd... RX, after the 1st TX/RX was successful.
             */
            else // This is not the first time around the while loop, so we are RX'ing only from now on.
                 // There may be multiple Mobile Terminated messages waiting for us, all queued up, so we
                 // continue RXing (with a limit) to get them all.
                 // Note: Because you have to TX before you can RX with ISBD, we send a NULL message (to avoid wasted credits)
            {
                debugPrintln("do_iridium_locarb() - because isbdFirstTx=false we are executing more RXs");
#ifndef BYPASS_IRIDIUM // We do not do multiple RX's when using the BYPASS link, the reason is non trivial and
                       // is explained inn comments further below under the "if (isbdValidRx)" code.
                isbdDoAdditionalRxs();
#endif
            }

            if (isbdUnvalidatedRx)                    // Was a potential MT message received (from either ISBD or Serial BYPASS)?
                isbdValidRx = isbdCheckMtMsg(); // If so, check if was valid MT formatted message.
            else
                isbdValidRx = false; // explicitly clear the flag to be tidy.

            if (isbdValidRx)
            {
                // During bench testing via BYPASS path, when a 2nd ISBD MT message arrives at the AGT
                // Serial (I2C/UART hw port), but the 1st one has not been read in by my parser,
                // it looks like the MCU just drops any bytes it can't store in its HW (or sw driver)
                // buffer. So I was ending up with 1.5 MT messages in that HW buffer and then when I
                // tried to parse in the 2nd message it was messy.
                // That is unlikely to happen in real
                // life (i.e. over ISBD) as in real life the AGT will start the modem, be sent the first message,
                // (even if the ISBD system has two messages), it will read it in and hence empty the HW buffer
                // before requesting any more MT messages.
                // To avoid the problem when doing BYPASS path testing, I will drain any bytes remaining in the
                // serial hw buffer now.
                while (Serial1.available())
                    Serial1.read();

                store_received_MT_msg();    // Copies current inBufferNew into inBufferNewLatest and
                                            // sets inBufferNewLatestSize = inBufferNewSize.
                printinBufferNew();         // Hex dumps the recently received inBufferNew to screen

                debugPrintln("do_iridium_locarb() - we received a valid ISBD MT message. So setting the following flags...");
                debugPrintln("do_iridium_locarb() - gotMsgFromGroundFlag = true");
                gotMsgFromGroundFlag = true; // (C) So main state machine knows we got one
                                                 // and we need to process/action it.
                // At this point a validated MT message is in
                // inBufferNewLatest[] with size inBufferNewLatestSize.
            }

            clear_mo_buffer();  // We always clear the MO message buffer after use.

            update_waiting_msgs_iridium();  // gets this from data we already have in the isbdModem object, if the TX/RX worked earlier.

        } // End of the big "while ((isbdNumWaitingMessages > 0).....)" loop where we tx/rx messages

        // XXXXXXXXXXXX - Once we get to here, we have done all we are going to do with ISBD TX/RX for this TXINT period. We have attempted
        // the TX and attempted as many RX's as there are MT msgs waiting for us.
        // Note: Only the last RX'd MT msg is kept and its stored in inBufferNewLatest[] with size inBufferNewLatestSize and
        //       subsequently processed.  We do not process multiple MT's, just the last one and we drop any others.

        print_iridium_endstate();

        sleep_iridium_modem();

    } // END - else (!iridiumOk)

    debugPrintln("do_iridium_locarb() - Disabling Iridium Modem power");
    iridiumOFF(); // <---DO NOT REMOVE THIS - To save power, always disable Iridium Power at the end of this state
                  // Even if we are BYPASS'ing, just to be super safe, we will still turn it off.

    debugPrintln("do_iridium_locarb() - Complete");
} // END - do_iridium_locarb()
