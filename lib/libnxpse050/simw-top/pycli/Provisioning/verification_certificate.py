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
# The generated keys shall be available in aws directory
# These keys are used to provision to the secure element

import os
import sys
import subprocess
import logging
log = logging.getLogger(__name__)


def main():
    """
    Generates keys and CA certificates using openssl engine.
    :return: None
    """
    cur_dir = os.path.abspath(os.path.dirname(__file__))
    openssl_dir = os.path.abspath(os.path.dirname(__file__)
                                  + ".."
                                  + os.sep + ".."
                                  + os.sep + "ext"
                                  + os.sep + "openssl"
                                  + os.sep + "bin")

    os.environ['PATH'] = openssl_dir + os.pathsep + os.environ["PATH"]

    if "linux" in sys.platform:
        OPENSSL_EXE = 'openssl'
    else:
        OPENSSL_EXE = os.path.join(cur_dir, '..', '..', 'ext', 'openssl', 'bin', 'openssl.exe')
    OPENSSL_CONFIG_FILE = os.path.join(cur_dir, "..", "..", "ext", "openssl", "ssl", "openssl.cnf")
    os.environ['OPENSSL_CONF'] = OPENSSL_CONFIG_FILE
    ROOTCA_CERT = sys.argv[1]
    ROOTCA_KEY = sys.argv[2]
    VERIFICATION_CODE = sys.argv[3]
    VERIFY_CERT = "verifyCert.cer"
    VERIFY_KEY = "verifyKey.pem"
    VERIFY_CSR = "verify_csr.csr"

    error_file = cur_dir + os.sep + "error_log.txt"
    err_write = open(error_file, 'wb+')

    output_file = cur_dir + os.sep + "output_log.txt"
    output_write = open(output_file, 'wb+')

    a = subprocess.Popen("\"%s\" ecparam -name prime256v1 -genkey -out \"%s\"" %
                         (OPENSSL_EXE, VERIFY_KEY),
                         shell=True, stdout=output_write,
                         stderr=err_write)
    a.wait()

    a = subprocess.Popen("\"%s\" req -new -key \"%s\" -subj \"/CN=%s\" -out \"%s\"" %
                         (OPENSSL_EXE, VERIFY_KEY, VERIFICATION_CODE, VERIFY_CSR),
                         shell=True, stdout=output_write, stderr=err_write)
    a.wait()

    a = subprocess.Popen("\"%s\" x509 -req -sha256 -days 2800 -in \"%s\" -CAserial root_srl.srl "
                         "-CAcreateserial -CA \"%s\" -CAkey \"%s\" -out \"%s\"" %
                         (OPENSSL_EXE, VERIFY_CSR, ROOTCA_CERT, ROOTCA_KEY, VERIFY_CERT),
                         shell=True, stdout=output_write, stderr=err_write)
    a.wait()

    output_write.close()
    err_write.close()
    os.remove(VERIFY_KEY)
    os.remove(VERIFY_CSR)
    os.remove("root_srl.srl")


def usage():
    """
    Usage information
    :return: None
    """
    print("Usage:\n%s <RootCA_Certificate> <RootCA_Keypair> <verification_code>" % sys.argv[0])


if __name__ == "__main__":
    if len(sys.argv) == 4:
        main()
    else:
        usage()
