..
    Copyright 2019,2020 NXP

    This software is owned or controlled by NXP and may only be used
    strictly in accordance with the applicable license terms.  By expressly
    accepting such terms or by downloading, installing, activating and/or
    otherwise using the software, you are agreeing that you have read, and
    that you agree to comply with and are bound by, such license terms.  If
    you do not agree to be bound by the applicable license terms, then you
    may not retain, install, activate or otherwise use the software.


.. highlight:: bat

.. _ex-se05x-mandate-scp:

=======================================================================
 SE05X Mandate SCP example
=======================================================================

This project demonstrates how to configure SE05X to mandate platform SCP. This
can be used if you always want the communication with SE05X to be encrypted.

.. note:: After this example runs successfully, further communication would require
  Platform SCP03 encryption.


Prerequisites
=====================
- Micro USB cable
- Kinetis FRDM-K64F/imx-RT1050 board
- Personal Computer
- SE05x Arduino shield


Building the Demo
=======================================================================

- Build Plug & Trust middleware stack. (Refer :ref:`building`)
- Project: ``se05x_MandatePlatformSCP``

  - CMake configuration

    ``SE05X_Auth``:``None``


Running the Example
=======================================================================

If you have built a binary, flash the binary on to the board and reset
the board.

If you have built an *exe* to be run from Windows using VCOM, run as::

    se05x_MandatePlatformSCP.exe <PORT NAME>

Where **<PORT NAME>** is the VCOM COM port.
