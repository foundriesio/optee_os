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
import os
import pickle
import logging
from inspect import currentframe
from . import authkey
from . import sss_api as apis
from . import util
from . import const
from .util import status_to_str
log = logging.getLogger(__name__)


class Session:  # pylint: disable=too-many-instance-attributes
    """
    Session open and close operation
    """
    def __init__(self, from_pickle=True, host_session=False):
        """
        Constructor
        :param from_pickle: take session parameters from pickle file or not
        :param host_session: whether session is host session or not
        """
        if from_pickle:
            # Check for host session pkl file, To support parallel session with se050 session
            if host_session:
                pkl = util.get_host_session_pkl_path()
            else:
                pkl = util.get_session_pkl_path()
            if os.path.isfile(pkl):
                pkl_session = open(pkl, 'rb')
                session_params = pickle.load(pkl_session)
                try:
                    pkl_v_major = session_params["pkl_v_major"]
                except KeyError:
                    pkl_v_major = 0
                if 1 == pkl_v_major:  # pylint: disable=misplaced-comparison-constant
                    pass
                else:
                    raise const.SSSUsageError("Session data is in old format. "
                                              "Please close and open session again.")

                # Public
                self.subsystem = session_params['subsystem']

                # Protected
                self._connection_method = session_params['connection_method']
                self._port_name = session_params['port_name']
                self._auth_type = session_params['auth_type']

            else:
                log.error("No open session, try connecting first")
                log.error("Run 'ssscli connect --help' for more information.")
                raise Exception("No open session, try connecting first")

        # Public
        self.session_ctx = None

        # Protected
        self._host_session = None
        self._host_keystore = None
        self._connect_ctx = None
        self.session_policy = None
        self._auth_id = 0
        self._connection_type = apis.kSSS_ConnectionType_Plain

    def open_host_session(self, subsystem):
        """
        Open Host session
        :param subsystem: Host session subsystem. Eg: Mbedtls, Openssl
        :return: status
        """
        if not self.session_ctx:
            self.session_ctx = apis.sss_session_t()
        status = apis.sss_session_open(
            ctypes.byref(self.session_ctx), subsystem, 0,
            apis.kSSS_ConnectionType_Plain, None)
        if status == apis.kStatus_SSS_Success:
            self.subsystem = subsystem
        return status

    def session_open(self):  # pylint: disable=too-many-branches
        """
        Session open
        :return: status
        """
        if not self.session_ctx:
            self.session_ctx = apis.sss_session_t()

        session_data = None

        self._connect_ctx = apis.SE_Connect_Ctx_t()
        self._connect_ctx.connType = self._connection_method
        port_name = ctypes.create_string_buffer(1024)
        port_name.value = self._port_name.encode('utf-8')
        self._connect_ctx.portName = apis.String(port_name)

        if self.subsystem in [apis.kType_SSS_SE_A71CH, apis.kType_SSS_SE_A71CL]:
            session_data = ctypes.byref(self._connect_ctx)

        elif self.subsystem == apis.kType_SSS_SE_SE05x:
            self.session_ctx = apis.sss_se05x_session_t()

            # apply session policy
            if self.session_policy is not None:
                self._connect_ctx.session_policy = ctypes.pointer(self.session_policy)

            self._connect_ctx.auth.authType = self._auth_type

            # Host Session open
            if self._auth_type in [apis.kSE05x_AuthType_UserID,
                                   apis.kSE05x_AuthType_AESKey,
                                   apis.kSE05x_AuthType_ECKey,
                                   apis.kSE05x_AuthType_SCP03]:

                status = self.setup_counter_part_session()
                if status != apis.kStatus_SSS_Success:
                    raise Exception("Could not open host session. Status = %s" % (status,))

                if self._auth_type == apis.kSE05x_AuthType_SCP03:
                    self._connection_type = apis.kSSS_ConnectionType_Encrypted
                    self._se05x_prepare_host_platformscp()

                elif self._auth_type == apis.kSE05x_AuthType_UserID:
                    self._auth_id = authkey.SE050_AUTHID_USER_ID
                    self._connection_type = apis.kSSS_ConnectionType_Password
                    obj = apis.sss_object_t()
                    self._host_crypto_alloc_setkeys(currentframe().f_lineno, obj,
                                                    apis.kSSS_CipherType_UserID,
                                                    authkey.SE050_AUTHID_USER_ID_VALUE)
                    self._connect_ctx.auth.ctx.idobj.pObj = ctypes.pointer(obj)

                elif self._auth_type == apis.kSE05x_AuthType_AESKey:
                    self._auth_id = authkey.SE050_AUTHID_AESKEY
                    self._connection_type = apis.kSSS_ConnectionType_Encrypted
                    self._se05x_prepare_host_applet_scp03_keys()

                elif self._auth_type == apis.kSE05x_AuthType_ECKey:
                    self._auth_id = authkey.SE050_AUTHID_ECKEY
                    self._connection_type = apis.kSSS_ConnectionType_Encrypted
                    self._se05x_prepare_host_eckey()
            session_data = ctypes.byref(self._connect_ctx)

        elif self.subsystem in [apis.kType_SSS_mbedTLS, apis.kType_SSS_OpenSSL]:
            session_data = apis.String(port_name)

        status = apis.sss_session_open(
            ctypes.byref(self.session_ctx), self.subsystem, self._auth_id,
            self._connection_type, session_data)

        if status != apis.kStatus_SSS_Success:
            subsystem_str = list(const.SUBSYSTEM_TYPE.keys())[
                list(const.SUBSYSTEM_TYPE.values()).index(self.subsystem)]
            connection_type_str = list(const.CONNECTION_TYPE.keys())[
                list(const.CONNECTION_TYPE.values()).index(self._connection_method)]
            log.warning("#     Connection parameters:")
            log.warning("#     subsystem       : %s" % subsystem_str)
            log.warning("#     connection_type : %s" % connection_type_str)
            log.warning("#     connection_data : %s" % self._port_name)
            raise Exception("sss_session_open failed. status: %s" % status_to_str(status))

        log.debug("sss_session_open %s", status_to_str(status))
        return status

    def session_close(self):
        """
        Session close
        :return: None
        """
        if self.session_ctx:
            log.debug("Closing port")
            apis.sss_session_close(ctypes.byref(self.session_ctx))
            self.session_ctx = None

        if self._host_keystore:
            self._host_keystore.free()
            self._host_keystore = None

        if self._host_session:
            self._host_session.session_close()
            self._host_session = None

    def setup_counter_part_session(self):
        """
        Open Host Crypto Session. Either MbedTLS or Openssl depending on sssapisw library.
        :return: Status
        """
        from . import keystore  # pylint: disable=import-outside-toplevel
        self._host_session = Session(from_pickle=False)
        status = self._host_session.open_host_session(apis.kType_SSS_mbedTLS)
        if status != apis.kStatus_SSS_Success:
            # Retry with OpenSSL
            status = self._host_session.open_host_session(apis.kType_SSS_OpenSSL)
        if status != apis.kStatus_SSS_Success:
            log.error("Failed to openHost Session")
            return status

        self._host_keystore = keystore.KeyStore(self._host_session)
        return status

    def _se05x_prepare_host_applet_scp03_keys(self):
        """
        Set keys using host for Applet SCP03 session
        :return:
        """

        static_ctx = apis.NXSCP03_StaticCtx_t()
        dynamic_ctx = apis.NXSCP03_DynCtx_t()

        self._host_crypto_alloc_setkeys(currentframe().f_lineno, static_ctx.Enc,
                                        apis.kSSS_CipherType_AES,
                                        authkey.SE050_AUTHID_AESKEY_VALUE)

        self._host_crypto_alloc_setkeys(currentframe().f_lineno, static_ctx.Mac,
                                        apis.kSSS_CipherType_AES,
                                        authkey.SE050_AUTHID_AESKEY_VALUE)

        self._host_crypto_alloc_setkeys(currentframe().f_lineno, static_ctx.Dek,
                                        apis.kSSS_CipherType_AES,
                                        authkey.SE050_AUTHID_AESKEY_VALUE)

        self._alloc_applet_scp03_key_to_se05x_authctx(currentframe().f_lineno,
                                                      dynamic_ctx.Enc)

        self._alloc_applet_scp03_key_to_se05x_authctx(currentframe().f_lineno,
                                                      dynamic_ctx.Mac)

        self._alloc_applet_scp03_key_to_se05x_authctx(currentframe().f_lineno,
                                                      dynamic_ctx.Rmac)

        self._connect_ctx.auth.ctx.scp03.pStatic_ctx = ctypes.pointer(static_ctx)
        self._connect_ctx.auth.ctx.scp03.pDyn_ctx = ctypes.pointer(dynamic_ctx)

    def _alloc_applet_scp03_key_to_se05x_authctx(self, key_id, key_obj):
        """
        Perform key object init and key object allocate handle using host session.
        :param key_id: Key index
        :param key_obj: key object
        :return: Status
        """

        status = apis.sss_key_object_init(ctypes.byref(
            key_obj), ctypes.byref(self._host_keystore.keystore))

        if status != apis.kStatus_SSS_Success:
            raise Exception("Prepare Host sss_key_object_init %s" % status_to_str(status))

        status = apis.sss_key_object_allocate_handle(
            ctypes.byref(key_obj), key_id, apis.kSSS_KeyPart_Default,
            apis.kSSS_CipherType_AES, 16, apis.kKeyObject_Mode_Persistent)
        if status != apis.kStatus_SSS_Success:
            raise Exception("Prepare Host sss_key_object_allocate_handle %s" %
                            status_to_str(status))

    def _se05x_prepare_host_platformscp(self):
        """
        Prepare host for Platform SCP session
        :return: Status
        """
        static_ctx = apis.NXSCP03_StaticCtx_t()
        dynamic_ctx = apis.NXSCP03_DynCtx_t()

        static_ctx.keyVerNo = authkey.SE050_OEF_A1F4_KEY_VERSION_NO

        if self._connection_method == apis.kType_SE_Conn_Type_JRCP_V2:
            key_enc = authkey.SSS_AUTH_SE05X_KEY_ENC_JRCPV2
            key_mac = authkey.SSS_AUTH_SE05X_KEY_MAC_JRCPV2
            key_dek = authkey.SSS_AUTH_SE05X_KEY_DEK_JRCPV2
        else:
            key_enc = authkey.SSS_AUTH_SE05X_KEY_ENC
            key_mac = authkey.SSS_AUTH_SE05X_KEY_MAC
            key_dek = authkey.SSS_AUTH_SE05X_KEY_DEK

        self._host_crypto_alloc_setkeys(currentframe().f_lineno,
                                        static_ctx.Enc,
                                        apis.kSSS_CipherType_AES, key_enc)
        self._host_crypto_alloc_setkeys(currentframe().f_lineno,
                                        static_ctx.Mac,
                                        apis.kSSS_CipherType_AES, key_mac)
        self._host_crypto_alloc_setkeys(currentframe().f_lineno,
                                        static_ctx.Dek,
                                        apis.kSSS_CipherType_AES, key_dek)
        self._alloc_applet_scp03_key_to_se05x_authctx(currentframe().f_lineno,
                                                      dynamic_ctx.Enc)

        self._alloc_applet_scp03_key_to_se05x_authctx(currentframe().f_lineno,
                                                      dynamic_ctx.Mac)

        self._alloc_applet_scp03_key_to_se05x_authctx(currentframe().f_lineno,
                                                      dynamic_ctx.Rmac)

        self._connect_ctx.auth.ctx.scp03.pStatic_ctx = ctypes.pointer(static_ctx)
        self._connect_ctx.auth.ctx.scp03.pDyn_ctx = ctypes.pointer(dynamic_ctx)

    def _se05x_prepare_host_eckey(self):
        """
        Set keys using host for Fast SCP session
        :return: Status
        """

        static_ctx = apis.NXECKey03_StaticCtx_t()
        dynamic_ctx = apis.NXSCP03_DynCtx_t()

        # Init allocate Host ECDSA Key pair
        status = self._alloc_eckey_key_to_se05x_authctx(static_ctx.HostEcdsaObj,
                                                        currentframe().f_lineno,
                                                        apis.kSSS_KeyPart_Pair)
        if status != apis.kStatus_SSS_Success:
            log.error("_alloc_eckey_key_to_se05x_authctx %s", status_to_str(status))
            return status

        # Set Host ECDSA Key pair
        status = apis.sss_key_store_set_key(
            ctypes.byref(self._host_keystore.keystore),
            ctypes.byref(static_ctx.HostEcdsaObj),
            ctypes.byref((ctypes.c_ubyte * len(authkey.SSS_AUTH_SE05X_KEY_HOST_ECDSA_KEY))
                         (*authkey.SSS_AUTH_SE05X_KEY_HOST_ECDSA_KEY)),
            len(authkey.SSS_AUTH_SE05X_KEY_HOST_ECDSA_KEY),
            len(authkey.SSS_AUTH_SE05X_KEY_HOST_ECDSA_KEY) * 8, 0, 0)
        if status != apis.kStatus_SSS_Success:
            log.error("sss_key_store_set_key %s", status_to_str(status))
            return status

        # Init allocate Host ECKA Key pair
        status = self._alloc_eckey_key_to_se05x_authctx(static_ctx.HostEcKeypair,
                                                        currentframe().f_lineno,
                                                        apis.kSSS_KeyPart_Pair)
        if status != apis.kStatus_SSS_Success:
            log.error("_alloc_eckey_key_to_se05x_authctx %s", status_to_str(status))
            return status

        # Generate Host EC Key pair
        status = apis.sss_key_store_generate_key(
            ctypes.byref(self._host_keystore.keystore),
            ctypes.byref(static_ctx.HostEcKeypair), 256, None)

        if status != apis.kStatus_SSS_Success:
            log.error("_alloc_eckey_key_to_se05x_authctx %s", status_to_str(status))
            return status

        # Init allocate SE ECKA Public Key
        status = self._alloc_eckey_key_to_se05x_authctx(static_ctx.SeEcPubKey,
                                                        currentframe().f_lineno,
                                                        apis.kSSS_KeyPart_Public)
        if status != apis.kStatus_SSS_Success:
            log.error("_alloc_eckey_key_to_se05x_authctx %s", status_to_str(status))
            return status

        # Init Allocate Master Secret
        self._alloc_applet_scp03_key_to_se05x_authctx(currentframe().f_lineno,
                                                      static_ctx.masterSec)

        # Init Allocate ENC Session Key
        self._alloc_applet_scp03_key_to_se05x_authctx(currentframe().f_lineno,
                                                      dynamic_ctx.Enc)

        # Init Allocate MAC Session Key
        self._alloc_applet_scp03_key_to_se05x_authctx(currentframe().f_lineno,
                                                      dynamic_ctx.Mac)

        # Init Allocate DEK Session Key
        self._alloc_applet_scp03_key_to_se05x_authctx(currentframe().f_lineno,
                                                      dynamic_ctx.Rmac)

        self._connect_ctx.auth.ctx.eckey.pStatic_ctx = ctypes.pointer(static_ctx)
        self._connect_ctx.auth.ctx.eckey.pDyn_ctx = ctypes.pointer(dynamic_ctx)
        return status

    def _alloc_eckey_key_to_se05x_authctx(self, key_obj, key_id, key_type):
        """
        Key object initialization and allocate handle for fast SCP session
        :param key_obj: Key Object
        :param key_id: Key index
        :param key_type: key type
        :return: Status
        """

        status = apis.sss_key_object_init(ctypes.byref(
            key_obj), ctypes.byref(self._host_keystore.keystore))

        if status != apis.kStatus_SSS_Success:
            log.error("sss_key_object_init %s", status_to_str(status))
            return status

        status = apis.sss_key_object_allocate_handle(ctypes.byref(key_obj), key_id, key_type,
                                                     apis.kSSS_CipherType_EC_NIST_P, 256,
                                                     apis.kKeyObject_Mode_Persistent)
        if status != apis.kStatus_SSS_Success:
            log.error("sss_key_object_allocate_handle %s", status_to_str(status))

        return status

    def _host_crypto_alloc_setkeys(self, key_id, key_obj, cypher_type, key_value):
        """
        Key object initialization, allocate handle and Set key using host
        :param key_id: Key Index
        :param key_obj: Key object
        :param cypher_type: Cypher type
        :param key_value: Key value
        :return: None
        """

        status = apis.sss_key_object_init(ctypes.byref(
            key_obj), ctypes.byref(self._host_keystore.keystore))

        if status != apis.kStatus_SSS_Success:
            raise Exception("Prepare Host sss_key_object_init %s" % status_to_str(status))

        status = apis.sss_key_object_allocate_handle(
            ctypes.byref(key_obj), key_id, apis.kSSS_KeyPart_Default,
            cypher_type, len(key_value), apis.kKeyObject_Mode_Persistent)
        if status != apis.kStatus_SSS_Success:
            raise Exception("sss_key_object_allocate_handle %s" % status_to_str(status))

        status = apis.sss_key_store_set_key(
            ctypes.byref(self._host_keystore.keystore),
            ctypes.byref(key_obj),
            ctypes.byref((ctypes.c_ubyte * len(key_value))(*key_value)),
            len(key_value), len(key_value) * 8, 0, 0)
        if status != apis.kStatus_SSS_Success:
            raise Exception("sss_key_store_set_key %s" % status_to_str(status))

    def refresh_session(self):
        """
        Refresh session
        :return: Status
        """
        status = apis.kStatus_SSS_Fail
        if self.session_policy is not None:
            status = apis.sss_se05x_refresh_session(ctypes.byref(self.session_ctx),
                                                    ctypes.byref(self.session_policy))
        return status
