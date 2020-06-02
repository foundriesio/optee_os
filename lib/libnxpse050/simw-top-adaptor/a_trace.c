// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 * Author: Jorge Ramirez <jorge@foundries.io>
 */

#include "a_trace.h"

#if (TRACE_LEVEL >= TRACE_INFO)
struct strbuf {
	char buf[MAX_PRINT_SIZE];
	char *ptr;
};

#define MIN(a, b)				\
(__extension__({ __typeof__(a) _a = (a);	\
__typeof__(b) _b = (b);				\
_a < _b ? _a : _b; }))

static int __printf(2, 3) append(struct strbuf *sbuf, const char *fmt, ...)
{
	int left;
	int len;
	va_list ap;

	if (!sbuf->ptr)
		sbuf->ptr = sbuf->buf;

	left = sizeof(sbuf->buf) - (sbuf->ptr - sbuf->buf);
	va_start(ap, fmt);
	len = vsnprintk(sbuf->ptr, left, fmt, ap);
	va_end(ap);
	if (len < 0) {
		/* Format error */
		return 0;
	}
	if (len >= left) {
		/* Output was truncated */
		return 0;
	}
	sbuf->ptr += MIN(left, len);
	return 1;
}

void hex_dump(const char *function, int line, int level, const void *buf,
	      int len)
{
	int i;
	int ok;
	struct strbuf sbuf;
	char *in = (char *)buf;

	if (level <= trace_level) {
		sbuf.ptr = NULL;
		for (i = 0; i < len; i++) {
			if ((i % 16) == 0) {
				ok = append(&sbuf, "%0*" PRIxVA "  ",
					    PRIxVA_WIDTH, (vaddr_t)(in + i));
				if (!ok)
					goto err;
			}
			ok = append(&sbuf, "%02x ", in[i]);
			if (!ok)
				goto err;
			if ((i % 16) == 7) {
				ok = append(&sbuf, " ");
				if (!ok)
					goto err;
			} else if ((i % 16) == 15) {
				trace_printf(function, line, level, true, "%s",
					     sbuf.buf);
				sbuf.ptr = NULL;
			}
		}
		if (sbuf.ptr) {
			/* Buffer is not empty: flush it */
			trace_printf(function, line, level, true, "%s",
				     sbuf.buf);
		}
	}
	return;
err:
	DMSG("Hex dump error");
}

#else
void hex_dump(const char *function __unused, int line __unused,
	      int level __unused,
	      const void *buf __unused, int len __unused)
{
}
#endif
