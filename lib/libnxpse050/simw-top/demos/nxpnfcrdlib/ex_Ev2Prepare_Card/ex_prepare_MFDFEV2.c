/**
 * @file ex_Ev2Prepare_Card.c
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
 * This is an example project for preparing the MIFARE DESFire EV2 card for
 * running other SE050 MIFARE DESFire EV2 examples.
 * This example formats the card creates app, keys and file
 * for running the other examples.
 */


/* *****************************************************************************************************************
* Includes
* ***************************************************************************************************************** */
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <phTMIUtils.h>
#include <phalMfdfEv2.h>
#include <phalVca.h>
#include <stdio.h>
#include "np_Delegate_Bal.h"
#include "np_Delegate_Platform.h"
#include "np_NxpNfcRdLib.h"
#include "np_TypeA.h"
#include "phApp_Init.h"
#include "phExMfDfCrypto.h"
#include "phNfcLib.h"
#include "ph_Status.h"
#include "phOsal.h"
#include "nxLog_App.h"
#include "nfc_support.h"
#include "ex_sss.h"
#include "ex_sss_boot.h"
#include "fsl_sss_se05x_types.h"
#include "se05xExHelper.h"
#include <phApp_Init.h>
#ifdef PH_PLATFORM_HAS_ICFRONTEND
#include "BoardSelection.h"
#endif
#include <phNfcLib.h>
#ifdef __cplusplus
}
#endif


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
sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    phStatus_t status = PH_ERR_SUCCESS;
    sss_status_t sssStatus = kStatus_SSS_Fail;
    NPNxpNfcRdLibCtx_t* pRdCtx;
#if defined LPC_55x
    //Workaround as LPC55s executes the previous binary during flashing
    LOG_I("Sleeping for 10 seconds for debugger stabilization");
    sm_sleep(10000);
#endif
    nfcInit(&pRdCtx);
    nfcConnect(pRdCtx);
    //Prepare the card
    status = phEx_Personalize_AFCApp(&pRdCtx->salMfdfEv2);
    CHECK_SUCCESS(status);
    status = phEx_Create_ValueFile(&pRdCtx->salMfdfEv2);
    CHECK_SUCCESS(status);
    status = nfcDisconnect(pRdCtx);
    CHECK_SUCCESS(status);

    sssStatus = kStatus_SSS_Success;
    return sssStatus;
}
