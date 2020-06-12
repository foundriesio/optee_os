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

import ctypes
import logging
from . import sss_api as apis
from .util import hex_uid

log = logging.getLogger(__name__)


class Se05x:
    """
    Se05x specific operation
    """

    def __init__(self, session_obj):
        """
        Constructor
        :param session_obj: Instance of session
        """
        self._session = session_obj

    def debug_reset(self):
        """
        Se05x Debug reset
        :return: None
        """
        if self._session.subsystem == apis.kType_SSS_SE_SE05x:
            apis.Se05x_API_DeleteAll_Iterative(ctypes.byref(self._session.session_ctx.s_ctx))
        else:
            log.warning("Unsupported subsystem='%s'", (self._session.subsystem,))

    def get_unique_id(self):
        """
        Retrieve 18 byte unique id
        :return: Unique id
        """
        unique_id_len = ctypes.c_uint16(18)
        unique_id = (ctypes.c_uint8 * unique_id_len.value)(0)

        if self._session.subsystem == apis.kType_SSS_SE_SE05x:
            apis.sss_session_prop_get_au8(ctypes.byref(self._session.session_ctx),
                                          apis.kSSS_SessionProp_UID,
                                          ctypes.byref(unique_id),
                                          ctypes.byref(unique_id_len))
        log.info(hex_uid(unique_id))
        return hex_uid(unique_id)

    def get_cert_unique_id(self):
        """
        Retrieve cert unique id
        :return: Cert unique id
        """
        unique_id_len = ctypes.c_uint16(10)
        unique_id = (ctypes.c_uint8 * unique_id_len.value)(0)

        if self._session.subsystem == apis.kType_SSS_SE_SE05x:
            apis.sss_session_prop_get_au8(ctypes.byref(self._session.session_ctx),
                                          apis.kSSS_SE05x_SessionProp_CertUID,
                                          ctypes.byref(unique_id),
                                          ctypes.byref(unique_id_len))
        log.info(hex_uid(unique_id))
        return hex_uid(unique_id)
