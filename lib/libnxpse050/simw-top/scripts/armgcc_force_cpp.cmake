# Copyright 2018 NXP
#
# This software is owned or controlled by NXP and may only be used
# strictly in accordance with the applicable license terms.  By expressly
# accepting such terms or by downloading, installing, activating and/or
# otherwise using the software, you are agreeing that you have read, and
# that you agree to comply with and are bound by, such license terms.  If
# you do not agree to be bound by the applicable license terms, then you
# may not retain, install, activate or otherwise use the software.
#
INCLUDE(CMakeForceCompiler)

# TOOLCHAIN EXTENSION
IF(WIN32)
    SET(TOOLCHAIN_EXT ".exe")
ELSE()
    SET(TOOLCHAIN_EXT "")
ENDIF()

# EXECUTABLE EXTENSION
SET(CMAKE_EXECUTABLE_SUFFIX ".elf")

SET(CROSS_COMPILE_PREFIX "arm-none-eabi-")

FIND_FILE(
    arm-gcc
    NAMES ${CROSS_COMPILE_PREFIX}gcc ${CROSS_COMPILE_PREFIX}gcc.exe
    PATHS $ENV{ARMGCC_DIR}/bin;$ENV{PATH}
)

IF(
    "${arm-gcc}"
    STREQUAL
    "arm-gcc-NOTFOUND"
)
    MESSAGE(WARNING "***Can not find ${CROSS_COMPILE_PREFIX}gcc ***")
    MESSAGE(FATAL_ERROR "***Please set ARMGCC_DIR in environment variables***")
ENDIF()

FIND_FILE(
    arm-gxx
    NAMES ${CROSS_COMPILE_PREFIX}g++ ${CROSS_COMPILE_PREFIX}g++.exe
    PATHS $ENV{ARMGCC_DIR}/bin;$ENV{PATH}
)
FIND_FILE(
    arm-asm
    NAMES ${CROSS_COMPILE_PREFIX}gcc ${CROSS_COMPILE_PREFIX}gcc.exe
    PATHS $ENV{ARMGCC_DIR}/bin;$ENV{PATH}
)
FIND_FILE(
    arm-ar
    NAMES ${CROSS_COMPILE_PREFIX}ar ${CROSS_COMPILE_PREFIX}ar.exe
    PATHS $ENV{ARMGCC_DIR}/bin;$ENV{PATH}
)

FIND_FILE(
    arm-objcopy
    NAMES ${CROSS_COMPILE_PREFIX}objcopy ${CROSS_COMPILE_PREFIX}objcopy.exe
    PATHS $ENV{ARMGCC_DIR}/bin;$ENV{PATH}
)
FIND_FILE(
    arm-objdump
    NAMES ${CROSS_COMPILE_PREFIX}objdump ${CROSS_COMPILE_PREFIX}objdump.exe
    PATHS $ENV{ARMGCC_DIR}/bin;$ENV{PATH}
)
# FIND_FILE(arm-nm NAMES ${CROSS_COMPILE_PREFIX}nm ${CROSS_COMPILE_PREFIX}nm.exe PATHS $ENV{ARMGCC_DIR}/bin;$ENV{PATH} )
# FIND_FILE(arm-size NAMES ${CROSS_COMPILE_PREFIX}size ${CROSS_COMPILE_PREFIX}size.exe PATHS
# $ENV{ARMGCC_DIR}/bin;$ENV{PATH} )

SET(CMAKE_SYSTEM_NAME GNU)
SET(CMAKE_SYSTEM_PROCESSOR arm)

SET(CMAKE_C_COMPILER ${arm-gcc})
SET(CMAKE_CXX_COMPILER ${arm-gxx})
SET(CMAKE_ASM_COMPILER ${arm-asm})

SET(
    CMAKE_OBJCOPY
    ${arm-objcopy}
    CACHE INTERNAL "objcopy tool"
)
SET(
    CMAKE_OBJDUMP
    ${arm-objdump}
    CACHE INTERNAL "objdump tool"
)

# SET(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_DIR}/${TARGET_TRIPLET} ${EXTRA_FIND_PATH})
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)
