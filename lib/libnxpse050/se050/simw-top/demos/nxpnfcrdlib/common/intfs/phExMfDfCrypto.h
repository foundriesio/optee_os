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
 * phExMfCrypto_MfDfCrypto.h : Contains the Api signatures and other declarations for MIFARE DESFire Crypto and
 *                             authentication operations.
 *
* $Author: Kiran Kumar Makam Nataraja (nxp69453) $
* $Revision: 5404 $
* $Date: 2016-07-04 16:00:39 +0530 (Mon, 04 Jul 2016) $
 */

#ifndef PHEXMFDFCRYPTO_H
#define PHEXMFDFCRYPTO_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Status.h"
#include <phKeyStore.h>
#include <phalMfdfEv2.h>

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

#define PH_EXMFCRYPTO_MFDFCRYPTO_MIFAREDESFIRE_SAK 0x20

#define IID_KEY_COUNT 0x13U /* number of IID keys */

#define AMKEY 0      /* Application Master Key number. */
#define ACKEY 1      /* Application Common Key number. */
#define ARKEY 2      /* Application Read Key number. */
#define AWKEY 3      /* Application Write Key number. */
#define AAKEY 4      /* Application Accreditation Key number. */
#define PICCKEY 5    /* PICC Key entry number in key store. */
#define PICCAESKEY 6 /* New PICC (AES128) Key entry number in key store. */
#define AMKEY_NEW 7  /* New Application Master Key entry number in Key store. */
#define SYSTEM_KEY 8 /* System key entry number in key store. */
#define NPKEY0 9     /* Next PACS App Key 0. */
#define NPKEY1 10    /* Next PACS App Key 1. */
#define SYSTEM_KEY_NP 11      /* Next PACS System key. */
#define DEFAULT_3K3DES 12     /* Default 3K3DES KEY. All zeros. */
#define NPKEY_FOR_UID 13      /* Used to retrieve UID */
#define AFCKEY_FOR_UID 14     /* Used to retrieve UID */
#define PICCDAMAUTHKEY 15     /* PICC DAM Auth Key for DAM */
#define PICCORIGINALITYKEY 16 /* PICC Originality Key */
#define VCCONFIGKEY 17        /* PICC VC Configuration Key */
#define VCPROXIMITYKEY 18     /* PICC VC Proximity Key */
#define VCSELECTMACKEY 19     /* PICC VC Select MAC Key */
#define VCSELECTENCKEY 20     /* PICC VC Select ENC Key */

#define STDDATAFILE1 1   /* File number of Standard data file 1. */
#define STDDATAFILE2 2   /* File number of Standard data file 2. */
#define BCKUPDATAFILE1 6 /* File number od Backup data file.*/
#define BCKUPDATAFILE2 7 /* File number of backup datafile. */
#define VALUEFILE 3      /* File number of Value file. */
#define RECORDFILE 4     /* File number of Record file. */
#define TMACFILE 5       /* File number of Record file. */

#define NPCARDOBJ 1         /* File number of NP Card object. */
#define NPPACSOBJ 2         /* File number of NP PACS object. */
#define TMI_BUFFER_SIZE 255 /* TMI Buffer Size */
#define IID_KEY_COUNT 0x13U /* number of IID keys */
#define VCA_AUTH_RND_LEN \
    36 /* Size of the Virtual Card and Proximity Check Random numbers */

#define KEYCOUNT 21
#define KEYVERSIONS 1

/* define Key Store constants */
#define PH_EXMFCRYPTO_MIFAREDESFIRE_NUMBER_OF_KEYENTRIES KEYCOUNT
#define PH_EXMFCRYPTO_MIFAREDESFIRE_NUMBER_OF_KEYVERSIONPAIRS KEYVERSIONS
#define PH_EXMFCRYPTO_MIFAREDESFIRE_NUMBER_OF_KUCENTRIES KEYCOUNT

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

extern phKeyStore_Sw_KeyEntry_t
    pKeyEntries[PH_EXMFCRYPTO_MIFAREDESFIRE_NUMBER_OF_KEYENTRIES];
extern phKeyStore_Sw_KeyVersionPair_t
    pKeyVersionPairs[PH_EXMFCRYPTO_MIFAREDESFIRE_NUMBER_OF_KEYVERSIONPAIRS *
                     PH_EXMFCRYPTO_MIFAREDESFIRE_NUMBER_OF_KEYENTRIES];
extern phKeyStore_Sw_KUCEntry_t
    pKUCEntries[PH_EXMFCRYPTO_MIFAREDESFIRE_NUMBER_OF_KUCENTRIES];

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
* MIFARE DESFire related layer initialization.
*
* @param pNxpRdLib : Example data params.
* @return Status of the operation.
*/
extern phStatus_t CheckOutAfcApp(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2);

extern phStatus_t phEx_Personalize_AFCApp(
    phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2);
extern phStatus_t phEx_Auth_AFCApp_EV2(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2);
extern phStatus_t phEx_Create_ValueFile(
    phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2);
extern phStatus_t phEx_Use_ValueFile(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2);
extern phStatus_t Accreditation(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2);

#endif /* PHEXMFDFCRYPTO_H */
