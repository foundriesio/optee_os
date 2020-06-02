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

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */
#ifndef NXP_MFA_UTILS_H_INCLUDED
#define NXP_MFA_UTILS_H_INCLUDED

#include <ex_sss.h>
#include <ex_sss_boot.h>
#include <mal_agent_context.h>

void prepareTear(uint32_t tear_time);
void mfa_process_loadpkg_with_tear(const char *pkgname, uint32_t tear_time);

#endif //NXP_MFA_UTILS_H_INCLUDED