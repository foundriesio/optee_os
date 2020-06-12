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
import traceback
import sys
sss_dir = os.path.abspath(os.getcwd()
                                + os.sep + ".."
                                + os.sep + ".."
                                + os.sep + "pycli"
                                + os.sep + "src")
sys.path.append(sss_dir)

from Provision import GenerateGCPCredentials
from Provision import ResetAndUpdate_GCP
from Provision.Provision_util import *


# Keypair and certificate index for GCP (simw-top\demos\ksdk\gcp\gcp_iot_config.h)
KEYPAIR_INDEX_CLIENT_PRIVATE = 0x20181001
CERTIFICATE_INDEX = 0x20181002


def main():
    try:
        # working directory for SIMW-TOP/binaries/pySSSCLI
        cur_dir = os.getcwd()
        status = GenerateGCPCredentials.generateCredentials(cur_dir)
        if status == STATUS_SUCCESS:
            ResetAndUpdate_GCP.reset_and_update(cur_dir, KEYPAIR_INDEX_CLIENT_PRIVATE, CERTIFICATE_INDEX)
    except Exception as exc:
        log.error("%s" % str(exc))
        error_file = os.getcwd() + os.sep + "error_log.txt"
        if not os.path.isfile(error_file):
            err_write = open(error_file, 'w+')
        else:
            err_write = open(error_file, 'a+')
            err_write.write("\n\n")
        traceback.print_exc(None, err_write)
        err_write.close()


if __name__ == '__main__':
    main()
