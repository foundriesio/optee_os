# Copyright 2020 NXP
#
# This software is owned or controlled by NXP and may only be used
# strictly in accordance with the applicable license terms.  By expressly
# accepting such terms or by downloading, installing, activating and/or
# otherwise using the software, you are agreeing that you have read, and
# that you agree to comply with and are bound by, such license terms.  If
# you do not agree to be bound by the applicable license terms, then you
# may not retain, install, activate or otherwise use the software.
#

PROJECT(psa_nonsecure)
FILE(
    GLOB
    files
    *.c
)

ADD_EXECUTABLE(${PROJECT_NAME} ${KSDK_STARTUP_FILE} ${files})

TARGET_INCLUDE_DIRECTORIES(
    ${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/sss/ex/inc
    PRIVATE ${CMAKE_SOURCE_DIR}/sss/plugin/psa/inc
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/binaries/PSA_ALT_CMSE_lib.o
    SSS_APIs
    ex_common
    mbedtls
    PSA_ALT
)

CREATE_BINARY(${PROJECT_NAME})
