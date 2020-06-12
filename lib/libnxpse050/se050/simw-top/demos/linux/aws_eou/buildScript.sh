#!/bin/sh
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

# Determine directory where script is stored
AWS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo ${AWS_DIR}

git clone https://github.com/aws/aws-iot-device-sdk-embedded-C.git
cd aws-iot-device-sdk-embedded-C
git checkout v4_beta
git submodule update --init --recursive
mkdir build
cp ../aws_c_sdk_eou.patch .
patch -p1 < aws_c_sdk_eou.patch
cd  build
cmake ../.
cmake -DIOT_NETWORK_USE_OPENSSL:BOOL=ON .
make iot_demo_mqtt -j

