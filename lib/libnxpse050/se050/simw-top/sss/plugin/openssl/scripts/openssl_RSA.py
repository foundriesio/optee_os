# Copyright 2019 NXP
#
# This software is owned or controlled by NXP and may only be used
# strictly in accordance with the applicable license terms.  By expressly
# accepting such terms or by downloading, installing, activating and/or
# otherwise using the software, you are agreeing that you have read, and
# that you agree to comply with and are bound by, such license terms.  If
# you do not agree to be bound by the applicable license terms, then you
# may not retain, install, activate or otherwise use the software.


"""

Validation of OpenSSL Engine using RSA keys

This example showcases crypto operations and sign verify operations using RSA keys.

"""

import argparse

from openssl_util import *

log = logging.getLogger(__name__)

example_text = '''

Example invocation::

    python %s --key_type rsa2048
    python %s --key_type rsa4096 --connection_data 127.0.0.1:8050

''' % (__file__, __file__,)


def parse_in_args():
    parser = argparse.ArgumentParser(
        description=__doc__, epilog=example_text,
        formatter_class=argparse.RawTextHelpFormatter)
    required = parser.add_argument_group('required arguments')
    optional = parser.add_argument_group('optional arguments')
    required.add_argument(
        '--key_type',
        default="",
        help='Supported key types =>  ``%s``' % ("``, ``".join(SUPPORTED_RSA_KEY_TYPES)),
        required=True)
    optional.add_argument(
        '--connection_data',
        default="none",
        help='Parameter to connect to SE => eg. ``COM3``, ``127.0.0.1:8050``, ``none``. Default: ``none``')

    if len(sys.argv) == 1:
        parser.print_help(sys.stderr)
        return None

    args = parser.parse_args()

    if args.key_type not in SUPPORTED_RSA_KEY_TYPES:
        parser.print_help(sys.stderr)
        return None

    if args.connection_data.find(':') >= 0:
        port_data = args.connection_data.split(':')
        jrcp_host_name = port_data[0]
        jrcp_port = port_data[1]
        os.environ['JRCP_HOSTNAME'] = jrcp_host_name
        os.environ['JRCP_PORT'] = jrcp_port
        os.environ['EX_SSS_BOOT_SSS_PORT'] = args.connection_data
        log.info("JRCP_HOSTNAME: %s" % jrcp_host_name)
        log.info("JRCP_PORT: %s" % jrcp_port)
        log.info("EX_SSS_BOOT_SSS_PORT: %s" % args.connection_data)

    return args


def main():
    args = parse_in_args()
    if args is None:
        return

    key_size = args.key_type.replace("rsa", "")

    keys_dir = os.path.join(cur_dir, '..', 'keys', args.key_type)
    if not os.path.exists(keys_dir):
        log.error("keys are not generated. Please run \"openssl_provisionRSA.py\" first.")

    output_dir = cur_dir + os.sep + "output"
    if not os.path.exists(output_dir):
        log.info(" %s Folder does not exist. Creating it.")
        os.mkdir(output_dir)

    rsa_key_pair = keys_dir + os.sep + "rsa_" + key_size + "_1_prv.pem"
    rsa_ref_key_pair = keys_dir + os.sep + "rsa_" + key_size + "_ref_prv.pem"
    sha_type = "-sha256"
    input_data = cur_dir + os.sep + "inputData.txt"
    input_data_sha = output_dir + os.sep + "inputData_sha256.txt"
    encrypt_data = output_dir + os.sep + "encrypt_data.txt"
    decrypt_data = output_dir + os.sep + "decrypt_data.txt"
    sign_data = output_dir + os.sep + "sign_data.txt"

    # Calculate SHA of input data - Used for sign/verify using pkeyutl
    run("%s dgst %s -binary -out %s %s" % (openssl, sha_type, input_data_sha, input_data,))

    # USE SSS LAYER FOR CRYPTO OPERATIONS
    log.info(";")
    log.info(";")
    log.info(";")
    log.info("############ STARTING CRYPTO OPERATIONS USING SSS APIs ##################")
    log.info(";")
    log.info(";")
    log.info(";")
    log.info("### PUBLIC ENCRYPT USING SSS - PKCSV1.5")
    run("%s rsautl -engine %s -encrypt -inkey %s -out %s -in %s" %
        (openssl, openssl_engine, rsa_ref_key_pair, encrypt_data, input_data))
    run("%s rsautl -engine %s -decrypt -inkey %s -in %s -out %s" %
        (openssl, openssl_engine, rsa_ref_key_pair, encrypt_data, decrypt_data))
    compare(input_data, decrypt_data)

    log.info("### PUBLIC ENCRYPT USING SSS - OAEP")
    run("%s rsautl -engine %s -encrypt -inkey %s -out %s -in %s -oaep" %
        (openssl, openssl_engine, rsa_ref_key_pair, encrypt_data, input_data))
    log.info("### PRIVATE DECRYPT USING SSS - OAEP")
    run("%s rsautl -engine %s -decrypt -inkey %s -in %s -out %s -oaep" %
        (openssl, openssl_engine, rsa_ref_key_pair, encrypt_data, decrypt_data))
    compare(input_data, decrypt_data)

    log.info("### SIGNING USING SSS - PKCSV1.5")
    run("%s dgst -engine %s %s -sign %s -out %s %s" %
        (openssl, openssl_engine, sha_type, rsa_ref_key_pair, sign_data, input_data))
    log.info("### VERIFY USING SSS - PKCSV1.5")
    run("%s dgst -engine %s %s -prverify %s -signature %s %s" %
        (openssl, openssl_engine, sha_type, rsa_ref_key_pair, sign_data, input_data))

    # USE OPENSSL STACK FOR CRYPTO OPERATIONS
    log.info("############ STARTING CRYPTO OPERATIONS USING OPENSSL ##################")
    log.info(";")
    log.info(";")
    log.info(";")
    log.info("### PUBLIC ENCRYPT USING OPENSSL - PKCSV1.5")
    log.info(";")
    log.info(";")
    log.info(";")
    log.info("### PUBLIC ENCRYPT USING OPENSSL - PKCSV1.5")
    run("%s rsautl -encrypt -inkey %s -out %s -in %s" %
        (openssl, rsa_key_pair, encrypt_data, input_data))
    log.info("### PRIVATE DECRYPT USING OPENSSL - PKCSV1.5")
    run("%s rsautl -decrypt -inkey %s -in %s -out %s" %
        (openssl, rsa_key_pair, encrypt_data, decrypt_data))
    compare(input_data, decrypt_data)

    log.info("### PUBLIC ENCRYPT USING OPENSSL - OAEP")
    run("%s rsautl -encrypt -inkey %s -out %s -in %s  -oaep" %
        (openssl, rsa_key_pair, encrypt_data, input_data))
    log.info("### PRIVATE DECRYPT USING OPENSSL - OAEP")
    run("%s rsautl -decrypt -inkey %s -in %s -out %s -oaep" %
        (openssl, rsa_key_pair, encrypt_data, decrypt_data))
    compare(input_data, decrypt_data)
    log.info("### SIGNING USING OPENSSL - PKCSV1.5")
    run("%s dgst %s -sign %s -out %s %s" %
        (openssl, sha_type, rsa_key_pair, sign_data, input_data))
    log.info("### VERIFY USING OPENSSL - PKCSV1.5")
    run("%s dgst %s -prverify %s -signature %s %s" %
        (openssl, sha_type, rsa_key_pair, sign_data, input_data))

    # COUNTERPART TESTING FOT CRYPTO OPERATIONS
    log.info(";")
    log.info(";")
    log.info(";")
    log.info("############ STARTING COUNTERPART TESTING ##################")
    log.info(";")
    log.info(";")
    log.info(";")
    log.info("### PUBLIC ENCRYPT USING SSS - PKCSV1.5")
    run("%s rsautl -engine %s -encrypt -inkey %s -out %s -in %s" %
        (openssl, openssl_engine, rsa_ref_key_pair, encrypt_data, input_data))
    log.info("### PRIVATE DECRYPT USING OPENSSL - PKCSV1.5")
    run("%s rsautl -decrypt -inkey %s -in %s -out %s" %
        (openssl, rsa_key_pair, encrypt_data, decrypt_data))
    compare(input_data, decrypt_data)

    log.info("### PUBLIC ENCRYPT USING OPENSSL - PKCSV1.5")
    run("%s rsautl -encrypt -inkey %s -out %s -in %s" %
        (openssl, rsa_key_pair, encrypt_data, input_data))
    log.info("### PRIVATE DECRYPT USING SSS - PKCSV1.5")
    run("%s rsautl -engine %s -decrypt -inkey %s -in %s -out %s" %
        (openssl, openssl_engine, rsa_ref_key_pair, encrypt_data, decrypt_data))
    compare(input_data, decrypt_data)

    log.info("### PUBLIC ENCRYPT USING SSS - OAEP")
    run("%s rsautl -engine %s -encrypt -inkey %s -out %s -in %s  -oaep" %
        (openssl, openssl_engine, rsa_ref_key_pair, encrypt_data, input_data))
    log.info("### PRIVATE DECRYPT USING OPENSSL - OAEP")
    run("%s rsautl -decrypt -inkey %s -in %s -out %s -oaep" %
        (openssl, rsa_key_pair, encrypt_data, decrypt_data))
    compare(input_data, decrypt_data)

    log.info("### PUBLIC ENCRYPT USING OPENSSL - OAEP")
    run("%s rsautl -encrypt -inkey %s -out %s -in %s  -oaep" %
        (openssl, rsa_ref_key_pair, encrypt_data, input_data))
    log.info("### PRIVATE DECRYPT USING SSS - OAEP")
    run("%s rsautl -engine %s -decrypt -inkey %s -in %s -out %s -oaep" %
        (openssl, openssl_engine, rsa_ref_key_pair, encrypt_data, decrypt_data))
    compare(input_data, decrypt_data)

    log.info("### SIGNING USING SSS - PKCSV1.5")
    run("%s dgst -engine %s %s -sign %s -out %s %s" %
        (openssl, openssl_engine, sha_type, rsa_ref_key_pair, sign_data, input_data))

    log.info("### VERIFY USING OPENSSL - PKCSV1.5")
    run("%s dgst %s -prverify %s -signature %s %s" %
        (openssl, sha_type, rsa_key_pair, sign_data, input_data))
    log.info("### SIGNING USING OPENSSL - PKCSV1.5")
    run("%s dgst %s -sign %s -out %s %s" %
        (openssl, sha_type, rsa_key_pair, sign_data, input_data))
    log.info("### VERIFY USING SSS - PKCSV1.5")
    run("%s dgst -engine %s %s -prverify %s -signature %s %s" %
        (openssl, openssl_engine, sha_type, rsa_ref_key_pair, sign_data, input_data))

    # OPENSSL SOFTWARE FALLBACK TEST CASES
    log.info(";")
    log.info(";")
    log.info(";")
    log.info("############ OPENSSL SOFTWARE FALLBACK TEST CASES ##################")
    log.info(";")
    log.info(";")
    log.info(";")
    log.info("### PUBLIC ENCRYPT USING SSS - PKCSV1.5")
    run("%s rsautl -engine %s -encrypt -inkey %s -out %s -in %s" %
        (openssl, openssl_engine, rsa_key_pair, encrypt_data, input_data))
    run("%s rsautl -engine %s -decrypt -inkey %s -in %s -out %s" %
        (openssl, openssl_engine, rsa_key_pair, encrypt_data, decrypt_data))
    compare(input_data, decrypt_data)

    log.info("### PUBLIC ENCRYPT USING SSS - OAEP")
    run("%s rsautl -engine %s -encrypt -inkey %s -out %s -in %s -oaep" %
        (openssl, openssl_engine, rsa_key_pair, encrypt_data, input_data))
    log.info("### PRIVATE DECRYPT USING SSS - OAEP")
    run("%s rsautl -engine %s -decrypt -inkey %s -in %s -out %s -oaep" %
        (openssl, openssl_engine, rsa_key_pair, encrypt_data, decrypt_data))
    compare(input_data, decrypt_data)

    log.info("### SIGNING USING SSS - PKCSV1.5")
    run("%s dgst -engine %s %s -sign %s -out %s %s" %
        (openssl, openssl_engine, sha_type, rsa_key_pair, sign_data, input_data))
    log.info("### VERIFY USING SSS - PKCSV1.5")
    run("%s dgst -engine %s %s -prverify %s -signature %s %s" %
        (openssl, openssl_engine, sha_type, rsa_key_pair, sign_data, input_data))

    log.info("##############################################################")
    log.info("#                                                            #")
    log.info("#     Program completed successfully                         #")
    log.info("#                                                            #")
    log.info("##############################################################")


if __name__ == '__main__':
    main()
