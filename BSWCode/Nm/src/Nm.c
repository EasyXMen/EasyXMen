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
/*
 ************************************************************************************************************************
 **
 **  @file               : Nm.c
 **  @author             : Wanglili
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Implementation for Nm
 **
 ***********************************************************************************************************************/
/* PRQA S 1503, 1532 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */

#include "SchM_Nm.h"
#include "ComM_Nm.h"
#include "Nm.h"
#include "Nm_Internal.h"

#if NM_DEV_ERROR_DETECT == STD_ON
#include "Det.h"
#endif

#if NM_STATE_CHANGE_IND_ENABLED == STD_ON
#include "Com.h"
#endif

#if NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON
#include "IStdLib.h"
#endif

#if NM_MULTIPLE_PARTITION_USED == STD_ON
#include "Os.h"
#endif
#if NM_STATE_CHANGE_IND_ENABLED == STD_ON || NM_CAR_WAKE_UP_RX_ENABLED == STD_ON
#include "BswM_Nm.h"
#endif

/* ===================================================== macros ===================================================== */
#if NM_MULTIPLE_PARTITION_USED == STD_ON
#define NM_GET_PARTITION_OF_CHANNEL(chIdx) Nm_ChLConfig[chIdx].ChannelReferencedPartitionId
#endif

#define NM_UNUSED(v) (void)(v)
/* ================================================ type definitions ================================================ */
/* ========================================== internal function declarations ======================================== */
NM_LOCAL uint8 Nm_GetPartitionIndex(void);
#if ((NM_COORDINATOR_SUPPORT_ENABLED == STD_ON) || (NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON))
NM_LOCAL void Nm_InitChannelRuntimeVariables(void);
#endif
#if ((NM_COORDINATOR_SUPPORT_ENABLED == STD_ON) && (0u < NM_NUMBER_OF_CLUSTERS))
NM_LOCAL void Nm_Coordinator_MainHandle(uint8 chIdx);
NM_LOCAL void Nm_ShutdownStatusStartHandle(uint8 chIdx, uint8 clusterIndex);
NM_LOCAL void Nm_ShutdownStatusRunHandle(uint8 chIdx, uint8 clusterIndex);
NM_LOCAL void Nm_ShutdownStatusAbortHandle(uint8 chIdx, uint8 clusterIndex);
#endif

NM_LOCAL Nm_ChannelIndexType Nm_FindChannelIndex(NetworkHandleType networkHandle);

#if (STD_ON == NM_COORDINATOR_SUPPORT_ENABLED)
NM_LOCAL Std_ReturnType Nm_FindClusterIndex(uint8 chIndex, uint8* clusterIndexPtr);

#if (STD_ON == NM_COORDINATOR_SYNC_SUPPORT)
NM_LOCAL void Nm_CoorReadySleepBitHandle(boolean* topMostCoor, uint8 chIdx, boolean* cluserRsb, boolean reqCSRBitFlag);
NM_LOCAL void Nm_PassivelyCoordinatedChannelHandle(uint8 chIdx);
NM_LOCAL void Nm_PassivelyCoordinatedChannelRelease(uint8 clusterIndex, uint8 chIdx);
#endif

NM_LOCAL void Nm_CoorShutdownReady(boolean* topMostCoor, uint8 chIdx, uint8* clusterReadySleepInChMask);
/**
 * coordinator shutdown timer load default value.
 */
NM_LOCAL void Nm_CoorShutdownTimerLoader(
    const Nm_PerClusterConfigType* perClusPtr,
    uint8                          chIdx,
    uint8*                         shutdownTimerLoadOkInChMask);
NM_LOCAL Std_ReturnType Nm_CheckChannelMaskInCluster(uint8 channelMask, uint8 clusterIndex);
/**
 * coordinator shutdown timer handle function.
 */
NM_LOCAL Std_ReturnType Nm_CoorShutDownTimerHandle(uint8 chIdx, uint8* sleepAllowedInChMask);

/**
 * coordinator shutdown abort handle function.
 */
NM_LOCAL void Nm_CoorShutdownAbortHandle(uint8 chIdx, uint8* shutdownAbortFinishedInChMask);
#endif

#if NM_IF_ALL_CHANNEL_PASSIVE_MODE_ENABLED == STD_OFF
/**
 * special bus nm network request.
 */
NM_LOCAL Std_ReturnType Nm_BusNmNetworkRequest(uint8 chIndex);

/**
 * special bus network release.
 */
NM_LOCAL Std_ReturnType Nm_BusNmNetworkRelease(uint8 chIndex);
#endif

#if ((STD_ON == NM_COORDINATOR_SUPPORT_ENABLED) && (STD_ON == NM_BUS_SYNCHRONIZATION_ENABLED))
/**
 * special bus nm request sync.
 */
NM_LOCAL Std_ReturnType Nm_BusNmRequestBusSynchronization(uint8 chIndex);
#endif

#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
/**
 * special bus nm set coordinator ready sleep bit.
 */
NM_LOCAL Std_ReturnType Nm_BusNmSetSleepReadyBit(uint8 chIndex, boolean nmSleepReadyBit);
#endif

#if NM_DEV_ERROR_DETECT == STD_ON
NM_LOCAL boolean Nm_ValidateInitStatus(uint8 apiId);
NM_LOCAL boolean Nm_ValidatePartitionContext(uint8 apiId);
NM_LOCAL boolean Nm_ValidateAlreadyInitialized(void);
NM_LOCAL boolean Nm_ValidateNetworkHandle(uint8 apiId, NetworkHandleType networkHandle);
NM_LOCAL boolean Nm_ValidatePointer(uint8 apiId, const void* pointer);
NM_LOCAL boolean Nm_ValidateNetworkHandleAndPointer(uint8 apiId, NetworkHandleType networkHandle, const void* pointer);
NM_LOCAL boolean Nm_ValidateNetworkHandlePointerAndPointer(
    uint8             apiId,
    NetworkHandleType networkHandle,
    const void*       pointer1,
    const void*       pointer2);
#endif

#if (NM_PN_ERA_CALC_ENABLED == STD_ON) || (NM_PN_EIRA_CALC_ENABLED == STD_ON)

NM_LOCAL boolean Nm_PncAggregateAndTimerHandle(Nm_ChannelIndexType chIndex, boolean eraFlg, const uint8* rawData);

#if (NM_PN_EIRA_CALC_ENABLED == STD_ON)
NM_LOCAL void Nm_PnEiraResetTimerHandle(void);
#endif

#if (NM_PN_ERA_CALC_ENABLED == STD_ON)
NM_LOCAL void Nm_PnEraResetTimerHandle(uint8 chIndex);
#endif

#endif
/* ============================================ internal data definitions =========================================== */
#define NM_START_SEC_VAR_CLEARED_8
#include "Nm_MemMap.h"
NM_LOCAL Nm_InitStatusType Nm_InitStatus[NM_PARTITION_NUMBER]; /* PRQA S 3218, 3229 */ /* VL_Nm_3218, VL_Nm_3229 */
#define NM_STOP_SEC_VAR_CLEARED_8
#include "Nm_MemMap.h"

#if ((NM_COORDINATOR_SUPPORT_ENABLED == STD_ON) || (NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON))
#define NM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Nm_MemMap.h"
NM_LOCAL boolean Nm_GlobalVariableInitialized = FALSE;
#define NM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Nm_MemMap.h"
#endif

#if (NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON) && (NM_PN_EIRA_CALC_ENABLED == STD_ON)
#define NM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"
NM_LOCAL Nm_InnerGlobalType Nm_InnerGlobal;
#define NM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"
#endif

#if (STD_ON == NM_COORDINATOR_SUPPORT_ENABLED)
#if (NM_NUMBER_OF_CLUSTERS > 0u)
/**
 * per nm cluster have a struct verial ro management status .
 */
#define NM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"
NM_LOCAL Nm_InnerClusterType Nm_InnerCluster[NM_NUMBER_OF_CLUSTERS];
#define NM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"
#endif
#endif
/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/**
 * Initializes the NM Interface.
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_Init(const Nm_ConfigType* configPtr)
{
    NM_UNUSED(configPtr);
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_INIT) && Nm_ValidateAlreadyInitialized())
#endif
    {
#if ((NM_COORDINATOR_SUPPORT_ENABLED == STD_ON) || (NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON))
        Nm_InitChannelRuntimeVariables();

        SchM_Enter_Nm_Coordinator();
        if (!Nm_GlobalVariableInitialized)
        {
            Nm_GlobalVariableInitialized = TRUE;
#if NM_NUMBER_OF_CLUSTERS > 0u
            for (uint8 index = 0u; index < NM_NUMBER_OF_CLUSTERS; index++)
            {
                Nm_InnerCluster[index].TopMostCoor                      = TRUE;
                Nm_InnerCluster[index].ShutdownStatus                   = NM_SHUTDOWN_STU_INIT;
                Nm_InnerCluster[index].ReadySleepInChMask               = 0u;
                Nm_InnerCluster[index].CoorRsbInCluster                 = FALSE;
                Nm_InnerCluster[index].ShutdownTimerLoadOkInChMask      = 0u;
                Nm_InnerCluster[index].SleepAllowedInChMask             = 0u;
                Nm_InnerCluster[index].ShutdownAbortFinishedInChMask    = 0u;
                Nm_InnerCluster[index].PassiveChannelReadySleepInChMask = 0u;
            }
#endif

#if (NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON) && (NM_PN_EIRA_CALC_ENABLED == STD_ON)

            /**
             *   Clear pn EIRA info buffer.
             */
            Nm_InnerGlobal.UpDateEiraFlg = FALSE;
            for (uint8 loop = 0u; loop < NM_PN_EIRA_INFO_LENGTH; loop++)
            {
                Nm_InnerGlobal.PnEiraInfo[loop] = 0x00u;
            }

            /**
             *   Clear pn EIRA reset timer.
             */
            for (uint8 loop = 0u; loop < (NM_PN_EIRA_INFO_LENGTH * NM_SINGLEBYTE_BASENUM); loop++)
            {
                Nm_InnerGlobal.PnEiraResetTimer[loop] = 0x00u;
            }
#endif
        }
        SchM_Exit_Nm_Coordinator();
#endif

        Nm_InitStatus[Nm_GetPartitionIndex()] = NM_INIT;
    }
}

/**
 * This function calls the BusNm PassiveStartUp function
 */
Std_ReturnType Nm_PassiveStartUp(NetworkHandleType networkHandle)
{
    Std_ReturnType ret = E_NOT_OK;
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_PASSIVESTARTUP) && Nm_ValidateInitStatus(NM_SERVICE_ID_PASSIVESTARTUP)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_PASSIVESTARTUP, networkHandle))
#endif
    {
        Nm_ChannelIndexType chIndex = Nm_FindChannelIndex(networkHandle);
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
        /* if this network is part of a coordinator cluster,shall translate nm_networkRequest */
        if (Nm_ChLConfig[chIndex].CoordClusterIndex != NM_INVALID_CLUSTER_INDEX)
        {
            ret = Nm_NetworkRequest(networkHandle);
        }
        else
#endif
        {
            if (Nm_ChLConfig[chIndex].BusNmApi != NULL_PTR)
            {
                const BusNm_PassiveStartUp_Type specific_PassiveStartUp =
                    Nm_ChLConfig[chIndex].BusNmApi->BusNm_PassiveStartUp;
                if (specific_PassiveStartUp != NULL_PTR)
                {
                    ret = specific_PassiveStartUp(networkHandle);
                }
            }
        }
    }
    return ret;
}

/**
 * This function calls the <BusNm>_NetworkRequest function
 */
Std_ReturnType Nm_NetworkRequest(NetworkHandleType networkHandle)
{
    Std_ReturnType ret = E_NOT_OK;

#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_NETWORKREQUEST) && Nm_ValidateInitStatus(NM_SERVICE_ID_NETWORKREQUEST)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_NETWORKREQUEST, networkHandle))
#endif
    {
/* SWS_Nm_00130 */
#if NM_IF_ALL_CHANNEL_PASSIVE_MODE_ENABLED == STD_OFF
        Nm_ChannelIndexType chIndex = Nm_FindChannelIndex(networkHandle);
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
        if (Nm_ChLConfig[chIndex].CoordClusterIndex != NM_INVALID_CLUSTER_INDEX)
        {
            SchM_Enter_Nm_Coordinator();
            Nm_InnerChannel[chIndex]->UpLayNetRequest = TRUE;
            Nm_InnerChannel[chIndex]->AllowShutdown   = FALSE;
            SchM_Exit_Nm_Coordinator();
#if (NM_NUMBER_OF_CLUSTERS > 0u)
            uint8 clusterIndex = 0x0u;
            if (Nm_FindClusterIndex(chIndex, &clusterIndex) == (uint8)E_OK)
            {
                SchM_Enter_Nm_Coordinator();
                if ((Nm_InnerCluster[clusterIndex].ShutdownStatus == NM_SHUTDOWN_STU_RUN)
                    || (Nm_InnerCluster[clusterIndex].ShutdownStatus == NM_SHUTDOWN_STU_ABORT))
                {
                    Nm_InnerCluster[clusterIndex].ShutdownStatus = NM_SHUTDOWN_STU_ABORT;
                }
                else
                {
                    Nm_InnerCluster[clusterIndex].ShutdownStatus = NM_SHUTDOWN_STU_START;
                }
                SchM_Exit_Nm_Coordinator();
            }
#endif
        }
#endif
        /**
         * Nm shall inform ComM about start of network by calling ComM_Nm_NetworkMode in this case.
         * */
        if (Nm_ChLConfig[chIndex].BusNmType == NM_BUSNM_LOCALNM)
        {
            ComM_Nm_NetworkMode(Nm_ChLConfig[chIndex].ComMChannelRef);
            ret = E_OK;
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
            SchM_Enter_Nm_Coordinator();
            Nm_InnerChannel[chIndex]->BusAwakeFlg       = TRUE;
            Nm_InnerChannel[chIndex]->RemoteSleepIndFlg = FALSE;
            SchM_Exit_Nm_Coordinator();
#endif
        }
        else
        {
            ret = Nm_BusNmNetworkRequest(chIndex);
        }
#endif
    }
    return ret;
}

/**
 * This function calls the BusNm NetworkRelease function
 */
Std_ReturnType Nm_NetworkRelease(NetworkHandleType networkHandle)
{
    Std_ReturnType ret = E_NOT_OK;

#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_NETWORKRELEASE) && Nm_ValidateInitStatus(NM_SERVICE_ID_NETWORKRELEASE)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_NETWORKRELEASE, networkHandle))
#endif
    {
#if NM_IF_ALL_CHANNEL_PASSIVE_MODE_ENABLED == STD_OFF
        Nm_ChannelIndexType chIndex = Nm_FindChannelIndex(networkHandle);
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
        if (Nm_ChLConfig[chIndex].CoordClusterIndex != NM_INVALID_CLUSTER_INDEX)
        {
            SchM_Enter_Nm_Coordinator();
            Nm_InnerChannel[chIndex]->UpLayNetRequest = FALSE;
            SchM_Exit_Nm_Coordinator();
            ret = E_OK;
        }
        else
        {
            ret = Nm_BusNmNetworkRelease(chIndex);
        }
#else
        ret = Nm_BusNmNetworkRelease(chIndex);
#endif
#endif
    }
    return ret;
}

#if (NM_COM_CONTROL_ENABLED == STD_ON)
/**
 * disables the NM PDU transmission ability.
 */
Std_ReturnType Nm_DisableCommunication(NetworkHandleType networkHandle)
{
    Std_ReturnType ret = E_NOT_OK;
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_DISABLECOMMUNICATION)
        && Nm_ValidateInitStatus(NM_SERVICE_ID_DISABLECOMMUNICATION)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_DISABLECOMMUNICATION, networkHandle))
#endif
    {
#if NM_IF_ALL_CHANNEL_PASSIVE_MODE_ENABLED == STD_OFF
        Nm_ChannelIndexType               chIndex     = Nm_FindChannelIndex(networkHandle);
        const Nm_SpecificLowLayerApiType* busNmApiPtr = Nm_ChLConfig[chIndex].BusNmApi;
        if (busNmApiPtr != NULL_PTR)
        {
            const BusNm_DisableCommunication_Type specific_DisableCommunication =
                busNmApiPtr->BusNm_DisableCommunication;
            if (specific_DisableCommunication != NULL_PTR)
            {
                ret = specific_DisableCommunication(networkHandle);
            }
        }
#endif
    }
    return ret;
}
#endif

#if (NM_COM_CONTROL_ENABLED == STD_ON)
/**
 * Enables the NM PDU transmission ability.
 */
Std_ReturnType Nm_EnableCommunication(NetworkHandleType networkHandle)
{
    Std_ReturnType ret = E_NOT_OK;

#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_ENABLECOMMUNICATION)
        && Nm_ValidateInitStatus(NM_SERVICE_ID_ENABLECOMMUNICATION)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_ENABLECOMMUNICATION, networkHandle))
#endif
    {
#if NM_IF_ALL_CHANNEL_PASSIVE_MODE_ENABLED == STD_OFF
        Nm_ChannelIndexType               chIndex     = Nm_FindChannelIndex(networkHandle);
        const Nm_SpecificLowLayerApiType* busNmApiPtr = Nm_ChLConfig[chIndex].BusNmApi;
        if (busNmApiPtr != NULL_PTR)
        {
            const BusNm_EnableCommunication_Type specific_EnableCommunication = busNmApiPtr->BusNm_EnableCommunication;
            if (specific_EnableCommunication != NULL_PTR)
            {
                ret = specific_EnableCommunication(networkHandle);
            }
        }
#endif
    }
    return ret;
}
#endif

#if ((NM_COM_USER_DATA_SUPPORT == STD_OFF) && (NM_USER_DATA_ENABLED == STD_ON))
/**
 * Set user data for NM messages transmitted next on the bus.
 */
Std_ReturnType Nm_SetUserData(NetworkHandleType networkHandle, const uint8* nmUserDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidateNetworkHandleAndPointer(NM_SERVICE_ID_SETUSERDATA, networkHandle, nmUserDataPtr))
#endif
    {
#if (NM_IF_ALL_CHANNEL_PASSIVE_MODE_ENABLED == STD_OFF)
        Nm_ChannelIndexType               chIndex     = Nm_FindChannelIndex(networkHandle);
        const Nm_SpecificLowLayerApiType* busNmApiPtr = Nm_ChLConfig[chIndex].BusNmApi;
        if (busNmApiPtr != NULL_PTR)
        {
            const BusNm_SetUserData_Type specific_SetUserData = busNmApiPtr->BusNm_SetUserData;
            if (specific_SetUserData != NULL_PTR)
            {
                ret = specific_SetUserData(networkHandle, nmUserDataPtr);
            }
        }
#endif
    }
    return ret;
}
#endif

#if (NM_USER_DATA_ENABLED == STD_ON)
/**
 * Get user data out of the last successfully received NM message
 */
Std_ReturnType Nm_GetUserData(NetworkHandleType networkHandle, uint8* nmUserDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidateNetworkHandleAndPointer(NM_SERVICE_ID_GETUSERDATA, networkHandle, nmUserDataPtr))
#endif
    {
        Nm_ChannelIndexType               chIndex     = Nm_FindChannelIndex(networkHandle);
        const Nm_SpecificLowLayerApiType* busNmApiPtr = Nm_ChLConfig[chIndex].BusNmApi;
        if (busNmApiPtr != NULL_PTR)
        {
            const BusNm_GetUserData_Type specific_GetUserData = busNmApiPtr->BusNm_GetUserData;
            if (specific_GetUserData != NULL_PTR)
            {
                ret = specific_GetUserData(networkHandle, nmUserDataPtr);
            }
        }
    }
    return ret;
}
#endif
/**
 *  Get the whole PDU data out of the most recently received NM message. For that purpose
 *  <BusNm>_GetPduData shall be called in case NmBusType is not set to NM_BUSNM_LOCALNM.
 *  (e.g. CanNm_GetPduData function is called if channel is configured as CAN).
 */
Std_ReturnType Nm_GetPduData(NetworkHandleType networkHandle, uint8* nmPduData)
{
    Std_ReturnType ret = E_NOT_OK;
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidateNetworkHandleAndPointer(NM_SERVICE_ID_GETPDUDATA, networkHandle, nmPduData))
#endif
    {
        Nm_ChannelIndexType               chIndex     = Nm_FindChannelIndex(networkHandle);
        const Nm_SpecificLowLayerApiType* busNmApiPtr = Nm_ChLConfig[chIndex].BusNmApi;
        if (busNmApiPtr != NULL_PTR)
        {
            const BusNm_GetPduData_Type specific_GetPduData = busNmApiPtr->BusNm_GetPduData;
            if (specific_GetPduData != NULL_PTR)
            {
                ret = specific_GetPduData(networkHandle, nmPduData);
            }
        }
    }
    return ret;
}
/**
 * Set Repeat Message Request Bit for NM messages transmitted next on the bus. For that purpose
 * Bus Nm_RepeatMessageRequest shall be called in case NmBusType is not set to NM_BUSNM_LOCALNM.
 * (e.g. CanNm_RepeatMessageRequest function is called if channel is configured as CAN). This will force all nodes on
 */
Std_ReturnType Nm_RepeatMessageRequest(NetworkHandleType networkHandle)
{
    Std_ReturnType ret = E_NOT_OK;
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_REPEATMESSAGEREQUEST)
        && Nm_ValidateInitStatus(NM_SERVICE_ID_REPEATMESSAGEREQUEST)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_REPEATMESSAGEREQUEST, networkHandle))
#endif
    {
#if (NM_IF_ALL_CHANNEL_PASSIVE_MODE_ENABLED == STD_OFF)
        Nm_ChannelIndexType               chIndex     = Nm_FindChannelIndex(networkHandle);
        const Nm_SpecificLowLayerApiType* busNmApiPtr = Nm_ChLConfig[chIndex].BusNmApi;
        if (busNmApiPtr != NULL_PTR)
        {
            const BusNm_RepeatMessageRequest_Type specific_RepeatMessageRequest =
                busNmApiPtr->BusNm_RepeatMessageRequest;
            if (specific_RepeatMessageRequest != NULL_PTR)
            {
                ret = specific_RepeatMessageRequest(networkHandle);
            }
        }
#endif
    }
    return ret;
}
/**
 * Get node identifier out of the last successfully received NM-message. The function
 * <Bus>Nm_GetNodeIdentifier shall be called in case NmBusType is not set to NM_BUSNM_LOCALNM.
 * (e.g. CanNm_GetNodeIdentifier function is called if channel is configured as CAN).
 */
Std_ReturnType Nm_GetNodeIdentifier(NetworkHandleType networkHandle, uint8* nmNodeIdPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidateNetworkHandleAndPointer(NM_SERVICE_ID_GETNODEIDENTIFIER, networkHandle, nmNodeIdPtr))
#endif
    {
        Nm_ChannelIndexType               chIndex     = Nm_FindChannelIndex(networkHandle);
        const Nm_SpecificLowLayerApiType* busNmApiPtr = Nm_ChLConfig[chIndex].BusNmApi;
        if (busNmApiPtr != NULL_PTR)
        {
            const BusNm_GetNodeIdentifier_Type specific_GetNodeIdentifier = busNmApiPtr->BusNm_GetNodeIdentifier;
            if (specific_GetNodeIdentifier != NULL_PTR)
            {
                ret = specific_GetNodeIdentifier(networkHandle, nmNodeIdPtr);
            }
        }
    }
    return ret;
}
/**
 * Get node identifier configured for the local node. For that purpose
 * <Bus>Nm_GetLocalNodeIdentifier shall be called in case NmBusType is not set to NM_BUSNM_LOCALNM. (e.g.
 * CanNm_GetLocalNodeIdentifier function is called if channel is configured as CAN).
 */
Std_ReturnType Nm_GetLocalNodeIdentifier(NetworkHandleType networkHandle, uint8* nmNodeIdPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidateNetworkHandleAndPointer(NM_SERVICE_ID_GETLOCALNODEIDENTIFIER, networkHandle, nmNodeIdPtr))
#endif
    {
        Nm_ChannelIndexType               chIndex     = Nm_FindChannelIndex(networkHandle);
        const Nm_SpecificLowLayerApiType* busNmApiPtr = Nm_ChLConfig[chIndex].BusNmApi;
        if (busNmApiPtr != NULL_PTR)
        {
            const BusNm_GetLocalNodeIdentifier_Type specific_GetLocalNodeIdentifier =
                busNmApiPtr->BusNm_GetLocalNodeIdentifier;
            if (specific_GetLocalNodeIdentifier != NULL_PTR)
            {
                ret = specific_GetLocalNodeIdentifier(networkHandle, nmNodeIdPtr);
            }
        }
    }
    return ret;
}

#if (NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
/**
 * Check if remote sleep indication takes place or not
 */
Std_ReturnType Nm_CheckRemoteSleepIndication(NetworkHandleType networkHandle, boolean* remoteSleepIndPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidateNetworkHandleAndPointer(NM_SERVICE_ID_CHECKREMOTESLEEPINDICATION, networkHandle, remoteSleepIndPtr))
#endif
    {
#if (NM_IF_ALL_CHANNEL_PASSIVE_MODE_ENABLED == STD_OFF)
        Nm_ChannelIndexType               chIndex     = Nm_FindChannelIndex(networkHandle);
        const Nm_SpecificLowLayerApiType* busNmApiPtr = Nm_ChLConfig[chIndex].BusNmApi;
        if (busNmApiPtr != NULL_PTR)
        {
            const BusNm_CheckRemoteSleepIndType spf_CheckRemoteSleepIndication = busNmApiPtr->BusNm_CheckRemoteSleepInd;
            if (spf_CheckRemoteSleepIndication != NULL_PTR)
            {
                ret = spf_CheckRemoteSleepIndication(networkHandle, remoteSleepIndPtr);
            }
        }
#endif
    }
    return ret;
}
#endif

/**
 * Returns the state of the network management. The function <Bus>Nm_GetState shall be called in case NmBusType is not
 * set to NM_BUSNM_LOCALNM. (e.g. CanNm_GetState
 */
Std_ReturnType Nm_GetState(NetworkHandleType nmNetworkHandle, Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidateNetworkHandlePointerAndPointer(NM_SERVICE_ID_GETSTATE, nmNetworkHandle, nmStatePtr, nmModePtr))
#endif
    {
        Nm_ChannelIndexType               chIndex     = Nm_FindChannelIndex(nmNetworkHandle);
        const Nm_SpecificLowLayerApiType* busNmApiPtr = Nm_ChLConfig[chIndex].BusNmApi;
        if (busNmApiPtr != NULL_PTR)
        {
            const BusNm_GetState_Type specific_GetState = busNmApiPtr->BusNm_GetState;
            if (specific_GetState != NULL_PTR)
            {
                ret = specific_GetState(nmNetworkHandle, nmStatePtr, nmModePtr);
            }
        }
    }
    return ret;
}

#if (NM_VERSION_INFO_API == STD_ON)
/**
 * This service returns the version information of this module.
 */
void Nm_GetVersionInfo(Std_VersionInfoType* nmVerInfoPtr)
{
#if (NM_DEV_ERROR_DETECT == STD_ON)
    if (Nm_ValidatePointer(NM_SERVICE_ID_GETVERSIONINFO, nmVerInfoPtr))
#endif
    {
        nmVerInfoPtr->moduleID         = NM_MODULE_ID;
        nmVerInfoPtr->vendorID         = NM_VENDOR_ID;
        nmVerInfoPtr->sw_major_version = NM_SW_MAJOR_VERSION;
        nmVerInfoPtr->sw_minor_version = NM_SW_MINOR_VERSION;
        nmVerInfoPtr->sw_patch_version = NM_SW_PATCH_VERSION;
    }
}
#endif

/**
 * Notification that a NM-message has been received in the Bus-Sleep Mode, what indicates that some nodes in the
 * network have already entered the Network Mode.
 */
void Nm_NetworkStartIndication(NetworkHandleType nmNetworkHandle)
{
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_NETWORKSTARTINDICATION)
        && Nm_ValidateInitStatus(NM_SERVICE_ID_NETWORKSTARTINDICATION)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_NETWORKSTARTINDICATION, nmNetworkHandle))
#endif
    {
        Nm_ChannelIndexType chIndex = Nm_FindChannelIndex(nmNetworkHandle);
        /**
         * The indication through callback function Nm_NetworkStartIndication:shall be forwarded to ComM by calling
         * the ComM_Nm_NetworkStartIndication.
         */
        ComM_Nm_NetworkStartIndication(Nm_ChLConfig[chIndex].ComMChannelRef);
    }
}

/**
 * Notification that the network management has entered Network Mode.
 */
void Nm_NetworkMode(NetworkHandleType nmNetworkHandle)
{
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_NETWORKMODE) && Nm_ValidateInitStatus(NM_SERVICE_ID_NETWORKMODE)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_NETWORKMODE, nmNetworkHandle))
#endif
    {
        Nm_ChannelIndexType chIndex = Nm_FindChannelIndex(nmNetworkHandle);
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
        Nm_InnerChannel[chIndex]->BusAwakeFlg       = TRUE;
        Nm_InnerChannel[chIndex]->RemoteSleepIndFlg = FALSE;
#if (NM_NUMBER_OF_CLUSTERS > 0u)
        uint8 clusterIndex = 0x0u;
        if (Nm_FindClusterIndex(chIndex, &clusterIndex) == (uint8)E_OK)
        {
            SchM_Enter_Nm_Coordinator();
            if ((Nm_InnerCluster[clusterIndex].ShutdownStatus == NM_SHUTDOWN_STU_RUN)
                || (Nm_InnerCluster[clusterIndex].ShutdownStatus == NM_SHUTDOWN_STU_ABORT))
            {
                Nm_InnerCluster[clusterIndex].ShutdownStatus = NM_SHUTDOWN_STU_ABORT;
            }
            else
            {
                Nm_InnerCluster[clusterIndex].ShutdownStatus = NM_SHUTDOWN_STU_START;
            }
            if (!Nm_ChLConfig[chIndex].ActiveCoordinator)
            {
                Nm_InnerCluster[clusterIndex].TopMostCoor = FALSE;
            }
            SchM_Exit_Nm_Coordinator();
        }
#endif
#endif
        /**
         * The indication through callback function Nm_PrepareBusSleepMode: shall be forwarded to ComM by calling
         * ComM_Nm_PrepareBusSleepMode.
         */
        ComM_Nm_NetworkMode(Nm_ChLConfig[chIndex].ComMChannelRef);
    }
}
/**
 * Notification that the network management has entered Bus-Sleep Mode.
 *
 */
void Nm_BusSleepMode(NetworkHandleType nmNetworkHandle)
{
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_BUSSLEEPMODE) && Nm_ValidateInitStatus(NM_SERVICE_ID_BUSSLEEPMODE)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_BUSSLEEPMODE, nmNetworkHandle))
#endif
    {
        Nm_ChannelIndexType chIndex = Nm_FindChannelIndex(nmNetworkHandle);
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
        /* clear All inner varilable */
        Nm_InnerChannel[chIndex]->LowLayBusSyncPoint = FALSE;
        Nm_InnerChannel[chIndex]->BusAwakeFlg        = FALSE;
        Nm_InnerChannel[chIndex]->RemoteSleepIndFlg  = TRUE;

#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
        Nm_InnerChannel[chIndex]->CoorRsb = FALSE;
#endif

/** SWS_Nm_00293
 * release channel when detects the change of mode.
 * */
#if (NM_NUMBER_OF_CLUSTERS > 0u)
        uint8 clusterIndex = 0x0u;
        if (Nm_FindClusterIndex(chIndex, &clusterIndex) == (uint8)E_OK)
        {
            SchM_Enter_Nm_Coordinator();
            if ((Nm_InnerCluster[clusterIndex].ShutdownStatus == NM_SHUTDOWN_STU_START)
                && (!Nm_InnerChannel[chIndex]->UpLayNetRequest))
            {
                (void)Nm_BusNmNetworkRelease(chIndex);
            }
            if (!Nm_ChLConfig[chIndex].ActiveCoordinator)
            {
                Nm_InnerCluster[clusterIndex].TopMostCoor = TRUE;
            }
            SchM_Exit_Nm_Coordinator();
        }
#endif
#endif
        /**
         * The indication through callback function Nm_PrepareBusSleepMode: shall be forwarded to ComM by calling
         * ComM_Nm_PrepareBusSleepMode.
         */
        ComM_Nm_BusSleepMode(Nm_ChLConfig[chIndex].ComMChannelRef);
    }
}

/**
 * Notification that the network management has entered Prepare Bus-Sleep Mode.
 */
void Nm_PrepareBusSleepMode(NetworkHandleType nmNetworkHandle)
{
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_PREPAREBUSSLEEPMODE)
        && Nm_ValidateInitStatus(NM_SERVICE_ID_PREPAREBUSSLEEPMODE)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_PREPAREBUSSLEEPMODE, nmNetworkHandle))
#endif
    {
        Nm_ChannelIndexType chIndex = Nm_FindChannelIndex(nmNetworkHandle);
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
        Nm_InnerChannel[chIndex]->LowLayBusSyncPoint = FALSE;
        Nm_InnerChannel[chIndex]->RemoteSleepIndFlg  = TRUE;

/** SWS_Nm_00293
 * release channel when detects the change of mode.
 * */
#if (NM_NUMBER_OF_CLUSTERS > 0u)
        uint8 clusterIndex = 0x0u;
        if (Nm_FindClusterIndex(chIndex, &clusterIndex) == (uint8)E_OK)
        {
            SchM_Enter_Nm_Coordinator();
            if ((Nm_InnerCluster[clusterIndex].ShutdownStatus == NM_SHUTDOWN_STU_START)
                && (!Nm_InnerChannel[chIndex]->UpLayNetRequest))
            {
                (void)Nm_BusNmNetworkRelease(chIndex);
            }
            SchM_Exit_Nm_Coordinator();
        }
#endif
#endif
        /** The indication through callback function Nm_PrepareBusSleepMode: shall be forwarded to ComM by
         * calling ComM_Nm_PrepareBusSleepMode. */
        ComM_Nm_PrepareBusSleepMode(Nm_ChLConfig[chIndex].ComMChannelRef);
    }
}

#if (NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
/**
 * Notification that the network management has detected that
 * all other nodes on the network are ready to enter Bus-Sleep Mode.
 *
 */
void Nm_RemoteSleepIndication(NetworkHandleType nmNetworkHandle)
{
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_REMOTESLEEPINDICATION)
        && Nm_ValidateInitStatus(NM_SERVICE_ID_REMOTESLEEPINDICATION)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_REMOTESLEEPINDICATION, nmNetworkHandle))
#endif
    {
        Nm_ChannelIndexType chIndex = Nm_FindChannelIndex(nmNetworkHandle);
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
        Nm_InnerChannel[chIndex]->RemoteSleepIndFlg = TRUE;
#endif
    }
}
#endif

#if (NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
/**
 * Notification that the network management has detected that not all other nodes on the network are longer ready to
 * enter Bus-Sleep Mode
 */
void Nm_RemoteSleepCancellation(NetworkHandleType nmNetworkHandle)
{
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_REMOTESLEEPCANCELLATION)
        && Nm_ValidateInitStatus(NM_SERVICE_ID_REMOTESLEEPCANCELLATION)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_REMOTESLEEPCANCELLATION, nmNetworkHandle))
#endif
    {
        Nm_ChannelIndexType chIndex = Nm_FindChannelIndex(nmNetworkHandle);
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
        Nm_InnerChannel[chIndex]->RemoteSleepIndFlg = FALSE;
#if (NM_NUMBER_OF_CLUSTERS > 0u)
        if (!Nm_ChLConfig[chIndex].ChannelSleepMaster)
        {
            uint8 clusterIndex = 0x0u;
            if (Nm_FindClusterIndex(chIndex, &clusterIndex) == (uint8)E_OK)
            {
                SchM_Enter_Nm_Coordinator();
                if ((Nm_InnerCluster[clusterIndex].ShutdownStatus == NM_SHUTDOWN_STU_RUN)
                    || (Nm_InnerCluster[clusterIndex].ShutdownStatus == NM_SHUTDOWN_STU_ABORT))
                {
                    Nm_InnerCluster[clusterIndex].ShutdownStatus = NM_SHUTDOWN_STU_ABORT;
                }
                else
                {
                    Nm_InnerCluster[clusterIndex].ShutdownStatus = NM_SHUTDOWN_STU_START;
                }
                SchM_Exit_Nm_Coordinator();
            }
        }
#endif
#endif
    }
}
/**
 * Notification to the NM Coordinator functionality that this is a suitable point in time to initiate the coordinated
 * shutdown on.
 */
void Nm_SynchronizationPoint(NetworkHandleType nmNetworkHandle)
{
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_SYNCHRONIZATIONPOINT)
        && Nm_ValidateInitStatus(NM_SERVICE_ID_SYNCHRONIZATIONPOINT)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_SYNCHRONIZATIONPOINT, nmNetworkHandle))
#endif
    {
        Nm_ChannelIndexType chIndex = Nm_FindChannelIndex(nmNetworkHandle);
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
        if (Nm_ChLConfig[chIndex].SynchronizingNetwork)
        {
            /** avoid user config ChannelSleepMaster and SynchronizingNetwork is common channel, although this not
             * correct */
            if (!Nm_ChLConfig[chIndex].ChannelSleepMaster)
            {
                Nm_InnerChannel[chIndex]->LowLayBusSyncPoint = Nm_InnerChannel[chIndex]->RemoteSleepIndFlg;
            }
            else
            {
                if (!Nm_InnerChannel[chIndex]->UpLayNetRequest)
                {
                    Nm_InnerChannel[chIndex]->LowLayBusSyncPoint = TRUE;
                }
                else
                {
                    Nm_InnerChannel[chIndex]->LowLayBusSyncPoint = FALSE;
                }
            }
        }
#endif
    }
}
#endif
#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
/**
 * Sets an indication, when the NM Coordinator Sleep Ready bit in the Control Bit Vector is set
 */
void Nm_CoordReadyToSleepIndication(NetworkHandleType nmChannelHandle)
{
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_COORDREADYTOSLEEPINDICATION)
        && Nm_ValidateInitStatus(NM_SERVICE_ID_COORDREADYTOSLEEPINDICATION)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_COORDREADYTOSLEEPINDICATION, nmChannelHandle))
#endif
    {
        Nm_ChannelIndexType chIndex = Nm_FindChannelIndex(nmChannelHandle);
        /**
         *  recv CSR=1 in passive channel,then set CSR=1 in active channel
         */
        if (!Nm_ChLConfig[chIndex].ActiveCoordinator)
        {
            Nm_InnerChannel[chIndex]->CoorRsb = TRUE;
        }
    }
}
#endif

#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
/**
 * Cancels an indication, when the NM Coordinator Sleep Ready bit in the Control Bit Vector is set back to 0
 */
void Nm_CoordReadyToSleepCancellation(NetworkHandleType nmChannelHandle)
{
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_COORDREADYTOSLEEPCANCELLATION)
        && Nm_ValidateInitStatus(NM_SERVICE_ID_COORDREADYTOSLEEPCANCELLATION)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_COORDREADYTOSLEEPCANCELLATION, nmChannelHandle))
#endif
    {
        Nm_ChannelIndexType chIndex = Nm_FindChannelIndex(nmChannelHandle);
        if (!Nm_ChLConfig[chIndex].ActiveCoordinator)
        {
            Nm_InnerChannel[chIndex]->CoorRsb = FALSE;
#if (NM_NUMBER_OF_CLUSTERS > 0u)
            uint8 clusterIndex = 0x0u;
            if (Nm_FindClusterIndex(chIndex, &clusterIndex) == (uint8)E_OK)
            {
                SchM_Enter_Nm_Coordinator();
                if ((Nm_InnerCluster[clusterIndex].ShutdownStatus == NM_SHUTDOWN_STU_RUN)
                    || (Nm_InnerCluster[clusterIndex].ShutdownStatus == NM_SHUTDOWN_STU_ABORT))
                {
                    Nm_InnerCluster[clusterIndex].ShutdownStatus = NM_SHUTDOWN_STU_ABORT;
                }
                else
                {
                    Nm_InnerCluster[clusterIndex].ShutdownStatus = NM_SHUTDOWN_STU_START;
                }
                SchM_Exit_Nm_Coordinator();
            }
#endif
        }
    }
}
#endif
/**
 *   Notification that the network management has entered
 *   Synchronize Mode.
 */
void Nm_SynchronizeMode(NetworkHandleType nmNetworkHandle)
{
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_SYNCHRONIZEMODE)
        && Nm_ValidateInitStatus(NM_SERVICE_ID_SYNCHRONIZEMODE)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_SYNCHRONIZEMODE, nmNetworkHandle))
#endif
    {
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
        Nm_ChannelIndexType chIndex = Nm_FindChannelIndex(nmNetworkHandle);
        /**
         *  clear All inner varilable
         */
        Nm_InnerChannel[chIndex]->LowLayBusSyncPoint = FALSE;
        Nm_InnerChannel[chIndex]->BusAwakeFlg        = FALSE;
        Nm_InnerChannel[chIndex]->RemoteSleepIndFlg  = TRUE;

#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
        Nm_InnerChannel[chIndex]->CoorRsb = FALSE;
#endif

/** SWS_Nm_00293
 * release channel when detects the change of mode.
 * */
#if (NM_NUMBER_OF_CLUSTERS > 0u)
        uint8 clusterIndex = 0x0u;
        if (Nm_FindClusterIndex(chIndex, &clusterIndex) == (uint8)E_OK)
        {
            SchM_Enter_Nm_Coordinator();
            if ((Nm_InnerCluster[clusterIndex].ShutdownStatus == NM_SHUTDOWN_STU_START)
                && (!Nm_InnerChannel[chIndex]->UpLayNetRequest))
            {
                (void)Nm_BusNmNetworkRelease(chIndex);
            }
            SchM_Exit_Nm_Coordinator();
        }
#endif
#else
        NM_UNUSED(nmNetworkHandle);
#endif
    }
}

#if (STD_ON == NM_PDU_RX_INDICATION_ENABLED)
/**
 *   Notification that a NM message has been received.
 */
void Nm_PduRxIndication(NetworkHandleType nmNetworkHandle)
{
    NM_UNUSED(nmNetworkHandle);
}
#endif

#if (NM_STATE_CHANGE_IND_ENABLED == STD_ON)
/**
 *   notification that the state of the lower layer <Bus>Nm has changed.
 */
void Nm_StateChangeNotification(
    NetworkHandleType nmNetworkHandle,
    Nm_StateType      nmPreviousState,
    Nm_StateType      nmCurrentState)
{
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_STATECHANGENOTIFICATION)
        && Nm_ValidateInitStatus(NM_SERVICE_ID_STATECHANGENOTIFICATION)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_STATECHANGENOTIFICATION, nmNetworkHandle))
#endif
    {
        Nm_ChannelIndexType chIndex = Nm_FindChannelIndex(nmNetworkHandle);
        /* state change Notification to comM. */
        if (Nm_ChLConfig[chIndex].StateReportEnabled)
        {
#if (NM_SUPPORT_COM_SENDSIGNAL == STD_ON)
            /**
             *   NMS shall to set to signal value according to the followling table
             */
            const Nm_StateReportKindType Nm_StateReportKind[NM_STATE_CHANGE_TYPE_NUM] = {
                {NM_STATE_BUS_SLEEP, NM_STATE_REPEAT_MESSAGE, 0x01u},
                {NM_STATE_PREPARE_BUS_SLEEP, NM_STATE_REPEAT_MESSAGE, 0x02u},
                {NM_STATE_REPEAT_MESSAGE, NM_STATE_NORMAL_OPERATION, 0x04u},
                {NM_STATE_READY_SLEEP, NM_STATE_NORMAL_OPERATION, 0x08u},
                {NM_STATE_READY_SLEEP, NM_STATE_REPEAT_MESSAGE, 0x10u},
                {NM_STATE_NORMAL_OPERATION, NM_STATE_REPEAT_MESSAGE, 0x20u},
            };
            uintx   index;
            boolean findFlag = FALSE;
            for (index = 0u; (index < NM_UBOND(Nm_StateReportKind)) && (!findFlag); index++)
            {
                if ((nmCurrentState == Nm_StateReportKind[index].CurrentState)
                    && (nmPreviousState == Nm_StateReportKind[index].PreState))
                {
                    findFlag = TRUE;
                }
            }
            if (findFlag)
            {
                Com_SignalIdType signalId = Nm_ChLConfig[chIndex].ComHandleId;
                if (signalId != NM_INVALID_COM_SIGNELID)
                {
                    (void)Com_SendSignal(signalId, &Nm_StateReportKind[index - 1u].SignalValue);
                }
                BswM_Nm_StateChangeNotification(nmNetworkHandle, nmCurrentState);
            }
#else
            (void)nmPreviousState;
            (void)nmCurrentState;
#endif
        }
    }
}
#endif
/**
 *   Service to indicate that an NM message with set Repeat
 *   Message Request Bit has been received
 */
void Nm_RepeatMessageIndication(NetworkHandleType nmNetworkHandle)
{
    NM_UNUSED(nmNetworkHandle);
}
/**
 *   Service to indicate that an attempt to send an NM message
 *   failed.
 */
void Nm_TxTimeoutException(NetworkHandleType nmNetworkHandle)
{
    NM_UNUSED(nmNetworkHandle);
}

#if (NM_CAR_WAKE_UP_RX_ENABLED == STD_ON)
/**
 *   This function is called by a <Bus>Nm to indicate reception
 *   of a CWU request
 */
void Nm_CarWakeUpIndication(NetworkHandleType nmChannelHandle)
{
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_CARWAKEUPINDICATION)
        && Nm_ValidateInitStatus(NM_SERVICE_ID_CARWAKEUPINDICATION)
        && Nm_ValidateNetworkHandle(NM_SERVICE_ID_CARWAKEUPINDICATION, nmChannelHandle))
#endif
    {
        Nm_ChannelIndexType chIndex = Nm_FindChannelIndex(nmChannelHandle);
        /* @SWS_Nm_00252 */
        if (Nm_GlobalLConfig.CarWakeUpCallout != NULL_PTR)
        {
            Nm_GlobalLConfig.CarWakeUpCallout(Nm_ChLConfig[chIndex].ComMChannelRef);
        }
        else
        {
            BswM_Nm_CarWakeUpIndication(nmChannelHandle);
        }
    }
}
#endif
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON || NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON)
/**
 *  implements the processes of the NM Interface which need a fix cyclic scheduling.
 */
void Nm_MainFunction(uint8 chIdx)
{
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidatePartitionContext(NM_SERVICE_ID_MAINFUNCTION)) /* PRQA S 2991  */ /* VL_QAC_2991 */
#endif
    {
        if (Nm_InitStatus[Nm_GetPartitionIndex()] == NM_INIT)
        {
#if ((NM_COORDINATOR_SUPPORT_ENABLED == STD_ON) && (0u < NM_NUMBER_OF_CLUSTERS))
#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
            Nm_PassivelyCoordinatedChannelHandle(chIdx);
#endif
            Nm_Coordinator_MainHandle(chIdx);
#endif

#if (NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON)
#if NM_PN_ERA_CALC_ENABLED == STD_ON
            const Nm_ChannelLConfigType*    chCfgPtr = &Nm_ChLConfig[chIdx];
            Nm_InnerChannelType*            chRTPtr  = Nm_InnerChannel[chIdx];
            const Nm_PncRelatedRunTimeType* chGRTPtr = &Nm_ChannelPncRelatedRuntimeRef[chIdx];

            if (chCfgPtr->PnEraCalcEnabled)
            {
                Nm_PnEraResetTimerHandle(chIdx);
                if (chRTPtr->UpDateEraFlg)
                {
                    ComM_Nm_UpdateERA(chCfgPtr->ComMChannelRef, chGRTPtr->EraInChannel);
                    chRTPtr->UpDateEraFlg = FALSE;
                }
            }

#endif
#if NM_PN_EIRA_CALC_ENABLED == STD_ON
            if (chIdx == NM_HANDLE_EIRA_TIMER_CHANNEL_INDEX)
            {
                Nm_PnEiraResetTimerHandle();
                if (Nm_InnerGlobal.UpDateEiraFlg)
                {
                    ComM_Nm_UpdateEIRA(&Nm_InnerGlobal.PnEiraInfo[0]);
                    Nm_InnerGlobal.UpDateEiraFlg = FALSE;
                }
            }
#endif
#endif
        }
    }
}
#endif
/**
 * Indication by ComM of internal PNC requests.
 */
#if NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON
void Nm_UpdateIRA(NetworkHandleType networkHandle, const uint8* pncBitVectorPtr)
{
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidateNetworkHandleAndPointer(NM_SERVICE_ID_UPDATEIRA, networkHandle, pncBitVectorPtr))
#endif
    {
        Nm_ChannelIndexType             chIndex       = Nm_FindChannelIndex(networkHandle);
        const Nm_ChannelLConfigType*    channelConfig = &Nm_ChLConfig[chIndex];
        const Nm_PncRelatedRunTimeType* chGRTPtr      = &Nm_ChannelPncRelatedRuntimeRef[chIndex];
        if (channelConfig->PnEnabled)
        {
            SchM_Enter_Nm_IRA();
            (void)IStdLib_MemCpy(chGRTPtr->IraInChannel, pncBitVectorPtr, channelConfig->PncBitVectorLength);
            SchM_Exit_Nm_IRA();
        }
    }
}

/**
 * Function called by BusNms to request the aggregated internal PNC requests for transmission within the Nm message.
 */
void Nm_PncBitVectorTxIndication(NetworkHandleType networkHandle, uint8* pncBitVectorPtr)
{
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidateNetworkHandleAndPointer(NM_SERVICE_ID_PNCBITVECTORTXINDICATION, networkHandle, pncBitVectorPtr))
#endif
    {
        Nm_ChannelIndexType             chIndex  = Nm_FindChannelIndex(networkHandle);
        const Nm_ChannelLConfigType*    chCfgPtr = &Nm_ChLConfig[chIndex];
        const Nm_PncRelatedRunTimeType* chGRTPtr = &Nm_ChannelPncRelatedRuntimeRef[chIndex];

        if (chCfgPtr->PnEnabled)
        {
            SchM_Enter_Nm_IRA();
            (void)IStdLib_MemCpy(pncBitVectorPtr, chGRTPtr->IraInChannel, chCfgPtr->PncBitVectorLength);

#if NM_PN_EIRA_CALC_ENABLED == STD_ON
            (void)Nm_PncAggregateAndTimerHandle(chIndex, FALSE, chGRTPtr->IraInChannel);
#endif
            SchM_Exit_Nm_IRA();
        }
    }
}
/**
 *  Process received ERA requests and update EIRA. If there are any changes in ERA or EIRA, ComM will be notified
 */
void Nm_PncBitVectorRxIndication(
    NetworkHandleType networkHandle,
    const uint8*      pncBitVectorPtr,
    boolean*          relevantPncRequestDetectedPtr)
{
#if NM_DEV_ERROR_DETECT == STD_ON
    if (Nm_ValidateNetworkHandlePointerAndPointer(
            NM_SERVICE_ID_PNCBITVECTORRXINDICATION,
            networkHandle,
            pncBitVectorPtr,
            relevantPncRequestDetectedPtr))
#endif
    {
        Nm_ChannelIndexType          chIndex  = Nm_FindChannelIndex(networkHandle);
        const Nm_ChannelLConfigType* chCfgPtr = &Nm_ChLConfig[chIndex];

        *relevantPncRequestDetectedPtr = FALSE;
#if (NM_PN_ERA_CALC_ENABLED == STD_ON) || (NM_PN_EIRA_CALC_ENABLED == STD_ON)
        if (chCfgPtr->PnEnabled)
        {
            SchM_Enter_Nm_IRA();
            *relevantPncRequestDetectedPtr =
                Nm_PncAggregateAndTimerHandle(chIndex, chCfgPtr->PnEraCalcEnabled, pncBitVectorPtr);
            SchM_Exit_Nm_IRA();
        }
#endif
    }
}
/* ========================================== internal function definitions ========================================= */
/**
 * @brief  Timer of Eira handle,if timeout occurs,the timer will be reset to 0.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66719
 */
#if (NM_PN_EIRA_CALC_ENABLED == STD_ON)
NM_LOCAL void Nm_PnEiraResetTimerHandle(void)
{
    for (uint16 index = 0u; index < (sizeof(Nm_InnerGlobal.PnEiraResetTimer) / sizeof(uint16)); index++)
    {
        if (Nm_InnerGlobal.PnEiraResetTimer[index] > 0u)
        {
            Nm_InnerGlobal.PnEiraResetTimer[index]--;
            if (Nm_InnerGlobal.PnEiraResetTimer[index] == 0u)
            {
                uint8 byteIdx = (uint8)(index / NM_SINGLEBYTE_BASENUM);
                uint8 bitMask = (1u << (index % NM_SINGLEBYTE_BASENUM));

                Nm_InnerGlobal.PnEiraInfo[byteIdx] &= (uint8)(~bitMask);
                Nm_InnerGlobal.UpDateEiraFlg = TRUE;
            }
        }
    }
}
#endif
/**
 * @brief  Timer of Eira handle,if timeout occurs,the timer will be reset to 0.
 * @param[in]    chIndex   Identification of the NM-channel
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66720
 */
#if (NM_PN_ERA_CALC_ENABLED == STD_ON)
NM_LOCAL void Nm_PnEraResetTimerHandle(uint8 chIndex)
{
    const Nm_ChannelLConfigType*    chCfgPtr = &Nm_ChLConfig[chIndex];
    const Nm_PncRelatedRunTimeType* chGRTPtr = &Nm_ChannelPncRelatedRuntimeRef[chIndex];

    uint8 pnIdx = 0u;

    for (uint8 index = 0u; index < (chCfgPtr->PncBitVectorLength * NM_SINGLEBYTE_BASENUM); index++)
    {
        uint8 bitMask        = (1u << (index % NM_SINGLEBYTE_BASENUM));
        uint8 byteIndex      = index / NM_SINGLEBYTE_BASENUM;
        uint8 filterMaskData = chCfgPtr->PnFilterMaskBytePtr[byteIndex].PnFilterMaskByteValue;

        if ((filterMaskData & bitMask) > 0u)
        {
            if (chGRTPtr->EraTimerInChannel[pnIdx] > 0u)
            {
                chGRTPtr->EraTimerInChannel[pnIdx]--;
                if (chGRTPtr->EraTimerInChannel[pnIdx] == 0u)
                {
                    chGRTPtr->EraInChannel[byteIndex] &= (uint8)(~bitMask);
                    Nm_InnerChannel[chIndex]->UpDateEraFlg = TRUE;
                }
            }
            pnIdx++;
        }
    }
}
#endif

#endif

/**
 * @brief  find nm local map to channel index.
 * @param[in]    networkHandle   Identification of the NM-channel
 * @return       Nm_ChannelIndexType the index of channelConfiguration
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66721
 */
NM_LOCAL Nm_ChannelIndexType Nm_FindChannelIndex(NetworkHandleType networkHandle)
{
    Nm_ChannelIndexType chIndex = 0u;
    for (; chIndex < NM_NUMBER_OF_CHANNELS; ++chIndex)
    {
        if (networkHandle == Nm_ChLConfig[chIndex].ComMChannelRef)
        {
            break;
        }
    }
    return chIndex;
}

#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
/**
 * @brief  find cluster index the channel belongs to
 * @param[in]    chIndex   index of channel
 * @param[out]   clusterIndexPtr  the cluster index the channel belongs to
 * @return       Std_ReturnType
 * @retval       E_OK: No   find
 * @retval       E_NOT_OK:  not find
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66869
 */
NM_LOCAL Std_ReturnType Nm_FindClusterIndex(uint8 chIndex, uint8* clusterIndexPtr)
{
    Std_ReturnType findFlg = E_NOT_OK;
    if (NULL_PTR != Nm_Config.PerCluPtr)
    {
        for (uint8 index = 0u; (index < NM_NUMBER_OF_CLUSTERS) && ((uint8)E_NOT_OK == findFlg); index++)
        {
            if (Nm_ChLConfig[chIndex].CoordClusterIndex == Nm_Config.PerCluPtr[index].CoorClusterIndex)
            {
                findFlg          = E_OK;
                *clusterIndexPtr = index;
            }
        }
    }
    return findFlg;
}
#endif

#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
#if (NM_NUMBER_OF_CLUSTERS > 0u)
/**
 * @brief check if all channels in cluster meet the requirements
 * @param[in]    channelMask   bit mask of channel in cluster
 * @param[in]    clusterIndex the index of cluster
 * @return       Std_ReturnType
 * @retval       E_OK    this channel meet the requirements
 * @retval       E_NOT_OK   this channel do not meet the requirements
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
NM_LOCAL Std_ReturnType Nm_CheckChannelMaskInCluster(uint8 channelMask, uint8 clusterIndex)
{
    Std_ReturnType                 ret        = E_OK;
    const Nm_PerClusterConfigType* perClusPtr = &Nm_Config.PerCluPtr[clusterIndex];
    for (uint8 i = 0u; i < perClusPtr->IncNumCh; i++)
    {
        uint8 chIdx = perClusPtr->IncCh[i];
        if ((channelMask & (uint8)(1u << chIdx)) == 0u)
        {
            ret = E_NOT_OK;
            break;
        }
    }
    return ret;
}
#endif
/**
 * @brief  topmost shutdown ready CSRBit set,and not topmost CSRBit
 *         change handler
 * @param[in]    perClusPtr   Pointer to the current cluster
 * @param[in]    chIdx  the configured channel index
 * @param[in]    ReqCSRBitFlag  if topmost coorshutdown
 * @param[out]   cluserRsb  the readysleep Bit of passive coordinate channel
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66871
 */
NM_LOCAL void Nm_CoorReadySleepBitHandle(boolean* topMostCoor, uint8 chIdx, boolean* cluserRsb, boolean reqCSRBitFlag)
{
    boolean NeedHandle = FALSE;
    if (*topMostCoor)
    {
        if (reqCSRBitFlag)
        {
            *cluserRsb = TRUE;
            NeedHandle = TRUE;
        }
    }
    else
    {
        NeedHandle = TRUE;
        if (!Nm_ChLConfig[chIdx].ActiveCoordinator)
        {
            SchM_Enter_Nm_Coordinator();
            /*get passive channel CSRbit value*/
            *cluserRsb = Nm_InnerChannel[chIdx]->CoorRsb;

            if (Nm_InnerChannel[chIdx]->RemoteSleepIndFlg)
            {
                *cluserRsb = TRUE;
            }
            SchM_Exit_Nm_Coordinator();
        }
    }

    if (NeedHandle)
    {
        /*topmost set CSR=1 in active channel or not top most
         *forward CSR from passive channel to active channel*/
        if (Nm_ChLConfig[chIdx].ActiveCoordinator)
        {
            SchM_Enter_Nm_Coordinator();
            if (Nm_InnerChannel[chIdx]->CoorRsb != (*cluserRsb))
            {
                /*forward CSRBit in all active channel*/
                if (E_OK == Nm_BusNmSetSleepReadyBit(chIdx, *cluserRsb))
                {
                    Nm_InnerChannel[chIdx]->CoorRsb = *cluserRsb;
                }
            }
            SchM_Exit_Nm_Coordinator();
        }
    }
}
/**
 * @brief        the behavior of passively coordinated channel in nested cluster
 * @param[in]    chIdx  the configured channel index
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-70029
 */
NM_LOCAL void Nm_PassivelyCoordinatedChannelHandle(uint8 chIdx)
{
    const Nm_ChannelLConfigType* chCfgPtr     = &Nm_ChLConfig[chIdx];
    uint8                        clusterIndex = 0u;
    if (Nm_FindClusterIndex(chIdx, &clusterIndex) == E_OK)
    {
        if (!Nm_InnerCluster[clusterIndex].TopMostCoor)
        {
            Nm_PassivelyCoordinatedChannelRelease(clusterIndex, chIdx);
        }
        if (Nm_InnerChannel[chIdx]->LowLayNetRequest && !chCfgPtr->ActiveCoordinator
            && (Nm_CheckChannelMaskInCluster(
                    Nm_InnerCluster[clusterIndex].PassiveChannelReadySleepInChMask,
                    clusterIndex)
                == E_OK))
        {
            (void)Nm_BusNmNetworkRelease(chIdx);
        }
    }
}
/**
 * @brief        check if passive coordinated channel should be released
 * @param[in]    clusterIndex  the index of cluster
 * @param[in]    chIdx  the configured channel index
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-70030
 */
NM_LOCAL void Nm_PassivelyCoordinatedChannelRelease(uint8 clusterIndex, uint8 chIdx)
{
    const Nm_ChannelLConfigType* chCfgPtr = &Nm_ChLConfig[chIdx];
    SchM_Enter_Nm_Coordinator();
    Nm_InnerCluster[clusterIndex].PassiveChannelReadySleepInChMask |= (uint8)(1u << chIdx);
    SchM_Exit_Nm_Coordinator();
    if (Nm_InnerChannel[chIdx]->UpLayNetRequest)
    {
        SchM_Enter_Nm_Coordinator();
        Nm_InnerCluster[clusterIndex].PassiveChannelReadySleepInChMask &= (uint8)(~(uint8)(1u << chIdx));
        SchM_Exit_Nm_Coordinator();
    }
    else if (
        Nm_InnerChannel[chIdx]->BusAwakeFlg && !Nm_InnerChannel[chIdx]->RemoteSleepIndFlg && chCfgPtr->ActiveCoordinator
        && !chCfgPtr->ChannelSleepMaster)
    {
        SchM_Enter_Nm_Coordinator();
        Nm_InnerCluster[clusterIndex].PassiveChannelReadySleepInChMask &= (uint8)(~(uint8)(1u << chIdx));
        SchM_Exit_Nm_Coordinator();
    }
}
#endif

#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
/**
 * @brief        check alive channel in coordinator cluster,judge cluster
 *               is ready to coor shutdown or not
 * @param[in]    perClusPtr   Pointer to the current cluster
 * @param[in]    chIdx  the configured channel index
 * @param[out]   clusterReadySleepInChMask  the bit mask of channel,specify if all channels in this cluster are ready to
 * sleep
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66872
 */
NM_LOCAL void Nm_CoorShutdownReady(boolean* topMostCoor, uint8 chIdx, uint8* clusterReadySleepInChMask)
{
    const Nm_ChannelLConfigType* chCfgPtr = &Nm_ChLConfig[chIdx];
    const Nm_InnerChannelType*   chRTPtr  = Nm_InnerChannel[chIdx];

    SchM_Enter_Nm_Coordinator();
    *clusterReadySleepInChMask |= (uint8)(1u << chIdx);
    SchM_Exit_Nm_Coordinator();
    /* new check mode */
    /*  pre-condition,special bus is awake */
    if (chRTPtr->BusAwakeFlg)
    {
        if (chRTPtr->UpLayNetRequest)
        {
            SchM_Enter_Nm_Coordinator();
            *clusterReadySleepInChMask &= (uint8)(~(uint8)(1u << chIdx));
            SchM_Exit_Nm_Coordinator();
        }
        else
        {
            /* pre-condition: no sleep master
            next: active channel need check remote ind,
            but passive channel is not need remote ind
            next : coor rsb,
            */
            if ((!chCfgPtr->ChannelSleepMaster &&
#if (STD_ON == NM_COORDINATOR_SYNC_SUPPORT)
                 chCfgPtr->ActiveCoordinator &&
#endif
                 !chRTPtr->RemoteSleepIndFlg)
#if (STD_ON == NM_COORDINATOR_SYNC_SUPPORT)
                || (!(*topMostCoor) && chCfgPtr->ActiveCoordinator && !chRTPtr->CoorRsb)
#endif
            )
            {
                SchM_Enter_Nm_Coordinator();
                *clusterReadySleepInChMask &= (uint8)(~(uint8)(1u << chIdx));
                SchM_Exit_Nm_Coordinator();
            }
#if (STD_ON == NM_COORDINATOR_SYNC_SUPPORT)
            if (chRTPtr->RemoteSleepIndFlg)
            {
                if ((*topMostCoor) && (chCfgPtr->BusNmType != NM_BUSNM_LOCALNM))
                {
                    /* sync network not recv lowlayer sync ind */
                    if (chCfgPtr->SynchronizingNetwork && !chRTPtr->LowLayBusSyncPoint)
                    {
                        /*not ready to coor shutdown*/
                        SchM_Enter_Nm_Coordinator();
                        *clusterReadySleepInChMask &= (uint8)(~(uint8)(1u << chIdx));
                        SchM_Exit_Nm_Coordinator();
                    }
                }
            }
#endif
        }
    }
    else
    {
        if (chRTPtr->UpLayNetRequest)
        {
            SchM_Enter_Nm_Coordinator();
            *clusterReadySleepInChMask &= (uint8)(~(uint8)(1u << chIdx));
            SchM_Exit_Nm_Coordinator();
        }
    }
}
#endif

#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
/**
 * @brief        coordinator shutdown timer load default value.
 * @param[in]    perClusPtr   Pointer to the current cluster
 * @param[in]    chIdx  the configured channel index
 * @param[out]   shutdownTimerLoadOkInChMask  the bit mask of channel,specify if all channels in this cluster load
 * default value
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66874
 */
NM_LOCAL void Nm_CoorShutdownTimerLoader(
    const Nm_PerClusterConfigType* perClusPtr,
    uint8                          chIdx,
    uint8*                         shutdownTimerLoadOkInChMask)
{
    Std_ReturnType nextFlag = E_OK;
    /*SyncNetFlg is generated by the configuration*/
    if (perClusPtr->SyncNetFlg)
    {
        /* @SWS_Nm_00172 Synchronou network shall be delay until sync point */
        if (Nm_InnerChannel[chIdx]->BusAwakeFlg && Nm_ChLConfig[chIdx].SynchronizingNetwork
            && !Nm_InnerChannel[chIdx]->LowLayBusSyncPoint)
        {
            *shutdownTimerLoadOkInChMask &= (uint8)(~(uint8)(1u << chIdx));
            nextFlag = E_NOT_OK;
        }
    }
    if (E_OK == nextFlag)
    {
        *shutdownTimerLoadOkInChMask |= (uint8)(1u << chIdx);
        Nm_InnerChannelType* chRTPtr = Nm_InnerChannel[chIdx];
        if (chRTPtr->BusAwakeFlg)
        {
            /* @SWS_Nm_00175 reload ShutdownTimer */
            SchM_Enter_Nm_Coordinator();
            chRTPtr->AllowShutdown = TRUE;
            SchM_Exit_Nm_Coordinator();
            if ((NM_GLOBAL_COORDINATOR_TIME < Nm_ChLConfig[chIdx].BusShutdownTime)
#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
                || !Nm_ChLConfig[chIdx].ActiveCoordinator
#endif
            )
            {
                chRTPtr->ShutdownTimer = 0u;
            }
            else
            {
                /*@req SWS_Nm_00175*/
                if (Nm_ChLConfig[chIdx].BusNmType == NM_BUSNM_LOCALNM)
                {
                    chRTPtr->ShutdownTimer = NM_GLOBAL_COORDINATOR_TIME;
                }
                else
                {
                    chRTPtr->ShutdownTimer =
                        (Nm_ShutdownTimerType)NM_GLOBAL_COORDINATOR_TIME - Nm_ChLConfig[chIdx].BusShutdownTime;
                }
            }
        }
    }
}
#endif

#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
/**
 * @brief        coordinator shutdown timer handle function
 * @param[in]    chIdx  the configured channel index
 * @param[out]   sleepAllowedInChMask  the bit mask of channel,specify if all channels in this cluster are asleep
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66876
 */
NM_LOCAL Std_ReturnType Nm_CoorShutDownTimerHandle(uint8 chIdx, uint8* sleepAllowedInChMask)
{
    Std_ReturnType ret           = E_OK;
    boolean        sleepAllowFlg = TRUE;
    /* check sync network point */
    Nm_InnerChannelType* chRTPtr = Nm_InnerChannel[chIdx];
    if (chRTPtr->BusAwakeFlg)
    {
        sleepAllowFlg = FALSE;
        if (chRTPtr->ShutdownTimer > 0u)
        {
            chRTPtr->ShutdownTimer--;
        }
        /* @SWS_Nm_00176
        timer expiers for network ,whill be call busNetRelease */
        if ((chRTPtr->ShutdownTimer == 0u) && chRTPtr->AllowShutdown)
        {
            SchM_Enter_Nm_Coordinator();
            chRTPtr->AllowShutdown = FALSE;
            SchM_Exit_Nm_Coordinator();

            if (Nm_ChLConfig[chIdx].BusNmType == NM_BUSNM_LOCALNM)
            {
                /*@req SWS_Nm_00176*/
                ComM_Nm_BusSleepMode(Nm_ChLConfig[chIdx].ComMChannelRef);
                chRTPtr->LowLayBusSyncPoint = FALSE;
                chRTPtr->BusAwakeFlg        = FALSE;
                chRTPtr->RemoteSleepIndFlg  = TRUE;
                chRTPtr->LowLayNetRequest   = FALSE;
            }
            else
            {
#if (NM_BUS_SYNCHRONIZATION_ENABLED == STD_ON)
                (void)Nm_BusNmRequestBusSynchronization(chIdx);
#endif
                if (chRTPtr->LowLayNetRequest)
                {
                    ret = Nm_BusNmNetworkRelease(chIdx);
                }
            }
        }
    }
    *sleepAllowedInChMask &= (uint8)(~(uint8)(1u << chIdx));
    if (sleepAllowFlg)
    {
        *sleepAllowedInChMask |= (uint8)(1u << chIdx);
    }
    return ret;
}
#endif

#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
/**
 * @brief        coordinator shutdown abort handle function
 * @param[in]    chIdx  the index of channel
 * @param[out]   shutdownAbortFinishedInChMask  the bit mask of channel,specify if all channels in this cluster finish
 * the abortion
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66879
 */
NM_LOCAL void Nm_CoorShutdownAbortHandle(uint8 chIdx, uint8* shutdownAbortFinishedInChMask)
{
    const Nm_ChannelLConfigType* chCfgPtr = &Nm_ChLConfig[chIdx];
    Nm_InnerChannelType*         chRTPtr  = Nm_InnerChannel[chIdx];
    if (chRTPtr->BusAwakeFlg)
    {
        if (!chRTPtr->LowLayNetRequest)
        {
            chRTPtr->AllowShutdown = FALSE;
            /*@req SWS_Nm_00183*/
            if (chCfgPtr->BusNmType == NM_BUSNM_LOCALNM)
            {
                ComM_Nm_NetworkMode(chCfgPtr->ComMChannelRef);
            }
            else
            {
                (void)Nm_BusNmNetworkRequest(chIdx);
            }
        }
    }
    else
    {
        ComM_Nm_RestartIndication(chCfgPtr->ComMChannelRef);
    }
#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
    /* call low api to set coor Ready sleep bit in all active channel,
    passive channel only change local ver */
    if (!chCfgPtr->ActiveCoordinator)
    {
        chRTPtr->CoorRsb = FALSE;
    }
    else
    {
        if (Nm_BusNmSetSleepReadyBit(chIdx, FALSE) == (uint8)E_OK)
        {
            chRTPtr->CoorRsb = FALSE;
        }
    }
#endif
    *shutdownAbortFinishedInChMask |= (uint8)(1u << chIdx);
}
#endif

#if (NM_IF_ALL_CHANNEL_PASSIVE_MODE_ENABLED == STD_OFF)
/**
 * @brief        special bus nm network request
 * @param[in]    chIdx  the index of channel
 * @return       Std_ReturnType
 * @retval       E_OK: No   successfully request
 * @retval       E_NOT_OK:  request failed
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66881
 */
NM_LOCAL Std_ReturnType Nm_BusNmNetworkRequest(uint8 chIndex)
{
    Std_ReturnType                    ret         = E_NOT_OK;
    const Nm_SpecificLowLayerApiType* busNmApiPtr = Nm_ChLConfig[chIndex].BusNmApi;
    if (busNmApiPtr != NULL_PTR)
    {
        const BusNm_NetworkRequest_Type specific_NetworkRequest = busNmApiPtr->BusNm_NetworkRequest;
        if (specific_NetworkRequest != NULL_PTR)
        {
            ret = specific_NetworkRequest(Nm_ChLConfig[chIndex].ComMChannelRef);
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
            Nm_InnerChannel[chIndex]->LowLayNetRequest = TRUE;
            if (ret != (uint8)E_OK)
            {
                /* @SWS_Nm_00235
                if BusNm_NetworkRequest return E_NOT_OK,that network shall not be
                considered awake */
                Nm_InnerChannel[chIndex]->BusAwakeFlg = FALSE;
            }
#endif
        }
    }
    return ret;
}
#endif

#if (NM_IF_ALL_CHANNEL_PASSIVE_MODE_ENABLED == STD_OFF)
/**
 * @brief        special bus network release
 * @param[in]    chIdx  Configured channel index
 * @return       Std_ReturnType
 * @retval       E_OK: No   successfully release
 * @retval       E_NOT_OK:  release failed
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66882
 */
NM_LOCAL Std_ReturnType Nm_BusNmNetworkRelease(uint8 chIndex)
{
    Std_ReturnType                    ret         = E_NOT_OK;
    const Nm_SpecificLowLayerApiType* busNmApiPtr = Nm_ChLConfig[chIndex].BusNmApi;
    if (busNmApiPtr != NULL_PTR)
    {
        const BusNm_NetworkRequest_Type specific_NetworkRelease = busNmApiPtr->BusNm_NetworkRelease;
        if (specific_NetworkRelease != NULL_PTR)
        {
            ret = specific_NetworkRelease(Nm_ChLConfig[chIndex].ComMChannelRef);
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
            Nm_InnerChannel[chIndex]->LowLayNetRequest = FALSE;
#endif
        }
    }
    return ret;
}
#endif

#if ((NM_COORDINATOR_SUPPORT_ENABLED == STD_ON) && (NM_BUS_SYNCHRONIZATION_ENABLED == STD_ON))
/**
 * @brief        special bus nm request sync
 * @param[in]    chIdx  Configured channel index
 * @return       Std_ReturnType
 * @retval       E_OK: No   success
 * @retval       E_NOT_OK:  request
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66884
 */
NM_LOCAL Std_ReturnType Nm_BusNmRequestBusSynchronization(uint8 chIndex)
{
    Std_ReturnType                    ret         = E_NOT_OK;
    const Nm_SpecificLowLayerApiType* busNmApiPtr = Nm_ChLConfig[chIndex].BusNmApi;
    if (busNmApiPtr != NULL_PTR)
    {
        const BusNm_RequestBusSynchronization_Type specific_RequestBusSynchronization =
            busNmApiPtr->BusNm_RequestBusSynchronization;
        if (specific_RequestBusSynchronization != NULL_PTR)
        {
            ret = specific_RequestBusSynchronization(Nm_ChLConfig[chIndex].ComMChannelRef);
        }
    }
    return ret;
}
#endif

#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
/**
 * @brief        special bus nm set coordinator ready sleep bit
 * @param[in]    chIdx   Configured channel index
 * @param[in]    nmSleepReadyBit  value of the SleepReadyBit
 * @return       Std_ReturnType
 * @retval       E_OK: No   successfully set
 * @retval       E_NOT_OK:  set failed
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66885
 */
NM_LOCAL Std_ReturnType Nm_BusNmSetSleepReadyBit(uint8 chIndex, boolean nmSleepReadyBit)
{
    Std_ReturnType                    ret         = E_NOT_OK;
    const Nm_SpecificLowLayerApiType* busNmApiPtr = Nm_ChLConfig[chIndex].BusNmApi;
    if (busNmApiPtr != NULL_PTR)
    {
        const BusNm_SetSleepReadyBit_Type specific_SetSleepReadyBit = busNmApiPtr->BusNm_SetSleepReadyBit;
        if (specific_SetSleepReadyBit != NULL_PTR)
        {
            ret = specific_SetSleepReadyBit(Nm_ChLConfig[chIndex].ComMChannelRef, nmSleepReadyBit);
        }
    }

    if (Nm_ChLConfig[chIndex].BusNmType == NM_BUSNM_LOCALNM)
    {
        ret = E_OK;
    }
    return ret;
}
#endif

#if NM_DEV_ERROR_DETECT == STD_ON

/**
 * @brief Development error validation of invalid partition context
 * @param[in]    apiId  ID of API service in which error is detected
 * @return       boolean
 * @retval       TRUE   Development error not occurred
 * @retval       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66934
 */
NM_LOCAL boolean Nm_ValidatePartitionContext(uint8 apiId)
{
    if (Nm_GetPartitionIndex() >= NM_PARTITION_NUMBER)
    {
        (void)Det_ReportError(NM_MODULE_ID, NM_INSTANCE_ID, apiId, NM_E_INVALID_PARTITION_CONTEXT);
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief Development error validation of uninit
 * @param[in]    apiId  ID of API service in which error is detected
 * @return       boolean
 * @retval       TRUE   Development error not occurred
 * @retval       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66933
 */
NM_LOCAL boolean Nm_ValidateInitStatus(uint8 apiId)
{
    boolean ret = TRUE;
    if (Nm_InitStatus[Nm_GetPartitionIndex()] != NM_INIT)
    {
        (void)Det_ReportError(NM_MODULE_ID, NM_INSTANCE_ID, apiId, NM_E_UNINIT);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief Development error validation of already initialized
 * @retval       TRUE   Development error not occurred
 * @retval       FALSE  Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        -
 */
NM_LOCAL boolean Nm_ValidateAlreadyInitialized(void)
{
    boolean ret = TRUE;
    if (Nm_InitStatus[Nm_GetPartitionIndex()] == NM_INIT)
    {
        (void)Det_ReportError(NM_MODULE_ID, NM_INSTANCE_ID, NM_SERVICE_ID_INIT, NM_E_ALREADY_INITIALIZED);
        ret = FALSE;
    }
    return ret;
}
/**
 * @brief   validate if the channel belongs to configuration.And check if passive channel calls Nm API.
 * @param[in]           apiId          the id of API
 * @param[in]           networkHandle  Identification of the NM-channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-59688
 */
/******************************************************************************/
NM_LOCAL boolean Nm_ValidateNetworkHandle(uint8 apiId, NetworkHandleType networkHandle)
{
    uint8                        error    = NM_E_NO_ERROR;
    Nm_ChannelIndexType          chIndex  = Nm_FindChannelIndex(networkHandle);
    const Nm_ChannelLConfigType* chCfgPtr = &Nm_ChLConfig[chIndex];

    if (chIndex >= NM_NUMBER_OF_CHANNELS)
    {
        error = NM_E_INVALID_CHANNEL;
    }
#if NM_MULTIPLE_PARTITION_USED == STD_ON
    else if (GetApplicationID() != NM_GET_PARTITION_OF_CHANNEL(chIndex))
    {
        error = NM_E_INVALID_CHANNEL;
    }
#endif
#if (NM_PASSIVE_MODE_ENABLED == STD_ON)
    else if (chCfgPtr->PassiveModeEnabled)
    {
        if ((apiId == NM_SERVICE_ID_NETWORKREQUEST) || (apiId == NM_SERVICE_ID_NETWORKRELEASE)
            || (apiId == NM_SERVICE_ID_DISABLECOMMUNICATION) || (apiId == NM_SERVICE_ID_ENABLECOMMUNICATION)
            || (apiId == NM_SERVICE_ID_SETUSERDATA) || (apiId == NM_SERVICE_ID_REPEATMESSAGEREQUEST)
            || (apiId == NM_SERVICE_ID_CHECKREMOTESLEEPINDICATION))
        {
            error = NM_E_INVALID_CHANNEL;
        }
    }
#endif
    if (error != NM_E_NO_ERROR)
    {
        (void)Det_ReportError(NM_MODULE_ID, NM_INSTANCE_ID, apiId, error);
    }

    return error == NM_E_NO_ERROR;
}
/**
 * @brief   validate if the pointer is NULL.
 * @param[in]           apiId          the id of API
 * @param[in]           pointer        pointer to be validated
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-66936
 */
/******************************************************************************/
NM_LOCAL boolean Nm_ValidatePointer(uint8 apiId, const void* pointer)
{
    boolean valid = TRUE;

    if (pointer == NULL_PTR)
    {
        valid = FALSE;
        (void)Det_ReportError(NM_MODULE_ID, NM_INSTANCE_ID, apiId, NM_E_PARAM_POINTER);
    }
    return valid;
}
/**
 * @brief               validate the networkHandle and if the pointer is NULL.
 * @param[in]           apiId          the id of API
 * @param[in]           networkHandle  Identification of the NM-channel
 * @param[in]           pointer        pointer to be validated
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-66937
 */
NM_LOCAL boolean Nm_ValidateNetworkHandleAndPointer(uint8 apiId, NetworkHandleType networkHandle, const void* pointer)
{
    return Nm_ValidatePartitionContext(apiId) && Nm_ValidateInitStatus(apiId)
           && Nm_ValidateNetworkHandle(apiId, networkHandle) && Nm_ValidatePointer(apiId, pointer);
}
/**
 * @brief               validate the networkHandle and if the two pointers is NULL.
 * @param[in]           apiId          the id of API
 * @param[in]           networkHandle  Identification of the NM-channel
 * @param[in]           pointer1       first pointer to be validated
 * @param[in]           pointer2       second pointer to be validated
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-66939
 */
NM_LOCAL boolean Nm_ValidateNetworkHandlePointerAndPointer(
    uint8             apiId,
    NetworkHandleType networkHandle,
    const void*       pointer1,
    const void*       pointer2)
{
    return Nm_ValidateNetworkHandleAndPointer(apiId, networkHandle, pointer1) && Nm_ValidatePointer(apiId, pointer2);
}
#endif

/**
 * @brief        Returns the index of current partition in the Nm configuration.
 * @retval       the index of partition
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66895
 */
NM_LOCAL uint8 Nm_GetPartitionIndex(void)
{
    uint8 partitionIndex = 0u;
#if NM_MULTIPLE_PARTITION_USED == STD_ON
    ApplicationType application = GetApplicationID();
    for (; partitionIndex < NM_PARTITION_NUMBER; ++partitionIndex)
    {
        if (Nm_OsApplications[partitionIndex] == application)
        {
            break;
        }
    }
#endif
    return partitionIndex;
}

#if (NM_PN_ERA_CALC_ENABLED == STD_ON) || (NM_PN_EIRA_CALC_ENABLED == STD_ON)
/**
 * @brief  reset the timer of EIRA and ERA,and aggreagate the ERA and EIRA.
 * @param[in]    chIndex  index of channel
 * @param[in]    eraFlg   if detecte the change of ERA
 * @param[in]    rawData  Raw data
 * @return       boolean
 * @retval       TRUE   there is pnc bit in RawData which is related to this channel
 * @retval       FALSE  there is no pnc bit in RawData which is related to this channel
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-66718
 */
NM_LOCAL boolean Nm_PncAggregateAndTimerHandle(Nm_ChannelIndexType chIndex, boolean eraFlg, const uint8* rawData)
{
    const Nm_ChannelLConfigType*    chCfgPtr    = &Nm_ChLConfig[chIndex];
    const Nm_PncRelatedRunTimeType* chGRTPtr    = &Nm_ChannelPncRelatedRuntimeRef[chIndex];
    boolean                         relevantPnc = FALSE;

#if (NM_PN_ERA_CALC_ENABLED == STD_ON)
    uint16 pnIdx = 0u;
#endif

    for (uint8 byteIdx = 0u; byteIdx < chCfgPtr->PncBitVectorLength; byteIdx++)
    {
        uint8 bitIdx = 0u;
        uint8 pnMask = chCfgPtr->PnFilterMaskBytePtr[byteIdx].PnFilterMaskByteValue;
        /* Handle ERA and EIRA external request Timer*/
        for (bitIdx = 0u; bitIdx < NM_SINGLEBYTE_BASENUM; bitIdx++)
        {
            uint8 bitMask = 1u << bitIdx;
            if ((uint8)(bitMask & pnMask) != 0u)
            {
                if ((bitMask & rawData[byteIdx]) != 0u)
                {
                    /* rx pdu pninfo bit 0--->1 */
#if (NM_PN_ERA_CALC_ENABLED == STD_ON)
                    if (eraFlg)
                    {
                        chGRTPtr->EraTimerInChannel[pnIdx] = Nm_GlobalLConfig.PnResetTime;
                        if ((uint8)(bitMask & chGRTPtr->EraInChannel[byteIdx]) == 0u)
                        {
                            Nm_InnerChannel[chIndex]->UpDateEraFlg = TRUE;
                            chGRTPtr->EraInChannel[byteIdx] |= bitMask;
                        }
                    }
#endif
#if (NM_PN_EIRA_CALC_ENABLED == STD_ON)
                    Nm_InnerGlobal.PnEiraResetTimer[bitIdx + (byteIdx * NM_SINGLEBYTE_BASENUM)] =
                        Nm_GlobalLConfig.PnResetTime;
                    if ((uint8)(bitMask & Nm_InnerGlobal.PnEiraInfo[byteIdx]) == 0u)
                    {
                        Nm_InnerGlobal.UpDateEiraFlg = TRUE;
                        Nm_InnerGlobal.PnEiraInfo[byteIdx] |= bitMask;
                    }
#endif
                    relevantPnc = TRUE;
                }

#if (NM_PN_ERA_CALC_ENABLED == STD_ON)
                pnIdx += (eraFlg) ? 1u : 0u;
#endif
            }
        }
    }
    return relevantPnc;
}
#endif
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON) || (NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON)
/**
 * @brief        initialization of runtime variables related to channels.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-69583
 */
NM_LOCAL void Nm_InitChannelRuntimeVariables(void)
{
    for (uintx index = 0u; index < NM_NUMBER_OF_CHANNELS; index++)
    {
#if NM_MULTIPLE_PARTITION_USED == STD_ON
        ApplicationType curPartitionId = GetApplicationID();
        if (curPartitionId != NM_GET_PARTITION_OF_CHANNEL(index))
        {
            continue;
        }
#endif

#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
        /*coordinate related initial*/
        Nm_InnerChannel[index]->RemoteSleepIndFlg = TRUE;
        Nm_InnerChannel[index]->BusAwakeFlg       = FALSE;
        Nm_InnerChannel[index]->LowLayNetRequest  = FALSE;
        Nm_InnerChannel[index]->UpLayNetRequest   = FALSE;
        Nm_InnerChannel[index]->AllowShutdown     = FALSE;
#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
        Nm_InnerChannel[index]->CoorRsb = FALSE;
#endif
        Nm_InnerChannel[index]->LowLayBusSyncPoint = FALSE;
        Nm_InnerChannel[index]->ShutdownTimer      = 0u;
#endif

#if (NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON)
        /*pnc related initial*/
        const Nm_ChannelLConfigType*    chCfgPtr = &Nm_ChLConfig[index];
        const Nm_PncRelatedRunTimeType* chGRTPtr = &Nm_ChannelPncRelatedRuntimeRef[index];
        Nm_InnerChannelType*            chRTPtr  = Nm_InnerChannel[index];
        if (chCfgPtr->PnEnabled)
        {
            chRTPtr->UpDateEraFlg = FALSE;

#if (NM_PN_ERA_CALC_ENABLED == STD_ON)
            if (chCfgPtr->PnEraCalcEnabled)
            {
                /* Clear pn  buffer */
                for (uint8 loop = 0u; loop < chCfgPtr->PncBitVectorLength; loop++)
                {
                    chGRTPtr->EraInChannel[loop] = 0x0u;
                    chGRTPtr->IraInChannel[loop] = 0x0u;
                }

                /* Clear pn ERA reset timer */
                for (uint8 loop = 0u; loop < chCfgPtr->PnEraNum; loop++)
                {
                    chGRTPtr->EraTimerInChannel[loop] = 0x0u;
                }
            }
#endif
        }
#endif
    }
}
#endif
/**
 * @brief        The coordinator processing function when the shutdown status is NM_SHUTDOWN_STU_START.
 * @param[in]    chIdx          the index of channel
 * @param[in]    clusterIndex          the index of coordinate cluster
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-69584
 */
#if ((NM_COORDINATOR_SUPPORT_ENABLED == STD_ON) && (0u < NM_NUMBER_OF_CLUSTERS))

NM_LOCAL void Nm_ShutdownStatusStartHandle(uint8 chIdx, uint8 clusterIndex)
{
    const Nm_PerClusterConfigType* perClusPtr         = &Nm_Config.PerCluPtr[clusterIndex];
    Nm_ShutdownStatusType*         ShutdownStatus     = &Nm_InnerCluster[clusterIndex].ShutdownStatus;
    boolean*                       CoorRsbInCluster   = &Nm_InnerCluster[clusterIndex].CoorRsbInCluster;
    uint8*                         ReadySleepInChMask = &Nm_InnerCluster[clusterIndex].ReadySleepInChMask;
    uint8*   ShutdownTimerLoadOkInChMask              = &Nm_InnerCluster[clusterIndex].ShutdownTimerLoadOkInChMask;
    boolean* topMostCoor                              = &Nm_InnerCluster[clusterIndex].TopMostCoor;
    /*handle not topmost CSRBit change*/
#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
    /*handle not topmost CSRBit change*/
    Nm_CoorReadySleepBitHandle(topMostCoor, chIdx, CoorRsbInCluster, FALSE);
#endif
    /*all channel not in alive,shutdown ready*/
    Nm_CoorShutdownReady(topMostCoor, chIdx, ReadySleepInChMask);
    if (Nm_CheckChannelMaskInCluster(*ReadySleepInChMask, clusterIndex) == E_OK)
    {
#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
        /*shutdown ready set topmost active channel CSRBit = 1*/
        Nm_CoorReadySleepBitHandle(topMostCoor, chIdx, CoorRsbInCluster, TRUE);
#endif
        /*start CSR=1 channel's coordinate shutdown,start
         shutdown timer*/
        Nm_CoorShutdownTimerLoader(perClusPtr, chIdx, ShutdownTimerLoadOkInChMask);
        if (Nm_CheckChannelMaskInCluster(*ShutdownTimerLoadOkInChMask, clusterIndex) == E_OK)
        {
            *ShutdownStatus = NM_SHUTDOWN_STU_RUN;
        }
    }
}
/**
 * @brief        The coordinator processing function when the shutdown status is NM_SHUTDOWN_STU_RUN.
 * @param[in]    chIdx          the index of channel
 * @param[in]    clusterIndex          the index of coordinate cluster
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-69585
 */
NM_LOCAL void Nm_ShutdownStatusRunHandle(uint8 chIdx, uint8 clusterIndex)
{
    const Nm_PerClusterConfigType* perClusPtr           = &Nm_Config.PerCluPtr[clusterIndex];
    Nm_ShutdownStatusType*         ShutdownStatus       = &Nm_InnerCluster[clusterIndex].ShutdownStatus;
    uint8*                         ReadySleepInChMask   = &Nm_InnerCluster[clusterIndex].ReadySleepInChMask;
    uint8*                         SleepAllowedInChMask = &Nm_InnerCluster[clusterIndex].SleepAllowedInChMask;
    boolean*                       topMostCoor          = &Nm_InnerCluster[clusterIndex].TopMostCoor;
    Nm_CoorShutdownReady(topMostCoor, chIdx, ReadySleepInChMask);
    if (Nm_CheckChannelMaskInCluster(*ReadySleepInChMask, clusterIndex) == E_OK)
    {
        if (Nm_CoorShutDownTimerHandle(chIdx, SleepAllowedInChMask) == E_NOT_OK)
        {
            *ShutdownStatus = NM_SHUTDOWN_STU_ABORT;
        }
        else if (Nm_CheckChannelMaskInCluster(*SleepAllowedInChMask, clusterIndex) == E_OK)
        {
            *ShutdownStatus = NM_SHUTDOWN_STU_STOP;
        }
    }
}
/**
 * @brief        The coordinator processing function when the shutdown status is NM_SHUTDOWN_STU_ABORT.
 * @param[in]    chIdx          the index of channel
 * @param[in]    clusterIndex          the index of coordinate cluster
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-69586
 */
NM_LOCAL void Nm_ShutdownStatusAbortHandle(uint8 chIdx, uint8 clusterIndex)
{
    uint8* ShutdownAbortFinishedInChMask = &Nm_InnerCluster[clusterIndex].ShutdownAbortFinishedInChMask;
    Nm_CoorShutdownAbortHandle(chIdx, ShutdownAbortFinishedInChMask);
    if (Nm_CheckChannelMaskInCluster(*ShutdownAbortFinishedInChMask, clusterIndex) == E_OK)
    {
        Nm_InnerCluster[clusterIndex].ShutdownStatus                   = NM_SHUTDOWN_STU_START;
        Nm_InnerCluster[clusterIndex].ReadySleepInChMask               = 0u;
        Nm_InnerCluster[clusterIndex].ShutdownTimerLoadOkInChMask      = 0u;
        Nm_InnerCluster[clusterIndex].SleepAllowedInChMask             = 0u;
        Nm_InnerCluster[clusterIndex].ShutdownAbortFinishedInChMask    = 0u;
        Nm_InnerCluster[clusterIndex].PassiveChannelReadySleepInChMask = 0u;
    }
}
/**
 * @brief        The coordinator processing Main function.
 * @param[in]    chIdx          the index of channel
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-69587
 */
NM_LOCAL void Nm_Coordinator_MainHandle(uint8 chIdx)
{
    uint8 clusterIndex;
    if (Nm_FindClusterIndex(chIdx, &clusterIndex) == E_OK)
    {
        const Nm_ShutdownStatusType* ShutdownStatus = &Nm_InnerCluster[clusterIndex].ShutdownStatus;
        if (*ShutdownStatus == NM_SHUTDOWN_STU_START)
        {
            Nm_ShutdownStatusStartHandle(chIdx, clusterIndex);
        }
        if (*ShutdownStatus == NM_SHUTDOWN_STU_RUN)
        {
            Nm_ShutdownStatusRunHandle(chIdx, clusterIndex);
        }
        if (*ShutdownStatus == NM_SHUTDOWN_STU_ABORT)
        {
            Nm_ShutdownStatusAbortHandle(chIdx, clusterIndex);
        }
    }
}
#endif

#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
/* PRQA S 1503, 1532 -- */
