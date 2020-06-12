..
    Copyright 2020 NXP

    This software is owned or controlled by NXP and may only be used
    strictly in accordance with the applicable license terms.  By expressly
    accepting such terms or by downloading, installing, activating and/or
    otherwise using the software, you are agreeing that you have read, and
    that you agree to comply with and are bound by, such license terms.  If
    you do not agree to be bound by the applicable license terms, then you
    may not retain, install, activate or otherwise use the software.

.. _psa-example:

=================================================
 PSA Non Secure Example
=================================================

This example is to demonstrate how to use PSA library APIs
to perform a Sign/Verify operations.


Pre-requisites
=================================================

You need to build PSA-ALT library for TrustZone before compiling this
application code.

Refer to :numref:`psa-alt` :ref:`psa-alt`.


Assigning PSA APIs
=================================================

Assign function pointers for the secure world PSA APIs that you want
to support.

.. literalinclude:: psa_nonsecure.c
   :language: c
   :dedent: 4
   :start-after: /* doc:start:psa-assign-fp */
   :end-before: /* doc:end:psa-assign-fp */

Generate an asymmetric key.

.. literalinclude:: psa_nonsecure.c
   :language: c
   :dedent: 4
   :start-after: /* doc:start:psa-generate-key */
   :end-before: /* doc:end:psa-generate-key */

Perform Sign-Verify operation with the key.

.. literalinclude:: psa_nonsecure.c
   :language: c
   :dedent: 4
   :start-after: /* doc:start:psa-sign-verify */
   :end-before: /* doc:end:psa-sign-verify */


Building Example
=================================================

This example running in normal world should link with the secure world library (PSA) so that
definition for veneer table APIs can be found. Build the normal world example with
the following CMake configurations:

- ``Host=lpcxpresso55s_ns``

- ``HostCrypto=MBEDCRYPTO``

- ``RTOS=Default``

- ``SMCOM=T1oI2C``

- ``PROJECT=psa_nonsecure``
