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
 **  @file               : DoIP_Internal_TCP.h
 **  @author             : hao.wen
 **  @date               : 2024/09/08
 **  @vendor             : isoft
 **  @description        : Implement the TCP part of DoIP
 **
 ***********************************************************************************************************************/

#ifndef DOIP_INTERNAL_TCP_H_
#define DOIP_INTERNAL_TCP_H_

/* =================================================== inclusions =================================================== */

#include "DoIP_Internal_Common.h"

#ifdef __cplusplus
extern "C" {
#endif
/* ===================================================== macros ===================================================== */

#define DOIP_SA_TA_LEN 4u /**< Length of source adddress and target address  */

/* ================================================ type definitions ================================================ */

/**
 * @brief Socket assignment enumeration
 */
typedef enum
{
    DOIP_SOCKET_ASSIGNMENT_FAILED,     /**< Socket assignment fail @range 0 */
    DOIP_SOCKET_ASSIGNMENT_SUCCESSFUL, /**< Socket assignment success @range 1 */
    DOIP_SOCKET_ASSIGNMENT_PENDING     /**< Wait free socket @range 2 */
} DoIP_SocketAssignmentResultType;

/**
 * @brief Used when the routing activation request is need suspended.
 */
typedef struct
{
    boolean          IsPending;    /**< Whether or not pended @range 0..1 */
    uint8            TxRxCtxIdx;   /**< Send/receive context @range 0..255 */
    uint16           Sa;           /**< Source address of routing activation message @range 0..65535 */
    uint16           SoadTxPduRef; /**< The connection that received routing activation message @range 0..65535 */
    SoAd_SoConIdType SoConId;      /**< The connection that received routing activation message @range NA */
} DoIP_PendingRaType;

/**
 * @brief Routing activation status.
 */
typedef enum
{
    DOIP_SOCKET_NONACTIVATED, /**< Without activate routing @range 0 */
    DOIP_SOCKET_ACTIVATED     /**< Already activate routing @range 1*/
} DoIP_SocketRaStateType;

/**
 * @brief TCP connection status.
 */
typedef struct
{
    boolean WaitAliveCheckRsp; /**< Wait alive check response message @range 0..1 */
    boolean WaitCloseSocket;   /**< Will close this TCP connection @range 0...1 */

    boolean AuthState;    /**< Authentication status of RA @range 0..1 */
    boolean ConfirmState; /**< Confirm status of RA @range 0..1 */

    uint8 TxRxCtxIdx; /**< Index of DoIP_TcpTxRxContext @range 0..255 */

    uint8 RaType; /**< Routing activation type @range 0..255 */

    uint16 SoadRxPduId;  /**< DoIPSoAdRxPduId @range 0..65535 */
    uint16 SoadTxPduId;  /**< DoIPSoAdTxPduId @range 0..65535 */
    uint16 SoadTxPduRef; /**< DoIPSoAdTxPduRef @range 0..65535 */

    uint16 Sa; /**< LA of tester @range 0..65535 */
    uint16 Ta; /**< LA of DoIP  @range 0..65535 */

    SoAd_SoConIdType   SoConId;    /**< ID of socket @range NA */
    SoAd_SoConModeType SoConState; /**< State of connection @range NA*/

    uint32 InitInactiveTimer;    /**< initial inactive timer @range 0..0xFFFFFFFF*/
    uint32 GeneralInactiveTimer; /**< General inactive timer @range 0..0xFFFFFFFF*/
    uint32 AliveCheckTimer;      /**< Alive check timer @range 0..0xFFFFFFFF*/

    DoIP_SocketRaStateType RaState; /**< Connection whether or not active routing @range NA */
} DoIP_TcpConnStatusType;

/**
 * @brief Send message type of TCP
 */
typedef enum
{
    DOIP_TX_NON_MSG,      /**< Invalid value @range 0 */
    DOIP_TX_NON_DIAG_MSG, /**< Non diagnostic message @range 1 */
    DOIP_TX_DIAG_IF_MSG,  /**< Diagnostic message: IF @range 2 */
    DOIP_TX_DIAG_TP_MSG   /**< Diagnostic message: TP @range 3 */
} DoIP_TcpTxMsgType;

/**
 * @brief Status of buffer
 */
typedef enum
{
    DOIP_BUFFER_IDLE,    /**< Buffer is free @range 0 */
    DOIP_BUFFER_OCCUPIED /**< Other connections are in use @range 1 */
} DoIP_BufferStateType;

/**
 * @brief Receiving message status
 */
typedef enum
{
    DOIP_RX_STATE_IDLE,             /**< Haven't received the message yet @range 0 */
    DOIP_RX_STATE_START,            /**< Start receive message  @range 1 */
    DOIP_RX_STATE_RECEIVING_HEADER, /**< Receiving a message header @range 2 */
    DOIP_RX_STATE_RECEIVED_HEADER,  /**< Already receive message header @range 3 */
    DOIP_RX_STATE_CF,               /**< Receive messages continuously @range 4 */
    DOIP_RX_STATE_DISCARD           /**< Discard message @range 5 */
} DoIP_TcpRxStateType;

/**
 * @brief Sending message status
 */
typedef enum
{
    DOIP_TX_STATE_IDLE,              /**< Haven't send the message yet @range 0 */
    DOIP_TX_STATE_START,             /**< Start send message  @range 1 */
    DOIP_TX_STATE_TRANSMITED_HEADER, /**< Already send message header @range 2 */
} DoIP_TcpTxStateType;

/**
 * @brief Receive message context of TCP
 */
typedef struct
{
    DoIP_TcpRxStateType RxState; /**< Receiving message status @range NA */

    DoIP_BufferStateType RxBufState;  /**< Status of buffer @range NA */
    PduLengthType        RxBufPos;    /**< The amount of data received @range NA */
    PduLengthType        RxBufRemain; /**< The size of the remaining buffer @range NA */
    PduLengthType RxDataRemain; /**< How much more data needs to be received to make up a complete message @range NA */

    uint16        PayloadType; /**< Message type @range 0..65535*/
    PduLengthType PayloadLen;  /**< Length of payload @range NA */

    PduLengthType BlockSize;    /**< used for pdur reception @range NA */
    PduIdType     PdurRxPduRef; /**< DoIP rx uppper(PDUR) pdu id @range 0..255 */
    PduIdType     SoadRxPduId;  /**< DoIP rx pdu id,to identify which buffer copy data shall use @range 0..255 */

    uint16 Sa; /**< LA of tester @range 0..65535 */
    uint16 Ta; /**< LA of DoIP @range 0..65535 */
} DoIP_TcpRxCtrlType;

/**
 * @brief TP queue
 */
typedef struct
{
    uint8 SduDataPtr[DOIP_TP_QUEUE_BUFFER_SIZE]; /**< Buffer @range NA*/
} DoIP_TpPduDataType;

/**
 * @brief The item saved in the queue
 */
typedef struct
{
    PduIdType          PdurTxPduId; /**< The connection to send the message @range 0..255 */
    PduInfoType        PduInfo;     /**< Saved PduInfoType @range NA */
    DoIP_TpPduDataType PduData;     /**< Sended message @range NA */
} DoIP_TpItemType;

/**
 * @brief Struct of queue
 */
typedef struct
{
    uint8 Head;                                     /**< Head of queue @range 0..255 */
    uint8 Tail;                                     /**< Tail of queue @range 0..255 */
    /* PRQA S 1039 ++ */                            /* VL_DoIP_FlexibleArray */
    DoIP_TpItemType Item[DOIP_TP_QUEUE_BUFFER_NUM]; /**< Saved item @range NA*/
    /* PRQA S 1039 -- */
    uint8 Count;
} DoIP_TpQueueType;

/**
 * @brief IF queue
 */
typedef struct
{
    uint8 MetaDataPtr[DOIP_SA_TA_LEN];           /**< Saved metadata @range NA */
    uint8 SduDataPtr[DOIP_IF_QUEUE_BUFFER_SIZE]; /**< buffer @range NA */
} DoIP_IfPduDataType;

/**
 * @brief The item saved in the queue
 */
typedef struct
{
    PduIdType          PdurTxPduId; /**< The connection to send the message @range 0..255 */
    PduInfoType        PduInfo;     /**< Saved PduInfoType @range NA */
    DoIP_IfPduDataType PduData;     /**< Sended message @range NA*/
} DoIP_IfItemType;

/**
 * @brief Struct of queue
 */
typedef struct
{
    uint8 Head;                                     /**< Head of queue @range 0..255 */
    uint8 Tail;                                     /**< Tail of queue @range 0..255 */
    /* PRQA S 1039 ++ */                            /* VL_DoIP_FlexibleArray */
    DoIP_IfItemType Item[DOIP_IF_QUEUE_BUFFER_NUM]; /**< Saved item @range NA */
    /* PRQA S 1039 -- */
    uint8 Count;
} DoIP_IfQueueType;

/**
 * @brief Non diagnostic queue
 */
typedef struct
{
    uint8 SduDataPtr[DOIP_NON_DIAG_QUEUE_BUFFER_SIZE]; /**< Buffer @range NA */
} DoIP_NonDiagPduDataType;

/**
 * @brief The item saved in the queue
 */
typedef struct
{
    PduIdType               SoadTxPduRef; /**< The connection to send the message @range 0..255 */
    PduInfoType             PduInfo;      /**< Saved PduInfoType @range NA */
    DoIP_NonDiagPduDataType PduData;      /**< Sended message @range NA */
} DoIP_NonDiagItemType;

/**
 * @brief Struct of queue
 */
typedef struct
{
    uint8 Head;                                                /**< Head of queue @range 0..255 */
    uint8 Tail;                                                /**< Tail of queue @range 0..255 */
    /* PRQA S 1039 ++ */                                       /* VL_DoIP_FlexibleArray */
    DoIP_NonDiagItemType Item[DOIP_NON_DIAG_QUEUE_BUFFER_NUM]; /**< Saved item @range NA */
    /* PRQA S 1039 -- */
    uint8 Count;
} DoIP_NonDiagQueueType;

/**
 * @brief Send message context of TCP
 */
typedef struct
{
    DoIP_TcpTxStateType TxState; /**< Send message status @range NA */

    DoIP_BufferStateType TxBufState; /**< Status of buffer @range NA */
    PduLengthType        TxBufPos;   /**< The amount of data sended @range NA */

    DoIP_TcpTxMsgType TxMsgType;       /**< Message type @range NA */
    PduLengthType     TxMaxDataLength; /**< How much data needs to be sent @range NA*/

    PduIdType PdurTxPduId;  /**< DoIP tx pdu id, to identify which buffer send data shall use @range 0..255 */
    PduIdType PdurTxPduRef; /**< DoIP Tx uppper(PDUR) pdu id @range 0..255 */
} DoIP_TcpTxCtrlType;

/**
 * @brief Send/Receive context of TCP
 */
typedef struct
{
    DoIP_TcpRxCtrlType RxCtrl; /**< Receive message context of TCP @range NA */
    DoIP_TcpTxCtrlType TxCtrl; /**< Send message context of TCP @range NA */

    DoIP_TpQueueType TxTpQueue; /**< queue for DoIP_TpTransmit info, eg. meta data @range NA */
    DoIP_NonDiagQueueType
        TxNonDiagQueue; /**< queue for diag ack/nack, routing active req/rsp,alive check req/rsp, tcp nack @range NA */
    DoIP_IfQueueType TxIfQueue; /**< queue for uudt message @range NA */

    uint8 RxBuf[DOIP_RX_BUFFER_SIZE]; /**< Receive buffer @range NA */
    uint8 TxBuf[DOIP_TX_BUFFER_SIZE]; /**< Send buffer @range NA */
} DoIP_TcpTxRxContextType;

/**
 * @brief Handle diagnostic context
 */
typedef struct
{
    uint8            TxRxCtxIdx;   /**< Index of DoIP_TcpTxRxContext @range 0..255 */
    uint16           SoadTxPduRef; /**< Used for send diagnsitc ack @range 0..65535 */
    SoAd_SoConIdType SoConId;      /**< Used for getDoIP_TcpConnStatusType @range NA */
} DoIP_HandleDiagContextType;

/* =========================================== external data declarations =========================================== */

/**
 * @brief Number of connections that have been established
 *        @range 0..255
 */
extern uint8 DoIP_CurrentTcpConnCnt;

/**
 * @brief Status of TCP connection
 *        @range NA
 */
extern DoIP_TcpConnStatusType DoIP_TcpConnStatus[DOIP_TCP_MAX_CONNECTION];

/**
 * @brief Send and receive context of TCP
 *        @range NA
 */
extern DoIP_TcpTxRxContextType DoIP_TcpTxRxContext[DOIP_TCP_MAX_CONNECTION];

/* ========================================= external function declarations ========================================= */

/* cfg */

/**
 * @brief       Get TCP configuration.
 * @param[in]   soadRxPduId: Specify which TCP configuration to get.
 * @return      DoIP_PBConnectionCfgType
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 5016 ++ */ /* VL_DoIP_5016 */
const DoIP_PBConnectionCfgType* DoIP_GetTcpConnBySoAdRxPduId(PduIdType soadRxPduId);
/* PRQA S 5016 -- */

/**
 * @brief       Get PdurRxPdu.
 * @param[in]   sa: Used to match channel.
 * @param[in]   ta: Used to match channel.
 * @param[out]  pdurRxPduRef: PdurRxPdu.
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_GetPduRRxPduRef(uint16 sa, uint16 ta, PduIdType* pdurRxPduRef);

/**
 * @brief       Get SoadTxPduRef.
 * @param[in]   soadRxPduId: Specify which pdu'ref to get.
 * @param[out]  soadTxPduRef: SoadTxPduRef.
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_GetSoAdTxPduRefBySoAdRxPduId(PduIdType soadRxPduId, uint16* soadTxPduRef);

/**
 * @brief       Get index of channel.
 * @param[in]   pdurRxPduId: Specifies the pdu associated with the channel.
 * @param[out]  index: Index of channel.
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_GetChannelIdxByPduRRxPduId(PduIdType pdurRxPduId, uint16* index);

/**
 * @brief       Get index of channel.
 * @param[in]   pdurTxPduId: Specifies the pdu associated with the channel.
 * @param[in]   pduType: PduType - IFPDU or TPPDU.
 * @param[out]  index: Index of channel.
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_GetChannelIdxByPduRTxPduId(PduIdType pdurTxPduId, DoIP_PBPduTypeType pduType, uint16* index);

/* connect */

/**
 * @brief       Open or close TCP connection.
 * @param[inout] active: Specifies open or close, and return status of connectin.
 * @return      boolean
 * @retval      TRUE: Switch success.
 * @retval      FALSE: Switch fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
boolean DoIP_SwitchTcp(const boolean* active);

/**
 * @brief       Return status of TCP.
 * @param[in]   channelIdx: Obtain SA from the channel.
 * @return      boolean
 * @retval      TRUE: Online.
 * @retval      FALSE: Offline.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
boolean DoIP_IsSoConOnline(uint16 channelIdx);

/**
 * @brief       Notify status of TCP.
 * @param[in]   soConId: Specifies connection.
 * @param[in]   mode: Status of connection.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_SoConModeChgTcp(SoAd_SoConIdType soConId, SoAd_SoConModeType mode);

/**
 * @brief       Close TCP.
 * @param[in]   soConId: Specifies connection.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_CloseTcpConnection(SoAd_SoConIdType soConId);

/* connect's status */

/**
 * @brief       Get index of DoIP_TcpConnStatus.
 * @param[in]   soadTxPduId: Specifies connection.
 * @param[out]  index: Index of DoIP_TcpConnStatus.
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_GetTcpConnStatusIdxBySoAdTxPduId(uint16 soadTxPduId, uint8* index);

/**
 * @brief       Get index of DoIP_TcpConnStatus.
 * @param[in]   soadRxPduId: Specifies connection.
 * @param[out]  index: Index of DoIP_TcpConnStatus.
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_GetTcpConnStatusIdxBySoAdRxPduId(uint16 soadRxPduId, uint8* index);

/**
 * @brief       Get index of DoIP_TcpConnStatus.
 * @param[in]   sa: Match DoIP_TcpConnStatus by sa.
 * @param[out]  index: Index of DoIP_TcpConnStatus.
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_GetTcpConnStatusIdxBySa(uint16 sa, uint8* index);

/**
 * @brief       Reset of DoIP_TcpConnStatus.
 * @param[in]   index: Index of DoIP_TcpConnStatus.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_ResetTcpConnStatus(uint8 index);

/* tx rx context */

/**
 * @brief       Get index of DoIP_TcpTxRxContext.
 * @param[in]   pdurTxPduId: Match DoIP_TcpConnStatus by pdurTxPduId.
 * @param[out]  txRxCtxIdx: Index of DoIP_TcpTxRxContext.
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_GetTcpTxRxCtxIdxByPduRTxPduId(PduIdType pdurTxPduId, uint8* txRxCtxIdx);

/**
 * @brief       Get index of DoIP_TcpTxRxContext.
 * @param[in]   soadRxPduId: Match DoIP_TcpConnStatus by soadRxPduId.
 * @param[out]  txRxCtxIdx: Index of DoIP_TcpTxRxContext.
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_GetTcpTxRxCtxIdxBySoAdRxPduId(PduIdType soadRxPduId, uint8* txRxCtxIdx);

/**
 * @brief       Get index of DoIP_TcpTxRxContext.
 * @param[in]   channelIdx: Obtain SA from the channel.
 * @param[out]  txRxCtxIdx: Index of DoIP_TcpTxRxContext.
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_GetTcpTxRxCtxIdxByChannelIdx(uint16 channelIdx, uint8* txRxCtxIdx);

/**
 * @brief       Reset of send DoIP_TcpTxRxContext.
 * @param[in]   txRxCtxIdx: Index of DoIP_TcpConnStatus.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_ResetTcpTxContext(uint8 txRxCtxIdx);

/**
 * @brief       Reset of receive DoIP_TcpTxRxContext.
 * @param[in]   txRxCtxIdx: Index of DoIP_TcpConnStatus.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_ResetTcpRxContext(uint8 txRxCtxIdx);

/**
 * @brief       Reset of send/receive DoIP_TcpTxRxContext.
 * @param[in]   txRxCtxIdx: Index of DoIP_TcpConnStatus.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_ResetTcpTxRxContext(uint8 txRxCtxIdx);

/* send */

/**
 * @brief       Check whether the metadata is valid.
 * @param[in]   channelIdx: Index of channel.
 * @param[in]   pduInfoPtr: PduInfoType
 * @return      boolean
 * @retval      TRUE: Valid.
 * @retval      FALSE: Invalid.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
boolean DoIP_IsValidMatadata(uint16 channelIdx, const PduInfoType* pduInfoPtr);

/**
 * @brief       Trigger send TP message.
 * @param[in]   soadTxPduRef: Specifies the connection to send the message.
 * @param[in]   pduInfoPtr: Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 * (SduLength).
 * @param[in]   txCtrl: Send context
 * @param[in]   msgType: message type
 * @return      Std_ReturnType
 * @retval      E_OK: Send success.
 * @retval      E_NOT_OK: Send fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_TriggerTpTransmit(
    PduIdType           soadTxPduRef,
    const PduInfoType*  pduInfoPtr,
    DoIP_TcpTxCtrlType* txCtrl,
    DoIP_TcpTxMsgType   msgType);

/**
 * @brief       Trigger send TP message.
 * @param[in]   pdurTxPduId: Used find channel.
 * @param[in]   pduInfoPtr: Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 * (SduLength).
 * @return      Std_ReturnType
 * @retval      E_OK: Send success.
 * @retval      E_NOT_OK: Send fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_TpTransmitInternal(PduIdType pdurTxPduId, const PduInfoType* pduInfoPtr);

/**
 * @brief       Trigger send IF message.
 * @param[in]   pdurTxPduId: Used find channel.
 * @param[in]   pduInfoPtr: Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 * (SduLength).
 * @return      Std_ReturnType
 * @retval      E_OK: Send success.
 * @retval      E_NOT_OK: Send fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_IfTransmitInternal(PduIdType pdurTxPduId, const PduInfoType* pduInfoPtr);

/**
 * @brief        Copy non TP data form send buffer.
 * @param[in]    pduInfoPtr: Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 * (SduLength).
 * @param[inout] ctx: Send receive context.
 * @param[out]   availableDataPtr: Indicates the remaining number of bytes that are available in the upper layer
 * module's Tx buffer.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: Data has been copied to the transmit buffer completely as requested.
 * @retval      BUFREQ_E_NOT_OK:  Data has not been copied. Request failed.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
BufReq_ReturnType DoIP_SoAdTpCopyTxNoTpData(
    DoIP_TcpTxRxContextType* ctx,
    const PduInfoType*       pduInfoPtr,
    PduLengthType*           availableDataPtr);

/**
 * @brief        Copy TP data form send buffer.
 * @param[in]    pduInfoPtr: Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 * (SduLength).
 * @param[in]    retry: This parameter is used to acknowledge transmitted data or toretransmit data after transmission
 * problems.
 * @param[inout] ctx: Send receive context.
 * @param[out]   availableDataPtr: Indicates the remaining number of bytes that are available in the upper layer
 * module's Tx buffer.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: Data has been copied to the transmit buffer completely as requested.
 * @retval      BUFREQ_E_NOT_OK:  Data has not been copied. Request failed.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
BufReq_ReturnType DoIP_SoAdTpCopyTxTpData(
    DoIP_TcpTxRxContextType* ctx,
    const PduInfoType*       pduInfoPtr,
    const RetryInfoType*     retry,
    PduLengthType*           availableDataPtr);

/* pending */

/**
 * @brief       Enqueue TP message.
 * @param[in]   queue: Used for save TP message.
 * @param[in]   pdurTxPduId: Specifies the associated PDU.
 * @param[in]   pduInfoPtr: Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 * (SduLength).
 * @return      Std_ReturnType
 * @retval      E_OK: Enqueue success.
 * @retval      E_NOT_OK: Enqueue fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_EnqueueTpMsg(DoIP_TpQueueType* queue, PduIdType pdurTxPduId, const PduInfoType* pduInfoPtr);

/**
 * @brief       Enqueue IF message.
 * @param[in]   queue: Used for save IF message.
 * @param[in]   pdurTxPduId: Specifies the associated PDU.
 * @param[in]   pduInfoPtr: Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 * (SduLength).
 * @return      Std_ReturnType
 * @retval      E_OK: Enqueue success.
 * @retval      E_NOT_OK: Enqueue fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_EnqueueIfMsg(DoIP_IfQueueType* queue, PduIdType pdurTxPduId, const PduInfoType* pduInfoPtr);

/**
 * @brief       Clear IF queue.
 * @param[in]   ctx: Send receive context.
 * @param[in]   isNotifyPdur: whether or not notify PduR.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DOIP_ClearIfQueue(DoIP_TcpTxRxContextType* ctx, boolean isNotifyPdur);

/**
 * @brief       Clear TP queue.
 * @param[in]   ctx: Send receive context.
 * @param[in]   isNotifyPdur: whether or not notify PduR.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DOIP_ClearTpQueue(DoIP_TcpTxRxContextType* ctx, boolean isNotifyPdur);

/**
 * @brief       Clear pending routing activation request.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_ClearPendingRaMsg(void);

/**
 * @brief       Handle pending message.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_HandlePendingMsg(void);

/* rcv */

/**
 * @brief       Get resource for receive TP message.
 * @param[in]   soadRxPduId: Specifies the connection to receive the message.
 * @param[out]  soadTxPduRef: Connection's pduref.
 * @param[out]  soConId: ID of connection.
 * @param[out]  txRxCtxIdx: Index of DoIP_TcpTxRxContext.
 * @param[out]  tcpConnStatusIdx: Index of DoIP_TcpConnStatus.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: Get resource success.
 * @retval      BUFREQ_E_NOT_OK: Get resource fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
BufReq_ReturnType DoIP_GetResourseForRcvTpMsg(
    PduIdType         soadRxPduId,
    uint16*           soadTxPduRef,
    SoAd_SoConIdType* soConId,
    uint8*            txRxCtxIdx,
    uint8*            tcpConnStatusIdx);

/**
 * @brief       Get resource for receive TP message.
 * @param[in]   pduInfoPtr: Provides the source buffer (SduDataPtr) and the number of bytes to be copied (SduLength).
 * @param[in]   txRxCtxIdx: Index of DoIP_TcpTxRxContext.
 * @param[in]   soadTxPduRef: Specifies the connection to receive the message.
 * @param[out]  bufferSizePtr: Available receive buffer after data has been copied.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: Success.
 * @retval      BUFREQ_E_NOT_OK: Fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
BufReq_ReturnType DoIP_HandleTpRxHeader(
    const PduInfoType* pduInfoPtr,
    uint16             soadTxPduRef,
    PduLengthType*     bufferSizePtr,
    uint8              txRxCtxIdx);

/**
 * @brief       Handle receive messages continuously.
 * @param[in]   pduInfoPtr: Provides the source buffer (SduDataPtr) and the number of bytes to be copied (SduLength).
 * @param[in]   soadTxPduRef: Specifies the connection to receive the message.
 * @param[out]  bufferSizePtr: Available receive buffer after data has been copied.
 * @param[out]  handleDiagCtx: Context of handle diagnostic message.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: Success.
 * @retval      BUFREQ_E_NOT_OK: Fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
BufReq_ReturnType DoIP_HandleTpCFRx(
    DoIP_HandleDiagContextType* handleDiagCtx,
    const PduInfoType*          pduInfoPtr,
    uint16                      soadTxPduRef,
    PduLengthType*              bufferSizePtr);

/* timer */

/**
 * @brief       Handle timers.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_HandleTcpTimeout(void);
#ifdef __cplusplus
}
#endif
#endif
