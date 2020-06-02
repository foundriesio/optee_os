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

PROJECT(cloud_demo_ibm_watson)

FILE(
    GLOB
    files
    ibm_client_credential_keys.h
    ibm_watson_default_root_certificate.h
    ibm_watson_demo_Main_lwip.c
    ibm_watson_iot_config.h
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
        ibm_watson_demo_Main_x86.c
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
