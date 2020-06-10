/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 * Author: Jorge Ramirez <jorge@foundries.io>
 */

#ifndef _APIS_H
#define _APIS_H

#include <fsl_sss_se05x_types.h>
#include <nxScp03_Types.h>

/*
 * Context management
 */
typedef struct {
	SE_Connect_Ctx_t se05x_open_ctx;
	sss_se05x_session_t session;
	sss_se05x_key_store_t ks;

	/* scp support*/
	struct SE_Auth_Ctx {
		NXSCP03_StaticCtx_t static_ctx;
		NXSCP03_DynCtx_t dynamic_ctx;
	} se05x_auth;
	sss_session_t host_session;
	sss_key_store_t host_ks;
} sss_se05x_ctx_t;

sss_status_t se050_kestore_and_object_init(sss_se05x_ctx_t *pCtx);
sss_status_t se050_factory_reset(sss_se05x_ctx_t *pCtx);
sss_status_t se050_session_open(sss_se05x_ctx_t *pCtx);
sss_status_t se050_configure_host(sss_session_t *host_session,
				  sss_key_store_t *host_ks,
				  SE_Connect_Ctx_t *se05x_open_ctx,
				  struct SE_Auth_Ctx *se05x_auth_ctx,
				  SE_AuthType_t auth_type);
/* requires a context restart */
void se050_display_board_info(sss_se05x_session_t *session);

uint8_t se050_key_exists(uint32_t keyId, pSe05xSession_t session_ctx);
sss_status_t se050_get_freemem(pSe05xSession_t session_ctx,
			       uint16_t *t, uint16_t *p);

/*
 * Cipher
 */
sss_status_t sss_se05x_cipher_update_nocache(sss_se05x_symmetric_t *context,
					     const uint8_t *srcData,
					     size_t srcLen, uint8_t *destData,
					     size_t *destLen);

/*
 * DER utils
 */
void se050_signature_der2bin(uint8_t *p, size_t *p_len);

/*
 * Key Store operations
 */
struct rsa_keypair_bin {
	uint8_t *e;		/* Public exponent */
	size_t e_len;
	uint8_t *d;		/* Private exponent */
	size_t d_len;
	uint8_t *n;		/* Modulus */
	size_t n_len;

	/* Optional CRT parameters (all NULL if unused) */
	uint8_t *p;		/* N = pq */
	size_t p_len;
	uint8_t *q;
	size_t q_len;
	uint8_t *qp;		/* 1/q mod p */
	size_t qp_len;
	uint8_t *dp;		/* d mod (p-1) */
	size_t dp_len;
	uint8_t *dq;		/* d mod (q-1) */
	size_t dq_len;
};

struct rsa_public_key_bin {
	uint8_t *e;		/* Public exponent */
	size_t e_len;
	uint8_t *n;		/* Modulus */
	size_t n_len;
};

struct ecc_public_key_bin {
	uint8_t *x;		/* Public value x */
	size_t x_len;
	uint8_t *y;		/* Public value y */
	size_t y_len;
	uint32_t curve;	        /* Curve type */
};

struct ecc_keypair_bin {
	uint8_t *d;		/* Private value */
	size_t d_len;
	uint8_t *x;		/* Public value x */
	size_t x_len;
	uint8_t *y;		/* Public value y */
	size_t y_len;
	uint32_t curve;	        /* Curve type */
};

sss_status_t sss_se05x_key_store_set_rsa_key_bin(sss_se05x_key_store_t
						 *keyStore,
						 sss_se05x_object_t *keyObject,
						 struct rsa_keypair_bin
						 *key_pair,
						 struct rsa_public_key_bin
						 *key_pub,
						 size_t keyBitLen);

sss_status_t sss_se05x_key_store_set_ecc_key_bin(sss_se05x_key_store_t
						 *keyStore,
						 sss_se05x_object_t *keyObject,
						 struct ecc_keypair_bin
						 *key_pair,
						 struct ecc_public_key_bin
						 *key_pub);

sss_status_t sss_se05x_key_store_get_ecc_key_bin(sss_se05x_key_store_t
						 *keyStore,
						 sss_se05x_object_t *keyObject,
						 uint8_t *key,
						 size_t *keylen,
						 size_t *pKeyBitLen);
#endif
