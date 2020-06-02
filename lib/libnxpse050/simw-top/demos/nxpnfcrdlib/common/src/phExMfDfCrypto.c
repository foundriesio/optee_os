/*
*                    Copyright (c), NXP Semiconductors
*
*                       (C) NXP Semiconductors 2014,2015
*
*         All rights are reserved. Reproduction in whole or in part is
*        prohibited without the written consent of the copyright owner.
*    NXP reserves the right to make changes without notice at any time.
*   NXP makes no warranty, expressed, implied or statutory, including but
*   not limited to any implied warranty of merchantability or fitness for any
*  particular purpose, or that the use will not infringe any third party patent,
*   copyright or trademark. NXP must not be liable for any loss or damage
*                            arising from its use.
*/

/** @file
*
* phExMfCrypto_MfDfCrypto.c : Implements the core logic demonstrating MIFARE DESFire features.
*
* $Author: Purnank G (ing05193) $
* $Revision: 5445 $
* $Date: 2016-07-06 12:02:22 +0530 (Wed, 06 Jul 2016) $
*/

/**
* Reader Library Headers
*/
#include <stdio.h>
#include "np_NxpNfcRdLib.h"
#include "phNfcLib.h"
#include <phApp_Init.h>
#include <phExMfDfCrypto.h>
#include <phTMIUtils.h>
#include <phalMfdfEv2.h>
#include <phalVca.h>
#include "nxLog_App.h"

//
///* Local headers */
//#include "NxpNfcRdLib_Ver.h"

//#ifdef NXPBUILD_EX11_MANDATORY_LAYERS

///* *****************************************************************************************************************
//* Internal Definitions
//* ***************************************************************************************************************** */

/*
* Application master key is changeable and same KeyNo is necessary to change a key and changeable if
* authenticated with the application master key.
* CreateFile/DeleteFile is permitted without application master key authentication.
* Authentication is required for executing the GetFileIDs GetFileSettings and GetKeySettings commands.
*/
#define PH_EXMFCRYPTO_MIFAREDESFIRE_KEYSETTINGS1 0xED

/*
* Application is authenticated using AES with no File Identifiers for files within the application and
* upto 4 Keys can be stored in application.
*/
#define PH_EXMFCRYPTO_MIFAREDESFIRE_KEYSETTINGS2 0x84
#define TMI_BUFFER_SIZE 255 /* TMI Buffer Size */

/* *****************************************************************************************************************
* Type Definitions
* ***************************************************************************************************************** */

phStatus_t StoreKeysInKeyStore(phKeyStore_Sw_DataParams_t *pKeyStore);
phStatus_t CreateStandardFile(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2);

/* *****************************************************************************************************************
* Global and Static Variables
* Total Size: NNNbytes
* ***************************************************************************************************************** */
phCryptoSym_Sw_DataParams_t sCryptoSymRnd;
phCryptoSym_Sw_DataParams_t cryptoMac;


///* data parameter storage */
phKeyStore_Sw_KeyEntry_t
    pKeyEntries[PH_EXMFCRYPTO_MIFAREDESFIRE_NUMBER_OF_KEYENTRIES];
phKeyStore_Sw_KeyVersionPair_t
    pKeyVersionPairs[PH_EXMFCRYPTO_MIFAREDESFIRE_NUMBER_OF_KEYVERSIONPAIRS *
                     PH_EXMFCRYPTO_MIFAREDESFIRE_NUMBER_OF_KEYENTRIES];
phKeyStore_Sw_KUCEntry_t
    pKUCEntries[PH_EXMFCRYPTO_MIFAREDESFIRE_NUMBER_OF_KUCENTRIES];

//
///**
//* Virtual Card
//* TODO: do it the new way using pointers
//*/
static uint8_t gbGenBuffer[100];

/* clang-format off */
/* AES PICC key */
static uint8_t bAESPICCKey[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/* Application Master key of AFC App */
static uint8_t bAMKey[16] =     {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/* Application Master key of AFC App */
static uint8_t bACKey[16] =     {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static uint8_t bPICCKEY[16] =     {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static uint8_t bPICCORIGNINALITYKEY[16] =     {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/* PICC VC Configuration Key */
static uint8_t bVCCONFIGKEY[16] =     {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/* PICC VC Proximity Key */
static uint8_t bVCPROXIMITYKEY[16] =     {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/* PICC VC Select MAC Key */
static uint8_t bVCSELECTMACKEY[16] =     {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/* PICC VC Select ENC Key */
static uint8_t bVCSELECTENCKEY[16] =     {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/* PICC DAM Auth Key for DAM */
static uint8_t  bPICCDAMAUTHKEY[16]= {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/* Application Read Key of AFC App */
static uint8_t bARKey[16] =     {
    0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
    0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22
};
/* Application Write Key of AFC App */
static uint8_t bAWKey[16] =     {
    0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
    0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33
};
/* Application Accreditation Key of AFC App */
static uint8_t bAAKey[16] =     {
    0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
    0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44
};
/* New Application Master key of AFC App */
static uint8_t bAMKey_New[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/* AFC App Key to retrieve UID */
static uint8_t bAfcKeyN[24]  =   {
    0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F, 0x70, 0x81,
    0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
    0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98
};

/* System key of AFC App */
static uint8_t bSysKey[16] =    {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16
};
/* Next PACs System key */
static uint8_t bDEFAULT_3K3DES[24] =  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/* Default 3K 3DES Key */
static uint8_t bSysKeyNP[24] =  {
    0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xCA, 0xCB,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16
};
/* NextPac key 0 */
static uint8_t bNPKey0[24]  =   {
    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11,
    0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99,
    0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80
};
/* NextPac key 1 */
static uint8_t bNPKey1[24]  =   {
    0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF, 0xE0, 0xE1,
    0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9,
    0xC0, 0xD0, 0xE0, 0xF0, 0x20, 0x30, 0x40, 0x50
};

/* NP Key to retrieve UID */
static uint8_t bNPKey2[24]  =   {
    0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xC0, 0xA1,
    0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9,
    0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80
};

/* clang-format on */

//
///* MASTER APPLICATION ID */
//static uint8_t bAfcMApp[3] = {0x00, 0x00, 0x00};
/* AFC APPLICATION id */

/* doc:start:mif-afc-app */
uint8_t bAfcApp[3] = {0x11, 0x22, 0x33};
/* doc:end:mif-afc-app */
//static uint8_t bAfcApp[3] = { 0x33, 0x44, 0x55};//{0x2A, 0x8C, 0xF1};
///* AFC APPLICATION2 id */
//static uint8_t bAfcApp2[3] = {0x02, 0x02, 0x02};
//
///* Delegated AFC APPLICATION id */
//static uint8_t bAfcDelApp[3] = {0x01, 0x01, 0x01};
//
static uint8_t bCardUid[10];
//
//static uint8_t bVersionString[28];

/* TMC & TMV */
uint8_t bTMC[4] = {0x00, 0x00, 0x00, 0x00};
uint8_t bTMV[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//
///* *****************************************************************************************************************
//* Public Functions
//* ***************************************************************************************************************** */

phStatus_t StoreKeysInKeyStore(phKeyStore_Sw_DataParams_t *pKeyStore)
{
    phStatus_t status;
    uint8_t bDefKey[24];

    /* Set the default PICC Master key in the key store */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, PICCKEY, PH_CRYPTOSYM_KEY_TYPE_2K3DES);
    CHECK_SUCCESS(status);

    status = phKeyStore_SetKeyAtPos(
        pKeyStore, PICCKEY, 0x00, PH_CRYPTOSYM_KEY_TYPE_2K3DES, bPICCKEY, 0x00);
    CHECK_SUCCESS(status);

    /* Set the Application Master key in the key store */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, AMKEY, PH_CRYPTOSYM_KEY_TYPE_2K3DES);
    CHECK_SUCCESS(status);

    status = phKeyStore_SetKeyAtPos(
        pKeyStore, AMKEY, 0x00, PH_CRYPTOSYM_KEY_TYPE_2K3DES, bAMKey, 0x00);
    CHECK_SUCCESS(status);

    /* PICC DAM Auth Key for DAM */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, PICCDAMAUTHKEY, PH_CRYPTOSYM_KEY_TYPE_DES);
    CHECK_SUCCESS(status);

    status = phKeyStore_SetKeyAtPos(pKeyStore,
        PICCDAMAUTHKEY,
        0x00,
        PH_CRYPTOSYM_KEY_TYPE_DES,
        bPICCDAMAUTHKEY,
        0x00);
    CHECK_SUCCESS(status);

    /* Set the Application Common key in the key store */
    status =
        phKeyStore_FormatKeyEntry(pKeyStore, ACKEY, PH_CRYPTOSYM_KEY_TYPE_DES);
    CHECK_SUCCESS(status);

    status = phKeyStore_SetKeyAtPos(
        pKeyStore, ACKEY, 0x00, PH_CRYPTOSYM_KEY_TYPE_DES, bACKey, 0x00);
    CHECK_SUCCESS(status);

    /* Set the Application Read key in the key store */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, ARKEY, PH_CRYPTOSYM_KEY_TYPE_AES128);
    CHECK_SUCCESS(status);

    status = phKeyStore_SetKeyAtPos(
        pKeyStore, ARKEY, 0x00, PH_CRYPTOSYM_KEY_TYPE_AES128, bARKey, 0x00);
    CHECK_SUCCESS(status);

    /* Set the Application Write key in the key store */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, AWKEY, PH_CRYPTOSYM_KEY_TYPE_AES128);
    CHECK_SUCCESS(status);

    status = phKeyStore_SetKeyAtPos(
        pKeyStore, AWKEY, 0x00, PH_CRYPTOSYM_KEY_TYPE_AES128, bAWKey, 0x00);
    CHECK_SUCCESS(status);

    /* Set the Application Accreditation key in the key store */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, AAKEY, PH_CRYPTOSYM_KEY_TYPE_AES128);
    CHECK_SUCCESS(status);

    status = phKeyStore_SetKeyAtPos(
        pKeyStore, AAKEY, 0x00, PH_CRYPTOSYM_KEY_TYPE_AES128, bAAKey, 0x00);
    CHECK_SUCCESS(status);

    /* Set the new PICC AES128 key in the key store */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, PICCAESKEY, PH_CRYPTOSYM_KEY_TYPE_AES128);
    CHECK_SUCCESS(status);

    status = phKeyStore_SetKeyAtPos(pKeyStore,
        PICCAESKEY,
        0x00,
        PH_CRYPTOSYM_KEY_TYPE_AES128,
        bAESPICCKey,
        0x00);
    CHECK_SUCCESS(status);

    /* Set the new PICC ORIGINALITY CHECK AES128 key in the key store. */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, PICCORIGINALITYKEY, PH_CRYPTOSYM_KEY_TYPE_AES128);
    CHECK_SUCCESS(status);
    status = phKeyStore_SetKeyAtPos(pKeyStore,
        PICCORIGINALITYKEY,
        0x00,
        PH_CRYPTOSYM_KEY_TYPE_AES128,
        bPICCORIGNINALITYKEY,
        0x00);
    CHECK_SUCCESS(status);
    /* Set the new PICC VC Configuration Key AES128 key in the key store. */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, VCCONFIGKEY, PH_CRYPTOSYM_KEY_TYPE_AES128);
    CHECK_SUCCESS(status);
    status = phKeyStore_SetKeyAtPos(pKeyStore,
        VCCONFIGKEY,
        0x00,
        PH_CRYPTOSYM_KEY_TYPE_AES128,
        bVCCONFIGKEY,
        0x00);
    CHECK_SUCCESS(status);

    /* Set the new PICC VC PROXIMITY KEY AES128 key in the key store. */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, VCPROXIMITYKEY, PH_CRYPTOSYM_KEY_TYPE_AES128);
    CHECK_SUCCESS(status);
    status = phKeyStore_SetKeyAtPos(pKeyStore,
        VCPROXIMITYKEY,
        0x00,
        PH_CRYPTOSYM_KEY_TYPE_AES128,
        bVCPROXIMITYKEY,
        0x00);
    CHECK_SUCCESS(status);

    /* Set the new PICC VC Select MAC KEY AES128 key in the key store. */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, VCSELECTMACKEY, PH_CRYPTOSYM_KEY_TYPE_AES128);
    CHECK_SUCCESS(status);
    status = phKeyStore_SetKeyAtPos(pKeyStore,
        VCSELECTMACKEY,
        0x00,
        PH_CRYPTOSYM_KEY_TYPE_AES128,
        bVCSELECTMACKEY,
        0x00);
    CHECK_SUCCESS(status);
    /* Set the new PICC VC Select ENC KEY AES128 key in the key store. */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, VCSELECTENCKEY, PH_CRYPTOSYM_KEY_TYPE_AES128);
    CHECK_SUCCESS(status);
    status = phKeyStore_SetKeyAtPos(pKeyStore,
        VCSELECTENCKEY,
        0x00,
        PH_CRYPTOSYM_KEY_TYPE_AES128,
        bVCSELECTENCKEY,
        0x00);
    CHECK_SUCCESS(status);
    /* Set the new Application Master key in the key store */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, AMKEY_NEW, PH_CRYPTOSYM_KEY_TYPE_AES128);
    CHECK_SUCCESS(status);

    status = phKeyStore_SetKeyAtPos(pKeyStore,
        AMKEY_NEW,
        0x00,
        PH_CRYPTOSYM_KEY_TYPE_AES128,
        bAMKey_New,
        0x00);
    CHECK_SUCCESS(status);

    /* Set the SYSTEM key in the key store */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, SYSTEM_KEY, PH_CRYPTOSYM_KEY_TYPE_AES128);
    CHECK_SUCCESS(status);

    status = phKeyStore_SetKeyAtPos(pKeyStore,
        SYSTEM_KEY,
        0x00,
        PH_CRYPTOSYM_KEY_TYPE_AES128,
        bSysKey,
        0x00);
    CHECK_SUCCESS(status);

    /* Set the UID retriever key in the key store */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, AFCKEY_FOR_UID, PH_CRYPTOSYM_KEY_TYPE_AES128);
    CHECK_SUCCESS(status);

    status = phKeyStore_SetKeyAtPos(pKeyStore,
        AFCKEY_FOR_UID,
        0x00,
        PH_CRYPTOSYM_KEY_TYPE_AES128,
        bAfcKeyN,
        0x00);
    CHECK_SUCCESS(status);

    /* Set the NPKEY0 in the key store */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, NPKEY0, PH_CRYPTOSYM_KEY_TYPE_3K3DES);
    CHECK_SUCCESS(status);

    status = phKeyStore_SetKeyAtPos(
        pKeyStore, NPKEY0, 0x00, PH_CRYPTOSYM_KEY_TYPE_3K3DES, bNPKey0, 0x00);
    CHECK_SUCCESS(status);

    /* Set the NPKEY1 in the key store */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, NPKEY1, PH_CRYPTOSYM_KEY_TYPE_3K3DES);
    CHECK_SUCCESS(status);

    status = phKeyStore_SetKeyAtPos(
        pKeyStore, NPKEY0, 0x00, PH_CRYPTOSYM_KEY_TYPE_3K3DES, bNPKey1, 0x00);
    CHECK_SUCCESS(status);

    /* Set the NP system KEY in the key store */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, SYSTEM_KEY_NP, PH_CRYPTOSYM_KEY_TYPE_3K3DES);
    CHECK_SUCCESS(status);

    status = phKeyStore_SetKeyAtPos(pKeyStore,
        SYSTEM_KEY_NP,
        0x00,
        PH_CRYPTOSYM_KEY_TYPE_3K3DES,
        bSysKeyNP,
        0x00);
    CHECK_SUCCESS(status);

    memset(bDefKey, 0, 24);
    /* Set the default 3K3DES KEY in the key store */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, DEFAULT_3K3DES, PH_CRYPTOSYM_KEY_TYPE_3K3DES);
    CHECK_SUCCESS(status);

    status = phKeyStore_SetKeyAtPos(pKeyStore,
        DEFAULT_3K3DES,
        0x00,
        PH_CRYPTOSYM_KEY_TYPE_3K3DES,
        bDEFAULT_3K3DES,
        0x00);
    CHECK_SUCCESS(status);

    /* Set the UID retriever KEY for NP App in the key store */
    status = phKeyStore_FormatKeyEntry(
        pKeyStore, NPKEY_FOR_UID, PH_CRYPTOSYM_KEY_TYPE_3K3DES);
    CHECK_SUCCESS(status);

    status = phKeyStore_SetKeyAtPos(pKeyStore,
        NPKEY_FOR_UID,
        0x00,
        PH_CRYPTOSYM_KEY_TYPE_3K3DES,
        bNPKey2,
        0x00);
    CHECK_SUCCESS(status);

    return 0;
}

///* Creates and pre personalizes the AFC Application */
phStatus_t phEx_Personalize_AFCApp(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2)
{

    phStatus_t status;
    uint8_t bLength;
    uint8_t bOption;
    uint8_t bKeySettings1;
    uint8_t bKeySettings2;
    uint8_t bKeySettings3;
    uint8_t bKeySetValues[4];
    uint8_t bISOFileId[2];
    uint8_t pISODFName[16];
    uint8_t bDFLen;
    uint8_t bConfigByte;
    uint8_t bIndex = 0;
    LOG_I("Performing Pre Personalization ...... \n");
    /* ####################Pre personalization############################### */

    /* Authenticate with the PICC Master key */
    status = phalMfdfEv2_Authenticate(pAlMfdfEv2,   /* [In] Pointer to parameters data structure */
      PHAL_MFDFEV2_NO_DIVERSIFICATION,            /* [In] Diversification option */
      PICCKEY,                                    /* [In]KEY number in key store */
      0x00,                                       /* [In] Key version in key store */
      0x00,                                       /* [In] Key number on Card */
      NULL,                                       /* [In] Diversification input */
      0);                                         /* [In] Diversification input length */
    CHECK_SUCCESS(status);

    /* Get a fresh card */
    status = phalMfdfEv2_Format(pAlMfdfEv2);
    CHECK_SUCCESS(status);
      LOG_I("Formating the card Successful\n");

    /* Set Configuration - default key and version */
    bOption = PHAL_MFDFEV2_SET_CONFIG_OPTION1; /* option to set default key and version */
    memset(gbGenBuffer, 0x00, 24);             /* Default key */
    gbGenBuffer[24] = 0x00;                    /* default key version */
    bLength = 25;                              /* length */
    status = phalMfdfEv2_SetConfiguration(pAlMfdfEv2,
      bOption,
      gbGenBuffer,
      bLength);
    CHECK_SUCCESS(status);

    /* Set Configuration - Configure for Random UID */
    bOption = PHAL_MFDFEV2_SET_CONFIG_OPTION0;          /* option to set configuration byte */
    bConfigByte = 0x02;                                 /* 00000010 : Random ID Enabled */
    status = phalMfdfEv2_SetConfiguration(pAlMfdfEv2,
      bOption,
      &bConfigByte,
      0x01);
    CHECK_SUCCESS(status);
    /* Get the card UID - wk423, As of now it is not working with Csharp Simulator, rev 5311. */
    /* Please uncomment later once it is working*/
    status = phalMfdfEv2_GetCardUID(pAlMfdfEv2,      /* [In] Pointer to parameters data structure */
      bCardUid);                          /* [Out] 7 Byte Card UID */
    CHECK_SUCCESS(status);

      for(bIndex = 0; bIndex < sizeof(bCardUid); bIndex++)
      {
        LOG_I("bCardUid[%d] = 0x%x\n", bIndex, bCardUid[bIndex]);
      }

      /* Create AFC Application */
      /* bOption = 0x03 indicates that application has both ISO Fid and DF Name */
      bOption = 0x03;
      /* Iso File Id = 0x00AA. Sent LSB first */
      bISOFileId[0] = 0xAA;
      bISOFileId[1] = 0x00;
      /* DF Name = AFCAPPLICATION */
      bDFLen = 14;
      memcpy(pISODFName, "AFCAPPLICATION", bDFLen);
      /* Application masterkey settings 00001011
      * =  conf. changeable +
      *    create/delete file requires authentication +
      *   directory access is free +
      *   App MK is changeable
      *   Change access rights = 0x00 (AMK)
      */
      bKeySettings1 = 0x0F;
      /* Keysettings 2 = 10100110
      * AES crypto + supports 2 byte ISO fids +
      * max 6 keys in this app
      */
      bKeySettings2 = 0xB6;
      /* Keysetting 3 = 0000 0001. b[0] indicates App key set presence */
      bKeySettings3 = 0x01;
      /* Key Set Values */
      bKeySetValues[0] = 0x01;         /* byte0 = AKS ver */
      bKeySetValues[1] = 0x02;         /* byte1 = #keysets */
      bKeySetValues[2] = 0x10;         /* byte2 = MaxKeysize */
      bKeySetValues[3] = 0x00;         /* byte3 = AppKeySetSettings */
      /* Create AFC Application */
      status = phalMfdfEv2_CreateApplication( pAlMfdfEv2,    /* [In] Pointer to parameters data structure */
        bOption,                                           /* [In] Option to indicate whether this application has ISO Fid and DF */
        bAfcApp,                                           /* [In] 3 byte AID, LSB First */
        bKeySettings1,                                     /* [In] Key Settings 1 - 1 Byte */
        bKeySettings2,                                     /* [In] Key Settings 2 - 1 Byte */
        bKeySettings3,                                     /* [In] Key Settings 3 - 1 Byte */
        &bKeySetValues[0],                                 /* [In] 4 byte keyset values.  */
        bISOFileId,                                        /* [In] Two byte ISO File Id, LSB First */
        pISODFName,                                        /* [In] ISO DF Name. Maximum 16 bytes */
        bDFLen);                                           /* [In] Length of DF Name provided above */
      CHECK_SUCCESS(status);

    LOG_I("Create AFC Application Successful\n");

    /* Select the AFC Application */
    status = phalMfdfEv2_SelectApplication(pAlMfdfEv2,
        0x00, /* [In] bOption-0x00 for single and 0x01 for second application */
        bAfcApp, /* [In] 3 byte AID, LSB First */
        0x00); /* [In] Second AID...NA in this case; we do not need this as bOption is 0x00 */
    CHECK_SUCCESS(status);
    LOG_I("Select the AFC Application Successful\n");
    return status;

}


phStatus_t phEx_Create_ValueFile(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2)
{
    phStatus_t status;
    uint8_t bTMKeyVer;
    uint8_t bAccessRights[2];
    uint8_t bLimitedCredit = 0;
    uint8_t bLLimit[4];
    uint8_t bULimit[4];
    uint8_t bValue[4];
    /* clang-format off */
    /* doc:start:card-aes-key */
    uint8_t bTMKey[16] = {  0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00 };
    /* doc:end:card-aes-key */
    /* clang-format on */

    LOG_I("phEx_Create_ValueFile...\n");

    bTMKeyVer = 0x00;
    /* accessRights For Transaction MAC File*/
    bAccessRights[0] = 0xF0; /* RW access = AAK, Change access rights = AMK  */
    bAccessRights[1] = 0x0F; /* Read access = AWKey, Write access = AWKey */

    /* create Transaction MAC File*/
    status = phalMfdfEv2_CreateTransactionMacFile(
        pAlMfdfEv2, /* [In] Pointer to parameters data structure */
        0,
        TMACFILE,                       /* [In] File number */
        PHAL_MFDFEV2_COMMUNICATION_ENC, /* [In] Communication settings of the file */
        bAccessRights,                /* [In] 2 Byte access rights. LSB First */
        PICCAESKEY,                   /* [In] Key no of the TMKey to be used. */
        PHAL_MFDFEV2_KEY_TYPE_AES128, /* [In] 1byte. Bit0, 1 indicate key type.. */
        bTMKey,                       /* [In] Key value. 16 bytes. */
        bTMKeyVer,
        NULL,
        0); /* [In] TM Key version. Value should be valid for AES key. Ignored for other key types. */
    CHECK_SUCCESS(status);

    LOG_I(" create Transaction MAC File  Successful\n");
    /* Create Value File */
    bAccessRights[0] = 0x00; /* RW access = AAK, Change access rights = AMK  */
    bAccessRights[1] = 0x00; /* Read access = AWKey, Write access = AWKey */

    /* Create Value File */
    memset(bLLimit, 0x00, 4);
    memset(bULimit, 0x00, 4);
    memset(bValue, 0x00, 4);

    bLLimit[0] = 0x14; /*Lower Limit  20. */
    bValue[0] = 0x14;  /*Initial value 20. */
    bULimit[0] = 0xF4;
    bULimit[1] = 0x01; /*500 is UpperLimit. */

    /* FreeGetValue Enabled */
    bLimitedCredit = PHAL_MFDFEV2_ENABLE_FREE_GETVALUE;
    /* Create Value File */
    status = phalMfdfEv2_CreateValueFile(
        pAlMfdfEv2, /* [In] Pointer to parameters data structure */
        VALUEFILE,  /* [In] File number */
        PHAL_MFDFEV2_COMMUNICATION_ENC, /* [In] Communication settings of the file */
        bAccessRights, /* [In] 2 Byte access rights. LSB First */
        bLLimit,       /* [In] Lower Limit. 4 Byte value LSB First */
        bULimit,       /* [In] Upper Limit. 4 Byte value LSB First */
        bValue,        /* [In] 4 Byte value LSB First */
        bLimitedCredit /* [In] Limited Credit and free getvalue setting. */
    );
    CHECK_SUCCESS(status);
    LOG_I(" create Value File  Successful\n");

    status = CreateStandardFile(pAlMfdfEv2);
    CHECK_SUCCESS(status);
    /* ####################End Pre personalization########################### */
    return status;
}

phStatus_t phEx_Use_ValueFile(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2)
{
    LOG_I("phEx_Use_ValueFile...\n");
    return Accreditation(pAlMfdfEv2);
#if 0
    phStatus_t status;
    uint8_t bValue[4] = { 0 };

    /* Get Current Value */
    status = phalMfdfEv2_GetValue(
        pAlMfdfEv2, /* [In] Pointer to this layers param structure. */
        PHAL_MFDFEV2_COMMUNICATION_PLAIN, /* [In] Communication option. */
        VALUEFILE,                        /* [In] 1 byte file number. */
        bValue); /* [Out] 4 Byte array to store the value read out. LSB First. */
    LOG_MAU8_I("bValue", bValue, sizeof(bValue));
    return status;
#endif
}
//
phStatus_t CreateStandardFile(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2)
{
    phStatus_t status;
    uint8_t fileNo = 0x1F;
    uint8_t IsoFileID[2] = {0x56, 0x78};
    uint8_t fileOption = 0x00;
    uint8_t accessRights[2] = {0xE0, 0x00};
    uint8_t fileSize[3] = {0xFF, 0x00, 0x00};
    status = phalMfdfEv2_CreateStdDataFile(pAlMfdfEv2,
        0x01,
        fileNo,
        IsoFileID,
        fileOption,
        accessRights,
        fileSize);
    CHECK_SUCCESS(status);
    LOG_I("***** Creating standard data file SUCCESS!!*******\n");
    return status;
}

/* Credits a given value to the AFC Application */
phStatus_t Accreditation_WithValue(
    phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2, uint8_t *pValue)
{
    phStatus_t status;
    uint8_t bValue[4];
    uint32_t dwVal1;

    /* ########################## Accreditation ################################### */
    LOG_I("Performing Accreditation in AFC App.... \n");

#if 0
    /* Authenticate with the Write Key of Value File */
    status = phalMfdfEv2_AuthenticateAES(
        pAlMfdfEv2, /* [In] Pointer to parameters data structure */
        PHAL_MFDFEV2_NO_DIVERSIFICATION, /* [In] Diversification option */
        PICCAESKEY,                      /* [In]KEY number in key store */
        0x00,                            /* [In] Key version in key store */
        0x00,                            /* [In] Key number on Card */
        NULL,                            /* [In] Diversification input */
        0);                              /* [In] Diversification input length */
    CHECK_SUCCESS(status);
#endif
    LOG_I("Performing Accreditation in AFC App Successful \n");
    memset(bValue, 0, 4);
    /* Get Current Value */
    LOG_I("(Plain Communicatioon)Trying to Get the Current Value. Plain Communicatioon \n");
    status = phalMfdfEv2_GetValue(
        pAlMfdfEv2, /* [In] Pointer to this layers param structure. */
        PHAL_MFDFEV2_COMMUNICATION_PLAIN, /* [In] Communication option. */
        VALUEFILE,                        /* [In] 1 byte file number. */
        bValue); /* [Out] 4 Byte array to store the value read out. LSB First. */
    CHECK_SUCCESS(status);
    LOG_I("Getting current value Successful \n");
    dwVal1 = bValue[3];
    dwVal1 = dwVal1 << 8 | bValue[2];
    dwVal1 = dwVal1 << 8 | bValue[1];
    dwVal1 = dwVal1 << 8 | bValue[0];

    /* Credit the value provided */
    memcpy(bValue, pValue, 4);

    /* Add money (100) to the account */
    LOG_I("(Enc Communication using session Key)Trying to Add money to the account\n");
    status = phalMfdfEv2_Credit(
        pAlMfdfEv2, /* [In] Pointer to this layers param structure. */
        PHAL_MFDFEV2_COMMUNICATION_ENC, /* [In] Communication option. */
        VALUEFILE,                      /* [In] 1 byte file number. */
        bValue); /* [In] 4 byte value array. LSB first. */
    CHECK_SUCCESS(status);

    LOG_I("Add money to the account successful\n");
    /* Commit after credit*/
    status = phalMfdfEv2_CommitTransaction(
        pAlMfdfEv2, /* [In] Pointer to this layers param structure. */
        0x01,       /* [In] Set to 1 to get TMC & TMV */
        bTMC,       /* [Out] 4 byte TMAC counter. */
        bTMV);      /* [Out] 8 byte TMAC value. */
    CHECK_SUCCESS(status);

    /* Performing Get Value After Credit */
    status = phalMfdfEv2_GetValue(
        pAlMfdfEv2, /* [In] Pointer to this layers param structure. */
        PHAL_MFDFEV2_COMMUNICATION_PLAIN, /* [In] Communication option. */
        VALUEFILE,                        /* [In] 1 byte file number. */
        bValue); /* [Out] 4 Byte array to store the value read out. LSB First. */
    CHECK_SUCCESS(status);

    LOG_I(" The amount in your account  After credit is %x %x %x %x \n",
        bValue[3],
        bValue[2],
        bValue[1],
        bValue[0]);
    /* ########################## End Accreditation ############################### */
    LOG_I("\tAccreditation DONE! \n");
    return 0;
}

/* Credits 100 units to the AFC Application */
phStatus_t Accreditation(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2)
{
    uint8_t bValue[4] = {3, 0, 0, 0};
    return Accreditation_WithValue(pAlMfdfEv2, bValue);
}

/* Checkout of AFC App. Debits 10 units from the AFC Application */
phStatus_t CheckOutAfcApp(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2)
{
    phStatus_t status;
    uint8_t bValue[4] = {0, 0, 0, 0};
    uint32_t dwVal1;

    dwVal1 = bValue[3];
    dwVal1 = dwVal1 << 8 | bValue[2];
    dwVal1 = dwVal1 << 8 | bValue[1];
    dwVal1 = dwVal1 << 8 | bValue[0];

    /* Prepare value to be debited */
    memset(bValue, 0, 4);
    bValue[0] = 10;
    /* Debit 10 units */
    status = phalMfdfEv2_Debit(
        pAlMfdfEv2, /* [In] Pointer to this layers param structure. */
        PHAL_MFDFEV2_COMMUNICATION_ENC, /* [In] Communication option. */
        VALUEFILE,                      /* [In] 1 byte file number. */
        bValue); /* [In] 4 byte value array. LSB first. */
    CHECK_SUCCESS(status);
    LOG_I("Debit 10 units succesful \n");
    /* Commit after  Debiting the value */
    status = phalMfdfEv2_CommitTransaction(
        pAlMfdfEv2, /* [In] Pointer to this layers param structure. */
        0x01,       /* [In] Lower nibble : Return Calculated TMAC or not.*/
        bTMC,       /* [Out] 4 byte TMAC counter. */
        bTMV);      /* [Out] 8 byte TMAC value. */
    CHECK_SUCCESS(status);
    LOG_I("Commit after  Debiting the value succesful \n");
    /* GetValue After Debiting The Value */
    status = phalMfdfEv2_GetValue(
        pAlMfdfEv2, /* [In] Pointer to this layers param structure. */
        PHAL_MFDFEV2_COMMUNICATION_PLAIN, /* [In] Communication option. */
        VALUEFILE,                        /* [In] 1 byte file number. */
        bValue); /* [Out] 4 Byte array to store the value read out. LSB First. */
    CHECK_SUCCESS(status);

    LOG_I(" The amount in your account After Debit is %x %x %x %x \n",
        bValue[3],
        bValue[2],
        bValue[1],
        bValue[0]);
    LOG_I("\tCheckOutAfcApp DONE! \n");
    return 0;
}

//#endif /* NXPBUILD_EX11_MANDATORY_LAYERS */
