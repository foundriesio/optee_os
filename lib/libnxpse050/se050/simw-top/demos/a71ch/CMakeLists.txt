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

PROJECT(ex_a71ch_sss_ecc)

ADD_EXECUTABLE(${PROJECT_NAME} ${KSDK_STARTUP_FILE} ${PROJECT_NAME}.c)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    SSS_APIs
    ex_common
    a7x_utils
    a71ch
)

CREATE_BINARY(${PROJECT_NAME})

PROJECT(ex_a71ch_sss_aes_wrap_key)

ADD_EXECUTABLE(${PROJECT_NAME} ${KSDK_STARTUP_FILE} ${PROJECT_NAME}.c)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    SSS_APIs
    ex_common
    a7x_utils
    a71ch
)

CREATE_BINARY(${PROJECT_NAME})

IF(NXPInternal)
    ADD_SUBDIRECTORY(tst)
ENDIF(NXPInternal)
