/*
 * Copyright 2018,2020 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 */

#ifndef _NXP_IOT_AGENT_KEYSTORE_MAL_SE05X_H_
#define _NXP_IOT_AGENT_KEYSTORE_MAL_SE05X_H_

#include <mal_agent_context.h>
#include <nxp_iot_agent_common.h>
#include <nxp_iot_agent_keystore.h>

#if SSS_HAVE_APPLET_SE05X_IOT

#include <fsl_sss_api.h>

/*!
 * @addtogroup mal_agent_keystore_SE05X
 * @{
 */

/**
  * @brief A context holding the state of a keystore; this is passed to keystore interface functions.
  */
typedef struct iot_agent_keystore_mal_se05x_context_t
{
    mal_agent_ctx_t *mal_agent_context;
} iot_agent_keystore_mal_se05x_context_t;

/*! @brief Initialize a MALS keystore.
 *
 * Ownership of the keystore_context remains at the caller!
 */
iot_agent_status_t iot_agent_keystore_mal_se05x_init(
    iot_agent_keystore_t *keystore, int identifier, iot_agent_keystore_mal_se05x_context_t *keystore_context);

/*! @brief Destroy the context of the MALS keystore.
 */
iot_agent_status_t iot_agent_keystore_mal_se05x_destroy(void *context);

/*! @brief Open a connection to the MALS keystore.
 */
iot_agent_status_t iot_agent_keystore_mal_se05x_open_session(void *context);

/*! @brief Cloxe a connection to the MALS keystore.
 */
void iot_agent_keystore_mal_se05x_close_session(void *context);

/*! @brief Get an endpoint information of the MALS keystore
 * @param[in] context End point context
 * @param[in] endpoint_information End point information
 *
 */
bool iot_agent_keystore_mal_se05x_get_endpoint_info(void *context, void *endpoint_information);

/** @copydoc iot_agent_endpoint_request_handler_t
*
*/
bool iot_agent_keystore_mal_se05x_handle_request(
    pb_istream_t *istream, pb_ostream_t *ostream, const pb_field_t *message_type, void *context);

extern const iot_agent_keystore_interface_t iot_agent_keystore_mal_se05x_interface;

/*!
*@}
*/ /* end of edgelock2go_agent_keystore_SE05X */

#endif // #if SSS_HAVE_APPLET_SE05X_IOT

#endif // _NXP_IOT_AGENT_KEYSTORE_MAL_SE05X_H_
