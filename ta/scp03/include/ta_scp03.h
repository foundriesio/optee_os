/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (c) 2020, Foundries.IO */

#ifndef __TA_SCP03_H
#define __TA_SCP03_H

#define TA_SCP03_UUID { 0x86a5c8d3, 0x3961, 0x4606, \
		      { 0xb9, 0x0e, 0xc1, 0x16, 0x49, 0x9d, 0x1b, 0x54 } }

/*
 * Enable SCP03 support on the SE. Rotate scp03 keys if requested
 *
 * in	params[0].a = operation

 */
#define TA_CRYPT_CMD_ENABLE_SCP03		0

#endif /*__TA_SCP03_H*/
