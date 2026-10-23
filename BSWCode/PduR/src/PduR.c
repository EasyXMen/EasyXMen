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
 **  @file               : PduR.h
 **  @author             : zhengfei.li
 **  @date               : 2020/06/22
 **  @vendor             : isoft
 **  @description        : Implementation for PduR
 **
 ***********************************************************************************************************************/

/* PRQA S 3415 EOF */ /* VL_PduR_3415 */
/* PRQA S 2983 EOF */ /* VL_PduR_2983 */

/* =================================================== inclusions =================================================== */
#include "PduR_Internal.h"
#include "PduR_Arti.h"

#if (STD_OFF == PDUR_ZERO_COST_OPERATION)
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ internal data definition ============================================ */
/* PduR Module status */
#define PDUR_START_SEC_VAR_CLEARED_8
#include "PduR_MemMap.h"
PDUR_LOCAL PduR_StateType PduR_Status;
#define PDUR_STOP_SEC_VAR_CLEARED_8
#include "PduR_MemMap.h"
/* ============================================ external data definition ============================================ */
/*PduR PB Configuration Run Time point parameter*/
#define PDUR_START_SEC_VAR_CLEARED_PTR
#include "PduR_MemMap.h"
const PduR_PBConfigType* PduR_ConfigStd;
#define PDUR_STOP_SEC_VAR_CLEARED_PTR
#include "PduR_MemMap.h"
/* ========================================= internal function declarations ========================================= */
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static boolean PduR_ValidateInitStatus(uint8 apiId);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static boolean PduR_ValidatePointer(uint8 apiId, uint8 ErrorId, const void* pointer);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static boolean
    PduR_ValidateMaxValue(uint8 apiId, uint8 ErrorId, uint16 targetValue, uint16 referenceValue, boolean isContain);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static boolean PduR_ValidatePartitionContext(uint8 apiId, uint8 targetPartition);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static boolean PduR_ValidateMetaData(uint8 apiId, uint8 metaDataLen, const uint8* metaDataPtr);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static boolean PduR_ValidateIsTpRoute(uint8 apiId, boolean flag);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif
/* ========================================== external function definition ========================================== */
/**
 * Initializes the PDU Router
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
void PduR_Init(const PduR_PBConfigType* ConfigPtr) /* PRQA S 1503,1532 */ /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Start, 2, 0xF0, ConfigPtr);

#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PduR_ValidatePointer(PDUR_INIT_ID, PDUR_E_INIT_FAILED, ConfigPtr)
        && PduR_ValidateMaxValue(
            PDUR_INIT_ID,
            PDUR_E_INIT_FAILED,
            ConfigPtr->PduRRoutingPathNum,
            PDUR_ROUTINGPATH_MAX,
            FALSE)
        && PduR_ValidateMaxValue(
            PDUR_INIT_ID,
            PDUR_E_INIT_FAILED,
            ConfigPtr->RoutingPathGroupCnt,
            PDUR_ROUTING_PATH_GROUP_MAX,
            FALSE))
#endif
    {
        SchM_Enter_PduR_ExclusiveArea_Init();
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
        if (PduR_ConfigStd == NULL_PTR)
#endif
        {
            /* first initialized module PduR */
            PduR_ConfigStd = ConfigPtr;
#if (PDUR_ROUTING_PATH_GROUP_MAX > 0u)
            PduR_RoutingPathGroupInit();
#endif
#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
            PduR_BufferInit();
#endif
            PduR_RouteInit();
            PduR_Status = PDUR_ONLINE;
        }
        SchM_Exit_PduR_ExclusiveArea_Init();
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Return, 1, 0xF0);
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#if (STD_ON == PDUR_VERSION_INFO_API)
/**
 * Returns the version information of this module.
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
void PduR_GetVersionInfo(Std_VersionInfoType* versionInfo) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Start, 2, 0xF1, versionInfo);

#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PduR_ValidatePointer(PDUR_GETVERSIONINFO_ID, PDUR_E_PARAM_POINTER, (const Std_VersionInfoType*)versionInfo))
#endif
    {
        (versionInfo)->vendorID         = PDUR_VENDOR_ID;
        (versionInfo)->moduleID         = PDUR_MODULE_ID;
        (versionInfo)->sw_major_version = PDUR_SW_MAJOR_VERSION;
        (versionInfo)->sw_minor_version = PDUR_SW_MINOR_VERSION;
        (versionInfo)->sw_patch_version = PDUR_SW_PATCH_VERSION;
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Return, 1, 0xF1);
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif
/**
 * Returns the unique identifier of the post-build time configuration of the PDU Router
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
PduR_PBConfigIdType PduR_GetConfigurationId(void) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    PduR_PBConfigIdType configId = 0u;
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PduR_ValidateInitStatus(PDUR_GETCONFIGURATIONID_ID))
    {
        configId = PduR_ConfigStd->PduRConfigId;
    }
#endif
    return configId;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
/**
 * Enables a routing path group.
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
void PduR_EnableRouting(PduR_RoutingPathGroupIdType id) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Start, 2, 0xF3, id);

#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PduR_ValidateInitStatus(PDUR_ENABLEROUTING_ID))
#endif
    {
#if (PDUR_ROUTING_PATH_GROUP_MAX > 0u)
#if (PDUR_MULITIVARIANT_SUPPORT == STD_ON)
        PduR_RoutingPathGroupIdType internalId = PduR_GetGroupIdFromUserId(PDUR_ENABLEROUTING_ID, id);
#else
        PduR_RoutingPathGroupIdType internalId = id;
#endif
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
        if (PduR_ValidateMaxValue(
                PDUR_ENABLEROUTING_ID,
                PDUR_E_ROUTING_PATH_GROUP_ID_INVALID,
                internalId,
                PduR_GetRoutingPathGroupCntOfPBConfig,
                TRUE))
#endif
        {
            SchM_Enter_PduR_ExclusiveArea_Group();
            if (!PduR_RoutingPathGroupEnabled[internalId])
            {
                PduIdType destPduNumber = PDUR_ROUTEPATHGROUP_CFG(internalId).PduRDestPduRefNumber;
                for (PduIdType index = 0u; index < destPduNumber; index++)
                {
                    PduIdType destPduId = PDUR_ROUTEPATHGROUP_CFG(internalId).PduRDestPduIdRef[index];
                    if (PduR_RoutingPathEnabled[destPduId] == PDUR_UNUSED_UINT16)
                    {
                        PduR_RoutingPathEnabled[destPduId] = 1u;
                    }
                    else
                    {
                        PduR_RoutingPathEnabled[destPduId]++;
                    }
                }
                PduR_RoutingPathGroupEnabled[internalId] = TRUE;
            }
            SchM_Exit_PduR_ExclusiveArea_Group();
        }
#endif
    }
    PDUR_NOUSED(id);

    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Return, 1, 0xF3);
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
/**
 * Disables a routing path group.
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
void PduR_DisableRouting(PduR_RoutingPathGroupIdType id, boolean initialize)
/* PRQA S 1503 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Start, 3, 0xF4, id, initialize);

#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PduR_ValidateInitStatus(PDUR_DISABLEROUTING_ID))
#endif
    {
#if (PDUR_ROUTING_PATH_GROUP_MAX > 0u)
#if (PDUR_MULITIVARIANT_SUPPORT == STD_ON)
        PduR_RoutingPathGroupIdType internalId = PduR_GetGroupIdFromUserId(PDUR_DISABLEROUTING_ID, id);
#else
        PduR_RoutingPathGroupIdType internalId = id;
#endif
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
        if (PduR_ValidateMaxValue(
                PDUR_DISABLEROUTING_ID,
                PDUR_E_ROUTING_PATH_GROUP_ID_INVALID,
                internalId,
                PduR_GetRoutingPathGroupCntOfPBConfig,
                TRUE))
#endif
        {
            SchM_Enter_PduR_ExclusiveArea_Group();
            PduIdType destPduNumber = PDUR_ROUTEPATHGROUP_CFG(internalId).PduRDestPduRefNumber;
            if (PduR_RoutingPathGroupEnabled[internalId])
            {
                for (PduIdType index = 0u; index < destPduNumber; index++)
                {
                    PduIdType destPduId = PDUR_ROUTEPATHGROUP_CFG(internalId).PduRDestPduIdRef[index];
                    PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
                    if (PduR_RoutingPathEnabled[destPduId] > 0u)
                    {
#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
                        if (PduR_RoutingPathEnabled[destPduId] == 1u)
                        {
                            PduR_DisableRoutingQueueHandle(destPduId, initialize);
                        }
#endif
                        PduR_RoutingPathEnabled[destPduId]--;
                    }
                }
                PduR_RoutingPathGroupEnabled[internalId] = FALSE;
            }
            SchM_Exit_PduR_ExclusiveArea_Group();
        }
#endif
    }
    PDUR_NOUSED(id);
    PDUR_NOUSED(initialize);

    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Return, 1, 0xF4);
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#if (PDUR_TRANSMIT_SUPPORT == STD_ON)
/**
 * Requests transmission of a PDU.
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1503, 6070, 6030 ++ */ /* VL_QAC_NoUsedApi,VL_MTR_PduR_STCAL, VL_MTR_PduR_STMIF */
Std_ReturnType PduR_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 1503, 6070, 6030 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Start, 3, 0x49, TxPduId, PduInfoPtr);

    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PduR_ValidateInitStatus(PDUR_TRANSMIT_ID))
#endif
    {
#if (PDUR_MULITIVARIANT_SUPPORT == STD_ON)
        PduIdType internalTxPduId = PduR_GetPduIdFromUserId(PDUR_TRANSMIT_ID, TxPduId, TRUE);
#else
        PduIdType internalTxPduId = TxPduId;
#endif
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
        if (PduR_ValidateMaxValue(
                PDUR_TRANSMIT_ID,
                PDUR_E_PDU_ID_INVALID,
                internalTxPduId,
                PduR_GetSrcPduNumOfPBConfig,
                TRUE)
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            && PduR_ValidatePartitionContext(PDUR_TRANSMIT_ID, PduR_GetPartitionIndexOfSrcPdu(internalTxPduId))
#endif
            && PduR_ValidatePointer(PDUR_TRANSMIT_ID, PDUR_E_PARAM_POINTER, PduInfoPtr)
            && PduR_ValidateMetaData(
                PDUR_TRANSMIT_ID,
                PduR_GetMetaDataLengthOfSrcPdu(internalTxPduId),
                (const uint8*)PduInfoPtr->MetaDataPtr))
#endif
        {
            const PduInfoType* infoPtrPtr = PduInfoPtr;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            PduInfoType pduInfo;
            if (NULL_PTR != PduR_GetBufDataInterPartitionOfRoutingPath(internalTxPduId))
            {
                /* The route is Inter-Partition and no queue set */
                infoPtrPtr = PduR_PduDeepCopy(internalTxPduId, PduInfoPtr, &pduInfo);
            }
#endif
            /* Tp route 1:1, If route 1:n, Dependent configuration tool */
            uint8 destPduNum = PduR_GetDestPduNumOfSrcPdu(internalTxPduId);
            for (uint8 cnt = 0u; cnt < destPduNum; cnt++)
            {
                PduIdType destPduId = PduR_GetDestPduOfRoutingPath(internalTxPduId, cnt);
                /* If a routing path group is disabled the PduR shall directly return (SWS_PduR_00805) */
                if (PduR_IsRoutingPathEnable(destPduId))
                {
                    Std_ReturnType tmpResult = E_NOT_OK;

                    /* Pre-set source route status to PENDING before attempting this destination.
                     * the status write is atomic w.r.t. TxConfirmation handlers. If this
                     * destination does not accept the transmit and no other destination
                     * succeeded, the source status will be reset to INITIAL. */
                    PduR_SourceRouteStatus[internalTxPduId] = PDUR_RES_PENDING;

                    switch (PduR_GetRouteTypeOfDestPdu(destPduId))
                    {
                    case PDUR_ROUTE_IF_TX_NOBUFFERED:
                        tmpResult = PduR_PduRIfTransmit(internalTxPduId, destPduId, infoPtrPtr);
                        break;
                    case PDUR_ROUTE_TP_TX_NOBUFFERED_UNICAST:
                    case PDUR_ROUTE_TP_TX_NOBUFFERED_MULTICAST:
                        tmpResult = PduR_PduRTpTransmit(internalTxPduId, destPduId, infoPtrPtr);
                        break;
                    default:
                        /* do nothing */
                        break;
                    }

                    /* If this destination accepted the request, mark overall result E_OK. */
                    SchM_Enter_PduR_ExclusiveArea_Route();
                    if (tmpResult == E_OK)
                    {
                        result = E_OK;
#if STD_ON == PDUR_MULTICAST_TP_TRANSMIT_SUPPORT
                        if (destPduNum > 1u && PduR_IsTpRouteOfSrcPdu(internalTxPduId))
                        {
                            PduR_DestinationMultiCastTpTransmittedCnt[destPduId] = 0u;
                            if (cnt == 0u)
                            {
                                PduR_SourceMultiCastTpTransmittedCnt[internalTxPduId] = 0u;
                            }
                        }
#endif
                    }
                    SchM_Exit_PduR_ExclusiveArea_Route();
                }
            }
            /* If no destination accepted the transmit request, ensure source route is not left pending.
             * This preserves the 1:n semantics: success if at least one dest returned E_OK; otherwise E_NOT_OK. */
            if (result != E_OK)
            {
                PduR_SourceRouteStatus[internalTxPduId] = PDUR_RES_INITIAL;
            }
        }
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Return, 2, 0x49, result);

    return result;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_CANCEL_TRANSMIT == STD_ON)
/**
 * Requests cancellation of an ongoing transmission of a PDU in a lower layer communication module.
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1503,1532,6030 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_OneFunRef, VL_MTR_PduR_STMIF */
Std_ReturnType PduR_CancelTransmit(PduIdType TxPduId)
/* PRQA S 1503,1532,6030 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Start, 2, 0x4A, TxPduId);

    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PduR_ValidateInitStatus(PDUR_CANCELTRANSMIT_ID))
#endif
    {
#if (PDUR_MULITIVARIANT_SUPPORT == STD_ON)
        PduIdType internalTxPduId = PduR_GetPduIdFromUserId(PDUR_CANCELTRANSMIT_ID, TxPduId, TRUE);
#else
        PduIdType internalTxPduId = TxPduId;
#endif
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
        if (PduR_ValidateMaxValue(
                PDUR_CANCELTRANSMIT_ID,
                PDUR_E_PDU_ID_INVALID,
                internalTxPduId,
                PduR_GetSrcPduNumOfPBConfig,
                TRUE)
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            && PduR_ValidatePartitionContext(PDUR_CANCELTRANSMIT_ID, PduR_GetPartitionIndexOfSrcPdu(internalTxPduId))
#endif
        )
#endif
        {
            /* Only all dest pdu cancel transmit OK,then return OK. */
            uint8   destPduNum = PduR_GetDestPduNumOfSrcPdu(internalTxPduId);
            boolean allDestOk  = TRUE;
            for (uint8 cnt = 0u; cnt < destPduNum; cnt++)
            {
                PduIdType destPduId = PduR_GetDestPduOfRoutingPath(internalTxPduId, cnt);
                if (!PduR_IsRoutingPathEnable(destPduId))
                {
                    allDestOk = FALSE;
                    continue; /* PRQA S 0770 */ /* VL_PduR_0770 */
                }
                /* Cross-core call must be outside critical section */
                if (PduR_PduRCancelTransmit(internalTxPduId, destPduId) == E_OK)
                {
                    /* Protect DestinationRouteStatus write (multi-core safe).
                     * Writing PDUR_RES_INITIAL is idempotent with DisableRouting,
                     * so no need to re-check enable state here. */
                    SchM_Enter_PduR_ExclusiveArea_Route();
                    PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
                    SchM_Exit_PduR_ExclusiveArea_Route();
                }
                else
                {
                    allDestOk = FALSE;
                }
            }
            if (allDestOk)
            {
                /* Protect SourceRouteStatus write (multi-core safe) */
                SchM_Enter_PduR_ExclusiveArea_Route();
                PduR_SourceRouteStatus[internalTxPduId] = PDUR_RES_INITIAL;
                SchM_Exit_PduR_ExclusiveArea_Route();
                result = E_OK;
            }
        }
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Return, 2, 0x4A, result);

    return result;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_CANCEL_RECEIVE == STD_ON)
/**
 * Requests cancellation of an ongoing reception of a PDU in a lower layer transport protocol module.
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1503,1532 ++ */      /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef */
/* PRQA S 6030,6070,6010 ++ */ /* VL_MTR_PduR_STMIF,VL_MTR_PduR_STCAL,VL_MTR_PduR_STCYC */
Std_ReturnType PduR_CancelReceive(PduIdType RxPduId)
/* PRQA S 6030,6070,6010 -- */
/* PRQA S 1503,1532 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Start, 2, 0x4C, RxPduId);

    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PduR_ValidateInitStatus(PDUR_CANCELRECEIVE_ID))
#endif
    {
#if (PDUR_MULITIVARIANT_SUPPORT == STD_ON)
        PduIdType internalRxPduId = PduR_GetPduIdFromUserId(PDUR_CANCELRECEIVE_ID, RxPduId, FALSE);
#else
        PduIdType internalRxPduId = RxPduId;
#endif
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
        if (PduR_ValidateMaxValue(
                PDUR_CANCELRECEIVE_ID,
                PDUR_E_PDU_ID_INVALID,
                internalRxPduId,
                PduR_GetPduRRoutingPathNumOfPBConfig,
                TRUE)
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            && PduR_ValidatePartitionContext(PDUR_CANCELRECEIVE_ID, PduR_GetPartitionIndexOfDestPdu(internalRxPduId))
#endif
            && PduR_ValidateIsTpRoute(
                PDUR_CANCELRECEIVE_ID,
                PduR_IsTpRouteOfSrcPdu(PduR_GetSrcPduOfDestPdu(internalRxPduId, 0u))))
#endif
        {
            PduIdType srcPduId = PduR_GetSrcPduOfDestPdu(internalRxPduId, 0u);
            if (PduR_IsRoutingPathEnable(internalRxPduId))
            {
                uint8   destPduNum = PduR_GetDestPduNumOfSrcPdu(srcPduId);
                boolean callLower  = FALSE;
                if (destPduNum == 1u)
                {
                    /* Single destination: directly forward the cancel to the lower TP module. */
                    callLower = TRUE;
                    result    = PduR_PduRCancelReceive(srcPduId, internalRxPduId);
                }
                else
                {
                    SchM_Enter_PduR_ExclusiveArea_Route();
                    boolean allOtherCancelled = TRUE;
                    for (uint8 cnt = 0u; cnt < destPduNum; cnt++)
                    {
                        PduIdType destPduId = PduR_GetDestPduOfRoutingPath(srcPduId, cnt);
                        if (PduR_IsRoutingPathEnable(destPduId))
                        {
                            if (destPduId == internalRxPduId)
                            {
                                continue; /* PRQA S 0770 */ /* VL_PduR_0770 */
                            }
                            if (PduR_DestinationRouteStatus[destPduId] != PDUR_RES_INITIAL)
                            {
                                allOtherCancelled = FALSE;
                                break;
                            }
                        }
                    }
                    SchM_Exit_PduR_ExclusiveArea_Route();
                    if (allOtherCancelled)
                    {
                        /* Last upper destination cancelling -> call lower TP CancelReceive once. */
                        callLower = TRUE;
                        result    = PduR_PduRCancelReceive(srcPduId, internalRxPduId);
                    }
                    else
                    {
                        /* Other upper destinations are still receiving; accept this cancel locally. */
                        result = E_OK;
                    }
                }

                if (result == E_OK)
                {
                    SchM_Enter_PduR_ExclusiveArea_Route();
                    PduR_DestinationRouteStatus[internalRxPduId] = PDUR_RES_INITIAL;
                    if (callLower)
                    {
                        PduR_SourceRouteStatus[srcPduId] = PDUR_RES_INITIAL;
                    }
                    SchM_Exit_PduR_ExclusiveArea_Route();
                }
            }
        }
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Return, 2, 0x4C, result);

    return result;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_RX_INDICATION == STD_ON)
/**
 * Indication of a received PDU from a lower layer communication interface module.
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1503,6070 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_PduR_STCAL */
void PduR_IfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 1503,6070 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Start, 3, 0x42, RxPduId, PduInfoPtr);

#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PduR_ValidateInitStatus(PDUR_IFRXINDICATION_ID))
#endif
    {
#if (PDUR_MULITIVARIANT_SUPPORT == STD_ON)
        PduIdType internalRxPduId = PduR_GetPduIdFromUserId(PDUR_IFRXINDICATION_ID, RxPduId, TRUE);
#else
        PduIdType internalRxPduId = RxPduId;
#endif
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
        if (PduR_ValidateMaxValue(
                PDUR_IFRXINDICATION_ID,
                PDUR_E_PDU_ID_INVALID,
                internalRxPduId,
                PduR_GetSrcPduNumOfPBConfig,
                TRUE)
            && PduR_ValidateIsTpRoute(PDUR_IFRXINDICATION_ID, !(PDUR_ROUTINGPATH_CFG(internalRxPduId).TpRoute))
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            && PduR_ValidatePartitionContext(PDUR_IFRXINDICATION_ID, PduR_GetPartitionIndexOfSrcPdu(internalRxPduId))
#endif
            && PduR_ValidatePointer(PDUR_IFRXINDICATION_ID, PDUR_E_PARAM_POINTER, PduInfoPtr)
            && PduR_ValidatePointer(PDUR_IFRXINDICATION_ID, PDUR_E_PARAM_POINTER, (const uint8*)PduInfoPtr->SduDataPtr)
            && PduR_ValidateMetaData(
                PDUR_IFRXINDICATION_ID,
                PduR_GetMetaDataLengthOfSrcPdu(internalRxPduId),
                (const uint8*)PduInfoPtr->MetaDataPtr))
#endif
        {
            uint8              destPduNum = PduR_GetDestPduNumOfSrcPdu(internalRxPduId);
            const PduInfoType* infoPtrPtr = PduInfoPtr;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            PduInfoType pduInfo;
            if (NULL_PTR != PduR_GetBufDataInterPartitionOfRoutingPath(internalRxPduId))
            {
                /* The route is Inter-Partition and no queue set */
                infoPtrPtr = PduR_PduDeepCopy(internalRxPduId, PduInfoPtr, &pduInfo);
            }
#endif
            for (uint8 cnt = 0u; cnt < destPduNum; cnt++)
            {
                PduIdType destPduId = PduR_GetDestPduOfRoutingPath(internalRxPduId, cnt);
                if (PduR_IsRoutingPathEnable(destPduId))
                {
                    PduR_PduRIfRxIndication(internalRxPduId, destPduId, infoPtrPtr);
                }
            }
        }
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Return, 1, 0x42);
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_TX_CONFIRMATION == STD_ON)
/**
 * The lower layer communication interface module confirms the transmission of a PDU, or the failure to transmit a PDU.
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
void PduR_IfTxConfirmation(PduIdType TxPduId, Std_ReturnType result) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Start, 3, 0x40, TxPduId, result);

#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PduR_ValidateInitStatus(PDUR_IFTXCONFIRMATION_ID))
#endif
    {
#if (PDUR_MULITIVARIANT_SUPPORT == STD_ON)
        PduIdType internalTxPduId = PduR_GetPduIdFromUserId(PDUR_IFTXCONFIRMATION_ID, TxPduId, FALSE);
#else
        PduIdType internalTxPduId = TxPduId;
#endif
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
        if (PduR_ValidateMaxValue(
                PDUR_IFTXCONFIRMATION_ID,
                PDUR_E_PDU_ID_INVALID,
                internalTxPduId,
                PduR_GetPduRRoutingPathNumOfPBConfig,
                TRUE)
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            && PduR_ValidatePartitionContext(PDUR_IFTXCONFIRMATION_ID, PduR_GetPartitionIndexOfDestPdu(internalTxPduId))
#endif
        )
#endif
        {
            /* Only the first confirmation is accepted (SWS_PDUR_xxxxx AR 4.2.2) */
            PduIdType srcPduId = PduR_GetSrcPduOfDestPdu(internalTxPduId, 0u);
            PduR_PduRIfTxConfirmation(srcPduId, internalTxPduId, result);
        }
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Return, 1, 0x40);
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_TRIGGER_TRANSMIT == STD_ON)
/**
 * The function called by TriggerTransmit function of the respective communication interface module.
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1503,3673,6070 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_3673, VL_MTR_PduR_STCAL */
Std_ReturnType PduR_IfTriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
/* PRQA S 1503,3673,6070 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Start, 3, 0x41, TxPduId, PduInfoPtr);

    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PduR_ValidateInitStatus(PDUR_IFTRIGGERTRANSMIT_ID))
#endif
    {
#if (PDUR_MULITIVARIANT_SUPPORT == STD_ON)
        PduIdType internalTxPduId = PduR_GetPduIdFromUserId(PDUR_IFTRIGGERTRANSMIT_ID, TxPduId, FALSE);
#else
        PduIdType internalTxPduId = TxPduId;
#endif
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
        if (PduR_ValidateMaxValue(
                PDUR_IFTRIGGERTRANSMIT_ID,
                PDUR_E_PDU_ID_INVALID,
                internalTxPduId,
                PduR_GetPduRRoutingPathNumOfPBConfig,
                TRUE)
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            && PduR_ValidatePartitionContext(
                PDUR_IFTRIGGERTRANSMIT_ID,
                PduR_GetPartitionIndexOfDestPdu(internalTxPduId))
#endif
            && PduR_ValidatePointer(PDUR_IFTRIGGERTRANSMIT_ID, PDUR_E_PARAM_POINTER, (const PduInfoType*)PduInfoPtr)
            && PduR_ValidatePointer(
                PDUR_IFTRIGGERTRANSMIT_ID,
                PDUR_E_PARAM_POINTER,
                (const uint8*)PduInfoPtr->SduDataPtr))
#endif
        {
            PduIdType    srcPduId   = PduR_GetSrcPduOfDestPdu(internalTxPduId, 0u);
            PduInfoType* infoPtrPtr = PduInfoPtr;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            PduInfoType pduInfo;
            if (NULL_PTR != PduR_GetBufDataInterPartitionOfRoutingPath(srcPduId))
            {
                /* The route is Inter-Partition and no queue set,need copy data to Global buffer */
                infoPtrPtr = PduR_PduDeepCopy(srcPduId, PduInfoPtr, &pduInfo);
            }
#endif
            result = PduR_PduRTriggerTransmit(srcPduId, internalTxPduId, infoPtrPtr);
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            if (NULL_PTR != PduR_GetBufDataInterPartitionOfRoutingPath(srcPduId))
            {
                /* The route is Inter-Partition and no queue set,need copy data to SrcPartition buffer */
                /* copy data and metadata from Global buffer to Low-layer */
                PduInfoPtr->SduLength = infoPtrPtr->SduLength;
                (void)IStdLib_MemCpy(PduInfoPtr->SduDataPtr, infoPtrPtr->SduDataPtr, infoPtrPtr->SduLength);
                if (NULL_PTR != infoPtrPtr->MetaDataPtr)
                {
                    (void)IStdLib_MemCpy(
                        PduInfoPtr->MetaDataPtr,
                        infoPtrPtr->MetaDataPtr,
                        PduR_GetMetaDataLengthOfSrcPdu(srcPduId));
                }
            }
#endif
        }
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Return, 2, 0x41, result);

    return result;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_TP_COPYRXDATA == STD_ON)
/**
 * The function called by TpCopyRxData function of transport protocol module.
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1503, 5017, 6070, 6030 ++ */ /* VL_QAC_NoUsedApi, VL_PduR_5017, VL_MTR_PduR_STCAL, VL_MTR_PduR_STMIF */
/* PRQA S 6010 ++ */                   /* VL_MTR_PduR_STCYC */
BufReq_ReturnType PduR_TpCopyRxData(PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr)
/* PRQA S 6010 -- */
/* PRQA S 1503, 5017, 6070, 6030 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Start, 4, 0x44, id, info, bufferSizePtr);

    BufReq_ReturnType bufQeqReturn = BUFREQ_E_NOT_OK;
#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON         \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PduR_ValidateInitStatus(PDUR_TPCOPYRXDATA_ID))
#endif
    {
#if (PDUR_MULITIVARIANT_SUPPORT == STD_ON)
        PduIdType internalId = PduR_GetPduIdFromUserId(PDUR_TPCOPYRXDATA_ID, id, TRUE);
#else
        PduIdType internalId = id;
#endif
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
        if (PduR_ValidateMaxValue(
                PDUR_TPCOPYRXDATA_ID,
                PDUR_E_PDU_ID_INVALID,
                internalId,
                PduR_GetSrcPduNumOfPBConfig,
                TRUE)
            && PduR_ValidateIsTpRoute(PDUR_TPCOPYRXDATA_ID, PDUR_ROUTINGPATH_CFG(internalId).TpRoute)
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            && PduR_ValidatePartitionContext(PDUR_TPCOPYRXDATA_ID, PduR_GetPartitionIndexOfSrcPdu(internalId))
#endif
            && PduR_ValidatePointer(PDUR_TPCOPYRXDATA_ID, PDUR_E_PARAM_POINTER, info)
            && PduR_ValidatePointer(PDUR_TPCOPYRXDATA_ID, PDUR_E_PARAM_POINTER, (const PduLengthType*)bufferSizePtr))
#endif
        {
            uint8              destPduNum = PduR_GetDestPduNumOfSrcPdu(internalId);
            const PduInfoType* infoPtrPtr = info;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            PduInfoType pduInfo;
            if (NULL_PTR != PduR_GetBufDataInterPartitionOfRoutingPath(internalId))
            {
                /* The route is Inter-Partition and no queue set */
                infoPtrPtr = PduR_PduDeepCopy(internalId, info, &pduInfo);
            }
#endif
            PduLengthType bufferSizeMin = PDUR_PDU_LENGTH_INVALID;
            for (uint8 cnt = 0u; cnt < destPduNum; cnt++) /* PRQA S 0771 */ /* VL_PduR_Logical */
            {
                PduIdType destPduId = PduR_GetDestPduOfRoutingPath(internalId, cnt);
                if (PduR_IsRoutingPathEnable(destPduId))
                {
                    PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);
#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
                    /* Only support 1:1 routing for transport protocol I-PDU forwarding */
                    if (routeType == PDUR_ROUTE_TP_RX_NOBUFFERED)
                    {
                        bufQeqReturn = PduR_CopyRxDataForward(internalId, destPduId, infoPtrPtr, bufferSizePtr);
                        break;
                    }
#endif

#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
                    if ((routeType == PDUR_ROUTE_TP_RX_BUFFERED) || (routeType == PDUR_ROUTE_TP_GW_BUFFERED))
                    {
                        if (PduR_SourceRouteStatus[internalId] != PDUR_RES_PENDING)
                        {
                            break;
                        }

                        PduLengthType bufferSize;
                        if (PduR_PduRCopyRxDataBuffered(internalId, destPduId, infoPtrPtr, &bufferSize) == BUFREQ_OK)
                        {
                            bufQeqReturn  = BUFREQ_OK;
                            bufferSizeMin = (bufferSizeMin > bufferSize) ? bufferSize : bufferSizeMin;
                        }
                    }
#endif
                }
                /* PRQA S 2992,2996,2880 ++ */ /* VL_PduR_2992,VL_PduR_2996,VL_PduR_2880 */
                if ((bufQeqReturn == BUFREQ_OK) && ((cnt + 1u) == destPduNum))
                {
                    *bufferSizePtr = bufferSizeMin;
                }
                /*  PRQA S 2992,2996,2880 -- */
            }
        }
    }
#else
    PDUR_NOUSED(id);
    PDUR_NOUSED(info);
    PDUR_NOUSED(bufferSizePtr);
#endif

    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Return, 2, 0x44, bufQeqReturn);

    return bufQeqReturn;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_TP_RXINDICATION == STD_ON)
/**
 * The function called by TpRxIndication function of transport protocol module.
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1503, 6030 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_PduR_STMIF */
void PduR_TpRxIndication(PduIdType id, Std_ReturnType result)
/* PRQA S 1503, 6030 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Start, 3, 0x45, id, result);

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON         \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PduR_ValidateInitStatus(PDUR_TPRXINDICATION_ID))
#endif
    {
#if (PDUR_MULITIVARIANT_SUPPORT == STD_ON)
        PduIdType internalId = PduR_GetPduIdFromUserId(PDUR_TPRXINDICATION_ID, id, TRUE);
#else
        PduIdType internalId = id;
#endif
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
        if (PduR_ValidateMaxValue(
                PDUR_TPRXINDICATION_ID,
                PDUR_E_PDU_ID_INVALID,
                internalId,
                PduR_GetSrcPduNumOfPBConfig,
                TRUE)
            && PduR_ValidateIsTpRoute(PDUR_TPRXINDICATION_ID, PDUR_ROUTINGPATH_CFG(internalId).TpRoute)
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            && PduR_ValidatePartitionContext(PDUR_TPRXINDICATION_ID, PduR_GetPartitionIndexOfSrcPdu(internalId))
#endif
        )
#endif
        {
            uint8 destPduNum = PduR_GetDestPduNumOfSrcPdu(internalId);
            for (uint8 cnt = 0u; cnt < destPduNum; cnt++) /* PRQA S 0771 */ /* VL_PduR_Logical */
            {
                PduIdType      destPduId = PduR_GetDestPduOfRoutingPath(internalId, cnt);
                PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
                if (routeType == PDUR_ROUTE_TP_RX_NOBUFFERED)
                {
                    PduR_TpRxIndicationForward(internalId, destPduId, result);
                    break;
                }
#endif
#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
                if ((routeType == PDUR_ROUTE_TP_RX_BUFFERED) || (routeType == PDUR_ROUTE_TP_GW_BUFFERED))
                {
                    if (PduR_SourceRouteStatus[internalId] != PDUR_RES_PENDING)
                    {
                        break;
                    }

                    PduR_PduRTpRxIndicationBuffered(internalId, destPduId, result);

                    if ((cnt + 1u) == destPduNum)
                    {
                        PduR_SourceRouteStatus[internalId] = PDUR_RES_INITIAL;
                    }
                }
#endif
            }
        }
    }
#else
    PDUR_NOUSED(id);
    PDUR_NOUSED(result);
#endif

    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Return, 1, 0x45);
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_TP_STARTOFRECEPTION_TRANSMIT == STD_ON)
/**
 * The function called by TpStartOfReception function of transport protocol module.
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1503, 5017, 6070, 6030 ++ */ /* VL_QAC_NoUsedApi, VL_PduR_5017, VL_MTR_PduR_STCAL, VL_MTR_PduR_STMIF */
/* PRQA S 6010 ++ */                   /* VL_MTR_PduR_STCYC */
BufReq_ReturnType PduR_TpStartOfReception(
    PduIdType          id,
    const PduInfoType* info,
    PduLengthType      TpSduLength,
    PduLengthType*     bufferSizePtr)
/* PRQA S 6010 -- */
/* PRQA S 1503, 5017, 6070, 6030 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Start, 5, 0x46, id, info, TpSduLength, bufferSizePtr);

    BufReq_ReturnType bufQeqReturn = BUFREQ_E_NOT_OK;
#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON         \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PduR_ValidateInitStatus(PDUR_TPSTARTOFRECEPTION_ID))
#endif
    {
#if (PDUR_MULITIVARIANT_SUPPORT == STD_ON)
        PduIdType internalId = PduR_GetPduIdFromUserId(PDUR_TPSTARTOFRECEPTION_ID, id, TRUE);
#else
        PduIdType internalId = id;
#endif
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
        if (PduR_ValidateMaxValue(
                PDUR_TPSTARTOFRECEPTION_ID,
                PDUR_E_PDU_ID_INVALID,
                internalId,
                PduR_GetSrcPduNumOfPBConfig,
                TRUE)
            && PduR_ValidateIsTpRoute(PDUR_TPSTARTOFRECEPTION_ID, PDUR_ROUTINGPATH_CFG(internalId).TpRoute)
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            && PduR_ValidatePartitionContext(PDUR_TPSTARTOFRECEPTION_ID, PduR_GetPartitionIndexOfSrcPdu(internalId))
#endif
            && PduR_ValidatePointer(
                PDUR_TPSTARTOFRECEPTION_ID,
                PDUR_E_PARAM_POINTER,
                (const PduLengthType*)bufferSizePtr)
            && ((info == NULL_PTR)
                || PduR_ValidateMetaData(
                    PDUR_TPSTARTOFRECEPTION_ID,
                    PduR_GetMetaDataLengthOfSrcPdu(internalId),
                    (const uint8*)info->MetaDataPtr)))
#endif
        {
            uint8              destPduNum = PduR_GetDestPduNumOfSrcPdu(internalId);
            const PduInfoType* infoPtrPtr = info;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            PduInfoType pduInfo;
            if (NULL_PTR != PduR_GetBufDataInterPartitionOfRoutingPath(internalId))
            {
                /* The route is Inter-Partition and no queue set */
                infoPtrPtr = PduR_PduDeepCopy(internalId, info, &pduInfo);
            }
#endif
            PduLengthType bufferSizeMin = PDUR_PDU_LENGTH_INVALID;
            for (uint8 cnt = 0u; cnt < destPduNum; cnt++) /* PRQA S 0771 */ /* VL_PduR_Logical */
            {
                PduIdType destPduId = PduR_GetDestPduOfRoutingPath(internalId, cnt);
                if (PduR_IsRoutingPathEnable(destPduId))
                {
                    PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);
#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
                    /* Only support 1:1 routing for transport protocol I-PDU forwarding */
                    if (routeType == PDUR_ROUTE_TP_RX_NOBUFFERED)
                    {
                        bufQeqReturn =
                            PduR_StartOfReceptionForward(internalId, destPduId, infoPtrPtr, TpSduLength, bufferSizePtr);
                        break;
                    }
#endif

#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
                    if ((routeType == PDUR_ROUTE_TP_RX_BUFFERED) || (routeType == PDUR_ROUTE_TP_GW_BUFFERED))
                    {
                        if (PduR_SourceRouteStatus[internalId] != PDUR_RES_INITIAL)
                        {
                            break;
                        }

                        PduLengthType bufferSize;
                        if (PduR_PduRStartOfReceptionBuffered(
                                internalId,
                                destPduId,
                                infoPtrPtr,
                                TpSduLength,
                                &bufferSize)
                            == BUFREQ_OK)
                        {
                            bufQeqReturn  = BUFREQ_OK;
                            bufferSizeMin = (bufferSizeMin > bufferSize) ? bufferSize : bufferSizeMin;
                        }
                    }
#endif
                }
                /* PRQA S 2992,2996 ++ */ /* VL_PduR_2992,VL_PduR_2996 */
                if ((cnt + 1u) == destPduNum)
                {
                    if (bufQeqReturn == BUFREQ_OK)
                    {
                        *bufferSizePtr                     = bufferSizeMin;
                        PduR_SourceRouteStatus[internalId] = PDUR_RES_PENDING;
                    }
                    else
                    {
                        bufQeqReturn = BUFREQ_E_OVFL;
                    }
                }
                /* PRQA S 2992,2996 -- */
            }
        }
    }
#else
    PDUR_NOUSED(id);
    PDUR_NOUSED(info);
    PDUR_NOUSED(TpSduLength);
    PDUR_NOUSED(bufferSizePtr);
#endif

    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Return, 2, 0x46, bufQeqReturn);

    return bufQeqReturn;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_TP_COPYTXDATA == STD_ON)
/**
 * The function called by TpCopyTxData function of transport protocol module.
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1503, 6070 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_PduR_STCAL */
BufReq_ReturnType PduR_TpCopyTxData(
    PduIdType            id,
    const PduInfoType*   info,
    const RetryInfoType* retry,
    PduLengthType*       availableDataPtr)
/* PRQA S 1503, 6070 -- */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Start, 5, 0x43, id, info, retry, availableDataPtr);

    BufReq_ReturnType bufQeqReturn = BUFREQ_E_NOT_OK;
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PduR_ValidateInitStatus(PDUR_TPCOPYTXDATA_ID))
#endif
    {
#if (PDUR_MULITIVARIANT_SUPPORT == STD_ON)
        PduIdType internalId = PduR_GetPduIdFromUserId(PDUR_TPCOPYTXDATA_ID, id, FALSE);
#else
        PduIdType internalId = id;
#endif
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
        if (PduR_ValidateMaxValue(
                PDUR_TPCOPYTXDATA_ID,
                PDUR_E_PDU_ID_INVALID,
                internalId,
                PduR_GetPduRRoutingPathNumOfPBConfig,
                TRUE)
            && PduR_ValidateIsTpRoute(PDUR_TPCOPYTXDATA_ID, PduR_IsTpRouteOfDestPdu(internalId))
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            && PduR_ValidatePartitionContext(PDUR_TPCOPYTXDATA_ID, PduR_GetPartitionIndexOfDestPdu(internalId))
#endif
        )
#endif
        {
            PduIdType          srcPdu     = PduR_GetSrcPduOfDestPdu(internalId, 0u);
            const PduInfoType* infoPtrPtr = info;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            PduInfoType pduInfo;
            if (NULL_PTR != PduR_GetBufDataInterPartitionOfRoutingPath(srcPdu))
            {
                /* The route is Inter-Partition and no queue set */
                infoPtrPtr = PduR_PduDeepCopy(srcPdu, info, &pduInfo);
            }
#endif
            if (PduR_IsRoutingPathEnable(internalId))
            {
                bufQeqReturn = PduR_PduRCopyTxData(srcPdu, internalId, infoPtrPtr, retry, availableDataPtr);
            }
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            if ((info != NULL_PTR) && (NULL_PTR != PduR_GetBufDataInterPartitionOfRoutingPath(srcPdu)))
            {
                /* The route is Inter-Partition and no queue set,need copy data to SrcPartition buffer */
                /* copy data and metadata from Global buffer to Low-layer */
                (void)IStdLib_MemCpy(info->SduDataPtr, infoPtrPtr->SduDataPtr, infoPtrPtr->SduLength);
                if (NULL_PTR != infoPtrPtr->MetaDataPtr)
                {
                    (void)IStdLib_MemCpy(
                        info->MetaDataPtr,
                        infoPtrPtr->MetaDataPtr,
                        PduR_GetMetaDataLengthOfSrcPdu(srcPdu));
                }
            }
#endif
        }
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Return, 2, 0x43, bufQeqReturn);

    return bufQeqReturn;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_TP_TXCONFIRMATION == STD_ON)
/**
 * The function called by TpTxConfirmation function of transport protocol module.
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
void PduR_TpTxConfirmation(PduIdType id, Std_ReturnType result) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Start, 3, 0x48, id, result);

#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PduR_ValidateInitStatus(PDUR_TPTXCONFIRMATION_ID))
#endif
    {
#if (PDUR_MULITIVARIANT_SUPPORT == STD_ON)
        PduIdType internalId = PduR_GetPduIdFromUserId(PDUR_TPTXCONFIRMATION_ID, id, FALSE);
#else
        PduIdType internalId = id;
#endif
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
        if (PduR_ValidateMaxValue(
                PDUR_TPTXCONFIRMATION_ID,
                PDUR_E_PDU_ID_INVALID,
                internalId,
                PduR_GetPduRRoutingPathNumOfPBConfig,
                TRUE)
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
            && PduR_ValidatePartitionContext(PDUR_TPTXCONFIRMATION_ID, PduR_GetPartitionIndexOfDestPdu(internalId))
#endif
        )
#endif
        {
            PduIdType srcPdu = PduR_GetSrcPduOfDestPdu(internalId, 0u);
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
            if (PduR_ValidateIsTpRoute(PDUR_TPTXCONFIRMATION_ID, PDUR_ROUTINGPATH_CFG(srcPdu).TpRoute))
#endif
            {
                PduR_PduRTpTxConfirmation(srcPdu, internalId, result);
            }
        }
    }

    ARTI_TRACE_N(USER, AR_CP_BSW_API, PduR, 0, Bsw_Return, 1, 0x48);
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif
/* ========================================== internal function definition ========================================== */
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 2889, 6060 ++ */ /* VL_QAC_MultiReturn, VL_MTR_PduR_STM19 */
static boolean PduR_ValidateInitStatus(uint8 apiId)
{
    if (PDUR_ONLINE != PduR_Status)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, apiId, PDUR_E_UNINIT);
        return FALSE;
    }
    return TRUE;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static boolean PduR_ValidatePointer(uint8 apiId, uint8 ErrorId, const void* pointer)
{
    if (pointer == NULL_PTR)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, apiId, ErrorId);
        return FALSE;
    }
    return TRUE;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static boolean
    PduR_ValidateMaxValue(uint8 apiId, uint8 ErrorId, uint16 targetValue, uint16 referenceValue, boolean isContain)
{
    if ((targetValue > referenceValue) || (isContain && (targetValue == referenceValue)))
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, apiId, ErrorId);
        return FALSE;
    }
    return TRUE;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static boolean PduR_ValidatePartitionContext(uint8 apiId, uint8 targetPartition)
{
    ApplicationType curAppId = GetApplicationID();
    if (curAppId != targetPartition)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, apiId, PDUR_E_PDU_ID_INVALID);
        return FALSE;
    }
    return TRUE;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static boolean PduR_ValidateMetaData(uint8 apiId, uint8 metaDataLen, const uint8* metaDataPtr)
{
    if ((metaDataLen > 0u) && (metaDataPtr == NULL_PTR))
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, apiId, PDUR_E_PARAM_POINTER);
        return FALSE;
    }
    return TRUE;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static boolean PduR_ValidateIsTpRoute(uint8 apiId, boolean flag)
{
    if (!flag)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, apiId, PDUR_E_PDU_ID_INVALID);
        return FALSE;
    }
    return TRUE;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 2889, 6060 -- */
#endif
#endif /* end of PDUR_ZERO_COST_OPERATION */
