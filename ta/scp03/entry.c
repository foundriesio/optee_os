// SPDX-License-Identifier: BSD-3-Clause
/* Copyright (c) 2020, Foundries.IO */

#include <ta_scp03.h>
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include <string.h>
#include <util.h>

static TEE_Result ta_entry_enable_scp03(uint32_t pt,
					TEE_Param params[4])
{
	const uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE);

	if (pt != exp_pt)
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_EnableScp03(params[0].value.a);
	return TEE_SUCCESS;
}

TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t pt __unused,
				    TEE_Param params[4] __unused,
				    void **session __unused)
{
	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void *sess __unused)
{
}

TEE_Result TA_InvokeCommandEntryPoint(void *sess __unused, uint32_t cmd,
				      uint32_t pt,
				      TEE_Param params[TEE_NUM_PARAMS])
{
	switch (cmd) {
	case TA_CRYPT_CMD_ENABLE_SCP03:
		return ta_entry_enable_scp03(pt, params);
	default:
		EMSG("Command ID 0x%x is not supported", cmd);
		return TEE_ERROR_NOT_SUPPORTED;
	}
}
