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
**  FILENAME    : PduR.c                                                      **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for PDUR                                     **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR Classic Platform 4.2.2 and R19_11              **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      REVISION   HISTORY                                    **
*******************************************************************************/
/* Refer to PduR.h */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR_Internal.h"
#if (STD_OFF == PDUR_ZERO_COST_OPERATION)

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if ((PDUR_CFG_H_SW_MAJOR_VERSION != 2u) || (PDUR_CFG_H_SW_MINOR_VERSION != 3u) || (PDUR_CFG_H_SW_PATCH_VERSION != 1u))
#error "PduR: Mismatch in Specification Software Version"
#endif

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define PDUR_START_SEC_VAR_INIT_8
#include "PduR_MemMap.h"
PDUR_LOCAL VAR(PduR_StateType, PDUR_VAR_POWER_ON_INIT) PduR_Status = PDUR_UNINIT;
#define PDUR_STOP_SEC_VAR_INIT_8
#include "PduR_MemMap.h"
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
/*PduR PB Configuration Run Time point parameter*/
#define PDUR_START_SEC_VAR_INIT_PTR
#include "PduR_MemMap.h"
P2CONST(PduR_PBConfigType, PDUR_CONST, PDUR_CONST_PBCFG) PduR_ConfigStd = NULL_PTR;
#define PDUR_STOP_SEC_VAR_INIT_PTR
#include "PduR_MemMap.h"
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/******************************************************************************/
/*
 * Brief               Initializes the PDU Router
 * ServiceId           0xf0
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: Pointer to Post build configuration data.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void PduR_Init(const PduR_PBConfigType* ConfigPtr)
{
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if ((NULL_PTR == ConfigPtr) || (ConfigPtr->PduRRoutingPathNum > PDUR_ROUTINGPATH_MAX)
        || (ConfigPtr->routingPathGroupCnt > PDUR_ROUTING_PATH_GROUP_MAX))
    {
        PduR_Det_ReportError(PDUR_INIT_ID, PDUR_E_INIT_FAILED);
    }
    else
#endif
    {
        PduR_ConfigStd = ConfigPtr;
#if (PDUR_ROUTING_PATH_GROUP_ENABLED == STD_ON)
        PduR_RoutingPathGroupInit();
#endif /* PDUR_ROUTING_PATH_GROUP_ENABLED == STD_ON */
#if (PDUR_NUMBER_OF_QUEUES > 0u)
        PduR_BufferInit();
#endif /* PDUR_NUMBER_OF_QUEUES > 0u */
        PduR_RouteInit();
        PduR_Status = PDUR_ONLINE;
    }
}
#if (STD_ON == PDUR_VERSION_INFO_API)
/******************************************************************************/
/*
 * Brief               This service returns the version information of this module.
 * ServiceId           0xf1
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     versionInfo, Pointer to where to store the version information
 *                     of this module
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void PduR_GetVersionInfo(Std_VersionInfoType* versionInfo)
{
#if (STD_ON == PDUR_DEV_ERROR_DETECT)

    if (NULL_PTR == versionInfo)
    {
        PduR_Det_ReportError(PDUR_INIT_ID, PDUR_E_INIT_FAILED);
    }
    else
#endif /* STD_ON == PDUR_DEV_ERROR_DETECT */
    {
        (versionInfo)->vendorID = PDUR_VENDOR_ID;
        (versionInfo)->moduleID = PDUR_MODULE_ID;
        (versionInfo)->sw_major_version = PDUR_SW_MAJOR_VERSION;
        (versionInfo)->sw_minor_version = PDUR_SW_MINOR_VERSION;
        (versionInfo)->sw_patch_version = PDUR_SW_PATCH_VERSION;
    }
}
#endif /* STD_ON == PDUR_VERSION_INFO_API */
/******************************************************************************/
/*
 * Brief               Returns the unique identifier of the post-build time
 *                     configuration of the PDU Router
 * ServiceId           0xf2
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              PduR_PBConfigIdType: Identifier of the post-build time configuration
 */
/******************************************************************************/
PduR_PBConfigIdType PduR_GetConfigurationId(void)
{
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PDUR_ONLINE != PduR_Status)
    {
        PduR_Det_ReportError(PDUR_GETCONFIGURATIONID_ID, PDUR_E_UNINIT);
    }
#endif
    return PduR_ConfigStd->PduRConfigId;
}
/******************************************************************************/
/*
 * Brief               Enables a routing path table
 * ServiceId           0xf3
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the routing path group.
 *                     Routing path groups are defined in the PDU router configuration
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void PduR_EnableRouting(PduR_RoutingPathGroupIdType id)
{
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PDUR_ONLINE != PduR_Status)
    {
        PduR_Det_ReportError(PDUR_ENABLEROUTING_ID, PDUR_E_UNINIT);
    }
    else if (id >= PduR_GetRoutingPathGroupCntOfPBConfig())
    {
        PduR_Det_ReportError(PDUR_ENABLEROUTING_ID, PDUR_E_ROUTING_PATH_GROUP_ID_INVALID);
    }
    else
#endif
    {
#if (PDUR_ROUTING_PATH_GROUP_ENABLED == STD_ON)
        SchM_Enter_PduR_ExclusiveArea_Group();
        PduIdType destPduNumber = PDUR_ROUTEPATHGROUP_CFG(id).PduRDestPduRefNumber;
        for (PduIdType index = 0u; index < destPduNumber; index++)
        {
            PduIdType destPduId = PDUR_ROUTEPATHGROUP_CFG(id).PduRDestPduIdRef[index];
            if (!PduR_RoutingPathEnabled[destPduId])
            {
                PduR_RoutingPathEnabled[destPduId] = TRUE;
            }
        }
        SchM_Exit_PduR_ExclusiveArea_Group();
#endif /* PDUR_ROUTING_PATH_GROUP_ENABLED == STD_ON */
    }
    PDUR_NOUSED(id);
}

/******************************************************************************/
/*
 * Brief               Disables a routing path table
 * ServiceId           0xf4
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the routing path group.
 *                     Routing path groups are defined in the PDU router configuration
 *                     initialize (true,false)
 *                     true: initialize single buffers to the default value
 *                     false: retain current value of single buffers
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void PduR_DisableRouting(PduR_RoutingPathGroupIdType id, boolean initialize)
{
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PDUR_ONLINE != PduR_Status)
    {
        PduR_Det_ReportError(PDUR_DISABLEROUTING_ID, PDUR_E_UNINIT);
    }
    else if (id >= PduR_GetRoutingPathGroupCntOfPBConfig())
    {
        PduR_Det_ReportError(PDUR_DISABLEROUTING_ID, PDUR_E_ROUTING_PATH_GROUP_ID_INVALID);
    }
    else
#endif
    {
#if (PDUR_ROUTING_PATH_GROUP_ENABLED == STD_ON)
        SchM_Enter_PduR_ExclusiveArea_Group();
        PduIdType destPduNumber = PDUR_ROUTEPATHGROUP_CFG(id).PduRDestPduRefNumber;
        for (PduIdType index = 0u; index < destPduNumber; index++)
        {
            PduIdType destPduId = PDUR_ROUTEPATHGROUP_CFG(id).PduRDestPduIdRef[index];
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
            if (PduR_RoutingPathEnabled[destPduId])
            {
#if (PDUR_NUMBER_OF_QUEUES > 0u)
                /* FIFO: flush and report PDUR_E_PDU_INSTANCES_LOST to DET (if enable) (SWS_PduR_00663) */
                PduR_QueueType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
                if (queuePtr != NULL_PTR)
                {
                    PduR_BufferType* bufferPtr = PduR_QueueGetHead(queuePtr); /* PRQA S 3678 */ /* MISRA Rule 8.13 */
                    if (bufferPtr != NULL_PTR)
                    {
                        if (queuePtr->depth == 1u)
                        {
#if (PDUR_DEFAULT_VALUE_ENABLED == STD_ON)
                            /* Single buffer: set to the default value with initialize set to true (SWS_PduR_00810) */
                            if (initialize)
                            {
                                PduRDefaultValueType* defaultValuePtr = PduR_GetDefaultValuePtrOfDestPdu(destPduId);
                                PduR_MetaDataLengthType metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
                                if (defaultValuePtr != NULL_PTR)
                                {
                                    bufferPtr->pduCurLength = metaDataLength + defaultValuePtr->DefaultValueLength;
                                    (void)ILib_memcpy(
                                        &bufferPtr->data[metaDataLength],
                                        &PduR_Default_value[defaultValuePtr->DefaultValueStart],
                                        defaultValuePtr->DefaultValueLength);
                                }
                            }
                            else
#endif /* PDUR_DEFAULT_VALUE_ENABLED == STD_ON */
                            {
                                /* Single buffer: do nothing with initialize set to false (SWS_PduR_00810) */
                            }
                        }
                        else
                        {
                            /* Multiple buffer: buffer flush,report DET(SWS_PduR_00663) */
                            PduR_QueueFlush(queuePtr);
                            PduR_Det_ReportRuntimeError(PDUR_DISABLEROUTING_ID, PDUR_E_PDU_INSTANCES_LOST);
                        }
                    }
                }
#endif /* PDUR_NUMBER_OF_QUEUES > 0u */
                PduR_RoutingPathEnabled[destPduId] = FALSE;
            }
        }
        SchM_Exit_PduR_ExclusiveArea_Group();
#endif /* PDUR_ROUTING_PATH_GROUP_ENABLED == STD_ON */
    }
    PDUR_NOUSED(id);
    PDUR_NOUSED(initialize);
}

#if (PDUR_TRANSMIT_SUPPORT == STD_ON)
/******************************************************************************/
/*
 * Brief               Requests transmission of an I-PDU.
 * ServiceId           0x49
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      TxPduId: Identification of the I-PDU.
 *                     PduInfoPtr: Length and pointer to the buffer of the I-PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType (E_OK,E_NOT_OK)
 *                     E_OK - request is accepted by the destination module; transmission is continued.
 *                     E_NOT_OK - request is not accepted by the destination module;transmission is aborted.
 */
/******************************************************************************/
Std_ReturnType PduR_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PDUR_ONLINE != PduR_Status)
    {
        PduR_Det_ReportError(PDUR_TRANSMIT_ID, PDUR_E_UNINIT);
    }
    else if (TxPduId >= PduR_ConfigStd->PduRSrcPduNum)
    {
        PduR_Det_ReportError(PDUR_TRANSMIT_ID, PDUR_E_PDU_ID_INVALID);
    }
    else if (
        (NULL_PTR == PduInfoPtr)
        || ((0u < PduR_GetMetaDataLengthOfSrcPdu(TxPduId)) && (NULL_PTR == PduInfoPtr->MetaDataPtr)))
    {
        PduR_Det_ReportError(PDUR_TRANSMIT_ID, PDUR_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == PDUR_DEV_ERROR_DETECT */
    {
        /* Tp route 1:1, If route 1:n, Dependent configuration tool */
        for (uint8 cnt = 0u; cnt < PduR_GetDestPduNumOfSrcPdu(TxPduId); cnt++)
        {
            PduIdType destPduId = PduR_GetDestPduOfRoutingPath(TxPduId, cnt);
            /* If a routing path group is disabled the PduR shall directly return (SWS_PduR_00805) */
            if (PduR_IsRoutingPathEnable(destPduId))
            {
                Std_ReturnType tmpResult = E_NOT_OK;
                switch (PduR_GetRouteTypeOfDestPdu(destPduId))
                {
                case PDUR_ROUTE_IF_TX_NOBUFFERED:
                    tmpResult = PduR_PduRIfTransmit(TxPduId, destPduId, PduInfoPtr);
                    break;
                case PDUR_ROUTE_TP_TX_NOBUFFERED:
                    tmpResult = PduR_PduRTpTransmit(TxPduId, destPduId, PduInfoPtr);
                    break;
                default:
                    /* do nothing */
                    break;
                }

                /* The PDU Router reports E_OK if at least one destination lower layer reports E_OK.
                 * (SWS_PduR_xxxxx 4.1.1) */
                if (tmpResult == E_OK)
                {
                    result = E_OK;
                    PduR_SourceRouteStatus[TxPduId] = PDUR_RES_PENDING;
                }
            }
        }
    }
    return result;
}
#endif /* PDUR_TRANSMIT_SUPPORT == STD_ON */

#if (PDUR_CANCEL_TRANSMIT == STD_ON)
/******************************************************************************/
/*
 * Brief               Requests cancellation of an ongoing transmission of an I-PDU in a lower
 *                     layer communication interface or transport protocol module.
 * ServiceId           0x4a
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id: Identification of the I-PDU to be cancelled.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType (E_OK,E_NOT_OK)
 *                     E_OK - Cancellation was executed successfully by the destination module.
 *                     E_NOT_OK - Cancellation was rejected by the destination module.
 */
/******************************************************************************/
Std_ReturnType PduR_CancelTransmit(PduIdType TxPduId)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PDUR_ONLINE != PduR_Status)
    {
        PduR_Det_ReportError(PDUR_CANCELTRANSMIT_ID, PDUR_E_UNINIT);
    }
    else if (TxPduId >= PduR_ConfigStd->PduRSrcPduNum)
    {
        PduR_Det_ReportError(PDUR_CANCELTRANSMIT_ID, PDUR_E_PDU_ID_INVALID);
    }
    else
#endif
    {
        /* Only all dest pdu cancel transmit OK,then return OK. */
        uint8 destPduNum = PduR_GetDestPduNumOfSrcPdu(TxPduId);
        for (uint8 cnt = 0u; cnt < destPduNum; cnt++)
        {
            PduIdType destPduId = PduR_GetDestPduOfRoutingPath(TxPduId, cnt);
            if (PduR_IsRoutingPathEnable(destPduId))
            {
                result = PduR_PduRCanceTransmit(TxPduId, destPduId);
            }

            if (result != E_OK)
            {
                break;
            }
        }
    }
    return result;
}
#endif /* PDUR_CANCEL_TRANSMIT == STD_ON */

#if (PDUR_CANCEL_RECEIVE == STD_ON)
/******************************************************************************/
/*
 * Brief               Requests cancellation of an ongoing reception of an I-PDU in a lower layer transport protocol
 * module. ServiceId           0x4c Sync/Async          Synchronous Reentrancy          Non Reentrant Param-Name[in]
 * RxPduId: Identification of the I-PDU to be cancelled. Param-Name[out]     None Param-Name[in/out]  None Return
 * Std_ReturnType (E_OK,E_NOT_OK) E_OK: Cancellation was executed successfully by the destination module. E_NOT_OK:
 * Cancellation was rejected by the destination module.
 */
/******************************************************************************/
Std_ReturnType PduR_CancelReceive(PduIdType RxPduId)
{
    Std_ReturnType result = E_NOT_OK;
    PduIdType srcPduId = PduR_GetSrcPduOfDestPdu(RxPduId, 0u);
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PDUR_ONLINE != PduR_Status)
    {
        PduR_Det_ReportError(PDUR_CANCELRECEIVE_ID, PDUR_E_UNINIT);
    }
    else if (RxPduId >= PduR_ConfigStd->PduRRoutingPathNum)
    {
        PduR_Det_ReportError(PDUR_CANCELRECEIVE_ID, PDUR_E_PDU_ID_INVALID);
    }
    else if (FALSE == PduR_IsTpRouteOfSrcPdu(srcPduId))
    {
        PduR_Det_ReportError(PDUR_CANCELRECEIVE_ID, PDUR_E_PDU_ID_INVALID);
    }
    else
#endif
    {
        if (PduR_IsRoutingPathEnable(RxPduId))
        {
            result = PduR_PduRCancelReceive(srcPduId, RxPduId);
        }
    }
    return result;
}
#endif /* PDUR_CANCEL_RECEIVE == STD_ON */

#if (PDUR_RX_INDICATION == STD_ON)
/******************************************************************************/
/*
 * Brief               Indication of a received I-PDU from a lower layer communication interface module.
 * ServiceId           0x42
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      RxPduId: ID of the received I-PDU.
 *                     PduInfoPtr: Contains the length (SduLength) of the received I-PDU and a pointer to a buffer
 * (SduDataPtr) containing the I-PDU. Param-Name[out]     None Param-Name[in/out]  None Return              None
 */
/******************************************************************************/
void PduR_IfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PDUR_ONLINE != PduR_Status)
    {
        PduR_Det_ReportError(PDUR_IFRXINDICATION_ID, PDUR_E_UNINIT);
    }
    else if (
        (NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr)
        || ((0u < PduR_GetMetaDataLengthOfSrcPdu(RxPduId)) && (NULL_PTR == PduInfoPtr->MetaDataPtr)))
    {
        PduR_Det_ReportError(PDUR_IFRXINDICATION_ID, PDUR_E_PARAM_POINTER);
    }
    else if ((RxPduId >= PduR_ConfigStd->PduRSrcPduNum) || (TRUE == PDUR_ROUTINGPATH_CFG(RxPduId).TpRoute))
    {
        PduR_Det_ReportError(PDUR_IFRXINDICATION_ID, PDUR_E_PDU_ID_INVALID);
    }
    else
#endif
    {
        uint8 destPduNum = PduR_GetDestPduNumOfSrcPdu(RxPduId);
        for (uint8 cnt = 0u; cnt < destPduNum; cnt++)
        {
            PduIdType destPduId = PduR_GetDestPduOfRoutingPath(RxPduId, cnt);
            if (PduR_IsRoutingPathEnable(destPduId))
            {
                PduR_PduRIfRxIndication(RxPduId, destPduId, PduInfoPtr);
            }
        }
    }
}
#endif /* PDUR_RX_INDICATION == STD_ON */

#if (PDUR_TX_CONFIRMATION == STD_ON)
/******************************************************************************/
/*
 * Brief               The lower layer communication interface module confirms the transmission of an IPDU.
 * ServiceId           0x40
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      TxPduId: ID of the I-PDU that has been transmitted.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void PduR_IfTxConfirmation(PduIdType TxPduId)
{
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PDUR_ONLINE != PduR_Status)
    {
        PduR_Det_ReportError(PDUR_IFTXCONFIRMATION_ID, PDUR_E_UNINIT);
    }
    else if (TxPduId >= PduR_ConfigStd->PduRRoutingPathNum)
    {
        PduR_Det_ReportError(PDUR_IFTXCONFIRMATION_ID, PDUR_E_PDU_ID_INVALID);
    }
    else
#endif
    {
        /* Only the first confirmation is accepted (SWS_PDUR_xxxxx AR 4.2.2) */
        PduIdType srcPduId = PduR_GetSrcPduOfDestPdu(TxPduId, 0u);
        PduR_PduRIfTxConfirmation(srcPduId, TxPduId, E_OK);
    }
}
#endif /* PDUR_TX_CONFIRMATION == STD_ON */

#if (PDUR_TRIGGER_TRANSMIT == STD_ON)
/******************************************************************************/
/*
 * Brief               Within this API, the upper layer module (called module) shall check whether the
 *                     available data fits into the buffer size reported by PduInfoPtr->SduLength.
 *                     If it fits, it shall copy its data into the buffer provided by PduInfoPtr->SduDataPtr
 *                     and update the length of the actual copied data in PduInfoPtr->SduLength.
 *                     If not, it returns E_NOT_OK without changing PduInfoPtr.
 * ServiceId           0x41
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      TxPduId: ID of the SDU that is requested to be transmitted.
 * Param-Name[out]     None
 * Param-Name[in/out]  PduInfoPtr: Contains a pointer to a buffer (SduDataPtr) to where the SDU
 *                     data shall be copied, and the available buffer size in SduLengh.
 *                     On return, the service will indicate the length of the copied SDU
 *                     data in SduLength.
 * Return              None
 */
/******************************************************************************/
Std_ReturnType PduR_IfTriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PDUR_ONLINE != PduR_Status)
    {
        PduR_Det_ReportError(PDUR_IFTRIGGERTRANSMIT_ID, PDUR_E_UNINIT);
    }
    else if (TxPduId >= PduR_ConfigStd->PduRRoutingPathNum)
    {
        PduR_Det_ReportError(PDUR_IFTRIGGERTRANSMIT_ID, PDUR_E_PDU_ID_INVALID);
    }
    else if ((PduInfoPtr == NULL_PTR) || (PduInfoPtr->SduDataPtr == NULL_PTR))
    {
        PduR_Det_ReportError(PDUR_IFTRIGGERTRANSMIT_ID, PDUR_E_PARAM_POINTER);
    }
    else
#endif
    {
        PduIdType srcPduId = PduR_GetSrcPduOfDestPdu(TxPduId, 0u);
        result = PduR_PduRTriggerTransmit(srcPduId, TxPduId, PduInfoPtr);
    }
    return result;
}
#endif /* PDUR_TRIGGER_TRANSMIT == STD_ON */

#if (PDUR_TP_COPYRXDATA == STD_ON)
/******************************************************************************/
/*
 * Brief               This function is called to provide the received data of an I-PDU segment (N-PDU) to the upper
 * layer. Each call to this function provides the next part of the I-PDU data. The size of the remaining data is written
 * to the position indicated by bufferSizePtr. ServiceId           0x44 Sync/Async          Synchronous Reentrancy
 * Reentrant Param-Name[in]      id: Identification of the received I-PDU. info: Provides the source buffer (SduDataPtr)
 * and the number of bytes to be copied (SduLength). An SduLength of 0 can be used to query the current amount of
 * available buffer in the upper layer module. In this case, the SduDataPtr may be a NULL_PTR. Param-Name[out]
 * bufferSizePtr: Available receive buffer after data has been copied. Param-Name[in/out]  None Return
 * BufReq_ReturnType(BUFREQ_OK,BUFREQ_E_NOT_OK) BUFREQ_OK: Data copied successfully BUFREQ_E_NOT_OK: Data was not copied
 * because an error occurred.
 */
/******************************************************************************/
BufReq_ReturnType PduR_TpCopyRxData(PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr)
{
    BufReq_ReturnType bufQeqReturn = BUFREQ_E_NOT_OK;
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PDUR_ONLINE != PduR_Status)
    {
        PduR_Det_ReportError(PDUR_TPCOPYRXDATA_ID, PDUR_E_UNINIT);
    }
    else if ((id >= PduR_ConfigStd->PduRSrcPduNum) || (FALSE == PDUR_ROUTINGPATH_CFG(id).TpRoute))
    {
        PduR_Det_ReportError(PDUR_TPCOPYRXDATA_ID, PDUR_E_PDU_ID_INVALID);
    }
    else if ((NULL_PTR == info) || (NULL_PTR == bufferSizePtr))
    {
        PduR_Det_ReportError(PDUR_TPCOPYRXDATA_ID, PDUR_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 destPduNum = PduR_GetDestPduNumOfSrcPdu(id);
#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
        PduLengthType bufferSizeMin = PDUR_PDU_LENGTH_INVALID;
#endif
        for (uint8 cnt = 0u; cnt < destPduNum; cnt++) /* PRQA S 0771 */ /* MISRA Rule 15.4 */
        {
            PduIdType destPduId = PduR_GetDestPduOfRoutingPath(id, cnt);
            if (!PduR_IsRoutingPathEnable(destPduId))
            {
                continue;
            }

            PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);
#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
            /* Only support 1:1 routing for transport protocol I-PDU forwarding */
            if (routeType == PDUR_ROUTE_TP_RX_NOBUFFERED)
            {
                bufQeqReturn = PduR_CopyRxDataForward(destPduId, info, bufferSizePtr);
                break;
            }
#endif /* PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON */

#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
            if ((routeType == PDUR_ROUTE_TP_RX_BUFFERED) || (routeType == PDUR_ROUTE_TP_GW_BUFFERED))
            {
                if (PduR_SourceRouteStatus[id] != PDUR_RES_PENDING)
                {
                    break;
                }

                PduLengthType bufferSize;
                if (PduR_PduRCopyRxDataBuffered(id, destPduId, info, &bufferSize) == BUFREQ_OK)
                {
                    bufQeqReturn = BUFREQ_OK;
                    if (bufferSizeMin > bufferSize)
                    {
                        bufferSizeMin = bufferSize;
                    }
                }

                if ((bufQeqReturn == BUFREQ_OK) && ((cnt + 1u) == destPduNum))
                {
                    *bufferSizePtr = bufferSizeMin;
                }
            }
#endif /* PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
          || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON */
        }
    }
    return bufQeqReturn;
}
#endif /* PDUR_TP_COPYRXDATA == STD_ON */

#if (PDUR_TP_RXINDICATION == STD_ON)
/******************************************************************************/
/*
 * Brief               Called after an I-PDU has been received via the TP API, the result indicates
 *                     whether the transmission was successful or not.
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
void PduR_TpRxIndication(PduIdType id, Std_ReturnType result)
{
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PDUR_ONLINE != PduR_Status)
    {
        PduR_Det_ReportError(PDUR_TPRXINDICATION_ID, PDUR_E_UNINIT);
    }
    else if ((id >= PduR_ConfigStd->PduRSrcPduNum) || (FALSE == PDUR_ROUTINGPATH_CFG(id).TpRoute))
    {
        PduR_Det_ReportError(PDUR_TPRXINDICATION_ID, PDUR_E_PDU_ID_INVALID);
    }
    else
#endif
    {
        uint8 destPduNum = PduR_GetDestPduNumOfSrcPdu(id);
        for (uint8 cnt = 0u; cnt < destPduNum; cnt++) /* PRQA S 0771 */ /* MISRA Rule 15.4 */
        {
            PduIdType destPduId = PduR_GetDestPduOfRoutingPath(id, cnt);
            PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
            if (routeType == PDUR_ROUTE_TP_RX_NOBUFFERED)
            {
                PduR_TpRxIndicationForward(destPduId, result);
                break;
            }
#endif /* PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON */
#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
            if ((routeType == PDUR_ROUTE_TP_RX_BUFFERED) || (routeType == PDUR_ROUTE_TP_GW_BUFFERED))
            {
                if (PduR_SourceRouteStatus[id] != PDUR_RES_PENDING)
                {
                    break;
                }

                PduR_PduRTpRxIndicationBuffered(id, destPduId, result);

                if ((cnt + 1u) == destPduNum)
                {
                    PduR_SourceRouteStatus[id] = PDUR_RES_INITIAL;
                }
            }
#endif /* PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
          || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON */
        }
    }
}
#endif /* PDUR_TP_RXINDICATION == STD_ON */

#if (PDUR_TP_STARTOFRECEPTION_TRANSMIT == STD_ON)
/******************************************************************************/
/*
 * Brief               This function is called at the start of receiving an N-SDU. The N-SDU might be
 *                     fragmented into multiple N-PDUs (FF with one or more following CFs) or might
 *                     consist of a single N-PDU (SF).
 * ServiceId           0x46
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the I-PDU.
 *                     info: Pointer to a PduInfoType structure containing the payload
 *                     data (without protocol information) and payload length of the
 *                     first frame or single frame of a transport protocol I-PDU
 *                     reception. Depending on the global parameter
 *                     MetaDataLength, additional bytes containing MetaData (e.g.
 *                     the CAN ID) are appended after the payload data, increasing
 *                     the length accordingly. If neither first/single frame data nor
 *                     MetaData are available, this parameter is set to NULL_PTR.
 *                     TpSduLength: Total length of the N-SDU to be received.
 * Param-Name[out]     bufferSizePtr: Available receive buffer in the receiving module. This
 *                     parameter will be used to compute the Block Size (BS) in the transport protocol module.
 * Param-Name[in/out]  None
 * Return              BufReq_ReturnType(BUFREQ_OK,BUFREQ_E_NOT_OK,BUFREQ_E_OVFL)
 *                     BUFREQ_OK: Connection has been accepted. bufferSizePtr
 *                     indicates the available receive buffer; reception is continued.
 *                     If no buffer of the requested size is available, a receive buffer
 *                     size of 0 shall be indicated by bufferSizePtr.
 *                     BUFREQ_E_NOT_OK: Connection has been rejected;
 *                     reception is aborted. bufferSizePtr remains unchanged.
 *                     BUFREQ_E_OVFL: No buffer of the required length can be
 *                     provided; reception is aborted. bufferSizePtr remains unchanged.
 */
/******************************************************************************/
BufReq_ReturnType PduR_TpStartOfReception(
    PduIdType id,
    const PduInfoType* info,
    PduLengthType TpSduLength,
    PduLengthType* bufferSizePtr)
{
    BufReq_ReturnType bufQeqReturn = BUFREQ_E_NOT_OK;
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PDUR_ONLINE != PduR_Status)
    {
        PduR_Det_ReportError(PDUR_TPSTARTOFRECEPTION_ID, PDUR_E_UNINIT);
    }
    else if ((id >= PduR_ConfigStd->PduRSrcPduNum) || (FALSE == PDUR_ROUTINGPATH_CFG(id).TpRoute))
    {
        PduR_Det_ReportError(PDUR_TPSTARTOFRECEPTION_ID, PDUR_E_PDU_ID_INVALID);
    }
    else if (
        (NULL_PTR == bufferSizePtr)
        || ((NULL_PTR != info) && (0u < PduR_GetMetaDataLengthOfSrcPdu(id)) && (NULL_PTR == info->MetaDataPtr)))
    {
        PduR_Det_ReportError(PDUR_TPSTARTOFRECEPTION_ID, PDUR_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint8 destPduNum = PduR_GetDestPduNumOfSrcPdu(id);
#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
        PduLengthType bufferSizeMin = PDUR_PDU_LENGTH_INVALID;
#endif /* PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
          || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON */
        for (uint8 cnt = 0u; cnt < destPduNum; cnt++) /* PRQA S 0771 */ /* MISRA Rule 15.4 */
        {
            PduIdType destPduId = PduR_GetDestPduOfRoutingPath(id, cnt);
            if (!PduR_IsRoutingPathEnable(destPduId))
            {
                continue;
            }

            PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);
#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
            /* Only support 1:1 routing for transport protocol I-PDU forwarding */
            if (routeType == PDUR_ROUTE_TP_RX_NOBUFFERED)
            {
                bufQeqReturn = PduR_StartOfReceptionForward(destPduId, info, TpSduLength, bufferSizePtr);
                break;
            }
#endif /* PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON */

#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
            if ((routeType == PDUR_ROUTE_TP_RX_BUFFERED) || (routeType == PDUR_ROUTE_TP_GW_BUFFERED))
            {
                if (PduR_SourceRouteStatus[id] != PDUR_RES_INITIAL)
                {
                    break;
                }

                PduLengthType bufferSize;
                if (PduR_PduRStartOfReceptionBuffered(id, destPduId, info, TpSduLength, &bufferSize) == BUFREQ_OK)
                {
                    bufQeqReturn = BUFREQ_OK;
                    if (bufferSizeMin > bufferSize)
                    {
                        bufferSizeMin = bufferSize;
                    }
                }

                if ((cnt + 1u) == destPduNum)
                {
                    if (bufQeqReturn == BUFREQ_OK)
                    {
                        *bufferSizePtr = bufferSizeMin;
                        PduR_SourceRouteStatus[id] = PDUR_RES_PENDING;
                    }
                    else
                    {
                        bufQeqReturn = BUFREQ_E_OVFL;
                    }
                }
            }
#endif /* PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
          || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON */
        }
    }
    return bufQeqReturn;
}
#endif /* PDUR_TP_STARTOFRECEPTION_TRANSMIT == STD_ON */

#if (PDUR_TP_COPYTXDATA == STD_ON)
/******************************************************************************/
/*
 * Brief               This function is called to acquire the transmit data of an I-PDU segment (N-PDU).
 *                     Each call to this function provides the next part of the I-PDU data unless retry-
 *                     >TpDataState is TP_DATARETRY. In this case the function restarts to copy the
 *                     data beginning at the offset from the current position indicated by retry-
 *                     >TxTpDataCnt. The size of the remaining data is written to the position indicated
 *                     by availableDataPtr.
 * ServiceId           0x43
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the transmitted I-PDU.
 *                     info: Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 * (SduLength). If not enough transmit data is available, no data is copied by the upper layer module and BUFREQ_E_BUSY
 * is returned.The lower layer module may retry the call.An SduLength of 0 can be used to indicate state changes in the
 * retry parameter or to query the current amount of available data in the upper layer module. In this case, the
 * SduDataPtr may be a NULL_PTR. retry: This parameter is used to acknowledge transmitted data or to retransmit data
 * after transmission problems. If the retry parameter is a NULL_PTR, it indicates that the transmit data can be removed
 * from the buffer immediately after it has been copied. Otherwise, the retry parameter must point to a valid
 * RetryInfoType element. If TpDataState indicates TP_CONFPENDING, the previously copied data must remain in the TP
 * buffer to be available for error recovery.TP_DATACONF indicates that all data that has been copied before this call
 * is confirmed and can be removed from the TP buffer. Data copied by this API call is excluded and will be confirmed
 * later.TP_DATARETRY indicates that this API call shall copy previously copied data in order to recover from an error.
 * In this case TxTpDataCnt specifies the offset in bytes from the current data copy position. Param-Name[out]
 * availableDataPtr: Indicates the remaining number of bytes that are available in the upper layer module's Tx buffer.
 *                     availableDataPtr can be used by TP modules that support dynamic payload lengths (e.g. FrIsoTp) to
 * determine the size of the following CFs. Param-Name[in/out]  None Return              BufReq_ReturnType
 * (BUFREQ_OK,BUFREQ_E_BUSY,BUFREQ_E_NOT_OK) BUFREQ_OK: Data has been copied to the transmit buffer completely as
 * requested. BUFREQ_E_BUSY: Request could not be fulfilled, because the required amount of Tx data is not available.
 * The lower layer module may retry this call later on. No data has been copied. BUFREQ_E_NOT_OK: Data has not been
 * copied. Request failed.
 */
/******************************************************************************/
BufReq_ReturnType PduR_TpCopyTxData(
    PduIdType id,
    const PduInfoType* info,
    const RetryInfoType* retry,
    PduLengthType* availableDataPtr)
{
    BufReq_ReturnType bufQeqReturn = BUFREQ_E_NOT_OK;
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PDUR_ONLINE != PduR_Status)
    {
        PduR_Det_ReportError(PDUR_TPCOPYTXDATA_ID, PDUR_E_UNINIT);
    }
    else if ((id >= PduR_ConfigStd->PduRRoutingPathNum) || !PduR_IsTpRouteOfDestPdu(id))
    {
        PduR_Det_ReportError(PDUR_TPCOPYTXDATA_ID, PDUR_E_PDU_ID_INVALID);
    }
    else
#endif
    {
        if (PduR_IsRoutingPathEnable(id))
        {
            bufQeqReturn = PduR_PduRCopyTxData(id, info, retry, availableDataPtr);
        }
    }
    return bufQeqReturn;
}
#endif /* PDUR_TP_COPYTXDATA == STD_ON */

#if (PDUR_TP_TXCONFIRMATION == STD_ON)
/******************************************************************************/
/*
 * Brief               This function is called after the I-PDU has been transmitted on its network, the
 *                     result indicates whether the transmission was successful or not.
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
void PduR_TpTxConfirmation(PduIdType id, Std_ReturnType result)
{
    PduIdType srcPdu = PduR_GetSrcPduOfDestPdu(id, 0u);
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (PDUR_ONLINE != PduR_Status)
    {
        PduR_Det_ReportError(PDUR_TPTXCONFIRMATION_ID, PDUR_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((detNoErr) && ((id >= PduR_ConfigStd->PduRRoutingPathNum) || (FALSE == PDUR_ROUTINGPATH_CFG(srcPdu).TpRoute)))
    {
        PduR_Det_ReportError(PDUR_TPTXCONFIRMATION_ID, PDUR_E_PDU_ID_INVALID);
        detNoErr = FALSE;
    }
    if (detNoErr)
#endif
    {
        PduR_PduRTpTxConfirmation(srcPdu, id, result);
    }
}
#endif /* PDUR_TP_TXCONFIRMATION == STD_ON */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#endif /* STD_OFF == PDUR_ZERO_COST_OPERATION */
/*=======[E N D   O F   F I L E]==============================================*/
