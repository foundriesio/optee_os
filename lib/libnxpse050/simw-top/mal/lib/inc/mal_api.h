/* Copyright 2019,2020 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef MAL_API_H_INC
#define MAL_API_H_INC

#include <fsl_sss_se05x_types.h>
#include <mal_agent_common.h>
#include <mal_agent_context.h>
#include <sm_types.h>

#include "mal_api_ver.h"

#if SSS_HAVE_APPLET_SE05X_IOT
#include "fsl_sss_api.h"
#include "nxLog.h"
#include "nxScp03_Types.h"

/* *****************************************************************************************************************
 * Global Variables
 * ***************************************************************************************************************** */
#define MALS_API_VERSION_MAJOR MALS_AGENT_VER_MAJOR
#define MALS_API_VERSION_MINOR MALS_AGENT_VER_MINOR
#define MALS_API_VERSION_PATCH MALS_AGENT_VER_DEV

#define MALS_GET_DATA_CMD_BUF_LEN (256 + 5)

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/** Check Tear down Amd-I section 4.14 */
typedef enum _mal_tearDown_status_t
{
    /** The script has been completely executed */
    mal_notear = 0,
    /** Script execution was interrupted because of teardown. */
    mal_tear                    = 1,
    mal_tearDown_status_invalid = 0x7F,
} mal_tearDown_status_t;

typedef enum _mal_upgradeProgress_status_t
{
    /** Upgrade session Not in Progress */
    mal_upgrade_not_inProgress = 0,
    /** Upgrade session In Progress */
    mal_upgrade_inProgress = 1,
    mal_upgrade_invalid    = 0x7F,
} mal_upgradeProgress_status_t;

typedef enum _mal_recovery_status_t
{
    /** Recovery Not Started */
    mal_recovery_not_started = 0,
    /** Recovery Started */
    mal_recovery_started = 1,
} mal_recovery_status_t;

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/** @defgroup mal_agent MAL Agent
 *
 * API to load an available update package on the SE.
 */

/**
 * @addtogroup mal_agent
 * @{
 */

/** Information of about Applet/Package
 *
 * See Table 11-36: GlobalPlatform Registry Data (TLV), GPCardSpc_v2.2.pdf
 *
 * The response from Applet is put to rspBuf
 *
 * After parsing that response, the pointers to respective members is
 * set and it points to relevant part in rspBuf, this way saving
 * memory.  However, the Length is updated so that application use
 * this information.
 *
 */
typedef struct
{
    // 'E3' Variable GlobalPlatform Registry related data
    // '4F' 5 - 16 AID
    /** Applet ID */
    uint8_t *pAID;
    /** Length of the Applet ID */
    uint8_t AIDLen;
    // '9F70' 1 Life Cycle State
    /** Life-cycle state */
    uint8_t LifeCycleState;
    // 'C5' 0, 1, 3 Privileges(byte 1 - byte 2 - byte 3)
    /** Privileges. */
    uint8_t *pPriviledges;
    /** Length of Privileges */
    uint8_t PriviledgesLen;
    //
    // 'C4' 1 - n
    //! Application's Executable Load File AID
    uint8_t *pLoadFileAID;
    //! Length of LoadFileAID
    uint8_t LoadFileAIDLen;
    // 'CE' 1 - n
    //! Executable Load File Version Number
    uint8_t *pLoadFileVersionNumber;
    //! Length of pLoadFileVersionNumber
    uint8_t LoadFileVersionNumberLen;
    // '84' 1 - n First or only Executable Module AID
    // ... ... ... ...
    // '84' 1 - n Last Executable Module AID
    // 'CC' 1-n
    //! Associated Security Domain's AID
    uint8_t *pSecurityDomainAID;
    //! Length of SecurityDomainAID
    uint8_t SecurityDomainAIDLen;
    //! Response from Applet
    uint8_t rspBuf[MALS_GET_DATA_CMD_BUF_LEN];
    //! Length of response from Applet
    size_t rspBufLen;
} mals_SEAppInfoList_t;

/** Same as @ref mals_SEAppInfoList_t for for list of installed packages */
typedef mals_SEAppInfoList_t mals_PKGInfoList_t;

/**
 * @brief Load Applet package.
 *
 * This function load an available update package on the SE and assure
 * the tearing safe update of the SE.
 *
 * @param context Pointer to mal agent context.
 *
 * @param pkgBuf Pointer to package
 *
 * @param pkgBufLen Length of the package
 *
 * @param sigBuf Pointer to signature
 *
 * @param sigBufLen Length of the signature
 *
 * @returns Status of the operation
 *
 * @retval #kStatus_MAL_Success The operation has completed
 * successfully.
 *
 * @retval #kStatus_MAL_ERR_COM Communication to SE failed.
 *
 * @retval #kStatus_MAL_ERR_DoReRun Update not completed please
 * provide update package again.
 *
 * @retval #kStatus_MAL_ERR_NotApplicable Update not applicable on
 * this Chip/type.
 *
 * @retval #kStatus_MAL_ERR_DoRecovery Update can not be
 * completed. Please provide recovery package, to roll back to last
 * working version.
 *
 * @retval #kStatus_MAL_ERR_Fatal Unresolvable error. (This category
 * of errors can only appear in testing of an update package, in the
 * case of NXP updates this is testes before by NXP)
 *
 * @retval #kStatus_MAL_ERR_General Generic error.
 *
 * @note More return codes would be added to request host to either
 * retry or install older package.
 *
 */
mal_status_t mal_agent_load_package(
    mal_agent_ctx_t *context, uint8_t *pkgBuf, size_t pkgBufLen, uint8_t *sigBuf, size_t sigBufLen);

/**
 * @brief Initialize MALS agent context.
 *
 * This function is used to initialize MALS agent context.
 *
 * @param context Pointer to mal agent context.
 *
 * @param boot_ctx Pointer to sss session context
 *
 * @returns Status of the operation
 *
 * @retval #kStatus_MAL_Success The operation has completed successfully.
 *
 * @retval #kStatus_MAL_ERR_General The operation has failed.
 *
 */
sss_status_t mal_agent_init_context(mal_agent_ctx_t *context, sss_session_t *boot_ctx);

/**
 * @brief      Open a Physical connection to MALS Applet
 *
 * Calling this API opens Locical Connection 1 and selecs the MALs
 * applet.
 *
 * @param      context  MALS Agent Context
 *
 * @return     The api status.
 *
 * @retval #kStatus_MAL_Success Could connect to MALS Applet.
 *
 * @retval #kStatus_MAL_ERR_General Could not connect to MALS Applet.
 */
sss_status_t mal_agent_session_open(mal_agent_ctx_t *context);

/**
 * @brief      Close the connection to MALS Applet
 *
 * @param      context  The context
 *
 * @return     The api status.
 *
 * @retval #kStatus_MAL_Success Could close connection 1.
 *
 * @retval #kStatus_MAL_ERR_General Could not close connection 1.
 */
sss_status_t mal_agent_session_close(mal_agent_ctx_t *context);

/**
 * @brief Retrieve UUID from SE
 *
 * This API read UUID of the SE.
 *
 * @param pContext Pointer to mal agent context.
 *
 * @param pRspBuf Pointer to response Buffer.
 *
 * @param pRspBufLen Pointer to length of the response Buffer.
 *
 * @returns Status of the operation
 *
 * @retval #kStatus_SSS_Success The operation has completed successfully.
 *
 * @retval #kStatus_SSS_Fail The operation has failed.
 *
 *
 */
sss_status_t mals_get_UUID(mal_agent_ctx_t *pContext, uint8_t *pRspBuf, size_t *pRspBufLen);

/**
 * @brief Read Public Key
 *
 * This API will read root certificates public key of the device.
 *
 * @param pContext Pointer to mal agent context.
 *
 * @param pRspBuf Pointer to response Buffer.
 *
 * @param pRspBufLen Pointer to length of the response Buffer.
 *
 * @returns Status of the operation
 *
 * @retval #kStatus_SSS_Success The operation has completed successfully.
 *
 * @retval #kStatus_SSS_Fail The operation has failed.
 *
 *
 */
sss_status_t mals_get_Publickey(mal_agent_ctx_t *pContext, uint8_t *pRspBuf, size_t *pRspBufLen);

/**
 * @brief Low level API to get Raw App INFO from the SE
 *
 * This API will read the currently present Applications ELF/ELM AIDs and
 * versions as well as the present instances from the SE.
 *
 * @param pContext Pointer to mal agent context.
 *
 * @param[out] pRspBuf Pointer to response Buffer.
 *
 * @param[in]  searchAID     The search aid
 *
 * @param[in]  searchAidLen  The search aid length
 *
 * @param[in,out] pRspBufLen Pointer to length of the response Buffer.
 *
 * @returns Status of the operation
 *
 * @retval #kStatus_SSS_Success The operation has completed successfully.
 *
 * @retval #kStatus_SSS_Fail The operation has failed.
 *
 */
sss_status_t mals_get_SEAppInfoRAW(
    mal_agent_ctx_t *pContext, const uint8_t *searchAID, uint8_t searchAidLen, uint8_t *pRspBuf, size_t *pRspBufLen);

/**
 * @brief Low level API to get App INFO from the SE
 * according to format mentioned in -
 * Table 11-36: GlobalPlatform Registry Data (TLV), GPCardSpc_v2.2.pdf
 *
 * This API will read the currently present Applications ELF/ELM AIDs and
 * versions as well as the present instances from the SE.
 *
 * @param pContext Pointer to mal agent context.
 *
 * @param[in]  searchAID     The search aid
 *
 * @param[in]  searchAidLen  The search aid length
 *
 * @param      pAppInfo      Parsed structures
 *
 * @param[in,out]      pAppInfoLen   Length of parsed structures.
 *
 * @returns Status of the operation
 *
 * @retval #kStatus_SSS_Success The operation has completed successfully.
 *
 * @retval #kStatus_SSS_Fail The operation has failed.
 *
 */

sss_status_t mals_get_SEAppInfo(mal_agent_ctx_t *pContext,
    const uint8_t *searchAID,
    uint8_t searchAidLen,
    mals_SEAppInfoList_t *pAppInfo,
    size_t *pAppInfoLen);

/**
 * @brief Low level API to get RAW PKG INFO from the SE
 *
 * This API will read the currently present Applications ELF/ELM AIDs and
 * versions as well as the present instances from the SE.
 *
 * @param pContext Pointer to mal agent context.
 *
 * @param[in]  searchAID     The search aid
 *
 * @param[in]  searchAidLen  The search aid length
 *
 * @param[out] pRspBuf Pointer to response Buffer.
 *
 * @param[in,out] pRspBufLen Pointer to length of the response Buffer.
 *
 * @returns Status of the operation
 *
 * @retval #kStatus_SSS_Success The operation has completed successfully.
 *
 * @retval #kStatus_SSS_Fail The operation has failed.
 */
sss_status_t mals_get_SEPkgInfoRAW(
    mal_agent_ctx_t *pContext, const uint8_t *searchAID, uint8_t searchAidLen, uint8_t *pRspBuf, size_t *pRspBufLen);

/**
 * @brief Low level API to get PKG INFO from the SE
 * according to format mentioned in -
 * Table 11-36: GlobalPlatform Registry Data (TLV), GPCardSpc_v2.2.pdf
 *
 * This API will read the currently present Applications ELF/ELM AIDs and
 * versions as well as the present instances from the SE.
 *
 * @param pContext Pointer to mal agent context.
 *
 * @param[in]  searchAID     The search aid
 *
 * @param[in]  searchAidLen  The search aid length
 *
 * @param      pAppInfo      Parsed structures
 *
 * @param[in,out]      pAppInfoLen   Length of parsed structures.
 *
 * @returns Status of the operation
 *
 * @retval #kStatus_SSS_Success The operation has completed successfully.
 *
 * @retval #kStatus_SSS_Fail The operation has failed.
 *
 */

sss_status_t mals_get_SEPkgInfo(mal_agent_ctx_t *pContext,
    const uint8_t *searchAID,
    uint8_t searchAidLen,
    mals_SEAppInfoList_t *pAppInfo,
    size_t *pAppInfoLen);

/**
 * @brief Check Tear during script execution
 *
 * This API will check whether there was tear during
 * script execution
 *
 * @param pContext Pointer to mal agent context.
 *
 * @param pTearStatus Pointer to tear status.
 *
 * @returns Status of the operation
 *
 * @retval #kStatus_SSS_Success The operation has completed successfully.
 *
 * @retval #kStatus_SSS_Fail The operation has failed.
 *
 *
 */
sss_status_t mals_check_Tear(mal_agent_ctx_t *pContext, mal_tearDown_status_t *pTearStatus);

/**
 * @brief Get the signature of last executed script
 *
 * This API will called in case there is tear down
 *
 * @param pContext Pointer to mal agent context.
 *
 * @param pRspBuf Pointer to response Buffer.
 *
 * @param pRspBufLen Pointer to length of the response Buffer
 *
 * @returns Status of the operation
 *
 * @retval #kStatus_SSS_Success The operation has completed successfully.
 *
 * @retval #kStatus_SSS_Fail The operation has failed.
 *
 *
 */
sss_status_t mals_get_SignatureofLastScript(mal_agent_ctx_t *pContext, uint8_t *pRspBuf, size_t *pRspBufLen);

/**
 * @brief Get the Mals Agent version
 *
 * This API will return the MALS Agent Version no.
 *
 * @param pRspBuf Pointer to response Buffer.
 *
 * @param pRspBufLen Pointer to length of the response Buffer
 *
 * @returns Status of the operation
 *
 * @retval #kStatus_SSS_Success The operation has completed successfully.
 *
 * @retval #kStatus_SSS_Fail The operation has failed.
 *
 *
 */
sss_status_t mals_get_AgentVersion(uint8_t *pRspBuf, size_t *pRspBufLen);

/**
 * @brief Get the Applet version
 *
 * This API will return the MALS Applet Version no.
 *
 * @param pContext Pointer to mal agent context.
 *
 * @param pRspBuf Pointer to response Buffer.
 *
 * @param pRspBufLen Pointer to length of the response Buffer
 *
 * @returns Status of the operation
 *
 * @retval #kStatus_SSS_Success The operation has completed successfully.
 *
 * @retval #kStatus_SSS_Fail The operation has failed.
 *
 *
 */
sss_status_t mals_get_AppletVersion(mal_agent_ctx_t *pContext, uint8_t *pRspBuf, size_t *pRspBufLen);

/**
 * @brief Check Applet Upgrade Progress
 *
 * This API will return the status of applet upgrade progress status
 *
 * @param pContext Pointer to mal agent context.
 *
 * @param pUpgradeStatus Pointer to upgrade status.
 *
 * @returns Status of the operation
 *
 * @retval #kStatus_SSS_Success The operation has completed successfully.
 *
 * @retval #kStatus_SSS_Fail The operation has failed.
 *
 *
 */
sss_status_t mals_check_AppletUpgradeProgress(mal_agent_ctx_t *pContext, mal_upgradeProgress_status_t *pUpgradeStatus);

/**
 * @brief Check Applet Recovery Status
 *
 * This API will return the status of applet recovery status
 *
 * @param pContext Pointer to mal agent context.
 *
 * @param pRecoveryStatus Pointer to recovery status.
 *
 * @returns Status of the operation
 *
 * @retval #kStatus_SSS_Success The operation has completed successfully.
 *
 * @retval #kStatus_SSS_Fail The operation has failed.
 *
 *
 */
sss_status_t mals_check_AppletRecoveryStatus(mal_agent_ctx_t *pContext, mal_recovery_status_t *pRecoveryStatus);

/**
 * @brief Get the ENC Identifier.
 *
 * This API will return the ENC Identifier.
 *
 * @param pContext Pointer to mal agent context.
 *
 * @param pRspBuf Pointer to response Buffer.
 *
 * @param pRspBufLen Pointer to length of the response Buffer
 *
 * @returns Status of the operation
 *
 * @retval #kStatus_SSS_Success The operation has completed successfully.
 *
 * @retval #kStatus_SSS_Fail The operation has failed.
 *
 *
 */
sss_status_t mals_get_ENCIdentifier(mal_agent_ctx_t *pContext, uint8_t *pRspBuf, size_t *pRspBufLen);
/**
 *@}
 */ /* end of mal_agent */

#endif // SSS_HAVE_APPLET_SE05X_IOT
#endif // !MAL_API_H_INC
