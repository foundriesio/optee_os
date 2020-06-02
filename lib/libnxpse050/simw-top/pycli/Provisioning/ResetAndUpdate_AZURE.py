# Copyright 2018-2020 NXP
#
# This software is owned or controlled by NXP and may only be used
# strictly in accordance with the applicable license terms.  By expressly
# accepting such terms or by downloading, installing, activating and/or
# otherwise using the software, you are agreeing that you have read, and
# that you agree to comply with and are bound by, such license terms.  If
# you do not agree to be bound by the applicable license terms, then you
# may not retain, install, activate or otherwise use the software.
#

"""License text"""

# This script is used to provision key and certificate to secure element.
# These Provisioned keys and certificates are used in azure demo

import os
import sys
import logging

log = logging.getLogger(__name__)
logging.basicConfig(format='%(message)s', level=logging.INFO)

# Keypair and certificate index for AZURE
# (simw-top\demos\ksdk\azure\azure_iot_config.h)
KEYPAIR_INDEX_CLIENT_PRIVATE = 0x223344
CERTIFICATE_INDEX = 0x223345


def main():
    """
    This function provision the generated key pair and certificates for AZURE cloud.
    :return: None
    """

    # Implementation of this function has been
    # moved to simw-top/pycli/src/Provision/ResetAndUpdate_AZURE.py
    sys.path.append(os.path.abspath(os.path.dirname(__file__)
                                    + os.sep + ".."
                                    + os.sep + "src"
                                    + os.sep + "Provision"))
    import Provision.ResetAndUpdate_AZURE as ResetAndUpdate_AZURE  # pylint: disable=import-outside-toplevel

    cur_dir = os.getcwd()
    ResetAndUpdate_AZURE.reset_and_update(cur_dir, KEYPAIR_INDEX_CLIENT_PRIVATE, CERTIFICATE_INDEX)


if __name__ == '__main__':
    main()
