// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 * Author: Jorge Ramirez <jorge@foundries.io>
 */

#include <fsl_sss_api.h>
#include <kernel/delay.h>

static unsigned long next = 1;

sss_status_t sm_sleep(uint32_t ms)
{
	mdelay(ms);

	return kStatus_SSS_Success;
}

int rand(void)
{
	next = next * 1103515245L + 12345;

	return (unsigned int)(next / 65536L) % 32768L;
}

void srand(unsigned int seed)
{
	next = seed;
}

unsigned int time(void *foo)
{
	static int time = 1;

	return time++;
}
