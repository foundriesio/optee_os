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

PROJECT(a7x_vcom)

FILE(
    GLOB
    ${PROJECT_NAME}_files
    *.c
    *.h
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/*.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/device/class/cdc/*.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/osa/usb_osa_bm.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/device/class/usb_device_cdc_acm.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/device/class/usb_device_class.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/device/usb_device_ch9.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/device/usb_device_dci.c
)

IF(WithHost_frdmk64f)
    FILE(
        GLOB
        frdmk64f_files
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/device/usb_device_khci.c
    )
ENDIF()

IF(WithHost_evkbimxrt1050)
    FILE(
        GLOB
        evkbimxrt1050_files
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/device/usb_device_ehci.c
        # ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/device/usb_device_lpcip3511.c
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/phy/usb_phy.c
    )
ENDIF()

IF(
    WithHost_lpcxpresso55s
    OR WithHost_lpcxpresso55s_s
    OR WithHost_lpcxpresso55s_ns
)
    FILE(
        GLOB
        lpcxpresso55s_files
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/device/usb_device_ehci.c
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/device/usb_device_lpcip3511.c
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/phy/usb_phy.c
    )
ENDIF()

ADD_EXECUTABLE(
    ${PROJECT_NAME}
    ${KSDK_STARTUP_FILE}
    ${${PROJECT_NAME}_files}
    ${frdmk64f_files}
    ${evkbimxrt1050_files}
    ${lpcxpresso55s_files}
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    a7x_utils
    smCom
)

TARGET_INCLUDE_DIRECTORIES(
    ${PROJECT_NAME}
    PRIVATE
        .
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/device
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/device/class
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/include
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/osa
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/usb/phy
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/usb_vcom
)

TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PRIVATE USB_STACK_BM)

CREATE_BINARY(${PROJECT_NAME})
IF(
    CMAKE_BUILD_TYPE
    STREQUAL
    "Release"
)
    COPY_TO_SOURCEDIR(
        ${PROJECT_NAME}
        binaries
        ${SMCOM}
    )
ENDIF()
