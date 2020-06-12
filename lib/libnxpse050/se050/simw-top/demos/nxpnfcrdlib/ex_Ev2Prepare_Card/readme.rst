..
    Copyright 2019,2020 NXP

    This software is owned or controlled by NXP and may only be used
    strictly in accordance with the applicable license terms.  By expressly
    accepting such terms or by downloading, installing, activating and/or
    otherwise using the software, you are agreeing that you have read, and
    that you agree to comply with and are bound by, such license terms.  If
    you do not agree to be bound by the applicable license terms, then you
    may not retain, install, activate or otherwise use the software.

.. highlight:: bat

.. _ex_Ev2Prepare_Card:

=======================================================================
 MIFARE DESFire EV2 : Prepare MFDFEV2
=======================================================================

.. warning::
  To run this example, you would need the nxpnfcrdlib component for which a Non-Disclosure Agreement(NDA) needs to be signed. Please contact your FAE for additional details.

Prerequisites
^^^^^^^^^^^^^^^

- Bring Up Hardware. (Refer :ref:`dev-platforms`)
- Connect RC663 to your micronctroller. (Refer :ref:`mifarekdf-rc663 <mifarekdf-rc663>`)


About the Example
^^^^^^^^^^^^^^^^^
This is an example project for preparing the MIFARE DESFire EV2 card for
running other SE050 MIFARE DESFire EV2 examples.This example formats the card
creates app, keys and file for running the other examples.

This does not make use of any SE05X APIs.
It make use of nxpNfcrdLib apis for
- formatting the card
- Creating an AFC application
- Selecting the application
- Creating a value file

Creation of application
^^^^^^^^^^^^^^^^^^^^^^^^^
creation of application with application ID happens in
- :cpp:func:`phEx_Personalize_AFCApp()`:

The application ID is

.. literalinclude:: ../common/src/phExMfDfCrypto.c
   :language: c
   :start-after: /* doc:start:mif-afc-app */
   :end-before: /* doc:end:mif-afc-app */

AES Keys provisioned in card
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
We create One key in the card, This key is set when we are creating the transaction mac file.
The key value is

.. literalinclude:: ../common/src/phExMfDfCrypto.c
   :language: c
   :start-after: /* doc:start:card-aes-key */
   :end-before: /* doc:end:card-aes-key */


Running the Demo
^^^^^^^^^^^^^^^^^^^^^^

#) Either press the reset button on your board or launch the debugger in your IDE to begin running the demo

If everything is setup correctly the output would be as follows::

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
