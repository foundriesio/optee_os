/**
 * @file se05xExHelper.c
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
 * File for Helper fuctions for Examples They need both nfcrdlib and se05x support
 */

/* *****************************************************************************************************************
* Includes
* ***************************************************************************************************************** */
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "np_Delegate_Bal.h"
#include "np_Delegate_Platform.h"
#include "np_NxpNfcRdLib.h"
#include "np_TypeA.h"
#include "phApp_Init.h"
#include "phNfcLib.h"
#include "ph_Status.h"
#include "phOsal.h"
#include "ex_sss.h"
#include "fsl_sss_se05x_types.h"
#include "nxLog_App.h"
#include "nfc_support.h"
#include "se05x_MfDfInit.h"
#include "ex_sss_boot.h"
#include "phExMfDfCrypto_se05x.h"
#include "nxLog_App.h"
#include "se05xExHelper.h"

#ifdef __cplusplus
}
#endif

/**
Macro to enable/dible if the changed key has to be reverted back
*/
# define REVERT_CHANGED_KEY 1

/* *****************************************************************************************************************
* Public Functions
* ***************************************************************************************************************** */

phStatus_t nxEx_AuthChangeVerifyRevert(sss_se05x_session_t * pSession, NPNxpNfcRdLibCtx_t* pRdCtx,
    uint8_t bCardKeyAuth, uint32_t se05xAuthobj, uint8_t bCardKeyToChange,
    uint32_t se05xChangeOldobj, uint32_t se05xChangeNewobj, uint8_t* pCardUid,
    uint8_t bCardUidLen, uint16_t wOption)
{
    uint8_t bCardKeyVerChange = 1;
    phStatus_t status = PH_ERR_SUCCESS;
    status = phEx_ChangeKey_Se05xEV2(&pRdCtx->salMfdfEv2,
        pSession,
        se05xAuthobj,      //SE050 key to be used for Auth
        se05xChangeOldobj,      //SE050 key old Key for ChangeKey
        se05xChangeNewobj,              //SE050 key new Key for ChangeKey
        wOption,
        bCardKeyAuth,                 //Card Key number for auth
        bCardKeyToChange,               //Card Key number to change
        bCardKeyVerChange,                              //Card Key version to change
        pCardUid,                      //Diversifacation Input
        bCardUidLen);                    //Diversifacation Input Len    CHECK_SUCCESS(status);
    CHECK_SUCCESS(status);

    if (bCardKeyAuth != bCardKeyToChange)
    {
        LOG_I("Checking that the previous auth session is still valid by trying an encrypted communication");
        CHECK_SUCCESS(status);
        status = nxEx_Use_GetUid(&pRdCtx->salMfdfEv2);
        CHECK_SUCCESS(status);
        LOG_I("Previous auth session is still valid");
    }
    else
    {
        se05xAuthobj = se05xChangeNewobj;
        LOG_I("The previous auth session is not valid anymore. ", bCardKeyAuth);
        LOG_I("So..... ");
    }

    LOG_I("Auth with the changed cardkey %d", bCardKeyToChange);
    status = phEx_Auth_Se05xEV2(&pRdCtx->salMfdfEv2,
        pSession,
        se05xChangeNewobj,
        PHAL_MFDFEV2_NO_DIVERSIFICATION,
        bCardKeyToChange);
    CHECK_SUCCESS(status);

    LOG_I("Checking that the auth session with changed key is valid by trying an encrypted communication");
    status = nxEx_Use_GetUid(&pRdCtx->salMfdfEv2);
    CHECK_SUCCESS(status);
    LOG_I("Encrypted communication with changed key successful");

#if    REVERT_CHANGED_KEY
    LOG_I("Reverting the changed cardkey %d", bCardKeyToChange);
    status = phEx_ChangeKey_Se05xEV2(&pRdCtx->salMfdfEv2,
        pSession,
        se05xAuthobj,                  //SE050 key to be used for Auth
        se05xChangeNewobj,          //SE050 key old Key for ChangeKey
        se05xChangeOldobj,                  //SE050 key new Key for ChangeKey
        PHAL_MFDFEV2_NO_DIVERSIFICATION,
        bCardKeyAuth,                 //Card Key number for auth
        bCardKeyToChange,               //Card Key number to change
        bCardKeyVerChange,                              //Card Key version to change
        NULL,                      //Diversifacation Input
        0);                    //Diversifacation Input Len
    CHECK_SUCCESS(status);
    LOG_I("Reverting Successful ");
#endif

    return status;

}


phStatus_t nxEx_Use_GetUid(phalMfdfEv2_Sw_DataParams_t *pAlMfdfEv2)
{
    phStatus_t status;
    uint8_t bCardUid[7];

    status = phalMfdfEv2_GetCardUID(pAlMfdfEv2, bCardUid);
    CHECK_SUCCESS(status);
    LOG_MAU8_I("CARD UID is as below ", bCardUid, 7);
    return status;
}
