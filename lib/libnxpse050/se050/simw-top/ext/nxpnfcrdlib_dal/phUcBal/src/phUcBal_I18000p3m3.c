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

/** @file
 *
 * phUcBal_Version.c:  <The purpose and scope of this file>
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

/** @ingroup CommandGroupVersion
 * @{
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "phUcBal_I18000p3m3.h"
#include "NxpNfcRdLib_Ver.h"
#include "NNC_uC_VCOM_Ver.h"
#include "phhalHw.h"
#include "phUcBal_Config.h"

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
#ifdef NXPBUILD__PHHAL_HW_RC663
/** Implementation of I18000p3m3 Inventory */
static phStatus_t phUcBal_I18000p3m3_Inventory(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/** Implementation of I18000p3m3 Resume Inventory */
static phStatus_t phUcBal_I18000p3m3_ResumeInventory(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/** Implementation of I18000p3m3 Inventory */
static phStatus_t phUcBal_I18000p3m3_ApplyProtocolSettings(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phUcBal_I18000p3m3(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    switch(Cmd->header.INS)
    {
    PH_UCBAL_CASE(I18000p3m3, Inventory);
    PH_UCBAL_CASE(I18000p3m3, ResumeInventory);
    PH_UCBAL_CASE(I18000p3m3, ApplyProtocolSettings);
    }

    return (PH_ERR_INVALID_PARAMETER | PH_COMP_DRIVER);
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

static phStatus_t phUcBal_I18000p3m3_Inventory(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
	phStatus_t status;
	uint8_t * pRxBuf;
	uint16_t RxBufLen;

	status = phhalHw_I18000p3m3Inventory(
			gpphUcBal_PtrHal,
			(uint8_t *)gpkphUcBal_Config->SelectCommand,
			gpkphUcBal_Config->SelectCommandLength,
			gpkphUcBal_Config->NumValidBitsinLastByte,
			(uint8_t *)gpkphUcBal_Config->BeginRoundCommand,
			gpkphUcBal_Config->TSProcessingStrategy,
			&pRxBuf,
			&RxBufLen
			);
	memcpy(Rsp->header.Response, pRxBuf, RxBufLen);
	Rsp->header.Lr = RxBufLen;
	return status;
}

static phStatus_t phUcBal_I18000p3m3_ResumeInventory(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
	phStatus_t status;
	uint8_t * pRxBuf;
	uint16_t RxBufLen;

	status = phhalHw_I18000p3m3ResumeInventory(
			gpphUcBal_PtrHal,
			&pRxBuf,
			&RxBufLen
			);
	memcpy(Rsp->header.Response, pRxBuf, RxBufLen);
	Rsp->header.Lr = RxBufLen;
	return status;
}

static phStatus_t phUcBal_I18000p3m3_ApplyProtocolSettings(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
	phStatus_t status;
	status = phhalHw_ApplyProtocolSettings(gpphUcBal_PtrHal, PHHAL_HW_CARDTYPE_I18000P3M3);
	return status;
}
#endif /* NXPBUILD__PHHAL_HW_RC663 */

/** @} */

