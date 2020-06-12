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

IF(CMAKE_COMPILER_IS_GNUCC)
    ADD_DEFINITIONS("-Wno-unused-function -Wno-format")
ENDIF()

PROJECT(apdu_player_demo)

ADD_EXECUTABLE(${PROJECT_NAME} apdu_player.c)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    SSS_APIs
    ex_common
    smCom
)

CREATE_BINARY(${PROJECT_NAME})

IF(WithHost_LinuxLike)
    INSTALL(TARGETS ${PROJECT_NAME} DESTINATION bin)
ENDIF()
