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

MACRO(DO_PROVISOING_PROJECT PROVISOING_PROJECT_NAME)

    PROJECT(cloud_provisioning_${PROVISOING_PROJECT_NAME})

    FILE(
        GLOB
        headers
        *.h
    )

    ADD_EXECUTABLE(
        ${PROJECT_NAME}
        ${KSDK_STARTUP_FILE}
        ${headers}
        provisioning.c
        provisioning_${PROVISOING_PROJECT_NAME}.c
    )

    TARGET_INCLUDE_DIRECTORIES(
        ${PROJECT_NAME}
        PRIVATE ${CMAKE_SOURCE_DIR}/sss/ex/inc
        PRIVATE ../common
    )

    TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE ../${PROVISOING_PROJECT_NAME})

    TARGET_LINK_LIBRARIES(
        ${PROJECT_NAME}
        ex_common
        a7x_utils
    )

    IF(WithRTOS_FreeRTOS)
        TARGET_LINK_LIBRARIES(${PROJECT_NAME} freertos-ip)
    ENDIF()

    CREATE_BINARY(${PROJECT_NAME})
ENDMACRO()

DO_PROVISOING_PROJECT(aws)
DO_PROVISOING_PROJECT(gcp)
DO_PROVISOING_PROJECT(azure)
DO_PROVISOING_PROJECT(ibm_watson)
