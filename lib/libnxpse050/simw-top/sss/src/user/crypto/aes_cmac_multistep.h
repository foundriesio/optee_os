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
#ifndef __AES_CMAC_MULTI_STEP_H__
#define __AES_CMAC_MULTI_STEP_H__

#include <string.h>
#include <stdlib.h>

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if SSS_HAVE_HOSTCRYPTO_USER
#include "aes_cmac.h"
void aes_cmac_onego(aes_ctx_t *context, uint8_t *input, unsigned long length, uint8_t *key, uint8_t *mac_value);
void aes_cmac_update(
    aes_ctx_t *context, uint8_t *input, uint8_t *IV, unsigned long length, uint8_t *key, uint8_t *mac_value);
void aes_cmac_finish(
    aes_ctx_t *context, uint8_t *input, uint8_t *IV, unsigned long length, uint8_t *key, uint8_t *mac_value);

#endif //#if SSS_HAVE_HOSTCRYPTO_USER
#endif
