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
# ksdk_evkbimxrt1050.cmake

FILE(
    GLOB
    KSDK_STARTUP_FILE
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/gcc/*.S
    ${CMAKE_SOURCE_DIR}/demos/ksdk/common/boards/${KSDK_BoardName}/xip/*_flexspi_nor_config.c
    ${CMAKE_SOURCE_DIR}/demos/ksdk/common/boards/${KSDK_BoardName}/xip/*_sdram_ini_dcd.c
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/xip/fsl_flexspi_nor_boot.c
)

ADD_DEFINITIONS(
    -DCPU_MIMXRT1052DVL6B
    -DCPU_MIMXRT1052DVL6B_cm7
    -DXIP_EXTERNAL_FLASH=1
    -DXIP_BOOT_HEADER_ENABLE=1
    # -DXIP_BOOT_HEADER_DCD_ENABLE=1
    -DMIMXRT1050_EVK
    -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE
    -DIMX_RT
)

IF(WithmbedTLS_ALT_None)
    # With mbedTLS ALT, we could have covered this in SSS specific file, but without mbedTLS ALT, this needs to be
    # defined for mbedTLS SW Crypto

    ADD_DEFINITIONS(-DMBEDTLS_CTR_DRBG_C)
ENDIF()

IF(WithRTOS_Default)
    ADD_DEFINITIONS(-DFSL_RTOS_BM -DSDK_OS_BAREMETAL)
ENDIF()

IF(WithRTOS_FreeRTOS)
    ADD_DEFINITIONS(-DFSL_RTOS_FREE_RTOS)
ENDIF()

INCLUDE_DIRECTORIES(demos/ksdk/common/boards/evkbimxrt1050/se_hostlib_examples/mainA71CH)

SET(_FLAGS_CPU " -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard ")
SET(_FLAGS_L_SPECS "--specs=nano.specs --specs=nosys.specs ")
IF(WithRTOS_FreeRTOS)
    SET(
        _FLAGS_L_MEM
        " \
    -Xlinker --defsym=__ram_vector_table__=1 \
    -Xlinker --defsym=__stack_size__=0x2000 \
    -Xlinker --defsym=__heap_size__=0x8000 "
    )
    SET(
        _FLAGS_L_LD
        " \
    -T${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/MIMXRT1052/gcc/WithRTOS_FreeRTOS/MIMXRT1052xxxxx_flexspi_nor.ld \
    -static "
    )
ENDIF()
IF(WithRTOS_Default)
    SET(
        _FLAGS_L_MEM
        " \
    -Xlinker --defsym=__ram_vector_table__=1 \
    -Xlinker --defsym=__stack_size__=0x1000 \
    -Xlinker --defsym=__heap_size__=0x1000 "
    )
    SET(
        _FLAGS_L_LD
        " \
    -T${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/MIMXRT1052/gcc/WithRTOS_Default/MIMXRT1052xxxxx_flexspi_nor.ld \
    -static "
    )
ENDIF()
