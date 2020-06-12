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
 * phUcBal_Version.c:  <The purpose and scope of this file>
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

/** @ingroup CommandGroupVersion
 * @{
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "phUcBal_Version.h"
#include "NxpNfcRdLib_Ver.h"
#include "NNC_uC_VCOM_Ver.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/** Helper macro for the case when we are returning a number */
#define PH_UCBAL_VERSION_CASE_U32(CLASS, INSTRUCTION, U32Value)    \
    case CLASS ## _INS_ ## INSTRUCTION: \
        return phUcBal_ ## CLASS ## _U32(Cmd,Rsp,U32Value)

/** Helper macro for the case when we are returning a string */
#define PH_UCBAL_VERSION_CASE_STR(CLASS, INSTRUCTION, strValue)    \
    case CLASS ## _INS_ ## INSTRUCTION: \
        return phUcBal_ ## CLASS ## _Str(Cmd,Rsp,strValue)

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

/** Implementation to return an integer */
static phStatus_t phUcBal_Version_U32(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp, uint32_t u32Version);
/** Implementation to return a string */
static phStatus_t phUcBal_Version_Str(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp, const char * strInformation);
/** Implementation to return the type of FrontEnd for which this FW is compiled */
static phStatus_t phUcBal_Version_FrontEnd(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phUcBal_Version(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp)
{
    switch(Cmd->header.INS)
    {
    PH_UCBAL_VERSION_CASE_U32(Version, RD_Major, NxpNfcRdLib_VER_MAJOR);
    PH_UCBAL_VERSION_CASE_U32(Version, RD_Minor, NxpNfcRdLib_VER_MINOR);
    PH_UCBAL_VERSION_CASE_U32(Version, RD_Dev, NxpNfcRdLib_VER_DEV);
    PH_UCBAL_VERSION_CASE_STR(Version, RD_String, NxpNfcRdLib_VER_FILEDESCRIPTION);

    PH_UCBAL_VERSION_CASE_U32(Version, uC_Major, NNC_uC_VCOM_VER_MAJOR);
    PH_UCBAL_VERSION_CASE_U32(Version, uC_Minor, NNC_uC_VCOM_VER_MINOR);
    PH_UCBAL_VERSION_CASE_U32(Version, uC_Dev, NNC_uC_VCOM_VER_DEV);
    PH_UCBAL_VERSION_CASE_STR(Version, uC_String, NNC_uC_VCOM_VER_FILEDESCRIPTION);
    PH_UCBAL_VERSION_CASE_STR(Version, uC_DateTime, __DATE__ " " __TIME__ );

    PH_UCBAL_CASE(Version, FrontEnd);
    }

    return (PH_ERR_INVALID_PARAMETER | PH_COMP_DRIVER);
}

union u32_buffer {
    uint32_t u32;
    char buffer[4];
};

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

static phStatus_t phUcBal_Version_U32(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp, uint32_t u32Version) {
    union u32_buffer value;
    value.u32 = u32Version;
    Rsp->header.Lr = sizeof(value.buffer);
    memcpy(Rsp->header.Response, value.buffer, sizeof(value.buffer));
    return PH_ERR_SUCCESS;
}

static phStatus_t phUcBal_Version_Str(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp, const char * theString) {
    Rsp->header.Lr = strlen(theString) + 1;
    memcpy(Rsp->header.Response, theString, Rsp->header.Lr);
    return PH_ERR_SUCCESS;
}

static phStatus_t phUcBal_Version_FrontEnd(const phUcBal_Cmd_t * Cmd, phUcBal_Rsp_t * Rsp) {
#ifdef NXPBUILD__PHHAL_HW_PN5180
    static const char szFrontEnd[] = "PN5180";
#endif
#ifdef NXPBUILD__PHHAL_HW_RC663
    static const char szFrontEnd[] = "CLRC663";
#endif
    Rsp->header.Lr = sizeof(szFrontEnd);
    memcpy(Rsp->header.Response, szFrontEnd, sizeof(szFrontEnd));
    return PH_ERR_SUCCESS;
}

/** @} */

