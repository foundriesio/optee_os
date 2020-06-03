// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2015, Linaro Limited
 */

#include <inttypes.h>
#include <kernel/tee_common_otp.h>
#include <kernel/huk_subkey.h>
#include <trace.h>

/*
 * Override these in your platform code to really fetch device-unique
 * bits from e-fuses or whatever.
 *
 * The default implementation just sets it to a constant.
 */

#if !defined(CFG_NXP_SE05X_HUK_DRV)
/*
 *  revisit:
 *   __weak gets called when the symbol exists in a library: rather than
 *  reorganizing the link script we should perhaps compile it out for platforms
 *  that implement it in a lib
 */
__weak TEE_Result tee_otp_get_hw_unique_key(struct tee_hw_unique_key *hwkey)
{
	memset(&hwkey->data[0], 0, sizeof(hwkey->data));
	return TEE_SUCCESS;
}
#endif

__weak int tee_otp_get_die_id(uint8_t *buffer, size_t len)
{
	if (huk_subkey_derive(HUK_SUBKEY_DIE_ID, NULL, 0, buffer, len))
		return -1;

	return 0;
}
