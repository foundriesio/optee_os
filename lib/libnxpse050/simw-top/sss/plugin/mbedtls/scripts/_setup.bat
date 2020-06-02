@echo off
@REM Copyright 2018,2019 NXP
@REM
@REM This software is owned or controlled by NXP and may only be used
@REM strictly in accordance with the applicable license terms.  By expressly
@REM accepting such terms or by downloading, installing, activating and/or
@REM otherwise using the software, you are agreeing that you have read, and
@REM that you agree to comply with and are bound by, such license terms.  If
@REM you do not agree to be bound by the applicable license terms, then you
@REM may not retain, install, activate or otherwise use the software.
@REM
cls

SET CERT_DIR=..\keys\%~1
SET P_dtls=
SET P_exchanges=
SET P_force_version=
SET P_force_ciphersuite=
SET P_debug_level=
SET P_ca_file=
SET P_auth_mode=
SET P_connect=


REM SET P_dtls=dtls=1
SET P_exchanges=exchanges=1
SET P_force_version=force_version=tls1_2
SET P_debug_level=debug_level=1
SET P_ca_file=ca_file=%CERT_DIR%\tls_rootca.cer
SET P_auth_mode=auth_mode=required

SET P_key_file_client=key_file=none
SET P_crt_file_client=crt_file=none

SET P_key_file_client_none=key_file=none
SET P_crt_file_client_none=crt_file=none

SET P_key_file_server=key_file=%CERT_DIR%\tls_server_key.pem
SET P_crt_file_server=crt_file=%CERT_DIR%\tls_server.cer

@REM ==================================
SET P_MBED=%P_dtls% %P_exchanges% %P_force_version% %P_debug_level% %P_ca_file% %P_auth_mode%


SET P_MBED_SERVER=%P_key_file_server% %P_crt_file_server%
SET P_MBED_CLIENT=%P_key_file_client% %P_crt_file_client%
SET P_MBED_CLIENT_AX=%P_key_file_client_none% %P_crt_file_client_none%

set CUR_DIR=


@REM TLS-ECDH-ECDSA-WITH-AES-128-CBC-SHA
@REM TLS-ECDH-ECDSA-WITH-AES-128-CBC-SHA256
@REM TLS-ECDH-ECDSA-WITH-AES-256-CBC-SHA
@REM TLS-ECDHE-ECDSA-WITH-AES-128-CBC-SHA
@REM TLS-ECDHE-ECDSA-WITH-AES-128-CBC-SHA256
@REM TLS-ECDHE-ECDSA-WITH-AES-256-CBC-SHA
