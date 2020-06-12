/**
 * @file HostCryptoAPIStubs.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2017 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * Host Crypto Stub implementation
 *
 * @par HISTORY
 *
 */

#include "HostCryptoAPI.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>


HLSE_RET_CODE   HLCRYPT_GetSupportedMechanisms(HLSE_MECHANISM_TYPE* mechanism, U32* mechanismLen)
{
    return HOST_CRYPTO_ERROR;
}

HLSE_RET_CODE   HLCRYPT_Encrypt(HLSE_MECHANISM_INFO* pMechanismType, U8* inKey, U32 inKeyLen,
    U8* inData, U32 inDataLen,
    U8* outEncryptedData, U32* outEncryptedDataLen)
{
    return HOST_CRYPTO_ERROR;
}

HLSE_RET_CODE   HLCRYPT_Decrypt(HLSE_MECHANISM_INFO* pMechanismType, U8* inKey, U32 inKeyLen,
    U8* inData, U32 inDataLen,
    U8* outDecryptedData, U32* outDecryptedDataLen)
{
    return HOST_CRYPTO_ERROR;
}

HLSE_RET_CODE   HLCRYPT_Digest(HLSE_MECHANISM_INFO* pMechanismType,
    U8* inData, U32 inDataLen,
    U8* outDigest, U32* outDigestLen)
{
    return HOST_CRYPTO_ERROR;
}



HLSE_RET_CODE   HLCRYPT_Sign(HLSE_MECHANISM_INFO* pMechanismType, U8* inKey, U32 inKeyLen,
    U8* inData, U32 inDataLen,
    U8* outSignature, U32* outSignatureLen)
{
    return HOST_CRYPTO_ERROR;
}

HLSE_RET_CODE   HLCRYPT_SignInit(HLSE_MECHANISM_INFO* pMechanismType, U8* inKey, U32 inKeyLen, HLSE_CONTEXT_HANDLE* hContext)
{
    return HOST_CRYPTO_ERROR;
}

HLSE_RET_CODE   HLCRYPT_SignUpdate(HLSE_CONTEXT_HANDLE hContext, U8* inDataPart, U32 inDataPartLen)
{
    return HOST_CRYPTO_ERROR;
}

HLSE_RET_CODE   HLCRYPT_SignFinal(HLSE_CONTEXT_HANDLE hContext, U8* outSignature, U32* outSignatureLen)
{
    return HOST_CRYPTO_ERROR;
}

HLSE_RET_CODE    HLCRYPT_GetRandom(U32 inLen, U8 * pRandom)
{
    return HOST_CRYPTO_ERROR;
}
