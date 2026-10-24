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
 **  @file               : Dcm_Dsl.h
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : DcmDsl Functions/Types Declaration
 **
 ***********************************************************************************************************************/
#ifndef DCM_DSL_H_
#define DCM_DSL_H_

/* =================================================== inclusions =================================================== */
#include "Dcm_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#define DCM_METADATA_SIZE 4uL /**< dcm used metadata size*/
/* ================================================ type definitions ================================================ */
/**
 * @brief Dcm State enum
 */
typedef enum
{
    DCM_MSG_IDLE,       /**< The protocol is active, dcm is wating for new requests @range 0x00*/
    DCM_MSG_RECEIVING,  /**< Dcm is receiving a diag message, in the process of copy @range 0x01*/
    DCM_MSG_RECEIVED,   /**< Dcm has received the complete message, but not doing external processing @range 0x02*/
    DCM_MSG_PROCESSING, /**< Dcm has received the complete message, in the process of external processing @range 0x03*/
#if (STD_ON == DCM_MULTICORE_ENABLED)
    DCM_MSG_SENDING, /**< Dcm has done processing and sent the response, waiting for sending @range 0x04*/
#endif
    DCM_MSG_SENT,         /**< Dcm has done processing and sent the response, waiting for TxConfirmation @range 0x05*/
    DCM_MSG_PENDING_SENT, /**< Dcm has sent a pending response, still need further process @range 0x06*/
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    DCM_MSG_PAGEDBUFFER_SENT, /**< Dcm has sent some pages, still need further process @range 0x07*/
#endif
} Dcm_StateType;

/**
 * @brief Dcm ComM State
 */
typedef enum
{
    DCM_COMM_NO_COMMUNICATION     = 0, /**< communication is "NO Communication" status @range 0x00*/
    DCM_COMM_SILENT_COMMUNICATION = 1, /**< communication is "Silent Communication" status @range 0x01*/
    DCM_COMM_FULL_COMMUNICATION   = 2  /**< communication is "Full Communication" status @range 0x02*/
} Dcm_CommStateType;

/**
 * @brief Dcm diagnostic State
 */
typedef enum
{
    DCM_INACTIVE, /**< diagnostic State is inactive @range 0x00*/
    DCM_ACTIVE    /**< diagnostic State is active @range 0x01*/
} Dcm_DiagnosticStateType;

/**
 * @brief Dcm protocol control type
 */
typedef struct
{
    PduIdType                    RxPduId;        /**< requested RxPduId @range NA*/
    uint16                       ConnectionId;   /**< requested mainConnectionId @range 0..65535*/
    Dcm_StateType                State;          /**< current State of protocol @range NA*/
    boolean                      ProtocolActive; /**< indicates whether the protocol is active @range 0..1*/
    Dcm_ExtendedOpStatusType     OpStatus;       /**< the current operation status of protocol @range NA*/
    uint8                        Sid;            /**< the requested sid @range 0..255*/
    uint16                       ServIndex;      /**< current requested service index @range 0..65535*/
    uint16                       SubServIndex;   /**< current requested subService index @range 0..65535*/
    Dcm_NegativeResponseCodeType ErrorCode;      /**< store the transmitting nrc @range 0xNA*/
    Dcm_MsgLenType               RxCopyLen;      /**< current rx copied length @range NA*/
    Dcm_MsgLenType               TxCopyLen;      /**< current tx copied length @range NA*/
    uint8*                       MetaDataPtr;    /**< stored metadata @range NA*/
    uint32                       P2Timer;        /**< current P2 timer @range 0..0xFFFFFFFF*/
    uint32                       P4Timer;        /**< current P4 timer @range 0..0xFFFFFFFF*/
    uint8                        PendingCounter; /**< current pending times counter @range 0..255*/
    uint8 ConcurrentTesterPresentRxCopyLen; /**< current functional address request copied length (for 0x3E80) @range
                                               0..255*/
    boolean ConcurrentTesterPresent; /**< indicates it is possible that an function addressed 0x3E80 is requesting
                                        @range 0..1*/
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    boolean        PagedBufferStarted; /**< whether a pagedbuffer response is in process @range NA*/
    Dcm_MsgLenType RemainPageLength;   /**< remain pagedbuffer response length @range NA*/
    Dcm_MsgLenType FirstPageLength;    /**<current pagedbuffer response length @range NA*/
#endif
} Dcm_ProtocolCtrlType;

/**
 * @brief Dcm confirmation info type
 */
typedef struct
{
    boolean      Pending;      /**< whether there is a pending confirmation notification @range 0..1*/
    uint8        Sid;          /**< The requested service Id @range 0..255*/
    unsigned int ReqType : 1;  /**< (Pos LSB+0) 0 = physical request 1 = functional request @range 0..0xFFFFFFFF*/
    uint16       ConnectionId; /**< The requested connection id @range 0..65535*/
    Dcm_ConfirmationStatusType ConfirmationStatus; /**< curruent confirmation status @range NA*/
} Dcm_ConfirmationHandleType;

/**
 * @brief Dcm session control unit
 */
typedef struct
{
    uint8 CurrentSessionProtocolId; /**< the protocol id that is current holding the non-default session @range 0..255*/
    Dcm_SesCtrlType CurrentSession; /**< current session id @range NA*/
    Dcm_SesCtrlType NewSession;     /**< new session that is about to change @range NA*/
    uint8           NewSessionProtocolId; /**< the new session requested protocol id @range 0..255*/
    uint32          SessionTimer;         /**< the current session timer for S3 @range 0..0xFFFFFFFF*/
} Dcm_SessionCtrlType;

/**
 * @brief Dcm security control unit
 */
typedef struct
{
    uint32 SecurityTimer;  /**< the current security timer for delayed request @range 0..0xFFFFFFFF*/
    uint8  AttemptCounter; /**< current attempt counter @range 0..255*/
#if ((STD_ON == DCM_UDS_0X27) && (STD_ON == DCM_SECURITY_READOUT))
    boolean InitialGetAttemptCounter; /**< TRUE when initial getAttemptCounter has not called @range 0..1*/
    boolean PendingGetAttemptCounter; /**< TRUE when there is pending getAttemptCounter @range 0..1*/
    boolean PendingSetAttemptCounter; /**< TRUE when there is pending setAttemptCounter @range 0..1*/
    boolean DelayedSetAttemptCounter; /**< TRUE when there is pending setAttemptCounter and a new request is coming
                                         @range 0..1*/
#endif
} Dcm_SecurityCtrlType;

/**
 * @brief active diagnostic status enum
 */
/* PRQA S 1271++ */ /* VL_Dcm_1271 */
typedef enum
{
    DCM_COMM_ACTIVE     = 0u, /**< communication is active @range 0x00*/
    DCM_COMM_NOT_ACTIVE = 1u, /**< communication is not active @range 0x01*/
} Dcm_ActiveDiagnosticType;
/* PRQA S 1271-- */ /* VL_Dcm_1271 */
/* =========================================== external data declarations =========================================== */
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief current comm State @range NA
 */
extern Dcm_CommStateType Dcm_CommState[DCM_MAINCONNECTION_NUM];
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
extern boolean Dcm_RequestRejectedDueToBoot;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
extern boolean Dcm_ResRejectedDueToAFTER_RESET;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
/**
 * @brief indicates if there is pending setProgramConditions globally  @range 0..1
 */
extern boolean Dcm_PendingSetProg;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
/**
 * @brief indicates if there is onging setProgramConditions which is process in Dcm_MainFunction  @range 0..1
 */
extern boolean Dcm_ProcessingSetProg;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief current active diagnostic status  @range NA
 */
extern Dcm_ActiveDiagnosticType Dcm_ActiveDiagnostic;
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/**
 * @brief session control unit  @range NA
 */
extern Dcm_SessionCtrlType Dcm_SessionCtrl;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief current security level  @range NA
 */
extern Dcm_SecLevelType Dcm_CurrentSecurity;
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#if ((STD_ON == DCM_UDS_0X27) && (STD_ON == DCM_SECURITY_READOUT))
#define DCM_START_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
/**
 * @brief current security readout timer for initial read  @range 0..0xFFFFFFFF
 */
extern uint32 Dcm_SecurityReadoutTimer;
#define DCM_STOP_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
#endif
#if (STD_ON == DCM_UDS_0X27)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/**
 * @brief security control unit  @range NA
 */
extern Dcm_SecurityCtrlType Dcm_SecurityCtrl[DCM_SECURITY_NUM];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#if (STD_ON == DCM_SECURITY_READOUT)
#define DCM_START_SEC_VAR_INIT_BOOLEAN
#include "Dcm_MemMap.h"
/**
 * @brief indicates if there is pending getAttemptCounter globally  @range 0..1
 */
extern boolean Dcm_PendingGetAttemptCounter;
#define DCM_STOP_SEC_VAR_INIT_BOOLEAN
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_INIT_BOOLEAN
#include "Dcm_MemMap.h"
/**
 * @brief indicates if there is pending setAttemptCounter globally  @range 0..1
 */
extern boolean Dcm_PendingSetAttemptCounter;
#define DCM_STOP_SEC_VAR_INIT_BOOLEAN
#include "Dcm_MemMap.h"
#endif
#endif
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/**
 * @brief protocol control unit  @range NA
 */
extern Dcm_ProtocolCtrlType Dcm_ProtocolCtrl[DCM_PROTOCOLROW_NUM];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/**
 * @brief msgContext for storing reqData and resData  @range NA
 */
extern Dcm_MsgContextType Dcm_MsgContext[DCM_PROTOCOLROW_NUM];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief the whole dcm buffer (all TX+RX)  @range 0..255
 */
extern uint8 Dcm_Buffer[DCM_TOTAL_BUFFER_SIZE];
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#if (DCM_METADATA_NUM > 0u)
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief stored metadata  @range NA
 */
extern uint8 Dcm_MetaData[DCM_METADATA_NUM * DCM_METADATA_SIZE];
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#endif
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief The current request queue index  @range 0..255
 */
extern uint8 Dcm_QueuedIndex;
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief request queue buffer  @range 0..255
 */
extern uint8 Dcm_RequestQueuedBuffer[2u][DCM_REQUEST_QUEUED_BUFFER_SIZE];
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/**
 * @brief request queue control unit  @range NA
 */
extern Dcm_QueuedRequestCtrlType Dcm_QueuedRequestCtrl[2u];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#endif
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/**
 * @brief confirmation info for delayed confirmation callout  @range NA
 */
extern Dcm_ConfirmationHandleType Dcm_ConfirmationHandle[DCM_PROTOCOLROW_NUM];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/**
 * @brief pduInfoBuffer for multicore transmission  @range NA
 */
extern PduInfoType Dcm_TempPduInfo[DCM_PROTOCOLROW_NUM];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/* ========================================= external function declarations ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * @brief         initialize the session to default status
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DslInternal_SessionInit(void);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * @brief         initialize the security to default status
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DslInternal_SecurityInit(void);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * @brief         initialize the protocolCtrl to initial status
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DslInternal_ProtocolInit(void);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * @brief         This function sets the security level
 * @param[in]     SecurityLevel : The requested sercurity level
 * @return        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DslInternal_SetSecurityLevel(Dcm_SecLevelType SecurityLevel);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * @brief         This function resets to default session
 * @param[in]     Immediate : whether the request is immediately effective
 * @return        void
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DslInternal_ResetToDefaultSession(boolean Immediate);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * @brief         This function sets the session
 * @param[in]     ProtocolId   : the current protocolId
 * @param[in]     SesCtrlType  : the requested session
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        DCM_E_PENDING : Request was pending
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType DslInternal_SetSesCtrlType(uint8 ProtocolId, Dcm_SesCtrlType SesCtrlType);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * @brief         This function searches for matched protocol and connection Id by RxPduId
 * @param[in]     Id           : Identification of the I-PDU.
 * @param[out]    ProtocolId   : the matched protocolId
 * @param[out]    connectionId : the matched connectionId (under the protocol)
 * @return        Std_ReturnType
 * @retval        E_OK     : Succesfully find the matched one
 * @retval        E_NOT_OK : Fail to find any mathced one
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType DslInternal_FindProtocolRowByRxPduId(PduIdType Id, uint8* ProtocolId, uint16* connectionId);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * @brief         This function sets the current diagnosticState and notify ComM
 * @param[in]     ConnectionId    : the current connection id
 * @param[in]     diagnosticState : the current diagnostic State
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DslInternal_SetDiagnosticState(uint16 ConnectionId, Dcm_DiagnosticStateType diagnosticState);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * @brief         This function sets the protocol State to idle
 * @param[in]     protocolId         : the current protocol id
 * @param[in]     confirmationStatus : the final confirmation status
 * @param[in]     immediateHandle    : whether to handle supplier/manufacturer confirmation immediately or
 *                                     asynchronously
 * @param[in]     reachDsp           : whether to call DspInternal_DcmConfirmation
 * @return        void
 * @reentrant     TRUE for different protocolId
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DslInternal_SetStateIdle(
    uint8                             protocolId,
    const Dcm_ConfirmationStatusType* confirmationStatus,
    boolean                           immediateHandle,
    boolean                           reachDsp);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

/**
 * @brief         This function initialize the queued request ctrl status
 * @param[in]     index : the requested index of queued request ctrl
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DslInternal_InitChannelQueuedRequestCtrl(uint8 index);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

/**
 * @brief         This function stops the target protocol
 * @param[in]     ProtocolId : the stopped protocol Id
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE for different protocolId
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
Std_ReturnType DslInternal_StopProtocol(uint8 ProtocolId);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

/**
 * @brief         This function actually sends the data
 * @param[in]     ProtocolId : The index of the protocol
 * @return        void
 * @reentrant     TRUE for different protocolId
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DslInternal_ProcessTransmit(uint8 ProtocolId);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

/**
 * @brief         This function initialize pagedBuffer vars
 * @param[in]     protocolCtrlPtr : the target protocolCtrlPtr
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
void DslInternal_PagedBufferInit(Dcm_ProtocolCtrlType* protocolCtrlPtr);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#ifdef __cplusplus
}
#endif
#endif
