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
**************************************************************************** **
**                                                                           **
**  FILENAME    : Rte_Dlt_Type.h                                             **
**                                                                           **
**  Created on  :                                                            **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Dlt RTE types define                                       **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                     **
**                                                                           **
**************************************************************************** */
#ifndef RTE_DLT_TYPE_H_
#define RTE_DLT_TYPE_H_

#include "ComStack_Types.h"

typedef uint32 Dlt_ApplicationIDType;
typedef uint32 Dlt_ContextIDType;
typedef uint32 Dlt_SessionIDType;
typedef uint16 Dlt_MessageArgumentCountType;
typedef uint32 Dlt_LogChannelNameType;

typedef enum
{
    DLT_LOG_OFF = 0u,   /*Turn off logging*/
    DLT_LOG_FATAL = 1u, /*Fatal system error*/
    DLT_LOG_ERROR = 2u, /*Errors occurring in a SW-C with impact to correct functionality*/
    DLT_LOG_WARN = 3u,  /*Log messages where a incorrect behavior can not be ensured*/
    DLT_LOG_INFO =
        4u, /*Log messages providing information for better understanding of the internal behavior of a software*/
    DLT_LOG_DEBUG = 5u,   /*Log messages, which are usable only for debugging of a software*/
    DLT_LOG_VERBOSE = 6u, /*Log messages with the highest communicative level, here all possible states, information and
                             everything else can be logged*/
} Dlt_MessageLogLevelType;

typedef enum
{
    DLT_TRACE_VARIABLE = 1u,
    DLT_TRACE_FUNCTION_IN = 2u,
    DLT_TRACE_FUNCTION_OUT = 3u,
    DLT_TRACE_STATE = 4u,
    DLT_TRACE_VFB = 5u,
} Dlt_MessageTraceType;

typedef struct
{
    Dlt_ContextIDType contextId;
    uint8 verbose_mode;
    Dlt_SessionIDType session_id;
    Dlt_MessageLogLevelType logLevel;
    boolean traceStatus;
    uint16 lenContextDescription;
    uint8* contextDescription;
} Dlt_ContextIdInfoType;

typedef struct
{
    Dlt_ApplicationIDType appId;
    uint16 contextIdCount;
    Dlt_ContextIdInfoType* contextInfoList;
    uint16 appDescLen;
    uint8* appDesc;
} Dlt_ApplicationIdInfoType;

typedef struct
{
    uint16 appIdCount;
    Dlt_ApplicationIdInfoType* appIdInfo;
} Dlt_LogInfoType;

/* PRQA S 0750,0635 ++ */ /* MISRA Rule 19.2,6.1 */
typedef union {
    uint8 AllBits;
    struct
    {
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        uint8 verbose_mode : 1;
        uint8 message_type : 3;
        uint8 reserved : 4;
#else
        uint8 reserved : 4;
        uint8 message_type : 3;
        uint8 verbose_mode : 1;
#endif
    } Dlt_Bits;
} Dlt_MessageOptionsType;
/* PRQA S 0750,0635 -- */ /* MISRA Rule 19.2,6.1 */

typedef struct
{
    Dlt_MessageArgumentCountType argCount;
    Dlt_MessageLogLevelType logLevel;
    Dlt_MessageOptionsType options;
    Dlt_ApplicationIDType appId;
    Dlt_ContextIDType contextId;
} Dlt_MessageLogInfoType;

typedef struct
{
    Dlt_MessageTraceType traceInfo;
    Dlt_MessageOptionsType options;
    Dlt_ApplicationIDType appId;
    Dlt_ContextIDType contextId;
} Dlt_MessageTraceInfoType;

typedef enum
{
    DLT_ASSIGN_ADD = 1u,
    DLT_ASSIGN_REMOVE = 2u
} Dlt_AssignmentOperation;

#endif /* RTE_DLT_TYPE_H_ */
