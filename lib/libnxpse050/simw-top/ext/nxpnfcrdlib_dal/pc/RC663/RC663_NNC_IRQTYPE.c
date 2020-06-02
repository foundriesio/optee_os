/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
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

/** \file
* Command functions for Rc663 specific HAL-Component of Reader Library Framework.
* $Author: Purnank G (ing05193) $
* $Revision: 5076 $
* $Date: 2016-06-13 17:29:09 +0530 (Mon, 13 Jun 2016) $
*
* History:
*  CHu: Generated 19. March 2010
*
*/
#include <phhalHw.h>
#include <phDriver.h>

#ifdef NXPBUILD__PHHAL_HW_RC663

uint8_t gbPin_Irq_Trigger_Type = PH_DRIVER_INTERRUPT_FALLINGEDGE;

/* NNC implementation to set Irq type using the global variable */
phStatus_t phhalHw_Rc663_Cmd_SetIrqType(
                                   phhalHw_Rc663_DataParams_t * pDataParams,
                                   uint8_t bIntConfig
                                   )
{
    gbPin_Irq_Trigger_Type = bIntConfig;
    return PH_ERR_SUCCESS;
}

#endif //NXPBUILD__PHHAL_HW_RC663