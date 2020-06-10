// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 * Author: Jorge Ramirez <jorge@foundries.io>
 */

#include <apis.h>
#include <se050_default_keys.h>

static uint32_t oid = 0x70000000;

#ifndef CFG_CORE_SE05X_OEFID
#define OEFID SE050DV
#else
#define OEFID CFG_CORE_SE05X_OEFID
#endif

#if OEFID > SE050DV
#error "Invalid OEFID"
#endif

static sss_status_t alloc_scp_key_to_auth(sss_object_t *keyObject,
					  sss_key_store_t *pKs, uint32_t keyId)
{
	sss_status_t status = kStatus_SSS_Fail;

	status = sss_host_key_object_init(keyObject, pKs);
	if (status != kStatus_SSS_Success)
		return status;

	status = sss_host_key_object_allocate_handle(keyObject, keyId,
						     kSSS_KeyPart_Default,
						     kSSS_CipherType_AES, 16,
						     kKeyObject_Mode_Transient);
	return status;
}

static sss_status_t prepare_host_scp(NXSCP03_AuthCtx_t *scp,
				     struct SE_Auth_Ctx *auth,
				     sss_key_store_t *pKs)
{
	sss_status_t status = kStatus_SSS_Fail;
	NXSCP03_StaticCtx_t *pStatic_ctx = &auth->static_ctx;
	NXSCP03_DynCtx_t *pDyn_ctx = &auth->dynamic_ctx;
	size_t len = 0;

	scp->pStatic_ctx = pStatic_ctx;
	scp->pDyn_ctx = pDyn_ctx;
	pStatic_ctx->keyVerNo = 0x0B;

	/* ENC Static Key */
	status = alloc_scp_key_to_auth(&pStatic_ctx->Enc, pKs, oid++);
	if (status != kStatus_SSS_Success)
		return status;

	len = sizeof(se050_default_keys[OEFID].enc);
	status = sss_host_key_store_set_key(pKs, &pStatic_ctx->Enc,
					    se050_default_keys[OEFID].enc,
					    len, len * 8, NULL, 0);
	if (status != kStatus_SSS_Success)
		return status;

	/* MAC Static Key */
	status = alloc_scp_key_to_auth(&pStatic_ctx->Mac, pKs, oid++);
	if (status != kStatus_SSS_Success)
		return status;

	len = sizeof(se050_default_keys[OEFID].mac);
	status = sss_host_key_store_set_key(pKs, &pStatic_ctx->Mac,
					    se050_default_keys[OEFID].mac,
					    len, len * 8, NULL, 0);
	if (status != kStatus_SSS_Success)
		return status;

	/* DEK Static Key */
	status = alloc_scp_key_to_auth(&pStatic_ctx->Dek, pKs, oid++);
	if (status != kStatus_SSS_Success)
		return status;

	len = sizeof(se050_default_keys[OEFID].dek);
	status = sss_host_key_store_set_key(pKs, &pStatic_ctx->Dek,
					    se050_default_keys[OEFID].dek,
					    len, len * 8, NULL, 0);
	if (status != kStatus_SSS_Success)
		return status;

	/* Set ENC Session Key */
	status = alloc_scp_key_to_auth(&pDyn_ctx->Enc, pKs, oid++);
	if (status != kStatus_SSS_Success)
		return status;

	/* Set MAC Session Key */
	status = alloc_scp_key_to_auth(&pDyn_ctx->Mac, pKs, oid++);
	if (status != kStatus_SSS_Success)
		return status;

	/* Set DEK Session Key */
	status = alloc_scp_key_to_auth(&pDyn_ctx->Rmac, pKs, oid++);

	return status;
}

sss_status_t se050_configure_host(sss_session_t *host_session,
				  sss_key_store_t *host_ks,
				  SE_Connect_Ctx_t *open_ctx,
				  struct SE_Auth_Ctx *auth,
				  SE_AuthType_t auth_type)
{
	sss_status_t status = kStatus_SSS_Fail;

	if (host_session->subsystem != kType_SSS_SubSystem_NONE)
		goto prepare;

	status = sss_host_session_open(host_session, kType_SSS_Software, 0,
				       kSSS_ConnectionType_Plain, NULL);
	if (status != kStatus_SSS_Success)
		return status;

	status = sss_host_key_store_context_init(host_ks, host_session);
	if (status != kStatus_SSS_Success)
		return status;

	status = sss_host_key_store_allocate(host_ks, oid++);
	if (status != kStatus_SSS_Success)
		return status;
prepare:
	status = prepare_host_scp(&open_ctx->auth.ctx, auth, host_ks);
	if (status != kStatus_SSS_Success)
		return status;

	open_ctx->auth.authType = auth_type;

	return status;
}
