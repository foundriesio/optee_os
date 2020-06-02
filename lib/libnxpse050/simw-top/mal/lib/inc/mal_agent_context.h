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

#ifndef MAL_AGENT_CONTEXT_INC
#define MAL_AGENT_CONTEXT_INC

#include <sm_types.h>

#include "fsl_sss_api.h"
#include "mal_agent_common.h"
#include "se05x_tlv.h"

//#include "fsl_sss_api.h"
#if SSS_HAVE_APPLET_SE05X_IOT
//#include "fsl_sss_se05x_types.h"
#endif

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

typedef struct
{
    int current_command_index;
    int checkpoint_index;
    int retry_count;
} mal_agent_request_ctx_t;

typedef struct
{
    /** Boot context */
    Se05xSession_t *pS05x_Ctx;
    /** Status word of last R-APDU. */
    uint32_t status_word;
    /** Flag for get recovery script */
    bool recovery_executed;

    /** Skip following commands
     *
     * Internal state variable.
     *
     * When one APDU in this APDU stream has failed,
     * skip through all the next APDUs (do not send them)
     * and return back to the caller.
     */
    bool skip_next_commands;

#ifdef MAL_AGENT_CHANNEL_1
    /** FIXME: TODO
     *
     * The currnt used logical channel.
     *
     * We use Channel 0 for all normal communication (To the IoT Applet)
     * And we use Channel 1 for communcation to the MALS Applet.
     *
     * This variable helps to know on which channel communication is
     * happening, and it should continue there.  Selecting the MALS applet
     * at two channels is not possible.
     *
     * In case we have not switched to channel 1, for actual upload of the
     * package, we can still use chanel 0 to talk to MALS Applet and
     * also to the IoT Applet.
     *
     */
    uint8_t n_logical_channel;
#endif

} mal_agent_ctx_t;

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/*!
 *@}
 */ /* end of mal_agent */

#endif // !MAL_AGENT_CONTEXT_INC
