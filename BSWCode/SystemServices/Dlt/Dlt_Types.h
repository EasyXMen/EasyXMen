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
**  FILENAME    : Dlt_Types.h                                                **
**                                                                           **
**  Created on  :                                                            **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Dlt types define                                           **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                     **
**                                                                           **
**************************************************************************** */
#ifndef DLT_TYPES_H_
#define DLT_TYPES_H_

#include "Rte_Dlt_Type.h"

/*===================[VERSION  INFORMATION]====================================*/
#define DLT_PBCFG_H_AR_MAJOR_VERSION 4U
#define DLT_PBCFG_H_AR_MINOR_VERSION 5U
#define DLT_PBCFG_H_AR_PATCH_VERSION 0U
#define DLT_PBCFG_H_SW_MAJOR_VERSION 2U
#define DLT_PBCFG_H_SW_MINOR_VERSION 0U
#define DLT_PBCFG_H_SW_PATCH_VERSION 0U

/*============================================================================*/
/*=======[R E V I S I O N   H I S T O R Y]====================================*
*  <VERSION>    <DATE>       <AUTHOR>         <REVISION LOG>
*  V2.0.0       2020/11/18    tao.yu       Initial version
============================================================================*/

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*==============================================================================
*                                          CONSTANTS
==============================================================================*/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
typedef uint32 Dlt_MessageIDType;
typedef uint32 Dlt_LogChannelIDType;

typedef enum
{
    DLT_TYPE_LOG = 0u,
    DLT_TYPE_APP_TRACE = 1u,
    DLT_TYPE_NW_TRACE = 2u,
    DLT_TYPE_CONTROL = 3u,
} Dlt_MessageType;

typedef enum
{
    DLT_CONTROL_REQUEST = 1u,
    DLT_CONTROL_RESPONSE = 2u
} Dlt_MessageControlType;

typedef enum
{
    DLT_NW_TRACE_IPC = 1u,
    DLT_NW_TRACE_CAN = 2u,
    DLT_NW_TRACE_FLEXRAY = 3u,
    DLT_NW_TRACE_MOST = 4u,
    DLT_NW_TRACE_ETHERNET = 5u,
    DLT_NW_TRACE_SOMEIP = 6u,
} Dlt_MessageNetworkTraceInfoType;

typedef struct
{
    Dlt_ApplicationIDType SwcApplicationId;
    Dlt_ContextIDType SwcContextId;
} Dlt_SwcContextType;

typedef struct
{
    Dlt_MessageLogLevelType DltThreshold;
    P2CONST(Dlt_SwcContextType, AUTOMATIC, DLT_APPL_CONST) DltLogLevelThresholdSwcContextRef;
} Dlt_LogLevelThesholdType;

typedef struct
{
    Dlt_MessageLogLevelType DltDefaultLogLevel;
    uint8 LogLevelThesholdNum;
    P2CONST(Dlt_LogLevelThesholdType, AUTOMATIC, DLT_APPL_CONST) DltLogLevelThreshold;
} Dlt_LogLevelSettingType;

typedef struct
{
    /*TX PDUR ID*/
    PduIdType DltTxPduRPduId;
    /* DLT internal handle Id*/
    PduIdType DltTxHandlePduId;
    boolean DltTxPduUsesTp;
} Dlt_TxPduType;

typedef struct
{
    PduIdType DltRxPduId;
    boolean DltRxPduUsesTp;
} Dlt_RxPduType;

typedef struct
{
    uint16 DltLogChannelBufferOverflowTimer;
    uint32 DltLogChannelBufferSize;
    uint32 DltLogChannelId;
    uint16 DltLogChannelMaxMessageLength;
    uint8 DltLogChannelMaxNumOfRetries;
    Dlt_MessageLogLevelType DltLogChannelThreshold;
    uint32 DltLogChannelTrafficShapingBandwidth;
    uint16 DltLogChannelTransmitCycle; /*not used*/
    boolean DltLogTraceStatusFlag;
    P2CONST(Dlt_TxPduType, AUTOMATIC, DLT_APPL_CONST) DltTxPdu;
} Dlt_LogChannelType;

typedef struct
{
    P2CONST(Dlt_SwcContextType, AUTOMATIC, DLT_APPL_CONST) DltLogChannelAssignmentSwcContextRef;
    uint16 DltLogChannelRef;
} Dlt_LogChannelAssignmentType;

typedef struct
{
    uint16 ChannelNum;
    P2CONST(Dlt_LogChannelType, AUTOMATIC, DLT_APPL_CONST) LogChannel;
    uint16 LogChannelAssignmentNum;
    P2CONST(Dlt_LogChannelAssignmentType, AUTOMATIC, DLT_APPL_CONST) LogChannelAssignment;
    uint16 DltDefaultLogChannelRef;
} Dlt_LogOutputType;

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(uint32, DCM_APPL_CODE, Dlt_EcuIdCalloutTypes)(void);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

typedef struct
{
    Dlt_EcuIdCalloutTypes DltEcuIdCallout;
    uint32 DltEcuIdValue;
    /*Container Choices
     * if DltEcuIdCallout Choices then DltEcuIdValue shall be 0xFFFFFFFF
     * if DltEcuIdValue Choices then DltEcuIdCallout shall be NULL_PTR*/
} Dlt_EcuIdTypes;

typedef struct
{
    boolean DltHeaderUseEcuId;
    boolean DltHeaderUseSessionID;
    boolean DltHeaderUseTimestamp;
    boolean DltUseExtHeaderInNonVerbMode;
    boolean DltUseVerboseMode;
    P2CONST(Dlt_EcuIdTypes, AUTOMATIC, DLT_APPL_CONST) DltEcuId;
} Dlt_ProtocolType;

typedef struct
{
    boolean DltTraceStatus;
    P2CONST(Dlt_SwcContextType, AUTOMATIC, DLT_APPL_CONST) DltTraceStatusAssignmentSwcContextRef;
} Dlt_TraceStatusAssignmentTypes;

typedef struct
{
    boolean DltDefaultTraceStatus;
    uint8 DltTraceStatusAssignmentNum;
    P2CONST(Dlt_TraceStatusAssignmentTypes, AUTOMATIC, DLT_APPL_CONST) DltTraceStatusAssignment;
} Dlt_TraceStatusSettingType;

typedef struct
{
    P2CONST(Dlt_LogLevelSettingType, AUTOMATIC, DLT_APPL_CONST) LogLevelSetting;
    P2CONST(Dlt_LogOutputType, AUTOMATIC, DLT_APPL_CONST) LogOutput;
    P2CONST(Dlt_ProtocolType, AUTOMATIC, DLT_APPL_CONST) Protocol;
    uint16 RxPduNum;
    P2CONST(Dlt_RxPduType, AUTOMATIC, DLT_APPL_CONST) RxPdu;
    P2CONST(Dlt_TraceStatusSettingType, AUTOMATIC, DLT_APPL_CONST) TraceStatusSetting;
} Dlt_ConfigType;

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(void, DCM_APPL_CODE, LogLevelChangedNotificationType)(
    Dlt_ApplicationIDType appId,
    Dlt_ContextIDType contextId,
    Dlt_MessageLogLevelType newLogLevel);

typedef P2FUNC(void, DCM_APPL_CODE, TraceStatusChangedNotificationType)(
    Dlt_ApplicationIDType appId,
    Dlt_ContextIDType contextId,
    boolean newTraceStatus);

typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, InjectionCallbackType)(
    Dlt_ApplicationIDType appId,
    Dlt_ContextIDType contextId,
    uint32 serviceId,
    uint32 dataLength,
    uint8* data);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

typedef struct
{
    Dlt_SessionIDType DltSwcSessionId;
    boolean DltSwcSupportLogLevelAndTraceStatusChangeNotification;
    TraceStatusChangedNotificationType TraceStatusChangedNotification;
    LogLevelChangedNotificationType LogLevelChangedNotification;
    InjectionCallbackType InjectionCallback;
    uint16 MaxSwcLogMessageLength;
    uint16 MaxSwcTraceMessageLength;
    uint16 DltSwcContextNum;
    P2CONST(Dlt_SwcContextType, AUTOMATIC, DLT_APPL_CONST) DltSwcContext;
} Dlt_SwcTypes;

typedef struct
{
    uint16 DltSwcNum;
    P2CONST(Dlt_SwcTypes, AUTOMATIC, DLT_APPL_CONST) DltSwc;
} Dlt_SwcType;

#endif /* DLT_TYPES_H_ */
