#!/bin/bash
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

cd ${SIMW_TOP_DIR}/../
dos2unix ${SIMW_TOP_DIR}/scripts/android/aosp_based/${BUILD_SCRIPT}

if [ -e ${BUILD_SCRIPT} ]; then
    rm ${BUILD_SCRIPT}
fi


cd ${SIMW_TOP_DIR}/../
cp ${SIMW_TOP_DIR}/akm/src/interface_keymaster/patch/aosp/${INTERFACE_KEYMASTER_PATCH} ${INTERFACE_KEYMASTER_DIRECTORY}
cp ${SIMW_TOP_DIR}/scripts/android/aosp_based/${VTS_FIPS_PATCH} ${INTERFACE_KEYMASTER_DIRECTORY}

cd ${INTERFACE_KEYMASTER_DIRECTORY}
git co -- keymaster/3.0/default/Android.mk
git co -- keymaster/3.0/default/KeymasterDevice.cpp
git co -- keymaster/3.0/vts/functional/keymaster_hidl_hal_test.cpp
patch -p1 < ${INTERFACE_KEYMASTER_PATCH}
if [[ "$CONFIG_BOARD" = "hikey960" ]]; then
	patch -p1 < ${VTS_FIPS_PATCH}
fi

cd ${SIMW_TOP_DIR}/scripts/android/aosp_based
chmod +x ${BUILD_SCRIPT}
./${BUILD_SCRIPT}
