// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 * Author: Jorge Ramirez <jorge@foundries.io>
 */

#include <fsl_sss_api.h>

void nLog(const char *comp, int level, const char *format, ...)
{
}

void nLog_au8(const char *comp, int level, const char *message,
	      const unsigned char *array, size_t array_len)
{
}

sss_status_t sss_digest_context_init(sss_digest_t *context,
				     sss_session_t *session,
				     sss_algorithm_t algorithm, sss_mode_t mode)
{
	return kStatus_SSS_Fail;
}

sss_status_t sss_digest_one_go(sss_digest_t *context, const uint8_t *message,
			       size_t messageLen, uint8_t *digest,
			       size_t *digestLen)
{
	return kStatus_SSS_Fail;
}

sss_status_t sss_digest_init(sss_digest_t *context)
{
	return kStatus_SSS_Fail;
}

sss_status_t sss_digest_update(sss_digest_t *context, const uint8_t *message,
			       size_t messageLen)
{
	return kStatus_SSS_Fail;
}

sss_status_t sss_digest_finish(sss_digest_t *context, uint8_t *digest,
			       size_t *digestLen)
{
	return kStatus_SSS_Fail;
}

void sss_digest_context_free(sss_digest_t *context)
{
}

sss_status_t sss_key_store_set_key(sss_key_store_t *keyStore,
				   sss_object_t *keyObject, const uint8_t *data,
				   size_t dataLen, size_t keyBitLen,
				   void *options, size_t optionsLen)
{
	return kStatus_SSS_Fail;
}

sss_status_t sss_key_store_get_key(sss_key_store_t *keyStore,
				   sss_object_t *keyObject, uint8_t *data,
				   size_t *dataLen, size_t *pKeyBitLen)
{
	return kStatus_SSS_Fail;
}

sss_status_t sss_rng_context_init(sss_rng_context_t *context,
				  sss_session_t *session)
{
	return kStatus_SSS_Fail;
}

sss_status_t sss_rng_get_random(sss_rng_context_t *context,
				uint8_t *random_data, size_t dataLen)
{
	return kStatus_SSS_Fail;
}

int sscanf(const char *str, const char *format, ...)
{
	return -1;
}
