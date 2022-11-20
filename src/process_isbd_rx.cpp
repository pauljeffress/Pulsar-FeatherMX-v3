/*
 * process_isbd_rx.ino
 * 
 */

 #include "global.h"
 #include "doFmxCmd_fns.h"

/*
 * IF we received an MT msg from ground in the previous ISBD TX/RX, and it was validated, then it would be
 * stored in inBufferNewLatest[] with size inBufferNewLatestSize.
 * We are then called to process and action that MT message.
 * Remember, some of the MT params are for the FMX itself, some are for the AP.
 *
 */
void case_process_isbd_rx()
{
    
    debugPrintln("\ncase_process_isbd_rx() - Starting");
    oled.println("process_isbd_rx()");
    
    pulsar_mt_msg_init();       // zeros out both pulsar_mt_msg and global_mission_subset, ready for loading with newly received data from Ground.
    
    inBuffer_to_mt_structs();   // Parses the recently received inBuffer and populates both pulsar_mt_msg and global_mission_subset.

    pulsar_mt_msg_print();          // Print the just populated pulsar_mt_msg struct
    global_mission_subset_print();  // Print the just populated global_mission_subset struct
    
    /*
     * Do Local FMX actions
     */
    debugPrintln("case_process_isbd_rx() - Do local FMX actions");
    debugPrint("case_process_isbd_rx() - MT Msg Origin Timestamp:");pulsar_timestamp_print(pulsar_mt_msg.mt_send_timestamp_unix_s);

    setFmxContactGroundInt(pulsar_mt_msg.set_fmx_contact_ground_int_m);
    putFmxSettings();   // Write FmxSettings to EEPROM after the above change was made.

    if (pulsar_mt_msg.fmx_cmd_byte1 != FMX_CMD_BYTE_NC)     doFmxCmd1(pulsar_mt_msg.fmx_cmd_byte1);
    if (pulsar_mt_msg.fmx_cmd_byte2 != FMX_CMD_BYTE_NC)     doFmxCmd2(pulsar_mt_msg.fmx_cmd_byte2);
    if (pulsar_mt_msg.fmx_cmd_byte3 != FMX_CMD_BYTE_NC)     doFmxCmd3(pulsar_mt_msg.fmx_cmd_byte3);
    if (pulsar_mt_msg.fmx_cmd_byte4 != FMX_CMD_BYTE_NC)     doFmxCmd4(pulsar_mt_msg.fmx_cmd_byte4);
    if (pulsar_mt_msg.fmx_cmd_byte5 != FMX_CMD_BYTE_NC)     doFmxCmd5(pulsar_mt_msg.fmx_cmd_byte5);
    if (pulsar_mt_msg.fmx_cmd_byte6 != FMX_CMD_BYTE_NC)     doFmxCmd6(pulsar_mt_msg.fmx_cmd_byte6);
    if (pulsar_mt_msg.fmx_cmd_byte7 != FMX_CMD_BYTE_NC)     doFmxCmd7(pulsar_mt_msg.fmx_cmd_byte7);
    if (pulsar_mt_msg.fmx_cmd_byte8 != FMX_CMD_BYTE_NC)     doFmxCmd8(pulsar_mt_msg.fmx_cmd_byte8);
    if (pulsar_mt_msg.fmx_cmd_byte9 != FMX_CMD_BYTE_NC)     doFmxCmd9(pulsar_mt_msg.fmx_cmd_byte9);
    if (pulsar_mt_msg.fmx_cmd_byte10 != FMX_CMD_BYTE_NC)    doFmxCmd10(pulsar_mt_msg.fmx_cmd_byte10);


    debugPrintln("case_process_isbd_rx() - Finished local FMX actions");

    /*
     * Do AP actions
     */
    debugPrintln("case_process_isbd_rx() - Do AP actions");

    process_MT_msg_for_ap();    // Process recently received MT msg and set appropriate flags
    tx_to_autopilot();          // Based on above processing, send Mavlink data/cmds to the autopilot.

    debugPrintln("case_process_isbd_rx() - Finished AP actions");

    gotMsgFromGroundFlag = false;   // Clear flag as we have actioned it.

    debugPrintln("case_process_isbd_rx() - Complete");
}   // END - case_process_isbd_rx()


