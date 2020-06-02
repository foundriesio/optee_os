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

# This script is used to generate keys and CA certificates using openssl engine.
# The generated keys shall be available in azure directory
# These keys are used to provision to the secure element

import os
import sys
import logging
logging.basicConfig(format='%(message)s', level=logging.INFO)
log = logging.getLogger(__name__)


def main():
    """
    This function generates key pair and certificates for AZURE cloud.
    :return: None
    """

    # Implementation of this function has been
    # moved to simw-top/pycli/src/Provision/GenerateAZURECredentials.py
    sys.path.append(os.path.abspath(os.path.dirname(__file__)
                                    + os.sep + ".."
                                    + os.sep + "src"))
    import Provision.GenerateAZURECredentials as GenerateAZURECredentials  # pylint: disable=import-outside-toplevel

    cur_dir = os.path.abspath(os.getcwd())
    GenerateAZURECredentials.generateCredentials(cur_dir)


if __name__ == "__main__":
    main()
