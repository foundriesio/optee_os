/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include <ex_sss.h>
#include <ex_sss_boot.h>
#include <fsl_sss_se05x_apis.h>
#include <nxEnsure.h>
#include <nxLog_App.h>
#include <string.h>
/* doc:start:MALS-include-files */
#include "mal_api.h"
/* doc:end:MALS-include-files */

/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */

/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

/* ************************************************************************** */
/* Private Functions                                                          */
/* ************************************************************************** */

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

static ex_sss_boot_ctx_t gex_sss_boot_ctx;

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_boot_ctx)
#define EX_SSS_BOOT_DO_ERASE 0
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0
#define EX_SSS_BOOT_SKIP_SELECT_APPLET 1

#include <ex_sss_main_inc.h>

#if defined(SMCOM_JRCP_V2) && SSSFTR_SW_TESTCOUNTERPART
/* NXP Internal Testing */
static const uint8_t gbuffer[] = {
#include "MALS_UpgradeTo_iotDev-4.5.0-20200213-00_SIM.h"
};
#else // JRCP_V2
/* doc:start:MALS-protobuf-declare */
static const uint8_t gbuffer[] = {
#include "MALS_UpgradeTo_iotDev-4.5.0-20200213-00.h"
};
/* doc:end:MALS-protobuf-declare */
#endif

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    mal_status_t status = kStatus_MAL_ERR_General;
    sss_status_t rv = kStatus_SSS_Fail;
    uint8_t *buffer = NULL;
    /* doc:start:MALS-context-declare */
    mal_agent_ctx_t s_mal_agent_ctx = {0};
    /* doc:end:MALS-context-declare */

    /* doc:start:MALS-api-usage-init */
    rv = mal_agent_init_context(&s_mal_agent_ctx, &pCtx->session);
    /* doc:end:MALS-api-usage-init */
    ENSURE_OR_GO_CLEANUP(rv == kStatus_SSS_Success);

    /* In case channel is not close in former operation. */
    rv = mal_agent_session_close(&s_mal_agent_ctx);
    ENSURE_OR_GO_CLEANUP(rv == kStatus_SSS_Success);

    /* doc:start:MALS-api-usage-open */
    rv = mal_agent_session_open(&s_mal_agent_ctx);
    /* doc:end:MALS-api-usage-open */
    ENSURE_OR_GO_CLEANUP(rv == kStatus_SSS_Success);

    /* doc:start:MALS-api-usage-load-pkg */
    buffer = (uint8_t *)gbuffer;
    status = mal_agent_load_package(
        &s_mal_agent_ctx, buffer, sizeof(gbuffer), NULL, 0);
    /* doc:end:MALS-api-usage-load-pkg */
    if (status != kStatus_MAL_Success) {
        mal_agent_session_close(&s_mal_agent_ctx);
    }
    ENSURE_OR_GO_CLEANUP(status == kStatus_MAL_Success);

    /* doc:start:MALS-api-close */
    rv = mal_agent_session_close(&s_mal_agent_ctx);
    /* doc:end:MALS-api-close */
    ENSURE_OR_GO_CLEANUP(rv == kStatus_SSS_Success);

    LOG_I("Update Applet successful !!!");

cleanup:
    if (kStatus_MAL_Success == status) {
        LOG_I("mals_ex_update Example Success !!!...");
        rv = kStatus_SSS_Success;
    }
    else {
        LOG_E("mals_ex_update Example Failed !!!...");
        rv = kStatus_SSS_Fail;
    }

    return rv;
}
