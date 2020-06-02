/* Copyright 2018,2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include <fsl_sss_api.h>
#include <fsl_sss_se05x_types.h>
#include <stdio.h>

//#if defined(__GNUC__)
//static const void * keep_symbols[] __attribute__((used)) = {
//  &Se05x_API_DeleteAll_Iterative,
//  NULL,
//};
//#endif

#ifdef _WIN32

#include <windows.h>

BOOL APIENTRY DllMain(HANDLE hModule, // Handle to DLL module
    DWORD ul_reason_for_call,
    LPVOID lpReserved) // Reserved
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        // A process is loading the DLL.
        break;

    case DLL_THREAD_ATTACH:
        // A process is creating a new thread.
        break;

    case DLL_THREAD_DETACH:
        // A thread exits normally.
        break;

    case DLL_PROCESS_DETACH:
        // A process unloads the DLL.
        break;
    }
    return TRUE;
}

#endif /* _WIN32 */
