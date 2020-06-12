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

import logging
from .util import generate_openssl_ecc_refkey, generate_openssl_rsa_refkey
from .getkey import Get
from . import sss_api as apis

log = logging.getLogger(__name__)


class RefPem:
    """
    Create reference key
    """

    def __init__(self, session_obj):
        """
        Constructor
        :param session_obj: Instance of session
        """
        self._session = session_obj
        self._get_object = Get(session_obj)

    def do_ecc_refpem_pair(self, key_id, file_name, encode_format="", password="nxp"):
        """
        Create ECC reference key pair
        :param key_id: Key index of the private key
        :param file_name: File name to store reference key
        :param encode_format: Encode format to store key. Eg: DER, PEM
        :param password: Password to encrypt pkcs12 reference key
        :return: Status
        """
        status = self._get_object.get_key(key_id)

        if status == apis.kStatus_SSS_Success:
            if self._get_object.key_type == apis.kSSS_KeyPart_Pair:
                key_part = '10'
                status = generate_openssl_ecc_refkey(self._session.session_ctx,
                                                     self._get_object.key, key_id,
                                                     file_name, key_part,
                                                     self._get_object.curve_id,
                                                     encode_format, password,
                                                     self._get_object)
            else:
                log.error("Reference Key creation is not supported for this key type: %s",
                          (self._get_object.key_type,))
                status = apis.kStatus_SSS_Fail
        return status

    def do_ecc_refpem_pub(self, key_id, file_name, encod_format="", password="nxp"):
        """
        Create ECC reference public key
        :param key_id: Key index
        :param file_name: File name to store reference key
        :param encod_format: Encode format to store key. Eg: DER, PEM
        :param password: Password to encrypt pkcs12 reference key
        :return: Status
        """
        status = self._get_object.get_key(key_id)
        if status == apis.kStatus_SSS_Success:
            if self._get_object.key_type in [apis.kSSS_KeyPart_Pair, apis.kSSS_KeyPart_Public]:
                key_part = '20'
                status = generate_openssl_ecc_refkey(self._session.session_ctx,
                                                     self._get_object.key, key_id, file_name,
                                                     key_part, self._get_object.curve_id,
                                                     encod_format, password,
                                                     self._get_object)
            else:
                log.error("Reference Key creation is not supported for this key type: %s",
                          (self._get_object.key_type,))
                status = apis.kStatus_SSS_Fail
        return status

    def do_rsa_refpem_pair(self, key_id, file_name, encode_format="", password="nxp"):
        """
        Create RSA reference key pair
        :param key_id: Key index
        :param file_name: File name to store reference key
        :param encode_format: Encode format to store key. Eg: DER, PEM
        :param password: Password to encrypt pkcs12 reference key
        :return: Status
        """
        status = self._get_object.get_key(key_id)
        if status == apis.kStatus_SSS_Success:
            if self._get_object.key_type in [apis.kSSS_KeyPart_Pair, apis.kSSS_KeyPart_Public]:
                key_size = 0
                if self._get_object.key_size == 4400:
                    key_size = 4096
                elif self._get_object.key_size == 3376:
                    key_size = 3072
                elif self._get_object.key_size == 2352:
                    key_size = 2048
                elif self._get_object.key_size == 1296:
                    key_size = 1024
                status = generate_openssl_rsa_refkey(self._session.session_ctx,
                                                     self._get_object.key, key_id, file_name,
                                                     key_size, encode_format, password,
                                                     self._get_object)
            else:
                log.error("Reference Key creation is not supported for this key type: %s",
                          (self._get_object.key_type,))
                status = apis.kStatus_SSS_Fail
        return status
