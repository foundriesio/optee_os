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

PROJECT(MAL_AGENT_APIs LANGUAGES C)


FILE(
    GLOB
        API_FILES
        lib/inc/*.h
        lib/src/*.c
        lib/src/*.h
)

IF(WithHost_Android)
    ADD_LIBRARY(${PROJECT_NAME} SHARED ${API_FILES})
ELSE()
    ADD_LIBRARY(${PROJECT_NAME} ${API_FILES})
ENDIF()

TARGET_INCLUDE_DIRECTORIES(
    ${PROJECT_NAME}
    PUBLIC lib/inc
    PUBLIC ../nxp_iot_agent/src
    PUBLIC ../nxp_iot_agent/inc
    PUBLIC ../nxp_iot_agent/src/protobuf/
    PUBLIC ../hostlib/hostlib/inc
    PUBLIC ../hostlib/hostlib/libCommon
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    SSS_APIs
    nxp_iot_agent_common
)

IF(WithHost_LinuxLike)
    INSTALL(TARGETS ${PROJECT_NAME} DESTINATION lib)
ENDIF()

IF(WithLargePrograms)
    ADD_SUBDIRECTORY(ex)
ENDIF()

IF(WithRTOS_Default AND NXPInternal)
    IF(
        WithHost_frdmk64f
        OR WithHost_lpcxpresso55s
        OR WithHost_evkbimxrt1050
        OR WithHost_lpcxpresso55s_s
        OR WithHost_lpcxpresso55s_ns
    )
        # Not yet. Due to memory limitation.
    ELSE()
        ADD_SUBDIRECTORY(tst)
    ENDIF()
ENDIF()



IF(WithHost_LinuxLike)
    INSTALL(
        DIRECTORY
            inc/
            port/default/
        DESTINATION ${SIMW_INSTALL_INC_DIR}
        FILES_MATCHING
        PATTERN "*.h"
    )
ENDIF()
