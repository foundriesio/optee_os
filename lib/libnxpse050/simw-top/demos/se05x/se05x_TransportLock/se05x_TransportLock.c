/* Copyright 2020 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
*/

#include <nxLog_App.h>
#include <se05x_APDU.h>

#include "se05x_TransportAuth.h"

static ex_sss_boot_ctx_t gex_sss_transport_lock_boot_ctx;

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_transport_lock_boot_ctx)
#define EX_SSS_BOOT_DO_ERASE 0
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0

static sss_status_t se05x_TransportLock(Se05xSession_t *pSe05xSession);

#include <ex_sss_main_inc.h>
sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    sss_status_t status           = kStatus_SSS_Fail;
    sss_se05x_session_t *pSession = (sss_se05x_session_t *)&pCtx->session;
    smStatus_t sw_status;
    Se05xSession_t *pSe05xSession;
    sss_session_t txSession = {0};
    sss_tunnel_t txTunnel   = {0};

    /*Create transport object in host*/
    status = se05x_TA_CreateHostKey(pCtx);
    if (kStatus_SSS_Success != status) {
        LOG_E("Failed to set transport object in host!!!");
        goto cleanup;
    }
    pSe05xSession = &pSession->s_ctx;
    /*Create transport object in SE*/
    sw_status = se05x_TA_CreateSETransportLockKey(pSe05xSession);
    if (sw_status == SM_ERR_CONDITIONS_OF_USE_NOT_SATISFIED) {
        LOG_W("Transport to se05x applet is Locked!!!");
    }

    status = se05x_TA_OpenAuthSession(pCtx, &txSession, &txTunnel);

    if (kStatus_SSS_Success == status) {
        pSe05xSession = &((sss_se05x_session_t *)&txSession)->s_ctx;
        status        = se05x_TransportLock(pSe05xSession);
    }

    sss_session_close(&txSession);

cleanup:
    return status;
}

static sss_status_t se05x_TransportLock(Se05xSession_t *pSe05xSession)
{
    sss_status_t status = kStatus_SSS_Fail;
    smStatus_t sw_status;
    uint8_t lockIndicator = 0; /*Persistent Lock*/
    uint8_t lockState     = 1; /* Lock Tx*/

    sw_status = Se05x_API_SetLockState(pSe05xSession, lockIndicator, lockState);
    if (SM_OK != sw_status) {
        LOG_E("Failed Se05x_API_SetLockState");
        goto cleanup;
    }
    else {
        LOG_W("Se05x_API_SetLockState Transport Lock feature is success");
        status = kStatus_SSS_Success;
    }

cleanup:
    return status;
}
