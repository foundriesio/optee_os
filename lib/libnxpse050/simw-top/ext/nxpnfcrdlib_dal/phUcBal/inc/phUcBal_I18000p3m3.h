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
 * phUcBal_I18000p3m3.h:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 24-Jan-2017 $
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

#ifndef PHUCBAL_I18000P3M3_H
#define PHUCBAL_I18000P3M3_H

/** @ingroup CommandGroup18000p3m3
 * @{
 */

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "phUcBal.h"
#include "phUcBal_Protocol.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/** Instructions for 18000p3m3 */
enum phUcBal_18000p3m3_INS {
    /** [\b 0x01] Instruction to do 'I'nventory and retrieve UII response */
	I18000p3m3_INS_Inventory   = 0x01,
    /** [\b 0xE1] Instruction to perform R'E'sume'I'nventory and retrieve UII response */
	I18000p3m3_INS_ResumeInventory   = 0xE1,
    /** [\b 0x0A] Instruction to do ApplyProtocolSettings for I18000p3m3*/
	I18000p3m3_INS_ApplyProtocolSettings   = 0x0A,
};

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

/** @copybrief CommandGroupVersion */
phStatus_t phUcBal_I18000p3m3(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/** @} */

#endif /* PHUCBAL_I18000P3M3_H */
