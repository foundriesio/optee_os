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
DEMO_DIR="$( cd "$(dirname "$0")" ; pwd -P )"
echo ${DEMO_DIR}
cd ${DEMO_DIR}

COMMON_DIR=${DEMO_DIR}/../common/
cd ${COMMON_DIR}
# building jansson
if [ ! -d jansson ]; then
	git clone https://github.com/akheron/jansson.git
	cd jansson
	autoreconf -i
	./configure
	make
	make install
	cd ..
fi

# building libjwt
if [ ! -d libjwt ]; then
	export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
	git clone https://github.com/benmcollins/libjwt.git
	cd libjwt
	autoreconf -i
	./configure
	make all
	sudo make install
	cd ..
fi

# building paho c client
# NOTE: Building the paho c client requires a filesystem
# that supports symbolic links
if [ ! -d paho.mqtt.c ]; then
	git clone https://github.com/eclipse/paho.mqtt.c.git
	cd paho.mqtt.c
	make
	sudo make install
	ldconfig /usr/local/lib
	cd ..
fi

#building the app
cd ${DEMO_DIR}
mkdir gcp
cd gcp
git clone https://github.com/GoogleCloudPlatform/cpp-docs-samples.git
cp ../app.patch cpp-docs-samples/iot/mqtt-ciotc/
cd cpp-docs-samples/iot/mqtt-ciotc/
perl -p -e 's/\r$//' < app.patch > unix_app.patch
git apply unix_app.patch
make







