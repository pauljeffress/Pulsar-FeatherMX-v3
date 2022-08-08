/*
 * actuator_fns.cpp
 */

#include "global.h" // My main header file for this project itself

void actuatorsSetup()
{
    debugPrintln("actuatorsSetup() - executing");
    // Note: PinMode() commands are taken care of in setupPins();

    // /*
    //  *  Test All
    //  */
    // debugPrintln("actuatorsSetup() - All ON");
    // actuatorPiOn();
    // actuatorPowerFeatherOn();
    // actuatorStrobeOn();
    // while(1);
    // debugPrintln("actuatorsSetup() - All OFF");

    /*
     *  Test Pi
     */
    debugPrintln("actuatorsSetup() - Pi ON");
    // turn Pi on for a few seconds so I can see its working at powerup.
    actuatorPiOn();
    // delay(1000);
    // actuatorPiOff();  // xxx - temporarily not turning it off here until I get additional code in.
    // debugPrintln("actuatorsSetup() - Pi OFF");

    /*
     *  Test the Strobe Light
     */
    debugPrintln("actuatorsSetup() - Strobe Light ON");
    // turn Strobe Light on for a few seconds so I can see its working at powerup.
    // Note: its a self flashing strobe, once it has power it just flashes itself at about 1Hz.
    actuatorStrobeOn();
    // delay(8000);
    // actuatorStrobeOff();
    // debugPrintln("actuatorsSetup() - Strobe Light OFF");

    /*
     *  Test Power Feather
     */
    debugPrintln("actuatorsSetup() - PowerFeather ON");
    // turn PowerFeather on for a few seconds so I can see its working at powerup.
    actuatorPowerFeatherOn();
    // delay(1000);
    // actuatorPowerFeatherOff();  // xxx - temporarily not turning it off here until I get additional code in.
    // debugPrintln("actuatorsSetup() - PowerFeather OFF");

    debugPrintln("actuatorsSetup() - complete");
} // END - actuatorsSetup()

void actuatorStrobeOn()
{
    digitalWrite(STROBE_LIGHT_PWR_PIN, HIGH); // Turn Strobe Light ON
}

void actuatorStrobeOff()
{
    digitalWrite(STROBE_LIGHT_PWR_PIN, LOW); // Turn Strobe Light OFF
}

void actuatorPiOn()
{
    digitalWrite(PI_PWR_PIN, HIGH); // Turn Pi ON
}

void actuatorPiOff()
{
    digitalWrite(PI_PWR_PIN, LOW); // Turn Pi OFF
}

void actuatorPowerFeatherOn()
{
    digitalWrite(POWER_FEATHER_PWR_PIN, HIGH); // Turn Power Feather ON
}

void actuatorPowerFeatherOff()
{
    digitalWrite(POWER_FEATHER_PWR_PIN, LOW); // Turn Power Feather OFF
}