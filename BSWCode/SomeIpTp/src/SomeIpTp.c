/**
 * Copyright (C) 2008-2026 isoft Infrastructure Software Co., Ltd.
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
/*
 ************************************************************************************************************************
 **
 **  @file               : SomeIpTp.c
 **  @author             : HuRongbo
 **  @date               : 2020/07/07
 **  @vendor             : isoft
 **  @description        : SOMEIP Transport Protocol
 **
 ***********************************************************************************************************************/

/* PRQA S 3414 EOF */ /* VL_QAC_FctLikeMacro */
/* PRQA S 3415 EOF */ /* VL_SomeIpTp_3415 */
/* PRQA S 1503 EOF */ /* VL_QAC_NoUsedApi */
/* PRQA S 1532 EOF */ /* VL_QAC_OneFunRef */
/* PRQA S 6080 EOF */ /* VL_MTR_SomeIpTp_STPTH */
/* =================================================== inclusions =================================================== */
#include "SomeIpTp.h"
#include "SomeIpTp_Internal.h"
#include "SomeIpTp_Arti.h"
#include "PduR_SomeIpTp.h"
#include "Det.h"
#include "IStdLib.h"
#if (STD_ON == SOMEIPTP_ECUC_MULTIPLE_PARTITION_USED)
#include "Os.h"
#endif
/* ===================================================== macros ===================================================== */

#define SOMEIPTP_C_AR_MAJOR_VERSION 4U
#define SOMEIPTP_C_AR_MINOR_VERSION 9U
#define SOMEIPTP_C_AR_PATCH_VERSION 0U
#define SOMEIPTP_C_SW_MAJOR_VERSION 2U
#define SOMEIPTP_C_SW_MINOR_VERSION 0U
#define SOMEIPTP_C_SW_PATCH_VERSION 9U

/* ---------------------------------------------------- Version  Check -----------------------------------------------
 */
#if (SOMEIPTP_C_AR_MAJOR_VERSION != SOMEIPTP_AR_MAJOR_VERSION)
#error "SomeIpTp.c : Mismatch in Specification Major Version"
#endif
#if (SOMEIPTP_C_AR_MINOR_VERSION != SOMEIPTP_AR_MINOR_VERSION)
#error "SomeIpTp.c : Mismatch in Specification _MINOR Version"
#endif
#if (SOMEIPTP_C_AR_PATCH_VERSION != SOMEIPTP_AR_PATCH_VERSION)
#error "SomeIpTp.c : Mismatch in Specification PATCH Version"
#endif
#if (SOMEIPTP_C_SW_MAJOR_VERSION != SOMEIPTP_H_SW_MAJOR_VERSION)
#error "SomeIpTp.c : Mismatch in Software Major Version"
#endif
#if (SOMEIPTP_C_SW_MINOR_VERSION != SOMEIPTP_H_SW_MINOR_VERSION)
#error "SomeIpTp.c : Mismatch in Software MINOR Version"
#endif

/* ----------------------------------------------- Private Macro Definitions -----------------------------------------
 */

/* inline keyword define,if compiler not support inline,define SOMEIPTP_INLINE to null */
#define SOMEIPTP_LOCAL_INLINE static inline

#if !defined(SOMEIPTP_LOCAL)
#define SOMEIPTP_LOCAL static
#endif

#define SOMEIPTP_MAGIC_NUMBER_4_MAGIC         (4u)
#define SOMEIPTP_MAGIC_NUMBER_0_UNSIGNED_LONG (0UL)
#define SOMEIPTP_MORE_SEGMENT_FLAG_MASK       (1UL)

/* ================================================ type definitions ================================================ */

/* ============================================ internal data definition ============================================ */

#define SOMEIPTP_START_SEC_VAR_CLEARED_PTR
#include "SomeIpTp_MemMap.h"
/* Pointer to configuration data */
SOMEIPTP_LOCAL const SomeIpTp_ConfigType* SomeIpTp_ConfigPtr = NULL_PTR;
#define SOMEIPTP_STOP_SEC_VAR_CLEARED_PTR
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_VAR_CLEARED_GLOBAL_BOOLEAN
#include "SomeIpTp_MemMap.h"
/* Variable store SOMEIPTP initialization status */
SOMEIPTP_LOCAL boolean SomeIpTp_InitStatus[SOMEIPTP_USED_PARTITION_NUM];

#define SOMEIPTP_STOP_SEC_VAR_CLEARED_GLOBAL_BOOLEAN
#include "SomeIpTp_MemMap.h"

/* ============================================ external data definition ============================================ */

/* ========================================= internal function declarations ========================================= */

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL void SomeIpTp_TxTimerHandle(uint16 ch, uint8 someIpTpPartitionId);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL void SomeIpTp_TxPduHandle(uint16 ch, uint8 someIpTpPartitionId);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL void
    SomeIpTp_TxCancelHandle(const SomeIpTp_TxNSduType* txNSduCfgPtr, SomeIpTp_TxPduRTType* txPduRtPtr, uint8 apiId);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

SOMEIPTP_LOCAL_INLINE void SomeIpTp_ResetTxRtData(SomeIpTp_TxPduRTType* txPduRtPtr);

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL const SomeIpTp_ChannelType* SomeIpTp_GetTxChannelCfgPtr(PduIdType txPduId);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL const SomeIpTp_TxNSduType* SomeIpTp_GetTxNSduCfgPtr(PduIdType txPduId);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL PduLengthType
    SomeIpTp_CalcNextSegmentSize(const SomeIpTp_TxNSduType* txNSduCfgPtr, SomeIpTp_TxPduRTType* txPduRtPtr);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL PduLengthType SomeIpTp_CalcSizeGetFromUpperLayer(const SomeIpTp_TxPduRTType* txPduRtPtr);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL uint8*
    SomeIpTp_CalcAddrToCopyDataFromUpperLayer(const SomeIpTp_TxPduRTType* txPduRtPtr, uint8* sduDataPtr);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL void SomeIpTp_BuildHeader(const SomeIpTp_TxPduRTType* txPduRtPtr, uint8* headerAddr);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL void SomeIpTp_RxTimerHandle(uint16 ch, uint8 someIpTpPartitionId);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL Std_ReturnType SomeIpTp_RxMessageCheck(
    const PduInfoType*         PduInfoPtr,
    const SomeIpTp_RxNSduType* rxNSduCfgPtr,
    SomeIpTp_RxPduRTType*      rxPduRtPtr,
    uint32*                    tpHeaderPtr);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL void SomeIpTp_RxCancelHandle(
    SomeIpTp_RxPduRTType*      rxPduRtPtr,
    const SomeIpTp_RxNSduType* rxNSduCfgPtr,
    uint8                      apiId,
    uint8                      errorId);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

SOMEIPTP_LOCAL_INLINE void SomeIpTp_ResetRxRtData(SomeIpTp_RxPduRTType* rxPduRtPtr);

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL const SomeIpTp_ChannelType* SomeIpTp_GetRxChannelCfgPtr(PduIdType rxPduId);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL const SomeIpTp_RxNSduType* SomeIpTp_GetRxNSduCfgPtr(PduIdType rxPduId);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL void SomeIpTp_RxSingleSegmentHandle(
    const PduInfoType*         PduInfoPtr,
    const SomeIpTp_RxNSduType* rxNSduCfgPtr,
    SomeIpTp_RxPduRTType*      rxPduRtPtr);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL void SomeIpTp_RxFirstSegmentHandle(
    const PduInfoType*         PduInfoPtr,
    const SomeIpTp_RxNSduType* rxNSduCfgPtr,
    SomeIpTp_RxPduRTType*      rxPduRtPtr,
    uint32                     tpHeader);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL void SomeIpTp_RxConsecutiveSegmentHandle(
    const PduInfoType*         PduInfoPtr,
    const SomeIpTp_RxNSduType* rxNSduCfgPtr,
    SomeIpTp_RxPduRTType*      rxPduRtPtr,
    uint32                     tpHeader);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
SOMEIPTP_LOCAL_INLINE void SomeIpTp_EndianSwap(uint8* dataPtr, uint8 size);
#endif

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL uint32 SomeIpTp_ParseTpHeader(const void* dataPtr);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

SOMEIPTP_LOCAL_INLINE PduLengthType SomeIpTp_Min(PduLengthType a, PduLengthType b);

#if (STD_ON == SOMEIPTP_DEV_ERROR_DETECT)
SOMEIPTP_LOCAL_INLINE void SomeIpTp_DetReportError(uint8 ApiId, uint8 ErrorId);
#endif

SOMEIPTP_LOCAL_INLINE void SomeIpTp_DetReportRuntimeError(uint8 ApiId, uint8 ErrorId);

#if (STD_ON == SOMEIPTP_ECUC_MULTIPLE_PARTITION_USED)
SOMEIPTP_LOCAL_INLINE uint8 SomeIpTp_GetSomeIpTpPartionByEcuCParttion(uint8 ecuCPartition);
#endif

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL Std_ReturnType SomeIpTp_TriggerTransmitHandle(
    PduInfoType*               pduInfoPtr,
    const SomeIpTp_TxNSduType* txNSduCfgPtr,
    SomeIpTp_TxPduRTType*      txPduRtPtr);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL void SomeIpTp_RxIndicationHandle(
    PduIdType                  rxPduId,
    const PduInfoType*         pduInfoPtr,
    const SomeIpTp_RxNSduType* rxNSduCfgPtr,
    SomeIpTp_RxPduRTType*      rxPduRtPtr);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL void SomeIpTp_TxConfirmationHandle(
    PduIdType                  cfgTxPduId,
    const SomeIpTp_TxNSduType* txNSduCfgPtr,
    SomeIpTp_TxPduRTType*      txPduRtPtr);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL Std_ReturnType SomeIpTp_RxMessageCheckConsecutiveFragment(
    const PduInfoType*         pduInfoPtr,
    const SomeIpTp_RxNSduType* rxNSduCfgPtr,
    SomeIpTp_RxPduRTType*      rxPduRtPtr,
    uint32                     localTpHeader);
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"
/* ========================================== external function definition ========================================== */

/**
 * @brief    Returns the version information of this module.
 */
#if (STD_ON == SOMEIPTP_VERSION_INFO_API)
#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"

void SomeIpTp_GetVersionInfo(Std_VersionInfoType* VersionInfo)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, SomeIpTp, 0, Bsw_Start, 2, 0x01, VersionInfo);

#if (STD_ON == SOMEIPTP_DEV_ERROR_DETECT)
    if (NULL_PTR == VersionInfo)
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_GETVERSIONINFO, SOMEIPTP_E_PARAM_POINTER);
    }
    else
#endif
    {
        VersionInfo->vendorID         = SOMEIPTP_VENDOR_ID;
        VersionInfo->moduleID         = SOMEIPTP_MODULE_ID;
        VersionInfo->sw_major_version = SOMEIPTP_C_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = SOMEIPTP_C_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = SOMEIPTP_C_SW_PATCH_VERSION;
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, SomeIpTp, 0, Bsw_Return, 1, 0x01);
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"
#endif

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Initializes the SOME/IP TP module.
 */
void SomeIpTp_Init(const SomeIpTp_ConfigType* config)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, SomeIpTp, 0, Bsw_Start, 2, 0x02, config);

#if (STD_ON == SOMEIPTP_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition     = (uint8)GetApplicationID();
    uint8 someIpTpPartition = SomeIpTp_GetSomeIpTpPartionByEcuCParttion(ecuCPartition);
#else
    uint8 someIpTpPartition = 0u;
#endif
#if (STD_ON == SOMEIPTP_DEV_ERROR_DETECT)
#if (STD_ON == SOMEIPTP_ECUC_MULTIPLE_PARTITION_USED)
    if (someIpTpPartition >= SOMEIPTP_USED_PARTITION_NUM)
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_INIT, SOMEIPTP_E_INIT_FAILED);
    }
    else
#endif
        if (NULL_PTR == config)
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_INIT, SOMEIPTP_E_PARAM_POINTER);
    }
    else
#endif
    {
        PduIdType             pduIdx;
        SomeIpTp_TxPduRTType* txPduRtPtr;
        SomeIpTp_RxPduRTType* rxPduRtPtr;
        /* Initialize config data pointer*/
        SomeIpTp_ConfigPtr = config;
        /* Initialize run time structure variable for every receive pdu */
#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
        for (pduIdx = 0u; pduIdx < SomeIpTp_ConfigPtr->RxPduMaxNmberInCurrentVariant; pduIdx++)
#else
        for (pduIdx = 0u; pduIdx < SOMEIPTP_RX_PDU_MACRO_NUM; pduIdx++)
#endif
        {
            rxPduRtPtr = &SomeIpTp_RxPduRTData[someIpTpPartition][pduIdx];
            SomeIpTp_ResetRxRtData(rxPduRtPtr);
        }

        /* Initialize run time structure variable for every transmit pdu */
#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
        for (pduIdx = 0u; pduIdx < SomeIpTp_ConfigPtr->TxPduMaxNmberInCurrentVariant; pduIdx++)
#else
        for (pduIdx = 0u; pduIdx < SOMEIPTP_TX_PDU_MACRO_NUM; pduIdx++)
#endif
        {
            txPduRtPtr = &SomeIpTp_TxPduRTData[someIpTpPartition][pduIdx];
            SomeIpTp_ResetTxRtData(txPduRtPtr);
        }

        /* Change initialization status */
        SomeIpTp_InitStatus[someIpTpPartition] = TRUE;
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, SomeIpTp, 0, Bsw_Return, 1, 0x02);
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/* PRQA S 6030 ++ */ /* VL_MTR_SomeIpTp_QacSTMIF */
/**
 * @brief    Requests transmission of a PDU.
 */
Std_ReturnType SomeIpTp_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, SomeIpTp, 0, Bsw_Start, 3, 0x49, TxPduId, PduInfoPtr);

    /* PRQA S 6030 -- */
    /* @req SWS_SomeIpTp_00076 */
    Std_ReturnType ret        = E_NOT_OK;
    PduIdType      cfgTxPduId = TxPduId;
#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
    if (TxPduId < SOMEIPTP_TX_PDU_MACRO_NUM)
    {
        cfgTxPduId = SomeIpTp_ConfigPtr->TxNSduAndTxNPduMacroMap[TxPduId];
    }
#endif
#if (STD_ON == SOMEIPTP_DEV_ERROR_DETECT)
#if (STD_ON == SOMEIPTP_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition     = (uint8)GetApplicationID();
    uint8 someIpTpPartition = SomeIpTp_GetSomeIpTpPartionByEcuCParttion(ecuCPartition);
    if (someIpTpPartition >= SOMEIPTP_USED_PARTITION_NUM)
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_TRANSMIT, SOMEIPTP_E_PARAM);
    }
    else if (!SomeIpTp_InitStatus[someIpTpPartition])
#else
    if (!SomeIpTp_InitStatus[0u])
#endif
    {
        /* @req SWS_SomeIpTp_00073 */
        SomeIpTp_DetReportError(SOMEIPTP_SID_TRANSMIT, SOMEIPTP_E_UNINIT);
    }
#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
    else if (cfgTxPduId >= SomeIpTp_ConfigPtr->TxPduMaxNmberInCurrentVariant)
#else
    else if (cfgTxPduId >= SOMEIPTP_TX_PDU_MACRO_NUM)
#endif
    {
        /* @req SWS_SomeIpTp_00074 */
        SomeIpTp_DetReportError(SOMEIPTP_SID_TRANSMIT, SOMEIPTP_E_PARAM);
    }
    else if (NULL_PTR == PduInfoPtr)
    {
        /* @req SWS_SomeIpTp_00075 */
        SomeIpTp_DetReportError(SOMEIPTP_SID_TRANSMIT, SOMEIPTP_E_PARAM_POINTER);
    }
    else
#endif
    {
        const SomeIpTp_TxNSduType* txNSduCfgPtr = SomeIpTp_GetTxNSduCfgPtr(cfgTxPduId);
#if (SOMEIPTP_USED_PARTITION_NUM > 1)
        uint8                 partitionIdxInCfg = txNSduCfgPtr->SomeIpTpPartition;
        SomeIpTp_TxPduRTType* txPduRtPtr = &SomeIpTp_TxPduRTData[someIpTpPartition][txNSduCfgPtr->OnPartitionIndex];
#else
        SomeIpTp_TxPduRTType* txPduRtPtr = &SomeIpTp_TxPduRTData[0u][cfgTxPduId];
#endif
#if ((STD_ON == SOMEIPTP_DEV_ERROR_DETECT) && (SOMEIPTP_USED_PARTITION_NUM > 1))
        if (someIpTpPartition != partitionIdxInCfg)
        {
            SomeIpTp_DetReportError(SOMEIPTP_SID_TRANSMIT, SOMEIPTP_E_PARAM);
        }
        else
#endif
        {
            if (SOMEIPTP_PDU_IDLE != txPduRtPtr->PduState)
            {
                /* @SWS_SomeIpTp_00022: called with a PDU ID which is currently used for an ongoing segmentation. */
                SomeIpTp_TxCancelHandle(txNSduCfgPtr, txPduRtPtr, SOMEIPTP_SID_TRANSMIT);
            }
            else
            {
                /* @SWS_SomeIpTp_00001: module shall remember the PDU length separately for every PDU ID. */
                txPduRtPtr->PduState = SOMEIPTP_PDU_TX_WAIT;
                txPduRtPtr->TxPduLen = PduInfoPtr->SduLength;
                ret                  = E_OK;
            }
        }
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, SomeIpTp, 0, Bsw_Return, 2, 0x49, ret);

    return ret;
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE_FAST
#include "SomeIpTp_MemMap.h"

/* PRQA S 6030 ++ */ /* VL_MTR_SomeIpTp_QacSTMIF */
/**
 * @brief    Within this API, the upper layer module (called module)
 */
Std_ReturnType SomeIpTp_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, SomeIpTp, 0, Bsw_Start, 3, 0x41, TxPduId, PduInfoPtr);

    /* PRQA S 6030 -- */
    Std_ReturnType ret        = E_NOT_OK;
    PduIdType      cfgTxPduId = TxPduId;
#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
    if (TxPduId < SOMEIPTP_TX_PDU_MACRO_NUM)
    {
        cfgTxPduId = SomeIpTp_ConfigPtr->TxNSduAndTxNPduMacroMap[TxPduId];
    }
#endif
#if (STD_ON == SOMEIPTP_DEV_ERROR_DETECT)
#if (STD_ON == SOMEIPTP_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition     = (uint8)GetApplicationID();
    uint8 someIpTpPartition = SomeIpTp_GetSomeIpTpPartionByEcuCParttion(ecuCPartition);
    if (someIpTpPartition >= SOMEIPTP_USED_PARTITION_NUM)
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_TRIGGERTRANSMIT, SOMEIPTP_E_PARAM);
    }
    else if (!SomeIpTp_InitStatus[someIpTpPartition])
#else
    if (!SomeIpTp_InitStatus[0u])
#endif
    {
        /* @req SWS_SomeIpTp_00072 */
        SomeIpTp_DetReportError(SOMEIPTP_SID_TRIGGERTRANSMIT, SOMEIPTP_E_UNINIT);
    }
#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
    else if (cfgTxPduId >= SomeIpTp_ConfigPtr->TxPduMaxNmberInCurrentVariant)
#else
    else if (cfgTxPduId >= SOMEIPTP_TX_PDU_MACRO_NUM)
#endif
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_TRIGGERTRANSMIT, SOMEIPTP_E_PARAM);
    }
    else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_TRIGGERTRANSMIT, SOMEIPTP_E_PARAM_POINTER);
    }
    else
#endif
    {
        const SomeIpTp_TxNSduType* txNSduCfgPtr = SomeIpTp_GetTxNSduCfgPtr(cfgTxPduId);
#if (SOMEIPTP_USED_PARTITION_NUM > 1)
        uint8                 partitionIdxInCfg = txNSduCfgPtr->SomeIpTpPartition;
        SomeIpTp_TxPduRTType* txPduRtPtr = &SomeIpTp_TxPduRTData[someIpTpPartition][txNSduCfgPtr->OnPartitionIndex];
#else
        SomeIpTp_TxPduRTType* txPduRtPtr = &SomeIpTp_TxPduRTData[0u][cfgTxPduId];
#endif
#if ((STD_ON == SOMEIPTP_DEV_ERROR_DETECT) && (SOMEIPTP_USED_PARTITION_NUM > 1))
        if (someIpTpPartition != partitionIdxInCfg)
        {
            SomeIpTp_DetReportError(SOMEIPTP_SID_TRIGGERTRANSMIT, SOMEIPTP_E_PARAM);
        }
        else
#endif
        {
            if ((SOMEIPTP_PDU_TX_COPY_DATA == txPduRtPtr->PduState)
                && (PduInfoPtr->SduLength >= txPduRtPtr->SegmentSize) && (NULL_PTR != txNSduCfgPtr))
            {
                ret = SomeIpTp_TriggerTransmitHandle(PduInfoPtr, txNSduCfgPtr, txPduRtPtr);
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
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, SomeIpTp, 0, Bsw_Return, 2, 0x41, ret);

    return ret;
}
#define SOMEIPTP_STOP_SEC_CODE_FAST
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE_FAST
#include "SomeIpTp_MemMap.h"
/* PRQA S 6030 ++ */ /* VL_MTR_SomeIpTp_QacSTMIF */
/**
 * @brief    Indication of a received PDU from a lower layer communication interface module.
 */
void SomeIpTp_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, SomeIpTp, 0, Bsw_Start, 3, 0x42, RxPduId, PduInfoPtr);

    /* PRQA S 6030 -- */
    PduIdType cfgRxPduId = RxPduId;
#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
    if (RxPduId < SOMEIPTP_RX_PDU_MACRO_NUM)
    {
        cfgRxPduId = SomeIpTp_ConfigPtr->RxNSduAndRxNPduMacroMap[RxPduId];
    }
#endif
#if (STD_ON == SOMEIPTP_DEV_ERROR_DETECT)
#if (STD_ON == SOMEIPTP_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition     = (uint8)GetApplicationID();
    uint8 someIpTpPartition = SomeIpTp_GetSomeIpTpPartionByEcuCParttion(ecuCPartition);
    if (someIpTpPartition >= SOMEIPTP_USED_PARTITION_NUM)
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_TRIGGERTRANSMIT, SOMEIPTP_E_PARAM);
    }
    else if (!SomeIpTp_InitStatus[someIpTpPartition])
#else
    if (!SomeIpTp_InitStatus[0u])
#endif
    {
        /* @req SWS_SomeIpTp_00057 */
        SomeIpTp_DetReportError(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_UNINIT);
    }
#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
    else if (cfgRxPduId >= SomeIpTp_ConfigPtr->RxPduMaxNmberInCurrentVariant)
#else
    else if (cfgRxPduId >= SOMEIPTP_RX_PDU_MACRO_NUM)
#endif
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_PARAM);
    }
    else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_PARAM_POINTER);
    }
    else if (PduInfoPtr->SduLength < SOMEIPTP_SOMEIP_HEADER_LEN)
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_PARAM);
    }
    else
#endif
    {
        const SomeIpTp_RxNSduType* rxNSduCfgPtr = SomeIpTp_GetRxNSduCfgPtr(cfgRxPduId);
#if (SOMEIPTP_USED_PARTITION_NUM > 1)
        uint8                 partitionIdxInCfg = rxNSduCfgPtr->SomeIpTpPartition;
        SomeIpTp_RxPduRTType* rxPduRtPtr = &SomeIpTp_RxPduRTData[someIpTpPartition][rxNSduCfgPtr->OnPartitionIndex];
#else
        SomeIpTp_RxPduRTType* rxPduRtPtr = &SomeIpTp_RxPduRTData[0u][cfgRxPduId];
#endif
#if ((STD_ON == SOMEIPTP_DEV_ERROR_DETECT) && (SOMEIPTP_USED_PARTITION_NUM > 1))
        if (someIpTpPartition != partitionIdxInCfg)
        {
            SomeIpTp_DetReportError(SOMEIPTP_SID_TRIGGERTRANSMIT, SOMEIPTP_E_PARAM);
        }
        else
#endif
        {
            SomeIpTp_RxIndicationHandle(RxPduId, PduInfoPtr, rxNSduCfgPtr, rxPduRtPtr);
        }
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, SomeIpTp, 0, Bsw_Return, 1, 0x42);
}
#define SOMEIPTP_STOP_SEC_CODE_FAST
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE_FAST
#include "SomeIpTp_MemMap.h"
/* PRQA S 6030 ++ */ /* VL_MTR_SomeIpTp_QacSTMIF */
/**
 * @brief    This service is called by the Ethernet Interface to report a transceiver link state change.
 */
void SomeIpTp_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, SomeIpTp, 0, Bsw_Start, 3, 0x40, TxPduId, result);

    /* PRQA S 6030 -- */
    PduIdType cfgTxPduId = TxPduId;
#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
    if (TxPduId < SOMEIPTP_TX_PDU_MACRO_NUM)
    {
        cfgTxPduId = SomeIpTp_ConfigPtr->TxNSduAndTxNPduMacroMap[TxPduId];
    }
#endif
#if (STD_ON == SOMEIPTP_DEV_ERROR_DETECT)
#if (STD_ON == SOMEIPTP_ECUC_MULTIPLE_PARTITION_USED)
    uint8 ecuCPartition     = (uint8)GetApplicationID();
    uint8 someIpTpPartition = SomeIpTp_GetSomeIpTpPartionByEcuCParttion(ecuCPartition);
    if (someIpTpPartition >= SOMEIPTP_USED_PARTITION_NUM)
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_TRIGGERTRANSMIT, SOMEIPTP_E_PARAM);
    }
    else if (!SomeIpTp_InitStatus[someIpTpPartition])
#else
    if (!SomeIpTp_InitStatus[0u])
#endif
    {
        /* @req SWS_SomeIpTp_00067 */
        SomeIpTp_DetReportError(SOMEIPTP_SID_TXCONFIRMATION, SOMEIPTP_E_UNINIT);
    }
#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
    else if (cfgTxPduId >= SomeIpTp_ConfigPtr->TxPduMaxNmberInCurrentVariant)
#else
    else if (cfgTxPduId >= SOMEIPTP_TX_PDU_MACRO_NUM)
#endif
    {
        SomeIpTp_DetReportError(SOMEIPTP_SID_TXCONFIRMATION, SOMEIPTP_E_PARAM);
    }
    else
#endif
    {
        const SomeIpTp_TxNSduType* txNSduCfgPtr = SomeIpTp_GetTxNSduCfgPtr(cfgTxPduId);
#if (SOMEIPTP_USED_PARTITION_NUM > 1)
        uint8                 partitionIdxInCfg = txNSduCfgPtr->SomeIpTpPartition;
        SomeIpTp_TxPduRTType* txPduRtPtr = &SomeIpTp_TxPduRTData[someIpTpPartition][txNSduCfgPtr->OnPartitionIndex];
#else
        SomeIpTp_TxPduRTType* txPduRtPtr = &SomeIpTp_TxPduRTData[0u][cfgTxPduId];
#endif
#if ((STD_ON == SOMEIPTP_DEV_ERROR_DETECT) && (SOMEIPTP_USED_PARTITION_NUM > 1))
        if (someIpTpPartition != partitionIdxInCfg)
        {
            SomeIpTp_DetReportError(SOMEIPTP_SID_TRANSMIT, SOMEIPTP_E_PARAM);
        }
        else
#endif
        {
            if (SOMEIPTP_PDU_TX_WAIT_CONF == txPduRtPtr->PduState)
            {
                if (E_NOT_OK == result)
                {
                    /* @SWS_SomeIpTp_00023: If the API SomeIpTp_TxConfirmation() is called with parameter success set to
                     * FALSE. */
                    SomeIpTp_TxCancelHandle(txNSduCfgPtr, txPduRtPtr, SOMEIPTP_SID_TXCONFIRMATION);
                    PduR_SomeIpTpTxConfirmation(txNSduCfgPtr->SomeIpTpTxNSduUpperLayerId, E_NOT_OK);
                }
                else
                {
                    SomeIpTp_TxConfirmationHandle(cfgTxPduId, txNSduCfgPtr, txPduRtPtr);
                }
            }
        }
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, SomeIpTp, 0, Bsw_Return, 1, 0x40);
}
#define SOMEIPTP_STOP_SEC_CODE_FAST
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"

/**
 * @brief    This function performs the processing of the AUTOSAR SOME/IP TP module's transmission activities.
 */
void SomeIpTp_MainFunctionTx(uint8 someIpTpPartitionId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, SomeIpTp, 0, Bsw_Start, 2, 0x03, someIpTpPartitionId);

    /* @req SWS_SomeIpTp_00059 */
    if (SomeIpTp_InitStatus[someIpTpPartitionId])
    {
        const SomeIpTp_ChannelType* chCfgPtr;
        uint16                      ch;

#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
        for (ch = 0u; ch < SomeIpTp_ConfigPtr->ChannelMaxNmberInCurrentVariant; ch++)
#else
        for (ch = 0u; ch < SOMEIPTP_CHANNEL_MACRO_NUM; ch++)
#endif
        {
            chCfgPtr = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
            if (chCfgPtr->SomeIpTpTxNSduNum > 0u)
            {
                SomeIpTp_TxTimerHandle(ch, someIpTpPartitionId);
                SomeIpTp_TxPduHandle(ch, someIpTpPartitionId);
            }
        }
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, SomeIpTp, 0, Bsw_Return, 1, 0x03);
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    This function performs the processing of the AUTOSAR SOME/IP TP module's reception activities.
 */
void SomeIpTp_MainFunctionRx(uint8 someIpTpPartitionId)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, SomeIpTp, 0, Bsw_Start, 2, 0x04, someIpTpPartitionId);

    /* @req SWS_SomeIpTp_00070 */
    if (SomeIpTp_InitStatus[someIpTpPartitionId])
    {
        const SomeIpTp_ChannelType* chCfgPtr;
        uint16                      ch;

#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
        for (ch = 0u; ch < SomeIpTp_ConfigPtr->ChannelMaxNmberInCurrentVariant; ch++)
#else
        for (ch = 0u; ch < SOMEIPTP_CHANNEL_MACRO_NUM; ch++)
#endif
        {
            chCfgPtr = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
            if (chCfgPtr->SomeIpTpRxNSduNum > 0u)
            {
                SomeIpTp_RxTimerHandle(ch, someIpTpPartitionId);
            }
        }
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, SomeIpTp, 0, Bsw_Return, 1, 0x04);
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

/* --------------------------------------------Private Function Definitions ----------------------------------------- */
#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"

/**
 * @brief    Handle transmit relevant timer
 */
SOMEIPTP_LOCAL void SomeIpTp_TxTimerHandle(uint16 ch, uint8 someIpTpPartitionId)
{
    const SomeIpTp_ChannelType* chCfgPtr          = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
    const SomeIpTp_TxNSduType*  txNSduCfgPtr      = chCfgPtr->SomeIpTpTxNSdu;
    PduIdType                   firstPduOfChannel = chCfgPtr->SomeIpTpTxNSdu[0u].SomeIpTpTxNSduHandleId;
    SomeIpTp_TxPduRTType*       txPduRtPtr        = &SomeIpTp_TxPduRTData[someIpTpPartitionId][firstPduOfChannel];
    PduIdType                   pduIdx;

    for (pduIdx = 0u; pduIdx < chCfgPtr->SomeIpTpTxNSduNum; pduIdx++)
    {
#if (SOMEIPTP_USED_PARTITION_NUM > 1)
        if (txNSduCfgPtr->SomeIpTpPartition != someIpTpPartitionId)
        {
            continue;
        }
#endif
/* Handle tx timeout timer */
#ifndef SOMEIPTP_VER_R19_11
        if (txPduRtPtr->TxTimeoutTimer > SOMEIPTP_MAGIC_NUMBER_0_UNSIGNED_LONG)
        {
            txPduRtPtr->TxTimeoutTimer--;
            if (SOMEIPTP_MAGIC_NUMBER_0_UNSIGNED_LONG == txPduRtPtr->TxTimeoutTimer)
            {
                /* @req SWS_SomeIpTp_00023 */
                SomeIpTp_TxCancelHandle(txNSduCfgPtr, txPduRtPtr, SOMEIPTP_SID_MAINFUNCTIONTX);
            }
        }
#endif

        /* Handle separation timer */
        if (txPduRtPtr->SeparationTimer > SOMEIPTP_MAGIC_NUMBER_0_UNSIGNED_LONG)
        {
            txPduRtPtr->SeparationTimer--;
        }

        txPduRtPtr++;
#ifndef SOMEIPTP_VER_R19_11
        txNSduCfgPtr++;
#endif
    }
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Pdu transmit handler
 */
SOMEIPTP_LOCAL void SomeIpTp_TxPduHandle(uint16 ch, uint8 someIpTpPartitionId)
{
    const SomeIpTp_ChannelType* chCfgPtr          = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
    const SomeIpTp_TxNSduType*  txNSduCfgPtr      = chCfgPtr->SomeIpTpTxNSdu;
    PduIdType                   firstPduOfChannel = chCfgPtr->SomeIpTpTxNSdu[0u].SomeIpTpTxNSduHandleId;
    SomeIpTp_TxPduRTType*       txPduRtPtr        = &SomeIpTp_TxPduRTData[someIpTpPartitionId][firstPduOfChannel];
    PduIdType                   pduIdx;
    PduInfoType                 pduInfo;
    PduLengthType               availableDataLen;
    PduLengthType               remainDataLen;
    PduLengthType               segmentSize;
    BufReq_ReturnType           bufReqRet;
    Std_ReturnType              ret;
    boolean                     errCancelFlag = FALSE;

    for (pduIdx = 0u; pduIdx < chCfgPtr->SomeIpTpTxNSduNum; pduIdx++)
    {
#if (SOMEIPTP_USED_PARTITION_NUM > 1)
        if (txNSduCfgPtr->SomeIpTpPartition != someIpTpPartitionId)
        {
            continue;
        }
#endif
        /* @req SWS_SomeIpTp_00020 */
        if ((SOMEIPTP_PDU_TX_WAIT == txPduRtPtr->PduState)
            || ((SOMEIPTP_PDU_TX_CONFIRMED == txPduRtPtr->PduState)
                && (SOMEIPTP_MAGIC_NUMBER_0_UNSIGNED_LONG == txPduRtPtr->SeparationTimer)))
        {
            /* First segment,query the available data in upper layer module*/
            if (0UL == txPduRtPtr->TxDataCnt)
            {
                pduInfo.SduLength  = (PduLengthType)0;
                pduInfo.SduDataPtr = NULL_PTR;
                bufReqRet          = PduR_SomeIpTpCopyTxData(
                    txNSduCfgPtr->SomeIpTpTxNSduUpperLayerId,
                    &pduInfo,
                    NULL_PTR,
                    &availableDataLen);

                if (BUFREQ_OK == bufReqRet)
                {
                    txPduRtPtr->AvailableDataLen = availableDataLen;
                }
                else
                {
                    txPduRtPtr->AvailableDataLen = (PduLengthType)0;
                }
            }

            /* Calculate next segment size and judge if first/last segment */
            remainDataLen                = txPduRtPtr->TxPduLen - txPduRtPtr->TxDataCnt;
            txPduRtPtr->AvailableDataLen = SomeIpTp_Min(txPduRtPtr->AvailableDataLen, remainDataLen);
            segmentSize                  = SomeIpTp_CalcNextSegmentSize(txNSduCfgPtr, txPduRtPtr);
            if (((txPduRtPtr->AvailableDataLen < SOMEIPTP_OFFSETFIELD_LSB) && (FALSE == txPduRtPtr->LastSegmentFlag))
                || ((txPduRtPtr->AvailableDataLen < remainDataLen) && (TRUE == txPduRtPtr->LastSegmentFlag)))
            {
                /* @SWS_SomeIpTp_00024 */
                errCancelFlag = TRUE;
            }
            else
            {
                txPduRtPtr->SegmentSize = segmentSize;
                txPduRtPtr->PduState    = SOMEIPTP_PDU_TX_COPY_DATA;

                /* @req SWS_SomeIpTp_00017,@req SWS_SomeIpTp_00019 */
                pduInfo.SduLength  = segmentSize;
                pduInfo.SduDataPtr = NULL_PTR;
                ret                = PduR_SomeIpTpTransmit(txNSduCfgPtr->SomeIpTpTxNPduLowerLayerId, &pduInfo);
                if (E_NOT_OK == ret)
                {
                    /* @SWS_SomeIpTp_00082: PduR_SomeIpTpTransmit() returns something different than E_OK. */
                    errCancelFlag = TRUE;
                }
                else
                {
/* Start Timeout timer */
#ifndef SOMEIPTP_VER_R19_11
                    txPduRtPtr->TxTimeoutTimer = chCfgPtr->SomeIpTpTxTimeoutTime;
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
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Transmit cancel process
 */
SOMEIPTP_LOCAL void
    SomeIpTp_TxCancelHandle(const SomeIpTp_TxNSduType* txNSduCfgPtr, SomeIpTp_TxPduRTType* txPduRtPtr, uint8 apiId)
{
    SomeIpTp_ResetTxRtData(txPduRtPtr);

    PduR_SomeIpTpTxConfirmation(txNSduCfgPtr->SomeIpTpTxNSduUpperLayerId, E_NOT_OK);

    SomeIpTp_DetReportRuntimeError(apiId, SOMEIPTP_E_DISASSEMBLY_INTERRUPT);
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

/**
 * @brief    Set tx runtime data to default value
 */
SOMEIPTP_LOCAL_INLINE void SomeIpTp_ResetTxRtData(SomeIpTp_TxPduRTType* txPduRtPtr)
{
    txPduRtPtr->PduState        = SOMEIPTP_PDU_IDLE;
    txPduRtPtr->SeparationTimer = 0u;
#if (SOMEIPTP_HAS_CONFIGED_TX_BURST_SIZE == STD_ON)
    txPduRtPtr->TxBurstSegmentCount = 0u;
#endif
    txPduRtPtr->TxTimeoutTimer   = 0u;
    txPduRtPtr->AvailableDataLen = 0u;
    txPduRtPtr->TxDataCnt        = 0u;
    txPduRtPtr->TxPduLen         = 0u;
    txPduRtPtr->SegmentSize      = 0u;
    txPduRtPtr->FirstSegmentFlag = FALSE;
    txPduRtPtr->NoSegmentFlag    = FALSE;
    txPduRtPtr->LastSegmentFlag  = FALSE;
    (void)IStdLib_MemSet(&txPduRtPtr->Header, 0u, SOMEIPTP_SOMEIP_HEADER_LEN);
}

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Get channel config data by TxPduId
 */
SOMEIPTP_LOCAL const SomeIpTp_ChannelType* SomeIpTp_GetTxChannelCfgPtr(PduIdType txPduId)
{
    const SomeIpTp_ChannelType* chCfgPtr;
    const SomeIpTp_TxNSduType*  txNSduCfgPtr;
    uint16                      ch;
    uint16                      idx;
    boolean                     findFlag = FALSE;

#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
    for (ch = 0u; ch < SomeIpTp_ConfigPtr->ChannelMaxNmberInCurrentVariant; ch++)
#else
    for (ch = 0u; ch < SOMEIPTP_CHANNEL_MACRO_NUM; ch++)
#endif
    {
        chCfgPtr     = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
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
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Get TxNSdu config data(SomeIpTpTxNSdu) by TxPduId
 */
SOMEIPTP_LOCAL const SomeIpTp_TxNSduType* SomeIpTp_GetTxNSduCfgPtr(PduIdType txPduId)
{
    const SomeIpTp_ChannelType* chCfgPtr;
    const SomeIpTp_TxNSduType*  txNSduCfgPtr;
    uint16                      ch;
    uint16                      idx;
    boolean                     findFlag = FALSE;

#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
    for (ch = 0u; ch < SomeIpTp_ConfigPtr->ChannelMaxNmberInCurrentVariant; ch++)
#else
    for (ch = 0u; ch < SOMEIPTP_CHANNEL_MACRO_NUM; ch++)
#endif
    {
        chCfgPtr     = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
        txNSduCfgPtr = chCfgPtr->SomeIpTpTxNSdu;
        for (idx = 0u; idx < chCfgPtr->SomeIpTpTxNSduNum; idx++)
        {
            if (txNSduCfgPtr->SomeIpTpTxNSduHandleId == txPduId)
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
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Calculate next to be transmited segment's size
 */
SOMEIPTP_LOCAL PduLengthType
    SomeIpTp_CalcNextSegmentSize(const SomeIpTp_TxNSduType* txNSduCfgPtr, SomeIpTp_TxPduRTType* txPduRtPtr)
{
    PduLengthType segmentSize;
    PduLengthType availablePayloadSize;
    PduLengthType maxLenOfLowerPdu;
    PduLengthType remainDataLen;
    boolean       needSegmentFlag = TRUE;

    remainDataLen = txPduRtPtr->TxPduLen - txPduRtPtr->TxDataCnt;
    /* First segment */
    if (0u == txPduRtPtr->TxDataCnt)
    {
        txPduRtPtr->FirstSegmentFlag = TRUE;

        if ((txPduRtPtr->AvailableDataLen == remainDataLen)
            && (remainDataLen <= SomeIpTp_Min(
                    txNSduCfgPtr->SomeIpTpTxMaxPduLen,
                    (PduLengthType)(SOMEIPTP_MAX_PDU_LEN - SOMEIPTP_TP_HEADER_LEN))))
        {
            /* SDU can be send in one frame */
            /*@SWS_SomeIpTp_00009: If the provided SDU fits into one single PDU
               the provided SOME/IP header shall be used with no modification. */
            segmentSize     = remainDataLen;
            needSegmentFlag = FALSE;

            txPduRtPtr->NoSegmentFlag   = TRUE;
            txPduRtPtr->LastSegmentFlag = TRUE;
        }
        else
        {
            availablePayloadSize = txPduRtPtr->AvailableDataLen - SOMEIPTP_SOMEIP_HEADER_LEN;
            /* Calculate the max data with SOMEIPTP_OFFSETFIELD_LSB */
            availablePayloadSize =
                (PduLengthType)((availablePayloadSize / SOMEIPTP_OFFSETFIELD_LSB) * SOMEIPTP_OFFSETFIELD_LSB);
        }
    }
    else
    {
        if ((txPduRtPtr->AvailableDataLen == remainDataLen)
            && ((remainDataLen + SOMEIPTP_TOTAL_HEADER_LEN)
                <= SomeIpTp_Min(txNSduCfgPtr->SomeIpTpTxMaxPduLen, (PduLengthType)SOMEIPTP_MAX_PDU_LEN)))
        {
            /* Last segment,Left SDU can be send in one frame */
            segmentSize     = (PduLengthType)(remainDataLen + SOMEIPTP_TOTAL_HEADER_LEN);
            needSegmentFlag = FALSE;

            txPduRtPtr->LastSegmentFlag = TRUE;
        }
        else
        {
            /* Calculate the max data with SOMEIPTP_OFFSETFIELD_LSB */
            availablePayloadSize =
                (PduLengthType)((txPduRtPtr->AvailableDataLen / SOMEIPTP_OFFSETFIELD_LSB) * SOMEIPTP_OFFSETFIELD_LSB);
        }
    }

    if (TRUE == needSegmentFlag)
    {
        /* @req SWS_SomeIpTp_00017,@req SWS_SomeIpTp_00019 */
        /* Calculate the possible maximum size for next segment*/
        /* @SWS_SomeIpTp_00002: The amount of generated SOME/IP segments shall be as little as possible. */
        maxLenOfLowerPdu = (PduLengthType)(txNSduCfgPtr->SomeIpTpTxMaxPduLen - SOMEIPTP_TOTAL_HEADER_LEN);
        /* Calculate the max data with SOMEIPTP_OFFSETFIELD_LSB */
        maxLenOfLowerPdu = (PduLengthType)((maxLenOfLowerPdu / SOMEIPTP_OFFSETFIELD_LSB) * SOMEIPTP_OFFSETFIELD_LSB);
        maxLenOfLowerPdu = (PduLengthType)(maxLenOfLowerPdu + SOMEIPTP_TOTAL_HEADER_LEN);
        maxLenOfLowerPdu = SomeIpTp_Min(maxLenOfLowerPdu, (PduLengthType)SOMEIPTP_MAX_PDU_LEN);

        segmentSize = (PduLengthType)(availablePayloadSize + SOMEIPTP_TOTAL_HEADER_LEN);
        segmentSize = SomeIpTp_Min(segmentSize, maxLenOfLowerPdu);
    }

    return segmentSize;
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Calculate data size need to be copied from upper layer
 */
SOMEIPTP_LOCAL PduLengthType SomeIpTp_CalcSizeGetFromUpperLayer(const SomeIpTp_TxPduRTType* txPduRtPtr)
{
    PduLengthType dataSizeFromUpper;

    /* First segment */
    if (TRUE == txPduRtPtr->FirstSegmentFlag)
    {
        if (TRUE == txPduRtPtr->NoSegmentFlag)
        {
            /* SDU can be send in one frame */
            dataSizeFromUpper = txPduRtPtr->SegmentSize;
        }
        else
        {
            dataSizeFromUpper = (PduLengthType)(txPduRtPtr->SegmentSize - SOMEIPTP_TP_HEADER_LEN);
        }
    }
    else
    {
        dataSizeFromUpper = (PduLengthType)(txPduRtPtr->SegmentSize - SOMEIPTP_TOTAL_HEADER_LEN);
    }

    return dataSizeFromUpper;
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Calculate address when copy data from upper layer
 */
SOMEIPTP_LOCAL uint8*
    SomeIpTp_CalcAddrToCopyDataFromUpperLayer(const SomeIpTp_TxPduRTType* txPduRtPtr, uint8* sduDataPtr)
{
    uint8* addr;

    /* First segment */
    if (TRUE == txPduRtPtr->FirstSegmentFlag)
    {
        if (TRUE == txPduRtPtr->NoSegmentFlag)
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
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Build header(Set TP-Flag bit and add TP header)
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      txPduRtPtr: Pointer of tx pdu runtime data
 *                     headerAddr: Header address
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
SOMEIPTP_LOCAL void SomeIpTp_BuildHeader(const SomeIpTp_TxPduRTType* txPduRtPtr, uint8* headerAddr)
{
    uint32 tpHeader = 0u;
    uint32 offset;
    /* Copy SOME/IP Header to first 8 byte */
    (void)IStdLib_MemCpy(headerAddr, (const uint8*)&txPduRtPtr->Header, SOMEIPTP_SOMEIP_HEADER_LEN);

    /* Set TP-Flag bit in Message Type Field to 1 */
    headerAddr[SOMEIPTP_HEADER_MESSAGE_TYPE_INDEX] |= SOMEIPTP_TPFLAG_MASK;

    /* Set TP-Header */
    if (0u == txPduRtPtr->TxDataCnt)
    {
        /* @SWS_SomeIpTp_00011: The Offset Field of the first SOME/IP segment shall be set to 0. */
        offset = 0u;
    }
    else
    {
        offset = (uint32)((uint32)(((uint32)txPduRtPtr->TxDataCnt) - (uint32)SOMEIPTP_TP_HEADER_LEN)
                          / SOMEIPTP_OFFSETFIELD_LSB);
    }
    tpHeader = (offset << SOMEIPTP_MAGIC_NUMBER_4_MAGIC);
    if (TRUE != txPduRtPtr->LastSegmentFlag)
    {
        tpHeader |= 1u;
    }

#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    SomeIpTp_EndianSwap((uint8*)&tpHeader, SOMEIPTP_MAGIC_NUMBER_4_MAGIC);
#endif
    (void)IStdLib_MemCpy(&headerAddr[SOMEIPTP_SOMEIP_HEADER_LEN], (uint8*)&tpHeader, sizeof(uint32));
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Handle receive relevant timer
 */
SOMEIPTP_LOCAL void SomeIpTp_RxTimerHandle(uint16 ch, uint8 someIpTpPartitionId)
{
    const SomeIpTp_ChannelType* chCfgPtr          = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
    const SomeIpTp_RxNSduType*  rxNSduCfgPtr      = chCfgPtr->SomeIpTpRxNSdu;
    PduIdType                   firstPduOfChannel = chCfgPtr->SomeIpTpRxNSdu[0u].SomeIpTpRxNPduHandleId;
    SomeIpTp_RxPduRTType*       rxPduRtPtr        = &SomeIpTp_RxPduRTData[someIpTpPartitionId][firstPduOfChannel];
    PduIdType                   pduIdx;

    for (pduIdx = 0u; pduIdx < chCfgPtr->SomeIpTpRxNSduNum; pduIdx++)
    {
#if (SOMEIPTP_USED_PARTITION_NUM > 1)
        if (rxNSduCfgPtr->SomeIpTpPartition != someIpTpPartitionId)
        {
            continue;
        }
#endif
        /* Handle rx timeout timer */
        if (rxPduRtPtr->RxTimeoutTimer > SOMEIPTP_MAGIC_NUMBER_0_UNSIGNED_LONG)
        {
            rxPduRtPtr->RxTimeoutTimer--;
            if (SOMEIPTP_MAGIC_NUMBER_0_UNSIGNED_LONG == rxPduRtPtr->RxTimeoutTimer)
            {
                /* @SWS_SomeIpTp_00041 */
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
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Check validity of received message
 */
SOMEIPTP_LOCAL Std_ReturnType SomeIpTp_RxMessageCheck(
    const PduInfoType*         PduInfoPtr,
    const SomeIpTp_RxNSduType* rxNSduCfgPtr,
    SomeIpTp_RxPduRTType*      rxPduRtPtr,
    uint32*                    tpHeaderPtr)
{
    Std_ReturnType ret               = E_OK;
    boolean        tpSegmentFlag     = TRUE;
    uint8          someIpMessageType = PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_MESSAGE_TYPE_INDEX];
    uint32         localTpHeader;
    if (PduInfoPtr->SduLength > SOMEIPTP_MAX_PDU_LEN)
    {
        /* Ignore the received Pdu */
        ret = E_NOT_OK;
    }

    if ((someIpMessageType & SOMEIPTP_TPFLAG_MASK) == SOMEIPTP_TPFLAG_MASK)
    {
        if (PduInfoPtr->SduLength > SOMEIPTP_TOTAL_HEADER_LEN)
        {
            localTpHeader = SomeIpTp_ParseTpHeader(&PduInfoPtr->SduDataPtr[SOMEIPTP_SOMEIP_HEADER_LEN]);
            *tpHeaderPtr  = localTpHeader;
        }
        else
        {
            ret = E_NOT_OK;
        }
    }
    else
    {
        /* message is a single segment not a TP segment */
        tpSegmentFlag = FALSE;
    }

    /* violate : @SWS_SomeIpTp_00080: Header Inconsistency check before TP-Flag check. */
    if ((E_OK == ret) && (SOMEIPTP_PDU_RX_CONSECUTIVE_SEG == rxPduRtPtr->PduState))
    {
        if ((someIpMessageType & SOMEIPTP_TPFLAG_MASK) == 0u)
        {
            /* @SWS_SomeIpTp_00045: If the TP-Flag is not set to 1. */
            SomeIpTp_RxCancelHandle(rxPduRtPtr, rxNSduCfgPtr, SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_MESSAGE_TYPE);
            ret = E_NOT_OK;
        }

        if ((E_OK == ret)
            && (IStdLib_MemCmp(PduInfoPtr->SduDataPtr, (uint8*)&rxPduRtPtr->Header, SOMEIPTP_SOMEIP_HEADER_LEN) != 0u))
        {
            /* @SWS_SomeIpTp_00062: detects an inconsistency of the received SOME/IP TP headers. */
            SomeIpTp_RxCancelHandle(
                rxPduRtPtr,
                rxNSduCfgPtr,
                SOMEIPTP_SID_RXINDICATION,
                SOMEIPTP_E_INCONSISTENT_HEADER);
            if (!((tpSegmentFlag) && (0u == (localTpHeader >> SOMEIPTP_MAGIC_NUMBER_4_MAGIC))))
            {
                ret = E_NOT_OK;
            }
        }
    }

    if ((E_OK == ret) && (tpSegmentFlag))
    {
        ret = SomeIpTp_RxMessageCheckConsecutiveFragment(PduInfoPtr, rxNSduCfgPtr, rxPduRtPtr, localTpHeader);
    }

    return ret;
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Receive cancel process
 */
SOMEIPTP_LOCAL void SomeIpTp_RxCancelHandle(
    SomeIpTp_RxPduRTType*      rxPduRtPtr,
    const SomeIpTp_RxNSduType* rxNSduCfgPtr,
    uint8                      apiId,
    uint8                      errorId)
{
    /* @SWS_SomeIpTp_00054 :
        1.The Rx timeout time defined by SomeIpTpRxTimeoutTime shall be canceled.
        2.Call the API PduR_SomeIpTpRxIndication()for this PDU ID with E_NOT_OK.
    */
    SomeIpTp_ResetRxRtData(rxPduRtPtr);
    PduR_SomeIpTpRxIndication(rxNSduCfgPtr->SomeIpTpRxSduRef, E_NOT_OK);

    SomeIpTp_DetReportRuntimeError(apiId, errorId);
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

/**
 * @brief    Set receive runtime data to default value
 */
SOMEIPTP_LOCAL_INLINE void SomeIpTp_ResetRxRtData(SomeIpTp_RxPduRTType* rxPduRtPtr)
{
    rxPduRtPtr->PduState       = SOMEIPTP_PDU_IDLE;
    rxPduRtPtr->RxTimeoutTimer = 0u;
    rxPduRtPtr->RxPayloadCnt   = 0u;
    rxPduRtPtr->BufferSize     = 0u;
    (void)IStdLib_MemSet(&rxPduRtPtr->Header, 0u, SOMEIPTP_SOMEIP_HEADER_LEN);
}

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Get channel config data by RxPduId
 */
SOMEIPTP_LOCAL const SomeIpTp_ChannelType* SomeIpTp_GetRxChannelCfgPtr(PduIdType rxPduId)
{
    const SomeIpTp_ChannelType* chCfgPtr;
    const SomeIpTp_RxNSduType*  rxNSduCfgPtr;
    uint16                      ch;
    uint16                      idx;
    boolean                     findFlag = FALSE;
#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
    for (ch = 0u; ch < SomeIpTp_ConfigPtr->ChannelMaxNmberInCurrentVariant; ch++)
#else
    for (ch = 0u; ch < SOMEIPTP_CHANNEL_MACRO_NUM; ch++)
#endif
    {
        chCfgPtr     = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
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
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Get RxNSdu config data(SomeIpTpRxNSdu) by RxPduId
 */
SOMEIPTP_LOCAL const SomeIpTp_RxNSduType* SomeIpTp_GetRxNSduCfgPtr(PduIdType rxPduId)
{
    const SomeIpTp_ChannelType* chCfgPtr;
    const SomeIpTp_RxNSduType*  rxNSduCfgPtr;
    uint16                      ch;
    uint16                      idx;
    boolean                     findFlag = FALSE;

#if (SOMEIPTP_CONFIG_NUM_OF_VARIANTS_USED > 1)
    for (ch = 0u; ch < SomeIpTp_ConfigPtr->ChannelMaxNmberInCurrentVariant; ch++)
#else
    for (ch = 0u; ch < SOMEIPTP_CHANNEL_MACRO_NUM; ch++)
#endif
    {
        chCfgPtr     = &SomeIpTp_ConfigPtr->SomeIpTpChannel[ch];
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
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Procee when receive single segment
 */
SOMEIPTP_LOCAL void SomeIpTp_RxSingleSegmentHandle(
    const PduInfoType*         PduInfoPtr,
    const SomeIpTp_RxNSduType* rxNSduCfgPtr,
    SomeIpTp_RxPduRTType*      rxPduRtPtr)
{
    BufReq_ReturnType bufReqRet;
    PduInfoType       pduInfo;
    boolean           errorQuitFlag = FALSE;
    rxPduRtPtr->PduState            = SOMEIPTP_PDU_RX_SINGLE_SEG;
    bufReqRet = PduR_SomeIpTpStartOfReception(rxNSduCfgPtr->SomeIpTpRxSduRef, NULL_PTR, 0u, &rxPduRtPtr->BufferSize);
    if (BUFREQ_OK != bufReqRet)
    {
        /* @req SWS_SomeIpTp_00051 */
        SomeIpTp_ResetRxRtData(rxPduRtPtr);
        SomeIpTp_DetReportRuntimeError(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_ASSEMBLY_INTERRUPT);
    }
    else
    {
        /* @req SWS_SomeIpTp_00034 */
        if (rxPduRtPtr->BufferSize < PduInfoPtr->SduLength)
        {
            /* @req SWS_SomeIpTp_00049 */
            errorQuitFlag = TRUE;
        }
        else
        {
            /* @req SWS_SomeIpTp_00031 */
            pduInfo.SduLength  = PduInfoPtr->SduLength;
            pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
            /* @req SWS_SomeIpTp_00034 */
            bufReqRet = PduR_SomeIpTpCopyRxData(rxNSduCfgPtr->SomeIpTpRxSduRef, &pduInfo, &rxPduRtPtr->BufferSize);
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
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Procee when receive first segment
 */
SOMEIPTP_LOCAL void SomeIpTp_RxFirstSegmentHandle(
    const PduInfoType*         PduInfoPtr,
    const SomeIpTp_RxNSduType* rxNSduCfgPtr,
    SomeIpTp_RxPduRTType*      rxPduRtPtr,
    uint32                     tpHeader)
{
    BufReq_ReturnType bufReqRet;
    boolean           errorQuitFlag = FALSE;

    rxPduRtPtr->PduState = SOMEIPTP_PDU_RX_FIRST_SEG;

    /* @SWS_SomeIpTp_00032: store the values of the received SOMEIP header for each PDU ID separately. */
    (void)IStdLib_MemCpy((uint8*)&rxPduRtPtr->Header, PduInfoPtr->SduDataPtr, sizeof(SomeIpTp_HeaderType));
    bufReqRet = PduR_SomeIpTpStartOfReception(rxNSduCfgPtr->SomeIpTpRxSduRef, NULL_PTR, 0u, &rxPduRtPtr->BufferSize);
    if (BUFREQ_OK != bufReqRet)
    {
        /* @req SWS_SomeIpTp_00051 */
        SomeIpTp_ResetRxRtData(rxPduRtPtr);
        SomeIpTp_DetReportRuntimeError(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_ASSEMBLY_INTERRUPT);
    }
    else
    {
        /* @req SWS_SomeIpTp_00034 */
        if (rxPduRtPtr->BufferSize < (PduInfoPtr->SduLength - SOMEIPTP_TP_HEADER_LEN))
        {
            /* @req SWS_SomeIpTp_00049 */
            errorQuitFlag = TRUE;
        }
        else
        {
            /* @SWS_SomeIpTp_00035:set the TP-Flag contained in the Message Type back to 0. */
            PduInfoType pduInfo;
            uint8       SomeIpTp_RxHeaderBuffer[SOMEIPTP_SOMEIP_HEADER_LEN];
            (void)IStdLib_MemCpy(SomeIpTp_RxHeaderBuffer, PduInfoPtr->SduDataPtr, SOMEIPTP_SOMEIP_HEADER_LEN);
            SomeIpTp_RxHeaderBuffer[SOMEIPTP_HEADER_MESSAGE_TYPE_INDEX] &= (uint8)(~SOMEIPTP_TPFLAG_MASK);

            /* @SWS_SomeIpTp_00079: pass the SOME/IP header excluding the SOME/IP TP header. */
            pduInfo.SduLength  = (PduLengthType)(SOMEIPTP_SOMEIP_HEADER_LEN);
            pduInfo.SduDataPtr = (uint8*)&SomeIpTp_RxHeaderBuffer;
            bufReqRet = PduR_SomeIpTpCopyRxData(rxNSduCfgPtr->SomeIpTpRxSduRef, &pduInfo, &rxPduRtPtr->BufferSize);

            if (BUFREQ_OK == bufReqRet)
            {
                /* @SWS_SomeIpTp_00079: call the API PduR_SomeIpTpCopyRxData() again, to provide the payload. */
                pduInfo.SduLength  = (PduLengthType)(PduInfoPtr->SduLength - SOMEIPTP_TOTAL_HEADER_LEN);
                pduInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[SOMEIPTP_TOTAL_HEADER_LEN];
                bufReqRet = PduR_SomeIpTpCopyRxData(rxNSduCfgPtr->SomeIpTpRxSduRef, &pduInfo, &rxPduRtPtr->BufferSize);
            }
            else
            {
                /* @SWS_SomeIpTp_00050 */
                errorQuitFlag = TRUE;
            }
        }

        /* Error occur,Cancel receive process */
        if (errorQuitFlag || (BUFREQ_OK != bufReqRet))
        {
            SomeIpTp_RxCancelHandle(rxPduRtPtr, rxNSduCfgPtr, SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_ASSEMBLY_INTERRUPT);
        }
        else if (SOMEIPTP_MAGIC_NUMBER_0_UNSIGNED_LONG == (uint32)(tpHeader & SOMEIPTP_MORE_SEGMENT_FLAG_MASK))
        {
            /* @SWS_SomeIpTp_00071:  More Segment Flag set to 0. */
            PduR_SomeIpTpRxIndication(rxNSduCfgPtr->SomeIpTpRxSduRef, E_OK);

            /* Receive finish, clear rx runtime buffer */
            SomeIpTp_ResetRxRtData(rxPduRtPtr);
        }
        else
        {
            /* No error occur */
            /* @req SWS_SomeIpTp_00036 */
            rxPduRtPtr->RxPayloadCnt = (PduLengthType)(PduInfoPtr->SduLength - SOMEIPTP_TOTAL_HEADER_LEN);
            rxPduRtPtr->PduState     = SOMEIPTP_PDU_RX_CONSECUTIVE_SEG;
        }
    }
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Procee when receive consecutive segment
 */
SOMEIPTP_LOCAL void SomeIpTp_RxConsecutiveSegmentHandle(
    const PduInfoType*         PduInfoPtr,
    const SomeIpTp_RxNSduType* rxNSduCfgPtr,
    SomeIpTp_RxPduRTType*      rxPduRtPtr,
    uint32                     tpHeader)
{
    BufReq_ReturnType bufReqRet;
    PduInfoType       pduInfo;
    boolean           errorQuitFlag = FALSE;

    /* @SWS_SomeIpTp_00039: if the bufferSizePtr provided by the previous call of the API PduR_SomeIpTpCopyRxData
     is greater or equal to the received payload. */
    if (rxPduRtPtr->BufferSize >= (PduInfoPtr->SduLength - SOMEIPTP_TOTAL_HEADER_LEN))
    {
        pduInfo.SduLength  = PduInfoPtr->SduLength - (PduLengthType)SOMEIPTP_TOTAL_HEADER_LEN;
        pduInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[SOMEIPTP_TOTAL_HEADER_LEN];

        bufReqRet = PduR_SomeIpTpCopyRxData(rxNSduCfgPtr->SomeIpTpRxSduRef, &pduInfo, &rxPduRtPtr->BufferSize);
        if (BUFREQ_OK != bufReqRet)
        {
            /* @SWS_SomeIpTp_00050: PduR_SomeIpTpCopyRxData()returns something else than BUFREQ_OK. */
            errorQuitFlag = TRUE;
        }
    }
    else
    {
        /* @SWS_SomeIpTp_00049 */
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
        if (SOMEIPTP_MAGIC_NUMBER_0_UNSIGNED_LONG == (tpHeader & SOMEIPTP_MORE_SEGMENT_FLAG_MASK))
        {
            /* @SWS_SomeIpTp_00040: A SOME/IP segment is successfully received with the More Segment Flag set to 0. */
            PduR_SomeIpTpRxIndication(rxNSduCfgPtr->SomeIpTpRxSduRef, E_OK);
            /* Receive finish, clear rx runtime buffer */
            SomeIpTp_ResetRxRtData(rxPduRtPtr);
        }
        else
        {
            /* @SWS_SomeIpTp_00038: store the number of Payload bytes for every PDU ID separately */
            rxPduRtPtr->RxPayloadCnt += pduInfo.SduLength;
        }
    }
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

/**
 * @brief    Switch data byte order
 */
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
SOMEIPTP_LOCAL_INLINE void SomeIpTp_EndianSwap(uint8* dataPtr, uint8 size)
{
    uint8 i;
    uint8 temp;
    uint8 halfSize = size >> 1u;
    uint8 tSize    = size - 1u;
    uint8 tIdx;

    for (i = 0u; i < halfSize; i++)
    {
        tIdx          = tSize - i;
        temp          = dataPtr[tIdx];
        dataPtr[tIdx] = dataPtr[i];
        dataPtr[i]    = temp;
    }
}
#endif

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    Parse TP header
 */
SOMEIPTP_LOCAL uint32 SomeIpTp_ParseTpHeader(const void* dataPtr)
{
    uint32 tpHeader;
    (void)IStdLib_MemCpy((uint8*)&tpHeader, (const void*)dataPtr, sizeof(uint32));
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    SomeIpTp_EndianSwap((uint8*)&tpHeader, sizeof(uint32));
#endif
    return tpHeader;
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

/**
 * @brief    Get min value in input parameter a and
 */
SOMEIPTP_LOCAL_INLINE PduLengthType SomeIpTp_Min(PduLengthType a, PduLengthType b)
{
    return ((a < b) ? a : b);
}

/**
 * @brief    Report Det error to DET module
 */
#if (STD_ON == SOMEIPTP_DEV_ERROR_DETECT)
SOMEIPTP_LOCAL_INLINE void SomeIpTp_DetReportError(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(SOMEIPTP_MODULE_ID, SOMEIPTP_INSTANCE_ID, ApiId, ErrorId);
}
#endif

/**
 * @brief    Report runtime error to DET module
 */
SOMEIPTP_LOCAL_INLINE void SomeIpTp_DetReportRuntimeError(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportRuntimeError(SOMEIPTP_MODULE_ID, SOMEIPTP_INSTANCE_ID, ApiId, ErrorId);
}

#if (STD_ON == SOMEIPTP_ECUC_MULTIPLE_PARTITION_USED)
SOMEIPTP_LOCAL_INLINE uint8 SomeIpTp_GetSomeIpTpPartionByEcuCParttion(uint8 ecuCPartition)
{
    uint8 someIpTpPartition = SOMEIPTP_USED_PARTITION_NUM;
    if (ecuCPartition < SOMEIPTP_ECUC_USED_PARTITION_NUM)
    {
        someIpTpPartition = SomeIpTp_SomeIpTpPartitionIdMapOnEcucPartition[ecuCPartition];
    }
    return someIpTpPartition;
}
#endif

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    handle triggerTransmit.
 */
SOMEIPTP_LOCAL Std_ReturnType SomeIpTp_TriggerTransmitHandle(
    PduInfoType*               pduInfoPtr,
    const SomeIpTp_TxNSduType* txNSduCfgPtr,
    SomeIpTp_TxPduRTType*      txPduRtPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    PduInfoType    pduInfo;
    pduInfo.SduLength           = SomeIpTp_CalcSizeGetFromUpperLayer(txPduRtPtr);
    pduInfo.SduDataPtr          = SomeIpTp_CalcAddrToCopyDataFromUpperLayer(txPduRtPtr, pduInfoPtr->SduDataPtr);
    BufReq_ReturnType bufReqRet = PduR_SomeIpTpCopyTxData(
        txNSduCfgPtr->SomeIpTpTxNSduUpperLayerId,
        &pduInfo,
        NULL_PTR,
        &txPduRtPtr->AvailableDataLen);
    if (BUFREQ_OK != bufReqRet)
    {
        /* @SWS_SomeIpTp_00025: returns something else than BUFREQ_OK. */
        SomeIpTp_TxCancelHandle(txNSduCfgPtr, txPduRtPtr, SOMEIPTP_SID_TRIGGERTRANSMIT);
    }
    else
    {
        /* Save SOME/IP Header to internal tx buffer */
        if (TRUE == txPduRtPtr->FirstSegmentFlag)
        {
            /* @SWS_SomeIpTp_00008: shall store SomeIp header for every PDU ID. */
            (void)IStdLib_MemCpy((uint8*)&txPduRtPtr->Header, pduInfo.SduDataPtr, SOMEIPTP_SOMEIP_HEADER_LEN);
        }

        /*@SWS_SomeIpTp_00010: create SomeIpTp header. */
        pduInfoPtr->SduLength = txPduRtPtr->SegmentSize;

        /* Build SOME/IP-TP Header */
        if (FALSE == txPduRtPtr->NoSegmentFlag)
        {
            SomeIpTp_BuildHeader(txPduRtPtr, pduInfoPtr->SduDataPtr);
        }

        txPduRtPtr->PduState = SOMEIPTP_PDU_TX_WAIT_CONF;
        ret                  = E_OK;
    }
    return ret;
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    handle SomeIpTp_RxIndication.
 */
SOMEIPTP_LOCAL void SomeIpTp_RxIndicationHandle(
    PduIdType                  rxPduId,
    const PduInfoType*         pduInfoPtr,
    const SomeIpTp_RxNSduType* rxNSduCfgPtr,
    SomeIpTp_RxPduRTType*      rxPduRtPtr)
{
    uint8  someIpMessageType = pduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_MESSAGE_TYPE_INDEX];
    uint32 tpHeader;
    if (E_OK == SomeIpTp_RxMessageCheck(pduInfoPtr, rxNSduCfgPtr, rxPduRtPtr, &tpHeader))
    {
        if ((someIpMessageType & SOMEIPTP_TPFLAG_MASK) == 0u)
        {
            /* @SWS_SomeIpTp_00031: Single Segment */
            SomeIpTp_RxSingleSegmentHandle(pduInfoPtr, rxNSduCfgPtr, rxPduRtPtr);
        }
        else
        {
            /* @SWS_SomeIpTp_00033: Start the Rx timeout time. */
            /* @SWS_SomeIpTp_00037: restart the SomeIpTpRxTimeoutTime. */
            const SomeIpTp_ChannelType* chCfgPtr = SomeIpTp_GetRxChannelCfgPtr(rxPduId);
            rxPduRtPtr->RxTimeoutTimer           = chCfgPtr->SomeIpTpRxTimeoutTime;
            uint32 offset                        = (tpHeader >> SOMEIPTP_MAGIC_NUMBER_4_MAGIC);
            if (SOMEIPTP_MAGIC_NUMBER_0_UNSIGNED_LONG == offset)
            {
                SomeIpTp_RxFirstSegmentHandle(pduInfoPtr, rxNSduCfgPtr, rxPduRtPtr, tpHeader);
            }
            else
            {
                SomeIpTp_RxConsecutiveSegmentHandle(pduInfoPtr, rxNSduCfgPtr, rxPduRtPtr, tpHeader);
            }
        }
    }
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
/**
 * @brief    handle SomeIpTp_TxConfirmation.
 */
SOMEIPTP_LOCAL void SomeIpTp_TxConfirmationHandle(
    PduIdType                  cfgTxPduId,
    const SomeIpTp_TxNSduType* txNSduCfgPtr,
    SomeIpTp_TxPduRTType*      txPduRtPtr)
{
    if (TRUE == txPduRtPtr->LastSegmentFlag)
    {
        /* @SWS_SomeIpTp_00021: If the last SOME/IP segment of the original SOME/IP PDU has been transmitted
         * successfully. */
        PduR_SomeIpTpTxConfirmation(txNSduCfgPtr->SomeIpTpTxNSduUpperLayerId, E_OK);
        SomeIpTp_ResetTxRtData(txPduRtPtr);
    }
    else
    {
        /* Update runtime data */
        txPduRtPtr->PduState = SOMEIPTP_PDU_TX_CONFIRMED;
        txPduRtPtr->TxDataCnt += SomeIpTp_CalcSizeGetFromUpperLayer(txPduRtPtr);
        txPduRtPtr->FirstSegmentFlag = FALSE;
        txPduRtPtr->SegmentSize      = 0u;
#ifndef SOMEIPTP_VER_R19_11
        /* Stop Timeout timer */
        txPduRtPtr->TxTimeoutTimer = 0u;
#endif
        /* @SWS_SomeIpTp_00020: The SOME/IP TP module shall debounce subsequent calls of the
        API PduR_SomeIpTpTransmit() for the same PDU. */
        const SomeIpTp_ChannelType* chCfgPtr = SomeIpTp_GetTxChannelCfgPtr(cfgTxPduId);
#if (SOMEIPTP_HAS_CONFIGED_TX_BURST_SIZE == STD_ON)
        txPduRtPtr->TxBurstSegmentCount++;
        if (txPduRtPtr->TxBurstSegmentCount >= chCfgPtr->SomeIpTpTxBurstSize)
        {
            txPduRtPtr->TxBurstSegmentCount = 0u;
            txPduRtPtr->SeparationTimer     = chCfgPtr->SomeIpTpNPduSeparationTime;
        }
#else
        txPduRtPtr->SeparationTimer = chCfgPtr->SomeIpTpNPduSeparationTime;
#endif
    }
}
#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"

#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h"
SOMEIPTP_LOCAL Std_ReturnType SomeIpTp_RxMessageCheckConsecutiveFragment(
    const PduInfoType*         pduInfoPtr,
    const SomeIpTp_RxNSduType* rxNSduCfgPtr,
    SomeIpTp_RxPduRTType*      rxPduRtPtr,
    uint32                     localTpHeader)
{
    Std_ReturnType ret    = E_OK;
    uint32         offset = (localTpHeader >> SOMEIPTP_MAGIC_NUMBER_4_MAGIC);

    if ((SOMEIPTP_PDU_IDLE == rxPduRtPtr->PduState) && (offset > SOMEIPTP_MAGIC_NUMBER_0_UNSIGNED_LONG))
    {
        /* @SWS_SomeIpTp_00042: called with the Offset Value is > 0 but no session is currently running. */
        /* Ignore the received Pdu and report runtime error */
        SomeIpTp_DetReportRuntimeError(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_INCONSISTENT_SEQUENCE);
        ret = E_NOT_OK;
    }
    else if (
        (1UL == (localTpHeader & SOMEIPTP_MORE_SEGMENT_FLAG_MASK))
        && ((uint32)(((pduInfoPtr->SduLength - SOMEIPTP_TOTAL_HEADER_LEN) % SOMEIPTP_OFFSETFIELD_LSB))
            != SOMEIPTP_MAGIC_NUMBER_0_UNSIGNED_LONG))
    {
        /* @rSWS_SomeIpTp_00063: If the received payload bytes are not dividable by 16. */
        SomeIpTp_RxCancelHandle(rxPduRtPtr, rxNSduCfgPtr, SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_ASSEMBLY_INTERRUPT);
        ret = E_NOT_OK;
    }
    else if (rxPduRtPtr->RxPayloadCnt != (offset * SOMEIPTP_OFFSETFIELD_LSB))
    {
        /* @SWS_SomeIpTp_00064: Offset Value in units of 16 bytes does not match to the sum of the received Payload
         * bytes. */
        /* @req SWS_SomeIpTp_00064, @req SWS_SomeIpTp_00048 */
        SomeIpTp_RxCancelHandle(rxPduRtPtr, rxNSduCfgPtr, SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_INCONSISTENT_SEQUENCE);
        if (SOMEIPTP_MAGIC_NUMBER_0_UNSIGNED_LONG != offset)
        {
            ret = E_NOT_OK;
        }
        else
        {
            /* @req SWS_SomeIpTp_00048 */
            /* ret = E_OK,Restart a new assembly process */
        }
    }
    else
    {
        /* Do nothing */
    }
    return ret;
}

#define SOMEIPTP_STOP_SEC_CODE
#include "SomeIpTp_MemMap.h"
