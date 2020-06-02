/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef KSDK_MBEDCRYPTO_HW_CONFIG_H
#define KSDK_MBEDCRYPTO_HW_CONFIG_H


/* Application-specific paltform macros */
#if !defined(MBEDTLS_PLATFORM_EXIT_MACRO)
#define MBEDTLS_PLATFORM_EXIT_MACRO(x)      do{}while(1)    /* Default exit macro to use, can be undefined */
#endif

#if !defined(MBEDTLS_PLATFORM_PRINTF_MACRO)
#include "fsl_debug_console.h"
#define MBEDTLS_PLATFORM_PRINTF_MACRO       PRINTF          /* Default printf macro to use, can be undefined */
#endif

#endif /* KSDK_MBEDCRYPTO_HW_CONFIG_H */
