# Copyright 2018,2019 NXP
#
# This software is owned or controlled by NXP and may only be used
# strictly in accordance with the applicable license terms.  By expressly
# accepting such terms or by downloading, installing, activating and/or
# otherwise using the software, you are agreeing that you have read, and
# that you agree to comply with and are bound by, such license terms.  If
# you do not agree to be bound by the applicable license terms, then you
# may not retain, install, activate or otherwise use the software.
#

from setuptools import setup
import sys
import os
try:
    import py2exe
except ImportError:
    pass

sys.path.append(os.path.abspath(os.path.dirname(__file__) + os.sep + "sss"))

try:
    import cli
    import sss
except ImportError:
    pass

try:
    import plugandtrust_ver as plugandtrust_ver
except ImportError:
    class plugandtrust_ver(object):
        PLUGANDTRUST_MAJOR=2
        PLUGANDTRUST_MINOR=14
        PLUGANDTRUST_DEV=0

tools_dir = os.path.abspath(os.path.dirname(__file__) \
    + os.sep + ".." \
    + os.sep + ".." \
    + os.sep + "tools")

if tools_dir == r"C:\tools":
    data_files = []
else:
    data_files = [(tools_dir, ["libsssapisw.dll",
            "libsssapisw.dylib"]),]

setup(
    name='ssscli',
    version='%d.%d.%d'%(
        int(plugandtrust_ver.PLUGANDTRUST_MAJOR),
        int(plugandtrust_ver.PLUGANDTRUST_MINOR),
        int(plugandtrust_ver.PLUGANDTRUST_DEV)),
    py_modules=['ssscli'],
    options={
        'py2exe': {
            'packages': [
                'cffi',
                'click',
                'cryptography',
                'func_timeout',
            ],
        }
    },

    install_requires=[
        'cffi',
        'click',
        'cryptography',
    ],
    entry_points='''
        [console_scripts]
        ssscli=cli.cli:cli
    ''',
    console=[
        'pySSSCLI.py',
        'Provision/Provision_GCP.py',
        'Provision/Provision_IBM.py',
        'Provision/Provision_AZURE.py',
        'Provision/Provision_AWS.py',
        ],
    data_files=data_files,
)
