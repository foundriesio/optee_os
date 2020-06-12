# Copyright 2019 NXP
#
# This software is owned or controlled by NXP and may only be used
# strictly in accordance with the applicable license terms.  By expressly
# accepting such terms or by downloading, installing, activating and/or
# otherwise using the software, you are agreeing that you have read, and
# that you agree to comply with and are bound by, such license terms.  If
# you do not agree to be bound by the applicable license terms, then you
# may not retain, install, activate or otherwise use the software.
#

credentials = {
    "DEVICE_KEY": "",
    "GATEWAY_KEY": "",
    "GATEWAY_CERT": "",
    "DEVICE_REF_KEY": "",
    "DEVICE_CERT": "",
    "INTERM_CA_CERT": "",
    "INTERM_CA_KEY": "",
    "ROOT_CA_KEY": "",
    "ROOT_CA_CERT_CER": "",
    "ROOT_CA_CERT_CRT": "",
}


def create_credentials(uid):
    credentials["DEVICE_KEY"] = "%d_device_key.pem" % uid
    credentials["DEVICE_REF_KEY"] = "%d_device_reference_key.pem" % uid
    credentials["DEVICE_CERT"] = "%d_device_certificate.cer" % uid
    credentials["GATEWAY_KEY"] = "%d_gateway_key.pem" % uid
    credentials["GATEWAY_CERT"] = "%d_gateway_certificate.crt" % uid
    credentials["INTERM_CA_CERT"] = "interCA_certificate.cer"
    credentials["INTERM_CA_KEY"] = "interCA_key.pem"
    credentials["ROOT_CA_KEY"] = "rootCA_key.pem"
    credentials["ROOT_CA_CERT_CER"] = "rootCA_certificate.cer"
    credentials["ROOT_CA_CERT_CRT"] = "rootCA_certificate.crt"
    return credentials
