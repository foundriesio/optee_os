/**
 * @file se05xExHelper.h
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
 * Header file for Helper fuctions for Examples They need both nfcrdlib and se05x support
 */

#ifndef SE05XEXHELPER_H
#define SE05XEXHELPER_H

/* *****************************************************************************************************************
* Public Functions
* ***************************************************************************************************************** */

/*!
 * @brief support function for changekey example
 *  This example also reverts back the changed keys
 * @param   pSession sss session pointer
 * @param   pRdCtx rdLib context pointer
 * @param   bCardKeyAuth The card key to auth
 * @param   se05xAuthobj Se050 object ID for authentication
 * @param   bCardKeyToChange The card key to change
 * @param   se05xChangeOldobj Se050 objectID which is currently being used to authenticate
.* @param   se05xChangeNewobj Se050 objectID which will be used in future to authenticate

 * @return  status
 */
extern phStatus_t nxEx_AuthChangeVerifyRevert(sss_se05x_session_t * pSession, NPNxpNfcRdLibCtx_t* pRdCtx,
    uint8_t bCardKeyAuth, uint32_t se05xAuthobj, uint8_t bCardKeyToChange,
    uint32_t se05xChangeOldobj, uint32_t se05xChangeNewobj, uint8_t* pCardUid,
    uint8_t bCardUidLen, uint16_t wOption);


/*!
 * @brief Example to get card UID
 * @param   pAlMfdfEv2 mifare desfire ev2 params
 * @return  status
 */
extern phStatus_t nxEx_Use_GetUid(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2);

#endif  /*SE05XEXHELPER_H*/
