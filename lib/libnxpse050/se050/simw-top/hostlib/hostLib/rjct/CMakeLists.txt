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

PROJECT(jrcpv1_server)

FILE(
    GLOB
    jrcpv1_server_sources
    inc/*.h
    src/*.c
)

ADD_EXECUTABLE(${PROJECT_NAME} ${KSDK_STARTUP_FILE} ${jrcpv1_server_sources})

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC inc)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} smCom)

IF(SSS_HAVE_SCP_SCP03_SSS)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} SSS_APIs)
ENDIF()

CREATE_BINARY(${PROJECT_NAME})

IF(WithHost_LinuxLike)
    INSTALL(TARGETS ${PROJECT_NAME} DESTINATION bin)
ENDIF()
