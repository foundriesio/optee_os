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

.. _se-get-certificate:

==========================================================
  Get Certificate from the SE
==========================================================

This tool is to retrieve Trust provisioned certificates from the SE. It will
read the certificate and store it on the file system. It takes as argument,
the keyID at which certificate is stored and the file path where to save the
certificate on the file system.

.. note:: It can only be compiled when Host Crypto is mbedTLS.


Building the example
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Use the following CMake configurations to compile the example

- CMake configurations: ``WithHostCrypto_MBEDTLS``: ON

- Project: ``se05x_Get_Certificate``

How to use
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Run the tool as::

    se05x_Get_Certificate <32-bit keyID> </path/to/file> <port>


- The keys ID has to be in HEX format.

- For systems connecting with T=1 over I2C, the port parameter can be skipped.

- The certificate will be stored in *PEM* format at the specified path on the
  file system.
