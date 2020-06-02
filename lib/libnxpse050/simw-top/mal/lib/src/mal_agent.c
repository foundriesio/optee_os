/*
 * Copyright 2019,2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mal_agent.h"

#include <ex_sss_boot.h>
#include <nxLog_App.h>
#include <sm_types.h>

#include "mal_api.h"
#include "nxEnsure.h"
#include "nxp_iot_agent_context.h"
#include "nxp_iot_agent_keystore.h"
#include "pb.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "se05x_apis.h"
#include "string.h" /* memset */
//#include "mal_agent.pb.h"
#include <smCom.h>

#include "global_platf.h"
#include "mal_agent_context.h"

#if (SSS_HAVE_SSS > 1)
/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */
mal_status_t mal_agent_register_keystore(iot_agent_context_t *ctx, iot_agent_keystore_t *keystore)
{
    mal_status_t retval = kStatus_MAL_ERR_General;

    ENSURE_OR_GO_EXIT(ctx->numKeystores < NXP_IOT_AGENT_MAX_NUM_KEYSTORES);

    ctx->keystores[ctx->numKeystores++] = keystore;
    retval                              = kStatus_MAL_Success;
exit:
    return retval;
}

mal_status_t mal_agent_init_dispatcher(iot_agent_dispatcher_context_t *dispatcher_context,
    iot_agent_context_t *agent_context,
    iot_agent_stream_type_t stream_type)
{
    mal_status_t retval  = kStatus_MAL_ERR_General;
    size_t num_endpoints = 0;

    ENSURE_OR_GO_EXIT((dispatcher_context != NULL) && (agent_context != NULL));

    dispatcher_context->agent_context   = agent_context;
    dispatcher_context->current_request = NULL;
    dispatcher_context->closed          = false;

    for (size_t i = 0; i < NXP_IOT_AGENT_MAX_NUM_ENDPOINTS; i++)
        dispatcher_context->endpoints[i].type = nxp_iot_EndpointType_INVALID;

    for (size_t i = 0; i < agent_context->numKeystores; i++) {
        dispatcher_context->endpoints[num_endpoints].type = agent_context->keystores[i]->type;
        dispatcher_context->endpoints[num_endpoints].id   = agent_context->keystores[i]->identifier;
        dispatcher_context->endpoints[num_endpoints].endpoint_interface =
            agent_context->keystores[i]->iface.endpoint_interface;
        dispatcher_context->endpoints[num_endpoints].endpoint_context = agent_context->keystores[i]->context;
        num_endpoints++;
    }

    dispatcher_context->closed      = false;
    dispatcher_context->stream_type = stream_type;
    retval                          = kStatus_MAL_Success;
exit:
    return retval;
}

mal_status_t mal_agent_handle_status_word(void *context)
{
    mal_agent_ctx_t *mal_agent_context = (mal_agent_ctx_t *)context;
    uint32_t status_word;
    mal_status_t return_code;

    if (context == NULL) {
        LOG_E("MALS Check Status Word: Pointer Error!!!");
        return_code = kStatus_MAL_ERR_General;
    }
    else {
        status_word = mal_agent_context->status_word;
        LOG_D("MALS Check Status Word: %x.", status_word);

        if (status_word == MAL_AGENT_STATUS_WORD_INIT) {
            /** status word is initial value. Some error other than R-APDU happens */
            LOG_E("MALS Check Status Word: Generic Error!!!");
            return_code = kStatus_MAL_ERR_General;
        }
        else if ((status_word & MAL_AGENT_STATUS_WORD_HOST_CMD_MASK) == MAL_AGENT_STATUS_WORD_HOST_CMD_MASK) {
            /** The only possible status word for host control command is OK by now */
            LOG_I("MALS Check Status Word: Host Command Success.");
            return_code = kStatus_MAL_Success;
        }
        else if (status_word == 0x9000) {
            LOG_I("MALS Check Status Word: Success.");
            return_code = kStatus_MAL_Success;
        }
        else if (status_word == MAL_AGENT_STATUS_WORD_USER_DEFINE_SUCCESS) {
            LOG_I("MALS Check Status Word: Success.");
            return_code = kStatus_MAL_Success;
        }
        else if (((status_word & 0xFF00) == 0x6100) || status_word == 0x6310) {
            LOG_I("MALS Check Status Word: Success.");
            return_code = kStatus_MAL_Success;
        }
        else if (status_word == 0x6A82 || status_word == 0x6A88 || status_word == 0x6999 || status_word == 0x6A80 ||
                 status_word == 0x6982) {
            LOG_E("MALS Check Status Word: Not Applicable.");
            return_code = kStatus_MAL_ERR_NotApplicable;
        }
        else if (status_word == 0x6201) {
            if (mal_agent_context->recovery_executed) {
                // Report Successful. Recovery to old version. Exit current script.
                LOG_I("MALS Check Status Word: Recovery Success.");
                return_code = kStatus_MAL_Success;
            }
            else {
                LOG_E("MALS Check Status Word: Not Applicable.");
                return_code = kStatus_MAL_ERR_NotApplicable;
            }
        }
        else if ((status_word & 0xFF00) == 0x6400) {
            LOG_E("MALS Check Status Word: Fatal Error.");
            return_code = kStatus_MAL_ERR_Fatal;
        }
        else if (status_word == MAL_AGENT_STATUS_WORD_COM_FAILURE) {
            LOG_E("MALS Check Status Word: COM Error.");
            return_code = kStatus_MAL_ERR_COM;
        }
        else {
            // Whether applet upgrade session is in progress. GET DATA [p1p2=00C2]
            mal_upgradeProgress_status_t upgradeStatus;
            sss_status_t sss_stat = kStatus_SSS_Fail;

            sss_stat = mals_check_AppletUpgradeProgress(mal_agent_context, &upgradeStatus);
            if (sss_stat != kStatus_SSS_Success) {
                LOG_E(
                    "MALS Check Status Word: Fail to get applet upgrade "
                    "progress.");
                return_code = kStatus_MAL_ERR_General;
            }
            else if (upgradeStatus == mal_upgrade_not_inProgress) {
                /*not in progress*/
                LOG_E("MALS Check Status Word: Not Applicable.");
                return_code = kStatus_MAL_ERR_NotApplicable;
            }
            else if ((status_word == 0x6200) || (status_word == 0x6202) || (status_word == 0x6203)) {
                LOG_E("MALS Check Status Word: Do Recovery.");
                return_code = kStatus_MAL_ERR_DoRecovery;
            }
            else if (status_word == 0x6A84) {
                mal_recovery_status_t recoveryStatus;
                sss_stat = mals_check_AppletRecoveryStatus(mal_agent_context, &recoveryStatus);
                if (sss_stat != kStatus_SSS_Success) {
                    LOG_E(
                        "MALS Check Status Word: Fail to get applet recovery "
                        "progress.");
                    return_code = kStatus_MAL_ERR_General;
                }
                else if (recoveryStatus == mal_recovery_not_started) {
                    LOG_E("MALS Check Status Word: Fatal Error.");
                    return_code = kStatus_MAL_ERR_Fatal;
                }
                else {
                    LOG_E("MALS Check Status Word: Do Recovery.");
                    return_code = kStatus_MAL_ERR_DoRecovery;
                }
            }
            else {
                LOG_E("MALS Check Status Word: Not Applicable.");
                return_code = kStatus_MAL_ERR_NotApplicable;
            }
        }
    }

    return return_code;
}

mal_status_t mal_agent_dispatcher(
    iot_agent_dispatcher_context_t *dispatcher_context, pb_istream_t *input, pb_ostream_t *output)
{
    iot_agent_status_t iot_agent_ret = IOT_AGENT_FAILURE;

    iot_agent_ret = iot_agent_dispatcher(dispatcher_context, input, output);

    if (iot_agent_ret != IOT_AGENT_SUCCESS) {
        return kStatus_MAL_ERR_General;
    }
    else {
        return kStatus_MAL_Success;
    }
}

#endif /* SSS_HAVE_SSS > 1 */
