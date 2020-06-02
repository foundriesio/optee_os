# Copyright 2019,2020 NXP
#
# This software is owned or controlled by NXP and may only be used
# strictly in accordance with the applicable license terms.  By expressly
# accepting such terms or by downloading, installing, activating and/or
# otherwise using the software, you are agreeing that you have read, and
# that you agree to comply with and are bound by, such license terms.  If
# you do not agree to be bound by the applicable license terms, then you
# may not retain, install, activate or otherwise use the software.
#

PROJECT(cloud_demo_gcp)

FILE(
    GLOB
    files
    gcpJWTDemo.c
    gcpJWTDemo.h
    gcpJWTDemo_Main_lwip.c
    gcpNTPGetEpoch_lwip.c
    gcpPubSub.c
    gcp_client_credential_keys.h
    gcp_default_root_certificates.h
    gcp_iot_config.h
    ../common/ledHandler.c
    ../common/*.h
)

IF(WithmbedTLS_ALT_SSS AND (WithApplet_A71XX OR WithApplet_SE050_EAR))
    LIST(
        APPEND
        files
        ${CMAKE_SOURCE_DIR}/sss/ex/src/ex_sss_a71ch.c
    )
ENDIF()

IF(WithHost_PCWindows)
    LIST(
        APPEND
        files
        gcpJWTDemo_Main_x86.c
        gcpJwtTask_lwip.c
        gcpJwtTask_x86.c
        gcpNTPGetEpoch_time.c
    )
ENDIF()

ADD_EXECUTABLE(${PROJECT_NAME} ${KSDK_STARTUP_FILE} ${files})

TARGET_INCLUDE_DIRECTORIES(
    ${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/sss/ex/inc
    PRIVATE ../common/https_client ../common
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    freertos-ip
    ex_common
)
CREATE_BINARY(${PROJECT_NAME})

IF(
    "${CMAKE_CXX_COMPILER_ID}"
    MATCHES
    "MSVC"
)
    IF(NXPInternal)
        TARGET_COMPILE_OPTIONS(${PROJECT_NAME} PRIVATE /wd4127 # conditional expression is constant
        )
    ENDIF()
ENDIF()
