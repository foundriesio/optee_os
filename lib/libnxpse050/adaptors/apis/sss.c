// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 * Author: Jorge Ramirez <jorge@foundries.io>
 */

#include <se050.h>

/*
 * @param pCtx
 *
 * @return sss_status_t
 */
sss_status_t se050_session_open(sss_se05x_ctx_t *ctx, bool encryption)
{
	SE_Connect_Ctx_t *pConnectCtx = NULL;
	sss_se05x_session_t *pSession = NULL;
	sss_status_t status = kStatus_SSS_Fail;

	if (!ctx)
		return kStatus_SSS_Fail;

	pConnectCtx = &ctx->open_ctx;
	pSession = &ctx->session;

	pConnectCtx->connType = kType_SE_Conn_Type_T1oI2C;
	pConnectCtx->portName = NULL;

	if (!encryption)
		return sss_se05x_session_open(pSession, kType_SSS_SE_SE05x, 0,
					      kSSS_ConnectionType_Plain,
					      pConnectCtx);

	status = se050_configure_host(&ctx->host_session,
				      &ctx->host_ks,
				      &ctx->open_ctx,
				      &ctx->se05x_auth,
				      kSSS_AuthType_SCP03);
	if (status != kStatus_SSS_Success)
		return status;

	return sss_se05x_session_open(pSession, kType_SSS_SE_SE05x, 0,
				      kSSS_ConnectionType_Encrypted,
				      pConnectCtx);
}

/*
 * @param pCtx
 *
 * @return sss_status_t
 */
sss_status_t se050_kestore_and_object_init(sss_se05x_ctx_t *ctx)
{
	sss_status_t status = kStatus_SSS_Fail;

	if (!ctx)
		return status;

	status = sss_se05x_key_store_context_init(&ctx->ks, &ctx->session);
	if (status != kStatus_SSS_Success)
		EMSG(" sss_key_store_context_init Failed...");

	return status;
}

/*
 * pkcs-11 key deletion support:
 * scan a buffer looking for a persistent key and delete it from the SE050
 * memory
 */
void se050_delete_persistent_key(uint8_t *data, size_t len)
{
	sss_se05x_object_t k_object = { 0 };
	uint32_t val = SE050_KEY_WATERMARK;
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

	status = sss_se05x_key_object_init(&k_object, se050_kstore);
	if (status != kStatus_SSS_Success) {
		EMSG("error deleting persistent key");
		return;
	}

	status = sss_se05x_key_object_get_handle(&k_object, val);
	if (status != kStatus_SSS_Success) {
		EMSG("error deleting persistent key");
		return;
	}

	status = sss_se05x_key_store_erase_key(se050_kstore, &k_object);
	if (status != kStatus_SSS_Success) {
		EMSG("error deleting persistent key");
		return;
	}

	IMSG("deleted se050 persistent key 0x%x", val);
}

