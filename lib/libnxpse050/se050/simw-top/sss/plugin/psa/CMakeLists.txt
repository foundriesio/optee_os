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

PROJECT(PSA_ALT)

FILE(
    GLOB
    ${PROJECT_NAME}_files
    #### src/psa_alt.c
    src/psa_alt_utils.c
    #### src/sss_psa_alt.c
    #### src/veneer_table.c
)

IF(WithHost_lpcxpresso55s_s)
    LIST(
        APPEND
        ${PROJECT_NAME}_files
        src/veneer_table.c
        src/sss_psa_alt.c
        src/psa_secure.c
    )

ELSEIF(WithHost_lpcxpresso55s_ns)
    LIST(
        APPEND
        ${PROJECT_NAME}_files
        src/psa_alt_veneer.c
    )

ELSE()
    LIST(
        APPEND
        ${PROJECT_NAME}_files
        src/psa_alt.c
        src/sss_psa_alt.c
    )

ENDIF()

IF(WithHost_lpcxpresso55s_s)
    ADD_EXECUTABLE(${PROJECT_NAME} ${KSDK_STARTUP_FILE} ${${PROJECT_NAME}_files})
ELSE()
    ADD_LIBRARY(${PROJECT_NAME} ${${PROJECT_NAME}_files})
ENDIF()

IF(WithHost_PCWindows)
    # FIXME: Only required for testing
    ADD_DEFINITIONS(-DEX_SSS_BOOT_DO_ERASE=1)
ENDIF()

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    ex_common
    SSS_APIs
)

IF(WithApplet_SE05X)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} se05x)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} a7x_utils)
ENDIF()

TARGET_INCLUDE_DIRECTORIES(
    ${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/sss/ex/inc
    PRIVATE ${CMAKE_SOURCE_DIR}/ext/mbed-crypto/include
    PUBLIC inc
)

IF(WithHost_lpcxpresso55s_s)
    CREATE_BINARY(${PROJECT_NAME})
ENDIF()

#### PROJECT(psa_interface)
#### ADD_LIBRARY(psa_interface INTERFACE)
####
#### TARGET_INCLUDE_DIRECTORIES(psa_interface INTERFACE inc)
