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
# ksdk_frdmk64f.cmake

FILE(
    GLOB
    KSDK_STARTUP_FILE
    ${CMAKE_SOURCE_DIR}/ext/mcu-sdk/devices/${KSDK_CPUName}/gcc/*.S
)

# -fstack-usage

ADD_DEFINITIONS(
    -DCPU_MK64FN1M0VLL12
    -DCPU_MK64FN1M0VLL12_cm4
    -DFRDM_K64F
    -DFREEDOM
)

INCLUDE_DIRECTORIES(demos/ksdk/common/boards/frdmk64f/se_hostlib_examples/cloud_demo hostlib/hostlib/platform/ksdk)

SET(_FLAGS_CPU " -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 ")
SET(_FLAGS_L_SPECS "--specs=nano.specs --specs=nosys.specs ")
IF(WithRTOS_FreeRTOS)
    SET(
        _FLAGS_L_MEM
        " \
    -Xlinker --defsym=__ram_vector_table__=1 \
    -Xlinker --defsym=__stack_size__=0x2000 \
    -Xlinker --defsym=__heap_size__=0x8000 "
    )
ENDIF()
IF(WithRTOS_Default)
    SET(
        _FLAGS_L_MEM
        " \
    -Xlinker --defsym=__ram_vector_table__=1 \
    -Xlinker --defsym=__stack_size__=0x200 \
    -Xlinker --defsym=__heap_size__=0x200 "
    )
ENDIF()
SET(
    _FLAGS_L_LD
    " \
    -T${CMAKE_SOURCE_DIR}/demos/ksdk/common/boards/frdmk64f/se_hostlib_examples/cloud_demo/linker/MK64FN1M0xxx12_flash_heap.ld \
    -static "
)
