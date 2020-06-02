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

IF(WithRTOS_Default)

    IF(NOT WithHost_Win10IoT)
        ADD_SUBDIRECTORY(se05x_minimal)
    ENDIF()

    IF(WithHost_WithFileSystem)
        ADD_SUBDIRECTORY(se05x_export)
        ADD_SUBDIRECTORY(se05x_import)
        IF(SSS_HAVE_HOSTCRYPTO_ANY)
            IF(SSS_HAVE_SE05X_Auth_ECKey OR SSS_HAVE_SE05X_Auth_ECKey_PlatfSCP03)
                ADD_SUBDIRECTORY(se05x_ImportExternalObjectPrepare)
            ENDIF()
            IF(SSS_HAVE_SE05X_Auth_None OR SSS_HAVE_SE05X_Auth_PlatfSCP03)
                ADD_SUBDIRECTORY(se05x_ImportExternalObjectCreate)
            ENDIF()
        ENDIF(SSS_HAVE_HOSTCRYPTO_ANY)
    ENDIF()

    IF(SSSFTR_SE05X_AuthSession AND SSS_HAVE_HOSTCRYPTO_ANY)
        IF(SSS_HAVE_SE05X_Auth_None OR SSS_HAVE_SE05X_Auth_PlatfSCP03)
            ADD_SUBDIRECTORY(se05x_Delete_and_test_provision)
            ADD_SUBDIRECTORY(se05x_MandatePlatformSCP)
        ELSE()
            MESSAGE(STATUS "se05x_Delete_and_test_provision is not built because SE05X_Auth is '${SE05X_Auth}'.")
        ENDIF()
    ENDIF(SSSFTR_SE05X_AuthSession AND SSS_HAVE_HOSTCRYPTO_ANY)

    IF(SSS_HAVE_SE05X_Auth_PlatfSCP03)
        ADD_SUBDIRECTORY(se05x_RotatePlatformSCP03Keys)
    ENDIF()

    IF(SSSFTR_SE05X_AuthSession  AND SSS_HAVE_HOSTCRYPTO_ANY)
        IF(SSS_HAVE_SE05X_Auth_None OR SSS_HAVE_SE05X_Auth_PlatfSCP03)
            ADD_SUBDIRECTORY(se05x_TransportLock)
            ADD_SUBDIRECTORY(se05x_TransportUnLock)
        ENDIF()
    ENDIF(SSSFTR_SE05X_AuthSession AND SSS_HAVE_HOSTCRYPTO_ANY)

    IF(NOT WithHost_Win10IoT)
        ADD_SUBDIRECTORY(ex_se05x_WiFiKDF)
    ENDIF()

    ADD_SUBDIRECTORY(se05x_Get_Info)
    ADD_SUBDIRECTORY(ex_i2cMaster)
    IF(SSS_HAVE_ECC)
        ADD_SUBDIRECTORY(se05x_policy)
    ENDIF()

    IF(SSS_HAVE_HOSTCRYPTO_ANY)
        ADD_SUBDIRECTORY(se05x_ReadWithAttestation)
    ENDIF()

    IF(SSS_HAVE_SCP_None)
        ADD_SUBDIRECTORY(se05x_GetAPDUBuffer)
    ENDIF()
    IF(SSS_HAVE_HOSTCRYPTO_ANY)
        ADD_SUBDIRECTORY(se05x_TimeStamp)
    ENDIF()
ENDIF()

IF(WithRTOS_FreeRTOS)
    ADD_SUBDIRECTORY(se05x_minimal)
ENDIF()

IF(WithHostCrypto_OPENSSL)
    ADD_SUBDIRECTORY(certificate_demo)
    ADD_SUBDIRECTORY(seTool)
ENDIF()

IF(WithHostCrypto_MBEDTLS)
    ADD_SUBDIRECTORY(se05x_Get_Certificate)
ENDIF()

IF(WithHostCrypto_MBEDCRYPTO)
    IF(WithHost_lpcxpresso55s)
        ADD_SUBDIRECTORY(tfm_example)
    ENDIF()
    IF(WithHost_lpcxpresso55s_ns)
        ADD_SUBDIRECTORY(psa_nonsecure)
    ENDIF()
ENDIF()

IF(WithRTOS_FreeRTOS AND WithmbedTLS_ALT_SSS)
    ADD_SUBDIRECTORY(pkcs11)
ENDIF()
