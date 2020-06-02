..
    Copyright 2020 NXP

    This software is owned or controlled by NXP and may only be used
    strictly in accordance with the applicable license terms.  By expressly
    accepting such terms or by downloading, installing, activating and/or
    otherwise using the software, you are agreeing that you have read, and
    that you agree to comply with and are bound by, such license terms.  If
    you do not agree to be bound by the applicable license terms, then you
    may not retain, install, activate or otherwise use the software.



.. _mals-demo-cli-app:

MALS CLI APP
=================================

This example, is more advanced than :numref:`ex-mals-agent-demo` :ref:`ex-mals-agent-demo`.

This example is takes command line parameters to talk to
the MALS Applet and

.. warning::

    This application is alpha status and would keep evolving/changing.

CLI Parameters
-----------------------------------


- ``--loadpkg`` <path-to-Mals-applet-package-binary-file>

Load a binary file.

- ``--getuid``

Get UID

- ``--getappcontents`` <File-Name-to-store-card-contents> [optional-app-aid]

Store contents of card to ``File-Name-to-store-card-contents``

e.g. ``--getappcontents out_contents_file.bin 01030011``

- ``--getpkgcontents`` <File-Name-to-store-card-contents> [optional-pkg-aid]

- ``--malsgetversion``

Print version of the MALS Applet

- ``--getsignature`` <File-Name-to-store-signature>

Get signature of last script

- ``--checkTear``

Check if tearing has happened

- ``--checkUpgradeProgress``

Check upgrade progress

- ``--testapplet`` <applet-aid> <apdu-command>

Used to select test applet and send dummy APDU Command

Usage - K64F
--------------------------------

- Ensure VCOM interface is setup for K64.

- SET EX_SSS_BOOT_SSS_PORT=<COM Port>

- Run application with applicable commandline parameters



Usage - iMX Linux
--------------------------------

- Run application with applicable commandline parameters


