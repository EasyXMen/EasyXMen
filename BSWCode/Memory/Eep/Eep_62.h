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
**  FILENAME    : Eep_62.h                                                    **
**                                                                            **
**  Created on  : 2022/02/15                                                  **
**  Author      : Haibin.Shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*======================[Revision   History]==================================*/
/*  <VERSION>   <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0      20210123    Haibin.Shao     First Version
 *  V1.0.1      20230918    tao.yu          remove warning
 *  V1.0.2      20240202    xue.han         QAC
 *  V2.0.3      20240813    haibin.shao     Append QAC remarks and remove main function declarations from header files
 */
/*============================================================================*/
#ifndef EEP_62_H
#define EEP_62_H

#include "Std_Types.h"
#include "Eep_62_Cfg.h"
#include "Eep_62_Types.h"

#define EEP_62_VENDOR_ID                62U
#define EEP_62_MODULE_ID                250U
#define EEP_62_INSTANCE_ID              0U

#define EEP_62_AR_RELEASE_MAJOR_VERSION 4U
#define EEP_62_AR_RELEASE_MINOR_VERSION 5U
#define EEP_62_AR_RELEASE_PATCH_VERSION 0U
#define EEP_62_SW_MAJOR_VERSION         1U
#define EEP_62_SW_MINOR_VERSION         0U
#define EEP_62_SW_PATCH_VERSION         3U

/*==============[Macros]========================================================*/

/* function ID */
#define EEP_62_ID_INIT         (0x00U)
#define EEP_62_ID_SETMODE      (0x01U)
#define EEP_62_ID_READ         (0x02U)
#define EEP_62_ID_WRITE        (0x03U)
#define EEP_62_ID_ERASE        (0x04U)
#define EEP_62_ID_COMPARE      (0x05U)
#define EEP_62_ID_CANCEL       (0x06U)
#define EEP_62_ID_GETSTATUS    (0x07U)
#define EEP_62_ID_GETJOBRESULT (0x08U)
#define EEP_62_ID_MFUNC        (0x09U)
#define EEP_62_ID_GETVInfo     (0x0AU)

/* Error detection */
/* Invalid  configuration  set selection */
#define EEP_62_E_INIT_FAILED   (0x10U)
#define EEP_62_E_PARAM_ADDRESS (0x11U)
#define EEP_62_E_PARAM_DATA    (0x12U)
#define EEP_62_E_PARAM_LENGTH  (0x13U)

/*API service called without module initialization */
#define EEP_62_E_UNINT (0x20U)
/* API service called while driver still busy */
#define EEP_62_E_BUSY (0x21U)
/* Timeout exceeded */
#define EEP_62_E_TIMEOUT (0x22U)
/* API service called with a NULL pointer */
#define EEP_62_E_PARAM_POINTER (0x23U)

/* Re INIT */
#define EEP_62_E_REINIT (0x24U)

/* RUN IN MAINFCTION when Call EEP_Cancel */
#define EEP_62_E_IN_MAINF (0x25U)

/* Size of the buffer used for data verification */
#define EEP_COMPARE_BUF_SIZE (0x400U)

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

extern FUNC(void, EEP_PUBLIC_CODE) Eep_62_Init(P2CONST(Eep_62_ConfigType, AUTOMATIC, EEP_APPL_CONST) ConfigPtr);
extern FUNC(void, EEP_PUBLIC_CODE)
    Eep_62_SetMode(VAR(uint8, AUTOMATIC) deviceIndex, VAR(MemIf_ModeType, AUTOMATIC) Mode);
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, EEP_PUBLIC_CODE) Eep_62_Read(
    VAR(uint8, AUTOMATIC) deviceIndex,
    VAR(Eep_62_AddressType, AUTOMATIC) EepromAddress,
    P2VAR(uint8, AUTOMATIC, EEP_APPL_DATA) DataBufferPtr,
    VAR(Eep_62_LengthType, AUTOMATIC) Length);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, EEP_PUBLIC_CODE) Eep_62_Write(
    VAR(uint8, AUTOMATIC) deviceIndex,
    VAR(Eep_62_AddressType, AUTOMATIC) EepromAddress,
    P2CONST(uint8, AUTOMATIC, EEP_APPL_CONST) DataBufferPtr,
    VAR(Eep_62_LengthType, AUTOMATIC) Length);
extern FUNC(Std_ReturnType, EEP_PUBLIC_CODE) Eep_62_Erase(
    VAR(uint8, AUTOMATIC) deviceIndex,
    VAR(Eep_62_AddressType, AUTOMATIC) EepromAddress,
    VAR(Eep_62_LengthType, AUTOMATIC) Length);
extern FUNC(Std_ReturnType, EEP_PUBLIC_CODE) Eep_62_Compare(
    VAR(uint8, AUTOMATIC) deviceIndex,
    VAR(Eep_62_AddressType, AUTOMATIC) EepromAddress,
    P2CONST(uint8, AUTOMATIC, EEP_APPL_CONST) DataBufferPtr,
    VAR(Eep_62_LengthType, AUTOMATIC) Length);
extern FUNC(void, EEP_PUBLIC_CODE) Eep_62_Cancel(VAR(uint8, AUTOMATIC) deviceIndex);
extern FUNC(MemIf_StatusType, EEP_PUBLIC_CODE) Eep_62_GetStatus(VAR(uint8, AUTOMATIC) deviceIndex);
extern FUNC(MemIf_JobResultType, EEP_PUBLIC_CODE) Eep_62_GetJobResult(VAR(uint8, AUTOMATIC) deviceIndex);

#if (EEP_62_VERSION_INFO_API == STD_ON)
extern FUNC(void, EEP_PUBLIC_CODE)
    Eep_62_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, EEP_APPL_DATA) versioninfo);
#endif /* EEP_62_VERSION_INFO_API == STD_ON */

#endif
