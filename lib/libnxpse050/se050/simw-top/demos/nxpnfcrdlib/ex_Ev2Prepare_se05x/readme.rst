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

.. _ex_Ev2Prepare_se050:

=======================================================================
 MIFARE DESFire EV2 : Prepare SE050
=======================================================================

Prerequisites
=======================================================================

- Bring Up Hardware. (Refer :ref:`dev-platforms`)
- Connect RC663 to your micronctroller. (Refer :ref:`mifarekdf-rc663 <mifarekdf-rc663>`)


About the Example
=======================================================================
This is an example project for provisioning the SE050 for running other
SE050 MIFARE DESFire EV2 examples. This example Creates and sets 2 SE050 secure
objects for further use by other examples.

It uses the following APIs and data types:

- :cpp:func:`sss_key_store_set_key()`


The two AES key storage for NFC application n SE050
====================================================
We create two AES Keys in SE050

This example calls the - :cpp:func:`InitialSetupSe050()`:
which inits, allocates and sets 2 AES keys.

The keyIDs are as below

.. literalinclude:: ../common/intfs/se05x_MfDfInit.h
   :language: c
   :start-after: /* doc:start:mif-kdf-keyids */
   :end-before: /* doc:end:mif-kdf-keyids */

.. literalinclude:: ../../../sss/ex/inc/ex_sss_objid.h
   :language: c
   :start-after: /* doc:start:mif-kdf-start-keyid */
   :end-before: /* doc:end:mif-kdf-start-keyid */


Key values
^^^^^^^^^^^^^^
The key that is provisioned into SE050 is called the oldKey and newKey
and it takes the values as below.

.. literalinclude:: ../common/src/se05x_MfDfInit.c
   :language: c
   :start-after: /* doc:start:mif-key-values */
   :end-before: /* doc:end:mif-key-values */


Running the Demo
=======================================================================


#) Either press the reset button on your board or launch the debugger in your IDE to begin running the demo

If everything is setup correctly the output would be as follows::

    sss:INFO :atr (Len=35)
        00 A0 00 00     03 96 04 03     E8 00 FE 02     0B 03 E8 08
        01 00 00 00     00 64 00 00     0A 4A 43 4F     50 34 20 41
        54 50 4F
    sss:WARN :Communication channel is Plain.
    sss:WARN :!!!Not recommended for production use.!!!
    App:INFO :SE050 prepared successfully for MIFARE DESFire EV2 examples
    App:INFO :ex_sss Finished



