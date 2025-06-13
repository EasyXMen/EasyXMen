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
**  FILENAME    : CDD_FVM.h                                                       **
**                                                                            **
**  Created on  : 2023-10-20                                                  **
**  Author      : Jian.Jiang                                                  **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#ifndef CDD_FVM_H
#define CDD_FVM_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "CDD_FVM_Types.h"
#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
#include "Det.h"
#endif
#include "SecOC.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define CDD_FVM_MODULE_ID        2099u
#define CDD_FVM_INSTANCE_ID      0u
#define CDD_FVM_VENDOR_ID        62u
#define CDD_FVM_SW_MAJOR_VERSION 1u
#define CDD_FVM_SW_MINOR_VERSION 0u
#define CDD_FVM_SW_PATCH_VERSION 6u
#define CDD_FVM_AR_MAJOR_VERSION 4u
#define CDD_FVM_AR_MINOR_VERSION 5u
#define CDD_FVM_AR_PATCH_VERSION 0u

/* Development Errors */
#define CDD_FVM_E_PARAM_POINTER      0x01u
#define CDD_FVM_E_UNINIT             0x02u
#define CDD_FVM_E_INVALID_PDU_SDU_ID 0x03u
#define CDD_FVM_E_INIT_FAILED        0x07u

/*Service ID*/
#define CDD_FVM_INIT_ID                   ((uint8)0x01u)
#define CDD_FVM_DEINIT_ID                 ((uint8)0x02u)
#define CDD_FVM_GETVERSIONINFO            ((uint8)0x03u)
#define CDD_FVM_GETRXFRESHNESS            ((uint8)0x04u)
#define CDD_FVM_GETRXFRESHNESSAUTHDATA    ((uint8)0x05u)
#define CDD_FVM_GETTXFRESHNESS            ((uint8)0x06u)
#define CDD_FVM_GETTXFRESHNESSTRUNCDATA   ((uint8)0x07u)
#define CDD_FVM_SPDUTXCONFIRMATION        ((uint8)0x08u)
#define CDD_FVM_RXINDICATION_ID           ((uint8)0X09u)
#define CDD_FVM_SLAVEREQUESTSYNCMSG       ((uint8)0X0Au)
#define CDD_FVM_RXSECOCVERIFICATIONNOTIFY ((uint8)0X0Bu)

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/
extern CONST(CDD_FVM_ConfigType, CDD_FVM_CONST) CDD_FVM_ConfigData;

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
static inline void CDD_FVM_DET_REPORTERROR(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(CDD_FVM_MODULE_ID, CDD_FVM_INSTANCE_ID, (ApiId), (ErrorId));
}
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */

/* PRQA S 3472 ++ */ /* MISRA Dir 4.9 */
#if !defined(CDD_FVM_PARA_UNUSED)
#define CDD_FVM_PARA_UNUSED(x) ((void)(x))
#endif               /* defined CDD_FVM_PARA_UNUSED */
/* PRQA S 3472 -- */ /* MISRA Dir 4.9 */

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

FUNC(void, CDD_FVM_CODE) CDD_FVM_Init(P2CONST(CDD_FVM_ConfigType, AUTOMATIC, CDD_FVM_CFG_CONST) config);

FUNC(void, CDD_FVM_CODE) CDD_FVM_DeInit(void);

FUNC(void, CDD_FVM_CODE)
CDD_FVM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CDD_FVM_APPL_DATA) versioninfo);

FUNC(Std_ReturnType, CDD_FVM_CODE)
CDD_FVM_GetRxFreshness(
    uint16 FVMFreshnessValueID,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMTruncatedFreshnessValue,
    uint32 FVMTruncatedFreshnessValueLength,
    uint16 FVMAuthVerifyAttempts,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValue,
    P2VAR(uint32, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValueLength
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
);

FUNC(Std_ReturnType, CDD_FVM_CODE)
CDD_FVM_GetRxFreshnessAuthData(
    uint16 FVMFreshnessValueID,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMTruncatedFreshnessValue,
    uint32 FVMTruncatedFreshnessValueLength,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMAuthDataFreshnessValue,
    uint16 FVMAuthDataFreshnessValueLength,
    uint16 FVMAuthVerifyAttempts,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValue,
    P2VAR(uint32, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValueLength
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
);

FUNC(Std_ReturnType, CDD_FVM_CODE)
CDD_FVM_GetTxFreshness(
    uint16 FVMFreshnessValueID,
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValue,
    P2VAR(uint32, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValueLength
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
);

FUNC(Std_ReturnType, CDD_FVM_CODE)
CDD_FVM_GetTxFreshnessTruncData(
    uint16 FVMFreshnessValueID,
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMFreshnessValue,
    P2VAR(uint32, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValueLength,
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMTruncatedFreshnessValue,
    P2VAR(uint32, AUTOMATIC, CDD_FVM_APPL_DATA) FVMTruncatedFreshnessValueLength
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
);

FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_SPduTxConfirmation(uint16 FVMFreshnessValueID);

FUNC(void, CDD_FVM_CODE) CDD_FVM_RxSecOCVerificationNotify(SecOC_VerificationStatusType verificationStatus);

FUNC(void, CDD_FVM_CODE)
CDD_FVM_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, CDD_FVM_APPL_CONST) PduInfoPtr);

#if (CDD_FVM_MASTER_ECU_ENABLED == STD_ON) && (CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u)
FUNC(void, CDD_FVM_CODE) CDD_FVM_MainFunctionMaster(void);
#endif /* CDD_FVM_MASTER_ECU_ENABLED == STD_ON */

#if (CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON)
FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_SlaveRequestSyncMsg(uint16 syncMsgId);
#endif /* CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON && CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u*/

#endif /* CDD_FVM_H */
