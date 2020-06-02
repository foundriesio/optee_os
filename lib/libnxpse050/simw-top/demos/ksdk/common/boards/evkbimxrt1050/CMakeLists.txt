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

# evkbimxrt1050

PROJECT(board)

FILE(
    GLOB
    board_files
    ${CMAKE_SOURCE_DIR}/hostlib/hostLib/platform/ksdk/ax_reset.c
    ${CMAKE_SOURCE_DIR}/hostlib/hostLib/platform/ksdk/i2c_imxrt10xx.c
    ${CMAKE_SOURCE_DIR}/hostlib/hostLib/platform/ksdk/se05x_reset.c
    ${CMAKE_SOURCE_DIR}/hostlib/hostLib/platform/ksdk/se_pit_config.c
    ${CMAKE_SOURCE_DIR}/hostlib/hostLib/platform/ksdk/timer_kinetis.c
    ${CMAKE_SOURCE_DIR}/demos/ksdk/common/boards/${KSDK_BoardName}/*.c
    ${CMAKE_SOURCE_DIR}/demos/ksdk/common/boards/${KSDK_BoardName}/*.h
    ${CMAKE_SOURCE_DIR}/demos/ksdk/common/boards/${KSDK_BoardName}/se_hostlib_examples/cloud_demo/pin_mux.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/components/serial_manager/*.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/components/uart/lpuart_adapter.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/components/lists/generic_list.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_clock.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_dcp.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_gpio.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_lpi2c.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_lpspi.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_lpuart.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_pit.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_trng.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_flexio.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/system_${KSDK_CPUName}.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/mmcau/fsl_mmcau.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/platform/utilities/debug_console/*.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/platform/utilities/str/*.c
)

IF(WithRTOS_FreeRTOS)
    LIST(
        APPEND
        board_files
        ${CMAKE_SOURCE_DIR}/hostlib/hostLib/platform/ksdk/timer_kinetis_freertos.c
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_lpi2c_freertos.c
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/components/osa/fsl_os_abstraction_free_rtos.c
    )
ELSE()
    LIST(
        APPEND
        board_files
        ${CMAKE_SOURCE_DIR}/hostlib/hostLib/platform/ksdk/timer_kinetis_bm.c
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/components/osa/fsl_os_abstraction_bm.c
    )
ENDIF()

ADD_LIBRARY(board ${board_files})

TARGET_INCLUDE_DIRECTORIES(
    board
    PRIVATE
        ${CMAKE_SOURCE_DIR}/hostlib/hostLib/platform/inc
        ${CMAKE_SOURCE_DIR}/hostlib/hostLib/libCommon/infra
        ${CMAKE_SOURCE_DIR}/hostlib/hostLib/libCommon/smCom
)

TARGET_INCLUDE_DIRECTORIES(
    board
    PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/lwip/src/include
    PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/lwip/port
    PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/components/osa
    PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/components/lists
)

IF(WithRTOS_FreeRTOS)
    TARGET_LINK_LIBRARIES(board freertos-kernel)
ENDIF()

TARGET_LINK_LIBRARIES(board -Wl,--start-group)
TARGET_LINK_LIBRARIES(
    board
    debug
    m
)
TARGET_LINK_LIBRARIES(
    board
    debug
    c
)
TARGET_LINK_LIBRARIES(
    board
    debug
    gcc
)
TARGET_LINK_LIBRARIES(
    board
    debug
    nosys
)
TARGET_LINK_LIBRARIES(
    board
    optimized
    m
)
TARGET_LINK_LIBRARIES(
    board
    optimized
    c
)
TARGET_LINK_LIBRARIES(
    board
    optimized
    gcc
)
TARGET_LINK_LIBRARIES(
    board
    optimized
    nosys
)
TARGET_LINK_LIBRARIES(board _mmcau)
TARGET_LINK_LIBRARIES(board -Wl,--end-group)
