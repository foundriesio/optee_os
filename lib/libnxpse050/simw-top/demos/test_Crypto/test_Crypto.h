/*
 * Copyright 2020 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef TST_CRYPTO_H_INC
#define TST_CRYPTO_H_INC
#include <stdio.h>
#include <string.h>

#include "fsl_sss_api.h"
#include "fsl_sss_user_apis.h"
#include "fsl_sss_mbedtls_apis.h"
#include "fsl_sss_openssl_apis.h"
#include "nxLog_App.h"

typedef struct userCtx
{
    sss_session_t session;
    sss_key_store_t ks;
    sss_object_t key;
    sss_rng_context_t rng;
    sss_symmetric_t symm;
    sss_mac_t mac;
} userCtx_t;

#endif
