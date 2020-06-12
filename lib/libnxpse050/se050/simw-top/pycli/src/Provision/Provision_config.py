# Copyright 2019,2020 NXP
#
# This software is owned or controlled by NXP and may only be used
# strictly in accordance with the applicable license terms.  By expressly
# accepting such terms or by downloading, installing, activating and/or
# otherwise using the software, you are agreeing that you have read, and
# that you agree to comply with and are bound by, such license terms.  If
# you do not agree to be bound by the applicable license terms, then you
# may not retain, install, activate or otherwise use the software.

import sys
import platform
SUBSYSTEM = "se050"

# t1oi2c connection method used in iMX
if 'imx' in platform.node() or 'raspberrypi' in platform.node():
    if SUBSYSTEM == "a71ch":
        CONNECTION_METHOD = "sci2c"
    else:
        CONNECTION_METHOD = "t1oi2c"
else:
    CONNECTION_METHOD = "vcom"



