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


# This script is used to provision key and certificate to secure element.
# These Provisioned keys and certificates are used in azure demo

from .Provision_util import *
from . import cloud_credentials


def reset_and_update(cur_dir, keypair_index_private, certificate_index):

    # Session Open
    session_obj = session_open()

    # Read UID from the device
    SE_UID = get_unique_id(session_obj)
    if SE_UID is None or SE_UID == 0:
        return STATUS_FAIL

    INPUT_DIR = cur_dir + os.sep + "azure" + os.sep

    credentials = cloud_credentials.create_credentials(SE_UID)

    ecc_key_pair_file = INPUT_DIR + credentials["DEVICE_KEY"]
    certificate_file = INPUT_DIR + credentials["DEVICE_CERT"]
    refpem_file = INPUT_DIR + credentials["DEVICE_REF_KEY"]

    if not os.path.isfile(certificate_file):
        log.info("Certificates not created\nRun GenerateAZURECredentials.py to create certificates")
        return STATUS_FAIL

    # Reset the Secure Element
    reset(session_obj)

    # Inject ECC key Pair
    status = set_ecc_pair(session_obj, keypair_index_private, ecc_key_pair_file)
    if status != apis.kStatus_SSS_Success:
        return STATUS_FAIL

    # Inject Certificate
    status = set_cert(session_obj, certificate_index, certificate_file)
    if status != apis.kStatus_SSS_Success:
        return STATUS_FAIL

    # Create Reference key
    status = refpem_ecc(session_obj, keypair_index_private, refpem_file)
    if status != apis.kStatus_SSS_Success:
        return STATUS_FAIL
    else:
        session_close(session_obj)
        log.info("##############################################################")
        log.info("#                                                            #")
        log.info("#     Program Completed Successfully                         #")
        log.info("#                                                            #")
        log.info("##############################################################")
        return STATUS_SUCCESS
