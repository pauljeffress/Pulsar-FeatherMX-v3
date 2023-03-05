/*
 * storage_fns.cpp
 *
 * All EEPROM related code
 *
 *
 */

#include "global.h"
#include "storage_fns.h"

ExternalEEPROM myStorage;      // Declare the EEPROM object
bool myStorage_status = false; // track status of my EEPROM storage...is it working or not

bool checkEEPROM()
{
    uint8_t stx = 0;
    uint8_t a = 0;
    uint8_t b = 0;
    uint8_t c = 0;
    uint8_t d = 0;
    uint8_t e = 0;
    uint8_t f = 0;

    debugPrintln("checkEEPROM() -  Starting");
    debugPrintln("checkEEPROM() -  1st 128 mem dump");
    for (int i = 0; i < 128; i++)
    {
        stx = 1;
        myStorage.get(i, stx);
        Serial.print("0x");
        if (stx < 0x10)
            Serial.print("0");
        if ((i+1) % 8 == 0)
            Serial.println(stx, HEX);
        else
        {
            Serial.print(stx, HEX);
            Serial.print("  ");
        }
    }
    Serial.println();


    myStorage.get(LOCATION_FMX_STX, stx);
    myStorage.get(LOCATION_FMX_EEPROM_MAGIC_NUM_A, a);
    myStorage.get(LOCATION_FMX_EEPROM_MAGIC_NUM_B, b);
    myStorage.get(LOCATION_FMX_EEPROM_MAGIC_NUM_C, c);
    myStorage.get(LOCATION_FMX_EEPROM_MAGIC_NUM_D, d);
    myStorage.get(LOCATION_FMX_EEPROM_MAGIC_NUM_E, e);
    myStorage.get(LOCATION_FMX_EEPROM_MAGIC_NUM_F, f);

    bool result = true;
    result = result && (stx == DEF_FMX_STX); // Check that EEPROM STX matches the expected values
    result = result && ((a == DEF_FMX_EEPROM_MAGIC_NUM_A) && (b == DEF_FMX_EEPROM_MAGIC_NUM_B));   // Check that the EEPROM magic nums are as expected.
    result = result && ((c == DEF_FMX_EEPROM_MAGIC_NUM_C) && (d == DEF_FMX_EEPROM_MAGIC_NUM_D));   // Check that the EEPROM magic nums are as expected.
    result = result && ((e == DEF_FMX_EEPROM_MAGIC_NUM_E) && (f == DEF_FMX_EEPROM_MAGIC_NUM_F));   // Check that the EEPROM magic nums are as expected.
    
    
    debugPrint("checkEEPROM() -  ");
    if (result)
        debugPrintln("valid");
    else
        debugPrintln("WARNING - EEPROM CONTENTS INVALID");

    return (result);
}

void putFmxSettings() // Erase EEPROM and write the myFmxSettings struct from RAM into EEPROM
{
    myStorage.erase();                               // Erase any old data in EEPROM first
    myStorage.put(LOCATION_SETTINGS, myFmxSettings); //  Write myFmxSettings struct from RAM to EEPROM
    debugPrintln("putFmxSettings() - myFmxSettings WRITE from RAM to EEPROM");
}

void getFmxSettings() // Read the myFmxSettings from EEPROM into RAM
{
    myStorage.get(LOCATION_SETTINGS, myFmxSettings);
    debugPrintln("getFmxSettings() - myFmxSettings READ from EEPROM to RAM");
}

void setupFmxSettings()
{
    debugPrintln("setupFmxSettings() - Starting");

    initFmxSettings(); // Set the myFmxSettings struct in RAM to defaults before we try to get from EEPROM.

    if (!myStorage.begin(0x57))
    {
        Serial.println("setupFmxSettings() - ERROR - No EEPROM memory detected. myFmxSetting set to defaults");
        myStorage_status = false; // set flag so we will not try to use the EEPROM storage
        delay(5000);              // time for me to see the message.
    }
    else
    {
        Serial.println("setupFmxSettings() - Memory detected");

        //Set settings for this EEPROM type AT24C32N - https://ww1.microchip.com/downloads/en/DeviceDoc/doc0336.pdf
        myStorage.setMemorySize(4096); //In bytes. 32kbit = 4kbyte
        myStorage.setPageSize(32); //In bytes. Has 32 byte page size.
        myStorage.enablePollForWriteComplete(); //Supports I2C polling of write completion
        myStorage.setPageWriteTime(10); //10 ms max write time
        Serial.print("setupFmxSettings() - Mem size in bytes: ");
        Serial.println(myStorage.length());

        // If the EEPROM already contains valid settings, they will always used - unless you
        // set DIPsw2, in which case the EEPROM contents will be reset to defaults on boot/reset.
        // Note this used to be controlled by #define RESET_EEPROM_TO_DEFAULTS in global.h
        if (flag_reset_eeprom)
        {
            Serial.print(__FUNCTION__);
            debugPrintln("() - WARNING - EEPROM contents are being wiped and re-initialised with defaults!");
            Serial.print(__FUNCTION__);
            debugPrintln("() - because DIPsw2 is currently ON");
            putFmxSettings(); // store the recently set to defaults, myFmxSettings struct to EEPROM.
            delay(5000);
        }

        // Check if the EEPROM data is valid
        if (checkEEPROM()) // if it is valid...
        {
            getFmxSettings(); // Copy EEPROM data into RAM "myFmxSettings"
        }
        else // if its invalid...
        {
            putFmxSettings(); // Write the defaults into EEPROM to fix it.
        }

    } // else

    debugPrintln("setupFmxSettings() - Complete");

} // END - setupFmxSettings()



// END of file storage_fns.cpp