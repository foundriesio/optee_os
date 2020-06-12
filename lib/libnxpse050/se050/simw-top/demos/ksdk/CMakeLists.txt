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

SET(ADD_KSDK_DEMOS OFF)

IF(NXPInternal AND WithHost_PCWindows)
    SET(ADD_KSDK_DEMOS ON)
ENDIF()

IF(WithKSDK)
    SET(ADD_KSDK_DEMOS ON)
    ADD_SUBDIRECTORY(common/boards/${KSDK_BoardName})
ENDIF()

INCLUDE_DIRECTORIES(common)

IF(SSS_HAVE_HOSTCRYPTO_NONE)
    SET(ADD_KSDK_DEMOS OFF)
ENDIF()

IF(SSS_HAVE_HOST_LPCXPRESSO55S_NS)
    SET(ADD_KSDK_DEMOS OFF)
ENDIF(SSS_HAVE_HOST_LPCXPRESSO55S_NS)

IF(NXPInternal)
    IF(WithHost_PCWindows)
        ADD_SUBDIRECTORY(x86pc)
    ENDIF()
ENDIF()

IF(ADD_KSDK_DEMOS)
    IF(WithmbedTLS_ALT_SSS)
        IF(WithRTOS_FreeRTOS)
            ADD_SUBDIRECTORY(gcp)
            ADD_SUBDIRECTORY(azure)
            ADD_SUBDIRECTORY(aws_jitr)
            ADD_SUBDIRECTORY(ibm_watson)
        ELSE()
            MESSAGE(STATUS "Set RTOS=FreeRTOS to Build Cloud Examples ")
        ENDIF()
    ELSE()
        MESSAGE(STATUS "Set mbedTLS_ALT=SSS to Build Cloud Examples ")
    ENDIF()
    IF(SSS_HAVE_APPLET)
        ADD_SUBDIRECTORY(provisioning)
    ENDIF()
ENDIF()
