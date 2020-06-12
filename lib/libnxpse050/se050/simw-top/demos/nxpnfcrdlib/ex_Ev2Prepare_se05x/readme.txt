Overview
========

This project demonstrates provisioning the Se050 for running other Se050 Desfire EV2 examples. This example Creates and sets 2 Se050 secure objects for further use by other examples.

Warning
==========
To run this example, you would need the nxpnfcrdlib component for which a Non-Disclosure Agreement(NDA) needs to be signed. Please contact your FAE for additional details.


Toolchain supported
===================
- MCUXpresso


Hardware requirements
=====================
- Micro USB cable
- Kinetis FRDM-K64F/imx-RT1050 board
- Personal Computer
- SE05x Arduino shield

Prerequisites
===============
1. Connect RC663 to your micronctroller.

Prepare the Demo
================
1. Build the demo
2. Connect a USB cable between the PC host and the OpenSDA USB port on the
   target board.
3. Download the program to the target board.
4. Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
    - change Setup->Terminal->New-line->Receive->AUTO
5. Either press the reset button on your board or launch the debugger in your
   IDE to begin running the demo.

Running the demo
================

The below messages are seen on the terminal

    sss:INFO :atr (Len=35)
        00 A0 00 00     03 96 04 03     E8 00 FE 02     0B 03 E8 08
        01 00 00 00     00 64 00 00     0A 4A 43 4F     50 34 20 41
        54 50 4F
    sss:WARN :Communication channel is Plain.
    sss:WARN :!!!Not recommended for production use.!!!
    App:INFO :SE050 prepared successfully for mifare desfire EV2 examples
    App:INFO :ex_sss Finished

