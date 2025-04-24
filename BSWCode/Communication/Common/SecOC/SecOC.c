/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : SecOC.c                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* <VERSION>   <DATE>     <AUTHOR>   <REVISION LOG>
 * V1.0.0      20200818   HuRongbo   Initial version
 * V2.0.0      20200818   HuRongbo   Bug fix
 * V2.0.1      20220711   HuRongbo   QAC check issue fix
 * V2.0.2      20220921   HuRongbo
 *   1> Delete Det check in SecOC_MainFunctionTx/Rx() (JIRA:PRD0032020-219)
 * V2.0.3      20221229   HuRongbo
 *   1> Resolves the issue of compile error when SECOC_MAX_MSG_LINK_LEN is 0
 *      when the compiler does not support GNU extension-zero length array
 * V2.0.4      20230620   Jian.Jiang  Solve the problem of recycling wrong configuration
 *                                    values in the functions SecOC_CopyTxData and SecOC_TpTxConfirmation
 * V2.0.5      20230712   Jian.Jiang  Solve the problem when delivering discovered SecOC using TP.
 * V2.1.0      20230718   Jian.jiang  CP2.1 Release Version.
 * V2.1.1      20230926   Jian.Jiang  Add macro definition control to reference the interface function of CSM module
 * V2.1.2      20231027   Jian.jiang  Solve the problem that when building and sending secure I-PDU fails, the upper
 * layer should be notified of the building failure.
 * V2.1.3      20231101   Jian.Jiang  An error occurred when modifying the SecOC parsing truncation freshness value
 * V2.1.4      20231121   Jian.Jiang  Optimize code and resolve some compilation warning issues
 * V2.1.4      20231127   Jian.Jiang  QAC check issue fix
 * V2.1.5      20240110   Jian.Jiang  Solve the problem that the function of SecOCIgnoreVerificationResult is invalid
 * after opening it
 * V2.1.6      20240201   Jian.Jiang  Modify the configuration parameter SecOCIgnoreVerificationResult generation rule
 * to variable
 * V2.1.7      20240408    Jian.Jiang  Solving the problem of unused bits not being set to zero
 * V2.1.8      20240410    Jian.Jiang  Solve the problem of sending out security messages with less data
 * V2.1.9      20240412    Jian.Jiang  Solve the problem of abnormal clearing of unused bits when sending.
 * V2.1.10     20240524    Jian.Jiang  Increase in exclusive areas
 * V2.1.11     20240813    Jian.Jiang  QAC check issue fix
 * V2.1.12     20240820    Jian.Jiang  Resolves the header Det contain exception problem
 * V2.1.13     20240826    Jian.Jiang  It solves the problem of data storage error of encrypted sent message
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:SecOC<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code.

    \li PRQA S 3219 MISRA Rule 2.1 .<br>
    Reason:Configuration file design implementation needs.

    \li PRQA S 3408 MISRA Rule 8.4 .<br>
    Reason:Configuration design needs.

    \li PRQA S 2995 MISRA Rule 2.2 .<br>
    Reason:Configuration file design implementation needs.

    \li PRQA S 2991 MISRA Rule 14.3 .<br>
    Reason:Configuration file design implementation needs.

    \li PRQA S 3673 MISRA Rule 8.13 .<br>
    Reason:Configuration file design implementation needs.This QAC error is reported when a function parameter is not
  used.

 */
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

#define SECOC_C_AR_MAJOR_VERSION 4
#define SECOC_C_AR_MINOR_VERSION 5
#define SECOC_C_AR_PATCH_VERSION 0
#define SECOC_C_SW_MAJOR_VERSION 2
#define SECOC_C_SW_MINOR_VERSION 1
#define SECOC_C_SW_PATCH_VERSION 13

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "SecOC_Internal.h"
#include "PduR_SecOC.h"
#include "Det.h"
#include "Csm.h"
#include "istd_lib.h"
/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (SECOC_C_AR_MAJOR_VERSION != SECOC_H_AR_MAJOR_VERSION)
#error "SecOC.c : Mismatch in Specification Major Version"
#endif
#if (SECOC_C_AR_MINOR_VERSION != SECOC_H_AR_MINOR_VERSION)
#error "SecOC.c : Mismatch in Specification _MINOR Version"
#endif
#if (SECOC_C_AR_PATCH_VERSION != SECOC_H_AR_PATCH_VERSION)
#error "SecOC.c : Mismatch in Specification PATCH Version"
#endif
#if (SECOC_C_SW_MAJOR_VERSION != SECOC_H_SW_MAJOR_VERSION)
#error "SecOC.c : Mismatch in Software Major Version"
#endif
#if (SECOC_C_SW_MINOR_VERSION != SECOC_H_SW_MINOR_VERSION)
#error "SecOC.c : Mismatch in Software MINOR Version"
#endif
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define SECOC_MIN(a, b) (((a) < (b)) ? (a) : (b)) /* PRQA S 3472 */ /* MISRA Dir 4.9 */

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define SECOC_START_SEC_CODE
#include "SecOC_MemMap.h"

#if (SECOC_TX_PDU_NUM > 0u)
static FUNC(void, SECOC_CODE) SecOC_TxGenerateAuthenticator(uint16 index);

static FUNC(uint32, SECOC_CODE) SecOC_TxCalcDataToAuthenticatorLength(uint16 index);

static FUNC(Std_ReturnType, SECOC_CODE) SecOC_TxConstructDataToAuthenticator(uint16 index);

static FUNC(void, SECOC_CODE) SecOC_TxGenerateAuthenticatorRetry(uint16 index);

static FUNC(void, SECOC_CODE) SecOC_TxBuildAuthenticatorFailHandle(uint16 index);

static FUNC(void, SECOC_CODE) SecOC_TxSendSecuredIPDU(uint16 index);

static FUNC(void, SECOC_CODE) SecOC_TxConstructSecuredIPDU(uint16 index);

static FUNC(void, SECOC_CODE) SecOC_TxConstructAuthenticIPDU(uint16 index);

static FUNC(void, SECOC_CODE) SecOC_TxConstructCryptographicIPDU(uint16 index);

static FUNC(void, SECOC_CODE) SecOC_TxBroadcastSecuredIPDU(uint16 index);

static FUNC(void, SECOC_CODE) SecOC_TxBroadcastAuthenticIPDU(uint16 index);

static FUNC(void, SECOC_CODE) SecOC_TxBroadcastCryptographicIPDU(uint16 index);

static FUNC(void, SECOC_CODE) SECOC_SET_TX_EVENT(uint16 i, uint16 e);

static FUNC(void, SECOC_CODE) SECOC_CLR_TX_EVENT(uint16 i, uint16 e);

static FUNC(boolean, SECOC_CODE) SECOC_IS_TX_EVENT(uint16 i, uint16 e);

static P2CONST(SecOC_TxPduProcessingType, TYPEDEF, SECOC_CONST) SecOC_GetTxPduProcessingPtr(uint16 idx);

static P2CONST(SecOC_TxAuthenticPduLayerType, AUTOMATIC, SECOC_CONST) SecOC_GetTxAuthenticPduLayerPtr(uint16 idx);

static P2CONST(SecOC_TxSecuredPduLayerType, TYPEDEF, SECOC_CONST) SecOC_GetTxSecuredPduLayerPtr(uint16 idx);

static P2CONST(SecOC_TxSecuredPduType, AUTOMATIC, SECOC_CONST) SecOC_GetTxSecuredPduPtr(uint16 idx);

static P2CONST(SecOC_TxSecuredPduCollectionType, TYPEDEF, SECOC_CONST) SecOC_GetTxSecuredPduCollectionPtr(uint16 idx);

static P2CONST(SecOC_TxAuthenticPduType, AUTOMATIC, SECOC_CONST) SecOC_GetTxAuthenticPduPtr(uint16 idx);

static P2CONST(SecOC_TxCryptographicPduType, AUTOMATIC, SECOC_CONST) SecOC_GetTxCryptographicPduPtr(uint16 idx);

static P2CONST(SecOC_UseMessageLinkType, AUTOMATIC, SECOC_CONST) SecOC_GetTxUseMessageLinkPtr(uint16 idx);

#if (SECOC_TX_TP_PDU_NUM > 0u)
static FUNC(void, SECOC_CODE) SecOC_TxTpAcquireData(uint16 index);
#endif

static FUNC(void, SECOC_CODE) SecOC_TxChannelReset(uint16 index);
#endif /* SECOC_TX_PDU_NUM > 0u */

#if (SECOC_RX_PDU_NUM > 0u)
static FUNC(void, SECOC_CODE) SECOC_SET_RX_EVENT(uint16 i, uint16 e);

static FUNC(void, SECOC_CODE) SECOC_CLR_RX_EVENT(uint16 i, uint16 e);

static FUNC(boolean, SECOC_CODE) SECOC_IS_RX_EVENT(uint16 i, uint16 e);

static P2CONST(SecOC_RxPduProcessingType, TYPEDEF, SECOC_CONST) SecOC_GetRxPduProcessingPtr(uint16 idx);

static P2CONST(SecOC_RxAuthenticPduLayerType, AUTOMATIC, SECOC_APPL_DATA) SecOC_GetRxAuthenticPduLayerPtr(uint16 idx);

static P2CONST(SecOC_RxSecuredPduLayerType, TYPEDEF, SECOC_CONST) SecOC_GetRxSecuredPduLayerPtr(uint16 idx);

static P2CONST(SecOC_RxSecuredPduType, AUTOMATIC, SECOC_CONST) SecOC_GetRxSecuredPduPtr(uint16 idx);

static P2CONST(SecOC_RxSecuredPduCollectionType, AUTOMATIC, SECOC_CONST) SecOC_GetRxSecuredPduCollectionPtr(uint16 idx);

static P2CONST(SecOC_RxAuthenticPduType, AUTOMATIC, SECOC_CONST) SecOC_GetRxAuthenticPduPtr(uint16 idx);

static P2CONST(SecOC_RxCryptographicPduType, AUTOMATIC, SECOC_CONST) SecOC_GetRxCryptographicPduPtr(uint16 idx);

#if (SECOC_RX_IF_PDU_NUM > 0u)
static FUNC(void, SECOC_CODE) SecOC_RxIfSecuredPduIndication(
    uint16 index,
    PduIdType RxPduId,
    P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) PduInfoPtr);

static FUNC(void, SECOC_CODE)
    SecOC_RxIfAuthenticPduIndication(uint16 index, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) PduInfoPtr);

static FUNC(void, SECOC_CODE)
    SecOC_RxIfCryptographicPduIndication(uint16 index, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) PduInfoPtr);
#endif /* SECOC_RX_IF_PDU_NUM > 0u */

static FUNC(void, SECOC_CODE) SecOC_RxParseSecuredIpdu(uint16 index);

static FUNC(void, SECOC_CODE) SecOC_RxVerifyProcee(uint16 index);

static FUNC(void, SECOC_CODE) SecOC_RxVerifyAuthenticationInfo(uint16 index);

static FUNC(uint32, SECOC_CODE) SecOC_RxCalcDataToAuthenticatorLength(uint16 index);

static FUNC(Std_ReturnType, SECOC_CODE) SecOC_RxConstructDataToAuthenticator(uint16 index);

static FUNC(void, SECOC_CODE) SecOC_RxAuthenticationBuildRetry(uint16 index);

static FUNC(void, SECOC_CODE) SecOC_RxVerifyAuthenticInfoRetry(uint16 index);

#if (SECOC_MAX_MSG_LINK_LEN > 0u)
static FUNC(Std_ReturnType, SECOC_CODE) SecOC_RxMessageLinkCheck(uint16 index);
static P2CONST(SecOC_UseMessageLinkType, AUTOMATIC, SECOC_CONST) SecOC_GetRxUseMessageLinkPtr(uint16 idx);
#endif /* SECOC_MAX_MSG_LINK_LEN > 0u */

static FUNC(void, SECOC_CODE) SecOC_RxPassAuthenticPduToUpperLayer(uint16 index);

#if (SECOC_RX_TP_PDU_NUM > 0u)
static FUNC(void, SECOC_CODE) SecOC_RxTransTpDataToUpperLayer(uint16 index);

static FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_RxTpSecuredPduStartOfReception(
    uint16 index,
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) bufferSizePtr /* PRQA S 3432 */ /* MISRA Rule 20.7 */
);

static FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_RxTpAuthenticPduStartOfReception(
    uint16 index,
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) bufferSizePtr /* PRQA S 3432 */ /* MISRA Rule 20.7 */
);

static FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_RxTpCryptographicPduStartOfReception(
    uint16 index,
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) bufferSizePtr /* PRQA S 3432 */ /* MISRA Rule 20.7 */
);
#endif /* SECOC_RX_TP_PDU_NUM > 0u */

#if (SECOC_RX_IF_PDU_NUM > 0u)
static FUNC(void, SECOC_CODE) SecOC_RxLoadQueuedPdu(uint16 index);
#endif

static FUNC(void, SECOC_CODE) SecOC_RxReportVerifyStatus(uint16 index);

static FUNC(void, SECOC_CODE) SecOC_RxChannelReset(uint16 index);

static FUNC(PduLengthType, SECOC_CODE)
    SecOC_RxGetPduLength(uint8 headerLength, P2CONST(uint8, AUTOMATIC, SECOC_APPL_CONST) headerPtr);
#endif /* SECOC_RX_PDU_NUM > 0u */

static FUNC(void, SECOC_CODE) SecOC_EndianSwap(
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) dataPtr,
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
    uint8 size);

static FUNC(void, SECOC_CODE) SecOC_ArrayShiftLeft(
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) numArr,
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
    uint16 numLen,
    uint16 shiftBits);

static FUNC(void, SECOC_CODE) SecOC_ArrayShiftRight(
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) numArr,
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
    uint16 numLen,
    uint16 shiftBits);

static FUNC(void, SECOC_CODE) SecOC_ArrayOrOp(
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) numArr1,
    P2CONST(uint8, AUTOMATIC, SECOC_CONST) numArr2,
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
    uint16 numLen);

static FUNC(void, SECOC_CODE) SecOC_ClearLowBit(
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) numArr,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    uint16 numLen,
    uint16 shiftBits);

static FUNC(void, SECOC_CODE)
    SecOC_DataInQueue(uint16 index, PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) PduInfoPtr);

static FUNC(Std_ReturnType, SECOC_CODE) SecOC_DataOutQueue(
    uint16 index,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(PduIdType, AUTOMATIC, SECOC_APPL_DATA) RxPduIdPtr,
    P2VAR(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) PduInfoPtr
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
);

#if (STD_ON == SECOC_DEV_ERROR_DETECT)
static inline void SECOC_DET_REPORT(uint8 ApiId, uint8 ErrorId);
static inline void SECOC_DET_REPORT_RUNTIMEERROR(uint8 ApiId, uint8 ErrorId);
#else
#define SECOC_DET_REPORT(ApiId, ErrorId)
#define SECOC_DET_REPORT_RUNTIMEERROR(ApiId, ErrorId)
#endif

#define SECOC_STOP_SEC_CODE
#include "SecOC_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define SECOC_START_SEC_VAR_INIT_UNSPECIFIED
#include "SecOC_MemMap.h"
/* Variable store SECOC initialization status */
static VAR(SecOC_StateType, SECOC_VAR_POWER_ON_INIT) SecOC_InitStatus = SECOC_UNINIT;
#define SECOC_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_VAR_INIT_PTR
#include "SecOC_MemMap.h"
/* Pointer to configuration data */
static P2CONST(SecOC_ConfigType, SECOC_VAR, SECOC_CONST) SecOC_ConfigPtr = NULL_PTR;
#define SECOC_STOP_SEC_VAR_INIT_PTR
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "SecOC_MemMap.h"
static uint8 SecOC_DataToAuthenticator[SECOC_DATA_TO_AUTHENTICATOR_LENGTH];

#if (SECOC_RX_PDU_NUM > 0)
static uint16 SecOC_RxEvent[SECOC_RX_PDU_NUM];
static SecOC_RxPduRTType SecOC_RxRtData[SECOC_RX_PDU_NUM];
#endif
#if (SECOC_TX_PDU_NUM > 0)
static uint16 SecOC_TxEvent[SECOC_TX_PDU_NUM];
static SecOC_TxPduRTType SecOC_TxRtData[SECOC_TX_PDU_NUM];
#endif
#define SECOC_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "SecOC_MemMap.h"

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define SECOC_START_SEC_CODE
#include "SecOC_MemMap.h"
/******************************************************************************/
/*
 * Brief               Initializes the the SecOC module.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      config: Pointer to a selected configuration structure
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SECOC_CODE)
SecOC_Init(P2CONST(SecOC_ConfigType, AUTOMATIC, SECOC_CFG_CONST) config)
{
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (NULL_PTR == config)
    {
        SECOC_DET_REPORT(SECOC_INIT_ID, SECOC_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint16 index;

        /*Save the configuration pointer */
        SecOC_ConfigPtr = config;

#if (SECOC_TX_PDU_NUM > 0u)
        for (index = 0; index < SECOC_TX_PDU_NUM; index++)
        {
            SecOC_TxChannelReset(index);
        }
#endif /* SECOC_TX_PDU_NUM > 0u */

#if (SECOC_RX_PDU_NUM > 0u)
        for (index = 0; index < SECOC_RX_PDU_NUM; index++)
        {
            SecOC_RxChannelReset(index);
            SecOC_RxRtData[index].rxOverrideStatus = SECOC_OVERRIDE_CANCEL;
            SecOC_RxRtData[index].rxNumOfMsgToOverride = 0u;
            SecOC_RxRtData[index].rxBakNumOfMsgToOverride = 0u;
        }
#endif /* SECOC_RX_PDU_NUM > 0u */

        /*Set the Init status */
        SecOC_InitStatus = SECOC_INIT;
    }
}

/******************************************************************************/
/*
 * Brief               This service stops the secure onboard communication.
 *                     All buffered I-PDU are removed and have to be obtained
 *                     again, if needed, after SecOC_Init has been called.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SECOC_CODE) SecOC_DeInit(void)
{
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_DEINIT_ID, SECOC_E_UNINIT);
    }
    else
#endif
    {
        uint16 index;

#if (SECOC_TX_PDU_NUM > 0u)
        for (index = 0; index < SECOC_TX_PDU_NUM; index++)
        {
            SecOC_TxChannelReset(index);
        }
#endif /* SECOC_TX_PDU_NUM > 0u */

#if (SECOC_RX_PDU_NUM > 0u)
        for (index = 0; index < SECOC_RX_PDU_NUM; index++)
        {
            SecOC_RxChannelReset(index);
            SecOC_RxRtData[index].rxOverrideStatus = SECOC_OVERRIDE_CANCEL;
            SecOC_RxRtData[index].rxNumOfMsgToOverride = 0u;
            SecOC_RxRtData[index].rxBakNumOfMsgToOverride = 0u;
        }
#endif /* SECOC_RX_PDU_NUM > 0u */

        /*Set the Init status */
        SecOC_InitStatus = SECOC_UNINIT;
    }
}

/******************************************************************************/
/*
 * Brief               Returns the version information of this module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     versioninfo: Pointer to where to store the version
 *                                  information of this module.
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SECOC_CODE)
SecOC_GetVersionInfo(
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(Std_VersionInfoType, AUTOMATIC, SECOC_APPL_DATA) versioninfo
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
)
{
#if (SECOC_DEV_ERROR_DETECT == STD_ON)
    if (versioninfo == NULL_PTR)
    {
        SECOC_DET_REPORT(SECOC_GETVERSIONINFO_ID, SECOC_E_PARAM_POINTER);
    }
    else
#endif
    {
        versioninfo->vendorID = SECOC_VENDOR_ID;
        versioninfo->moduleID = SECOC_MODULE_ID;
        versioninfo->sw_major_version = SECOC_C_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = SECOC_C_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = SECOC_C_SW_PATCH_VERSION;
    }
}

/******************************************************************************/
/*
 * Brief               Requests transmission of a PDU.
 * ServiceId           0x49
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds.
 * Param-Name[in]      TxPduId: Identifier of the PDU to be transmitted.
 *                     PduInfoPtr: Length of and pointer to the PDU data and
 *                                 pointer to MetaData.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Transmit request has been accepted.
 *                     E_NOT_OK: Transmit request has not been accepted.
 */
/******************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE)
SecOC_IfTransmit(PduIdType TxPduId, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) PduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*not initialized*/
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_IFTRANSMIT_ID, SECOC_E_UNINIT);
        detNoErr = FALSE;
    }
    /*TxPduId error*/
    if (TxPduId >= SECOC_TX_IF_PDU_NUM)
    {
        SECOC_DET_REPORT(SECOC_IFTRANSMIT_ID, SECOC_E_INVALID_PDU_SDU_ID);
        detNoErr = FALSE;
    }
    /*NULL POINTER*/
    if (PduInfoPtr == NULL_PTR)
    {
        SECOC_DET_REPORT(SECOC_IFTRANSMIT_ID, SECOC_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_TX_IF_PDU_NUM > 0u)
        P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
        txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(TxPduId);
        P2CONST(SecOC_SameBufferPduCollectionType, AUTOMATIC, SECOC_CONST)
        sameBufferPtr = txPduProcessingPtr->SecOCSameBufferPduRef;

        /* Not use sameBufferCollection */
        if ((NULL_PTR == sameBufferPtr) && (txPduProcessingPtr->SecOCAuthenticPduBuffLength >= PduInfoPtr->SduLength))
        {
            (void)ILib_memcpy(
                txPduProcessingPtr->SecOCAuthenticPduBufferRef,
                PduInfoPtr->SduDataPtr,
                PduInfoPtr->SduLength);
            result = E_OK;
        }
        else if (
            (NULL_PTR != sameBufferPtr) && (FALSE == sameBufferPtr->SecOCSameBufferInUse[0u])
            && (sameBufferPtr->SecOCAuthenticPduBuffLength >= PduInfoPtr->SduLength))
        {
            (void)ILib_memcpy(sameBufferPtr->SecOCAuthenticPduBufferRef, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
            sameBufferPtr->SecOCSameBufferInUse[0u] = TRUE;

            result = E_OK;
        }
        else
        {
            result = E_NOT_OK;
        }

        if (E_OK == result)
        {
            SchM_Enter_SecOC_TxData();
            SecOC_TxRtData[TxPduId].txPduStatus = SECOC_TX_CALC;
            SECOC_SET_TX_EVENT(TxPduId, SECOC_TX_REQUEST | SECOC_TX_DATA_ACQUIRED);
            SecOC_TxRtData[TxPduId].txPduLength = PduInfoPtr->SduLength;
            /* @req SWS_SecOC_00225,SWS_SecOC_00226 */
            SecOC_TxRtData[TxPduId].txAuthBuildCnt = 0u;
            SchM_Exit_SecOC_TxData();
        }
#endif /* SECOC_TX_IF_PDU_NUM > 0u */
    }
    return result;
}

/******************************************************************************/
/*
 * Brief               Requests transmission of a PDU.
 * ServiceId           0x49
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds.
 * Param-Name[in]      TxPduId: Identifier of the PDU to be transmitted.
 *                     PduInfoPtr: Length of and pointer to the PDU data and
 *                                 pointer to MetaData.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Transmit request has been accepted.
 *                     E_NOT_OK: Transmit request has not been accepted.
 */
/******************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE)
SecOC_TpTransmit(PduIdType TxPduId, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) PduInfoPtr)
{
    Std_ReturnType result = E_OK;

#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*not initialized*/
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_TPTRANSMIT_ID, SECOC_E_UNINIT);
        detNoErr = FALSE;
    }
    /*TxPduId error*/
    if (!((TxPduId >= SECOC_TX_IF_PDU_NUM) && (TxPduId < SECOC_TX_PDU_NUM)))
    {
        SECOC_DET_REPORT(SECOC_TPTRANSMIT_ID, SECOC_E_INVALID_PDU_SDU_ID);
        detNoErr = FALSE;
    }
    /*NULL POINTER*/
    if (PduInfoPtr == NULL_PTR)
    {
        SECOC_DET_REPORT(SECOC_TPTRANSMIT_ID, SECOC_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_TX_TP_PDU_NUM > 0u)
        P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
        txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(TxPduId);
        P2CONST(SecOC_TxPduSecuredAreaType, TYPEDEF, SECOC_CONST)
        SecOCTxPduSecuredArea = txPduProcessingPtr->SecOCTxPduSecuredArea;
        uint32 txPduSecuredAreaLength;
        if (NULL_PTR != SecOCTxPduSecuredArea)
        {
            /* @req SWS_SecOC_00313 */
            txPduSecuredAreaLength =
                SecOCTxPduSecuredArea->SecOCSecuredTxPduLength + SecOCTxPduSecuredArea->SecOCSecuredTxPduOffset;
            if (txPduSecuredAreaLength > PduInfoPtr->SduLength)
            {
                result = E_NOT_OK;
            }
        }

        if (result == E_OK)
        {
            SchM_Enter_SecOC_TxData();
            SecOC_TxRtData[TxPduId].txPduStatus = SECOC_TX_CALC;
            SECOC_SET_TX_EVENT(TxPduId, SECOC_TX_REQUEST);
            SecOC_TxRtData[TxPduId].txPduLength = PduInfoPtr->SduLength;
            /* @req SWS_SecOC_00225,SWS_SecOC_00226 */
            SecOC_TxRtData[TxPduId].txAuthBuildCnt = 0u;
            SchM_Exit_SecOC_TxData();
        }
#else
        (void)(TxPduId);
        (void)(PduInfoPtr);
#endif /* SECOC_TX_TP_PDU_NUM > 0u */
    }
    return result;
}

/******************************************************************************/
/*
 * Brief               Requests cancellation of an ongoing transmission of a PDU
 *                     in a lower layer communication module.
 * ServiceId           0x4a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds.
 * Param-Name[in]      TxPduId: Identification of the PDU to be cancelled.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Cancellation was executed successfully by the
 *                           destination module.
 *                     E_NOT_OK: Cancellation was rejected by the destination
 *                               module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE)
SecOC_IfCancelTransmit(PduIdType TxPduId)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*not initialized*/
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_IFCANCELTRANSMIT_ID, SECOC_E_UNINIT);
        detNoErr = FALSE;
    }
    /*id error*/
    if (TxPduId >= SECOC_TX_IF_PDU_NUM)
    {
        SECOC_DET_REPORT(SECOC_IFCANCELTRANSMIT_ID, SECOC_E_INVALID_PDU_SDU_ID);
        detNoErr = FALSE;
    }

    if (TRUE == detNoErr)
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_TX_IF_PDU_NUM > 0u)
        P2CONST(SecOC_TxSecuredPduType, AUTOMATIC, SECOC_CONST)
        txSecuredPduPtr = SecOC_GetTxSecuredPduPtr(TxPduId);
        P2CONST(SecOC_TxAuthenticPduType, AUTOMATIC, SECOC_CONST)
        txAuthenticPduPtr;
        P2CONST(SecOC_TxCryptographicPduType, AUTOMATIC, SECOC_CONST)
        txCryptographicPduPtr;
        Std_ReturnType result2 = E_OK;

        if (NULL_PTR != txSecuredPduPtr)
        {
            result = PduR_SecOCCancelTransmit(txSecuredPduPtr->SecOCTxPduRAsLowerLayerId);
        }
        else
        {
            txAuthenticPduPtr = SecOC_GetTxAuthenticPduPtr(TxPduId);
            txCryptographicPduPtr = SecOC_GetTxCryptographicPduPtr(TxPduId);
            result = PduR_SecOCCancelTransmit(txAuthenticPduPtr->SecOCTxPduRAsLowerLayerId);
            result2 = PduR_SecOCCancelTransmit(txCryptographicPduPtr->SecOCTxPduRAsLowerLayerId);
        }

        if ((E_OK == result) && (E_OK == result2))
        {
            SecOC_TxChannelReset(TxPduId);
        }
        else
        {
            /* Return E_NOT_OK */
        }
#endif
    }

    return result;
}

/******************************************************************************/
/*
 * Brief               Requests cancellation of an ongoing transmission of a PDU
 *                     in a lower layer communication module.
 * ServiceId           0x4a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds.
 * Param-Name[in]      TxPduId: Identification of the PDU to be cancelled.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Cancellation was executed successfully by the
 *                           destination module.
 *                     E_NOT_OK: Cancellation was rejected by the destination
 *                               module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE)
SecOC_TpCancelTransmit(PduIdType TxPduId)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*not initialized*/
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_TPCANCELTRANSMIT_ID, SECOC_E_UNINIT);
        detNoErr = FALSE;
    }
    /*id error*/
    if ((TxPduId < SECOC_TX_IF_PDU_NUM) || (TxPduId >= SECOC_TX_PDU_NUM))
    {
        SECOC_DET_REPORT(SECOC_TPCANCELTRANSMIT_ID, SECOC_E_INVALID_PDU_SDU_ID);
        detNoErr = FALSE;
    }

    if (TRUE == detNoErr)
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_TX_TP_PDU_NUM > 0u)
        P2CONST(SecOC_TxSecuredPduType, AUTOMATIC, SECOC_CONST)
        txSecuredPduPtr = SecOC_GetTxSecuredPduPtr(TxPduId);
        P2CONST(SecOC_TxAuthenticPduType, AUTOMATIC, SECOC_CONST)
        txAuthenticPduPtr;
        P2CONST(SecOC_TxCryptographicPduType, AUTOMATIC, SECOC_CONST)
        txCryptographicPduPtr;
        Std_ReturnType result2 = E_OK;

        if (NULL_PTR != txSecuredPduPtr)
        {
            result = PduR_SecOCCancelTransmit(txSecuredPduPtr->SecOCTxPduRAsLowerLayerId);
        }
        else
        {
            txAuthenticPduPtr = SecOC_GetTxAuthenticPduPtr(TxPduId);
            txCryptographicPduPtr = SecOC_GetTxCryptographicPduPtr(TxPduId);
            result = PduR_SecOCCancelTransmit(txAuthenticPduPtr->SecOCTxPduRAsLowerLayerId);
            result2 = PduR_SecOCCancelTransmit(txCryptographicPduPtr->SecOCTxPduRAsLowerLayerId);
        }

        if ((E_OK == result) && (E_OK == result2))
        {
            SecOC_TxChannelReset(TxPduId);
        }
        else
        {
            /* Return E_NOT_OK */
        }
#else
        (void)(TxPduId);
#endif
    }

    return result;
}

/******************************************************************************/
/*
 * Brief               Requests cancellation of an ongoing reception of a PDU in
 *                     a lower layer transport protocol module.
 * ServiceId           0x4c
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxPduId: Identification of the PDU to be cancelled.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Cancellation was executed successfully by the
 *                           destination module.
 *                     E_NOT_OK: Cancellation was rejected by the destination
 *                               module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE)
SecOC_TpCancelReceive(PduIdType RxPduId)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*not initialized*/
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_TPCANCELRECEIVE_ID, SECOC_E_UNINIT);
        detNoErr = FALSE;
    }
    /*id error*/
    if ((RxPduId < SECOC_RX_IF_PDU_NUM) || (RxPduId >= SECOC_RX_PDU_NUM))
    {
        SECOC_DET_REPORT(SECOC_TPCANCELRECEIVE_ID, SECOC_E_INVALID_PDU_SDU_ID);
        detNoErr = FALSE;
    }

    if (TRUE == detNoErr)
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_TX_TP_PDU_NUM > 0u)
        P2CONST(SecOC_RxSecuredPduType, AUTOMATIC, SECOC_CONST)
        rxSecuredPduPtr = SecOC_GetRxSecuredPduPtr(RxPduId);
        P2CONST(SecOC_RxAuthenticPduType, AUTOMATIC, SECOC_CONST)
        rxAuthenticPduPtr;
        P2CONST(SecOC_RxCryptographicPduType, AUTOMATIC, SECOC_CONST)
        rxCryptographicPduPtr;
        Std_ReturnType result2 = E_OK;

        if (NULL_PTR != rxSecuredPduPtr)
        {
            result = PduR_SecOCCancelReceive(rxSecuredPduPtr->SecOCRxPduRAsLowerLayerPduId);
        }
        else
        {
            rxAuthenticPduPtr = SecOC_GetRxAuthenticPduPtr(RxPduId);
            rxCryptographicPduPtr = SecOC_GetRxCryptographicPduPtr(RxPduId);
            result = PduR_SecOCCancelReceive(rxAuthenticPduPtr->SecOCRxPduRAsLowerLayerPduId);
            result2 = PduR_SecOCCancelReceive(rxCryptographicPduPtr->SecOCRxPduRAsLowerLayerPduId);
        }

        if ((E_OK == result) && (E_OK == result2))
        {
            SecOC_TxChannelReset(RxPduId);
        }
        else
        {
            /* Return E_NOT_OK */
        }
#else
        (void)(RxPduId);
#endif
    }

    return result;
}

/******************************************************************************/
/*
 * Brief               This service provides the ability to force specific
 *                     behaviour of SecOc: accept or drop an I-PDU with or
 *                     without performing the verification of authenticator or
 *                     independent of the authenticator verification result, and
 *                     force a specific result for SecOC_VerificationResultType
 *                     allowing additional fault handling in the application.
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different Freshness ValueIDs
 * Param-Name[in]      ValueID: ID of the Freshness Value used to control the
 *                     verification behaviour of all assigned Secured I-PDUs
 *                     according to the override Status.Or ValueID is the DataID
 *                     of a Secured I-PDU that shall be controlled by the
 *                     overrideStatus.
 *                     overrideStatus: Defines whether verification is executed
 *                     and whether the I-PDU is passed on, and for how long the
 *                     override is active.
 *                     numberOfMessagesToOverride: Number of sequential
 *                     verification to override when using a specific counter
 *                     for authentication verification.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: request successful
 *                     E_NOT_OK: request failed
 */
/******************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE)
SecOC_VerifyStatusOverride(uint16 ValueID, SecOC_OverrideStatusType overrideStatus, uint8 numberOfMessagesToOverride)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_VERIFYSTATUSOVERRIDE_ID, SECOC_E_UNINIT);
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
        P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
        rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(0u);
        P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
        rxRtPtr = &SecOC_RxRtData[0u];
        uint16 index;
        SchM_Enter_SecOC_RxData();
        for (index = 0u; index < SECOC_RX_PDU_NUM; index++)
        {
#if (STD_ON == SECOC_OVERRIDE_STATUS_WITH_DATA_ID)
            if (rxPduProcessingPtr->SecOCDataId == ValueID)
#else
            if (rxPduProcessingPtr->SecOCFreshnessValueId == ValueID)
#endif
            {
                if (((SECOC_OVERRIDE_DROP_UNTIL_NOTICE == overrideStatus)
                     || (SECOC_OVERRIDE_DROP_UNTIL_LIMIT == overrideStatus)
                     || (SECOC_OVERRIDE_CANCEL == overrideStatus))
                    || (STD_ON == SECOC_ENABLE_FORCED_PASS_OVERRIDE))
                {
                    rxRtPtr->rxOverrideStatus = overrideStatus;
                    rxRtPtr->rxNumOfMsgToOverride = numberOfMessagesToOverride;
                    ret = E_OK;
                }
            }

            rxPduProcessingPtr++;
            rxRtPtr++;
        }
        SchM_Exit_SecOC_RxData();
    }

    return ret;
}

#if defined(SECOC_DEFAULT_AUTHENTICATION_INFORMATION_PATTERN)
/******************************************************************************/
/*
 * Brief               The service provides the ability to enable the sending of
 *                     un-authenticated PDU to lower layer.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different FreshnessValueIDs.
 * Param-Name[in]      FreshnessValueID: ID of the Freshness Value for which
 *                     sending SecOCDefaultAuthenticationInformationPattern
 *                     should be enabled.
 *                     sendDefaultAuthenticationInformation: FALSE - sending
 *                     SecOCDefaultAuthenticationInformation Pattern shall be
 *                     disabled for given FreshnessValueID TRUE - sending
 *                     SecOCDefaultAuthenticationInformationPattern shall be
 *                     enabled for given FreshnessValueID
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: request successful
 *                     E_NOT_OK: request failed
 */
/******************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE)
SecOC_SendDefaultAuthenticationInformation(uint16 FreshnessValueID, boolean sendDefaultAuthenticationInformation)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_SENDDEFAULTAUTHENTICATIONINFORMATION_ID, SECOC_E_UNINIT);
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
        P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
        txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(0u);
        SchM_Enter_SecOC_TxData();
        P2VAR(SecOC_TxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
        txRtPtr = &SecOC_TxRtData[0u];
        uint16 index;

        for (index = 0u; index < SECOC_TX_PDU_NUM; index++)
        {
            if (txPduProcessingPtr->SecOCFreshnessValueId == FreshnessValueID)
            {
                txRtPtr->txDefaultAuthInfoEnable = sendDefaultAuthenticationInformation;
                ret = E_OK;
            }
            txPduProcessingPtr++;
            txRtPtr++;
        }
        SchM_Exit_SecOC_TxData();
    }

    return ret;
}
#endif
#define SECOC_STOP_SEC_CODE
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_SECOCRXINDICATION_CALLBACK_CODE
#include "SecOC_MemMap.h"
/******************************************************************************/
/*
 * Brief               Indication of a received PDU from a lower layer
 *                     communication interface module.
 * ServiceId           0x42
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds.
 * Param-Name[in]      RxPduId: ID of the received PDU.
 *                     PduInfoPtr: Contains the length (SduLength) of the
 *                     received PDU,a pointer to a buffer (SduDataPtr)containing
 *                     the PDU, and the MetaData related to this PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SECOC_CODE)
SecOC_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) PduInfoPtr)
{
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_RXINDICATION_ID, SECOC_E_UNINIT);
        detNoErr = FALSE;
    }
    if (RxPduId >= SECOC_RX_IF_PDU_NUM)
    {
        SECOC_DET_REPORT(SECOC_RXINDICATION_ID, SECOC_E_INVALID_PDU_SDU_ID);
        detNoErr = FALSE;
    }
    if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        SECOC_DET_REPORT(SECOC_RXINDICATION_ID, SECOC_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_RX_IF_PDU_NUM > 0u)
        P2CONST(SecOC_RxSecuredPduType, AUTOMATIC, SECOC_CONST) rxSecuredPduPtr;
        P2CONST(SecOC_RxAuthenticPduType, AUTOMATIC, SECOC_CONST)
        rxAuthenticPduPtr;
        P2CONST(SecOC_RxCryptographicPduType, AUTOMATIC, SECOC_CONST)
        rxCryptographicPduPtr;
        uint16 index;
        boolean rxPduTag = FALSE;

        for (index = 0u; index < SECOC_RX_IF_PDU_NUM; index++)
        {
            rxSecuredPduPtr = SecOC_GetRxSecuredPduPtr(index);
            if (NULL_PTR != rxSecuredPduPtr)
            {
                if (rxSecuredPduPtr->SecOCRxSecuredLayerPduId == RxPduId)
                {
                    SecOC_RxIfSecuredPduIndication(index, RxPduId, PduInfoPtr);
                    rxPduTag = TRUE;
                }
            }
            else
            {
                rxAuthenticPduPtr = SecOC_GetRxAuthenticPduPtr(index);
                if (rxAuthenticPduPtr->SecOCRxAuthenticPduId == RxPduId)
                {
                    SecOC_RxIfAuthenticPduIndication(index, PduInfoPtr);
                    rxPduTag = TRUE;
                }

                rxCryptographicPduPtr = SecOC_GetRxCryptographicPduPtr(index);
                if (rxCryptographicPduPtr->SecOCRxCryptographicPduId == RxPduId)
                {
                    SecOC_RxIfCryptographicPduIndication(index, PduInfoPtr);
                    rxPduTag = TRUE;
                }
            }
            if (TRUE == rxPduTag)
            {
                break;
            }
        }
#endif /* SECOC_RX_IF_PDU_NUM > 0u */
    }
}
#define SECOC_STOP_SEC_SECOCRXINDICATION_CALLBACK_CODE
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_SECOCTPRXINDICATION_CALLBACK_CODE
#include "SecOC_MemMap.h"
/******************************************************************************/
/*
 * Brief               Indication of a received PDU from a lower layer
 *                     communication interface module.
 * ServiceId           0x45
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the received I-PDU.
 *                     result: Result of the reception.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SECOC_CODE)
SecOC_TpRxIndication(PduIdType id, Std_ReturnType result)
{
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*not initialized*/
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_TPRXINDICATION_ID, SECOC_E_UNINIT);
        detNoErr = FALSE;
    }
    /*id error*/
    if ((id >= SECOC_RX_PDU_NUM) || (id < SECOC_RX_IF_PDU_NUM))
    {
        SECOC_DET_REPORT(SECOC_TPRXINDICATION_ID, SECOC_E_INVALID_PDU_SDU_ID);
        detNoErr = FALSE;
    }

    if (detNoErr == TRUE)
#endif
    {
#if (SECOC_RX_TP_PDU_NUM > 0u)

        SchM_Enter_SecOC_RxData();
        P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
        rxRtPtr = &SecOC_RxRtData[SECOC_RX_IF_PDU_NUM];
        uint16 index;

        for (index = SECOC_RX_IF_PDU_NUM; index < SECOC_RX_PDU_NUM; index++)
        {
            if (rxRtPtr->curPduId == id)
            {
                break;
            }

            rxRtPtr++;
        }
        SchM_Exit_SecOC_RxData();

        if ((index < SECOC_RX_PDU_NUM) && (SECOC_IS_RX_EVENT(index, SECOC_RX_TP_DATA_FROM_LOWER))
            && (0u == rxRtPtr->sduRemaining))
        {
            if (E_OK == result)
            {
                SchM_Enter_SecOC_RxData();
                /* @req SWS_SecOC_00085 */
                SECOC_CLR_RX_EVENT(index, SECOC_RX_TP_DATA_FROM_LOWER);
                SECOC_SET_RX_EVENT(index, SECOC_RX_INDICATION);
                rxRtPtr->rxPduStatus = SECOC_RX_CALC;
                SchM_Exit_SecOC_RxData();
            }
            else
            {
                /* @req SWS_SecOC_00089 */
                /* Error occur,Drop the Secured I-PDU and free all buffers */
                SecOC_RxChannelReset(index);
            }
        }
        else
        {
            /* @req SWS_SecOC_00084 */
            /* Reception of a Secured I-PDU not completed,Directly return */
        }
#else
        (void)(id);
        (void)(result);
#endif
    }
}
#define SECOC_STOP_SEC_SECOCTPRXINDICATION_CALLBACK_CODE
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_SECOCTXCONFIRMATION_CALLBACK_CODE
#include "SecOC_MemMap.h"
/******************************************************************************/
/*
 * Brief               The lower layer communication interface module confirms
 *                     the transmission of a PDU,or the failure transmit a PDU.
 * ServiceId           0x40
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds.
 * Param-Name[in]      TxPduId: ID of the PDU that has been transmitted.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SECOC_CODE)
SecOC_TxConfirmation(PduIdType TxPduId)
{

#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*not initialized*/
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_TXCONFIRMATION_ID, SECOC_E_UNINIT);
        detNoErr = FALSE;
    }
    /*id error*/
    if (TxPduId >= SECOC_TX_IF_PDU_NUM)
    {
        SECOC_DET_REPORT(SECOC_TXCONFIRMATION_ID, SECOC_E_INVALID_PDU_SDU_ID);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_TX_IF_PDU_NUM > 0u)
        P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
        txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(0u);
        P2CONST(SecOC_TxSecuredPduType, AUTOMATIC, SECOC_CONST) txSecuredPduPtr;
        P2CONST(SecOC_TxAuthenticPduType, AUTOMATIC, SECOC_CONST)
        txAuthenticPduPtr;
        P2CONST(SecOC_TxCryptographicPduType, AUTOMATIC, SECOC_CONST)
        txCryptographicPduPtr;
        P2VAR(SecOC_TxPduRTType, AUTOMATIC, SECOC_APPL_DATA) txRtPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
        PduIdType upLayerPduId;
        uint16 index;
        boolean txPduTag = FALSE;
        SchM_Enter_SecOC_TxData();
        for (index = 0u; index < SECOC_TX_IF_PDU_NUM; index++)
        {
            txSecuredPduPtr = SecOC_GetTxSecuredPduPtr(index);
            if (NULL_PTR != txSecuredPduPtr)
            {
                if (txSecuredPduPtr->SecOCTxSecuredLayerPduId == TxPduId)
                {
                    txPduTag = TRUE;
                }
            }
            else
            {
                txAuthenticPduPtr = SecOC_GetTxAuthenticPduPtr(index);
                if (txAuthenticPduPtr->SecOCTxAuthenticPduId == TxPduId)
                {
                    SECOC_SET_TX_EVENT(index, SECOC_TX_AUTHENTIC_PDU_CONFIRMED);
                    txPduTag = TRUE;
                }

                txCryptographicPduPtr = SecOC_GetTxCryptographicPduPtr(index);
                if (txCryptographicPduPtr->SecOCTxCryptographicPduId == TxPduId)
                {
                    SECOC_SET_TX_EVENT(index, SECOC_TX_CRYPTOGRAPHIC_PDU_CONFIRMED);
                    txPduTag = TRUE;
                }
            }
            if (TRUE == txPduTag)
            {
                break;
            }

            txPduProcessingPtr++;
        }

        if (index < SECOC_TX_IF_PDU_NUM)
        {
            /* @req SWS_SecOC_00063 */
            upLayerPduId = txPduProcessingPtr->SecOCTxAuthenticPduLayer->SecOCTxPduRAsUpLayerId;
            if ((NULL_PTR != txSecuredPduPtr)
                || (SECOC_IS_TX_EVENT(index, SECOC_TX_AUTHENTIC_PDU_CONFIRMED)
                    && SECOC_IS_TX_EVENT(index, SECOC_TX_CRYPTOGRAPHIC_PDU_CONFIRMED)))
            {
                PduR_SecOCIfTxConfirmation(upLayerPduId);
            }

            /* @req SWS_SecOC_00064 */
            /* Free the buffer that contains the Secured I-PDU */
            txRtPtr = &SecOC_TxRtData[index];
            txRtPtr->txPduReady = FALSE;
            if (NULL_PTR != txPduProcessingPtr->SecOCSameBufferPduRef)
            {
                *(txPduProcessingPtr->SecOCSameBufferPduRef->SecOCSameBufferInUse) = FALSE;
            }

            /* Last process complete,clear the tx status */
            txRtPtr->txPduStatus = SECOC_TX_NONE;
            txRtPtr->txAuthBuildCnt = 0u;
            /* Clear tx event */
            SECOC_CLR_TX_EVENT(index, SECOC_EVENT_TX_ALL);
        }
        SchM_Exit_SecOC_TxData();
#endif /* SECOC_TX_IF_PDU_NUM > 0u */
    }
}
#define SECOC_STOP_SEC_SECOCTXCONFIRMATION_CALLBACK_CODE
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_SECOCTPTXCONFIRMATION_CALLBACK_CODE
#include "SecOC_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function is called after the I-PDU has been
 *                     transmitted on its network, the result indicates whether
 *                     the transmission was successful or not.
 * ServiceId           0x48
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the transmitted I-PDU.
 *                     result: Result of the transmission of the I-PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SECOC_CODE)
SecOC_TpTxConfirmation(PduIdType id, Std_ReturnType result)
{
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*not initialized*/
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_TPTXCONFIRMATION_ID, SECOC_E_UNINIT);
        detNoErr = FALSE;
    }
    /*id error*/
    if ((id >= SECOC_TX_PDU_NUM) || (id < SECOC_TX_IF_PDU_NUM))
    {
        SECOC_DET_REPORT(SECOC_TPTXCONFIRMATION_ID, SECOC_E_INVALID_PDU_SDU_ID);
        detNoErr = FALSE;
    }
    if (detNoErr == TRUE)
#endif
    {
#if (SECOC_TX_TP_PDU_NUM > 0)
        P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
        txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(SECOC_TX_IF_PDU_NUM);
        P2CONST(SecOC_TxAuthenticPduLayerType, AUTOMATIC, SECOC_CONST)
        txAuthenticPduLayerPtr;
        P2CONST(SecOC_TxSecuredPduType, AUTOMATIC, SECOC_CONST)
        txSecuredPduPtr = SecOC_GetTxSecuredPduPtr(SECOC_TX_IF_PDU_NUM);
        P2CONST(SecOC_TxAuthenticPduType, AUTOMATIC, SECOC_CONST)
        txAuthenticPduPtr;
        P2CONST(SecOC_TxCryptographicPduType, AUTOMATIC, SECOC_CONST)
        txCryptographicPduPtr;
        P2VAR(SecOC_TxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
        txRtPtr = &SecOC_TxRtData[SECOC_TX_IF_PDU_NUM];
        PduIdType upLayerPduId;
        uint16 index;
        boolean txPduTag = FALSE;
        SchM_Enter_SecOC_TxData();
        for (index = SECOC_TX_IF_PDU_NUM; index < SECOC_TX_PDU_NUM; index++)
        {
            if (NULL_PTR != txSecuredPduPtr)
            {
                if (txSecuredPduPtr->SecOCTxSecuredLayerPduId == id)
                {
                    txPduTag = TRUE;
                }
            }
            else
            {
                txAuthenticPduPtr = SecOC_GetTxAuthenticPduPtr(index);
                if (txAuthenticPduPtr->SecOCTxAuthenticPduId == id)
                {
                    SECOC_SET_TX_EVENT(index, SECOC_TX_AUTHENTIC_PDU_CONFIRMED);
                    txPduTag = TRUE;
                }

                txCryptographicPduPtr = SecOC_GetTxCryptographicPduPtr(index);
                if (txCryptographicPduPtr->SecOCTxCryptographicPduId == id)
                {
                    SECOC_SET_TX_EVENT(index, SECOC_TX_CRYPTOGRAPHIC_PDU_CONFIRMED);
                    txPduTag = TRUE;
                }
            }
            if (TRUE == txPduTag)
            {
                break;
            }

            txPduProcessingPtr++;
            txSecuredPduPtr++;
            txRtPtr++;
        }
        SchM_Exit_SecOC_TxData();

        if (index < SECOC_TX_PDU_NUM)
        {
            txAuthenticPduLayerPtr = SecOC_GetTxAuthenticPduLayerPtr(index);
            upLayerPduId = txAuthenticPduLayerPtr->SecOCTxPduRAsUpLayerId;
            if ((NULL_PTR != txSecuredPduPtr)
                || (SECOC_IS_TX_EVENT(index, SECOC_TX_AUTHENTIC_PDU_CONFIRMED)
                    && SECOC_IS_TX_EVENT(index, SECOC_TX_CRYPTOGRAPHIC_PDU_CONFIRMED)))
            {
                /* @req SWS_SecOC_00074 */
                if (SECOC_IFPDU == txAuthenticPduLayerPtr->SecOCPduType)
                {
                    if (E_OK == result)
                    {
                        PduR_SecOCIfTxConfirmation(upLayerPduId);
                    }
                }
                else
                {
                    PduR_SecOCTpTxConfirmation(upLayerPduId, result);
                }
                SchM_Enter_SecOC_TxData();
                /* Free the buffer that contains the Secured I-PDU */
                txRtPtr = &SecOC_TxRtData[index];
                txRtPtr->txPduReady = FALSE;
                if (NULL_PTR != txPduProcessingPtr->SecOCSameBufferPduRef)
                {
                    *(txPduProcessingPtr->SecOCSameBufferPduRef->SecOCSameBufferInUse) = FALSE;
                }

                /* Last process complete,clear the tx status */
                txRtPtr->txPduStatus = SECOC_TX_NONE;
                txRtPtr->txAuthBuildCnt = 0u;
                /* Clear tx event */
                SECOC_CLR_TX_EVENT(index, SECOC_EVENT_TX_ALL);
                SchM_Exit_SecOC_TxData();
            }
        }
#else
        (void)(id);
        (void)(result);
#endif
    }
}
#define SECOC_STOP_SEC_SECOCTPTXCONFIRMATION_CALLBACK_CODE
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_SECOCTRIGGERTRANSMIT_CALLBACK_CODE
#include "SecOC_MemMap.h"
/******************************************************************************/
/*
 * Brief               Within this API, the upper layer module (called module)
 *                     shall check whether the available data fits into the
 *                     buffer size reported by PduInfoPtr->SduLength.If it fits,
 *                     it shall copy its data into the buffer provided by
 *                     PduInfoPtr->SduDataPtr and update the length of the
 *                     actual copied data in PduInfoPtr->SduLength. If not, it
 *                     returns E_NOT_OK without changing PduInfoPtr.
 * ServiceId           0x41
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds.
 * Param-Name[in]      TxPduId:ID of the SDU that is requested to be transmitted
 *                     PduInfoPtr: Contains a pointer to a buffer (SduDataPtr)
 *                     to where the SDU data shall be copied, and the available
 *                     buffer size in SduLengh. On return, the service will
 *                     indicate the length of the copied SDU data in SduLength.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: SDU has been copied and SduLength indicates the
 *                           number of copied bytes.
 *                     E_NOT_OK: No SDU data has been copied. PduInfoPtr must
 *                               not be used since it may contain a NULL pointer
 *                               or point to invalid data.
 */
/******************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE)
SecOC_TriggerTransmit(
    PduIdType TxPduId,
    P2VAR(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) PduInfoPtr /* PRQA S 3432 */ /* MISRA Rule 20.7 */
)
{
    Std_ReturnType result = E_NOT_OK;

#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*not initialized*/
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_TRIGGERTRANSMIT_ID, SECOC_E_UNINIT);
        detNoErr = FALSE;
    }
    /*id error*/
    if (TxPduId >= SECOC_TX_IF_PDU_NUM)
    {
        SECOC_DET_REPORT(SECOC_TRIGGERTRANSMIT_ID, SECOC_E_INVALID_PDU_SDU_ID);
        detNoErr = FALSE;
    }
    /*NULL POINTER*/
    if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        SECOC_DET_REPORT(SECOC_TRIGGERTRANSMIT_ID, SECOC_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
#if (SECOC_TX_IF_PDU_NUM > 0u)
        P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
        txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(0u);
        P2CONST(SecOC_TxSecuredPduType, AUTOMATIC, SECOC_CONST) txSecuredPduPtr;
        P2CONST(SecOC_TxAuthenticPduType, AUTOMATIC, SECOC_CONST)
        txAuthenticPduPtr;
        P2CONST(SecOC_TxCryptographicPduType, AUTOMATIC, SECOC_CONST)
        txCryptographicPduPtr;
        P2CONST(SecOC_UseMessageLinkType, AUTOMATIC, SECOC_CONST)
        txUseMessageLinkPtr;
        P2CONST(SecOC_TxPduRTType, AUTOMATIC, SECOC_CONST) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
        txRtPtr = &SecOC_TxRtData[0u];
        P2CONST(uint8, AUTOMATIC, SECOC_CONST) srcPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
        uint16 index;
        uint16 securedPduLength;
        uint16 authInfoTxLength;
        uint16 msgLinkLen = 0u;
        uint8 fvTruncLenInBits;
        uint8 headerLenInBytes;
        boolean txSecuredPduFlag = FALSE;
        boolean txAuthenticPduFlag = FALSE;
        boolean txCryptographicPduFlag;
        boolean txPduTag = FALSE;

        for (index = 0u; index < SECOC_TX_IF_PDU_NUM; index++)
        {
            txSecuredPduPtr = SecOC_GetTxSecuredPduPtr(index);
            if (NULL_PTR != txSecuredPduPtr)
            {
                if (txSecuredPduPtr->SecOCTxSecuredLayerPduId == TxPduId)
                {
                    txSecuredPduFlag = TRUE;
                    txPduTag = TRUE;
                }
            }
            else
            {
                txAuthenticPduPtr = SecOC_GetTxAuthenticPduPtr(index);
                if (txAuthenticPduPtr->SecOCTxAuthenticPduId == TxPduId)
                {
                    txAuthenticPduFlag = TRUE;
                    txPduTag = TRUE;
                }

                txCryptographicPduPtr = SecOC_GetTxCryptographicPduPtr(index);
                if (txCryptographicPduPtr->SecOCTxCryptographicPduId == TxPduId)
                {
                    txCryptographicPduFlag = TRUE;
                    txPduTag = TRUE;
                }
            }
            if (TRUE == txPduTag)
            {
                break;
            }

            txRtPtr++;
            txPduProcessingPtr++;
        }

        if ((index < SECOC_TX_IF_PDU_NUM) && (TRUE == txRtPtr->txPduReady))
        {
            /* Calculate pdu length */
            authInfoTxLength = txPduProcessingPtr->SecOCAuthInfoTruncLength;
            fvTruncLenInBits = txPduProcessingPtr->SecOCFreshnessValueTruncLength;
            if (TRUE == txSecuredPduFlag)
            {
                headerLenInBytes = txSecuredPduPtr->SecOCAuthPduHeaderLength;
                SchM_Enter_SecOC_TxData();
                securedPduLength =
                    headerLenInBytes + txRtPtr->txPduLength + SECOC_BIT_TO_BYTE(authInfoTxLength + fvTruncLenInBits);
                SchM_Exit_SecOC_TxData();
                if (NULL_PTR != txPduProcessingPtr->SecOCSameBufferPduRef)
                {
                    srcPtr = txPduProcessingPtr->SecOCSameBufferPduRef->SecOCSecuredPduBufferRef;
                }
                else
                {
                    srcPtr = txSecuredPduPtr->SecOCTxSecuredPduBufferRef;
                }
            }
            else if (TRUE == txAuthenticPduFlag)
            {
                headerLenInBytes = txAuthenticPduPtr->SecOCAuthPduHeaderLength;
                SchM_Enter_SecOC_TxData();
                securedPduLength = headerLenInBytes + txRtPtr->txPduLength;
                SchM_Exit_SecOC_TxData();
                if (NULL_PTR != txPduProcessingPtr->SecOCSameBufferPduRef)
                {
                    srcPtr = txPduProcessingPtr->SecOCSameBufferPduRef->SecOCAuthenticPduBufferRef;
                }
                else
                {
                    srcPtr = txAuthenticPduPtr->SecOCTxColAuthenticPduBufferRef;
                }
            }
            else if (TRUE == txCryptographicPduFlag) /* PRQA S 2991,2995 */ /* MISRA Rule 14.3, Rule 2.2 */
            {
                txUseMessageLinkPtr = SecOC_GetTxUseMessageLinkPtr(index);
                if (NULL_PTR != txUseMessageLinkPtr)
                {
                    msgLinkLen = txUseMessageLinkPtr->SecOCMessageLinkLen;
                }
                securedPduLength = SECOC_BIT_TO_BYTE((uint16)fvTruncLenInBits + authInfoTxLength + msgLinkLen);
                if (NULL_PTR != txPduProcessingPtr->SecOCSameBufferPduRef)
                {
                    srcPtr = txPduProcessingPtr->SecOCSameBufferPduRef->SecOCCryptographicPduBufferRef;
                }
                else
                {
                    srcPtr = txCryptographicPduPtr->SecOCTxCryptographicPduBufferRef;
                }
            }
            else
            {
                /* Nothing to do here */
            }

            if (PduInfoPtr->SduLength >= securedPduLength)
            {
                /* @req SWS_SecOC_00068 */
                /* Copy the Secured I-PDU to the lower layer*/
                (void)ILib_memcpy(PduInfoPtr->SduDataPtr, srcPtr, securedPduLength);
                PduInfoPtr->SduLength = securedPduLength;

                result = E_OK;
            }
            else
            {
                result = E_NOT_OK;
            }
        }
        else
        {
            /* @req SWS_SecOC_00150 */
            /* No Secured I-PDU available,Return E_NOT_OK */
            result = E_NOT_OK;
        }
#endif /* SECOC_TX_IF_PDU_NUM > 0u */
    }

    return result;
}
#define SECOC_STOP_SEC_SECOCTRIGGERTRANSMIT_CALLBACK_CODE
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_SECOCCOPYRXDATA_CALLBACK_CODE
#include "SecOC_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function is called to provide the received data of
 *                     an I-PDU segment (N-PDU) to the upper layer. Each call to
 *                     this function provides the next part of the I-PDU data.
 *                     The size of the remaining buffer is written to the
 *                     position indicated by bufferSizePtr.
 * ServiceId           0x44
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the received I-PDU.
 *                     info: Provides the source buffer (SduDataPtr) and the
 *                     number of bytes to be copied (SduLength). An SduLength of
 *                     0 can be used to query the current amount of available
 *                     buffer in the upper layer module. In this case, the
 *                     SduDataPtr may be a NULL_PTR.
 * Param-Name[out]     bufferSizePtr: Available receive buffer after data has
 *                                    been copied.
 * Param-Name[in/out]  None
 * Return              BUFREQ_OK: Data copied successfully
 *                     BUFREQ_E_NOT_OK: Data was not copied because an error
 *                                      occurred.
 */
/******************************************************************************/
FUNC(BufReq_ReturnType, SECOC_CODE)
SecOC_CopyRxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) info,
    /* PRQA S 3432, 3673 ++ */ /* MISRA Rule 20.7, 8.13 */
    P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) bufferSizePtr
    /* PRQA S 3432, 3673 -- */ /* MISRA Rule 20.7, 8.13 */
)
{
    BufReq_ReturnType result = BUFREQ_E_NOT_OK;

#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*not initialized*/
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_COPYRXDATA_ID, SECOC_E_UNINIT);
        detNoErr = FALSE;
    }
    /*id error*/
    if ((id >= SECOC_RX_PDU_NUM) || (id < SECOC_RX_IF_PDU_NUM))
    {
        SECOC_DET_REPORT(SECOC_COPYRXDATA_ID, SECOC_E_INVALID_PDU_SDU_ID);
        detNoErr = FALSE;
    }
    /*NULL POINTER*/
    if ((NULL_PTR == info) || (NULL_PTR == bufferSizePtr))
    {
        SECOC_DET_REPORT(SECOC_COPYRXDATA_ID, SECOC_E_PARAM_POINTER);
        detNoErr = FALSE;
    }

    if (detNoErr == TRUE)
#endif
    {
#if (SECOC_RX_TP_PDU_NUM > 0)
        SchM_Enter_SecOC_RxData();
        P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
        rxRtPtr = &SecOC_RxRtData[SECOC_RX_IF_PDU_NUM];
        uint16 index;

        for (index = SECOC_RX_IF_PDU_NUM; index < SECOC_RX_PDU_NUM; index++)
        {
            if (rxRtPtr->curPduId == id)
            {
                break;
            }

            rxRtPtr++;
        }

        if ((index < SECOC_RX_PDU_NUM) && (SECOC_IS_RX_EVENT(index, SECOC_RX_TP_DATA_FROM_LOWER)))
        {

            if (0u == info->SduLength)
            {
                /* Query the current amount of available buffer */
                *bufferSizePtr = rxRtPtr->sduRemaining;
                result = BUFREQ_OK;
            }
            else
            {
                if (rxRtPtr->sduRemaining >= info->SduLength)
                {
                    /* @req SWS_SecOC_00083 */
                    (void)ILib_memcpy(
                        &rxRtPtr->tpRxDestBufPtr[rxRtPtr->tpRxPduOffset],
                        info->SduDataPtr,
                        info->SduLength);
                    rxRtPtr->tpRxPduOffset += info->SduLength;
                    rxRtPtr->sduRemaining -= info->SduLength;
                    *bufferSizePtr = rxRtPtr->sduRemaining;
                    result = BUFREQ_OK;
                }
            }
        }
        else
        {
            /* Not find coresponding runtime data,return BUFREQ_E_NOT_OK */
        }
        SchM_Exit_SecOC_RxData();
#else
        (void)(id);
        (void)(info);
        (void)(bufferSizePtr);
#endif
    }

    return result;
}
#define SECOC_STOP_SEC_SECOCCOPYRXDATA_CALLBACK_CODE
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_SECOCCOPYTXDATA_CALLBACK_CODE
#include "SecOC_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function is called to acquire the transmit data of
 *                     an I-PDU segment (N-PDU).Each call to this function
 *                     provides the next part of the I-PDU data unless retry->Tp
 *                     DataState is TP_DATARETRY. In this case the function
 *                     restarts to copy the data beginning at the offset from
 *                     the current position indicated by retry->TxTpDataCnt.The
 *                     size of the remaining data is written to the position
 *                     indicated by availableDataPtr.
 * ServiceId           0x43
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the transmitted I-PDU.
 *                     info: Provides the destination buffer (SduDataPtr) and
 *                           the number of bytes to be copied (SduLength).
 *                     retry: This parameter is used to acknowledge transmitted
 *                     data or to retransmit data after transmission problems.
 * Param-Name[out]     availableDataPtr: Indicates the remaining number of bytes
 *                     that are available in the upper layer module's Tx buffer.
 * Param-Name[in/out]  None
 * Return              BUFREQ_OK: Data has been copied to the transmit buffer
 *                                completely as requested.
 *                     BUFREQ_E_BUSY: Request could not be fulfilled, because
 *                     the required amount of Tx data is not available.
 *                     BUFREQ_E_NOT_OK: Data has not been copied. Request failed
 */
/******************************************************************************/
FUNC(BufReq_ReturnType, SECOC_CODE)
SecOC_CopyTxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) info,
    P2CONST(RetryInfoType, AUTOMATIC, SECOC_APPL_DATA) retry,
    /* PRQA S 3432, 3673 ++ */ /* MISRA Rule 20.7, 8.13 */
    P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) availableDataPtr
    /* PRQA S 3432, 3673 -- */ /* MISRA Rule 20.7, 8.13 */
)
{
    BufReq_ReturnType bufferReqStatus = BUFREQ_E_NOT_OK;

#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*not initialized*/
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_COPYTXDATA_ID, SECOC_E_UNINIT);
        detNoErr = FALSE;
    }
    /*id error*/
    if ((id >= SECOC_TX_PDU_NUM) || (id < SECOC_TX_IF_PDU_NUM))
    {
        SECOC_DET_REPORT(SECOC_COPYTXDATA_ID, SECOC_E_INVALID_PDU_SDU_ID);
        detNoErr = FALSE;
    }
    /*NULL POINTER*/
    if ((NULL_PTR == info) || (NULL_PTR == availableDataPtr))
    {
        SECOC_DET_REPORT(SECOC_COPYTXDATA_ID, SECOC_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (detNoErr == TRUE)
#endif
    {
#if (SECOC_TX_TP_PDU_NUM > 0u)
        P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
        txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(SECOC_TX_IF_PDU_NUM);
        P2CONST(SecOC_TxSecuredPduType, AUTOMATIC, SECOC_CONST)
        txSecuredPduPtr = SecOC_GetTxSecuredPduPtr(SECOC_TX_IF_PDU_NUM);
        P2CONST(SecOC_TxAuthenticPduType, AUTOMATIC, SECOC_CONST)
        txAuthenticPduPtr;
        P2CONST(SecOC_TxCryptographicPduType, AUTOMATIC, SECOC_CONST)
        txCryptographicPduPtr;
        P2VAR(SecOC_TxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
        txRtPtr = &SecOC_TxRtData[SECOC_TX_IF_PDU_NUM];
        P2CONST(uint8, AUTOMATIC, SECOC_CONST) srcPtr = NULL_PTR; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
        PduLengthType dataLength = 0u;
        uint16 index;

        boolean txPduTag = FALSE;

        for (index = SECOC_TX_IF_PDU_NUM; index < SECOC_TX_PDU_NUM; index++)
        {
            if (NULL_PTR != txSecuredPduPtr)
            {
                if (txSecuredPduPtr->SecOCTxSecuredLayerPduId == id)
                {
                    dataLength = txRtPtr->txSecuredPduLen;
                    if (NULL_PTR == txPduProcessingPtr->SecOCSameBufferPduRef)
                    {
                        srcPtr = txSecuredPduPtr->SecOCTxSecuredPduBufferRef;
                    }
                    else
                    {
                        srcPtr = txPduProcessingPtr->SecOCSameBufferPduRef->SecOCSecuredPduBufferRef;
                    }
                    txPduTag = TRUE;
                }
            }
            else
            {
                txAuthenticPduPtr = SecOC_GetTxAuthenticPduPtr(index);
                if (txAuthenticPduPtr->SecOCTxAuthenticPduId == id)
                {
                    dataLength = txRtPtr->txAuthenticPduLen;
                    if (NULL_PTR == txPduProcessingPtr->SecOCSameBufferPduRef)
                    {
                        srcPtr = txAuthenticPduPtr->SecOCTxColAuthenticPduBufferRef;
                    }
                    else
                    {
                        srcPtr = txPduProcessingPtr->SecOCSameBufferPduRef->SecOCAuthenticPduBufferRef;
                    }
                    txPduTag = TRUE;
                }

                txCryptographicPduPtr = SecOC_GetTxCryptographicPduPtr(index);
                if (txCryptographicPduPtr->SecOCTxCryptographicPduId == id)
                {
                    dataLength = txRtPtr->txCryptographicPduLen;
                    if (NULL_PTR == txPduProcessingPtr->SecOCSameBufferPduRef)
                    {
                        srcPtr = txCryptographicPduPtr->SecOCTxCryptographicPduBufferRef;
                    }
                    else
                    {
                        srcPtr = txPduProcessingPtr->SecOCSameBufferPduRef->SecOCCryptographicPduBufferRef;
                    }
                    txPduTag = TRUE;
                }
            }
            if (TRUE == txPduTag)
            {
                break;
            }

            txPduProcessingPtr++;
            txSecuredPduPtr++;
            txRtPtr++;
        }

        if (retry != NULL_PTR)
        {
            if (TP_DATARETRY == retry->TpDataState)
            {
                if (txRtPtr->tpTxPduOffset >= retry->TxTpDataCnt)
                {
                    txRtPtr->tpTxPduOffset -= retry->TxTpDataCnt;
                }
                else
                {
                    return BUFREQ_E_NOT_OK;
                }
            }
            else if (TP_DATACONF == retry->TpDataState)
            {
                if (retry->TxTpDataCnt == dataLength)
                {
                    /* TP transmit finished, Clear internal buffer */
                    return BUFREQ_OK;
                }
            }
            else
            {
                /* Nothing to do here */
            }
        }

        if (FALSE == txRtPtr->txPduReady)
        {
            return BUFREQ_E_BUSY;
        }

        if ((txRtPtr->tpTxPduOffset + info->SduLength) > dataLength)
        {
            return BUFREQ_E_NOT_OK;
        }

        if ((info->SduLength > 0u) && (NULL_PTR != srcPtr))
        {
            SchM_Enter_SecOC_TxData();
            /* @req SWS_SecOC_00073 */
            (void)ILib_memcpy(info->SduDataPtr, &srcPtr[txRtPtr->tpTxPduOffset], info->SduLength);
            txRtPtr->tpTxPduOffset += info->SduLength;
            SchM_Exit_SecOC_TxData();
        }
        *availableDataPtr = dataLength - txRtPtr->tpTxPduOffset;

        bufferReqStatus = BUFREQ_OK;
#else
        (void)(id);
        (void)(info);
        (void)(retry);
        (void)(availableDataPtr);
#endif
    }
    return bufferReqStatus;
}
#define SECOC_STOP_SEC_SECOCCOPYTXDATA_CALLBACK_CODE
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_SECOCSTARTOFRECEPTION_CALLBACK_CODE
#include "SecOC_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function is called at the start of receiving an
 *                     N-SDU. The N-SDU might be fragmented into multiple N-PDUs
 *                     (FF with one or more following CFs) or might consist of a
 *                     single N-PDU (SF).The service shall provide the currently
 *                     available maximum buffer size when invoked with
 *                     TpSduLength equal to 0.
 * ServiceId           0x46
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the I-PDU.
 *                     info: Pointer to a PduInfoType structure containing the
 *                     payload data (without protocol information) and payload
 *                     length of the first frame or single frame of a transport
 *                     protocol I-PDU reception, and the MetaData related to
 *                     this PDU. If neither first/single frame data nor MetaData
 *                     are available, this parameter is set to NULL_PTR.
 *                     TpSduLength: Total length of the N-SDU to be received.
 * Param-Name[out]     bufferSizePtr: Available receive buffer in the receiving
 *                     module. This parameter will be used to compute the Block
 *                     Size (BS) in the transport protocol module.
 * Param-Name[in/out]  None
 * Return              BUFREQ_OK: Connection has been accepted.
 *                     BUFREQ_E_NOT_OK: Connection has been rejected.
 *                     BUFREQ_E_OVFL: No buffer of the required length can be
 *                                    provided; reception is aborted.
 */
/******************************************************************************/
FUNC(BufReq_ReturnType, SECOC_CODE)
SecOC_StartOfReception(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) info,
    PduLengthType TpSduLength,
    /* PRQA S 3432, 3673 ++ */ /* MISRA Rule 20.7, 8.13 */
    P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) bufferSizePtr
    /* PRQA S 3432, 3673 -- */ /* MISRA Rule 20.7, 8.13 */
)
{
    BufReq_ReturnType bufReqRet = BUFREQ_E_NOT_OK;

#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*not initialized*/
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_STARTOFRECEPTION_ID, SECOC_E_UNINIT);
        detNoErr = FALSE;
    }
    /*id error*/
    if ((id >= SECOC_RX_PDU_NUM) || (id < SECOC_RX_IF_PDU_NUM))
    {
        SECOC_DET_REPORT(SECOC_STARTOFRECEPTION_ID, SECOC_E_INVALID_PDU_SDU_ID);
        detNoErr = FALSE;
    }
    /*NULL POINTER*/
    if (NULL_PTR == bufferSizePtr)
    {
        SECOC_DET_REPORT(SECOC_STARTOFRECEPTION_ID, SECOC_E_PARAM_POINTER);
        detNoErr = FALSE;
    }

    if (detNoErr == TRUE)
#endif
    {
#if (SECOC_RX_TP_PDU_NUM > 0)
        P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
        rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(SECOC_RX_IF_PDU_NUM);
        P2CONST(SecOC_RxSecuredPduType, AUTOMATIC, SECOC_CONST)
        rxSecuredPduPtr = SecOC_GetRxSecuredPduPtr(SECOC_RX_IF_PDU_NUM);
        P2CONST(SecOC_RxAuthenticPduType, AUTOMATIC, SECOC_CONST)
        rxAuthenticPduPtr;
        P2CONST(SecOC_RxCryptographicPduType, AUTOMATIC, SECOC_CONST)
        rxCryptographicPduPtr;
        uint16 index;
        boolean rxPduTag = FALSE;

        for (index = SECOC_RX_IF_PDU_NUM; index < SECOC_RX_PDU_NUM; index++)
        {
            if (NULL_PTR != rxSecuredPduPtr)
            {
                if (rxSecuredPduPtr->SecOCRxSecuredLayerPduId == id)
                {
                    bufReqRet = SecOC_RxTpSecuredPduStartOfReception(index, id, info, TpSduLength, bufferSizePtr);
                    rxPduTag = TRUE;
                }
            }
            else
            {
                rxAuthenticPduPtr = SecOC_GetRxAuthenticPduPtr(index);
                if (rxAuthenticPduPtr->SecOCRxAuthenticPduId == id)
                {
                    bufReqRet = SecOC_RxTpAuthenticPduStartOfReception(index, id, info, TpSduLength, bufferSizePtr);
                    rxPduTag = TRUE;
                }

                rxCryptographicPduPtr = SecOC_GetRxCryptographicPduPtr(index);
                if (rxCryptographicPduPtr->SecOCRxCryptographicPduId == id)
                {
                    bufReqRet = SecOC_RxTpCryptographicPduStartOfReception(index, id, info, TpSduLength, bufferSizePtr);
                    rxPduTag = TRUE;
                }
            }
            if (TRUE == rxPduTag)
            {
                break;
            }

            rxPduProcessingPtr++;
            rxSecuredPduPtr++;
        }
#else
        (void)(id);
        (void)(info);
        (void)(TpSduLength);
        (void)(bufferSizePtr);
#endif
    }

    return bufReqRet;
}
#define SECOC_STOP_SEC_SECOCSTARTOFRECEPTION_CALLBACK_CODE
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_SECOCCSMGENERATEJOBFINISHEDINDICATION_CALLBACK_CODE
#include "SecOC_MemMap.h"
/******************************************************************************/
/*
 * Brief               CSM notifies SecOC that a Authenticator generate job has
 *                     finished.
 * ServiceId           0x50
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      job: JobID of the operation that caused the callback.
 *                     result: Contains the result of cryptographic operation.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SECOC_CODE)
SecOC_CsmGenerateJobFinishedIndication(
    P2CONST(Crypto_JobType, AUTOMATIC, SECOC_APPL_CONST) job,
    Crypto_ResultType result)
{
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_CSMVERIFYJOBFINISHEDINDICATION_ID, SECOC_E_UNINIT);
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
        P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
        txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(0u);
        uint16 index;

        for (index = 0u; index < SECOC_TX_PDU_NUM; index++)
        {
            if (txPduProcessingPtr->SecOCTxAuthServiceConfigRef->SecOCJobId == job->jobId)
            {
                if (E_OK == result)
                {
                    if (SECOC_IS_TX_EVENT(index, SECOC_TX_GENERATE_FINISH_IND_WAITING))
                    {
                        SECOC_SET_TX_EVENT(index, SECOC_TX_GENERATE_FINISH_IND_CONFIRMED);
                    }
                }
                else if (CRYPTO_E_BUSY == result)
                {
                    /* @req SWS_SecOC_00151 */
                    /* CSM module reports a recoverable error,Initiate retry */
                    SecOC_TxGenerateAuthenticatorRetry(index);
                }
                else
                {
                    /* @req SWS_SecOC_00229 */
                    SecOC_TxBuildAuthenticatorFailHandle(index);
                }
                break;
            }
            txPduProcessingPtr++;
        }
    }
}
#define SECOC_STOP_SEC_SECOCCSMGENERATEJOBFINISHEDINDICATION_CALLBACK_CODE
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_SECOCCSMVERIFYJOBFINISHEDINDICATION_CALLBACK_CODE
#include "SecOC_MemMap.h"
/******************************************************************************/
/*
 * Brief               CSM notifies SecOC that a Authenticator verify job has
 *                     finished.
 * ServiceId           0x51
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      job: JobID of the operation that caused the callback.
 *                     result: Contains the result of cryptographic operation.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SECOC_CODE)
SecOC_CsmVerifyJobFinishedIndication(P2CONST(Crypto_JobType, AUTOMATIC, SECOC_APPL_CONST) job, Crypto_ResultType result)
{
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
    if (SecOC_InitStatus == SECOC_UNINIT)
    {
        SECOC_DET_REPORT(SECOC_CSMVERIFYJOBFINISHEDINDICATION_ID, SECOC_E_UNINIT);
    }
    else
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    {
        P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
        rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(0u);
        P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
        rxRtPtr = &SecOC_RxRtData[0u];
        uint16 index;

        for (index = 0u; index < SECOC_RX_PDU_NUM; index++)
        {
            if (rxPduProcessingPtr->SecOCRxAuthServiceConfigRef->SecOCJobId == job->jobId)
            {
                if (E_OK == result)
                {
                    SchM_Enter_SecOC_RxData();
                    if (SECOC_IS_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_WAITING))
                    {
                        SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);
                    }

                    /* @req SWS_SecOC_00239 */
                    /* Verification of the authenticator successfully executed,
                     * the authentication build counter shall be set to 0. */
                    rxRtPtr->rxAuthBuildAttempts = 0u;
                    SchM_Exit_SecOC_RxData();
                }
                else if (CRYPTO_E_BUSY == result)
                {
                    /* @req SWS_SecOC_00237 */
                    /* Recoverable error occur */
                    SecOC_RxAuthenticationBuildRetry(index);
                }
                else
                {
                    /* No-recoverable error */
                    /* @req SWS_SecOC_00241 */
                    if ((SECOC_OVERRIDE_PASS_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)
                        || ((SECOC_OVERRIDE_PASS_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
                            && (rxRtPtr->rxBakNumOfMsgToOverride > 0u)))
                    {
                        SchM_Enter_SecOC_RxData();
                        /* Continue to send authentic I-PDU to upper layer */
                        SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);
                        SchM_Exit_SecOC_RxData();
                    }
                    else
                    {
                        rxRtPtr->rxVeryfyResult = SECOC_VERIFICATIONFAILURE;
                        /* Report verify status */
                        SecOC_RxReportVerifyStatus(index);

                        /* Non-recoverable error occur */
                        SecOC_RxChannelReset(index);
                    }
                }
                break;
            }
            rxRtPtr++;
            rxPduProcessingPtr++;
        }
    }
}
#define SECOC_STOP_SEC_SECOCCSMVERIFYJOBFINISHEDINDICATION_CALLBACK_CODE
#include "SecOC_MemMap.h"

#define SECOC_START_SEC_CODE
#include "SecOC_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function performs the processing of the SecOC
 *                     module's authentication and verification processing for
 *                     the Rx path.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SECOC_CODE) SecOC_MainFunctionRx(void)
{
#if (SECOC_RX_PDU_NUM > 0u)
    uint16 index;

    if (SECOC_INIT == SecOC_InitStatus)
    {
        for (index = 0u; index < SECOC_RX_PDU_NUM; index++)
        {
#if (SECOC_RX_IF_PDU_NUM > 0u)
            if (SECOC_RX_NONE == SecOC_RxRtData[index].rxPduStatus)
            {
                SecOC_RxLoadQueuedPdu(index);
            }
#endif

            if (SECOC_RX_NONE != SecOC_RxRtData[index].rxPduStatus)
            {
                switch (SecOC_RxRtData[index].rxPduStatus)
                {
                case SECOC_RX_CALC:
                case SECOC_RX_RETRY:
                    /* @req SWS_SecOC_00079 */
                    SecOC_RxParseSecuredIpdu(index);
                    SecOC_RxVerifyProcee(index);
                    SecOC_RxPassAuthenticPduToUpperLayer(index);
#if (SECOC_RX_TP_PDU_NUM > 0u)
                    SecOC_RxTransTpDataToUpperLayer(index);
#endif
                    break;

                default:
                    /* Nothing to do here */
                    break;
                }
            }
        }
    }
#endif /* SECOC_RX_PDU_NUM > 0u */
}

/******************************************************************************/
/*
 * Brief               This function performs the processing of the SecOC
 *                     module's authentication and verification processing for
 *                     the Tx path.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SECOC_CODE) SecOC_MainFunctionTx(void)
{
#if (SECOC_TX_PDU_NUM > 0u)
    uint16 index;

    if (SECOC_INIT == SecOC_InitStatus)
    {
        /* @req SWS_SecOC_00060,SWS_SecOC_00061 */
        /* Process Tx Pdus */
        for (index = 0u; index < SECOC_TX_PDU_NUM; index++)
        {
            if (SECOC_TX_NONE != SecOC_TxRtData[index].txPduStatus)
            {
                switch (SecOC_TxRtData[index].txPduStatus)
                {
                case SECOC_TX_CALC:
                case SECOC_TX_RETRY:
#if (SECOC_TX_TP_PDU_NUM > 0u)
                    SecOC_TxTpAcquireData(index);
#endif
                    /* @req SWS_SecOC_00060,SWS_SecOC_00065,
                     * SWS_SecOC_00070 */
                    SecOC_TxGenerateAuthenticator(index);
                    /* @req SWS_SecOC_00061,SWS_SecOC_00066,
                     * SWS_SecOC_00071  */
                    SecOC_TxSendSecuredIPDU(index);
                    break;

                default:
                    /* Nothing to do here*/
                    break;
                }
            }
        }
    }
#endif /* SECOC_TX_PDU_NUM > 0u */
}
#define SECOC_STOP_SEC_CODE
#include "SecOC_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define SECOC_START_SEC_CODE
#include "SecOC_MemMap.h"

#if (SECOC_TX_PDU_NUM > 0u)

/******************************************************************************/
/*
 * Description         This function is used to set a specific transmit event.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      i: Index of the event.
 *                     e: Event to be set.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SECOC_SET_TX_EVENT(uint16 i, uint16 e)
{
    (SecOC_TxEvent[i] |= (e));
}

/******************************************************************************/
/*
 * Description         This function is used to clear a specific transmit event.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      i: Index of the event.
 *                     e: Event to be cleared.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SECOC_CLR_TX_EVENT(uint16 i, uint16 e)
{
    (SecOC_TxEvent[i] &= ~(e));
}

/******************************************************************************/
/*
 * Description         This function is used to check if a specific event is a transmit event.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      i: Index of the event.
 *                     e: Event to be checked.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean: TRUE if the event is a transmit event, FALSE otherwise.
 */
/******************************************************************************/
static FUNC(boolean, SECOC_CODE) SECOC_IS_TX_EVENT(uint16 i, uint16 e)
{
    return ((SecOC_TxEvent[i] & (e)) != 0u) ? TRUE : FALSE;
}

/******************************************************************************/
/*
 * Description         This function is used to retrieve the pointer to the transmit PDU Processing for a specific
 * index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      idx:Index for which the pointer to the transmit PDU Processing is requested.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              SecOC_TxPduProcessingType: Pointer to the transmit PDU Processing.
 */
/******************************************************************************/
static P2CONST(SecOC_TxPduProcessingType, TYPEDEF, SECOC_CONST) SecOC_GetTxPduProcessingPtr(uint16 idx)
{
    return (&SecOC_ConfigPtr->SecOCTxPduProcessing[idx]);
}

/******************************************************************************/
/*
 * Description         This function is used to retrieve the pointer to the transmit Authentic PDU Layer for a specific
 * index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      idx:Index for which the pointer to the transmit Authentic PDU Layer is requested.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              SecOC_TxAuthenticPduLayerType: Pointer to the transmit Authentic PDU Layer.
 */
/******************************************************************************/
static P2CONST(SecOC_TxAuthenticPduLayerType, AUTOMATIC, SECOC_CONST) SecOC_GetTxAuthenticPduLayerPtr(uint16 idx)
{
    return (SecOC_GetTxPduProcessingPtr(idx)->SecOCTxAuthenticPduLayer);
}

/******************************************************************************/
/*
 * Description         This function is used to retrieve the pointer to the transmit Secured PDU Layer for a specific
 * index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      idx: Index for which the pointer to the transmit Secured PDU Layer is requested.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              SecOC_TxSecuredPduLayerType: Pointer to the transmit Secured PDU Layer.
 */
/******************************************************************************/
static P2CONST(SecOC_TxSecuredPduLayerType, TYPEDEF, SECOC_CONST) SecOC_GetTxSecuredPduLayerPtr(uint16 idx)
{
    return (SecOC_GetTxPduProcessingPtr(idx)->SecOCTxSecuredPduLayer);
}

/******************************************************************************/
/*
 * Description         This function is used to retrieve the pointer to the transmit Secured PDU for a specific index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      idx: Index for which the pointer to the transmit Secured PDU is requested.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              SecOC_TxSecuredPduType: Pointer to the transmit Secured PDU.
 */
/******************************************************************************/
static P2CONST(SecOC_TxSecuredPduType, AUTOMATIC, SECOC_CONST) SecOC_GetTxSecuredPduPtr(uint16 idx)
{
    return (SecOC_GetTxSecuredPduLayerPtr(idx)->SecOCTxSecuredPdu);
}

/******************************************************************************/
/*
 * Description         This function returns a pointer to the Secured PDU collection based on the provided index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      idx: Index representing the Secured PDU collection.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              SecOC_TxSecuredPduCollectionType: Pointer to the Secured PDU collection.
 */
/******************************************************************************/
static P2CONST(SecOC_TxSecuredPduCollectionType, TYPEDEF, SECOC_CONST) SecOC_GetTxSecuredPduCollectionPtr(uint16 idx)
{
    return (SecOC_GetTxSecuredPduLayerPtr(idx)->SecOCTxSecuredPduCollection);
}

/******************************************************************************/
/*
 * Description         This function is used to retrieve the pointer to the transmit Authentic PDU for a specific index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      idx: Index for which the pointer to the transmit Authentic PDU is requested.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              SecOC_TxAuthenticPduType: Pointer to the transmit Authentic PDU.
 */
/******************************************************************************/
static P2CONST(SecOC_TxAuthenticPduType, AUTOMATIC, SECOC_CONST) SecOC_GetTxAuthenticPduPtr(uint16 idx)
{
    return (SecOC_GetTxSecuredPduCollectionPtr(idx)->SecOCTxAuthenticPdu);
}

/******************************************************************************/
/*
 * Description         This function is used to retrieve the pointer to the transmit Cryptographic PDU for a specific
 * index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      idx: Index for which the pointer to the transmit Cryptographic PDU is requested.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              SecOC_TxCryptographicPduType: Pointer to the transmit Cryptographic PDU.
 */
/******************************************************************************/
static P2CONST(SecOC_TxCryptographicPduType, AUTOMATIC, SECOC_CONST) SecOC_GetTxCryptographicPduPtr(uint16 idx)
{
    return (SecOC_GetTxSecuredPduCollectionPtr(idx)->SecOCTxCryptographicPdu);
}

/******************************************************************************/
/*
 * Description         This function is used to retrieve the pointer to the transmit Use Message Link for a specific
 * index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      idx:Index for which the pointer to the transmit Use Message Link is requested.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              SecOC_UseMessageLinkType: Pointer to the transmit Use Message Link.
 */
/******************************************************************************/
static P2CONST(SecOC_UseMessageLinkType, AUTOMATIC, SECOC_CONST) SecOC_GetTxUseMessageLinkPtr(uint16 idx)
{
    return (SecOC_GetTxSecuredPduCollectionPtr(idx)->SecOCUseMessageLink);
}

/******************************************************************************/
/*
 * Brief               Generate authenticator used to create Secured I-PDU
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SECOC Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_TxGenerateAuthenticator(uint16 index)
{
    P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
    txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    P2CONST(SecOC_CsmJobType, AUTOMATIC, SECOC_CONST)
    jobPtr = txPduProcessingPtr->SecOCTxAuthServiceConfigRef;
    P2VAR(SecOC_TxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    txRtPtr = &SecOC_TxRtData[index];
    uint32 dataLength;
    Std_ReturnType ret;
    boolean retryFlag = FALSE;
    boolean failFlag = FALSE;

    if ((TRUE == SECOC_IS_TX_EVENT(index, SECOC_TX_REQUEST))
        && (TRUE == SECOC_IS_TX_EVENT(index, SECOC_TX_DATA_ACQUIRED)))
    {
        dataLength = SecOC_TxCalcDataToAuthenticatorLength(index);
        ret = SecOC_TxConstructDataToAuthenticator(index);
        txRtPtr->txResultLength = SECOC_MAX_AUTHENTICATOR_LEN;
        if (E_OK == ret)
        {
            /* @req SWS_SecOC_00035 */
            switch (jobPtr->SecOCCsmPrimitives)
            {
#if (SECOC_CSMMAC_ENABLED == STD_ON)
            case SECOC_CSMMAC:
                ret = Csm_MacGenerate(
                    jobPtr->SecOCJobId,
                    CRYPTO_OPERATIONMODE_SINGLECALL,
                    &SecOC_DataToAuthenticator[0u],
                    dataLength,
                    &txRtPtr->txResultBuffer[0u],
                    &txRtPtr->txResultLength);
                break;
#endif /* SECOC_CSMMAC_ENABLED == STD_ON */

#if (SECOC_CSMSIGNATRUE_ENABLED == STD_ON)
            case SECOC_CSMSIGNATURE:
                ret = Csm_SignatureGenerate(
                    jobPtr->SecOCJobId,
                    CRYPTO_OPERATIONMODE_SINGLECALL,
                    &SecOC_DataToAuthenticator[0u],
                    dataLength,
                    &txRtPtr->txResultBuffer[0u],
                    &txRtPtr->txResultLength);
                break;
#endif /* SECOC_CSMSIGNATRUE_ENABLED == STD_ON */

            default:
                ret = E_NOT_OK;
                break;
            }

            if (E_OK == ret)
            {
                SchM_Enter_SecOC_TxData();
                SECOC_CLR_TX_EVENT(index, SECOC_TX_REQUEST | SECOC_TX_DATA_ACQUIRED);
                if (SECOC_CRYPTO_PROCESSING_SYNC == jobPtr->SecOCCryptoProcessingType)
                {
                    SECOC_SET_TX_EVENT(index, SECOC_TX_GENERATE_FINISH_IND_CONFIRMED);
                }
                else
                {
                    SECOC_SET_TX_EVENT(index, SECOC_TX_GENERATE_FINISH_IND_WAITING);
                }
                SchM_Exit_SecOC_TxData();
            }
            else if (CRYPTO_E_BUSY == ret)
            {
                /* @req SWS_SecOC_00227,SWS_SecOC_00228 */
                retryFlag = TRUE;
            }
            else
            {
                /* Non-recoverable error */
                failFlag = TRUE;
            }
        }
        else if (E_BUSY == ret)
        {
            /* @req SWS_SecOC_00227,SWS_SecOC_00228 */
            retryFlag = TRUE;
        }
        else
        {
            /* Non-recoverable error */
            failFlag = TRUE;
        }

        if (TRUE == retryFlag)
        {
            /* Initiate retry process */
            SecOC_TxGenerateAuthenticatorRetry(index);
        }

        if (TRUE == failFlag)
        {
            /* @req SWS_SecOC_00229 */
            SecOC_TxBuildAuthenticatorFailHandle(index);
        }
    }
}

/******************************************************************************/
/*
 * Brief               Calculate the length of DataToAuthenticator
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SECOC Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              The length of DataToAuthenticator
 */
/******************************************************************************/
static FUNC(uint32, SECOC_CODE) SecOC_TxCalcDataToAuthenticatorLength(uint16 index)
{
    P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
    txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    P2CONST(SecOC_TxPduRTType, AUTOMATIC, SECOC_CONST) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    txRtPtr = &SecOC_TxRtData[index];
    P2CONST(SecOC_TxPduSecuredAreaType, TYPEDEF, SECOC_CONST)
    SecOCTxPduSecuredArea = txPduProcessingPtr->SecOCTxPduSecuredArea;
    uint32 dataLength = SECOC_DATAID_LEN;

    if (NULL_PTR != SecOCTxPduSecuredArea)
    {
        /* Use part of the Authentic I-PDU to construct DataToAuthenticator*/
        dataLength = dataLength + SecOCTxPduSecuredArea->SecOCSecuredTxPduLength;
    }
    else
    {
        SchM_Enter_SecOC_TxData();
        /* Use the complete Authentic I-PDU to construct DataToAuthenticator*/
        dataLength = dataLength + txRtPtr->txPduLength;
        SchM_Exit_SecOC_TxData();
    }

    dataLength += SECOC_BIT_TO_BYTE((uint16)txPduProcessingPtr->SecOCFreshnessValueLength);

    return dataLength;
}

/******************************************************************************/
/*
 * Brief               Construct the DataToAuthenticator
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SECOC Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(Std_ReturnType, SECOC_CODE) SecOC_TxConstructDataToAuthenticator(uint16 index)
{
    P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
    txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    P2VAR(SecOC_TxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    txRtPtr = &SecOC_TxRtData[index];
    P2CONST(uint8, AUTOMATIC, SECOC_CONST) authenticPduPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2CONST(SecOC_TxPduSecuredAreaType, TYPEDEF, SECOC_CONST)
    SecOCTxPduSecuredArea = txPduProcessingPtr->SecOCTxPduSecuredArea;
    SecOC_FreshnessArrayType truncatedFreshnessValue;
    uint32 securedIPDULength;
    uint32 securedIPDUOffset;
    uint32 freshnessValueLen;
    uint32 freshnessValueTruncLen;
    uint16 dataId;
    Std_ReturnType ret;

    SchM_Enter_SecOC_Authentication();

    (void)ILib_memset(SecOC_DataToAuthenticator, 0, SECOC_DATA_TO_AUTHENTICATOR_LENGTH);

    /* @req SWS_SecOC_00034 */
    /*
     * DataToAuthenticator = Data Identifier | secured part of the Authentic
     * I-PDU | Complete Freshness Value
     */

    /* Data Identifie */
    dataId = txPduProcessingPtr->SecOCDataId;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)

    SecOC_EndianSwap((uint8*)&dataId, sizeof(dataId));
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
    (void)ILib_memcpy(&SecOC_DataToAuthenticator[0u], (uint8*)&dataId, SECOC_DATAID_LEN);

    /* Secured part of the Authentic */
    if (NULL_PTR == txPduProcessingPtr->SecOCSameBufferPduRef)
    {
        authenticPduPtr = txPduProcessingPtr->SecOCAuthenticPduBufferRef;
    }
    else
    {
        authenticPduPtr = txPduProcessingPtr->SecOCSameBufferPduRef->SecOCAuthenticPduBufferRef;
    }

    if (NULL_PTR != SecOCTxPduSecuredArea)
    {
        /* Use part of the Authentic I-PDU to construct DataToAuthenticator*/
        securedIPDULength = SecOCTxPduSecuredArea->SecOCSecuredTxPduLength;
        securedIPDUOffset = SecOCTxPduSecuredArea->SecOCSecuredTxPduOffset;
    }
    else
    {
        /* Use the complete Authentic I-PDU to construct DataToAuthenticator*/
        securedIPDULength = txRtPtr->txPduLength;
        securedIPDUOffset = 0u;
    }

    (void)ILib_memcpy(
        &SecOC_DataToAuthenticator[SECOC_DATAID_LEN],
        &authenticPduPtr[securedIPDUOffset],
        securedIPDULength);

    /* Complete Freshness Value */
    freshnessValueLen = txPduProcessingPtr->SecOCFreshnessValueLength;
    freshnessValueTruncLen = txPduProcessingPtr->SecOCFreshnessValueTruncLength;
    SchM_Exit_SecOC_Authentication();
#if (SECOC_CFUNC == SECOC_QUERY_FRESHNESS_VALUE)
    if (TRUE == txPduProcessingPtr->SecOCProvideTxTruncatedFreshnessValue)
    {
        /* @req SWS_SecOC_00221,SWS_SecOC_00230 */
        ret = SecOC_GetTxFreshnessTruncData(
            txPduProcessingPtr->SecOCFreshnessValueId,
            txRtPtr->txFreshnessValue,
            &freshnessValueLen,
            truncatedFreshnessValue,
            &freshnessValueTruncLen);
    }
    else
    {
        /* @req SWS_SecOC_00222,SWS_SecOC_00231 */
        ret = SecOC_GetTxFreshness(
            txPduProcessingPtr->SecOCFreshnessValueId,
            txRtPtr->txFreshnessValue,
            &freshnessValueLen);
    }
#else
    if (TRUE == txPduProcessingPtr->SecOCProvideTxTruncatedFreshnessValue)
    {
        /* @req SWS_SecOC_00223 */
        ret = FreshnessManagement_GetTxFreshnessTruncData(
            txPduProcessingPtr->SecOCFreshnessValueId,
            txRtPtr->txFreshnessValue,
            &freshnessValueLen,
            truncatedFreshnessValue,
            &freshnessValueTruncLen);
    }
    else
    {
        /* @req SWS_SecOC_00224 */
        ret = FreshnessManagement_GetTxFreshness(
            txPduProcessingPtr->SecOCFreshnessValueId,
            txRtPtr->txFreshnessValue,
            &freshnessValueLen);
    }
#endif
    SchM_Enter_SecOC_Authentication();
    if (E_OK == ret)
    {
        freshnessValueLen = SECOC_BIT_TO_BYTE((uint16)txPduProcessingPtr->SecOCFreshnessValueLength);
        (void)ILib_memcpy(
            &SecOC_DataToAuthenticator[SECOC_DATAID_LEN + securedIPDULength],
            txRtPtr->txFreshnessValue,
            freshnessValueLen);
    }
    SchM_Exit_SecOC_Authentication();

    return ret;
}

/******************************************************************************/
/*
 * Brief               Generate Authenticator retry process
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SECOC Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_TxGenerateAuthenticatorRetry(uint16 index)
{
    P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
    txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    P2VAR(SecOC_TxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    txRtPtr = &SecOC_TxRtData[index];

    if (txRtPtr->txAuthBuildCnt < txPduProcessingPtr->SecOCAuthenticationBuildAttempts)
    {
        SchM_Enter_SecOC_TxData();
        /* @req SWS_SecOC_00227,SWS_SecOC_00228 */
        txRtPtr->txAuthBuildCnt++;
        txRtPtr->txPduStatus = SECOC_TX_RETRY;
        SchM_Exit_SecOC_TxData();
    }
    else
    {
        /* @req SWS_SecOC_00229 */
        SecOC_TxBuildAuthenticatorFailHandle(index);

/* @req SWS_SecOC_00155 */
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
        SECOC_DET_REPORT(SECOC_MAINFUNCTIONTX_ID, SECOC_E_CRYPTO_FAILURE);
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    }
}

/******************************************************************************/
/*
 * Brief               Build Authenticator fail process
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SECOC Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_TxBuildAuthenticatorFailHandle(uint16 index)
{
    P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
    txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    P2CONST(SecOC_SameBufferPduCollectionType, AUTOMATIC, SECOC_CONST)
    sameBufferPtr = txPduProcessingPtr->SecOCSameBufferPduRef;
    P2VAR(SecOC_TxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    txRtPtr = &SecOC_TxRtData[index];
    boolean cancelFlag = FALSE;
    P2CONST(SecOC_TxAuthenticPduLayerType, AUTOMATIC, SECOC_CONST)
    txAuthenticPduLayerPtr = SecOC_GetTxAuthenticPduLayerPtr(index);
    PduIdType upLayerPduId = txAuthenticPduLayerPtr->SecOCTxPduRAsUpLayerId;
/* @req SWS_SecOC_00229 */
#if defined(SECOC_DEFAULT_AUTHENTICATION_INFORMATION_PATTERN)
    SchM_Enter_SecOC_TxData();
    if (TRUE == txRtPtr->txDefaultAuthInfoEnable)
    {
        /* Use SecOCDefaultAuthenticationInformationPattern as Freshness Value
         * and Authenticator. */
        (void)ILib_memset(
            txRtPtr->txFreshnessValue,
            SECOC_DEFAULT_AUTHENTICATION_INFORMATION_PATTERN,
            sizeof(txRtPtr->txFreshnessValue));
        (void)ILib_memset(
            txRtPtr->txResultBuffer,
            SECOC_DEFAULT_AUTHENTICATION_INFORMATION_PATTERN,
            SECOC_MAX_AUTHENTICATOR_LEN);
        SECOC_CLR_TX_EVENT(index, SECOC_TX_REQUEST | SECOC_TX_DATA_ACQUIRED);
        SECOC_SET_TX_EVENT(index, SECOC_TX_GENERATE_FINISH_IND_CONFIRMED);
    }
    else
    {
        /* Cancel transmission */
        cancelFlag = TRUE;
    }
    SchM_Exit_SecOC_TxData();
#else
    /*  Cancel transmission */
    cancelFlag = TRUE;
#endif

    if (TRUE == cancelFlag)
    {
        SchM_Enter_SecOC_TxData();
        /*  Cancel transmission */
        SECOC_CLR_TX_EVENT(index, SECOC_EVENT_TX_ALL);
        txRtPtr->txPduStatus = SECOC_TX_NONE;
        txRtPtr->txAuthBuildCnt = 0u;
        SchM_Exit_SecOC_TxData();

        if (txAuthenticPduLayerPtr->SecOCPduType == SECOC_TPPDU)
        {
            PduR_SecOCTpTxConfirmation(upLayerPduId, E_NOT_OK);
        }
        SchM_Enter_SecOC_TxData();
        /* Free the buffer that contains the Secured I-PDU */
        txRtPtr->txPduReady = FALSE;
        SchM_Exit_SecOC_TxData();
        if (NULL_PTR != sameBufferPtr)
        {
            *(sameBufferPtr->SecOCSameBufferInUse) = FALSE;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Send Secured I-PDU
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC Channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_TxSendSecuredIPDU(uint16 index)
{
    P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
    txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    P2CONST(SecOC_TxSecuredPduType, AUTOMATIC, SECOC_CONST)
    txSecuredPduPtr = SecOC_GetTxSecuredPduPtr(index);
    uint16 freshnessValueId;

    if (SECOC_IS_TX_EVENT(index, SECOC_TX_GENERATE_FINISH_IND_CONFIRMED))
    {
        SECOC_CLR_TX_EVENT(index, SECOC_TX_GENERATE_FINISH_IND_CONFIRMED);
        if (NULL_PTR != txSecuredPduPtr)
        {
            /* SecOC transmit the Secured I-PDU in one message */
            SecOC_TxConstructSecuredIPDU(index);
            SecOC_TxBroadcastSecuredIPDU(index);
        }
        else
        {
            /* @req SWS_SecOC_00201,SWS_SecOC_00202 */
            /*
             * Use Secured Pdu Collection: SecOC shall transmit the Secured I-PDU as
             * two messages: The original Authentic I-PDU and a separate
             * Cryptographic I-PDU.
             */

            /* Send original Authentic I-PDU */
            SecOC_TxConstructAuthenticIPDU(index);
            SecOC_TxBroadcastAuthenticIPDU(index);

            /* Send Cryptographic I-PDU */
            SecOC_TxConstructCryptographicIPDU(index);
            SecOC_TxBroadcastCryptographicIPDU(index);
        }

        /* @req SWS_SecOC_00232,SWS_SecOC_00233 */
        /* Indicate the Secured I-PDU has been initiated for transmission.*/
        if (TRUE == txPduProcessingPtr->SecOCUseTxConfirmation)
        {
            freshnessValueId = txPduProcessingPtr->SecOCFreshnessValueId;
#if (SECOC_CFUNC == SECOC_QUERY_FRESHNESS_VALUE)
            (void)SecOC_SPduTxConfirmation(freshnessValueId);
#else
            FreshnessManagement_SPduTxConfirmation(freshnessValueId);
#endif
        }
    }
}

/******************************************************************************/
/*
 * Brief               Construct the Secured IPDU to be transmit
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC Channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_TxConstructSecuredIPDU(uint16 index)
{
    P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
    txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    P2CONST(SecOC_SameBufferPduCollectionType, AUTOMATIC, SECOC_CONST)
    sameBufferPtr = txPduProcessingPtr->SecOCSameBufferPduRef;
    P2CONST(SecOC_TxSecuredPduType, AUTOMATIC, SECOC_CONST)
    txSecuredPduPtr = SecOC_GetTxSecuredPduPtr(index);
    P2VAR(SecOC_TxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    txRtPtr = &SecOC_TxRtData[index];
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) destPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2CONST(uint8, AUTOMATIC, SECOC_CONST) srcPtr; /* PRQA S 3432 */    /* MISRA Rule 20.7 */
    uint32 offset = 0u;
    PduLengthType pduLength;
    uint16 authInfoTruncLenInBytes;
    uint8 fvLenInBytes;
    uint8 fvLenInBits;
    uint8 fvTruncLenInBytes;
    uint8 fvTruncLenInBits = 0u;
    uint8 authInfoShiftBits;
    uint16 fvOffsetBits = 0u;

    SchM_Enter_SecOC_TxData();
    if (NULL_PTR == sameBufferPtr)
    {
        destPtr = txSecuredPduPtr->SecOCTxSecuredPduBufferRef;
        srcPtr = txPduProcessingPtr->SecOCAuthenticPduBufferRef;
        (void)ILib_memset(destPtr, 0x00, txSecuredPduPtr->SecOCTxSecuredPduBuffLength);
    }
    else
    {
        destPtr = sameBufferPtr->SecOCSecuredPduBufferRef;
        srcPtr = sameBufferPtr->SecOCAuthenticPduBufferRef;
        (void)ILib_memset(destPtr, 0x00, sameBufferPtr->SecOCSecuredPduBuffLength);
    }

    /* Construct Secured I-PDU */
    /* @req SWS_SecOC_00037 */
    /* SecuredPDU = SecuredIPDUHeader(optional) | AuthenticIPDU | FreshnessValue
     * [SecOCFreshnessValueTruncLength] (optional) | Authenticator
     * [SecOCAuthInfoTruncLength] */

    /* @req SWS_SecOC_00262 */
    /* Header */
    if (txSecuredPduPtr->SecOCAuthPduHeaderLength > 0u)
    {
        pduLength = txRtPtr->txPduLength;

#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        SecOC_EndianSwap((uint8*)&pduLength, sizeof(pduLength));
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
        (void)ILib_memcpy(&destPtr[offset], (uint8*)&pduLength, sizeof(pduLength));
        offset += sizeof(pduLength);
    }

    /* Authentic I-PDU */
    (void)ILib_memcpy(&destPtr[offset], srcPtr, txRtPtr->txPduLength);
    offset += txRtPtr->txPduLength;

    /* Freshness value */
    /* @req SWS_SecOC_00094 */
    if (txPduProcessingPtr->SecOCFreshnessValueTruncLength > 0u)
    {
        fvLenInBits = txPduProcessingPtr->SecOCFreshnessValueLength;
        fvTruncLenInBits = txPduProcessingPtr->SecOCFreshnessValueTruncLength;
        fvLenInBytes = (uint8)SECOC_BIT_TO_BYTE(fvLenInBits);
        fvTruncLenInBytes = (uint8)SECOC_BIT_TO_BYTE(fvTruncLenInBits);
        if (0u != (SECOC_MOD_FROM_EIGHT((fvLenInBits))))
        {
            fvOffsetBits = 8u - (uint16)SECOC_MOD_FROM_EIGHT(fvLenInBits);
        }

        fvOffsetBits = ((uint16)fvLenInBits - (uint16)fvTruncLenInBits) + fvOffsetBits;

        SecOC_ArrayShiftLeft(txRtPtr->txFreshnessValue, fvLenInBytes, fvOffsetBits);
        (void)ILib_memcpy(&destPtr[offset], txRtPtr->txFreshnessValue, fvTruncLenInBytes);
        if (0u != (SECOC_MOD_FROM_EIGHT((fvTruncLenInBits))))
        {
            offset += ((uint32)fvTruncLenInBytes - 1u);
            destPtr[offset] &= (0xFFu << (8u - (SECOC_MOD_FROM_EIGHT((fvTruncLenInBits)))));
        }
        else
        {
            offset += fvTruncLenInBytes;
        }
    }

    /* Authenticator */
    /* @req SWS_SecOC_00036 */
    /* Truncate the resulting Authenticator down to the number of bits specified
     * by SecOCAuthInfoTruncLength. */
    authInfoShiftBits = SECOC_MOD_FROM_EIGHT((fvTruncLenInBits));
    authInfoTruncLenInBytes =
        SECOC_BIT_TO_BYTE(txPduProcessingPtr->SecOCAuthInfoTruncLength + (uint16)authInfoShiftBits);
    SecOC_ArrayShiftRight(&txRtPtr->txResultBuffer[0u], authInfoTruncLenInBytes, authInfoShiftBits);
    SecOC_ClearLowBit(
        &txRtPtr->txResultBuffer[0u],
        authInfoTruncLenInBytes,
        SECOC_MOD_FROM_EIGHT((txPduProcessingPtr->SecOCAuthInfoTruncLength + fvTruncLenInBits)));
    SecOC_ArrayOrOp(&destPtr[offset], &txRtPtr->txResultBuffer[0u], authInfoTruncLenInBytes);
    SchM_Exit_SecOC_TxData();
}

/******************************************************************************/
/*
 * Brief               Construct Authentic IPDU to be transmit
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_TxConstructAuthenticIPDU(uint16 index)
{
    P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
    txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    P2CONST(SecOC_SameBufferPduCollectionType, AUTOMATIC, SECOC_CONST)
    sameBufferPtr = txPduProcessingPtr->SecOCSameBufferPduRef;
    P2CONST(SecOC_TxAuthenticPduType, AUTOMATIC, SECOC_CONST)
    txAuthenticPduPtr = SecOC_GetTxAuthenticPduPtr(index);
    P2CONST(SecOC_TxPduRTType, AUTOMATIC, SECOC_CONST) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    txRtPtr = &SecOC_TxRtData[index];
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) destPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2CONST(uint8, AUTOMATIC, SECOC_CONST) srcPtr; /* PRQA S 3432 */    /* MISRA Rule 20.7 */
    uint32 offset = 0u;
    PduLengthType pduLength;

    SchM_Enter_SecOC_TxData();
    if (NULL_PTR == sameBufferPtr)
    {
        destPtr = txAuthenticPduPtr->SecOCTxColAuthenticPduBufferRef;
        srcPtr = txPduProcessingPtr->SecOCAuthenticPduBufferRef;
        (void)ILib_memset(destPtr, 0x00, txAuthenticPduPtr->SecOCTxColAuthenticPduBuffLength);
    }
    else
    {
        destPtr = sameBufferPtr->SecOCColAuthenticPduBufferRef;
        srcPtr = sameBufferPtr->SecOCAuthenticPduBufferRef;
        (void)ILib_memset(destPtr, 0x00, sameBufferPtr->SecOCColAuthenticPduBuffLength);
    }

    /* Construct Orignal Authentic I-PDU */
    /* @req SWS_SecOC_00262 */
    /* Header */
    if (txAuthenticPduPtr->SecOCAuthPduHeaderLength > 0u)
    {
        pduLength = txRtPtr->txPduLength;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        SecOC_EndianSwap((uint8*)&pduLength, sizeof(pduLength));
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

        (void)ILib_memcpy(&destPtr[offset], (uint8*)&pduLength, sizeof(pduLength));
        offset += sizeof(pduLength);
    }

    /* Authentic I-PDU */
    (void)ILib_memcpy(&destPtr[offset], srcPtr, txRtPtr->txPduLength);
    SchM_Exit_SecOC_TxData();
}

/******************************************************************************/
/*
 * Brief               Construct Cryptographic IPDU to be transmit
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_TxConstructCryptographicIPDU(uint16 index)
{
    P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
    txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    P2CONST(SecOC_SameBufferPduCollectionType, AUTOMATIC, SECOC_CONST)
    sameBufferPtr = txPduProcessingPtr->SecOCSameBufferPduRef;
    P2CONST(SecOC_TxCryptographicPduType, AUTOMATIC, SECOC_CONST)
    txCryptographicPduPtr = SecOC_GetTxCryptographicPduPtr(index);
    P2CONST(SecOC_UseMessageLinkType, AUTOMATIC, SECOC_CONST)
    messageLinkPtr = SecOC_GetTxUseMessageLinkPtr(index);
    P2VAR(SecOC_TxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    txRtPtr = &SecOC_TxRtData[index];
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) destPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) srcPtr; /* PRQA S 3432 */  /* MISRA Rule 20.7 */
    uint16 offset = 0u;
    uint16 authInfoTruncLenInBytes;
    uint16 authInfoTruncLenInBits;
    uint16 msgLinkLenInBytes;
    uint16 msgLinkLenInBits;
    uint16 msgLinkPos;
    uint8 fvLenInBytes;
    uint8 fvLenInBits;
    uint8 fvTruncLenInBytes;
    uint8 fvTruncLenInBits = 0u;
    uint8 authInfoShiftBits;
    uint8 msgLinkShiftBits;

    if (NULL_PTR == sameBufferPtr)
    {
        destPtr = txCryptographicPduPtr->SecOCTxCryptographicPduBufferRef;
        srcPtr = txPduProcessingPtr->SecOCAuthenticPduBufferRef;
        (void)ILib_memset(destPtr, 0x00, txCryptographicPduPtr->SecOCTxCryptographicPduBuffLength);
    }
    else
    {
        destPtr = sameBufferPtr->SecOCCryptographicPduBufferRef;
        srcPtr = sameBufferPtr->SecOCAuthenticPduBufferRef;
        (void)ILib_memset(destPtr, 0x00, sameBufferPtr->SecOCCryptographicPduBuffLength);
    }

    /* @req SWS_SecOC_00209 */
    /* Cryptographic I-PDU = Authentication Data | Message Linker */
    /* Freshness value */
    if (txPduProcessingPtr->SecOCFreshnessValueTruncLength > 0u)
    {
        uint16 fvOffsetBits = 0u;
        fvLenInBits = txPduProcessingPtr->SecOCFreshnessValueLength;
        fvTruncLenInBits = txPduProcessingPtr->SecOCFreshnessValueTruncLength;
        fvLenInBytes = (uint8)SECOC_BIT_TO_BYTE((uint16)fvLenInBits);
        fvTruncLenInBytes = (uint8)SECOC_BIT_TO_BYTE((uint16)fvTruncLenInBits);

        if (0u != (SECOC_MOD_FROM_EIGHT((fvLenInBits))))
        {
            fvOffsetBits = 8u - (uint16)SECOC_MOD_FROM_EIGHT(fvLenInBits);
        }

        fvOffsetBits = ((uint16)fvLenInBits - (uint16)fvTruncLenInBits) + fvOffsetBits;

        SchM_Enter_SecOC_TxData();
        SecOC_ArrayShiftLeft(txRtPtr->txFreshnessValue, fvLenInBytes, fvOffsetBits);
        (void)ILib_memcpy(&destPtr[offset], txRtPtr->txFreshnessValue, fvTruncLenInBytes);
        SchM_Exit_SecOC_TxData();
        if (0u != (SECOC_MOD_FROM_EIGHT((fvTruncLenInBits))))
        {
            offset = ((uint16)fvTruncLenInBytes - 1u);
            destPtr[offset] &= (0xFFu << (8u - (SECOC_MOD_FROM_EIGHT((fvTruncLenInBits)))));
        }
        else
        {
            offset = fvTruncLenInBytes;
        }
    }
    SchM_Enter_SecOC_TxData();
    /* Authenticator */
    authInfoShiftBits = SECOC_MOD_FROM_EIGHT((fvTruncLenInBits));
    authInfoTruncLenInBits = txPduProcessingPtr->SecOCAuthInfoTruncLength;
    authInfoTruncLenInBytes = SECOC_BIT_TO_BYTE(authInfoTruncLenInBits + (uint16)authInfoShiftBits);
    SecOC_ArrayShiftRight(&txRtPtr->txResultBuffer[0u], authInfoTruncLenInBytes, authInfoShiftBits);
    SecOC_ClearLowBit(
        &txRtPtr->txResultBuffer[0u],
        authInfoTruncLenInBytes,
        SECOC_MOD_FROM_EIGHT((txPduProcessingPtr->SecOCAuthInfoTruncLength + fvTruncLenInBits)));
    SecOC_ArrayOrOp(&destPtr[offset], &txRtPtr->txResultBuffer[0u], authInfoTruncLenInBytes);
    if (0u != (SECOC_MOD_FROM_EIGHT((authInfoTruncLenInBits + fvTruncLenInBits))))
    {
        offset += (authInfoTruncLenInBytes - 1u);
        destPtr[offset] &= (0xFFu << (8u - (SECOC_MOD_FROM_EIGHT((authInfoTruncLenInBits + fvTruncLenInBits)))));
    }
    else
    {
        offset += authInfoTruncLenInBytes;
    }

    /* @req SWS_SecOC_00210 */
    /* Message Linker */
    if (NULL_PTR != messageLinkPtr)
    {
        msgLinkLenInBits = messageLinkPtr->SecOCMessageLinkLen;
        msgLinkPos = messageLinkPtr->SecOCMessageLinkPos;
        SecOC_ArrayShiftLeft(srcPtr, (uint16)txRtPtr->txPduLength, msgLinkPos);
        msgLinkShiftBits = (uint8)(SECOC_MOD_FROM_EIGHT((fvTruncLenInBits + authInfoTruncLenInBits)));
        msgLinkLenInBytes = SECOC_BIT_TO_BYTE(msgLinkLenInBits + (uint16)msgLinkShiftBits);
        SecOC_ArrayShiftRight(srcPtr, msgLinkLenInBytes, msgLinkShiftBits);
        SecOC_ArrayOrOp(&destPtr[offset], srcPtr, msgLinkLenInBytes);
    }
    SchM_Exit_SecOC_TxData();
}

/******************************************************************************/
/*
 * Brief               Broadcast Secured I-PDU to lower layer
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_TxBroadcastSecuredIPDU(uint16 index)
{
    P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
    txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    P2CONST(SecOC_TxSecuredPduType, AUTOMATIC, SECOC_CONST)
    txSecuredPduPtr = SecOC_GetTxSecuredPduPtr(index);
    P2VAR(SecOC_TxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    txRtPtr = &SecOC_TxRtData[index];
    PduIdType pduId = txSecuredPduPtr->SecOCTxPduRAsLowerLayerId;
    PduInfoType pduInfo;
    uint16 authInfoTxLength = txPduProcessingPtr->SecOCAuthInfoTruncLength;
    uint8 fvTruncLenInBits = txPduProcessingPtr->SecOCFreshnessValueTruncLength;
    uint8 headerLenInBytes = txSecuredPduPtr->SecOCAuthPduHeaderLength;

    SchM_Enter_SecOC_TxData();
    uint16 SecuredIpduLength =
        headerLenInBytes + txRtPtr->txPduLength + SECOC_BIT_TO_BYTE(authInfoTxLength + fvTruncLenInBits);
    if (SecuredIpduLength < txSecuredPduPtr->SecOCTxSecuredPduBuffLength)
    {
        pduInfo.SduLength = txSecuredPduPtr->SecOCTxSecuredPduBuffLength;
    }
    else
    {
        pduInfo.SduLength = SecuredIpduLength;
    }
    SchM_Exit_SecOC_TxData();

    if (NULL_PTR == txPduProcessingPtr->SecOCSameBufferPduRef)
    {
        pduInfo.SduDataPtr = txSecuredPduPtr->SecOCTxSecuredPduBufferRef;
    }
    else
    {
        pduInfo.SduDataPtr = txPduProcessingPtr->SecOCSameBufferPduRef->SecOCSecuredPduBufferRef;
    }
    /* @req SWS_SecOC_00062,SWS_SecOC_00067,SWS_SecOC_00072 */
    /* Transmit Secured I-PDU to lower layer */
    (void)PduR_SecOCTransmit(pduId, &pduInfo);

    SchM_Enter_SecOC_TxData();
    txRtPtr->txSecuredPduLen = pduInfo.SduLength;
    txRtPtr->txPduReady = TRUE;
    txRtPtr->txPduStatus = SECOC_TX_TRANS;
/* TP Pdu */
#if (SECOC_TX_TP_PDU_NUM > 0u)
    if (index >= SECOC_TX_IF_PDU_NUM)
    {
        /* Tp data copy offset reinitialization */
        txRtPtr->tpTxPduOffset = 0u;
    }
#endif
    SchM_Exit_SecOC_TxData();
}

/******************************************************************************/
/*
 * Brief               Broadcast Authentic I-PDU to lower layer
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_TxBroadcastAuthenticIPDU(uint16 index)
{
    P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
    txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    P2CONST(SecOC_TxAuthenticPduType, AUTOMATIC, SECOC_CONST)
    txAuthenticPduPtr = SecOC_GetTxAuthenticPduPtr(index);
    P2VAR(SecOC_TxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    txRtPtr = &SecOC_TxRtData[index];
    PduIdType pduId = txAuthenticPduPtr->SecOCTxPduRAsLowerLayerId;
    PduInfoType pduInfo;
    uint8 headerLenInBytes = txAuthenticPduPtr->SecOCAuthPduHeaderLength;
    SchM_Enter_SecOC_TxData();
    pduInfo.SduLength = headerLenInBytes + txRtPtr->txPduLength;
    SchM_Exit_SecOC_TxData();
    if (NULL_PTR == txPduProcessingPtr->SecOCSameBufferPduRef)
    {
        pduInfo.SduDataPtr = txAuthenticPduPtr->SecOCTxColAuthenticPduBufferRef;
    }
    else
    {
        pduInfo.SduDataPtr = txPduProcessingPtr->SecOCSameBufferPduRef->SecOCAuthenticPduBufferRef;
    }
    /* @req SWS_SecOC_00062,SWS_SecOC_00067,SWS_SecOC_00072 */
    /* Transmit Secured I-PDU to lower layer */
    (void)PduR_SecOCTransmit(pduId, &pduInfo);

    SchM_Enter_SecOC_TxData();
    txRtPtr->txAuthenticPduLen = pduInfo.SduLength;
    txRtPtr->txPduReady = TRUE;
    txRtPtr->txPduStatus = SECOC_TX_TRANS;
/* TP Pdu */
#if (SECOC_TX_TP_PDU_NUM > 0u)
    if (index >= SECOC_TX_IF_PDU_NUM)
    {
        /* Tp data copy offset reinitialization */
        txRtPtr->tpTxPduOffset = 0u;
    }
#endif
    SchM_Exit_SecOC_TxData();
}

/******************************************************************************/
/*
 * Brief               Broadcast Cryptographic I-PDU to lower layer
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_TxBroadcastCryptographicIPDU(uint16 index)
{
    P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
    txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    P2CONST(SecOC_TxCryptographicPduType, AUTOMATIC, SECOC_CONST)
    txCryptographicPduPtr = SecOC_GetTxCryptographicPduPtr(index);
    P2CONST(SecOC_UseMessageLinkType, AUTOMATIC, SECOC_CONST)
    messageLinkPtr = SecOC_GetTxUseMessageLinkPtr(index);
    P2VAR(SecOC_TxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    txRtPtr = &SecOC_TxRtData[index];
    PduIdType pduId = txCryptographicPduPtr->SecOCTxPduRAsLowerLayerId;
    PduInfoType pduInfo;
    uint16 msgLinkLen;
    uint16 authInfoTxLength = txPduProcessingPtr->SecOCAuthInfoTruncLength;
    uint8 fvTruncLenInBits = txPduProcessingPtr->SecOCFreshnessValueTruncLength;

    if (NULL_PTR != messageLinkPtr)
    {
        msgLinkLen = messageLinkPtr->SecOCMessageLinkLen;
    }
    else
    {
        msgLinkLen = 0u;
    }
    pduInfo.SduLength = (uint16)SECOC_BIT_TO_BYTE((uint16)fvTruncLenInBits + authInfoTxLength + msgLinkLen);
    if (NULL_PTR == txPduProcessingPtr->SecOCSameBufferPduRef)
    {
        pduInfo.SduDataPtr = txCryptographicPduPtr->SecOCTxCryptographicPduBufferRef;
    }
    else
    {
        pduInfo.SduDataPtr = txPduProcessingPtr->SecOCSameBufferPduRef->SecOCCryptographicPduBufferRef;
    }
    /* @req SWS_SecOC_00062,SWS_SecOC_00067,SWS_SecOC_00072 */
    /* Transmit Secured I-PDU to lower layer */
    (void)PduR_SecOCTransmit(pduId, &pduInfo);

    SchM_Enter_SecOC_TxData();
    txRtPtr->txCryptographicPduLen = pduInfo.SduLength;
    txRtPtr->txPduReady = TRUE;
    txRtPtr->txPduStatus = SECOC_TX_TRANS;
/* TP Pdu */
#if (SECOC_TX_TP_PDU_NUM > 0u)
    if (index >= SECOC_TX_IF_PDU_NUM)
    {
        /* Tp data copy offset reinitialization */
        txRtPtr->tpTxPduOffset = 0u;
    }
#endif
    SchM_Exit_SecOC_TxData();
}

#if (SECOC_TX_TP_PDU_NUM > 0u)
/******************************************************************************/
/*
 * Brief               Acquire TP data from upper layer
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_TxTpAcquireData(uint16 index)
{
    P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
    txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    P2CONST(SecOC_TxPduRTType, AUTOMATIC, SECOC_CONST) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    txRtPtr = &SecOC_TxRtData[index];
    BufReq_ReturnType bufReqRet;
    PduLengthType availableDataLen;
    PduInfoType pduInfo;
    PduIdType pduId;
    uint16 offset = 0u;

    if (SECOC_IS_TX_EVENT(index, SECOC_TX_REQUEST) && !SECOC_IS_TX_EVENT(index, SECOC_TX_DATA_ACQUIRED))
    {
        if (NULL_PTR == txPduProcessingPtr->SecOCSameBufferPduRef)
        {
            pduInfo.SduDataPtr = txPduProcessingPtr->SecOCAuthenticPduBufferRef;
        }
        else
        {
            pduInfo.SduDataPtr = txPduProcessingPtr->SecOCSameBufferPduRef->SecOCAuthenticPduBufferRef;
        }
        SchM_Enter_SecOC_TxData();
        pduInfo.SduLength = txRtPtr->txPduLength;
        SchM_Exit_SecOC_TxData();

        /* @req SWS_SecOC_00254 */
        /* Copy data from upper layer */
        pduId = txPduProcessingPtr->SecOCTxAuthenticPduLayer->SecOCTxPduRAsUpLayerId;
        bufReqRet = PduR_SecOCTpCopyTxData(pduId, &pduInfo, NULL_PTR, &availableDataLen);
        if (BUFREQ_E_BUSY == bufReqRet)
        {
            /* @req SWS_SecOC_00260 */
            /* Retry to acquire data in next main function */
        }
        else if (BUFREQ_E_NOT_OK == bufReqRet)
        {
            /* @req SWS_SecOC_00266 */
            /* Abort transmission*/
            PduR_SecOCTpTxConfirmation(pduId, E_NOT_OK);
            /* Channel Reset */
            SecOC_TxChannelReset(index);
        }
        else
        {
            /* BUFREQ_OK */
            SECOC_SET_TX_EVENT(index, SECOC_TX_DATA_ACQUIRED);
        }
    }
}
#endif

/******************************************************************************/
/*
 * Brief               Reset the runtime data to default value
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_TxChannelReset(uint16 index)
{
    P2CONST(SecOC_TxPduProcessingType, AUTOMATIC, SECOC_CONST)
    txPduProcessingPtr = SecOC_GetTxPduProcessingPtr(index);
    P2VAR(SecOC_TxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    txRtPtr = &SecOC_TxRtData[index];

    SchM_Enter_SecOC_TxData();
    txRtPtr->txPduStatus = SECOC_TX_NONE;
    txRtPtr->txPduLength = 0u;
    txRtPtr->txSecuredPduLen = 0u;
    txRtPtr->txAuthenticPduLen = 0u;
    txRtPtr->txCryptographicPduLen = 0u;
    (void)ILib_memset(txRtPtr->txFreshnessValue, 0, SECOC_MAX_FRESHNESS_SIZE);
    txRtPtr->txPduReady = FALSE;
    txRtPtr->txAuthBuildCnt = 0u;
    (void)ILib_memset(txRtPtr->txResultBuffer, 0, SECOC_MAX_AUTHENTICATOR_LEN);
    txRtPtr->txResultLength = 0u;
    txRtPtr->txDefaultAuthInfoEnable = FALSE;
#if (SECOC_TX_TP_PDU_NUM > 0u)
    txRtPtr->tpTxPduOffset = 0u;
#endif

    /* Unlock Samebuffer */
    if (NULL_PTR != txPduProcessingPtr->SecOCSameBufferPduRef)
    {
        txPduProcessingPtr->SecOCSameBufferPduRef->SecOCSameBufferInUse[0] = FALSE;
    }

    /* Clear tx event */
    SECOC_CLR_TX_EVENT(index, SECOC_EVENT_TX_ALL);
    SchM_Exit_SecOC_TxData();
}
#endif /* SECOC_TX_PDU_NUM > 0u */

#if (SECOC_RX_PDU_NUM > 0u)
/******************************************************************************/
/*
 * Description         Macro function to set the receive event for a specific index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      i: Index for which the receive event needs to be set.
 *                     e: Event value to set.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SECOC_SET_RX_EVENT(uint16 i, uint16 e)
{
    (SecOC_RxEvent[i] |= (e));
}

/******************************************************************************/
/*
 * Description         This function is used to clear a specific receive event.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      i: Index of the event.
 *                     e: Event to be cleared.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SECOC_CLR_RX_EVENT(uint16 i, uint16 e)
{
    (SecOC_RxEvent[i] &= ~(e));
}

/******************************************************************************/
/*
 * Description         This function is used to check if a specific event is a receive event.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      i: Index of the event.
 *                     e: Event to be checked.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean: TRUE if the event is a receive event, FALSE otherwise.
 */
/******************************************************************************/
static FUNC(boolean, SECOC_CODE) SECOC_IS_RX_EVENT(uint16 i, uint16 e)
{
    return ((SecOC_RxEvent[i] & (e)) != 0u) ? TRUE : FALSE;
}

/******************************************************************************/
/*
 * Description         This function is used to retrieve the pointer to the receive PDU Processing for a specific index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      idx: Index for which the pointer to the receive PDU Processing is requested.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              SecOC_RxPduProcessingType: Pointer to the receive PDU Processing.
 */
/******************************************************************************/
static P2CONST(SecOC_RxPduProcessingType, TYPEDEF, SECOC_CONST) SecOC_GetRxPduProcessingPtr(uint16 idx)
{
    return &SecOC_ConfigPtr->SecOCRxPduProcessing[idx];
}

/******************************************************************************/
/*
 * Description         This function is used to retrieve the pointer to the receive Authentic PDU Layer for a specific
 * index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      idx:Index for which the pointer to the receive Authentic PDU Layer is requested.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              SecOC_RxAuthenticPduLayerType: Pointer to the receive Authentic PDU Layer.
 */
/******************************************************************************/
static P2CONST(SecOC_RxAuthenticPduLayerType, AUTOMATIC, SECOC_APPL_DATA) SecOC_GetRxAuthenticPduLayerPtr(uint16 idx)
{
    return (SecOC_GetRxPduProcessingPtr(idx)->SecOCRxAuthenticPduLayer);
}

/******************************************************************************/
/*
 * Description         This function is used to retrieve the pointer to the receive Secured PDU Layer for a specific
 * index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      idx: Index for which the pointer to the receive Secured PDU Layer is requested.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              SecOC_RxSecuredPduLayerType: Pointer to the receive Secured PDU Layer.
 */
/******************************************************************************/
static P2CONST(SecOC_RxSecuredPduLayerType, TYPEDEF, SECOC_CONST) SecOC_GetRxSecuredPduLayerPtr(uint16 idx)
{
    return (SecOC_GetRxPduProcessingPtr(idx)->SecOCRxSecuredPduLayer);
}

/******************************************************************************/
/*
 * Description         This function is used to retrieve the pointer to the receive secured PDU for a specific index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      idx: Index for which the pointer to the receive secured PDU is requested.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              SecOC_RxSecuredPduType: Pointer to the receive secured PDU.
 */
/******************************************************************************/
static P2CONST(SecOC_RxSecuredPduType, AUTOMATIC, SECOC_CONST) SecOC_GetRxSecuredPduPtr(uint16 idx)
{
    return (SecOC_GetRxSecuredPduLayerPtr(idx)->SecOCRxSecuredPdu);
}

/******************************************************************************/
/*
 * Description         This function is used to retrieve the pointer to the receive Secured PDU Collection for a
 * specific index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      idx: Index for which the pointer to the receive Secured PDU Collection is requested.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              SecOC_RxSecuredPduCollectionType: Pointer to the receive Secured PDU Collection.
 */
/******************************************************************************/
static P2CONST(SecOC_RxSecuredPduCollectionType, AUTOMATIC, SECOC_CONST) SecOC_GetRxSecuredPduCollectionPtr(uint16 idx)
{
    return (SecOC_GetRxSecuredPduLayerPtr(idx)->SecOCRxSecuredPduCollection);
}

/******************************************************************************/
/*
 * Description         This function is used to retrieve the pointer to the receive Authentic PDU for a specific index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      idx: Index for which the pointer to the receive Authentic PDU is requested.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              SecOC_RxAuthenticPduType: Pointer to the receive Authentic PDU.
 */
/******************************************************************************/
static P2CONST(SecOC_RxAuthenticPduType, AUTOMATIC, SECOC_CONST) SecOC_GetRxAuthenticPduPtr(uint16 idx)
{
    return (SecOC_GetRxSecuredPduCollectionPtr(idx)->SecOCRxAuthenticPdu);
}

/******************************************************************************/
/*
 * Description         This function is used to retrieve the pointer to the receive Cryptographic PDU for a specific
 * index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      idx:Index for which the pointer to the receive Cryptographic PDU is requested.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              SecOC_RxCryptographicPduType: Pointer to the receive Cryptographic PDU.
 */
/******************************************************************************/
static P2CONST(SecOC_RxCryptographicPduType, AUTOMATIC, SECOC_CONST) SecOC_GetRxCryptographicPduPtr(uint16 idx)
{
    return (SecOC_GetRxSecuredPduCollectionPtr(idx)->SecOCRxCryptographicPdu);
}

#if (SECOC_RX_IF_PDU_NUM > 0u)
/******************************************************************************/
/*
 * Brief               Secured IPDU receive indication process
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 *                     RxPduId: ID of the received PDU.
 *                     PduInfoPtr: Contains the length (SduLength) of the
 *                                 received PDU,a pointer to a buffer
 *                                 (SduDataPtr)containing the PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_RxIfSecuredPduIndication(
    uint16 index,
    PduIdType RxPduId,
    P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) PduInfoPtr)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2CONST(SecOC_SameBufferPduCollectionType, AUTOMATIC, SECOC_CONST)
    sameBufferPtr = rxPduProcessingPtr->SecOCSameBufferPduRef;
    P2CONST(SecOC_RxSecuredPduType, AUTOMATIC, SECOC_CONST) rxSecuredPduPtr;
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) destPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    PduLengthType interBufLen;
    PduInfoType pduInfo;

    rxSecuredPduPtr = SecOC_GetRxSecuredPduPtr(index);
    if ((TRUE == rxSecuredPduPtr->SecOCDynamicLength)
        || ((FALSE == rxSecuredPduPtr->SecOCDynamicLength)
            && (PduInfoPtr->SduLength >= rxSecuredPduPtr->SecOCPduLength)))
    {
        pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
        pduInfo.SduLength = PduInfoPtr->SduLength;
        /* @req SWS_SecOC_00078 */
        /* SecOC module shall copy the I-PDU into the according buffer according
         * to the minimum of received length and configured length of this I-PDU
         */
        if (rxSecuredPduPtr->SecOCPduLength < pduInfo.SduLength)
        {
            pduInfo.SduLength = rxSecuredPduPtr->SecOCPduLength;
        }

        if (SECOC_RX_NONE != rxRtPtr->rxPduStatus)
        {
            switch (rxPduProcessingPtr->SecOCReceptionOverflowStrategy)
            {
            case SECOC_REJECT:
                /* @req SWS_SecOC_00215 */
                /* The Secured I-PDU is processing, Ignore the new request*/
                break;

            case SECOC_REPLACE:
                /* @req SWS_SecOC_00214 */
                /* Free all buffer related to the Secured I-PDU,Rx restart*/
                SecOC_RxChannelReset(index);
                break;

            case SECOC_QUEUE:
                /* @req SWS_SecOC_00216 */
                /* Queue the Secured I-PDU for a subsequent processing */
                SecOC_DataInQueue(index, RxPduId, &pduInfo);
                break;

            default:
                /* Nothing to do here*/
                break;
            }
        }

        if (SECOC_RX_NONE == rxRtPtr->rxPduStatus)
        {
            if (NULL_PTR == sameBufferPtr)
            {
                destPtr = rxSecuredPduPtr->SecOCRxSecuredPduBufferRef;
                interBufLen = rxSecuredPduPtr->SecOCRxSecuredPduBuffLength;
            }
            else
            {
                destPtr = sameBufferPtr->SecOCSecuredPduBufferRef;
                interBufLen = sameBufferPtr->SecOCSecuredPduBuffLength;
            }

            if (interBufLen >= pduInfo.SduLength)
            {
                if ((NULL_PTR == sameBufferPtr) || (FALSE == sameBufferPtr->SecOCSameBufferInUse[0u]))
                {
                    SchM_Enter_SecOC_RxData();
                    (void)ILib_memcpy(destPtr, pduInfo.SduDataPtr, pduInfo.SduLength);

                    SECOC_SET_RX_EVENT(index, SECOC_RX_INDICATION);
                    SECOC_SET_RX_EVENT(index, SECOC_RX_SECURED_PDU);
                    rxRtPtr->rxPduStatus = SECOC_RX_CALC;
                    rxRtPtr->rxSecuredPduLen = pduInfo.SduLength;
                    /* @req SWS_SecOC_00235 */
                    rxRtPtr->rxVerifyAttempts = 0u;
                    rxRtPtr->rxAuthBuildAttempts = 0u;
                    if (NULL_PTR != sameBufferPtr)
                    {
                        sameBufferPtr->SecOCSameBufferInUse[0u] = TRUE;
                    }
                    SchM_Exit_SecOC_RxData();
                }
            }
        }
    }
    else
    {
        /* @req SWS_SecOC_00268 */
        /* Received length is smaller than configured length, discard the IPDU*/
    }
}

/******************************************************************************/
/*
 * Brief               Authentic IPDU receive indication process
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 *                     PduInfoPtr: Contains the length (SduLength) of the
 *                                 received PDU,a pointer to a buffer
 *                                 (SduDataPtr)containing the PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE)
    SecOC_RxIfAuthenticPduIndication(uint16 index, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) PduInfoPtr)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2CONST(SecOC_SameBufferPduCollectionType, AUTOMATIC, SECOC_CONST)
    sameBufferPtr = rxPduProcessingPtr->SecOCSameBufferPduRef;
    P2CONST(SecOC_RxAuthenticPduType, AUTOMATIC, SECOC_CONST) rxAuthenticPduPtr;
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) destPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    PduLengthType interBufLen;
    PduInfoType pduInfo;

    rxAuthenticPduPtr = SecOC_GetRxAuthenticPduPtr(index);
    if ((TRUE == rxAuthenticPduPtr->SecOCDynamicLength)
        || ((FALSE == rxAuthenticPduPtr->SecOCDynamicLength)
            && (PduInfoPtr->SduLength >= rxAuthenticPduPtr->SecOCPduLength)))
    {
        pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
        pduInfo.SduLength = PduInfoPtr->SduLength;
        /* @req SWS_SecOC_00078 */
        /* SecOC module shall copy the I-PDU into the according buffer according
         * to the minimum of received length and configured length of this I-PDU
         */
        if (rxAuthenticPduPtr->SecOCPduLength < pduInfo.SduLength)
        {
            pduInfo.SduLength = rxAuthenticPduPtr->SecOCPduLength;
        }

        if (SECOC_RX_NONE != rxRtPtr->rxPduStatus)
        {
            switch (rxPduProcessingPtr->SecOCReceptionOverflowStrategy)
            {
            case SECOC_REJECT:
                /* @req SWS_SecOC_00215 */
                /* The Secured I-PDU is processing, Ignore the new request*/
                break;

            case SECOC_REPLACE:
                /* @req SWS_SecOC_00214 */
                /* Free all buffer related to the Secured I-PDU,Rx restart*/
                SecOC_RxChannelReset(index);
                break;

            case SECOC_QUEUE:
                /* @req SWS_SecOC_CONSTR_00265 */
                break;

            default:
                /* Nothing to do here*/
                break;
            }
        }

        if (SECOC_RX_NONE == rxRtPtr->rxPduStatus)
        {
            if (NULL_PTR == sameBufferPtr)
            {
                destPtr = rxAuthenticPduPtr->SecOCRxColAuthenticPduBufferRef;
                interBufLen = rxAuthenticPduPtr->SecOCRxColAuthenticPduBuffLength;
            }
            else
            {
                destPtr = sameBufferPtr->SecOCColAuthenticPduBufferRef;
                interBufLen = sameBufferPtr->SecOCColAuthenticPduBuffLength;
            }

            if (interBufLen >= pduInfo.SduLength)
            {
                if ((NULL_PTR == sameBufferPtr) || (FALSE == sameBufferPtr->SecOCSameBufferInUse[0u]))
                {
                    SchM_Enter_SecOC_RxData();
                    (void)ILib_memcpy(destPtr, pduInfo.SduDataPtr, pduInfo.SduLength);

                    /* @req SWS_SecOC_00203,SWS_SecOC_00208 */
                    SECOC_SET_RX_EVENT(index, SECOC_RX_INDICATION);
                    SECOC_SET_RX_EVENT(index, SECOC_RX_AUTHENTIC_PDU);
                    if (SECOC_IS_RX_EVENT(index, SECOC_RX_CRYPTOGRAPHIC_PDU))
                    {
                        rxRtPtr->rxPduStatus = SECOC_RX_CALC;
                    }
                    rxRtPtr->rxAuthenticPduLen = pduInfo.SduLength;
                    rxRtPtr->rxVerifyAttempts = 0u;
                    if (NULL_PTR != sameBufferPtr)
                    {
                        sameBufferPtr->SecOCSameBufferInUse[0u] = TRUE;
                    }
                    SchM_Exit_SecOC_RxData();
                }
            }
        }
    }
    else
    {
        /* @req SWS_SecOC_00268 */
        /* Received length is smaller than configured length, discard the IPDU*/
    }
}

/******************************************************************************/
/*
 * Brief               Cryptographic IPDU receive indication process
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 *                     PduInfoPtr: Contains the length (SduLength) of the
 *                                 received PDU,a pointer to a buffer
 *                                 (SduDataPtr)containing the PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE)
    SecOC_RxIfCryptographicPduIndication(uint16 index, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) PduInfoPtr)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2CONST(SecOC_SameBufferPduCollectionType, AUTOMATIC, SECOC_CONST)
    sameBufferPtr = rxPduProcessingPtr->SecOCSameBufferPduRef;
    P2CONST(SecOC_RxCryptographicPduType, AUTOMATIC, SECOC_CONST)
    rxCryptographicPduPtr;
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) destPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    PduLengthType interBufLen;
    PduInfoType pduInfo;

    rxCryptographicPduPtr = SecOC_GetRxCryptographicPduPtr(index);
    if ((TRUE == rxCryptographicPduPtr->SecOCDynamicLength)
        || ((FALSE == rxCryptographicPduPtr->SecOCDynamicLength)
            && (PduInfoPtr->SduLength >= rxCryptographicPduPtr->SecOCPduLength)))
    {
        pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
        pduInfo.SduLength = PduInfoPtr->SduLength;
        /* @req SWS_SecOC_00078 */
        /* SecOC module shall copy the I-PDU into the according buffer according
         * to the minimum of received length and configured length of this I-PDU
         */
        if (rxCryptographicPduPtr->SecOCPduLength < pduInfo.SduLength)
        {
            pduInfo.SduLength = rxCryptographicPduPtr->SecOCPduLength;
        }

        if (SECOC_RX_NONE != rxRtPtr->rxPduStatus)
        {
            switch (rxPduProcessingPtr->SecOCReceptionOverflowStrategy)
            {
            case SECOC_REJECT:
                /* @req SWS_SecOC_00215 */
                /* The Secured I-PDU is processing, Ignore the new request*/
                break;

            case SECOC_REPLACE:
                /* @req SWS_SecOC_00214 */
                /* Free all buffer related to the Secured I-PDU,Rx restart*/
                SecOC_RxChannelReset(index);
                break;

            case SECOC_QUEUE:
                /* @req SWS_SecOC_CONSTR_00265 */
                break;

            default:
                /* Nothing to do here*/
                break;
            }
        }

        if (SECOC_RX_NONE == rxRtPtr->rxPduStatus)
        {
            if (NULL_PTR == sameBufferPtr)
            {
                destPtr = rxCryptographicPduPtr->SecOCRxCryptographicPduBufferRef;
                interBufLen = rxCryptographicPduPtr->SecOCRxCryptographicPduBuffLength;
            }
            else
            {
                destPtr = sameBufferPtr->SecOCCryptographicPduBufferRef;
                interBufLen = sameBufferPtr->SecOCCryptographicPduBuffLength;
            }

            if (interBufLen >= pduInfo.SduLength)
            {
                if ((NULL_PTR == sameBufferPtr) || (FALSE == sameBufferPtr->SecOCSameBufferInUse[0u]))
                {
                    SchM_Enter_SecOC_RxData();
                    (void)ILib_memcpy(destPtr, pduInfo.SduDataPtr, pduInfo.SduLength);

                    /* @req SWS_SecOC_00203,SWS_SecOC_00208 */
                    SECOC_SET_RX_EVENT(index, SECOC_RX_INDICATION);
                    SECOC_SET_RX_EVENT(index, SECOC_RX_CRYPTOGRAPHIC_PDU);
                    if (SECOC_IS_RX_EVENT(index, SECOC_RX_AUTHENTIC_PDU))
                    {
                        rxRtPtr->rxPduStatus = SECOC_RX_CALC;
                    }
                    rxRtPtr->rxCryptographicPduLen = pduInfo.SduLength;
                    rxRtPtr->rxVerifyAttempts = 0u;
                    if (NULL_PTR != sameBufferPtr)
                    {
                        sameBufferPtr->SecOCSameBufferInUse[0u] = TRUE;
                    }
                    SchM_Exit_SecOC_RxData();
                }
            }
        }
    }
    else
    {
        /* @req SWS_SecOC_00268 */
        /* Received length is smaller than configured length, discard the IPDU*/
    }
}
#endif /* SECOC_RX_IF_PDU_NUM > 0u */

/******************************************************************************/
/*
 * Brief               Parse the received Secured I-PDU
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_RxParseSecuredIpdu(uint16 index)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2CONST(SecOC_RxAuthenticPduLayerType, AUTOMATIC, SECOC_CONST) rxAuthenticPduLayerPtr;
    P2CONST(SecOC_SameBufferPduCollectionType, AUTOMATIC, SECOC_CONST)
    sameBufferPtr = rxPduProcessingPtr->SecOCSameBufferPduRef;
    P2CONST(SecOC_RxSecuredPduType, AUTOMATIC, SECOC_CONST)
    rxSecuredPduPtr = SecOC_GetRxSecuredPduPtr(index);
    P2CONST(SecOC_RxAuthenticPduType, AUTOMATIC, SECOC_CONST) rxAuthenticPduPtr;
    P2CONST(SecOC_RxCryptographicPduType, AUTOMATIC, SECOC_CONST)
    rxCryptographicPduPtr;
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) rxSecuredPduBufPtr; /* PRQA S 3432 */       /* MISRA Rule 20.7 */
    P2CONST(uint8, AUTOMATIC, SECOC_CONST) rxAuthenticPduBufPtr; /* PRQA S 3432 */       /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) rxCryptographicPduBufPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    PduLengthType authPduLen;
    PduLengthType offset;
    PduLengthType cfgAuthPduLen = rxPduProcessingPtr->SecOCRxAuthenticPduLayer->SecOCPduLength;
    uint8 headerLen;
    boolean securedRxPduVerification;
    boolean dropMsgFlag = FALSE;
    uint8 fvTruncLenInBits = rxPduProcessingPtr->SecOCFreshnessValueTruncLength;
    uint8 fvLenInBytes = (uint8)SECOC_BIT_TO_BYTE((uint16)fvTruncLenInBits);

    /* @req SWS_SecOC_00203,SWS_SecOC_00208 */
    /* If SecOCRxSecuredPduCollection is used then SecOC shall not perform any
     * verification until it has received both the Authentic I-PDU and
     * Cryptographic I-PDU which make up the Secured I-PDU.*/
    if ((SECOC_IS_RX_EVENT(index, SECOC_RX_INDICATION) && SECOC_IS_RX_EVENT(index, SECOC_RX_SECURED_PDU))
        || (SECOC_IS_RX_EVENT(index, SECOC_RX_INDICATION) && SECOC_IS_RX_EVENT(index, SECOC_RX_AUTHENTIC_PDU)
            && SECOC_IS_RX_EVENT(index, SECOC_RX_CRYPTOGRAPHIC_PDU))
        || (((SECOC_OVERRIDE_SKIP_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus) && (rxRtPtr->rxNumOfMsgToOverride > 0u))
            || (SECOC_OVERRIDE_SKIP_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)))
    {
#if (SECOC_MAX_MSG_LINK_LEN > 0u)
        /* @req SWS_SecOC_00211 */
        if (E_OK == SecOC_RxMessageLinkCheck(index))
#endif /* SECOC_MAX_MSG_LINK_LEN > 0u */
        {
            /* @req SWS_SecOC_00042 */
            /* Upon reception of a secured I-PDU,SecOC shall parse the Authentic
             * I-PDU, the Freshness Value and the Authenticator from it. */
            if (NULL_PTR != rxSecuredPduPtr)
            {
                securedRxPduVerification = rxSecuredPduPtr->SecOCSecuredRxPduVerification;
                if (NULL_PTR == sameBufferPtr)
                {
                    rxRtPtr->rxParsedAuthPduBufPtr = rxPduProcessingPtr->SecOCAuthenticPduBufferRef;
                    rxSecuredPduBufPtr = rxSecuredPduPtr->SecOCRxSecuredPduBufferRef;
                }
                else
                {
                    rxRtPtr->rxParsedAuthPduBufPtr = sameBufferPtr->SecOCAuthenticPduBufferRef;
                    rxSecuredPduBufPtr = sameBufferPtr->SecOCSecuredPduBufferRef;
                }

                headerLen = rxSecuredPduPtr->SecOCAuthPduHeaderLength;
                if (0u == headerLen)
                {
                    /* @req SWS_SecOC_00257 */
                    /* Extract the Authentic I-PDU use configured length */
                    if (FALSE == rxSecuredPduPtr->SecOCDynamicLength)
                    {
                        rxAuthenticPduLayerPtr = SecOC_GetRxAuthenticPduLayerPtr(index);
                        authPduLen = rxAuthenticPduLayerPtr->SecOCPduLength;
                    }
                    else
                    {
                        /* @req SWS_SecOC_00258 */
                        /* Extract the Authentic I-PDU use received length */
                        authPduLen = rxRtPtr->rxSecuredPduLen - rxPduProcessingPtr->SecOCAuthFrsInfoLength;
                    }
                }
                else
                {
                    /* @req SWS_SecOC_00259 */
                    /* Extract the Authentic I-PDU use runtime length */
                    authPduLen = SecOC_RxGetPduLength(headerLen, rxSecuredPduBufPtr);
                    /* @req SWS_SecOC_00263 */
                    /* Runtime length of Authentic I-PDU in the Header is longer
                     * than configured length,Discard the IPDU */
                    if (authPduLen > cfgAuthPduLen)
                    {
                        dropMsgFlag = TRUE;
                    }
                }

                if (FALSE == dropMsgFlag)
                {
                    SchM_Enter_SecOC_RxData();
                    offset = headerLen;
                    /* Parse Authentic PDU */
                    (void)ILib_memcpy(rxRtPtr->rxParsedAuthPduBufPtr, &rxSecuredPduBufPtr[offset], authPduLen);
                    offset += authPduLen;
                    /* Parse Freshness value */
                    (void)ILib_memcpy(
                        rxRtPtr->rxParsedFreshnessValue,
                        &rxSecuredPduBufPtr[offset],
                        SECOC_BIT_TO_BYTE((uint16)rxPduProcessingPtr->SecOCFreshnessValueTruncLength));
                    if (0u != SECOC_MOD_FROM_EIGHT(fvTruncLenInBits))
                    {
                        SecOC_ArrayShiftRight(
                            rxRtPtr->rxParsedFreshnessValue,
                            fvLenInBytes,
                            (8u - (uint16)SECOC_MOD_FROM_EIGHT((uint16)fvTruncLenInBits)));
                    }
                    /* Parse Authenticator */
                    SecOC_ArrayShiftLeft(
                        &rxSecuredPduBufPtr[offset],
                        rxPduProcessingPtr->SecOCAuthFrsInfoLength,
                        rxPduProcessingPtr->SecOCFreshnessValueTruncLength);
                    (void)ILib_memcpy(
                        rxRtPtr->rxParsedAuthenticator,
                        &rxSecuredPduBufPtr[offset],
                        (uint32)SECOC_BIT_TO_BYTE((uint16)rxPduProcessingPtr->SecOCAuthInfoTruncLength));
                    SchM_Exit_SecOC_RxData();
                }
            }
            else
            {
                rxAuthenticPduPtr = SecOC_GetRxAuthenticPduPtr(index);
                rxCryptographicPduPtr = SecOC_GetRxCryptographicPduPtr(index);
                securedRxPduVerification = rxPduProcessingPtr->SecOCRxSecuredPduLayer->SecOCRxSecuredPduCollection
                                               ->SecOCSecuredRxPduVerification;

                if (NULL_PTR == sameBufferPtr)
                {
                    rxRtPtr->rxParsedAuthPduBufPtr = rxPduProcessingPtr->SecOCAuthenticPduBufferRef;
                    rxAuthenticPduBufPtr = rxAuthenticPduPtr->SecOCRxColAuthenticPduBufferRef;
                    rxCryptographicPduBufPtr = rxCryptographicPduPtr->SecOCRxCryptographicPduBufferRef;
                }
                else
                {
                    rxRtPtr->rxParsedAuthPduBufPtr = sameBufferPtr->SecOCAuthenticPduBufferRef;
                    rxAuthenticPduBufPtr = sameBufferPtr->SecOCColAuthenticPduBufferRef;
                    rxCryptographicPduBufPtr = sameBufferPtr->SecOCCryptographicPduBufferRef;
                }

                headerLen = rxAuthenticPduPtr->SecOCAuthPduHeaderLength;
                if (0u == headerLen)
                {
                    /* @req SWS_SecOC_00257 */
                    /* Extract the Authentic I-PDU use configured length */
                    if (FALSE == rxAuthenticPduPtr->SecOCDynamicLength)
                    {
                        authPduLen = rxAuthenticPduPtr->SecOCPduLength;
                    }
                    else
                    {
                        /* @req SWS_SecOC_00258 */
                        /* Extract the Authentic I-PDU use received length */
                        authPduLen = rxRtPtr->rxAuthenticPduLen;
                    }
                }
                else
                {
                    /* @req SWS_SecOC_00259 */
                    /* Extract the Authentic I-PDU use runtime length */
                    authPduLen = SecOC_RxGetPduLength(headerLen, rxAuthenticPduBufPtr);
                    /* @req SWS_SecOC_00263 */
                    /* Runtime length of Authentic I-PDU in the Header is longer
                     * than configured length,Discard the IPDU */
                    if (authPduLen > cfgAuthPduLen)
                    {
                        dropMsgFlag = TRUE;
                    }
                }

                if (FALSE == dropMsgFlag)
                {
                    offset = headerLen;
                    SchM_Enter_SecOC_RxData();
                    /* Parse Authentic PDU */
                    (void)ILib_memcpy(rxRtPtr->rxParsedAuthPduBufPtr, &rxAuthenticPduBufPtr[offset], authPduLen);

                    if (SECOC_IS_RX_EVENT(index, SECOC_RX_CRYPTOGRAPHIC_PDU))
                    {
                        /* Parse Freshness value */
                        (void)ILib_memcpy(
                            rxRtPtr->rxParsedFreshnessValue,
                            rxCryptographicPduBufPtr,
                            SECOC_BIT_TO_BYTE((uint16)rxPduProcessingPtr->SecOCFreshnessValueTruncLength));
                        if (0u != SECOC_MOD_FROM_EIGHT(fvTruncLenInBits))
                        {
                            SecOC_ArrayShiftRight(
                                rxRtPtr->rxParsedFreshnessValue,
                                fvLenInBytes,
                                (8u - (uint16)SECOC_MOD_FROM_EIGHT((uint16)fvTruncLenInBits)));
                        }
                        /* Parse Authenticator */
                        SecOC_ArrayShiftLeft(
                            rxCryptographicPduBufPtr,
                            rxPduProcessingPtr->SecOCAuthFrsInfoLength,
                            rxPduProcessingPtr->SecOCFreshnessValueTruncLength);
                        (void)ILib_memcpy(
                            rxRtPtr->rxParsedAuthenticator,
                            rxCryptographicPduBufPtr,
                            SECOC_BIT_TO_BYTE((uint16)rxPduProcessingPtr->SecOCAuthInfoTruncLength));
                    }
                    SchM_Exit_SecOC_RxData();
                }
            }
            /* Save Parsed Authentic Pdu Length */
            rxRtPtr->rxParsedAuthPduLen = authPduLen;

            SchM_Enter_SecOC_RxData();
            if (TRUE == securedRxPduVerification)
            {
                SECOC_SET_RX_EVENT(index, SECOC_RX_PARSED_MESSAGE);
            }
            else
            {
                /* @req SWS_SecOC_00265 */
                /* SecOC extract the Authentic I-PDU without Authentication*/
                SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);
            }
            SchM_Exit_SecOC_RxData();
        }
#if (SECOC_MAX_MSG_LINK_LEN > 0u)
        else
        {
            /* Message Link check not passed */
            /* @req SWS_SecOC_00208 */
            /* Use SecOCRxSecuredPduCollection,check may passed when
             * Authentic I-PDU or Cryptographic PDU Update. */
            rxRtPtr->rxPduStatus = SECOC_RX_NONE;
        }
#endif /* SECOC_MAX_MSG_LINK_LEN > 0u */

        if (TRUE == dropMsgFlag)
        {
            /* Channel reset */
            SecOC_RxChannelReset(index);
        }
        SchM_Enter_SecOC_RxData();
        SECOC_CLR_RX_EVENT(index, SECOC_RX_INDICATION);
        SchM_Exit_SecOC_RxData();
    }
    else
    {
        rxRtPtr->rxPduStatus = SECOC_RX_NONE;
    }
}

/******************************************************************************/
/*
 * Brief               Verify process
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_RxVerifyProcee(uint16 index)
{
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];

    if (SECOC_IS_RX_EVENT(index, SECOC_RX_PARSED_MESSAGE))
    {
        /* Handle numberOfMessageOverride counter */
        rxRtPtr->rxBakNumOfMsgToOverride = rxRtPtr->rxNumOfMsgToOverride;

        if (((SECOC_OVERRIDE_DROP_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus) && (rxRtPtr->rxNumOfMsgToOverride == 0u))
            || (SECOC_OVERRIDE_CANCEL == rxRtPtr->rxOverrideStatus)
            || (SECOC_OVERRIDE_PASS_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)
            || ((SECOC_OVERRIDE_SKIP_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus) && (rxRtPtr->rxNumOfMsgToOverride == 0u))
            || (SECOC_OVERRIDE_PASS_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus))
        {
            /* Performe authenticator verification */
            SecOC_RxVerifyAuthenticationInfo(index);
        }
        else
        {
            /* Not performe authenticator verification */
            SECOC_CLR_RX_EVENT(index, SECOC_RX_PARSED_MESSAGE);
            rxRtPtr->rxVeryfyResult = SECOC_NO_VERIFICATION;

            if ((SECOC_OVERRIDE_SKIP_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
                || (SECOC_OVERRIDE_SKIP_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus))
            {
                /* I-PDU is sent to upper layer */
                SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);
            }
            else
            {
                /* Drop message */
                /* Report verify status */
                SecOC_RxReportVerifyStatus(index);
            }
        }

        /* Handle numberOfMessageOverride counter */
        if ((rxRtPtr->rxNumOfMsgToOverride > 0u) && (SECOC_RX_RETRY != rxRtPtr->rxPduStatus))
        {
            rxRtPtr->rxNumOfMsgToOverride--;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Verify the received Secured I-PDU
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_RxVerifyAuthenticationInfo(uint16 index)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];
    P2CONST(SecOC_CsmJobType, TYPEDEF, SECOC_CONST)
    jobPtr = rxPduProcessingPtr->SecOCRxAuthServiceConfigRef;
    uint32 dataLength;
    Std_ReturnType ret;

    if (SECOC_IS_RX_EVENT(index, SECOC_RX_PARSED_MESSAGE))
    {
        dataLength = SecOC_RxCalcDataToAuthenticatorLength(index);
        ret = SecOC_RxConstructDataToAuthenticator(index);
        if (E_OK == ret)
        {
            /* @req SWS_SecOC_00047 */
            switch (jobPtr->SecOCCsmPrimitives)
            {
#if (SECOC_CSMMAC_ENABLED == STD_ON)
            case SECOC_CSMMAC:
                ret = Csm_MacVerify(
                    jobPtr->SecOCJobId,
                    CRYPTO_OPERATIONMODE_SINGLECALL,
                    &SecOC_DataToAuthenticator[0u],
                    dataLength,
                    rxRtPtr->rxParsedAuthenticator,
                    rxPduProcessingPtr->SecOCAuthInfoTruncLength,
                    &rxRtPtr->rxVerifyResult);
                break;
#endif /* SECOC_CSMMAC_ENABLED == STD_ON */

#if (SECOC_CSMSIGNATRUE_ENABLED == STD_ON)
            case SECOC_CSMSIGNATURE:
                ret = Csm_SignatureVerify(
                    jobPtr->SecOCJobId,
                    CRYPTO_OPERATIONMODE_SINGLECALL,
                    &SecOC_DataToAuthenticator[0u],
                    dataLength,
                    rxRtPtr->rxParsedAuthenticator,
                    SECOC_BIT_TO_BYTE((uint16)rxPduProcessingPtr->SecOCAuthInfoTruncLength),
                    &rxRtPtr->rxVerifyResult);
                break;
#endif /* SECOC_CSMSIGNATRUE_ENABLED == STD_ON */

            default:
                /* Nothing to do here*/
                break;
            }

            if (E_OK == ret)
            {
                SECOC_CLR_RX_EVENT(index, SECOC_RX_PARSED_MESSAGE);
                if (SECOC_CRYPTO_PROCESSING_SYNC == jobPtr->SecOCCryptoProcessingType)
                {
                    SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);

                    /* @req SWS_SecOC_00239 */
                    /* Verification of the authenticator successfully executed,
                     * the authentication build counter shall be set to 0. */
                    rxRtPtr->rxAuthBuildAttempts = 0u;
                }
                else
                {
                    SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_WAITING);
                }
            }
            else if (CRYPTO_E_BUSY == ret)
            {
                /* @req SWS_SecOC_00121,SWS_SecOC_00237 */
                /* Recoverable error */
                SecOC_RxAuthenticationBuildRetry(index);
            }
            else
            {
                /* No-recoverable error */
                /* @req SWS_SecOC_00241 */
                if ((SECOC_OVERRIDE_PASS_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)
                    || ((SECOC_OVERRIDE_PASS_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
                        && (rxRtPtr->rxBakNumOfMsgToOverride > 0u))
                    || (*SecOC_ConfigPtr->SecOCIgnoreVerificationResultRef == TRUE))
                {
                    /* Continue to send authentic I-PDU to upper layer */
                    SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);
                }
                else
                {
                    rxRtPtr->rxVeryfyResult = SECOC_VERIFICATIONFAILURE;
                    /* Report verify status */
                    SecOC_RxReportVerifyStatus(index);

                    /* Non-recoverable error occur */
                    SecOC_RxChannelReset(index);
                }
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief               Calculate the length of DataToAuthenticator
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(uint32, SECOC_CODE) SecOC_RxCalcDataToAuthenticatorLength(uint16 index)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2CONST(SecOC_RxPduRTType, AUTOMATIC, SECOC_CONST)
    rxRtPtr = &SecOC_RxRtData[index];
    uint32 dataLength = SECOC_DATAID_LEN;

    if (NULL_PTR != rxPduProcessingPtr->SecOCRxPduSecuredArea)
    {
        /* Use part of the Authentic I-PDU to construct DataToAuthenticator*/
        dataLength = dataLength + rxPduProcessingPtr->SecOCRxPduSecuredArea->SecOCSecuredRxPduLength;
    }
    else
    {
        /* Use the complete Authentic I-PDU to construct DataToAuthenticator*/
        dataLength = dataLength + rxRtPtr->rxParsedAuthPduLen;
    }

    dataLength += SECOC_BIT_TO_BYTE((uint16)rxPduProcessingPtr->SecOCFreshnessValueLength);

    return dataLength;
}

/******************************************************************************/
/*
 * Brief               Construct DataToAuthenticator
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Construct DataToAuthenticator success
 *                     E_NOT_OK: Construct DataToAuthenticator fail
 */
/******************************************************************************/
static FUNC(Std_ReturnType, SECOC_CODE) SecOC_RxConstructDataToAuthenticator(uint16 index)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2CONST(SecOC_SameBufferPduCollectionType, AUTOMATIC, SECOC_CONST)
    sameBufferPtr = rxPduProcessingPtr->SecOCSameBufferPduRef;
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];
    P2CONST(SecOC_RxSecuredPduType, AUTOMATIC, SECOC_CONST)
    rxSecuredPduPtr = SecOC_GetRxSecuredPduPtr(index);
    P2CONST(SecOC_RxAuthenticPduType, AUTOMATIC, SECOC_CONST) rxAuthenticPduPtr;
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) authenticPduPtr; /* PRQA S 3432 */    /* MISRA Rule 20.7 */
    P2CONST(uint8, AUTOMATIC, SECOC_CONST) authDataFreshnessPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    uint32 securedIPDULength;
    uint32 securedIPDUOffset;
    uint32 freshnessValueLen;
    uint16 dataId;
    uint8 headerLen;
    Std_ReturnType ret;

    SchM_Enter_SecOC_Authentication();

    (void)ILib_memset(SecOC_DataToAuthenticator, 0, SECOC_DATA_TO_AUTHENTICATOR_LENGTH);
    /* @req SWS_SecOC_00046 */
    /*
     * DataToAuthenticator = Data Identifier | secured part of the Authentic
     * I-PDU | Complete Freshness Value
     */

    /* Data Identifie */
    dataId = rxPduProcessingPtr->SecOCDataId;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    SecOC_EndianSwap((uint8*)&dataId, sizeof(dataId));
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
    (void)ILib_memcpy(&SecOC_DataToAuthenticator[0], (uint8*)&dataId, SECOC_DATAID_LEN);

    /* Secured part of the Authentic */
    if (NULL_PTR != rxPduProcessingPtr->SecOCRxPduSecuredArea)
    {
        /* Use part of the Authentic I-PDU to construct DataToAuthenticator*/
        securedIPDULength = rxPduProcessingPtr->SecOCRxPduSecuredArea->SecOCSecuredRxPduLength;
        securedIPDUOffset = rxPduProcessingPtr->SecOCRxPduSecuredArea->SecOCSecuredRxPduOffset;
    }
    else
    {
        /* Use the complete Authentic I-PDU to construct
         * DataToAuthenticator.*/
        securedIPDULength = rxRtPtr->rxParsedAuthPduLen;
        securedIPDUOffset = 0u;
    }

    (void)ILib_memcpy(
        &SecOC_DataToAuthenticator[SECOC_DATAID_LEN],
        &rxRtPtr->rxParsedAuthPduBufPtr[securedIPDUOffset],
        securedIPDULength);

    SchM_Exit_SecOC_Authentication();
    /* Complete Freshness Value */
    /* @req SWS_SecOC_00219 */
    /* SecOC use a part of the Authentic I-PDU as freshness */
    if (rxPduProcessingPtr->SecOCAuthDataFreshnessLen > 0u)
    {
        if (NULL_PTR != rxSecuredPduPtr)
        {
            headerLen = rxSecuredPduPtr->SecOCAuthPduHeaderLength;
            if (NULL_PTR == sameBufferPtr)
            {
                authenticPduPtr = &rxSecuredPduPtr->SecOCRxSecuredPduBufferRef[headerLen];
            }
            else
            {
                authenticPduPtr = &sameBufferPtr->SecOCSecuredPduBufferRef[headerLen];
            }
        }
        else
        {
            rxAuthenticPduPtr = SecOC_GetRxAuthenticPduPtr(index);
            headerLen = rxAuthenticPduPtr->SecOCAuthPduHeaderLength;
            if (NULL_PTR == sameBufferPtr)
            {
                authenticPduPtr = &rxAuthenticPduPtr->SecOCRxColAuthenticPduBufferRef[headerLen];
            }
            else
            {
                authenticPduPtr = &sameBufferPtr->SecOCColAuthenticPduBufferRef[headerLen];
            }
        }
        SecOC_ArrayShiftLeft(
            authenticPduPtr,
            (uint16)rxRtPtr->rxParsedAuthPduLen,
            rxPduProcessingPtr->SecOCAuthDataFreshnessStartPosition);
        authDataFreshnessPtr = authenticPduPtr;
    }
    else
    {
        authDataFreshnessPtr = NULL_PTR;
    }

    freshnessValueLen = rxPduProcessingPtr->SecOCFreshnessValueLength;
#if (SECOC_CFUNC == SECOC_QUERY_FRESHNESS_VALUE)
    if (TRUE == rxPduProcessingPtr->SecOCUseAuthDataFreshness)
    {
        /* @req SWS_SecOC_00246,SWS_SecOC_00249 */
        ret = SecOC_GetRxFreshnessAuthData(
            rxPduProcessingPtr->SecOCFreshnessValueId,
            rxRtPtr->rxParsedFreshnessValue,
            rxPduProcessingPtr->SecOCFreshnessValueTruncLength,
            authDataFreshnessPtr,
            rxPduProcessingPtr->SecOCAuthDataFreshnessLen,
            rxRtPtr->rxVerifyAttempts,
            rxRtPtr->rxFreshnessValue,
            &freshnessValueLen);
    }
    else
    {
        /* @req SWS_SecOC_00247,SWS_SecOC_00250 */
        ret = SecOC_GetRxFreshness(
            rxPduProcessingPtr->SecOCFreshnessValueId,
            rxRtPtr->rxParsedFreshnessValue,
            rxPduProcessingPtr->SecOCFreshnessValueTruncLength,
            rxRtPtr->rxVerifyAttempts,
            rxRtPtr->rxFreshnessValue,
            &freshnessValueLen);
    }
#else
    if (TRUE == rxPduProcessingPtr->SecOCUseAuthDataFreshness)
    {
        /* @req SWS_SecOC_00244 */
        ret = FreshnessManagement_GetRxFreshnessAuthData(
            rxPduProcessingPtr->SecOCFreshnessValueId,
            rxRtPtr->rxParsedFreshnessValue,
            rxPduProcessingPtr->SecOCFreshnessValueTruncLength,
            authDataFreshnessPtr,
            rxPduProcessingPtr->SecOCAuthDataFreshnessLen,
            rxRtPtr->rxVerifyAttempts,
            rxRtPtr->rxFreshnessValue,
            &freshnessValueLen);
    }
    else
    {
        /* @req SWS_SecOC_00245 */
        ret = FreshnessManagement_GetRxFreshness(
            rxPduProcessingPtr->SecOCFreshnessValueId,
            rxRtPtr->rxParsedFreshnessValue,
            rxPduProcessingPtr->SecOCFreshnessValueTruncLength,
            rxRtPtr->rxVerifyAttempts,
            rxRtPtr->rxFreshnessValue,
            &freshnessValueLen);
    }
#endif

    if (E_OK == ret)
    {
        SchM_Enter_SecOC_Authentication();
        freshnessValueLen = (uint32)SECOC_BIT_TO_BYTE((uint16)rxPduProcessingPtr->SecOCFreshnessValueLength);
        (void)ILib_memcpy(
            &SecOC_DataToAuthenticator[SECOC_DATAID_LEN + securedIPDULength],
            rxRtPtr->rxFreshnessValue,
            freshnessValueLen);
        SchM_Exit_SecOC_Authentication();
    }
    else if (E_BUSY == ret)
    {
        /* @req SWS_SecOC_00236 */
        SecOC_RxAuthenticationBuildRetry(index);
    }
    else
    {
        /* @req SWS_SecOC_00256 */
        rxRtPtr->rxVeryfyResult = SECOC_FRESHNESSFAILURE;
        /* Report verify status */
        SecOC_RxReportVerifyStatus(index);

        /* Get freshness value return E_NOT_OK,Reset channel */
        SecOC_RxChannelReset(index);
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Build DataToAuthenticator retry process
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_RxAuthenticationBuildRetry(uint16 index)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];

    /* @req SWS_SecOC_00236,SWS_SecOC_00237,SWS_SecOC_00238 */
    if (rxRtPtr->rxAuthBuildAttempts < rxPduProcessingPtr->SecOCAuthenticationBuildAttempts)
    {
        SchM_Enter_SecOC_RxData();
        rxRtPtr->rxAuthBuildAttempts++;
        rxRtPtr->rxPduStatus = SECOC_RX_RETRY;
        SchM_Exit_SecOC_RxData();
    }
    else
    {
        /* @req SWS_SecOC_00240 */
        if ((SECOC_OVERRIDE_PASS_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)
            || ((SECOC_OVERRIDE_PASS_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
                && (rxRtPtr->rxBakNumOfMsgToOverride > 0u)))
        {
            SchM_Enter_SecOC_RxData();
            /* According override setup, send authentic IPDU to upper layer */
            SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);
            SchM_Exit_SecOC_RxData();
        }
        else
        {
            SchM_Enter_SecOC_RxData();
            /* @req SWS_SecOC_00240 */
            rxRtPtr->rxVeryfyResult = SECOC_AUTHENTICATIONBUILDFAILURE;
            SchM_Exit_SecOC_RxData();
            /* Report verify status */
            SecOC_RxReportVerifyStatus(index);

            /* Authentication Build retry fail */
            if (SECOC_IS_RX_EVENT(index, SECOC_RX_SECURED_PDU))
            {
                /* Not use SecOCRxSecuredPduCollection, channel reset */
                SecOC_RxChannelReset(index);
            }
            else
            {
                SchM_Enter_SecOC_RxData();
                /* Use SecOCRxSecuredPduCollection,Authentication build may
                 * success when Authentic I-PDU or Cryptographic PDU Update */
                SECOC_CLR_RX_EVENT(index, SECOC_RX_PARSED_MESSAGE);
                SchM_Exit_SecOC_RxData();
            }
        }

        /* @req SWS_SecOC_00248 */
        /* Report runtime error to DET */
#if (STD_ON == SECOC_DEV_ERROR_DETECT)
        SECOC_DET_REPORT_RUNTIMEERROR(SECOC_MAINFUNCTIONRX_ID, SECOC_E_FRESHNESS_FAILURE);
#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */
    }
}

/******************************************************************************/
/*
 * Brief               Verify Authentication information retry process
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_RxVerifyAuthenticInfoRetry(uint16 index)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];

    /* @req SWS_SecOC_00239 */
    if (rxRtPtr->rxVerifyAttempts < rxPduProcessingPtr->SecOCAuthenticationVerifyAttempts)
    {
        SchM_Enter_SecOC_RxData();
        rxRtPtr->rxVerifyAttempts++;
        rxRtPtr->rxPduStatus = SECOC_RX_RETRY;
        SchM_Exit_SecOC_RxData();
    }
    else
    {
        if ((SECOC_OVERRIDE_PASS_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)
            || ((SECOC_OVERRIDE_PASS_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
                && (rxRtPtr->rxBakNumOfMsgToOverride > 0u)))
        {
            SchM_Enter_SecOC_RxData();
            /* Continue to send authentic I-PDU to upper layer */
            SECOC_SET_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);
            SchM_Exit_SecOC_RxData();
        }
        else
        {
            SchM_Enter_SecOC_RxData();
            /* @req SWS_SecOC_00241 */
            rxRtPtr->rxVeryfyResult = SECOC_VERIFICATIONFAILURE;
            SchM_Exit_SecOC_RxData();
            /* Report verify status */
            SecOC_RxReportVerifyStatus(index);

            /* Verify Authentication retry fail */
            if (SECOC_IS_RX_EVENT(index, SECOC_RX_SECURED_PDU))
            {
                /* Not use SecOCRxSecuredPduCollection, channel reset */
                SecOC_RxChannelReset(index);
            }
            else
            {
                SchM_Enter_SecOC_RxData();
                /* Use SecOCRxSecuredPduCollection,Verify authentication may
                   success when Authentic I-PDU or Cryptographic PDU Update */
                SECOC_CLR_RX_EVENT(index, SECOC_RX_PARSED_MESSAGE);
                SchM_Exit_SecOC_RxData();
            }
        }
    }
}

#if (SECOC_MAX_MSG_LINK_LEN > 0u)

/******************************************************************************/
/*
 * Description         This function is used to get the pointer to the use message link structure for a specific index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      idx: Index for which the pointer to the use message link structure is requested.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              SecOC_UseMessageLinkType: Pointer to the use message link structure.
 */
/******************************************************************************/
static P2CONST(SecOC_UseMessageLinkType, AUTOMATIC, SECOC_CONST) SecOC_GetRxUseMessageLinkPtr(uint16 idx)
{
    return (SecOC_GetRxSecuredPduCollectionPtr(idx)->SecOCUseMessageLink);
}

/******************************************************************************/
/*
 * Brief               Authentic Pdu and Cryptographic Pdu message link check
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Message Link Check success
 *                     E_NOT_OK: Message Link Check fail
 */
/******************************************************************************/
static FUNC(Std_ReturnType, SECOC_CODE) SecOC_RxMessageLinkCheck(uint16 index)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2CONST(SecOC_SameBufferPduCollectionType, AUTOMATIC, SECOC_CONST)
    sameBufferPtr = rxPduProcessingPtr->SecOCSameBufferPduRef;
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];
    P2CONST(SecOC_RxSecuredPduCollectionType, AUTOMATIC, SECOC_CONST)
    rxSecuredPduCollectionPtr = SecOC_GetRxSecuredPduCollectionPtr(index);
    P2CONST(SecOC_UseMessageLinkType, AUTOMATIC, SECOC_CONST) msgLinkPtr;
    P2CONST(SecOC_RxAuthenticPduType, AUTOMATIC, SECOC_CONST) rxAuthenticPduPtr;
    P2CONST(SecOC_RxCryptographicPduType, AUTOMATIC, SECOC_CONST)
    rxCryptographicPduPtr;
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) tBufPtr; /* PRQA S 3432 */      /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) cryptoPduPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    uint8 msgLinkArray[SECOC_MAX_MSG_LINK_LEN] = {0};
    uint16 offsetBits;
    uint16 authPduLen;
    uint16 msgLinkStartPos;
    uint16 msgLinkLenInByte;
    uint8 headerLen;
    Std_ReturnType ret = E_OK;

    if ((NULL_PTR != rxSecuredPduCollectionPtr))
    {
        SchM_Enter_SecOC_RxData();
        msgLinkPtr = SecOC_GetRxUseMessageLinkPtr(index);
        if ((NULL_PTR != msgLinkPtr) && SECOC_IS_RX_EVENT(index, SECOC_RX_INDICATION)
            && SECOC_IS_RX_EVENT(index, SECOC_RX_AUTHENTIC_PDU) && SECOC_IS_RX_EVENT(index, SECOC_RX_CRYPTOGRAPHIC_PDU))
        {
            /* Pickup message link from Authentic I-PDU */
            rxAuthenticPduPtr = SecOC_GetRxAuthenticPduPtr(index);
            rxCryptographicPduPtr = SecOC_GetRxCryptographicPduPtr(index);
            headerLen = rxAuthenticPduPtr->SecOCAuthPduHeaderLength;
            authPduLen = rxRtPtr->rxAuthenticPduLen - headerLen;

            if (NULL_PTR == sameBufferPtr)
            {
                tBufPtr = rxPduProcessingPtr->SecOCAuthenticPduBufferRef;
                (void)ILib_memcpy(tBufPtr, &rxAuthenticPduPtr->SecOCRxColAuthenticPduBufferRef[headerLen], authPduLen);
                cryptoPduPtr = rxCryptographicPduPtr->SecOCRxCryptographicPduBufferRef;
            }
            else
            {
                tBufPtr = sameBufferPtr->SecOCAuthenticPduBufferRef;
                (void)ILib_memcpy(tBufPtr, &sameBufferPtr->SecOCColAuthenticPduBufferRef[headerLen], authPduLen);
                cryptoPduPtr = sameBufferPtr->SecOCCryptographicPduBufferRef;
            }

            offsetBits = ((uint16)headerLen * 8u) + msgLinkPtr->SecOCMessageLinkPos;
            SecOC_ArrayShiftLeft(tBufPtr, authPduLen, offsetBits);
            (void)ILib_memcpy(msgLinkArray, tBufPtr, authPduLen);

            /* Pickup message link from Cryptographic Pdu */
            offsetBits =
                rxPduProcessingPtr->SecOCAuthInfoTruncLength + rxPduProcessingPtr->SecOCFreshnessValueTruncLength;
            if (SECOC_MOD_FROM_EIGHT(offsetBits) == 0u)
            {
                msgLinkStartPos = rxPduProcessingPtr->SecOCAuthFrsInfoLength;
            }
            else
            {
                msgLinkStartPos = rxPduProcessingPtr->SecOCAuthFrsInfoLength - 1u;
            }
            (void)ILib_memcpy(
                tBufPtr,
                &cryptoPduPtr[msgLinkStartPos],
                ((uint32)rxRtPtr->rxCryptographicPduLen - (uint32)msgLinkStartPos));
            msgLinkLenInByte = SECOC_BIT_TO_BYTE(msgLinkPtr->SecOCMessageLinkLen);
            SecOC_ArrayShiftLeft(tBufPtr, msgLinkLenInByte, SECOC_MOD_FROM_EIGHT(offsetBits));
            offsetBits = 8u - SECOC_MOD_FROM_EIGHT(msgLinkPtr->SecOCMessageLinkLen);
            tBufPtr[msgLinkLenInByte] &= (0xffu << offsetBits);

            /* Compare message link */
            msgLinkLenInByte = SECOC_BIT_TO_BYTE(msgLinkPtr->SecOCMessageLinkLen);
            if (0 != ILib_memcmp(tBufPtr, msgLinkArray, msgLinkLenInByte))
            {
                ret = E_NOT_OK;
            }
        }
        SchM_Exit_SecOC_RxData();
    }

    return ret;
}
#endif /* SECOC_MAX_MSG_LINK_LEN > 0u */

/******************************************************************************/
/*
 * Brief               Send Authentic pdu to upper layer
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_RxPassAuthenticPduToUpperLayer(uint16 index)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2CONST(SecOC_RxAuthenticPduLayerType, AUTOMATIC, SECOC_APPL_DATA)
    rxAuthenticPduLayerPtr = SecOC_GetRxAuthenticPduLayerPtr(index);
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];
    PduInfoType pduInfo;
    PduIdType pduId;
    BufReq_ReturnType bufReqRet;

    if (SECOC_IS_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED))
    {
        SECOC_CLR_RX_EVENT(index, SECOC_RX_VERIFY_FINISH_IND_CONFIRMED);

        /* @req SWS_SecOC_00081,SWS_SecOC_00088 */
        if ((SECOC_OVERRIDE_PASS_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)
            || (((SECOC_OVERRIDE_SKIP_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
                 || (SECOC_OVERRIDE_PASS_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus))
                && (rxRtPtr->rxBakNumOfMsgToOverride > 0u))
            || ((SECOC_OVERRIDE_DROP_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
                && (rxRtPtr->rxBakNumOfMsgToOverride == 0u))
            || (SECOC_OVERRIDE_SKIP_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)
            || (CRYPTO_E_VER_OK == rxRtPtr->rxVerifyResult)
            || (*SecOC_ConfigPtr->SecOCIgnoreVerificationResultRef == TRUE))
        {
            SchM_Enter_SecOC_RxData();
            /* I-PDU is sent to upper layer*/
            pduInfo.SduLength = rxRtPtr->rxParsedAuthPduLen;
            pduInfo.SduDataPtr = rxRtPtr->rxParsedAuthPduBufPtr;
            SchM_Exit_SecOC_RxData();
            pduId = rxAuthenticPduLayerPtr->SecOCRxPduRAsUpLayerId;
            /* @req SWS_SecOC_00050,SWS_SecOC_00080 */
            if (SECOC_IFPDU == rxAuthenticPduLayerPtr->SecOCPduType)
            {
                /* @req SWS_SecOC_00080,SWS_SecOC_00086 */
                PduR_SecOCIfRxIndication(pduId, &pduInfo);
                /* @req SWS_SecOC_00207 */
                /* Channel Reset */
                SecOC_RxChannelReset(index);
            }
#if (SECOC_RX_TP_PDU_NUM > 0)
            else
            {
                /* @req SWS_SecOC_00082 */
                /* Upper layer is TP pdu,Notify upper layer start of reception*/
                SchM_Enter_SecOC_RxData();
                rxRtPtr->upLayerBufSize = 0u;
                rxRtPtr->sduRemaining = rxRtPtr->rxParsedAuthPduLen;
                SchM_Exit_SecOC_RxData();
                bufReqRet = PduR_SecOCTpStartOfReception(pduId, NULL_PTR, pduInfo.SduLength, &rxRtPtr->upLayerBufSize);
                if ((BUFREQ_OK == bufReqRet) && (rxRtPtr->upLayerBufSize > 0u))
                {
                    SchM_Enter_SecOC_RxData();
                    SECOC_SET_RX_EVENT(index, SECOC_RX_TRANS_TP_DATA_TO_UPPER);
                    SchM_Exit_SecOC_RxData();
                }
                else
                {
                    /* PduR return error,Abort transmit data to upper layer */
                    PduR_SecOCTpRxIndication(pduId, E_NOT_OK);
                    /* Channel Reset */
                    SecOC_RxChannelReset(index);
                }
            }
#endif
        }
        else
        {
            /* @req SWS_SecOC_00239 */
            /* Verification of the authenticator successfully executed but the
             * verification failed. */
            SecOC_RxVerifyAuthenticInfoRetry(index);
        }
        SchM_Enter_SecOC_RxData();
        if (CRYPTO_E_VER_OK == rxRtPtr->rxVerifyResult)
        {
            /* @req SWS_SecOC_00242 */
            rxRtPtr->rxVeryfyResult = SECOC_VERIFICATIONSUCCESS;
        }
        else
        {
            if ((SECOC_OVERRIDE_PASS_UNTIL_NOTICE == rxRtPtr->rxOverrideStatus)
                || ((SECOC_OVERRIDE_PASS_UNTIL_LIMIT == rxRtPtr->rxOverrideStatus)
                    && (rxRtPtr->rxBakNumOfMsgToOverride > 0u)))
            {
                rxRtPtr->rxVeryfyResult = SECOC_VERIFICATIONFAILURE_OVERWRITTEN;
            }
            else
            {
                rxRtPtr->rxVeryfyResult = SECOC_VERIFICATIONFAILURE;
            }
        }
        SchM_Exit_SecOC_RxData();
        /* Report verify status */
        SecOC_RxReportVerifyStatus(index);
    }
}

#if (SECOC_RX_TP_PDU_NUM > 0u)
/******************************************************************************/
/*
 * Brief               Send Authentic pdu(TP) to upper layer
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_RxTransTpDataToUpperLayer(uint16 index)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2CONST(SecOC_RxAuthenticPduLayerType, AUTOMATIC, SECOC_APPL_DATA)
    rxAuthenticPduLayerPtr = SecOC_GetRxAuthenticPduLayerPtr(index);
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];
    BufReq_ReturnType bufReqRet;
    PduInfoType pduInfo;
    PduLengthType pduLen;
    PduLengthType offset = rxRtPtr->rxParsedAuthPduLen - rxRtPtr->sduRemaining;
    PduIdType pduId;
    boolean quitFlag = FALSE;

    if (SECOC_IS_RX_EVENT(index, SECOC_RX_TRANS_TP_DATA_TO_UPPER))
    {
        SchM_Enter_SecOC_RxData();
        pduLen = SECOC_MIN(rxRtPtr->sduRemaining, rxRtPtr->upLayerBufSize);
        pduId = rxAuthenticPduLayerPtr->SecOCRxPduRAsUpLayerId;

        /* @req SWS_SecOC_00086 */
        /* Copy data to upper layer complete */
        pduInfo.SduDataPtr = &rxRtPtr->rxParsedAuthPduBufPtr[offset];
        pduInfo.SduLength = pduLen;
        SchM_Exit_SecOC_RxData();
        bufReqRet = PduR_SecOCTpCopyRxData(pduId, &pduInfo, &rxRtPtr->upLayerBufSize);
        if (BUFREQ_OK == bufReqRet)
        {
            SchM_Enter_SecOC_RxData();
            rxRtPtr->sduRemaining -= pduLen;
            SchM_Exit_SecOC_RxData();
            if (0u == rxRtPtr->sduRemaining)
            {
                /* Notify upper layer data copy finished */
                PduR_SecOCTpRxIndication(pduId, E_OK);
                /* @req SWS_SecOC_00207 */
                /* Channel Reset */
                SecOC_RxChannelReset(index);
            }
            else
            {
                if (0u == rxRtPtr->upLayerBufSize)
                {
                    /* PduR return error,Abort transmit data to upper layer */
                    quitFlag = TRUE;
                }
            }
        }
        else
        {
            /* @req SWS_SecOC_00267 */
            /* PduR return BUFREQ_E_NOT_OK,Abort transmit data to upper layer */
            quitFlag = TRUE;
        }

        if (TRUE == quitFlag)
        {
            /* @req SWS_SecOC_00267 */
            /* Notify PduR with E_NOT_OK */
            PduR_SecOCTpRxIndication(pduId, E_NOT_OK);
            /* Channel Reset */
            SecOC_RxChannelReset(index);
        }
    }
}

/******************************************************************************/
/*
 * Brief               Start to receive Secured TP pdu process
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 *                     id: Identification of the I-PDU.
 *                     info: Pointer to a PduInfoType structure containing the
 *                           payload data (without protocol information) and
 *                           payload length
 *                     TpSduLength: Total length of the N-SDU to be received.
 * Param-Name[out]     bufferSizePtr: Available receive buffer in the receiving
 *                                    module.
 * Param-Name[in/out]  None
 * Return              BUFREQ_OK: Connection has been accepted.
 *                     BUFREQ_E_NOT_OK: Connection has been rejected.
 *                     BUFREQ_E_OVFL: No buffer of the required length can be
 *                                    provided.
 */
/******************************************************************************/
static FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_RxTpSecuredPduStartOfReception(
    uint16 index,
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) bufferSizePtr /* PRQA S 3432 */ /* MISRA Rule 20.7 */
)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2CONST(SecOC_SameBufferPduCollectionType, AUTOMATIC, SECOC_CONST)
    sameBufferPtr = rxPduProcessingPtr->SecOCSameBufferPduRef;
    P2CONST(SecOC_RxSecuredPduType, AUTOMATIC, SECOC_CONST) rxSecuredPduPtr;
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) destPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    PduLengthType interBufLen;
    BufReq_ReturnType bufReqRet = BUFREQ_E_NOT_OK;

    if (0u != TpSduLength)
    {
        if (SECOC_RX_NONE != rxRtPtr->rxPduStatus)
        {
            switch (rxPduProcessingPtr->SecOCReceptionOverflowStrategy)
            {
            case SECOC_REJECT:
                /* @req SWS_SecOC_00215 */
                /* The Secured I-PDU is processing, Ignore the new request*/
                break;

            case SECOC_REPLACE:
                /* @req SWS_SecOC_00214 */
                /* Free all buffer related to the Secured I-PDU,Rx restart*/
                SecOC_RxChannelReset(index);
                break;

            case SECOC_QUEUE:
                /* Not support tp data */
                break;

            default:
                /* Nothing to do here*/
                break;
            }
        }
        SchM_Enter_SecOC_RxData();
        if (SECOC_RX_NONE == rxRtPtr->rxPduStatus)
        {
            rxSecuredPduPtr = SecOC_GetRxSecuredPduPtr(index);
            if (NULL_PTR == sameBufferPtr)
            {
                destPtr = rxSecuredPduPtr->SecOCRxSecuredPduBufferRef;
                interBufLen = rxSecuredPduPtr->SecOCRxSecuredPduBuffLength;
            }
            else
            {
                destPtr = sameBufferPtr->SecOCSecuredPduBufferRef;
                interBufLen = sameBufferPtr->SecOCSecuredPduBuffLength;
            }

            if (interBufLen >= TpSduLength)
            {
                if ((NULL_PTR == sameBufferPtr) || (FALSE == sameBufferPtr->SecOCSameBufferInUse[0u]))
                {
                    rxRtPtr->sduRemaining = TpSduLength;
                    rxRtPtr->tpRxPduOffset = 0u;
                    if (NULL_PTR != info)
                    {
                        (void)ILib_memcpy(destPtr, info->SduDataPtr, info->SduLength);
                        rxRtPtr->sduRemaining -= info->SduLength;
                        rxRtPtr->tpRxPduOffset += info->SduLength;
                    }

                    SECOC_SET_RX_EVENT(index, SECOC_RX_TP_DATA_FROM_LOWER);
                    SECOC_SET_RX_EVENT(index, SECOC_RX_SECURED_PDU);
                    rxRtPtr->rxPduStatus = SECOC_RX_CALC;
                    rxRtPtr->rxSecuredPduLen = TpSduLength;
                    rxRtPtr->rxVerifyAttempts = 0u;
                    rxRtPtr->curPduId = id;
                    rxRtPtr->tpRxDestBufPtr = destPtr;
                    if (NULL_PTR != sameBufferPtr)
                    {
                        sameBufferPtr->SecOCSameBufferInUse[0u] = TRUE;
                    }

                    *bufferSizePtr = interBufLen;

                    bufReqRet = BUFREQ_OK;
                }
                else
                {
                    bufReqRet = BUFREQ_E_OVFL;
                }
            }
            else
            {
                bufReqRet = BUFREQ_E_OVFL;
            }
        }
        SchM_Exit_SecOC_RxData();
    }
    else
    {
        /* @req SWS_SecOC_00181 */
        /*
         * TpSduLength equal to 0,return BUFREQ_E_NOT_OK and no further action
         * shall be taken.
         */
    }

    return bufReqRet;
}

/******************************************************************************/
/*
 * Brief               Start to receive Authentic TP pdu process
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 *                     id: Identification of the I-PDU.
 *                     info: Pointer to a PduInfoType structure containing the
 *                           payload data (without protocol information) and
 *                           payload length
 *                     TpSduLength: Total length of the N-SDU to be received.
 * Param-Name[out]     bufferSizePtr: Available receive buffer in the receiving
 *                                    module.
 * Param-Name[in/out]  None
 * Return              BUFREQ_OK: Connection has been accepted.
 *                     BUFREQ_E_NOT_OK: Connection has been rejected.
 *                     BUFREQ_E_OVFL: No buffer of the required length can be
 *                                    provided.
 */
/******************************************************************************/
static FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_RxTpAuthenticPduStartOfReception(
    uint16 index,
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) bufferSizePtr /* PRQA S 3432 */ /* MISRA Rule 20.7 */
)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2CONST(SecOC_SameBufferPduCollectionType, AUTOMATIC, SECOC_CONST)
    sameBufferPtr = rxPduProcessingPtr->SecOCSameBufferPduRef;
    P2CONST(SecOC_RxAuthenticPduType, AUTOMATIC, SECOC_CONST) rxAuthenticPduPtr;
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) destPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    PduLengthType interBufLen;
    BufReq_ReturnType bufReqRet = BUFREQ_E_NOT_OK;

    if (0u != TpSduLength)
    {
        if (SECOC_RX_NONE != rxRtPtr->rxPduStatus)
        {
            switch (rxPduProcessingPtr->SecOCReceptionOverflowStrategy)
            {
            case SECOC_REJECT:
                /* @req SWS_SecOC_00215 */
                /* The Secured I-PDU is processing, Ignore the new request*/
                break;

            case SECOC_REPLACE:
                /* @req SWS_SecOC_00214 */
                /* Free all buffer related to the Secured I-PDU,Rx restart*/
                SecOC_RxChannelReset(index);
                break;

            case SECOC_QUEUE:
                /* @req SWS_SecOC_CONSTR_00265 */
                break;

            default:
                /* Nothing to do here*/
                break;
            }
        }
        SchM_Enter_SecOC_RxData();
        if (SECOC_RX_NONE == rxRtPtr->rxPduStatus)
        {
            rxAuthenticPduPtr = SecOC_GetRxAuthenticPduPtr(index);
            if (NULL_PTR == sameBufferPtr)
            {
                destPtr = rxAuthenticPduPtr->SecOCRxColAuthenticPduBufferRef;
                interBufLen = rxAuthenticPduPtr->SecOCRxColAuthenticPduBuffLength;
            }
            else
            {
                destPtr = sameBufferPtr->SecOCColAuthenticPduBufferRef;
                interBufLen = sameBufferPtr->SecOCColAuthenticPduBuffLength;
            }

            if (interBufLen >= TpSduLength)
            {
                if ((NULL_PTR == sameBufferPtr) || (FALSE == sameBufferPtr->SecOCSameBufferInUse[0u]))
                {
                    rxRtPtr->sduRemaining = TpSduLength;
                    rxRtPtr->tpRxPduOffset = 0u;
                    if (NULL_PTR != info)
                    {
                        (void)ILib_memcpy(destPtr, info->SduDataPtr, info->SduLength);
                        rxRtPtr->sduRemaining -= info->SduLength;
                        rxRtPtr->tpRxPduOffset += info->SduLength;
                    }

                    SECOC_SET_RX_EVENT(index, SECOC_RX_TP_DATA_FROM_LOWER);
                    SECOC_SET_RX_EVENT(index, SECOC_RX_AUTHENTIC_PDU);
                    rxRtPtr->rxPduStatus = SECOC_RX_CALC;
                    rxRtPtr->rxAuthenticPduLen = TpSduLength;
                    rxRtPtr->rxVerifyAttempts = 0u;
                    rxRtPtr->curPduId = id;
                    rxRtPtr->tpRxDestBufPtr = destPtr;
                    if (NULL_PTR != sameBufferPtr)
                    {
                        sameBufferPtr->SecOCSameBufferInUse[0u] = TRUE;
                    }

                    *bufferSizePtr = interBufLen;

                    bufReqRet = BUFREQ_OK;
                }
                else
                {
                    bufReqRet = BUFREQ_E_OVFL;
                }
            }
            else
            {
                bufReqRet = BUFREQ_E_OVFL;
            }
        }
        SchM_Exit_SecOC_RxData();
    }
    else
    {
        /* @req SWS_SecOC_00181 */
        /*
         * TpSduLength equal to 0,return BUFREQ_E_NOT_OK and no further action
         * shall be taken.
         */
    }

    return bufReqRet;
}

/******************************************************************************/
/*
 * Brief               Start to receive Cryptographic TP pdu process
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 *                     id: Identification of the I-PDU.
 *                     info: Pointer to a PduInfoType structure containing the
 *                           payload data (without protocol information) and
 *                           payload length
 *                     TpSduLength: Total length of the N-SDU to be received.
 * Param-Name[out]     bufferSizePtr: Available receive buffer in the receiving
 *                                    module.
 * Param-Name[in/out]  None
 * Return              BUFREQ_OK: Connection has been accepted.
 *                     BUFREQ_E_NOT_OK: Connection has been rejected.
 *                     BUFREQ_E_OVFL: No buffer of the required length can be
 *                                    provided.
 */
/******************************************************************************/
static FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_RxTpCryptographicPduStartOfReception(
    uint16 index,
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_CONST) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) bufferSizePtr /* PRQA S 3432 */ /* MISRA Rule 20.7 */
)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2CONST(SecOC_SameBufferPduCollectionType, AUTOMATIC, SECOC_CONST)
    sameBufferPtr = rxPduProcessingPtr->SecOCSameBufferPduRef;
    P2CONST(SecOC_RxCryptographicPduType, AUTOMATIC, SECOC_CONST)
    rxCryptographicPduPtr;
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) destPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    PduLengthType interBufLen;
    BufReq_ReturnType bufReqRet = BUFREQ_E_NOT_OK;

    if ((0u != TpSduLength) && (NULL_PTR != info) && (info->SduLength <= TpSduLength))
    {
        if (SECOC_RX_NONE != rxRtPtr->rxPduStatus)
        {
            switch (rxPduProcessingPtr->SecOCReceptionOverflowStrategy)
            {
            case SECOC_REJECT:
                /* @req SWS_SecOC_00215 */
                /* The Secured I-PDU is processing, Ignore the new request*/
                break;

            case SECOC_REPLACE:
                /* @req SWS_SecOC_00214 */
                /* Free all buffer related to the Secured I-PDU,Rx restart*/
                SecOC_RxChannelReset(index);
                break;

            case SECOC_QUEUE:
                /* @req SWS_SecOC_CONSTR_00265 */
                break;

            default:
                /* Nothing to do here*/
                break;
            }
        }

        if (SECOC_RX_NONE == rxRtPtr->rxPduStatus)
        {
            rxCryptographicPduPtr = SecOC_GetRxCryptographicPduPtr(index);
            if (NULL_PTR == sameBufferPtr)
            {
                destPtr = rxCryptographicPduPtr->SecOCRxCryptographicPduBufferRef;
                interBufLen = rxCryptographicPduPtr->SecOCRxCryptographicPduBuffLength;
            }
            else
            {
                destPtr = sameBufferPtr->SecOCCryptographicPduBufferRef;
                interBufLen = sameBufferPtr->SecOCCryptographicPduBuffLength;
            }
            SchM_Enter_SecOC_RxData();
            if (interBufLen >= TpSduLength)
            {
                if ((NULL_PTR == sameBufferPtr) || (FALSE == sameBufferPtr->SecOCSameBufferInUse[0u]))
                {
                    rxRtPtr->sduRemaining = TpSduLength;
                    rxRtPtr->tpRxPduOffset = 0u;

                    (void)ILib_memcpy(destPtr, info->SduDataPtr, info->SduLength);
                    rxRtPtr->sduRemaining -= info->SduLength;
                    rxRtPtr->tpRxPduOffset += info->SduLength;

                    SECOC_SET_RX_EVENT(index, SECOC_RX_TP_DATA_FROM_LOWER);
                    SECOC_SET_RX_EVENT(index, SECOC_RX_CRYPTOGRAPHIC_PDU);
                    rxRtPtr->rxPduStatus = SECOC_RX_CALC;
                    rxRtPtr->rxCryptographicPduLen = TpSduLength;
                    rxRtPtr->rxVerifyAttempts = 0u;
                    rxRtPtr->curPduId = id;
                    rxRtPtr->tpRxDestBufPtr = destPtr;
                    if (NULL_PTR != sameBufferPtr)
                    {
                        sameBufferPtr->SecOCSameBufferInUse[0u] = TRUE;
                    }

                    *bufferSizePtr = interBufLen;

                    bufReqRet = BUFREQ_OK;
                }
                else
                {
                    bufReqRet = BUFREQ_E_OVFL;
                }
            }
            else
            {
                bufReqRet = BUFREQ_E_OVFL;
            }
            SchM_Exit_SecOC_RxData();
        }
    }
    else
    {
        /* @req SWS_SecOC_00181 */
        /*
         * TpSduLength equal to 0 or info->SduLength less than TpSduLength,return
         * BUFREQ_E_NOT_OK and no further action shall be taken.
         */
    }

    return bufReqRet;
}
#endif /* SECOC_RX_TP_PDU_NUM > 0u */

/******************************************************************************/
/*
 * Brief               Load the queued pdu to start process
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if (SECOC_RX_IF_PDU_NUM > 0u)
static FUNC(void, SECOC_CODE) SecOC_RxLoadQueuedPdu(uint16 index)
{
    uint8 SecOC_TempMsgBuf[SECOC_MAX_RX_SECURED_MSG_LENGTH];
    PduInfoType pduInfo;
    PduIdType pduId;
    Std_ReturnType ret;

    pduInfo.SduLength = SECOC_MAX_RX_SECURED_MSG_LENGTH;
    pduInfo.SduDataPtr = SecOC_TempMsgBuf;
    ret = SecOC_DataOutQueue(index, &pduId, &pduInfo);
    if (E_OK == ret)
    {
        SecOC_RxIfSecuredPduIndication(index, pduId, &pduInfo);
    }
}
#endif

/******************************************************************************/
/*
 * Brief               Report verify status to other module,use configured
 *                     callout function
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_RxReportVerifyStatus(uint16 index)
{
#if (SECOC_VERIFICATION_STATUS_CALLOUT_NUM > 0)
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];
    SecOC_VerificationStatusType verificationStatus;
    uint16 idx;
    boolean notifyFlag = FALSE;

    /* @req SWS_SecOC_00048,SWS_SecOC_00243 */
    /* Notify other modules on the verification status of the most recently
     * received Secured I-PDU */
    if (SECOC_NONE != rxPduProcessingPtr->SecOCVerificationStatusPropagationMode)
    {
        SchM_Enter_SecOC_RxData();
        verificationStatus.freshnessValueID = rxPduProcessingPtr->SecOCFreshnessValueId;
        verificationStatus.verificationStatus = rxRtPtr->rxVeryfyResult;
        verificationStatus.secOCDataId = rxPduProcessingPtr->SecOCDataId;

        /* Both True and False AuthenticationStatus is propagated to SWC */
        if (SECOC_BOTH == rxPduProcessingPtr->SecOCVerificationStatusPropagationMode)
        {
            notifyFlag = TRUE;
        }
        /* Only False AuthenticationStatus is propagated to SWC */
        else if (SECOC_FAILURE_ONLY == rxPduProcessingPtr->SecOCVerificationStatusPropagationMode)
        {
            if ((SECOC_VERIFICATIONFAILURE == rxRtPtr->rxVeryfyResult)
                || (SECOC_FRESHNESSFAILURE == rxRtPtr->rxVeryfyResult)
                || (SECOC_AUTHENTICATIONBUILDFAILURE == rxRtPtr->rxVeryfyResult))
            {
                notifyFlag = TRUE;
            }
        }
        else
        {
            /* No AuthenticationStatus is propagated to SWC */
        }
        SchM_Exit_SecOC_RxData();

        if (TRUE == notifyFlag)
        {
            for (idx = 0u; idx < SECOC_VERIFICATION_STATUS_CALLOUT_NUM; idx++)
            {
                SecOC_ConfigPtr->SecOCVerificationStatusCallout[idx](verificationStatus);
            }
        }
    }
#else
    (void)(index);
#endif
}

/******************************************************************************/
/*
 * Brief               Reset the channel runtime data to default value
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SecOC channel index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_RxChannelReset(uint16 index)
{
    SchM_Enter_SecOC_RxData();
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];

    /* Reset runtime data */
    rxRtPtr->rxPduStatus = SECOC_RX_NONE;
    rxRtPtr->curPduId = 0u;
    rxRtPtr->rxSecuredPduLen = 0u;
    rxRtPtr->rxAuthenticPduLen = 0u;
    rxRtPtr->rxCryptographicPduLen = 0u;
    rxRtPtr->rxParsedAuthPduLen = 0u;
    rxRtPtr->queueManage.freeQueSize = rxPduProcessingPtr->SecOCReceptionQueueSize;
    rxRtPtr->queueManage.freeQueStartIdx = 0u;
    rxRtPtr->queueManage.queueFrameCnt = 0u;
    rxRtPtr->queueManage.queueInfoIdx = 0u;
    (void)ILib_memset(rxRtPtr->rxParsedFreshnessValue, 0, 8u);
    (void)ILib_memset(rxRtPtr->rxParsedAuthenticator, 0, SECOC_MAX_AUTHENTICATOR_LEN);
    rxRtPtr->rxParsedAuthPduBufPtr = NULL_PTR;
    rxRtPtr->rxAuthBuildAttempts = 0u;
    rxRtPtr->rxVerifyAttempts = 0u;
    (void)ILib_memset(rxRtPtr->rxFreshnessValue, 0, sizeof(rxRtPtr->rxFreshnessValue));
    rxRtPtr->rxVeryfyResult = SECOC_VERIFICATIONFAILURE;
#if (SECOC_RX_TP_PDU_NUM > 0)
    rxRtPtr->tpRxDestBufPtr = NULL_PTR;
    rxRtPtr->sduRemaining = 0u;
    rxRtPtr->upLayerBufSize = 0u;
    rxRtPtr->tpRxPduOffset = 0u;
#endif

    /* Unlock Samebuffer */
    if (NULL_PTR != rxPduProcessingPtr->SecOCSameBufferPduRef)
    {
        rxPduProcessingPtr->SecOCSameBufferPduRef->SecOCSameBufferInUse[0] = FALSE;
    }
    SchM_Exit_SecOC_RxData();

    /* Clear rx event */
    SECOC_CLR_RX_EVENT(index, SECOC_EVENT_RX_ALL);
}

/******************************************************************************/
/*
 * Brief               Calculate runtime pdu length
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      headerLength: Header length(runtime pdu length in header)
 *                     headerPtr: Pointer to the buffer of Header
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Runtime pdu length
 */
/******************************************************************************/
static FUNC(PduLengthType, SECOC_CODE) SecOC_RxGetPduLength(
    uint8 headerLength,
    P2CONST(uint8, AUTOMATIC, SECOC_CONST) headerPtr /* PRQA S 3432 */ /* MISRA Rule 20.7 */
)
{
    uint32 pduLength = 0u;
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) ptr = (uint8*)&pduLength; /* PRQA S 3432 */ /* MISRA Rule 20.7 */

    (void)ILib_memcpy(&ptr[4u - headerLength], headerPtr, headerLength);
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    SecOC_EndianSwap(ptr, 4u);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

    return ((PduLengthType)pduLength);
}
#endif /* SECOC_RX_PDU_NUM > 0u */

/******************************************************************************/
/*
 * Brief               Switch data byte order
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      dataPtr: Pointer to data to be convert
 *                     size: Data length in bytes
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_EndianSwap(
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) dataPtr,
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
    uint8 size)
{
    uint8 i;
    uint8 temp;
    uint8 halfSize = size / 2u;
    uint8 tempSize = size - 1u;
    uint8 tempIndex;

    for (i = 0u; i < halfSize; i++)
    {
        tempIndex = tempSize - i;
        temp = dataPtr[tempIndex];
        dataPtr[tempIndex] = dataPtr[i];
        dataPtr[i] = temp;
    }
}

/******************************************************************************/
/*
 * Brief               Array shift left N bits
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      numLen: Array length in Byte
 *                     shiftBits: Shift bits
 * Param-Name[out]     None
 * Param-Name[in/out]  numArr: number array for operate and also store result
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_ArrayShiftLeft(
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) numArr,
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
    uint16 numLen,
    uint16 shiftBits)
{
    uint16 leftNumByte = SECOC_BIT_TO_BYTE(((numLen * 8u) - shiftBits));
    uint16 shiftPerByte = SECOC_MOD_FROM_EIGHT(shiftBits);
    uint16 firstOpNumIdx = numLen - leftNumByte;
    uint16 i;

    /* Array Shift Left */
    for (i = 0u; i < leftNumByte; i++)
    {
        if ((firstOpNumIdx + i + 1u) < numLen)
        {
            numArr[i] =
                (numArr[firstOpNumIdx + i] << shiftPerByte) | (numArr[firstOpNumIdx + i + 1u] >> (8u - shiftPerByte));
        }
        else
        {
            numArr[i] = (numArr[firstOpNumIdx + i] << shiftPerByte);
        }
    }
    /* Clear tail unused bytes */
    for (i = leftNumByte; i < numLen; i++)
    {
        numArr[i] = 0x00;
    }
}

/******************************************************************************/
/*
 * Brief               Array shift right N bits
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      numLen: Array length in Byte
 *                     shiftBits: Shift bits
 * Param-Name[out]     None
 * Param-Name[in/out]  numArr: number array for operate and also store result
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_ArrayShiftRight(
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) numArr,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    uint16 numLen,
    uint16 shiftBits)
{
    uint16 leftNumByte = SECOC_BIT_TO_BYTE(((numLen * 8u) - shiftBits));
    uint16 shiftPerByte = SECOC_MOD_FROM_EIGHT(shiftBits);
    uint16 firstOpNumIdx = leftNumByte - 1u;
    uint16 i;

    /* Array Shift Right */
    for (i = 0u; i < leftNumByte; i++)
    {
        if ((firstOpNumIdx - i) >= 1u)
        {
            numArr[numLen - 1u - i] =
                (numArr[firstOpNumIdx - i] >> shiftPerByte) | (numArr[firstOpNumIdx - i - 1u] << (8u - shiftPerByte));
        }
        else
        {
            numArr[numLen - 1u - i] = numArr[firstOpNumIdx - i] >> shiftPerByte;
        }
    }
    /* Clear head unused bytes */
    for (i = 0u; i < (numLen - leftNumByte); i++)
    {
        numArr[i] = 0x00u;
    }
}

/******************************************************************************/
/*
 * Brief               Array or operation
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      numLen: Array length in Byte
 *                     numArr2: Number array input for or operation
 * Param-Name[out]     None
 * Param-Name[in/out]  numArr1: number array for operate and also store result
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_ArrayOrOp(
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) numArr1,
    P2CONST(uint8, AUTOMATIC, SECOC_CONST) numArr2,
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
    uint16 numLen)
{
    /* Note: The length of numArr1 must equal to numArr2 */
    uint16 i;

    for (i = 0u; i < numLen; i++)
    {
        numArr1[i] = numArr1[i] | numArr2[i];
    }
}

/******************************************************************************/
/*
 * Description         This function is used to clear the low bits of a number array by shifting.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      numArr: Pointer to the number array.
 *                     numLen: Length of the number array.
 *                     shiftBits: Number of bits to shift.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE) SecOC_ClearLowBit(
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) numArr,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    uint16 numLen,
    uint16 shiftBits)
{
    uint16 leftNumByte = SECOC_BIT_TO_BYTE(((numLen * 8u) - shiftBits));
    /* Clear head unused bytes */
    if (0u != SECOC_MOD_FROM_EIGHT(shiftBits))
    {
        numArr[leftNumByte - 1u] &= (~((1u << shiftBits) - 1u));
    }
}

/******************************************************************************/
/*
 * Brief               Save the input PduInfo to queue
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SECOC Channel index
 *                     RxPduId: PduId of received message
 *                     PduInfoPtr: PduLength and pdu buffer
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              TRUE: Save data to queue success
 *                     FALSE: Save data to queue failed
 */
/******************************************************************************/
static FUNC(void, SECOC_CODE)
    SecOC_DataInQueue(uint16 index, PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) PduInfoPtr)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];
    P2VAR(SecOC_QueueManageType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    queueManagePtr = &rxRtPtr->queueManage;
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxQueueBufPtr = rxPduProcessingPtr->SecOCReceptionQueueRef;
    SecOC_QueueInfoType queInfo;
    uint16 needBufSize;
    uint16 queueSize = rxPduProcessingPtr->SecOCReceptionQueueSize;
    uint16 offset = queueManagePtr->freeQueStartIdx;
    uint16 part1Len;
    uint16 part2Len;
    uint8 queInfoSize = sizeof(SecOC_QueueInfoType);

    /* @req SWS_SecOC_00216 */
    needBufSize = PduInfoPtr->SduLength + queInfoSize;
    SchM_Enter_SecOC_RxData();
    if (queueManagePtr->freeQueSize >= needBufSize)
    {
        /* Build queue information */
        queInfo.pduId = RxPduId; /* PRQA S 2995 */
        queInfo.pduLength = PduInfoPtr->SduLength;

        /* Save queue information to ReceptionQueueBuffer */
        /* Sequence buffer enough,not need turn-back */
        if ((queueSize - offset) >= queInfoSize)
        {
            (void)ILib_memcpy(&rxQueueBufPtr[offset], (uint8*)&queInfo, queInfoSize);
            offset += queInfoSize;
        }
        else
        {
            part1Len = queueSize - offset;
            (void)ILib_memcpy(&rxQueueBufPtr[offset], (uint8*)&queInfo, part1Len);
            part2Len = queInfoSize - part1Len;
            (void)ILib_memcpy(&rxQueueBufPtr[0u], ((uint8*)&queInfo + part1Len), part2Len); /* PRQA S 0488 */
            offset = part2Len;
        }

        /* Save received Secured I-PDU to ReceptionQueueBuffer */
        if ((queueSize - offset) >= (uint16)PduInfoPtr->SduLength)
        {
            (void)ILib_memcpy(&rxQueueBufPtr[offset], PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
            offset += queInfoSize;
        }
        else
        {
            part1Len = queueSize - offset;
            (void)ILib_memcpy(&rxQueueBufPtr[offset], PduInfoPtr->SduDataPtr, part1Len);
            part2Len = (uint16)PduInfoPtr->SduLength - part1Len;
            (void)ILib_memcpy(&rxQueueBufPtr[0u], &PduInfoPtr->SduDataPtr[part1Len], part2Len);
            offset = part2Len;
        }

        /* Update queue manage info */
        queueManagePtr->queueFrameCnt++;
        queueManagePtr->freeQueStartIdx = offset;
        queueManagePtr->freeQueSize -= needBufSize;
    }
    else
    {
        /* Free buffer not enough,rejecte the Secured I-PDU */
    }
    SchM_Exit_SecOC_RxData();
}

/******************************************************************************/
/*
 * Brief               Output the queued info to 'RxPduIdPtr' and 'PduInfoPtr'
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      index: SECOC Channel index
 *                     RxPduIdPtr: Pointer to a buffer where to store RxPduId
 * Param-Name[out]     None
 * Param-Name[in/out]  PduInfoPtr: Input buffer length and buffer address,when
 *                     return store queued PDU to buffer and update buffer
 *                     length to SduLength
 * Return              TRUE: Output the queued info success
 *                     FALSE: Output the queued info failed
 */
/******************************************************************************/
static FUNC(Std_ReturnType, SECOC_CODE) SecOC_DataOutQueue(
    uint16 index,
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(PduIdType, AUTOMATIC, SECOC_APPL_DATA) RxPduIdPtr,
    P2VAR(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) PduInfoPtr
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
)
{
    P2CONST(SecOC_RxPduProcessingType, AUTOMATIC, SECOC_CONST)
    rxPduProcessingPtr = SecOC_GetRxPduProcessingPtr(index);
    P2VAR(SecOC_RxPduRTType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxRtPtr = &SecOC_RxRtData[index];
    P2VAR(SecOC_QueueManageType, AUTOMATIC, SECOC_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    queueManagePtr = &rxRtPtr->queueManage;
    P2CONST(uint8, AUTOMATIC, SECOC_CONST) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    rxQueueBufPtr = rxPduProcessingPtr->SecOCReceptionQueueRef;
    SecOC_QueueInfoType queInfo;
    uint16 queueSize = rxPduProcessingPtr->SecOCReceptionQueueSize;
    uint16 offset = queueManagePtr->queueInfoIdx;
    uint16 part1Len;
    uint16 part2Len;
    uint8 queInfoSize = sizeof(SecOC_QueueInfoType);
    Std_ReturnType ret = E_NOT_OK;

    SchM_Enter_SecOC_RxData();
    if (queueManagePtr->queueFrameCnt > 0u)
    {
        /* Recover QueueInfo from Queue */
        if ((queueSize - offset) >= queInfoSize)
        {
            (void)ILib_memcpy((uint8*)&queInfo, &rxQueueBufPtr[offset], queInfoSize);
            offset += queInfoSize;
        }
        else
        {
            part1Len = queueSize - offset;
            (void)ILib_memcpy((uint8*)&queInfo, &rxQueueBufPtr[offset], part1Len);
            part2Len = queInfoSize - offset;
            (void)ILib_memcpy(((uint8*)&queInfo + part1Len), &rxQueueBufPtr[0u], part2Len); /* PRQA S 0488 */
            offset = part2Len;
        }
        *RxPduIdPtr = queInfo.pduId;

        if (PduInfoPtr->SduLength >= queInfo.pduLength)
        {
            /* Output RxPduId */
            *RxPduIdPtr = queInfo.pduId;

            /* Output PduLength and Secured I-PDU to PduInfoPtr */
            PduInfoPtr->SduLength = queInfo.pduLength;
            if ((queueSize - offset) >= (uint16)PduInfoPtr->SduLength)
            {
                (void)ILib_memcpy(PduInfoPtr->SduDataPtr, &rxQueueBufPtr[offset], PduInfoPtr->SduLength);
                offset += (uint16)PduInfoPtr->SduLength;
            }
            else
            {
                part1Len = queueSize - offset;
                (void)ILib_memcpy(PduInfoPtr->SduDataPtr, &rxQueueBufPtr[offset], part1Len);
                part2Len = (uint16)PduInfoPtr->SduLength - offset;
                (void)ILib_memcpy(&PduInfoPtr->SduDataPtr[part1Len], &rxQueueBufPtr[0u], part2Len);
                offset = part2Len;
            }

            /* Update queue manage info */
            queueManagePtr->queueFrameCnt--;
            queueManagePtr->freeQueStartIdx = offset;
            queueManagePtr->freeQueSize += (queInfoSize + (uint16)queInfo.pduLength);
            queueManagePtr->queueInfoIdx = offset;

            ret = E_OK;
        }
        else
        {
            /* Buffer not enough, return with E_NOT_OK */
            /* Return to E_NOT_OK; */
        }
    }
    SchM_Exit_SecOC_RxData();

    return ret;
}

#if (STD_ON == SECOC_DEV_ERROR_DETECT)
static inline void SECOC_DET_REPORT(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(SECOC_MODULE_ID, SECOC_INSTANCE_ID, (ApiId), (ErrorId));
}
static inline void SECOC_DET_REPORT_RUNTIMEERROR(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportRuntimeError(SECOC_MODULE_ID, SECOC_INSTANCE_ID, (ApiId), (ErrorId));
}

#endif /* STD_ON == SECOC_DEV_ERROR_DETECT */

#define SECOC_STOP_SEC_CODE
#include "SecOC_MemMap.h"
