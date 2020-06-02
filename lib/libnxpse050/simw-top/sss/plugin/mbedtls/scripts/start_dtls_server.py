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

import os
import sys
from util import *

def printUsage():
    print('Invalid input argument')
    print('Run as -  start_dtls_server.py  <keyType> ')
    print('supported key types -')
    print(ecc_types)
    print(rsa_types)
    print('Example invocation - start_dtls_server.py prime256v1')
    sys.exit()

if len(sys.argv) != 2:
    printUsage()
else:
    cur_dir = os.path.abspath(os.path.dirname(__file__))
    keytype = sys.argv[1]
    if isValidKeyType(keytype) != True:
        printUsage()
    tls_rootCA = os.path.join(cur_dir, '..', 'keys', keytype, 'tls_rootca.cer')
    tls_server_key = os.path.join(cur_dir, '..', 'keys', keytype, 'tls_server_key.pem')
    tls_server_cer = os.path.join(cur_dir, '..', 'keys', keytype, 'tls_server.cer')
    mbedtls_server = os.path.join(cur_dir, '..', '..', '..', '..', 'tools', 'mbedtls_ex_orig_dtls_server')
    run("%s exchanges=1 force_version=tls1_2 debug_level=1 ca_file=%s auth_mode=required key_file=%s crt_file=%s"
        %(mbedtls_server, tls_rootCA, tls_server_key, tls_server_cer))
