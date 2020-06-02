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

/** @page porting Porting Guideline
 *
 * This part of the document gives an overview on the steps needed to use/port this application on different platforms
 * other than the default LPC1769.
 *
 * @section preconditions Pre-Conditions
 *
 * @subsection rdlibporting phPlatform / OSAL / BAL Porting
 *
 * It is mandatory that phPlatform and relevant OSAL and BAL porting has already been performed onto the target
 * platform.
 *
 * Apart from the process of receiving @ref protocolcmd from the @ref pchost and sending @ref protocolrsp back to the
 * @ref pchost, this application directly uses the ``phPlatform`` porting layer of NxpNfcRdLib to perform low level
 * operations.
 *
 * @subsection vcomporting VCOM/RS232 Serial Interface
 *
 * It is mandatory that the @ref uchost has some method/approach to expose either a USB VCOM CDC Class interface to
 * the @ref pchost, or at the minimum a serial port interface.
 *
 * @section ucdut Communication between Micro Controller Host and DUT
 *
 * If the @ref preconditions of @ref rdlibporting has already been met, and reference examples provided with
 * NxpNfcRdLib are already running on the target platform, the communication between @ref uchost and @ref dut should
 * already be fully functional.  Nothing special should be required for porting from the GUI.
 *
 * @section pcuc Communication between PC Host and Micro Controller Host
 *
 * The way VCOM/RS232 is implemented is specific to the micro controller and not explained in this document.
 * The end user is expected to port APIs of the group as listed in @ref vcom Group.
 *
 * To verify whether such porting for @ref pcuc is complete, @ref CommandGroupLoopBack are alread implemented
 * inside this application.  A separate test application to be run on PC is provided with NXP NFC Cockpit : @ref UcBalPCTestApp
 *
 * @section UcBalPCTestApp Micro Controller BAL - PC Test Application
 *
 * ``UcBalPCTestApp.exe`` is a stand alone application implemented to verify the porting of @ref pcuc.
 * As of writing of this document, the following commands are available for testing.
 *
 * |            Command | Description                                                          |
 * |--------------------|----------------------------------------------------------------------|
 * |                    | **General Commands**                                                 |
 * |             /Usage | Prints the Usage                                                     |
 * |              /help | Same as /Usage                                                       |
 * |                    | **GPIOs**                                                            |
 * |           /GetBusy | Get Value of Busy PIN                                                |
 * |            /GetIRQ | Get Value of IRQ PIN                                                 |
 * |          /SetDWL=1 | Set Download Pin to 1                                                |
 * |          /SetDWL=0 | Set Download Pin to 0                                                |
 * |                    | **Tx/Rx**                                                            |
 * |              /Echo | Send a Dummy frame from PC to ucHost and check if it is Echoed back  |
 * |       /TxAscending | Send a Dummy frame from PC to ucHost, in ascending order             |
 * |       /RxAscending | Get a Dummy frame from ucHost to PC, in ascending order              |
 * |      /RxDescending | Receive a Dummy frame from ucHost to PC, in descending order         |
 * |                    | **Secondary FW**                                                     |
 * |      /GetTaskCount | Get Secondary FW Task Count                                          |
 * |      /GetTaskNames | Get Secondary FW Task Names                                          |
 * |       /StartTask=0 | Start Task[0], if present                                            |
 * |       /StartTask=1 | Start Task[1], if present                                            |
 * |          /StopTask | Stop Task, if running                                                |
 *
 * @note  The above table may not be up-to-date.
 *        Running ``UcBalPCTestApp.exe`` (without any parameters) or ``UcBalPCTestApp.exe /Help``,
 *        will print the latest implemented Commands and description by the version of
 *        ``UcBalPCTestApp.exe`` supplied in this package.
 *
 * ``UcBalPCTestApp.exe`` is written to incrementally confirm and check what is working and what is
 * not working between the @ref pchost and @ref uchost, without running the complete GUI.
 *
 * @section securefwupgrade Secondary FW Upgrade
 *
 * Performing Secondary FW Upgrade, IAP (In Application Programming), build system changes to support required memory
 * map, strategy for the bootloader to ensure validity of the secondary application during upgrade mode, hard/soft
 * reboot post download, vector re-mapping in case of moving from primary to secondary application, etc. are advanced
 * topics in themselves and hence not covered in this document.  The design of @ref CommandGroupSECFwUpgrade is not
 * mandatory for actually running @ref CommandGroupSECFw tasks from the GUI.  The implementation and architecture of
 * @ref CommandGroupSECFwUpgrade is shared in phNncBootloader and is out of scope of this document.
 *
 */
