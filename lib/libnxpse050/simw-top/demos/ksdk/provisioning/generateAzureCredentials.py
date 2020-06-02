# Copyright 2018 NXP
#
# This software is owned or controlled by NXP and may only be used
# strictly in accordance with the applicable license terms.  By expressly
# accepting such terms or by downloading, installing, activating and/or
# otherwise using the software, you are agreeing that you have read, and
# that you agree to comply with and are bound by, such license terms.  If
# you do not agree to be bound by the applicable license terms, then you
# may not retain, install, activate or otherwise use the software.
#

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.asymmetric import ec, rsa
from cryptography.hazmat.primitives import serialization
from cryptography.x509.base import load_pem_x509_certificate
import os
import sys

header_file = os.path.abspath(os.path.dirname(__file__))+os.sep+"azure_credentials.h"

c_header = "/* Copyright 2019 NXP \n\
 * \n\
 * This software is owned or controlled by NXP and may only be used \n\
 * strictly in accordance with the applicable license terms.  By expressly \n\
 * accepting such terms or by downloading, installing, activating and/or \n\
 * otherwise using the software, you are agreeing that you have read, and \n\
 * that you agree to comply with and are bound by, such license terms.  If \n\
 * you do not agree to be bound by the applicable license terms, then you \n\
 * may not retain, install, activate or otherwise use the software. \n\
 */\n\n"

def writeHFile(o, var_name, binary):
    count = 0
    var_name_string = "const uint8_t %s[] = {"%var_name
    o.write(var_name_string.encode())
    for i in binary:
        if count % 8 == 0:
            o.write(" \\\n".encode())
        elif count % 4 == 0:
            o.write("    ".encode())
        value_to_write = "0x%02X, "%(i,)
        o.write(value_to_write.encode())
        count = count + 1

    o.write("};\n\n".encode())


def main(keypair_file, certificate_file):
    with open(keypair_file, "rb") as infile:
        keypair = infile.read()

    with open(certificate_file, "rb") as infile:
        certificate = infile.read()

    priv_pem_obj = serialization.load_pem_private_key(keypair, None, default_backend())
    cert_pem_obj = load_pem_x509_certificate(certificate, default_backend())
    priv_der = priv_pem_obj.private_bytes(serialization.Encoding.DER, serialization.PrivateFormat.PKCS8, serialization.NoEncryption())
    cert_der = cert_pem_obj.public_bytes(serialization.Encoding.DER)

    o = open(header_file, "wb")
    o.write(c_header.encode())
    o.write("/*Generated file. Do not modify*/\n".encode())
    writeHFile(o, "client_key", priv_der)
    writeHFile(o, "client_cer", cert_der)
    o.close()



def usage():
    print("Usage:\ngenerateAzureCredentials.py <keypair_file> <certificate_file>")
    sys.exit()

if __name__ == "__main__":
    argc = len(sys.argv)
    argv = sys.argv
    if(argc != 3):
        usage()
    main(argv[1], argv[2])
    print("Generated %s"%header_file)
