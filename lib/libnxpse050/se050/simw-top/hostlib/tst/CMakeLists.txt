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

IF(WithApplet_LoopBack)
    ADD_SUBDIRECTORY(LoopBack)
    IF(WithHost_frdmk64f)
        ADD_SUBDIRECTORY(T1oI2C_stability_Loopback)
    ENDIF()
ENDIF()

IF(WithHost_frdmk64f AND (WithApplet_A71CH_SIM OR WithApplet_SE05X))
    IF(WithSMCOM_T1oI2C OR WithSMCOM_PN7150)
        # See SIMW-630 : Refactor FCC_CE_cert
        # ADD_SUBDIRECTORY(FCC_CE_cert)
    ENDIF()
ENDIF()

IF(WithRTOS_Default)
    IF(WithLog_Silent)
        # No need
    ELSE()
        IF(WithApplet_SE05X AND (NOT SSS_HAVE_HOST_LPCXPRESSO55S_NS))
            ADD_SUBDIRECTORY(logging)
        ENDIF()
    ENDIF()
ENDIF()

IF(WithApplet_LoopBack OR WithApplet_SE05X AND (NOT SSS_HAVE_HOST_LPCXPRESSO55S_NS))
    IF(WithKSDK)
        IF(WithRTOS_Default)
            ADD_SUBDIRECTORY(T1oI2C_Negative_test)
            ADD_SUBDIRECTORY(Session_open_close)
        ENDIF()
    ENDIF()
ENDIF()

IF(WithSMCOM_PCSC)
    ADD_SUBDIRECTORY(CcidWtx_test)
ENDIF()
