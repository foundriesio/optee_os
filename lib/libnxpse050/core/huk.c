// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 * Author: Jorge Ramirez <jorge@foundries.io>
 */

#include <kernel/tee_common_otp.h>
#include <se050.h>
#include <string.h>
#include <trace.h>

static uint8_t huk[HW_UNIQUE_KEY_LENGTH];
static bool huk_ready;

TEE_Result tee_otp_get_hw_unique_key(struct tee_hw_unique_key *hwkey)
{
	uint8_t se050_huk[SE050_MODULE_UNIQUE_ID_LEN];
	size_t se050_huk_len = sizeof(se050_huk);
	sss_status_t status;

	if (huk_ready)
		goto done;

	status = sss_se05x_session_prop_get_au8(se050_session,
						kSSS_SessionProp_UID,
						se050_huk, &se050_huk_len);
	if (status != kStatus_SSS_Success)
		return TEE_ERROR_GENERIC;

	memcpy(huk, se050_huk, sizeof(huk));
	huk_ready = true;
done:
	memcpy(hwkey->data, huk, sizeof(hwkey->data));

	return TEE_SUCCESS;
}
