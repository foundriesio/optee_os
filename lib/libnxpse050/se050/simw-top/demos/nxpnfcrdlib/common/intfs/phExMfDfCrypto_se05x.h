/**
 * @file phExMfDfCrypto_se05x.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * Header file for nfc desfire EV2 crypto implementation using SE05x
 */


#ifndef PHEXMFDFCRYPTO_SE05X_H
#define PHEXMFDFCRYPTO_SE05X_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Status.h"
#include <fsl_sss_se05x_types.h>
#include <phalMfdfEv2.h>


/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/*!
* MIFARE DESFire related layer initialization.
* @param   pAlMfdfEv2 mifare desfire ev2 params
* @param   pSession sss session pointer
* @param   objId se050 objects that has to be used for card authentication
* @param   wOption options as per rdlib
* @param   bKeyNoCard The card key number which has to be used for authentication
* @return  Status of the operation.
*/

extern phStatus_t phEx_Auth_Se05xEV2(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2, sss_se05x_session_t * pSession, uint32_t objId, uint16_t wOption, uint8_t bKeyNoCard);

/*!
* MIFARE DESFire related layer initialization.
* @param   pAlMfdfEv2 mifare desfire ev2 params
* @param   pSession sss session pointer
* @param   se05x_Auth_keyid se050 objects that has to be used for card authentication
* @param   se05x_Change_Currentkeyid current se050 object that has to be changed
* @param   se05x_Change_derivedkeyid se050 object that will  be changed to
* @param   wOption options as per rdlib
* @param   bKeyNoCardAuth The card key number which has to be used for authentication
* @param   bKeyNoCardAuth The card key number that has to be changed
* @param   bKeyNoCardAuth The card key version to be updated to
* @param   pDivInput Diversification Input
* @param   bDivLen Diversification Input len);
* @return  Status of the operation.
*/
extern phStatus_t phEx_ChangeKey_Se05xEV2(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2,
    sss_se05x_session_t * pSession,
    uint32_t se05x_Auth_keyid,
    uint32_t se05x_Change_Currentkeyid,
    uint32_t se05x_Change_derivedkeyid,
    uint16_t wOption,
    uint8_t bKeyNoCardAuth,
    uint8_t bKeyNoCardChange,
    uint8_t bKeyVerCardChange,
    uint8_t * pDivInput,
    uint8_t bDivLen);

/*!
* MIFARE DESFire related layer initialization.
* @param   pAlMfdfEv2 mifare desfire ev2 params
* @param   pSession sss session pointer
* @param   bCard The card key number which has been used for authentication
* @return  Status of the operation.
*/
phStatus_t phEx_KillAuth_Se05xEV2(sss_se05x_session_t * pSession, phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2, uint8_t bCard);



#endif /* PHEXMFDFCRYPTO_SE05X_H */
