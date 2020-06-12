..
    Copyright 2020 NXP

    This software is owned or controlled by NXP and may only be used
    strictly in accordance with the applicable license terms.  By expressly
    accepting such terms or by downloading, installing, activating and/or
    otherwise using the software, you are agreeing that you have read, and
    that you agree to comply with and are bound by, such license terms.  If
    you do not agree to be bound by the applicable license terms, then you
    may not retain, install, activate or otherwise use the software.

.. _import-external-obj-prepare:

=================================================
 Import External Object Prepare
=================================================

Import External Object command allows the user to import
an external object wrapped with a secure ``ECKey_Auth``
context. A session is not required to execute this command,
the ``ECKey_Auth`` parameters are provided with the wrapped
WriteSecureObject command. The applet will use ``ECKey_Auth``
parameters and derive session keys to unwrap the command
and execute it. ``ImportExternalObject`` command works in its
own session. It will open an ``ECKey`` session, write the secure
object and close the session.



In this example, we prepare a complete raw APDU to be sent
to SE05x. A ``WriteSecureObject`` command needs to be prepared
which will be wrapped and sent as a part of ``ImportExternalObject``
command. For an example we are preparing ``WriteSymmKey`` command as :


.. literalinclude:: se05x_ImportExternalObjectPrepare.c
   :dedent: 4
   :language: c
   :start-after: /* doc:start:writesecureobjbuf */
   :end-before: /* doc:end:writesecureobjbuf */


.. literalinclude:: se05x_ImportExternalObjectPrepare.c
   :language: c
   :start-after: /* doc:start:createapdubuf */
   :end-before: /* doc:end:createapdubuf */


You can call any of the ``WriteSecureObject`` API with your data
and create the buffer.


Building
=================================================

Build the project with the following configurations.

**se05x_ImportExternalObjectPrepare**

- ``Project = se05x_ImportExternalObjectPrepare``
- ``SCP=SCP03_SSS``
- ``SE05x_Auth=ECKey`` or ``SE05x_Auth=ECKey_PlatfSCP03``


How to use
=================================================

Generate the raw APDU file by running the executable. Run
**se05x_ImportExternalObjectPrepare** as ::

    se05x_ImportExternalObjectPrepare.exe -keyid 0x7DA00003 -file eckey_ecdsa.der -out rawAPDU.der <portName>

where,

- *keyid* is the authentication keyId at which ECDSA public key is stored.
- *file* is the input ECDSA keypair file (in binary format)
- *out* is the output file where the raw APDU will be stored.
- *portName* is the name of the port over which to connect (COMPORT in case running over VCOM)


After this executes successfully, you need to send the generated raw APDU
to SE05x. Refer to :numref:`import-external-obj-create` :ref:`import-external-obj-create`
