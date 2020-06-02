..
    Copyright 2020 NXP

    This software is owned or controlled by NXP and may only be used
    strictly in accordance with the applicable license terms.  By expressly
    accepting such terms or by downloading, installing, activating and/or
    otherwise using the software, you are agreeing that you have read, and
    that you agree to comply with and are bound by, such license terms.  If
    you do not agree to be bound by the applicable license terms, then you
    may not retain, install, activate or otherwise use the software.

.. _import-external-obj-create:

=================================================
 Import External Object Create
=================================================

In this example, we send a raw APDU to SE05x which will
import the external object wrapped with ECKey context in the APDU.

.. note:: This APDU must be prepared by ``se05x_ImportExternalObjectPrepare``
  executable.


Pre-requisites
=================================================

Raw APDU should be prepared by ``se05x_ImportExternalObjectPrepare``.
Refer to :numref:`import-external-obj-prepare` :ref:`import-external-obj-prepare`.


Building
=================================================

Build the project with the following configurations.

**se05x_ImportExternalObjectCreate**

- ``Project = se05x_ImportExternalObjectCreate``
- ``SCP=SCP03_SSS``
- ``SE05x_Auth=None`` or ``SE05x_Auth=PlatfSCP03``


How to use
=================================================


Run executable **se05x_ImportExternalObjectCreate** as ::

    se05x_ImportExternalObjectCreate.exe -file rawAPDU.der <portName>

where,

- *file* is the input file containing raw APDU to be sent. Same as the output file from ``se05x_ImportExternalObjectPrepare``.
- *portName* is the name of the port over which to connect (COMPORT in case running over VCOM)

.. note:: ``se05x_ImportExternalObjectCreate`` example will fail if the object already exists.
