# Copyright 2019,2020 NXP
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

from . import cli
from . import cli_get
from . import cli_set
from . import cli_generate
from . import cli_cloud
from . import cli_se05x
from . import cli_a71ch
from . import cli_refpem

logging.basicConfig(level=logging.INFO)

# This is required to support click on imx
try:
    ENV_LANG = os.environ['LANG']
    if ENV_LANG is None:
        os.environ['LC_ALL'] = "en_US.utf-8"
        os.environ['LANG'] = "en_US.utf-8"
except Exception as exc:  # pylint: disable=broad-except
    os.environ['LC_ALL'] = "en_US.utf-8"
    os.environ['LANG'] = "en_US.utf-8"
