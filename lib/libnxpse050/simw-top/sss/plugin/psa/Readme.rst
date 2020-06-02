..
    Copyright 2020 NXP

    This software is owned or controlled by NXP and may only be used
    strictly in accordance with the applicable license terms.  By expressly
    accepting such terms or by downloading, installing, activating and/or
    otherwise using the software, you are agreeing that you have read, and
    that you agree to comply with and are bound by, such license terms.  If
    you do not agree to be bound by the applicable license terms, then you
    may not retain, install, activate or otherwise use the software.

.. _psa-alt:

=================================================
 Platform Security Architecture
=================================================

The Platform Security Architecture (PSA) by ARM is a holistic set
of threat models, security analyses, hardware and firmware
architecture specifications, and an open source firmware
reference implementation

ARMmbed provides an interface for PSA APIs as a part of its mbed-crypto
project. Also, it supports SE driver interface which allows the user to
use its own implementation for PSA cryptographic functions. This is useful
for integrating Secure Element with mbed-crypto provided PSA interface.

For details on PSA specification, refer to `ARMmbed PSA Specification`_.

PSA SE Driver Interface
=================================================

The SE Driver interface allows the user to register Secure Element drivers
for various cryptographic operations. It is not necessary that one driver
should offer all cryptographic functionalities, we can register up to 4 drivers
which may offer different functionalities.

Cryptographic APIs are split in to the following :

- SE_KEY_MANAGEMENT - Key management APIs like import/generate/destroy
- SE_MAC - Mac operations
- SE_CIPHER - Symmetric encrypt/decrypt operations
- SE_AEAD - AEAD/GCM operations
- SE_ASYMMETRIC - Asymmetric sign/verify/encrypt/decrypt operations
- SE_KEY_DERIVATION - Key derivation operations

The driver may support any subset of cryptographic functionalities, mbed-crypto
offers software fall-back for APIs unavailable from SE.


.. note:: For SE interface, currently only SE_KEY_MANAGEMENT APIs, and asymmetric sign and
    asymmetric verify APIs are supported.


PSA Concepts
=================================================

**Lifetime**
    The lifetime of an object refers to its persistence. An object can
    either be ``PERSISTENT`` or ``VOLATILE``. However, for SE based PSA
    implementation, it refers to the module ID of SE Driver. Lifetime
    identifies the scope of an object (where the object is stored and
    which operations are available for it). This is used while performing
    any operation on an object. We can use different lifetimes while
    performing operations with the same object as long as different
    drivers can access the object.

**Slot ID**
    This is a 64-bit ID indicating where the object is loaded in the library.
    PSA offers a maximum of 31 slots, which indicates that at a time, a
    maximum of 31 objects can be used for any operation. If an object is
    not being used, the object handle can be closed to free the slot. Since
    there is no concept of an object being loaded for SE, the Slot ID will
    refer to the Key ID of the object. Any application will remain unaware
    of Slot ID and this should be managed internally. PSA library should
    use this value to refer to any object.

    For SSS based PSA implementation, we have a 1:1 mapping of Slot ID from
    Key ID.

**Key ID**
    This is a 32-bit ID indicating the file name of the object which will be
    stored on the file system. Apart from the actual object, PSA also maintains
    an object file which will store details of the object such as policies and
    supported algorithms. Before performing any operation, these values are validated.
    The applications will use this value to refer to any object.

    For SSS based PSA implementation, the contents of this file are also stored
    on SE.

**PSA Objects**
    -   **LIFETIME_FILE** - This is SE specific file which can contain SE(driver) specific
        persistent data

    -   **TRANSACTION_FILE** - This is a temporary file created at the time of any operation.
        It will be deleted after the operation.

    -   **OBJECT_FILE** - This file corresponds to any object we create inside the SE. It
        will store data about policy, supported algorithms, etc.
        (keyID range is 0x20000000 - 0x2FFFFFFF)

    -   **OBJECT** - This is the actual object created inside the SE.
        (keyID range is 0x30000000 - 0x3FFFFFFF)

    For any provided Key ID, the most significant nibble is masked out. The effective Key ID is
    28-bit long.

    .. warning::
        This logic of managing KeyIDs of various PSA objects is temporary and
        it will be changed in the future.

.. _psa-alt-building:

Building PSA for TrustZone
=================================================

PSA library is intended to run in ARM TrustZone. All examples will run in normal
world and link to PSA library to perform cryptographic operations. Build the library
for TrustZone with the following CMake configurations:

- ``Host=lpcxpresso55s_s``

- ``HostCrypto=MBEDCRYPTO``

- ``RTOS=Default``

- ``SMCOM=T1oI2C``

- ``PROJECT=PSA_ALT``


.. _ARMmbed PSA Specification: https://armmbed.github.io/mbed-crypto/html/general.html