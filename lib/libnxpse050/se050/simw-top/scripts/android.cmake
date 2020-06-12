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

MACRO(CREATE_BINARY PROJECT_NAME)
    # Nothing here.
ENDMACRO()

MACRO(
    COPY_TO_SOURCEDIR
    PROJECT_NAME
    TARGET_DIRNAME
    TARGET_SUFFIX
)
    # Nothing here.
ENDMACRO()

IF(NXPInternal)
    ADD_DEFINITIONS(-Werror)
ENDIF()

ADD_DEFINITIONS("-Wno-main-return-type")
ADD_DEFINITIONS("-Wformat")
ADD_DEFINITIONS(-DFTR_FILE_SYSTEM)
