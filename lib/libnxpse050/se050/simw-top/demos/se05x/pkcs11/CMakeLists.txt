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

PROJECT(pkcs11)

FILE(
    GLOB
    files
    ${CMAKE_SOURCE_DIR}/sss/plugin/pkcs11/*.c
    #${CMAKE_SOURCE_DIR}/ext/amazon-freertos/lib/pkcs11/portable/nxp/se_hostlib/*.c
)

ADD_DEFINITIONS(-DPKCS11_SESSION_OPEN)

ADD_LIBRARY(${PROJECT_NAME} ${files})

TARGET_INCLUDE_DIRECTORIES(
    ${PROJECT_NAME}
    #PUBLIC ${CMAKE_SOURCE_DIR}/ext/amazon-freertos/lib/include
    #PUBLIC ${CMAKE_SOURCE_DIR}/ext/amazon-freertos/lib/include/private
    PUBLIC ${CMAKE_SOURCE_DIR}/ext/amazon-freertos/libraries/3rdparty/pkcs11
    PUBLIC ${CMAKE_SOURCE_DIR}/ext/amazon-freertos/libraries/freertos_plus/standard/crypto/include
    PUBLIC
        ${CMAKE_SOURCE_DIR}/ext/amazon-freertos/libraries/freertos_plus/standard/pkcs11/include
        #PUBLIC ${CMAKE_SOURCE_DIR}/ext/amazon-freertos/lib/third_party/mcu_vendor/nxp/mflash/se_hostlib
        ###################PRIVATE ${CMAKE_SOURCE_DIR}/demos/se05x/pkcs11_testbench
        ###################PRIVATE ${CMAKE_SOURCE_DIR}/ext/amazon-freertos/lib/third_party/pkcs11
        ###################PRIVATE ${CMAKE_SOURCE_DIR}/ext/amazon-freertos/tests/common/include
        ###################PRIVATE ${CMAKE_SOURCE_DIR}/demos/aws/pkcs11testbench
)

TARGET_INCLUDE_DIRECTORIES(
    ${PROJECT_NAME}
    PUBLIC ${DEMOS_KSDK_DIR}/common
    PUBLIC ${CMAKE_SOURCE_DIR}/sss/plugin/pkcs11
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    ex_common
    freertos-kernel
)

#TARGET_INCLUDE_DIRECTORIES(
#    ${PROJECT_NAME}
#    PRIVATE ${CMAKE_SOURCE_DIR}/demos/se05x/pkcs11_testbench
#    PRIVATE ${CMAKE_SOURCE_DIR}/ext/amazon-freertos/lib/third_party/pkcs11
#    PRIVATE ${CMAKE_SOURCE_DIR}/ext/amazon-freertos/tests/common/include
#    PRIVATE ${CMAKE_SOURCE_DIR}/demos/aws/pkcs11testbench
#)

#TARGET_LINK_LIBRARIES(
#    ${PROJECT_NAME}
#    unity
#)
