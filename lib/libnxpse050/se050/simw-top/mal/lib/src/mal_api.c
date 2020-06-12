/*
 * Copyright 2019,2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "mal_api.h"

#include <ex_sss_boot.h>
#include <nxLog_App.h>
#include <smCom.h>
#include <sm_types.h>

#include "mal_agent.h"
#include "mal_agent_common.h"
#include "mal_agent_context.h"
#include "mal_keystore_se05x.h"
#include "nxEnsure.h"
#include "nxp_iot_agent_dispatcher.h"
#include "string.h" /* memset */

#if (SSS_HAVE_SSS > 1)
/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */
mal_status_t mal_agent_load_package(
    mal_agent_ctx_t *context, uint8_t *pkgBuf, size_t pkgBufLen, uint8_t *sigBuf, size_t sigBufLen)
{
    mal_status_t retval                  = kStatus_MAL_ERR_General;
    mal_tearDown_status_t tearStatus     = mal_notear;
    mal_recovery_status_t recoveryStatus = mal_recovery_not_started;
    sss_status_t sss_stat                = kStatus_SSS_Fail;
    pb_istream_t input_stream;
    pb_ostream_t output_stream;
    uint8_t prev_sign_buffer[128];
    size_t prev_sign_buffer_len = 0;
    uint8_t response_buffer_memory[4096];
    iot_agent_context_t iot_agent_context                   = {0};
    iot_agent_keystore_t keystore                           = {0};
    iot_agent_keystore_mal_se05x_context_t keystore_context = {0};
    iot_agent_dispatcher_context_t dispatcher_context       = {0};

    ENSURE_OR_GO_EXIT((pkgBuf != NULL) && (context != NULL));

    if (sigBuf != NULL) {
        // In case user provide signature, check tear status.
        if (mals_check_Tear(context, &tearStatus) == kStatus_SSS_Success) {
            if (tearStatus == mal_tear) {
                // Tear happens in last script
                LOG_I("FN %s: Teardown in last script", __FUNCTION__);

                // Get Last script signature
                if (mals_get_SignatureofLastScript(context, prev_sign_buffer, &prev_sign_buffer_len) ==
                    kStatus_SSS_Success) {
                    if (prev_sign_buffer_len == 0) {
                        LOG_E("MALS get null signature!!!");
                        retval = kStatus_MAL_ERR_General;
                        goto exit;
                    }

                    // Signature doesn't match. Inform user.
                    if ((sigBufLen != prev_sign_buffer_len) || (memcmp(sigBuf, prev_sign_buffer, sigBufLen) != 0)) {
                        LOG_I("MALS signature not match!!!");
                        retval = kStatus_MAL_ERR_DoReRun;
                        goto exit;
                    }
                }
                else {
                    LOG_E("MALS get signature failed!!!");
                    retval = kStatus_MAL_ERR_General;
                    goto exit;
                }
            }
        }
        else {
            LOG_E("MALS check tear status failed!!!");
            retval = kStatus_MAL_ERR_General;
            goto exit;
        }
    }

    sss_stat = mals_check_AppletRecoveryStatus(context, &recoveryStatus);
    ENSURE_OR_GO_EXIT(sss_stat == kStatus_SSS_Success);
    if (recoveryStatus == mal_recovery_started) {
        // If recovery started.
        context->recovery_executed = true;
    }
    else
        context->recovery_executed = false;

    context->status_word        = MAL_AGENT_STATUS_WORD_INIT;
    context->skip_next_commands = false;

    keystore_context.mal_agent_context = context;
    iot_agent_keystore_mal_se05x_init(&keystore, MAL_AGENT_KEYSTORE_ID, &keystore_context);

    retval = mal_agent_register_keystore(&iot_agent_context, &keystore);
    ENSURE_OR_GO_EXIT(retval == kStatus_MAL_Success);

    retval = mal_agent_init_dispatcher(&dispatcher_context, &iot_agent_context, STREAM_TYPE_BUFFER_REQUESTS);
    ENSURE_OR_GO_EXIT(retval == kStatus_MAL_Success);

    input_stream  = pb_istream_from_buffer(pkgBuf, pkgBufLen);
    output_stream = pb_ostream_from_buffer(response_buffer_memory, sizeof(response_buffer_memory));
    retval        = mal_agent_dispatcher(&dispatcher_context, &input_stream, &output_stream);

    if (retval != kStatus_MAL_Success) {
        if (context->status_word == MAL_AGENT_STATUS_WORD_COM_FAILURE)
            retval = kStatus_MAL_ERR_COM;
        else
            retval = kStatus_MAL_ERR_General;

        goto exit;
    }

    /** Process status word and map to return code. */
    retval = mal_agent_handle_status_word(context);

    ENSURE_OR_GO_EXIT(retval == kStatus_MAL_Success);

    /** Check tear status before exit.
        *   Only do it when retval is success
        */
    if (mals_check_Tear(context, &tearStatus) == kStatus_SSS_Success) {
        if (tearStatus == mal_tear) {
            LOG_I("FN %s: Teardown happens", __FUNCTION__);
            retval = kStatus_MAL_ERR_Fatal;
            goto exit;
        }
        else {
            retval = kStatus_MAL_Success;
        }
    }
    else {
        LOG_E("MALS final check tear status failed!!!");
        retval = kStatus_MAL_ERR_General;
    }

exit:
    return retval;
}

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */
sss_status_t mal_agent_init_context(mal_agent_ctx_t *context, sss_session_t *boot_ctx)
{
    sss_status_t retval = kStatus_SSS_Fail;
    sss_se05x_session_t *pSE05x_Session;

    ENSURE_OR_GO_EXIT((context != NULL) && (boot_ctx != NULL));

    LOG_D("MALS Agent Version: %d.%d.%d", MALS_API_VERSION_MAJOR, MALS_API_VERSION_MINOR, MALS_API_VERSION_PATCH);

    pSE05x_Session = (sss_se05x_session_t *)boot_ctx;

    context->pS05x_Ctx          = &pSE05x_Session->s_ctx;
    context->status_word        = MAL_AGENT_STATUS_WORD_INIT;
    context->recovery_executed  = false;
    context->skip_next_commands = false;
#ifdef MAL_AGENT_CHANNEL_1
    context->n_logical_channel = MAL_AGENT_CHANNEL_0;
#endif
    retval = kStatus_SSS_Success;
exit:
    return retval;
}

sss_status_t mal_agent_session_open(mal_agent_ctx_t *context)
{
    sss_status_t status = kStatus_SSS_Success;
    U32 respstat;
#ifdef MAL_AGENT_CHANNEL_1
    const uint8_t openCmd[] = {0x00, 0x70, 0x00, 0x00, 0x01};
    U16 openCmdLen          = sizeof(openCmd);
#endif

    /* clang-format off */
    // A397 orchestrator
    const uint8_t selectCmd[] = {
#ifdef MAL_AGENT_CHANNEL_1
        0x01, 0xA4, 0x04, 0x00,     0x10, 0xA0, 0x00, 0x00,
#else
        0x00, 0xA4, 0x04, 0x00,     0x10, 0xA0, 0x00, 0x00,
#endif
        0x03, 0x96, 0x54, 0x53,     0x00, 0x00, 0x00, 0x01,
        0x03, 0x30, 0x00, 0x00,     0x00, 0x00,
    };

    /* clang-format on */
    U16 selectCmdLen  = sizeof(selectCmd);
    uint8_t resp[128] = {0x00};
    U32 respLen       = sizeof(resp);

    ENSURE_OR_GO_EXIT(context != NULL);

    if (context->pS05x_Ctx == NULL) {
        LOG_W("Failed : Called Session Open without Session Init");
        return status;
    }

#ifdef MAL_AGENT_CHANNEL_1
    respstat = smCom_TransceiveRaw(context->pS05x_Ctx->conn_ctx, (uint8_t *)openCmd, openCmdLen, resp, &respLen);
    ENSURE_OR_GO_EXIT(respstat == SM_OK);
#endif
    respLen  = sizeof(resp);
    respstat = smCom_TransceiveRaw(context->pS05x_Ctx->conn_ctx, (uint8_t *)selectCmd, selectCmdLen, resp, &respLen);
    ENSURE_OR_GO_EXIT(respstat == SM_OK);

#ifdef MAL_AGENT_CHANNEL_1
    context->n_logical_channel = MAL_AGENT_CHANNEL_1;
#endif

    status = kStatus_SSS_Success;
exit:
    return status;
}

sss_status_t mal_agent_session_close(mal_agent_ctx_t *context)
{
    sss_status_t status = kStatus_SSS_Fail;
    U32 respstat;
#ifdef MAL_AGENT_CHANNEL_1
    uint8_t closeCmd[] = {0x00, 0x70, 0x80, 0x01};
#else
    uint8_t closeCmd[] = {0x00, 0xA4, 0x04, 0x00, 0x00};
#endif
    U16 closeCmdLen       = sizeof(closeCmd);
    uint8_t closeResp[32] = {0x00};
    U32 closeRespLen      = sizeof(closeResp);

    ENSURE_OR_GO_EXIT(context != NULL);

    respstat =
        smCom_TransceiveRaw(context->pS05x_Ctx->conn_ctx, (uint8_t *)closeCmd, closeCmdLen, closeResp, &closeRespLen);
    ENSURE_OR_GO_EXIT(respstat == SM_OK);

#ifdef MAL_AGENT_CHANNEL_1
    context->n_logical_channel = MAL_AGENT_CHANNEL_0;
#endif
    status = kStatus_SSS_Success;

exit:
    return status;
}

#endif /* SSS_HAVE_SSS > 1 */
