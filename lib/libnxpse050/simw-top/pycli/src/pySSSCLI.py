# Copyright 2018,2019 NXP
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
import os
sss_dir = os.path.abspath(os.getcwd()
                                + os.sep + ".."
                                + os.sep + ".."
                                + os.sep + "pycli"
                                + os.sep + "src")
sys.path.append(sss_dir)
import cli

if __name__ == '__main__':
    cli.cli.cli(sys.argv[1:])
    #cli.cli.cli("a71ch reset".split())
    #cli.cli.cli("generate ecc 0x12322 256".split())
