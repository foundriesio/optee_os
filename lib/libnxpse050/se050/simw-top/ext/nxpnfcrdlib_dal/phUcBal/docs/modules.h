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

/** @defgroup groupProtocol  Protocol
 *
 * Protocol between PC and Micro-controller.
 *
 * See @ref protocol-overview for an overview of structure of command frame between PC and the Micro Controller Host.
 *
 */

/** @defgroup CommandGroups     Command Groups
 *
 * Group of commands between PC and the Micro-Controller
 */

/** @ingroup CommandGroups
 *      @defgroup CommandGroupTransReceive      Trans Receive
 *
 * Send(Transmit) and receive commands/data between Micro-Controller and the NFC Device.
 *
 * The APIs/Commands in this module itself have no notion/idea about the connection
 * between @ref uchost and the @ref dut.  On the contrary, it depends on the
 * ``phPlatform_Init()`` and relevant porting.
 *
 */
/** @ingroup CommandGroups
 *      @defgroup CommandGroupGPIO              GPIO Control
 *
 * Manage GPIOs of @ref uchost between the @ref uchost and @ref dut.
 *
 * The APIs/Commands in this module internally depend on ``phPlatform`` to check the values
 * of the GPIOs and set/get their values.  During ``phPlatform_Init()`` the relevant
 * GPIOs should already have been initialized and set as expected.
 *
 */

/** @ingroup CommandGroups
 *      @defgroup CommandGroupConfig            Configuration
 *
 * Manage runtime configurations.
 *
 * For example, Depending on the state of PN5180 (@ref dut) and it's (Normal mode vs Secure FW Upgrade mode)
 * the IRQ pin handling would change between the @ref uchost and @ref dut.
 *
 * This component ensures that such handling fur the current case (and more complex future scenarios)
 * is managed in a specific module.
 */

/** @ingroup CommandGroups
 *      @defgroup CommandGroupVersion            Version
 *
 * Version information of the running firmware.
 *
 * This allows the PC Application to fetch version information of the pre-compiled binary
 * running on the @ref uchost at run time.
 *
 */
/** @ingroup CommandGroups
 *      @defgroup CommandGroupSECFw             Secondary Firmware
 *
 * Secondary Firmware management/control of the @ref uchost.
 *
 * Within the @ref uchost, where ever applicable, Secondary Applications
 * like EMVCo loop Back, etc. can be implemented.   In the current implementation,
 * these secondary applications are treated as RTOS Tasks and can be started
 * and stopped from the GUI.
 *
 * At a time, only one secondary Application is allowed to be run.
 *
 * @msc
 *      GUI, ucHost;
 *
 *      GUI => ucHost [label = "phUcBal_SECFw_GetTaskCount()", URL="\ref phUcBal_SECFw_GetTaskCount" ];
 *      GUI <<= ucHost [label = "returns the number of secondary tasks"];
 *      GUI => ucHost [label = "phUcBal_SECFw_GetTaskName(0)", URL="\ref phUcBal_SECFw_GetTaskName"];
 *      GUI <<= ucHost [label = "returns the name of task '0'"];
 *      --- [ label = "Go on for more tasks." ];
 *      --- [ label = "Update GUI with the names of tasks." ];
 *      GUI => ucHost [label = "phUcBal_SECFw_StartAppTask(n)", URL="\ref phUcBal_SECFw_StartAppTask"];
 *      --- [ label = "Task number 'n' would be running as\na new RTOS task"];
 *      GUI => ucHost [label = "phUcBal_SECFw_StopAppTask(n)", URL="\ref phUcBal_SECFw_StopAppTask"];
 *      --- [ label = "Last running task would stop"];
 *      |||;
 *
 * @endmsc
 *
 */

/** @ingroup CommandGroupSECFw
 *      @defgroup CommandGroupSECFwUpgrade  Secondary Firmware Upgrade
 *
 * For full design, see phNncBootLoader.
 *
 * @note Entering Secondary FW Upgrade Mode and allowing over-write of Secondary FW
 * from the PC is an optional feature.   An application can be built that does not
 * support Secondary FW Upgrade, but still such an application can be downloaded on
 * @ref uchost via relevant debugger/programmer,  and secondary tasks can be
 * triggered/stopped from the GUI.
 */

/** @ingroup CommandGroups
 *      @defgroup CommandGroupLoopBack             Loop Back (For Testing)
 *
 * Commands that can be used to test Loop-back between the @ref pchost and @ref uchost.
 */

/** @defgroup GRdLibStack       Globals for NxpNfcRdLib Stack
 *
 *  Reader Library Stack Management
 */
/** @defgroup RTOSDefaults      Defaults for RTOS
 *
 *  Defaults used for RTOS in this example
 */

/** @defgroup ExecFlow          Execution Flow
 *
 *  Execution Flow of this Example
 */

/** @defgroup vcom          VCOM Interface
 *
 *  Separate VCOM interface block for porting between @ref pchost and @ref uchost.
 *
 */
