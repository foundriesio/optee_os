/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 * Author: Jorge Ramirez <jorge@foundries.io>
 */

#ifndef SE050_CORE_KEY_CRYPTO_H_
#define SE050_CORE_KEY_CRYPTO_H_

/* search for the key in a buffer of size len*/
void se050_delete_persistent_key(uint8_t *data, size_t len);

#endif
