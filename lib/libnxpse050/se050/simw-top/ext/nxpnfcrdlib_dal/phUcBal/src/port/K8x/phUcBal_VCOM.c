/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2016
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
 * $Date: 2016-10-26 14:37:40 +0530 (Wed, 26 Oct 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 6421 $
 */


/*
 *  _   ___   _______    _   _ ______ _____    _____           _          _ _
 * | \ | \ \ / /  __ \  | \ | |  ____/ ____|  / ____|         | |        (_) |
 * |  \| |\ V /| |__) | |  \| | |__ | |      | |     ___   ___| | ___ __  _| |_
 * | . ` | > < |  ___/  | . ` |  __|| |      | |    / _ \ / __| |/ / '_ \| | __|
 * | |\  |/ . \| |      | |\  | |   | |____  | |___| (_) | (__|   <| |_) | | |_
 * |_| \_/_/ \_\_|      |_| \_|_|    \_____|  \_____\___/ \___|_|\_\ .__/|_|\__|
 *                                                                 | |
 *                                                                 |_|        */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "phUcBal.h"
#include <phUcBal_Protocol.h>
#include "fsl_common.h"
#include "comm_if_module.h"
#include "BoardSelection.h"
#include "fsl_pit.h"
#include "fsl_edma.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_mpu.h"
#include "fsl_lmem_cache.h"
#include "cardtek_l2_hal.h"
#include "fsl_debug_console.h"
#include "phDriver.h"

/** @ingroup vcom
 * @{
 */

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

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

static void vcommif_callback(commif_flags_t flags)
{
    /* NOTE: This callback is executed from within ISR context!!! */
    switch(flags)
    {
        case kCOMMIF_EventFlag_LinkStatus:
            if(COMMIF_IsLinkUp())
            {
                /* nothing to be done so far... */
            }
            else
            {
                /* nothing to be done so far... */
            }
            break;

        case kCOMMIF_EventFlag_ApplicationStatus:
            if(COMMIF_IsAppConnected())
            {
                /* nothing to be done so far... */
            }
            else
            {
                /* nothing to be done so far... */
            }
            break;

        default:
            break;
    }
}

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

void phCpu_Init(void)
{
    pit_config_t pitConfig;
    edma_config_t edmaConfig;

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();

    /* To support Debug PRINTF */
    BOARD_InitDebugConsole();

    /* Initialize the eDMA. */
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(DMA0, &edmaConfig);

    /* MPU is used by the USB stack. */
    MPU_Enable(MPU, 0);

    /* KSDK Workaround  for Initialize the Cache. */
    /* First, invalidate the entire cache. */
    LMEM_SystemCacheInvalidateAll(LMEM);

    /* Now enable the cache. */
    LMEM->PSCCR |= LMEM_PSCCR_ENCACHE_MASK ;

    /* pitConfig.enableRunInDebug = false */
    PIT_GetDefaultConfig(&pitConfig);
    /* Init pit module */
    PIT_Init(PIT, &pitConfig);

    HAL_Init();
}

void phUcBal_VCOM_Init()
{

	if(COMMIF_Init(kCOMMIF_Type_UsbCdc, vcommif_callback) != kStatus_COMMIF_Success)
	{
		/* Hang here. */
		while(1);
	}
}

uint32_t phUcBal_VCOM_IsConnected(void)
{
	return (uint32_t)COMMIF_IsAppConnected();
}

uint32_t phUcBal_VCOM_Write(uint8_t *pTxBuffer, uint32_t dwLen)
{
	if(kStatus_COMMIF_Success != COMMIF_SendData(pTxBuffer, dwLen))
	{
		return 0;
	}

	return dwLen;
}

uint32_t phUcBal_VCOM_Read(uint8_t *pRxBuffer, uint32_t dwBufferSize)
{
	uint32_t rxLength = 0;

	/* Receive data. */
	if(kStatus_COMMIF_Success != COMMIF_ReceiveData(pRxBuffer, &rxLength, 0xFFFFFFFFUL))
	{
		return 0;
	}

	return rxLength;
}

void phUcBal_PN5180Irq_DisableIRQ()
{
     NVIC_DisableIRQ(EINT_IRQn);
}


/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

/** @} */
