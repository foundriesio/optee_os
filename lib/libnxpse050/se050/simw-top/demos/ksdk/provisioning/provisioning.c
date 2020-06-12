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

#include "provisioning.h"
#include <ex_sss.h>
#include <ex_sss_boot.h>
#include <nxLog_App.h>

static ex_sss_boot_ctx_t gex_sss_provisioning_ctx;

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_provisioning_ctx)
#if SSS_HAVE_A71CH || SSS_HAVE_A71CH_SIM
#define EX_SSS_BOOT_DO_ERASE 1
#else
#define EX_SSS_BOOT_DO_ERASE 0
#endif
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0

#include <ex_sss_main_inc.h>
