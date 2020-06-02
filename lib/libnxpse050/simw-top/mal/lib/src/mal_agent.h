/* Copyright 2019,2020 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef MAL_AGENT_INC
#define MAL_AGENT_INC

#include <ex_sss_boot.h>
#include <se05x_enums.h>
#include <sm_types.h>

#include "fsl_sss_api.h"
#include "nxLog.h"
#include "nxScp03_Types.h"
#if SSS_HAVE_APPLET_SE05X_IOT
#include "fsl_sss_se05x_types.h"
#endif

#include "mal_agent_context.h"
#include "nxp_iot_agent_dispatcher.h"
#include "pb.h"
#include "pb_decode.h"
#include "pb_encode.h"

/** @defgroup mals_agent MALS Agent
 *
 * Multicast Applet Loader Services Agent
 * @{
 */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/** Status of the MAL APIs */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/**
 * @brief      { function_description }
 *
 * @param      ctx                  The context
 * @param[in]  keystore             The keystore
 *
 * @return     The mal status.
 */
mal_status_t mal_agent_register_keystore(iot_agent_context_t *ctx, iot_agent_keystore_t *keystore);

/**
 * @brief      { function_description }
 *
 * @param      dispatcher_context  The dispatcher context
 * @param      agent_context       The agent context
 * @param[in]  stream_type         The stream type
 *
 * @return     The mal status.
 */
mal_status_t mal_agent_init_dispatcher(iot_agent_dispatcher_context_t *dispatcher_context,
    iot_agent_context_t *agent_context,
    iot_agent_stream_type_t stream_type);

/**
 * @brief      { function_description }
 *
 * @param      context  The context
 *
 * @return     The mal status.
 */
mal_status_t mal_agent_handle_status_word(void *context);

/**
 * @brief      { function_description }
 *
 * @param      context  The context
 * @param      input  Input stream
 * @param      output  Output stream
 *
 * @return     The mal status.
 */
mal_status_t mal_agent_dispatcher(
    iot_agent_dispatcher_context_t *dispatcher_context, pb_istream_t *input, pb_ostream_t *output);

/*!
 *@}
 */ /* end of mal_agent */

#endif // !MAL_AGENT_INC
