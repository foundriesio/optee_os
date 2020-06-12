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

IF(WithApplet_A71CH_SIM AND NXPInternal)
    IF(
        "${SCP}"
        STREQUAL
        "SCP03_HostCrypto"
    )

    ENDIF()
ENDIF()

IF(
    WithApplet_LoopBack
    AND WithHost_frdmk64f
    AND NXPInternal
)
    ADD_SUBDIRECTORY(WithApplet_LoopBack_use)
ENDIF()
