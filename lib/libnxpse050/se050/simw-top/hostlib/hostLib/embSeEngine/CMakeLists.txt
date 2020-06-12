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

PROJECT(a71ch_engine_legacy)

FILE(
    GLOB
    ${PROJECT_NAME}_files
    src/ax_a71chEngine.c
    src/ax_cryptoIpc.c
)

ADD_LIBRARY(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_files})

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    a71ch
    a7x_utils
    SSS_APIs
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OPENSSL_LIBRARIES})

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC inc)

CREATE_BINARY(${PROJECT_NAME})

ADD_CUSTOM_COMMAND(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_SOURCE_DIR}/hostlib/hostLib/embSeEngine/bin
    COMMENT "Copy dll to ${CMAKE_SOURCE_DIR}/hostlib/hostLib/embSeEngine/bin"
)
