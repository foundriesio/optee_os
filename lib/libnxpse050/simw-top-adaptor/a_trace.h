/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (C) Foundries Ltd. 2020 - All Rights Reserved
 * Author: Jorge Ramirez <jorge@foundries.io>
 */

#ifndef _A_TRACE_H
#define _A_TRACE_H

#include <compiler.h>
#include <printk.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <trace.h>
#include <trace_levels.h>

#if (TRACE_LEVEL < TRACE_INFO)
#define LOG_I(...) ((void)0)
#else
#define LOG_I(...) trace_printf_helper(TRACE_INFO, true, __VA_ARGS__)
#endif

#if (TRACE_LEVEL < TRACE_ERROR)
#define LOG_E(...) ((void)0)
#define LOG_W(...) ((void)0)
#else
#define LOG_E(...) trace_printf_helper(TRACE_ERROR, true, __VA_ARGS__)
#define LOG_W(...) trace_printf_helper(TRACE_INFO, true, __VA_ARGS__)
#endif

/* LOG_D is too intensive */
#if (TRACE_LEVEL < TRACE_MAX)
#define LOG_D(...) ((void)0)
#else
#define LOG_D(...) trace_printf_helper(TRACE_ERROR, true, __VA_ARGS__)
#endif

#ifndef errno
#define errno 0
#endif

/* TODO */
#define LOG_MAU8_D(...) ((void)0)
#define LOG_MAU8_E(...) ((void)0)
#define LOG_AU8_D(...) ((void)0)
#define LOG_AU8_E(...) ((void)0)

void hex_dump(const char *function __unused, int line __unused,
	      int level __unused,
	      const void *buf __unused, int len __unused);

#if (TRACE_LEVEL < TRACE_INFO)
#define LOG_MAU8_I(...) ((void)0)
#define LOG_AU8_I(...) ((void)0)
#else
#define LOG_MAU8_I(msg, buf, len)				\
	do {							\
		LOG_I(msg);					\
		hex_dump("\t", 0, TRACE_INFO, buf, len);	\
	} while (0)
#define LOG_AU8_I(buf, len) hex_dump("", 0, TRACE_INFO, buf, len)
#endif
#endif
