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



#include <ph_NxpBuild.h>



#if defined(NXPBUILD__PHHAL_HW_RC663)
#   define USB_CDC_idVendor         0x1FC9U
#   define USB_CDC_idProduct        0x012AU
#   define USB_CDC_bcdDevice        0xD663U
#elif defined(NXPBUILD__PHHAL_HW_PN5180)
#   define USB_CDC_idVendor         0x1FC9U
#   define USB_CDC_idProduct        0x012AU
#   define USB_CDC_bcdDevice        0xD180U
#endif

#include "../../usbd_lib_cdc/appusbd/app_usbd_cfg.h"
