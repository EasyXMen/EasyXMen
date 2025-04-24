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
**************************************************************************** **
**                                                                           **
**  FILENAME    :                                                            **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION :                                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#ifndef DCM_TYPES_H
#define DCM_TYPES_H

#include "Std_Types.h"
#include "Rte_Dcm_Type.h"
#include "Det.h"
#include "Dcm_PBcfg.h"
#include "Dcm_Cfg.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

#define DCM_MODULE_ID   (0x35u)
#define DCM_VENDOR_ID   (62u)   /*vendor*/
#define DCM_INSTANCE_ID (0x00u) /*instance*/
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define DCM_INVALID_UINT8  (0xFFu)
#define DCM_INVALID_UINT16 (0xFFFFu)
#define DCM_INVALID_UINT32 (0xFFFFFFFFUL)
#define DCM_INVALID_PDUID  (0xFFu)
/*****************************Development error values ***********************/
#define DCM_E_INTERFACE_TIMEOUT      (0x01u) /*Application-Interface: Timeout*/
#define DCM_E_INTERFACE_RETURN_VALUE (0x02u)
/*Application-Interface: Return-value out of range*/
#define DCM_E_INTERFACE_BUFFER_OVERFLOW (0x03u) /*Application-Interface: Buffer Overflow*/
#define DCM_E_UNINIT                    (0x05u) /*Internal: DCM not initialized*/
#define DCM_E_PARAM                     (0x06u)
/*DCM API function with invalid input parameter*/
#define DCM_E_PARAM_POINTER (0x07u)
/*DCM API service invoked with NULL POINTER as parameter*/
#define DCM_E_INIT_FAILED              (0x08u) /*Dcm initialisation failed*/
#define DCM_E_SET_PROG_CONDITIONS_FAIL (0x09u) /*Storing the ProgConditions failed*/
/*******************************Dcm Module API ID*****************************/
#define DCM_STARTOFRECEPTION_ID       (0x46u)
#define DCM_COPYRXDATA_ID             (0x44u)
#define DCM_TPRXINDICATION_ID         (0x45u)
#define DCM_COPYTXDATA_ID             (0x43u)
#define DCM_TPTXCONFIRMATION_ID       (0x48u)
#define DCM_TXCONFIRMATION_ID         (0x40u)
#define DCM_GETSESCTRLTYPE_ID         (0x06u) /*Dcm_GetSesCtrlType() */
#define DCM_GETSECURITYLEVEL_ID       (0x0Du) /*Dcm_GetSecurityLevel()*/
#define DCM_GETACTIVEPROTOCOL_ID      (0x0Fu) /*Dcm_GetActiveProtocol()*/
#define DCM_COMM_NOCOMMODEENTERED     (0x21u) /*Dcm_Comm_NoComModeEntered()*/
#define DCM_COMM_SILENTCOMMODEENTERED (0x22u) /*Dcm_Comm_SilentComModeEntered()*/
#define DCM_COMM_FULLCOMMODEENTERED   (0x23u) /*Dcm_Comm_FULLComModeEntered()*/
#define DCM_GETVERSIONINFO_ID         (0x24u) /*Dcm_GetVersionInfo()*/
#define DCM_GETVIN_ID                 (0x07u) /*Dcm_GetVin()*/
#define DCM_MAIN_FUNCTION_ID          (0x25u) /*Dcm_Main_Function()*/
#define DCM_DEMTRIGGERONDTCSTATUS_ID  (0x2Bu) /*Dcm_DemTriggerOnDTCStatus()*/
#define DCM_RESETTODEFAULTSESSION_ID  (0x2Au) /*Dcm_ResetToDefaultSession()*/
#define DCM_TRIGGERONEVENT_ID         (0x2Du) /*Dcm_TriggerOnEvent()*/
#define DCM_SETACTIVEDIAGNOSTIC_ID    (0x56u) /*Dcm_SetActiveDiagnostic()*/
#define DCM_INIT_ID                   (0x01u) /*Dcm_Init()*/

#define DCM_METADATA_LENGTH_MASK      (uint8)0x0F

#define DCM_ADDRESS_EXTENSION_8_MASK  (uint8)0x10

#define DCM_SA16_AND_TA16_MASK        (uint8)0x20

#define DCM_UNUSED(a)                 (void)(a)

/* Definition of DCM_NRC22_SPECIFIC_CAUSE_CODE macro to control whether to report the cause of NRC22. */
#if !defined(DCM_NRC22_SPECIFIC_CAUSE_CODE)
#define DCM_NRC22_SPECIFIC_CAUSE_CODE STD_OFF
#endif /* DCM_NRC22_SPECIFIC_CAUSE_CODE */

#if (STD_ON == DCM_NRC22_SPECIFIC_CAUSE_CODE)
typedef uint8 Dcm_SpecificCauseCodeType;
#endif /* STD_ON == DCM_NRC22_SPECIFIC_CAUSE_CODE */

typedef uint8 Dcm_DidSupportedType;
#define DCM_DID_SUPPORTED     ((Dcm_DidSupportedType)0x00)
#define DCM_DID_NOT_SUPPORTED ((Dcm_DidSupportedType)0x01)

typedef uint8 Dcm_CommunicationModeType;
#define DCM_ENABLE_RX_TX_NORM            0x00u
#define DCM_ENABLE_RX_DISABLE_TX_NORM    0x01u
#define DCM_DISABLE_RX_ENABLE_TX_NORM    0x02u
#define DCM_DISABLE_RX_TX_NORMAL         0x03u
#define DCM_ENABLE_RX_TX_NM              0x04u
#define DCM_ENABLE_RX_DISABLE_TX_NM      0x05u
#define DCM_DISABLE_RX_ENABLE_TX_NM      0x06u
#define DCM_DISABLE_RX_TX_NM             0x07u
#define DCM_ENABLE_RX_TX_NORM_NM         0x08u
#define DCM_ENABLE_RX_DISABLE_TX_NORM_NM 0x09u
#define DCM_DISABLE_RX_ENABLE_TX_NORM_NM 0x0Au
#define DCM_DISABLE_RX_TX_NORM_NM        0x0Bu

typedef uint8 Dcm_EcuStartModeType;
#define DCM_COLD_START ((Dcm_EcuStartModeType)0x00)
#define DCM_WARM_START ((Dcm_EcuStartModeType)0x01)

#if (                                                                                       \
    (STD_ON == DCM_UDS_SERVICE0X14_ENABLED) || (STD_ON == DCM_OBD_SERVICE0X0A_ENABLED)      \
    || (STD_ON == DCM_OBD_SERVICE0X02_ENABLED) || (STD_ON == DCM_OBD_SERVICE0X03_ENABLED)   \
    || (STD_ON == DCM_OBD_SERVICE0X04_ENABLED) || (STD_ON == DCM_OBD_SERVICE0X06_ENABLED)   \
    || ((STD_ON == DCM_OBD_SERVICE0X07_ENABLED)) || (STD_ON == DCM_UDS_SERVICE0X19_ENABLED) \
    || (STD_ON == DCM_UDS_SERVICE0X85_ENABLED) || (STD_ON == DCM_UDS_SERVICE0X86_01_ENABLED))
#define DCM_DEM_SUPPOTR STD_ON
#else
#define DCM_DEM_SUPPOTR STD_OFF
#endif

/**********************************************************************
 *            UDS 0x36 service status define
 **********************************************************************/
#define DCM_UDS0X36_INIT        (0u)
#define DCM_UDS0X36_TO_DOWNLOAD (1u)
#define DCM_UDS0X36_TO_UPLOAD   (2u)
#define DCM_UDS0X38_0x01        (3u)
#define DCM_UDS0X38_0x02        (4u)
#define DCM_UDS0X38_0x03        (5u)
#define DCM_UDS0X38_0x04        (6u)
#define DCM_UDS0X38_0x05        (7u)

/****************************************************************************/
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/****************************************************************************/
#ifndef E_SESSION_NOT_ALLOWED
#define E_SESSION_NOT_ALLOWED ((Std_ReturnType)4)
#endif /* E_SESSION_NOT_ALLOWED */
/*Application does not allow the session change*/
#ifndef E_PROTOCOL_NOT_ALLOWED
#define E_PROTOCOL_NOT_ALLOWED ((Std_ReturnType)5)
#endif /* E_PROTOCOL_NOT_ALLOWED */
/*Application does not allow further processing of the protocol*/
#ifndef E_REQUEST_NOT_ACCEPTED
#define E_REQUEST_NOT_ACCEPTED ((Std_ReturnType)8) /**/
#endif                                             /* E_REQUEST_NOT_ACCEPTED */
#ifndef E_REQUEST_ENV_NOK
#define E_REQUEST_ENV_NOK ((Std_ReturnType)9) /**/
#endif                                        /* E_REQUEST_ENV_NOK */
#ifndef DCM_E_PENDING
#define DCM_E_PENDING ((Std_ReturnType)10) /**/
#endif                                     /* DCM_E_PENDING */
#ifndef E_COMPARE_KEY_FAILED
#define E_COMPARE_KEY_FAILED ((Std_ReturnType)11) /*Compare key failure*/
#endif                                            /* E_COMPARE_KEY_FAILED */
#ifndef DCM_E_FORCE_RCRRP
#define DCM_E_FORCE_RCRRP ((Std_ReturnType)12)
#endif /* DCM_E_FORCE_RCRRP */
/*Application requests sent immediately NRC = 0x78*/
/****************************************************************************/
typedef uint8 Dcm_StatusType;
#define DCM_E_OK                      ((Dcm_StatusType)0x00)
#define DCM_E_ROE_NOT_ACCEPTED        ((Dcm_StatusType)0x06)
#define DCM_E_PERIODICID_NOT_ACCEPTED ((Dcm_StatusType)0x07)
/****************************************************************************/
typedef uint8 Dcm_ReturnReadMemoryType;
#define DCM_READ_OK          ((Dcm_ReturnReadMemoryType)0x00)
#define DCM_READ_PENDING     ((Dcm_ReturnReadMemoryType)0x01)
#define DCM_READ_FAILED      ((Dcm_ReturnReadMemoryType)0x02)
#define DCM_READ_FORCE_RCRRP ((Dcm_ReturnReadMemoryType)0x03)
/****************************************************************************/
typedef uint8 Dcm_ReturnWriteMemoryType;
#define DCM_WRITE_OK          ((Dcm_ReturnWriteMemoryType)0x00)
#define DCM_WRITE_PENDING     ((Dcm_ReturnWriteMemoryType)0x01)
#define DCM_WRITE_FAILED      ((Dcm_ReturnWriteMemoryType)0x02)
#define DCM_WRITE_FORCE_RCRRP ((Dcm_ReturnWriteMemoryType)0x03)
/****************************************************************************/
/****************************************************************************/

#define NONE_PID          0x00u
#define SUPPORT_REQUEST   0x01u
#define NORMAL_REQUEST    0x02u
#define NEED_CALL_UDS_API 0x03u

#define OBD_DATA_LSB_MASK ((uint32)0x000000FFu)
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
typedef enum
{
    DCM_P2TIMER_ON = 0,
    DCM_P2TIMER_OFF = 1
} Dcm_P2StateType;

typedef struct
{
    uint32 Dcm_P2CurTimer;
    uint32 Dcm_P2ExpiredTimer;
    uint8 PendingNum;
    Dcm_P2StateType Dcm_P2State;
} Dcm_P2CtrlType;

/********************************************
 Channel operation status type / control type
 *******************************************/
typedef enum
{
    DCM_CH_IDLE = 0,    /*Channel "idle" state*/
    DCM_CH_OCCUPIED = 1 /*Channel "occupied" state*/
} Dcm_ChannelStateType;

typedef enum
{
    DCM_PENDING_REQUEST_NONE = 0,    /* No pending request */
    DCM_PENDING_REQUEST_RECEIVE = 1, /* Pending request is receiving */
    DCM_PENDING_REQUEST_READY = 2,   /* Pending request is ready */
    DCM_PENDING_REQUEST_PROCESS = 3  /* Pending Request is processing */
} Dcm_PendingRequestStateType;

typedef struct
{
    uint8 Dcm_ChannelCfgIndex;
    /*Static configuration channel index number, mapping connection index*/
    uint32 Dcm_BufferCunrentPosition;
    uint32 Dcm_BufferErasePosition;
    Dcm_ChannelStateType Dcm_ChannelRxState; /*Static configuration channel receiving state*/
    Dcm_ChannelStateType Dcm_ChannelTxState; /*Static configuration channel transmission status*/
} Dcm_ChannelCtrlType;

#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
typedef struct
{
    PduIdType Dcm_QueuedRequestRxPduId;
    uint32 Dcm_QueuedRequestBufferCunrentPosition;
    uint32 Dcm_QueuedRequestLength;
    Dcm_PendingRequestStateType Dcm_PendingRequestState;
} Dcm_QueuedRequestCtrlType;
#endif /* STD_ON == DCM_REQUEST_QUEUED_ENABLED */

typedef struct
{
    uint8 Dcm_ConnectionCfgIndex;
    boolean Dcm_ConnectionActive;
} Dcm_ConnectionCtrlType;

/************************************
 Message Run-time data structures
 ************************************/
typedef uint8 Dcm_MsgItemType;
typedef Dcm_MsgItemType* Dcm_MsgType;
typedef uint32 Dcm_MsgLenType;
typedef uint8 Dcm_IdContextType;

typedef struct
{
    uint8 ReqType; /*=FALSE:physical Addressing,=TRUE:Functional Addressing*/
    boolean SuppressPosResponse;
    /*:=FALSE:Allow positive response;=TRUE:Suppress positive response*/
    boolean CancelOperation;
    /*=FALSE:Not cancel Pending, =TRUE:Cancel Pending*/
} Dcm_MsgAddInfoType;

typedef struct
{
    Dcm_MsgType pReqData;      /*Point to request data*/
    Dcm_MsgLenType ReqDataLen; /*Request data length*/
    /*Old Request data length for 3E 80 Fun to resumed last pending length*/
    Dcm_MsgType pResData;      /*Point to the response data (including the SID data)*/
    Dcm_MsgLenType ResDataLen; /*Response packet data length(including the SID data)*/
    Dcm_MsgAddInfoType MsgAddInfo;
    /*Additional information for service requests and responses*/
    Dcm_MsgLenType ResMaxDataLen; /*The maximum number of bytes of response data*/
    Dcm_IdContextType IdContext;
    PduIdType DcmRxPduId; /*Request message identifier*/
} Dcm_MsgContextType;

typedef enum
{
    DCM_MSG_WAIT = 0,
    DCM_MSG_RECEIVED = 1,
    DCM_MSG_PROCESSED = 2,
    DCM_MSG_TRANSMISSION = 3,
    DCM_MSG_CONFIRMATION = 4
} Dcm_MsgStateType;

typedef enum
{
    DCM_POS_RSP = 0,         /*Positive response*/
    DCM_NEG_RSP = 1,         /*Negative response*/
    DCM_POS_RSP_SUPPRESS = 2 /*Positive response Suppress*/
} Dcm_RspType;

typedef struct
{
    uint8 SID;         /*Service ID*/
    uint8 Subfunction; /*Service Subfunction */
    Dcm_NegativeResponseCodeType NRC;
    /*Negative response code, default = 0xFF*/
    boolean SendFlag; /*=TRUE: being sent;=FALSE: not sent*/
    PduIdType DcmTxPduId;
    uint8 Dcm_RxCtrlChannelIndex;  /*Message receiver channel control block index number*/
    uint8 Dcm_TxCtrlChannelIndex;  /*Message Transmit channel control block index number*/
    Dcm_RspType RspStyle;          /*Type of response*/
    Dcm_MsgContextType MsgContext; /*Request/response message content*/
    Dcm_P2CtrlType Dcm_P2Ctrl;     /*P2Timer control block*/
    Dcm_MsgStateType Dcm_MsgState; /*Message status*/
    Std_ReturnType Dcm_Ret;
    Dcm_OpStatusType Dcm_OpStatus;
#if (STD_ON == DCM_GENERIC_CONNECTION)
    uint16 Dcm_MetaData_SA;
    uint16 Dcm_MetaData_TA;
    uint16 Dcm_MetaData_SA_Backup;
    uint16 Dcm_MetaData_TA_Backup;
    PduIdType DcmRxPduId;
#endif
} Dcm_MsgCtrlType;

/********************************************
      function addressing buffer
 ********************************************/
typedef struct
{
    PduLengthType Length;
    uint8 Buffer[DCM_FRAME_LENGTH];
    boolean ConcurrentTesterFailed;
} Dcm_FunctionMessageType;

typedef enum
{
    DCM_EQUALS = 0,
    DCM_EQUALS_NOT,
    DCM_GREATER_OR_EQUAL,
    DCM_GREATER_THAN,
    DCM_LESS_OR_EQUAL,
    DCM_LESS_THAN
} Dcm_ConditionType;

typedef struct
{
    Dcm_ConditionType DcmConditionType;
    /*some type referenced from other modules*/
} Dcm_ModeConditionCfgType;

typedef enum
{
    DCM_AND = 0,
    DCM_OR
} Dcm_LogicalOperatorType;

typedef struct
{
    Dcm_LogicalOperatorType DcmLogicalOperator;
    uint8 DcmModeRuleNrcValue;
} Dcm_ModeRuleCfgType;

typedef struct
{
    P2CONST(Dcm_ModeConditionCfgType, TYPEDEF, DCM_CONST) DcmModeCondition;
    P2CONST(Dcm_ModeRuleCfgType, TYPEDEF, DCM_CONST) DcmModeRule;
    /*DcmArgumentRef*/
} Dcm_ProcessingConditionsCfgType;

/***************************************************
 ********DcmDsp Container(Multiplicity=1)***********
 ***************************************************/
typedef enum
{
    DCM_BIG_ENDIAN = 0,
    DCM_LITTLE_ENDIAN = 1,
    DCM_OPAQUE = 2
} Dcm_DspEndianType;

/*security access*/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_GetSeedFncType)(
#if (DCM_SECURITY_ADR_MAXSIZE > 0)
    const uint8* SecurityAccessDataRecord,
#endif
    Dcm_OpStatusType OpStatus,
    uint8* Seed,
    Dcm_NegativeResponseCodeType* ErrorCode);
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_CompareKeyFncType)(
    P2CONST(uint8, AUTOMATIC, DCM_VAR) key,
    Dcm_OpStatusType OpStatus,
    Dcm_NegativeResponseCodeType* ErrorCode);
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_GetSecurityAttemptCounterFncType)(
    Dcm_OpStatusType OpStatus,
    uint8* AttemptCounter);
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_SetSecurityAttemptCounterFncType)(
    Dcm_OpStatusType OpStatus,
    uint8 AttemptCounter);

/********************************Clear DTC****************************************/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ClearDTCCheckFncType)(
    uint32 GoDTC,
    Dcm_NegativeResponseCodeType* ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

typedef struct
{
    Dcm_ClearDTCCheckFncType DcmDsp_ClearDTCCheckFnc;
} Dcm_DspClearDTCType;

/********************************Com Control**************************************/
typedef struct
{
    boolean DcmDspComControlAllChannelUsed;
    NetworkHandleType DcmDspComMChannelId;
} Dcm_DspComControlAllChannelType;

typedef struct
{
    uint8 idle;
} Dcm_DspComControlSettingType;

typedef struct
{
    boolean DcmDspComControlSpecificChannelUsed;
    uint8 DcmDspSubnetNumber;
    NetworkHandleType DcmDspSpecificComMChannelId;
} Dcm_DspComControlSpecificChannelType;

typedef struct
{
    uint16 DcmDspComControlSubNodeId;
    boolean DcmDspComControlSubNodeUsed;
    NetworkHandleType DcmDspComMSubNodeChannelId;
} Dcm_DspComControlSubNodeType;

typedef struct
{
    uint8 DcmDspComControlAllChannelNum;
    P2CONST(Dcm_DspComControlAllChannelType, TYPEDEF, DCM_CONST) DcmDspComControlAllChannel;
    P2CONST(Dcm_DspComControlSettingType, TYPEDEF, DCM_CONST) DcmDspComControlSetting;
    uint8 DcmDspComControlSpecificChannelNum;
    P2CONST(Dcm_DspComControlSpecificChannelType, TYPEDEF, DCM_CONST)
    DcmDspComControlSpecificChannel; /* PRQA S 0779 */ /* MISRA Rule 5.2 */
    uint8 DcmDspComControlSubNodeNum;
    P2CONST(Dcm_DspComControlSubNodeType, TYPEDEF, DCM_CONST) DcmDspComControlSubNode;
} Dcm_DspComControlType;

typedef struct
{
    uint8 DcmDspCommonAuthorizationSecurityLevelRefNum;
    /* PRQA S 0779++ */ /* MISRA Rule 5.2 */
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDspCommonAuthorizationSecurityLevelRef;
    /* PRQA S 0779-- */ /* MISRA Rule 5.2 */
    uint8 DcmDspCommonAuthorizationSessionRefNum;
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDspCommonAuthorizationSessionRef; /* PRQA S 0779 */ /* MISRA Rule 5.2 */
} Dcm_DspCommonAuthorizationType;

/*************************Control DTC Setting******************************************/
typedef struct
{
    boolean DcmSupportDTCSettingControlOptionRecord;
} Dcm_DspControlDTCSettingType;

typedef struct
{
    uint16 DcmDspDataScalingInfoSize;
} Dcm_DspDataInfoType;

typedef enum
{
    USE_ASYNCH_CLIENT_SERVER,
    USE_ASYNCH_FNC
} Dcm_DspSecurityUsePortType;

/*Multiplicity=0..31*/
typedef struct
{
    uint8 DcmDspSecurityLevel;    /*Range:1~63*/
    uint8 DcmDspSecuritySeedSize; /*size of the security seed (in Bytes),Range:1~255*/
    uint8 DcmDspSecurityKeySize;  /*size of the security key (in Bytes),Range:1~255*/
    uint8 DcmDspSecurityADRSize;
    /*Size of the AccessDataRecord used in GetSeed,Range:1~255*/
    boolean DcmDspSecurityAttemptCounterEnabled;
    uint8 DcmDspSecurityNumAttDelay;
    /*Number of failed security accesses after which the delay time is activated,Range:1~255*/
    uint16 DcmDspSecurityDelayTime;
    /*Delay time after failed security access(unit:ms)*/
    uint16 DcmDspSecurityDelayTimeOnBoot; /*Start delay timer on power on(unit:ms)*/
    Dcm_GetSeedFncType Dcm_GetSeedFnc;
    Dcm_CompareKeyFncType Dcm_CompareKeyFnc;
    Dcm_GetSecurityAttemptCounterFncType Dcm_GetSecurityAttemptCounterFnc;
    Dcm_SetSecurityAttemptCounterFncType Dcm_SetSecurityAttemptCounterFnc;
    Dcm_DspSecurityUsePortType DcmDspSecurityUsePort;
} Dcm_DspSecurityRowType;

/*Multiplicity = 1*/
typedef struct
{
    P2CONST(Dcm_DspSecurityRowType, TYPEDEF, DCM_CONST) pDcm_DspSecurityRow;
    /*reference DspSecurityRow container*/
    uint8 DcmDspSecurityRow_Num; /*Number of DspSecurityRow*/
} Dcm_DspSecurityType;

typedef enum
{
    DCM_NO_BOOT,
    DCM_OEM_BOOT,
    DCM_OEM_BOOT_RESPAPP,
    DCM_SYS_BOOT,
    DCM_SYS_BOOT_RESPAPP
} Dcm_DspSessionForBootType;

/*DcmDspSession */
/*Multiplicity=0..31*/
typedef struct
{
    Dcm_DspSessionForBootType DcmDspSessionForBoot;
    uint8 DcmDspSessionLevel; /*hex value of the Session contro;Range:1~126*/
    uint16 DcmDspSessionP2ServerMax;
    /*This is the session value for P2ServerMax(in ms);Range 0 .. 1*/
    uint16 DcmDspSessionP2StarServerMax;
    /*This is the session value for P2*ServerMax(in ms);Range 0 .. 100*/
} Dcm_DspSessionRowType;

/*Multiplicity=1*/
typedef struct
{
    P2CONST(Dcm_DspSessionRowType, TYPEDEF, DCM_CONST) pDcmDspSessionRow;
    /*reference DspSessionRow container*/
    uint8 DcmDspSessionRow_Num; /*Number of DspSessionRow*/
} Dcm_DspSessionType;

/****************************DcmDspData*************************************/

typedef enum
{
    DCM_BOOLEAN = 0,
    DCM_SINT16 = 1,
    DCM_SINT16_N = 2,
    DCM_SINT32 = 3,
    DCM_SINT32_N = 4,
    DCM_SINT8 = 5,
    DCM_SINT8_N = 6,
    DCM_UINT16 = 7,
    DCM_UINT16_N = 8,
    DCM_UINT32 = 9,
    DCM_UINT32_N = 10,
    DCM_UINT8 = 11,
    DCM_UINT8_DYN = 12,
    DCM_UINT8_N = 13,
    DCM_VARIABLE_LENGTH = 14 /*for 31*/
} Dcm_DspDataEnumType;

typedef enum
{
    USE_BLOCK_ID = 0,
    USE_DATA_ASYNCH_CLIENT_SERVER = 1,
    USE_DATA_ASYNCH_CLIENT_SERVER_ERROR = 2,
    USE_DATA_ASYNCH_FNC = 3,
    USE_DATA_ASYNCH_FNC_ERROR = 4,
    USE_DATA_SENDER_RECEIVER = 5,
    USE_DATA_SENDER_RECEIVER_AS_SERVICE = 6,
    USE_DATA_SYNCH_CLIENT_SERVER = 7,
    USE_DATA_SYNCH_FNC = 8,
    USE_ECU_SIGNAL = 9
} Dcm_DspDataUsePortEnumType;

typedef enum
{
    DCM_CONTROLMASK_EXTERNAL = 0,
    DCM_CONTROLMASK_INTERNAL = 1,
    DCM_CONTROLMASK_NO = 2
} Dcm_DspDidControlMaskEnumType;

typedef struct
{
    uint8 DcmDspDidControlMaskBitPosition;
} Dcm_DspDidControlEnableMaskType;

typedef struct
{
    Dcm_DspDidControlMaskEnumType DcmDspDidControlMask;
    uint8 DcmDspDidControlMaskSize;
    uint8 DcmDspDidControlSecurityLevelRefNum;
    /*Number Of DcmDspSecurityRow Referenced*/
    P2CONST(uint8, TYPEDEF, DCM_CONST) pDcmDspDidControlSecurityLevelRow;
    /*DcmDspSecurityRow Referenced*/
    uint8 DcmDspDidControlSessionRefNum;
    /*Number Of DcmDspSessionRow Referenced*/
    P2CONST(uint8, TYPEDEF, DCM_CONST) pDcmDspDidControlSessionRow;
    uint8 DcmDspDidControlRoleRef_Num;
    /*DcmDspSessionRow Referenced*/
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDspDidControlRoleRef;
    boolean DcmDspDidFreezeCurrentState;
    /*Multiplicity=0..1; This indicates the presence of "FreezeCurrentState" and
    refers to a container defining the sizes of the parameters*/
    boolean DcmDspDidResetToDefault;
    /*Multiplicity=0..1; This indicates the presence of "ResetToDefault" and
    refers to a container defining the sizes of the parameters*/
    boolean DcmDspDidShortTermAdjustement;
    /*Multiplicity=0..1;This indicates the presence of "ShortTermAdjustment" and
     * refers to a container defining the sizes of the parameters*/
    P2CONST(Dcm_DspDidControlEnableMaskType, TYPEDEF, DCM_CONST) DcmDspDidControlEnableMask;
    /*Multiplicity=0..1; This indicates the presence of "ReturnControlToEcu" and
    refers to a container defining the sizes of the parameters*/
} Dcm_DspDidControlType;

typedef struct
{
    /*modeRule*/
    uint8 DcmDspDidReadSecurityLevelRefNum; /*Number Of DcmDspSecurityRow Referenced*/
    P2CONST(uint8, TYPEDEF, DCM_CONST)
    pDcmDspDidReadSecurityLevelRow;                              /*DcmDspSecurityRow Referenced*/
    uint8 DcmDspDidReadSessionRefNum;                            /*Number Of DcmDspSessionRow Referenced*/
    P2CONST(uint8, TYPEDEF, DCM_CONST) pDcmDspDidReadSessionRow; /*DcmDspSessionRow Referenced*/
    uint8 DcmDspDidReadRoleRef_Num;
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDspDidReadRoleRef;
} Dcm_DspDidReadType;

typedef struct
{
    /*modeRule*/
    uint8 DcmDspDidWriteSecurityLevelRefNum; /*Number Of DcmDspSecurityRow Referenced*/
    P2CONST(uint8, TYPEDEF, DCM_CONST)
    pDcmDspDidWriteSecurityLevelRow;                              /*DcmDspSecurityRow Referenced*/
    uint8 DcmDspDidWriteSessionRefNum;                            /*Number Of DcmDspSessionRow Referenced*/
    P2CONST(uint8, TYPEDEF, DCM_CONST) pDcmDspDidWriteSessionRow; /*DcmDspSessionRow Referenced*/
    uint8 DcmDspDidWriteRoleRef_Num;
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDspDidWriteRoleRef;
} Dcm_DspDidWriteType;

/*DcmDspDidInfo,Multiplicity=0..* */
typedef struct
{
    uint8 DcmDspDDDIDMaxElements;
    boolean DcmDspDidDynamicallyDefined;
    /*Multiplicity=1,true = DID can be dynamically
     * defined false = DID can not bedynamically defined */
    /*Multiplicity=1,true = datalength of the DID is fixed false =
     *  datalength of the DID is variable */
    /*Multiplicity=0..1,If Scaling information service is available for
     * this DID, it provides the size of the scaling information. */
    P2CONST(Dcm_DspDidControlType, TYPEDEF, DCM_CONST) pDcmDspDidControl;
    /*Multiplicity=0..1, the configuration (parameters)of the DID control*/
    P2CONST(Dcm_DspDidReadType, TYPEDEF, DCM_CONST) pDcmDspDidRead;
    /*Multiplicity=0..1, configuration (parameters)of the DID read*/
    P2CONST(Dcm_DspDidWriteType, TYPEDEF, DCM_CONST) pDcmDspDidWrite;
    /*Multiplicity=0..1, configuration (parameters)of the DID write*/
} Dcm_DspDidInfoType;

/*DcmDspDid,Multiplicity=0..* */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ConditionCheckReadFncType)(
    Dcm_OpStatusType OpStatus,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);
/* PRQA S 4604++ */ /* MISRA Rule 21.2 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_EcuSignalFncType)(uint8 action, uint8 signal);
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReadEcuSignalFncType)(P2VAR(uint8, AUTOMATIC, DCM_VAR) signal);
/* PRQA S 4604-- */ /* MISRA Rule 21.2 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_GetScalingInformationFncType)(
    P2VAR(uint8, AUTOMATIC, DCM_VAR) ScalingInfo,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_PidReadDataFncType)(P2VAR(uint8, AUTOMATIC, DCM_VAR) Data);
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReadDataFncType)(
    Dcm_OpStatusType OpStatus,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) Data,
    Dcm_NegativeResponseCodeType* ErrorCode);
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReadDataLengthFncType)(
    Dcm_OpStatusType OpStatus,
    P2VAR(uint16, AUTOMATIC, DCM_VAR) DidLength);

typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_FreezeCurrentStateFncType)(
    Dcm_OpStatusType OpStatus,
    P2CONST(uint8, AUTOMATIC, DCM_VAR) ControlEnableMaskRecord,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ResetToDefaultFncType)(
    Dcm_OpStatusType OpStatus,
    P2CONST(uint8, AUTOMATIC, DCM_VAR) ControlEnableMaskRecord,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReturnControlToECUFncType)(
    P2CONST(uint8, AUTOMATIC, DCM_VAR) ControlEnableMaskRecord,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentFncType)(
    P2CONST(uint8, AUTOMATIC, DCM_VAR) ControlStateInfo,
    uint16 DataLength,
    Dcm_OpStatusType OpStatus,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) ControlMask,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_WriteDataFncType)(
    P2CONST(uint8, AUTOMATIC, DCM_VAR) data,
    uint16 dataLength,
    Dcm_OpStatusType OpStatus,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/*****************************************DataServices_DIDRange************************/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReadDidRangeDataLengthFncType)(
    uint16 DID,
    Dcm_OpStatusType OpStatus,
    P2VAR(uint16, AUTOMATIC, DCM_VAR) DataLength);

typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_IsDidAvailableFncType)(
    uint16 DID,
    Dcm_OpStatusType OpStatus,
    P2VAR(Dcm_DidSupportedType, AUTOMATIC, DCM_VAR) supported);

typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReadDidDataFncType)(
    uint16 DID,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) Data,
    Dcm_OpStatusType OpStatus,
    uint16 DataLength,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_WriteDidDataFncType)(
    uint16 DID,
    P2CONST(uint8, AUTOMATIC, DCM_VAR) Data,
    Dcm_OpStatusType OpStatus,
    uint16 DataLength,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

typedef enum
{
    LINEAR = 0,
    SCALE_LINEAR_AND_TEXTTABLE = 1,
    TEXTTABLE = 2
} Dcm_DspDataTypeCategoryType;

typedef struct
{
    /*DcmDataElement;Foreign reference to [ VARIABLE-DATA-PROTOTYPE ]*/
    uint8 idle; /*DcmPortInterfaceMapping;Foreign reference to [ PORT-INTERFACE-MAPPING ]*/
} Dcm_DspAlternativeDataInterfaceType;

typedef struct
{
    uint32 DcmDspDiagnosisRepresentationDataLowerRange;
    uint32 DcmDspDiagnosisRepresentationDataOffset; /* PRQA S 0779 */     /* MISRA Rule 5.2 */
    uint32 DcmDspDiagnosisRepresentationDataResolution; /* PRQA S 0779 */ /* MISRA Rule 5.2 */
    uint32 DcmDspDiagnosisRepresentationDataUpperRange; /* PRQA S 0779 */ /* MISRA Rule 5.2 */
} Dcm_DspLinearScaleType;

typedef struct
{
    uint32 DcmDspDiagnosisRepresentationDataValue;
    uint32 DcmDspInternalDataValue;
} Dcm_DspTextTableMappingType;

typedef struct
{
    P2CONST(Dcm_DspDataTypeCategoryType, TYPEDEF, DCM_CONST) DcmDspDataTypeCategory;
    P2CONST(Dcm_DspLinearScaleType, TYPEDEF, DCM_CONST) DcmDspLinearScale;
    P2CONST(Dcm_DspTextTableMappingType, TYPEDEF, DCM_CONST) DcmDspTextTableMapping;
} Dcm_DspAlternativeDataPropsType;

typedef struct
{
    /*DcmApplicationDataType;Foreign reference to [ APPLICATION-PRIMITIVE-DATA-TYPE ]*/
    P2CONST(Dcm_DspTextTableMappingType, TYPEDEF, DCM_CONST) DcmDspTextTableMapping;
} Dcm_DspAlternativeDataTypeType;

typedef struct
{
    P2CONST(Dcm_DspAlternativeDataInterfaceType, TYPEDEF, DCM_CONST) DcmDspAlternativeDataInterface;
    P2CONST(Dcm_DspAlternativeDataPropsType, TYPEDEF, DCM_CONST) DcmDspAlternativeDataProps;
    P2CONST(Dcm_DspAlternativeDataTypeType, TYPEDEF, DCM_CONST) DcmDspAlternativeDataType;
} Dcm_DspDiagnosisScalingType;

typedef struct
{
    uint8 idle;
} Dcm_DspExternalSRDataElementClassType;

typedef struct
{
    Dcm_ConditionCheckReadFncType DcmDspDataConditionCheckReadFnc;
    /*Function name to demand application if the conditions
     * (e.g. System state) to read the DID are correct.Multiplicity=0..1*/
    boolean DcmConditionCheckReadFncUsed;
    Dcm_EcuSignalFncType DcmDspDataEcuSignalFnc;
    Dcm_ReadEcuSignalFncType DcmDspDataReadEcuSignalFnc;
    Dcm_DspEndianType DcmDspDataEndianness;
    Dcm_FreezeCurrentStateFncType DcmDspDataFreezeCurrentStateFnc;
    /*Function name to request to application to freeze the current state of
     *  an IOControl,Multiplicity=0..1*/
    Dcm_GetScalingInformationFncType DcmDspDataGetScalingInfoFnc;
    /*Function name to request to application the
     * scaling information of the DID,Multiplicity=0..1*/
    Dcm_ReadDataLengthFncType DcmDspDataReadDataLengthFnc;
    /*Function name to request from application the data length of a DID*/
    Dcm_ReadDataFncType DcmDspDataReadFnc;
    /*Function name to request from application the data value of a DID*/
    Dcm_ResetToDefaultFncType DcmDspDataResetToDefaultFnc;
    /*Function name to request to application to reset an IOControl to default value.*/
    Dcm_ReturnControlToECUFncType DcmDspDataReturnControlToECUFnc;
    /*Function name to request to application to return control to ECU of an IOControl*/
    Dcm_ShortTermAdjustmentFncType DcmDspDataShortTermAdjustmentFnc;
    /*Function name to request to application to adjuste the IO signal*/
    Dcm_WriteDataFncType DcmDspDataWriteFnc;
    /*Function name to request application to write the data value of a DID*/
    uint16 DcmDspDataSize;
    Dcm_DspDataEnumType DcmDspDataType;
    Dcm_DspDataUsePortEnumType DcmDspDataUsePort;
    uint16 DcmDspDataBlockId;
    uint8 DcmDspDataInfoIndex;
    /*if no DcmDspDataInfo Ref it is 0xFF*/
    /*DcmDspOdxDataDescription*/
    P2CONST(Dcm_DspDiagnosisScalingType, TYPEDEF, DCM_CONST) DcmDspDiagnosisScaling;
    P2CONST(Dcm_DspExternalSRDataElementClassType, TYPEDEF, DCM_CONST)
    DcmDspExternalSRDataElementClass;
} Dcm_DspDataType;

typedef struct
{
    uint16 DcmDspDidDataPos;
    P2CONST(Dcm_DspDataType, TYPEDEF, DCM_CONST) pDcmDspDidData;
} Dcm_DspDidSignalType;

typedef struct
{
    boolean DcmDspDidRangeHasGaps;
    uint16 DcmDspDidRangeIdentifierLowerLimit;
    uint16 DcmDspDidRangeIdentifierUpperLimit;
    Dcm_IsDidAvailableFncType DcmDspDidRangeIsDidAvailableFnc;
    Dcm_ReadDidRangeDataLengthFncType DcmDspDidRangeReadDataLengthFnc;
    Dcm_ReadDidDataFncType DcmDspDidRangeReadDidFnc;
    Dcm_WriteDidDataFncType DcmDspDidRangeWriteDidFnc;
    uint16 DcmDspDidRangeMaxDataLength;
    boolean DcmDspDidRangeUsePort;
    uint8 DcmDspDidRangeInfoIndex;
} Dcm_DspDidRangeType;

typedef struct DcmDspDid
{
    uint16 DcmDspDidId; /*2 byte Identifier of the DID,Multiplicity=1*/
    boolean DcmDspDidUsed;
    uint16 DcmDspDidInfoIndex;
    /*Reference to DcmDspDidInfo containing information on this DID*/
    uint8 DcmDspRefDidNum;
    /*Number Of Did referenced*/
    P2CONST(uint16, TYPEDEF, DCM_CONST) pDcmDspRefDidIdArray;
    /*Reference to DcmDspDid in case this DID refer to one or serveral other DID's*/
    /*Number Of DcmDspDidControlRecordSizes Container*/
    /*This container defines the sizes of the data sent
     * and received with the DID control functions*/
    uint16 DcmDspDidSignalNum;
    P2CONST(Dcm_DspDidSignalType, TYPEDEF, DCM_CONST) pDcmDspDidSignal;
} Dcm_DspDidType;

/*****************************************************************/
typedef enum
{
    DCM_SERVICE_01 = 0,
    DCM_SERVICE_01_02 = 1,
    DCM_SERVICE_02 = 2
} Dcm_DspPidServiceEnumType;

typedef struct
{
    uint8 DcmDspPidSupportInfoLen;
    uint8 DcmDspPidSupportInfoPos;
} Dcm_DspPidSupportInfoType;

typedef struct
{
    Dcm_DspEndianType DcmDspPidDataEndianness;
    Dcm_PidReadDataFncType DcmDspPidDataReadFnc;
    Dcm_DspDataEnumType DcmDspPidDataType;
    Dcm_DspDataUsePortEnumType DcmDspPidDataUsePort;
} Dcm_DspPidService01Type;

typedef struct
{
    uint8 idle;
} Dcm_DspPidService02Type;

typedef struct
{
    uint8 DcmDspPidDataSupportInfoBit;
    P2CONST(Dcm_DspPidSupportInfoType, TYPEDEF, DCM_CONST) DcmDspPidDataSupportInfoRef;
} Dcm_DspPidDataSupportInfoType;

typedef struct
{
    uint16 DcmDspPidDataPos; /*This is the position in bit of the PID structure
     and will not start at position 0 in case a support information
      is available (for packeted PIDs).*/
    uint16 DcmDspPidDataSize;
    P2CONST(Dcm_DspPidDataSupportInfoType, TYPEDEF, DCM_CONST) pDcmDspPidDataSupportInfo;
    P2CONST(Dcm_DspPidService01Type, TYPEDEF, DCM_CONST) DcmDspPidService01;
    P2CONST(Dcm_DspPidService02Type, TYPEDEF, DCM_CONST) DcmDspPidService02;
} Dcm_DspPidDataType;

/*DcmDspPid container,Multiplicity=0..* */
typedef struct
{
    uint8 DcmDspPidIdentifier; /*2 bytes Identifier of the PID*/
    Dcm_DspPidServiceEnumType DcmDspPidService;
    uint8 DcmDspPidSize; /*Length of data associated to the PID.*/
    boolean DcmDspPidUsed;
    uint8 DcmDspPidSupportInfoNum;
    P2CONST(Dcm_DspPidSupportInfoType, TYPEDEF, DCM_CONST) DcmDspPidSupportInfo;
    uint8 pDcmDspPidDataNum;
    P2CONST(Dcm_DspPidDataType, TYPEDEF, DCM_CONST) pDcmDspPidData;
} Dcm_DspPidType;

/*****************************************************************/
typedef struct
{
    uint8 DcmDspSupportedAddressAndLengthFormatIdentifier;
} Dcm_DspAddressAndLengthFormatIdentifierType;

typedef struct
{
    uint32 DcmDspReadMemoryRangeHigh;
    uint32 DcmDspReadMemoryRangeLow;
    /*modeRule*/
    uint8 DcmDspReadMemoryRangeSessionLevelRefNum; /*Number Of DcmDspSecurityRow referenced*/
    P2CONST(uint8, TYPEDEF, DCM_CONST) pDcmDspReadMemoryRangeSessionLevelRow;
    /*Reference to DcmDspSecurityRow */
    uint8 DcmDspReadMemorySecurityLevelRefNum; /*Number Of DcmDspSecurityRow referenced*/
    P2CONST(uint8, TYPEDEF, DCM_CONST) pDcmDspReadMemorySecurityLevelRow;
    /*Reference to DcmDspSecurityRow */
} Dcm_DspReadMemoryRangeInfoType;

typedef struct
{
    uint32 DcmDspWriteMemoryRangeHigh;
    uint32 DcmDspWriteMemoryRangeLow;
    /*modeRule*/
    uint8 DcmDspWriteMemoryRangeSessionLevelRefNum; /*Number Of DcmDspSecurityRow referenced*/
    P2CONST(uint8, TYPEDEF, DCM_CONST) pDcmDspWriteMemoryRangeSessionLevelRow;
    /*Reference to DcmDspSecurityRow */
    uint8 DcmDspWriteMemorySecurityLevelRefNum; /*Number Of DcmDspSecurityRow referenced*/
    P2CONST(uint8, TYPEDEF, DCM_CONST) pDcmDspWriteMemorySecurityLevelRow;
    /*Reference to DcmDspSecurityRow */
} Dcm_DspWriteMemoryRangeInfoType;

typedef struct
{
    uint8 DcmDspMemoryIdValue;
    uint8 DcmDspReadMemoryRangeInfoNum;
    P2CONST(Dcm_DspReadMemoryRangeInfoType, TYPEDEF, DCM_CONST) DcmDspReadMemoryRangeInfo;
    uint8 DcmDspWriteMemoryRangeInfoNum;
    P2CONST(Dcm_DspWriteMemoryRangeInfoType, TYPEDEF, DCM_CONST) DcmDspWriteMemoryRangeInfo;
} Dcm_DspMemoryIdInfoType;

typedef struct
{
    uint8 DcmDspAddressAndLengthFormatIdentifierNum;
    P2CONST(Dcm_DspAddressAndLengthFormatIdentifierType, TYPEDEF, DCM_CONST)
    DcmDspAddressAndLengthFormatIdentifier; /* PRQA S 0779 */ /* MISRA Rule 5.2 */
    uint8 DcmDspMemoryIdInfoNum;
    P2CONST(Dcm_DspMemoryIdInfoType, TYPEDEF, DCM_CONST) DcmDspMemoryIdInfo;
} Dcm_DspMemoryType;

/*****************************************************************/
/*DcmDspRequestControl container,Multiplicity=0..* */
typedef struct
{
    uint8 DcmDspRequestControlInBufferSize;
    uint8 DcmDspRequestControlOutBufferSize;
    uint8 DcmDspRequestControlTestId; /*Test Id for OBD Service $08*/
} Dcm_DspRequestControlType;

typedef struct
{
    uint8 DcmRequestFileTransferFileSizeParameterLength;
    uint8 DcmRequestFileTransferLengthFormatIdentifier;
} Dcm_DspRequestFileTransferType;

/*****************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_StartRoutineFncType)(
    P2CONST(uint8, AUTOMATIC, DCM_VAR) InBuffer,
    Dcm_OpStatusType OpStatus,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) OutBuffer,
    P2VAR(uint16, AUTOMATIC, DCM_VAR) currentDataLength,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_StopRoutineFncType)(
    P2CONST(uint8, AUTOMATIC, DCM_VAR) InBuffer,
    Dcm_OpStatusType OpStatus,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) OutBuffer,
    P2VAR(uint16, AUTOMATIC, DCM_VAR) currentDataLength,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RequestResultsRoutineFncType)(
    P2CONST(uint8, AUTOMATIC, DCM_VAR) InBuffer,
    Dcm_OpStatusType OpStatus,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) OutBuffer,
    P2VAR(uint16, AUTOMATIC, DCM_VAR) currentDataLength,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

typedef struct
{
    uint8 idle; /*Foreign reference to [ ARGUMENT-DATA-PROTOTYPE ]*/
} Dcm_DspAlternativeArgumentDataType;

typedef struct
{
    P2CONST(Dcm_DspAlternativeArgumentDataType, TYPEDEF, DCM_CONST) DcmDspAlternativeArgumentData;
    P2CONST(Dcm_DspAlternativeDataPropsType, TYPEDEF, DCM_CONST) DcmDspAlternativeDataProps;
    P2CONST(Dcm_DspAlternativeDataTypeType, TYPEDEF, DCM_CONST) DcmDspAlternativeDataType;
} Dcm_DspArgumentScalingType;

typedef struct
{
    Dcm_DspEndianType DcmDspRoutineSignalEndianness;
    uint16 DcmDspRoutineSignalLength;
    uint16 DcmDspRoutineSignalPos;
    Dcm_DspDataEnumType DcmDspRoutineSignalType;
    P2CONST(Dcm_DspArgumentScalingType, TYPEDEF, DCM_CONST) DcmDspArgumentScaling;
} Dcm_DspRoutineInOutSignalType;
typedef struct
{
    uint8 RoutineInOutSignalNum;
    P2CONST(Dcm_DspRoutineInOutSignalType, TYPEDEF, DCM_CONST) DcmDspRoutineInOutSignal;
} Dcm_DspRoutineInOutType;
typedef struct
{
    Dcm_RequestResultsRoutineFncType DcmDspRequestResultsRoutineFnc;
    /*Function name for request to application the results of a routine*/
    P2CONST(Dcm_DspCommonAuthorizationType, TYPEDEF, DCM_CONST)
    DcmDspRequestRoutineResultsCommonAuthorizationRef;
    uint8 DcmDspRequestRoutineResultsRoleRef_Num;
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDspRequestRoutineResultsRoleRef; /* PRQA S 0779 */ /* MISRA Rule 5.2 */
    P2CONST(Dcm_DspRoutineInOutType, TYPEDEF, DCM_CONST) DcmDspRequestRoutineResultsIn;
    P2CONST(Dcm_DspRoutineInOutType, TYPEDEF, DCM_CONST) DcmDspRequestRoutineResultsOut;
} Dcm_DspRequestRoutineResultsType;

/*DcmDspRoutine,Multiplicity=0..* */
typedef struct
{
    Dcm_StartRoutineFncType DcmDspStartRoutineFnc;
    /*Function name for request to application to start a routine.*/
    P2CONST(Dcm_DspCommonAuthorizationType, TYPEDEF, DCM_CONST)
    DcmDspStartRoutineCommonAuthorizationRef;
    uint8 DcmDspStartRoutineRoleRef_Num;
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDspStartRoutineRoleRef;
    P2CONST(Dcm_DspRoutineInOutType, TYPEDEF, DCM_CONST) DcmDspStartRoutineIn;
    P2CONST(Dcm_DspRoutineInOutType, TYPEDEF, DCM_CONST) DcmDspStartRoutineOut;
} Dcm_DspStartRoutineType;

typedef struct
{
    Dcm_StopRoutineFncType DcmDspRoutineStopFnc;
    /*Function name for request to application to stop a routine.*/
    P2CONST(Dcm_DspCommonAuthorizationType, TYPEDEF, DCM_CONST)
    DcmDspStopRoutineCommonAuthorizationRef;
    uint8 DcmDspStopRoutineRoleRef_Num;
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDspStopRoutineRoleRef;
    P2CONST(Dcm_DspRoutineInOutType, TYPEDEF, DCM_CONST) DcmDspStopRoutineIn;
    P2CONST(Dcm_DspRoutineInOutType, TYPEDEF, DCM_CONST) DcmDspStopRoutineOut;
} Dcm_DspStopRoutineType;

typedef struct
{
    uint16 DcmDspRoutineId; /*2 bytes Identifier of the RID*/
    uint8 DcmDspRoutineInfoByte;
    boolean DcmDspRoutineUsePort;
    boolean DcmDspRoutineUsed;
    P2CONST(Dcm_DspCommonAuthorizationType, TYPEDEF, DCM_CONST) DcmDspCommonAuthorizationRef;
    P2CONST(Dcm_DspRequestRoutineResultsType, TYPEDEF, DCM_CONST) DcmDspRequestRoutineResults;
    P2CONST(Dcm_DspStartRoutineType, TYPEDEF, DCM_CONST) DcmDspStartRoutine;
    P2CONST(Dcm_DspStopRoutineType, TYPEDEF, DCM_CONST) DcmDspStopRoutine;
} Dcm_DspRoutineType;

/*********************************************************************/
/*DcmDspVehInfo Container,Multiplicity=0..* */
typedef Std_ReturnType (*Dcm_GetInfoTypeValueFncType)(Dcm_OpStatusType OpStatus, uint8* DataValueBuffer);
typedef struct
{
    uint8 DcmDspVehInfoDataOrder;
    uint8 DcmDspVehInfoDataSize; /*Length of data of associated INFOTYPE.*/
    boolean DcmDspVehInfoDataUsePort;
    Dcm_GetInfoTypeValueFncType DcmGetInfoTypeValueFnc;
    /*Function name for reading the associated INFOTYPE.*/
} Dcm_DspVehInfoDataType;

typedef struct
{
    uint8 DcmDspVehInfoInfoType; /*INFOTYPE for Service $09 */
    boolean DcmDspVehInfoNODIProvResp;
    /* In case the responsibility is on provider side
    (DcmDspVehInfoNODIProvResp is set to TRUE), only one DcmDspVehInfoData
    container shall be allowed*/
    /* In case DcmDspVehInfoDataUsePort is set to FALSE and
    DcmDspVehInfoDataReadFnc is set to either Dem_DcmInfoTypeValue08 or
    Dem_DcmInfoTypeValue0B then DcmDspVehInfoNODIProvResp shall be set to
    TRUE.*/
    uint8 DcmDspVehInfoDataNum;
    P2CONST(Dcm_DspVehInfoDataType, TYPEDEF, DCM_CONST) DcmDspVehInfoData;
} Dcm_DspVehInfoType;

typedef enum
{
    AFTER_RESET = 0,
    BEFORE_RESET = 1
} Dcm_RespToEcuResetType;

typedef struct
{
    uint32 DcmDspPeriodicTransmissionFastRate;
    uint32 DcmDspPeriodicTransmissionMediumRate;
    uint32 DcmDspPeriodicTransmissionSlowRate;
} Dcm_DspPeriodicTransmissionTypes;

typedef enum
{
    DCM_ROE_CLEARED = 0,
    DCM_ROE_STOPPED = 1,
    DCM_ROE_STARTED = 2
} Dcm_RoeEventStatesTypes;

typedef struct
{
    uint16 DcmDspRoeDidRef; /*Did value*/
} Dcm_DspRoeOnChangeOfDataIdentifierType;

typedef struct
{
    uint8 DcmDspRoeDTCStatusMask;
} Dcm_DspRoeOnDTCStatusChangeType;

typedef struct
{
    P2CONST(Dcm_DspRoeOnChangeOfDataIdentifierType, TYPEDEF, DCM_CONST)
    DcmDspRoeOnChangeOfDataIdentifier;
    P2CONST(Dcm_DspRoeOnDTCStatusChangeType, TYPEDEF, DCM_CONST) DcmDspRoeOnDTCStatusChange;
} Dcm_DspRoeEventPropertiesType;

typedef struct
{
    uint8 DcmDspRoeEventId;
    Dcm_RoeEventStatesTypes DcmDspRoeInitialEventStatus;
    P2CONST(Dcm_DspRoeEventPropertiesType, TYPEDEF, DCM_CONST) DcmDspRoeEventProperties;
} Dcm_DspRoeEventType;

typedef enum
{
    DCM_ROE_EVENT_WINDOW_INFINITE = 2,
    DCM_ROE_EVENT_WINDOW_CURRENT_CYCLE = 3,
    DCM_ROE_EVENT_WINDOW_CURRENT_AND_FOLLOWING_CYCLE = 4,
} Dcm_DspRoeEventWindowTimeType;

typedef struct
{
    Dcm_DspRoeEventWindowTimeType DcmDspRoeEventWindowTime;
} Dcm_DspRoeEventWindowTimeTypes;

typedef struct
{
    uint16 DcmDspRoeInterMessageTime;
    uint8 DcmDspRoeEventNum;
    P2CONST(Dcm_DspRoeEventType, TYPEDEF, DCM_CONST) DcmDspRoeEvent;
    uint8 DcmDspRoeEventWindowTimeNum;
    P2CONST(Dcm_DspRoeEventWindowTimeTypes, TYPEDEF, DCM_CONST) DcmDspRoeEventWindowTime;
} Dcm_DspRoeTypes;

typedef struct
{
    uint16 DcmDspAuthenticationEcuChallengeLength;
    uint8 DcmDspAuthenticationConnectionMainConnectionRef;
    uint32 DcmDspAuthenticationCertificatePublicKeyStoreJobRef; /*ref CsmJob*/
    uint32 DcmDspAuthenticationClientChallengeSignJobRef;       /*ref CsmJob*/
    uint16 DcmDspAuthenticationConnectionCertificateRef;        /*ref KeyMCertificate*/
    P2CONST(uint16, TYPEDEF, DCM_CONST)
    DcmDspAuthenticationECUCertificateRef;                         /*ref KeyMCertificateElement */
    uint16 DcmDspAuthenticationPublicKeyElementRef;                /*ref KeyMCertificateElement*/
    uint32 DcmDspAuthenticationRandomJobRef;                       /*ref CsmJob*/
    uint16 DcmDspAuthenticationRoleElementRef;                     /*ref KeyMCertificateElement*/
    uint32 DcmDspAuthenticationVerifyProofOfOwnerShipClientJobRef; /*ref CsmJob*/
    P2CONST(uint16, TYPEDEF, DCM_CONST)
    DcmDspAuthenticationWhiteListDIDElementRef; /*ref KeyMCertificateElement*/
    P2CONST(uint16, TYPEDEF, DCM_CONST)
    DcmDspAuthenticationWhiteListRIDElementRef;             /*ref KeyMCertificateElement*/
    uint16 DcmDspAuthenticationWhiteListServicesElementRef; /*ref KeyMCertificateElement*/
} Dcm_DspAuthenticationConnectionTypes;

typedef struct
{
    P2CONST(uint32, TYPEDEF, DCM_CONST) DcmDspAuthenticationDefaultSessionTimeOut;
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDspAuthenticationGeneralNRC;
    uint8 DcmDspAuthenticationRoleSize;
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDspAuthenticationWhiteListDIDMaxSize;
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDspAuthenticationWhiteListRIDMaxSize;
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDspAuthenticationWhiteListServicesMaxSize;
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDspAuthenticationDeauthenticatedRoleRef;
    uint8 DcmDspAuthenticationDeauthenticatedRoleRefNum; /* PRQA S 0779 */ /* MISRA Rule 5.2 */
    P2CONST(uint16, TYPEDEF, DCM_CONST) DcmDspAuthenticationPersistRef;
    uint8 DcmDspAuthenticationConnectionNum;
    P2CONST(Dcm_DspAuthenticationConnectionTypes, TYPEDEF, DCM_CONST)
    DcmDspAuthenticationConnection;
} Dcm_DspAuthenticationTypes;

typedef struct
{
    P2CONST(boolean, TYPEDEF, DCM_CONST) DcmDspDDDIDcheckPerSourceDID;
    Dcm_DspEndianType DcmDspDataDefaultEndianness;
    boolean DcmDspEnableObdMirror;
    uint16 DcmDspMaxDidToRead;
    /*Indicates the maximum allowed DIDs in a single
     * "ReadDataByIdentifier" request. If set to 0, then no limitation is applied. */
    uint16 DcmDspMaxPeriodicDidToRead;
    uint8 DcmDspPowerDownTime;
    Dcm_RespToEcuResetType DcmResponseToEcuReset;

    P2CONST(Dcm_DspClearDTCType, TYPEDEF, DCM_CONST) pDcmDspClearDTC;
    P2CONST(Dcm_DspComControlType, TYPEDEF, DCM_CONST) pDcmDspComControl;
    P2CONST(Dcm_DspCommonAuthorizationType, TYPEDEF, DCM_CONST) pDcmDspCommonAuthorization;
    P2CONST(Dcm_DspControlDTCSettingType, TYPEDEF, DCM_CONST) pDcmDspControlDTCSetting;

#if (STD_ON == DCM_DSP_DATA_FUNC_ENABLED)
    P2CONST(Dcm_DspDataType, TYPEDEF, DCM_CONST) pDcmDspData;
    P2CONST(Dcm_DspDataInfoType, TYPEDEF, DCM_CONST) pDcmDspDataInfo;
#endif

/*DcmDspDid and DcmDspDidInfo configeration(Multiplicity=0..*)*/
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
    uint16 DcmDspDidNum; /*Number Of Did*/
    P2CONST(Dcm_DspDidType, TYPEDEF, DCM_CONST) pDcmDspDid;
    uint8 DcmDspDidInfoNum; /*Number Of DidInfo*/
    P2CONST(Dcm_DspDidInfoType, TYPEDEF, DCM_CONST) pDcmDspDidInfo;
    uint8 DcmDspDidRangeNum;
    P2CONST(Dcm_DspDidRangeType, TYPEDEF, DCM_CONST) pDcmDspDidRange;
#endif

    P2CONST(Dcm_DspMemoryType, TYPEDEF, DCM_CONST) pDcmDspMemory;

/*DcmDspPid configeration(Multiplicity=0..*)*/
#if (STD_ON == DCM_DSP_PID_FUNC_ENABLED)
    uint8 DcmDspPidNum;
    P2CONST(Dcm_DspPidType, TYPEDEF, DCM_CONST) pDcmDspPid;
#endif

/*DcmDspRequestControl configuration(Multiplicity=0..*)*/
#if (STD_ON == DCM_DSP_REQUESTCONTROL_FUNC_ENABLED)
    uint8 DcmDspRequestControlNum;
    P2CONST(Dcm_DspRequestControlType, TYPEDEF, DCM_CONST) pDcmDspRequestControl;
#endif
    P2CONST(Dcm_DspRequestFileTransferType, TYPEDEF, DCM_CONST) DcmDspRequestFileTransfer;

/*DcmDspRoutine and DcmDspRoutineInfo configeration(Multiplicity=0..*)*/
#if (STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED)
    uint8 DcmDspRoutineNum;
    P2CONST(Dcm_DspRoutineType, TYPEDEF, DCM_CONST) pDcmDspRoutine;
#endif
    /*DcmDspSecurity configeration(Multiplicity=1)*/
    P2CONST(Dcm_DspSecurityType, TYPEDEF, DCM_CONST) pDcm_DspSecurity;
    /*DcmDspSession configeration(Multiplicity=1)*/
    P2CONST(Dcm_DspSessionType, TYPEDEF, DCM_CONST) pDcm_DspSession;

/*DcmDspVehInfo configeration*/
#if (STD_ON == DCM_DSP_VEHINFO_FUNC_ENABLED)
    uint8 DcmDspVehInfoNum;
    P2CONST(Dcm_DspVehInfoType, TYPEDEF, DCM_CONST) pDcmDspVehInfo;
#endif
    uint16 DcmDspMaxPeriodicDidScheduler;
    P2CONST(Dcm_DspPeriodicTransmissionTypes, TYPEDEF, DCM_CONST) Dcm_DspPeriodicTransmission;
#if (DCM_DSP_ROE_NUM > 0)
    P2CONST(Dcm_DspRoeTypes, TYPEDEF, DCM_CONST) Dcm_DspRoe;
#endif
    P2CONST(Dcm_DspAuthenticationTypes, TYPEDEF, DCM_CONST) Dcm_DspAuthentication;
} Dcm_DspCfgType;

/**************************************************************************
 ************************DcmDsd container********************
 ************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_DiagnosticService)(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_DiagnosticService_SubService)(
    Dcm_OpStatusType OpStatus,
    P2CONST(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

typedef struct
{
    P2CONST(Dcm_DiagnosticService_SubService, TYPEDEF, DCM_CONST) DcmDsdSubServiceFnc;
    uint8 DcmDsdSubServiceId;
    boolean DcmDsdSubServiceUsed;
    P2CONST(Dcm_ModeRuleCfgType, TYPEDEF, DCM_CONST) DcmDsdSubServiceModeRuleRef;
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDsdSubServiceSecurityLevelRef;
    /*Reference to DcmDspSecurityRow*/
    uint8 DcmDsdSubServiceSecurityLevel_Num; /*Number Of Security Access Levels*/
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDsdSubServiceSessionLevelRef;
    uint8 DcmDsdSubServiceSessionLevel_Num; /*Number Of  the Session Control*/
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDsdSubServiceRoleRef;
    uint8 DcmDsdSubServiceRoleRef_Num;
} Dcm_DsdSubServiceCfgType;

typedef enum
{
    DCM_PHYSICAL = 0,
    DCM_FUNCTIONAL = 1,
    DCM_PHYANDFUNC = 2
} Dcm_DslProtocolRxAddrType;

/*DcmDsdService container,Multiplicity=1..*,*/
typedef struct
{
    boolean DcmDsdServiceUsed;
    Dcm_DiagnosticService DcmDsdSidTabFnc;
    uint8 DcmDsdServiceId; /*Id of the Service identifier in hex*/
    boolean DcmDsdSubfuncAvial;
    boolean DcmDsdSuppressPosRsp;
    /*Information whether the DcmDsdSidTabServiceId includes Subfunctions or not*/
    Dcm_DslProtocolRxAddrType AddressingFormat;
    P2CONST(Dcm_ModeRuleCfgType, TYPEDEF, DCM_CONST) DcmDsdModeRuleRef;
    uint8 DcmDsdSecurityLevel_Num;                              /*Number Of Security Access Levels*/
    P2CONST(uint8, TYPEDEF, DCM_CONST) pDcmDsdSecurityLevelRef; /*Reference to DcmDspSecurityRow*/
    uint8 DcmDsdSessionLevel_Num;                               /*Number Of  the Session Control*/
    P2CONST(uint8, TYPEDEF, DCM_CONST) pDcmDsdSessionLevelRef;
    uint8 DcmDsdSubService_Num;
    P2CONST(Dcm_DsdSubServiceCfgType, TYPEDEF, DCM_CONST) DcmDsdSubService;
    P2CONST(uint8, TYPEDEF, DCM_CONST) DcmDsdServiceRoleRef;
    uint8 DcmDsdServiceRoleRef_Num;
} Dcm_DsdServiceCfgType;

/*DcmDsdServiceTable container,Multiplicity=1..256*/
typedef struct
{
    uint8 DcmDsdSidTabId; /*service table id*/
    P2CONST(Dcm_DsdServiceCfgType, TYPEDEF, DCM_CONST)
    pDcmDsdService;                /* reference to DcmDsdService*/
    uint8 DcmDsdSidTab_ServiceNum; /*Number of the service*/
} Dcm_DsdServiceTableCfgType;

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
#if (DCM_DSD_REQUEST_SUPPLIER_NOTIFICATION_ENABLED == STD_ON)
typedef struct
{
    P2FUNC(Std_ReturnType, DCM_APPL_CODE, Indication)
    (uint8 SID,
     P2CONST(uint8, AUTOMATIC, DCM_VAR) RequestData,
     uint16 DataSize,
     uint8 ReqType,
     uint16 SourceAddress,
     P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
    P2FUNC(Std_ReturnType, DCM_APPL_CODE, Confirmation)
    (uint8 SID, uint8 ReqType, uint16 SourceAddress, Dcm_ConfirmationStatusType ConfirmationStatus);
} Dcm_DsdServiceReqSuppNotiType;
#endif

#if (DCM_DSD_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED == STD_ON)
typedef struct
{
    P2FUNC(Std_ReturnType, DCM_APPL_CODE, Indication)
    (uint8 SID,
     P2CONST(uint8, AUTOMATIC, DCM_VAR) RequestData,
     uint16 DataSize,
     uint8 ReqType,
     uint16 SourceAddress,
     P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
    P2FUNC(Std_ReturnType, DCM_APPL_CODE, Confirmation)
    (uint8 SID, uint8 ReqType, uint16 SourceAddress, Dcm_ConfirmationStatusType ConfirmationStatus);
} Dcm_DsdServiceReqManuNotiType;
#endif
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
/*DcmDsd container,Multiplicity=1*/
typedef struct
{
#if (DCM_DSD_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED == STD_ON)
    P2CONST(Dcm_DsdServiceReqManuNotiType, TYPEDEF, DCM_CONST) pDcmDsdServiceReqManufacturerNoti;
    uint16 DcmDsdServiceReqManufacturerNoti_PortNum;
#endif
#if (DCM_DSD_REQUEST_SUPPLIER_NOTIFICATION_ENABLED == STD_ON)
    P2CONST(Dcm_DsdServiceReqSuppNotiType, TYPEDEF, DCM_CONST) pDcmDsdServiceReqSupplierNoti;
    uint16 DcmDsdServiceReqSupplierNoti_PortNum;
#endif
    P2CONST(Dcm_DsdServiceTableCfgType, TYPEDEF, DCM_CONST) pDcmDsdServiceTable;
    /*pointer to Service table array*/
    uint16 DcmDsdServiceTable_Num; /*Number of the service table*/
} Dcm_DsdCfgType;

/**************************************************************************
 ************************DcmDsl container********************
 ************************************************************************/
/*DcmDslBuffer container,Multiplicity=1..256*/
typedef struct
{
    uint8 Dcm_DslBufferId;    /*buffer Id */
    uint32 Dcm_DslBufferSize; /*length of the buffer,Range=8~4095*/
    uint32 offset;
} Dcm_DslBufferType;

/*********************************/
/*DslCallbackDCMRequestService container,Multiplicity=1..*,*/
typedef struct
{
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2FUNC(Std_ReturnType, DCM_APPL_CODE, StartProtocol)(Dcm_ProtocolType ProtocolID);
    P2FUNC(Std_ReturnType, DCM_APPL_CODE, StopProtocol)(Dcm_ProtocolType ProtocolID);
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
} Dcm_DslCallbackDCMRequestServiceType;

/*********************************/
typedef struct
{
    boolean DcmDslDiagRespOnSecondDeclinedRequest;
    /*enable (TRUE) or disable (FALSE) the mechanism of directly
     * triggering of ResponsePending by application,Multiplicity=1*/
    uint8 DcmDslDiagRespMaxNumRespPend; /*Maximum number of negative
   responses with response code 0x78 per request,Multiplicity=1*/
    /*DcmDslDiagRespMaxNumRespPend = 0 means 0x78 is no limit*/
} Dcm_DslDiagRespType;

/*********************************/
typedef struct
{
    PduIdType DcmDslPeriodicTxConfirmationPduId;
    PduIdType DcmDslTxPduRPduId;
#if (STD_ON == DCM_GENERIC_CONNECTION)
    uint8 DcmDslMetaDataFlag; /*Reference to a Pdu in EcuC,bit0-3:Metadata
     length,bit4:CANTP_ADDRESS_EXTENSION_8_MASK,bit5:CANTP_SA16_AND_TA16_MASK*/
#endif
    /*Reference to a Pdu in EcuC*/
} Dcm_DslPeriodicConnectionType;

/*DcmDslPeriodicTransmission container,Multiplicity=0..1*/
typedef struct
{
    P2CONST(Dcm_DslPeriodicConnectionType, TYPEDEF, DCM_CONST) DcmDslPeriodicConnection;
    uint8 DcmPeriodicTxPduIdNum;
} Dcm_DslPeriodicTransmissionType;

/*DcmDslResponseOnEvent container,Multiplicity=0..1*/
typedef struct
{
    PduIdType DcmROETxPduId;
    uint8 DcmROETxPduIdNum;
    /*Reference to a Pdu in EcuC*/
} Dcm_DslResponseOnEventType;

/*Dcm_DslProtocolRx container,Multiplicity=1..*,*/
typedef struct
{
    uint8 DcmDslParentConnectionCtrlId; /*which connection it belongs*/
    Dcm_DslProtocolRxAddrType DcmDslProtocolRxAddrType;
    PduIdType DcmDslProtocolRxPduId;
#if (STD_ON == DCM_GENERIC_CONNECTION)
    uint8 DcmDslMetaDataFlag; /*Reference to a Pdu in EcuC,bit0-3:Metadata
     length,bit4:CANTP_ADDRESS_EXTENSION_8_MASK,bit5:CANTP_SA16_AND_TA16_MASK*/
#endif
} Dcm_DslProtocolRxType;

typedef struct
{
    uint8 DcmDslParentConnectionCtrlId; /*which connection it belongs*/
    PduIdType DcmDslTxConfirmationPduId;
    PduIdType DcmDslTxPduRPduId;
} Dcm_DslProtocolTxType;

/*DcmDslMainConnection container,Multiplicity=1*/
typedef struct
{
    uint16 DcmDslProtocolRxTesterSourceAddr;
    P2CONST(Dcm_DslPeriodicTransmissionType, TYPEDEF, DCM_CONST) pDcmDslPeriodicTranmissionConRef;
    NetworkHandleType DcmDslProtocolComMChannelId; /*Reference to the ComMChannel */
    P2CONST(Dcm_DslProtocolTxType, TYPEDEF, DCM_CONST) pDcmDslROEConnectionRef;
    /*Reference to DcmDslResponseOnEvent,Multiplicity=0..1,*/
    P2CONST(Dcm_DslProtocolRxType, TYPEDEF, DCM_CONST) pDcmDslProtocolRx;
    /*Reference to DcmDslProtocolRx,Multiplicity=1..*,*/
    uint8 DcmDslProtocolRx_Num;
    /*Number Of DcmDslProtocolRx*/
    P2CONST(Dcm_DslProtocolTxType, TYPEDEF, DCM_CONST) pDcmDslProtocolTx;
    /*Reference to DcmDslProtocolTx,Multiplicity=0..1,*/
    uint8 DcmDslProtocolTx_Num;
    /*Number Of DcmDslProtocolTx*/
} Dcm_DslMainConnectionType;
/*Dcm_DslConnection,Multiplicity=1..*,*/
typedef struct
{
    uint8 DcmDslParentProtocolRowCtrlId; /*which protocol it belongs*/
    P2CONST(Dcm_DslMainConnectionType, TYPEDEF, DCM_CONST) pDcmDslMainConnection;
    /*Reference to DcmDslMainConnection,Multiplicity=1,*/
    P2CONST(Dcm_DslPeriodicTransmissionType, TYPEDEF, DCM_CONST) pDcmDslPeriodicTransmission;
    /*Reference to DcmDslPeriodicTransmission,Multiplicity=0..1,*/
    P2CONST(Dcm_DslResponseOnEventType, TYPEDEF, DCM_CONST) pDcmDslResponseOnEvent;
    /*Reference to DcmDslResponseOnEvent,Multiplicity=0..1*/
} Dcm_DslConnectionType;

typedef enum
{
    DCM_PROTOCOL_TRAN_TYPE1 = 0x00,
    /*Messages on the DcmTxPduId already used for normal diagnostic responses*/
    DCM_PROTOCOL_TRAN_TYPE2 = 0x01, /*Messages on a separate DcmTxPduId.*/
    DCM_PROTOCAL_TRAN_NOT_VALID = 0x02
} Dcm_DslProtocolTransType;

/*Dcm_DslProtocolRowCfg container,Multiplicity=1..*,*/
typedef struct
{
    Dcm_ProtocolType DcmDslProtocolID; /*Protocol Id,Multiplicity=1*/
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
    uint16 DcmDslProtocolMaximumResponseSize;
/*This parameter is mandatory and defines the maximum length of the response message in case
 * DcmPagedBufferEnabled == TRUE*/
#endif
    uint8 DcmDslProtocolPriority;
    /*Protocol Priority,Multiplicity=1*/
    boolean DcmDslProtocolRowUsed;
    /*Allows to activate or deactivate the usage of a Protocol.
     * This parameter can be used for multi-purpose ECUs*/
    Dcm_DslProtocolTransType DcmDslProtocolTransType;
    /*the transmission type of ROE or Periodic,Multiplicity=1,*/
    boolean DcmSendRespPendOnTransToBoot;
    /*Parameter specifying if the ECU should send a NRC 0x78 (response pending)
     *  before transitioning to the bootloader (parameter set to TRUE) or if the
     *   transition shall be initiated without sending NRC 0x78 (parameter set to FALSE).*/
    uint16 DcmTimStrP2ServerAdjust;
    uint16 DcmTimStrP2StarServerAdjust;
    P2CONST(Dcm_DslBufferType, TYPEDEF, DCM_CONST) DcmDslProtocolRxBufferRef;
    P2CONST(Dcm_DslBufferType, TYPEDEF, DCM_CONST) DcmDslProtocolTxBufferRef;
    uint8 DcmDslServiceTableID;
    /*Reference to DcmDsdServiceTable,Multiplicity=1,*/
    P2CONST(Dcm_DslConnectionType, TYPEDEF, DCM_CONST) pDcmDslConnection;
    boolean DcmDslProtocolRequestQueued;
} Dcm_DslProtocolRowType;

typedef struct
{
    P2CONST(Dcm_DslProtocolRowType, TYPEDEF, DCM_CONST) pDcmDslProtocolRow;
    uint8 DcmDslProtocolRow_Num; /*Number of Protocol*/
} Dcm_DslProtocolType;

/*****************************/
/*DcmDsl container,Multiplicity=1*/
typedef struct
{
    uint8 DcmChannelCfg_Num;                                       /*Number of DcmDslBuffer configration*/
    P2CONST(Dcm_DslBufferType, TYPEDEF, DCM_CONST) pDcmChannelCfg; /*DcmDslBuffer*/
    uint8 DCMCallBackDcmRequest_PortNum;                           /*Number of DslCallbackDCMRequestService port*/
    P2CONST(Dcm_DslCallbackDCMRequestServiceType, TYPEDEF, DCM_CONST)
    pDcmDslCallback_DCMRequestService;
    P2CONST(Dcm_DslDiagRespType, TYPEDEF, DCM_CONST) pDcmDslDiagRespCfg;
    /*reference to DcmDslDiagResp configration*/
    P2CONST(Dcm_DslProtocolType, TYPEDEF, DCM_CONST) pDcmDslProtocol;
    /*reference to DcmDslProtocol configration*/
} Dcm_DslCfgType;

typedef struct
{
    uint32 DcmPagedBufferTimeout;
    /*the Timeout (in ms) towards the application for filling the next page*/
} Dcm_PageBufferCfgType;

typedef struct
{
    P2CONST(Dcm_DslCfgType, TYPEDEF, DCM_CONST) pDcmDslCfg;
    P2CONST(Dcm_DsdCfgType, TYPEDEF, DCM_CONST) pDcmDsdCfg;
    P2CONST(Dcm_DspCfgType, TYPEDEF, DCM_CONST) pDcmDspCfg;
    P2CONST(Dcm_DslProtocolRxType, TYPEDEF, DCM_CONST) DslProtocolConnectionRxCfg;
    P2CONST(Dcm_DslConnectionType, TYPEDEF, DCM_CONST) DslProtocolConnectionCfg;
    P2CONST(Dcm_DslMainConnectionType, TYPEDEF, DCM_CONST) DslProtocolMainConnectionCfg;
    P2CONST(Dcm_DslProtocolTxType, TYPEDEF, DCM_CONST) DslProtocolConnectionTxCfg;
    P2CONST(Dcm_PageBufferCfgType, TYPEDEF, DCM_CONST) pDcmPageBufferCfg;
    P2CONST(Dcm_ProcessingConditionsCfgType, TYPEDEF, DCM_CONST) pDcmProcessingConditionsCfg;
} Dcm_CfgType;

typedef struct
{
    /*enable or disable the output Version info*/
    uint32 DcmTaskTime;
/*the time (in ms)for the periodic cyclic task*/
#if (STD_ON == DCM_DSP_DATA_FUNC_ENABLED)
    P2CONST(Dcm_DspDidType, TYPEDEF, DCM_CONST) DcmVinRef;
/*Reference to the Did containing the VIN Information.This parameter
 *  is needed for function Dcm_GetVin*/
#endif
} Dcm_GeneralCfgType;

#if (STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED)
typedef enum
{
    DCM_DSP_ROUTINE_INIT = 0,
    DCM_DSP_ROUTINE_ON = 1,
    DCM_DSP_ROUTINE_OFF = 2
} Dcm_RoutineControlStateType;
#endif

typedef enum
{
    DCM_ON = 1,
    DCM_OFF = 0
} Dcm_MkStateType;

typedef struct
{
    Dcm_SesCtrlType Dcm_ActiveSes;       /*DCM Activity session*/
    Dcm_SecLevelType Dcm_ActiveSec;      /*DCM Activity security*/
    Dcm_ProtocolType Dcm_ActiveProtocol; /*DCM Activity Protocol*/
    uint8 Dcm_ActiveProtocolCfgCtrlId;
    Dcm_MkStateType Dcm_MkState; /*DCM Module status*/
    NetworkHandleType Dcm_ActiveNetwork;
} Dcm_MkCtrlType;

typedef struct
{
    uint16 TesterSourceAddr;     /*Tester source address configured per protocol*/
    uint8 ProtocolId;            /*Id of the protocol on wich the request has been received*/
    uint8 Sid;                   /*Service identifier of the received request*/
    uint8 SubFncId;              /*Identifier of the received subfonction*/
    boolean ReprogramingRequest; /*Set to true in order to request reprogramming of the ECU.*/
    boolean ApplUpdated;         /*Indicate whether the application has been updated or not.*/
    boolean ResponseRequired;
    /*Set to true in case the flashloader or application shall send a response.*/
} Dcm_ProgConditionsType;

typedef struct
{
    /* data program start address */
    uint32 address;

    /* data program block Index(compare with parameter transfered by test) */
    uint8 blockId;

    /* status of 0x36 to do */
    uint8 Status;

    /*callout need MemoryIdentifier*/
    uint8 MemoryIdInfoIndex;

    /*one block size*/
    uint32 BlockLength;

    /*total size*/
    uint32 MemorySize;

    boolean FirstFlag;

    uint8 reqBlockId;

    uint32 DcmLastloadSize;
} Dcm_DspProgramType;

typedef struct
{
    uint8 Subfunction;
    uint32 Data;
    uint16 Size;
} Dcm_DDDidElementsDataTypes;

typedef struct
{
    uint16 Did;
    uint8 SubFunction;
    uint8 Counter;
    uint16 Length;
    uint8 ProtocolCtrlId;
    boolean DDDid;
    uint8 Data[8];
#if (STD_ON == DCM_GENERIC_CONNECTION)
    uint16 TA;
    uint16 SA;
#endif
} SchedulerQueueTypes;

typedef struct
{
    PduIdType PduId;
    boolean Transmit;
} SchedulerQueueTransmitTypes;

/********************************************
      Page  buffer  Data
 ********************************************/
typedef struct
{
    uint32 TotalSize;
    uint16 IloopOne;
    uint16 TotalDtcCount;
    uint32 ReqOffset;
    uint32 ThisPageSize;
    uint32 ThisPageTxSize;
    uint32 LastFilledSize;
    uint32 AlreadyPageSize;
    boolean Filled;
    boolean PageTxOK;
    uint8 PageIndex;
    boolean LastFilled;
    Dcm_MsgType pResData;
#if (DCM_FD == STD_OFF)
    uint8 LastNotTxData[8];
#else
    uint8 LastNotTxData[64];
#endif
    uint32 LastNotTxDataSize;
    boolean TimerStart;
    boolean TimeOut;
    uint32 CurTimer;
    uint32 ExpiredTimer;
} Dcm_PageBufferDataType;

typedef struct
{
    uint8 Dcm_VIN[17];
    boolean Flag;
} Dcm_VINType;

/********************************************
      OBD  buffer
 ********************************************/
typedef struct
{
    PduLengthType Length;
    uint8 Buffer[DCM_FRAME_LENGTH];
} Dcm_OBDMessageType;

typedef enum
{
    DCM_COMM_NO_COMMUNICATION = 0,     /* conmmunication is "NO Communication"status */
    DCM_COMM_SILENT_COMMUNICATION = 1, /* conmmunication is "Silent Communication"status */
    DCM_COMM_FULL_COMMUNICATION = 2    /* conmmunication is "Full Communication"status*/
} Dcm_CommStateType;

typedef enum
{
    DCM_COMM_ACTIVE = 0,
    DCM_COMM_NOT_ACTIVE = 1,
} Dcm_ActiveDiagnosticType;

typedef struct
{
    Dcm_CommStateType Dcm_CommState;
    Dcm_ActiveDiagnosticType Dcm_ActiveDiagnostic;
    NetworkHandleType DcmDslProtocolComMChannelId;
} Dcm_CommCtrlType;

#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
/****************************** declarations *********************************/
/**********************************************************************
 *************Security level management data structure****************
 *********************************************************************/
typedef enum
{
    DCM_SECTIMER_ON = 1u, /*Security level timer "on" state*/
    DCM_SECTIMER_OFF = 0u /*Security level timer "oFF" state*/
} Dcm_SecTimerStateType;

typedef struct
{
    uint32 Dcm_SecCurTimer[DCM_SECURITY_NUM];     /*Security level timer curent Timer*/
    uint32 Dcm_SecExpiredTimer[DCM_SECURITY_NUM]; /*Security level timer timeout Timer*/
    Dcm_SecTimerStateType Dcm_SecTimerState[DCM_SECURITY_NUM];
} Dcm_SecTimerCtrlType;

typedef enum
{
    DCM_SERVICE_IDLE = 0u, /* "Idle" */
    DCM_SERVICE_SEED = 1u, /* After "receiving seed"*/
    DCM_SERVICE_KEY = 2u   /* After "comparing key" */
} Dcm_SecServiceStateType;

typedef struct
{
    uint8 Dcm_SubfunctionForSeed; /*Request seed sub-functions.*/
    uint8 Dcm_FalseAcessCount[DCM_SECURITY_NUM];
    /*the number of Compare key failures and consecutive Request Seed*/
    Dcm_SecLevelType Dcm_ActiveSec;              /*DCM module current level of security*/
    Dcm_SecLevelType Dcm_NewSec;                 /*DCM module to be changed security level*/
    Dcm_SecServiceStateType Dcm_SecServiceState; /*Security level change process status*/
    Dcm_SecTimerCtrlType Dcm_RunDlyCtrl;
    /*When you reach the number of failure,the delay access control block*/
    Dcm_OpStatusType Dcm_OpStatus;
    uint8 Dcm_SecFlag;
    uint8 Dcm_SecCfgIndex;
    uint16 Dcm_MaxReadoutTime;
} Dcm_SecCtrlType;
#endif /* STD_ON == DCM_SECURITY_FUNC_ENABLED */

/***********Session Management sub-function data structure*******
*****************************************************************/
typedef enum
{
    DCM_S3TIMER_ON = 0,
    DCM_S3TIMER_OFF = 1
} Dcm_S3StateType;

typedef struct
{
    uint32 Dcm_S3CurTimer;
    uint32 Dcm_S3ExpiredTimer;   /*S3Timer,timeout Timer*/
    Dcm_S3StateType Dcm_S3State; /*S3timer status */
    uint8 connectionId;
} Dcm_S3CtrlType;

typedef enum
{
    DCM_SESSION_DEFAULT = 0,  /*default session*/
    DCM_SESSION_UNDEFAULT = 1 /*undefault session*/
} Dcm_SesStateType;

typedef struct
{
    Dcm_SesCtrlType Dcm_ActiveSes; /*Module current session-state values;*/
    Dcm_SesCtrlType Dcm_NewSes;
    /*The session control value of the module to be changed;*/
    Dcm_S3CtrlType Dcm_S3Ctrl;         /*S3Timer control block*/
    Dcm_SesStateType Dcm_SessionState; /*DCM module session state*/
} Dcm_SesCtrlCBType;

/*********************************************************
 *The operation of the protocol managment data structure
 *********************************************************/
typedef enum
{
    DCM_PROTOCOL_VALID = 1,  /*Protocol "valid" state*/
    DCM_PROTOCOL_INVALID = 0 /*Protocol "Invalid" state*/
} Dcm_ProtocolStateType;

typedef struct
{
    Dcm_ProtocolType ProtocolId;             /*The protocol ID number*/
    uint8 ProtocolPri;                       /*protocol priority*/
    uint8 MsgCtrlIndex;                      /*Message control block index number*/
    uint32 P2ServerMax;                      /*In the current session state P2ServerMax.*/
    uint32 P2ServerMin;                      /*In the current session state P2ServerMin.*/
    uint32 P2StarServerMax;                  /*In the current session state P2*ServerMax.*/
    uint32 P2StarServerMin;                  /*In the current session state P2*ServerMin.*/
    uint32 S3Server;                         /*S3Server value*/
    Dcm_ProtocolStateType Dcm_ProtocolState; /*Protocol state*/
} Dcm_ProtocolCtrlType;

typedef struct
{
    boolean UDSCall;
    uint8 SID;
    uint8 pid;
} OBD_ReadPidValuesTypes;

/*definition of processing status of link control*/
typedef enum Dcm_LinkControlStatus_t
{
    LINK_CONTROL_IDLE = 0,
    LINK_CONTROL_FBR_VERIFICATION = 1,
    LINK_CONTROL_FBR_TRANSITION = 2,
    LINK_CONTROL_SBR_VERIFICATION = 3,
    LINK_CONTROL_SBR_TRANSITION = 4
} Dcm_LinkControlStatusType;

/*definition of 'link control' control block*/
typedef struct Dcm_LinkControlCtrlType_t
{
    VAR(Dcm_LinkControlStatusType, TYPEDEF) linkCtrlStatus;
    VAR(uint8, TYPEDEF) fixedBaudrate;
    VAR(uint32, TYPEDEF) specialBaudrate;
} Dcm_LinkControlCtrlType;

#endif /* DCM_TYPES_H */
