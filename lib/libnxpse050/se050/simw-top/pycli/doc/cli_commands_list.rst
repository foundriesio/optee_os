

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

.. _cli-cmd-list:

==================================
List of ``ssscli`` commands
==================================

SSSCLI uses PEM, DER and HEX data formats for keys and certificates. Refer :ref:`cli-data-format`.

.. note:: Linux Environment

    You can ``source pycli/ssscli-bash-completion.sh`` for auto-completion
    on bash with linux/posix based environemnt.


SSSCLI Commands
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

These are the top level commands accepted by the SSSCLI Tool.

1) ``ssscli``::

    Usage: ssscli [OPTIONS] COMMAND [ARGS]...

      Command line interface for SE050

    Options:
      -v, --verbose  Enables verbose mode.
      --version      Show the version and exit.
      --help         Show this message and exit.

    Commands:
      a71ch       A71CH specific commands
      cloud       (Not Implemented) Cloud Specific utilities.
      connect     Open Session.
      decrypt     Decrypt Operation
      disconnect  Close session.
      encrypt     Encrypt Operation
      erase       Erase ECC/RSA/AES Keys or Certificate (contents)
      generate    Generate ECC/RSA Key pair
      get         Get ECC/RSA/AES Keys or certificates
      refpem      Create Reference PEM/DER files (For OpenSSL Engine).
      se05x       SE05X specific commands
      set         Set ECC/RSA/AES Keys or certificates
      sign        Sign Operation
      verify      verify Operation


#) ``ssscli connect``::

    Usage: ssscli connect [OPTIONS] subsystem method port_name

      Open Session.

      subsystem = Security subsystem is selected to be used. Can be one of
      "mbedtls, a71ch, se050, a71cl, openssl"

      method = Connection method to the system. Can be one of "none, sci2c,
      vcom, t1oi2c, jrcpv1, jrcpv2, pcsc"

      port_name = Subsystem specific connection parameters. Example: COM6,
      127.0.0.1:8050. Use "None" where not applicable. e.g. SCI2C/T1oI2C

    Options:
      --auth_type [None|PlatformSCP|UserID|ECKey|AESKey]
                                      Authentication type. Default is "None". Can
                                      be one of "None, UserID, ECKey, AESKey,
                                      PlatformSCP"
      --help                          Show this message and exit.


#) ``ssscli disconnect``::

    Usage: ssscli disconnect [OPTIONS]

      Close session.

    Options:
      --help  Show this message and exit.


#) ``ssscli set``::

    Usage: ssscli set [OPTIONS] COMMAND [ARGS]...

      Set ECC/RSA/AES Keys or certificates

    Options:
      --help  Show this message and exit.

    Commands:
      aes   Set AES Keys
      cert  Set Certificate
      ecc   Set ECC Keys
      hmac  Set HMAC Keys
      rsa   Set RSA Keys


#) ``ssscli get``::

    Usage: ssscli get [OPTIONS] COMMAND [ARGS]...

      Get ECC/RSA/AES Keys or certificates

    Options:
      --help  Show this message and exit.

    Commands:
      aes   Get AES Keys
      cert  Get Certificate
      ecc   Get ECC Keys
      rsa   Get RSA Keys


#) ``ssscli generate``::

    Usage: ssscli generate [OPTIONS] COMMAND [ARGS]...

      Generate ECC/RSA Key pair

    Options:
      --help  Show this message and exit.

    Commands:
      ecc  Generate ECC Key
      pub  Generate ECC Public Key to file
      rsa  Generate RSA Key


#) ``ssscli erase``::

    Usage: ssscli erase [OPTIONS] keyid

      Erase ECC/RSA/AES Keys or Certificate (contents)

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

    Options:
      --help  Show this message and exit.


#) ``ssscli cloud``::

    Usage: ssscli cloud [OPTIONS] COMMAND [ARGS]...

      (Not Implemented) Cloud Specific utilities.

      This helps to handle GCP/AWS/Watson specific settings.

    Options:
      --help  Show this message and exit.

    Commands:
      aws  (Not Implemented) AWS (Amazon Web Services) Specific utilities This...
      gcp  (Not Implemented) GCP (Google Cloud Platform) Specific utilities
           This...
      ibm  (Not Implemented) IBM Watson Specific utilities This helps to handle...


#) ``ssscli a71ch``::

    Usage: ssscli a71ch [OPTIONS] COMMAND [ARGS]...

      A71CH specific commands

    Options:
      --help  Show this message and exit.

    Commands:
      reset  Debug Reset A71CH
      uid    Get A71CH Unique ID


#) ``ssscli se05x``::

    Usage: ssscli se05x [OPTIONS] COMMAND [ARGS]...

      SE05X specific commands

    Options:
      --help  Show this message and exit.

    Commands:
      certuid     Get SE05X Cert Unique ID (10 bytes)
      readidlist  Read contents of SE050
      reset       Reset SE05X
      uid         Get SE05X Unique ID (18 bytes)


#) ``ssscli refpem``::

    Usage: ssscli refpem [OPTIONS] COMMAND [ARGS]...

      Create Reference PEM/DER files (For OpenSSL Engine).

    Options:
      --help  Show this message and exit.

    Commands:
      ecc  Refpem ECC Keys
      rsa  Refpem RSA Keys


#) ``ssscli sign``::

    Usage: ssscli sign [OPTIONS] keyid certificate filename

      Sign Operation

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      certificate = Input data to sign. can be raw data (DER format) or in file.
      For file, by default filename with extension .pem and .cer considered as
      PEM format and others as DER format.

      filename = File name to store signature data. By default filename with
      extension .pem in PEM format and others in DER format.

    Options:
      --informat TEXT   Input certificate format. TEXT can be "DER" or "PEM"
      --outformat TEXT  Output file format. TEXT can be "DER" or "PEM"
      --hashalgo TEXT   Hash algorithm. TEXT can be one of "SHA1, SHA224, SHA256,
                        SHA384, SHA512,
                        RSASSA_PKCS1_V1_5_SHA1,
                        RSASSA_PKCS1_V1_5_SHA224,
                        RSASSA_PKCS1_V1_5_SHA256,
                        RSASSA_PKCS1_V1_5_SHA384,
                        RSASSA_PKCS1_V1_5_SHA512,
                        RSASSA_PKCS1_PSS_MGF1_SHA1,
                        RSASSA_PKCS1_PSS_MGF1_SHA224,
                        RSASSA_PKCS1_PSS_MGF1_SHA256,
                        RSASSA_PKCS1_PSS_MGF1_SHA384,
                        RSASSA_PKCS1_PSS_MGF1_SHA512"
      --help            Show this message and exit.


#) ``ssscli verify``::

    Usage: ssscli verify [OPTIONS] keyid certificate signature

      verify operation

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      certificate = Input data for verification. Can be in file or raw data

      For file, by default filename with extension .pem and .cer considered as
      PEM format and others as DER format.

      filename = signature data file for verification. By default filename with
      extension .pem in PEM format and others in DER format.

    Options:
      --format TEXT    certificate and signature file format. TEXT can be "DER" or
                       "PEM"
      --hashalgo TEXT  Hash algorithm. TEXT can be one of "SHA1, SHA224, SHA256,
                       SHA384, SHA512,
                       RSASSA_PKCS1_V1_5_SHA1,
                       RSASSA_PKCS1_V1_5_SHA224,
                       RSASSA_PKCS1_V1_5_SHA256,
                       RSASSA_PKCS1_V1_5_SHA384,
                       RSASSA_PKCS1_V1_5_SHA512,
                       RSASSA_PKCS1_PSS_MGF1_SHA1,
                       RSASSA_PKCS1_PSS_MGF1_SHA224,
                       RSASSA_PKCS1_PSS_MGF1_SHA256, RSASSA_PKCS1_PSS_MGF1_SHA384,
                       RSASSA_PKCS1_PSS_MGF1_SHA512"
      --help           Show this message and exit.


#) ``ssscli encrypt``::

    Usage: ssscli encrypt [OPTIONS] keyid input_data filename

      Sign Operation

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      input_data = Input data to Encrypt. can be raw string or in file.

      filename = Output file name to store encrypted data. Encrypted data will
      be stored in DER format.

    Options:
      --algo TEXT  Algorithm. TEXT can be one of "oaep", "rsaes"
      --help       Show this message and exit.


#) ``ssscli decrypt``::

    Usage: ssscli decrypt [OPTIONS] keyid encrypted_data filename

      Sign Operation

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      encrypted_data = Encrypted data to Decrypt. can be raw data or in file.
      Input data should be in DER format.

      filename = Output file name to store Decrypted data.

    Options:
      --algo TEXT  Algorithm. TEXT can be one of "oaep", "rsaes"
      --help       Show this message and exit.



Set Commands
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

These commands are used to set/put objects/keys to the target secure subsystem.

#) ``ssscli set aes``::

    Usage: ssscli set aes [OPTIONS] keyid key

      Set AES Keys

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      key = Can be in file or raw key in DER or HEX format

    Options:
      --help  Show this message and exit.


#) ``ssscli set hmac``::

    Usage: ssscli set hmac [OPTIONS] keyid key

      Set HMAC Keys

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      key = Can be in file or raw key in DER or HEX format

    Options:
      --help  Show this message and exit.


#) ``ssscli set cert``::

    Usage: ssscli set cert [OPTIONS] keyid key

      Set Certificate

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      key = Can be raw certificate (DER format) or in file. For file, by default
      filename with extension .pem and .cer considered as PEM format and others
      as DER format.

    Options:
      --format TEXT  Input certificate format. TEXT can be "DER" or "PEM"
      --help         Show this message and exit.


#) ``ssscli set ecc pair``::

    Usage: ssscli set ecc pair [OPTIONS] keyid key

      Set ECC Key pair

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      key = Can be raw key (DER format) or in file. For file, by default
      filename with extension .pem considered as PEM format and others as DER
      format.

    Options:
      --format TEXT  Input key format. TEXT can be "DER" or "PEM"
      --help         Show this message and exit.


#) ``ssscli set ecc pub``::

    Usage: ssscli set ecc pub [OPTIONS] keyid key

      Set ECC Public Keys

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      key = Can be raw key (DER format) or in file. For file, by default
      filename with extension .pem considered as PEM format and others as DER
      format.

    Options:
      --format TEXT  Input key format. TEXT can be "DER" or "PEM"
      --help         Show this message and exit.


#) ``ssscli set rsa pair``::

    Usage: ssscli set rsa pair [OPTIONS] keyid key

      Set RSA Key Pair

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      key = Can be raw key (DER format) or in file. For file, by default
      filename with extension .pem considered as PEM format and others as DER
      format.

    Options:
      --format TEXT  Input key format. TEXT can be "DER" or "PEM"
      --help         Show this message and exit.


#) ``ssscli set rsa pub``::

    Usage: ssscli set rsa pub [OPTIONS] keyid key

      Set RSA Public Keys

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      key = Can be raw key (DER format) or in file. For file, by default
      filename with extension .pem considered as PEM format and others as DER
      format.

    Options:
      --format TEXT  Input key format. TEXT can be "DER" or "PEM"
      --help         Show this message and exit.



Get Commands
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

These commands are used to retereive/get objects/keys from the target secure subsystem.

#) ``ssscli get aes``::

    Usage: ssscli get aes [OPTIONS] keyid filename

      Get AES Keys

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      filename = File name to store key. Data can be in PEM or DER format based
      on file extension. By default filename with extension .pem in PEM format
      and others in DER format.

    Options:
      --format TEXT  Output file format. TEXT can be "DER" or "PEM"
      --help         Show this message and exit.


#) ``ssscli get cert``::

    Usage: ssscli get cert [OPTIONS] keyid filename

      Get Certificate

      keyid = 32bit Key ID. Should be in hex format. Example: 401286E6

      filename = File name to store certificate. Data can be in PEM or DER
      format based on file extension. By default filename with extension .pem
      and .cer in PEM format and others in DER format.

    Options:
      --format TEXT  Output file format. TEXT can be "DER" or "PEM"
      --help         Show this message and exit.


#) ``ssscli get ecc pair``::

    Usage: ssscli get ecc pair [OPTIONS] keyid filename

      Get ECC Pair

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      filename = File name to store key. Data can be in PEM or DER format based
      on file extension. By default filename with extension .pem in PEM format
      and others in DER format.

    Options:
      --format TEXT  Output file format. TEXT can be "DER" or "PEM"
      --help         Show this message and exit.


#) ``ssscli get ecc pub``::

    Usage: ssscli get ecc pub [OPTIONS] keyid filename

      Get ECC Pub

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      filename = File name to store key. Data can be in PEM or DER format based
      on file extension. By default filename with extension .pem in PEM format
      and others in DER format.

    Options:
      --format TEXT  Output file format. TEXT can be "DER" or "PEM"
      --help         Show this message and exit.


#) ``ssscli get rsa pair``::

    Usage: ssscli get rsa pair [OPTIONS] keyid filename

      Get RSA Pair

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      filename = File name to store key. Data can be in PEM or DER format based
      on file extension. By default filename with extension .pem in PEM format
      and others in DER format.

    Options:
      --format TEXT  Output file format. TEXT can be "DER" or "PEM"
      --help         Show this message and exit.


#) ``ssscli get rsa pub``::

    Usage: ssscli get rsa pub [OPTIONS] keyid filename

      Get RSA Pub

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      filename = File name to store key. Data can be in PEM or DER format based
      on file extension. By default filename with extension .pem in PEM format
      and others in DER format.

    Options:
      --format TEXT  Output file format. TEXT can be "DER" or "PEM"
      --help         Show this message and exit.



Generate Commands
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

These commands are used to generate objects/keys inside the target secure subsystem.

#) ``ssscli generate ecc``::

    Usage: ssscli generate ecc [OPTIONS] keyid [NIST_P192|NIST_P224|NIST_P256|NIST
                               _P384|NIST_P521|Brainpool160|Brainpool192|Brainpool
                               224|Brainpool256|Brainpool320|Brainpool384|Brainpoo
                               l512|Secp160k1|Secp192k1|Secp224k1|Secp256k1|ED_255
                               19|MONT_DH_25519|MONT_DH_448]

      Generate ECC Key

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      curvetype = ECC Curve type. can be one of "NIST_P192, NIST_P224,
      NIST_P256, NIST_P384, NIST_P521, Brainpool160, Brainpool192, Brainpool224,
      Brainpool256, Brainpool320, Brainpool384, Brainpool512, Secp160k1,
      Secp192k1, Secp224k1, Secp256k1, ED_25519, MONT_DH_25519, MONT_DH_448"

    Options:
      --help  Show this message and exit.


#) ``ssscli generate rsa``::

    Usage: ssscli generate rsa [OPTIONS] keyid [1024|2048|3072|4096]

      Generate RSA Key

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      bits = Number of bits. can be one of "1024, 2048, 3072, 4096"

    Options:
      --help  Show this message and exit.



Refpem Commands
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

These commands are used to get Reference/masked Keys usable by openssl engines.

#) ``ssscli refpem ecc pair``::

    Usage: ssscli refpem ecc pair [OPTIONS] keyid filename

      Create reference PEM file for ECC Pair

      keyid = 32bit Key ID. Should be in hex format. Example: 0x20E8A001

      filename = File name to store key. Can be in PEM or DER or PKCS12 format
      based on file extension. By default filename with extension .pem in PEM
      format, .pfx or .p12 in PKCS12 format and others in DER format.

    Options:
      --format TEXT    Output file format. TEXT can be "DER" or "PEM" or "PKCS12"
      --password TEXT  Password used for PKCS12 format.
      --help           Show this message and exit.


#) ``ssscli refpem ecc pub``::

    Usage: ssscli refpem ecc pub [OPTIONS] keyid filename

      Create reference PEM file for ECC Pub

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      filename = File name to store key. Data Can be in PEM or DER format or
      PKCS12 format based on file extension. By default filename with extension
      .pem in PEM format, .pfx or .p12 in PKCS12 format and others in DER
      format.

    Options:
      --format TEXT    Output file format. TEXT can be "DER" or "PEM" or "PKCS12"
      --password TEXT  Password used for PKCS12 format.
      --help           Show this message and exit.


#) ``ssscli refpem rsa pair``::

    Usage: ssscli refpem rsa pair [OPTIONS] keyid filename

      Create reference PEM file for RSA Pair

      keyid = 32bit Key ID. Should be in hex format. Example: 20E8A001

      filename = File name to store key. Data Can be in PEM or DER format or
      PKCS12 format based on file extension. By default filename with extension
      .pem in PEM format, .pfx or .p12 in PKCS12 format and others in DER
      format.

    Options:
      --format TEXT    Output file format. TEXT can be "DER" or "PEM" or "PKCS12"
      --password TEXT  Password used for PKCS12 format.
      --help           Show this message and exit.



Se05x Commands
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

These are the SE05x specific commands.

#) ``ssscli se05x uid``::

    Usage: ssscli se05x uid [OPTIONS]

      Get 18 bytes Unique ID from the SE05X Secure Module.

    Options:
      --help  Show this message and exit.


#) ``ssscli se05x certuid``::

    Usage: ssscli se05x certuid [OPTIONS]

      Get 10 bytes Cert Unique ID from the SE05X Secure Module. The cert uid is
      a subset of the Secure Module Unique Identifier

    Options:
      --help  Show this message and exit.


#) ``ssscli se05x reset``::

    Usage: ssscli se05x reset [OPTIONS]

      Resets the SE05X Secure Module to the initial state.

      This command uses ``Se05x_API_DeleteAll_Iterative`` API of the SE05X MW to
      iterately delete objects provisioned inside the SE.  Because of this, some
      objects are purposefully skipped from deletion.

      It does not use the low level SE05X API ``Se05x_API_DeleteAll``

      For more information, see documentation/implementation of the
      ``Se05x_API_DeleteAll_Iterative`` API.

    Options:
      --help  Show this message and exit.


#) ``ssscli se05x readidlist``::

    Usage: ssscli se05x readidlist [OPTIONS]

      Read contents of SE050

    Options:
      --help  Show this message and exit.



A71CH Commands
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

These are the A71CH specific commands.

#) ``ssscli a71ch uid``::

    Usage: ssscli a71ch uid [OPTIONS]

      Get uid from the A71CH Secure Module.

    Options:
      --help  Show this message and exit.


#) ``ssscli a71ch reset``::

    Usage: ssscli a71ch reset [OPTIONS]

      Resets the A71CH Secure Module to the initial state.

    Options:
      --help  Show this message and exit.


