..
    Copyright 2019 NXP

    This software is owned or controlled by NXP and may only be used
    strictly in accordance with the applicable license terms.  By expressly
    accepting such terms or by downloading, installing, activating and/or
    otherwise using the software, you are agreeing that you have read, and
    that you agree to comply with and are bound by, such license terms.  If
    you do not agree to be bound by the applicable license terms, then you
    may not retain, install, activate or otherwise use the software.


.. _linux-demos-greengrass:

==================================================
Greengrass Demo for Linux
==================================================

AWS IoT Greengrass is a software provided by AWS to extend cloud capabilities
to locally connected devices. This allows local devices to publish/subscribe
to a topic even if there is no connectivity with AWS IoT console. A Greengrass
group consists of a Greengrass core, multiple Greengrass devices connected to
that core, and lambda functions and other services running on that core. In
this, the Greengrass core performs the functions of AWS IoT console.

Also see
https://docs.aws.amazon.com/greengrass/latest/developerguide/what-is-gg.html
for more details about AWS IoT Greengrass.

This demo is to demonstrate how to integrate SE050 with AWS IoT Greengrass
core and RaspberryPi as hardware security to store core specific credentials
for IoT client and MQTT server.

.. note:: Hardware security feature is available only for AWS IoT Greengrass
    Core v1.7 and later. We have used Greengrass core v1.9.2 for integration

Prerequisites
==================================================

- AWS Greengrass account (Also see supported regions for Greengrass)
- RaspberryPi 3 Model B+ or Model B. The architecture of your Pi must be
  armv7l or later
- Raspbian Buster operating system
- Python 2.7
- pyCLI Tool. Refer to :ref:`cli-tool`


.. _prepare-greengrass:

Preparing the Greengrass group
==================================================

1)  Follow the modules 1 and 2 as described in
    https://docs.aws.amazon.com/greengrass/latest/developerguide/module1.html
    to set up Greengrass group and Greengrass core.

    .. note:: In Module 2, if you choose Easy Group Creation, AWS will create credentials
        for Greengrass IoT core and provision in the registry. Skip the next step
        if you choose Easy Group Creation. You could otherwise create your
        own credentials and provision AWS registry as explained in the next step.


#)  If you wish to use your own credentials, upload the your RootCA and verification
    certificate in ``Secure->CAs`` tab under IoT Core.

    - While creating Greengrass group, choose ``Advanced group creation``.
    - You can either assign IAM role or skip it for later.
    - Under Set up your security, choose ``Advanced setup`` and then choose ``Use my certificate``.
    - Select your active RootCA certificate and upload corresponding device certificate


#)  If you used your own credentials, download sample :file:`config.json` file for greengrass available at
    https://docs.aws.amazon.com/greengrass/latest/developerguide/gg-core.html#config-json

    After completing Module 2, store your device certificate under certs
    directory where you have extracted AWS IoT Greengrass core software (by
    default :file:`/greengrass` directory) and the downloaded
    :file:`config.json` under config directory.


#)  Do **NOT** run the daemon yet.


Provisioning SE050 and Building PKCS#11 library
==================================================

1)  Before running the Greengrass daemon, you would need to
    provision your SE050 and build PKCS#11 library.

#)  Complete :numref:`cli-doc-pre-steps` :ref:`cli-doc-pre-steps` for pyCLI tool setup

#)  Run the following steps to provision your SE050 with Greengrass core keypair::

        ssscli connect se050 t1oi2c none
        ssscli se05x reset
        ssscli set ecc pair 0x20181001 <path-to-core-keypair>
        ssscli disconnect

    .. note:: Greengrass uses labels to address objects on tokens. To make the PKCS#11 library
        use a specific keyID, the label should start with ``sss:`` followed by 32-bit keyID in
        hexadecimal format. For example, the label for the command used above would be
        ``sss:20181001``.

#)  Build PKCS#11 library for Greengrass core. Refer to :numref:`pkcs11-standalone` :ref:`pkcs11-standalone`


Updating Greengrass configuration
==================================================

If you have successfully completed :ref:`prepare-greengrass`, you would have
:file:`config.json` under config directory of AWS IoT Greengrass core software
(by default as :file:`/greengrass` directory). A sample of :file:`config.json`
is::

    {
      "coreThing" : {
        "caPath" : "root.ca.pem",
        "certPath" : "hash.cert.pem",
        "keyPath" : "hash.private.key",
        "thingArn" : "arn:partition:iot:region:account-id:thing/core-thing-name",
        "iotHost" : "host-prefix-ats.iot.region.amazonaws.com",
        "ggHost" : "greengrass-ats.iot.region.amazonaws.com",
        "keepAlive" : 600
      },
      "runtime" : {
        "cgroup" : {
          "useSystemd" : "yes"
        }
      },
      "managedRespawn" : false,
      "crypto" : {
        "principals" : {
          "SecretsManager" : {
            "privateKeyPath" : "file:///greengrass/certs/hash.private.key"
          },
          "IoTCertificate" : {
            "privateKeyPath" : "file:///greengrass/certs/hash.private.key",
            "certificatePath" : "file:///greengrass/certs/hash.cert.pem"
          }
        },
        "caPath" : "file:///greengrass/certs/root.ca.pem"
      }
    }

Remove the ``caPath``, ``certPath``, and ``keyPath`` values from the ``coreThing`` object.

Update the ``certificatePath`` property of ``IoTCertificate`` object to the path of device certificate.

.. note:: Currently AWS IoT Greengrass core does not support loading certificates from hardware.
        These have to be provided as a path to a file on filesystem.

Update the values of ``privateKeyPath`` under ``SecretsManager`` and ``IoTCertificate`` objects
with *pkcs11:object=iotkey;type=private*.

Add the following ``MQTTServerCertificate`` object under ``principals`` object::

    "MQTTServerCertificate": {
        "privateKeyPath": "pkcs11:object=iotkey;type=private"
    }

Add the following ``PKCS11`` object under ``crypto`` object::

    "PKCS11": {
        "P11Provider": "/path/to/libgreengrass.so",
        "slotLabel": "greengrass",
        "slotUserPin": "1234"
    }

Add commas where needed to create a valid JSON document.

In this file, we have used a shared key for ``MQTTServerCertificate``,
``IoTCertificate`` and ``SecretsManager`` components. In ``PKCS11`` object, we
specify which PKCS#11 module to load and which slot to use in that module. All
PKCS#11 objects specified for different components will refer to the same
token.


Running Greengrass Core
==================================================

Start the Greengrass daemon by running the following command in
:file:`ggc/core` directory under AWS IoT Greengrass core software directory::

    sudo ./greengrassd start

The Daemon should start successfully. If you face any problem while starting
the Greengrass daemon, refer to Troubleshooting section below. Also see
runtime logs under :file:`/greengrass/ggc/var/log/system` directory.


Connecting Devices to Greengrass Core
==================================================

Follow steps mentioned from Module 3 to test Greengrass connectivity.
https://docs.aws.amazon.com/greengrass/latest/developerguide/module3-I.html


Troubleshooting
==================================================

1)  Error message **greengrass deployment failed too many levels of symbolic links**

    Check if your linux supports OverlayFS. Also confirm that the Raspberry Pi
    image version matches the version specified in
    https://docs.aws.amazon.com/greengrass/latest/developerguide/setup-filter.rpi.html.
    Currently, AWS IoT Greengrass Core has been tested on
    **2019-07-10-raspbian-buster** image. Greengrass core might not work with
    newer images like Raspbian Stretch.

#)  Error message **connection reset by peer**.

    Add properties ``iotHttpPort`` and ``ggHttpPort`` to ``coreThing`` object as::

        "iotHttpPort" : 443,
        "ggHttpPort" : 443


If you face any other issue, refer to
https://docs.aws.amazon.com/greengrass/latest/developerguide/gg-troubleshooting.html

