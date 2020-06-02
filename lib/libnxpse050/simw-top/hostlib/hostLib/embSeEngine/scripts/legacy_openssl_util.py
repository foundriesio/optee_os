# Copyright 2019 NXP
#
# This software is owned or controlled by NXP and may only be used
# strictly in accordance with the applicable license terms.  By expressly
# accepting such terms or by downloading, installing, activating and/or
# otherwise using the software, you are agreeing that you have read, and
# that you agree to comply with and are bound by, such license terms.  If
# you do not agree to be bound by the applicable license terms, then you
# may not retain, install, activate or otherwise use the software.

import os
import sys
import subprocess
import logging
logging.basicConfig(format='%(message)s', level=logging.DEBUG)
log = logging.getLogger(__name__)
cur_dir = os.path.abspath(os.path.dirname(__file__))

openssl_version = "openssl"

if sys.platform.startswith("win"):
    library_name = "a71ch_engine_legacy.dll"
    openssl = os.path.join(cur_dir, '..', '..', '..', '..', 'ext', openssl_version, 'bin', 'openssl.exe')
else:
    openssl = 'openssl'
    library_name = "liba71ch_engine_legacy.so"
openssl_engine = os.path.join(cur_dir, "..", "bin", library_name)


def run(cmd_str, ignore_result=0, exp_retcode=0):
    log.info("")
    log.info("Running command:")
    log.info("%s" % (cmd_str,))
    pipes = subprocess.Popen(
        cmd_str,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        shell=True,
    )
    std_out, std_err = pipes.communicate()
    std_out = std_out.strip()
    std_err = std_err.strip()
    if std_out != "":
        log.info("\nstd_out: \n%s" % std_out.decode())
    if not ignore_result:
        if pipes.returncode != exp_retcode:
            log.error("ERROR: Return code: %s, Expected return code: %s " % (pipes.returncode, exp_retcode))
            log.error("ERROR: std_err: %s" % std_err)
        else:
            log.info("Command execution was successful.")
        assert pipes.returncode == exp_retcode


def compare(input_file, decrypt_file):
    with open(input_file, 'rb') as raw_data:
        in_data = raw_data.read()

    with open(decrypt_file, 'rb') as decrypt_data:
        dec_data = decrypt_data.read()

    assert in_data == dec_data

    raw_data.close()
    decrypt_data.close()