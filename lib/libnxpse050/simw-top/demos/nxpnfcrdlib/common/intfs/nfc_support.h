/**
 * @file nfc_support.h
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
 * Header file for nfc support functions. Not dependent on sss/se050
 */

#ifndef _NFC_SUPPORT_H_
#define _NFC_SUPPORT_H_


/* *****************************************************************************************************************
* Includes
* ***************************************************************************************************************** */
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "ph_Status.h"

#ifdef __cplusplus
}
#endif


/* *****************************************************************************************************************
* Public Functions
* ***************************************************************************************************************** */

/*!
 * @brief Intialises the rdlib context
 * @param   pRdCtx rdLib context.
 * @return  void
 */
extern void nfcInit(NPNxpNfcRdLibCtx_t** ppRdLibCtx);

/*!
 * @brief connects to nfc
 * Inits, connects to nfc frontend activates the card.
 * @param   pRdCtx rdLib context.
 * @return  status
 */
extern phStatus_t nfcConnect(NPNxpNfcRdLibCtx_t* pRdLibCtx);


/*!
 * @brief card activate till level 4
 * Detects the card and does the L3 and L4 activation .
 * @param   pRdCtx rdLib context.
 * @return  status
 */
extern phStatus_t np_ActivateL4(NPNxpNfcRdLibCtx_t *pRdCtx);

/*!
 * @brief Disconnect from NFC and close the port
 * and close the port.
 * @param   pRdCtx rdLib context.
 * @return  status
 */
extern phStatus_t nfcDisconnect(NPNxpNfcRdLibCtx_t* pRdLibCtx);

/*!
 * @brief Initialises context of required layers
 * @param   pRdCtx rdLib context.
 * @return  status
 */
extern phStatus_t np_mfdf_Init(NPNxpNfcRdLibCtx_t *pRdCtx);



/* *****************************************************************************************************************
* Private Functions
* ***************************************************************************************************************** */
#endif
