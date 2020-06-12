/**
 * @file ex_Ev2Auth_se05x.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2019,2020 NXP
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
 * This is an example demonstrating the MIFARE DESFire EV2 authentcation
 * using Seo50. After authentication it performs encrypted communication
 * with the desfire EV2 card.
 * Prerequisites for running this example include
 * ex_Ev2Prepare_Card has been successfully executed and
 * ex_Ev2Prepare_se050 has been successfully executed
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
#include "phExMfDfCrypto.h"
#include "nxLog_App.h"
#include "nfc_support.h"
#include "se05x_MfDfInit.h"
#include "ex_sss_boot.h"
#include "phExMfDfCrypto_se05x.h"
#include "se05xExHelper.h"

#ifdef __cplusplus
}
#endif

/* *****************************************************************************************************************
* Internal Definitions
* ***************************************************************************************************************** */

/* *****************************************************************************************************************
* Type Definitions
* ***************************************************************************************************************** */

/* *****************************************************************************************************************
* Global and Static Variables
* Total Size: NNNbytes
* ***************************************************************************************************************** */
NPNxpNfcRdLibCtx_t gRdLibCtx = { 0 };

static ex_sss_boot_ctx_t gex_ev2Auth_boot_ctx;

#define EX_SSS_BOOT_PCONTEXT (&gex_ev2Auth_boot_ctx)
#define EX_SSS_BOOT_DO_ERASE 0
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0

#include <ex_sss_main_inc.h>

/* *****************************************************************************************************************
* Private Functions Prototypes
* ***************************************************************************************************************** */

/* *****************************************************************************************************************
* Public Functions
* ***************************************************************************************************************** */

/* *****************************************************************************************************************
* Private Functions
* ***************************************************************************************************************** */

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    phStatus_t status = PH_ERR_SUCCESS;
    sss_status_t sssStatus = kStatus_SSS_Success;
    sss_se05x_session_t * pSession = (sss_se05x_session_t *)&(pCtx->session);
    NPNxpNfcRdLibCtx_t* pRdCtx;

#if defined LPC_55x
    //Workaround as LPC55s executes the previous binary during flashing
    LOG_I("Sleeping for 10 seconds for debugger stabilization");
    sm_sleep(10000);
#endif

    nfcInit(&pRdCtx);
    nfcConnect(pRdCtx);

    SubsequentSetupSe050(&pCtx->ks, MFDFEV2_KEYID, MFDFEV2_CHANGED_KEYID);
    //other option is PHAL_MFDFEV2_NO_DIVERSIFICATION,  PHAL_MFDFEV2_CHGKEY_DIV_NEW_KEY
    status = phEx_Auth_Se05xEV2(&pRdCtx->salMfdfEv2, pSession, MFDFEV2_KEYID, PHAL_MFDFEV2_NO_DIVERSIFICATION, 0);
    CHECK_SUCCESS(status);

    status = nxEx_Use_GetUid(&pRdCtx->salMfdfEv2);
    CHECK_SUCCESS(status);
    status = phEx_Use_ValueFile(&pRdCtx->salMfdfEv2);
    CHECK_SUCCESS(status);
    status = phEx_KillAuth_Se05xEV2(pSession, &pRdCtx->salMfdfEv2, 0);
    CHECK_SUCCESS(status);
    status = nfcDisconnect(pRdCtx);
    CHECK_SUCCESS(status);
    return sssStatus;
}
