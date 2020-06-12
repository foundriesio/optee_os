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

PROJECT(sss_engine)

FILE(
    GLOB
    ${PROJECT_NAME}_files
    engine/src/ax_sssEngine_main.c
    engine/src/ax_sssEngine_rand.c
    engine/src/ax_sssEngine_ecc.c
    engine/src/ax_cryptoIpc.c
)

IF(
    WithApplet_SE05X
    OR WithHostCrypto_MBEDTLS
    AND WithHostCrypto_OPENSSL
)
    LIST(
        APPEND
        ${PROJECT_NAME}_files
        engine/src/ax_sssEngine_rsa.c
    )
ENDIF()

ADD_LIBRARY(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_files})

IF(WithApplet_SE05X)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} se05x)
ENDIF()

IF(WithApplet_A71CH OR WithApplet_A71CH_SIM)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} a71ch)
ENDIF()

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    ex_common
    SSS_APIs
)

IF(
    WithApplet_SE05X
    OR WithApplet_SE050_EAR
    OR WithApplet_A71XX
)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} a7x_utils)
ENDIF()

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OPENSSL_LIBRARIES})

TARGET_INCLUDE_DIRECTORIES(
    ${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/sss/ex/inc
    PUBLIC engine/inc
)

IF(
    "${CMAKE_SYSTEM_NAME}"
    STREQUAL
    "WindowsStore"
)
    SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY VS_WINRT_COMPONENT TRUE)
ENDIF()

IF(WithHost_Win10IoT)
    SET_TARGET_PROPERTIES(
        ${PROJECT_NAME}
        PROPERTIES LINK_FLAGS_RELEASE
                   "/defaultlib:vccorlib.lib /defaultlib:msvcrt.lib /wholearchive:smComWin10IoTWrapper.lib"
    )
    SET_TARGET_PROPERTIES(
        ${PROJECT_NAME}
        PROPERTIES LINK_FLAGS_DEBUG
                   "/defaultlib:vccorlibd.lib /defaultlib:msvcrtd.lib /wholearchive:smComWin10IoTWrapper.lib"
    )
ENDIF()

ADD_CUSTOM_COMMAND(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_SOURCE_DIR}/sss/plugin/openssl/bin
    COMMENT "Copy dll to ${CMAKE_SOURCE_DIR}/sss/plugin/openssl/bin"
)

IF(WithHost_LinuxLike)
    INSTALL(TARGETS ${PROJECT_NAME} DESTINATION lib)
ENDIF()
