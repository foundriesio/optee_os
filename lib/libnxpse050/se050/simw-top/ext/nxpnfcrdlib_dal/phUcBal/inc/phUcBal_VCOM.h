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
 * phUcBal_VCOM.h:  APIs for communication between PC and Micro Controller
 *
 * Project:  NXP NFC Cockpit
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

#ifndef PHUCBAL_VCOM_H
#define PHUCBAL_VCOM_H

/** @ingroup vcom
 * @{
 */
/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "phUcBal.h"
/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

/**
 * Initialize connection between @ref pchost and @ref uchost
 */
void phUcBal_VCOM_Init(void);

/**
 * Is @ref pchost and @ref uchost connected?
 *
 * @return Connection status.
 *
 * @retval 0 Not connected
 * @retval 1 Connected
 */
uint32_t phUcBal_VCOM_IsConnected(void);

/**
 * Send pRxBuffer from @ref uchost to @ref pchost
 *
 * @param pRxBuffer Data from @ref uchost to @ref pchost
 * @param dwLen Length of data
 * @return Number of bytes written
 */
uint32_t phUcBal_VCOM_Write(uint8_t *pRxBuffer, uint32_t dwLen);


/**
 * Get data from @ref pchost into @ref uchost
 *
 * @param pRxBuffer Data from @ref pchost into @ref uchost
 * @param dwBufferSize Maximum bytes that we can read
 * @return Number of bytes read
 */

uint32_t phUcBal_VCOM_Read(uint8_t *pRxBuffer, uint32_t dwBufferSize);

/** @} */

#endif /* PHUCBAL_VCOM_H */
