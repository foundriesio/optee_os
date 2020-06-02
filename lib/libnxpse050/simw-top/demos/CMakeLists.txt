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

IF(
    WithApplet_A71CH_SIM
    OR WithApplet_A71XX
    OR WithApplet_SE05X
    OR WithApplet_SE050_L
    OR WithApplet_LoopBack
    OR WithKSDK
)
    ADD_SUBDIRECTORY(ksdk)
ENDIF()

IF(NXPInternal AND (NOT SSS_HAVE_HOST_LPCXPRESSO55S_NS))
    ADD_SUBDIRECTORY(aws)
ENDIF()

IF(WithApplet_SE05X AND (NOT SSS_HAVE_HOST_LPCXPRESSO55S_NS))
    IF(WithLargePrograms) # Skip larger programs. e.g.
        ADD_SUBDIRECTORY(se05x)
        IF(NXPInternal)
            ADD_SUBDIRECTORY(se05x_nxp)
        ENDIF()
    ENDIF()
ENDIF()

IF(WithApplet_A71CH OR WithApplet_A71CH_SIM)
    ADD_SUBDIRECTORY(a71ch)
ENDIF()
IF(WithNXPNFCRdLib)
    ADD_SUBDIRECTORY(nxpnfcrdlib)
ENDIF()

IF(WithHost_WithFileSystem)
    IF(WithApplet_SE05X AND (SSS_HAVE_SE05X_Auth_None OR SSS_HAVE_SE05X_Auth_PlatfSCP03))
        ADD_SUBDIRECTORY(apdu_player)
    ENDIF()
ENDIF()

IF(WithHost_lpcxpresso55s_ns OR WithHost_lpcxpresso55s_s)
    ADD_SUBDIRECTORY(lpc55s)
ENDIF()

IF(WithHostCrypto_MBEDTLS)
    IF(WithRTOS_Default AND WithHost_LinuxLike)
        ADD_SUBDIRECTORY(linux/sss_pkcs11)
    ENDIF()
ENDIF()

IF(WithOPCUA_open62541)
    IF(
        WithHostCrypto_MBEDTLS
        AND
            (WithHost_PCWindows
             OR WithHost_iMXLinux
             OR WithHost_PCLinux64
             OR WithHost_Raspbian
            )
    )
        ADD_SUBDIRECTORY(opc_ua)
    ENDIF()
ENDIF()

IF(WithHostCrypto_None)
    # PASS
ELSE()
    IF(NOT SSS_HAVE_HOST_LPCXPRESSO55S_NS)
        ADD_SUBDIRECTORY(test_Crypto)
    ENDIF(NOT SSS_HAVE_HOST_LPCXPRESSO55S_NS)
ENDIF()
