// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 */

#include <compiler.h>
#include <crypto/crypto.h>
#include <tee/tee_cryp_utl.h>
#include <types_ext.h>

#if !defined(CFG_NXP_SE05X_SVC)
TEE_Result __weak crypto_enable_scp03(unsigned int rotate_keys)
{
	return TEE_SUCCESS;
}
#endif
