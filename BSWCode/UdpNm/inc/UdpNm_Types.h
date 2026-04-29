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
 **  @file               : UdpNm_Types.h
 **  @author             : caihong.liu
 **  @date               : 2025/01/16
 **  @vendor             : isoft
 **  @description        : Udp Netwprk Manager type define
 **
 ***********************************************************************************************************************/

/* ================================================ misar justifications ============================================ */

#ifndef UDPNM_TYPES_H_
#define UDPNM_TYPES_H_

/* =================================================== inclusions =================================================== */
#include "UdpNm_Cfg.h"
#include "NmStack_Types.h"

#if UDPNM_MULTIPLE_PARTITION_USED == STD_ON
#include "Os.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/**
 * @brief Index of UdpNm channel type.
 */
typedef uint8 UdpNm_ChannelIndexType; /**< @range 0..255 */

/**
 * @brief Network status Types.
 */
typedef enum
{
    UDPNM_NETWORK_REQUESTED, /**< network state is requested @range 0 */
    UDPNM_NETWORK_RELEASED,  /**< network state is released @range 1 */
} UdpNm_NetRequestStatusType;

/**
 * @brief Coordinator indication status type.
 */
typedef enum
{
    UDPNM_CSR_IND_INIT,      /**< Coordinator indication status is init @range 0 */
    UDPNM_CSR_IND_CANCELLED, /**< Coordinator indication status is cancelled @range 1 */
    UDPNM_CSR_IND_INDICATED  /**< Coordinator indication status is indicated @range 2 */
} UdpNm_CSRIndStatusType;

/**
 * @brief Timer kind in UdpNm module.
 */
typedef enum
{
    UDPNM_STATE_HOLE_TIMER, /**< state maintenance timer type @range 0 */
    UDPNM_NM_TIMEOUT_TIMER, /**< NM-Timeout timer @range 1 */
#if UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
    UDPNM_REMOTE_SLEEP_TIMER, /**< Remote node sleep detect timer type. Only used when UdpNmRemoteSleepIndEnabled is set
                                 to TRUE @range 3 */
#endif
    UDPNM_TXMSG_CYCLE_TIMER, /**< periodically send NM-Pdu timer type @range 3 */
    UDPNM_MAX_TIMER_TYPE,    /**< The max kind of UdpNm timer type @range 4 */
} UdpNm_TimersType;

/**
 * @brief Run time variable.
 */
typedef struct
{
    Nm_StateType UdpNmState; /**< Current udpNm state @range 0..7 */
    Nm_ModeType  UdpnmMode;  /**< network mode @range 0..3 */

    UdpNm_NetRequestStatusType NetRequestStatus; /**< Network request status @range 0..1 */
    boolean                    PassiveStartUp;   /**< Passive startup flag @range TRUE/FALSE */

    uint16  TickTimers[UDPNM_MAX_TIMER_TYPE];  /**< All timers used by the channel @range NA */
    boolean TimeoutFlag[UDPNM_MAX_TIMER_TYPE]; /**< All timeout flag used by the channel, only not used
                                                  UDPNM_TXMSG_CYCLE_TIMER @range NA */

#if (UDPNM_USER_DATA_ENABLED == STD_ON) || (UDPNM_NODE_ID_ENABLED == STD_ON) || (UDPNM_NODE_DETECTION_ENABLED == STD_ON)
    uint8 RxPduData[UDPNM_PDU_MAX_LENGTH]; /**< Last rx pdu data @range NA */
#endif
    uint8   TxPduData[UDPNM_PDU_MAX_LENGTH]; /**< Last tx pdu data @range NA */
    boolean RxPduExtFlg;                     /**< Flag of pdu had received @range TRUE/FALSE */

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
    boolean SendNmMsgFlg;  /**< Flag of it is time to send NM-Pdu once @range TRUE/FALSE */
    boolean NetRequestFlg; /**< Indicate if UdpNm_NetworkRequest was called within two MainFunction @range TRUE/FALSE */
#if UDPNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
    boolean RetrySendMsgFlag; /**< First NM Pdu send,if need retry @range TRUE/FALSE */
#endif
#if UDPNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
    uint8 ImmedMsgTxCnt; /**< Immediate transmit nm pdu count @range 0..255 */
#endif
#endif

#if UDPNM_COM_CONTROL_ENABLED == STD_ON
    boolean UdpNmTxEnable; /**< Whether the communication capability is enabled @range TRUE/FALSE */
#endif

#if UDPNM_COORDINATOR_SYNC_SUPPORT == STD_ON
    UdpNm_CSRIndStatusType CoordReadyToSleepInd; /**< Flag of coordinate indication status @range 0..2 */
#endif

#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
    boolean RepeatMessageRequest;  /**< Actively request to enter repeate message state @range TRUE/FALSE */
    boolean RepeatMessageDetected; /**< Passive request to enter repeate message state @range TRUE/FALSE */
#endif
} UdpNm_InnerChannelType;

/**
 * @brief Defines the type of position of the CBV or NID field within the NM PDU.
 */
typedef uint8 UdpNm_PduPositionType; /**< @range 0/1/255 */

/**
 * @name Pdu Position of UdpNm
 * @{
 * @brief Pdu Position
 */
#define UDPNM_PDU_BYTE_0 ((UdpNm_PduPositionType)(0x00u)) /**< NM-PDU uses CBV or NID field in the first byte */
#define UDPNM_PDU_BYTE_1 ((UdpNm_PduPositionType)(0x01u)) /**< NM-PDU uses CBV or NID field in the second byte */
#define UDPNM_PDU_OFF    ((UdpNm_PduPositionType)(0xFFu)) /**< NM-PDU not used CBV or NID field */
/**< @} */

/**
 * @brief PC item structure in channel configuration.
 */
typedef struct
{
#if UDPNM_MULTIPLE_PARTITION_USED == STD_ON
    uint16 ApplicationID; /**< Application Id of this channel @range 0..65535 */
#endif
#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
    boolean ActiveWakeupBitEnabled; /**< Enables/Disables the handling of the Active Wakeup Bit in the UdpNm module.
                                       @range TRUE/FALSE */
#endif

#if UDPNM_GLOBAL_PN_ENABLED == STD_ON
    boolean AllNmMessagesKeepAwake; /**< Specifies if UdpNm drops irrelevant NM PDUs. @range TRUE/FALSE */
#endif

#if UDPNM_CAR_WAKEUP_RX_ENABLED == STD_ON
    uint8   CarWakeUpBitPosition;   /**< Specifies the Bit position of the CWU within the NM PDU. @range 0..255 */
    uint8   CarWakeUpBytePosition;  /**< Specifies the Byte position of the CWU within the NM PDU. @range 0..255 */
    boolean CarWakeUpFilterEnabled; /**< Specifies if CWU filtering is supported @range TRUE/FALSE */

#if UDPNM_CAR_WAKEUP_FILTER_ENABLED == STD_ON
    uint8 CarWakeUpFilterNodeId; /**< Source node identifier for CWU filtering. @range 0..255 */
#endif

    boolean CarWakeUpRxEnabled; /**< Enables or disables support of CarWakeUp bit evaluation in received NM PDUs. @range
                                   TRUE/FALSE */
#endif

#if UDPNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
    uint16 ImmediateNmCycleTime; /**< Defines the immediate NM PDU cycle time in seconds which is used for
                                     UdpNmImmediateNmTransmissions NM PDU transmissions. @range 0..65535 */
#endif

    uint8 ImmediateNmTransmissions; /**< Defines the number of immediate NM PDUs which shall be transmitted. @range
                                       0..255 */
    uint16 MainFunctionPeriod;      /**< Call cycle in ms of UdpNm_MainFunction. @range 0..65535 */

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
    uint16 MsgCycleTime; /**< Period of a NM PDU in seconds. @range 0..65535 */
#endif

#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
    boolean
        NodeDetectionEnabled; /**< Pre-processor switch for enabling the node detection support. @range TRUE/FALSE */
#endif

    boolean NodeIdEnabled; /**< Pre-processor switch for enabling the source node identifier. @range TRUE/FALSE */
    UdpNm_PduPositionType
        PduCbvPosition; /**< Defines the position of the control bit vector within the NM PACKET. @range 0/1/255 */
    UdpNm_PduPositionType
        PduNidPosition; /**< Defines the position of the source node identifier within the NM PACKET @range 0/1/255 */

#if UDPNM_GLOBAL_PN_ENABLED == STD_ON
    boolean PnHandleMultipleNetworkRequests; /**< Specifies if UdpNm performs an additional transition from Network Mode
                                                 to Repeat Message State (true) or not (false). @range TRUE/FALSE */
#endif

#if UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
    uint16 RemoteSleepIndTime; /**< Timeout for Remote Sleep Indication. @range 0..65535 */
#endif

    uint16 RepeatMessageTime; /**< Timeout for Repeat Message State. @range 0..65535 */

#if UDPNM_REPEAT_MSG_IND_ENABLED == STD_ON
    boolean RepeatMsgIndEnabled; /**< Enable/disable the notification that a RepeatMessageRequest bit has been received.
                                    @range TRUE/FALSE */
#endif

#if UDPNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
    boolean RetryFirstMessageRequest; /**< Specifies if first message request in UdpNm is repeated until accepted
                                          by SoAd. @range TRUE/FALSE */
#endif

    boolean StayInPbsEnabled; /**< If this parameter is disabled Prepare Bus-Sleep Mode is left after
                                  UdpNmWaitBusSleepTime. @range TRUE/FALSE */
    uint16 TimeoutTime; /**< It denotes the time in seconds how long the NM shall stay in the Ready Sleep State before
                            transition into the Prepare Bus-Sleep Mode is initiated. @range 0..65535 */
    uint16            WaitBusSleepTime;     /**< Timeout for bus calm down phase. @range 0..65535 */
    NetworkHandleType ComMNetworkHandleRef; /**< This reference points to the unique channel defined by the ComMChannel
                                                and provides access to the unique channel index value in ComMChannelId.
                                               @range 0..255 */
    UdpNm_InnerChannelType* RuntimePtr;     /**< Runtime variable address for each channel. @range NA */
} UdpNm_ChannelLConfigType;

/**
 * @brief This container is used to configure the Rx PDU properties that are used for the UdpNm Channel.
 */
typedef struct
{
    PduIdType RxPduId; /**< This parameter defines the Rx PDU ID of the CanIf L-PDU range that is associated with this
                          CanNm channel. @range 0..65535 */
} UdpNm_RxPduType;

/**
 * @brief This container contains the UdpNmTxConfirmationPduId and the UdpNmTxPduRef.
 */
typedef struct
{
    PduIdType TxConfirmationPduId; /**< Handle Id to be used by the Lower Layer to confirm the transmission of the
                                      UdpNmTx Pdu to the LowerLayer. @range 0..65535 */
    PduIdType LowerLayerPduId;     /**< The macro value of the configuration when UdpNmTxPduRef is referenced by the
                                      configuration CanIfTxPduCfg in CanIf. @range 0..65535 */
} UdpNm_TxPduType;

#if UDPNM_COM_USERDATA_SUPPORT == STD_ON
/**
 * @brief This container is used to configure the UserNm PDU.
 */
typedef struct
{
    PduIdType TxUserDataPduId; /**< This parameter defines the Handle ID of the NM User Data I-PDU. @range 0..65535 */
    PduIdType UpperLayerPduId; /**< The macro value of the configuration when UdpNmTxUserDataPduRef is referenced
                                   by the configuration PduRDestPdu in PduR. @range 0..65535 */
} UdpNm_UserDataTxPduType;
#endif

/**
 * @brief PB entry structure in channel configuration.
 */
typedef struct
{
    uint16 MsgCycleOffset; /**< Time offset in the periodic transmission node. @range 0..65535 */
#if UDPNM_NODE_ID_ENABLED == STD_ON
    uint8 NodeId; /**< Node identifier of local node. @range 0..255 */
#endif

#if UDPNM_GLOBAL_PN_ENABLED == STD_ON
    boolean PnEnabled; /**< Enables or disables support of partial networking. @range TRUE/FALSE */
#endif
    uint8 ChannelPduLength; /**< Maximum pdu length for this channel configuration. @range 0..255 */
#if (UDPNM_COM_USERDATA_SUPPORT == STD_ON) || (UDPNM_USER_DATA_ENABLED == STD_ON)
    uint8 UserDataOffset; /**< The calculated location of the UserData segment in NM-Pdu. @range 0..255 */
    uint8 UserDataLength; /**< The calculated location of the UserData segment in NM-Pdu. @range 0..255 */
#endif
    const UdpNm_RxPduType* RxPdu;    /**< This container describes the UdpNm RX PDU's. @range NA */
    uint8                  RxPduNum; /**< The number of rx pdu had configed in this channel @range 0..255 */

    const UdpNm_TxPduType* TxPdu; /**< This container describes the UdpNm TX PDU's. @range NA */

#if UDPNM_COM_USERDATA_SUPPORT == STD_ON
    const UdpNm_UserDataTxPduType* UserDataTxPdu; /**< This container describes the UdpNm UserDataTx PDU's. @range NA */
#endif
} UdpNm_ChannelPBConfigType;

/**
 * @brief Definition of datatype UdpNm_ConfigType.
 */
typedef struct
{
    const UdpNm_ChannelPBConfigType* ChlPBCfgPtr; /**< UdpNm channel PB configuration data address @range NA */
} UdpNm_ConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif
#endif
