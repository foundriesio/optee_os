/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2017
 *
 *         All rights are reserved. Reproduction in whole or in part is
 *        prohibited without the written consent of the copyright owner.
 *    NXP reserves the right to make changes without notice at any time.
 *   NXP makes no warranty, expressed, implied or statutory, including but
 *   not limited to any implied warranty of merchantability or fitness for any
 *  particular purpose, or that the use will not infringe any third party patent,
 *   copyright or trademark. NXP must not be liable for any loss or damage
 *                            arising from its use.
 */

/* @file
 *
 * phUcBal_VCOM.c: VCOM Interface abstraction
 *
 * Project:  NXP NFC Cockpit
 *
 */
/*
 *  _   ___   _______    _   _ ______ _____    _____           _          _ _
 * | \ | \ \ / /  __ \  | \ | |  ____/ ____|  / ____|         | |        (_) |
 * |  \| |\ V /| |__) | |  \| | |__ | |      | |     ___   ___| | ___ __  _| |_
 * | . ` | > < |  ___/  | . ` |  __|| |      | |    / _ \ / __| |/ / '_ \| | __|
 * | |\  |/ . \| |      | |\  | |   | |____  | |___| (_) | (__|   <| |_) | | |_
 * |_| \_/_/ \_\_|      |_| \_|_|    \_____|  \_____\___/ \___|_|\_\ .__/|_|\__|
 *                                                                 | |
 *                                                                 |_|
 * $Revision$
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "phUcBal.h"
#include <usbd_lib_cdc.h>
#include <phUcBal_Protocol.h>


/** @ingroup vcom
 * @{
 */

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/* Helper macro to trace */
#define TRACE_COMMUNICATION 0

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

#if TRACE_COMMUNICATION
#	include <stdio.h>
#endif

void phUcBal_VCOM_Init()
{
    usbd_lib_cdcInit();
}

uint32_t phUcBal_VCOM_IsConnected(void)
{
    return usbd_lib_cdcConnected();
}

uint32_t phUcBal_VCOM_Write(uint8_t *pTxBuffer, uint32_t dwLen)
{
#if TRACE_COMMUNICATION
    if ( pTxBuffer[0] != CLA_GPIO) {
        printf("<\t");
        for ( int i = 0 ; i < dwLen; i++) {
            printf("%02X ", pTxBuffer[i]);
        }
        printf("\n");
    }
#endif
    return usbd_lib_cdcWrite(pTxBuffer, dwLen);
}

uint32_t phUcBal_VCOM_Read(uint8_t *pRxBuffer, uint32_t dwBufferSize)
{
    uint32_t retValue = usbd_lib_cdcRead(pRxBuffer, dwBufferSize);
#if TRACE_COMMUNICATION
    if ( retValue != 0 && pRxBuffer[0] != CLA_GPIO) {
        printf(">\t");
        for ( int i = 0 ; i < retValue; i++) {
            printf("%02X ", pRxBuffer[i]);
        }
        printf("\n");
    }
#endif
    return retValue;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

/** @} */
