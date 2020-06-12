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

ADD_SUBDIRECTORY(ex_Ev2Prepare_Card)
ADD_SUBDIRECTORY(ex_Ev2Prepare_se05x)
ADD_SUBDIRECTORY(ex_Ev2Auth_se05x)
ADD_SUBDIRECTORY(ex_Ev2ChangeKey_se05x)
ADD_SUBDIRECTORY(ex_Ev2DivChngKey_se05x)

IF(NXPInternal)
    ADD_SUBDIRECTORY(tst)
ENDIF()
