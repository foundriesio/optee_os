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
* Cards identification.
* $Author: Kiran Kumar Makam Nataraja (nxp69453) $
* $Revision: 4423 $
* $Date: 2016-04-22 11:50:54 +0530 (Fri, 22 Apr 2016) $
*
* History:
*
*
*/

#ifndef PHAPP_INIT_H
#define PHAPP_INIT_H
#define DEBUG 1

// /* Status header */
#include <ph_Status.h>

/* Driver Header */
#include <phDriver.h>

// /* NFCLIB Header */
 #include "phNfcLib.h"

#include "nxLog_App.h"

// /* LLCP header */
// #include <phlnLlcp.h>

// /* SNEP header */
// #include <phnpSnep.h>

/* Check for K82 controller based boards. */
#if defined(PHDRIVER_FRDM_K82FPN5180_BOARD) || defined(PHDRIVER_FRDM_K82FRC663_BOARD)
#define PHDRIVER_KINETIS_K82
#endif

#if defined PHDRIVER_FRDM_K64FRC663_BOARD
#define PHDRIVER_KINETIS_K64F
#endif

/* Check for LPC55S69 based boards */
#ifdef PHDRIVER_LPC55S69RC663_BOARD
#define PHDRIVER_LPC55S69
#endif

/* Check for IMXRT1050 based boards */
#ifdef PHDRIVER_IMXRT1050RC663_BOARD
#define PHDRIVER_IMXRT_1050
#endif

// if defined(PHDRIVER_LPC55S69)
// //#   include <fsl_debug_console.h>
// #   include <fsl_pint.h>
// #endif

// /* Check for LPC1769 controller based boards. */
// #if defined(PHDRIVER_LPC1769PN5180_BOARD) || defined(PHDRIVER_LPC1769RC663_BOARD)
// #define PHDRIVER_LPC1769
// #endif

// #if defined(PHDRIVER_PIPN5180_BOARD) || defined(PHDRIVER_PIRC663_BOARD)
// #include <phDriver_Linux_Int.h>
// #endif

#ifdef PHDRIVER_KINETIS_K82
#   include <fsl_debug_console.h>
#   include <stdio.h>
#endif

#ifdef DEBUG
#ifdef PHDRIVER_KINETIS_K82
#if SDK_DEBUGCONSOLE == 1
#define DEBUG_PRINTF DbgConsole_Printf
#else
#define DEBUG_PRINTF(...) printf(__VA_ARGS__);
#endif
#else /* PHDRIVER_KINETIS_K82 */
#include <stdio.h>
#define DEBUG_PRINTF(...) \
    printf(__VA_ARGS__);  \
    fflush(stdout)
#endif /* PHDRIVER_KINETIS_K82 */
#else  /* DEBUG */
#define DEBUG_PRINTF(...)
#endif /* DEBUG */

/*******************************************************************************
**   Global Variable Declaration
*******************************************************************************/

#define PH_NFCRDLIB_EXAMPLE_LPCD_GUARDTIME \
    100 /* LPCD Guard time(T4) in milli-seconds configured by application for Rc663. */
#define PH_NFCRDLIB_EXAMPLE_LPCD_RFON_TIME \
    56 /* LPCD RFON time(T3) in micro-seconds configured by application for Rc663. */

// /* HAL & BAL declarations */

 extern phbalReg_Type_t sBalParams;

// #ifdef NXPBUILD__PHLN_LLCP_SW
// extern phlnLlcp_Sw_DataParams_t           slnLlcp;            /* LLCP component */
// #endif /* NXPBUILD__PHLN_LLCP_SW */

// #ifdef NXPBUILD__PHHAL_HW_PN5180
// extern phhalHw_Pn5180_DataParams_t   * pHal;
// #endif

// #ifdef NXPBUILD__PHHAL_HW_RC663
 extern phhalHw_Rc663_DataParams_t    * pHal;
// #endif

// #ifdef NXPBUILD__PHHAL_HW_PN7462AU
// extern phhalHw_PN7462AU_DataParams_t * pHal;
// #endif

/**************************************************Prints if error is detected**************************************************************/
/* Enable(1) / Disable(0) printing error/info */
#define DETECT_ERROR 0

#if DETECT_ERROR
#define DEBUG_ERROR_PRINT(x) x
#define PRINT_INFO(...) DEBUG_PRINTF(__VA_ARGS__)
#else
#define DEBUG_ERROR_PRINT(x)
#define PRINT_INFO(...)
#endif

#define CHECK_STATUS(x)                                                       \
    if ((x) != PH_ERR_SUCCESS) {                                              \
        LOG_D(                                                         \
            "Line: %d   Error - (0x%04X) has occurred : 0xCCEE CC-Component " \
            "ID, EE-Error code. Refer-ph_Status.h\n",                         \
            __LINE__,                                                         \
            (x));                                                             \
    }

/* prints if error is detected */
#define CHECK_SUCCESS(x)                                            \
    if ((x) != PH_ERR_SUCCESS) {                                    \
        LOG_D(                                               \
            "\nLine: %d   Error - (0x%04X) has occurred : 0xCCEE "  \
            "CC-Component ID, EE-Error code. Refer-ph_Status.h\n ", \
            __LINE__,                                               \
            (x));                                                   \
        return (x);                                                 \
    }

/* prints if error is detected */
#define CHECK_NFCLIB_STATUS(x)                                         \
    if ((x) != PH_NFCLIB_STATUS_SUCCESS) {                             \
        LOG_D(                                                  \
            "\nLine: %d   Error - (0x%04X) has occurred in NFCLIB\n ", \
            __LINE__,                                                  \
            (x));                                                      \
    }

/*********************************************************************************************************************************************/

/*******************************************************************************
**   Function Declarations
*******************************************************************************/
void phEx10_PrintErrorInfo(phStatus_t wStatus);
extern phStatus_t phApp_Configure_IRQ();
#endif /* PHAPP_INIT_H */
