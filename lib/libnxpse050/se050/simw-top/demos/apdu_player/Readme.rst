..
    Copyright 2019 NXP

    This software is owned or controlled by NXP and may only be used
    strictly in accordance with the applicable license terms.  By expressly
    accepting such terms or by downloading, installing, activating and/or
    otherwise using the software, you are agreeing that you have read, and
    that you agree to comply with and are bound by, such license terms.  If
    you do not agree to be bound by the applicable license terms, then you
    may not retain, install, activate or otherwise use the software.

.. highlight:: shell

.. _apdu-player-demo:

==========================================================
 APDU Player Demo
==========================================================

This demo is to transceive raw APDUs to the SE. It takes a command line argument as the input file containing APDUs to be sent, followed by the reponse. An example of command to be in the file is
``/send 00A4040000 6F108408A000000151000000A5049F6501FF9000``


In this, the ``00A4040000`` is the command to be transmitted and ``6F108408A000000151000000A5049F6501FF9000`` is the expected response.

.. note ::
    Ensure that authentication with the SE is either plain or platform SCP
    This demo only supports only SE05x devices

How to use
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Run the demo as ``apdu_player_demo [Input-file] <Port>``
