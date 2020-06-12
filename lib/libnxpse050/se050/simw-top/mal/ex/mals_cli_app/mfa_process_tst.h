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
#ifndef MFA_PROCESS_TST_H_INCLUDED
#define MFA_PROCESS_TST_H_INCLUDED

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

/* ************************************************************************** */
/* Function declarations                                                      */
/* ************************************************************************** */

void mfa_process_testapplet(const char *aid, const char *command);
//#if (SMCOM_JRCP_V1) || (SMCOM_JRCP_V2)
//void mfa_process_prepareTear(const char *);
//#endif
void mfa_process_testIoTPreUpgrade();
void mfa_process_testIoTPostUpgrade();

#endif //MFA_PROCESS_TST_H_INCLUDED
