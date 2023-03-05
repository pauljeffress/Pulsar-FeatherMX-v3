/*
 * prep_binary_MO_message_locarb.cpp
 * 
 * 
 * This code is based on both the original Sparkfun Artemis Global Tracker example code
 * AND the locarb (https://www.locarbftw.com) way of packing and TX/RX'ing data over ISBD.
 * 
 * See do_iridium_oldway.ino for how I used to do it all the Sparkfun example way before brining in locarb stuff.
 * 
 */

#include "global.h"


/*
 * Populate a buffer (outBufferNew[]), in binary format, ready for ISBD TX from Boat to Shore
 * 
 * It's populated from myFmxSharedSettings & myAgtSettings & some local AGT variables
 * 
 */
void prep_binary_MO_message_locarb()
{                
    uint16_t pos = 0;       // track position in buffer as we load it up.
    uint32_t csuma = 0;     // for checksum calcs
    uint32_t csumb = 0;     // for checksum calcs
  
    debugPrintln("prep_binary_MO_message_locarb() - Starting");
    
    // zero out the buffer
    for (int i = 0; i < (BOAT_OUT_BUFFER_NEW_MAX_BYTES); i++)
        outBufferNew[i] = 0;

    // xxx - I think there is a better way to populate the buffer from my data source structures.

    /*
    * Header
    */
    // Start delimiters
    outBufferNew[0] = BUFFER_START_DELIM_1ST;
    outBufferNew[1] = BUFFER_START_DELIM_2ND;

    // Payload Length dummy place holder
    outBufferNew[2] = 255;  // this gets set later, I'm just putting a value here now as a placeholder.
    outBufferNew[3] = 255;  // this gets set later, I'm just putting a value here now as a placeholder.

    // ISBD MO magic number
    uint16_t isbdMoMagicNum = getNextISBDMoNum();
    debugPrint("prep_binary_MO_message_locarb() - using ISBD MO Magic Num: "); debugPrintlnInt(isbdMoMagicNum);
    outBufferNew[4] = lowByte(isbdMoMagicNum);
    outBufferNew[5] = highByte(isbdMoMagicNum);
   
    /*
    * Payload
    */
    pos = 6;  // start of Payload in buffer. Note the below pos++'s get executed after byte is assigned 
              // to position pos. So they point to the "next" byte position.

    // PowerFeather
    // ============
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_BATT_V);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_BATT_V);
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_TEMP);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_TEMP);    
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_RH);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_RH);
    outBufferNew[pos++] = (uint8_t)(myPowerFeatherSettings.PF_UPTIME_S);
    outBufferNew[pos++] = (uint8_t)(myPowerFeatherSettings.PF_UPTIME_S>>8);
    outBufferNew[pos++] = (uint8_t)(myPowerFeatherSettings.PF_UPTIME_S>>16);
    outBufferNew[pos++] = (uint8_t)(myPowerFeatherSettings.PF_UPTIME_S>>24);
    // PowerFeather - Charger 1  
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER1_PID);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER1_PID);
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER1_V);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER1_V);
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER1_I);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER1_I);
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER1_VPV);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER1_VPV);
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER1_PPV);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER1_PPV);
    outBufferNew[pos++] = (uint8_t)(myPowerFeatherSettings.PF_CHARGER1_CS);
    outBufferNew[pos++] = (uint8_t)(myPowerFeatherSettings.PF_CHARGER1_ERR);
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER1_LOAD);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER1_LOAD);  
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER1_IL);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER1_IL);
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER1_H20);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER1_H20);  
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER1_H21);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER1_H21);
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER1_HSDS);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER1_HSDS);
    outBufferNew[pos++] = (uint8_t)(myPowerFeatherSettings.PF_CHARGER1_MPPT);
    // PowerFeather - Charger 2  
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER2_PID);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER2_PID);
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER2_V);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER2_V);
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER2_I);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER2_I);
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER2_VPV);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER2_VPV);
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER2_PPV);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER2_PPV);
    outBufferNew[pos++] = (uint8_t)(myPowerFeatherSettings.PF_CHARGER2_CS);
    outBufferNew[pos++] = (uint8_t)(myPowerFeatherSettings.PF_CHARGER2_ERR);
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER2_LOAD);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER2_LOAD);  
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER2_IL);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER2_IL);
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER2_H20);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER2_H20);  
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER2_H21);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER2_H21);
    outBufferNew[pos++] = lowByte(myPowerFeatherSettings.PF_CHARGER2_HSDS);
    outBufferNew[pos++] = highByte(myPowerFeatherSettings.PF_CHARGER2_HSDS);
    outBufferNew[pos++] = (uint8_t)(myPowerFeatherSettings.PF_CHARGER2_MPPT);

    // Autopilot (via FMX)
    // ===================
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_BASEMODE);
    outBufferNew[pos++] = (uint8_t)(0); // For AP_BASEMODE I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_CUSTOMMODE);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_CUSTOMMODE>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_CUSTOMMODE>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_CUSTOMMODE>>24);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SYSTEMSTATUS);
    outBufferNew[pos++] = (uint8_t)(0); // For AP_SYSTEMSTATUS I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_POSITIONTIMESTAMP);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_POSITIONTIMESTAMP>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_POSITIONTIMESTAMP>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_POSITIONTIMESTAMP>>24);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_LAT);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_LAT>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_LAT>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_LAT>>24);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_LON);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_LON>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_LON>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_LON>>24);
    outBufferNew[pos++] = lowByte(myFmxSettings.AP_VX);
    outBufferNew[pos++] = highByte(myFmxSettings.AP_VX);
    outBufferNew[pos++] = lowByte(myFmxSettings.AP_VY);
    outBufferNew[pos++] = highByte(myFmxSettings.AP_VY);
    outBufferNew[pos++] = lowByte(myFmxSettings.AP_HDG);
    outBufferNew[pos++] = highByte(myFmxSettings.AP_HDG);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SATS);
    outBufferNew[pos++] = (uint8_t)(0); // For AP_SATS I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = lowByte(myFmxSettings.AP_VEL);
    outBufferNew[pos++] = highByte(myFmxSettings.AP_VEL);
    outBufferNew[pos++] = lowByte(myFmxSettings.AP_COG);
    outBufferNew[pos++] = highByte(myFmxSettings.AP_COG);
    outBufferNew[pos++] = lowByte(myFmxSettings.AP_VCC);
    outBufferNew[pos++] = highByte(myFmxSettings.AP_VCC);
    outBufferNew[pos++] = lowByte(myFmxSettings.AP_VSERVO);
    outBufferNew[pos++] = highByte(myFmxSettings.AP_VSERVO);
    outBufferNew[pos++] = lowByte(myFmxSettings.AP_POWERFLAGS);
    outBufferNew[pos++] = highByte(myFmxSettings.AP_POWERFLAGS);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SENSORSPRESENT);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SENSORSPRESENT>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SENSORSPRESENT>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SENSORSPRESENT>>24); 
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SENSORSENABLED);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SENSORSENABLED>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SENSORSENABLED>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SENSORSENABLED>>24);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SENSORSHEALTH);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SENSORSHEALTH>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SENSORSHEALTH>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SENSORSHEALTH>>24);
    outBufferNew[pos++] = lowByte(myFmxSettings.AP_LOAD);
    outBufferNew[pos++] = highByte(myFmxSettings.AP_LOAD);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_TIMEUNIXUSEC);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_TIMEUNIXUSEC>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_TIMEUNIXUSEC>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_TIMEUNIXUSEC>>24);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_TIMEUNIXUSEC>>32);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_TIMEUNIXUSEC>>40);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_TIMEUNIXUSEC>>48);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_TIMEUNIXUSEC>>56); 
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_TIMEBOOTMS);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_TIMEBOOTMS>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_TIMEBOOTMS>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_TIMEBOOTMS>>24);
    outBufferNew[pos++] = lowByte(myFmxSettings.AP_NAVBEARING);
    outBufferNew[pos++] = highByte(myFmxSettings.AP_NAVBEARING);
    outBufferNew[pos++] = lowByte(myFmxSettings.AP_TARGETBEARING);
    outBufferNew[pos++] = highByte(myFmxSettings.AP_TARGETBEARING);
    outBufferNew[pos++] = lowByte(myFmxSettings.AP_WPDIST);
    outBufferNew[pos++] = highByte(myFmxSettings.AP_WPDIST);
    outBufferNew[pos++] = lowByte(myFmxSettings.AP_CURRENTBATTERY);
    outBufferNew[pos++] = highByte(myFmxSettings.AP_CURRENTBATTERY);
    outBufferNew[pos++] = lowByte(myFmxSettings.AP_VENDORID);
    outBufferNew[pos++] = highByte(myFmxSettings.AP_VENDORID);
    outBufferNew[pos++] = lowByte(myFmxSettings.AP_PRODUCTID);
    outBufferNew[pos++] = highByte(myFmxSettings.AP_PRODUCTID);
    // AutoPilot SETs & REQUESTs
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SET_ARM_RESULT);
    outBufferNew[pos++] = (uint8_t)(0); // I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SET_DISARM_RESULT);
    outBufferNew[pos++] = (uint8_t)(0); // I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SET_PARAM1_RESULT);
    outBufferNew[pos++] = (uint8_t)(0); // I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_SET_PARAM2_RESULT);
    outBufferNew[pos++] = (uint8_t)(0); // I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_REBOOT_RESULT);
    outBufferNew[pos++] = (uint8_t)(0); // For AP_REBOOT_RESULT I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_REQ_PARAM_VALUETYPE1);
    outBufferNew[pos++] = (uint8_t)(0); // I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_REQ_PARAM_VALUEINT1);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_REQ_PARAM_VALUEINT1>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_REQ_PARAM_VALUEINT1>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_REQ_PARAM_VALUEINT1>>24);
    outBufferNew[pos++] = (uint8_t)((uint32_t)myFmxSettings.AP_REQ_PARAM_VALUEFLOAT1);
    outBufferNew[pos++] = (uint8_t)((uint32_t)myFmxSettings.AP_REQ_PARAM_VALUEFLOAT1>>8);
    outBufferNew[pos++] = (uint8_t)((uint32_t)myFmxSettings.AP_REQ_PARAM_VALUEFLOAT1>>16);
    outBufferNew[pos++] = (uint8_t)((uint32_t)myFmxSettings.AP_REQ_PARAM_VALUEFLOAT1>>24);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_REQ_PARAM_VALUETYPE2);
    outBufferNew[pos++] = (uint8_t)(0); // I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_REQ_PARAM_VALUEINT2);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_REQ_PARAM_VALUEINT2>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_REQ_PARAM_VALUEINT2>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_REQ_PARAM_VALUEINT2>>24);
    outBufferNew[pos++] = (uint8_t)((uint32_t)myFmxSettings.AP_REQ_PARAM_VALUEFLOAT2);
    outBufferNew[pos++] = (uint8_t)((uint32_t)myFmxSettings.AP_REQ_PARAM_VALUEFLOAT2>>8);
    outBufferNew[pos++] = (uint8_t)((uint32_t)myFmxSettings.AP_REQ_PARAM_VALUEFLOAT2>>16);
    outBufferNew[pos++] = (uint8_t)((uint32_t)myFmxSettings.AP_REQ_PARAM_VALUEFLOAT2>>24);
    // AutoPilot MISSION RESULT
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.AP_MISSION_RESULT);
    outBufferNew[pos++] = (uint8_t)(0); // I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly

    // FMX Stuff
    // =========
    //debugPrint("myFmxSettings.FMX_BATT_V"); debugPrintlnInt(myFmxSettings.FMX_BATT_V);
    //debugPrint("myFmxSettings.FMX_TEMP"); debugPrintlnInt(myFmxSettings.FMX_TEMP);
    //debugPrint("myFmxSettings.FMX_RH"); debugPrintlnInt(myFmxSettings.FMX_RH);
    outBufferNew[pos++] = lowByte(myFmxSettings.FMX_BATT_V);
    outBufferNew[pos++] = highByte(myFmxSettings.FMX_BATT_V);
    outBufferNew[pos++] = lowByte(myFmxSettings.FMX_TEMP);
    outBufferNew[pos++] = highByte(myFmxSettings.FMX_TEMP);    
    outBufferNew[pos++] = lowByte(myFmxSettings.FMX_RH);
    outBufferNew[pos++] = highByte(myFmxSettings.FMX_RH);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_UPTIME_S);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_UPTIME_S>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_UPTIME_S>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_UPTIME_S>>24);
    outBufferNew[pos++] = lowByte(myFmxSettings.FMX_PRESS);
    outBufferNew[pos++] = highByte(myFmxSettings.FMX_PRESS);  
    outBufferNew[pos++] = lowByte(myFmxSettings.FMX_WATERTEMP);
    outBufferNew[pos++] = highByte(myFmxSettings.FMX_WATERTEMP);
    outBufferNew[pos++] = lowByte(myFmxSettings.FMX_AMBIENTLIGHT);
    outBufferNew[pos++] = highByte(myFmxSettings.FMX_AMBIENTLIGHT);
    // FMX's own GPS data
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_STATUS);      outBufferNew[pos++] = (uint8_t)(0); // I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_FIX);         outBufferNew[pos++] = (uint8_t)(0); // I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_SIV);         outBufferNew[pos++] = (uint8_t)(0); // I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_POSITIONTIMESTAMP);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_POSITIONTIMESTAMP>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_POSITIONTIMESTAMP>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_POSITIONTIMESTAMP>>24);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_LAT);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_LAT>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_LAT>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_LAT>>24);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_LON);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_LON>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_LON>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_LON>>24);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_ALT);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_ALT>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_ALT>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_ALT>>24);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_GSPEED);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_GSPEED>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_GSPEED>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_GSPEED>>24);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_HEAD_MOTION);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_HEAD_MOTION>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_HEAD_MOTION>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_GPS_HEAD_MOTION>>24);
    outBufferNew[pos++] = lowByte(myFmxSettings.FMX_GPS_PDOP);
    outBufferNew[pos++] = highByte(myFmxSettings.FMX_GPS_PDOP); 
    // FMX's STATUS FLAGS
    outBufferNew[pos++] = lowByte(myFmxSettings.FMX_STATUS_FLAGS_A);
    outBufferNew[pos++] = highByte(myFmxSettings.FMX_STATUS_FLAGS_A); 
    outBufferNew[pos++] = lowByte(myFmxSettings.FMX_STATUS_FLAGS_B);
    outBufferNew[pos++] = highByte(myFmxSettings.FMX_STATUS_FLAGS_B); 
    outBufferNew[pos++] = lowByte(myFmxSettings.FMX_STATUS_FLAGS_C);
    outBufferNew[pos++] = highByte(myFmxSettings.FMX_STATUS_FLAGS_C); 
    outBufferNew[pos++] = lowByte(myFmxSettings.FMX_STATUS_FLAGS_D);
    outBufferNew[pos++] = highByte(myFmxSettings.FMX_STATUS_FLAGS_D); 
    // FMX's measure of Batteries
    outBufferNew[pos++] = lowByte(myFmxSettings.FMX_BATT1_V);
    outBufferNew[pos++] = highByte(myFmxSettings.FMX_BATT1_V); 
    outBufferNew[pos++] = lowByte(myFmxSettings.FMX_BATT1_I);
    outBufferNew[pos++] = highByte(myFmxSettings.FMX_BATT1_I); 
    outBufferNew[pos++] = lowByte(myFmxSettings.FMX_BATT2_V);
    outBufferNew[pos++] = highByte(myFmxSettings.FMX_BATT2_V); 
    outBufferNew[pos++] = lowByte(myFmxSettings.FMX_BATT2_I);
    outBufferNew[pos++] = highByte(myFmxSettings.FMX_BATT2_I); 
    // FMX's timers
    outBufferNew[pos++] = lowByte(myFmxSettings.FMX_CONTACT_GND_INT_M);
    outBufferNew[pos++] = highByte(myFmxSettings.FMX_CONTACT_GND_INT_M);
    // FMX's peers "last heard" type data
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_LAST_AP_HEARTBEAT_UNIXTIME);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_LAST_AP_HEARTBEAT_UNIXTIME>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_LAST_AP_HEARTBEAT_UNIXTIME>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_LAST_AP_HEARTBEAT_UNIXTIME>>24);  
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_LAST_HEARD_PF_UNIXTIME);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_LAST_HEARD_PF_UNIXTIME>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_LAST_HEARD_PF_UNIXTIME>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_LAST_HEARD_PF_UNIXTIME>>24);  
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_LAST_HEARD_SF_UNIXTIME);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_LAST_HEARD_SF_UNIXTIME>>8);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_LAST_HEARD_SF_UNIXTIME>>16);
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_LAST_HEARD_SF_UNIXTIME>>24);
    // FMX's CMD_RESPonses
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_CMD_RESP1);outBufferNew[pos++] = (uint8_t)(0); // xxx - I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_CMD_RESP2);outBufferNew[pos++] = (uint8_t)(0); // xxx - I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_CMD_RESP3);outBufferNew[pos++] = (uint8_t)(0); // xxx - I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_CMD_RESP4);outBufferNew[pos++] = (uint8_t)(0); // xxx - I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_CMD_RESP5);outBufferNew[pos++] = (uint8_t)(0); // xxx - I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_CMD_RESP6);outBufferNew[pos++] = (uint8_t)(0); // xxx - I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_CMD_RESP7);outBufferNew[pos++] = (uint8_t)(0); // xxx - I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_CMD_RESP8);outBufferNew[pos++] = (uint8_t)(0); // xxx - I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_CMD_RESP9);outBufferNew[pos++] = (uint8_t)(0); // xxx - I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly
    outBufferNew[pos++] = (uint8_t)(myFmxSettings.FMX_CMD_RESP10);outBufferNew[pos++] = (uint8_t)(0); // xxx - I am taking a uint8_t and sending as a uint16_t to help GSE32 receive correctly


    // Now that we have finished building Payload we can go back and insert the Payload Length field correctly.
    //Serial.print("prep_binary_MO_message_locarb() - at end of buffer data build : pos=");Serial.println(pos);
    outBufferNew[2] = lowByte(pos-6);   // pos-6 now reflects the total number of bytes in Payload
    outBufferNew[3] = highByte(pos-6);  // pos-6 now reflects the total number of bytes in Payload

    /*
    * Tail
    */
    // Calculate the RFC 1145 Checksum bytes from Header + Payload
    for (size_t x = 0; x < pos; x++) // Calculate a sum of sums for every byte in Header & Payload
    {
        csuma = csuma + outBufferNew[x];
        csumb = csumb + csuma;
    }
    // Add the two checksum bytes to the end of the message and increment outBufferNewSize accordingly
    outBufferNew[pos++] = (byte)(csuma & 0x000000ff);          
    outBufferNew[pos++] = (byte)(csumb & 0x000000ff);

    // Add End delimiters
    outBufferNew[pos++] = BUFFER_END_DELIM_1ST;
    outBufferNew[pos] = BUFFER_END_DELIM_2ND;
    //Serial.print("prep_binary_MO_message_locarb() - at end of buffer dat build + cksum + delimter addons : pos=");Serial.println(pos);

    //Serial.print("prep_binary_MO_message_locarb() - Total Bytes used : ");Serial.println(pos+1); // the pos+1 is because the buffer positions start at 0.

    outBufferNewSize = pos+1; // so we know size when we send.     xxx - what???
    debugPrint("prep_binary_MO_message_locarb() - outBufferNewSize: "); debugPrintlnInt(outBufferNewSize);

    // ##################################################
    // End of binary message construction
    // ##################################################

    // Print the message
    //Serial.print("prep_binary_MO_message_locarb() - Binary message is>>");
    for (size_t x = 0; x < outBufferNewSize; x++)
    {
        //Serial.print(" 0x"); Serial.print(outBufferNew[x],HEX);
    }
    //Serial.println("<<");

    debugPrintln("prep_binary_MO_message_locarb() - Complete");

}   // END - prep_binary_MO_message_locarb()
