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

PROJECT(ex_Ev2DivChngKey_se05x)
FILE(
    GLOB
    files
    *.c
    *.h
    ../common/src/se05x_MfDfInit.c
    ../common/src/nfc_support.c
    ../common/src/np_mfdef.c
    ../common/src/np_NxpNfcRdLib.c
    ../common/src/phUtils.c
    ../common/src/phExMfDfCrypto.c
    ../common/src/phExMfDfCrypto_se05x.c
    ../common/src/se05x_ExHelper.c
)

IF(
    WithHost_frdmk64f
    OR WithHost_lpcxpresso55s
    OR WithHost_evkbimxrt1050
)
    FILE(
        GLOB
        mcu_files
        ../common/src/phApp_Init.c
    )
ENDIF()

ADD_EXECUTABLE(
    ${PROJECT_NAME}
    ${KSDK_STARTUP_FILE}
    ${files}
    ${mcu_files}
)

ADD_DEFINITIONS("-DRDR_LIB_PARAM_CHECK")

TARGET_INCLUDE_DIRECTORIES(
    ${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/sss/ex/inc
    PRIVATE ./intfs
    PRIVATE ${CMAKE_SOURCE_DIR}/ext/${NXPNFCRdLib_DIRNAME}/comps/phalMfdfEv2/src
    PRIVATE ${CMAKE_SOURCE_DIR}/ext/${NXPNFCRdLib_DIRNAME}/comps/phalMfdfEv2/src/Sw
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    SSS_APIs
    ex_common
    nxpnfcrdlib
)

CREATE_BINARY(${PROJECT_NAME})
