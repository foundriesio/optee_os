/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 * Author: Jorge Ramirez <jorge@foundries.io>
 */

#ifndef SE050_CORE_UTILS_H_
#define SE050_CORE_UTILS_H_

#include <stdlib.h>

static inline void se050_refcount_init_ctx(uint8_t **cnt)
{
	if (!*cnt) {
		*cnt = calloc(1, sizeof(uint8_t));
		if (*cnt)
			**cnt = 1;
	} else {
		**cnt = **cnt + 1;
	}
}

static inline int se050_refcount_final_ctx(uint8_t *cnt)
{
	if (!cnt)
		return 1;

	if (!*cnt) {
		free(cnt);
		return 1;
	}

	*cnt = *cnt - 1;

	return 0;
}

#endif
