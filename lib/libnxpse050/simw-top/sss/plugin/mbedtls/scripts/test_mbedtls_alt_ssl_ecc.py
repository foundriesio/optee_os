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

import sys
from util import *
import test_mbedtls_alt_ecc


def printUsage():
    print('Invalid input argument')
    print('Run as -  test_mbedtls_alt_ssl_ecc.py  <ec_type|all> <jrcpv2|vcom> <ip_address|port_name>  <a71ch|se050>')
    print('supported ec_type -')
    print(ecc_types)
    print('Example invocation - test_mbedtls_alt_ssl_ecc.py all jrcpv2 127.0.0.1:8050 se050')
    sys.exit()


if len(sys.argv) < 5:
    printUsage()
else:
    if test_mbedtls_alt_ecc.doTest(sys.argv, "ssl2", __file__) != 0:
        printUsage()
