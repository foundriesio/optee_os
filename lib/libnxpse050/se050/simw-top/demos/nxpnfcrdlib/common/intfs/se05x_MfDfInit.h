/**
 * @file se05x_MfDfInit.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2019 NXP
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
 * Header File for  functions that setup SE05X for MIFARE DESFire examples.
 * Please note Rdlib support is not needed for this file.
 */

#ifndef SE05X_SUPPORT_H
#define SE05X_SUPPORT_H
/* *****************************************************************************************************************
* Includes
* ***************************************************************************************************************** */
#include <stdio.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

#include "ex_sss.h"
#include "fsl_sss_se05x_types.h"
#include "ex_sss_boot.h"
#include "ex_sss_objid.h"
#ifdef __cplusplus
}
#endif

/* *****************************************************************************************************************
* Internal Definitions
* ***************************************************************************************************************** */

/* doc:start:mif-kdf-keyids */
#define MFDFEV2_KEYID (EX_SSS_OBJID_DEMO_MFDF_START)
#define MFDFEV2_CHANGED_KEYID (EX_SSS_OBJID_DEMO_MFDF_START +1)
/* doc:end:mif-kdf-keyids */


/* *****************************************************************************************************************
* Functions Prototypes
* ***************************************************************************************************************** */

/**
* Create the required objects in SE050 for NFC examples the First time
* The first key is the 'oldKey'
* The Second  key is the 'newKey'
* This function inits, allocates and stores the two keys
* The two keys are stored with the correct policies. i.e. Desfire auth capable
*
* @param pCtx boot context for the Example.
* @param keyId  First objID used for auth/changekey
* @param derivedkeyId    Subsequent objID used for auth/changekey
* @return Status of the operation.
*/
extern sss_status_t InitialSetupSe050(sss_key_store_t *pkeyStore, uint32_t keyId, uint32_t derivedkeyId);


/**
* Use the already created objects in SE050 for NFC examples
*
* @param pCtx boot context for the Example.
* @param keyId  First objID used for auth/changekey
* @param derivedkeyId    Subsequent objID used for auth/changekey
* @return Status of the operation.
*/
extern sss_status_t SubsequentSetupSe050(sss_key_store_t *pkeyStore, uint32_t keyId, uint32_t derivedkeyId);


#endif /* SE05X_SUPPORT_H*/
