/* Copyright 2019,2020 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef NX_LOG_ANDROID_H
#define NX_LOG_ANDROID_H

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <nxLog.h>

#ifdef ANDROID

#include <inttypes.h>
#include <android/log.h>

#define LOG_TAG "NXPKeymasterDevice"

#if NX_LOG_SHORT_PREFIX
static const char *szLevel[] = {"D", "I", "W", "E"};
#else
static const char *szLevel[] = {"DEBUG", "INFO ", "WARN ", "ERROR"};
#endif

#define NXP_LOG_D(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define NXP_LOG_I(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define NXP_LOG_W(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define NXP_LOG_E(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

void nLog(const char *comp, int level, const char *format, ...)
{
    va_list vArgs;
    char buffer[256];
    size_t size_buff = sizeof(buffer) / sizeof(buffer[0]) - 1;
    va_start(vArgs, format);
    vsnprintf(buffer, size_buff, format, vArgs);

    switch (level) {
    case NX_LEVEL_DEBUG:
        NXP_LOG_D("%s: %s", comp, buffer);
        break;
    case NX_LEVEL_INFO:
        NXP_LOG_I("%s: %s", comp, buffer);
        break;
    case NX_LEVEL_WARN:
        NXP_LOG_W("%s: %s", comp, buffer);
        break;
    case NX_LEVEL_ERROR:
        NXP_LOG_E("%s: %s", comp, buffer);
        break;
    }
    va_end(vArgs);
}

void nLog_au8(const char *comp, int level, const char *message, const unsigned char *array, size_t array_len)
{
    uint32_t i;
    char print_buffer[array_len * 3 + 1];

    memset(print_buffer, 0, sizeof(print_buffer));
    for (i = 0; i < array_len; i++) {
        snprintf(&print_buffer[i * 2], 3, "%02X", array[i]);
    }
    switch (level) {
    case NX_LEVEL_DEBUG:
        NXP_LOG_D("%s:%s (Len=%zu) %s", comp, message, array_len, print_buffer);
        break;
    case NX_LEVEL_INFO:
        NXP_LOG_I("%s:%s (Len=%zu) %s", comp, message, array_len, print_buffer);
        break;
    case NX_LEVEL_WARN:
        NXP_LOG_W("%s:%s (Len=%zu) %s", comp, message, array_len, print_buffer);
        break;
    case NX_LEVEL_ERROR:
        NXP_LOG_E("%s:%s (Len=%zu) %s", comp, message, array_len, print_buffer);
        break;
    }
}

/* clang-format on */

#endif /* ANDROID */

#endif /* NX_LOG_ANDROID_H */
