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
**  FILENAME    : Fee.h                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration and type definitions of FEE                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19_11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
#ifndef FEE_H
#define FEE_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Fee_Cfg.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define FEE_H_AR_MAJOR_VERSION 4u
#define FEE_H_AR_MINOR_VERSION 5u
#define FEE_H_AR_PATCH_VERSION 0u
#define FEE_H_SW_MAJOR_VERSION 2u
#define FEE_H_SW_MINOR_VERSION 0u
#define FEE_H_SW_PATCH_VERSION 3u

#define FEE_VENDOR_ID          62u
#define FEE_MODULE_ID          21u
#define FEE_INSTANCE_ID        0u
/*Service ID*/
#define FEE_INIT_ID                 0x00u
#define FEE_SETMODE_ID              0x01u
#define FEE_READ_ID                 0x02u
#define FEE_WRITE_ID                0x03u
#define FEE_CANCEL_ID               0x04u
#define FEE_GETSTATUS_ID            0x05u
#define FEE_GETJOBRESULT_ID         0x06u
#define FEE_INVALIDATEBLOCK_ID      0x07u
#define FEE_GETVERSIONINFO_ID       0x08u
#define FEE_ERASEIMMEDIATEBLOCK_ID  0x09u
#define FEE_JOBENDNOTIFICATION_ID   0x10u
#define FEE_JOBERRORNOTIFICATION_ID 0x11u
#define FEE_MAINFUNCTION_ID         0x12u
/*Development Errors*/
#define FEE_E_UNINIT            0x01u
#define FEE_E_INVALID_BLOCK_NO  0x02u
#define FEE_E_INVALID_BLOCK_OFS 0x03u
#define FEE_E_PARAM_POINTER     0x04u
#define FEE_E_INVALID_BLOCK_LEN 0x05u
#define FEE_E_INIT_FAILED       0x09u
/*Runtime Errors*/
#define FEE_E_BUSY           0x06u
#define FEE_E_INVALID_CANCEL 0x08u

/*the Private Macro Definition*/
#define FEE_UNUSED_UINT8     (0xFFu)
#define FEE_UNUSED_UINT16    (0xFFFFu)
#define FEE_UNUSED_UINT32    (0xFFFFFFFFu)
#define FEE_UNUSED_UINT64    (0xFFFFFFFFFFFFFFFFu)
#define FEE_NOUSED(paramter) (void)(paramter)
/*Change in special circumstances, all BANK data can be discarded*/
#define FEE_VALID_VALUE   0xAAu
#define FEE_INVALID_VALUE 0x55u
/*Flash write error(Write job accept,but not executed successfully),
 *Whether the next write job triggers Swap*/
#define FEE_ERROR_SWAP_BANK STD_ON
/*Development Errors FEE_E_INIT_FAILED detection enable or not.
 *R19_11:The development error is defined, but how to report it is not described.
 *R20_11:The development error has been removed.*/
#define FEE_E_INIT_FAILED_DET STD_OFF
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
BEGIN_C_DECLS

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

FUNC(void, FEE_CODE)
Fee_Init(P2CONST(Fee_ConfigType, AUTOMATIC, FEE_APPL_CONST) ConfigPtr);

FUNC(void, FEE_CODE)
Fee_SetMode(VAR(MemIf_ModeType, AUTOMATIC) Mode);

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(Std_ReturnType, FEE_CODE)
Fee_Read(
    VAR(uint16, AUTOMATIC) BlockNumber,
    VAR(uint16, AUTOMATIC) BlockOffset,
    P2VAR(uint8, AUTOMATIC, FEE_APPL_DATA) DataBufferPtr,
    VAR(uint16, AUTOMATIC) Length);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

FUNC(Std_ReturnType, FEE_CODE)
Fee_Write(VAR(uint16, AUTOMATIC) BlockNumber, P2CONST(uint8, AUTOMATIC, FEE_APPL_CONST) DataBufferPtr);

FUNC(void, FEE_CODE) Fee_Cancel(void);

FUNC(MemIf_StatusType, FEE_CODE) Fee_GetStatus(void);

FUNC(MemIf_JobResultType, FEE_CODE) Fee_GetJobResult(void);

FUNC(Std_ReturnType, FEE_CODE) Fee_InvalidateBlock(VAR(uint16, AUTOMATIC) BlockNumber);

#if (FEE_VERSION_INFO_API == STD_ON)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(void, FEE_CODE)
Fee_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, FEE_APPL_DATA) VersionInfoPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif

FUNC(Std_ReturnType, FEE_CODE) Fee_EraseImmediateBlock(VAR(uint16, AUTOMATIC) BlockNumber);

FUNC(void, FEE_CODE) Fee_MainFunction(void);

extern CONST(Fee_ConfigType, FEE_CONST_PBCFG) Fee_ConfigData;

END_C_DECLS

#endif /* FEE_H */
