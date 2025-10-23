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
 **  @file               : CanNm_Types.h
 **  @author             : caihong.liu
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Can network manager type definitions
 **
 ***********************************************************************************************************************/

/* ================================================ misar justifications ============================================ */

#ifndef CANNM_TYPES_H_
#define CANNM_TYPES_H_

/* =================================================== inclusions =================================================== */
#include "CanNm_Cfg.h"
#include "NmStack_Types.h"

#if CANNM_MULTIPLE_PARTITION_USED == STD_ON
#include "Os.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/**
 * @brief Index of CanNm channel type.
 */
typedef uint8 CanNm_ChannelIndexType; /**< @range 0..255 */

/**
 * @brief Network status Types.
 */
typedef enum CanNm_NetRequestStatusTag
{
    CANNM_NETWORK_REQUESTED, /**< network state is requested @range 0 */
    CANNM_NETWORK_RELEASED,  /**< network state is released @range 1 */
} CanNm_NetRequestStatusType;

/**
 * @brief Coordinator indication status type.
 */
typedef enum CanNm_CSRIndStatusTag
{
    CANNM_CSR_IND_INIT,      /**< Coordinator indication status is init @range 0 */
    CANNM_CSR_IND_CANCELLED, /**< Coordinator indication status is cancelled @range 1 */
    CANNM_CSR_IND_INDICATED  /**< Coordinator indication status is indicated @range 2 */
} CanNm_CSRIndStatusType;

/**
 * @brief Timer kind in CanNm module.
 */
typedef enum CanNm_TimersTag
{
    CANNM_STATE_HOLE_TIMER, /**< state maintenance timer type @range 0 */
    CANNM_NM_TIMEOUT_TIMER, /**< NM-Timeout timer @range 1 */
#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
    CANNM_TXMSG_TIMEOUT_TIMER, /**< NM-Pdu transmision timer type. Message Only used when CanNmGlobalPnSupport is set to
                                  TRUE @range 2 */
#endif
#if CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
    CANNM_REMOTE_SLEEP_TIMER, /**< Remote node sleep detect timer type. Only used when CanNmRemoteSleepIndEnabled is set
                                 to TRUE @range 3 */
#endif
    CANNM_TXMSG_CYCLE_TIMER, /**< periodically send NM-Pdu timer type @range 4 */
    CANNM_MAX_TIMER_TYPE,    /**< The max kind of CanNm timer type @range 5 */
} CanNm_TimersType;

/**
 * @brief Run time variable.
 */
typedef struct CanNm_InnerChannelTag
{
    Nm_StateType CanNmState; /**< Current canNm state @range 0..7 */
    Nm_ModeType  CanNmMode;  /**< Current network mode @range 0..3 */

    CanNm_NetRequestStatusType NetRequestStatus; /**< Network request status @range 0..1 */
    boolean                    PassiveStartUp;   /**< Passive startup flag @range TRUE/FALSE */

    uint16  TickTimers[CANNM_MAX_TIMER_TYPE];  /**< All timers used by the channel @range NA */
    boolean TimeoutFlag[CANNM_MAX_TIMER_TYPE]; /**< All timeout flag used by the channel, only not used
                                                  CANNM_TXMSG_CYCLE_TIMER @range NA */

#if (CANNM_USER_DATA_ENABLED == STD_ON) || (CANNM_NODE_ID_ENABLED == STD_ON) || (CANNM_NODE_DETECTION_ENABLED == STD_ON)
    uint8 RxPduData[CANNM_PDU_MAX_LENGTH]; /**< Last rx pdu data @range NA */
#endif
    uint8 TxPduData[CANNM_PDU_MAX_LENGTH]; /**< Last tx pdu data @range NA */

    boolean RxPduExtFlg; /**< Flag of pdu had received @range TRUE/FALSE */

#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
    boolean SendNmMsgFlg;  /**< Flag of it is time to send NM-Pdu once @range TRUE/FALSE */
    boolean NetRequestFlg; /**< Indicate if CanNm_NetworkRequest was called within two MainFunction @range TRUE/FALSE */
#if CANNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
    boolean RetrySendMsgFlag; /**< First NM Pdu send,if need retry @range TRUE/FALSE */
#endif
#if CANNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
    uint8 ImmedMsgTxCnt; /**< Immediate transmit nm pdu count @range 0..255 */
#endif
#endif

#if CANNM_COM_CONTROL_ENABLED == STD_ON
    boolean CanNmTxEnable; /**< Whether the communication capability is enabled @range TRUE/FALSE */
#endif

#if CANNM_COORDINATOR_SYNC_SUPPORT == STD_ON
    CanNm_CSRIndStatusType CoordReadyToSleepInd; /**< Flag of coordinate indication status @range 0..2 */
#endif

#if CANNM_NODE_DETECTION_ENABLED == STD_ON
    boolean RepeatMessageRequest;  /**< Actively request to enter repeate message state @range TRUE/FALSE */
    boolean RepeatMessageDetected; /**< Passive request to enter repeate message state @range TRUE/FALSE */
#endif

#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
    boolean PnFilterEnable; /**< Whether the function CanNm_ConfirmPnAvailability has called @range TRUE/FALSE */
#endif
} CanNm_InnerChannelType;

/**
 * @brief Defines the type of position of the CBV or NID field within the NM PDU.
 */
typedef uint8 CanNm_PduPositionType; /**< @range 0/1/255 */

/**
 * @name Pdu Position of CanNm
 * @{
 * @brief Pdu Position
 */
#define CANNM_PDU_BYTE_0 ((CanNm_PduPositionType)(0x00u)) /**< NM-PDU uses CBV or NID field in the first byte */
#define CANNM_PDU_BYTE_1 ((CanNm_PduPositionType)(0x01u)) /**< NM-PDU uses CBV or NID field in the second byte */
#define CANNM_PDU_OFF    ((CanNm_PduPositionType)(0xFFu)) /**< NM-PDU not used CBV or NID field */
/** @} */

/**
 * @brief PC item structure in channel configuration.
 */
typedef struct CanNm_ChannelConfigTag
{
#if CANNM_MULTIPLE_PARTITION_USED == STD_ON
    uint16 ApplicationID; /**< Application Id of this channel @range 0..65535 */
#endif
#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
    boolean ActiveWakeupBitEnabled; /**< Enables/Disables the handling of the Active Wakeup Bit in the CanNm module.
                                       @range TRUE/FALSE */
#endif

#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
    boolean AllNmMessagesKeepAwake; /**< Specifies if CanNm drops irrelevant NM PDUs. @range TRUE/FALSE */
#endif

#if CANNM_BUSLOAD_REDUCTION_ENABLED == STD_ON
    boolean BusLoadReductionActive; /**< This parameter defines if bus load reduction for the respective NM channel is
                                       active or not. @range TRUE/FALSE */
#endif

#if CANNM_CAR_WAKEUP_RX_ENABLED == STD_ON
    uint8   CarWakeUpBitPosition;   /**< Specifies the Bit position of the CWU within the NM PDU. @range 0..255 */
    uint8   CarWakeUpBytePosition;  /**< Specifies the Byte position of the CWU within the NM PDU. @range 0..255 */
    boolean CarWakeUpFilterEnabled; /**< Specifies if CWU filtering is supported @range TRUE/FALSE */

#if CANNM_CAR_WAKEUP_FILTER_ENABLED == STD_ON
    uint8 CarWakeUpFilterNodeId; /**< Source node identifier for CWU filtering. @range 0..255 */
#endif

    boolean CarWakeUpRxEnabled; /**< Enables or disables support of CarWakeUp bit evaluation in received NM PDUs. @range
                                   TRUE/FALSE */
#endif

#if CANNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
    uint16 ImmediateNmCycleTime; /**< Defines the immediate NM PDU cycle time in seconds which is used for
                                    CanNmImmediateNmTransmissions NM PDU transmissions. @range 0..65535 */
#endif

    uint8 ImmediateNmTransmissions; /**< Defines the number of immediate NM PDUs which shall be transmitted. @range
                                       0..255 */

#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
    uint16 MsgCycleTime; /**< Period of a NM PDU in seconds. @range 0..65535 */
#endif

#if CANNM_NODE_DETECTION_ENABLED == STD_ON
    boolean
        NodeDetectionEnabled; /**< Pre-processor switch for enabling the node detection support. @range TRUE/FALSE */
#endif

#if CANNM_NODE_ID_ENABLED == STD_ON
    boolean NodeIdEnabled; /**< Pre-processor switch for enabling the source node identifier. @range TRUE/FALSE */
#endif

    CanNm_PduPositionType
        PduCbvPosition; /**< Defines the position of the control bit vector within the NM PACKET. @range 0/1/255 */
    CanNm_PduPositionType
        PduNidPosition; /**< Defines the position of the source node identifier within the NM PACKET @range 0/1/255 */

#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
    boolean PnHandleMultipleNetworkRequests; /**< Specifies if CanNm performs an additional transition from Network Mode
                                                to Repeat Message State (true) or not (false). @range TRUE/FALSE */
#endif

#if CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
    uint16 RemoteSleepIndTime; /**< Timeout for Remote Sleep Indication. @range 0..65535 */
#endif

    uint16 RepeatMessageTime; /**< Timeout for Repeat Message State. @range 0..65535 */

#if CANNM_REPEAT_MSG_IND_ENABLED == STD_ON
    boolean RepeatMsgIndEnabled; /**< Enable/disable the notification that a RepeatMessageRequest bit has been received.
                                    @range TRUE/FALSE */
#endif

#if CANNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
    boolean RetryFirstMessageRequest; /**< Specifies if first message request in CanNm is repeated until accepted by
                                         CanIf. @range TRUE/FALSE */
#endif

    boolean StayInPbsEnabled; /**< If this parameter is disabled Prepare Bus-Sleep Mode is left after
                                 CanNmWaitBusSleepTime. @range TRUE/FALSE */
    uint16 TimeoutTime; /**< It denotes the time in seconds how long the NM shall stay in the Ready Sleep State before
                           transition into the Prepare Bus-Sleep Mode is initiated. @range 0..65535 */
    uint16 WaitBusSleepTime; /**< Timeout for bus calm down phase. @range 0..65535 */
    NetworkHandleType
        ComMNetworkHandleRef;           /**< This reference points to the unique channel defined by the ComMChannel
                                          and provides access to the unique channel index value in ComMChannelId. @range 0..255 */
    CanNm_InnerChannelType* RuntimePrt; /**< Runtime variable address for each channel. @range NA */
} CanNm_ChannelLConfigType;

/**
 * @brief This container is used to configure the Rx PDU properties that are used for the CanNm Channel.
 */
typedef struct CanNm_RxPduTag
{
    PduIdType RxPduId; /**< This parameter defines the Rx PDU ID of the CanIf L-PDU range that is associated with this
                          CanNm channel. @range 0..65535 */
} CanNm_RxPduType;

/**
 * @brief This container contains the CanNmTxConfirmationPduId and the CanNmTxPduRef.
 */
typedef struct CanNm_TxPduTag
{
    PduIdType TxConfirmationPduId; /**< Handle Id to be used by the Lower Layer to confirm the transmission of the
                                      CanNmTx Pdu to the LowerLayer. @range 0..65535 */
    PduIdType LowerLayerPduId;     /**< The macro value of the configuration when CanNmTxPduRef is referenced by the
                                      configuration CanIfTxPduCfg in CanIf. @range 0..65535 */
    boolean TxPduIsTriggerTrans;   /**< CanIfTxPduTriggerTransmit switch of the configuration when CanNmTxPduRef is
                                      referenced in the CanIfTxPduCfg configuration of CanIf. @range TRUE/FALSE */
} CanNm_TxPduType;

#if CANNM_COM_USERDATA_SUPPORT == STD_ON
/**
 * @brief This container is used to configure the UserNm PDU.
 */
typedef struct CanNm_UserDataTxPduTag
{
    PduIdType TxUserDataPduId; /**< This parameter defines the Handle ID of the NM User Data I-PDU. @range 0..65535 */
    PduIdType UpperLayerPduId; /**< The macro value of the configuration when CanNmTxUserDataPduRef is referenced by the
                                  configuration PduRDestPdu in PduR. @range 0..65535 */
} CanNm_UserDataTxPduType;
#endif

/**
 * @brief PB entry structure in channel configuration.
 */
typedef struct CanNm_ChannelPBConfigTag
{
    uint16 MsgCycleOffset; /**< Time offset in the periodic transmission node. @range 0..65535 */
#if CANNM_BUSLOAD_REDUCTION_ENABLED == STD_ON
    uint16 MsgReducedTime; /**< Node specific bus cycle time in the periodic transmission mode
                             with bus load reduction. @range 0..65535 */
#endif

#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF) && (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF)
    uint16 MsgTimeoutTime; /**< Transmission Timout of NM-message. @range 0..65535 */
#endif

#if CANNM_NODE_ID_ENABLED == STD_ON
    uint8 NodeId; /**< Node identifier of local node. @range 0..255 */
#endif

#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
    boolean PnEnabled; /**< Enables or disables support of partial networking. @range TRUE/FALSE */
#endif
    uint8 ChannelPduLength; /**< Maximum pdu length for this channel configuration. @range 0..255 */
#if (CANNM_COM_USERDATA_SUPPORT == STD_ON) || (CANNM_USER_DATA_ENABLED == STD_ON)
    uint8 UserDataOffset; /**< The calculated location of the UserData segment in NM-Pdu. @range 0..255 */
    uint8 UserDataLength; /**< The calculated location of the UserData segment in NM-Pdu. @range 0..255 */
#endif
    const CanNm_RxPduType* RxPdu;    /**< This container describes the CanNm RX PDU's. @range NA */
    uint8                  RxPduNum; /**< The number of rx pdu had configed in this channel @range 0..255 */

    const CanNm_TxPduType* TxPdu; /**< This container describes the CanNm TX PDU's. @range NA */

#if CANNM_COM_USERDATA_SUPPORT == STD_ON
    const CanNm_UserDataTxPduType* UserDataTxPdu; /**< This container describes the CanNm UserDataTx PDU's. @range NA */
#endif
} CanNm_ChannelPBConfigType;

/**
 * @brief Definition of datatype CanNm_ConfigType.
 */
typedef struct CanNm_ConfigTag
{
    const CanNm_ChannelPBConfigType* ChlPBCfgPtr; /**< CanNm channel PB configuration data address @range NA */
} CanNm_ConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif
#endif
