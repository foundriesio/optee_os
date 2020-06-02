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

INCLUDE_FLAGS := -I/usr/local/include/se05x
DEFINES := -DSSS_USE_FTR_FILE
LIBRARY_PATH := -L/usr/local/lib

# Auto generated and copied by make install.
-include /usr/local/share/se05x/cmake_options.mak

SIMW_LIBRARIES := \
  -lex_common \
  -lSSS_APIs \
  -lse05x \
  -la7x_utils \
  -lsmCom

ifeq (1,$(SSS_HAVE_SMCOM_JRCP_V2))
  SIMW_LIBRARIES := $(SIMW_LIBRARIES) -ljrcplib
endif

OPENSSL_LIBRARIES := -lcrypto

# Inluding libraries twice for cyclic-depdendencies across libraries.
CFLAGS += -MD -MMD $(INCLUDE_FLAGS) \
 $(DEFINES) \
 $(LIBRARY_PATH) \
 $(SIMW_LIBRARIES) \
 $(SIMW_LIBRARIES) \
 $(OPENSSL_LIBRARIES) \
 $(LOCAL_LIBRARIES)

all:
	c++   -fPIC -g se05x_minimal.c $(CFLAGS) -o se05x_minimal

clean:
	-rm *.d
	-rm se05x_minimal

-include se05x_minimal.d
