/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 * Author: Jorge Ramirez <jorge@foundries.io>
 */

#ifndef SE050_CORE_KEY_H_
#define SE050_CORE_KEY_H_

#include <se050.h>

struct rsa_keypair;
struct ecc_keypair;

uint32_t se050_rsa_keypair_from_nvm(struct rsa_keypair *key);
uint32_t se050_ecc_keypair_from_nvm(struct ecc_keypair *key);
uint64_t se050_generate_private_key(uint32_t oid);

sss_status_t se050_get_oid(sss_key_object_mode_t type, uint32_t *val);
void se050_cleanup_all_persistent_objects(void);

#endif
