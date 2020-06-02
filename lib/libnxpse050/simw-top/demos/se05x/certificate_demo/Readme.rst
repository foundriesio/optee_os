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

.. _certificate-demo:

==========================================================
 Building a self-signed certificate
==========================================================

This demo is to demonstrate how we can use provisioned keys to create a self-signed certificate to communicate with cloud platforms. In this example, we use two binaries, one to generate a keypair inside the secure element and another to use the generated keypair to create a self-signed certificate.

.. note ::
    We use OpenSSL in this example to create the certificate.

How to use
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. Run the binary ``generate_certificate_key`` to generate an ECC-256 keypair inside the secure element.
#. Run the binary ``generate_certificate`` to create a self-signed certificate. This demo provisions the the generated certificate into the secure element.
#. You can read-out the certificate using SSS-APIs from keyId ``CERTIFICATE_KEY_ID + 1`` as defined in ``certificate.h`` file.
