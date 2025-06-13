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
**  FILENAME    : CanNm.h                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Wanglili                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

#ifndef CANNM_H_
#define CANNM_H_

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "ComStack_Types.h"
#include "NmStack_Types.h"
#include "CanNm_Cfg.h"
#include "CanNm_Cbk.h"

#if CANNM_MULTIPLE_PARTITION_USED == STD_ON
#include "Os.h"
#endif

/******************************************************************************
**                      Global Symbols                                       **
******************************************************************************/
/* Published information elements */
#define CANNM_H_AR_RELEASE_MAJOR_VERSION 4u
#define CANNM_H_AR_RELEASE_MINOR_VERSION 5u
#define CANNM_H_AR_RELEASE_PATCH_VERSION 0u
#define CANNM_VENDOR_ID                  62u
#define CANNM_MODULE_ID                  33u
#define CANNM_INSTANCE_ID                0u
#define CANNM_DEFAULT_PDU_LENGTH         8u

/* Development errors used by CAN Network Management module */
#define CANNM_E_UNINIT                    ((uint8)(0x01u))
#define CANNM_E_INVALID_CHANNEL           ((uint8)(0x02u))
#define CANNM_E_INVALID_PDUID             ((uint8)(0x03u))
#define CANNM_E_NET_START_IND             ((uint8)(0x04u))
#define CANNM_E_INIT_FAILED               ((uint8)(0x05u))
#define CANNM_E_INVALID_PARTITION_CONTEXT ((uint8)(0x06u))
#define CANNM_E_NETWORK_TIMEOUT           ((uint8)(0x11u))
#define CANNM_E_PARAM_POINTER             ((uint8)(0x12u))
#define CANNM_E_NOT_IN_BUS_SLEEP          ((uint8)(0x13u))
#define CANNM_E_ALREADY_INITIALIZED       ((uint8)(0xF0u))

/* Service ID[hex] */
#define CANNM_SERVICE_ID_INIT                       ((uint8)(0x00u))
#define CANNM_SERVICE_ID_DEINIT                     ((uint8)(0x10u))
#define CANNM_SERVICE_ID_PASSIVESTARUP              ((uint8)(0x01u))
#define CANNM_SERVICE_ID_NETWORKREQUEST             ((uint8)(0x02u))
#define CANNM_SERVICE_ID_NETWORKRELEASE             ((uint8)(0x03u))
#define CANNM_SERVICE_ID_DISABLECOMMUNICATION       ((uint8)(0x0cu))
#define CANNM_SERVICE_ID_ENABLECOMMUNICATION        ((uint8)(0x0du))
#define CANNM_SERVICE_ID_SETUSERDATA                ((uint8)(0x04u))
#define CANNM_SERVICE_ID_GETUSERDATA                ((uint8)(0x05u))
#define CANNM_SERVICE_ID_TRANSMIT                   ((uint8)(0x14u))
#define CANNM_SERVICE_ID_GETNODEIDENTIFIER          ((uint8)(0x06u))
#define CANNM_SERVICE_ID_GETLOCALNODEIDENTIFIER     ((uint8)(0x07u))
#define CANNM_SERVICE_ID_REPEATMESSAGEREQUEST       ((uint8)(0x08u))
#define CANNM_SERVICE_ID_GETPDUDATA                 ((uint8)(0x0au))
#define CANNM_SERVICE_ID_GETSTATE                   ((uint8)(0x0bu))
#define CANNM_SERVICE_ID_GETVERSIONINFO             ((uint8)(0xf1u))
#define CANNM_SERVICE_ID_REQUESTBUSSYNCHRONIZATION  ((uint8)(0xc0u))
#define CANNM_SERVICE_ID_CHECKREMOTESLEEPINDICATION ((uint8)(0xd0u))
#define CANNM_SERVICE_ID_SETSLEEPREADYBIT           ((uint8)(0x17u))
#define CANNM_SERVICE_ID_TXCONFIRMATION             ((uint8)(0x40u))
#define CANNM_SERVICE_ID_RXINDICATION               ((uint8)(0x42u))
#define CANNM_SERVICE_ID_CONFIRMPNAVAILABILITY      ((uint8)(0x16u))
#define CANNM_SERVICE_ID_TRIGGERTRANSMIT            ((uint8)(0x41u))
#define CANNM_SERVICE_ID_MAINFUNCTION               ((uint8)(0x13u))

/* CANNM_VER_4_2_2 on or CANNM_VER_R19_11 on  */
#define CANNM_VER_4_2_2  STD_OFF
#define CANNM_VER_R19_11 STD_ON

typedef enum
{
    CANNM_PDU_BYTE_0 = 0x00,
    CANNM_PDU_BYTE_1 = 0x01,
    CANNM_PDU_OFF = 0xFF,
} CanNm_PduPositionType;

typedef struct
{
    PduIdType RxPduId; /* CanNmRxPduId */
} CanNm_RxPduType;

typedef struct
{
    PduIdType TxConfirmationPduId; /* CanNmTxConfirmationPduId */
    PduIdType LowerLayerPduId;     /* CanNmTxPduRef */
    boolean TxPduIsTriggerTrans;   /*from CanIfTxPduTriggerTransmit*/
} CanNm_TxPduType;

#if CANNM_COM_USERDATA_SUPPORT == STD_ON
typedef struct
{
    PduIdType TxUserDataPduId; /* CanNmTxUserDataPduId */
    PduIdType UpperLayerPduId; /* CanNmTxUserDataPduRef */
} CanNm_UserDataTxPduType;
#endif

typedef struct
{
#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
    /* Enables/Disables the handling of the Active Wakeup Bit in the CanNm
     * module.
     */
    boolean ActiveWakeupBitEnabled;
#endif /* STD_OFF == CANNM_PASSIVE_MODE_ENABLED */

#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
    /* Specifies if CanNm drops irrelevant NM PDUs. */
    boolean AllNmMessagesKeepAwake;
#endif

#if (STD_ON == CANNM_BUSLOAD_REDUCTION_ENABLED)
    /* This parameter defines if bus load reduction for the respective NM
        channel is active or not. */
    /* dependency: Shall only be True if CANNM_BUSLOAD_REDUCTION_ENABLED is
    True. */
    boolean BusLoadReductionActive;
#endif /* STD_ON == CANNM_BUSLOAD_REDUCTION_ENABLED */

#if (STD_ON == CANNM_CAR_WAKEUP_RX_ENABLED)
    /* Specifies the Bit position of the CWU within the NM PDU. */
    uint8 CarWakeUpBitPosition;

    /* Specifies the Byte position of the CWU within the NM PDU. */
    uint8 CarWakeUpBytePosition;

    /* Specifies if CWU filtering is supported */
    boolean CarWakeUpFilterEnabled;

#if (STD_ON == CANNM_CAR_WAKEUP_FILTER_ENABLED)
    /* Source node identifier for CWU filtering. */
    uint8 CarWakeUpFilterNodeId;
#endif /* STD_ON == CANNM_CAR_WAKEUP_FILTER_ENABLED */

    /* Enables or disables support of CarWakeUp bit evaluation in received
     * NM PDUs.
     */
    boolean CarWakeUpRxEnabled;
#endif /* STD_ON == CANNM_CAR_WAKEUP_RX_ENABLED */

#if (STD_ON == CANNM_IMMEDIATE_TRANSMIT_ENABLED)
    /* Defines the immediate NM PDU cycle time in seconds which is used for
     * CanNmImmediateNmTransmissions NM PDU transmissions.
     */
    uint16 ImmediateNmCycleTime;
#endif /* STD_ON == CANNM_IMMEDIATE_TRANSMIT_ENABLED */

    /* Defines the number of immediate NM PDUs which shall be transmitted.*/
    uint8 ImmediateNmTransmissions;

    /* Time offset in the periodic transmission node.*/
    uint16 MsgCycleOffset;

    /* Period of a NM PDU in seconds.*/
    uint16 MsgCycleTime;

#if (STD_ON == CANNM_BUSLOAD_REDUCTION_ENABLED)
    /* Node specific bus cycle time in the periodic transmission mode with bus
       load reduction.*/
    uint16 MsgReducedTime;
#endif /* STD_ON == CANNM_BUSLOAD_REDUCTION_ENABLED */

#if ((STD_OFF == CANNM_PASSIVE_MODE_ENABLED) && (STD_OFF == CANNM_IMMEDIATE_TXCONF_ENABLED))
    /* Transmission Timout of NM-message. */
    uint16 MsgTimeoutTime;
#endif

#if CANNM_NODE_DETECTION_ENABLED == STD_ON
    /* Pre-processor switch for enabling the node detection support. */
    boolean NodeDetectionEnabled;
#endif /* CANNM_NODE_DETECTION_ENABLED == STD_ON */

#if CANNM_NODE_ID_ENABLED == STD_ON
    /* Pre-processor switch for enabling the source node identifier. */
    boolean NodeIdEnabled;
#endif /* CANNM_NODE_ID_ENABLED == STD_ON */

    /* Node identifier of local node.*/
    uint8 NodeId;

    /*Defines the position of the control bit vector within the NM PACKET.*/
    CanNm_PduPositionType PduCbvPosition;

    /*Defines the position of the source node identifier within the NM PACKET*/
    CanNm_PduPositionType PduNidPosition;

#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
    /* Enables or disables support of partial networking. */
    boolean PnEnabled;

    /* Specifies if CanNm calculates the PN request information for ERA */
    boolean PnEraCalcEnabled;

    /* Specifies if CanNm performs an additional transition
     * from Network Mode to Repeat Message State (true) or not (false).
     */
    boolean PnHandleMultipleNetworkRequests;
#endif /* STD_ON == CANNM_GLOBAL_PN_SUPPORT */

#if (STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED)
    /* Timeout for Remote Sleep Indication. */
    uint16 RemoteSleepIndTime;
#endif /* STD_ON == CANNM_REMOTE_SLEEPIND_ENABLED */

    /* Timeout for Repeat Message State. */
    uint16 RepeatMessageTime;

#if CANNM_REPEAT_MSG_IND_ENABLED == STD_ON
    /*
     * Enable/disable the notification that a RepeatMessageRequest bit has
     * been received.
     */
    boolean RepeatMsgIndEnabled;
#endif /* CANNM_REPEAT_MSG_IND_ENABLED == STD_ON */

#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF) && (CANNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON)
    /* Specifies if first message request in CanNm is repeated until accepted
     * by CanIf.
     */
    boolean RetryFirstMessageRequest;
#endif

    /* If this parameter is disabled Prepare Bus-Sleep Mode is left after
     * CanNmWaitBusSleepTime.
     */
    boolean StayInPbsEnabled;

    /* It denotes the time in seconds how long the NM shall stay in the Ready
     * Sleep State before transition into the Prepare Bus-Sleep Mode is
     * initiated.*/
    uint16 TimeoutTime;

    /* Timeout for bus calm down phase. */
    uint16 WaitBusSleepTime;

    /* This reference points to the unique channel defined by the ComMChannel
     * and provides access to the unique channel index value in ComMChannelId
     */
    NetworkHandleType ComMNetworkHandleRef;

#if (STD_ON == CANNM_PN_ERA_CALC_ENABLED)
    /*
     * Reference to a Pdu in the COM-Stack.The SduRef is required for every
     * CanNm Channel,because ERA is reported per channel.
     */
    PduIdType PnEraRxNSduRef;
#endif /* STD_ON == CANNM_PN_ERA_CALC_ENABLED */

    /* This container describes the CanNm RX PDU's. */
    P2CONST(CanNm_RxPduType, TYPEDEF, CANNM_CONST) RxPdu;

    uint8 RxPduNum;

    /* This container describes the CanNm TX PDU's. */
    P2CONST(CanNm_TxPduType, TYPEDEF, CANNM_CONST) TxPdu;
    uint8 UserDataLength;
    uint8 UserDataOffset;
#if CANNM_COM_USERDATA_SUPPORT == STD_ON
    const CanNm_UserDataTxPduType* UserDataTxPdu;
#endif

#if CANNM_MULTIPLE_PARTITION_USED == STD_ON
    ApplicationType applicationID;
#endif
} CanNm_ChannelConfigType;

#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
typedef struct
{
    /* Index of the filter mask byte. Specifies the position within the filter
     * mask byte array.
     */
    uint8 PnFilterMaskByteIndex;

    /* Parameter to configure the filter mask byte. */
    uint8 PnFilterMaskByteValue;
} CanNm_PnFilterMaskByteType;

typedef struct
{
    /* Specifies the length of the PN request information in the NM PDU */
    uint8 CanNmPnInfoLength;

    /* Specifies the offset of the PN request information in the NM PDU. */
    uint8 CanNmPnInfoOffset;

    /* PN filter number */
    uint8 CanNmPnFilterMaskByteNum;

    /* PN information configuratio */
    P2CONST(CanNm_PnFilterMaskByteType, TYPEDEF, CANNM_CONST) PnFilterMaskByte;
} CanNm_PnInfoConfigType;
#endif

typedef struct
{
    /*Call cycle in seconds of CanNm_MainFunction.*/
    uint16 MainFunctionPeriod;

#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
    /* Specifies the runtime of the reset timer in seconds. */
    uint16 PnResetTime;
#endif

#if (STD_ON == CANNM_PN_EIRA_CALC_ENABLED)
    /* Reference to a Pdu in the COM-Stack. */
    PduIdType CanNmPnEiraRxNSduRef;
#endif

    /* This container contains the channel-specific configuration parameters of
     * the CanNm. */
    P2CONST(CanNm_ChannelConfigType, TYPEDEF, CANNM_CONST) ChannelConfig;

#if (STD_ON == CANNM_GLOBAL_PN_SUPPORT)
    /* PN information configuration */
    P2CONST(CanNm_PnInfoConfigType, TYPEDEF, CANNM_CONST) PnInfo;
#endif
} CanNm_ConfigType;

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/
extern CONST(CanNm_ConfigType, CANNM_CONST) CanNm_Config;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/******************************************************************************/
/*
 * Brief               Initialize the CanNm module.
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanNmConfigPtr, Pointer to a selected configuration
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void CanNm_Init(const CanNm_ConfigType* CanNmConfigPtr);

/******************************************************************************/
/*
 * Brief               De-initializes the CanNm module.
 * ServiceId           0x10
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void CanNm_DeInit(void);

/******************************************************************************/
/*
 * Brief               Passive startup of the AUTOSAR CAN NM.
 * ServiceId           0x01
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Passive startup of network management has failed
 */
/******************************************************************************/
Std_ReturnType CanNm_PassiveStartUp(NetworkHandleType nmChannelHandle);

#if (STD_OFF == CANNM_PASSIVE_MODE_ENABLED)
/******************************************************************************/
/*
 * Brief               Request the network, since ECU needs to communicate on
 *                     the bus.
 * ServiceId           0x02
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Requesting of network has failed
 */
/******************************************************************************/
Std_ReturnType CanNm_NetworkRequest(NetworkHandleType nmChannelHandle);

/******************************************************************************/
/*
 * Brief               Release the network, since ECU doesn't have to communicate
 *                     on the bus.
 * ServiceId           0x03
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Releasing of network has failed
 */
/******************************************************************************/
Std_ReturnType CanNm_NetworkRelease(NetworkHandleType nmChannelHandle);
#endif

#if (STD_ON == CANNM_COM_CONTROL_ENABLED)
/******************************************************************************/
/*
 * Brief               Disable the NM PDU transmission ability due to a ISO14229
 *                     Communication Control (28hex) service.
 * ServiceId           0x0c
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Disabling of NM PDU transmission ability has failed
 */
/******************************************************************************/
Std_ReturnType CanNm_DisableCommunication(NetworkHandleType nmChannelHandle);

/******************************************************************************/
/*
 * Brief               Enable the NM PDU transmission ability due to a ISO14229
 *                     Communication Control (28hex) service.
 * ServiceId           0x0d
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Enabling of NM PDU transmission ability has failed
 */
/******************************************************************************/
Std_ReturnType CanNm_EnableCommunication(NetworkHandleType nmChannelHandle);
#endif /* STD_ON == CANNM_COM_CONTROL_ENABLED */

#if (CANNM_USER_DATA_ENABLED == STD_ON) && (CANNM_COM_USERDATA_SUPPORT == STD_OFF) \
    && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
/******************************************************************************/
/*
 * Brief               Set user data for NM PDUs transmitted next on the bus.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 *                     nmUserDataPtr,Pointer where the user data for the next
 *                     transmitted NM PDU shall be copied from
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Setting of user data has failed
 * Trace               SWS_CanNm_00327
 */
/******************************************************************************/
Std_ReturnType CanNm_SetUserData(NetworkHandleType nmChannelHandle, const uint8* nmUserDataPtr);
#endif

#if CANNM_USER_DATA_ENABLED == STD_ON
/******************************************************************************/
/*
 * Brief               Get user data out of the most recently received NM PDU.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     nmUserDataPtr,Pointer where user data out of the most
 *                     recently received NM PDU shall be copied to
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Getting of user data has failed
 */
/******************************************************************************/
Std_ReturnType CanNm_GetUserData(NetworkHandleType nmChannelHandle, uint8* nmUserDataPtr);
#endif

#if (CANNM_COM_USERDATA_SUPPORT == STD_ON)
/**
 * @brief       Requests transmission of a PDU.
 * @id          0x49
 * @synchronous Synchronous
 * @reentrancy  Reentrant for different PduIds. Non reentrant for the same PduId.
 * @param[in]   TxPduId    Identifier of the PDU to be transmitted
 * @param[in]   PduInfoPtr Length of and pointer to the PDU data and pointer to MetaData.
 * @retval      E_OK       Transmit request has been accepted.
 * @retval      E_NOT_OK   Transmit request has not been accepted.
 */
Std_ReturnType CanNm_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);
#endif

/**
 * @brief       Get node identifier out of the most recently received NM PDU.
 * @id          0x06
 * @synchronous Synchronous
 * @reentrancy  Reentrant
 * @param[in]   nmChannelHandle Identification of the NM-channel
 * @param[out]  nmNodeIdPtr     Pointer where node identifier out of the most recently received NM PDU shall be copied
 *                              to.
 * @retval      E_OK            No error
 * @retval      E_NOT_OK        Getting of the node identifier out of the most recently received NM PDU has failed or is
 *                              not configured for this network handle.
 */
Std_ReturnType CanNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr);

/**
 * @brief       Get node identifier configured for the local node.
 * @id          0x07
 * @synchronous Synchronous
 * @reentrancy  Reentrant
 * @param[in]   nmChannelHandle Identification of the NM-channel
 * @param[out]  nmNodeIdPtr     Pointer where node identifier of the local node shall be copied to.
 * @retval      E_OK            No error
 * @retval      E_NOT_OK        Getting of the node identifier of the local node has failed or is not configured for
 *                              this network handle.
 */
Std_ReturnType CanNm_GetLocalNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr);

/**
 * @brief       Set Repeat Message Request Bit for NM PDUs transmitted next on the bus.
 * @id          0x08
 * @synchronous Asynchronous
 * @reentrancy  Reentrant (but not for the same NM-channel)
 * @param[in]   nmChannelHandle Identification of the NM-channel
 * @retval      E_OK            No error
 * @retval      E_NOT_OK        Setting of Repeat Message Request Bit has failed or is not configured for this network
 *                              handle.
 */
Std_ReturnType CanNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle);

/**
 * @brief       Get the whole PDU data out of the most recently received NM PDU.
 * @id          0x0a
 * @synchronous Synchronous
 * @reentrancy  Reentrant
 * @param[in]   nmChannelHandle Identification of the NM-channel
 * @param[out]  nmPduDataPtr    Pointer where NM PDU shall be copied to.
 * @retval      E_OK            No error
 * @retval      E_NOT_OK        Getting of NM PDU Data has failed or is not configured for this network handle.
 */
Std_ReturnType CanNm_GetPduData(NetworkHandleType nmChannelHandle, uint8* nmPduDataPtr);

/******************************************************************************/
/*
 * Brief               Returns the state and the mode of the network management.
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     nmStatePtr, Pointer where state of the network management
 *                     shall be copied to
 *                     nmModePtr, Pointer where the mode of the network management
 *                     shall be copied to
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Getting of NM state has failed
 */
/******************************************************************************/
Std_ReturnType CanNm_GetState(NetworkHandleType nmChannelHandle, Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr);

#if CANNM_VERSION_INFO_API == STD_ON
/******************************************************************************/
/*
 * Brief               This service returns the version information of this module.
 * ServiceId           0xf1
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     versioninfo, Pointer to where to store the version information
 *                     of this module
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void CanNm_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

#if ((STD_ON == CANNM_BUS_SYNCHRONIZATION_ENABLED) && (STD_OFF == CANNM_PASSIVE_MODE_ENABLED))
/******************************************************************************/
/*
 * Brief               Request bus synchronization.
 * ServiceId           0xc0
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Requesting of bus synchronization has failed
 */
/******************************************************************************/
Std_ReturnType CanNm_RequestBusSynchronization(NetworkHandleType nmChannelHandle);
#endif

#if (STD_ON == CANNM_REMOTE_SLEEP_IND_ENABLED)
/******************************************************************************/
/*
 * Brief               Check if remote sleep indication takes place or not.
 * ServiceId           0xd0
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     nmRemoteSleepIndPtr, Pointer where check result of remote
 *                     sleep indication shall be copied to
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Checking of remote sleep indication bits has
                       failed
 */
/******************************************************************************/
Std_ReturnType CanNm_CheckRemoteSleepIndication(NetworkHandleType nmChannelHandle, boolean* nmRemoteSleepIndPtr);
#endif

/* optional avaliable @SWS_CanNm_00340 */
#if (STD_ON == CANNM_COORDINATOR_SYNC_SUPPORT)
/******************************************************************************/
/*
 * Brief               Set the NM Coordinator Sleep Ready bit in the Control Bit
 *                     Vector.
 * ServiceId           0x17
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant (but not for the same NM-channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 *                     nmSleepReadyBit, Value written to ReadySleep Bit in CBV
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Writing of remote sleep indication bit has
                       failed
 */
/******************************************************************************/
Std_ReturnType CanNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle, boolean nmSleepReadyBit);
#endif

#endif /* CANNM_H_ */
