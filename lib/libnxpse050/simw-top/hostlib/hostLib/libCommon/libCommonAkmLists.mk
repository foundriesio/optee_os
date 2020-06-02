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

#This file is required for ANDROID to compile all libcommon realted  source files.
#Android Keymaster is having dependancies on below source files.

LIBCOMMON_SRC_FILES_LIST := \
            hostlib/hostLib/platform/imx/se05x_reset.c \
            hostlib/hostLib/platform/generic/sm_timer.c \
            hostlib/hostLib/tstUtil/tst_sm_time.c \
            hostlib/hostLib/libCommon/smCom/T1oI2C/phNxpEse_Api.c \
            hostlib/hostLib/libCommon/smCom/T1oI2C/phNxpEsePal_i2c.c \
            hostlib/hostLib/libCommon/smCom/T1oI2C/phNxpEseProto7816_3.c \
            hostlib/hostLib/libCommon/smCom/smComT1oI2C.c \
            hostlib/hostLib/libCommon/infra/sm_apdu.c \
            hostlib/hostLib/libCommon/infra/sm_errors.c \
            hostlib/hostLib/libCommon/infra/sm_printf.c \
            hostlib/hostLib/libCommon/smCom/smCom.c \
            hostlib/hostLib/libCommon/nxScp/nxScp03_Com.c\
            hostlib/hostLib/libCommon/infra/nxLog_Android.c


HIKEY_I2C_DRV_SRC_FILES_LIST := \
            hostlib/hostLib/platform/android/i2c_a7.c

IMX_I2C_DRV_SRC_FILES_LIST := \
            hostlib/hostLib/platform/imx/i2c_a7.c
