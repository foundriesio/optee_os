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

ADD_SUBDIRECTORY(src)

IF(NOT SSS_HAVE_HOST_LPCXPRESSO55S_NS)

IF(
    WithRTOS_Default
    AND (WithApplet_SE05X
         OR WithApplet_A71CH
         OR WithApplet_A71CH_SIM)
)
    IF(WithApplet_SE05X)
        ADD_SUBDIRECTORY(symmetric)
    ENDIF()
    IF(SSS_HAVE_HOSTCRYPTO_ANY)
        ADD_SUBDIRECTORY(hkdf)
    ENDIF()
    ADD_SUBDIRECTORY(md)
    ADD_SUBDIRECTORY(hmac)
    IF(SSS_HAVE_HOSTCRYPTO_ANY)
        ADD_SUBDIRECTORY(ecdh)
    ENDIF()
ENDIF()

IF(SSS_HAVE_ECC)
    ADD_SUBDIRECTORY(ecc)
ENDIF()
IF(SSS_HAVE_RSA)
    ADD_SUBDIRECTORY(rsa)
ENDIF()

IF(WithKSDK)
    #No native mbedTLS Examples
ELSE()
    IF(WithmbedTLS_ALT_SSS)
        ADD_SUBDIRECTORY(mbedtls)
    ENDIF()
ENDIF()

ENDIF(NOT SSS_HAVE_HOST_LPCXPRESSO55S_NS)
