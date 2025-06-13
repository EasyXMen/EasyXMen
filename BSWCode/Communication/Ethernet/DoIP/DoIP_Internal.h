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
**  FILENAME    : DoIP_Internal.h                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhiqiang.huang                                              **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#ifndef DOIP_INTERNAL_H
#define DOIP_INTERNAL_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "DoIP.h"
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
#include "Det.h"
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
#include "SchM_DoIP.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define DOIP_RX_BUFFER_SIZE   (DOIP_MAX_REQUEST_BYTES + 8u)
#define DOIP_TX_BUFFER_SIZE   (DOIP_MAX_RESPONSE_BYTES + 8u)
#define DOIP_SA_TA_LEN        4u
#define DOIP_PROTOCOL_VERSION 0x02u
#define DOIP_PROTOCOL_VERSION_AND_INVERSE \
    (uint16)(((uint16)DOIP_PROTOCOL_VERSION << 8u) | ((uint8)~DOIP_PROTOCOL_VERSION))

#define DOIP_E_INCORRECT_PATTERN_FORMAT              0x00u
#define DOIP_E_UNKNOWN_PAYLOAD_TYPE                  0x01u
#define DOIP_E_MESSAGE_TO_LARGE                      0x02u
#define DOIP_E_OUT_OF_MEMORY                         0x03u
#define DOIP_E_INVALID_PAYLOAD_LENGTH                0x04u

#define DOIP_INVALID_UINT8                           0xFFu
#define DOIP_INVALID_UINT16                          0xFFFFu
#define DOIP_INVALID_UINT32                          0xFFFFFFFFu

#define DOIP_MAX_NON_DIAGNOSTIC_DATA_LENGTH          0x19u
#define DOIP_HEADER_LEN                              8u
#define DoIP_ROUTINE_ACTIVE_RESPONSE_LENGTH          21u
#define DOIP_ALIVECHECK_REQUEST_LENGTH               8u

#define DOIP_GENERIC_HEADER_LEN                      8u
#define DOIP_VEHICLE_IDENT_REQ_PAY_LEN               0u
#define DOIP_ROUTING_ACTIVE_REQ_PAY_LEN              7u
#define DOIP_ROUTING_ACTIVE_REQ_PAY_LEN_WITH_OEMSPEC 11u /* PRQA S 0791 */ /* MISRA Rule 5.4 */
#define DOIP_ALIVE_CHECK_RESP_PAY_LEN                2u
#define DOIP_ENTITY_STATUS_REQ_PAY_LEN               0u
#define DOIP_POWER_MODE_REQ_PAY_LEN                  0u

#define DOIP_VIN_LEN                                 17u
#define DOIP_LOGICAL_ADDR_LEN                        2u
#define DOIP_EID_LEN                                 6u
#define DOIP_GID_LEN                                 6u
#define DOIP_FURTHER_ACTION_LEN                      1u
#define DOIP_SYNC_STATUS_LEN                         1u

#define DOIP_LOGICAL_ADDR_OFFSET                     DOIP_VIN_LEN
#define DOIP_EID_OFFSET                              DOIP_LOGICAL_ADDR_OFFSET + DOIP_LOGICAL_ADDR_LEN
#define DOIP_GID_OFFSET                              DOIP_EID_OFFSET + DOIP_EID_LEN
#define DOIP_FURTHER_ACTION_OFFSET                   DOIP_GID_OFFSET + DOIP_GID_LEN
#define DOIP_SYNC_STATUS_OFFSET                      DOIP_FURTHER_ACTION_OFFSET + DOIP_FURTHER_ACTION_LEN

#define DOIP_NO_REQ_W4_RSP                           0xFFFFu /*formanufacturer specific use, indicate there is no outgoing req wait for rsp*/
#define DOIP_GENERIC_DOIP_HEADER_NACK                0x0000u /*Generic DoIP header negative acknowledge*/
#define DOIP_VEHICLE_INDENT_REQ                      0x0001u /*client*/
#define DOIP_VEHICLE_INDENT_REQ_WITH_EID             0x0002u /*client*/
#define DOIP_VEHICLE_INDENT_REQ_WITH_VIN             0x0003u /*client*/
#define DOIP_VEHICLE_ANNOUNCE_OR_VEHICLE_INDENT_RSP  0x0004u
#define DOIP_ROUTING_ACTIVATION_REQ                  0x0005u /*client*/
#define DOIP_ROUTING_ACTIVATION_RSP                  0x0006u
#define DOIP_ALIVE_CHECK_REQ                         0x0007u
#define DOIP_ALIVE_CHECK_RSP                         0x0008u /*client*/
#define DOIP_ENTITY_STATUS_REQ                       0x4001u /*client*/
#define DOIP_ENTITY_STATUS_RSP                       0x4002u
#define DOIP_DIAG_POWER_MODE_INFO_REQ                0x4003u /*client*/
#define DOIP_DIAG_POWER_MODE_INFO_RSP                0x4004u
#define DOIP_DIAG_MSG                                0x8001u /*client*/
#define DOIP_DIAG_MSG_ACK                            0x8002u
#define DOIP_DIAG_MSG_NACK                           0x8003u

#define DOIP_DIAG_NACK_CODE_NONE                     0xFFu
#define DOIP_DIAG_NACK_CODE_INVALID_SOURCE_ADDR      0x02u
#define DOIP_DIAG_NACK_CODE_UNKNOWN_TARGET_ADDR      0x03u
#define DOIP_DIAG_NACK_CODE_MSG_TOO_LARGE            0x04u
#define DOIP_DIAG_NACK_CODE_OUT_OF_MEMORY            0x05u
#define DOIP_DIAG_NACK_CODE_TARGET_UNREACHABLE       0x06u
#define DOIP_DIAG_NACK_CODE_UNKNOWN_NETWORK          0x07u
#define DOIP_DIAG_NACK_CODE_TRANSPORT_PROTOCOL_ERROR 0x08u

#define DOIP_VIN_GID_SYNC_COMPLETE                   0x00u /*VIN and/or GID are synchronized*/
#define DOIP_VIN_GID_NOT_SYNC                        0x01u /*incomplete: VIN and GID are not synchronized*/

#define DOIP_SOCKET_AUTO_OPEN                        STD_ON

#if !defined(DOIP_LOCAL_INLINE)
#define DOIP_LOCAL_INLINE static inline
#endif /* !defined DOIP_LOCAL_INLINE */

#if !defined(DOIP_LOCAL)
#define DOIP_LOCAL static
#endif /* !defined DOIP_LOCAL */
/*******************************************************************************
**                      Private Data Types                                    **
*******************************************************************************/
/* define DOIP module initiation status */
typedef enum
{
    DOIP_INITED = 0u,
    DOIP_UNINIT
} DOIP_InitStatusType;

/*Socket Assignment Result Type*/
typedef enum
{
    DOIP_SOCKET_ASSIGNMENT_FAILED,
    DOIP_SOCKET_ASSIGNMENT_SUCCESSFUL,
    DOIP_SOCKET_ASSIGNMENT_PENDING
} DoIP_SocketAssignmentResultType;

/*Vehicle Connection state type*/
typedef enum
{
    DOIP_CONN_STATE_IDLE = 0u,
    DOIP_CONN_STATE_STARTED,            /*wait annoucement rsp, send req if timeout*/
    DOIP_CONN_STATE_DISCOVERY_START,    /*announcement received or req send out*/
    DOIP_CONN_STATE_DISCOVERY_WAIT,     /*wait for remote nodes sync*/
    DOIP_CONN_STATE_DISCOVERY_COMPLETE, /*discovery complete,may routing active immediately*/
    DOIP_CONN_STATE_ROUTING_ACTIVE,
    DOIP_CONN_STATE_CONNECT_SUCCESS,
    DOIP_CONN_STATE_TIMEOUT
} DoIPVehicleConnStateType;

/*Vehicle entry state type*/
typedef enum
{
    DOIP_VEHICLE_ENTRY_STATE_IDLE,
    DOIP_VEHICLE_ENTRY_STATE_CONNECTING,
    DOIP_VEHICLE_ENTRY_STATE_CONNECTED /* PRQA S 0779 */ /* MISRA Rule 5.2 */
} DoIPVehicleEntryStateType;

/*vehicle entry type*/
typedef struct
{
    DoIPVehicleEntryStateType state;
} DoIPVehicleEntryType;

/*vehicle connection control type*/
typedef struct
{
    uint8 vehicleStartCnt;
    uint8 waitForActiveCnt;
    DoIPVehicleConnStateType vehicleConnState;
    uint32 connTimer;
    uint32 discoveryWaitTimer;
#if (DOIP_MAX_VEHICLE > 0U)
    DoIPVehicleEntryType vehicleEntry[DOIP_MAX_VEHICLE];
#endif
} DoIPVehicleConnCtrlType;

/*doip entity node type*/
typedef enum
{
    DOIP_NODE_INVALIDE,
    DOIP_NODE_GATEWAY,
    DOIP_NODE_NODE
} DoIPNodeTypeType;

/*pending routing active type*/
typedef struct
{
    boolean pendingFlag;
    uint8 pendingBufferIndex;
    SoAd_SoConIdType pendingSocketConId;
    uint16 pendingTxPduId;
    uint16 pendingSa;

    uint8 responseDataLen; /* Used for auth and confirmation */
    uint8 responseData[4]; /* Used for auth and confirmation */
} PendingRoutingActiveType;

/*node info type, report to upper layer*/
typedef struct
{
    uint8 powerMode;
    uint8 nodeType;
    uint8 maxOpenSocket;
    uint8 currentlyOpenSocket;
    uint32 maxDataSize;
} DoIP_NodeInfoType;

/*vehicle entry type,report to upper layer*/
typedef struct
{
    uint8 vin[DOIP_VIN_LEN];
    uint8 eid[DOIP_EID_LEN];
    uint8 gid[DOIP_GID_LEN];
    uint16 logicalAddr;
} DoIP_VehicleEntryType;

/*doip VinGid Sync state*/
typedef enum
{
    DOIP_VINGID_SYNC_STATE_NONE = 0u,
    DOIP_VINGID_SYNC_STATE_OK,
    DOIP_VINGID_SYNC_STATE_INCOMPLETE
} DoIPVinGidSyncStateType;

/*udp connection type*/
typedef struct
{
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
    DoIPVinGidSyncStateType vinGidSyncState;
    DoIP_VehicleEntryType vehicleEntry;
    DoIP_NodeInfoType nodeInfo;
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/
    DoIP_RoleType role;
    SoAd_SoConIdType socketId;
    uint16 txPduId; /*for soad tx*/
    uint16 outgoingMsgType;
    SoAd_SoConModeType soConState;
    uint32 doipMsgTimer;
} DoIP_UdpConnStatusType;

/*vehicle annoucement connection type*/
typedef struct
{
    SoAd_SoConModeType soConState;
    SoAd_SoConIdType socketId;
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
    boolean vehicleAncTimerFlag;
    uint8 vehicleAncCnt;
    uint32 vehicleAncTimer;
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
} DoIP_VehicleAncConnStatusType;

/*routine active state*/
typedef enum
{
    DOIP_SOCKET_CONNECTION_INVALID,
    DOIP_SOCKET_CONNECTION_REGISTERED
} DoIP_SocketConnStateType;

#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
typedef struct
{
    boolean nrc78Rcvd;
    boolean waitRmtRsp;
    uint8 txNrc78Cnt;
    uint8 lastSid;
    uint32 waitRmtTimer;
    P2CONST(DoIP_GatewayWaitResponseType, AUTOMATIC, DOIP_VAR) waitRspCfg;
} DoIP_GatewayCtrlType;
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/

/*@SWS_DoIP_00002 TCP connection status*/
typedef struct
{
    /*@DoIP-152*/
    /*uint8 vehicleId; remove, but may reused*/
    boolean wait4DiagMsgResponse;
    boolean wait4AliveChkRsp;
    boolean wait4SocketClose;
    /*@DoIP-154*/
    boolean authState;
    boolean confirmState;
    uint8 bufferId;
    uint8 activationType;
    DoIP_RoleType role;
    uint16 rxPduId; /*for soad rx*/
    uint16 txPduId; /*for soad tx*/
    uint16 lowerlayerTxPduId;
    uint16 sa;
    uint16 ta;
    SoAd_SoConIdType socketId;
    /*@DoIP-153*/
    uint32 initInactiveTimer;
    uint32 genInactiveTimer; /*generalInactivityTimer*/
    uint32 diagMsgTimer;
    uint32 aliveChkTimer;
    SoAd_SoConModeType soConnState;
    /*@DoIP-128*/
    DoIP_SocketConnStateType routActiveState;
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
    DoIP_GatewayCtrlType gwCtrl;
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/
} DoIP_TcpConnStatusType;

/*TCP data transmit message type*/
typedef enum
{
    DOIP_NON_TX_MSG,
    DOIP_TX_NON_DIAG_MSG,
    DOIP_TX_DIAG_IF_MSG,
    DOIP_TX_DIAG_TP_MSG
} DoIP_TpTransmitMessageType;

/*buffer occipied or not*/
typedef enum
{
    DOIP_BUFFER_IDLE = 0u,    /*Buffer "idle" state*/
    DOIP_BUFFER_OCCUPIED = 1u /*Buffer "occupied" state*/
} DoIP_BufferStateType;

/*TCP data receive state*/
typedef enum
{
    DOIP_RX_PROCESS_IDLE = 0u,
    DOIP_RX_PROCESS_RECEIVING = 1u,
    DOIP_RX_PROCESS_HEADER_RECEIVING = 2u,
    DOIP_RX_PROCESS_HEADER_RECEIVED = 3u,
    DOIP_RX_PROCESS_CF_RECEIVING = 4u,
    DOIP_RX_PROCESS_DISCARD = 5u
} DoIP_RxProcessStatusType;

/*TCP data transmit state*/
typedef enum
{
    DOIP_TX_PROCESS_IDLE = 0u,
    DOIP_TX_PROCESS_TRANSMITING = 1u,
    DOIP_TX_PROCESS_HEADER_TRANSMITED = 2u,
    DOIP_TX_PROCESS_TRANSMITED = 3u
} DoIP_TxProcessStatusType;

/*buffer for TCP rx message*/
typedef struct
{
    DoIP_BufferStateType rxBufState;
    DoIP_RxProcessStatusType procStatus;
    PduLengthType rxBufPos;
    uint16 payloadType;
    PduLengthType payloadLen;
    PduLengthType rxBufRemain;
    PduLengthType rxDataRemain;
    PduLengthType BS;          /*used for pdur reception*/
    PduIdType pdurRxPduIdRef;  /*DoIP rx uppper(PDUR) pdu id*/
    PduIdType doipSoadRxPduId; /*DoIP rx pdu id,to identify which buffer copy data shall use*/
    uint16 sa;                 /*zzz may not need*/
    uint16 ta;                 /*zzz may not need*/
} RxChannelCtrlType;

/*TP queue*/
typedef struct
{
    uint8 dataPtr[DOIP_TP_QUEUE_BUFFER_SIZE];
} PendingTpPduType;

typedef struct
{
    PduIdType pendingPdurPduId;
    PduInfoType pendingPduInfo;
    PendingTpPduType pendingPdu;
} TxPendingTpQueueType;

typedef struct
{
    uint8 head;
    uint8 tail;
    TxPendingTpQueueType pendingQueue[DOIP_TP_QUEUE_BUFFER_NUM];
} PendingTxQueueTpMsgType;

/*IF queue*/
typedef struct
{
    uint8 metaDataPtr[DOIP_SA_TA_LEN];
    uint8 sduDataPtr[DOIP_IF_QUEUE_BUFFER_SIZE];
} PendingIfPduType;

typedef struct
{
    PduIdType pendingPdurPduId;
    DoIP_TpTransmitMessageType pendingTxMessageType;
    PduInfoType pendingPduInfo;
    PendingIfPduType pendingPdu;
} TxPendingIfQueueType;

typedef struct
{
    uint8 head;
    uint8 tail;
    TxPendingIfQueueType pendingQueue[DOIP_IF_QUEUE_BUFFER_NUM];
} PendingTxQueueIfMsgType;

/*NON diag queue*/
typedef struct
{
    uint8 dataPtr[DOIP_NON_DIAG_QUEUE_BUFFER_SIZE];
} PendingNonDiagPduType;

typedef struct
{
    PendingNonDiagPduType pendingPdu;
    PduIdType pendingSoAdTxPduId;
    PduInfoType pendingPduInfo;
} TxPendingNonDiagQueueType;

typedef struct
{
    uint8 head;
    uint8 tail;
    TxPendingNonDiagQueueType pendingQueue[DOIP_NON_DIAG_QUEUE_BUFFER_NUM];
} PendingTxQueueNonDiagMsgType;

typedef struct
{
    DoIP_BufferStateType bufferState;
    PduLengthType TxBufferPos;
    DoIP_TxProcessStatusType processStatus;
    DoIP_TpTransmitMessageType msgType;
    PduLengthType txMaxDataLength;
    PduIdType pdurTxPduId;    /*DoIP tx pdu id*/
    PduIdType pdurTxPduIdRef; /*DoIP tx upper(PDU) pdu id*/
} TxChannelCtrlType;

/*TCP main buffer container*/
typedef struct
{
    RxChannelCtrlType rxCtrl;
    TxChannelCtrlType txCtrl;
    PendingTxQueueTpMsgType pendingTxTpQueue; /*queue for DoIP_TpTransmit info, eg. meta data*/
    /*queue for diag ack/nack, routing active req/rsp,alive check req/rsp, tcp nack*/
    PendingTxQueueNonDiagMsgType pendingTxNonDiagQueue;
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
    PendingTxQueueIfMsgType pendingTxIfQueue; /*queue for uudt message*/
#endif                                        /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
    uint8 rxChannel[DOIP_RX_BUFFER_SIZE];
    uint8 txChannel[DOIP_TX_BUFFER_SIZE];
} DoIP_TcpInnerBufferType;

/*diag receive handle type*/
typedef struct
{
    uint8 bufIdx;
    uint16 txpduId;
    SoAd_SoConIdType SoConId;
    P2VAR(DoIP_TcpInnerBufferType, AUTOMATIC, DOIP_VAR) buffer; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
} DoIP_DiagRcvHandleType;

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
extern const DoIP_GeneralType DoIP_General;
/*pointer to PCConfig*/
extern P2CONST(DoIP_ConfigType, AUTOMATIC, DOIP_APPL_CONST) DoIP_PBCfgPtr;

#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
/*vechile connection control*/
extern VAR(DoIPVehicleConnCtrlType, DOIP_VAR_INIT) DoIP_VehicleConnCtrl;
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/

/*initialization status*/
extern VAR(DOIP_InitStatusType, DOIP_VAR_INIT) DoIP_InitStatusRunTime;

/*udp connection status*/
extern VAR(DoIP_UdpConnStatusType, DOIP_VAR_CLEARED) DoIP_UdpConnStatus[DOIP_UDP_MAX_CONNECTIONS];

/*tcp connection status*/
extern VAR(DoIP_TcpConnStatusType, DOIP_VAR_CLEARED) DoIP_TcpConnStatus[DOIP_MAX_TCP_CONNECTIONS];

/*vehicle announcement connection status*/
extern VAR(DoIP_VehicleAncConnStatusType, DOIP_VAR_CLEARED)
    DoIP_AncConnStatus[DOIP_UDP_VEHICLEANNOUNCEMENT_CONNECTIONS];

/*tcp internal buffer*/
extern VAR(DoIP_TcpInnerBufferType, DOIP_VAR_CLEARED) DoIP_TcpInnerBuf[DOIP_MAX_TCP_CONNECTIONS];

#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)

/*current connection tcp socket number*/
extern VAR(uint8, DOIP_VAR_INIT) DoIP_CurrentTcpSocketNbr;

/*pengding routing active message*/
extern VAR(PendingRoutingActiveType, DOIP_VAR_CLEARED) DoIP_PendingRoutMsg;

/*GID synchronization flag */
extern VAR(boolean, DOIP_VAR_INIT) DoIP_GidSynFlag;

/*GID synchronization status */
extern VAR(boolean, DOIP_VAR_INIT) DoIP_GidSynStatus;
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/

/*******************************************************************************
**                      Internal Function Declarations                         **
*******************************************************************************/
static inline const DoIPTcpConnectionType* DOIP_TCPCON_CFG(uint8 id)
{
    return &(DoIP_PBCfgPtr->DoIPConnections->DoIPTcpConnection[id]);
}
static inline const DoIPUdpConnectionType* DOIP_UDPCON_CFG(uint8 id)
{
    return &(DoIP_PBCfgPtr->DoIPConnections->DoIPUdpConnection[id]);
}
static inline const DoIPUdpVehicleAnnouncementConnectionType* DOIP_ANCCON_CFG(uint8 id)
{
    return &(DoIP_PBCfgPtr->DoIPConnections->DoIPUdpVehicleAnnouncementConnection[id]);
}

#if (STD_ON == DOIP_DEV_ERROR_DETECT)
static inline void DOIP_DET_REPORT(uint8 ApiId, uint8 ErrorId) /* PRQA S 3219 */ /* MISRA Rule 2.1 */
{
    (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, (ApiId), (ErrorId));
}
#else
#define DOIP_DET_REPORT(ApiId, ErrorId)
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/

/* PRQA S 3219++ */ /* MISRA Rule 2.1 */
static inline boolean DOIP_GENERICHEADER_CHECK(const uint8* dataPtr, PduLengthType SduLength)
/* PRQA S 3219-- */ /* MISRA Rule 2.1 */
{
    uint8 temp = dataPtr[0] ^ 0xffu;

    return (
        ((dataPtr[0] == 1u) || (dataPtr[0] == 2u) || (dataPtr[0] == 0xffu)) && (temp == dataPtr[1])
        && (SduLength >= DOIP_GENERIC_HEADER_LEN));
}

static inline uint16 DOIP_U8_2_U16(const uint8* array) /* PRQA S 3219 */ /* MISRA Rule 2.1 */
{
    return (((uint16)(array)[0] << 8) + (uint16)(array)[1]);
}
static inline uint32 DOIP_U8_2_U32(const uint8* array) /* PRQA S 3219 */ /* MISRA Rule 2.1 */
{
    return (
        ((uint32)(array)[0] << 24u) + ((uint32)(array)[1] << 16u) + ((uint32)(array)[2] << 8u) + ((uint32)(array)[3]));
}
/***** mini function *****/
static inline void DoIP_u16_2_u8(uint8* array, uint16 data) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    array[0] = (uint8)(data >> 8u);
    array[1] = (uint8)data;
}

static inline void DoIP_u32_2_u8(uint8* array, uint32 data) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    array[0] = (uint8)(data >> 24u);
    array[1] = (uint8)(data >> 16u);
    array[2] = (uint8)(data >> 8u);
    array[3] = (uint8)data;
}
/**********************Det function start************************************/
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
static inline FUNC(BufReq_ReturnType, DOIP_CODE)
    /* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
    DoIP_SoAdTpCopyTxData_DetCheck(
        VAR(PduIdType, AUTOMATIC) id,
        P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
        P2CONST(RetryInfoType, AUTOMATIC, DOIP_CONST) retry,
        P2CONST(PduLengthType, AUTOMATIC, DOIP_VAR) availableDataPtr)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;
    /*SWS_DoIP_00175*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADTPCOPYTXDATA, DOIP_E_UNINIT);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00176*/
    else if (DOIP_FOR_SOAD_TX_MAX_ID <= id)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADTPCOPYTXDATA, DOIP_E_INVALID_PDU_SDU_ID);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00177*/
    else if ((NULL_PTR == info) || (NULL_PTR == availableDataPtr))
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADTPCOPYTXDATA, DOIP_E_PARAM_POINTER);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00178*/
    else if (NULL_PTR != retry)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADTPCOPYTXDATA, DOIP_E_INVALID_PARAMETER);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    else
    {
        ret = BUFREQ_OK;
    }

    return ret;
}

#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
static inline FUNC(Std_ReturnType, DOIP_CODE)
    DoIP_IfTransmit_DetCheck(VAR(PduIdType, AUTOMATIC) id, P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info)
{
    Std_ReturnType ret = E_NOT_OK;
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_IFTRANSMIT, DOIP_E_UNINIT);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    else if (NULL_PTR == info)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_IFTRANSMIT, DOIP_E_PARAM_POINTER);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    else if (DOIP_FOR_PDUR_TX_MAX_ID <= id)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_IFTRANSMIT, DOIP_E_INVALID_PDU_SDU_ID);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    else
    {
        ret = E_OK;
    }

    return ret;
}

static inline FUNC(Std_ReturnType, DOIP_CODE) DoIP_IfCancelTransmit_DetCheck(VAR(PduIdType, AUTOMATIC) id)
{
    Std_ReturnType ret = E_NOT_OK;

    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_IFCANCELTRANSMIT, DOIP_E_UNINIT);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    else if (DOIP_FOR_PDUR_TX_MAX_ID <= id)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_IFCANCELTRANSMIT, DOIP_E_INVALID_PDU_SDU_ID);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    else
    {
        ret = E_OK;
    }

    return ret;
}
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/

static inline FUNC(BufReq_ReturnType, DOIP_CODE)
    /* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
    DoIP_SoAdTpCopyRxData_DetCheck(
        VAR(PduIdType, AUTOMATIC) id,
        P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
        P2CONST(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;

    /*SWS_DoIP_00183*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADTPCOPYRXDATA, DOIP_E_UNINIT);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00036*/
    else if (DOIP_FOR_SOAD_RX_MAX_ID <= id)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADTPCOPYRXDATA, DOIP_E_INVALID_PDU_SDU_ID);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00184*/
    else if ((NULL_PTR == info) || (NULL_PTR == bufferSizePtr))
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADTPCOPYRXDATA, DOIP_E_PARAM_POINTER);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    else
    {
        ret = BUFREQ_OK;
    }

    return ret;
}

static inline FUNC(boolean, DOIP_CODE)
    /* PRQA S 1532++ */ /* MISRA Rule 8.7 */
    DoIP_SoAdIfRxIndication_DetCheck(
        VAR(PduIdType, AUTOMATIC) RxPduId,
        P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    boolean Det_Status = FALSE;
    /*SWS_DoIP_00246*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADIFRXINDICATION, DOIP_E_UNINIT);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00247*/
    else if (DOIP_FOR_SOAD_RX_MAX_ID <= RxPduId)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADIFRXINDICATION, DOIP_E_INVALID_PDU_SDU_ID);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00248*/
    else if (NULL_PTR == PduInfoPtr)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADIFRXINDICATION, DOIP_E_PARAM_POINTER);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    else
    {
        Det_Status = TRUE;
    }

    return Det_Status;
}

static inline FUNC(BufReq_ReturnType, DOIP_CODE)
    /* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
    DoIP_SoAdTpStartOfReception_DetCheck(
        VAR(PduIdType, AUTOMATIC) id,
        VAR(PduLengthType, AUTOMATIC) TpSduLength,
        P2CONST(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;
    /*SWS_DoIP_00186*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADTPSTARTOFRECEPTION, DOIP_E_UNINIT);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00187*/
    else if (DOIP_FOR_SOAD_RX_MAX_ID <= id)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(
            DOIP_MODULE_ID,
            DOIP_INSTANCE_ID,
            DOIP_SID_SOADTPSTARTOFRECEPTION,
            DOIP_E_INVALID_PDU_SDU_ID);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00188*/
    else if (NULL_PTR == bufferSizePtr)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADTPSTARTOFRECEPTION, DOIP_E_PARAM_POINTER);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00189*/
    else if (0u != TpSduLength)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(
            DOIP_MODULE_ID,
            DOIP_INSTANCE_ID,
            DOIP_SID_SOADTPSTARTOFRECEPTION,
            DOIP_E_INVALID_PARAMETER);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    else
    {
        ret = BUFREQ_OK;
    }

    return ret;
}

static inline FUNC(Std_ReturnType, DOIP_CODE)
    /* PRQA S 1532++ */ /* MISRA Rule 8.7 */
    DoIP_TpTransmit_DetCheck(
        VAR(PduIdType, AUTOMATIC) DoIPPduRTxId,
        P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) DoIPPduRTxInfoPtr)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    /*SWS_DoIP_00162*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TPTRANSMIT, DOIP_E_UNINIT);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00164*/
    else if (NULL_PTR == DoIPPduRTxInfoPtr)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TPTRANSMIT, DOIP_E_PARAM_POINTER);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00163*/
    else if (DOIP_FOR_PDUR_TX_MAX_ID <= DoIPPduRTxId)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TPTRANSMIT, DOIP_E_INVALID_PDU_SDU_ID);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    else
    {
        ret = E_OK;
    }

    return ret;
}

static inline FUNC(boolean, DOIP_CODE)
    /* PRQA S 1532++ */ /* MISRA Rule 8.7 */
    DoIP_SoAdTpTxConfirmation_DetCheck(VAR(PduIdType, AUTOMATIC) id, VAR(Std_ReturnType, AUTOMATIC) result)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    boolean det_Status = FALSE;
    /*SWS_DoIP_00180*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADTPTXCONFIRMATION, DOIP_E_UNINIT);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00181*/
    else if (DOIP_FOR_SOAD_TX_MAX_ID <= id)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)
            Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADTPTXCONFIRMATION, DOIP_E_INVALID_PDU_SDU_ID);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00182*/
    else if ((E_OK != result) && (E_NOT_OK != result))
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)
            Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADTPTXCONFIRMATION, DOIP_E_INVALID_PARAMETER);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    else
    {
        det_Status = TRUE;
    }

    return det_Status;
}

static inline FUNC(boolean, DOIP_CODE)
    /* PRQA S 1532++ */ /* MISRA Rule 8.7 */
    DoIP_SoConModeChg_DetCheck(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_SoConModeType, AUTOMATIC) Mode)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    boolean det_status = FALSE;
    /*SWS_DoIP_00193*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_LOCALIPADDRASSIGNMENT, DOIP_E_UNINIT);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00194*/
    else if (
        ((SoAd_SoConIdType)DOIP_INVALID_UINT16 == SoConId)
        || ((SOAD_SOCON_ONLINE != Mode) && (SOAD_SOCON_RECONNECT != Mode) && (SOAD_SOCON_OFFLINE != Mode)))
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)
            Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_LOCALIPADDRASSIGNMENT, DOIP_E_INVALID_PARAMETER);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    else
    {
        det_status = TRUE;
    }

    return det_status;
}

/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
static inline FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpCancelReceive_DetCheck(VAR(PduIdType, AUTOMATIC) DoIPPduRRxId)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_OK;
    /*SWS_DoIP_00169*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TPCANCELRECEIVE, DOIP_E_UNINIT);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
        ret = E_NOT_OK;
    }
    /*SWS_DoIP_00170*/
    else if (DOIP_FOR_PDUR_RX_MAX_ID <= DoIPPduRRxId)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TPCANCELRECEIVE, DOIP_E_INVALID_PDU_SDU_ID);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
        ret = E_NOT_OK;
    }
    else
    {
        /*nothing*/
    }

    return ret;
}

/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
static inline FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpCancelTransmit_DetCheck(VAR(PduIdType, AUTOMATIC) DoIPPduRTxId)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_OK;
    /*SWS_DoIP_00166*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TPCANCELTRANSMIT, DOIP_E_UNINIT);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
        ret = E_NOT_OK;
    }
    /*SWS_DoIP_00167*/
    else if (DOIP_FOR_PDUR_TX_MAX_ID <= DoIPPduRTxId)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_TPCANCELTRANSMIT, DOIP_E_INVALID_PDU_SDU_ID);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
        ret = E_NOT_OK;
    }
    else
    {
        /*nothing*/
    }

    return ret;
}

static inline FUNC(boolean, DOIP_CODE)
    /* PRQA S 1532++ */ /* MISRA Rule 8.7 */
    DoIP_SoAdTpRxIndication_DetCheck(VAR(PduIdType, AUTOMATIC) id, VAR(Std_ReturnType, AUTOMATIC) result)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    boolean det_Status = FALSE;
    /*SWS_DoIP_00190*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADTPRXINDICATION, DOIP_E_UNINIT);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00191*/
    else if (DOIP_FOR_SOAD_RX_MAX_ID <= id)
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADTPRXINDICATION, DOIP_E_INVALID_PDU_SDU_ID);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    /*SWS_DoIP_00192*/
    else if ((E_OK != result) && (E_NOT_OK != result))
    {
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
        (void)Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, DOIP_SID_SOADTPRXINDICATION, DOIP_E_INVALID_PARAMETER);
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    }
    else
    {
        det_Status = TRUE;
    }

    return det_Status;
}
#endif
/**********************************Det function end*****************************/
FUNC(BufReq_ReturnType, DOIP_CODE)
DoIP_DiagMsgRxToPduR(
    PduIdType pdurRxId,
    const PduInfoType* info,
    PduLengthType* bufferSizePtr,
    DoIP_RoleType role,
    P2CONST(DoIP_DiagRcvHandleType, AUTOMATIC, DOIP_CONST) receiveHandle,
    uint8* nackCode);

FUNC(BufReq_ReturnType, DOIP_CODE)
DoIP_DiagMsgHandleHeaderReceived(
    DoIP_RoleType role,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr,
    P2CONST(DoIP_DiagRcvHandleType, AUTOMATIC, DOIP_VAR) receiveHandle);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

FUNC(void, DOIP_CODE) DoIP_TcpSocketTimeoutHandler(void);

FUNC(void, DOIP_CODE) DoIP_PendingDiagMsgHandler(void);

static inline FUNC(void, DOIP_CODE) DoIP_MemCpy(
    P2CONST(uint8, AUTOMATIC, DOIP_VAR) Source,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DOIP_VAR) Dest,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    VAR(PduLengthType, AUTOMATIC) Length)
{
    PduLengthType len = Length;
    while (len > 0u)
    {
        len--;
        Dest[len] = Source[len];
    }
    return;
}

FUNC(void, DOIP_CODE)
DoIP_SoConModeChgTcpNty(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_SoConModeType, AUTOMATIC) Mode);

FUNC(void, DOIP_CODE)
DoIP_SoConModeChgUdpNty(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_SoConModeType, AUTOMATIC) Mode);

FUNC(void, DOIP_CODE)
DoIP_SoConModeChgAncNty(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_SoConModeType, AUTOMATIC) Mode);

static inline FUNC(DoIP_RoleType, DOIP_CODE) DoIP_GetRoleByPduRTxPduId(PduIdType DoIPPduRTxId)
{
    DoIP_RoleType role = 0u; /*invalid role*/
    uint8 i;
    const DoIPChannelType* DoIPChannel;

    for (i = 0u; i < DoIP_PBCfgPtr->DoIPChannelNbr; i++)
    {
        DoIPChannel = &DoIP_PBCfgPtr->DoIPChannel[i];
        if ((NULL_PTR != DoIPChannel->DoIPPduRTxPdu) && (DoIPChannel->DoIPPduRTxPdu->DoIPPduRTxPduId == DoIPPduRTxId))
        {
            role = DoIPChannel->DoIPRole;
            break;
        }
    }

    return role;
}

static inline FUNC(Std_ReturnType, DOIP_CODE) DoIP_GetUdpConnBySoAdId(SoAd_SoConIdType id, uint8* index)
{
    uint8 i;
    Std_ReturnType ret = E_NOT_OK;

    for (i = 0u; i < DOIP_UDP_MAX_CONNECTIONS; i++)
    {
        if (DoIP_UdpConnStatus[i].socketId == id)
        {
            *index = i;
            ret = E_OK;
            break;
        }
    }
    return ret;
}

static inline FUNC(Std_ReturnType, DOIP_CODE)
    DoIP_GetTcpBufIdxByTxPduId(VAR(PduIdType, AUTOMATIC) txPduId, uint8* index)
{
    uint8 ret = E_NOT_OK;
    uint8 i;
    const TxChannelCtrlType* txCtrl;

    for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
    {
        txCtrl = &DoIP_TcpInnerBuf[i].txCtrl;
        if (txCtrl->pdurTxPduId == txPduId)
        {
            *index = i;
            ret = E_OK;
            break;
        }
    }

    return ret;
}

static inline FUNC(Std_ReturnType, DOIP_CODE) DoIP_GetTcpConnIdxByTxPduId(uint16 txPduId, uint8* index)
{
    uint8 ret = E_NOT_OK;
    uint8 i;

    for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
    {
        if (DoIP_TcpConnStatus[i].txPduId == txPduId)
        {
            *index = i;
            ret = E_OK;
            break;
        }
    }

    return ret;
}

static inline FUNC(const DoIPTcpConnectionType*, DOIP_CODE)
    DoIP_ExtractTcpConnByDoIPSoadRxPduId(PduIdType doIPSoadRxPduId)
{
    const DoIPTcpConnectionType* pTcpConn = NULL_PTR;
    const DoIPTcpConnectionType* tempTcpConn;
    uint8 i;

    for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPAllTcpConnectionNbr; i++)
    {
        tempTcpConn = &(DoIP_PBCfgPtr->DoIPConnections->DoIPTcpConnection[i]);
        if (tempTcpConn->DoIPSoAdRxPdu->DoIPSoAdRxPduId == doIPSoadRxPduId)
        {
            pTcpConn = tempTcpConn;
            break;
        }
    }

    return pTcpConn;
}

static inline FUNC(Std_ReturnType, DOIP_CODE)
    DoIP_GetTcpBufIdxByRxPduId(VAR(PduIdType, AUTOMATIC) rxPduId, uint8* index)
{
    uint8 ret = E_NOT_OK;
    uint8 i;
    const RxChannelCtrlType* rxCtrl;

    for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
    {
        rxCtrl = &DoIP_TcpInnerBuf[i].rxCtrl;
        if (rxCtrl->doipSoadRxPduId == rxPduId)
        {
            *index = i;
            ret = E_OK;
            break;
        }
    }

    return ret;
}

static inline FUNC(Std_ReturnType, DOIP_CODE) DoIP_GetTcpConnIdxByRxPduId(uint16 rxPduId, uint8* index)
{
    uint8 ret = E_NOT_OK;
    uint8 i;

    for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
    {
        if (DoIP_TcpConnStatus[i].rxPduId == rxPduId)
        {
            *index = i;
            ret = E_OK;
            break;
        }
    }

    return ret;
}

static inline FUNC(Std_ReturnType, DOIP_CODE) DoIP_ExtractChannelByPduRRxPduId(PduIdType DoIPPduRRxId, uint8* index)
{
    Std_ReturnType ret = E_NOT_OK;
    const DoIPPduRRxPduType* DoIPPduRRxPdu;
    uint8 i;

    for (i = 0u; i < DoIP_PBCfgPtr->DoIPChannelNbr; i++)
    {
        DoIPPduRRxPdu = DoIP_PBCfgPtr->DoIPChannel[i].DoIPPduRRxPdu;
        if ((NULL_PTR != DoIPPduRRxPdu) && (DoIPPduRRxPdu->DoIPPduRUpperlayerPduId == DoIPPduRRxId))
        {
            *index = i;
            ret = E_OK;
            break;
        }
    }

    return ret;
}

static inline FUNC(Std_ReturnType, DOIP_CODE)
    DoIP_GetPduRTxPduRefByTxId(PduIdType TxId, uint16* PduRTxPdu, DoIP_PduTypeType DoIP_PduType)
{
    Std_ReturnType ret = E_NOT_OK;
    const DoIPPduRTxPduType* DoIPPduRTxPdu;
    uint8 index;

    for (index = 0u; index < DoIP_PBCfgPtr->DoIPChannelNbr; index++)
    {
        DoIPPduRTxPdu = DoIP_PBCfgPtr->DoIPChannel[index].DoIPPduRTxPdu;
        if ((NULL_PTR != DoIPPduRTxPdu) && (DoIPPduRTxPdu->DoIPPduRTxPduId == TxId)
            && (DoIPPduRTxPdu->DoIPPduType == DoIP_PduType))
        {
            ret = E_OK;
            *PduRTxPdu = DoIPPduRTxPdu->DoIPPduRTxPduRef;
            break;
        }
    }
    return ret;
}

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_TransmitBufIdExtrac(
    VAR(PduIdType, AUTOMATIC) pdurid,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DOIP_VAR) bufferId);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

static inline FUNC(Std_ReturnType, DOIP_CODE)
    /* PRQA S 1505++ */ /* MISRA Rule 8.7 */
    DoIP_TriggerTpTransmit(
        VAR(PduIdType, AUTOMATIC) txPduId,
        P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_CONST) pduInfo,
        TxChannelCtrlType* txCtrl,
        DoIP_TpTransmitMessageType txMsgType)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType retVal;

    txCtrl->bufferState = DOIP_BUFFER_OCCUPIED;
    txCtrl->msgType = txMsgType;
    txCtrl->processStatus = DOIP_TX_PROCESS_TRANSMITING;
    txCtrl->txMaxDataLength = pduInfo->SduLength;

    if (E_OK == SoAd_TpTransmit(txPduId, pduInfo))
    {
        retVal = E_OK;
    }
    else
    {
        retVal = E_NOT_OK;
    }

    return retVal;
}

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_PdurSaTaExtract(
    VAR(PduIdType, AUTOMATIC) pdurid,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DOIP_VAR) sa,
    P2VAR(uint16, AUTOMATIC, DOIP_VAR) ta);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

static inline FUNC(void, DOIP_CODE) DoIP_ResetTcpInnerTxBuf(VAR(uint8, AUTOMATIC) bufferId)
{
    uint32 i;
    DoIP_TcpInnerBufferType* pBuffer = &DoIP_TcpInnerBuf[bufferId];
    TxChannelCtrlType* txCtrl = &pBuffer->txCtrl;

    for (i = 0u; i < DOIP_TX_BUFFER_SIZE; i++)
    {
        pBuffer->txChannel[i] = DOIP_INVALID_UINT8;
    }
    txCtrl->TxBufferPos = 0u;
    txCtrl->pdurTxPduId = DOIP_INVALID_UINT16;
    txCtrl->pdurTxPduIdRef = DOIP_INVALID_UINT16;
    txCtrl->bufferState = DOIP_BUFFER_IDLE;
    txCtrl->txMaxDataLength = DOIP_INVALID_UINT16;
    txCtrl->msgType = DOIP_NON_TX_MSG;
    txCtrl->processStatus = DOIP_TX_PROCESS_IDLE;
}

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_SoAdTxIdExtrac(
    PduIdType DoIPPduRTxId,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduIdType, AUTOMATIC, DOIP_VAR) soadID);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

static inline FUNC(Std_ReturnType, DOIP_CODE)
    DoIP_GetTcpConnIdxBySa(uint16 sa, uint8* index) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    uint8 ret = E_NOT_OK;
    uint8 i;

    for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
    {
        if (DoIP_TcpConnStatus[i].sa == sa)
        {
            *index = i;
            ret = E_OK;
            break;
        }
    }

    return ret;
}

FUNC(void, DOIP_CODE)
DoIP_DiagnosticHeaderFill(
    P2CONST(PduInfoType, AUTOMATIC, DOIP_VAR) info,
    VAR(uint8, AUTOMATIC) bufferId,
    VAR(uint16, AUTOMATIC) sa,
    VAR(uint16, AUTOMATIC) ta);

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_TpTransmitInternal(
    VAR(PduIdType, AUTOMATIC) DoIPPduRTxId,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) DoIPPduRTxInfoPtr);

FUNC(void, DOIP_CODE) DoIP_CloseTcpConnection(VAR(SoAd_SoConIdType, AUTOMATIC) socketId);

static inline FUNC(void, DOIP_CODE) DoIP_ResetTcpInnerRxBuf(VAR(uint8, AUTOMATIC) bufferId)
{
    uint32 i;
    DoIP_TcpInnerBufferType* pBuffer = &DoIP_TcpInnerBuf[bufferId];
    RxChannelCtrlType* rxCtrl = &pBuffer->rxCtrl;
    /*reset rx buffer*/
    for (i = 0u; i < DOIP_RX_BUFFER_SIZE; i++)
    {
        pBuffer->rxChannel[i] = DOIP_INVALID_UINT8;
    }
    rxCtrl->rxBufState = DOIP_BUFFER_IDLE;
    rxCtrl->BS = DOIP_INVALID_UINT16;
    rxCtrl->payloadLen = DOIP_INVALID_UINT16;
    rxCtrl->payloadType = DOIP_INVALID_UINT16;
    rxCtrl->procStatus = DOIP_RX_PROCESS_RECEIVING;
    rxCtrl->rxBufPos = 0u;
    rxCtrl->rxBufRemain = DOIP_INVALID_UINT16;
    rxCtrl->rxDataRemain = DOIP_INVALID_UINT16;
    rxCtrl->pdurRxPduIdRef = DOIP_INVALID_UINT16;
}

/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
static inline FUNC(void, DOIP_CODE) DoIP_ResetPendingTxNonDiagQueue(TxPendingNonDiagQueueType* Queue)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    uint8 i;
    Queue->pendingSoAdTxPduId = DOIP_INVALID_UINT16;
    for (i = 0u; i < DOIP_NON_DIAG_QUEUE_BUFFER_SIZE; i++)
    {
        Queue->pendingPdu.dataPtr[i] = DOIP_INVALID_UINT8;
    }
}

#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
static inline FUNC(void, DOIP_CODE)
    DoIP_ResetPendingTxIfQueue(TxPendingIfQueueType* Queue) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    uint8 i;
    Queue->pendingPdurPduId = DOIP_INVALID_UINT16;
    for (i = 0u; i < DOIP_IF_QUEUE_BUFFER_SIZE; i++)
    {
        Queue->pendingPdu.sduDataPtr[i] = DOIP_INVALID_UINT8;
    }
}
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/

static inline FUNC(void, DOIP_CODE)
    DoIP_ResetPendingTxTpQueue(TxPendingTpQueueType* Queue) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    uint8 i;
    Queue->pendingPdurPduId = DOIP_INVALID_UINT16;
    for (i = 0u; i < DOIP_TP_QUEUE_BUFFER_SIZE; i++)
    {
        Queue->pendingPdu.dataPtr[i] = DOIP_INVALID_UINT8;
    }
}

FUNC(void, DOIP_CODE) DoIP_ResetTcpInnerBuf(VAR(uint8, AUTOMATIC) bufferId);

static inline FUNC(void, DOIP_CODE) DoIP_ResetTcpConn(VAR(uint8, AUTOMATIC) index)
{
    DoIP_TcpConnStatusType* tcpConnStatus = &DoIP_TcpConnStatus[index];
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
    DoIP_GatewayCtrlType* gwCtrl = &tcpConnStatus->gwCtrl;
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/

    /*DoIP_UnRegisterSocket*/
    tcpConnStatus->socketId = DOIP_INVALID_UINT8;
    tcpConnStatus->rxPduId = DOIP_INVALID_UINT16;
    tcpConnStatus->txPduId = DOIP_INVALID_UINT16;
    tcpConnStatus->lowerlayerTxPduId = DOIP_INVALID_UINT16;
    tcpConnStatus->sa = DOIP_INVALID_UINT16;
    tcpConnStatus->ta = DOIP_INVALID_UINT16;
    tcpConnStatus->activationType = DOIP_INVALID_UINT8;
    tcpConnStatus->initInactiveTimer = DOIP_INVALID_UINT32;
    tcpConnStatus->genInactiveTimer = DOIP_INVALID_UINT32;
    tcpConnStatus->authState = FALSE;
    tcpConnStatus->confirmState = FALSE;
    tcpConnStatus->soConnState = SOAD_SOCON_OFFLINE;
    tcpConnStatus->routActiveState = DOIP_SOCKET_CONNECTION_INVALID;
    tcpConnStatus->bufferId = DOIP_INVALID_UINT8;
    tcpConnStatus->wait4SocketClose = FALSE;
    tcpConnStatus->wait4DiagMsgResponse = FALSE;
    tcpConnStatus->wait4AliveChkRsp = FALSE;
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
    gwCtrl->waitRmtTimer = 0u;
    gwCtrl->txNrc78Cnt = 0u;
    gwCtrl->waitRmtRsp = FALSE;
    gwCtrl->nrc78Rcvd = FALSE;
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/
}

FUNC(void, DOIP_CODE)
DoIP_TcpNonDiagMsgHandler(
    DoIP_RoleType role,
    VAR(PduIdType, AUTOMATIC) txPduId,
    VAR(uint8, AUTOMATIC) bufIdx,
    uint16 payloadType);

FUNC(boolean, DOIP_CODE) DoIP_SocketConnectedCheckFrmPdurId(VAR(PduIdType, AUTOMATIC) DoIPPduRTxId);

static inline FUNC(Std_ReturnType, DOIP_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    DoIP_PduRRxIdExtract(
        VAR(uint16, AUTOMATIC) sa,
        VAR(uint16, AUTOMATIC) ta,
        P2VAR(PduIdType, AUTOMATIC, DOIP_VAR) pduId)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 i;
    Std_ReturnType ret = E_NOT_OK;
    const DoIPChannelType* DoIPChannel;

    for (i = 0u; i < DoIP_PBCfgPtr->DoIPChannelNbr; i++)
    {
        DoIPChannel = &DoIP_PBCfgPtr->DoIPChannel[i];
        if ((DoIPChannel->DoIPChannelSARef->DoIPTesterSA == sa)
            && (DoIPChannel->DoIPChannelTARef->DoIPTargetAddressValue == ta)
            && (NULL_PTR != DoIPChannel->DoIPPduRRxPdu))
        {
            ret = E_OK;
            *pduId = DoIPChannel->DoIPPduRRxPdu->DoIPPduRUpperlayerPduId;
            break;
        }
    }
    return ret;
}

static inline FUNC(void, DOIP_CODE) DoIP_ResetUdpConn(VAR(uint8, AUTOMATIC) index)
{
    DoIP_UdpConnStatusType* udpConnStatus = &DoIP_UdpConnStatus[index];
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
    uint8 i;
    DoIP_VehicleEntryType* vehicleEntry = &udpConnStatus->vehicleEntry;
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/

    udpConnStatus->socketId = DOIP_INVALID_UINT8;
    udpConnStatus->soConState = SOAD_SOCON_OFFLINE;
    udpConnStatus->outgoingMsgType = DOIP_NO_REQ_W4_RSP;
    udpConnStatus->doipMsgTimer = 0u;
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
    udpConnStatus->nodeInfo.currentlyOpenSocket = DOIP_INVALID_UINT8;
    udpConnStatus->nodeInfo.powerMode = DOIP_INVALID_UINT8;
    udpConnStatus->nodeInfo.nodeType = DOIP_INVALID_UINT8;
    udpConnStatus->nodeInfo.maxOpenSocket = DOIP_INVALID_UINT8;
    udpConnStatus->nodeInfo.currentlyOpenSocket = DOIP_INVALID_UINT8;
    udpConnStatus->nodeInfo.maxDataSize = DOIP_INVALID_UINT32;
    udpConnStatus->vinGidSyncState = DOIP_VINGID_SYNC_STATE_NONE;
    vehicleEntry->logicalAddr = 0x0000;
    /*size of vin:17*/
    for (i = 0u; i < 17u; i++)
    {
        vehicleEntry->vin[i] = 0x00;
    }
    /*size of gid/eid:6*/
    for (i = 0u; i < 6u; i++)
    {
        vehicleEntry->gid[i] = 0x00;
        vehicleEntry->eid[i] = 0x00;
    }
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/
}

FUNC(void, DOIP_CODE)
DoIP_MsgIfRxHandler(
    DoIP_RoleType role,
    PduIdType txPduId,
    SoAd_SoConIdType SoConId,
    VAR(uint32, AUTOMATIC) payloadlength,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr);

static inline FUNC(Std_ReturnType, DOIP_CODE) DoIP_SwitchOnOneLine(uint16 txPduId, boolean assignment)
{
    Std_ReturnType retVal;
    SoAd_SoConIdType soConId;

    if (E_OK == SoAd_GetSoConId(txPduId, &soConId))
    {
        /*need to be TCPIP_IPADDR_ASSIGNMENT_ALL in AUTOSAR4.4.but SoAd 4.2.2 donot have this definiation*/
        if (TRUE == assignment)
        {
            (void)SoAd_RequestIpAddrAssignment(soConId, TCPIP_IPADDR_ASSIGNMENT_ALL, NULL_PTR, 0u, NULL_PTR);
        }
        retVal = SoAd_OpenSoCon(soConId);
    }
    else
    {
        retVal = E_NOT_OK;
    }

    return retVal;
}

static inline FUNC(Std_ReturnType, DOIP_CODE) DoIP_SwitchOffOneLine(uint16 txPduId, boolean assignment)
{
    Std_ReturnType retVal;
    SoAd_SoConIdType soConId;

    if (E_OK == SoAd_GetSoConId(txPduId, &soConId))
    {
        retVal = SoAd_CloseSoCon(soConId, TRUE);
        if (TRUE == assignment)
        {
            (void)SoAd_ReleaseIpAddrAssignment(soConId);
        }
    }
    else
    {
        retVal = E_NOT_OK;
    }

    return retVal;
}

FUNC(BufReq_ReturnType, DOIP_CODE)
DoIP_HeaderTpRxHandler(
    DoIP_RoleType role,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    uint16 txpduId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    uint8 bufIdx);

FUNC(BufReq_ReturnType, DOIP_CODE)
DoIP_CFTpRxHandler(
    DoIP_RoleType role,
    DoIP_DiagRcvHandleType* DoIP_DiagRcvHandle,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    uint16 txpduId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_PendingTpEnqueue(
    PendingTxQueueTpMsgType* queueMsg,
    PduIdType pduRTxId,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_VAR) pdurTxInfo);

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
static inline FUNC(P2VAR(TxPendingTpQueueType, AUTOMATIC, DOIP_VAR), DOIP_CODE)
    DoIP_PendingTpDequeue(P2VAR(PendingTxQueueTpMsgType, AUTOMATIC, DOIP_VAR) queueMsg)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 qID;
    TxPendingTpQueueType* tempQueue = NULL_PTR;

    if ((queueMsg->tail != queueMsg->head)) /*queueMsg not empty*/
    {
        qID = queueMsg->head % DOIP_TP_QUEUE_BUFFER_NUM;
        tempQueue = &queueMsg->pendingQueue[qID];
        queueMsg->head++;
    }

    return tempQueue;
}

/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
static inline FUNC(Std_ReturnType, DOIP_CODE) DoIP_ExtractUdpConnBySoadRxPduId(PduIdType rxPduId, uint8* index)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 i;
    const DoIPUdpConnectionType* pDoIPUdpConnection = DoIP_PBCfgPtr->DoIPConnections->DoIPUdpConnection;
    for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPAllUdpConnectionNbr; i++)
    {
        if ((pDoIPUdpConnection[i].DoIPSoAdRxPdu != NULL_PTR)
            && (pDoIPUdpConnection[i].DoIPSoAdRxPdu->DoIPSoAdRxPduId == rxPduId))
        {
            *index = i;
            ret = E_OK;
            break;
        }
    }

    return ret;
}

static inline FUNC(Std_ReturnType, DOIP_CODE) DoIP_GetAncConnIdBySocketId(SoAd_SoConIdType SoConId, uint8* id)
{
    uint8 ret = E_NOT_OK;
    uint8 i;

    for (i = 0u; i < DOIP_UDP_VEHICLEANNOUNCEMENT_CONNECTIONS; i++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        if (DoIP_AncConnStatus[i].socketId == SoConId)
        {
            *id = i;
            ret = E_OK;
            break;
        }
    }

    return ret;
}

static inline FUNC(Std_ReturnType, DOIP_CODE) DoIP_ExtractAnnounceConnBySoadRxPduId(PduIdType rxPduId, uint8* index)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 i;
    const DoIPUdpVehicleAnnouncementConnectionType* pDoIPUdpVehicleAnn =
        DoIP_PBCfgPtr->DoIPConnections->DoIPUdpVehicleAnnouncementConnection;
    for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPAllVehicleAnnouncementConnectionNbr; i++)
    {
        if ((pDoIPUdpVehicleAnn[i].DoIPSoAdRxPdu != NULL_PTR)
            && (pDoIPUdpVehicleAnn[i].DoIPSoAdRxPdu->DoIPSoAdRxPduId == rxPduId))
        {
            *index = i;
            ret = E_OK;
            break;
        }
    }

    return ret;
}

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_SoAdIfRxIndicationExtractParam(
    VAR(PduIdType, AUTOMATIC) RxPduId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DOIP_VAR) txpduId,
    P2VAR(SoAd_SoConIdType, AUTOMATIC, DOIP_VAR) SoConId,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    DoIP_RoleType* role);

#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
FUNC(void, DOIP_CODE) DoIP_SendVehicleIdentificationRsp(VAR(uint16, AUTOMATIC) txPduId);

FUNC(void, DOIP_CODE) DoIP_UdpSendNack(VAR(PduIdType, AUTOMATIC) TxPduId, VAR(uint8, AUTOMATIC) NackCode);

static inline FUNC(void, DOIP_CODE) DoIP_ResetPendingRoutingActiveMsg(void)
{
    DoIP_PendingRoutMsg.pendingBufferIndex = DOIP_INVALID_UINT8;
    DoIP_PendingRoutMsg.pendingFlag = FALSE;
    DoIP_PendingRoutMsg.pendingSa = DOIP_INVALID_UINT16;
    DoIP_PendingRoutMsg.pendingSocketConId = DOIP_INVALID_UINT8;
    DoIP_PendingRoutMsg.pendingTxPduId = DOIP_INVALID_UINT16;

    DoIP_PendingRoutMsg.responseDataLen = 0u;
}

static inline FUNC(Std_ReturnType, DOIP_CODE) DoIP_ExtractAnnounceConnBySoConId(SoAd_SoConIdType soConId, uint8* index)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 i;
    const DoIPUdpVehicleAnnouncementConnectionType* pDoIPUdpVehicleAnn =
        DoIP_PBCfgPtr->DoIPConnections->DoIPUdpVehicleAnnouncementConnection;
    for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPAllVehicleAnnouncementConnectionNbr; i++)
    {
        if (pDoIPUdpVehicleAnn[i].soConId == soConId)
        {
            *index = i;
            ret = E_OK;
            break;
        }
    }

    return ret;
}

FUNC(void, DOIP_CODE) DoIP_VehicleAncTimeouthandler(void);

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_IfTransmitInternal(VAR(PduIdType, AUTOMATIC) id, P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info);

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_PendingIfEnqueue(
    PendingTxQueueIfMsgType* queueMsg,
    PduIdType pduRTxId,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_VAR) pdurTxInfo);

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
static inline FUNC(P2VAR(TxPendingIfQueueType, AUTOMATIC, DOIP_VAR), DOIP_CODE)
    DoIP_PendingIfDequeue(P2VAR(PendingTxQueueIfMsgType, AUTOMATIC, DOIP_VAR) queueMsg)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 qID;
    TxPendingIfQueueType* tempQueue = NULL_PTR;

    if ((queueMsg->tail != queueMsg->head)) /*queueMsg not empty*/
    {
        qID = queueMsg->head % DOIP_IF_QUEUE_BUFFER_NUM;
        tempQueue = &queueMsg->pendingQueue[qID];
        queueMsg->head++;
    }

    return tempQueue;
}

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_ServerHeaderEvaluateWithPayloadType(
    VAR(uint16, AUTOMATIC) txPduId,
    VAR(uint8, AUTOMATIC) bufIdx,
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);

#endif /*DOIP_ROLE_SERVER_SUPPORT*/
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
FUNC(Std_ReturnType, DOIP_CODE) DoIP_ClientActivationLineSwitchOn(uint8 VehicleID);

FUNC(Std_ReturnType, DOIP_CODE) DoIP_ClientActivationLineSwitchOff(uint8 VehicleID);

FUNC(BufReq_ReturnType, DOIP_CODE)
DoIP_DiagNackRxHandler(
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr,
    P2CONST(DoIP_DiagRcvHandleType, AUTOMATIC, DOIP_CONST) receiveHandle);

FUNC(void, DOIP_CODE) DoIP_VehicleConnTimeoutHandler(void);

FUNC(void, DOIP_CODE) DoIP_DoIPMsgTimeoutHandler(void);

FUNC(void, DOIP_CODE) DoIP_VehicleDiscoveryTimeoutHandler(void);

static inline FUNC(void, DOIP_CODE) DoIP_DiagnosticMsgTimeoutHandler(void)
{
    uint8 i;

    for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
    {
        SchM_Enter_DoIP_ExclusiveArea();
        if (DoIP_TcpConnStatus[i].wait4DiagMsgResponse == TRUE)
        {
            if (DoIP_TcpConnStatus[i].diagMsgTimer >= DOIP_DIAG_MSG_TIME)
            {
                /*TODO, request or response lost, may retry request*/
                DoIP_TcpConnStatus[i].wait4DiagMsgResponse = FALSE;
            }
            else
            {
                DoIP_TcpConnStatus[i].diagMsgTimer += DOIP_MAIN_FUNCTION_PERIOD;
            }
        }
        SchM_Exit_DoIP_ExclusiveArea();
    }
}

static inline FUNC(Std_ReturnType, DOIP_CODE) DoIP_GetUdpTxPduByVehicleId(uint8 VehicleID, PduIdType* pduId)
{
    uint8 i;
    Std_ReturnType ret = E_NOT_OK;

    for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPAllUdpConnectionNbr; i++)
    {
        if (DOIP_UDPCON_CFG(i)->DoIPConnVehicleRef->DoIPVehicleId == VehicleID)
        {
            *pduId = DOIP_UDPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
            ret = E_OK;
            break;
        }
    }
    return ret;
}

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_ClientHeaderEvaluateWithPayloadType(VAR(uint8, AUTOMATIC) bufIdx, VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);

static inline FUNC(Std_ReturnType, DOIP_CODE) DoIP_IsAllVehicleSync(void)
{
    uint8 i;
    Std_ReturnType ret = E_OK;

    for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPClientUdpConnectionNbr; i++)
    {
        if ((DOIP_VINGID_SYNC_STATE_INCOMPLETE == DoIP_UdpConnStatus[i].vinGidSyncState)
            && (SOAD_SOCON_ONLINE == DoIP_UdpConnStatus[i].soConState))
        {
            ret = E_NOT_OK;
            break;
        }
    }
    return ret;
}

FUNC(void, DOIP_CODE)
DoIP_HandleVehicleIdentificationRsp(SoAd_SoConIdType SoConId, P2VAR(uint8, AUTOMATIC, DOIP_VAR) payloadPtr);

FUNC(void, DOIP_CODE)
DoIP_HandlePowerModeInfoRsp(PduIdType txPduId, SoAd_SoConIdType SoConId, P2VAR(uint8, AUTOMATIC, DOIP_VAR) payloadPtr);

FUNC(void, DOIP_CODE)
DoIP_HandleEntityStatusRsp(PduIdType txPduId, SoAd_SoConIdType SoConId, P2VAR(uint8, AUTOMATIC, DOIP_VAR) payloadPtr);

FUNC(void, DOIP_CODE)
DoIP_HandleRoutingActiveRsp(uint8 vehicleId, VAR(uint16, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx);

FUNC(Std_ReturnType, DOIP_CODE) DoIP_SendAllRoutingActiveReq(void);

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_SendRoutingActReq(VAR(PduIdType, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx, uint16 sa);

static inline FUNC(void, DOIP_CODE) DoIP_ResetVehicleConnCtrl(void)
{
    DoIP_VehicleConnCtrl.vehicleConnState = DOIP_CONN_STATE_IDLE;
    DoIP_VehicleConnCtrl.vehicleStartCnt = 0u;
    DoIP_VehicleConnCtrl.waitForActiveCnt = 0u;
    DoIP_VehicleConnCtrl.connTimer = DOIP_INVALID_UINT32;
    DoIP_VehicleConnCtrl.discoveryWaitTimer = DOIP_INVALID_UINT32;
}
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
FUNC(void, DOIP_CODE) DoIP_GatewayTxFakeResp(uint8 index, uint8 rspCode);
FUNC(void, DOIP_CODE) DoIP_GatewayTxFakeRespTimeoutHandler(void);
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/
#endif /*DOIP_INTERNAL_H*/
/*=======[E N D   O F   F I L E]==============================================*/
