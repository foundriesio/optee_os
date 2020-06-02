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

PROJECT(unity)

IF(NXPInternal)
    SET(csv_to_junit_xml csv_to_junit_xml.py)
ENDIF()

ADD_LIBRARY(
    unity
    ${csv_to_junit_xml}
    unity.c
    unity.h
    unity_fixture.c
    unity_fixture.h
    unity_fixture_addin.c
    unity_fixture_internals.h
    unity_fixture_malloc_overrides.h
    unity_internals.h
)

# add_definitions(-DUNITY_OUTPUT_COLOR)
ADD_DEFINITIONS(-DUNITY_INCLUDE_CONFIG_H)

TARGET_COMPILE_DEFINITIONS(unity PRIVATE UNITY_REPEAT_TEST_NAME)

TARGET_INCLUDE_DIRECTORIES(unity PUBLIC .)

IF(WithLog_SeggerRTT)
    TARGET_LINK_LIBRARIES(unity seggerrtt)
ENDIF()

TARGET_LINK_LIBRARIES(unity smCom)
