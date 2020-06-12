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

PROJECT(cloud_demo_aws)

FILE(
    GLOB
    files
    aws_cert.c
    ../common/ledHandler.c
    ../../../sss/ex/inc/*.h
    *.h
    ../common/*.h
)

IF(WithmbedTLS_ALT_SSS AND (WithApplet_A71XX OR WithApplet_SE050_EAR))
    LIST(
        APPEND
        files
        ${CMAKE_SOURCE_DIR}/sss/ex/src/ex_sss_a71ch.c
    )
ENDIF()

IF(WithKSDK)
    FILE(
        GLOB
        lwip_files
        aws_jitr_demo_Main_lwip.c
        aws_jitr_task_lwip.c
    )
ENDIF()
IF(WithHost_PCWindows)
    FILE(
        GLOB
        x86_files
        aws_jitr_demo_Main_x86.c
        aws_jitr_task_lwip.c
    )
ENDIF()

ADD_EXECUTABLE(
    ${PROJECT_NAME}
    ${KSDK_STARTUP_FILE}
    ${files}
    ${lwip_files}
    ${x86_files}
)

TARGET_INCLUDE_DIRECTORIES(
    ${PROJECT_NAME}
    PRIVATE ../common
    PRIVATE ../../../sss/ex/inc ../https_client
    PRIVATE .
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    a7x_utils
    freertos-ip
    freertos-kernel
    ex_common
)

IF(WithApplet_A71CH OR WithApplet_A71CH_SIM)
    TARGET_LINK_LIBRARIES(
        ${PROJECT_NAME}
        a71ch
        a7x_utils
    )
ENDIF()

CREATE_BINARY(${PROJECT_NAME})
