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
 **  @file               : Nm.h
 **  @author             : Wanglili
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : API and type definitions of Nm
 **
 ***********************************************************************************************************************/

/** ===================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date       Author         Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V2.1.0    2024-11-12 shuangyang.fu
 *      1. Support multi-partition distribution.
 *  V2.1.1    2025-04-30 caihong.liu
 *      CPT-11890: Add the scenario where the upstream gateway of the relay coordination gateway goes offline.
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Nm<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_QAC_2991
      Reason:The value of this 'if' controlling expression is always 'true'.
      Risk:No risk.
      Prevention:No need to check.
    \li VL_Nm_3218
      Reason:The initialization state should not be masked as it is used to indicate the module's initialization status.
      Risk:No risk.
      Prevention:No need to check.
    \li VL_Nm_3229
      Reason:The initialization state should not be masked as it is used to indicate the module's initialization status.
      Risk:No risk.
      Prevention:No need to check.

 */
#ifndef NM_H_
#define NM_H_

/* =================================================== inclusions =================================================== */
#include "Nm_Cfg.h"
#include "Nm_Internal.h"
#include "ComStack_Types.h"
#include "NmStack_Types.h"
#if NM_STATE_CHANGE_IND_ENABLED == STD_ON
#include "Com.h"
#endif
/* =============================================== version information ============================================== */
/** Published Information */
#define NM_VENDOR_ID                   ((uint16)(62u))
#define NM_MODULE_ID                   ((uint16)(29u))
#define NM_INSTANCE_ID                 ((uint8)(0u))
#define NM_AR_RELEASE_MAJOR_VERSION    ((uint8)(4u))
#define NM_AR_RELEASE_MINOR_VERSION    ((uint8)(9u))
#define NM_AR_RELEASE_REVISION_VERSION ((uint8)(0u))
#define NM_SW_MAJOR_VERSION            ((uint8)(2u))
#define NM_SW_MINOR_VERSION            ((uint8)(1u))
#define NM_SW_PATCH_VERSION            ((uint8)(1u))
/* ===================================================== macros ===================================================== */
/** Error classification */
#define NM_E_UNINIT                    ((uint8)(0x00u))
#define NM_E_INVALID_CHANNEL           ((uint8)(0x01u))
#define NM_E_PARAM_POINTER             ((uint8)(0x02u))
#define NM_E_ALREADY_INITIALIZED       ((uint8)(0x03u))
#define NM_E_INVALID_PARTITION_CONTEXT ((uint8)(0x0Fu))
#define NM_E_NO_ERROR                  ((uint8)(0xffu))

/** Service ID */
#define NM_SERVICE_ID_INIT                          ((uint8)0x00u)
#define NM_SERVICE_ID_PASSIVESTARTUP                ((uint8)0x01u)
#define NM_SERVICE_ID_NETWORKREQUEST                ((uint8)0x02u)
#define NM_SERVICE_ID_NETWORKRELEASE                ((uint8)0x03u)
#define NM_SERVICE_ID_DISABLECOMMUNICATION          ((uint8)0x04u)
#define NM_SERVICE_ID_ENABLECOMMUNICATION           ((uint8)0x05u)
#define NM_SERVICE_ID_SETUSERDATA                   ((uint8)0x06u)
#define NM_SERVICE_ID_GETUSERDATA                   ((uint8)0x07u)
#define NM_SERVICE_ID_GETPDUDATA                    ((uint8)0x08u)
#define NM_SERVICE_ID_REPEATMESSAGEREQUEST          ((uint8)0x09u)
#define NM_SERVICE_ID_GETNODEIDENTIFIER             ((uint8)0x0au)
#define NM_SERVICE_ID_GETLOCALNODEIDENTIFIER        ((uint8)0x0bu)
#define NM_SERVICE_ID_CHECKREMOTESLEEPINDICATION    ((uint8)0x0du)
#define NM_SERVICE_ID_GETSTATE                      ((uint8)0x0eu)
#define NM_SERVICE_ID_GETVERSIONINFO                ((uint8)0x0fu)
#define NM_SERVICE_ID_NETWORKSTARTINDICATION        ((uint8)0x11u)
#define NM_SERVICE_ID_NETWORKMODE                   ((uint8)0x12u)
#define NM_SERVICE_ID_BUSSLEEPMODE                  ((uint8)0x14u)
#define NM_SERVICE_ID_PREPAREBUSSLEEPMODE           ((uint8)0x13u)
#define NM_SERVICE_ID_REMOTESLEEPINDICATION         ((uint8)0x17u)
#define NM_SERVICE_ID_REMOTESLEEPCANCELLATION       ((uint8)0x18u)
#define NM_SERVICE_ID_SYNCHRONIZATIONPOINT          ((uint8)0x19u)
#define NM_SERVICE_ID_COORDREADYTOSLEEPINDICATION   ((uint8)0x1eu)
#define NM_SERVICE_ID_COORDREADYTOSLEEPCANCELLATION ((uint8)0x1fu)
#define NM_SERVICE_ID_PDURXINDICATION               ((uint8)0x15u)
#define NM_SERVICE_ID_STATECHANGENOTIFICATION       ((uint8)0x16u)
#define NM_SERVICE_ID_REPEATMESSAGEINDICATION       ((uint8)0x1au)
#define NM_SERVICE_ID_TXTIMEOUTEXCEPTION            ((uint8)0x1bu)
#define NM_SERVICE_ID_CARWAKEUPINDICATION           ((uint8)0x1du)
#define NM_SERVICE_ID_MAINFUNCTION                  ((uint8)0x10u)
#define NM_SERVICE_ID_SYNCHRONIZEMODE               ((uint8)0x21u)
#define NM_SERVICE_ID_UPDATEIRA                     ((uint8)0x26u)
#define NM_SERVICE_ID_PNCBITVECTORTXINDICATION      ((uint8)0x27u)
#define NM_SERVICE_ID_PNCBITVECTORRXINDICATION      ((uint8)0x28u)

/* ================================================ type definitions ================================================ */
typedef uint8 Nm_ChannelIndexType;

#if (NM_IF_ALL_CHANNEL_PASSIVE_MODE_ENABLED == STD_OFF)
/* <BusNm>_NetworkRequest */
typedef Std_ReturnType (*BusNm_NetworkRequest_Type)(NetworkHandleType nmNetworkHandle);
/* <BusNm>_NetworkRelease */
typedef Std_ReturnType (*BusNm_NetworkRelease_Type)(NetworkHandleType nmNetworkHandle);
#endif

/* <BusNm>_PassiveStartUp function */
typedef Std_ReturnType (*BusNm_PassiveStartUp_Type)(NetworkHandleType nmNetworkHandle);

#if (NM_COM_CONTROL_ENABLED == STD_ON)
/* <BusNm>DisableCommunication */
typedef Std_ReturnType (*BusNm_DisableCommunication_Type)(NetworkHandleType nmNetworkHandle);

/* <BusNm>_EnableCommunication */
typedef Std_ReturnType (*BusNm_EnableCommunication_Type)(NetworkHandleType nmNetworkHandle);
#endif

#if (                                                                         \
    (NM_COM_USER_DATA_SUPPORT == STD_OFF) && (NM_USER_DATA_ENABLED == STD_ON) \
    && (NM_IF_ALL_CHANNEL_PASSIVE_MODE_ENABLED == STD_OFF))

/* <BusNm>_SetUserData */
typedef Std_ReturnType (*BusNm_SetUserData_Type)(NetworkHandleType nmNetworkHandle, const uint8* nmUserDataPtr);
#endif

#if (NM_USER_DATA_ENABLED == STD_ON)
/* <BusNm>_GetUserData */
typedef Std_ReturnType (*BusNm_GetUserData_Type)(NetworkHandleType nmNetworkHandle, uint8* nmUserDataPtr);
#endif

/* <BusNm>_GetPduData */
typedef Std_ReturnType (*BusNm_GetPduData_Type)(NetworkHandleType nmNetworkHandle, uint8* nmPduData);

/* <BusNm>_RepeatMessageRequest */
typedef Std_ReturnType (*BusNm_RepeatMessageRequest_Type)(NetworkHandleType nmNetworkHandle);

/* <BusNm>_GetNodeIdentifier */
typedef Std_ReturnType (*BusNm_GetNodeIdentifier_Type)(NetworkHandleType nmNetworkHandle, uint8* nmNodeIdPtr);

/* <BusNm>_GetLocalNodeIdentifier */
typedef Std_ReturnType (*BusNm_GetLocalNodeIdentifier_Type)(NetworkHandleType nmNetworkHandle, uint8* nmNodeIdPtr);

#if (NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
/* <BusNm>_CheckRemoteSleepIndication */
typedef Std_ReturnType (
    *BusNm_CheckRemoteSleepIndType)(NetworkHandleType nmNetworkHandle, boolean* nmRemoteSleepIndPtr);
#endif

/* <BusNm>_GetState */
typedef Std_ReturnType (
    *BusNm_GetState_Type)(NetworkHandleType nmNetworkHandle, Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr);
#if (NM_BUS_SYNCHRONIZATION_ENABLED == STD_ON)
/* <BusNm>_RequestBusSynchronization */
typedef Std_ReturnType (*BusNm_RequestBusSynchronization_Type)(NetworkHandleType nmChannelHandle);
#endif

#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
/* <BusNm>_SetSleepReadyBit */
typedef Std_ReturnType (*BusNm_SetSleepReadyBit_Type)(NetworkHandleType nmChannelHandle, boolean nmSleepReadyBit);
#endif

/* stand bus and generic bus have common low layer call api */
typedef struct
{
#if (NM_IF_ALL_CHANNEL_PASSIVE_MODE_ENABLED == STD_OFF)
    BusNm_NetworkRequest_Type BusNm_NetworkRequest;
    BusNm_NetworkRelease_Type BusNm_NetworkRelease;
#endif
    BusNm_PassiveStartUp_Type BusNm_PassiveStartUp;

#if (NM_COM_CONTROL_ENABLED == STD_ON)
    BusNm_DisableCommunication_Type BusNm_DisableCommunication;
    BusNm_EnableCommunication_Type  BusNm_EnableCommunication;
#endif

#if (                                                                         \
    (NM_COM_USER_DATA_SUPPORT == STD_OFF) && (NM_USER_DATA_ENABLED == STD_ON) \
    && (STD_OFF == NM_IF_ALL_CHANNEL_PASSIVE_MODE_ENABLED))
    BusNm_SetUserData_Type BusNm_SetUserData;
#endif

#if (NM_USER_DATA_ENABLED == STD_ON)
    BusNm_GetUserData_Type BusNm_GetUserData;
#endif

    BusNm_GetPduData_Type             BusNm_GetPduData;
    BusNm_RepeatMessageRequest_Type   BusNm_RepeatMessageRequest;
    BusNm_GetNodeIdentifier_Type      BusNm_GetNodeIdentifier;
    BusNm_GetLocalNodeIdentifier_Type BusNm_GetLocalNodeIdentifier;

#if (NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
    BusNm_CheckRemoteSleepIndType BusNm_CheckRemoteSleepInd;
#endif

    BusNm_GetState_Type BusNm_GetState;

#if (NM_BUS_SYNCHRONIZATION_ENABLED == STD_ON)
    BusNm_RequestBusSynchronization_Type BusNm_RequestBusSynchronization;
#endif

#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
    BusNm_SetSleepReadyBit_Type BusNm_SetSleepReadyBit;
#endif
} Nm_SpecificLowLayerApiType;

/* main function call cycle time type */
typedef uint16 Nm_FuncCycTimeType;
/* shutdown time type */
typedef uint16 Nm_ShutDownTimeType;

#if NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON
typedef struct
{
    /** Index of the filter mask byte. Specifies the position within the filter
     * mask byte array.
     */
    uint8 PnFilterMaskByteIndex;

    /** Parameter to configure the filter mask byte. */
    uint8 PnFilterMaskByteValue;
} Nm_PnFilterMaskByteType;

#endif

/**
 * @brief     defines the runtime variables related PNC
 */
typedef struct
{
    /* Ira buffer of this channel */
    uint8* IraInChannel;
    /* Era buffer of this channel */
    uint8* EraInChannel;
    /* Era timer of this channel */
    uint16* EraTimerInChannel;
} Nm_PncRelatedRunTimeType;

/**
 * @brief     defines Configuration of NM Channel
 */
typedef struct
{
    /* Generic or Standard Specifies bus type */
    Nm_BusNmType BusNmType;
    /* Reference to the corresponding ComM Channel. */
    NetworkHandleType ComMChannelRef;

    /* call special bus network manageme  api */
    const Nm_SpecificLowLayerApiType* BusNmApi;

#if (NM_STATE_CHANGE_IND_ENABLED == STD_ON)
    /* Specifies if the NMS shall be set for the corresponding network. */
    /* dependency: only available if NM_STATE_CHANGE_IND_ENABLED
    is TRUE and <Bus>NM_COM_USER_DATA_SUPPORT is configured */
    boolean StateReportEnabled;

    /* Reference to the signal for setting the NMS by calling
    Com_SendSignal for the respective channel. */
    Com_SignalIdType ComHandleId;
#endif

#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
    /* This parameter indicates whether a NM channel */
    /* dependency: If the CoordinatorSyncSupport is set to true this
    feature is available */
    boolean ActiveCoordinator;
#endif
    /* This parameter shall be set to indicate if the sleep of this network can
    be absolutely decided by the local node only and that no other nodes
    can oppose that decision */
    /* dependency: If the parameter CoordClusterIndex is not defined,
    this parameter is not valid. */
    boolean ChannelSleepMaster;

    /* If this parameter is undefined for a channel, the corresponding bus
    does not belong to an NM coordination cluster. */
    uint8 CoordClusterIndex;

    /* If this parameter is true, then this network is a synchronizing network
    for the NM coordination cluster which it belongs to */
    boolean SynchronizingNetwork;

    /* special bus ShutdownTimer */
    Nm_ShutDownTimeType BusShutdownTime;

#endif

#if NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON
    /* if pnc enabled */
    boolean PnEnabled;
    /* pnc buffer Bytes lenth */
    uint8 PncBitVectorLength;
    /* Specifies if Nm calculates the PN request information for ERA */
    boolean PnEraCalcEnabled;
    /* pnc mask buffer */
    const Nm_PnFilterMaskByteType* PnFilterMaskBytePtr;
    /* the number of era pnc bit */
    uint16 PnEraNum;
#endif
#if NM_MULTIPLE_PARTITION_USED == STD_ON
    /*NmChannelRefPartitionId*/
    uint16 ChannelReferencedPartitionId;
#endif
    /* PassiveModeEnabled in channel */
    boolean PassiveModeEnabled;
} Nm_ChannelLConfigType;

#if (NM_CAR_WAKE_UP_RX_ENABLED == STD_ON)
/* Car wakeup callout function api typede */
typedef void (*CarWakeUpCallout_Type)(NetworkHandleType nmNetworkHandle);
#endif

/**
 * @brief  This type contains the Link time build configuration structure.
 */
#if (                                                                                \
    (NM_CAR_WAKE_UP_RX_ENABLED == STD_ON) || (NM_COORDINATOR_SYNC_SUPPORT == STD_ON) \
    || (NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON))
typedef struct
{
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON || NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON)
    /* The period between successive calls to the Main Function of the NM
    Interface in seconds */
    /* dependency: If NmCoordinatorSupportEnabled is set to TRUE or NmPartialNetworkSupportEnabled is set TRUE, then
    the CycletimeMainFunction shall be configured. */
    Nm_FuncCycTimeType CycletimeMainFunction;
#endif

#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
    /* Enables/disables the coordinator synchronisation support. */
    /* dependency: CoordinatorSyncSupport shall only be valid if
    NM_COORDINATOR_SUPPORT_ENABLED is TRUE. */
    boolean CoordinatorSyncSupport;
#endif

#if (NM_CAR_WAKE_UP_RX_ENABLED == STD_ON)
    /* Enables or disables CWU detection. */
    boolean CarWakeUpRxEnabled;
    /* Name of the callout function to be called if Nm_CarWakeUpIndication()
    is called. If this parameter is not configured, the NmIf will call
    BswM_CarWakeUpIndication. */
    CarWakeUpCallout_Type CarWakeUpCallout;
#endif
#if (NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON)
    uint8  PncBitVectorOffset;
    uint16 PnResetTime;
#endif
} Nm_GlobalLConfigType;
#endif

/* Post-build configuration parameter type definitions */

#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)

typedef struct
{
    /* have one or more sync network */
    boolean SyncNetFlg;
    /* link CoordClusterIndex */
    uint8 CoorClusterIndex;
    /* cluster include number of channel */
    uint8 IncNumCh;
    /* per cluster include channel */
    const uint8* IncCh;
} Nm_PerClusterConfigType;

#endif

/** @see SWS_Nm_00282
 * @brief  This type contains the implementation-specific post build configuration structure.
 */
typedef struct
{
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
    /* number of clusters */
    uint8                          NumOfClu;
    const Nm_PerClusterConfigType* PerCluPtr;
#else
    uint8 dummy;
#endif
} Nm_ConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
#if (                                                                                \
    (NM_CAR_WAKE_UP_RX_ENABLED == STD_ON) || (NM_COORDINATOR_SYNC_SUPPORT == STD_ON) \
    || (NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON))

extern const Nm_GlobalLConfigType Nm_GlobalLConfig;

#endif

extern const Nm_ChannelLConfigType Nm_ChLConfig[NM_NUMBER_OF_CHANNELS];

#if NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON
extern const Nm_PncRelatedRunTimeType Nm_ChannelPncRelatedRuntimeRef[NM_NUMBER_OF_CHANNELS];
#endif

#if NM_COORDINATOR_SUPPORT_ENABLED == STD_ON || NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON
extern Nm_InnerChannelType* const Nm_InnerChannel[NM_NUMBER_OF_CHANNELS];
#endif

/**********End of link time configuration parameter declarations*******/

/**********Post-Build configuration parameter declarations*************/

extern const Nm_ConfigType Nm_Config;

/* ========================================= external function declarations ========================================= */

/**
 * @brief               This function initializes the NM
 * @param[in]           configPtr  Identification of the NM-channel
 * @retval              E_OK     No error
 * @retval              E_NOT_OK Passive startup of network management has failed
 * @reentrant           TRUE
 * @synchronous         FALSE
 * @trace               CPD-59667
 */

void Nm_Init(const Nm_ConfigType* configPtr);

/**
 * @brief               This function calls the BusNm PassiveStartUp function
 * @param[in]           networkHandle  Identification of the NM channel
 * @retval              E_OK     No error
 * @retval              E_NOT_OK Passive startup of network management has failed
 * @reentrant           TRUE (Non-reentrant for the same networkHandle, reentrant otherwise)
 * @synchronous         FALSE
 * @trace               CPD-59674
 */

Std_ReturnType Nm_PassiveStartUp(NetworkHandleType networkHandle);

#if (NM_IF_ALL_CHANNEL_PASSIVE_MODE_ENABLED == STD_OFF)
/**
 * @brief               This function calls the <BusNm>_NetworkRequest function
 * @param[in]           networkHandle  Identification of the NM-channel
 * @retval              E_OK      No error
 * @retval              E_NOT_OK  Requesting of bus communication has failed
 * @reentrant           TRUE (Non-reentrant for the same NetworkHandle, reentrant otherwise)
 * @synchronous         FALSE
 * @trace               CPD-66650
 */
Std_ReturnType Nm_NetworkRequest(NetworkHandleType networkHandle);

/**
 * @brief               This function calls the <BusNm>_NetworkRelease function
 * @param[in]           networkHandle  Identification of the NM-channel
 * @retval              E_OK     No error
 * @retval              E_NOT_OK Releasing of bus communication has failed
 * @reentrant           TRUE (Non-reentrant for the same networkHandle, reentrant otherwise)
 * @synchronous         FALSE
 * @trace               CPD-59678
 */
Std_ReturnType Nm_NetworkRelease(NetworkHandleType networkHandle);
#endif

#if (NM_COM_CONTROL_ENABLED == STD_ON)
/**
 * @brief               disables the NM PDU transmission ability.
 * @param[in]           networkHandle  Identification of the NM-channel
 * @retval              E_OK      No error
 * @retval              E_NOT_OK  Disabling of NM PDU transmission ability has
 * @reentrant           TRUE (Non-reentrant for the same NetworkHandle, reentrant otherwise)
 * @synchronous         FALSE
 * @trace               CPD-59668
 */
Std_ReturnType Nm_DisableCommunication(NetworkHandleType networkHandle);

/**
 * @brief               Enables the NM PDU transmission ability.
 * @param[in]           networkHandle  Identification of the NM-channel
 * @retval              E_OK  No  error
 * @retval              E_NOT_OK  Enabling of NM PDU transmission ability has
 * @reentrant           TRUE (Non-reentrant for the same networkHandle, reentrant otherwise)
 * @synchronous         FALSE
 * @trace               CPD-59679
 */
Std_ReturnType Nm_EnableCommunication(NetworkHandleType networkHandle);
#endif

#if (                                                                         \
    (NM_COM_USER_DATA_SUPPORT == STD_OFF) && (NM_USER_DATA_ENABLED == STD_ON) \
    && (NM_IF_ALL_CHANNEL_PASSIVE_MODE_ENABLED == STD_OFF))
/**
 * @brief               Set user data for NM messages transmitted next on the bus.
 * @param[in]           networkHandle  Identification of the NM-channel
 * @param[in]           nmUserDataPtr  User data for the next transmitted NM message
 * @retval              E_OK  No  error
 * @retval              E_NOT_OK  Setting of user data has failed
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-59670
 */
Std_ReturnType Nm_SetUserData(NetworkHandleType networkHandle, const uint8* nmUserDataPtr);
#endif

#if (NM_USER_DATA_ENABLED == STD_ON)
/**
 * @brief               Get user data out of the last successfully received NM message.
 * @param[in]           networkHandle  Identification of the NM-channel
 * @param[out]          nmUserDataPtr  User data for the next transmitted NM message
 * @retval              E_OK       No error
 * @retval              E_NOT_OK   Getting of user data has failed
 * @reentrant           TRUE (Non-reentrant for the same networkHandle, reentrant otherwise)
 * @synchronous         TRUE
 * @trace               CPD-59681
 */
Std_ReturnType Nm_GetUserData(NetworkHandleType networkHandle, uint8* nmUserDataPtr);
#endif

/**
 * @brief               Get the whole PDU data out of the most recently received NM message. For that purpose
 *                      <BusNm>_GetPduData shall be called in case NmBusType is not set to NM_BUSNM_LOCALNM.
 *                      (e.g. CanNm_GetPduData function is called if channel is configured as CAN).
 * @param[in]           networkHandle Identification of the NM-channel
 * @param[out]          nmPduData      Pointer where NM PDU shall be copied to.
 * @retval              E_OK       No error
 * @retval              E_NOT_OK   Getting of NM PDU data has failed
 *                      NetworkHandle does not exist (development only)
 *                      Module not yet initialized (development only)
 * @reentrant           TRUE (Non-reentrant for the same NetworkHandle, reentrant otherwise)
 * @synchronous         TRUE
 * @trace               CPD-59682
 */
extern Std_ReturnType Nm_GetPduData(NetworkHandleType networkHandle, uint8* nmPduData);

/**
 * @brief               Set Repeat Message Request Bit for NM messages transmitted next on the bus. For that purpose
 *                      <Bus>Nm_RepeatMessageRequest shall be called in case NmBusType is not set to NM_BUSNM_LOCALNM.
 *                      (e.g. CanNm_RepeatMessageRequest function is called if channel is configured as CAN). This will
 *                      force all nodes on the bus to transmit NM messages so that they can be identified.
 * @param[in]           networkHandle Identification of the NM-channel
 * @retval              E_OK      No error
 * @retval              E_NOT_OK  Setting of Repeat Message Request Bit has failed
 *                      networkHandle does not exist (development only)
 *                      Module not yet initialized (development only)
 * @reentrant           TRUE (Non-reentrant for the same networkHandle, reentrant otherwise)
 * @synchronous         TRUE
 * @trace               CPD-59683
 */
extern Std_ReturnType Nm_RepeatMessageRequest(NetworkHandleType networkHandle);

/**
 * @brief               Get node identifier out of the last successfully received NM-message. The function
 *                      <Bus>Nm_GetNodeIdentifier shall be called in case NmBusType is not set to NM_BUSNM_LOCALNM.
 * (e.g. CanNm_GetNodeIdentifier function is called if channel is configured as CAN).
 * @param[in]           networkHandle Identification of the NM-channel
 * @param[out]          nmNodeIdPtr    Pointer where node identifier out of the last successfully received NM-message
 *                      shall be
 * @retval              E_OK     No error
 * @retval              E_NOT_OK Getting of the node identifier out of the last received NM-message has failed
 *                      NetworkHandle does not exist (development only)
 *                      Module not yet initialized (development only)
 * @reentrant           TRUE (Non-reentrant for the same NetworkHandle, reentrant otherwise)
 * @synchronous         TRUE
 * @trace               CPD-59684
 */
extern Std_ReturnType Nm_GetNodeIdentifier(NetworkHandleType networkHandle, uint8* nmNodeIdPtr);

/**
 * @brief               Get node identifier configured for the local node. For that purpose
 *                      <Bus>Nm_GetLocalNodeIdentifier shall be called in case NmBusType is not set to NM_BUSNM_LOCALNM.
 * (e.g. CanNm_GetLocalNodeIdentifier function is called if channel is configured as CAN).
 * @param[in]           networkHandle Identification of the NM-channel
 * @param[out]          nmNodeIdPtr    Pointer where node identifier out of the last successfully received NM-message
 *                      shall be copied to
 * @retval              E_OK     No error
 * @retval              E_NOT_OK Getting of the node identifier of the local node has failed
 *                      NetworkHandle does not exist (development only)
 *                      Module not yet initialized (development only)
 * @reentrant           TRUE (Non-reentrant for the same NetworkHandle, reentrant otherwise)
 * @synchronous         TRUE
 * @trace               CPD-59685
 */
extern Std_ReturnType Nm_GetLocalNodeIdentifier(NetworkHandleType networkHandle, uint8* nmNodeIdPtr);

#if (NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
/**
 * @brief               Check if remote sleep indication takes place or not
 * @param[in]           networkHandle Identification of the NM-channel
 * @param[out]          remoteSleepIndPtr  Pointer where check result of remote
 * @retval              E_OK       No error
 * @retval              E_NOT_OK   Checking of remote sleep indication bits has
 *                      failed
 * @reentrant           TRUE (Non-reentrant for the same networkHandle, reentrant otherwise)
 * @synchronous         TRUE
 * @trace               CPD-59686
 */
Std_ReturnType Nm_CheckRemoteSleepIndication(NetworkHandleType networkHandle, boolean* remoteSleepIndPtr);
#endif
/**
 * @brief               Returns the state of the network management. The function <Bus>Nm_GetState shall be
 *                      called in case NmBusType is not set to NM_BUSNM_LOCALNM. (e.g. CanNm_GetState
 *                      function is called if channel is configured as CAN).
 * @param[in]           nmNetworkHandle Identification of the NM-channel
 * @param[out]          nmStatePtr Pointer where state of the network management
 * @param[out]          nmModePtr Pointer to the location where the mode of the network
 *                      management shall be copied to
 * @retval              E_OK: No   error
 * @retval              E_NOT_OK   Getting of NM state has failed
 *                      NetworkHandle does not exist (development only)
 *                      Module not yet initialized (development only)
 * @reentrant           TRUE (Non-reentrant for the same NetworkHandle, reentrant otherwise)
 * @synchronous         TRUE
 * @trace               CPD-59687
 */
Std_ReturnType Nm_GetState(NetworkHandleType nmNetworkHandle, Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr);

#if (NM_VERSION_INFO_API == STD_ON)
/**
 * @brief               This service returns the version information of this module.
 * @param[out]          nmVerInfoPtr  Pointer to where to store the version information
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-59688
 */
void Nm_GetVersionInfo(Std_VersionInfoType* nmVerInfoPtr);
#endif

/**
 * @brief               Notification that a NM-message has been received in the
 *                      Bus-Sleep Mode, what indicates that some nodes in the
 *                      network have already entered the Network Mode.
 * @param[in]           nmNetworkHandle   Identification of the NM-channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-66675
 */
void Nm_NetworkStartIndication(NetworkHandleType nmNetworkHandle);

/**
 * @brief               Notification that the network management has entered Network Mode.
 * @param[in]           nmNetworkHandle   Identification of the NM-channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-66679
 */
void Nm_NetworkMode(NetworkHandleType nmNetworkHandle);

/**
 * @brief               Notification that the network management has entered Bus-Sleep
 *                      Mode.
 * @param[in]           nmNetworkHandle   Identification of the NM-channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-66680
 */
void Nm_BusSleepMode(NetworkHandleType nmNetworkHandle);

/**
 * @brief               Notification that the network management has entered Prepare
 *                      Bus-Sleep Mode.
 * @param[in]           nmNetworkHandle   Identification of the NM-channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-66681
 */
void Nm_PrepareBusSleepMode(NetworkHandleType nmNetworkHandle);

#if (NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
/**
 * @brief               Notification that the network management has detected that
 *                      all other nodes on the network are ready to enter Bus-Sleep
 *                      Mode.
 * @param[in]           nmNetworkHandle   Identification of the NM-channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-66682
 */

void Nm_RemoteSleepIndication(NetworkHandleType nmNetworkHandle);

/**
 * @brief               Notification that the network management has detected that
 *                      not all other nodes on the network are longer ready to enter
 *                      Bus-Sleep Mode
 * @param[in]           nmNetworkHandle   Identification of the NM-channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-66683
 */
void Nm_RemoteSleepCancellation(NetworkHandleType nmNetworkHandle);

/**
 * @brief               Notification to the NM Coordinator functionality that this
 *                      is a suitable point in time to initiate the coordinated
 *                      shutdown on
 * @param[in]           nmNetworkHandle   Identification of the NM-channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-66684
 */
void Nm_SynchronizationPoint(NetworkHandleType nmNetworkHandle);

#endif

#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)

/**
 * @brief               Sets an indication, when the NM Coordinator Sleep Ready
 *                      bit in the Control Bit Vector is set
 * @param[in]           nmChannelHandle   Identification of the NM-channel
 * @reentrant           FALSE
 * @synchronous         TRUE
 * @trace               CPD-66687
 */
void Nm_CoordReadyToSleepIndication(NetworkHandleType nmChannelHandle);

/**
 * @brief               Cancels an indication, when the NM Coordinator Sleep Ready
 *                      bit in the Control Bit Vector is set back to 0
 * @param[in]           nmChannelHandle   Identification of the NM-channel
 * @reentrant           FALSE
 * @synchronous         TRUE
 * @trace               CPD-66689
 */
void Nm_CoordReadyToSleepCancellation(NetworkHandleType nmChannelHandle);

#endif

#if (NM_PDU_RX_INDICATION_ENABLED == STD_ON)
/**
 * @brief               Notification that a NM message has been received.
 * @param[in]           nmNetworkHandle   Identification of the NM-channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-66691
 */

void Nm_PduRxIndication(NetworkHandleType nmNetworkHandle);

#endif

#if (NM_STATE_CHANGE_IND_ENABLED == STD_ON)
/**
 * @brief               Notification that the state of the lower layer <Bus>Nm has changed.
 * @param[in]           nmNetworkHandle   Identification of the NM-channel
 * @param[in]           nmPreviousState   previous state of the NM-channel
 * @param[in]           nmCurrentState    current state of the NM-channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-66692
 */

void Nm_StateChangeNotification(
    NetworkHandleType nmNetworkHandle,
    Nm_StateType      nmPreviousState,
    Nm_StateType      nmCurrentState);

#endif

/**
 * @brief               Service to indicate that an NM message with set Repeat
 *                      Message Request Bit has been received
 * @param[in]           nmNetworkHandle   Identification of the NM-channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-66693
 */

void Nm_RepeatMessageIndication(NetworkHandleType nmNetworkHandle);

/**
 * @brief               Service to indicate that an attempt to send an NM message
 *                      failed.
 * @param[in]           nmNetworkHandle   Identification of the NM-channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-66694
 */

void Nm_TxTimeoutException(NetworkHandleType nmNetworkHandle);

#if (NM_CAR_WAKE_UP_RX_ENABLED == STD_ON)
/**
 * @brief               This function is called by a <Bus>Nm to indicate reception
 *                      of a CWU request
 * @param[in]           nmChannelHandle   Identification of the NM-channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-66695
 */

void Nm_CarWakeUpIndication(NetworkHandleType nmChannelHandle);

#endif

#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON || NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON)
/**
 * @brief               implements the processes of the NM Interface which need a fix cyclic scheduling.
 * @param[in]           chIdx    the configured channel index
 * @reentrant           TRUE (Non-reentrant for the same NetworkHandle, reentrant otherwise)
 * @synchronous         FALSE
 * @trace               CPD-59673
 */
void Nm_MainFunction(uint8 chIdx);
#endif

/**
 * @brief               Notification that the network management has entered
 *                      Synchronize Mode.
 * @param[in]           nmNetworkHandle   Identification of the NM-channel
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-59693
 */

void Nm_SynchronizeMode(NetworkHandleType nmNetworkHandle);

#if NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON
/**
 * @brief               Indication by ComM of internal PNC requests. This is used to aggregate the internal PNC
 *                      requests.
 * @param[in]           networkHandle   Identification of the NM-channel
 * @param[in]           pncBitVectorPtr Pointer to the bit vector with all PNC bits set to "1" of internal requested
 *                      PNCs (IRA)
 * @reentrant           Non Reentrant for the same NetworkHandle, reentrant otherwise
 * @synchronous         TRUE (Non-reentrant for the same NetworkHandle, reentrant otherwise)
 * @trace               CPD-59680
 */
void Nm_UpdateIRA(NetworkHandleType networkHandle, const uint8* pncBitVectorPtr);
/**
 * @brief               Function called by <Bus>Nms to request the aggregated internal PNC requests for transmission
 *                      within the Nm message.
 * @param[in]           networkHandle   Identification of the NM-channel
 * @param[out]          pncBitVectorPtr Pointer to the bit vector with all PNC bits set to 1 of internal requested
 *                      PNCs
 * @reentrant           Non Reentrant for the same NetworkHandle, reentrant otherwise
 * @synchronous         TRUE (Non-reentrant for the same NetworkHandle, reentrant otherwise)
 * @trace               CPD-59701
 */
void Nm_PncBitVectorTxIndication(NetworkHandleType networkHandle, uint8* pncBitVectorPtr);
/**
 * @brief               The function evaluate if a relevant PNC request eg. PNC bit set to 1, is available in the
 * given PNC bit vector
 * @param[in]           networkHandle   Identification of the NM-channel
 * @param[in]           pncBitVectorPtr Pointer to the bit vector with all PNC bits set to "1" of external requested
 * PNCs
 * @param[out]          relevantPncRequestDetectedPtr Pointer to a boolean variable which indicates, if a relevant PNC
 * request
 * @reentrant           Non Reentrant for the same networkHandle, reentrant otherwise
 * @synchronous         TRUE (Non-reentrant for the same networkHandle, reentrant otherwise)
 * @trace               CPD-59700
 */
void Nm_PncBitVectorRxIndication(
    NetworkHandleType networkHandle,
    const uint8*      pncBitVectorPtr,
    boolean*          relevantPncRequestDetectedPtr);
#endif
#endif
