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
#ifndef MFA_PROCESS_H_INCLUDED
#define MFA_PROCESS_H_INCLUDED

#include <stdint.h>

#include "./protobuf/Agent.pb.h"
#include "./protobuf/Dispatcher.pb.h"
#include "./protobuf/pb.h"
#include "./protobuf/pb_decode.h"
#include "./protobuf/pb_encode.h"
#include "mal_api.h"

/* ************************************************************************** */
/* Defines                                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

/* clang-format off */

/* clang-format on */

/* ************************************************************************** */
/* Function declarations                                                      */
/* ************************************************************************** */

void mfa_process_loadpkg(const char *);
void mfa_process_getuid();
void mfa_process_getcardcontents(const char *);
void mfa_process_getPbkeyId();
void mfa_process_malsgetversion();
void mfa_process_getsignature(const char *);
void mfa_process_checkTear();
void mfa_process_checkUpgradeProgress();
void mfa_process_getENCIdentifier();
void mfa_process_getappcontents(const char *);
void mfa_process_getpkgcontents(const char *);
void mfa_process_getCAIdentifier();
void mfa_process_getCAKeyIdentifier();
void mfa_process_getPkgVerion(const char *pkgAid);

#endif //MFA_PROCESS_H_INCLUDED
