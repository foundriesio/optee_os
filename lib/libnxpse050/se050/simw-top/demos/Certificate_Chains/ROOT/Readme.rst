..
    Copyright 2019 NXP

    This software is owned or controlled by NXP and may only be used
    strictly in accordance with the applicable license terms.  By expressly
    accepting such terms or by downloading, installing, activating and/or
    otherwise using the software, you are agreeing that you have read, and
    that you agree to comply with and are bound by, such license terms.  If
    you do not agree to be bound by the applicable license terms, then you
    may not retain, install, activate or otherwise use the software.


.. _cert-chains:

===========================================================================
 Certificate Chains : ROOT
===========================================================================

.. contents::
    :backlinks: none
    :local:



The directory ``demos/Certificate_Chains/ROOT`` contains RootCA and
Intermediate Certificates used in various configurations of SE050.


ECC
%%%%%%%%%%%%%%

This directory contains the ECC chain of trust for cloud on-boarding.

ROOT CA
-------------------------------------------------------------------------------------------------------------------

The file ``IOT_NXP-01-CERT_IOT_CA_KEY-IoTRootCAvE305-01-20190320162439-EC_SEC_P384R1-4B7E5A.crt``  contains ROOT CA.

.. literalinclude:: IOT_NXP-01-CERT_IOT_CA_KEY-IoTRootCAvE305-01-20190320162439-EC_SEC_P384R1-4B7E5A.crt


Intermediate CA
-------------------------------------------------------------------------------------------------------------------

The file ``IOT_NXP-01-CERT_IOT_4LAYER_CA_KEY-IoTInt4LAYERCAvE205-01-20190320164314-EC_SEC_P256R1-14DBBE.crt``
contains the Intermediate CA.

.. literalinclude:: IOT_NXP-01-CERT_IOT_4LAYER_CA_KEY-IoTInt4LAYERCAvE205-01-20190320164314-EC_SEC_P256R1-14DBBE.crt


RSA
%%%%%%%%%%%%%%

This directory contains the RSA chain of trust for cloud on-boarding.

ROOT CA
-------------------------------------------------------------------------------------------------------------------

The file ``IOT_NXP-01-CERT_IOT_CA_KEY-IoTRootCAvR406-01-20190425163255-RSA4096-BAB872.crt`` contains the ROOT CA.

.. literalinclude:: IOT_NXP-01-CERT_IOT_CA_KEY-IoTRootCAvR406-01-20190425163255-RSA4096-BAB872.crt


Intermediate CA
-------------------------------------------------------------------------------------------------------------------

The file
``IOT_NXP-01-CERT_IOT_4LAYER_CA_KEY-IoTInt4LayerCAvR406-01-20190425163534-RSA4096-540F19.crt``
contains the intermediate CA.

.. literalinclude:: IOT_NXP-01-CERT_IOT_4LAYER_CA_KEY-IoTInt4LayerCAvR406-01-20190425163534-RSA4096-540F19.crt



.. note:: The certificates shown here were last Updated on June 18, 2019 in this page/document.
