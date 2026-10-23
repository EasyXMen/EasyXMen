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
/* PRQA S 3108-- */
/*
**************************************************************************** **
**                                                                           **
**  FILENAME    : Dlt_Types.h                                                **
**                                                                           **
**  Created on  :                                                            **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Dlt internal types define                                  **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                     **
**                                                                           **
**************************************************************************** */
#ifndef DLT_INTERNAL_H_
#define DLT_INTERNAL_H_

#include "Dlt_Cfg.h"
#include "Dlt_Types.h"
#if (STD_ON == DLT_DEV_ERROR_DETECT)
#include "Det.h"
#endif
#include "PduR_Dlt.h"
#if (DLT_TIME_STAMP_SUPPORT == STD_ON)
#if (DLT_GPT_SUPPORT == STD_ON)
#include "Gpt.h"
#else /* DLT_GPT_SUPPORT == STD_ON */
#include "StbM.h"
#endif /* DLT_GPT_SUPPORT == STD_ON */
#endif /* DLT_TIME_STAMP_SUPPORT == STD_ON */

/* Values of DLT return types */
#define DLT_OK                    ((uint8)0x0u)
#define DLT_E_NOT_OK              ((uint8)0x1u)
#define DLT_E_MSG_TOO_LARGE       ((uint8)0x2u)
#define DLT_E_CONTEXT_ALREADY_REG ((uint8)0x3u)
#define DLT_E_UNKNOWN_SESSION_ID  ((uint8)0x4u)
#define DLT_E_NO_BUFFER           ((uint8)0x5u)
#define DLT_E_CONTEXT_NOT_YET_REG ((uint8)0x6u)
#define DLT_E_NOT_SUPPORTED       ((uint8)0x7u)
#define DLT_E_ERROR               ((uint8)0x9u)

/* Service IDs definition */
#define DLT_API_ID_INIT                  ((uint8)0x01u)
#define DLT_API_ID_GETVERSIONINFO        ((uint8)0x02u)
#define DLT_API_ID_SENDLOGMESSAGE        ((uint8)0x03u)
#define DLT_API_ID_SENDTRACEMESSAGE      ((uint8)0x04u)
#define DLT_API_ID_REGISTERCONTEXT       ((uint8)0x05u)
#define DLT_API_ID_RESETTOFACTORYDEFAULT ((uint8)0x06u)
#define DLT_API_ID_DETFORWARDERRORTRACE  ((uint8)0x07u)
#define DLT_API_ID_SETLOGLEVEL           ((uint8)0x08u)
#define DLT_API_ID_SETTRACESTATUS        ((uint8)0x09u)
#define DLT_API_ID_GETLOGINFO            ((uint8)0x0Au)

#define DLT_API_ID_SETDEFAULTLOGLEVEL    ((uint8)0x11u)
#define DLT_API_ID_SETDEFAULTTRACESTATUS ((uint8)0x12u)

#define DLT_API_ID_UNREGISTERCONTEXT       ((uint8)0x16u)
#define DLT_API_ID_GETLOGCHANNELNAMES      ((uint8)0x17u)
#define DLT_API_ID_GETDEFAULTLOGLEVEL      ((uint8)0x18u)
#define DLT_API_ID_GETDEFAULTTRACESTATUS   ((uint8)0x19u)
#define DLT_API_ID_STORECONFIGURATION      ((uint8)0x1Au)
#define DLT_API_ID_STEMESSAGEFILTERING     ((uint8)0x1Bu)
#define DLT_API_ID_GETTRACESTATUS          ((uint8)0x1Fu)
#define DLT_API_ID_SETLOGCHANNELASSIGNMENT ((uint8)0x20u)
#define DLT_API_ID_SETLOGCHANNELTHRESHOLD  ((uint8)0x21u)
#define DLT_API_ID_GETLOGCHANNELTHRESHOLD  ((uint8)0x22u)

#define DLT_API_ID_TXCONFIRMATION   ((uint8)0x40u)
#define DLT_API_ID_TRIGGERTRANSMIT  ((uint8)0x41u)
#define DLT_API_ID_RXINDICATION     ((uint8)0x42u)
#define DLT_API_ID_COPYTXDATA       ((uint8)0x43u)
#define DLT_API_ID_COPYRXDATA       ((uint8)0x44u)
#define DLT_API_ID_TPRXINDICATION   ((uint8)0x45u)
#define DLT_API_ID_STARTOFRECEPTION ((uint8)0x46u)
#define DLT_API_ID_TPTXCONFIRMATION ((uint8)0x48u)

#define DLT_API_ID_TXFUNCTION ((uint8)0x80u)

/* Development Error Detection and error codes */
#define DLT_E_PARAM                ((uint8)0x01u)
#define DLT_E_PARAM_POINTER        ((uint8)0x02u)
#define DLT_E_INIT_FAILED          ((uint8)0x03u)
#define DLT_E_REGISTRATION         ((uint8)0x04u)
#define DLT_E_SKIPPED_TRANSMISSION ((uint8)0x05u)
#define DLT_E_DEPRECATED_PARAMETER ((uint8)0x06u)
#define DLT_E_MULTIPLE_REQUESTS    ((uint8)0x07u)
#define DLT_E_UNINIT               ((uint8)0x08u)

#define DLT_DET_SESSIONID 0x0Fu

#define DLT_BUFFERED_LENGTH_LENGTH 2u

#define DLT_BUFFERED_TIME_LENGTH 4u

#define DLT_BUFFERED_DATA_TYPE_LENGTH 1u

#define DLT_BUFFERED_DATA_INFO_LENGTH 2u

#define DLT_BUFFERED_SESSIONID_LENGTH 4u

#define DLT_BUFFERED_INFO_LENGTH                                                           \
    (DLT_BUFFERED_LENGTH_LENGTH + DLT_BUFFERED_TIME_LENGTH + DLT_BUFFERED_SESSIONID_LENGTH \
     + DLT_BUFFERED_DATA_TYPE_LENGTH + DLT_BUFFERED_DATA_INFO_LENGTH)

#define DLT_STANDARD_HEADER_LENGTH 16u

#define DLT_EXTENED_HEADER_LENGTH 10u

#define DLT_SERVICE_NOT_SUPPORT_LENGTH 5u /*Service ID:4byte   status:1byte*/

#define DLT_SERVICE_SET_LOG_LEVEL_LENGTH 5u /*Service ID:4byte   status:1byte*/
#define DLT_SET_LOG_LEVEL_TOTAL_LENGTH \
    17u /*Service ID:4byte   applicationId:4byte contextId:4byte  newLogLevel:1byte reserved:4byte*/

#define DLT_SERVICE_SET_TRACE_STATUS_LENGTH 5u /*Service ID:4byte   status:1byte*/
#define DLT_SET_TRACE_STATUS_TOTAL_LENGTH \
    17u /*Service ID:4byte   applicationId:4byte contextId:4byte  newTraceStatus:1byte reserved:4byte*/

#define DLT_GET_LOG_INFO_TOTAL_LENGTH \
    17u /*Service ID:4byte   options:1byte  applicationId:4byte contextId:4byte  reserved:4byte*/
#define DLT_GET_LOG_INFO_ERRRES_LENGTH 9u /*Service ID:4byte   status:1byte  reserved:4byte*/

#define DLT_GET_TRACE_STATUS_TOTAL_LENGTH 12u /*Service ID:4byte   applicationId :4byte  contextId:4byte*/
#define DLT_GET_TRACE_STATUS_RES_LENGTH   6u  /*Service ID:4byte   status:1byte  traceStatus:1byte*/

#define DLT_GET_DEFAULT_LOG_INFO_RES_LENGTH 6u /*Service ID:4byte   status:1byte  logLevel:1byte*/

#define DLT_STORE_CONFIGURATION_TOTAL_LENGTH 5u /*Service ID:4byte   status:1byte */

#define DLT_RESET_TO_FACTORY_DEFAULT_RES_LENGTH 5u /*Service ID:4byte   status:1byte*/

#define DLT_SET_MESSAGE_FILTERING_TOTAL_LENGTH 5u /*Service ID:4byte   newstatus:1byte*/
#define DLT_SET_MESSAGE_FILTERING_RES_LENGTH   5u /*Service ID:4byte   status:1byte*/

#define DLT_SET_DEFAULT_LOG_LEVEL_TOTAL_LENGTH 9u /*Service ID:4byte   newLogLevel:1byte  reserved:4byte*/
#define DLT_SET_DEFAULT_LOG_LEVEL_RES_LENGTH   5u /*Service ID:4byte   status:1byte*/

#define DLT_SET_DEFAULT_TRACE_STATUS_TOTAL_LENGTH 9u /*Service ID:4byte   newTraceStatus:1byte  reserved:4byte*/
#define DLT_SET_DEFAULT_TRACE_STATUS_RES_LENGTH   5u /*Service ID:4byte   status:1byte*/

#define DLT_GET_SOFT_WARE_VERSION_RES_LENGTH 16u /*Service ID:4byte   status:1byte  Length:4byte  swVersion:7byte*/
#define DLT_SOFT_WARE_VERSION_LENGTH         7u  /*swVersion:7byte*/

#define DLT_GET_DEFAULT_TRACE_STATUS_RES_LENGTH 6u /*Service ID:4byte   status:1byte  traceStatus:1byte*/

#define DLT_GET_LOG_CHANNEL_NAMES_RES_LENGTH \
    (6u + (4u * DLT_CHANNEL_NUM)) /*Service ID:4byte   status:1byte  countIf:1byte  logChannelNames: 4*uin8[]*/

#define DLT_TOTAL_LENGTH_SET_LOG_CHANNEL_ASSIGNMENT \
    17u /*Service ID:4byte   applicationId:4byte contextId:4byte  logChannelName:4byte addRemoveOp:1byte*/
#define DLT_RES_LENGTH_SET_LOG_CHANNEL_ASSIGNMENT 5u /*Service ID:4byte   status:1byte*/

#define DLT_SET_LOG_CHANNEL_THRESHOLD_TOTAL_LENGTH \
    10u /*Service ID:4byte   logChannelName:4byte logLevelThreshold:1byte traceStatus:1byte*/
#define DLT_SET_LOG_CHANNEL_THRESHOLD_RES_LENGTH 6u /*Service ID:4byte   status:1byte traceStatus:1byte*/

#define DLT_GET_LOG_CHANNEL_THRESHOLD_TOTAL_LENGTH 8u /*Service ID:4byte   logChannelName:4byte*/
#define DLT_GET_LOG_CHANNEL_THRESHOLD_RES_LENGTH \
    7u /*Service ID:4byte   status:1byte logLevelThreshold:1byte traceStatus:1byte*/

#define DLT_SYNC_TIME_STAMP_RES_LENGTH \
    15u /*Service ID:4byte   status:1byte  synctimestamp:Nanoseconds:4byte Seconds:4byte Seconds:2byte*/

#define DLT_CALL_SWC_INJECTION_RES_LENGTH 5u /*Service ID:4byte   status:1byte*/

#define DLT_BUFFER_OVER_FLOW_LENGTH 9u /*Service ID:4byte   status:1byte  overflowCounter:4byte*/

#define DLT_BUFFER_GET_LOG_INFO_LENGTH \
    17u /*Service ID:4byte   options:1byte  applicationId:4byte  contextId:4byte   reserved:4byte*/

#define DLT_MESSAGE_NO_OTHER_INFO_LENGTH 4u /*Service ID:4byte */

#define DLT_OPTIONS_MESSAGE_TYPE_MASK       0x0Eu
#define DLT_OPTIONS_MESSAGE_TYPE_RESET_MASK 0xF1u
#define DLT_OPTIONS_VERBOSE_MODE_MASK       0x01u
#define DLT_OPTIONS_VERBOSE_MODE_RESET_MASK 0xFEu

#define DLT_NVM_ACTIVATED_CODE 0xA5A5A5A5u

#if (DLT_GPT_SUPPORT == STD_ON)
#define DLT_GPT_TIME_RESOLUTION_MS 0x2710u
#endif

typedef enum
{
    DLT_LOG_DATA     = 0u,
    DLT_TRACE_DATA   = 1u,
    DLT_CONTROL_DATA = 2u
} Dlt_BufferDataTypeTypes;

typedef enum
{
    DLT_STATUS_OK            = 0u,
    DLT_STATUS_NOT_SUPPORTED = 1u,
    DLT_STATUS_ERROR         = 2u
} Dlt_statusTypes;

#define DLT_UNUSED(x) (void)(x)

#if (STD_ON == DLT_DEV_ERROR_DETECT)
#define DLT_DETREPORT(ApiId, ErrorId) (Det_ReportError(DLT_MODULE_ID, DLT_INSTANCE_ID, (ApiId), (ErrorId)))
#endif

#if (DLT_DET_FORWARD_TO_DET_SUPPORT == STD_ON)
typedef struct
{
    Dlt_MessageLogLevelType DltThreshold;
    uint16                  DltLogChannelRef;
    Dlt_ApplicationIDType   DetApplicationId;
    Dlt_ContextIDType       DetContextId;
} Dlt_DetContextInofType;

typedef struct
{
    Dlt_SessionIDType      DltSwcSessionId;
    Dlt_DetContextInofType DetContextInfo;
} Dlt_DetInfoType;
#endif

typedef struct
{
    Dlt_MessageLogLevelType DltThreshold;
    boolean                 TraceStatus;
    boolean                 Register;
    uint16                  DltLogChannelRefNum;
#if (DLT_CHANNEL_NUM > 0)
    uint16 DltLogChannelRef[DLT_CHANNEL_NUM];
#endif
    Dlt_SwcContextType SwcContext;
    uint8*             appDescription;
    uint8              lenAppDescription;
    uint8*             contextDescription;
    uint8              lenContextDescription;
} Dlt_SwcContextInofType;

typedef struct
{
    Dlt_SessionIDType DltSwcSessionId;
    uint16            DltSwcContextNum;
#if (DLT_SWC_MAX_CONTEXT_NUM > 0)
    Dlt_SwcContextInofType SwcContextInfo[DLT_SWC_MAX_CONTEXT_NUM];
#endif
} Dlt_SwcInfoType;

typedef struct
{
    Dlt_MessageLogLevelType DltLogChannelThreshold;
    boolean                 DltLogTraceStatusFlag;
} Dlt_ChannelInfoType;

typedef enum
{
    DLT_MESSAGE_FILTER,
    DLT_MESSAGE_BLOCKD,
    DLT_MESSAGE_PASS,
} Dlt_MessageFilterPassType;

typedef struct
{
    uint32                    ActivatedCode;
    boolean                   MessageFilterEnable;
    Dlt_MessageFilterPassType MessageFilterPass;
    Dlt_MessageLogLevelType   DefaultLogLevel;
    boolean                   DefaultTraceStatus;
#if (DLT_SWC_NUM > 0)
    Dlt_SwcInfoType SwcInfo[DLT_SWC_NUM];
#endif /* DLT_SWC_NUM > 0 */
#if (DLT_CHANNEL_NUM > 0)
    Dlt_ChannelInfoType ChannelInfo[DLT_CHANNEL_NUM];
#endif
#if (DLT_DET_FORWARD_TO_DET_SUPPORT == STD_ON)
    Dlt_DetInfoType DetInfo;
#endif
} Dlt_RunTimeType;

typedef enum
{
    DLT_STATE_UNINIT = 1u,
    DLT_STATE_INIT   = 2u,
} Dlt_ModeStateType;

typedef enum
{
    DLT_WAIT_SEND             = 0u,
    DLT_SEND_NEED_RETRY       = 1u,
    DLT_SEND_NOT_CONFIRMATION = 2u,
    DLT_CONTROL_SENDED        = 3u,
} Dlt_SendStateType;

typedef struct
{
    boolean                 OverFlow;
    boolean                 OverFlowSend;
    uint32                  overflowCounter;
    uint32                  BufferOverflowTimer;
    Dlt_SessionIDType       SessionId;
    uint32                  timestamp;
    Dlt_BufferDataTypeTypes BufferDataType;
    Dlt_SwcContextType      SwcContext;
} Dlt_OverFlowInfoTypes;

typedef struct
{
#if (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0)
    uint8 buffer[DLT_CHANNEL_MAX_BUFFER_LENGTH];
#endif
    uint32                ReadIndex;
    uint32                WriteIndex;
    Dlt_OverFlowInfoTypes OverFlowInfo;
    uint8                 MessageCounter;
    uint32                UnusedLength;
    Dlt_SendStateType     LastSendStatus;
    boolean               ControlSend;
    uint8                 SendCounter;
    uint32                LastSendLength;
    uint16                LastControlSendLength;
#if (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0)
    uint8 SendControlBuffer[DLT_CHANNEL_MAX_BUFFER_LENGTH];
#endif
    uint16 ControlSendOffset;
    uint16 MessageSendOffset;
} Dlt_ChannelType;

typedef struct
{
    uint16  RxLength;
    uint16  RxOffset;
    boolean Used;
#if (DLT_BUFFER_MAX_LENGTH > 0)
    uint8 RxBuffer[DLT_BUFFER_MAX_LENGTH];
#endif
} Dlt_RxStatusTypes;

typedef struct
{
    uint16                    ChannelIndex;
    Dlt_BufferDataTypeTypes   BufferDataType;
    Dlt_MessageLogInfoType*   logInfo;
    Dlt_MessageTraceInfoType* traceInfo;
    Dlt_SwcContextType        SwcContext;
} Dlt_CreateExtendedHeaderInfoTypes;

typedef struct
{
    uint16            ChannelIndex;
    uint16            Messagelength;
    Dlt_SessionIDType SessionId;
    uint32            timestamp;
} Dlt_CreateStandardHeaderInfoTypes;

typedef union
{
    uint32 AllBits;
    struct
    {
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        uint32 UEH     : 1;
        uint32 MSBF    : 1;
        uint32 WEID    : 1;
        uint32 WSID    : 1;
        uint32 WTMS    : 1;
        uint32 VERS    : 3;
        uint32 ErrFlag : 8;
        uint32 status  : 16;
#else
        uint32 status  : 16;
        uint32 ErrFlag : 8;
        uint32 VERS    : 3;
        uint32 WTMS    : 1;
        uint32 WSID    : 1;
        uint32 WEID    : 1;
        uint32 MSBF    : 1;
        uint32 UEH     : 1;
#endif
    } Bits;
} Dlt_OptionalFlagType;

#if ((DLT_RX_DATA_PATH_SUPPORT == STD_ON) && (DLT_RXPDU_NUM > 0))
#define DLT_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dlt_MemMap.h"
extern VAR(Dlt_RxStatusTypes, AUTOMATIC) Dlt_RxStatus[DLT_RXPDU_NUM];
#define DLT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dlt_MemMap.h"
#endif

#define DLT_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dlt_MemMap.h"
extern VAR(Dlt_RunTimeType, AUTOMATIC) Dlt_RunTime;
#define DLT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dlt_MemMap.h"
#if (STD_ON == DLT_DEV_ERROR_DETECT)
#define DLT_START_SEC_VAR_INIT_8
#include "Dlt_MemMap.h"
extern VAR(Dlt_ModeStateType, AUTOMATIC) Dlt_ModeState;
#define DLT_STOP_SEC_VAR_INIT_8
#include "Dlt_MemMap.h"
#endif

#define DLT_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dlt_MemMap.h"
extern P2CONST(Dlt_ConfigType, AUTOMATIC, DLT_APPL_CONST) Dlt_ConfigPtr;
#define DLT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dlt_MemMap.h"

#if (DLT_CHANNEL_NUM > 0)
#define DLT_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dlt_MemMap.h"
extern VAR(Dlt_ChannelType, AUTOMATIC) Dlt_Channel[DLT_CHANNEL_NUM];
#define DLT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dlt_MemMap.h"
#endif

static inline void Dlt_GetOptionFlag(Dlt_OptionalFlagType* OptionFlagPtr)
{
    const Dlt_ProtocolType* protocolPtr = Dlt_ConfigPtr->Protocol;
    OptionFlagPtr->Bits.UEH =
        ((protocolPtr->DltUseExtHeaderInNonVerbMode == TRUE) && (protocolPtr->DltUseVerboseMode == TRUE)) ? 1u : 0u;
    OptionFlagPtr->Bits.WEID    = (protocolPtr->DltHeaderUseEcuId == TRUE) ? 1u : 0u;
    OptionFlagPtr->Bits.WSID    = (protocolPtr->DltHeaderUseSessionID == TRUE) ? 1u : 0u;
    OptionFlagPtr->Bits.WTMS    = (protocolPtr->DltHeaderUseTimestamp == TRUE) ? 1u : 0u;
    OptionFlagPtr->Bits.ErrFlag = 0u;
}

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(void, DLT_CODE) Dlt_GetChannelIndex(
    uint16                SwcIndex,
    Dlt_ApplicationIDType app_id,
    Dlt_ContextIDType     context_id,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DLT_APPL_DATA) ChannelIndexList);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
extern FUNC(void, DLT_CODE) Dlt_MemCopy(uint8* Dest, const uint8* Src, uint32 Size);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(uint16, DLT_CODE) Dlt_GetSwcSessionIdIndex(Dlt_SessionIDType sessionId);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(void, DLT_CODE)
    Dlt_Tx_CopyTxDataToSendBuffer(uint16 ChannelIndex, uint8* Dest, uint32 SendOffset, uint32 Length);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
#define Dlt_CopyLongToArray Dlt_CopyLongToArrayLittleEndian
#define Dlt_CopyIntToArray  Dlt_CopyIntToArrayLittleEndian
#else
#define Dlt_CopyLongToArray Dlt_CopyLongToArrayBigEndian
#define Dlt_CopyIntToArray  Dlt_CopyIntToArrayBigEndian
#endif

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(void, DLT_CODE) Dlt_CopyArrayToLongLittleEndian(
    P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData,
    P2CONST(uint8, AUTOMATIC, DLT_CONST) SourceData);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(void, DLT_CODE)
    Dlt_CopyLongToArrayLittleEndian(uint32 SourceData, P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(void, DLT_CODE)
    Dlt_CopyIntToArrayLittleEndian(uint16 SourceData, P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#endif
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(void, DLT_CODE) Dlt_CopyArrayToLongBigEndian(
    P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData,
    P2CONST(uint8, AUTOMATIC, DLT_CONST) SourceData);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(void, DLT_CODE)
    Dlt_CopyLongToArrayBigEndian(uint32 SourceData, P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(void, DLT_CODE)
    Dlt_CopyIntToArrayBigEndian(uint16 SourceData, P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(void, DLT_CODE) Dlt_CopyArrayToIntBigEndian(
    P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) DestData,
    P2CONST(uint8, AUTOMATIC, DLT_CONST) SourceData);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(uint32, DLT_CODE) Dlt_GetEcuId(void);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(uint16, DLT_CODE)
    Dlt_GetMessageLength(boolean VerboseMode, uint16 PayloadLength, Dlt_OptionalFlagType OptionFlag);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_InterSetLogLevel(
    uint16                  ChannelIndex,
    Dlt_ApplicationIDType   appId,
    Dlt_ContextIDType       contextId,
    Dlt_MessageLogLevelType newLogLevel);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_InterSetTraceStatus(
    uint16                ChannelIndex,
    Dlt_ApplicationIDType appId,
    Dlt_ContextIDType     contextId,
    boolean               newTraceStatus);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_SendGetLogInfo(uint8 options, uint16 SwcIndex, uint16 SwcContextIndex);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_InterSetLogChannelAssignment(
    Dlt_ApplicationIDType   appId,
    Dlt_ContextIDType       contextId,
    uint32                  channelId,
    Dlt_AssignmentOperation addRemoveOp);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#if (DLT_NVM_RAM_SUPPORT == STD_ON)
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(boolean, DLT_CODE) Dlt_CheckNvmActivated(void);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_Store(void);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#endif
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(uint16, DLT_CODE) Dlt_GetChannelIndexByChannelName(uint32 DltLogChannelId);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(uint16, DLT_CODE) Dlt_GetChannelIndexByTxPduId(PduIdType id);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_DealRxData(uint16 RxIndex, const uint8* Data, uint16 Length);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(uint32, DLT_CODE) Dlt_GetTimeElapsed(void);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(void, DLT_CODE) Dlt_Init_From_Cfg(const Dlt_ConfigType* config);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(void, DLT_CODE) Dlt_CreateControlStandardHeader(
    Dlt_CreateStandardHeaderInfoTypes* CreateStandardHeader,
    uint8*                             Dest,
    uint16*                            CorrectionLength,
    Dlt_OptionalFlagType               OptionFlag);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(void, DLT_CODE)
    Dlt_CreateControlExtendedHeader(const Dlt_CreateExtendedHeaderInfoTypes* CreateExtendedHeader, uint8* Dest);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
extern FUNC(void, DLT_CODE) Dlt_MoveReadIndex(uint16 ChannelIndex, uint32 length);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#endif /* DLT_INTERNAL_H_ */
