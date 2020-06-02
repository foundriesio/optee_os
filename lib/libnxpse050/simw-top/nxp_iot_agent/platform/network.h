/*
 * Copyright 2018, 2019, 2020 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 */
#ifndef _NXP_IOT_AGENT_PLATFORM_NETWORK_H_
#define _NXP_IOT_AGENT_PLATFORM_NETWORK_H_

#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

#define NETWORK_STATUS_OK    0
#define NETWORK_STATUS_FAIL -1

#define NETWORK_EXIT_STATUS_MSG(STATUS, ...)                      \
	network_status = STATUS;                                      \
	IOT_AGENT_ERROR(__VA_ARGS__);                                 \
	goto exit;                                                    \

#define NETWORK_ASSERT_OR_EXIT_STATUS_MSG(CONDITION, STATUS, ...) \
if (!(CONDITION))                                                 \
{                                                                 \
	network_status = STATUS;                                      \
	IOT_AGENT_ERROR(__VA_ARGS__);                                 \
	goto exit;                                                    \
}

#define NETWORK_ASSERT_OR_EXIT_MSG(CONDITION, ...)                \
NETWORK_ASSERT_OR_EXIT_STATUS_MSG(CONDITION, NETWORK_STATUS_FAIL, __VA_ARGS__)

#define NETWORK_ASSERT_OR_EXIT_STATUS(CONDITION, STATUS)          \
NETWORK_ASSERT_OR_EXIT_STATUS_MSG(CONDITION, STATUS, "assert failed")

#define NETWORK_ASSERT_OR_EXIT(CONDITION)                         \
NETWORK_ASSERT_OR_EXIT_STATUS_MSG(CONDITION, NETWORK_STATUS_FAIL, "assert failed")


typedef struct iot_agent_context_t iot_agent_context_t;

void* network_new();
void network_free(void* context);

int network_configure(void* context, void* network_config);

int network_connect(void* context);
int network_disconnect(void* context);
int network_read(void* context, uint8_t* buffer, size_t len);
int network_write(void* context, const uint8_t* buffer, size_t len);

int network_verify_server_certificate(void* context, uint8_t* trusted_bytes, size_t trusted_size,
	uint8_t* crl_bytes, size_t crl_size, uint32_t* error);


#ifdef __cplusplus
}
#endif

#endif // _NXP_IOT_AGENT_PLATFORM_NETWORK_H_
