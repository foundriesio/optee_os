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

.. _ex-Ev2ChangeKey-se05x:

=======================================================================
 MIFARE DESFire EV2 : Change Key
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

This project demonstrates the Mifare Desfire EV2 ChangeKeyEv2 using Se050.
After Changing Keys, it performs encrypted communication with the desfire
EV2 card using the changed key. If enabled, It also reverts back the
changed key.

It uses the following APIs and data types:

- :cpp:func:`Se05x_API_DFChangeKeyPart1()`
- :cpp:func:`Se05x_API_DFChangeKeyPart2()`
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
          00 A0 00 00     03 96 04 03     E8 00 FE 02     0B 03 E8 08
          01 00 00 00     00 64 00 00     0A 4A 43 4F     50 34 20 41
          54 50 4F
           sss:WARN :Communication channel is Plain.
           sss:WARN :!!!Not recommended for production use.!!!
    Tx> 26
    Rx< 04 03
    Tx> 93 20
    Rx< 08 00 FD 08 FD
    Tx> 93 70 08 00 FD 08 FD
    Rx< 20
    UID after L3 Activation 08 00 FD 08 00 00 00 00 00 00
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
    Rx< 0B 00 AF B4 BF 4B CE F5 D6 B4 A8 78 7B 67 48 E2 9A 69 E2
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          B4 BF 4B CE     F5 D6 B4 A8     78 7B 67 48     E2 9A 69 E2
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          EF A9 8E BA     7A F6 29 E3     09 26 5C 2D     15 09 72 C1
          5B D3 8B FA     35 97 D4 74     C0 CC E9 3D     2C FC 0F B0
    Tx> 0A 00
    Tx> AF EF A9 8E BA 7A F6 29 E3 09 26 5C 2D 15 09 72 C1 5B D3 8B FA 35 97 D4 74 C0 CC E9 3D 2C FC 0F B0
    Rx< 0A 00 00 AA 21 3F 9C E2 93 74 37 96 7A 4B 2E E5 25 D0 6C A6 D2 95 03 56 05 51 E6 D8 76 BF 26 13 1A 54 E9
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=32)
          AA 21 3F 9C     E2 93 74 37     96 7A 4B 2E     E5 25 D0 6C
          A6 D2 95 03     56 05 51 E6     D8 76 BF 26     13 1A 54 E9
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=12)
          00 00 00 00     00 00 00 00     00 00 00 00
           App:INFO :Dumped Session Key is (Len=16)
          C0 F5 21 9F     2C 30 E6 A8     59 F8 91 3E     F5 8B D5 E8
           App:INFO :Dumped Session Mac is (Len=16)
          0B 69 27 1D     5F 13 EA 2D     5B 7A 96 7A     52 D7 78 26
           App:INFO :Dumped TI is (Len=4)
          A2 27 15 FA
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 First Authenticate  Successful

    Tx> 0B 00
    Tx> 77 00
    Rx< 0B 00 AF AA 90 E0 4D EA CA 82 06 CF BD 71 EC CC D4 13 EA
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          AA 90 E0 4D     EA CA 82 06     CF BD 71 EC     CC D4 13 EA
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          2D AC 96 1B     3D 9D 1D A9     FB A5 50 C3     D9 77 B1 AD
          6B E8 A1 1A     3A B9 70 59     4D DC 97 0E     43 FF 4E 1B
    Tx> 0A 00
    Tx> AF 2D AC 96 1B 3D 9D 1D A9 FB A5 50 C3 D9 77 B1 AD 6B E8 A1 1A 3A B9 70 59 4D DC 97 0E 43 FF 4E 1B
    Rx< 0A 00 00 C2 EA 3C CD 57 5A B5 BA 28 E1 ED 9F 5A 25 43 54
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=16)
          C2 EA 3C CD     57 5A B5 BA     28 E1 ED 9F     5A 25 43 54
           App:INFO :Dumped Session Key is (Len=16)
          63 BD 6A 50     20 21 B7 75     37 60 E2 20     FB 8C AE 16
           App:INFO :Dumped Session Mac is (Len=16)
          23 8F 24 BE     C0 3B BA 91     BB CB 4C 5B     35 AE F7 2B
           App:INFO :Dumped TI is (Len=4)
          A2 27 15 FA
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 Following Authenticate  Successful

           App:INFO :Authenticated with cardkey = 0 and Se0Obj ID = 2103308288
           App:INFO :attempting to change cardkey = 2 from  Old Se050ObjID= 2103308288 to new Se050ObjID= 2103308289
    Tx> 0B 00
    Tx> C6 00 02 1A 76 EC 43 C7 84 68 A3 9B 6F 48 89 3F 28 0E B3 84 88 1A B8 04 45 0C 93 98 B3 EF E7 69 81 BC A5 00 C9 71 35 A0 DF 8B D8
    Rx< 0B 00 00 05 5E 90 58 DB D5 33 95
           App:INFO : Change Key for card key 2 is Successful to Se050ObjID= 2103308289

           App:INFO :Checking that the previous auth session is still valid by trying an encrypted communication
    Tx> 0A 00
    Tx> 51 57 8B 91 0C E6 28 06 A0
    Rx< 0A 00 00 5D 03 C2 BB 05 1D 51 D1 91 F9 88 01 E4 F9 27 D2 69 F3 B4 67 D0 FA 65 D4
           App:INFO :CARD UID is as below  (Len=7)
          04 40 40 42     4D 4D 80
           App:INFO :Previous auth session is still valid
           App:INFO :Auth with the changed cardkey 2
    Tx> 0B 00
    Tx> 5A 11 22 33
    Rx< 0B 00 00
           App:INFO :Select the AFC Application Successful

           App:INFO :attempting to authenticate with cardkey = 2 and Se0Obj ID = 2103308289
    Tx> 0A 00
    Tx> 71 02 06 00 00 00 00 00 00
    Rx< 0A 00 AF E5 9E 5D E4 DB 0F 51 D7 79 46 18 7A CB 06 D5 0E
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          E5 9E 5D E4     DB 0F 51 D7     79 46 18 7A     CB 06 D5 0E
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          D6 C5 48 6C     C8 60 58 CC     D5 8A 6D 80     3A DD DF 32
          A3 CA FD AF     8A BA 71 B6     F6 7C 51 71     AA D6 46 AA
    Tx> 0B 00
    Tx> AF D6 C5 48 6C C8 60 58 CC D5 8A 6D 80 3A DD DF 32 A3 CA FD AF 8A BA 71 B6 F6 7C 51 71 AA D6 46 AA
    Rx< 0B 00 00 8A 86 8C AF ED F4 DB 56 68 81 71 1E 96 AD EF E6 5A A0 DD DA C5 BB 2E 9F BE 0F 7F 39 1F 0C 4B BF
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=32)
          8A 86 8C AF     ED F4 DB 56     68 81 71 1E     96 AD EF E6
          5A A0 DD DA     C5 BB 2E 9F     BE 0F 7F 39     1F 0C 4B BF
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=12)
          00 00 00 00     00 00 00 00     00 00 00 00
           App:INFO :Dumped Session Key is (Len=16)
          6E B1 6E 85     61 F3 B6 23     6D CD 72 66     35 A9 30 0A
           App:INFO :Dumped Session Mac is (Len=16)
          31 2D 73 1C     F6 90 66 AA     60 B4 C0 34     D5 03 54 6D
           App:INFO :Dumped TI is (Len=4)
          36 D6 43 CB
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 First Authenticate  Successful

    Tx> 0A 00
    Tx> 77 02
    Rx< 0A 00 AF 68 01 92 38 38 81 DB EE EB 9C 49 05 EA AD 1C B5
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          68 01 92 38     38 81 DB EE     EB 9C 49 05     EA AD 1C B5
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          01 A7 15 73     7F C3 34 4D     6C C7 43 9B     BB 1C 44 DE
          28 78 DE 4B     69 43 53 02     C9 85 A6 21     6F F5 73 E0
    Tx> 0B 00
    Tx> AF 01 A7 15 73 7F C3 34 4D 6C C7 43 9B BB 1C 44 DE 28 78 DE 4B 69 43 53 02 C9 85 A6 21 6F F5 73 E0
    Rx< 0B 00 00 8F 15 A7 DB DE 7F 4B D7 AD C4 B6 4C E4 BD 37 BD
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=16)
          8F 15 A7 DB     DE 7F 4B D7     AD C4 B6 4C     E4 BD 37 BD
           App:INFO :Dumped Session Key is (Len=16)
          BD 61 A3 45     2F 91 FA E4     87 7A 0A 6C     33 B7 29 B8
           App:INFO :Dumped Session Mac is (Len=16)
          87 87 14 42     F4 16 E5 74     F0 12 5F AA     B8 2C 70 86
           App:INFO :Dumped TI is (Len=4)
          36 D6 43 CB
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 Following Authenticate  Successful

           App:INFO :Authenticated with cardkey = 2 and Se0Obj ID = 2103308289
           App:INFO :Checking that the auth session with changed key is valid by trying an encrypted communication
    Tx> 0A 00
    Tx> 51 DA FA 27 73 68 30 43 16
    Rx< 0A 00 00 F3 40 C6 9E 11 23 4C A5 22 A7 35 30 D2 DB CD B3 D4 3D 89 66 CA 3D 57 7D
           App:INFO :CARD UID is as below  (Len=7)
          04 40 40 42     4D 4D 80
           App:INFO :Encrypted communication with changed key successful
           App:INFO :Reverting the changed cardkey 2
    Tx> 0B 00
    Tx> 5A 11 22 33
    Rx< 0B 00 00
           App:INFO :Select the AFC Application Successful

           App:INFO :attempting to authenticate with cardkey = 0 and Se0Obj ID = 2103308288
    Tx> 0A 00
    Tx> 71 00 06 00 00 00 00 00 00
    Rx< 0A 00 AF 41 EB 4E 85 BB A2 31 85 F6 8F 4B 7D FD 55 02 BC
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          41 EB 4E 85     BB A2 31 85     F6 8F 4B 7D     FD 55 02 BC
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          7F 08 80 7A     AD 97 69 12     95 C3 F5 5C     72 03 D6 D4
          F6 22 D4 ED     43 44 72 E2     C2 99 82 52     57 1D 80 57
    Tx> 0B 00
    Tx> AF 7F 08 80 7A AD 97 69 12 95 C3 F5 5C 72 03 D6 D4 F6 22 D4 ED 43 44 72 E2 C2 99 82 52 57 1D 80 57
    Rx< 0B 00 00 E3 73 4F F3 50 65 30 02 2F B0 B6 0A 49 57 43 E0 75 49 3C 5D C1 DE B4 78 84 8D 00 9A 67 85 33 EA
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=32)
          E3 73 4F F3     50 65 30 02     2F B0 B6 0A     49 57 43 E0
          75 49 3C 5D     C1 DE B4 78     84 8D 00 9A     67 85 33 EA
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=12)
          00 00 00 00     00 00 00 00     00 00 00 00
           App:INFO :Dumped Session Key is (Len=16)
          B3 44 56 9F     04 11 40 DF     5C DF 1A 24     82 E6 24 C7
           App:INFO :Dumped Session Mac is (Len=16)
          16 18 AA 95     F0 E9 40 0A     DA 22 FF 76     02 7B D6 8F
           App:INFO :Dumped TI is (Len=4)
          0D AC 1D F2
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 First Authenticate  Successful

    Tx> 0A 00
    Tx> 77 00
    Rx< 0A 00 AF E0 7B AA 84 C9 44 56 DE 96 72 FD 80 7C 27 31 D9
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          E0 7B AA 84     C9 44 56 DE     96 72 FD 80     7C 27 31 D9
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          D7 76 C8 85     E4 A1 5B 43     F0 8B F5 E5     30 FE 1C 24
          EA 93 A5 AB     78 D9 82 D2     1A 59 E5 D3     25 8A CA D2
    Tx> 0B 00
    Tx> AF D7 76 C8 85 E4 A1 5B 43 F0 8B F5 E5 30 FE 1C 24 EA 93 A5 AB 78 D9 82 D2 1A 59 E5 D3 25 8A CA D2
    Rx< 0B 00 00 0C A8 D5 4E FB F5 87 00 F3 2F 27 E5 4F 9A 8A 39
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=16)
          0C A8 D5 4E     FB F5 87 00     F3 2F 27 E5     4F 9A 8A 39
           App:INFO :Dumped Session Key is (Len=16)
          00 8E 4C DE     33 20 50 4C     52 23 CC BF     15 BD E2 27
           App:INFO :Dumped Session Mac is (Len=16)
          00 AE 0D 01     C1 4D 47 86     57 04 60 6D     3F DD A5 A0
           App:INFO :Dumped TI is (Len=4)
          0D AC 1D F2
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 Following Authenticate  Successful

           App:INFO :Authenticated with cardkey = 0 and Se0Obj ID = 2103308288
           App:INFO :attempting to change cardkey = 2 from  Old Se050ObjID= 2103308289 to new Se050ObjID= 2103308288
    Tx> 0A 00
    Tx> C6 00 02 0B 44 52 C6 A9 EE 58 C1 9B 03 4F 64 58 DA 86 AA DE DE 68 03 8A AC F4 E2 8A E5 B1 F3 84 F5 87 F5 3D DC CC 01 DF D8 E7 68
    Rx< 0A 00 00 9E C3 E2 37 5E 2D 40 E6
           App:INFO : Change Key for card key 2 is Successful to Se050ObjID= 2103308288

           App:INFO :Reverting Successful
    Tx> 0B 00
    Tx> 5A 11 22 33
    Rx< 0B 00 00
           App:INFO :Select the AFC Application Successful

           App:INFO :attempting to authenticate with cardkey = 0 and Se0Obj ID = 2103308288
    Tx> 0A 00
    Tx> 71 00 06 00 00 00 00 00 00
    Rx< 0A 00 AF 28 5A 92 39 D1 81 2E 77 92 07 5D C6 B2 8E 57 20
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          28 5A 92 39     D1 81 2E 77     92 07 5D C6     B2 8E 57 20
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          DE E0 70 04     F2 E8 55 7C     83 D7 D0 B0     EA 4B AA 60
          F5 4F B5 AF     9E 73 A0 DD     E6 EF 77 2C     BD 6D 99 BE
    Tx> 0B 00
    Tx> AF DE E0 70 04 F2 E8 55 7C 83 D7 D0 B0 EA 4B AA 60 F5 4F B5 AF 9E 73 A0 DD E6 EF 77 2C BD 6D 99 BE
    Rx< 0B 00 00 B7 55 91 A4 03 DA 29 3B ED 0D 63 E4 21 17 DC 86 D8 2C 62 39 4E EC B3 A0 31 82 64 BD 51 A1 D1 A0
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=32)
          B7 55 91 A4     03 DA 29 3B     ED 0D 63 E4     21 17 DC 86
          D8 2C 62 39     4E EC B3 A0     31 82 64 BD     51 A1 D1 A0
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=12)
          00 00 00 00     00 00 00 00     00 00 00 00
           App:INFO :Dumped Session Key is (Len=16)
          1C 92 1F 77     EC A4 26 FD     20 67 3C C0     63 62 20 15
           App:INFO :Dumped Session Mac is (Len=16)
          64 B2 DA 8B     05 8C 8B 24     1E AC 5E 31     CF B7 76 5D
           App:INFO :Dumped TI is (Len=4)
          40 77 24 27
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 First Authenticate  Successful

    Tx> 0A 00
    Tx> 77 00
    Rx< 0A 00 AF C8 F3 B0 76 49 C6 BF C3 43 F5 62 67 CF BF 4D CC
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          C8 F3 B0 76     49 C6 BF C3     43 F5 62 67     CF BF 4D CC
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          52 5A 01 9E     B3 F2 5E 42     6F 0A 61 70     46 C2 81 54
          C4 22 67 87     99 07 49 EA     B0 12 DB A3     69 24 38 4B
    Tx> 0B 00
    Tx> AF 52 5A 01 9E B3 F2 5E 42 6F 0A 61 70 46 C2 81 54 C4 22 67 87 99 07 49 EA B0 12 DB A3 69 24 38 4B
    Rx< 0B 00 00 18 58 A0 DF 76 76 BB A3 F8 96 97 B7 0A DD 8C FD
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=16)
          18 58 A0 DF     76 76 BB A3     F8 96 97 B7     0A DD 8C FD
           App:INFO :Dumped Session Key is (Len=16)
          2A A1 EA B5     1D 7A F1 AB     B7 3D 00 31     D1 26 2C 7A
           App:INFO :Dumped Session Mac is (Len=16)
          91 E9 3B CA     3D C1 53 C8     4C 13 28 28     C2 3F C9 6B
           App:INFO :Dumped TI is (Len=4)
          40 77 24 27
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 Following Authenticate  Successful

           App:INFO :Authenticated with cardkey = 0 and Se0Obj ID = 2103308288
           App:INFO :attempting to change cardkey = 0 from  Old Se050ObjID= 2103308288 to new Se050ObjID= 2103308289
    Tx> 0A 00
    Tx> C6 00 00 A8 60 79 3E 01 31 C9 91 16 BC 07 F4 85 2D BC 7D 8B D7 CD 34 4B B4 F5 A3 BD 97 AD 02 89 9B 47 00 FA C8 63 0F 97 E9 55 8A
    Rx< 0A 00 00
           App:INFO : Change Key for card key 0 is Successful to Se050ObjID= 2103308289

           App:INFO :The previous auth session is not valid anymore.
           App:INFO :So.....
           App:INFO :Auth with the changed cardkey 0
    Tx> 0B 00
    Tx> 5A 11 22 33
    Rx< 0B 00 00
           App:INFO :Select the AFC Application Successful

           App:INFO :attempting to authenticate with cardkey = 0 and Se0Obj ID = 2103308289
    Tx> 0A 00
    Tx> 71 00 06 00 00 00 00 00 00
    Rx< 0A 00 AF 96 E8 80 35 41 30 37 A7 24 8B 73 42 44 43 13 BC
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          96 E8 80 35     41 30 37 A7     24 8B 73 42     44 43 13 BC
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          C5 FA 74 68     B0 D8 B4 31     B6 FA 76 EF     1E 21 F4 ED
          07 74 8C 1D     7C F7 5F 0F     63 24 28 7F     95 09 EF C8
    Tx> 0B 00
    Tx> AF C5 FA 74 68 B0 D8 B4 31 B6 FA 76 EF 1E 21 F4 ED 07 74 8C 1D 7C F7 5F 0F 63 24 28 7F 95 09 EF C8
    Rx< 0B 00 00 F5 3D 5C 3D 95 9D A1 54 9B F8 BE 42 C0 BA EE 06 80 6F 29 91 79 89 46 FA 34 5C 82 C0 66 A5 CE AE
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=32)
          F5 3D 5C 3D     95 9D A1 54     9B F8 BE 42     C0 BA EE 06
          80 6F 29 91     79 89 46 FA     34 5C 82 C0     66 A5 CE AE
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=12)
          00 00 00 00     00 00 00 00     00 00 00 00
           App:INFO :Dumped Session Key is (Len=16)
          1B CA B6 46     6B 97 17 D0     A9 FA F4 DC     0B 01 59 CE
           App:INFO :Dumped Session Mac is (Len=16)
          06 B0 72 C5     8E 71 88 C0     44 51 DC 45     14 7A 42 AB
           App:INFO :Dumped TI is (Len=4)
          AD AB 0E F3
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 First Authenticate  Successful

    Tx> 0A 00
    Tx> 77 00
    Rx< 0A 00 AF AF CA 35 37 D2 96 8B FA F5 06 CD 1E 6B D8 6E E0
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          AF CA 35 37     D2 96 8B FA     F5 06 CD 1E     6B D8 6E E0
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          C2 72 DA 88     02 5E A0 5C     04 CC E7 86     AD 21 07 D3
          A8 74 9C 99     51 9C 62 00     00 C9 BE C9     AD FD DA 50
    Tx> 0B 00
    Tx> AF C2 72 DA 88 02 5E A0 5C 04 CC E7 86 AD 21 07 D3 A8 74 9C 99 51 9C 62 00 00 C9 BE C9 AD FD DA 50
    Rx< 0B 00 00 68 C1 89 4B C1 C0 E0 20 ED 66 D1 6C D3 BE 28 99
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=16)
          68 C1 89 4B     C1 C0 E0 20     ED 66 D1 6C     D3 BE 28 99
           App:INFO :Dumped Session Key is (Len=16)
          36 96 A4 26     4B 8F FC C3     46 A9 57 79     A7 1D 12 D3
           App:INFO :Dumped Session Mac is (Len=16)
          B8 21 B9 B6     6E 13 4F 99     E3 C7 07 89     17 09 FF 0E
           App:INFO :Dumped TI is (Len=4)
          AD AB 0E F3
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 Following Authenticate  Successful

           App:INFO :Authenticated with cardkey = 0 and Se0Obj ID = 2103308289
           App:INFO :Checking that the auth session with changed key is valid by trying an encrypted communication
    Tx> 0A 00
    Tx> 51 DE C0 9D 65 2B 8A 37 96
    Rx< 0A 00 00 AD 6F 30 A2 17 04 F6 4B 0A BC 56 B1 0C 27 04 ED 87 FE 88 49 11 44 4E 94
           App:INFO :CARD UID is as below  (Len=7)
          04 40 40 42     4D 4D 80
           App:INFO :Encrypted communication with changed key successful
           App:INFO :Reverting the changed cardkey 0
    Tx> 0B 00
    Tx> 5A 11 22 33
    Rx< 0B 00 00
           App:INFO :Select the AFC Application Successful

           App:INFO :attempting to authenticate with cardkey = 0 and Se0Obj ID = 2103308289
    Tx> 0A 00
    Tx> 71 00 06 00 00 00 00 00 00
    Rx< 0A 00 AF 13 41 E7 9C 31 3E 03 7F 84 46 DC 32 6A F0 81 61
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          13 41 E7 9C     31 3E 03 7F     84 46 DC 32     6A F0 81 61
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          2A 23 A4 C9     8F D9 42 FA     8E E2 2B 6E     5C E2 0B 63
          37 F7 D7 ED     68 33 0D 8F     84 8B 14 F7     7E 76 09 4A
    Tx> 0B 00
    Tx> AF 2A 23 A4 C9 8F D9 42 FA 8E E2 2B 6E 5C E2 0B 63 37 F7 D7 ED 68 33 0D 8F 84 8B 14 F7 7E 76 09 4A
    Rx< 0B 00 00 E6 93 12 92 02 B1 64 90 3F 85 3C 65 C0 F7 32 81 6E C8 34 3F 00 59 60 AA 48 DD 94 DC 48 AF C2 25
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=32)
          E6 93 12 92     02 B1 64 90     3F 85 3C 65     C0 F7 32 81
          6E C8 34 3F     00 59 60 AA     48 DD 94 DC     48 AF C2 25
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=12)
          00 00 00 00     00 00 00 00     00 00 00 00
           App:INFO :Dumped Session Key is (Len=16)
          22 78 6D 20     7D B1 0D 50     0D 95 B9 97     0C 0B 72 4D
           App:INFO :Dumped Session Mac is (Len=16)
          7A AD D3 1A     3B 3D DA 6A     7D 76 F5 64     26 A8 C4 97
           App:INFO :Dumped TI is (Len=4)
          48 3D F6 3C
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 First Authenticate  Successful

    Tx> 0A 00
    Tx> 77 00
    Rx< 0A 00 AF A6 60 13 9E 79 C2 5B 8A 12 E6 93 1B 84 BD C9 31
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          A6 60 13 9E     79 C2 5B 8A     12 E6 93 1B     84 BD C9 31
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          B8 95 BC 94     58 81 1D 8D     20 E4 F1 FB     EE 3A 68 27
          F0 BA B4 19     A6 5F 45 9A     F9 A4 04 D4     3B 19 E9 6F
    Tx> 0B 00
    Tx> AF B8 95 BC 94 58 81 1D 8D 20 E4 F1 FB EE 3A 68 27 F0 BA B4 19 A6 5F 45 9A F9 A4 04 D4 3B 19 E9 6F
    Rx< 0B 00 00 11 5F BE 3E 3E 3F A7 D3 3F 55 7E 44 05 C3 FC 72
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=16)
          11 5F BE 3E     3E 3F A7 D3     3F 55 7E 44     05 C3 FC 72
           App:INFO :Dumped Session Key is (Len=16)
          8F C6 94 CF     AF 83 63 3B     A8 44 40 D7     18 19 39 BC
           App:INFO :Dumped Session Mac is (Len=16)
          C0 FF 0B A2     18 CD 33 8C     25 C5 24 E1     72 D2 F9 FB
           App:INFO :Dumped TI is (Len=4)
          48 3D F6 3C
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 Following Authenticate  Successful

           App:INFO :Authenticated with cardkey = 0 and Se0Obj ID = 2103308289
           App:INFO :attempting to change cardkey = 0 from  Old Se050ObjID= 2103308289 to new Se050ObjID= 2103308288
    Tx> 0A 00
    Tx> C6 00 00 09 C9 09 2E 8F 81 40 6C EB B8 9C 0D 49 92 B6 C7 D2 18 87 B2 D8 EA 42 79 D1 99 AA 6B 56 9D 01 89 47 55 A9 8B 66 2B 51 D8
    Rx< 0A 00 00
           App:INFO : Change Key for card key 0 is Successful to Se050ObjID= 2103308288

           App:INFO :Reverting Successful
           App:INFO : Auth session is reset in software
           App:INFO : Auth session is killed in SE
           App:INFO :ex_sss Finished

