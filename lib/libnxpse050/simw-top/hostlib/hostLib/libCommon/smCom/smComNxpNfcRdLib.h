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
#ifndef _SMCOMNxpNfcRdLib_H_INC_
#define _SMCOMNxpNfcRdLib_H_INC_

#include "npRdLib/npRdLib.h"
#include "smCom.h"

/* ------------------------------------------------------------------------- */

/** TODO */
U16 smComNxpNfcRdLib_OpenVCOM(void **conn_ctx, const char * vPortName);

void smComNxpNfcRdLib_Close(void);

#endif /* _SMCOMNxpNfcRdLib_H_INC_ */
