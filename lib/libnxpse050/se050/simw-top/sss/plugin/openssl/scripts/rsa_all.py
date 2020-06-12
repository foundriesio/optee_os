# Copyright 2019,2020 NXP
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


This example injects keys with different supported RSA keys,
then showcases Crypto & sign verify operations using those keys.

"""

import argparse

from openssl_util import *

example_text = '''

Example invocation::

    python %s
    python %s --connection_data 169.254.0.1:8050
    python %s --connection_data 127.0.0.1:8050 --connection_type jrcpv2
    python %s --connection_data COM3

''' % (__file__, __file__, __file__, __file__,)


def parse_in_args():
    parser = argparse.ArgumentParser(
        description=__doc__,
        epilog=example_text,
        formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument(
        '--connection_data',
        default="none",
        help='Parameter to connect to SE => eg. ``COM3``, ``127.0.0.1:8050``, ``none``. Default: ``none``')
    parser.add_argument(
        '--connection_type',
        default="t1oi2c",
        help='Supported connection types => ``%s``. Default: ``t1oi2c``' % ("``, ``".join(SUPPORTED_CONNECTION_TYPES)))
    parser.add_argument(
        '--subsystem',
        default="se050",
        help='Supported subsystem => ``se050``. Default: ``se050``')

    args = parser.parse_args()

    if args.subsystem not in ["se050"]:
        parser.print_help(sys.stderr)
        return None

    if args.connection_data.find(':') >= 0:
        port_data = args.connection_data.split(':')
        jrcp_host_name = port_data[0]
        jrcp_port = port_data[1]
        os.environ['JRCP_HOSTNAME'] = jrcp_host_name
        os.environ['JRCP_PORT'] = jrcp_port
        log.info("JRCP_HOSTNAME: %s" % jrcp_host_name)
        log.info("JRCP_PORT: %s" % jrcp_port)
        if args.connection_type == "t1oi2c":
            args.connection_type = "jrcpv1"
    elif args.connection_data.find('COM') >= 0:
        if args.connection_type == "t1oi2c":
            args.connection_type = "vcom"
    elif args.connection_data.find('none') >= 0:
        if args.subsystem == "a71ch":
            args.connection_type = "sci2c"
    else:
        parser.print_help(sys.stderr)
        return None

    if args.connection_type not in SUPPORTED_CONNECTION_TYPES:
        parser.print_help(sys.stderr)
        return None

    return args


def main():
    args = parse_in_args()
    if args is None:
        return

    rsa_bit_len = [
        "rsa1024",
        "rsa2048",
        "rsa3072",
        "rsa4096"
    ]

    python_exe = sys.executable

    for bit_len in rsa_bit_len:
        print(bit_len)
        run("%s openssl_provisionRSA.py --key_type %s --connection_type %s --connection_data %s" %
            (python_exe, bit_len, args.connection_type, args.connection_data))
        run("%s openssl_RSA.py --key_type %s --connection_data %s" % (python_exe, bit_len, args.connection_data))


if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG)
    main()
