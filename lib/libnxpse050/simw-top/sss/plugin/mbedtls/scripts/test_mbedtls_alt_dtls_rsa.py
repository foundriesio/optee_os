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
import test_mbedtls_alt_rsa


def printUsage():
    print('Invalid input argument')
    print('Run as -  test_mbedtls_alt_dtls_rsa.py  <rsa_type|all> <jrcpv2|vcom> <ip_address|port_name>  ')
    print('supported rsa_type -')
    print(rsa_types)
    print('Example invocation - test_mbedtls_alt_dtls_rsa.py all jrcpv2 127.0.0.1:8050')
    sys.exit()


if len(sys.argv) < 4:
    printUsage()
else:
    if test_mbedtls_alt_rsa.doTest(sys.argv, "dtls", __file__) != 0:
        printUsage()
