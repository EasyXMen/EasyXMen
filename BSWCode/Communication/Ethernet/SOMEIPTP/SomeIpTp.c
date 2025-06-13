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
**  FILENAME    : SomeIpTp.c                                                  **
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
 * V2.0.0      20200707   HuRongbo   Initial version
 * V2.0.1      20210730   HuRongbo   Bug fix
 * V2.0.2      20220418   HuRongbo
 *  1> Fix bug when rx first segment memcpy length error
 * V2.0.3      20220522   HuRongbo
 *  1> Fix bug when reveive single segment
 *  2> Fix bug of SeparationTimer
 *  3> Modify call PduR_SomeIpTpTxConfirmation() function parameter from
 *     FALSE/TRUE to E_OK/E_NOT_OK
 * V2.0.4      20220608   HuRongbo
 *  1> Modify SomeIpTpTxNSduNum to SomeIpTpRxNSduNum in function
 *     SomeIpTp_GetRxChannelCfgPtr() and SomeIpTp_GetRxNSduCfgPtr()
 * V2.0.5      20220821   HuRongbo
 *  1> Modify rx first segment handle,Indicate to uppler layer when the frame
 *     is single frame (JIRA:CPT-6608)
 * V2.0.6      20220823   HuRongbo
 *  1> Resolve the sending exception caused by incorrect availableData
 *     returned by the upper layer module (JIRA:CPT-6617)
 * V2.0.7      20231122   HuRongbo
 *  1> Code execution efficiency optimization (JIRA:CPD-33590)
 *  2> QAC warning handle
 * V2.0.8      20240124   HuRongbo
 *  1> Replace standard library functions to iSoft library functions
 * V2.0.9      20240306   HuRongbo
 *  1> QAC check with new rule set
 */

/**
 \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
   ModeName:SomeIpTp<br>
 RuleSorce:puhua-rule.rcf 2.3.1

   \li PRQA S 2877 MISRA Dir 4.1.<br>
   Reason:The exception is that the number of loops is configurable.

   \li PRQA S 3432 MISRA Rule 20.7.<br>
   Reason:Compilation abstractions are the exception.

   \li PRQA S 0635 MISRA Rule 6.1.<br>
   Reason:It will be fixed in a later version.

   \li PRQA S 1531, 1532 MISRA Rule 8.7 .<br>
   Reason:The exception is global configuration data(1531) and API(1532).
*/
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

#define SOMEIPTP_C_AR_MAJOR_VERSION 4
#define SOMEIPTP_C_AR_MINOR_VERSION 5
#define SOMEIPTP_C_AR_PATCH_VERSION 0
#define SOMEIPTP_C_SW_MAJOR_VERSION 2
#define SOMEIPTP_C_SW_MINOR_VERSION 0
#define SOMEIPTP_C_SW_PATCH_VERSION 9

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "SomeIpTp.h"
#include "SomeIpTp_Internal.h"
#include "PduR_SomeIpTp.h"
#include "Det.h"
#include "istd_lib.h"

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (SOMEIPTP_C_AR_MAJOR_VERSION != SOMEIPTP_H_AR_MAJOR_VERSION)
#error "SomeIpTp.c : Mismatch in Specification Major Version"
#endif
#if (SOMEIPTP_C_AR_MINOR_VERSION != SOMEIPTP_H_AR_MINOR_VERSION)
#error "SomeIpTp.c : Mismatch in Specification _MINOR Version"
#endif
#if (SOMEIPTP_C_AR_PATCH_VERSION != SOMEIPTP_H_AR_PATCH_VERSION)
#error "SomeIpTp.c : Mismatch in Specification PATCH Version"
#endif
#if (SOMEIPTP_C_SW_MAJOR_VERSION != SOMEIPTP_H_SW_MAJOR_VERSION)
#error "SomeIpTp.c : Mismatch in Software Major Version"
#endif
#if (SOMEIPTP_C_SW_MINOR_VERSION != SOMEIPTP_H_SW_MINOR_VERSION)
#error "SomeIpTp.c : Mismatch in Software MINOR Version"
#endif
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
/* inline keyword define,if compiler not support inline,define SOMEIPTP_INLINE to null */
#define SOMEIPTP_LOCAL_INLINE static inline

#if !defined(SOMEIPTP_LOCAL)
#define SOMEIPTP_LOCAL static
#endif

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"

static FUNC(void, SOMEIPTP_CODE) SomeIpTp_TxTimerHandle(uint16 ch);

static FUNC(void, SOMEIPTP_CODE) SomeIpTp_TxPduHandle(uint16 ch);

static FUNC(void, SOMEIPTP_CODE) SomeIpTp_TxCancelHandle(
    P2CONST(SomeIpTp_TxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST) txNSduCfgPtr,
    P2VAR(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) txPduRtPtr, /*PRQA S 3432*/
    uint8 apiId);

SOMEIPTP_LOCAL_INLINE FUNC(void, SOMEIPTP_CODE)
    SomeIpTp_ResetTxRtData(P2VAR(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) txPduRtPtr /*PRQA S 3432*/
    );

static FUNC(const SomeIpTp_ChannelType*, SOMEIPTP_CODE) SomeIpTp_GetTxChannelCfgPtr(PduIdType txPduId);

static FUNC(const SomeIpTp_TxNSduType*, SOMEIPTP_CODE) SomeIpTp_GetTxNSduCfgPtr(PduIdType txPduId);

static FUNC(PduLengthType, SOMEIPTP_CODE) SomeIpTp_CalcNextSegmentSize(
    P2CONST(SomeIpTp_TxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST) txNSduCfgPtr,
    P2VAR(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) txPduRtPtr /*PRQA S 3432*/
);

static FUNC(PduLengthType, SOMEIPTP_CODE) SomeIpTp_CalcSizeGetFromUpperLayer(
    P2CONST(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    txPduRtPtr);

static FUNC(uint8*, SOMEIPTP_CODE) SomeIpTp_CalcAddrToCopyDataFromUpperLayer(
    P2CONST(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) txPduRtPtr, /*PRQA S 3432*/
    P2VAR(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) sduDataPtr                   /*PRQA S 3432*/
);

static FUNC(void, SOMEIPTP_CODE) SomeIpTp_BuildHeader(
    P2CONST(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) txPduRtPtr, /*PRQA S 3432*/
    P2VAR(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) headerAddr                   /*PRQA S 3432*/
);

static FUNC(void, SOMEIPTP_CODE) SomeIpTp_RxTimerHandle(uint16 ch);

static FUNC(Std_ReturnType, SOMEIPTP_CODE)
    SomeIpTp_RxMessageCheck(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr);

static FUNC(void, SOMEIPTP_CODE) SomeIpTp_RxCancelHandle(
    P2VAR(SomeIpTp_RxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) rxPduRtPtr, /*PRQA S 3432*/
    P2CONST(SomeIpTp_RxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST) rxNSduCfgPtr,
    uint8 apiId,
    uint8 errorId);

SOMEIPTP_LOCAL_INLINE FUNC(void, SOMEIPTP_CODE)
    SomeIpTp_ResetRxRtData(P2VAR(SomeIpTp_RxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) rxPduRtPtr /*PRQA S 3432*/
    );

static FUNC(const SomeIpTp_ChannelType*, SOMEIPTP_CODE) SomeIpTp_GetRxChannelCfgPtr(PduIdType rxPduId);

static FUNC(const SomeIpTp_RxNSduType*, SOMEIPTP_CODE) SomeIpTp_GetRxNSduCfgPtr(PduIdType rxPduId);

static FUNC(void, SOMEIPTP_CODE)
    SomeIpTp_RxSingleSegmentHandle(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr);

static FUNC(void, SOMEIPTP_CODE)
    SomeIpTp_RxFirstSegmentHandle(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr);

static FUNC(void, SOMEIPTP_CODE) SomeIpTp_RxConsecutiveSegmentHandle(
    PduIdType RxPduId,
    P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr);

#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
SOMEIPTP_LOCAL_INLINE FUNC(void, SOMEIPTP_CODE) SomeIpTp_EndianSwap(uint8* dataPtr, uint8 size);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

static FUNC(SomeIpTp_TpHeaderType, SOMEIPTP_CODE)
    SomeIpTp_ParseTpHeader(P2CONST(void, AUTOMATIC, SOMEIPTP_APPL_DATA) dataPtr);

SOMEIPTP_LOCAL_INLINE FUNC(PduLengthType, SOMEIPTP_CODE) SomeIpTp_Min(PduLengthType a, PduLengthType b);

#if (STD_ON == SOMEIPTP_DEV_ERROR_DETECT)
SOMEIPTP_LOCAL_INLINE FUNC(void, SOMEIPTP_CODE) SomeIpTp_DetReportError(uint8 ApiId, uint8 ErrorId);
#endif

SOMEIPTP_LOCAL_INLINE FUNC(void, SOMEIPTP_CODE) SomeIpTp_DetReportRuntimeError(uint8 ApiId, uint8 ErrorId);

#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define SOMEIPTP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "SomeIpTp_MemMap.h"
/* SOMEIPTP runtime data */
SOMEIPTP_LOCAL VAR(SomeIpTp_RxPduRTType, SOMEIPTP_VAR) SomeIpTp_RxPduRTData[SOMEIPTP_RX_PDU_NUM];

SOMEIPTP_LOCAL VAR(SomeIpTp_TxPduRTType, SOMEIPTP_VAR) SomeIpTp_TxPduRTData[SOMEIPTP_TX_PDU_NUM];

#define SOMEIPTP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_VAR_INIT_PTR
#include "SomeIpTp_MemMap.h"
/* Pointer to configuration data */
static P2CONST(SomeIpTp_ConfigType, SOMEIPTP_VAR, SOMEIPTP_CONST) SomeIpTp_ConfigPtr = NULL_PTR;
#define SOMEIPTP_STOP_SEC_VAR_INIT_PTR
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_VAR_INIT_UNSPECIFIED
#include "SomeIpTp_MemMap.h"
/* Variable store SOMEIPTP initialization status */
SOMEIPTP_LOCAL VAR(SomeIpTp_InitStateType, SOMEIPTP_VAR_POWER_ON_INIT) SomeIpTp_InitStatus = SOMEIPTP_UNINIT;
#define SOMEIPTP_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "SomeIpTp_MemMap.h"

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/******************************************************************************/
/*
 * Brief               Returns the version information of this module.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     VersionInfo: Pointer to where to store the version
 *                                  information of this module
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if (STD_ON == SOMEIPTP_VERSION_INFO_API)
FUNC(void, SOMEIPTP_CODE) SomeIpTp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) VersionInfo)
{
#if (STD_ON == SOMEIPTP_DEV_ERROR_DETECT)
    if (NULL_PTR == VersionInfo)
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_GETVERSIONINFO, SOMEIPTP_E_PARAM_POINTER);
    }
    else
#endif
    {
        VersionInfo->vendorID = SOMEIPTP_VENDOR_ID;
        VersionInfo->moduleID = SOMEIPTP_MODULE_ID;
        VersionInfo->sw_major_version = SOMEIPTP_C_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = SOMEIPTP_C_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = SOMEIPTP_C_SW_PATCH_VERSION;
    }
}
#endif /*STD_ON == SOMEIPTP_VERSION_INFO_API*/

/******************************************************************************/
/*
 * Brief               Initializes the SOME/IP TP module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      config: Base pointer to the configuration structure of
 *                             the SOME/IP TP module.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SOMEIPTP_CODE)
SomeIpTp_Init(const SomeIpTp_ConfigType* config) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == SOMEIPTP_DEV_ERROR_DETECT)
    if (NULL_PTR == config)
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_INIT, SOMEIPTP_E_PARAM_POINTER);
    }
    else
#endif
    {
        PduIdType pduIdx;
        P2VAR(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) txPduRtPtr; /*PRQA S 3432*/
        P2VAR(SomeIpTp_RxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) rxPduRtPtr; /*PRQA S 3432*/

        /* Initialize run time structure variable for every receive pdu */
        for (pduIdx = 0u; pduIdx < SOMEIPTP_RX_PDU_NUM; pduIdx++) /*PRQA S 2877*/
        {
            rxPduRtPtr = &SomeIpTp_RxPduRTData[pduIdx];
            SomeIpTp_ResetRxRtData(rxPduRtPtr);
        }

        /* Initialize run time structure variable for every transmit pdu */
        for (pduIdx = 0u; pduIdx < SOMEIPTP_TX_PDU_NUM; pduIdx++) /*PRQA S 2877*/
        {
            txPduRtPtr = &SomeIpTp_TxPduRTData[pduIdx];
            SomeIpTp_ResetTxRtData(txPduRtPtr);
        }

        /* Initialize config data pointer*/
        SomeIpTp_ConfigPtr = config;

        /* Change initialization status */
        SomeIpTp_InitStatus = SOMEIPTP_INIT;
    }
}

/******************************************************************************/
/*
 * Brief               Requests transmission of a PDU.
 * ServiceId           0x49
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds.
 * Param-Name[in]      TxPduId: Identifier of the PDU to be transmitted
 *                     PduInfoPtr: Length of and pointer to the PDU data and
 *                                 pointer to MetaData.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Transmit request has been accepted.
 *                     E_NOT_OK: Transmit request has not been accepted.
 */
/******************************************************************************/
FUNC(Std_ReturnType, SOMEIPTP_CODE)
SomeIpTp_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    /* @req SWS_SomeIpTp_00076 */
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == SOMEIPTP_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (SOMEIPTP_UNINIT == SomeIpTp_InitStatus)
    {
        /* @req SWS_SomeIpTp_00073 */
        SomeIpTp_DetReportError(SOMEIPTP_SID_TRANSMIT, SOMEIPTP_E_UNINIT);
        detNoErr = FALSE;
    }
    if (TxPduId >= SOMEIPTP_TX_PDU_NUM)
    {
        /* @req SWS_SomeIpTp_00074 */
        SomeIpTp_DetReportError(SOMEIPTP_SID_TRANSMIT, SOMEIPTP_E_PARAM);
        detNoErr = FALSE;
    }
    if (NULL_PTR == PduInfoPtr)
    {
        /* @req SWS_SomeIpTp_00075 */
        SomeIpTp_DetReportError(SOMEIPTP_SID_TRANSMIT, SOMEIPTP_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif
    {
        P2VAR(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) /*PRQA S 3432*/
        txPduRtPtr = &SomeIpTp_TxPduRTData[TxPduId];
        P2CONST(SomeIpTp_TxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST)
        txNSduCfgPtr = SomeIpTp_GetTxNSduCfgPtr(TxPduId);

        if (SOMEIPTP_PDU_IDLE != txPduRtPtr->pduState)
        {
            /* @req SWS_SomeIpTp_00022 */
            SomeIpTp_TxCancelHandle(txNSduCfgPtr, txPduRtPtr, SOMEIPTP_SID_TRANSMIT);
        }
        else
        {
            txPduRtPtr->pduState = SOMEIPTP_PDU_TX_WAIT;
            txPduRtPtr->txPduLen = PduInfoPtr->SduLength;

            ret = E_OK;
        }
    }

    return ret;
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_SOMEIPTPTRIGGERTRANSMIT_CALLBACK_CODE
#include "SomeIpTp_MemMap.h"
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
 * Param-Name[out]     None
 * Param-Name[in/out]  PduInfoPtr: Contains a pointer to a buffer (SduDataPtr)
 *                     to where the SDU data shall be copied, and the available
 *                     buffer size in SduLengh. On return, the service will
 *                     indicate the length of the copied SDU data in SduLength.
 * Return              E_OK: SDU has been copied and SduLength indicates the
 *                           number of copied bytes.
 *                     E_NOT_OK: No SDU data has been copied. PduInfoPtr must
 *                               not be used since it may contain a NULL pointer
 *                               or point to invalid data.
 */
/******************************************************************************/
FUNC(Std_ReturnType, SOMEIPTP_CODE)
SomeIpTp_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == SOMEIPTP_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (SOMEIPTP_UNINIT == SomeIpTp_InitStatus)
    {
        /* @req SWS_SomeIpTp_00072 */
        SomeIpTp_DetReportError(SOMEIPTP_SID_TRIGGERTRANSMIT, SOMEIPTP_E_UNINIT);
        detNoErr = FALSE;
    }
    if (TxPduId >= SOMEIPTP_TX_PDU_NUM)
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_TRIGGERTRANSMIT, SOMEIPTP_E_PARAM);
        detNoErr = FALSE;
    }
    if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_TRIGGERTRANSMIT, SOMEIPTP_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif
    {
        P2VAR(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) /*PRQA S 3432*/
        txPduRtPtr = &SomeIpTp_TxPduRTData[TxPduId];
        P2CONST(SomeIpTp_TxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST)
        txNSduCfgPtr;
        BufReq_ReturnType bufReqRet;
        PduInfoType pduInfo;

        if ((SOMEIPTP_PDU_TX_COPY_DATA == txPduRtPtr->pduState) && (PduInfoPtr->SduLength >= txPduRtPtr->segmentSize))
        {
            txNSduCfgPtr = SomeIpTp_GetTxNSduCfgPtr(TxPduId);
            if (NULL_PTR != txNSduCfgPtr)
            {
                pduInfo.SduLength = SomeIpTp_CalcSizeGetFromUpperLayer(txPduRtPtr);
                pduInfo.SduDataPtr = SomeIpTp_CalcAddrToCopyDataFromUpperLayer(txPduRtPtr, PduInfoPtr->SduDataPtr);
                bufReqRet = PduR_SomeIpTpCopyTxData(
                    txNSduCfgPtr->SomeIpTpTxNSduUpperLayerId,
                    &pduInfo,
                    NULL_PTR,
                    &txPduRtPtr->availableDataLen);
                if (BUFREQ_OK != bufReqRet)
                {
                    /* @req SWS_SomeIpTp_00025 */
                    SomeIpTp_TxCancelHandle(txNSduCfgPtr, txPduRtPtr, SOMEIPTP_SID_TRIGGERTRANSMIT);
                }
                else
                {
                    /* Save SOME/IP Header to internal tx buffer */
                    if (TRUE == txPduRtPtr->firstSegmentFlag)
                    {
                        (void)ILib_memcpy((uint8*)&txPduRtPtr->header, pduInfo.SduDataPtr, SOMEIPTP_SOMEIP_HEADER_LEN);
                    }

                    /* Return actual copied data length */
                    PduInfoPtr->SduLength = txPduRtPtr->segmentSize;

                    /* Build SOME/IP-TP Header */
                    if (FALSE == txPduRtPtr->noSegmentFlag)
                    {
                        SomeIpTp_BuildHeader(txPduRtPtr, PduInfoPtr->SduDataPtr);
                    }

                    txPduRtPtr->pduState = SOMEIPTP_PDU_TX_WAIT_CONF;
                    ret = E_OK;
                }
            }
        }
        else
        {
            /* @req SWS_SomeIpTp_00055 */
            /*
             * PduInfoPtr->SduLength is smaller than the actual PDU-length,
             * return E_NOT_OK.
             */
        }
    }

    return ret;
}
#define SOMEIPTP_STOP_SEC_SOMEIPTPTRIGGERTRANSMIT_CALLBACK_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_SOMEIPTPRXINDICATION_CALLBACK_CODE
#include "SomeIpTp_MemMap.h"
/******************************************************************************/
/*
 * Brief               Indication of a received PDU from a lower layer
 *                     communication interface module.
 * ServiceId           0x42
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds.
 * Param-Name[in]      RxPduId: ID of the received PDU.
 *                     PduInfoPtr: Contains the length (SduLength) of the
 *                     received PDU, a pointer to a buffer (SduDataPtr)
 *                     containing the PDU, and the MetaData related to this PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SOMEIPTP_CODE)
SomeIpTp_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == SOMEIPTP_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (SOMEIPTP_UNINIT == SomeIpTp_InitStatus)
    {
        /* @req SWS_SomeIpTp_00057 */
        SomeIpTp_DetReportError(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_UNINIT);
        detNoErr = FALSE;
    }
    if (RxPduId >= SOMEIPTP_RX_PDU_NUM)
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_PARAM);
        detNoErr = FALSE;
    }
    if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif
    {
        SomeIpTp_HeaderType header;
        SomeIpTp_TpHeaderType tpHeader = SomeIpTp_ParseTpHeader(&PduInfoPtr->SduDataPtr[SOMEIPTP_SOMEIP_HEADER_LEN]);

        (void)ILib_memcpy((uint8*)&header, PduInfoPtr->SduDataPtr, sizeof(SomeIpTp_HeaderType));
        if (E_OK == SomeIpTp_RxMessageCheck(RxPduId, PduInfoPtr))
        {
            if ((header.messageType & SOMEIPTP_TPFLAG_MASK) == 0u)
            {
                /* Single Segment */
                SomeIpTp_RxSingleSegmentHandle(RxPduId, PduInfoPtr);
            }
            else
            {
                if (0u == tpHeader.offset)
                {
                    SomeIpTp_RxFirstSegmentHandle(RxPduId, PduInfoPtr);
                }
                else
                {
                    SomeIpTp_RxConsecutiveSegmentHandle(RxPduId, PduInfoPtr);
                }
            }
        }
    }
}
#define SOMEIPTP_STOP_SEC_SOMEIPTPRXINDICATION_CALLBACK_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_SOMEIPTPTXCONFIRMATION_CALLBACK_CODE
#include "SomeIpTp_MemMap.h"
/******************************************************************************/
/*
 * Brief               This service is called by the Ethernet Interface to report
 *                     a transceiver link state change.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CtrlIdx: Index of the Ethernet controller within the context
 *                              of the Ethernet Interface
 *                     TransceiverLinkState: Actual transceiver link state of the
 *                                           specific network handle
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(void, SOMEIPTP_CODE)
SomeIpTp_TxConfirmation(
    PduIdType TxPduId
#ifdef SOMEIPTP_VER_R19_11
    ,
    Std_ReturnType result
#endif
)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == SOMEIPTP_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (SOMEIPTP_UNINIT == SomeIpTp_InitStatus)
    {
        /* @req SWS_SomeIpTp_00067 */
        SomeIpTp_DetReportError(SOMEIPTP_SID_TXCONFIRMATION, SOMEIPTP_E_UNINIT);
        detNoErr = FALSE;
    }
    if (TxPduId >= SOMEIPTP_TX_PDU_NUM)
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_TXCONFIRMATION, SOMEIPTP_E_PARAM);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif
    {
        P2VAR(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) /*PRQA S 3432*/
        txPduRtPtr = &SomeIpTp_TxPduRTData[TxPduId];
        P2CONST(SomeIpTp_TxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST)
        txNSduCfgPtr = SomeIpTp_GetTxNSduCfgPtr(TxPduId);
        P2CONST(SomeIpTp_ChannelType, AUTOMATIC, SOMEIPTP_APPL_CONST) chCfgPtr;

        if (SOMEIPTP_PDU_TX_WAIT_CONF == txPduRtPtr->pduState)
        {
#ifdef SOMEIPTP_VER_R19_11
            if (E_NOT_OK == result)
            {
                /* @req SWS_SomeIpTp_00023 */
                SomeIpTp_TxCancelHandle(txNSduCfgPtr, txPduRtPtr, SOMEIPTP_SID_TXCONFIRMATION);
                PduR_SomeIpTpTxConfirmation(txNSduCfgPtr->SomeIpTpTxNSduUpperLayerId, E_NOT_OK);
            }
            else
#endif
            {
                if (TRUE == txPduRtPtr->lastSegmentFlag)
                {
                    /* @req SWS_SomeIpTp_00021 */
                    PduR_SomeIpTpTxConfirmation(txNSduCfgPtr->SomeIpTpTxNSduUpperLayerId, E_OK);

                    SomeIpTp_ResetTxRtData(txPduRtPtr);
                }
                else
                {
                    /* Update runtime data */
                    txPduRtPtr->pduState = SOMEIPTP_PDU_TX_CONFIRMED;
                    txPduRtPtr->txDataCnt += SomeIpTp_CalcSizeGetFromUpperLayer(txPduRtPtr);
                    txPduRtPtr->firstSegmentFlag = FALSE;
                    txPduRtPtr->segmentSize = 0u;
#ifndef SOMEIPTP_VER_R19_11
                    /* Stop Timeout timer */
                    txPduRtPtr->txTimeoutTimer = 0u;
#endif
                    /* Start separation timer*/
                    chCfgPtr = SomeIpTp_GetTxChannelCfgPtr(TxPduId);
                    txPduRtPtr->separationTimer = chCfgPtr->SomeIpTpNPduSeparationTime;
                }
            }
        }
    }
}
#define SOMEIPTP_STOP_SEC_SOMEIPTPTXCONFIRMATION_CALLBACK_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function performs the processing of the AUTOSAR
 *                     SOME/IP TP module's transmission activities.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SOMEIPTP_CODE) SomeIpTp_MainFunctionTx(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    /* @req SWS_SomeIpTp_00059 */
    if (SOMEIPTP_INIT == SomeIpTp_InitStatus)
    {
        P2CONST(SomeIpTp_ChannelType, AUTOMATIC, SOMEIPTP_APPL_CONST) chCfgPtr;
        uint16 ch;

        for (ch = 0u; ch < SOMEIPTP_CHANNEL_NUM; ch++) /*PRQA S 2877*/
        {
            chCfgPtr = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
            if (chCfgPtr->SomeIpTpTxNSduNum > 0u)
            {
                SomeIpTp_TxTimerHandle(ch);
                SomeIpTp_TxPduHandle(ch);
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief               This function performs the processing of the AUTOSAR
 *                     SOME/IP TP module's reception activities.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, SOMEIPTP_CODE) SomeIpTp_MainFunctionRx(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    /* @req SWS_SomeIpTp_00070 */
    if (SOMEIPTP_INIT == SomeIpTp_InitStatus)
    {
        P2CONST(SomeIpTp_ChannelType, AUTOMATIC, SOMEIPTP_APPL_CONST) chCfgPtr;
        uint16 ch;

        for (ch = 0u; ch < SOMEIPTP_CHANNEL_NUM; ch++) /*PRQA S 2877*/
        {
            chCfgPtr = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
            if (chCfgPtr->SomeIpTpRxNSduNum > 0u)
            {
                SomeIpTp_RxTimerHandle(ch);
            }
        }
    }
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"

/******************************************************************************/
/*
 * Brief               Handle transmit relevant timer
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for differnt channel
 * Param-Name[in]      ch: SomeIpTp Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SOMEIPTP_CODE) SomeIpTp_TxTimerHandle(uint16 ch)
{
    P2CONST(SomeIpTp_ChannelType, AUTOMATIC, SOMEIPTP_APPL_CONST)
    chCfgPtr = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
    P2CONST(SomeIpTp_TxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST)
    txNSduCfgPtr = chCfgPtr->SomeIpTpTxNSdu;
    PduIdType firstPduOfChannel = chCfgPtr->SomeIpTpTxNSdu[0u].SomeIpTpTxNSduHandleId;
    P2VAR(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) /*PRQA S 3432*/
    txPduRtPtr = &SomeIpTp_TxPduRTData[firstPduOfChannel];
    PduIdType pduIdx;

    for (pduIdx = 0u; pduIdx < chCfgPtr->SomeIpTpTxNSduNum; pduIdx++)
    {
/* Handle tx timeout timer */
#ifndef SOMEIPTP_VER_R19_11
        if (txPduRtPtr->txTimeoutTimer > 0u)
        {
            txPduRtPtr->txTimeoutTimer--;
            if (0u == txPduRtPtr->txTimeoutTimer)
            {
                /* @req SWS_SomeIpTp_00023 */
                SomeIpTp_TxCancelHandle(txNSduCfgPtr, txPduRtPtr, SOMEIPTP_SID_MAINFUNCTIONTX);
            }
        }
#endif

        /* Handle separation timer */
        if (txPduRtPtr->separationTimer > 0u)
        {
            txPduRtPtr->separationTimer--;
        }

        txPduRtPtr++;
#ifndef SOMEIPTP_VER_R19_11
        txNSduCfgPtr++;
#endif
    }
}

/******************************************************************************/
/*
 * Brief               Pdu transmit handler
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for differnt channel
 * Param-Name[in]      ch: SomeIpTp Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SOMEIPTP_CODE) SomeIpTp_TxPduHandle(uint16 ch)
{
    P2CONST(SomeIpTp_ChannelType, AUTOMATIC, SOMEIPTP_APPL_CONST)
    chCfgPtr = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
    P2CONST(SomeIpTp_TxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST)
    txNSduCfgPtr = chCfgPtr->SomeIpTpTxNSdu;
    PduIdType firstPduOfChannel = chCfgPtr->SomeIpTpTxNSdu[0u].SomeIpTpTxNSduHandleId;
    P2VAR(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) /*PRQA S 3432*/
    txPduRtPtr = &SomeIpTp_TxPduRTData[firstPduOfChannel];
    PduIdType pduIdx;
    PduInfoType pduInfo;
    PduLengthType availableDataLen;
    PduLengthType remainDataLen;
    PduLengthType segmentSize;
    BufReq_ReturnType bufReqRet;
    Std_ReturnType ret;
    boolean errCancelFlag = FALSE;

    for (pduIdx = 0u; pduIdx < chCfgPtr->SomeIpTpTxNSduNum; pduIdx++)
    {
        /* @req SWS_SomeIpTp_00020 */
        if ((SOMEIPTP_PDU_TX_WAIT == txPduRtPtr->pduState)
            || ((SOMEIPTP_PDU_TX_CONFIRMED == txPduRtPtr->pduState) && (0u == txPduRtPtr->separationTimer)))
        {
            /* First segment,query the available data in upper layer module*/
            if (0u == txPduRtPtr->txDataCnt)
            {
                pduInfo.SduLength = 0u;
                pduInfo.SduDataPtr = NULL_PTR;
                bufReqRet = PduR_SomeIpTpCopyTxData(
                    txNSduCfgPtr->SomeIpTpTxNSduUpperLayerId,
                    &pduInfo,
                    NULL_PTR,
                    &availableDataLen);

                if (BUFREQ_OK == bufReqRet)
                {
                    txPduRtPtr->availableDataLen = availableDataLen;
                }
                else
                {
                    txPduRtPtr->availableDataLen = 0u;
                }
            }

            /* Calculate next segment size and judge if first/last segment */
            remainDataLen = txPduRtPtr->txPduLen - txPduRtPtr->txDataCnt;
            txPduRtPtr->availableDataLen = SomeIpTp_Min(txPduRtPtr->availableDataLen, remainDataLen);
            segmentSize = SomeIpTp_CalcNextSegmentSize(txNSduCfgPtr, txPduRtPtr);
            if (((txPduRtPtr->availableDataLen < SOMEIPTP_OFFSETFIELD_LSB) && (FALSE == txPduRtPtr->lastSegmentFlag))
                || ((txPduRtPtr->availableDataLen < remainDataLen) && (TRUE == txPduRtPtr->lastSegmentFlag)))
            {
                /* @req SWS_SomeIpTp_00024 */
                errCancelFlag = TRUE;
            }
            else
            {
                txPduRtPtr->segmentSize = segmentSize;
                txPduRtPtr->pduState = SOMEIPTP_PDU_TX_COPY_DATA;

                /* @req SWS_SomeIpTp_00017,@req SWS_SomeIpTp_00019 */
                pduInfo.SduLength = segmentSize;
                pduInfo.SduDataPtr = NULL_PTR;
                ret = PduR_SomeIpTpTransmit(txNSduCfgPtr->SomeIpTpTxNPduLowerLayerId, &pduInfo);
                if (E_NOT_OK == ret)
                {
                    errCancelFlag = TRUE;
                }
                else
                {
/* Start Timeout timer */
#ifndef SOMEIPTP_VER_R19_11
                    txPduRtPtr->txTimeoutTimer = chCfgPtr->SomeIpTpTxTimeoutTime;
#endif
                }
            }

            /* Error occur,Cancel transmit of this pdu */
            if (TRUE == errCancelFlag)
            {
                SomeIpTp_TxCancelHandle(txNSduCfgPtr, txPduRtPtr, SOMEIPTP_SID_MAINFUNCTIONTX);
            }
        }
        txPduRtPtr++;
        txNSduCfgPtr++;
    }
}

/******************************************************************************/
/*
 * Brief               Transmit cancel process
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      txPduRtPtr: Pointer of tx pdu runtime data
 *                     apiId: API id
 *                     txNSduCfgPtr: Pointer of SomeIpTpTxNSdu config data
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SOMEIPTP_CODE) SomeIpTp_TxCancelHandle(
    P2CONST(SomeIpTp_TxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST) txNSduCfgPtr,
    P2VAR(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) txPduRtPtr, /*PRQA S 3432*/
    uint8 apiId)
{
    SomeIpTp_ResetTxRtData(txPduRtPtr);

    PduR_SomeIpTpTxConfirmation(txNSduCfgPtr->SomeIpTpTxNSduUpperLayerId, E_NOT_OK);

    SomeIpTp_DetReportRuntimeError(apiId, SOMEIPTP_E_DISASSEMBLY_INTERRUPT);
}

/******************************************************************************/
/*
 * Brief               Set tx runtime data to default value
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      txPduRtPtr: Pointer of tx pdu runtime data
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
SOMEIPTP_LOCAL_INLINE FUNC(void, SOMEIPTP_CODE)
    SomeIpTp_ResetTxRtData(P2VAR(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) txPduRtPtr /*PRQA S 3432*/
    )
{
    txPduRtPtr->pduState = SOMEIPTP_PDU_IDLE;
    txPduRtPtr->separationTimer = 0u;
    txPduRtPtr->txTimeoutTimer = 0u;
    txPduRtPtr->availableDataLen = 0u;
    txPduRtPtr->txDataCnt = 0u;
    txPduRtPtr->txPduLen = 0u;
    txPduRtPtr->segmentSize = 0u;
    txPduRtPtr->firstSegmentFlag = FALSE;
    txPduRtPtr->noSegmentFlag = FALSE;
    txPduRtPtr->lastSegmentFlag = FALSE;
    (void)ILib_memset(&txPduRtPtr->header, 0x00, SOMEIPTP_SOMEIP_HEADER_LEN);
}

/******************************************************************************/
/*
 * Brief               Get channel config data by TxPduId
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      txPduId: tx pdu id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              When find corresponding config data ,return pointer to
 *                     the channel config data.When not find,return NULL_PTR.
 */
/******************************************************************************/
static FUNC(const SomeIpTp_ChannelType*, SOMEIPTP_CODE) SomeIpTp_GetTxChannelCfgPtr(PduIdType txPduId)
{
    P2CONST(SomeIpTp_ChannelType, AUTOMATIC, SOMEIPTP_APPL_CONST) chCfgPtr;
    P2CONST(SomeIpTp_TxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST) txNSduCfgPtr;
    uint16 ch;
    uint16 idx;
    boolean findFlag = FALSE;

    for (ch = 0u; ch < SOMEIPTP_CHANNEL_NUM; ch++) /*PRQA S 2877*/
    {
        chCfgPtr = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
        txNSduCfgPtr = chCfgPtr->SomeIpTpTxNSdu;
        for (idx = 0u; idx < chCfgPtr->SomeIpTpTxNSduNum; idx++)
        {
            if (txNSduCfgPtr->SomeIpTpTxNPduHandleId == txPduId)
            {
                findFlag = TRUE;
                break;
            }
            txNSduCfgPtr++;
        }

        if (TRUE == findFlag)
        {
            break;
        }
    }

    if (FALSE == findFlag)
    {
        chCfgPtr = NULL_PTR;
    }

    return chCfgPtr;
}

/******************************************************************************/
/*
 * Brief               Get TxNSdu config data(SomeIpTpTxNSdu) by TxPduId
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      txPduId: tx pdu id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(const SomeIpTp_TxNSduType*, SOMEIPTP_CODE) SomeIpTp_GetTxNSduCfgPtr(PduIdType txPduId)
{
    P2CONST(SomeIpTp_ChannelType, AUTOMATIC, SOMEIPTP_APPL_CONST) chCfgPtr;
    P2CONST(SomeIpTp_TxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST) txNSduCfgPtr;
    uint16 ch;
    uint16 idx;
    boolean findFlag = FALSE;

    for (ch = 0u; ch < SOMEIPTP_CHANNEL_NUM; ch++) /*PRQA S 2877*/
    {
        chCfgPtr = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
        txNSduCfgPtr = chCfgPtr->SomeIpTpTxNSdu;
        for (idx = 0u; idx < chCfgPtr->SomeIpTpTxNSduNum; idx++)
        {
            if (txNSduCfgPtr->SomeIpTpTxNPduHandleId == txPduId)
            {
                findFlag = TRUE;
                break;
            }
            txNSduCfgPtr++;
        }

        if (TRUE == findFlag)
        {
            break;
        }
    }

    if (FALSE == findFlag)
    {
        txNSduCfgPtr = NULL_PTR;
    }

    return txNSduCfgPtr;
}

/******************************************************************************/
/*
 * Brief               Calculate next to be transmited segment's size
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      txNSduCfgPtr: Pointer of SomeIpTpTxNSdu config data
 *                     txPduRtPtr: Pointer of tx pdu runtime data
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Segment size to be transmit
 */
/******************************************************************************/
static FUNC(PduLengthType, SOMEIPTP_CODE) SomeIpTp_CalcNextSegmentSize(
    P2CONST(SomeIpTp_TxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST) txNSduCfgPtr,
    P2VAR(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) txPduRtPtr /*PRQA S 3432*/
)
{
    PduLengthType segmentSize;
    PduLengthType availablePayloadSize;
    PduLengthType maxLenOfLowerPdu;
    PduLengthType remainDataLen;
    boolean needSegmentFlag = TRUE;

    remainDataLen = txPduRtPtr->txPduLen - txPduRtPtr->txDataCnt;
    /* First segment */
    if (0u == txPduRtPtr->txDataCnt)
    {
        txPduRtPtr->firstSegmentFlag = TRUE;

        if ((txPduRtPtr->availableDataLen == remainDataLen)
            && (remainDataLen
                <= SomeIpTp_Min(txNSduCfgPtr->SomeIpTpTxMaxPduLen, SOMEIPTP_MAX_PDU_LEN - SOMEIPTP_TP_HEADER_LEN)))
        {
            /* SDU can be send in one frame */
            segmentSize = remainDataLen;
            needSegmentFlag = FALSE;

            txPduRtPtr->noSegmentFlag = TRUE;
            txPduRtPtr->lastSegmentFlag = TRUE;
        }
        else
        {
            availablePayloadSize = txPduRtPtr->availableDataLen - SOMEIPTP_SOMEIP_HEADER_LEN;
            /* Calculate the max data with SOMEIPTP_OFFSETFIELD_LSB */
            availablePayloadSize = (availablePayloadSize / SOMEIPTP_OFFSETFIELD_LSB) * SOMEIPTP_OFFSETFIELD_LSB;
        }
    }
    else
    {
        if ((txPduRtPtr->availableDataLen == remainDataLen)
            && ((remainDataLen + SOMEIPTP_TOTAL_HEADER_LEN)
                <= SomeIpTp_Min(txNSduCfgPtr->SomeIpTpTxMaxPduLen, SOMEIPTP_MAX_PDU_LEN)))
        {
            /* Last segment,Left SDU can be send in one frame */
            segmentSize = remainDataLen + SOMEIPTP_TOTAL_HEADER_LEN;
            needSegmentFlag = FALSE;

            txPduRtPtr->lastSegmentFlag = TRUE;
        }
        else
        {
            /* Calculate the max data with SOMEIPTP_OFFSETFIELD_LSB */
            availablePayloadSize = (txPduRtPtr->availableDataLen / SOMEIPTP_OFFSETFIELD_LSB) * SOMEIPTP_OFFSETFIELD_LSB;
        }
    }

    if (TRUE == needSegmentFlag)
    {
        /* @req SWS_SomeIpTp_00017,@req SWS_SomeIpTp_00019 */
        /* Calculate the possible maximum size for next segment*/
        maxLenOfLowerPdu = txNSduCfgPtr->SomeIpTpTxMaxPduLen - SOMEIPTP_TOTAL_HEADER_LEN;
        /* Calculate the max data with SOMEIPTP_OFFSETFIELD_LSB */
        maxLenOfLowerPdu = (maxLenOfLowerPdu / SOMEIPTP_OFFSETFIELD_LSB) * SOMEIPTP_OFFSETFIELD_LSB;
        maxLenOfLowerPdu = maxLenOfLowerPdu + SOMEIPTP_TOTAL_HEADER_LEN;
        maxLenOfLowerPdu = SomeIpTp_Min(maxLenOfLowerPdu, SOMEIPTP_MAX_PDU_LEN);

        segmentSize = availablePayloadSize + SOMEIPTP_TOTAL_HEADER_LEN;
        segmentSize = SomeIpTp_Min(segmentSize, maxLenOfLowerPdu);
    }

    return segmentSize;
}

/******************************************************************************/
/*
 * Brief               Calculate data size need to be copied from upper layer
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      txPduRtPtr: Pointer of tx pdu runtime data
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Data size to be copied from upper layer
 */
/******************************************************************************/
static FUNC(PduLengthType, SOMEIPTP_CODE) SomeIpTp_CalcSizeGetFromUpperLayer(
    P2CONST(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) /* PRQA S 1532, 3432 */ /* MISRA Rule 8.7, 20.7 */
    txPduRtPtr)
{
    PduLengthType dataSizeFromUpper;

    /* First segment */
    if (TRUE == txPduRtPtr->firstSegmentFlag)
    {
        if (TRUE == txPduRtPtr->noSegmentFlag)
        {
            /* SDU can be send in one frame */
            dataSizeFromUpper = txPduRtPtr->segmentSize;
        }
        else
        {
            dataSizeFromUpper = txPduRtPtr->segmentSize - SOMEIPTP_TP_HEADER_LEN;
        }
    }
    else
    {
        dataSizeFromUpper = txPduRtPtr->segmentSize - SOMEIPTP_TOTAL_HEADER_LEN;
    }

    return dataSizeFromUpper;
}

/******************************************************************************/
/*
 * Brief               Calculate address when copy data from upper layer
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      txPduRtPtr: Pointer of tx pdu runtime data
 *                     sduDataPtr: Address passed by SomeIpTp_TriggerTransmit()
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Address to be used when copy data from upper layer
 */
/******************************************************************************/
static FUNC(uint8*, SOMEIPTP_CODE) SomeIpTp_CalcAddrToCopyDataFromUpperLayer(
    P2CONST(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) txPduRtPtr, /*PRQA S 3432*/
    P2VAR(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) sduDataPtr                   /*PRQA S 3432*/
)
{
    uint8* addr;

    /* First segment */
    if (TRUE == txPduRtPtr->firstSegmentFlag)
    {
        if (TRUE == txPduRtPtr->noSegmentFlag)
        {
            /* SDU can be send in one frame */
            addr = sduDataPtr;
        }
        else
        {
            addr = &sduDataPtr[SOMEIPTP_TP_HEADER_LEN];
        }
    }
    else
    {
        addr = &sduDataPtr[SOMEIPTP_TOTAL_HEADER_LEN];
    }

    return addr;
}

/******************************************************************************/
/*
 * Brief               Build header(Set TP-Flag bit and add TP header)
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      txPduRtPtr: Pointer of tx pdu runtime data
 *                     headerAddr: Header address
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SOMEIPTP_CODE) SomeIpTp_BuildHeader(
    P2CONST(SomeIpTp_TxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) txPduRtPtr, /*PRQA S 3432*/
    P2VAR(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) headerAddr                   /*PRQA S 3432*/
)
{
    SomeIpTp_TpHeaderType tpHeader;

    /* Copy SOME/IP Header to first 8 byte */
    (void)ILib_memcpy(headerAddr, (const uint8*)&txPduRtPtr->header, SOMEIPTP_SOMEIP_HEADER_LEN);

    /* Set TP-Flag bit in Message Type Field to 1 */
    headerAddr[SOMEIPTP_HEADER_MESSAGE_TYPE_INDEX] |= SOMEIPTP_TPFLAG_MASK;

    /* Set TP-Header */
    if (0u == txPduRtPtr->txDataCnt)
    {
        tpHeader.offset = 0u;
    }
    else
    {
        tpHeader.offset = (uint32)(((uint32)txPduRtPtr->txDataCnt - SOMEIPTP_TP_HEADER_LEN) / SOMEIPTP_OFFSETFIELD_LSB);
    }
    tpHeader.res = 0u;
    if (TRUE == txPduRtPtr->lastSegmentFlag)
    {
        tpHeader.moreSegment = 0u;
    }
    else
    {
        tpHeader.moreSegment = 1u;
    }

#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    SomeIpTp_EndianSwap((uint8*)&tpHeader, 4u);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

    (void)ILib_memcpy(&headerAddr[SOMEIPTP_SOMEIP_HEADER_LEN], (uint8*)&tpHeader, sizeof(SomeIpTp_TpHeaderType));
}

/******************************************************************************/
/*
 * Brief               Handle receive relevant timer
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for differnt channel
 * Param-Name[in]      ch: SomeIpTp Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SOMEIPTP_CODE) SomeIpTp_RxTimerHandle(uint16 ch)
{
    P2CONST(SomeIpTp_ChannelType, AUTOMATIC, SOMEIPTP_APPL_CONST)
    chCfgPtr = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
    P2CONST(SomeIpTp_RxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST)
    rxNSduCfgPtr = chCfgPtr->SomeIpTpRxNSdu;
    PduIdType firstPduOfChannel = chCfgPtr->SomeIpTpRxNSdu[0u].SomeIpTpRxNPduHandleId;
    P2VAR(SomeIpTp_RxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) /*PRQA S 3432*/
    rxPduRtPtr = &SomeIpTp_RxPduRTData[firstPduOfChannel];
    PduIdType pduIdx;

    for (pduIdx = 0u; pduIdx < chCfgPtr->SomeIpTpRxNSduNum; pduIdx++)
    {
        /* Handle rx timeout timer */
        if (rxPduRtPtr->rxTimeoutTimer > 0u)
        {
            rxPduRtPtr->rxTimeoutTimer--;
            if (0u == rxPduRtPtr->rxTimeoutTimer)
            {
                /* @req SWS_SomeIpTp_00041 */
                SomeIpTp_RxCancelHandle(
                    rxPduRtPtr,
                    rxNSduCfgPtr,
                    SOMEIPTP_SID_MAINFUNCTIONRX,
                    SOMEIPTP_E_ASSEMBLY_INTERRUPT);
            }
        }
        rxPduRtPtr++;
        rxNSduCfgPtr++;
    }
}

/******************************************************************************/
/*
 * Brief               Check received message is valid
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      RxPduId: Rx pdu id
 *                     PduInfoPtr: PduInfoPtr passed by SomeIpTp_RxIndication()
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Check pass
 *                     E_NOT_OK: Check not pass
 */
/******************************************************************************/
static FUNC(Std_ReturnType, SOMEIPTP_CODE)
    SomeIpTp_RxMessageCheck(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr)
{
    SomeIpTp_RxPduRTType* rxPduRtPtr = &SomeIpTp_RxPduRTData[RxPduId];
    const SomeIpTp_RxNSduType* rxNSduCfgPtr = SomeIpTp_GetRxNSduCfgPtr(RxPduId);
    SomeIpTp_HeaderType header;
    SomeIpTp_TpHeaderType tpHeader = SomeIpTp_ParseTpHeader(&PduInfoPtr->SduDataPtr[SOMEIPTP_SOMEIP_HEADER_LEN]);
    Std_ReturnType ret = E_OK;
    boolean singleSegmentFlag = FALSE;

    (void)ILib_memcpy((uint8*)&header, PduInfoPtr->SduDataPtr, sizeof(SomeIpTp_HeaderType));
    if ((header.messageType & SOMEIPTP_TPFLAG_MASK) == 0u)
    {
        singleSegmentFlag = TRUE;
    }

    if ((0u == PduInfoPtr->SduLength) || (PduInfoPtr->SduLength > SOMEIPTP_MAX_PDU_LEN))
    {
        /* Ignore the received Pdu */
        ret = E_NOT_OK;
    }

    if ((E_NOT_OK != ret) && (FALSE == singleSegmentFlag) && (SOMEIPTP_PDU_IDLE == rxPduRtPtr->pduState)
        && (tpHeader.offset > 0u))
    {
        /* @req SWS_SomeIpTp_00042 */
        /* Ignore the received Pdu and report runtime error */
        SomeIpTp_DetReportRuntimeError(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_INCONSISTENT_SEQUENCE);
        ret = E_NOT_OK;
    }

    if ((E_NOT_OK != ret) && (SOMEIPTP_PDU_RX_CONSECUTIVE_SEG == rxPduRtPtr->pduState) && (tpHeader.offset > 0u)
        && ((header.messageType & SOMEIPTP_TPFLAG_MASK) == 0u))
    {
        /* @req SWS_SomeIpTp_00045 */
        SomeIpTp_RxCancelHandle(rxPduRtPtr, rxNSduCfgPtr, SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_MESSAGE_TYPE);
        ret = E_NOT_OK;
    }

    if ((E_NOT_OK != ret) && (FALSE == singleSegmentFlag) && (tpHeader.offset > 0u))
    {
        if (ILib_memcmp(PduInfoPtr->SduDataPtr, (uint8*)&rxPduRtPtr->header, SOMEIPTP_SOMEIP_HEADER_LEN) != 0)
        {
            /* @req SWS_SomeIpTp_00062 */
            SomeIpTp_RxCancelHandle(
                rxPduRtPtr,
                rxNSduCfgPtr,
                SOMEIPTP_SID_RXINDICATION,
                SOMEIPTP_E_INCONSISTENT_HEADER);
            ret = E_NOT_OK;
        }
    }

    if ((E_NOT_OK != ret) && (FALSE == singleSegmentFlag)
        && ((1u == tpHeader.moreSegment)
            && ((((PduInfoPtr->SduLength - SOMEIPTP_TOTAL_HEADER_LEN) % SOMEIPTP_OFFSETFIELD_LSB)) != 0u)))
    {
        /* @req SWS_SomeIpTp_00063 */
        SomeIpTp_RxCancelHandle(rxPduRtPtr, rxNSduCfgPtr, SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_ASSEMBLY_INTERRUPT);
        ret = E_NOT_OK;
    }

    if ((E_NOT_OK != ret) && (FALSE == singleSegmentFlag)
        && (rxPduRtPtr->rxPayloadCnt != (tpHeader.offset * SOMEIPTP_OFFSETFIELD_LSB)))
    {
        /* @req SWS_SomeIpTp_00064, @req SWS_SomeIpTp_00048 */
        SomeIpTp_RxCancelHandle(rxPduRtPtr, rxNSduCfgPtr, SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_INCONSISTENT_SEQUENCE);
        if (0u != tpHeader.offset)
        {
            /* @req SWS_SomeIpTp_00064 */
            ret = E_NOT_OK;
        }
        else
        {
            /* @req SWS_SomeIpTp_00048 */
            /* ret = E_OK,Restart a new assembly process */
        }
    }

    if ((E_NOT_OK != ret) && (TRUE == singleSegmentFlag) && (SOMEIPTP_PDU_IDLE != rxPduRtPtr->pduState))
    {
        /* @req SWS_SomeIpTp_00048 */
        /* Restart a new assembly process */
        SomeIpTp_RxCancelHandle(rxPduRtPtr, rxNSduCfgPtr, SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_INCONSISTENT_SEQUENCE);
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Receive cancel process
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      txPduRtPtr: Pointer of rx pdu runtime data
 *                     rxNSduCfgPtr: Pointer of SomeIpTpRxNSdu
 *                     apiId: API id
 *                     errorId: Runtime error id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SOMEIPTP_CODE) SomeIpTp_RxCancelHandle(
    P2VAR(SomeIpTp_RxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) rxPduRtPtr, /*PRQA S 3432*/
    P2CONST(SomeIpTp_RxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST) rxNSduCfgPtr,
    uint8 apiId,
    uint8 errorId)
{
    SomeIpTp_ResetRxRtData(rxPduRtPtr);

    /* @req SWS_SomeIpTp_00054 */
    PduR_SomeIpTpRxIndication(rxNSduCfgPtr->SomeIpTpRxSduRef, E_NOT_OK);

    SomeIpTp_DetReportRuntimeError(apiId, errorId);
}

/******************************************************************************/
/*
 * Brief               Set receive runtime data to default value
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      txPduRtPtr: Pointer of rx pdu runtime data
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
SOMEIPTP_LOCAL_INLINE FUNC(void, SOMEIPTP_CODE)
    SomeIpTp_ResetRxRtData(P2VAR(SomeIpTp_RxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) rxPduRtPtr /*PRQA S 3432*/
    )
{
    rxPduRtPtr->pduState = SOMEIPTP_PDU_IDLE;
    rxPduRtPtr->rxTimeoutTimer = 0u;
    rxPduRtPtr->rxPayloadCnt = 0u;
    rxPduRtPtr->bufferSize = 0u;
    (void)ILib_memset(&rxPduRtPtr->header, 0x00, SOMEIPTP_SOMEIP_HEADER_LEN);
}

/******************************************************************************/
/*
 * Brief               Get channel config data by RxPduId
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      rxPduId: rx pdu id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              When find corresponding config data ,return pointer to
 *                     the channel config data.When not find,return NULL_PTR.
 */
/******************************************************************************/
static FUNC(const SomeIpTp_ChannelType*, SOMEIPTP_CODE) SomeIpTp_GetRxChannelCfgPtr(PduIdType rxPduId)
{
    P2CONST(SomeIpTp_ChannelType, AUTOMATIC, SOMEIPTP_APPL_CONST) chCfgPtr;
    P2CONST(SomeIpTp_RxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST) rxNSduCfgPtr;
    uint16 ch;
    uint16 idx;
    boolean findFlag = FALSE;

    for (ch = 0u; ch < SOMEIPTP_CHANNEL_NUM; ch++) /*PRQA S 2877*/
    {
        chCfgPtr = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
        rxNSduCfgPtr = chCfgPtr->SomeIpTpRxNSdu;
        for (idx = 0u; idx < chCfgPtr->SomeIpTpRxNSduNum; idx++)
        {
            if (rxNSduCfgPtr->SomeIpTpRxNPduHandleId == rxPduId)
            {
                findFlag = TRUE;
                break;
            }
            rxNSduCfgPtr++;
        }

        if (TRUE == findFlag)
        {
            break;
        }
    }

    if (FALSE == findFlag)
    {
        chCfgPtr = NULL_PTR;
    }

    return chCfgPtr;
}

/******************************************************************************/
/*
 * Brief               Get RxNSdu config data(SomeIpTpRxNSdu) by RxPduId
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      rxPduId: rx pdu id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              When find corresponding config data ,return pointer to
 *                     the RxNSdu config data.When not find,return NULL_PTR.
 */
/******************************************************************************/
static FUNC(const SomeIpTp_RxNSduType*, SOMEIPTP_CODE) SomeIpTp_GetRxNSduCfgPtr(PduIdType rxPduId)
{
    P2CONST(SomeIpTp_ChannelType, AUTOMATIC, SOMEIPTP_APPL_CONST) chCfgPtr;
    P2CONST(SomeIpTp_RxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST) rxNSduCfgPtr;
    uint16 ch;
    uint16 idx;
    boolean findFlag = FALSE;

    for (ch = 0u; ch < SOMEIPTP_CHANNEL_NUM; ch++) /*PRQA S 2877*/
    {
        chCfgPtr = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
        rxNSduCfgPtr = chCfgPtr->SomeIpTpRxNSdu;
        for (idx = 0u; idx < chCfgPtr->SomeIpTpRxNSduNum; idx++)
        {
            if (rxNSduCfgPtr->SomeIpTpRxNPduHandleId == rxPduId)
            {
                findFlag = TRUE;
                break;
            }
            rxNSduCfgPtr++;
        }

        if (TRUE == findFlag)
        {
            break;
        }
    }

    if (FALSE == findFlag)
    {
        rxNSduCfgPtr = NULL_PTR;
    }

    return rxNSduCfgPtr;
}

/******************************************************************************/
/*
 * Brief               Procee when receive single segment
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      rxPduId: rx pdu id
 *                     PduInfoPtr: PduInfoPtr passed by SomeIpTp_RxIndication()
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SOMEIPTP_CODE)
    SomeIpTp_RxSingleSegmentHandle(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr)
{
    P2CONST(SomeIpTp_ChannelType, AUTOMATIC, SOMEIPTP_APPL_CONST)
    chCfgPtr = SomeIpTp_GetRxChannelCfgPtr(RxPduId);
    P2VAR(SomeIpTp_RxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) /*PRQA S 3432*/
    rxPduRtPtr = &SomeIpTp_RxPduRTData[RxPduId];
    P2CONST(SomeIpTp_RxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST)
    rxNSduCfgPtr = SomeIpTp_GetRxNSduCfgPtr(RxPduId);
    BufReq_ReturnType bufReqRet;
    PduInfoType pduInfo;
    boolean errorQuitFlag = FALSE;

    rxPduRtPtr->pduState = SOMEIPTP_PDU_RX_SINGLE_SEG;

    /* @req SWS_SomeIpTp_00033 */
    /* Start the Rx timeout timer */
    rxPduRtPtr->rxTimeoutTimer = chCfgPtr->SomeIpTpRxTimeoutTime;
    bufReqRet = PduR_SomeIpTpStartOfReception(rxNSduCfgPtr->SomeIpTpRxSduRef, NULL_PTR, 0u, &rxPduRtPtr->bufferSize);
    if (BUFREQ_OK != bufReqRet)
    {
        /* @req SWS_SomeIpTp_00051 */
        SomeIpTp_ResetRxRtData(rxPduRtPtr);
        SomeIpTp_DetReportRuntimeError(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_ASSEMBLY_INTERRUPT);
    }
    else
    {
        /* @req SWS_SomeIpTp_00034 */
        if (rxPduRtPtr->bufferSize < PduInfoPtr->SduLength)
        {
            /* @req SWS_SomeIpTp_00049 */
            errorQuitFlag = TRUE;
        }
        else
        {
            /* @req SWS_SomeIpTp_00031 */
            pduInfo.SduLength = PduInfoPtr->SduLength;
            pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
            /* @req SWS_SomeIpTp_00034 */
            bufReqRet = PduR_SomeIpTpCopyRxData(rxNSduCfgPtr->SomeIpTpRxSduRef, &pduInfo, &rxPduRtPtr->bufferSize);
            if (BUFREQ_OK != bufReqRet)
            {
                /* @req SWS_SomeIpTp_00050 */
                errorQuitFlag = TRUE;
            }
        }

        /* Error occur,Cancel receive process */
        if (TRUE == errorQuitFlag)
        {
            SomeIpTp_RxCancelHandle(rxPduRtPtr, rxNSduCfgPtr, SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_ASSEMBLY_INTERRUPT);
        }
        else
        {
            /* No error occur */
            /* Single frame, call PduR_SomeIpTpRxIndication() */
            /* @req SWS_SomeIpTp_00031, @req SWS_SomeIpTp_00071 */
            PduR_SomeIpTpRxIndication(rxNSduCfgPtr->SomeIpTpRxSduRef, E_OK);

            /* Receive finish, clear rx runtime buffer */
            SomeIpTp_ResetRxRtData(rxPduRtPtr);
        }
    }
}

/******************************************************************************/
/*
 * Brief               Procee when receive first segment
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      rxPduId: rx pdu id
 *                     PduInfoPtr: PduInfoPtr passed by SomeIpTp_RxIndication()
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SOMEIPTP_CODE)
    SomeIpTp_RxFirstSegmentHandle(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr)
{
    static VAR(uint8, SOMEIPTP_VAR) SomeIpTp_RxBuffer[SOMEIPTP_RX_BUFFER_SIZE + SOMEIPTP_SOMEIP_HEADER_LEN];
    P2CONST(SomeIpTp_ChannelType, AUTOMATIC, SOMEIPTP_APPL_CONST)
    chCfgPtr = SomeIpTp_GetRxChannelCfgPtr(RxPduId);
    P2VAR(SomeIpTp_RxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) /*PRQA S 3432*/
    rxPduRtPtr = &SomeIpTp_RxPduRTData[RxPduId];
    P2CONST(SomeIpTp_RxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST)
    rxNSduCfgPtr = SomeIpTp_GetRxNSduCfgPtr(RxPduId);
    SomeIpTp_TpHeaderType tpHeader = SomeIpTp_ParseTpHeader(&PduInfoPtr->SduDataPtr[SOMEIPTP_SOMEIP_HEADER_LEN]);
    BufReq_ReturnType bufReqRet;
    PduInfoType pduInfo;
    boolean errorQuitFlag = FALSE;

    rxPduRtPtr->pduState = SOMEIPTP_PDU_RX_FIRST_SEG;

    /* @req SWS_SomeIpTp_00032 */
    /* Save header to internal rx buffer */
    (void)ILib_memcpy((uint8*)&rxPduRtPtr->header, PduInfoPtr->SduDataPtr, sizeof(SomeIpTp_HeaderType));

    /* @req SWS_SomeIpTp_00033 */
    /* Start the Rx timeout timer */
    rxPduRtPtr->rxTimeoutTimer = chCfgPtr->SomeIpTpRxTimeoutTime;
    bufReqRet = PduR_SomeIpTpStartOfReception(rxNSduCfgPtr->SomeIpTpRxSduRef, NULL_PTR, 0u, &rxPduRtPtr->bufferSize);
    if (BUFREQ_OK != bufReqRet)
    {
        /* @req SWS_SomeIpTp_00051 */
        SomeIpTp_ResetRxRtData(rxPduRtPtr);
        SomeIpTp_DetReportRuntimeError(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_ASSEMBLY_INTERRUPT);
    }
    else
    {
        /* @req SWS_SomeIpTp_00034 */
        if (rxPduRtPtr->bufferSize < (PduInfoPtr->SduLength - SOMEIPTP_TP_HEADER_LEN))
        {
            /* @req SWS_SomeIpTp_00049 */
            errorQuitFlag = TRUE;
        }
        else
        {
            /* Assembly first frame,include SOME/IP header and payload*/
            (void)ILib_memcpy(SomeIpTp_RxBuffer, PduInfoPtr->SduDataPtr, SOMEIPTP_SOMEIP_HEADER_LEN);
            (void)ILib_memcpy(
                &SomeIpTp_RxBuffer[SOMEIPTP_SOMEIP_HEADER_LEN],
                &PduInfoPtr->SduDataPtr[SOMEIPTP_TOTAL_HEADER_LEN],
                (uint32)PduInfoPtr->SduLength - SOMEIPTP_TOTAL_HEADER_LEN);
            /* @req SWS_SomeIpTp_00035 */
            /* Clear TP-Flag */
            SomeIpTp_RxBuffer[SOMEIPTP_HEADER_MESSAGE_TYPE_INDEX] &= (~SOMEIPTP_TPFLAG_MASK);

            pduInfo.SduLength = PduInfoPtr->SduLength - SOMEIPTP_TP_HEADER_LEN;
            pduInfo.SduDataPtr = SomeIpTp_RxBuffer;

            /* @req SWS_SomeIpTp_00034 */
            bufReqRet = PduR_SomeIpTpCopyRxData(rxNSduCfgPtr->SomeIpTpRxSduRef, &pduInfo, &rxPduRtPtr->bufferSize);
            if (BUFREQ_OK != bufReqRet)
            {
                /* @req SWS_SomeIpTp_00050 */
                errorQuitFlag = TRUE;
            }
        }

        /* Error occur,Cancel receive process */
        if (TRUE == errorQuitFlag)
        {
            SomeIpTp_RxCancelHandle(rxPduRtPtr, rxNSduCfgPtr, SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_ASSEMBLY_INTERRUPT);
        }
        else if (0u == tpHeader.moreSegment)
        {
            /* @req SWS_SomeIpTp_00040 */
            PduR_SomeIpTpRxIndication(rxNSduCfgPtr->SomeIpTpRxSduRef, E_OK);

            /* Receive finish, clear rx runtime buffer */
            SomeIpTp_ResetRxRtData(rxPduRtPtr);
        }
        else
        {
            /* No error occur */
            /* @req SWS_SomeIpTp_00036 */
            rxPduRtPtr->rxPayloadCnt = pduInfo.SduLength - SOMEIPTP_SOMEIP_HEADER_LEN;
            rxPduRtPtr->pduState = SOMEIPTP_PDU_RX_CONSECUTIVE_SEG;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Procee when receive consecutive segment
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      RxPduId: rx pdu id
 *                     PduInfoPtr: PduInfoPtr passed by SomeIpTp_RxIndication()
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, SOMEIPTP_CODE) SomeIpTp_RxConsecutiveSegmentHandle(
    PduIdType RxPduId,
    P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr)
{
    P2CONST(SomeIpTp_ChannelType, AUTOMATIC, SOMEIPTP_APPL_CONST)
    chCfgPtr = SomeIpTp_GetRxChannelCfgPtr(RxPduId);
    P2VAR(SomeIpTp_RxPduRTType, AUTOMATIC, SOMEIPTP_APPL_DATA) /*PRQA S 3432*/
    rxPduRtPtr = &SomeIpTp_RxPduRTData[RxPduId];
    P2CONST(SomeIpTp_RxNSduType, AUTOMATIC, SOMEIPTP_APPL_CONST)
    rxNSduCfgPtr = SomeIpTp_GetRxNSduCfgPtr(RxPduId);
    SomeIpTp_TpHeaderType tpHeader = SomeIpTp_ParseTpHeader(&PduInfoPtr->SduDataPtr[SOMEIPTP_SOMEIP_HEADER_LEN]);
    BufReq_ReturnType bufReqRet;
    PduInfoType pduInfo;
    boolean errorQuitFlag = FALSE;

    /* @req SWS_SomeIpTp_00039 */
    if (rxPduRtPtr->bufferSize >= (PduInfoPtr->SduLength - SOMEIPTP_TOTAL_HEADER_LEN))
    {
        pduInfo.SduLength = PduInfoPtr->SduLength - SOMEIPTP_TOTAL_HEADER_LEN;
        pduInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[SOMEIPTP_TOTAL_HEADER_LEN];

        bufReqRet = PduR_SomeIpTpCopyRxData(rxNSduCfgPtr->SomeIpTpRxSduRef, &pduInfo, &rxPduRtPtr->bufferSize);
        if (BUFREQ_OK != bufReqRet)
        {
            /* @req SWS_SomeIpTp_00050 */
            errorQuitFlag = TRUE;
        }
    }
    else
    {
        /* @req SWS_SomeIpTp_00049 */
        errorQuitFlag = TRUE;
    }

    /* Error occur,Cancel receive process */
    if (TRUE == errorQuitFlag)
    {
        SomeIpTp_RxCancelHandle(rxPduRtPtr, rxNSduCfgPtr, SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_ASSEMBLY_INTERRUPT);
    }
    else
    {
        /* No error occur */
        if (0u == tpHeader.moreSegment)
        {
            /* @req SWS_SomeIpTp_00040 */
            PduR_SomeIpTpRxIndication(rxNSduCfgPtr->SomeIpTpRxSduRef, E_OK);

            /* Receive finish, clear rx runtime buffer */
            SomeIpTp_ResetRxRtData(rxPduRtPtr);
        }
        else
        {
            /* @req SWS_SomeIpTp_00037 */
            /* Restart Rx Timeout Timer */
            rxPduRtPtr->rxTimeoutTimer = chCfgPtr->SomeIpTpRxTimeoutTime;

            /* @req SWS_SomeIpTp_00038 */
            rxPduRtPtr->rxPayloadCnt += pduInfo.SduLength;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Switch data byte order
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      dataPtr: Pointer to data to be convert
 *                     size: Data length in bytes
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
SOMEIPTP_LOCAL_INLINE FUNC(void, SOMEIPTP_CODE) SomeIpTp_EndianSwap(uint8* dataPtr, uint8 size)
{
    uint8 i;
    uint8 temp;
    uint8 halfSize = size >> 1u;
    uint8 tSize = size - 1u;
    uint8 tIdx;

    for (i = 0u; i < halfSize; i++)
    {
        tIdx = tSize - i;
        temp = dataPtr[tIdx];
        dataPtr[tIdx] = dataPtr[i];
        dataPtr[i] = temp;
    }
}
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

/******************************************************************************/
/*
 * Brief               Parse TP header
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      dataPtr: Pointer to rx tp header
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(SomeIpTp_TpHeaderType, SOMEIPTP_CODE)
    SomeIpTp_ParseTpHeader(P2CONST(void, AUTOMATIC, SOMEIPTP_APPL_DATA) dataPtr)
{
    SomeIpTp_TpHeaderType tpHeader;

    (void)ILib_memcpy((uint8*)&tpHeader, (const uint8*)dataPtr, sizeof(SomeIpTp_TpHeaderType));
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    SomeIpTp_EndianSwap((uint8*)&tpHeader, sizeof(SomeIpTp_TpHeaderType));
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

    return tpHeader;
}

/******************************************************************************/
/*
 * Brief               Get min value in input parameter a and b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      a,b
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              PduLengthType: Min value
 */
/******************************************************************************/
SOMEIPTP_LOCAL_INLINE FUNC(PduLengthType, SOMEIPTP_CODE) SomeIpTp_Min(PduLengthType a, PduLengthType b)
{
    return ((a < b) ? a : b);
}

/******************************************************************************/
/*
 * Brief               Report Det error to DET module
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      a,b
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              PduLengthType: Min value
 */
/******************************************************************************/
#if (STD_ON == SOMEIPTP_DEV_ERROR_DETECT)
SOMEIPTP_LOCAL_INLINE FUNC(void, SOMEIPTP_CODE) SomeIpTp_DetReportError(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(SOMEIPTP_MODULE_ID, SOMEIPTP_INSTANCE_ID, ApiId, ErrorId);
}
#endif

/******************************************************************************/
/*
 * Brief               Report runtime error to DET module
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      a,b
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              PduLengthType: Min value
 */
/******************************************************************************/
SOMEIPTP_LOCAL_INLINE FUNC(void, SOMEIPTP_CODE) SomeIpTp_DetReportRuntimeError(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportRuntimeError(SOMEIPTP_MODULE_ID, SOMEIPTP_INSTANCE_ID, ApiId, ErrorId);
}

#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"
