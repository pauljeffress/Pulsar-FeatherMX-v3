/*
 * serial_fns.cpp
 *
 * All Serial port specific functions live here.
 * Both code for existing ports and the new ones I need to build to give me extra Serial
 * ports to plug into the AGT and OpenLog.
 *
 *
 * Serial1 = ISBD 9603 module
 * Serial2 = Cube AutoPilot
 * Serial3 = OpenLog Artemis
 * 
 */


#include "global.h" // My main header file for this project itself

/* Create Serial2 - a new hw Serial port
 * Note I am calling it "Serial2", I'm not sure, but when I tried naming it something like "UartToAGT" I had problems tx/rx'ing.
 * On Feather D19=RX2, D18=TX2
 * Define -- SERCOM    RX  TX      RX PAD           TX PAD
 */
Uart Serial2(&sercom0, 19, 18, SERCOM_RX_PAD_2, UART_TX_PAD_0);

/* Create Serial3 - a new hw Serial port
 * On Feather D11=RX3, D12=TX3
 * Define -- SERCOM    RX  TX      RX PAD           TX PAD
 */
Uart Serial3(&sercom3, 11, 12, SERCOM_RX_PAD_3, UART_TX_PAD_0);


// The adafruit document https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports/creating-a-new-serial
// said for SAMD51 I needed to define all 5 interrupt handlers for each new SERCOM I used (e.g. SERCOM0 & SERCOM3 in this case)
void SERCOM0_0_Handler()
{
    Serial2.IrqHandler();
}
void SERCOM0_1_Handler()
{
    Serial2.IrqHandler();
}
void SERCOM0_2_Handler()
{
    Serial2.IrqHandler();
}
void SERCOM0_3_Handler()
{
    Serial2.IrqHandler();
}

void SERCOM3_0_Handler()
{
    Serial3.IrqHandler();
}
void SERCOM3_1_Handler()
{
    Serial3.IrqHandler();
}
void SERCOM3_2_Handler()
{
    Serial3.IrqHandler();
}
void SERCOM3_3_Handler()
{
    Serial3.IrqHandler();
}

void serialSetup()
{
    /*
     * Serial - setup the serial port (wired via USB) for console/SerialMonitor etc.
     */
    Serial.begin(115200); // Start the console serial port
    delay(2000);          // ensure time for the Serial port to get ready.

    /*
     * Serial1 - setup the serial port between Feather and Iridium 9603 Sat Modem
     */
    Serial1.begin(19200);

    /*
     * Serial2 - setup the serial port between Feather & Cube AutoPilot.
     */
    // Initialise
    Serial2.begin(57600);
    // Reassign pins on the internal SAMD pinmux, to connect to my SERCOMs. They may have defaulted to other peripherals.
    // Assign Arduino Pins D18 & D19 SERCOM functionality. Must happen after the SerialX.begin(xxxx) command.
    pinPeripheral(18, PIO_SERCOM_ALT); // the 'PIO_SERCOM' should be 'PIO_SERCOM_ALT' if we are trying to use the 'alternate' pins for this.
    pinPeripheral(19, PIO_SERCOM_ALT); // same as above comment.

    /*
     *  Serial 3 - setup the serial logging port between Feather & OpenLog Artemis for logging.
     */
    // Initialise the Serial that connects this Feather to the OpenLog Artemis
    Serial3.begin(57600);
    // Reassign pins on the internal SAMD pinmux, to connect to my SERCOMs. They may have defaulted to other peripherals.
    // Assign SAMD51 pads 12 & 11 SERCOM functionality. Must happen after the SerialX.begin(xxxx) command.
    pinPeripheral(12, PIO_SERCOM);     // the 'PIO_SERCOM' should be 'PIO_SERCOM_ALT' if we are trying to use the 'alternate' pins for this.
    pinPeripheral(11, PIO_SERCOM_ALT); // same as above comment.

} // END - serialSetup()