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

# lpcxpresso55s

PROJECT(board)

FILE(
    GLOB
    board_files
    ${CMAKE_SOURCE_DIR}/hostlib/hostLib/platform/ksdk/ax_reset.c
    ${CMAKE_SOURCE_DIR}/hostlib/hostLib/platform/ksdk/i2c_lpc55sxx.c
    ${CMAKE_SOURCE_DIR}/hostlib/hostLib/platform/ksdk/se05x_reset.c
    ${CMAKE_SOURCE_DIR}/hostlib/hostLib/platform/ksdk/se_pit_config.c
    ${CMAKE_SOURCE_DIR}/hostlib/hostLib/platform/ksdk/timer_kinetis.c
    ${CMAKE_SOURCE_DIR}/demos/ksdk/common/boards/${KSDK_BoardName}/*.c
    ${CMAKE_SOURCE_DIR}/demos/ksdk/common/boards/${KSDK_BoardName}/*.h
    ${CMAKE_SOURCE_DIR}/demos/ksdk/common/boards/${KSDK_BoardName}/se_hostlib_examples/cloud_demo/pin_mux.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/components/serial_manager/*.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/components/uart/usart_adapter.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/components/lists/generic_list.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_casper.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_clock.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_common.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_ctimer.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_dma.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_flexcomm.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_gpio.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_i2c.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_inputmux.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_pint.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_reset.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_spi.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_spi_dma.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_usart.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_rng.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/system_${KSDK_CPUName}*.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/mmcau/fsl_mmcau.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/platform/utilities/debug_console/*.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/platform/utilities/str/*.c
)

IF(WithHost_lpcxpresso55s_s)
    FILE(
        GLOB
        lpc55s_tzm_s
        ${CMAKE_SOURCE_DIR}/demos/lpc55s/tzm/tzm_config.c
    )
ENDIF()

IF(WithRTOS_FreeRTOS)
    LIST(
        APPEND
        board_files
        ${CMAKE_SOURCE_DIR}/hostlib/hostLib/platform/ksdk/timer_kinetis_freertos.c
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/drivers/fsl_i2c_freertos.c
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

ADD_LIBRARY(board ${board_files} ${lpc55s_tzm_s})

TARGET_INCLUDE_DIRECTORIES(
    board
    PRIVATE ${CMAKE_SOURCE_DIR}/hostlib/hostLib/platform/inc
    PRIVATE ${CMAKE_SOURCE_DIR}/hostlib/hostLib/libCommon/infra
    PRIVATE ${CMAKE_SOURCE_DIR}/hostlib/hostLib/libCommon/smCom
    PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/components/osa
    PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/components/lists
)

IF(WithHost_lpcxpresso55s_s)
    TARGET_INCLUDE_DIRECTORIES(board PUBLIC ${CMAKE_SOURCE_DIR}/demos/lpc55s/tzm)
ENDIF()

IF(WithHost_lpcxpresso55s_s OR WithHost_lpcxpresso55s_ns)
    TARGET_INCLUDE_DIRECTORIES(board PUBLIC ${CMAKE_SOURCE_DIR}/demos/lpc55s/common)
ENDIF()

#TARGET_INCLUDE_DIRECTORIES(board
#    PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/lwip/src/include
#    PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/lwip/port
#)

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
TARGET_LINK_LIBRARIES(board power_hardabi)
TARGET_LINK_LIBRARIES(board -Wl,--end-group)

IF(WithRTOS_FreeRTOS)
    PROJECT(wifi_qca)

    FILE(
        GLOB
        wifi_files
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/port/boards/lpcxpresso55s69/freertos/wifi_shield.c
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/port/boards/lpcxpresso55s69/freertos/wlan_qca400x.c
        ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/port/boards/lpcxpresso55s69/freertos/wlan_qcom.c
    )

    ADD_LIBRARY(wifi_qca ${wifi_files})

    TARGET_LINK_LIBRARIES(wifi_qca board)

    TARGET_INCLUDE_DIRECTORIES(
        wifi_qca
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/amazon-freertos/lib/FreeRTOS/portable
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/amazon-freertos/lib/FreeRTOS/include
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/common_src/hcd
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/common_src/include
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/common_src/stack_common
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/common_src/wmi
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/custom_src/include
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/custom_src/stack_custom
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/include
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/include/AR6002
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/include/AR6002/hw2.0/hw
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/include/AR6002/hw4.0/hw
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/port
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/port/drivers/flexcomm_freertos
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/port/env/freertos
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/port/boards/lpcxpresso55s69/freertos
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/port/boards/lpcxpresso55s69/freertos/silex2401
               #PUBLIC ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/middleware/wifi_qca/port/boards/lpcxpresso55s69/freertos/gt202
    )
ENDIF()
