/*
 * Copyright 2019,2020 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef DEMOS_SE05X_TLA_H_
#define DEMOS_SE05X_TLA_H_

#include <ex_sss_boot.h>

/** Open an Authenticated session to SE050 using
 * the reserved key Transport Lock. */
sss_status_t se05x_TA_OpenAuthSession(ex_sss_boot_ctx_t *pCtx, sss_session_t *pTxSession, sss_tunnel_t *ptxTunnel);

/** Create Key on the Host so that host crypto can use it
 * during authenticaiton */
sss_status_t se05x_TA_CreateHostKey(ex_sss_boot_ctx_t *pCtx);

/** Transport Auth session is authenticated using transport Obj
 */
smStatus_t se05x_TA_CreateSETransportLockKey(Se05xSession_t *pSe05xSession);

#endif //DEMOS_SE05X_TLA_H_
