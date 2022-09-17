/*
 * MO_and_MT_messages_fns.cpp
 *
 *
 *
 */

#include "MO_and_MT_messages.h"

#include <TimeLib.h>

#include "PulsarShared.h"
#include "debug_fns.h"
#include "mavlink_fns.h"
#include "pulsar_buffer_fns.h"
#include "time_manip_fns.h"


// Define any globals that only the functions in this file need.



// Functions




/*
 * Parses the buffer and populates pulsar_mt_msg and global_mission_subset.
 */
void inBuffer_to_mt_structs()
{
    debugPrintln("inBuffer_to_mt_structs() - starting.");

    uint16_t pos = 2; // index into buffer, starting at Payload Length bytes.

    pulsar_mt_msg.payload_length = fromBuf_uint16_t(inBufferNewLatest[pos], inBufferNewLatest[pos + 1]);        pos += 2;
    pulsar_mt_msg.mt_magic_number = fromBuf_uint16_t(inBufferNewLatest[pos], inBufferNewLatest[pos + 1]);       pos += 2;
    pulsar_mt_msg.mt_send_timestamp_unix_s = fromBuf_uint32_t(inBufferNewLatest[pos], 
                                                                inBufferNewLatest[pos + 1], 
                                                                inBufferNewLatest[pos + 2], 
                                                                inBufferNewLatest[pos + 3]);  pos += 4;
    pulsar_mt_msg.set_fmx_contact_ground_int_m = fromBuf_uint16_t(inBufferNewLatest[pos], inBufferNewLatest[pos + 1]);  pos += 2;
    pulsar_mt_msg.set_ap_arm_state_b = inBufferNewLatest[pos];      pos += 1;
    pulsar_mt_msg.set_ap_disarm_state_b = inBufferNewLatest[pos];   pos += 1;

    global_mission_subset.count = inBufferNewLatest[pos];
    pos += 1;
    if (global_mission_subset.count > 4)
        global_mission_subset.count = 4; // cap it at 4 items as thats all that my ISBD MT message can fit.
    for (int i = 0; i < global_mission_subset.count; i++)
    {
        // Remember, we only send across a subset of the mission_item_subset sub fields.
        // Things like the z position are irrelevant on the boat so we don't waste bytes sending them.
        global_mission_subset.pulsar_mission_item_subset[i].sequence_number = fromBuf_uint16_t(inBufferNewLatest[pos], inBufferNewLatest[pos + 1]);
        pos += 2;
        global_mission_subset.pulsar_mission_item_subset[i].frame = (MAV_FRAME)inBufferNewLatest[pos];
        pos += 1;
        global_mission_subset.pulsar_mission_item_subset[i].command = (MAV_CMD)inBufferNewLatest[pos];
        pos += 1;
        global_mission_subset.pulsar_mission_item_subset[i].is_current_item = inBufferNewLatest[pos];
        pos += 1;
        global_mission_subset.pulsar_mission_item_subset[i].auto_continue = inBufferNewLatest[pos];
        pos += 1;
        global_mission_subset.pulsar_mission_item_subset[i].x = fromBuf_int32_t(inBufferNewLatest[pos], inBufferNewLatest[pos + 1], inBufferNewLatest[pos + 2], inBufferNewLatest[pos + 3]);
        pos += 4;
        global_mission_subset.pulsar_mission_item_subset[i].y = fromBuf_int32_t(inBufferNewLatest[pos], inBufferNewLatest[pos + 1], inBufferNewLatest[pos + 2], inBufferNewLatest[pos + 3]);
        pos += 4;
    }

    global_mission_subset.status = (PULSAR_STORED_MISSION_STATUS)inBufferNewLatest[pos];
    pos += 1;

    pulsar_mt_msg.set_ap_flightmode = inBufferNewLatest[pos];
    pos += 1;

    for (int i = 0; i < PARAM_NAME_SIZE; i++) // copy all 16 bytes into our buffer
        pulsar_mt_msg.set_ap_param_paramname1[i] = (uint8_t)inBufferNewLatest[i + pos];
    pos += PARAM_NAME_SIZE; // move pos to next byte after above string was added

    pulsar_mt_msg.set_ap_param_paramvalue_int1 = fromBuf_int32_t(inBufferNewLatest[pos], inBufferNewLatest[pos + 1], inBufferNewLatest[pos + 2], inBufferNewLatest[pos + 3]);
    pos += 4;
    pulsar_mt_msg.set_ap_param_paramvalue_float1 = fromBuf_float(inBufferNewLatest[pos], inBufferNewLatest[pos + 1], inBufferNewLatest[pos + 2], inBufferNewLatest[pos + 3]);
    pos += 4;
    pulsar_mt_msg.set_ap_param_paramvaluetype1 = inBufferNewLatest[pos];
    pos += 1;

    for (int i = 0; i < PARAM_NAME_SIZE; i++) // copy all 16 bytes into our buffer
        pulsar_mt_msg.set_ap_param_paramname2[i] = (char)inBufferNewLatest[i + pos];
    pos += PARAM_NAME_SIZE; // move pos to next byte after above string was added

    pulsar_mt_msg.set_ap_param_paramvalue_int2 = fromBuf_int32_t(inBufferNewLatest[pos], inBufferNewLatest[pos + 1], inBufferNewLatest[pos + 2], inBufferNewLatest[pos + 3]);
    pos += 4;
    pulsar_mt_msg.set_ap_param_paramvalue_float2 = fromBuf_float(inBufferNewLatest[pos], inBufferNewLatest[pos + 1], inBufferNewLatest[pos + 2], inBufferNewLatest[pos + 3]);
    pos += 4;
    pulsar_mt_msg.set_ap_param_paramvaluetype2 = inBufferNewLatest[pos];
    pos += 1;

    pulsar_mt_msg.set_ap_reboot_m = inBufferNewLatest[pos];     pos += 1;

    pulsar_mt_msg.request_ap_param_paramvaluetype1 = inBufferNewLatest[pos];
    pos += 1;
    for (int i = 0; i < PARAM_NAME_SIZE; i++) // copy all 16 bytes into our buffer
        pulsar_mt_msg.request_ap_param_paramname1[i] = (char)inBufferNewLatest[i + pos];
    pos += PARAM_NAME_SIZE; // move pos to next byte after above string was added
    pulsar_mt_msg.request_ap_param_paramvaluetype2 = inBufferNewLatest[pos];
    pos += 1;
    for (int i = 0; i < PARAM_NAME_SIZE; i++) // copy all 16 bytes into our buffer
        pulsar_mt_msg.request_ap_param_paramname2[i] = (char)inBufferNewLatest[i + pos];
    pos += PARAM_NAME_SIZE; // move pos to next byte after above string was added

    pulsar_mt_msg.fmx_cmd_byte1 = inBufferNewLatest[pos];   pos += 1;
    pulsar_mt_msg.fmx_cmd_byte2 = inBufferNewLatest[pos];   pos += 1;
    pulsar_mt_msg.fmx_cmd_byte3 = inBufferNewLatest[pos];   pos += 1;
    pulsar_mt_msg.fmx_cmd_byte4 = inBufferNewLatest[pos];   pos += 1;
    pulsar_mt_msg.fmx_cmd_byte5 = inBufferNewLatest[pos];   pos += 1;
    pulsar_mt_msg.fmx_cmd_byte6 = inBufferNewLatest[pos];   pos += 1;
    pulsar_mt_msg.fmx_cmd_byte7 = inBufferNewLatest[pos];   pos += 1;
    pulsar_mt_msg.fmx_cmd_byte8 = inBufferNewLatest[pos];   pos += 1;
    pulsar_mt_msg.fmx_cmd_byte9 = inBufferNewLatest[pos];   pos += 1;
    pulsar_mt_msg.fmx_cmd_byte10 = inBufferNewLatest[pos];  pos += 1;

    pulsar_mt_msg.checksum_a = inBufferNewLatest[pos];      pos += 1;
    pulsar_mt_msg.checksum_b = inBufferNewLatest[pos];      pos += 1;

    debugPrintln("inBuffer_to_mt_structs() - complete.");
} // END - inBuffer_to_mt_structs

