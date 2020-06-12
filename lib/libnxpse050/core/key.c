// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 * Author: Jorge Ramirez <jorge@foundries.io>
 */

#include <crypto/crypto.h>
#include <rng_support.h>
#include <se050.h>
#include <se050_key_crypto.h>
#include <se050_key.h>
#include <string.h>
#include <tee/tee_cryp_utl.h>
#include <trace.h>
#include <utee_defines.h>

/* base value for secure objects (transient and persistent) */
#define OID_MIN			((uint32_t)(0x00000001))
#define OID_MAX			((uint32_t)(OID_MIN + 0x7BFFFFFE))
#define NBR_OID			((uint32_t)(OID_MAX - OID_MIN))

/* watermark keys to support SKS search */
#define WATERMARK		0xDEADBEEF
#define WATERMARKED(x)		((uint64_t)(((uint64_t)WATERMARK) << 32) + (x))
#define IS_WATERMARKED(x)	(((x) & WATERMARKED(0)) == WATERMARKED(0))

static uint32_t generate_oid(void)
{
	uint32_t oid = OID_MIN;
	uint32_t random = 0;
	int i = 0;

	for (i = 0; i < NBR_OID; i++) {
		if (crypto_rng_read(&random, sizeof(random)) != TEE_SUCCESS)
			return 0;

		random &= OID_MAX;

		oid = OID_MIN + random;
		if (oid > OID_MAX)
			continue;

		if (!se050_key_exists(oid, &se050_session->s_ctx))
			return oid;
	}

	return 0;
}

sss_status_t se050_get_oid(sss_key_object_mode_t type, uint32_t *val)
{
	sss_status_t status = kStatus_SSS_Success;
	uint16_t pmem = 0, tmem = 0;
	uint32_t oid = 0;

	if (!val)
		return kStatus_SSS_Fail;

	status = se050_get_freemem(&se050_session->s_ctx, &pmem, &tmem);
	if (status != kStatus_SSS_Success) {
		pmem = 0;
		tmem = 0;
		EMSG("failure retrieving free memory");
		return kStatus_SSS_Fail;
	}

	if (type == kKeyObject_Mode_Transient) {
		if (tmem && tmem < 100)
			IMSG("WARNING, low transient memory");
	} else {
		if (pmem && pmem < 100)
			IMSG("WARNING, low permanent memory");
	}

	oid = generate_oid();
	if (!oid) {
		EMSG("allocation error");
		return kStatus_SSS_Fail;
	}

	if (type == kKeyObject_Mode_Persistent)
		IMSG("allocated persistent object: 0x%x", oid);

	*val = oid;

	return kStatus_SSS_Success;
}

static uint32_t se050_key(uint64_t key)
{
	uint32_t oid = (uint32_t)key;

	if (!IS_WATERMARKED(key))
		return 0;

	if (oid < OID_MIN || oid > OID_MAX)
		return 0;

	return oid;
}

uint32_t se050_rsa_keypair_from_nvm(struct rsa_keypair *key)
{
	uint64_t key_id = 0;

	if (!key)
		return 0;

	if (crypto_bignum_num_bytes(key->d) != sizeof(uint64_t))
		return 0;

	crypto_bignum_bn2bin(key->d, (uint8_t *)&key_id);

	return se050_key(key_id);
}

uint32_t se050_ecc_keypair_from_nvm(struct ecc_keypair *key)
{
	uint64_t key_id = 0;

	if (!key)
		return 0;

	if (crypto_bignum_num_bytes(key->d) != sizeof(uint64_t))
		return 0;

	crypto_bignum_bn2bin(key->d, (uint8_t *)&key_id);

	return se050_key(key_id);
}

/*
 * pkcs-11 key deletion support:
 * scan a buffer looking for a persistent key and delete it from the SE050
 * memory
 */
void se050_delete_persistent_key(uint8_t *data, size_t len)
{
	sss_se05x_object_t ko = { 0 };
	uint32_t val = WATERMARK;
	sss_status_t status;
	uint8_t *p = data;
	bool found = false;

	if (!p) {
		EMSG("invalid buffer");
		return;
	}

	/*
	 * persistent keys were watermarked so they could be found in the buffer
	 */
	while (len > sizeof(uint64_t) && !found) {
		if (memcmp(p, &val, sizeof(val)) != 0) {
			p++;
			len--;
			continue;
		}
		found = true;
	}

	if (!found)
		return;

	p = p - 4;
	memcpy((void *)&val, p, sizeof(val));

	status = sss_se05x_key_object_init(&ko, se050_kstore);
	if (status != kStatus_SSS_Success) {
		EMSG("error deleting persistent key");
		return;
	}

	status = sss_se05x_key_object_get_handle(&ko, val);
	if (status != kStatus_SSS_Success) {
		EMSG("error deleting persistent key");
		return;
	}

	status = sss_se05x_key_store_erase_key(se050_kstore, &ko);
	if (status != kStatus_SSS_Success) {
		EMSG("error deleting persistent key");
		return;
	}

	IMSG("deleted se050 persistent key 0x%x", val);
}

uint64_t se050_generate_private_key(uint32_t oid)
{
	return WATERMARKED(oid);
}
