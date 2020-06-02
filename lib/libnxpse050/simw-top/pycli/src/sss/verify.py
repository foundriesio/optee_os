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

import os
import ctypes
import logging
import binascii
from . import sss_api as apis
from .keystore import KeyStore
from .keyobject import KeyObject
from .asymmetric import Asymmetric
from .const import HASH
from .util import hash_convert, hash_convert_raw, parse_signature, \
    load_certificate, transform_key_to_list

log = logging.getLogger(__name__)


class Verify:  # pylint: disable=too-few-public-methods
    """
    Verify operation
    """

    def __init__(self, session_obj):
        """
        Constructor
        :param session_obj: Instance of session
        """
        self._session = session_obj
        self._ctx_ks = KeyStore(self._session)
        self._ctx_key = KeyObject(self._ctx_ks)
        self.hash_algo = apis.kAlgorithm_None

    def do_verification(self, key_id, certificate, signature_file,  # pylint: disable=too-many-locals, too-many-arguments
                        encode_format="", hash_algo=''):
        """
        Do verify operation
        :param key_id: Key index
        :param certificate: Input data to verify with signature
        :param signature_file: Input signed data to verify with raw data
        :param encode_format: file Encode format. Eg: PEM, DER
        :param hash_algo: hash algorithm for verify
        :return: Status
        """

        status, object_type, cipher_type = self._ctx_key.get_handle(key_id)  # pylint: disable=unused-variable
        if status != apis.kStatus_SSS_Success:
            return status

        if hash_algo == '':
            # Default hash algorithm
            if cipher_type in [apis.kSSS_CipherType_RSA, apis.kSSS_CipherType_RSA_CRT]:
                log.info("Considering Algorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA256"
                         " as Default hash algorithm for RSA")
                self.hash_algo = apis.kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA256
            elif cipher_type in [apis.kSSS_CipherType_EC_TWISTED_ED]:
                log.info("Considering Algorithm_SSS_SHA512 as Default hash algorithm")
                self.hash_algo = apis.kAlgorithm_SSS_SHA512
            else:
                log.info("Considering Algorithm_SSS_SHA256 as Default hash algorithm")
                self.hash_algo = apis.kAlgorithm_SSS_SHA256
        else:
            # Take hash algorithm from user
            self.hash_algo = HASH[hash_algo]

        if cipher_type in [apis.kSSS_CipherType_EC_TWISTED_ED]:
            if not os.path.isfile(certificate):
                cert_hex = binascii.hexlify(str.encode(certificate))
                digest = transform_key_to_list(cert_hex)
                digest_len = len(digest)
            else:
                digest, digest_len = load_certificate(certificate)

        else:
            if not os.path.isfile(certificate):
                (digest, digest_len) = hash_convert_raw(certificate, self.hash_algo)
            else:
                (digest, digest_len) = hash_convert(certificate, encode_format, self.hash_algo)
        try:
            (signature, signature_len) = parse_signature(signature_file, encode_format)
        except IOError as exc:
            log.error(exc)
            return apis.kStatus_SSS_Fail
        digest_ctype = (ctypes.c_ubyte * digest_len)(*digest)
        mode = apis.kMode_SSS_Verify
        signature_ctype = (ctypes.c_uint8 * signature_len)(*signature)
        signature_len_ctype = ctypes.c_size_t(signature_len)

        ctx_asymm = Asymmetric(self._session, self._ctx_key, self.hash_algo, mode)
        if cipher_type in [apis.kSSS_CipherType_EC_TWISTED_ED]:
            status = ctx_asymm.se05x_verify(
                digest_ctype, digest_len, signature_ctype, signature_len_ctype)
        else:
            status = ctx_asymm.verify(
                digest_ctype, digest_len, signature_ctype, signature_len_ctype)
        return status
