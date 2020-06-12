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

#This file is required for Android to compile A7xUtils source files.
#Android Keymaster is having dependancies on below source files.

A7X_UTILS_SRC_FILES_LIST := \
        hostlib/hostLib/libCommon/infra/sm_app_boot.c \
        hostlib/hostLib/libCommon/infra/app_boot_nfc.c \
        hostlib/hostLib/libCommon/scp/scp.c \
        hostlib/hostLib/libCommon/infra/sm_connect.c \
        hostlib/hostLib/libCommon/infra/global_platf.c
