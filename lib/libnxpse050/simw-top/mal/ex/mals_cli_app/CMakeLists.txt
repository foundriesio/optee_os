# Copyright 2020 NXP
#
# This software is owned or controlled by NXP and may only be used
# strictly in accordance with the applicable license terms.  By expressly
# accepting such terms or by downloading, installing, activating and/or
# otherwise using the software, you are agreeing that you have read, and
# that you agree to comply with and are bound by, such license terms.  If
# you do not agree to be bound by the applicable license terms, then you
# may not retain, install, activate or otherwise use the software.
#
PROJECT(mals_cli_app)

FILE(
    GLOB
        mfa_files
        mfa_*.c
        mfa_*.h
)

IF(NXPInternal)
    FILE(
        GLOB
            nxp_files
            nxp_mfa_*.c
            nxp_mfa_*.h
    )
ENDIF()

ADD_EXECUTABLE(
    ${PROJECT_NAME}
    ${KSDK_STARTUP_FILE}
    ${mfa_files}
    ${nxp_files}
)

IF(NXPInternal)
    TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME}
        PRIVATE
        NXP_INTERNAL_IMPL=1
    )
ENDIF(NXPInternal)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    ex_common
    MAL_AGENT_APIs
)

CREATE_BINARY(${PROJECT_NAME})

COPY_TO_SOURCEDIR(
    ${PROJECT_NAME}
    binaries
    ${SMCOM}
)
