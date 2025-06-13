/* PRQA S 3108++ */
/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Crypto_Alg.c                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuhao.ge                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for Crypto                                   **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.4.0                       **
**                                                                            **
*******************************************************************************/
#ifndef CRYPTO_ALG_H
#define CRYPTO_ALG_H
/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>    <AUTHOR>        <REVISION LOG>
 *  V1.0     20200701    yuhao.ge    Initial version
 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Std_Types.h"
#include "Det.h"
#include "Crypto.h"
#include "Crypto_Types.h"
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

#define CRYPTO_KEY_VALID (0x01U)
/** @brief  Indicate that key is not valid */
#define CRYPTO_KEY_NOT_VALID (0x00U)

#define CRYPTO_JOB_NOT_VALID (0xffffU)

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
/*report DET error*/
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
static inline void Crypto_ReportDetErr(uint8 serviceId, uint8 error)
{
    (void)Det_ReportError(CRYPTO_MODULE_ID, CRYPTO_MODULE_INSTANCE_ID, (serviceId), (error));
}

#endif
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/
#if (CRYPTO_MAXKEY_CONFIGURED > 0)
/*Enumeration of the current Key state*/
typedef enum
{
    CRYPTO_KEYSTATE_INVALID = 0x00u, /*Job is in the state "Valid".*/
    CRYPTO_KEYSTATE_VALID = 0x01u    /*Job is in the state "Invalid".*/
} Crypto_KeyStateType;

extern CONST(Crypto_KeyCfgType, CRYPTO_CONST) Crypto_Key[CRYPTO_MAXKEY_CONFIGURED];

extern VAR(Crypto_KeyStateType, CRYPTO_VAR) CryptoKeyStatus[CRYPTO_MAXKEY_CONFIGURED];

extern CONST(Crypto_DriverObjectCfgType, CRYPTO_CONST) Crypto_DriverObjects[CRYPTO_MAX_DRIVER_OBJECT];

#endif /* CRYPTO_MAXKEY_CONFIGURED > 0 */

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyElementIndexFind(
    VAR(uint32, AUTOMATIC) cryptokeyId,
    VAR(uint32, AUTOMATIC) keyElementId,
    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) getkeyElementId);
/* PRQA S 3432  */ /* MISRA Rule 20.7 */
FUNC(uint32, CRYPTO_CODE) Crypto_KeyIndexFind(VAR(uint32, AUTOMATIC) cryptokeyId);
FUNC(Std_ReturnType, CRYPTO_CODE)
Get_Key(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) key,
    /* PRQA S 3432  */ /* MISRA Rule 20.7 */
    VAR(uint8, AUTOMATIC) keyelementid);
FUNC(void, CRYPTO_CODE) Crypto_memcpy(void* buf, const void* buf1, uint32 len);
FUNC(void, CRYPTO_CODE) Crypto_memset(void* buf, uint32 len);
FUNC(void, CRYPTO_CODE) Crypto_memsetVal(void* buf, uint8 val, uint32 len);
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyElementSetInternal(
    uint32 cryptokeyId,
    uint32 keyElementId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) keyPtr,
    uint32 keyLength);

#endif
