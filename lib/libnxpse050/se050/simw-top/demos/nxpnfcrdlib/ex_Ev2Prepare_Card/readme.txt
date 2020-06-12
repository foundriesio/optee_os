Overview
========

This project demonstrates preparing the DesfireEV2 card for
running other Se050 Desfire EV2 examples.This example formats the card creates app, keys and file for running the other examples.

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

      App:INFO :PlugAndTrust_v02.09.00_20190607
           sss:INFO :atr (Len=35)
                00 A0 00 00   03 96 04 03   E8 00 FE 02   0B 03 E8 08
                01 00 00 00   00 64 00 00   0A 4A 43 4F   50 34 20 41
                54 50 4F
           sss:WARN :Communication channel is Plain.
           sss:WARN :!!!Not recommended for production use.!!!
    Tx> 26
    Rx< 04 03
    Tx> 93 20
    Rx< 08 4B 3F 8A F6
    Tx> 93 70 08 4B 3F 8A F6
    Rx< 20
    UID after L3 Activation 08 4B 3F 8A 00 00 00 00 00 00
    Tx> E0 80
    Rx< 06 75 77 81 02 80
    ATS after L4 Activation 06 75 77 81 02 80
    Performing Pre Personalization ......
    Tx> 0A 00
    Tx> 0A 00
    Rx< 0A 00 AF CB 25 7E C2 10 CE 82 78
    Tx> 0B 00
    Tx> AF 04 B9 B6 CE 0D 73 DE 3F 4D 9F 70 01 F0 12 53 B7
    Rx< 0B 00 00 88 CC 75 06 D0 EA FE F2
    Tx> 0A 00
    Tx> FC
    Rx< FA 00 01
    Tx> FA 00 01
    Rx< 0A 00 00
    Formating the card Successful
    Tx> 0B 00
    Tx> 5C 01
    Tx> 72 38 8B 51 41 68 C4 08 BD C3 F4 15 C9 BF D1 56 CA EC FD 6C 18 AC 10 9B
    Tx> 11 79 59 17 C3 0D 57 B3
    Rx< 0B 00 00
    Tx> 0A 00
    Tx> 5C 00
    Tx> 1C DE 04 43 CC 47 C9 9B
    Rx< 0A 00 00
    Tx> 0B 00
    Tx> 51
    Rx< 0B 00 00 49 27 EF 57 76 83 6C 8F A3 7B D7 DF 6E E1 6D 0E
    bCardUid[0] = 0x4
    bCardUid[1] = 0x40
    bCardUid[2] = 0x40
    bCardUid[3] = 0x42
    bCardUid[4] = 0x4d
    bCardUid[5] = 0x4d
    bCardUid[6] = 0x80
    bCardUid[7] = 0x0
    bCardUid[8] = 0x0
    bCardUid[9] = 0x0
    Tx> 0A 00
    Tx> CA 11 22 33 0F B6 01 01 02 10 00 AA 00 41 46 43 41 50 50 4C 49 43 41 54 49 4F 4E
    Rx< 0A 00 00
    Create AFC Application Successful
    Tx> 0B 00
    Tx> 5A 11 22 33
    Rx< 0B 00 00
    Select the AFC Application Successful
    phEx_Create_ValueFile...
    Tx> 0A 00
    Tx> CE 05 03 F0 0F 02 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    Rx< 0A 00 00
     create Transaction MAC File  Successful
    Tx> 0B 00
    Tx> CC 03 03 00 00 14 00 00 00 F4 01 00 00 14 00 00 00 02
    Rx< 0B 00 00
     create Value File  Successful
    Tx> 0A 00
    Tx> CD 1F 56 78 00 E0 00 FF 00 00
    Rx< 0A 00 00
    ***** Creating standard data file SUCCESS!!*******
           App:INFO :ex_sss Finished
