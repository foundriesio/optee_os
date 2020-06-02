/*
 *         Copyright (c), NXP Semiconductors Bangalore / India
 *
 *                     (C)NXP Semiconductors
 *       All rights are reserved. Reproduction in whole or in part is
 *      prohibited without the written consent of the copyright owner.
 *  NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 *particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
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

#ifndef PHUCBAL_H
#define PHUCBAL_H

#include "ph_Status.h"
#include <phOsal.h>
#include <phhalHw.h>
#include <phUcBal_Port.h>

#define PH_UCBAL_CASE(CLASS, INSTRUCTION)    \
    case CLASS ## _INS_ ## INSTRUCTION: \
        return phUcBal_ ## CLASS ## _ ## INSTRUCTION(Cmd,Rsp)


/**
 * @ingroup GRdLibStack
 * @{
 */
#define PH_UCBAL_HAL_BUFSIZE        (300)

/** @} */


#define ASSERT_SUCCESS(x)     \
  if ((x) != PH_ERR_SUCCESS)  \
    {                         \
      while (1);              \
    }

#ifndef __DISABLE_IRQ
#	define __DISABLE_IRQ __disable_irq
#endif

/**
 * @ingroup GRdLibStack
 *
 * @{
 */

/** Pointer to phbalReg_Type_t BAL for given IC */
extern phbalReg_Type_t * gpphUcBal_PtrBal;

#ifndef NXPBUILD__PH_PLATFORM_DELEGATE

/** HAL Pointer for the IC/Frontend */
#ifdef NXPBUILD__PHHAL_HW_PN5180
extern phhalHw_Pn5180_DataParams_t * gpphUcBal_PtrHal;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */
#ifdef NXPBUILD__PHHAL_HW_RC663
extern phhalHw_Rc663_DataParams_t * gpphUcBal_PtrHal;
#endif /* NXPBUILD__PHHAL_HW_RC663 */

#endif /* NXPBUILD__PH_PLATFORM_DELEGATE */

/** @} */

#endif /* PHUCBAL_H */
