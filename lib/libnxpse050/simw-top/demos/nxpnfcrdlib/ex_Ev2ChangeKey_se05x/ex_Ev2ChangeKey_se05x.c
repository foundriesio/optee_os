/**
 * @file ex_Ev2ChangeKey_se05x.c
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
 * This is an example demonstrating the MIFARE DESFire EV2 ChangeKeyEv2
 * using Seo50. After Changing Keys it performs encrypted communication
 * with the desfire EV2 card using the changed key. If enabled, It also
 * reverts back the changed key.
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

/* *****************************************************************************************************************
* Internal Definitions
* ***************************************************************************************************************** */
#define CHANGE_REVERT_NON_AUTH_KEY  1
#define CHANGE_REVERT_AUTH_KEY      1

/* *****************************************************************************************************************
* Global and Static Variables
* Total Size: NNNbytes
* ***************************************************************************************************************** */

NPNxpNfcRdLibCtx_t gRdLibCtx = {0};

static ex_sss_boot_ctx_t gex_ev2ChngKey_boot_ctx;

#define EX_SSS_BOOT_PCONTEXT (&gex_ev2ChngKey_boot_ctx)
#define EX_SSS_BOOT_DO_ERASE 0
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0

#include <ex_sss_main_inc.h>

/* *****************************************************************************************************************
* Public Functions
* ***************************************************************************************************************** */
sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    phStatus_t status = PH_ERR_SUCCESS;
    sss_status_t sssStatus = kStatus_SSS_Success;
    sss_se05x_session_t * pSession = (sss_se05x_session_t *)&(pCtx->session);
    NPNxpNfcRdLibCtx_t* pRdCtx;
    uint8_t bCardKeyToAuth = 0;
    uint8_t bCardKeyToChange = 0;

#if defined LPC_55x
    //Workaround as LPC55s executes the previous binary during flashing
    LOG_I("Sleeping for 10 seconds for debugger stabilization");
    sm_sleep(10000);
#endif

    nfcInit(&pRdCtx);
    nfcConnect(pRdCtx);
    SubsequentSetupSe050(&pCtx->ks, MFDFEV2_KEYID, MFDFEV2_CHANGED_KEYID);

#if CHANGE_REVERT_NON_AUTH_KEY
    bCardKeyToChange = 2;
    status = nxEx_AuthChangeVerifyRevert(pSession, pRdCtx,
        bCardKeyToAuth, MFDFEV2_KEYID, bCardKeyToChange, MFDFEV2_KEYID, MFDFEV2_CHANGED_KEYID, NULL, 0, PHAL_MFDFEV2_NO_DIVERSIFICATION);
    CHECK_SUCCESS(status);

#endif

#if CHANGE_REVERT_AUTH_KEY
    bCardKeyToChange = 0;
    status = nxEx_AuthChangeVerifyRevert(pSession, pRdCtx,
        bCardKeyToAuth, MFDFEV2_KEYID, bCardKeyToChange, MFDFEV2_KEYID, MFDFEV2_CHANGED_KEYID, NULL, 0, PHAL_MFDFEV2_NO_DIVERSIFICATION);
    CHECK_SUCCESS(status);

#endif

    phEx_KillAuth_Se05xEV2(pSession, &gRdLibCtx.salMfdfEv2, 0);
    status = nfcDisconnect(pRdCtx);
    CHECK_SUCCESS(status);

    return sssStatus;
}
