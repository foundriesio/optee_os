// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 * Author: Jorge Ramirez <jorge@foundries.io>
 */

#include <atomic.h>
#include <crypto/crypto.h>
#include <kernel/panic.h>
#include <mm/core_memprot.h>
#include <rng_support.h>
#include <tee/cache.h>
#include <tee/tee_cryp_utl.h>
#include <string.h>
#include <initcall.h>

#include <se050.h>
#include <se050_key.h>

sss_se05x_key_store_t		*se050_kstore;
sss_se05x_session_t		*se050_session;
sss_se05x_ctx_t			se050_ctx;

static TEE_Result se050_service_init(void)
{
	sss_status_t status;

	status = se050_session_open(&se050_ctx);
	if (kStatus_SSS_Success != status)
		return TEE_ERROR_GENERIC;

	status = se050_factory_reset(&se050_ctx);
	if (kStatus_SSS_Success != status)
		return TEE_ERROR_GENERIC;

	if (se050_ctx.session.subsystem == kType_SSS_SubSystem_NONE)
		return TEE_ERROR_GENERIC;

	status = se050_kestore_and_object_init(&se050_ctx);
	if (kStatus_SSS_Success != status)
		return TEE_ERROR_GENERIC;

	IMSG("se050 [ready]");
	se050_session = (sss_se05x_session_t *)((void *)&se050_ctx.session);
	se050_kstore = (sss_se05x_key_store_t *)((void *)&se050_ctx.ks);

#if CFG_CORE_SE05X_INIT_NVM
	IMSG("=======================================");
	IMSG(" WARNING: DELETING ALL SE050 KEYS ");
	IMSG("=======================================");
	se050_cleanup_all_persistent_objects();
#endif
	return TEE_SUCCESS;
}

service_init(se050_service_init);
