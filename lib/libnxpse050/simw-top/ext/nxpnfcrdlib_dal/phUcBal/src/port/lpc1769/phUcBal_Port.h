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


#ifndef SRC_PORT_LPC1769_PHUCBAL_PORT_H_
#define SRC_PORT_LPC1769_PHUCBAL_PORT_H_

#include <phDriver.h>

#if defined(CORE_M3) && defined(__USE_LPCOPEN)
#	include <chip.h>
#	include <core_cm3.h>
#	include "cmsis.h"
#endif

/*
 *
 * GREEN  - LED200 - LED_G --- 27 --- P3.25/MAT0.0/PWM1.2
 * BLUE   - LED201 - LED_B --- 26 --- P3.26/STCLK/PWM1.3
 * RED    - LED202- LED_O --- 75 --- P2.0/PWM1.1/TXD1
 * ORANGE - LED203- LED_R --- 56 --- P0.22/RTS1/TD1
 */

#define LED_INIT()\
	Chip_GPIO_WriteDirBit(LPC_GPIO, 3, 25, true);	\
	Chip_GPIO_WriteDirBit(LPC_GPIO, 3, 26, true);	\
	Chip_GPIO_WriteDirBit(LPC_GPIO, 2, 0, true);	\
	Chip_GPIO_WriteDirBit(LPC_GPIO, 0, 22, true);	\
	LED_GREEN(0);	\
	LED_BLUE(0);	\
	LED_RED(0);	\
	LED_ORANGE(0);	\


#define LED_GREEN(On) \
	Chip_GPIO_WritePortBit(LPC_GPIO, 3, 25, On)

#define LED_BLUE(On) \
	Chip_GPIO_WritePortBit(LPC_GPIO, 3, 26, On)

#define LED_RED(On) \
	Chip_GPIO_WritePortBit(LPC_GPIO, 2, 0, On)

#define LED_ORANGE(On) \
	Chip_GPIO_WritePortBit(LPC_GPIO, 0, 22, On)

#endif /* SRC_PORT_LPC1769_PHUCBAL_PORT_H_ */
