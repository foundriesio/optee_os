/*
 * Copyright 2018, 2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 */
#ifndef _NXP_IOT_AGENT_PLATFORM_UNISTD_H_
#define _NXP_IOT_AGENT_PLATFORM_UNISTD_H_

#ifdef _WIN32

#include <io.h>
 /* Values for the second argument to access.
 These may be OR'd together.  */
#define R_OK    4       /* Test for read permission.  */
#define W_OK    2       /* Test for write permission.  */
//#define   X_OK    1       /* execute permission - unsupported in windows*/
#define F_OK    0       /* Test for existence.  */

#endif // _WIN32

#endif // _NXP_IOT_AGENT_PLATFORM_UNISTD_H_
