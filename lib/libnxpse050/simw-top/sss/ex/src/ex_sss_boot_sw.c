/*
 * Copyright 2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

/** @file
 *
 * ex_sss_boot_sw.c:  *The purpose and scope of this file*
 *
 * Project:  SecureIoTMW-Debug@appboot-top-eclipse_x86
 *
 * $Date: Mar 10, 2019 $
 * $Author: ing05193 $
 * $Revision$
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */

#include <stdio.h>

#include "ex_sss_boot_int.h"
#include "nxLog_App.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define TEST_ROOT_FOLDER "."

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

sss_status_t ex_sss_boot_mbedtls_open(ex_sss_boot_ctx_t *pCtx, const char *portName)
{
    sss_status_t status = kStatus_SSS_Fail;
#ifdef MBEDTLS_FS_IO
    if (portName == NULL)
        portName = TEST_ROOT_FOLDER;
#else
    portName = NULL;
#endif
    if (pCtx != NULL) {
        status = sss_session_open(&pCtx->session, kType_SSS_mbedTLS, 0, kSSS_ConnectionType_Plain, (void *)portName);
        if (status != kStatus_SSS_Success) {
            LOG_E("mbedtls Session open failed...");
            goto cleanup;
        }
    }

cleanup:
    return status;
}

sss_status_t ex_sss_boot_openssl_open(ex_sss_boot_ctx_t *pCtx, const char *portName)
{
    sss_status_t status = kStatus_SSS_Fail;
    if (portName == NULL)
        portName = TEST_ROOT_FOLDER;
    if (pCtx != NULL) {
        status = sss_session_open(&pCtx->session, kType_SSS_OpenSSL, 0, kSSS_ConnectionType_Plain, (void *)portName);
        if (status != kStatus_SSS_Success) {
            LOG_E("OpenSSL Session open failed...");
            goto cleanup;
        }
    }

cleanup:
    return status;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
