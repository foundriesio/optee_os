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
 * phUcBal_SECFw_K8x.c: K8x specific port for Secure FW.
 *
 * $Date: 03-Dec-2016 $
 * $Author: ing05193 $
 * $Revision$
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
#include "phUcBal_SECFw.h"

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

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/** @ingroup CommandGroupSECFwUpgrade
 * @{ */

phStatus_t phUcBal_SECFw_EnterFWUpgradeMode(
    const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    return PH_ERR_USE_CONDITION;
}

phStatus_t phUcBal_SECFw_CanUpgrade(
    const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
	/* No. We dont' support it for K8x */
	Rsp->header.Response[0] = 0;
    Rsp->header.Lr += 1;
	return PH_ERR_SUCCESS;
}

/** @} */

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
