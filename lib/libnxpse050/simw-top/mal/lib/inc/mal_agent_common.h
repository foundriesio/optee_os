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

/** @file */

#ifndef MAL_AGENT_COMMON_INC
#define MAL_AGENT_COMMON_INC

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

#ifndef __DOXYGEN__

#define MAL_AGENT_KEYSTORE_ID 1

#define MAL_AGENT_STATUS_WORD_INIT 0xFFFFFFFF
#define MAL_AGENT_STATUS_WORD_HOST_CMD_OK 0xFFFF9000
#define MAL_AGENT_STATUS_WORD_HOST_CMD_MASK 0xFFFF0000
#define MAL_AGENT_STATUS_WORD_COM_FAILURE 0xF0000000
#define MAL_AGENT_STATUS_WORD_USER_DEFINE_SUCCESS 0xF8000000

//#define MAL_AGENT_CHANNEL_1 0x01
//#define MAL_AGENT_CHANNEL_0 0x00
#endif /* __DOXYGEN__ */

/** Status of MALs update operation */
typedef enum _mal_status
{
    /** Operation was successful */
    kStatus_MAL_Success = 0x00,
    /** Communication Error */
    kStatus_MAL_ERR_COM,
    /** Update not completed please provide update package again. */
    kStatus_MAL_ERR_DoReRun,
    /** Update not applicable on this Chip/type. */
    kStatus_MAL_ERR_NotApplicable,
    /** Update can not be completed. Please provide recovery package, to roll back to last working version. */
    kStatus_MAL_ERR_DoRecovery,
    /** Unresolvable error. */
    kStatus_MAL_ERR_Fatal,
    /** General error. */
    kStatus_MAL_ERR_General
} mal_status_t;

#endif // !MAL_AGENT_COMMON_INC
