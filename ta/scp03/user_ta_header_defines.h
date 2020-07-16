/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (c) 2020, Foundries.IO */

#ifndef __USER_TA_HEADER_DEFINES_H
#define __USER_TA_HEADER_DEFINES_H

#include <ta_scp03.h>

#define TA_UUID				TA_SCP03_UUID

#define TA_FLAGS			(TA_FLAG_SINGLE_INSTANCE)

#define TA_STACK_SIZE			(16 * 1024)
#define TA_DATA_SIZE			(16 * 1024)

#endif /*__USER_TA_HEADER_DEFINES_H*/
