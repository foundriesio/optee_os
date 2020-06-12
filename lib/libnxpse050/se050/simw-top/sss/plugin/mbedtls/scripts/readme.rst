..
    Copyright 2019 NXP

    This software is owned or controlled by NXP and may only be used
    strictly in accordance with the applicable license terms.  By expressly
    accepting such terms or by downloading, installing, activating and/or
    otherwise using the software, you are agreeing that you have read, and
    that you agree to comply with and are bound by, such license terms.  If
    you do not agree to be bound by the applicable license terms, then you
    may not retain, install, activate or otherwise use the software.


.. _mbedTLS-alt:

Introduction on mbedTLS ALT Implementation
============================================

MbedTLS ALT implementation allows mbedTLS stack use the secure element
access using SSS layer. Crypto operations performed during TLS handshake
between client and server are performed using the secure element.


Using mbedTLS ALT
----------------------

For reference, let's look at the :file:`sss/ex/mbedtls/ex_sss_ssl2.c`.
The important sections of the file are.

Here we initialize the keys and relevent objects.

.. literalinclude:: ../../../ex/mbedtls/ex_sss_ssl2.c
    :language: c
    :dedent: 8
    :start-after: /* doc+:initialize-key-objs */
    :end-before: /* doc-:initialize-key-objs */

Here, we tell mbedTLS to use the public key from the SE.

.. literalinclude:: ../../../ex/mbedtls/ex_sss_ssl2.c
    :language: c
    :dedent: 8
    :start-after: /* doc+:use-public-key-from-se */
    :end-before: /* doc-:use-public-key-from-se */

Here, get certificate in DER format from the SE, and then convert it to PEM and share it with the mbedTLS stack.

.. literalinclude:: ../../../ex/mbedtls/ex_sss_ssl2.c
    :language: c
    :dedent: 12
    :start-after: /* doc+:load-certificate-from-se */
    :end-before: /* doc-:load-certificate-from-se */

Here, we tell mbedTLS to use the public key from the SE, generally for signing any contents.

.. literalinclude:: ../../../ex/mbedtls/ex_sss_ssl2.c
    :language: c
    :dedent: 8
    :start-after: /* doc+:set-handle-to-use-private-key-from-se */
    :end-before: /* doc-:set-handle-to-use-private-key-from-se */

Here, we tell mbedTLS to use the public key from the SE for ECDH handshake.

.. literalinclude:: ../../../ex/mbedtls/ex_sss_ssl2.c
    :language: c
    :dedent: 12
    :start-after: /* doc+:use-private-key-for-ecdh */
    :end-before: /* doc-:use-private-key-for-ecdh */



Testing
-------

Building mbedTLS SSL/DTLS server for testing
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Build mbedTLS server using the VS solution:
CMake configurations:
- ``RTOS_Default``: ON
- ``WithHostCrypto_MBEDTLS``: ON
- ``WithmbedTLS_ALT_SSS``: ON

- Project: ``mbedtls_ex_orig_ssl_server2`` / ``ex_mbedtls_origin_dtls_server``

Building mbedTLS SSL/DTLS client (with SSS-APIs integration)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Build mbedTLS client using the VS solution:
CMake configurations:
- ``RTOS_Default``: ON
- ``WithHostCrypto_MBEDTLS``: ON
- ``WithmbedTLS_ALT_SSS``: ON

- Project: ``mbedtls_ex_sss_ssl2`` / ``mbedtls_ex_sss_dtls``

Testings mbedTLS ALT
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Directory ``simw-top\sss\plugin\mbedtls\scripts`` contains test scripts for
starting mbedTLS server and client applications with different cipher suites.
Before executing some test scripts, the secure element must first be
provisioned.

1)  Complete :numref:`cli-doc-pre-steps` :ref:`cli-doc-pre-steps`


#)  Provision secure element using batch scripts in directory
    ``simw-top\sss\plugin\mbedtls\scripts``.
    Run the following commands in virtual environment:

    To configure secure element for ECC
        ``windowsProvisionEC.bat <ec_curve> jrcpv2/vcom 127.0.0.1:8050/COM#``

    To configure secure element for RSA
        ``windowsProvisionRSA.bat <rsa_type> jrcpv2/vcom 127.0.0.1:8050/COM#``

    To see usage, run without any parameters
        ``windowsProvisionRSA.bat`` or ``windowsProvisionEC.bat``

    .. note::
        Once provisioning is done the virtual environment is not needed anymore.

#)  Starting mbedTLS SSL client and server applications::

        start_ssl2_server.bat <ec_curve>/<rsa_type>
        start_ssl2_client.bat <ec_curve>/<rsa_type> <cipher suite> 127.0.0.1:8050/COM#

#)  Starting mbedTLS DTLS client and server applications::

        start_dtls_server.bat <ec_curve>/<rsa_type>
        start_dtls_client.bat <ec_curve>/<rsa_type> <cipher suite> 127.0.0.1:8050/COM#

    .. note::

        Ensure that ``ec_curve``/``rsa_type`` used in server and client
        applications is the same as used while provisioning the SE in step 2.

mbedTLS ALT APIs
----------------------

.. doxygengroup:: ax_mbed_tls
    :no-link:
    :members:
    :protected-members:
    :private-members:
    :undoc-members:

