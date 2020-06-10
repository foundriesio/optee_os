// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 * Author: Jorge Ramirez <jorge@foundries.io>
 */

#include <apis.h>
#include <fsl_sss_policy.h>
#include <fsl_sss_se05x_apis.h>
#include <fsl_sss_se05x_policy.h>
#include <fsl_sss_se05x_scp03.h>
#include <fsl_sss_se05x_types.h>
#include <fsl_sss_util_asn1_der.h>
#include <fsl_sss_util_rsa_sign_utils.h>
#include <nxEnsure.h>
#include <nxScp03_Apis.h>
#include <se05x_APDU.h>
#include <se05x_const.h>
#include <se05x_ecc_curves.h>
#include <se05x_tlv.h>
#include <sm_api.h>
#include <smCom.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <trace.h>

/*
 * @param keyId
 * @param session_ctx
 *
 * @return uint8_t
 */
uint8_t se050_key_exists(uint32_t keyId, pSe05xSession_t session_ctx)
{
	SE05x_Result_t inuse = kSE05x_Result_FAILURE;
	smStatus_t retStatus = SM_OK;

	retStatus = Se05x_API_CheckObjectExists(session_ctx, keyId, &inuse);
	if (retStatus != SM_OK)
		return 0;

	if (inuse == kSE05x_Result_SUCCESS)
		return 1;

	return 0;
}

/*
 * @param session_ctx
 * @param type
 * @param val
 *
 * @return sss_status_t
 */
sss_status_t  se050_get_freemem(pSe05xSession_t session_ctx,
				uint16_t *p, uint16_t *t)
{
	smStatus_t ret = SM_OK;

	ret = Se05x_API_GetFreeMemory(session_ctx,
				      kSE05x_MemoryType_PERSISTENT, p);
	if (ret != SM_OK) {
		EMSG("persistent");
		return kSE05x_Result_FAILURE;
	}

	ret = Se05x_API_GetFreeMemory(session_ctx,
				      kSE05x_MemoryType_TRANSIENT_RESET, t);
	if (ret != SM_OK) {
		EMSG("transient");
		return kSE05x_Result_FAILURE;
	}

	return kStatus_SSS_Success;
}

/*
 * @param store
 * @param ko
 * @param key_pair
 * @param key_pub
 * @param key_bit_len
 *
 * @return sss_status_t
 */
sss_status_t sss_se05x_key_store_set_rsa_key_bin(sss_se05x_key_store_t *store,
						 sss_se05x_object_t *ko,
						 struct rsa_keypair_bin
						 *key_pair,
						 struct rsa_public_key_bin
						 *key_pub,
						 size_t key_bit_len)
{
	Se05xSession_t *s_ctx = &store->session->s_ctx;
	SE05x_RSAKeyFormat_t rsa_format = kSE05x_RSAKeyFormat_RAW;
	SE05x_TransientType_t type = ko->isPersistant ?
				     kSE05x_TransientType_Persistent :
				     kSE05x_TransientType_Transient;
	uint32_t key_type = ko->objectType;
	Se05xPolicy_t policy = {
		.value = NULL, .value_len = 0,
	};
	sss_status_t retval = kStatus_SSS_Fail;
	smStatus_t status = SM_NOT_OK;

	switch (ko->cipherType) {
	case kSSS_CipherType_RSA:
		rsa_format = kSE05x_RSAKeyFormat_RAW;
		break;
	case kSSS_CipherType_RSA_CRT:
		rsa_format = kSE05x_RSAKeyFormat_CRT;
		break;
	default:
		retval = kStatus_SSS_Fail;
		goto exit;
	}

	if (se050_key_exists(ko->keyId, s_ctx))
		key_bit_len = 0;

	if (key_type != kSSS_KeyPart_Public)
		goto label_private;

	/* Set the Public Exponent */
	status = Se05x_API_WriteRSAKey(s_ctx,
				       &policy,
				       ko->keyId,
				       (U16)key_bit_len,
				       SE05X_RSA_NO_p,
				       SE05X_RSA_NO_q,
				       SE05X_RSA_NO_dp,
				       SE05X_RSA_NO_dq,
				       SE05X_RSA_NO_qInv,
				       key_pub->e,
				       key_pub->e_len,
				       SE05X_RSA_NO_priv,
				       SE05X_RSA_NO_pubMod,
				       type,
				       kSE05x_KeyPart_Public,
				       rsa_format);
	if (status != SM_OK) {
		EMSG("keybitlen %ld, e_len %ld", key_bit_len, key_pub->e_len);
		retval = kStatus_SSS_Fail;
		goto exit;
	}

	/* Set the Modulus */
	status = Se05x_API_WriteRSAKey(s_ctx,
				       NULL,
				       ko->keyId,
				       0,
				       SE05X_RSA_NO_p,
				       SE05X_RSA_NO_q,
				       SE05X_RSA_NO_dp,
				       SE05X_RSA_NO_dq,
				       SE05X_RSA_NO_qInv,
				       SE05X_RSA_NO_pubExp,
				       SE05X_RSA_NO_priv,
				       key_pub->n,
				       key_pub->n_len,
				       type,
				       kSE05x_KeyPart_NA,
				       rsa_format);
	if (status == SM_OK)
		retval = kStatus_SSS_Success;

	goto exit;

label_private:
	if (key_type != kSSS_KeyPart_Private)
		goto label_pair;

	if (ko->cipherType == kSSS_CipherType_RSA) {
		status = Se05x_API_WriteRSAKey(s_ctx,
					       &policy,
					       ko->keyId,
					       (U16)key_bit_len,
					       SE05X_RSA_NO_p,
					       SE05X_RSA_NO_q,
					       SE05X_RSA_NO_dp,
					       SE05X_RSA_NO_dq,
					       SE05X_RSA_NO_qInv,
					       SE05X_RSA_NO_pubExp,
					       key_pair->d,
					       key_pair->d_len,
					       SE05X_RSA_NO_pubMod,
					       type,
					       kSE05x_KeyPart_Pair,
					       rsa_format);
		if (status != SM_OK) {
			retval = kStatus_SSS_Fail;
			goto exit;
		}

		status = Se05x_API_WriteRSAKey(s_ctx,
					       NULL,
					       ko->keyId,
					       0,
					       SE05X_RSA_NO_p,
					       SE05X_RSA_NO_q,
					       SE05X_RSA_NO_dp,
					       SE05X_RSA_NO_dq,
					       SE05X_RSA_NO_qInv,
					       SE05X_RSA_NO_pubExp,
					       SE05X_RSA_NO_priv,
					       key_pair->n,
					       key_pair->n_len,
					       type,
					       kSE05x_KeyPart_NA,
					       rsa_format);
		if (status == SM_OK)
			retval = kStatus_SSS_Success;

		goto exit;
	}

	if (ko->cipherType == kSSS_CipherType_RSA_CRT) {
		status = Se05x_API_WriteRSAKey(s_ctx,
					       &policy,
					       ko->keyId,
					       (U16)key_bit_len,
					       key_pair->p,
					       key_pair->p_len,
					       SE05X_RSA_NO_q,
					       SE05X_RSA_NO_dp,
					       SE05X_RSA_NO_dq,
					       SE05X_RSA_NO_qInv,
					       SE05X_RSA_NO_pubExp,
					       SE05X_RSA_NO_priv,
					       SE05X_RSA_NO_pubMod,
					       type,
					       kSE05x_KeyPart_Private,
					       rsa_format);
		if (status != SM_OK) {
			retval = kStatus_SSS_Fail;
			goto exit;
		}

		status = Se05x_API_WriteRSAKey(s_ctx,
					       NULL,
					       ko->keyId,
					       0,
					       SE05X_RSA_NO_p,
					       key_pair->q,
					       key_pair->q_len,
					       SE05X_RSA_NO_dp,
					       SE05X_RSA_NO_dq,
					       SE05X_RSA_NO_qInv,
					       SE05X_RSA_NO_pubExp,
					       SE05X_RSA_NO_priv,
					       SE05X_RSA_NO_pubMod,
					       type,
					       kSE05x_KeyPart_NA,
					       rsa_format);
		if (status != SM_OK) {
			retval = kStatus_SSS_Fail;
			goto exit;
		}

		status = Se05x_API_WriteRSAKey(s_ctx,
					       NULL,
					       ko->keyId,
					       0,
					       SE05X_RSA_NO_p,
					       SE05X_RSA_NO_q,
					       key_pair->dp,
					       key_pair->dp_len,
					       SE05X_RSA_NO_dq,
					       SE05X_RSA_NO_qInv,
					       SE05X_RSA_NO_pubExp,
					       SE05X_RSA_NO_priv,
					       SE05X_RSA_NO_pubMod,
					       type,
					       kSE05x_KeyPart_NA,
					       rsa_format);
		if (status != SM_OK) {
			retval = kStatus_SSS_Fail;
			goto exit;
		}

		status = Se05x_API_WriteRSAKey(s_ctx,
					       NULL,
					       ko->keyId,
					       0,
					       SE05X_RSA_NO_p,
					       SE05X_RSA_NO_q,
					       SE05X_RSA_NO_dp,
					       key_pair->dq,
					       key_pair->dq_len,
					       SE05X_RSA_NO_qInv,
					       SE05X_RSA_NO_pubExp,
					       SE05X_RSA_NO_priv,
					       SE05X_RSA_NO_pubMod,
					       type,
					       kSE05x_KeyPart_NA,
					       rsa_format);
		if (status != SM_OK) {
			retval = kStatus_SSS_Fail;
			goto exit;
		}

		status = Se05x_API_WriteRSAKey(s_ctx,
					       NULL,
					       ko->keyId,
					       0,
					       SE05X_RSA_NO_p,
					       SE05X_RSA_NO_q,
					       SE05X_RSA_NO_dp,
					       SE05X_RSA_NO_dq,
					       key_pair->qp,
					       key_pair->qp_len,
					       SE05X_RSA_NO_pubExp,
					       SE05X_RSA_NO_priv,
					       SE05X_RSA_NO_pubMod,
					       type,
					       kSE05x_KeyPart_NA,
					       rsa_format);

		if (status == SM_OK)
			retval = kStatus_SSS_Success;
	}

	goto exit;

label_pair:
	if (key_type != kSSS_KeyPart_Pair)
		goto exit;

	if (ko->cipherType == kSSS_CipherType_RSA) {
		status = Se05x_API_WriteRSAKey(s_ctx,
					       &policy,
					       ko->keyId,
					       (U16)key_bit_len,
					       SE05X_RSA_NO_p,
					       SE05X_RSA_NO_q,
					       SE05X_RSA_NO_dp,
					       SE05X_RSA_NO_dq,
					       SE05X_RSA_NO_qInv,
					       key_pair->e,
					       key_pair->e_len,
					       SE05X_RSA_NO_priv,
					       SE05X_RSA_NO_pubMod,
					       type,
					       kSE05x_KeyPart_Pair,
					       rsa_format);
		if (status != SM_OK) {
			retval = kStatus_SSS_Fail;
			goto exit;
		}

		status = Se05x_API_WriteRSAKey(s_ctx,
					       NULL,
					       ko->keyId,
					       0,
					       SE05X_RSA_NO_p,
					       SE05X_RSA_NO_q,
					       SE05X_RSA_NO_dp,
					       SE05X_RSA_NO_dq,
					       SE05X_RSA_NO_qInv,
					       SE05X_RSA_NO_pubExp,
					       key_pair->d,
					       key_pair->d_len,
					       SE05X_RSA_NO_pubMod,
					       type,
					       kSE05x_KeyPart_NA,
					       rsa_format);
		if (status != SM_OK) {
			retval = kStatus_SSS_Fail;
			goto exit;
		}

		status = Se05x_API_WriteRSAKey(s_ctx,
					       NULL,
					       ko->keyId,
					       0,
					       SE05X_RSA_NO_p,
					       SE05X_RSA_NO_q,
					       SE05X_RSA_NO_dp,
					       SE05X_RSA_NO_dq,
					       SE05X_RSA_NO_qInv,
					       SE05X_RSA_NO_pubExp,
					       SE05X_RSA_NO_priv,
					       key_pair->n,
					       key_pair->n_len,
					       type,
					       kSE05x_KeyPart_NA,
					       rsa_format);
		if (status == SM_OK)
			retval = kStatus_SSS_Success;

		goto exit;
	}

	if (ko->cipherType == kSSS_CipherType_RSA_CRT) {
		status = Se05x_API_WriteRSAKey(s_ctx,
					       &policy,
					       ko->keyId,
					       (U16)key_bit_len,
					       key_pair->p,
					       key_pair->p_len,
					       SE05X_RSA_NO_q,
					       SE05X_RSA_NO_dp,
					       SE05X_RSA_NO_dq,
					       SE05X_RSA_NO_qInv,
					       SE05X_RSA_NO_pubExp,
					       SE05X_RSA_NO_priv,
					       SE05X_RSA_NO_pubMod,
					       type,
					       kSE05x_KeyPart_Pair,
					       rsa_format);
		if (status != SM_OK) {
			retval = kStatus_SSS_Fail;
			goto exit;
		}

		status = Se05x_API_WriteRSAKey(s_ctx,
					       NULL,
					       ko->keyId,
					       0,
					       SE05X_RSA_NO_p,
					       key_pair->q,
					       key_pair->q_len,
					       SE05X_RSA_NO_dp,
					       SE05X_RSA_NO_dq,
					       SE05X_RSA_NO_qInv,
					       SE05X_RSA_NO_pubExp,
					       SE05X_RSA_NO_priv,
					       SE05X_RSA_NO_pubMod,
					       type,
					       kSE05x_KeyPart_NA,
					       rsa_format);
		if (status != SM_OK) {
			retval = kStatus_SSS_Fail;
			goto exit;
		}

		status = Se05x_API_WriteRSAKey(s_ctx,
					       NULL,
					       ko->keyId,
					       0,
					       SE05X_RSA_NO_p,
					       SE05X_RSA_NO_q,
					       key_pair->dp,
					       key_pair->dp_len,
					       SE05X_RSA_NO_dq,
					       SE05X_RSA_NO_qInv,
					       SE05X_RSA_NO_pubExp,
					       SE05X_RSA_NO_priv,
					       SE05X_RSA_NO_pubMod,
					       type,
					       kSE05x_KeyPart_NA,
					       rsa_format);
		if (status != SM_OK) {
			retval = kStatus_SSS_Fail;
			goto exit;
		}

		status = Se05x_API_WriteRSAKey(s_ctx,
					       NULL,
					       ko->keyId,
					       0,
					       SE05X_RSA_NO_p,
					       SE05X_RSA_NO_q,
					       SE05X_RSA_NO_dp,
					       key_pair->dq,
					       key_pair->dq_len,
					       SE05X_RSA_NO_qInv,
					       SE05X_RSA_NO_pubExp,
					       SE05X_RSA_NO_priv,
					       SE05X_RSA_NO_pubMod,
					       type,
					       kSE05x_KeyPart_NA,
					       rsa_format);
		if (status != SM_OK) {
			retval = kStatus_SSS_Fail;
			goto exit;
		}

		status = Se05x_API_WriteRSAKey(s_ctx,
					       NULL,
					       ko->keyId,
					       0,
					       SE05X_RSA_NO_p,
					       SE05X_RSA_NO_q,
					       SE05X_RSA_NO_dp,
					       SE05X_RSA_NO_dq,
					       key_pair->qp,
					       key_pair->qp_len,
					       SE05X_RSA_NO_pubExp,
					       SE05X_RSA_NO_priv,
					       SE05X_RSA_NO_pubMod,
					       type,
					       kSE05x_KeyPart_NA,
					       rsa_format);
		if (status != SM_OK) {
			retval = kStatus_SSS_Fail;
			goto exit;
		}

		status = Se05x_API_WriteRSAKey(s_ctx,
					       NULL,
					       ko->keyId,
					       0,
					       SE05X_RSA_NO_p,
					       SE05X_RSA_NO_q,
					       SE05X_RSA_NO_dp,
					       SE05X_RSA_NO_dq,
					       SE05X_RSA_NO_qInv,
					       key_pair->e,
					       key_pair->e_len,
					       SE05X_RSA_NO_priv,
					       SE05X_RSA_NO_pubMod,
					       type,
					       kSE05x_KeyPart_NA,
					       rsa_format);
		if (status != SM_OK) {
			retval = kStatus_SSS_Fail;
			goto exit;
		}

		status = Se05x_API_WriteRSAKey(s_ctx,
					       NULL,
					       ko->keyId,
					       0,
					       SE05X_RSA_NO_p,
					       SE05X_RSA_NO_q,
					       SE05X_RSA_NO_dp,
					       SE05X_RSA_NO_dq,
					       SE05X_RSA_NO_qInv,
					       SE05X_RSA_NO_pubExp,
					       SE05X_RSA_NO_priv,
					       key_pair->n,
					       key_pair->n_len,
					       type,
					       kSE05x_KeyPart_NA,
					       rsa_format);

		if (status == SM_OK)
			retval = kStatus_SSS_Success;
	}
exit:
	return retval;
}

/*
 * @param store
 * @param ko
 * @param key_pair
 * @param key_pub
 *
 * @return sss_status_t
 */
sss_status_t sss_se05x_key_store_set_ecc_key_bin(sss_se05x_key_store_t *store,
						 sss_se05x_object_t *ko,
						 struct ecc_keypair_bin
						 *key_pair,
						 struct ecc_public_key_bin
						 *key_pub)
{
	SE05x_TransientType_t type = kSE05x_TransientType_Persistent;
	size_t public_keylen = 0;
	uint8_t buffer[256] = { 0x04 }; /* tag */
	uint8_t *public_key = buffer + 1;
	Se05xSession_t *s_ctx = NULL;
	Se05xPolicy_t policy = {
		.value = NULL, .value_len = 0,
	};
	sss_status_t retval = kStatus_SSS_Fail;
	smStatus_t status = SM_NOT_OK;

	if (!ko || !store || !store->session || (!key_pair && !key_pub))
		return retval;

	s_ctx = &store->session->s_ctx;
	ko->curve_id = key_pair ? key_pair->curve : key_pub->curve;
	type = ko->isPersistant ? kSE05x_TransientType_Persistent :
				  kSE05x_TransientType_Transient;

	if (ko->objectType != kSSS_KeyPart_Pair)
		goto label_public;

	public_keylen = key_pair->x_len + key_pair->y_len + 1;
	if (public_keylen > sizeof(buffer)) {
		EMSG("small buffer");
		goto exit;
	}
	memcpy(public_key, key_pair->x, key_pair->x_len);
	memcpy(public_key + key_pair->x_len, key_pair->y, key_pair->y_len);

	status = Se05x_API_WriteECKey(s_ctx,
				      &policy,
				      SE05x_MaxAttemps_UNLIMITED,
				      ko->keyId,
				      (SE05x_ECCurve_t)key_pair->curve,
				      key_pair->d,
				      key_pair->d_len,
				      buffer,
				      public_keylen,
				      type,
				      kSE05x_KeyPart_Pair);

	retval = status == SM_OK ? kStatus_SSS_Success : kStatus_SSS_Fail;
	goto exit;

label_public:
	if (ko->objectType != kSSS_KeyPart_Public)
		goto label_private;

	public_keylen = key_pub->x_len + key_pub->y_len + 1;
	if (public_keylen > sizeof(buffer)) {
		EMSG("small buffer");
		goto exit;
	}
	memcpy(public_key, key_pub->x, key_pub->x_len);
	memcpy(public_key + key_pub->x_len, key_pub->y, key_pub->y_len);

	status = Se05x_API_WriteECKey(s_ctx,
				      &policy,
				      SE05x_MaxAttemps_UNLIMITED,
				      ko->keyId,
				      (SE05x_ECCurve_t)key_pub->curve,
				      NULL,
				      0,
				      buffer,
				      public_keylen,
				      type,
				      kSE05x_KeyPart_Public);
	retval = status == SM_OK ? kStatus_SSS_Success : kStatus_SSS_Fail;
	goto exit;

label_private:
	if (ko->objectType != kSSS_KeyPart_Private)
		goto exit;

	status = Se05x_API_WriteECKey(s_ctx,
				      &policy,
				      SE05x_MaxAttemps_UNLIMITED,
				      ko->keyId,
				      (SE05x_ECCurve_t)key_pair->curve,
				      key_pair->d,
				      key_pair->d_len,
				      NULL,
				      0,
				      type,
				      kSE05x_KeyPart_Private);
	retval = status == SM_OK ? kStatus_SSS_Success : kStatus_SSS_Fail;
	goto exit;

exit:
	return retval;
}

/*
 * @param store
 * @param ko
 * @param key
 * @param keylen
 * @param pKeyBitLen
 *
 * @return sss_status_t
 */
sss_status_t sss_se05x_key_store_get_ecc_key_bin(sss_se05x_key_store_t *store,
						 sss_se05x_object_t *ko,
						 uint8_t *key,
						 size_t *keylen,
						 size_t *pKeyBitLen)
{
	Se05xSession_t *s_ctx = &store->session->s_ctx;
	sss_cipher_type_t cipher_type = ko->cipherType;
	sss_status_t retval = kStatus_SSS_Fail;
	smStatus_t status = SM_NOT_OK;

	switch (cipher_type) {
	case kSSS_CipherType_EC_NIST_P:
	case kSSS_CipherType_EC_NIST_K:
	case kSSS_CipherType_EC_BRAINPOOL:
	case kSSS_CipherType_EC_BARRETO_NAEHRIG:
	case kSSS_CipherType_EC_MONTGOMERY:
	case kSSS_CipherType_EC_TWISTED_ED: {
		uint8_t *key_buf = NULL;
		size_t key_buflen = 0;

		add_ecc_header(key, &key_buf, &key_buflen, ko->curve_id);
		status = Se05x_API_ReadObject(s_ctx, ko->keyId, 0, 0,
					      key_buf, keylen);
		if (status != SM_OK)
			goto exit;

		*keylen += key_buflen;

#if defined(DEBUG_ECC)
		LOG_AU8_I(key, *keylen);
#endif
		/* return only the binary data */
		key_buflen = *keylen;
		get_ecc_raw_data(key, &key_buf, &key_buflen, ko->curve_id);
		*keylen = key_buflen;
		memcpy(key, key_buf, key_buflen);

#if defined(DEBUG_ECC)
		LOG_AU8_I(key, key_buflen);
#endif
		break;
	}
	default:
		goto exit;
	}

	retval = kStatus_SSS_Success;
exit:
	return retval;
}

/*
 * @param context
 * @param srcData
 * @param srcLen
 * @param destData
 * @param destLen
 *
 * @return sss_status_t
 */
sss_status_t sss_se05x_cipher_update_nocache(sss_se05x_symmetric_t *context,
					     const uint8_t *srcData,
					     size_t srcLen,
					     uint8_t *destData, size_t *destLen)
{
	sss_status_t retval = kStatus_SSS_Fail;
	smStatus_t status = SM_NOT_OK;

	status = Se05x_API_CipherUpdate(&context->session->s_ctx,
					context->cryptoObjectId,
					srcData, srcLen,
					destData, destLen);
	if (status != SM_OK)
		goto exit;

	retval = kStatus_SSS_Success;
exit:
	if (retval == kStatus_SSS_Fail)
		*destLen = 0;

	return retval;
}

/*
 * @param pCtx
 *
 * @return sss_status_t
 */
sss_status_t se050_session_open(sss_se05x_ctx_t *pCtx, bool encryption)
{
	SE_Connect_Ctx_t *pConnectCtx = &pCtx->se05x_open_ctx;
	sss_se05x_session_t *pSession = &pCtx->session;
	sss_status_t status = kStatus_SSS_Fail;

	pConnectCtx->connType = kType_SE_Conn_Type_T1oI2C;
	pConnectCtx->portName = NULL;

	if (!encryption)
		return sss_se05x_session_open(pSession, kType_SSS_SE_SE05x, 0,
				      kSSS_ConnectionType_Plain, pConnectCtx);

	status = se050_configure_host(&pCtx->host_session,
				      &pCtx->host_ks,
				      &pCtx->se05x_open_ctx,
				      &pCtx->se05x_auth,
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
sss_status_t se050_factory_reset(sss_se05x_ctx_t *pCtx)
{
	sss_status_t status = kStatus_SSS_Fail;
	smStatus_t st = SM_OK;

	st = Se05x_API_DeleteAll_Iterative(&pCtx->session.s_ctx);
	if (st == SM_OK)
		status = kStatus_SSS_Success;

	return status;
}

/*
 * @param pCtx
 *
 * @return sss_status_t
 */
sss_status_t se050_kestore_and_object_init(sss_se05x_ctx_t *pCtx)
{
	sss_status_t status = kStatus_SSS_Fail;

	if (!pCtx)
		return status;

	status = sss_se05x_key_store_context_init(&pCtx->ks, &pCtx->session);
	if (status != kStatus_SSS_Success)
		EMSG(" sss_key_store_context_init Failed...");

	return status;
}

/*
 * Parse a DER formated signature and extract the raw data
 * @param p
 * @param p_len
 */
void se050_signature_der2bin(uint8_t *p, size_t *p_len)
{
	uint8_t buffer[256] = { 0 };
	uint8_t *k, *output = p;
	size_t buffer_len = 0;
	size_t len = 0;

	if (!p || !p_len)
		return;

	p++;		/* tag: 0x30      */
	p++;		/* field: total len */
	p++;		/* tag: 0x02      */
	len = *p++;	/* field: r_len */

	if (*p == 0x00) { /* handle special case */
		len = len - 1;
		p++;
	}
	memcpy(buffer, p, len);

	p = p + len;
	p++;		/* tag: 0x2       */
	k = p;
	p++;		/* field: s_len     */

	if (*p == 0x00) { /* handle special case */
		*k = *k - 1;
		p++;
	}
	memcpy(buffer + len, p, *k);
	buffer_len = len + *k;

	memcpy(output, buffer, buffer_len);
	*p_len = buffer_len;
}
