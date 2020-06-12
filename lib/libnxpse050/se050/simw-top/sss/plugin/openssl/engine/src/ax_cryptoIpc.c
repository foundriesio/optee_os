/*****************************************************************************
 * Copyright 2016 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 ****************************************************************************/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>
#ifdef __gnu_linux__
#include <sys/ioctl.h>
#include <unistd.h>
// #include <fcntl.h>         /* For O_* constants */
#include <sys/sem.h>
#include <sys/shm.h>
#endif
#include <signal.h>
#include <time.h>

#include "ax_cryptoIpc.h"
#include "sm_printf.h"
#include "sm_types.h"

/*Local Defines*/

/**
 * Initialization for Crypto Library Mutex. This should be invoked as part of App initialization.
 * Note:- In a system only 1 application (and the first one to be launched)  shall invoke this
 * API with parameter AX_CI_TRUE. All other API's invoke with parameter AX_CI_FALSE.
 * Dummy function
 *
 * @param bool - AX_CI_TRUE - set mutex value to 1, AX_CI_FALSE- Do not set Mutex val
 * @return 0 always
 */
int axCi_MutexInit(int setval)
{
    return 0;
}

/**
 * Grab mutex before entering Critical Section
 * Dummy function
 * @return void
 */
void axCi_MutexLock()
{
    return;
}

/**
 * Release mutex and after exit from Critical Section
 * Dummy function
 * @return void
 */
void axCi_MutexUnlock()
{
    return;
}

/**
 * Clean up of IPC resources. API to be invoked at App exit
 * @retval AX_CI_TRUE: OK to exit application
 * @retval AX_CI_FALSE: Application is in Critical section and shall clean up upon out from Critical Section. And then invoke App exit().
 */
int axCi_Close()
{
    return AX_CI_TRUE;
}
