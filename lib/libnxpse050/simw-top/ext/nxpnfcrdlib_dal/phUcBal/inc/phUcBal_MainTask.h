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
 * phUcBal_MainTask.h:  Main task managemnt
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

#ifndef PHUCBAL_MAINTASK_H
#define PHUCBAL_MAINTASK_H

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "phUcBal.h"



/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/**
 *
 * @ingroup RTOSDefaults
 *
 * Constants/defaults used for RTOS Task Creation
 *
 * @{
 */
/** RTOS Task priority, above normal */
#define PH_UCBAL_MAINTASK_PRIO      2
/** Memory provided to stack */
#define PH_UCBAL_MAINTASK_STACK     (400)

/** @} */

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
 * @ingroup ExecFlow
 * Main entry function of the example */

/** For testing, perform RF On/Off immediately on Boot Up.
 *
 * A kind of litmus test for the MicroController FW bootup.
 *
 * If you see the RF On Off happening on boot up, then the FW on @ref uchost
 * has booted up and also the communication between @ref uchost and @ref dut
 * is working fine as expected.
 */

#define PH_UCBAL_MAINTASK_PERFORM_RFONOFF_ON_BOOTUP 1

void phUcBal_MainTask(
    void * param);

#endif /* PHUCBAL_MAINTASK_H */
