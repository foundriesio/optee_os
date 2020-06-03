// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 * Author: Jorge Ramirez <jorge@foundries.io>
 */

#include <crypto/crypto.h>
#include <tee/tee_cryp_utl.h>
#include <utee_defines.h>
#include <se050.h>
#include <string.h>
#include <trace.h>

#include <rng_support.h>
#include <se050.h>
#include <se050_key_crypto.h>
#include <se050_key.h>

/* base value for secure objects (transient and persistent) */
#define OID			((uint32_t)(0x60000000))

/* allow for 0xFE persistent keys */
#define OID_PERSISTENT_MAX	((uint32_t)(OID + 0xFF))
#define OID_PERSISTENT_MIN	((uint32_t)(OID + 1))

#define NBR_PERSISTENT		\
((uint32_t)(OID_PERSISTENT_MAX - OID_PERSISTENT_MIN))

/* transient keys */
#define OID_TRANSIENT_MAX	((uint32_t)(OID + 0x0FFFFFFF))
#define OID_TRANSIENT_MIN	((uint32_t)(OID_PERSISTENT_MAX + 1))

#define NBR_TRANSIENT		\
((uint32_t)(OID_TRANSIENT_MAX - OID_TRANSIENT_MIN))

/* watermark keys to support SKS search */
#define WATERMARK		0xDEADBEEF
#define WATERMARKED(x)		((uint64_t)(((uint64_t)WATERMARK) << 32) + (x))
#define IS_OID(x)		((((x) | OID) & 0xFFFFFF00) == OID)
#define IS_WATERMARKED(x)	(((x) & WATERMARKED(OID)) == WATERMARKED(OID))

/*
 * persistent object ids are scattered in their range; this is to minimize the
 * range of retries when requesting one
 */
static uint32_t se050_get_persistent_oid(void)
{
	uint32_t oid = OID_PERSISTENT_MIN;
	uint32_t ret = 0;
	int i = 0;

	for (i = 0; i < NBR_PERSISTENT; i++) {
		oid = OID_PERSISTENT_MIN + hw_get_random_byte();

		if (oid > OID_PERSISTENT_MAX)
			oid = OID_PERSISTENT_MIN;

		ret = se050_key_exists(oid, &se050_session->s_ctx);
		if (!ret)
			return oid;

		DMSG("persistent object in use %d", oid_persistent);
	}

	return 0;
}

/*
 * transient object ids are allocated sequentially in their range
 */
static uint32_t se050_get_transient_oid(void)
{
	static uint32_t oid_transient = OID_TRANSIENT_MIN;
	uint32_t oid = 0, ret = 0;
	int i = 0;

	for (i = 0; i < NBR_TRANSIENT; i++) {
		if (oid_transient > OID_TRANSIENT_MAX)
			oid_transient = OID_TRANSIENT_MIN;

		ret = se050_key_exists(oid_transient, &se050_session->s_ctx);
		if (!ret) {
			oid = oid_transient;
			break;
		}

		DMSG("transient object in use %d", oid_transient);
		oid_transient += 1;
	}

	if (oid)
		oid_transient += 1;

	return oid;
}

sss_status_t se050_get_oid(sss_key_object_mode_t type, uint32_t *val)
{
	sss_status_t status = kStatus_SSS_Success;
	uint16_t pmem = 0, tmem = 0;
	uint32_t oid;

	status = se050_get_freemem(&se050_session->s_ctx, &pmem, &tmem);
	if (status != kStatus_SSS_Success) {
		pmem = 0;
		tmem = 0;
		EMSG("failure retrieving free memory");
	}

	if (type == kKeyObject_Mode_Transient) {
		if (tmem && tmem < 100)
			IMSG("WARNING, low transient memory");

		oid = se050_get_transient_oid();
		if (!oid)
			return kStatus_SSS_Fail;

		*val = oid;
	} else {
		if (pmem && pmem < 100)
			IMSG("WARNING, low permanent memory");

		oid = se050_get_persistent_oid();
		if (!oid)
			return kStatus_SSS_Fail;

		IMSG("allocated persistent object: 0x%x", oid);
		*val = oid;
	}

	return kStatus_SSS_Success;
}

void se050_cleanup_all_persistent_objects(void)
{
	sss_status_t status = kStatus_SSS_Fail;
	sss_se05x_object_t ko;
	uint32_t id;

	status = sss_se05x_key_object_init(&ko, se050_kstore);
	if (status != kStatus_SSS_Success)
		return;

	for (id = OID_PERSISTENT_MIN; id < OID_PERSISTENT_MAX + 1; id++) {
		if (!se050_key_exists(id, &se050_session->s_ctx))
			continue;

		status = sss_se05x_key_object_get_handle(&ko, id);
		if (status != kStatus_SSS_Success)
			continue;

		sss_se05x_key_store_erase_key(se050_kstore, &ko);
		IMSG("erased key: 0x%x", id);
	}
}

uint32_t se050_rsa_keypair_from_nvm(struct rsa_keypair *key)
{
	uint64_t key_id;

	if (crypto_bignum_num_bytes(key->d) != sizeof(uint64_t))
		return 0;

	crypto_bignum_bn2bin(key->d, (uint8_t *)&key_id);

	if (IS_WATERMARKED(key_id) && IS_OID((uint32_t)key_id))
		return (uint32_t)key_id;

	return 0;
}

uint32_t se050_ecc_keypair_from_nvm(struct ecc_keypair *key)
{
	uint64_t key_id;

	if (crypto_bignum_num_bytes(key->d) != sizeof(uint64_t))
		return 0;

	crypto_bignum_bn2bin(key->d, (uint8_t *)&key_id);

	if (IS_WATERMARKED(key_id) && IS_OID((uint32_t)key_id))
		return (uint32_t)key_id;

	return 0;
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
	if (oid < OID_PERSISTENT_MIN && oid >  OID_PERSISTENT_MAX)
		return 0;

	return WATERMARKED(oid);
}
