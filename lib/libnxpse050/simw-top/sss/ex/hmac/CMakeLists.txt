# Copyright 2019 NXP
#
# This software is owned or controlled by NXP and may only be used
# strictly in accordance with the applicable license terms.  By expressly
# accepting such terms or by downloading, installing, activating and/or
# otherwise using the software, you are agreeing that you have read, and
# that you agree to comply with and are bound by, such license terms.  If
# you do not agree to be bound by the applicable license terms, then you
# may not retain, install, activate or otherwise use the software.
#

PROJECT(ex_hmac)

FILE(
    GLOB
    ${PROJECT_NAME}_files
    *.c
    *.h
)

ADD_EXECUTABLE(${PROJECT_NAME} ${KSDK_STARTUP_FILE} ${${PROJECT_NAME}_files})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ex_common)

CREATE_BINARY(${PROJECT_NAME})
