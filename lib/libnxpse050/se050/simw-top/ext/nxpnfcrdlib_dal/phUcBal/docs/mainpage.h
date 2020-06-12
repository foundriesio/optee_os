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

/**
 * @mainpage VCOM Based FW for the Micro Controller Host
 *
 * This document describes implementation of VCOM Based Firmware to be used
 * along with NxpNfcCockpit.
 *
 * @section block Block Level Overview
 *
 *  At a very high level, the system is divided into three parts.
 *
 * -# @ref pchost
 * -# @ref uchost
 * -# @ref dut
 *
 * @image html  vcom_blockdia.png   "VCOM Based Firmware Block Diagram"  width=8cm
 * @image latex vcom_blockdia.eps   "VCOM Based Firmware Block Diagram"  width=8cm
 *
 *
 * @subsection pchost  PC Host
 *
 * The PC on which the GUI is running.
 *
 * The @ref pchost is connected to @ref uchost via a USB Serial VCOM interface.
 * Where applicable, the @ref pchost may be connected to @ref uchost over a plain RS232 Serial Interface too.
 *
 * @subsection uchost Micro Controller Host
 *
 * The application running on the Micro Controller host is minimalistic.  It does not try to be overly complex or
 * super intelligent.
 *
 * It primary and core purpose is to only behave as a translator for USB/Serial to SPI/I2C communication converter.
 * And also abstract to set/get values of the GPIOs.  For this, it contains a small and simple dispatcher to process various @ref CommandGroups .
 *
 * @subsection dut Device Under Test (DUT)
 *
 * The device under test can be either the PN5180 IC or the RC663 IC.
 *
 * @section breakup Document Breakup
 *
 * This document covers:
 *
 * -# @subpage protocol-overview
 * -# The @ref ExecFlow of the example
 * -# @ref CommandGroups
 *      -# @ref CommandGroupTransReceive
 *      -# @ref CommandGroupGPIO
 *      -# @ref CommandGroupConfig
 *      -# @ref CommandGroupSECFw
 *      -# @ref CommandGroupLoopBack
 * -# @subpage porting
 *
 */
