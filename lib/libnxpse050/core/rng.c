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
#include <se050.h>
#include <string.h>

static TEE_Result do_rng_read(void *buf, size_t blen)
{
	sss_se05x_rng_context_t rng = { 0 };
	sss_status_t status = kStatus_SSS_Success;

	sss_se05x_rng_context_init(&rng, se050_session);

	status = sss_se05x_rng_get_random(&rng, buf, blen);
	if (status != kStatus_SSS_Success)
		return TEE_ERROR_GENERIC;

	sss_se05x_rng_context_free(&rng);

	return TEE_SUCCESS;
}

void plat_rng_init(void)
{
}

TEE_Result crypto_rng_read(void *buf, size_t blen)
{
	if (!buf)
		return TEE_ERROR_BAD_PARAMETERS;

	return do_rng_read(buf, blen);
}

uint8_t hw_get_random_byte(void)
{
	uint8_t data = 0;

	if (do_rng_read(&data, 1) != TEE_SUCCESS)
		panic();

	return data;
}

