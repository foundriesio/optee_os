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

.. _ex-sss-symmetric:

=======================================================================
 Symmetric AES Example
=======================================================================

This project demonstrates symmetric cryptography - AES encryption and decryption operations.

Prerequisites
=======================================================================

- Build Plug & Trust middleware stack. (Refer :ref:`building`)


About the Example
=======================================================================
This example does a symmetric cryptography AES encryption and decryption operation.

It uses the following APIs and data types:
  - :cpp:func:`sss_symmetric_context_init()`
  - :cpp:enumerator:`kAlgorithm_SSS_AES_CBC` from :cpp:type:`sss_algorithm_t`
  - :cpp:enumerator:`kSSS_CipherType_AES` from :cpp:type:`sss_cipher_type_t`
  - :cpp:enumerator:`kMode_SSS_Encrypt` from :cpp:type:`sss_mode_t`
  - :cpp:func:`sss_cipher_one_go()`
  - :cpp:enumerator:`kMode_SSS_Decrypt` from :cpp:type:`sss_mode_t`



Console output
=======================================================================


If everything is successful, the output will be similar to:

.. literalinclude:: out_ex_symmetric.rst.txt
   :start-after: sss   :WARN :!!!Not recommended for production use.!!!


