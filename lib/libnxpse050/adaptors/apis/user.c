// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 * Author: Jorge Ramirez <jorge@foundries.io>
 */

#include <nxScp03_Apis.h>
#include <se050_user_apis.h>
#include <se050_utils.h>
#include <string.h>

/*
 * @param k_object
 * @param k_store
 * @param key_id
 *
 * @return sss_status_t
 */
static sss_status_t alloc_scp_key_to_auth(sss_object_t *k_object,
					  sss_key_store_t *k_store,
					  uint32_t key_id)
{
	sss_status_t status = kStatus_SSS_Fail;

	if (!k_object || !k_store)
		return kStatus_SSS_Fail;

	status = sss_host_key_object_init(k_object, k_store);
	if (status != kStatus_SSS_Success)
		return status;

	status = sss_host_key_object_allocate_handle(k_object, key_id,
						     kSSS_KeyPart_Default,
						     kSSS_CipherType_AES, 16,
						     kKeyObject_Mode_Transient);
	return status;
}

/*
 * @param scp
 * @param auth
 * @param k_store
 * @param oid
 *
 * @return sss_status_t
 */
static sss_status_t prepare_host_scp(NXSCP03_AuthCtx_t *scp,
				     struct se050_auth_ctx *auth,
				     sss_key_store_t *k_store,
				     struct se050_scp_key *keys,
				     uint32_t oid)
{
	sss_status_t status = kStatus_SSS_Fail;
	NXSCP03_StaticCtx_t *pStatic_ctx = NULL;
	NXSCP03_DynCtx_t *pDyn_ctx = NULL;
	size_t len = 0;

	if (!scp || !auth || !k_store)
		return kStatus_SSS_Fail;

	pStatic_ctx = &auth->static_ctx;
	pDyn_ctx = &auth->dynamic_ctx;

	scp->pStatic_ctx = pStatic_ctx;
	scp->pDyn_ctx = pDyn_ctx;
	pStatic_ctx->keyVerNo = 0x0B;

	/* ENC Static Key */
	status = alloc_scp_key_to_auth(&pStatic_ctx->Enc, k_store, oid++);
	if (status != kStatus_SSS_Success)
		return status;

	len = sizeof(keys->enc);
	status = sss_host_key_store_set_key(k_store, &pStatic_ctx->Enc,
					    keys->enc, len, len * 8, NULL, 0);
	if (status != kStatus_SSS_Success)
		return status;

	/* MAC Static Key */
	status = alloc_scp_key_to_auth(&pStatic_ctx->Mac, k_store, oid++);
	if (status != kStatus_SSS_Success)
		return status;

	len = sizeof(keys->mac);
	status = sss_host_key_store_set_key(k_store, &pStatic_ctx->Mac,
					    keys->mac, len, len * 8, NULL, 0);
	if (status != kStatus_SSS_Success)
		return status;

	/* DEK Static Key */
	status = alloc_scp_key_to_auth(&pStatic_ctx->Dek, k_store, oid++);
	if (status != kStatus_SSS_Success)
		return status;

	len = sizeof(keys->dek);
	status = sss_host_key_store_set_key(k_store, &pStatic_ctx->Dek,
					    keys->dek, len, len * 8, NULL, 0);
	if (status != kStatus_SSS_Success)
		return status;

	/* Set ENC Session Key */
	status = alloc_scp_key_to_auth(&pDyn_ctx->Enc, k_store, oid++);
	if (status != kStatus_SSS_Success)
		return status;

	/* Set MAC Session Key */
	status = alloc_scp_key_to_auth(&pDyn_ctx->Mac, k_store, oid++);
	if (status != kStatus_SSS_Success)
		return status;

	/* Set DEK Session Key */
	status = alloc_scp_key_to_auth(&pDyn_ctx->Rmac, k_store, oid++);

	return status;
}

/*
 * @param host_session
 * @param host_ks
 * @param open_ctx
 * @param auth
 * @param auth_type
 *
 * @return sss_status_t
 */
sss_status_t se050_configure_host(sss_session_t *host_session,
				  sss_key_store_t *host_ks,
				  SE_Connect_Ctx_t *open_ctx,
				  struct se050_auth_ctx *auth,
				  SE_AuthType_t auth_type,
				  struct se050_scp_key *keys)
{
	sss_status_t status = kStatus_SSS_Fail;
	uint32_t host_oid = 0;

	if (!host_session || !host_ks || !open_ctx || !auth)
		return kStatus_SSS_Fail;

	if (host_session->subsystem != kType_SSS_SubSystem_NONE)
		goto prepare;

	status = sss_host_session_open(host_session, kType_SSS_Software, 0,
				       kSSS_ConnectionType_Plain, NULL);
	if (status != kStatus_SSS_Success)
		return status;

	status = sss_host_key_store_context_init(host_ks, host_session);
	if (status != kStatus_SSS_Success)
		return status;

	status = sss_host_key_store_allocate(host_ks, host_oid++);
	if (status != kStatus_SSS_Success)
		return status;
prepare:
	status = prepare_host_scp(&open_ctx->auth.ctx, auth, host_ks, keys,
				  host_oid);
	if (status != kStatus_SSS_Success)
		return status;

	open_ctx->auth.authType = auth_type;

	return status;
}

/**
 *
 * @param keyStore
 * @param keyObject
 * @param data
 * @param dataLen
 * @param pKeyBitLen
 *
 * @return sss_status_t
 */
sss_status_t se050_host_key_store_get_key(sss_key_store_t *ks __unused,
					  sss_object_t *ko, uint8_t *data,
					  size_t *byte_len, size_t *bit_len)
{
	sss_user_impl_object_t *key_object = (sss_user_impl_object_t *)ko;

	memcpy(data, key_object->key, key_object->contents_size);
	*byte_len = key_object->contents_size;
	*bit_len = 8 * key_object->contents_size;

	return kStatus_SSS_Success;
}
