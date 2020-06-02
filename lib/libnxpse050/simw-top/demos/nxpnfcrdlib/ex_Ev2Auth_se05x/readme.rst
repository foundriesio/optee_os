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

.. _ex-Ev2Auth-se05x:

=======================================================================
 MIFARE DESFire EV2 : Authentication
=======================================================================

.. warning::
  To run this example, you would need the nxpnfcrdlib component for which a Non-Disclosure Agreement(NDA) needs to be signed. Please contact your FAE for additional details.

Prerequisites
=======================================================================

- :ref:`ex_Ev2Prepare_Card` must have been executed, so that the MIFARE DESFire EV2 card has the required credentials.
- :ref:`ex_Ev2Prepare_se050` must have been executed, so that the Secure element has the required credentials.
- Bring Up Hardware. (Refer :ref:`dev-platforms`)
- Connect RC663 to your micronctroller. (Refer :ref:`mifarekdf-rc663 <mifarekdf-rc663>`)

About the Example
=======================================================================

This project is an example demonstrating the Mifare Desfire EV2 authentication
using Seo50. After authentication it performs encrypted communication
with the desfire EV2 card.

It uses the following APIs and data types:

- :cpp:func:`Se05x_API_DFAuthenticateFirstPart1()`
- :cpp:func:`Se05x_API_DFAuthenticateNonFirstPart1()`
- :cpp:func:`Se05x_API_DFAuthenticateFirstPart2()`
- :cpp:func:`Se05x_API_DFAuthenticateNonFirstPart2()`
- :cpp:func:`Se05x_API_DFKillAuthentication()`


Running the Demo
=======================================================================


#) Either press the reset button on your board or launch the debugger in your IDE to begin running the demo

If everything is setup correctly the output would be as follows::

       App:INFO :PlugAndTrust_v02.09.00_20190607
           sss:INFO :atr (Len=35)
                            00 A0 00 00             03 96 04 03             E8 00 FE 02             0B 03 E8 08
                            01 00 00 00             00 64 00 00             0A 4A 43 4F             50 34 20 41
                            54 50 4F
           sss:WARN :Communication channel is Plain.
           sss:WARN :!!!Not recommended for production use.!!!
    Tx> 26
    Rx< 04 03
    Tx> 93 20
    Rx< 08 7D 6B 96 88
    Tx> 93 70 08 7D 6B 96 88
    Rx< 20
    UID after L3 Activation 08 7D 6B 96 00 00 00 00 00 00
    Tx> E0 80
    Rx< 06 75 77 81 02 80
    ATS after L4 Activation 06 75 77 81 02 80
    Tx> 0A 00
    Tx> 5A 11 22 33
    Rx< 0A 00 00
           App:INFO :Select the AFC Application Successful

           App:INFO :attempting to authenticate with cardkey = 0 and Se0Obj ID = 2103308288
    Tx> 0B 00
    Tx> 71 00 06 00 00 00 00 00 00
    Rx< 0B 00 AF 35 58 48 3C 62 54 DE 34 F5 78 03 24 14 53 13 7C
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
                            35 58 48 3C             62 54 DE 34             F5 78 03 24             14 53 13 7C
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
                            1B 45 77 91             64 D2 57 B5             20 40 95 FD             40 B8 D8 D4
                            74 C4 51 5D             5F A4 07 2C             0D 6F 8F 6B             EF 15 4F 62
    Tx> 0A 00
    Tx> AF 1B 45 77 91 64 D2 57 B5 20 40 95 FD 40 B8 D8 D4 74 C4 51 5D 5F A4 07 2C 0D 6F 8F 6B EF 15 4F 62
    Rx< 0A 00 00 78 4A 85 D0 4A 30 F5 B4 48 B6 EC 98 1F F8 43 62 BE 6E B9 03 2C 24 EF 19 5D A4 95 2B D8 B7 C6 8A
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=32)
                            78 4A 85 D0             4A 30 F5 B4             48 B6 EC 98             1F F8 43 62
                            BE 6E B9 03             2C 24 EF 19             5D A4 95 2B             D8 B7 C6 8A
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=12)
                            00 00 00 00             00 00 00 00             00 00 00 00
           App:INFO :Dumped Session Key is (Len=16)
                            41 39 2E 7D             09 3E 19 DD             17 8B 0E 73             E5 1E 12 B9
           App:INFO :Dumped Session Mac is (Len=16)
                            9D 15 63 F3             2D 66 1F 4B             D6 3A 16 15             E1 3F A6 97
           App:INFO :Dumped TI is (Len=4)
                            95 E5 18 5D
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 First Authenticate  Successful

    Tx> 0B 00
    Tx> 77 00
    Rx< 0B 00 AF 50 5C 0C 05 50 7D BC 3A 8E 6B 6C 88 B1 E2 A5 B0
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
                            50 5C 0C 05             50 7D BC 3A             8E 6B 6C 88             B1 E2 A5 B0
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
                            11 F3 C6 A4             44 EF A1 85             2F 37 AF 64             8F 6F 51 12
                            6E BD E7 B5             5B E6 B1 38             9A 4F 7D A7             8F 65 40 20
    Tx> 0A 00
    Tx> AF 11 F3 C6 A4 44 EF A1 85 2F 37 AF 64 8F 6F 51 12 6E BD E7 B5 5B E6 B1 38 9A 4F 7D A7 8F 65 40 20
    Rx< 0A 00 00 19 D9 C9 E8 4E 52 DE 9E C5 41 02 C7 80 68 76 A1
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=16)
                            19 D9 C9 E8             4E 52 DE 9E             C5 41 02 C7             80 68 76 A1
           App:INFO :Dumped Session Key is (Len=16)
                            12 EF F8 40             59 61 99 DA             B8 C4 08 3D             90 8B 73 51
           App:INFO :Dumped Session Mac is (Len=16)
                            FF 23 11 DC             17 A9 7A 0B             5C D0 45 AF             86 F1 87 3E
           App:INFO :Dumped TI is (Len=4)
                            95 E5 18 5D
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 Following Authenticate  Successful

           App:INFO :Authenticated with cardkey = 0 and Se0Obj ID = 2103308288
    Tx> 0B 00
    Tx> 51 D8 49 E7 5E F0 9B 7A 0F
    Rx< 0B 00 00 B0 1F 31 B1 91 54 B6 14 42 72 B7 CD BA 24 13 66 63 CF D9 A4 F0 1B B4 00
           App:INFO :CARD UID is as below  (Len=7)
                            04 40 40 42             4D 4D 80
    phEx_Use_ValueFile...
    Performing Accreditation in AFC App....
    Performing Accreditation in AFC App Successful
    (Plain Communicatioon)Trying to Get the Current Value. Plain Communicatioon
    Tx> 0A 00
    Tx> 6C 03
    Rx< 0A 00 00 14 00 00 00
    Getting current value Successful
    (Enc Communication using session Key)Trying to Add money to the account
    Tx> 0B 00
    Tx> 0C 03
    Tx> 78 E3 12 67 90 96 56 EA 91 88 6A B8 08 46 A2 14 49 37 CE 63 E2 27 7C 1B
    Rx< 0B 00 00 E2 CE 37 F1 09 D8 E1 EA
    Add money to the account successful
    Tx> 0A 00
    Tx> C7 01 8A 66 56 AC 96 BA B6 2E
    Rx< 0A 00 00 01 00 00 00 71 76 A5 CC 92 DA 07 A4 C2 23 23 28 FC EF 11 C7
    Tx> 0B 00
    Tx> 6C 03
    Rx< 0B 00 00 17 00 00 00
     The amount in your account  After credit is 0 0 0 17
            Accreditation DONE!
           App:INFO : Auth session is reset in software
           App:INFO : Auth session is killed in SE
           App:INFO :ex_sss Finished


