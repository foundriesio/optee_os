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

.. _ex-Ev2DivChngKey-se05x:

=======================================================================
 MIFARE DESFire EV2 : Diversified Change Key
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

This project demonstrates the Mifare Desfire EV2 Diversified
ChangeKeyEv2 using Seo50. The Key is diversified using the card UID After
changing Keys, it performs encrypted communication with the desfire EV2
card using the diversified changed key. If enabled, It also reverts back
the changed key.

It uses the following APIs and data types:

- :cpp:func:`Se05x_API_DFDiversifyKey()`
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
    Rx< 20
    UID after L3 Activation 08 04 4D 8D 00 00 00 00 00 00
    Tx> E0 80
    Rx< 06 75 77 81 02 80
    ATS after L4 Activation 06 75 77 81 02 80
           App:INFO :Auth with the  cardkey 0 and getting the card UID for diversification
    Tx> 0A 00
    Tx> 5A 11 22 33
    Rx< 0A 00 00
           App:INFO :Select the AFC Application Successful

           App:INFO :attempting to authenticate with cardkey = 0 and Se0Obj ID = 2103308288
    Tx> 0B 00
    Tx> 71 00 06 00 00 00 00 00 00
    Rx< 0B 00 AF CB 39 4E 26 F8 1E 1D CE 4B 66 2B E7 64 2D 5D 89
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          CB 39 4E 26     F8 1E 1D CE     4B 66 2B E7     64 2D 5D 89
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          9A 2F 8D 8C     2F 4E 1B C3     F3 D0 ED 84     36 9E DC 03
          79 A5 27 2A     66 91 F0 6D     EC 4F 90 C7     E5 25 EA 51
    Tx> 0A 00
    Tx> AF 9A 2F 8D 8C 2F 4E 1B C3 F3 D0 ED 84 36 9E DC 03 79 A5 27 2A 66 91 F0 6D EC 4F 90 C7 E5 25 EA 51
    Rx< 0A 00 00 A1 F9 0D 31 3A 6F 32 F2 06 69 25 1A E7 4E 3F 8C 20 EF B3 14 C5 0F DD A1 80 E3 13 43 C1 18 14 5D
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=32)
          A1 F9 0D 31     3A 6F 32 F2     06 69 25 1A     E7 4E 3F 8C
          20 EF B3 14     C5 0F DD A1     80 E3 13 43     C1 18 14 5D
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=12)
          00 00 00 00     00 00 00 00     00 00 00 00
           App:INFO :Dumped Session Key is (Len=16)
          4E 55 15 1D     F5 6C F5 B4     27 2E D1 4E     50 5C C7 22
           App:INFO :Dumped Session Mac is (Len=16)
          36 5C C9 22     F2 F7 67 7A     47 71 12 5E     79 3F D5 43
           App:INFO :Dumped TI is (Len=4)
          1F C1 6F CF
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 First Authenticate  Successful

    Tx> 0B 00
    Tx> 77 00
    Rx< 0B 00 AF 87 4A 81 2F 50 F1 44 55 5F E6 B0 31 AB AD 3B B0
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          87 4A 81 2F     50 F1 44 55     5F E6 B0 31     AB AD 3B B0
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          0D D4 AA 86     A4 CA 79 9B     F3 9D 2E 1F     C1 64 F9 3F
          47 EF 29 F9     0E 6F F1 C0     1E 0F E4 4E     BB 2D D0 1F
    Tx> 0A 00
    Tx> AF 0D D4 AA 86 A4 CA 79 9B F3 9D 2E 1F C1 64 F9 3F 47 EF 29 F9 0E 6F F1 C0 1E 0F E4 4E BB 2D D0 1F
    Rx< 0A 00 00 CE 33 17 C3 41 7A 8F DF 04 F9 04 E3 CB 2D 2E BC
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=16)
          CE 33 17 C3     41 7A 8F DF     04 F9 04 E3     CB 2D 2E BC
           App:INFO :Dumped Session Key is (Len=16)
          38 54 53 76     E8 15 A5 DD     32 28 EB D5     1F 15 87 25
           App:INFO :Dumped Session Mac is (Len=16)
          1C A4 94 0C     63 4C E6 63     FB F3 48 A1     8D D9 34 9B
           App:INFO :Dumped TI is (Len=4)
          1F C1 6F CF
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 Following Authenticate  Successful

           App:INFO :Authenticated with cardkey = 0 and Se0Obj ID = 2103308288
    Tx> 0B 00
    Tx> 51 61 14 F1 85 4C 47 DE 42
    Rx< 0B 00 00 25 D5 5B 00 6B F0 5D A9 C5 15 26 DF C2 96 D3 F3 A6 6B 4C 8E 80 E1 3D 24
           App:INFO :CARD UID will be used for diversification
    Tx> 0A 00
    Tx> 5A 11 22 33
    Rx< 0A 00 00
           App:INFO :Select the AFC Application Successful

           App:INFO :attempting to authenticate with cardkey = 0 and Se0Obj ID = 2103308288
    Tx> 0B 00
    Tx> 71 00 06 00 00 00 00 00 00
    Rx< 0B 00 AF 6A E9 E5 F3 4C E3 58 E5 DB 1A DE 6B A7 C6 79 68
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          6A E9 E5 F3     4C E3 58 E5     DB 1A DE 6B     A7 C6 79 68
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          54 6B B9 B4     41 F0 3D E6     37 40 28 DA     61 ED 81 97
          37 50 E0 F2     86 5D E7 E5     A2 F5 1F B5     0B 1A E7 7C
    Tx> 0A 00
    Tx> AF 54 6B B9 B4 41 F0 3D E6 37 40 28 DA 61 ED 81 97 37 50 E0 F2 86 5D E7 E5 A2 F5 1F B5 0B 1A E7 7C
    Rx< 0A 00 00 46 24 4B 15 7C 8F D5 19 56 32 6F 11 F1 77 4B 91 59 4D CB 02 26 42 49 22 C0 77 8F 67 15 68 74 D4
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=32)
          46 24 4B 15     7C 8F D5 19     56 32 6F 11     F1 77 4B 91
          59 4D CB 02     26 42 49 22     C0 77 8F 67     15 68 74 D4
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=12)
          00 00 00 00     00 00 00 00     00 00 00 00
           App:INFO :Dumped Session Key is (Len=16)
          73 DE FB 5D     7C 78 E3 1A     BF 97 53 35     C9 2F F4 12
           App:INFO :Dumped Session Mac is (Len=16)
          CB B3 14 4E     D8 15 4E 7D     A2 A1 F9 B4     35 ED E2 4C
           App:INFO :Dumped TI is (Len=4)
          8D 19 52 6A
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 First Authenticate  Successful

    Tx> 0B 00
    Tx> 77 00
    Rx< 0B 00 AF 6C 35 C4 E2 0D C8 CC 44 69 7C 75 44 7B CA 3F 36
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          6C 35 C4 E2     0D C8 CC 44     69 7C 75 44     7B CA 3F 36
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          42 96 EF CB     C9 41 DD B2     F8 D5 4B 36     7F 7E F1 F0
          A5 BD E7 FB     46 D1 39 EF     1F 91 61 F4     71 B0 2D BE
    Tx> 0A 00
    Tx> AF 42 96 EF CB C9 41 DD B2 F8 D5 4B 36 7F 7E F1 F0 A5 BD E7 FB 46 D1 39 EF 1F 91 61 F4 71 B0 2D BE
    Rx< 0A 00 00 3F 69 5E 84 B8 F5 04 F9 3B C3 63 07 67 BE 07 9E
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=16)
          3F 69 5E 84     B8 F5 04 F9     3B C3 63 07     67 BE 07 9E
           App:INFO :Dumped Session Key is (Len=16)
          4D 43 63 4D     95 98 F9 92     EB F5 AA 3C     7C 42 EE 4C
           App:INFO :Dumped Session Mac is (Len=16)
          10 91 B7 3A     B1 B4 2C 74     A2 71 EE 4B     C8 45 94 74
           App:INFO :Dumped TI is (Len=4)
          8D 19 52 6A
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 Following Authenticate  Successful

           App:INFO :Authenticated with cardkey = 0 and Se0Obj ID = 2103308288
           App:INFO :attempting to change cardkey = 2 from  Old Se050ObjID= 2103308288 to new Se050ObjID= 2103308289
    Tx> 0B 00
    Tx> C6 00 02 9A 7A DA 24 18 56 CB C8 65 92 52 07 AF 5C 31 19 EB AF BF 06 CA 98 41 94 9F EE A5 C8 60 86 D5 67 FD 22 6D 6B 75 E4 56 5B
    Rx< 0B 00 00 CA 57 0A F4 56 2C F9 7F
           App:INFO : Change Key for card key 2 is Successful to Se050ObjID= 2103308289

           App:INFO :Checking that the previous auth session is still valid by trying an encrypted communication
    Tx> 0A 00
    Tx> 51 CA 0E C5 B7 E8 4D BA ED
    Rx< 0A 00 00 CF 11 48 4D 32 34 D9 FE BF B9 F4 24 65 7B EC BC AD 65 93 C8 03 B7 AE 95
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
    Rx< 0A 00 AF 75 51 82 DB 46 CA A3 B5 C8 1C 3F 16 50 46 0A D4
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          75 51 82 DB     46 CA A3 B5     C8 1C 3F 16     50 46 0A D4
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          6F F3 1E C6     D4 B9 A6 72     DB C6 98 54     6B E8 F4 7F
          38 E3 CC 2A     34 D2 51 96     B0 D7 D4 A8     A3 30 76 4B
    Tx> 0B 00
    Tx> AF 6F F3 1E C6 D4 B9 A6 72 DB C6 98 54 6B E8 F4 7F 38 E3 CC 2A 34 D2 51 96 B0 D7 D4 A8 A3 30 76 4B
    Rx< 0B 00 00 40 93 D2 A1 6B DD B2 1F 6E F4 FA 75 15 28 22 62 18 FD 68 CF AF 1C EC B0 41 4D 08 2E E8 13 4B A1
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=32)
          40 93 D2 A1     6B DD B2 1F     6E F4 FA 75     15 28 22 62
          18 FD 68 CF     AF 1C EC B0     41 4D 08 2E     E8 13 4B A1
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=12)
          00 00 00 00     00 00 00 00     00 00 00 00
           App:INFO :Dumped Session Key is (Len=16)
          4A 69 72 BB     62 62 06 2F     02 7E 53 C2     B8 E9 F5 ED
           App:INFO :Dumped Session Mac is (Len=16)
          47 AA 13 45     A4 6C 6F 5B     D0 D8 22 C4     F2 75 DD 2C
           App:INFO :Dumped TI is (Len=4)
          19 3D B7 F6
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 First Authenticate  Successful

    Tx> 0A 00
    Tx> 77 02
    Rx< 0A 00 AF C3 38 BF 4F E5 13 7E 02 2B 8E 21 34 2F 6C 7F 6D
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          C3 38 BF 4F     E5 13 7E 02     2B 8E 21 34     2F 6C 7F 6D
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          42 76 C2 05     F8 D5 37 1D     38 9C B3 95     A9 2A 19 5D
          75 81 7C 66     7A 53 E0 6D     3C 25 B7 3D     FD C7 38 16
    Tx> 0B 00
    Tx> AF 42 76 C2 05 F8 D5 37 1D 38 9C B3 95 A9 2A 19 5D 75 81 7C 66 7A 53 E0 6D 3C 25 B7 3D FD C7 38 16
    Rx< 0B 00 00 12 CA C1 C4 BB B7 23 74 36 D4 17 00 4B FB 44 1F
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=16)
          12 CA C1 C4     BB B7 23 74     36 D4 17 00     4B FB 44 1F
           App:INFO :Dumped Session Key is (Len=16)
          A2 46 74 CE     9C 90 99 2F     60 04 07 FC     9C B7 E5 3A
           App:INFO :Dumped Session Mac is (Len=16)
          1F D2 94 A3     9F AC E6 57     51 9F C1 BC     84 B9 FA 99
           App:INFO :Dumped TI is (Len=4)
          19 3D B7 F6
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 Following Authenticate  Successful

           App:INFO :Authenticated with cardkey = 2 and Se0Obj ID = 2103308289
           App:INFO :Checking that the auth session with changed key is valid by trying an encrypted communication
    Tx> 0A 00
    Tx> 51 87 B0 70 A8 75 FC FA 04
    Rx< 0A 00 00 0A 31 4B 0C 44 BB 70 EA 61 31 00 E3 A3 E6 4E 1C 8E 6D 70 2F 86 08 A0 CD
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
    Rx< 0A 00 AF 95 85 6E 37 62 3C C5 16 A4 C7 A8 D2 6D A5 08 09
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          95 85 6E 37     62 3C C5 16     A4 C7 A8 D2     6D A5 08 09
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          9A C1 0D 08     D6 7F FC 05     3C 2E 7B 9B     5B 8B 98 36
          29 0E 57 8D     A7 BF 17 61     EB 81 CD 5C     79 2D 98 7B
    Tx> 0B 00
    Tx> AF 9A C1 0D 08 D6 7F FC 05 3C 2E 7B 9B 5B 8B 98 36 29 0E 57 8D A7 BF 17 61 EB 81 CD 5C 79 2D 98 7B
    Rx< 0B 00 00 CF 77 E6 A9 D0 03 A7 C5 C2 D1 26 20 17 0C 05 8F E0 65 3D AA 3C F6 5D 5C A2 8E 06 97 0B E6 73 BB
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=32)
          CF 77 E6 A9     D0 03 A7 C5     C2 D1 26 20     17 0C 05 8F
          E0 65 3D AA     3C F6 5D 5C     A2 8E 06 97     0B E6 73 BB
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=12)
          00 00 00 00     00 00 00 00     00 00 00 00
           App:INFO :Dumped Session Key is (Len=16)
          B5 0C 47 C4     C4 86 DA EC     E7 D4 C1 9C     09 92 C5 86
           App:INFO :Dumped Session Mac is (Len=16)
          C0 36 09 35     9E 9D 00 C5     CC CF 8C 0A     C6 AA 84 93
           App:INFO :Dumped TI is (Len=4)
          EE 5C D1 02
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 First Authenticate  Successful

    Tx> 0A 00
    Tx> 77 00
    Rx< 0A 00 AF 47 D5 AD E6 D8 9F 13 06 2C F8 32 CE A3 68 61 3A
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          47 D5 AD E6     D8 9F 13 06     2C F8 32 CE     A3 68 61 3A
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          71 83 22 6F     AD FC F8 89     F5 7F 63 15     85 87 EB 29
          7A 24 30 7F     C7 D1 03 27     0A 93 EE 3F     40 68 A3 1D
    Tx> 0B 00
    Tx> AF 71 83 22 6F AD FC F8 89 F5 7F 63 15 85 87 EB 29 7A 24 30 7F C7 D1 03 27 0A 93 EE 3F 40 68 A3 1D
    Rx< 0B 00 00 FF 26 85 C1 A3 96 A2 92 F5 D8 D2 98 A3 56 09 44
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=16)
          FF 26 85 C1     A3 96 A2 92     F5 D8 D2 98     A3 56 09 44
           App:INFO :Dumped Session Key is (Len=16)
          6A E0 C5 B9     C6 4F C2 75     C9 65 49 D8     91 FC A6 78
           App:INFO :Dumped Session Mac is (Len=16)
          A3 1F 10 92     43 D9 D2 63     1E 0C 7C A3     DC 3C 26 EC
           App:INFO :Dumped TI is (Len=4)
          EE 5C D1 02
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 Following Authenticate  Successful

           App:INFO :Authenticated with cardkey = 0 and Se0Obj ID = 2103308288
           App:INFO :attempting to change cardkey = 2 from  Old Se050ObjID= 2103308289 to new Se050ObjID= 2103308288
    Tx> 0A 00
    Tx> C6 00 02 E4 9B D3 FD 14 74 15 DC B2 D9 28 3A C5 9F BD 77 8B 12 78 3E CB 0D 4C 92 66 84 C8 EB 91 0E BA A2 E0 6F 53 AC 51 FC 42 A1
    Rx< 0A 00 00 37 FD 0F 81 21 25 CB 3C
           App:INFO : Change Key for card key 2 is Successful to Se050ObjID= 2103308288

           App:INFO :Reverting Successful
    Tx> 0B 00
    Tx> 5A 11 22 33
    Rx< 0B 00 00
           App:INFO :Select the AFC Application Successful

           App:INFO :attempting to authenticate with cardkey = 0 and Se0Obj ID = 2103308288
    Tx> 0A 00
    Tx> 71 00 06 00 00 00 00 00 00
    Rx< 0A 00 AF A0 64 12 F1 64 01 26 09 9B 74 F6 84 4E AA DA B1
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          A0 64 12 F1     64 01 26 09     9B 74 F6 84     4E AA DA B1
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          B7 19 71 EE     F0 43 C8 CF     93 C2 9A 9A     E2 22 E3 6C
          00 93 7E 42     C0 25 16 55     86 78 30 A5     83 DE EB 88
    Tx> 0B 00
    Tx> AF B7 19 71 EE F0 43 C8 CF 93 C2 9A 9A E2 22 E3 6C 00 93 7E 42 C0 25 16 55 86 78 30 A5 83 DE EB 88
    Rx< 0B 00 00 06 EF 22 A6 93 17 9D 14 50 C0 B9 34 E7 EA B5 16 3F 2E 8C FE CB 4D 49 1A C5 93 A6 42 6A 01 14 16
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=32)
          06 EF 22 A6     93 17 9D 14     50 C0 B9 34     E7 EA B5 16
          3F 2E 8C FE     CB 4D 49 1A     C5 93 A6 42     6A 01 14 16
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=12)
          00 00 00 00     00 00 00 00     00 00 00 00
           App:INFO :Dumped Session Key is (Len=16)
          07 25 BD 23     62 1F B6 B4     AC 19 EF AC     19 BD DA C1
           App:INFO :Dumped Session Mac is (Len=16)
          3B 7F 15 CD     91 9C 11 A6     79 E0 34 FC     78 E1 62 89
           App:INFO :Dumped TI is (Len=4)
          03 55 8B AB
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 First Authenticate  Successful

    Tx> 0A 00
    Tx> 77 00
    Rx< 0A 00 AF F8 AF 69 A2 31 62 00 75 FB 3F 80 DF AE 3C 2E 4C
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          F8 AF 69 A2     31 62 00 75     FB 3F 80 DF     AE 3C 2E 4C
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          CB 08 4D F4     AA D1 42 AB     89 12 22 CA     C4 50 26 39
          C5 AA 0C 0B     22 46 34 02     BE 41 26 07     A1 26 3A AB
    Tx> 0B 00
    Tx> AF CB 08 4D F4 AA D1 42 AB 89 12 22 CA C4 50 26 39 C5 AA 0C 0B 22 46 34 02 BE 41 26 07 A1 26 3A AB
    Rx< 0B 00 00 23 D9 43 F1 D1 3D 8C B0 24 C4 1C EC E1 5F B4 CC
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=16)
          23 D9 43 F1     D1 3D 8C B0     24 C4 1C EC     E1 5F B4 CC
           App:INFO :Dumped Session Key is (Len=16)
          3C DB CC D7     9F A1 EB BD     A7 79 A7 5C     20 53 B8 E0
           App:INFO :Dumped Session Mac is (Len=16)
          98 CF 64 9A     98 09 5B D4     E3 1E 1B D8     D6 E3 A9 34
           App:INFO :Dumped TI is (Len=4)
          03 55 8B AB
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 Following Authenticate  Successful

           App:INFO :Authenticated with cardkey = 0 and Se0Obj ID = 2103308288
           App:INFO :attempting to change cardkey = 0 from  Old Se050ObjID= 2103308288 to new Se050ObjID= 2103308289
    Tx> 0A 00
    Tx> C6 00 00 C0 A4 C0 2A 1C 5E 6C 81 EB 86 97 E6 ED EA A4 56 00 30 3F 72 3E 87 7F 1B E7 05 8E 5C 83 A3 42 8F B0 2A 05 5C C2 1F F5 95
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
    Rx< 0A 00 AF 2C 85 14 8C FB B8 77 28 04 40 21 3B FA F3 4A A9
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          2C 85 14 8C     FB B8 77 28     04 40 21 3B     FA F3 4A A9
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          96 70 F8 24     67 30 ED 30     98 AF 2A EC     3F A1 23 32
          37 B4 03 7D     E9 A3 8D 3E     E2 46 91 04     26 CD A0 26
    Tx> 0B 00
    Tx> AF 96 70 F8 24 67 30 ED 30 98 AF 2A EC 3F A1 23 32 37 B4 03 7D E9 A3 8D 3E E2 46 91 04 26 CD A0 26
    Rx< 0B 00 00 6B 86 58 E8 EA 62 B8 FC 13 7F 05 CC 57 38 5B 4E 2F D0 A7 DF 45 02 29 96 42 2C 07 D3 E1 F6 B8 3D
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=32)
          6B 86 58 E8     EA 62 B8 FC     13 7F 05 CC     57 38 5B 4E
          2F D0 A7 DF     45 02 29 96     42 2C 07 D3     E1 F6 B8 3D
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=12)
          00 00 00 00     00 00 00 00     00 00 00 00
           App:INFO :Dumped Session Key is (Len=16)
          C7 98 D4 C6     89 4A CB 9C     7C 7E 4C 2C     3B 19 D5 D1
           App:INFO :Dumped Session Mac is (Len=16)
          7D 9A F9 AA     DB A2 BC 68     4F 98 A6 92     7D 92 C4 0C
           App:INFO :Dumped TI is (Len=4)
          17 33 24 D0
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 First Authenticate  Successful

    Tx> 0A 00
    Tx> 77 00
    Rx< 0A 00 AF CF C0 D0 8F CE C0 26 BE 40 31 0B 0E 3F C2 18 B6
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          CF C0 D0 8F     CE C0 26 BE     40 31 0B 0E     3F C2 18 B6
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          D5 47 2E 21     4A CD 7F 95     A9 0E 82 E1     34 AB 30 3E
          96 B6 4D 0A     32 B2 9C 7D     DB 83 07 7D     74 E8 11 20
    Tx> 0B 00
    Tx> AF D5 47 2E 21 4A CD 7F 95 A9 0E 82 E1 34 AB 30 3E 96 B6 4D 0A 32 B2 9C 7D DB 83 07 7D 74 E8 11 20
    Rx< 0B 00 00 49 CF 34 0C 90 9D 1A 8D FF 2C 73 48 2A 41 88 19
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=16)
          49 CF 34 0C     90 9D 1A 8D     FF 2C 73 48     2A 41 88 19
           App:INFO :Dumped Session Key is (Len=16)
          34 40 1C 99     E3 5A 4E CD     1A CF 7A 92     4D 73 F6 28
           App:INFO :Dumped Session Mac is (Len=16)
          D3 48 4D 4B     17 4E C0 20     D3 6A 24 7C     FC E3 42 DF
           App:INFO :Dumped TI is (Len=4)
          17 33 24 D0
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 Following Authenticate  Successful

           App:INFO :Authenticated with cardkey = 0 and Se0Obj ID = 2103308289
           App:INFO :Checking that the auth session with changed key is valid by trying an encrypted communication
    Tx> 0A 00
    Tx> 51 D6 09 AF C6 31 06 8A 65
    Rx< 0A 00 00 98 2B 05 82 82 E6 B4 BB E2 84 13 93 84 EF A7 68 F3 AA D6 08 8E E4 EA 93
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
    Rx< 0A 00 AF 1B 65 22 92 28 22 2E 39 FB BB AF 5F 92 02 0B 48
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          1B 65 22 92     28 22 2E 39     FB BB AF 5F     92 02 0B 48
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          05 FF AE E6     71 6B 12 CF     7A FF B1 AB     7A B0 CF F0
          14 0D A2 CD     70 74 04 09     B8 EB EF 7F     3C 3E 5E FC
    Tx> 0B 00
    Tx> AF 05 FF AE E6 71 6B 12 CF 7A FF B1 AB 7A B0 CF F0 14 0D A2 CD 70 74 04 09 B8 EB EF 7F 3C 3E 5E FC
    Rx< 0B 00 00 BE 19 7B F2 D4 16 7B 3F 6A D3 9B 8A 6A E4 61 D5 BE BE 49 8B 52 56 CA ED FA 4C 43 4D CE 68 ED 62
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=32)
          BE 19 7B F2     D4 16 7B 3F     6A D3 9B 8A     6A E4 61 D5
          BE BE 49 8B     52 56 CA ED     FA 4C 43 4D     CE 68 ED 62
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=12)
          00 00 00 00     00 00 00 00     00 00 00 00
           App:INFO :Dumped Session Key is (Len=16)
          B9 A0 DF 3F     F5 AF 09 AB     DB E5 F2 8C     95 D6 B3 02
           App:INFO :Dumped Session Mac is (Len=16)
          BC BE C7 49     26 D2 C2 0D     12 2B A9 D9     8C 86 44 76
           App:INFO :Dumped TI is (Len=4)
          3D 06 B4 0F
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 First Authenticate  Successful

    Tx> 0A 00
    Tx> 77 00
    Rx< 0A 00 AF A9 AA 26 E3 BE 9E 1F 08 05 18 F2 18 BB BC C8 8C
           App:INFO :
     CARD =====> SE050   16-byte Ek(RndB)  =
     (Len=16)
          A9 AA 26 E3     BE 9E 1F 08     05 18 F2 18     BB BC C8 8C
           App:INFO :
      CARD <====== SE050  E(Kx, RandA || RandB') =
     (Len=32)
          C6 7D DD FE     93 2C 1B 1D     AE 73 E9 55     57 21 5B 67
          EB 77 CC CB     88 BD FD C2     D1 C6 63 79     8F 56 D6 C1
    Tx> 0B 00
    Tx> AF C6 7D DD FE 93 2C 1B 1D AE 73 E9 55 57 21 5B 67 EB 77 CC CB 88 BD FD C2 D1 C6 63 79 8F 56 D6 C1
    Rx< 0B 00 00 50 1F 48 BB 26 A9 33 96 F8 F0 F6 34 63 83 FE 0A
           App:INFO :
      CARD ======> SE050  32-byte E(Kx, TI||RndA'||PDCap2||PCDcap2) =
     (Len=16)
          50 1F 48 BB     26 A9 33 96     F8 F0 F6 34     63 83 FE 0A
           App:INFO :Dumped Session Key is (Len=16)
          08 C2 12 46     4F 51 3C A9     15 36 9F 4A     61 97 BA C2
           App:INFO :Dumped Session Mac is (Len=16)
          91 1E E8 D8     88 58 E4 0A     BB F6 93 B5     FA B5 C9 0B
           App:INFO :Dumped TI is (Len=4)
          3D 06 B4 0F
           App:INFO :pDataParams->wCmdCtr=0
           App:INFO : EV2 Following Authenticate  Successful

           App:INFO :Authenticated with cardkey = 0 and Se0Obj ID = 2103308289
           App:INFO :attempting to change cardkey = 0 from  Old Se050ObjID= 2103308289 to new Se050ObjID= 2103308288
    Tx> 0A 00
    Tx> C6 00 00 42 A7 99 85 84 E2 CA 46 3F D0 E5 12 59 62 F3 17 5E D6 C4 16 E5 09 49 8B 0D AD AD 73 75 20 A1 E7 43 06 EC 4D BC 1A 07 D1
    Rx< 0A 00 00
           App:INFO : Change Key for card key 0 is Successful to Se050ObjID= 2103308288

           App:INFO :Reverting Successful
           App:INFO : Auth session is reset in software
           App:INFO : Auth session is killed in SE
           App:INFO :ex_sss Finished


