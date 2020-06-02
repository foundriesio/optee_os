Overview
========

This project demonstrates the Mifare Desfire EV2 Diversified
ChangeKeyEv2 using Seo50. The Key is diversified using the card UID After changing Keys, it performs encrypted communication with the desfire EV2 card using the diversified changed key. If enabled, It also reverts back the changed key.

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
1. ex_Ev2Prepare_Card must have been executed, so that the Desfire EV2 card has the required credentials.
2. ex_Ev2Prepare_se050 must have been executed, so that the Secure element has the required credentials.
3. Connect RC663 to your micronctroller.


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
          00 A0 00 00     03 96 04 03     E8 00 FE 02     0B 03 E8 08
          01 00 00 00     00 64 00 00     0A 4A 43 4F     50 34 20 41
          54 50 4F
           sss:WA       App:INFO :PlugAndTrust_v02.09.00_20190607
           sss:INFO :atr (Len=35)
          00 A0 00 00     03 96 04 03     E8 00 FE 02     0B 03 E8 08
          01 00 00 00     00 64 00 00     0A 4A 43 4F     50 34 20 41
          54 50 4F
           sss:WARN :Communication channel is Plain.
           sss:WARN :!!!Not recommended for production use.!!!
    Tx> 26
    Rx< 04 03
    Tx> 93 20
    Rx< 08 04 4D 8D CC
    Tx> 93 70 08 04 4D 8D CC
    ...
    ...
    ...
    ...
           App:INFO : Change Key for card key 0 is Successful to Se050ObjID= 2103308288

           App:INFO :Reverting Successful
           App:INFO : Auth session is reset in software
           App:INFO : Auth session is killed in SE
           App:INFO :ex_sss Finished


