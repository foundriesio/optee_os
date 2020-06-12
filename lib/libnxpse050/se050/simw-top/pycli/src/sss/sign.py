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
import binascii
import logging
from . import sss_api as apis
from .keystore import KeyStore
from .keyobject import KeyObject
from .asymmetric import Asymmetric
from .util import hash_convert, hash_convert_raw, save_to_file, \
    load_certificate, transform_key_to_list
from .const import HASH
log = logging.getLogger(__name__)


class Sign:  # pylint: disable=too-few-public-methods
    """
    Sign operation
    """

    def __init__(self, session_obj):
        """
        Constructor
        :param session_obj: Instance of session
        """
        self._session = session_obj
        self._ctx_ks = KeyStore(self._session)
        self._ctx_key = KeyObject(self._ctx_ks)
        self.signature_data = None
        self.hash_algo = apis.kAlgorithm_None

    def do_signature(self, key_id, certificate, filename,  # pylint: disable=too-many-locals, too-many-arguments
                     in_encode_format="", out_encode_format="", hash_algo=""):
        """
        DO sign operation
        :param key_id: Key index
        :param certificate: Input data to be signed
        :param filename: File name to store the signature data
        :param in_encode_format: Input data file encode format
        :param out_encode_format: Output Signature file encode format
        :param hash_algo: Hash algorithm to sign
        :return: Status
        """
        status, object_type, cipher_type = self._ctx_key.get_handle(key_id)  # pylint: disable=unused-variable
        if status != apis.kStatus_SSS_Success:
            return status

        if hash_algo == '':
            # Default hash algorithm
            if cipher_type in [apis.kSSS_CipherType_RSA, apis.kSSS_CipherType_RSA_CRT]:
                log.info("Considering Algorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA256 "
                         "as Default hash algorithm for RSA")
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

        try:
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
                        (digest, digest_len) = hash_convert(certificate, in_encode_format, self.hash_algo)
        except Exception as exc:  # pylint: disable=broad-except
            if 'Unable to load certificate' in str(exc):
                log.error("Incorrect certificate, try signing with correct certificate")
                return apis.kStatus_SSS_Fail, None
            raise exc

        digest = (ctypes.c_ubyte * digest_len)(*digest)
        mode = apis.kMode_SSS_Sign
        # Signature length set to 1024 to support all EC curves and RSA key size
        signature_len = 1024
        data_buf = (ctypes.c_uint8 * signature_len)(0)
        signature_len = ctypes.c_size_t(signature_len)

        ctx_asymm = Asymmetric(self._session, self._ctx_key, self.hash_algo, mode)
        if cipher_type in [apis.kSSS_CipherType_EC_TWISTED_ED]:
            (signature_ctype, status) = ctx_asymm.se05x_sign(digest, digest_len, data_buf, signature_len)
        else:
            (signature_ctype, status) = ctx_asymm.sign(digest, digest_len, data_buf, signature_len)
        if signature_ctype is None:
            log.error("Received signature data is empty")
            return status
        signature_full_list = list(signature_ctype)
        signature_list = signature_full_list[:int(signature_len.value)]
        if filename is not None:
            status = save_to_file(signature_list, filename,
                                  apis.kSSS_KeyPart_NONE, out_encode_format)
        der = ""
        for signature_item in signature_list:
            der += str(signature_item)
        self.signature_data = binascii.unhexlify(der)
        return status
