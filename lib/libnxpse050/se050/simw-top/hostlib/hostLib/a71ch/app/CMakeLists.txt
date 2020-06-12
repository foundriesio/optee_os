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

PROJECT(A71CHConfigTool)

FILE(
    GLOB
    ${PROJECT_NAME}_files
    *.c
    *.h
    ../../tstUtil/axCliUtil.c
    ../../tstUtil/axEccRefPem.c
)

FILE(
    GLOB
    imx_files
    configCliInteractive_ln.c
)

LIST(
    REMOVE_ITEM
    ${PROJECT_NAME}_files
    ${imx_files}
)

ADD_EXECUTABLE(${PROJECT_NAME} ${KSDK_STARTUP_FILE} ${${PROJECT_NAME}_files})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} a7x_utils)

CREATE_BINARY(${PROJECT_NAME})

ADD_CUSTOM_COMMAND(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_SOURCE_DIR}/hostlib/hostLib/embSeEngine/bin
    COMMENT "Copy config tool exe to ${CMAKE_SOURCE_DIR}/hostlib/hostLib/embSeEngine/bin"
)

IF(
    "${CMAKE_CXX_COMPILER_ID}"
    MATCHES
    "MSVC"
)
    IF(NXPInternal)
        TARGET_COMPILE_OPTIONS(
            ${PROJECT_NAME}
            PRIVATE /wd4267 # '=': conversion from 'size_t' to 'int', possible loss of data
            PRIVATE /wd4701 #  potentially uninitialized local variable used
        )
    ENDIF()
ENDIF()

IF(WithHost_LinuxLike)
    INSTALL(TARGETS ${PROJECT_NAME} DESTINATION bin)
ENDIF()
