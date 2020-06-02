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
#ifndef _NXP_MAL_AGENT_HANDLE_H_
#define _NXP_MAL_AGENT_HANDLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#if SSS_HAVE_APPLET_SE05X_IOT

#include <mal_agent_context.h>

/** @ingroup mal_agent
*
* @page page_mal_agent MAL agent
*
* @brief Public Apis
*/

/*!
* @addtogroup mal_agent
* @{
*/

/*! @brief Reset SE
* @param[in]  context: mal agent context
* @retval kStatus_MAL_Success Upon success
*/
mal_status_t mal_agent_reset_se(mal_agent_ctx_t *mal_agent_context);

/*!
*@}
*/ /* end of mal_agent */

#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif // #ifndef _NXP_MAL_AGENT_HANDLE_H_
