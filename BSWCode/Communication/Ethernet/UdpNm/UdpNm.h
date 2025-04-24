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
**  FILENAME    : UdpNm.h                                                     **
**                                                                            **
**  Created on  : 25/07/19                                                    **
**  Author      : lili.wang                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : declaration of provided interface functions                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#ifndef UDPNM_H_
#define UDPNM_H_

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define UDPNM_VENDOR_ID          62u
#define UDPNM_MODULE_ID          33u
#define UDPNM_INSTANCE_ID        0u
#define UDPNM_H_AR_MAJOR_VERSION 4u
#define UDPNM_H_AR_MINOR_VERSION 5u
#define UDPNM_H_AR_PATCH_VERSION 0u

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "ComStack_Types.h"
#include "NmStack_Types.h"
#include "UdpNm_Cfg.h"
#include "UdpNm_Cbk.h"

/*******************************************************************************
**                      Macro Definitions                                     **
*******************************************************************************/
/* The following errors shall be detectable by the UdpNm */
#define UDPNM_E_UNINIT              ((uint8)(0x01u))
#define UDPNM_E_INVALID_CHANNEL     ((uint8)(0x02u))
#define UDPNM_E_INVALID_PDUID       ((uint8)(0x03u))
#define UDPNM_E_INIT_FAILED         ((uint8)(0x04u))
#define UDPNM_E_PARAM_POINTER       ((uint8)(0x12u))
#define UDPNM_E_NETWORK_TIMEOUT     ((uint8)(0x11u))
#define UDPNM_E_PARTITION           ((uint8)(0x13u))
#define UDPNM_E_ALREADY_INITIALIZED ((uint8)(0xF0u))

/* Service ID[hex] */
#define UDPNM_SERVICE_ID_INIT                       ((uint8)(0x01u))
#define UDPNM_SERVICE_ID_PASSIVESTARUP              ((uint8)(0x0eu))
#define UDPNM_SERVICE_ID_NETWORKREQUEST             ((uint8)(0x02u))
#define UDPNM_SERVICE_ID_NETWORKRELEASE             ((uint8)(0x03u))
#define UDPNM_SERVICE_ID_DISABLECOMMUNICATION       ((uint8)(0x0cu))
#define UDPNM_SERVICE_ID_ENABLECOMMUNICATION        ((uint8)(0x0du))
#define UDPNM_SERVICE_ID_SETUSERDATA                ((uint8)(0x04u))
#define UDPNM_SERVICE_ID_GETUSERDATA                ((uint8)(0x05u))
#define UDPNM_SERVICE_ID_GETNODEIDENTIFIER          ((uint8)(0x06u))
#define UDPNM_SERVICE_ID_GETLOCALNODEIDENTIFIER     ((uint8)(0x07u))
#define UDPNM_SERVICE_ID_REPEATMESSAGEREQUEST       ((uint8)(0x08u))
#define UDPNM_SERVICE_ID_GETPDUDATA                 ((uint8)(0x0au))
#define UDPNM_SERVICE_ID_GETSTATE                   ((uint8)(0x0bu))
#define UDPNM_SERVICE_ID_GETVERSIONINFO             ((uint8)(0x09u))
#define UDPNM_SERVICE_ID_REQUESTBUSSYNCHRONIZATION  ((uint8)(0x14u))
#define UDPNM_SERVICE_ID_CHECKREMOTESLEEPINDICATION ((uint8)(0x11u))
#define UDPNM_SERVICE_ID_SETCOORDBITS               ((uint8)(0x12u))
#define UDPNM_SERVICE_ID_SETSLEEPREADYBIT           ((uint8)(0x16u))
#define UDPNM_SERVICE_ID_TRANSMIT                   ((uint8)(0x49u))
#define UDPNM_SERVICE_ID_IFTXCONFIRMATION           ((uint8)(0x40u))
#define UDPNM_SERVICE_ID_RXINDICATION               ((uint8)(0x42u))
#define UDPNM_SERVICE_ID_IFTRIGGERTRANSMIT          ((uint8)(0x41u))
#define UDPNM_SERVICE_ID_MAINFUNCTION               ((uint8)(0x13u))

/* UDPNM_VER_4_2_2 or UDPNM_VER_R19_11 */
#define UDPNM_VER_R19_11
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/* Used to define the position of the control bit vector within the NM PACKET.*/
typedef enum
{
    UDPNM_PDU_BYTE_0 = 0x00,
    UDPNM_PDU_BYTE_1 = 0x01,
    UDPNM_PDU_OFF = 0xFF,
} UdpNm_PduPositionType;

typedef struct
{
    PduIdType RxPduId; /* UdpNmRxPduId */
} UdpNm_RxPduType;

typedef struct
{
    PduIdType TxConfirmationPduId; /* UdpNmTxConfirmationPduId */
    PduIdType LowerLayerPduId;     /* UdpNmTxPduRef */
} UdpNm_TxPduType;

#if UDPNM_COM_USERDATA_SUPPORT == STD_ON
typedef struct
{
    PduIdType TxUserDataPduId; /* UdpNmTxUserDataPduId */
    PduIdType UpperLayerPduId; /* UdpNmTxUserDataPduRef */
} UdpNm_UserDataTxPduType;
#endif

typedef struct
{
#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
    /* Enables/Disables the handling of the Active Wakeup Bit in the UdpNm
     * module.
     */
    boolean ActiveWakeupBitEnabled;
#endif /* STD_OFF == UDPNM_PASSIVE_MODE_ENABLED */

#if (STD_ON == UDPNM_GLOBAL_PN_ENABLED)
    /* Specifies if UdpNm drops irrelevant NM PDUs. */
    boolean AllNmMessagesKeepAwake;
#endif

#if (STD_ON == UDPNM_CAR_WAKEUP_RX_ENABLED)
    /* Specifies the Bit position of the CWU within the NM PDU. */
    uint8 CarWakeUpBitPosition;

    /* Specifies the Byte position of the CWU within the NM PDU. */
    uint8 CarWakeUpBytePosition;

#ifdef UDPNM_VER_R19_11
    /* Specifies if CWU filtering is supported */
    boolean CarWakeUpFilterEnabled;

#if (STD_ON == UDPNM_CAR_WAKEUP_FILTER_ENABLED)
    /* Source node identifier for CWU filtering. */
    uint8 CarWakeUpFilterNodeId;
#endif /* STD_ON == UDPNM_CAR_WAKEUP_FILTER_ENABLED */

    /* Enables or disables support of CarWakeUp bit evaluation in received
     * NM PDUs.
     */
    boolean CarWakeUpRxEnabled;
#endif /* UDPNM_VER_R19_11 */
#endif /* STD_ON == UDPNM_CAR_WAKEUP_RX_ENABLED */

#if (STD_ON == UDPNM_IMMEDIATE_TRANSMIT_ENABLED)
    /* Defines the immediate NM PDU cycle time in seconds which is used for
     * UdpNmImmediateNmTransmissions NM PDU transmissions.
     */
    uint16 ImmediateNmCycleTime;
#endif /* STD_ON == UDPNM_IMMEDIATE_TRANSMIT_ENABLED */

    /* Defines the number of immediate NM PDUs which shall be transmitted.*/
    uint8 ImmediateNmTransmissions;

    /* Call cycle in ms of UdpNm_MainFunction. */
    uint16 MainFunctionPeriod;

    /* Time offset in the periodic transmission node.*/
    uint16 MsgCycleOffset;

    /* Period of a NM PDU in seconds.*/
    uint16 MsgCycleTime;

    /* Transmission Timout of NM-message. */
    uint16 MsgTimeoutTime;

#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
    /* Pre-processor switch for enabling the node detection support. */
    boolean NodeDetectionEnabled;
#endif /* UDPNM_NODE_DETECTION_ENABLED == STD_ON */

#if UDPNM_NODE_ID_ENABLED == STD_ON
    /* Pre-processor switch for enabling the source node identifier. */
    boolean NodeIdEnabled;
#endif /* UDPNM_NODE_ID_ENABLED == STD_ON */

    /* Node identifier of local node.*/
    uint8 NodeId;

    /*Defines the position of the control bit vector within the NM PACKET.*/
    UdpNm_PduPositionType PduCbvPosition;

    /* Defines the length of the NM PACKET in bytes.*/
    uint8 UdpNmPduLength;

    /*Defines the position of the source node identifier within the NM PACKET*/
    UdpNm_PduPositionType PduNidPosition;

#if (STD_ON == UDPNM_GLOBAL_PN_ENABLED)
    /* Enables or disables support of partial networking. */
    boolean PnEnabled;

    /* Specifies if UdpNm calculates the PN request information for ERA */
    boolean PnEraCalcEnabled;

    /* Specifies if UdpNm performs an additional transition
     * from Network Mode to Repeat Message State (true) or not (false).
     */
    boolean PnHandleMultipleNetworkRequests;
#endif /* STD_ON == UDPNM_GLOBAL_PN_ENABLED */

    /* Timeout for Remote Sleep Indication. */
    uint16 RemoteSleepIndTime;

    /* Timeout for Repeat Message State. */
    uint16 RepeatMessageTime;

#if UDPNM_REPEAT_MSG_IND_ENABLED == STD_ON
    /*
     * Enable/disable the notification that a RepeatMessageRequest bit has
     * been received.
     */
    boolean RepeatMsgIndEnabled;
#endif /* UDPNM_REPEAT_MSG_IND_ENABLED == STD_ON */

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF) && (UDPNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON)
    /* Specifies if first message request in UdpNm is repeated until accepted
     * by SoAd.
     */
    boolean RetryFirstMessageRequest;
#endif

    /* If this parameter is disabled Prepare Bus-Sleep Mode is left after
     * UdpNmWaitBusSleepTime.
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
#if (STD_ON == UDPNM_GLOBAL_PN_ENABLED)
    /*
     * Reference to a Pdu in the COM-Stack.The SduRef is required for every
     * UdpNm Channel,because ERA is reported per channel.
     */
    PduIdType PnEraRxNSduRef;
#endif /* STD_ON == UDPNM_PN_ERA_CALC_ENABLED */

    /* This container describes the UdpNm RX PDU's. */
    P2CONST(UdpNm_RxPduType, TYPEDEF, UDPNM_CONST) RxPdu;
    uint8 RxPduNum;

    /* This container describes the UdpNm TX PDU's. */
    P2CONST(UdpNm_TxPduType, TYPEDEF, UDPNM_CONST) TxPdu;

    uint8 UserDataLength;
    uint8 UserDataOffset;
#if (STD_ON == UDPNM_COM_USERDATA_SUPPORT)
    /* Preprocessor switch for enabling the Tx path of Com UserData. */
    P2CONST(UdpNm_UserDataTxPduType, TYPEDEF, UDPNM_CONST) UserDataTxPdu;
#endif /* STD_ON == UDPNM_COM_USERDATA_SUPPORT */
} UdpNm_ChannelConfigType;

#if (STD_ON == UDPNM_GLOBAL_PN_ENABLED)
typedef struct
{
    /* Index of the filter mask byte. Specifies the position within the filter
     * mask byte array.
     */
    uint8 PnFilterMaskByteIndex;

    /* Parameter to configure the filter mask byte. */
    uint8 PnFilterMaskByteValue;
} UdpNm_PnFilterMaskByteType;

typedef struct
{
    /* Specifies the length of the PN request information in the NM PDU */
    uint8 UdpNmPnInfoLength;

    /* Specifies the offset of the PN request information in the NM PDU. */
    uint8 UdpNmPnInfoOffset;

    /* PN information configuratio */
    P2CONST(UdpNm_PnFilterMaskByteType, TYPEDEF, UDPNM_CONST) PnFilterMaskByte;
} UdpNm_PnInfoType;
#endif

typedef struct
{
#if (STD_ON == UDPNM_GLOBAL_PN_ENABLED)
    /* Specifies the runtime of the reset timer in seconds. */
    uint16 PnResetTime;
#endif

#if (STD_ON == UDPNM_PN_EIRA_CALC_ENABLED)
    /* Reference to a Pdu in the COM-Stack. */
    PduIdType UdpNmPnEiraRxNSduRef;
#endif

    /* This container contains the channel-specific configuration parameters of
     * the UdpNm. */
    P2CONST(UdpNm_ChannelConfigType, TYPEDEF, UDPNM_CONST) ChannelConfig;

#if (STD_ON == UDPNM_GLOBAL_PN_ENABLED)
    /* PN information configuration */
    P2CONST(UdpNm_PnInfoType, TYPEDEF, UDPNM_CONST) PnInfo;
#endif
} UdpNm_ConfigType;

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

extern CONST(UdpNm_ConfigType, UDPNM_CONST) UdpNm_Config;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/******************************************************************************/
/*
 * Brief               Initialize the complete UdpNm module.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      UdpNmConfigPtr, Pointer to a selected configuration
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void UdpNm_Init(const UdpNm_ConfigType* UdpNmConfigPtr);

/******************************************************************************/
/*
 * Brief               Passive startup of the AUTOSAR UdpNm.
 * ServiceId           0x0e
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK:Passive startup of network management has failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_PassiveStartUp(NetworkHandleType nmChannelHandle);

#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
/******************************************************************************/
/*
 * Brief               Request the network, since ECU needs to communicate on
 *                     the bus.Network state shall be changed to 'requested'.
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
Std_ReturnType UdpNm_NetworkRequest(NetworkHandleType nmChannelHandle);
#endif /*STD_OFF == UDPNM_PASSIVE_MODE_ENABLED*/

#if (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED)
/******************************************************************************/
/*
 * Brief               Release the network,since ECU doesn't have to communicate
 *                     on the bus.Network state shall be changed to 'released'.
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
Std_ReturnType UdpNm_NetworkRelease(NetworkHandleType nmChannelHandle);
#endif /*STD_OFF == UDPNM_PASSIVE_MODE_ENABLED*/

#if (STD_ON == UDPNM_COM_CONTROL_ENABLED)
/******************************************************************************/
/*
 * Brief               Disable the NM PDU transmission ability due to a ISO14229
 *                     Communication Control (0x28) service.
 * ServiceId           0x0c
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Disabling of NM PDU transmission ability has
 *                               failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_DisableCommunication(NetworkHandleType nmChannelHandle);
#endif /* STD_ON == UDPNM_COM_CONTROL_ENABLED*/

#if (STD_ON == UDPNM_COM_CONTROL_ENABLED)
/******************************************************************************/
/*
 * Brief               Enable the NM PDU transmission ability due to a ISO14229
 *                     Communication Control (0x28) service.
 * ServiceId           0x0d
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Enabling of NM PDU transmission ability has
 *                               failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_EnableCommunication(NetworkHandleType nmChannelHandle);
#endif /* STD_ON == UDPNM_COM_CONTROL_ENABLED */

#if (UDPNM_USER_DATA_ENABLED == STD_ON) && (UDPNM_COM_USERDATA_SUPPORT == STD_OFF) \
    && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
/******************************************************************************/
/*
 * Brief               Set user data for all NM messages transmitted on the bus
 *                     after this function has returned without error.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 *                     nmUserDataPtr, Pointer where the user data for the next
 *                     transmitted NM message shall be copied from.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Setting of user data has failed.
 * Trace               SWS_UdpNm_00312
 */
/******************************************************************************/
Std_ReturnType UdpNm_SetUserData(NetworkHandleType nmChannelHandle, const uint8* nmUserDataPtr);
#endif

#if (STD_ON == UDPNM_USER_DATA_ENABLED)
/******************************************************************************/
/*
 * Brief               Get user data from the most recently received NM message.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     nmUserDataPtr, Pointer where user data out of the most
 *                     recently received NM PDU shall be copied to
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Getting of user data has failed.
 */
/******************************************************************************/
Std_ReturnType UdpNm_GetUserData(NetworkHandleType nmChannelHandle, uint8* nmUserDataPtr);
#endif /* STD_ON ==  UDPNM_USER_DATA_ENABLED */

/**
 * @brief       Get node identifier from the most recently received NM PDU.
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
extern Std_ReturnType UdpNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr);

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
extern Std_ReturnType UdpNm_GetLocalNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr);

/**
 * @brief       Set Repeat Message Request Bit for all NM messages transmitted on the bus after this function has
 *              returned without error.
 * @id          0x08
 * @synchronous Asynchronous
 * @reentrancy  Reentrant (but not for the same NM-Channel)
 * @param[in]   nmChannelHandle Identification of the NM-channel
 * @retval      E_OK            No error
 * @retval      E_NOT_OK        Setting of Repeat Message Request Bit has failed or is not configured for this network
 *                              handle.
 */
extern Std_ReturnType UdpNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle);

/**
 * @brief       Get the whole PDU data out of the most recently received NM message.
 * @id          0x0a
 * @synchronous Synchronous
 * @reentrancy  Reentrant
 * @param[in]   nmChannelHandle Identification of the NM-channel
 * @param[out]  nmPduDataPtr    Pointer where NM PDU shall be copied to.
 * @retval      E_OK            No error
 * @retval      E_NOT_OK        Getting of NM PDU Data has failed or is not configured for this network handle.
 */
extern Std_ReturnType UdpNm_GetPduData(NetworkHandleType nmChannelHandle, uint8* nmPduDataPtr);

/******************************************************************************/
/*
 * Brief               Returns the state and the mode of the network management.
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     nmStatePtr, Pointer where state of the network
 *                                 management shall be copied to
 *                     nmModePtr, Pointer where the mode of the network
 *                                management shall be copied to
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Getting of NM state has failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_GetState(NetworkHandleType nmChannelHandle, Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr);

#if (STD_ON == UDPNM_VERSION_INFO_API)
/******************************************************************************/
/*
 * Brief               Returns the version information of this module.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     versioninfo, Pointer to where to store the version
 *                                  information of this module
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void UdpNm_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif /* STD_ON == UDPNM_VERSION_INFO_API */

#if ((STD_ON == UDPNM_BUS_SYNCHRONIZATION_ENABLED) && (STD_OFF == UDPNM_PASSIVE_MODE_ENABLED))
/******************************************************************************/
/*
 * Brief               Request bus synchronization.
 * ServiceId           0x14
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Requesting of bus synchronization has failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_RequestBusSynchronization(NetworkHandleType nmChannelHandle);
#endif

#if (STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED)
/******************************************************************************/
/*
 * Brief               Check if remote sleep indication takes place or not.
 * ServiceId           0x11
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 * Param-Name[out]     nmRemoteSleepIndPtr, Pointer where check result of
 *                     remote sleep indication shall be copied to
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Checking of remote sleep indication bits failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_CheckRemoteSleepIndication(NetworkHandleType nmChannelHandle, boolean* nmRemoteSleepIndPtr);
#endif /* STD_ON == UDPNM_REMOTE_SLEEP_IND_ENABLED */

#if (STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT)
/******************************************************************************/
/*
 * Brief               Sets the NM coordinator ID in the control bit vector of
 *                     each NM message.
 * ServiceId           0x12
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant (but not for the same NM-Channel)
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 *                     nmCoordBits, 2 bit value to set the NM coordinator ID in
 *                     the control bit vector of each NM message
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Setting the coordinator ID bits has failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_SetCoordBits(NetworkHandleType nmChannelHandle, uint8 nmCoordBits);
#endif /*STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT*/

#if (STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT)
/******************************************************************************/
/*
 * Brief               Set the NM Coordinator Sleep Ready bit in the Control
 *                     Bit Vector.
 * ServiceId           0x16
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      nmChannelHandle, Identification of the NM-channel
 *                     nmSleepReadyBit, Value written to ReadySleep Bit in CBV
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: No error
 *                     E_NOT_OK: Set of remote sleep indication bit has failed
 */
/******************************************************************************/
Std_ReturnType UdpNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle, boolean nmSleepReadyBit);
#endif /* STD_ON == UDPNM_COORDINATOR_SYNC_SUPPORT */

#if (STD_ON == UDPNM_COM_USERDATA_SUPPORT)
/******************************************************************************/
/*
 * Brief               Requests transmission of a PDU.
 * ServiceId           0x49
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the
 *                     same PduId.
 * Param-Name[in]      TxPduId, Identifier of the PDU to be transmitted
 *                     PduInfoPtr, Length of and pointer to the PDU data and
 *                                 pointer to Meta Data.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Transmit request has been accepted.
 *                     E_NOT_OK: Transmit request has not been accepted.
 */
/******************************************************************************/
Std_ReturnType UdpNm_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);
#endif /* STD_ON == UDPNM_COM_USERDATA_SUPPORT */

/******************************************************************************/
/*
 * Brief               Main function of the UdpNm which processes the algorithm
 *                     describes in that document.
 * ServiceId           0x13
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      channel, channel of the UdpNm
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
void UdpNm_MainFunction(NetworkHandleType channel);

#endif /* UDPNM_H_ */
