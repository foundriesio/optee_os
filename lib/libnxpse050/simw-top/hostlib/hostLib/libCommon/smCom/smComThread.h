/**
 * Copyright 2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 *****************************************************************************/

#ifndef SM_COM_THREAD_H
#define SM_COM_THREAD_H

#include "smCom.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * closes communication layer.
 *
 * @return
 */
U16 smComThread_Close(void);

/**
 * Initializes the communication layer.
 *
 * @return
 */
U16 smComThread_Open(U8 *Threadatr, U16 *ThreadatrLen);

#if defined(__cplusplus)
}
#endif

#endif /* SM_COM_THREAD_H */
