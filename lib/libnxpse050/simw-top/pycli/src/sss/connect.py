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
import logging
import pickle
from . import util
from . import sss_api as apis
log = logging.getLogger(__name__)


def do_open_session(subsystem, connection_method, port_name, host_session=False,
                    auth_type=apis.kSE05x_AuthType_None):
    """
    Stores session open parameter in pkl file format
    :param subsystem: SE050, A71CH, A71CL, MBEDTLS, OPENSSL
    :param connection_method: vcom, t1oi2c, sci2c, jrcpv1, jrcpv2, pcsc
    :param port_name: connection data. eg: COM6, 192.168.1.1:8050, None
    :param host_session: session is host session or not
    :param auth_type: authentication type for session open. eg: None, userid, aeskey,
    eckey, platformscp
    :return: None
    """
    session_params = {
        'pkl_v_major': 1,
        'pkl_v_minor': 0,
        'subsystem': subsystem,
        'connection_method': connection_method,
        'port_name': port_name,
        'auth_type': auth_type}

    # Keep Host session parameters separate to support parallel session with se050
    if host_session:
        if not os.path.isfile(util.get_host_session_pkl_path()):
            pkl_session = open(util.get_host_session_pkl_path(), 'wb+')
            pickle.dump(session_params, pkl_session)
            pkl_session.close()
        else:
            log.warning('Session already open, close current session first')
    else:
        if not os.path.isfile(util.get_session_pkl_path()):
            pkl_session = open(util.get_session_pkl_path(), 'wb+')
            pickle.dump(session_params, pkl_session)
            pkl_session.close()
        else:
            log.warning('Session already open, close current session first')


def do_close_session():
    """
    Erase session open parameters
    :return: None
    """
    pkl = util.get_session_pkl_path()
    host_pkl = util.get_host_session_pkl_path()
    if os.path.isfile(host_pkl):
        os.remove(host_pkl)
    if os.path.isfile(pkl):
        os.remove(pkl)
    else:
        log.warning('No open session. Nothing to do')
