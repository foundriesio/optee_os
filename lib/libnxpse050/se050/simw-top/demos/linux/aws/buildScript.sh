#!/bin/sh
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

# Determine directory where script is stored
AWS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo ${AWS_DIR}

cd aws-iot-device-sdk-cpp
mkdir build
sed -i '31 a ADD_DEFINITIONS\(\-DOPENSSL_LOAD_CONF\)' CMakeLists.txt
cd  build
cmake ../.
make pub-sub-sample

cp -f ${AWS_DIR}/SampleConfig.json ${AWS_DIR}/aws-iot-device-sdk-cpp/build/bin/config
cp -f ${AWS_DIR}/../../../pycli/Provisioning/aws/*_device_key.pem ${AWS_DIR}/aws-iot-device-sdk-cpp/build/bin/certs
cp -f ${AWS_DIR}/../../../pycli/Provisioning/aws/*_device_certificate.cer ${AWS_DIR}/aws-iot-device-sdk-cpp/build/bin/certs
cp -f ${AWS_DIR}/../../../pycli/Provisioning/aws/*_device_reference_key.pem ${AWS_DIR}/aws-iot-device-sdk-cpp/build/bin/certs
cp -f ${AWS_DIR}/AmazonRootCA1.pem ${AWS_DIR}/aws-iot-device-sdk-cpp/build/bin/certs
