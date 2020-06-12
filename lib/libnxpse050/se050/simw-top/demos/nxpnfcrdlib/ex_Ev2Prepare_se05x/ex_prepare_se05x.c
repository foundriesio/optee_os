/**
 * @file ex_Ev2Prepare_se050.c
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
 * This is an example project for provisioning the SE050 for
 * running other SE050 MIFARE DESFire EV2 examples.
 * This example Creates and sets 2 SE050 secure objects for
 * further use by other examples.
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

#include "ex_sss.h"
#include "fsl_sss_se05x_types.h"
#include "nxLog_App.h"
#include "se05x_MfDfInit.h"

#include "ex_sss_boot.h"
#ifdef __cplusplus
}
#endif

/* *****************************************************************************************************************
* Global and Static Variables
* Total Size: NNNbytes
* ***************************************************************************************************************** */
static ex_sss_boot_ctx_t gex_sss_prepareSe05x_boot_ctx;

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_prepareSe05x_boot_ctx)
#define EX_SSS_BOOT_DO_ERASE 1
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0

#include <ex_sss_main_inc.h>

/* *****************************************************************************************************************
* Public Functions
* ***************************************************************************************************************** */
sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    sss_status_t status = kStatus_SSS_Success;
#if defined LPC_55x
    //Workaround as LPC55s executes the previous binary during flashing
    LOG_I("Sleeping for 10 seconds for debugger stabilization");
    sm_sleep(10000);
#endif
    status = InitialSetupSe050(&pCtx->ks, MFDFEV2_KEYID, MFDFEV2_CHANGED_KEYID);
    return status;
}
