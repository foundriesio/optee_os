/*
 * Copyright 2019,2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <ex_sss_boot.h>
#include <nxLog_App.h>

#include "mal_agent.h"
#include "string.h" /* memset */
#if SSS_HAVE_Host_PC == 0
#include "se05x_apis.h"
#endif
#include "mal_agent_handle.h"
#include "nxEnsure.h"
#include "pb.h"
#include "pb_decode.h"
#include "pb_encode.h"

//#include "mal_agent.pb.h"

#if SSS_HAVE_SE05X

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */
extern void se05x_ic_reset();

mal_status_t mal_agent_reset_se(mal_agent_ctx_t *mal_agent_context)
{
    ENSURE_OR_RETURN_ON_ERROR((mal_agent_context != NULL), kStatus_MAL_ERR_General);

#if AX_EMBEDDED
    se05x_ic_reset();
#endif

    return kStatus_MAL_Success;
}

#endif /* SSS_HAVE_SE05X */
