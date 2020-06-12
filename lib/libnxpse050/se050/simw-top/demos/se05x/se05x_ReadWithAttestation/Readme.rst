..
    Copyright 2020 NXP

    This software is owned or controlled by NXP and may only be used
    strictly in accordance with the applicable license terms.  By expressly
    accepting such terms or by downloading, installing, activating and/or
    otherwise using the software, you are agreeing that you have read, and
    that you agree to comply with and are bound by, such license terms.  If
    you do not agree to be bound by the applicable license terms, then you
    may not retain, install, activate or otherwise use the software.

.. highlight::shell

.. _read-with-attestation:

=================================================
 Read object with Attestation
=================================================

This example demonstrates how to read an object with
attestation and parse the attested data to check
various object attributes.

In this example, we use an EC NIST-P 256 keypair as the
attestation key and a binary object which will be attested.

.. note:: The maximum size of a binary object that can be attested
  at a time is 500 bytes. The API available will only work on
  binary objects with size up to 500 bytes. To perform attestation
  on an object of greater size, we need to call corresponding
  ``Se05x`` API in a loop, verifying the obtained signature every time.

  A reference implementation is available at :ref:`read-large-bin-obj`

Building
=================================================

Build the project with the following configurations.

**se05x_ReadWithAttestation**

- ``Project = se05x_ReadWithAttestation``


Running
=================================================

On running the example, you would be able to see object attributes
logged on the screen like::

  App   :INFO :Running example se05x_ReadWithAttestation
  App   :INFO :Type:
  App   :INFO :   BINARY_FILE
  App   :INFO :Auth:
  App   :INFO :   Not Set
  App   :INFO :Neg auth count:
  App   :INFO :   0x0000
  App   :INFO :Owner:
  App   :INFO :   0x0000
  App   :INFO :Neg auth count max:
  App   :INFO :   0x0000
  App   :INFO :Auth Object:
  App   :INFO :   No authentication required
  App   :INFO :Policies:
  App   :INFO :   POLICY_OBJ_ALLOW_READ
  App   :INFO :   POLICY_OBJ_ALLOW_WRITE
  App   :INFO :   POLICY_OBJ_ALLOW_DELETE
  App   :INFO :Origin:
  App   :INFO :   EXTERNAL
  App   :INFO :Example success
  App   :INFO :ex_sss Finished

You can see the various attributes associated with the object such
as object type, authentication mechanism, origin and policies.

An example of how to perform read with attestation is given below

.. literalinclude:: se05x_ReadWithAttestation.c
   :language: c
   :start-after: /* doc:start:read-w-attestation */
   :end-before: /* doc:end:read-w-attestation */
   :dedent: 4

The data received in ``att_data`` variable can be parsed to
read the object attributes.


.. _read-large-bin-obj:

Reading large binary objects with attestation
=================================================

Following is an example code on how to read a large binary file
with attestation.

.. note:: This is required only when reading binary objects of size larger
  than 500 bytes. For any other case, you should use SSS API as above

.. literalinclude:: se05x_ReadWithAttestation.c
   :language: c
   :start-after: /* doc:start:read-large-binary-obj-w-attestation */
   :end-before: /* doc:end:read-large-binary-obj-w-attestation */
