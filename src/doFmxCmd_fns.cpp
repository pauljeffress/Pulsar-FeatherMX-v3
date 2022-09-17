/*
 * doFmxCmd_fns.cpp
 * 
 * Ground can send commands to the Agt by setting
 * pulsar_mt_msg.agt_cmd_byte1 thru pulsar_mt_msg.agt_cmd_byte5 to a non zero value
 * 
 * When an agt_cmd_byteX is received from the Ground, and it is non zero, the corresponding
 * doFmxCmdX(uint8_t ...) is called here.
 * 
 */

#include "global.h"

#include "doFmxCmd_fns.h"


void doFmxCmd1(uint8_t cmdByte)
{
    debugPrintln("doFmxCmd1() - Starting.");
    debugPrint("doFmxCmd1() - RX'd cmdByte="); debugPrintlnInt(cmdByte);

    // Dummy instructions until I have a proper use for this particular cmdByte
    // We just copy the cmdByte into the corresponding RESP byte and flag to send it back to the Ground.
    myFmxSettings.FMX_CMD_RESP1 = 2 * cmdByte;
    contactGroundFlag = true;  // flag that we have something to send to ground

    debugPrintln("doFmxCmd1() - Complete.");
}   // END

void doFmxCmd2(uint8_t cmdByte)
{
    debugPrintln("doFmxCmd2() - Starting.");
    debugPrint("doFmxCmd2() - RX'd cmdByte="); debugPrintlnInt(cmdByte);

    // Dummy instructions until I have a proper use for this particular cmdByte
    // We just copy the cmdByte into the corresponding RESP byte and flag to send it back to the Ground.
    myFmxSettings.FMX_CMD_RESP2 = 2 * cmdByte;
    contactGroundFlag = true;  // flag that we have something to send to ground

    debugPrintln("doFmxCmd2() - Complete.");
}   // END

void doFmxCmd3(uint8_t cmdByte)
{
    debugPrintln("doFmxCmd3() - Starting.");
    debugPrint("doFmxCmd3() - RX'd cmdByte="); debugPrintlnInt(cmdByte);

    // Dummy instructions until I have a proper use for this particular cmdByte
    // We just copy the cmdByte into the corresponding RESP byte and flag to send it back to the Ground.
    myFmxSettings.FMX_CMD_RESP3 = 2 * cmdByte;
    contactGroundFlag = true;  // flag that we have something to send to ground

    debugPrintln("doFmxCmd3() - Complete.");
}   // END

void doFmxCmd4(uint8_t cmdByte)
{
    debugPrintln("doFmxCmd4() - Starting.");
    debugPrint("doFmxCmd4() - RX'd cmdByte="); debugPrintlnInt(cmdByte);

    // Dummy instructions until I have a proper use for this particular cmdByte
    // We just copy the cmdByte into the corresponding RESP byte and flag to send it back to the Ground.
    myFmxSettings.FMX_CMD_RESP4 = 2 * cmdByte;
    contactGroundFlag = true;  // flag that we have something to send to ground

    debugPrintln("doFmxCmd4() - Complete.");
}   // END

void doFmxCmd5(uint8_t cmdByte)
{
    debugPrintln("doFmxCmd5() - Starting.");
    debugPrint("doFmxCmd5() - RX'd cmdByte="); debugPrintlnInt(cmdByte);

    // Dummy instructions until I have a proper use for this particular cmdByte
    // We just copy the cmdByte into the corresponding RESP byte and flag to send it back to the Ground.
    myFmxSettings.FMX_CMD_RESP5 = 2 * cmdByte;
    contactGroundFlag = true;  // flag that we have something to send to ground

    debugPrintln("doFmxCmd5() - Complete.");
}   // END

void doFmxCmd6(uint8_t cmdByte)
{
    debugPrintln("doFmxCmd6() - Starting.");
    debugPrint("doFmxCmd6() - RX'd cmdByte="); debugPrintlnInt(cmdByte);

    // Dummy instructions until I have a proper use for this particular cmdByte
    // We just copy the cmdByte into the corresponding RESP byte and flag to send it back to the Ground.
    myFmxSettings.FMX_CMD_RESP6 = 2 * cmdByte;
    contactGroundFlag = true;  // flag that we have something to send to ground

    debugPrintln("doFmxCmd6() - Complete.");
}   // END

void doFmxCmd7(uint8_t cmdByte)
{
    debugPrintln("doFmxCmd7() - Starting.");
    debugPrint("doFmxCmd7() - RX'd cmdByte="); debugPrintlnInt(cmdByte);

    // Dummy instructions until I have a proper use for this particular cmdByte
    // We just copy the cmdByte into the corresponding RESP byte and flag to send it back to the Ground.
    myFmxSettings.FMX_CMD_RESP7 = 2 * cmdByte;
    contactGroundFlag = true;  // flag that we have something to send to ground

    debugPrintln("doFmxCmd7() - Complete.");
}   // END

void doFmxCmd8(uint8_t cmdByte)
{
    debugPrintln("doFmxCmd8() - Starting.");
    debugPrint("doFmxCmd8() - RX'd cmdByte="); debugPrintlnInt(cmdByte);

    // Dummy instructions until I have a proper use for this particular cmdByte
    // We just copy the cmdByte into the corresponding RESP byte and flag to send it back to the Ground.
    myFmxSettings.FMX_CMD_RESP8 = 2 * cmdByte;
    contactGroundFlag = true;  // flag that we have something to send to ground

    debugPrintln("doFmxCmd8() - Complete.");
}   // END

void doFmxCmd9(uint8_t cmdByte)
{
    debugPrintln("doFmxCmd9() - Starting.");
    debugPrint("doFmxCmd9() - RX'd cmdByte="); debugPrintlnInt(cmdByte);

    // Dummy instructions until I have a proper use for this particular cmdByte
    // We just copy the cmdByte into the corresponding RESP byte and flag to send it back to the Ground.
    myFmxSettings.FMX_CMD_RESP9 = 2 * cmdByte;
    contactGroundFlag = true;  // flag that we have something to send to ground

    debugPrintln("doFmxCmd9() - Complete.");
}   // END

void doFmxCmd10(uint8_t cmdByte)
{
    debugPrintln("doFmxCmd10() - Starting.");
    debugPrint("doFmxCmd10() - RX'd cmdByte="); debugPrintlnInt(cmdByte);

    // Dummy instructions until I have a proper use for this particular cmdByte
    // We just copy the cmdByte into the corresponding RESP byte and flag to send it back to the Ground.
    myFmxSettings.FMX_CMD_RESP10 = 2 * cmdByte;
    contactGroundFlag = true;  // flag that we have something to send to ground

    debugPrintln("doFmxCmd10() - Complete.");
}   // END


