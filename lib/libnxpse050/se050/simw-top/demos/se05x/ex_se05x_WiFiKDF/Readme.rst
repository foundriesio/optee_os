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

.. _ex-se05x-wifi-kdf:

=======================================================================
 SE05X WiFi KDF Example
=======================================================================

This project is to demonstrate Password based KDF (PBKDF) operation using
SE05X. This operation is used in deriving Pre-Shared key (PSK) for WiFi ssid
using stored passwords.


Prerequisites
=====================
- Micro USB cable
- Kinetis FRDM-K64F/imx-RT1050 board
- Personal Computer
- SE05x Arduino shield


Building the Demo
=======================================================================

- Build Plug & Trust middleware stack. (Refer :ref:`building`)
- Project: ``ex_se05x_WiFiKDF_inject``
- Project: ``ex_se05x_WiFiKDF_derive``


Running the Example
=======================================================================

If you have built a binary, first flash the ``ex_se05x_WiFiKDF_inject``
binary on to the board and reset the board. Then flash ``ex_se05x_WiFiKDF_derive``
binary and reset the board.

If you have built an *exe* to be run from PC using VCOM, run as::

    ex_se05x_WiFiKDF_inject.exe <PORT NAME>
    ex_se05x_WiFiKDF_derive.exe -s <ssid_name> <PORT NAME>

Where **<PORT NAME>** is the VCOM COM port and **<ssid_name>** is the
name of SSID for which you want to derive the PSK.


Console output
=======================================================================

During injection, if everything is successful, the output will be similar to:

.. literalinclude:: out_ex_se05x_WiFiKDF_inject.rst.txt
   :start-after: sss   :WARN :!!!Not recommended for production use.!!!

While deriving the key, if everything is successful, the output will be similar to:

.. literalinclude:: out_ex_se05x_WiFiKDF_derive.rst.txt
   :start-after: sss   :WARN :!!!Not recommended for production use.!!!
