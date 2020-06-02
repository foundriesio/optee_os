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
 * phUcBal_SECFw_LPC1769.c:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
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
#include "phDriver.h"
#include "phUcBal_MainTask.h"
#include "chip.h"
#include <phDriver.h>

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/** @ingroup CommandGroupSECFwUpgrade
 * @{
 */

/** MAGIC Register used between Secondary Firmware and Boot Loader to
 * request upload of secondary Firmware without going to Flash Erase/program cycle.
 *
 */

#define LPC1769_GPREG0              (LPC_REGFILE->REGFILE[0])
/** MAGIC number for @ref LPC1769_GPREG0 */
#define USER_APP_LEN_MAGIC           0x76706B64U

/** @} */

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
static void timerCBPerformNVICReset();

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/** @ingroup CommandGroupSECFwUpgrade
 * @{ */

/** Enter the Secondary FW Upgrade Mode.
 *
 * @warning this API would reboot the system back to the phNncBootloader.
 *
 * See ``phNncBootloader`` to understand the design and architecture of the
 * boot loader and the interaction between the bootloader and the secondary
 * FW Application.
 */

phStatus_t phUcBal_SECFw_EnterFWUpgradeMode(
    const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    LPC1769_GPREG0 = USER_APP_LEN_MAGIC;
    phDriver_TimerStart(PH_DRIVER_TIMER_MILLI_SECS, 500, &timerCBPerformNVICReset);
    return PH_ERR_SUCCESS;
}

phStatus_t phUcBal_SECFw_CanUpgrade(
    const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    unsigned int *pSCB_VTOR = (unsigned int *) 0xE000ED08;
    if ( *pSCB_VTOR == 0x4000)
        Rsp->header.Response[0] = 1;
    else
        Rsp->header.Response[0] = 0;

    Rsp->header.Lr += 1;
	return PH_ERR_SUCCESS;
}

/**
 * Callback to reboot this system.
 *
 * When we want to enter Secondary FW Upgrade mode, we have to reboot back
 * to the phNncBootloader.  But when we go back to phNncBootloader, after the
 * reset cycle, the VCOM port connection between @ref pchost and @ref uchost
 * would no longer be valid.  So we fire up a timer before rebooting ourselves.
 * This will allow the @ref pchost to close the USB VCOM Port before we have
 * actually rebooted the @ref uchost. */
static void timerCBPerformNVICReset()
{
    NVIC_SystemReset();

    while(1) {
        __WFI();
    }
}


/** @} */

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
