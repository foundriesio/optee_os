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

#This file is required for ANDROID to compile all se05x and Applet version based source files.
#Android Keymaster is having dependancies on below source files.

SE05X_SRC_FILES_LIST := \
		hostlib/hostLib/se05x/src/se05x_tlv.c \
		hostlib/hostLib/se05x/src/se05x_mw.c \
		hostlib/hostLib/se05x/src/se05x_ECC_curves.c \

APPLET_03_00_SRC_FILES_LIST := hostlib/hostLib/se05x_03_xx_xx/se05x_APDU.c
APPLET_03_00_C_INCLUDES := hostlib/hostLib/se05x_03_xx_xx

