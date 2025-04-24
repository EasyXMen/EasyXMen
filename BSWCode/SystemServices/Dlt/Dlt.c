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
**  FILENAME    : Dlt.c                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : Public functions implementation of Dlt                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Dlt<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code.

    \li PRQA S 1532 MISRA Rule 8.7 .<br>
    Reason:In order to make the module code structure clear, the functions are classified.

    \li PRQA S 0791 MISRA Rule 5.4 .<br>
    Reason:Configuration data design is required.

    \li PRQA S 0311 MISRA Rule 11.8 .<br>
    Reason:Design required type conversions.

    \li PRQA S 4340 MISRA Rule 10.5 .<br>
    Reason:Design required type conversions.

    \li PRQA S 4330 MISRA Rule 10.5 .<br>
    Reason:Design required type conversions.

    \li PRQA S 4332 MISRA Rule 10.5 .<br>
    Reason:Design required type conversions.

    \li PRQA S 4394 MISRA Rule 10.8 .<br>
    Reason:Design required type conversions.

    \li PRQA S 4342 MISRA Rule 10.5 .<br>
    Reason:Design required type conversions.

    \li PRQA S 4304 MISRA Rule 10.5 .<br>
    Reason:Design required type conversions.

    \li PRQA S 3218 MISRA Rule 8.9 .<br>
    Reason:Design required type conversions.

    \li PRQA S 0750 MISRA Rule 12.9 .<br>
    Reason:Design required type conversions.

    \li PRQA S 0635 MISRA Rule 6.1 .<br>
    Reason:Design required type conversions.

    \li PRQA S 3415 MISRA Rule 13.5 .<br>
    Reason:Right hand operand of '&&' or '||' is an expression with persistent side effects.

    \li PRQA S 4442 MISRA Rule 10.3 .<br>
    Reason:An expression of 'essentially unsigned' type (%1s) is being converted to enum type, '%2s' on assignment.

    \li PRQA S 3440 MISRA Rule 13.3 .<br>
    Reason:Using the value resulting from a ++ or -- operation.

    \li PRQA S 3396 MISRA Rule 12.1 .<br>
    Reason:Extra parentheses recommended. A binary operation is the operand of a conditional operator.

    \li PRQA S 3225 MISRA Rule 18.6 .<br>
    Reason:Address of automatic object exported using a function parameter.

    \li PRQA S 4461 MISRA Rule 10.3 .<br>
    Reason:A non-constant expression of 'essentially unsigned' type (%1s) is being converted to narrower unsigned type,
  '%2s' on assignment.
 */

#include "Dlt.h"
#include "Dlt_internal.h"
#include "SchM_Dlt.h"

#if (DLT_NVM_RAM_SUPPORT == STD_ON)
#include "Nvm.h"
#endif

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#ifndef DLT_AR_RELEASE_MAJOR_VERSION
#error " DLT version miss"
#endif
#ifndef DLT_AR_RELEASE_MINOR_VERSION
#error " DLT version miss"
#endif
#ifndef DLT_AR_RELEASE_PATCH_VERSION
#error " DLT version miss"
#endif
#if ((1 != DLT_CFG_SW_MAJOR_VERSION) || (0 != DLT_CFG_SW_MINOR_VERSION) || (0 != DLT_CFG_SW_PATCH_VERSION))
#error " cfg version mismatching"
#endif

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

#define DLT_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dlt_MemMap.h"
VAR(Dlt_RunTimeType, AUTOMATIC) Dlt_RunTime;
#define DLT_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dlt_MemMap.h"

#define DLT_START_SEC_VAR_NO_INIT_16BIT
#include "Dlt_MemMap.h"
static VAR(uint16, AUTOMATIC) Dlt_StartUpDelayTimer;
#define DLT_STOP_SEC_VAR_NO_INIT_16BIT
#include "Dlt_MemMap.h"

#define DLT_START_SEC_VAR_POWER_ON_INIT_8BIT
#include "Dlt_MemMap.h"
#if (STD_OFF == DLT_DEV_ERROR_DETECT)
static
#endif
    VAR(Dlt_ModeStateType, AUTOMATIC) Dlt_ModeState = DLT_STATE_UNINIT;
#define DLT_STOP_SEC_VAR_POWER_ON_INIT_8BIT
#include "Dlt_MemMap.h"

#if ((DLT_GPT_SUPPORT == STD_OFF) && ((DLT_TIME_STAMP_SUPPORT == STD_ON)))
#define DLT_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dlt_MemMap.h"
static StbM_TimeStampType Dlt_TimeStamp;
#define DLT_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dlt_MemMap.h"
#endif

/* PRQA S 0791++ */ /* MISRA Rule 5.4 */
#define DLT_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dlt_MemMap.h"
P2CONST(Dlt_ConfigType, AUTOMATIC, DLT_APPL_CONST) Dlt_ConfigPtr = NULL_PTR;
#define DLT_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "Dlt_MemMap.h"
/* PRQA S 0791-- */ /* MISRA Rule 5.4 */

#if (DLT_CHANNEL_NUM > 0)
#define DLT_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dlt_MemMap.h"
VAR(Dlt_ChannelType, AUTOMATIC) Dlt_Channel[DLT_CHANNEL_NUM];
#define DLT_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dlt_MemMap.h"
#endif

#if ((DLT_DET_FORWARD_TO_DET_SUPPORT == STD_ON) && (DLT_SWC_MAX_CONTEXT_NUM > 0))

/* PRQA S 3218++ */ /* MISRA Rule 8.9 */
#define DLT_START_SEC_CONST_32BIT
#include "Dlt_MemMap.h"
/* DET application ID */
static CONST(uint32, DLT_CONST) Dlt_Det_ApplId = 0x44455400; /* DET  */ /* PRQA S 3218 */ /* MISRA Rule 8.9 */
#define DLT_STOP_SEC_CONST_32BIT
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CONST_32BIT
#include "Dlt_MemMap.h"
/* DET context ID */
static CONST(uint32, DLT_CONST) Dlt_Det_ContextId = 0x53544400; /* STD  */ /* PRQA S 3218 */ /* MISRA Rule 8.9 */
#define DLT_STOP_SEC_CONST_32BIT
#include "Dlt_MemMap.h"
/* PRQA S 3218-- */ /* MISRA Rule 8.9 */

/* PRQA S 3218++ */ /* MISRA Rule 8.9 */
#define DLT_START_SEC_CONST_8BIT
#include "Dlt_MemMap.h"
#if (DLT_DET_APPLID_DESC_LENGTH > 0)
/* DET application ID description */
static const uint8 Dlt_Det_ApplId_Desc[DLT_DET_APPLID_DESC_LENGTH] = {
    /* D,    E,    T,     ,    E,    v,    e,    n,    t, NULL */
    0x44,
    0x45,
    0x54,
    0x20,
    0x45,
    0x76,
    0x65,
    0x6E,
    0x74,
    0x00 /* DET Event */
};
#endif

#if (DLT_DET_CONTEXTID_DESC_LENGTH > 0)
/* DET context ID description */
static const uint8 Dlt_Det_ContextId_Desc[DLT_DET_CONTEXTID_DESC_LENGTH] = {
    /* S,    T,    D, NULL */
    0x53,
    0x54,
    0x44,
    0x00 /* STD */
};
#endif
#define DLT_STOP_SEC_CONST_8BIT
#include "Dlt_MemMap.h"
/* PRQA S 3218-- */ /* MISRA Rule 8.9 */

#endif /* DLT_DET_FORWARD_TO_DET_SUPPORT == STD_ON && DLT_SWC_MAX_CONTEXT_NUM > 0 */

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
static FUNC(boolean, DLT_CODE)
    Dlt_GetSwcTraceStatus(uint16 SwcIndex, P2CONST(Dlt_MessageTraceInfoType, AUTOMATIC, DLT_APPL_CONST) traceInfo);
static FUNC(Dlt_MessageLogLevelType, DLT_CODE)
    Dlt_GetSwcLogLevel(uint16 SwcIndex, P2CONST(Dlt_MessageLogInfoType, AUTOMATIC, DLT_APPL_CONST) logInfo);
static FUNC(boolean, DLT_CODE) Dlt_GetChannelTraceStatus(uint16 ChannelIndex);
static FUNC(Dlt_MessageLogLevelType, DLT_CODE) Dlt_GetChannelLogLevel(uint16 ChannelIndex);
static FUNC(void, DLT_CODE) Dlt_MoveWriteIndexCopy(uint16 ChannelIndex, uint8 value);
static FUNC(boolean, DLT_CODE)
    Dlt_CompareLogLevel(Dlt_MessageLogLevelType CurrentLogLevel, Dlt_MessageLogLevelType MessageLogLevel);
static FUNC(boolean, DLT_CODE) Dlt_CompareTraceStatus(boolean CurrentLogLevel);
static FUNC(void, DLT_CODE) Dlt_Tx_Channel(uint16 ChannelIndex);
static FUNC(void, DLT_CODE) Dlt_SendOverFlow(uint16 ChannelIndex);
static FUNC(void, DLT_APPL_CODE) Dlt_MoveChannelRef(uint16* DltLogChannelRef, uint16 Length);
static FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_InterSendTraceMessage(
    Dlt_SessionIDType sessionId,
    P2CONST(Dlt_MessageTraceInfoType, AUTOMATIC, DLT_APPL_CONST) traceInfo,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) traceData,
    uint16 traceDataLength);
static FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_InterSendLogMessage(
    Dlt_SessionIDType sessionId,
    P2CONST(Dlt_MessageLogInfoType, AUTOMATIC, DLT_APPL_CONST) logInfo,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) logData,
    uint16 logDataLength);
static FUNC(void, DLT_CODE) Dlt_Tx_RemoveBuffer(uint16 ChannelIndex, uint32 RemoveOffset, uint32 Length);
static FUNC(void, DLT_CODE) Dlt_MemSet(uint8* Dest, uint8 Value, uint32 Size);
static FUNC(void, DLT_CODE) Dlt_CreateStandardHeader(const Dlt_CreateStandardHeaderInfoTypes* CreateStandardHeader);
static FUNC(void, DLT_CODE) Dlt_CreateExtendedHeader(const Dlt_CreateExtendedHeaderInfoTypes* CreateExtendedHeader);
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               Dlt is using the NVRamManager and is to be initialized very
 *                     late in the ECU startup phase. The Dlt_Init() function should
 *                     be called after the NVRamManager is initialized.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      config: Pointer to a DLT configuration structure
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 */
FUNC(void, DLT_APPL_CODE) Dlt_Init(P2CONST(Dlt_ConfigType, AUTOMATIC, DLT_APPL_CONST) config)
{
#if (DLT_NVM_RAM_SUPPORT == STD_ON)
    NvM_RequestResultType RequestResult;
    Std_ReturnType ret;
#endif
#if ((DLT_GPT_SUPPORT == STD_OFF) && ((DLT_TIME_STAMP_SUPPORT == STD_ON)))
    StbM_UserDataType userData;
#endif
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    if (NULL_PTR == config)
    {
        DLT_DETREPORT(DLT_API_ID_INIT, DLT_E_PARAM_POINTER);
    }
    else
#endif
    {
        /* Copy global Configuration item to store the configuration pointer. */
        Dlt_ConfigPtr = config;

#if (DLT_CHANNEL_NUM > 0)
        Dlt_MemSet((uint8*)Dlt_Channel, 0u, (sizeof(Dlt_ChannelType) * DLT_CHANNEL_NUM));
#endif

#if ((DLT_RX_DATA_PATH_SUPPORT == STD_ON) && (DLT_RXPDU_NUM > 0))
        Dlt_MemSet((uint8*)Dlt_RxStatus, 0u, (sizeof(Dlt_RxStatusTypes) * DLT_RXPDU_NUM));
#endif

#if (DLT_TIME_STAMP_SUPPORT == STD_ON)
#if (DLT_GPT_SUPPORT == STD_ON)
        /* Gets the current timestamp from the GPT module */
        Gpt_StartTimer(DLT_TIME_REF, DLT_GPT_START_TIME);
#else
        /* Gets the current timestamp from the STBM module */
        (void)StbM_GetCurrentTime(DLT_TIME_REF, &Dlt_TimeStamp, &userData);
#endif
#endif
#if (DLT_NVM_RAM_SUPPORT == STD_ON)
        ret = NvM_GetErrorStatus(DLT_NVM_RAM_REF, &RequestResult);
        /* Check if the storage stack is ready */
        if ((ret == E_OK) && (RequestResult == NVM_REQ_OK))
        {
            /* Gets initialization parameters from the NvM storage stack */
            Dlt_MemCopy(
                (uint8*)&Dlt_RunTime,
                (const uint8*)NvM_BlockDescriptor[DLT_NVM_RAM_REF - 1u].NvmRamBlockDataAddress,
                sizeof(Dlt_RunTimeType));
            uint16 index = 0;
#if (1u < DLT_CHANNEL_NUM)
            for (; index < DLT_CHANNEL_NUM; index++)
#endif /* 1u < DLT_CHANNEL_NUM */
            {
                Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[index]);
                uint32 logChannelBufferSize = config->LogOutput->LogChannel[index].DltLogChannelBufferSize;
                SchM_Enter_Dlt_MsgChannel();
                ChannelPtr->UnusedLength = logChannelBufferSize;
                ChannelPtr->LastSendStatus = DLT_WAIT_SEND;
                SchM_Exit_Dlt_MsgChannel();
            }
        }
        else
#endif
        {
            /* Get the initialization parameters from the configuration information structure */
            Dlt_Init_From_Cfg(config);
        }
        /* This parameter is used to set the startup delay of the DLT module */
        Dlt_StartUpDelayTimer = DLT_START_UP_DELAY_TIMER;
        /* Set the status of the DLT module to initialized */
        Dlt_ModeState = DLT_STATE_INIT;
    }
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#if (DLT_VERSION_INFO_API == STD_ON)
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               Returns the version information of this module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     versioninfo:Pointer to where to store the version information of this module.
 * Param-Name[in/out]  N/A
 * Return              N/A
 */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(void, DLT_APPL_CODE) Dlt_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DLT_APPL_DATA) versioninfo)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    if (NULL_PTR == versioninfo)
    {
        DLT_DETREPORT(DLT_API_ID_GETVERSIONINFO, DLT_E_PARAM_POINTER);
    }
    else
#endif
    {
        versioninfo->vendorID = DLT_VENDOR_ID;
        versioninfo->moduleID = DLT_MODULE_ID;
        versioninfo->sw_major_version = DLT_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = DLT_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = DLT_SW_PATCH_VERSION;
    }
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#endif

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"

/*
 * Brief               The service represents the interface to be used by basic
 *                     software modules or by software components to trace parameters.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      sessionId:Number of the module (Module ID within BSW, Port defined argument value within SW-C)
 *                     traceInfo:Structure containing the relevant information for filtering the message.
 *                     traceData:Buffer containing the parameters to be traced. The contents of this pointer
 *                     represents the payload of the Trace Message to be sent.
 *                     traceDataLength:Length of the data buffer traceData
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType:E_OK: The required operation succeeded.
 *                     DLT_E_MSG_TOO_LARGE: The message is too large for all assigned LogChannels.
 *                     DLT_E_NO_BUFFER: Not enough buffer available, the Dlt message cannot be buffered for at least one
 * LogChannel. DLT_E_UNKNOWN_SESSION_ID: The provided session id is unknown.
 */
FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_SendTraceMessage(
    Dlt_SessionIDType sessionId,
    P2CONST(Dlt_MessageTraceInfoType, AUTOMATIC, DLT_APPL_CONST) traceInfo,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) traceData,
    uint16 traceDataLength)
{
    Std_ReturnType ret;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    ret = E_NOT_OK;
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else if ((NULL_PTR == traceInfo) || (NULL_PTR == traceData))
    {
        DLT_DETREPORT(DLT_API_ID_SENDTRACEMESSAGE, DLT_E_PARAM_POINTER);
    }
    else
#endif
    {
        ret = Dlt_InterSendTraceMessage(sessionId, traceInfo, traceData, traceDataLength);
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               The service represents the interface to be used by basic software modules or by software
 * component to send Log Messages. ServiceId           0x03 Sync/Async          Synchronous Reentrancy Reentrant
 * Param-Name[in]      sessionId:For SW-C this is not visible (Port defined argument value), for BSWmodules it is the
 * module number logInfo: Structure containing the relevant information for filtering the message. logData:Buffer
 * containing the parameters to be logged. The contents of this pointer represents the payload of the Log Message to be
 * sent. logDataLengt: Length of the data buffer logData. Param-Name[out]     N/A Param-Name[in/out]  N/A Return
 * Std_ReturnType:DLT_OK: The required operation succeeded. DLT_E_MSG_TOO_LARGE: The message is too large for all
 * assigned LogChannels DLT_E_NO_BUFFER: The LogMessage could not be buffered at any assigned LogChannel
 *                     DLT_E_UNKNOWN_SESSION_ID: The provided session id is unknown.
 */
FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_SendLogMessage(
    Dlt_SessionIDType sessionId,
    P2CONST(Dlt_MessageLogInfoType, AUTOMATIC, DLT_APPL_CONST) logInfo,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) logData,
    uint16 logDataLength)
{
    Std_ReturnType ret;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    ret = E_NOT_OK;
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else if ((NULL_PTR == logInfo) || (NULL_PTR == logData))
    {
        DLT_DETREPORT(DLT_API_ID_SENDLOGMESSAGE, DLT_E_PARAM_POINTER);
    }
    else
#endif
    {
        ret = Dlt_InterSendLogMessage(sessionId, logInfo, logData, logDataLength);
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               The service has to be called when a software module wants to use services offered
 *                     by DLT software component for a specific context. If a ContextId is being registered
 *                     for an already registered ApplicationId then appDescription can be NULL and lenappDescription
 * zero. ServiceId           0x05 Sync/Async          Synchronous Reentrancy          Reentrant Param-Name[in]
 * sessionId:number of the module (Module ID within BSW, Port defined argument value within SW-C) appId:the
 * ApplicationId contextId:the ContextId appDescription:Points to description string for the provided ApplicationId. At
 * maximum 255 characters are interpreted. lenAppDescription:The length of the description for the ApplicationId string
 * (number of characters of description string). contextDescription:Points to description string for the provided
 * context. At maximum 255 characters are interpreted. lenContextDescription:The length of the description string
 * (number of characters of description string). Param-Name[out]     N/A Param-Name[in/out]  N/A Return
 * Std_ReturnType:E_OK: The required operation succeeded. DLT_E_CONTEXT_ALREADY_REG: The software module context has
 * already registered. DLT_E_UNKNOWN_SESSION_ID: The provided session id is unknown.
 */
FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_RegisterContext(
    Dlt_SessionIDType sessionId,
    Dlt_ApplicationIDType appId,
    Dlt_ContextIDType contextId,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) appDescription,
    uint8 lenAppDescription,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) contextDescription,
    uint8 lenContextDescription)
{
    Std_ReturnType ret = E_NOT_OK;
#if (DLT_REGISTER_CONTEXT_NOTIFICATION_SUPPORT == STD_ON)
    uint8 options;
#if (DLT_GET_INFO_STATUS7_SUPPORT == STD_OFF)
    DLT_UNUSED(appDescription);
    DLT_UNUSED(lenAppDescription);
    DLT_UNUSED(contextDescription);
    DLT_UNUSED(lenContextDescription);
#endif
#else
    DLT_UNUSED(appDescription);
    DLT_UNUSED(lenAppDescription);
    DLT_UNUSED(contextDescription);
    DLT_UNUSED(lenContextDescription);
#endif

#if (STD_ON == DLT_DEV_ERROR_DETECT)
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else
#endif
    {
        /* The Swc index number is extracted by the session identifier */
        uint16 SwcIndex = Dlt_GetSwcSessionIdIndex(sessionId);
#if (1u < DLT_SWC_NUM)
        if (SwcIndex == DLT_SWC_NUM)
        {
            /* Return unknown session identifier */
            ret = DLT_E_UNKNOWN_SESSION_ID;
        }
        else
#endif /* 1u < DLT_SWC_NUM */
        {
#if ((DLT_SWC_NUM > 0) && (DLT_SWC_MAX_CONTEXT_NUM > 0))
            /* The Swc context number is extracted by Swc index */
            uint16 DltSwcContextNum = Dlt_RunTime.SwcInfo[SwcIndex].DltSwcContextNum;
            for (uint16 SwcContextIndex = 0; SwcContextIndex < DltSwcContextNum; SwcContextIndex++)
            {
                Dlt_SwcContextInofType* SwcContextInfoPtr =
                    &(Dlt_RunTime.SwcInfo[SwcIndex].SwcContextInfo[SwcContextIndex]);
                Dlt_ApplicationIDType SwcApplicationId = SwcContextInfoPtr->SwcContext.SwcApplicationId;
                Dlt_ContextIDType SwcContextId = SwcContextInfoPtr->SwcContext.SwcContextId;
                /* Check if the incoming appId/contextId parameter is matche */
                if ((appId == SwcApplicationId) && (contextId == SwcContextId))
                {
                    SchM_Enter_Dlt_SwcContext();
                    /* Set the Swc context information to registered */
                    SwcContextInfoPtr->Register = TRUE;
                    /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
                    /* Attach the application description for the Swc context information */
                    SwcContextInfoPtr->appDescription = (uint8*)appDescription;
                    /* Attach the context description for the Swc context information */
                    SwcContextInfoPtr->contextDescription = (uint8*)contextDescription;
                    /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
                    /* Set the application description length for the Swc context information */
                    SwcContextInfoPtr->lenAppDescription = lenAppDescription;
                    /* Set the context description length for the Swc context information */
                    SwcContextInfoPtr->lenContextDescription = lenContextDescription;
                    SchM_Exit_Dlt_SwcContext();
#if (DLT_REGISTER_CONTEXT_NOTIFICATION_SUPPORT == STD_ON)
#if (DLT_GET_INFO_STATUS7_SUPPORT == STD_ON)
                    options = 7;
#else  /* DLT_GET_INFO_STATUS7_SUPPORT == STD_ON */
                    options = 6;
#endif /* DLT_GET_INFO_STATUS7_SUPPORT == STD_ON */
                    /* If DLT supports context notification, the response message is sent to get log information */
                    ret = Dlt_SendGetLogInfo(options, SwcIndex, SwcContextIndex);
#else
                    ret = E_OK;
#endif /* STD_ON == DLT_REGISTER_CONTEXT_NOTIFICATION_SUPPORT */
                }
            }
#endif /* DLT_SWC_NUM > 0 && DLT_SWC_MAX_CONTEXT_NUM > 0 */
        }
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               The service has to be called when a software module is going to be stopped.
 * ServiceId           0x16
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      sessionId:number of the module (Module ID within BSW, Port defined argument value within SW-C)
 *                     appId:the ApplicationId
 *                     contextId:the ContextId
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType:E_OK: The required operation succeeded.
 *                     DLT_E_CONTEXT_ALREADY_REG: The software module context has already registered.
 *                     DLT_E_UNKNOWN_SESSION_ID: The provided session id is unknown.
 */
FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_UnregisterContext(Dlt_SessionIDType sessionId, Dlt_ApplicationIDType appId, Dlt_ContextIDType contextId)
{
    Std_ReturnType ret = E_NOT_OK;
#if (DLT_REGISTER_CONTEXT_NOTIFICATION_SUPPORT == STD_ON)
    uint8 options = 5;
#endif

#if (STD_ON == DLT_DEV_ERROR_DETECT)
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else
#endif
    {
        /* The Swc index number is extracted by the session identifier */
        uint16 SwcIndex = Dlt_GetSwcSessionIdIndex(sessionId);
#if (1u < DLT_SWC_NUM)
        if (SwcIndex == DLT_SWC_NUM)
        {
            /* Return unknown session identifier */
            ret = DLT_E_UNKNOWN_SESSION_ID;
        }
        else
#endif /* 1u < DLT_SWC_NUM */
        {
#if ((DLT_SWC_NUM > 0) && (DLT_SWC_MAX_CONTEXT_NUM > 0))
            /* The Swc context number is extracted by Swc index */
            uint16 DltSwcContextNum = Dlt_RunTime.SwcInfo[SwcIndex].DltSwcContextNum;
            for (uint16 SwcContextIndex = 0; SwcContextIndex < DltSwcContextNum; SwcContextIndex++)
            {
                Dlt_SwcContextInofType* SwcContextInfoPtr =
                    &(Dlt_RunTime.SwcInfo[SwcIndex].SwcContextInfo[SwcContextIndex]);
                Dlt_ApplicationIDType SwcApplicationId = SwcContextInfoPtr->SwcContext.SwcApplicationId;
                Dlt_ContextIDType SwcContextId = SwcContextInfoPtr->SwcContext.SwcContextId;
                /* Check if the incoming appId/contextId parameter is matche */
                if ((appId == SwcApplicationId) && (contextId == SwcContextId))
                {
                    SchM_Enter_Dlt_SwcContext();
                    /* Set the Swc context information to unregistered */
                    SwcContextInfoPtr->Register = FALSE;
                    /* Set the application description length for the Swc context information */
                    SwcContextInfoPtr->lenAppDescription = 0U;
                    /* Set the context description length for the Swc context information */
                    SwcContextInfoPtr->lenContextDescription = 0U;
                    /* Clear the application description for the Swc context information */
                    SwcContextInfoPtr->appDescription = NULL_PTR;
                    /* Clear the context description for the Swc context information */
                    SwcContextInfoPtr->contextDescription = NULL_PTR;
                    SchM_Exit_Dlt_SwcContext();
#if (DLT_REGISTER_CONTEXT_NOTIFICATION_SUPPORT == STD_ON)
                    /* If DLT supports context notification, the response message is sent to get log information */
                    ret = Dlt_SendGetLogInfo(options, SwcIndex, SwcContextIndex);
#else
                    ret = E_OK;
#endif
                }
            }
#endif /* DLT_SWC_NUM > 0 && DLT_SWC_MAX_CONTEXT_NUM > 0 */
        }
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#if (DLT_DET_FORWARD_TO_DET_SUPPORT == STD_ON)
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               Service to forward error reports from Det to Dlt
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      moduleId:Module ID of calling module.
 *                     instanceId:The identifier of the index based instance of a module, starting from 0.
 *                     If the module is a single instance module it shall pass 0 as the instanceId.
 *                     apiId:ID of API service in which error is detected (defined in SWS of calling module)
 *                     errorId:ID of detected development error (defined in SWS of calling module).
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 */
/* "DetForwardErrorTrace" Send log message format: StandardHeader(16), ExternHeader(10), Payload(9/21) */
/* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
/* Payload format1:
 * MessageId(4),MuduleId(2),InstanceId(1),AppId(1),ErrorId(1) */
/* Payload format2:
 * MessageId(4),MuduleId(2),ExInstanceId(4),InstanceId(1),ExAppId(4),AppId(1),ExErrorId(4),ErrorId(1) */
FUNC(void, DLT_APPL_CODE) Dlt_DetForwardErrorTrace(uint16 moduleId, uint8 instanceId, uint8 apiId, uint8 errorId)
{
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else
#endif
    {
#if ((DLT_SWC_NUM > 0) && (DLT_SWC_MAX_CONTEXT_NUM > 0))
        uint32 timestamp = 0U;
        Dlt_MessageLogInfoType logInfo;

        logInfo.logLevel = DLT_LOG_ERROR;
        logInfo.argCount = 4;
        logInfo.appId = Dlt_Det_ApplId;
        logInfo.contextId = Dlt_Det_ContextId;
        logInfo.options.Dlt_Bits.message_type = (uint8)DLT_TYPE_LOG;
        /*Generating the timestamp*/
        if (Dlt_ConfigPtr->Protocol->DltHeaderUseTimestamp == TRUE)
        {
            timestamp = Dlt_GetTimeElapsed();
        }
        /* The Swc index number is extracted by the session identifier */
        uint16 SwcIndex = Dlt_GetSwcSessionIdIndex(DLT_DET_SESSIONID);
#if (1u < DLT_SWC_NUM)
        /* Check if the Swc index number is valid */
        if (SwcIndex != DLT_SWC_NUM)
#endif /* 1u < DLT_SWC_NUM */
        {
            boolean Filter;
            Dlt_MessageLogLevelType CurrentLogLevel;
            uint16 ChannelIndexList[DLT_CHANNEL_NUM];

            Dlt_MemSet((uint8*)ChannelIndexList, 0xFF, (DLT_CHANNEL_NUM + DLT_CHANNEL_NUM));
            /* The Swc index and log information are used to extract the current log level */
            CurrentLogLevel = Dlt_GetSwcLogLevel(SwcIndex, &logInfo);
            /* Message filtering */
            Filter = Dlt_CompareLogLevel(CurrentLogLevel, logInfo.logLevel);
            /* Check if the message filtering status is passed */
            if (Filter == TRUE)
            {
                /*Select target LogChannel*/
                Dlt_GetChannelIndex(SwcIndex, logInfo.appId, logInfo.contextId, ChannelIndexList);
                uint16 iloop = 0;
                /* Traverse the list of all log channel index */
#if (1u < DLT_CHANNEL_NUM)
                while ((ChannelIndexList[iloop] != 0xFFFFu) && (iloop < DLT_CHANNEL_NUM))
#else  /* 1u < DLT_CHANNEL_NUM */
                if (ChannelIndexList[iloop] != 0xFFFFu)
#endif /* 1u < DLT_CHANNEL_NUM */
                {
                    uint16 logDataLength;
                    /* Set the index number of the log channel */
                    uint16 ChannelIndex = ChannelIndexList[iloop];
                    /* Check if the DLT message extension header is support verbose mode */
                    boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
                    /* Check if the DLT log data is support verbose mode */
                    if (Dlt_ConfigPtr->Protocol->DltUseVerboseMode == FALSE)
                    {
                        /* Calculate the byte length of the DLT log data */
                        logDataLength = 9;
                    }
                    else
                    {
                        /* Calculate the byte length of the DLT log data */
                        logDataLength = 21;
                    }
                    uint16 logChannelMaxMessageLength =
                        Dlt_ConfigPtr->LogOutput->LogChannel[ChannelIndex].DltLogChannelMaxMessageLength;
                    uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, logDataLength);
                    /* Check if the message length is within valid range */
                    if (logChannelMaxMessageLength >= Messagelength)
                    {
                        /* The current log level is extracted by the log channel index */
                        CurrentLogLevel = Dlt_GetChannelLogLevel(ChannelIndex);
                        /* Message filtering */
                        Filter = Dlt_CompareLogLevel(CurrentLogLevel, logInfo.logLevel);
                        /* Check if the message filtering status is passed */
                        if (Filter == TRUE)
                        {
                            Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
                            /* Copying Dlt message to the LogChannel buffer */
                            /* Check if the length of the DLT message buffer is overflow */
                            if (ChannelPtr->UnusedLength < (uint32)Messagelength)
                            {
                                Dlt_OverFlowInfoTypes* OverFlowInfoPtr = &(ChannelPtr->OverFlowInfo);
                                SchM_Enter_Dlt_MsgChannel();
                                /* Set the message buffer overflow flag to "TRUE" for given log channel */
                                OverFlowInfoPtr->OverFlow = TRUE;
                                /* Set the message buffer overflow send flag to "TRUE" for given log channel */
                                OverFlowInfoPtr->OverFlowSend = FALSE;
                                /* clear the message buffer overflow timer for given log channel */
                                OverFlowInfoPtr->BufferOverflowTimer = 0U;
                                /* Set the message buffer overflow counter for given log channel */
                                OverFlowInfoPtr->overflowCounter = (uint32)Messagelength;
                                /* Set the message buffer data type to "DLT_CONTROL_DATA" for given log channel */
                                OverFlowInfoPtr->BufferDataType = DLT_CONTROL_DATA;
                                /* Set the message buffer SessionId to "DLT_DET_SESSIONID" for given log channel */
                                OverFlowInfoPtr->SessionId = DLT_DET_SESSIONID;
                                /* Set the message buffer timestamp for given log channel */
                                OverFlowInfoPtr->timestamp = timestamp;
                                /* Set the message buffer SwcApplicationId for given log channel */
                                OverFlowInfoPtr->SwcContext.SwcApplicationId = logInfo.appId;
                                /* Set the message buffer SwcContextId for given log channel */
                                OverFlowInfoPtr->SwcContext.SwcContextId = logInfo.contextId;
                                SchM_Exit_Dlt_MsgChannel();
                            }
                            else
                            {
                                uint8 logData[21];
                                Dlt_CreateExtendedHeaderInfoTypes CreateExtendedHeaderInfo;
                                Dlt_CreateStandardHeaderInfoTypes CreateStandardHeaderInfo;

                                SchM_Enter_Dlt_MsgChannel();
                                /* Create standard header for the message of the DLT given log channel */
                                CreateStandardHeaderInfo.ChannelIndex = ChannelIndex;
                                CreateStandardHeaderInfo.Messagelength = Messagelength;
                                CreateStandardHeaderInfo.SessionId = DLT_DET_SESSIONID;
                                CreateStandardHeaderInfo.timestamp = timestamp;
                                Dlt_CreateStandardHeader(&CreateStandardHeaderInfo);
                                /* Check if the DLT message extension header is support verbose mode */
                                if (Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode == TRUE)
                                {
                                    /* Create extended header for the message of the DLT given log channel */
                                    CreateExtendedHeaderInfo.ChannelIndex = ChannelIndex;
                                    CreateExtendedHeaderInfo.BufferDataType = DLT_LOG_DATA;
                                    CreateExtendedHeaderInfo.logInfo = (Dlt_MessageLogInfoType*)&logInfo;
                                    CreateExtendedHeaderInfo.traceInfo = NULL_PTR;
                                    CreateExtendedHeaderInfo.SwcContext.SwcApplicationId = logInfo.appId;
                                    CreateExtendedHeaderInfo.SwcContext.SwcContextId = logInfo.contextId;
                                    Dlt_CreateExtendedHeader(&CreateExtendedHeaderInfo);
                                }
                                /* Check if the DLT log data is support verbose mode */
                                if (Dlt_ConfigPtr->Protocol->DltUseVerboseMode == FALSE)
                                {
                                    /* Specify field fill "MessageId" to the DLT message buffer */
                                    Dlt_CopyLongToArrayBigEndian((uint32)0x00000002, logData);
                                    /* Specify field fill "moduleId" to the DLT message buffer */
                                    Dlt_CopyIntToArrayBigEndian(moduleId, &(logData[4]));
                                    /* Specify field fill "instanceId" to the DLT message buffer */
                                    logData[6] = instanceId;
                                    /* Specify field fill "apiId" to the DLT message buffer */
                                    logData[7] = apiId;
                                    /* Specify field fill "errorId" to the DLT message buffer */
                                    logData[8] = errorId;
                                }
                                else
                                {
                                    /* Type Info - MessageId - Unsigned int, 32 bit */
                                    /* Specify field fill "MessageId" to the DLT message buffer */
                                    Dlt_CopyLongToArrayBigEndian((uint32)0x00000042, logData);
                                    /* Type Info - ModuleId - Unsigned int, 16 bit */
                                    /* Specify field fill "MuduleId" to the DLT message buffer */
                                    Dlt_CopyIntToArrayBigEndian(moduleId, &(logData[4]));
                                    /* Type Info - Extend InstanceId - Unsigned int, 32 bit */
                                    /* Specify field fill "Extend InstanceId" to the DLT message buffer */
                                    Dlt_CopyLongToArrayBigEndian((uint32)0x00000041, &(logData[6]));
                                    /* Type Info - InstanceId - Unsigned int, 8 bit */
                                    /* Specify field fill "InstanceId" to the DLT message buffer */
                                    logData[10] = instanceId;
                                    /* Type Info - Extend AppId - Unsigned int, 32 bit */
                                    /* Specify field fill "Extend AppId" to the DLT message buffer */
                                    Dlt_CopyLongToArrayBigEndian((uint32)0x00000041, &(logData[11]));
                                    /* Type Info - AppId - Unsigned int, 8 bit */
                                    /* Specify field fill "AppId" to the DLT message buffer */
                                    logData[15] = apiId;
                                    /* Type Info - Extend ErrorId - Unsigned int, 32 bit */
                                    /* Specify field fill "Extend ErrorId" to the DLT message buffer */
                                    Dlt_CopyLongToArrayBigEndian((uint32)0x00000041, &(logData[16]));
                                    /* Specify field fill "ErrorId" to the DLT message buffer */
                                    logData[20] = errorId;
                                }
                                /* Copies the user's trace data into the buffer of the given log channel */
                                for (uint16 jloop = 0; jloop < logDataLength; jloop++)
                                {
                                    Dlt_MoveWriteIndexCopy(ChannelIndex, logData[jloop]);
                                }
                                ChannelPtr->UnusedLength -= (uint32)Messagelength;
                                ChannelPtr->MessageCounter++;

                                SchM_Exit_Dlt_MsgChannel();
                            }
                        }
                    }
#if (1u < DLT_CHANNEL_NUM)
                    iloop++;
#endif /* 1u < DLT_CHANNEL_NUM */
                }
            }
        }
#endif
    }
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#endif

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               This service is used to change the LogLevel for the given tuple of ApplicationID/ContextID.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      appId:ID of the SW-C
 *                     contextId:ID of the context
 *                     newLogLevel:new log level to set
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType:E_OK: No error occurred
 *                     E_NOT_OK: LogLevel could not be changed
 */
FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_SetLogLevel(Dlt_ApplicationIDType appId, Dlt_ContextIDType contextId, Dlt_MessageLogLevelType newLogLevel)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else
#endif
    {
        uint16 index = 0u;
#if (DLT_CHANNEL_NUM > 0)
        uint16 ChannelIndexList[DLT_CHANNEL_NUM];
#if (1u < DLT_SWC_NUM)
        for (; index < DLT_SWC_NUM; index++)
#endif /* 1u < DLT_SWC_NUM */
        {
            /*Select target LogChannel*/
            Dlt_GetChannelIndex(index, appId, contextId, ChannelIndexList);
        }
        index = 0u;
#if (1u < DLT_CHANNEL_NUM)
        for (; ((ChannelIndexList[index] != 0xFFFFu) && (index < DLT_CHANNEL_NUM)); index++)
#else  /* 1u < DLT_CHANNEL_NUM */
        if (ChannelIndexList[index] != 0xFFFFu)
#endif /* 1u < DLT_CHANNEL_NUM */
        {
            ret = Dlt_InterSetLogLevel(ChannelIndexList[index], appId, contextId, newLogLevel);
        }
#endif
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               The service Dlt_SetTraceStatus sets the trace status for a specific tuple of ApplicationID and
 * ContextID. ServiceId           0x09 Sync/Async          Synchronous Reentrancy          Non Reentrant Param-Name[in]
 * appId:ID of the SW-C contextId:ID of the context newLogLevel:new log level to set Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType:E_OK: No error occurred
 *                     E_NOT_OK: LogLevel could not be changed
 */
FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_SetTraceStatus(Dlt_ApplicationIDType appId, Dlt_ContextIDType contextId, boolean newTraceStatus)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else
#endif
    {
#if (DLT_CHANNEL_NUM > 0)
        uint16 index = 0u;
        uint16 ChannelIndexList[DLT_CHANNEL_NUM];
#if (1u < DLT_SWC_NUM)
        for (; index < DLT_SWC_NUM; index++)
#endif /* 1u < DLT_SWC_NUM */
        {
            /*Select target LogChannel*/
            Dlt_GetChannelIndex(index, appId, contextId, ChannelIndexList);
        }
        index = 0u;
#if (1u < DLT_CHANNEL_NUM)
        for (; ((ChannelIndexList[index] != 0xFFFFu) && (index < DLT_CHANNEL_NUM)); index++)
#else  /* 1u < DLT_CHANNEL_NUM */
        if (ChannelIndexList[index] != 0xFFFFu)
#endif /* 1u < DLT_CHANNEL_NUM */
        {
            ret = Dlt_InterSetTraceStatus(ChannelIndexList[index], appId, contextId, newTraceStatus);
        }
#endif
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               Called to request information about registered ApplicationIds, their ContextIds and the
 * corresponding log level. ServiceId           0x0a Sync/Async          Synchronous Reentrancy          Non Reentrant
 * Param-Name[in]      options: Used to filter the response in respect to the ApplicationId, Context Id and Trace Status
 * information appId:Representation of the ApplicationId contextId:Representation of the ContextId Param-Name[out]
 * status logInfo:Details about the returned Application IDs Param-Name[in/out]  N/A Return Std_ReturnType:E_OK: No
 * error occurred E_NOT_OK:  LogInfo could not be returned
 */
FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_GetLogInfo(
    uint8 options,
    Dlt_ApplicationIDType appId,
    Dlt_ContextIDType contextId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) status,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    Dlt_LogInfoType* logInfo)
{
    Std_ReturnType ret = E_NOT_OK;
    DLT_UNUSED(options);
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else if ((status == NULL_PTR) || (logInfo == NULL_PTR))
    {
        DLT_DETREPORT(DLT_API_ID_GETLOGINFO, DLT_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint16 SwcIndex = 0;
#if (1u < DLT_SWC_NUM)
        for (; (SwcIndex < DLT_SWC_NUM) && (ret == E_NOT_OK); SwcIndex++)
#endif /* 1u < DLT_SWC_NUM */
        {
#if (DLT_SWC_NUM > 0 && DLT_SWC_MAX_CONTEXT_NUM > 0)
            uint16 DltSwcContextNum = Dlt_RunTime.SwcInfo[SwcIndex].DltSwcContextNum;
            for (uint16 SwcContextIndex = 0; (SwcContextIndex < DltSwcContextNum) && (ret == E_NOT_OK);
                 SwcContextIndex++)
            {
                Dlt_SwcContextInofType* SwcContextInfoPtr =
                    &(Dlt_RunTime.SwcInfo[SwcIndex].SwcContextInfo[SwcContextIndex]);
                Dlt_ApplicationIDType SwcApplicationId = SwcContextInfoPtr->SwcContext.SwcApplicationId;
                Dlt_ContextIDType SwcContextId = SwcContextInfoPtr->SwcContext.SwcContextId;
                /* Checks if the ApplicationId/ContextId of the Swc information structure is match */
                if ((appId == SwcApplicationId) && (contextId == SwcContextId))
                {
                    boolean useVerboseMode = Dlt_ConfigPtr->Protocol->DltUseVerboseMode;
                    uint8 lenAppDescription = SwcContextInfoPtr->lenAppDescription;
                    uint8 lenContextDescription = SwcContextInfoPtr->lenContextDescription;
                    Dlt_SessionIDType SwcSessionId = Dlt_RunTime.SwcInfo[SwcIndex].DltSwcSessionId;

                    Dlt_ApplicationIdInfoType appIdInfo[1];
                    Dlt_ContextIdInfoType contextInfoList[1];

                    /* Copy the "contextId" described by the Swc context to the log information */
                    contextInfoList[0].contextId = contextId;
                    /* Copy the "VerboseMode" described by the Swc context to the log information */
                    /* PRQA S 4304++ */ /* MISRA Rule 10.5 */
                    contextInfoList[0].verbose_mode = (uint8)useVerboseMode;
                    /* PRQA S 4304-- */ /* MISRA Rule 10.5 */
                    /* Copy the "SessionId" described by the Swc context to the log information */
                    contextInfoList[0].session_id = SwcSessionId;
                    /* Copy the "logLevel" described by the Swc context to the log information */
                    contextInfoList[0].logLevel = SwcContextInfoPtr->DltThreshold;
                    /* Copy the "traceStatus" described by the Swc context to the log information */
                    contextInfoList[0].traceStatus = SwcContextInfoPtr->TraceStatus;
                    /* Copy the "lenContextDescription" described by the Swc context to the log information */
                    contextInfoList[0].lenContextDescription = lenContextDescription;
                    /* Copy the "contextDescription" described by the Swc context to the log information */
                    contextInfoList[0].contextDescription = SwcContextInfoPtr->contextDescription;

                    /* Copy the "AppId" of the Swc application description to the log information */
                    appIdInfo[0].appId = appId;
                    /* Copy the "lenAppDescription" described by the Swc application into the log information */
                    appIdInfo[0].appDescLen = lenAppDescription;
                    /* Copy the "appDescription" of the Swc application description to the log information */
                    appIdInfo[0].appDesc = SwcContextInfoPtr->appDescription;
                    /* Copy the "contextIdCount" of Swc context descriptions into the log information */
                    appIdInfo[0].contextIdCount = 1;
                    /* Copy the "contextInfoList" described in the Swc context into the log information */
                    appIdInfo[0].contextInfoList = &(contextInfoList[0]);

                    *status = (uint8)SwcContextInfoPtr->TraceStatus; /* PRQA S 4304 */ /* MISRA Rule 10.5 */
                    /* Copy the number of Swc application descriptions to the log information */
                    logInfo->appIdCount = 1;
                    /* Copy the Swc application description packet into the log information */
                    logInfo->appIdInfo = &(appIdInfo[0]); /* PRQA S 3225 */ /* MISRA Rule 18.6 */

                    ret = E_OK;
                }
            }
#endif /* DLT_SWC_NUM > 0 && DLT_SWC_MAX_CONTEXT_NUM > 0 */
        }
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               Returns the Default Log Level currently used by the Dlt module.
 *                     The returned Log Level might differ from the one which is stored non volatile.
 * ServiceId           0x18
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     defaultLogLevel:Returns the stored LogLevel setting
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType:E_OK: No error occurred
 *                     E_NOT_OK:The default LogLevel could not be returned
 */
FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_GetDefaultLogLevel(Dlt_MessageLogLevelType* defaultLogLevel)
{
    Std_ReturnType ret;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    ret = E_NOT_OK;
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else if (defaultLogLevel == NULL_PTR)
    {
        DLT_DETREPORT(DLT_API_ID_GETDEFAULTLOGLEVEL, DLT_E_PARAM_POINTER);
    }
    else
#endif
    {
        *defaultLogLevel = Dlt_RunTime.DefaultLogLevel;
        ret = E_OK;
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               Copies the current Dlt configuration to NvRAM by calling NvM_WriteBlock().
 * ServiceId           0x1a
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType:E_OK: No error occurred
 *                     E_NOT_OK: The configuration could not be stored
 *                     DLT_E_NOT_SUPPORTED: Service is not supported
 */
FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_StoreConfiguration(void)
{
#if (DLT_NVM_RAM_SUPPORT == STD_ON)
    Std_ReturnType ret = E_NOT_OK;
#else
    Std_ReturnType ret = DLT_E_NOT_SUPPORTED;
#endif
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else
#endif
    {
#if (DLT_NVM_RAM_SUPPORT == STD_ON)
        ret = Dlt_Store();
        if (ret == E_NOT_OK)
        {
            ret = DLT_E_ERROR;
        }
#else
        /*idle*/
#endif
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               The service Dlt_ResetToFactoryDefault sets the LogLevel and TraceStatus back to
 *                     the persistently stored default values. If the feature NvMRAM support is enabled,
 *                     all stored Dlt values in the NvM are deleted.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType:E_OK: Configuration has been reset successfully
 *                     E_NOT_OK: Configuration has not been reset
 */
FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_ResetToFactoryDefault(void)
{
    Std_ReturnType ret;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    ret = E_NOT_OK;
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else
#endif
    {
        Dlt_Init_From_Cfg(Dlt_ConfigPtr);
#if (DLT_NVM_RAM_SUPPORT == STD_ON)
        (void)Dlt_Store();
#endif
        ret = E_OK;
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               Switches on/off the message filtering functionality of the Dlt module.
 * ServiceId           0x1b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      status:TRUE: enable message filtering
 *                     FALSE: disable message filtering
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType:E_OK: No error occurred
 *                     E_NOT_OK: Setting of message filtering failed
 */
FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_SetMessageFiltering(boolean status)
{
    Std_ReturnType ret;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    ret = E_NOT_OK;
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else
#endif
    {
        SchM_Enter_Dlt_SwcContext();
        Dlt_RunTime.MessageFilterEnable = status;
        SchM_Exit_Dlt_SwcContext();
        ret = E_OK;
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               Called to modify the pass through range for Log Messages for all not explicit set ContextIds.
 * ServiceId           0x11
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      newLogLevel:sets the new filter value
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType:E_OK: No error occurred
 *                     E_NOT_OK: Default LogLevel could not be set
 */
FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_SetDefaultLogLevel(Dlt_MessageLogLevelType newLogLevel)
{
    Std_ReturnType ret;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    ret = E_NOT_OK;
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else
#endif
    {
        SchM_Enter_Dlt_SwcContext();
        Dlt_RunTime.DefaultLogLevel = newLogLevel;
        SchM_Exit_Dlt_SwcContext();
        ret = E_OK;
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               Called to enable or disable trace messages for all not explicitly set ContextIds.
 * ServiceId           0x12
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      newTraceStatus:enabling/disabling of Trace messages
 *                     logChannelName:Name of the addressed LogChannel
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType:E_OK: No error occurred
 *                     E_NOT_OK: Default Trace Status could not be set
 */
FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_SetDefaultTraceStatus(boolean newTraceStatus, Dlt_LogChannelNameType logChannelName)
{
    Std_ReturnType ret;
    DLT_UNUSED(logChannelName);
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    ret = E_NOT_OK;
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else
#endif
    {
        SchM_Enter_Dlt_SwcContext();
        Dlt_RunTime.DefaultTraceStatus = newTraceStatus;
        SchM_Exit_Dlt_SwcContext();
        ret = E_OK;
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               Returns the current Trace Status of the addressed LogChannel.
 * ServiceId           0x19
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      logChannelName:Name of the addressed LogChannel
 * Param-Name[out]     traceStatus:current trace status (enabled/disabled)
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType:E_OK: No error occurred
 *                     E_NOT_OK: Default Trace Status could not be returned
 */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_GetDefaultTraceStatus(Dlt_LogChannelNameType logChannelName, P2VAR(boolean, AUTOMATIC, DLT_APPL_DATA) traceStatus)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret;
    DLT_UNUSED(logChannelName);
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    ret = E_NOT_OK;
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else if (NULL_PTR == traceStatus)
    {
        DLT_DETREPORT(DLT_API_ID_GETDEFAULTTRACESTATUS, DLT_E_PARAM_POINTER);
    }
    else
#endif
    {
        *traceStatus = Dlt_RunTime.DefaultTraceStatus;
        ret = E_OK;
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               The caller provides the number of logChannelNames to be returned.
 *                     The function returns the requested amount of LogChannelNames and updates
 *                     numberOfLog Channels as the outgoing information on how many LogChannels are actually configured.
 * ServiceId           0x17
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     logChannelNames:Returns a list of configured LogChannel names
 * Param-Name[in/out]  numberOfLogChannels: Contains the number of requested LogChannels names. On Return it holds the
 * number of configured LogChannels Return              Std_ReturnType:E_OK: No error occurred E_NOT_OK:LogChannelNames
 * could not be returned
 */
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_GetLogChannelNames(
    P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) numberOfLogChannels,
    Dlt_LogChannelNameType* logChannelNames)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    ret = E_NOT_OK;
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else if ((NULL_PTR == logChannelNames) || (NULL_PTR == numberOfLogChannels))
    {
        DLT_DETREPORT(DLT_API_ID_GETLOGCHANNELNAMES, DLT_E_PARAM_POINTER);
    }
    else
#endif
    {
#if (DLT_CHANNEL_NUM > 0)
        *numberOfLogChannels = DLT_CHANNEL_NUM;
        uint16 index = 0;
#if (1u < DLT_CHANNEL_NUM)
        for (; index < DLT_CHANNEL_NUM; index++)
#endif /* 1u < DLT_CHANNEL_NUM */
        {
            /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
            const Dlt_LogChannelType* logChannelPtr =
                (Dlt_LogChannelType*)&(Dlt_ConfigPtr->LogOutput->LogChannel[index]);
            /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
            logChannelNames[index] = logChannelPtr->DltLogChannelId;
        }
#endif
        ret = E_OK;
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               Returns the current Trace Status for a given tuple ApplicationId/ContextId.
 * ServiceId           0x1f
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      appId:ApplicationId
 *                     contextId:ContextId
 * Param-Name[out]     traceStatus:current Trace Status of the tuple ApplicationId/ContextId
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType:E_OK: No error occurred
 *                     E_NOT_OK:TraceStatus could not be returned
 */
FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_GetTraceStatus(
    Dlt_ApplicationIDType appId,
    Dlt_ContextIDType contextId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(boolean, AUTOMATIC, DLT_APPL_DATA) traceStatus)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else if (traceStatus == NULL_PTR)
    {
        DLT_DETREPORT(DLT_API_ID_GETTRACESTATUS, DLT_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint16 SwcIndex = 0;
#if (1u < DLT_SWC_NUM)
        for (; (SwcIndex < DLT_SWC_NUM) && (ret == E_NOT_OK); SwcIndex++)
#endif /* 1u < DLT_SWC_NUM */
        {
#if ((DLT_SWC_NUM > 0) && (DLT_SWC_MAX_CONTEXT_NUM > 0))
            uint16 DltSwcContextNum = Dlt_RunTime.SwcInfo[SwcIndex].DltSwcContextNum;
            for (uint16 SwcContextIndex = 0; (SwcContextIndex < DltSwcContextNum) && (ret == E_NOT_OK);
                 SwcContextIndex++)
            {
                const Dlt_SwcContextInofType* SwcContextInfoPtr =
                    &(Dlt_RunTime.SwcInfo[SwcIndex].SwcContextInfo[SwcContextIndex]);
                Dlt_ApplicationIDType SwcApplicationId = SwcContextInfoPtr->SwcContext.SwcApplicationId;
                Dlt_ContextIDType SwcContextId = SwcContextInfoPtr->SwcContext.SwcContextId;
                if ((appId == SwcApplicationId) && (contextId == SwcContextId))
                {
                    *traceStatus = SwcContextInfoPtr->TraceStatus;
                    ret = E_OK;
                }
            }
#endif /* DLT_SWC_NUM > 0 && DLT_SWC_MAX_CONTEXT_NUM > 0 */
        }
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               Adds/removes the addressed tuple ApplicationId/ContextId to/from the addressed LogChannel.
 * ServiceId           0x20
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      appId:ID of the addressed application / SW-C
 *                     contextId:ID of the addressed context
 *                     logChannelName:Name of the addressed LogChannel
 *                     addRemoveOp:Operation to add/remove the addressed tuple ApplicationId/ContextId to/from the
 * addressed LogChannel Param-Name[out]     N/A Param-Name[in/out]  N/A Return              Std_ReturnType:E_OK: No
 * error occurred E_NOT_OK:LogChannel assignment failed
 */
FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_SetLogChannelAssignment(
    Dlt_ApplicationIDType appId,
    Dlt_ContextIDType contextId,
    Dlt_LogChannelNameType logChannelName,
    Dlt_AssignmentOperation addRemoveOp)
{
    Std_ReturnType ret;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    ret = E_NOT_OK;
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else
#endif
    {
        ret = Dlt_InterSetLogChannelAssignment(appId, contextId, logChannelName, addRemoveOp);
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               Sets the filter threshold for the given LogChannel.
 * ServiceId           0x21
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different LogChannelNames
 * Param-Name[in]      logChannelName:Name of the addressed LogChannel
 *                     newThreshold:Threshold for LogMessages
 *                     newTraceStatus:TRUE: enable TraceMessages FALSE: disable Trace Messages
 *                     addRemoveOp:Operation to add/remove the addressed tuple ApplicationId/ContextId to/from the
 * addressed LogChannel Param-Name[out]     N/A Param-Name[in/out]  N/A Return              Std_ReturnType:E_OK: No
 * error occurred E_NOT_OK:Setting of LogChannelThreshold failed
 */
FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_SetLogChannelThreshold(
    Dlt_LogChannelNameType logChannelName,
    Dlt_MessageLogLevelType newThreshold,
    boolean newTraceStatus)
{
    Std_ReturnType ret;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    ret = E_NOT_OK;
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else if (newThreshold > DLT_LOG_VERBOSE)
    {
        DLT_DETREPORT(DLT_API_ID_SETLOGCHANNELTHRESHOLD, DLT_E_PARAM);
    }
    else
#endif
    {
        uint16 ChannelIndex = Dlt_GetChannelIndexByChannelName(logChannelName);
#if (1u < DLT_CHANNEL_NUM)
        if (ChannelIndex != DLT_CHANNEL_NUM)
#endif /* 1u < DLT_CHANNEL_NUM */
        {
#if (DLT_CHANNEL_NUM > 0)
            Dlt_ChannelInfoType* ChannelInfoPtr = &(Dlt_RunTime.ChannelInfo[ChannelIndex]);
            SchM_Enter_Dlt_SwcContext();
            ChannelInfoPtr->DltLogChannelThreshold = newThreshold;
            ChannelInfoPtr->DltLogTraceStatusFlag = newTraceStatus;
            SchM_Exit_Dlt_SwcContext();
#endif
            ret = E_OK;
        }
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               Returns the filter threshold for the given LogChannel.
 * ServiceId           0x22
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different LogChannelNames
 * Param-Name[in]      logChannelName: Addressed LogChannel name
 * Param-Name[out]     logChannelThreshold:Current LogChannelThreshold
 *                     traceStatus:Current TraceStatus. TRUE: TraceMessages enabled.
 *                     FALSE: TraceMessages disabled.
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType:E_OK: No error occurred
 *                     E_NOT_OK:LogChannelThreshold could not be returned
 */
FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_GetLogChannelThreshold(
    Dlt_LogChannelNameType logChannelName,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dlt_MessageLogLevelType, AUTOMATIC, DLT_APPL_DATA) logChannelThreshold,
    P2VAR(boolean, AUTOMATIC, DLT_APPL_DATA) traceStatus)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    ret = E_NOT_OK;
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Note: If DLT is not initialized, there is no need to report error to DET */
    }
    else if ((NULL_PTR == traceStatus) || (NULL_PTR == logChannelThreshold))
    {
        DLT_DETREPORT(DLT_API_ID_SETLOGCHANNELTHRESHOLD, DLT_E_PARAM_POINTER);
    }
    else
#endif
    {
        /* The channel index number is extracted by the log channel name */
        uint16 ChannelIndex = Dlt_GetChannelIndexByChannelName(logChannelName);
        if (DLT_CHANNEL_NUM != ChannelIndex)
        {
#if (DLT_CHANNEL_NUM > 0)
            const Dlt_ChannelInfoType* ChannelInfoPtr = &(Dlt_RunTime.ChannelInfo[ChannelIndex]);
            *logChannelThreshold = ChannelInfoPtr->DltLogChannelThreshold;
            *traceStatus = ChannelInfoPtr->DltLogTraceStatusFlag;
#endif
            ret = E_OK;
        }
    }
    return ret;
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               Returns the filter threshold for the given LogChannel.
 * ServiceId           0x80
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 */
FUNC(void, DLT_APPL_CODE) Dlt_TxFunction(void)
{
    uint16 index = 0;
    /* Check if the DLT module has been initialized */
    if (DLT_STATE_INIT == Dlt_ModeState)
    {
#if ((DLT_RX_DATA_PATH_SUPPORT == STD_ON) && (DLT_BUFFER_MAX_LENGTH > 0) && (DLT_RXPDU_NUM > 0))
        /*if channel is busy deal the rx data in main*/
#if (1u < DLT_RXPDU_NUM)
        /* Traversal all the received PDU in the DLT module */
        for (; index < DLT_RXPDU_NUM; index++)
#endif /* 1u < DLT_RXPDU_NUM */
        {
            Dlt_RxStatusTypes* RxStatusPtr = &(Dlt_RxStatus[index]);
            /* Check if there are unprocessed messages in the receiving status information of the DLT module */
            if ((RxStatusPtr->Used == TRUE) && (RxStatusPtr->RxOffset == RxStatusPtr->RxLength))
            {
                /* The unprocessed received messages of the DLT module are refined */
                if (E_OK == Dlt_DealRxData(index, RxStatusPtr->RxBuffer, RxStatusPtr->RxLength))
                {
                    SchM_Enter_Dlt_RxStatus();
                    RxStatusPtr->Used = FALSE;
                    RxStatusPtr->NeedDeal = FALSE;
                    SchM_Exit_Dlt_RxStatus();
                }
            }
        }
#endif /* DLT_RX_DATA_PATH_SUPPORT == STD_ON */
        /* Check if the startup delay time of the DLT module has reached */
        if (Dlt_StartUpDelayTimer == 0u)
        {
            index = 0;
#if (1u < DLT_CHANNEL_NUM)
            for (; index < DLT_CHANNEL_NUM; index++)
#endif /* 1u < DLT_CHANNEL_NUM */
            {
#if (DLT_CHANNEL_NUM > 0)
                Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[index]);
                /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
                const Dlt_LogChannelType* logChannelPtr =
                    (Dlt_LogChannelType*)&(Dlt_ConfigPtr->LogOutput->LogChannel[index]);
                /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
                if ((ChannelPtr->OverFlowInfo.OverFlow == TRUE) && (ChannelPtr->OverFlowInfo.BufferOverflowTimer == 0u)
                    && (ChannelPtr->LastSendStatus == DLT_WAIT_SEND))
                {
                    uint16 logChannelBufOverflowTimer = logChannelPtr->DltLogChannelBufferOverflowTimer;
                    Dlt_SendOverFlow(index);
                    SchM_Enter_Dlt_MsgChannel();
                    ChannelPtr->OverFlowInfo.BufferOverflowTimer = logChannelBufOverflowTimer;
                    SchM_Exit_Dlt_MsgChannel();
                }
                else
                {
                    if (ChannelPtr->OverFlowInfo.BufferOverflowTimer > DLT_TASK_TIME)
                    {
                        SchM_Enter_Dlt_MsgChannel();
                        ChannelPtr->OverFlowInfo.BufferOverflowTimer -= DLT_TASK_TIME;
                        SchM_Exit_Dlt_MsgChannel();
                    }
                    else
                    {
                        SchM_Enter_Dlt_MsgChannel();
                        ChannelPtr->OverFlowInfo.BufferOverflowTimer = 0;
                        if (ChannelPtr->OverFlowInfo.OverFlowSend == TRUE)
                        {
                            ChannelPtr->OverFlowInfo.OverFlow = FALSE;
                            ChannelPtr->OverFlowInfo.OverFlowSend = FALSE;
                        }
                        SchM_Exit_Dlt_MsgChannel();
                    }
                    Dlt_Tx_Channel(index);
                }
#endif
            }
        }
        else
        {
            if (Dlt_StartUpDelayTimer <= DLT_TASK_TIME)
            {
                Dlt_StartUpDelayTimer = 0;
            }
            else
            {
                Dlt_StartUpDelayTimer -= DLT_TASK_TIME;
            }
        }
    }
}
#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(void, DLT_CODE) Dlt_Tx_CopyTxDataToSendBuffer(uint16 ChannelIndex, uint8* Dest, uint32 SendOffset, uint32 Length)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
#if (DLT_CHANNEL_NUM > 0 && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint32 Offset = SendOffset;
    uint32 logChannelBufferSize;
    const Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
    const Dlt_LogChannelType* logChannelPtr =
        (Dlt_LogChannelType*)&(Dlt_ConfigPtr->LogOutput->LogChannel[ChannelIndex]);
    /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
    for (uint32 Cnt = 0; Cnt < Length; Cnt++)
    {
        Dest[Cnt] = ChannelPtr->buffer[Offset]; /* PRQA S 0311 */ /* MISRA Rule 11.8 */
        logChannelBufferSize = logChannelPtr->DltLogChannelBufferSize;
        if ((Offset + 1u) == logChannelBufferSize)
        {
            Offset = 0;
        }
        else
        {
            Offset += 1u;
        }
    }
#endif
}

static FUNC(void, DLT_CODE) Dlt_Tx_Channel(uint16 ChannelIndex)
{
    uint32 SendOffset;
    PduInfoType PduInfoPtr;
#if (DLT_IF_TRANSMIT == STD_ON)
    uint8 Dlt_IfTransmitBuffer[DLT_BUFFER_MAX_LENGTH];
    boolean TxPduUsesTp;
#endif
#if (DLT_CHANNEL_NUM > 0)
    /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
    const Dlt_LogChannelType* logChannelPtr =
        (Dlt_LogChannelType*)&(Dlt_ConfigPtr->LogOutput->LogChannel[ChannelIndex]);
    /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
    uint32 logChannelBufferSize = logChannelPtr->DltLogChannelBufferSize;
    PduIdType TxPduId = logChannelPtr->DltTxPdu->DltTxPduRPduId;
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    if (ChannelPtr->LastSendStatus == DLT_SEND_NEED_RETRY)
    {
        uint8 logChannelMaxNumOfRetries = logChannelPtr->DltLogChannelMaxNumOfRetries;
        if (ChannelPtr->SendCounter < logChannelMaxNumOfRetries)
        {
            SendOffset = ChannelPtr->ReadIndex;
            PduInfoPtr.SduLength = (uint16)ChannelPtr->LastSendLength;
#if (DLT_IF_TRANSMIT == STD_ON)
            TxPduUsesTp = logChannelPtr->DltTxPdu->DltTxPduUsesTp;
            if (TxPduUsesTp == TRUE)
            {
                PduInfoPtr.SduDataPtr = NULL_PTR;
            }
            else
            {
                SchM_Enter_Dlt_MsgChannel();
                Dlt_Tx_CopyTxDataToSendBuffer(ChannelIndex, Dlt_IfTransmitBuffer, SendOffset, PduInfoPtr.SduLength);
                SchM_Exit_Dlt_MsgChannel();
                PduInfoPtr.SduDataPtr = Dlt_IfTransmitBuffer;
            }
#else
            PduInfoPtr.SduDataPtr = NULL_PTR;
#endif
            if (PduR_DltTransmit(TxPduId, &PduInfoPtr) == E_NOT_OK)
            {
                SchM_Enter_Dlt_MsgChannel();
                Dlt_Tx_RemoveBuffer(ChannelIndex, SendOffset, (uint32)PduInfoPtr.SduLength);
                ChannelPtr->LastSendStatus = DLT_WAIT_SEND;
                SchM_Exit_Dlt_MsgChannel();
            }
            else
            {
                SchM_Enter_Dlt_MsgChannel();
                Dlt_MemSet((uint8*)ChannelPtr, 0u, sizeof(Dlt_ChannelType));
                ChannelPtr->SendCounter++;
                ChannelPtr->LastSendStatus = DLT_SEND_NOT_CONFIRMATION;
                ChannelPtr->MessageSendOffset = 0;
                SchM_Exit_Dlt_MsgChannel();
            }
        }
        else
        {
            SchM_Enter_Dlt_MsgChannel();
            ChannelPtr->SendCounter = 0;
            ChannelPtr->UnusedLength = logChannelBufferSize;
            Dlt_Tx_RemoveBuffer(ChannelIndex, ChannelPtr->ReadIndex, ChannelPtr->LastSendLength);
            ChannelPtr->LastSendStatus = DLT_WAIT_SEND;
            ChannelPtr->ReadIndex = 0;
            ChannelPtr->WriteIndex = 0;
            ChannelPtr->MessageSendOffset = 0;
            ChannelPtr->LastSendLength = 0;
            SchM_Exit_Dlt_MsgChannel();
        }
    }
    else
    {
        uint32 NeedSendLength = logChannelBufferSize - ChannelPtr->UnusedLength;
#if (DLT_TRAFFIC_SHAPING_SUPPORT == STD_ON)
        uint32 logChannelTrafficShapingBandwidth = logChannelPtr->DltLogChannelTrafficShapingBandwidth;
        if (logChannelTrafficShapingBandwidth < NeedSendLength)
        {
            NeedSendLength = logChannelTrafficShapingBandwidth;
        }
#endif
        if ((NeedSendLength > 0u) && (ChannelPtr->LastSendStatus == DLT_WAIT_SEND))
        {
            SchM_Enter_Dlt_MsgChannel();
            SendOffset = ChannelPtr->ReadIndex;
            ChannelPtr->LastSendLength = NeedSendLength;
            SchM_Exit_Dlt_MsgChannel();
            PduInfoPtr.SduLength = (uint16)NeedSendLength;
#if (DLT_IF_TRANSMIT == STD_ON)
            TxPduUsesTp = logChannelPtr->DltTxPdu->DltTxPduUsesTp;
            if (TxPduUsesTp == TRUE)
            {
                PduInfoPtr.SduDataPtr = NULL_PTR;
            }
            else
            {
                SchM_Enter_Dlt_MsgChannel();
                Dlt_Tx_CopyTxDataToSendBuffer(ChannelIndex, Dlt_IfTransmitBuffer, SendOffset, PduInfoPtr.SduLength);
                SchM_Exit_Dlt_MsgChannel();
                PduInfoPtr.SduDataPtr = Dlt_IfTransmitBuffer;
            }
#else
            PduInfoPtr.SduDataPtr = NULL_PTR;
#endif
            if (PduR_DltTransmit(TxPduId, &PduInfoPtr) == E_NOT_OK)
            {
                SchM_Enter_Dlt_MsgChannel();
                Dlt_Tx_RemoveBuffer(ChannelIndex, SendOffset, (uint32)PduInfoPtr.SduLength);
                ChannelPtr->LastSendStatus = DLT_WAIT_SEND;
                SchM_Exit_Dlt_MsgChannel();
            }
            else
            {
                SchM_Enter_Dlt_MsgChannel();
                ChannelPtr->SendCounter++;
                ChannelPtr->LastSendStatus = DLT_SEND_NOT_CONFIRMATION;
                ChannelPtr->MessageSendOffset = 0;
                SchM_Exit_Dlt_MsgChannel();
            }
        }
    }
#endif
}

FUNC(void, DLT_CODE) Dlt_Init_From_Cfg(const Dlt_ConfigType* config)
{
    Dlt_RunTimeType* RunTimePtr = &Dlt_RunTime;
    SchM_Enter_Dlt_SwcContext();
    RunTimePtr->MessageFilterEnable = TRUE;
    RunTimePtr->MessageFilterPass = DLT_MESSAGE_FILTER;
    RunTimePtr->DefaultLogLevel = config->LogLevelSetting->DltDefaultLogLevel;
    RunTimePtr->DefaultTraceStatus = config->TraceStatusSetting->DltDefaultTraceStatus;
    SchM_Exit_Dlt_SwcContext();
    uint16 index = 0;
#if (1u < DLT_CHANNEL_NUM)
    for (; index < DLT_CHANNEL_NUM; index++)
#endif /* 1u < DLT_CHANNEL_NUM */
    {
#if (DLT_CHANNEL_NUM > 0)
        Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[index]);
        Dlt_ChannelInfoType* ChannelInfoPtr = &(Dlt_RunTime.ChannelInfo[index]);
        uint32 logChannelBufferSize = config->LogOutput->LogChannel[index].DltLogChannelBufferSize;
        boolean logTraceStatusFlag = config->LogOutput->LogChannel[index].DltLogTraceStatusFlag;
        Dlt_MessageLogLevelType logChannelThreshold = config->LogOutput->LogChannel[index].DltLogChannelThreshold;
        SchM_Enter_Dlt_SwcContext();
        ChannelInfoPtr->DltLogChannelThreshold = logChannelThreshold;
        ChannelInfoPtr->DltLogTraceStatusFlag = logTraceStatusFlag;
        ChannelPtr->UnusedLength = logChannelBufferSize;
        ChannelPtr->LastSendStatus = DLT_WAIT_SEND;
        SchM_Exit_Dlt_SwcContext();
#endif
    }
    index = 0;
#if (1u < DLT_SWC_NUM)
    for (; index < DLT_SWC_NUM; index++)
#endif /* 1u < DLT_SWC_NUM */
    {
#if ((DLT_SWC_NUM > 0) && (DLT_SWC_MAX_CONTEXT_NUM > 0))
        const Dlt_SwcTypes* SwcPtr = &(Dlt_Swc.DltSwc[index]);
        Dlt_SwcInfoType* SwcInfoPtr = &(Dlt_RunTime.SwcInfo[index]);

        SchM_Enter_Dlt_SwcContext();
        SwcInfoPtr->DltSwcSessionId = SwcPtr->DltSwcSessionId;
        SwcInfoPtr->DltSwcContextNum = SwcPtr->DltSwcContextNum;
        SchM_Exit_Dlt_SwcContext();
        for (uint16 iloop = 0; iloop < SwcInfoPtr->DltSwcContextNum; iloop++)
        {
            /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
            const Dlt_SwcContextType* SwcContextPtr = (Dlt_SwcContextType*)&(SwcPtr->DltSwcContext[iloop]);
            /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
            Dlt_SwcContextInofType* SwcContextInfoPtr = &(SwcInfoPtr->SwcContextInfo[iloop]);
            SchM_Enter_Dlt_SwcContext();
            SwcContextInfoPtr->SwcContext.SwcApplicationId = SwcContextPtr->SwcApplicationId;
            SwcContextInfoPtr->SwcContext.SwcContextId = SwcContextPtr->SwcContextId;
            SwcContextInfoPtr->Register = FALSE;
#if (DLT_DET_FORWARD_TO_DET_SUPPORT == STD_ON)
            if ((SwcContextPtr->SwcApplicationId == Dlt_Det_ApplId)
                && (SwcContextPtr->SwcContextId == Dlt_Det_ContextId))
            {
                /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
#if (DLT_DET_APPLID_DESC_LENGTH > 0)
                SwcContextInfoPtr->lenAppDescription = DLT_DET_APPLID_DESC_LENGTH;
                SwcContextInfoPtr->appDescription = (uint8*)Dlt_Det_ApplId_Desc;
#else
                SwcContextInfoPtr->lenAppDescription = 0u;
                SwcContextInfoPtr->appDescription = NULL_PTR;
#endif
#if (DLT_DET_CONTEXTID_DESC_LENGTH > 0)
                SwcContextInfoPtr->lenContextDescription = DLT_DET_CONTEXTID_DESC_LENGTH;
                SwcContextInfoPtr->contextDescription = (uint8*)Dlt_Det_ContextId_Desc;
#else
                SwcContextInfoPtr->lenContextDescription = 0u;
                SwcContextInfoPtr->contextDescription = NULL_PTR;
#endif
                /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
            }
            else
            {
                SwcContextInfoPtr->lenAppDescription = 0;
                SwcContextInfoPtr->lenContextDescription = 0;
                SwcContextInfoPtr->appDescription = NULL_PTR;
                SwcContextInfoPtr->contextDescription = NULL_PTR;
            }
#endif
            SchM_Exit_Dlt_SwcContext();
            uint16 cnt = 0;
#if (1u < DLT_TRACE_STATUS_ASSIGNMENT_NUM)
            for (; cnt < DLT_TRACE_STATUS_ASSIGNMENT_NUM; cnt++)
#endif /* 1u < DLT_TRACE_STATUS_ASSIGNMENT_NUM */
            {
                /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
                const Dlt_TraceStatusAssignmentTypes* traceStatusAssignmentPtr =
                    (Dlt_TraceStatusAssignmentTypes*)&(config->TraceStatusSetting->DltTraceStatusAssignment[cnt]);
                /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
                Dlt_ApplicationIDType SwcApplicationId =
                    traceStatusAssignmentPtr->DltTraceStatusAssignmentSwcContextRef->SwcApplicationId;
                Dlt_ContextIDType SwcContextId =
                    traceStatusAssignmentPtr->DltTraceStatusAssignmentSwcContextRef->SwcContextId;
                if ((SwcApplicationId == SwcContextInfoPtr->SwcContext.SwcApplicationId)
                    && (SwcContextId == SwcContextInfoPtr->SwcContext.SwcContextId))
                {
                    SchM_Enter_Dlt_SwcContext();
                    SwcContextInfoPtr->TraceStatus = traceStatusAssignmentPtr->DltTraceStatus;
                    SchM_Exit_Dlt_SwcContext();
#if (1u < DLT_TRACE_STATUS_ASSIGNMENT_NUM)
                    break;
#endif /* 1u < DLT_TRACE_STATUS_ASSIGNMENT_NUM */
                }
            }
            cnt = 0;
#if (1u < DLT_LOG_LEVEL_THRESHOLD_NUM)
            for (; cnt < DLT_LOG_LEVEL_THRESHOLD_NUM; cnt++)
#endif /* 1u < DLT_LOG_LEVEL_THRESHOLD_NUM */
            {
                /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
                const Dlt_LogLevelThesholdType* logLevelThresholdPtr =
                    (Dlt_LogLevelThesholdType*)&(config->LogLevelSetting->DltLogLevelThreshold[cnt]);
                /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
                Dlt_ApplicationIDType SwcApplicationId =
                    logLevelThresholdPtr->DltLogLevelThresholdSwcContextRef->SwcApplicationId;
                Dlt_ContextIDType SwcContextId = logLevelThresholdPtr->DltLogLevelThresholdSwcContextRef->SwcContextId;
                if ((SwcApplicationId == SwcContextInfoPtr->SwcContext.SwcApplicationId)
                    && (SwcContextId == SwcContextInfoPtr->SwcContext.SwcContextId))
                {
                    SchM_Enter_Dlt_SwcContext();
                    SwcContextInfoPtr->DltThreshold = logLevelThresholdPtr->DltThreshold;
                    SchM_Exit_Dlt_SwcContext();
#if (1u < DLT_LOG_LEVEL_THRESHOLD_NUM)
                    break;
#endif /* 1u < DLT_LOG_LEVEL_THRESHOLD_NUM */
                }
            }
            SwcContextInfoPtr->DltLogChannelRefNum = 0;
            cnt = 0;
#if (1u < DLT_LOG_CHANNEL_ASSIGNMENT_NUM)
            for (; cnt < DLT_LOG_CHANNEL_ASSIGNMENT_NUM; cnt++)
#endif /* 1u < DLT_LOG_CHANNEL_ASSIGNMENT_NUM */
            {
                /* PRQA S 0311++ */ /* MISRA Rule 11.18 */
                const Dlt_LogChannelAssignmentType* logChannelAssignmentPtr =
                    (Dlt_LogChannelAssignmentType*)&(config->LogOutput->LogChannelAssignment[cnt]);
                Dlt_ApplicationIDType SwcApplicationId =
                    logChannelAssignmentPtr->DltLogChannelAssignmentSwcContextRef->SwcApplicationId;
                Dlt_ContextIDType SwcContextId =
                    logChannelAssignmentPtr->DltLogChannelAssignmentSwcContextRef->SwcContextId;
                /* PRQA S 0311-- */ /* MISRA Rule 11.18 */
                if ((SwcApplicationId == SwcContextInfoPtr->SwcContext.SwcApplicationId)
                    && (SwcContextId == SwcContextInfoPtr->SwcContext.SwcContextId))
                {
                    SchM_Enter_Dlt_SwcContext();
                    SwcContextInfoPtr->DltLogChannelRef[SwcContextInfoPtr->DltLogChannelRefNum] =
                        logChannelAssignmentPtr->DltLogChannelRef;
                    SwcContextInfoPtr->DltLogChannelRefNum++;
                    SchM_Exit_Dlt_SwcContext();
#if (1u < DLT_LOG_CHANNEL_ASSIGNMENT_NUM)
                    break;
#endif /* 1u < DLT_LOG_CHANNEL_ASSIGNMENT_NUM */
                }
            }
        }
#endif /* DLT_SWC_NUM > 0 && DLT_SWC_MAX_CONTEXT_NUM > 0 */
    }
}

FUNC(void, DLT_CODE) Dlt_MemCopy(uint8* Dest, const uint8* Src, uint32 Size)
{
    uint32 cnt = Size;
    while (cnt > 0u)
    {
        cnt--;
        Dest[cnt] = Src[cnt];
    }
}

#if (DLT_NVM_RAM_SUPPORT == STD_ON)
FUNC(Std_ReturnType, DLT_CODE) Dlt_Store(void)
{
    Dlt_MemCopy(
        NvM_BlockDescriptor[DLT_NVM_RAM_REF - 1u].NvmRamBlockDataAddress,
        (const uint8*)&Dlt_RunTime,
        sizeof(Dlt_RunTimeType));
    return NvM_WriteBlock(DLT_NVM_RAM_REF, NULL_PTR);
}
#endif

/*this return value is in 0.1 MS*/
FUNC(uint32, DLT_CODE) Dlt_GetTimeElapsed(void)
{
    uint32 Timer;
#if (DLT_TIME_STAMP_SUPPORT == STD_ON)
#if (DLT_GPT_SUPPORT == STD_ON)
    Timer = (uint32)(Gpt_GetTimeElapsed(DLT_TIME_REF) / DLT_GPT_TICK_TIME);
#else
    StbM_UserDataType userData;
    StbM_TimeStampType TimeStamp;
    if (E_OK == StbM_GetCurrentTime(DLT_TIME_REF, &TimeStamp, &userData))
    {
        uint32 nanoseconds = TimeStamp.nanoseconds - Dlt_TimeStamp.nanoseconds;
        uint32 seconds = TimeStamp.seconds - Dlt_TimeStamp.seconds;
        Timer = (uint32)((nanoseconds) / 100000u) + (uint32)((seconds) * 100000u);
    }
    else
    {
        Timer = 0;
    }
#endif
#else
    Timer = 0;
#endif
    return Timer;
}

FUNC(void, DLT_CODE)
Dlt_GetChannelIndex(
    uint16 SwcIndex,
    Dlt_ApplicationIDType app_id,
    Dlt_ContextIDType context_id,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DLT_APPL_DATA) ChannelIndexList)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 Cnt = 0;
    /*To find the matching LogChannels for the ApplicationId/ContextId tuple contained
     * in a Dlt_SendLogMessage or Dlt_SendTraceMessage call, the Dlt module shall do the following steps*/
#if ((DLT_SWC_NUM > 0) && (DLT_SWC_MAX_CONTEXT_NUM > 0))
    const Dlt_SwcContextInofType* SwcContextInfoPtr;
    /*1. From all mapping elements, where ApplicationId/ContextId tuple of mapping element equals
     * to the ApplicationId/ContextId tuple of the log/trace message, the LogChannel
     * shall be added to the list of output LogChannels.*/
    uint16 SwcContextNum = Dlt_RunTime.SwcInfo[SwcIndex].DltSwcContextNum;
    for (uint16 iloop = 0U; iloop < SwcContextNum; iloop++)
    {
        SwcContextInfoPtr = &(Dlt_RunTime.SwcInfo[SwcIndex].SwcContextInfo[iloop]);
        Dlt_ApplicationIDType SwcApplicationId = SwcContextInfoPtr->SwcContext.SwcApplicationId;
        Dlt_ContextIDType SwcContextId = SwcContextInfoPtr->SwcContext.SwcContextId;
        if ((app_id == SwcApplicationId) && (context_id == SwcContextId))
        {
            for (uint16 index = 0U; index < SwcContextInfoPtr->DltLogChannelRefNum; index++)
            {
                ChannelIndexList[Cnt] = SwcContextInfoPtr->DltLogChannelRef[index];
                Cnt++;
            }
        }
    }
    /*2. From all mapping elements, where ApplicationID of mapping element equals to the ApplicationID
     * of the log/trace message AND the ContextId of mapping element equals wildcard (value 0x00000000),
     *  the LogChannel shall be added to the list of output LogChannels.*/
    for (uint16 iloop = 0U; iloop < Dlt_RunTime.SwcInfo[SwcIndex].DltSwcContextNum; iloop++)
    {
        SwcContextInfoPtr = &(Dlt_RunTime.SwcInfo[SwcIndex].SwcContextInfo[iloop]);
        Dlt_ApplicationIDType SwcApplicationId = SwcContextInfoPtr->SwcContext.SwcApplicationId;
        if ((app_id == SwcApplicationId) && (0u == context_id))
        {
            for (uint16 index = 0U; index < SwcContextInfoPtr->DltLogChannelRefNum; index++)
            {
                ChannelIndexList[Cnt] = SwcContextInfoPtr->DltLogChannelRef[index];
                Cnt++;
            }
        }
    }
#endif /* DLT_SWC_NUM > 0 && DLT_SWC_MAX_CONTEXT_NUM > 0 */
    /*3. If the list of output LogChannels is still empty after step 1 and 2.
     * The default LogChannel (see configuration parameter DltDefaultLogChannelRef)
     * shall be added to the list of output LogChannels.*/
    if (Cnt == 0u)
    {
        ChannelIndexList[0] = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;
    }
}

FUNC(uint16, DLT_CODE) Dlt_GetChannelIndexByChannelName(Dlt_LogChannelNameType DltLogChannelId)
{
    uint16 retVal = DLT_CHANNEL_NUM;
    uint16 iloop = 0;
#if (1u < DLT_CHANNEL_NUM)
    /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
    for (; iloop < DLT_CHANNEL_NUM; iloop++)
    /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
#endif                  /* 1u < DLT_CHANNEL_NUM */
    {
#if (DLT_CHANNEL_NUM > 0)
        /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
        const Dlt_LogChannelType* logChannelPtr = (Dlt_LogChannelType*)&(Dlt_ConfigPtr->LogOutput->LogChannel[iloop]);
        /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
        if (logChannelPtr->DltLogChannelId == DltLogChannelId)
        {
            retVal = iloop;
#if (1u < DLT_CHANNEL_NUM)
            break;
#endif /* 1u < DLT_CHANNEL_NUM */
        }
#endif
    }
    return retVal;
}

FUNC(uint16, DLT_CODE) Dlt_GetChannelIndexByTxPduId(PduIdType id) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint16 retVal = DLT_CHANNEL_NUM;
    uint16 iloop = 0;
#if (1u < DLT_CHANNEL_NUM)
    for (; iloop < DLT_CHANNEL_NUM; iloop++)
#endif /* 1u < DLT_CHANNEL_NUM */
    {
#if (DLT_CHANNEL_NUM > 0)
        /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
        const Dlt_LogChannelType* logChannelPtr = (Dlt_LogChannelType*)&(Dlt_ConfigPtr->LogOutput->LogChannel[iloop]);
        /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
        if (logChannelPtr->DltTxPdu->DltTxHandlePduId == id)
        {
            retVal = iloop;
#if (1u < DLT_CHANNEL_NUM)
            break;
#endif /* 1u < DLT_CHANNEL_NUM */
        }
#endif
    }
    return retVal;
}

static FUNC(Dlt_MessageLogLevelType, DLT_CODE)
    Dlt_GetSwcLogLevel(uint16 SwcIndex, P2CONST(Dlt_MessageLogInfoType, AUTOMATIC, DLT_APPL_CONST) logInfo)
{
    Dlt_MessageLogLevelType ret = Dlt_RunTime.DefaultLogLevel;
    /*To find the matching LogChannels for the ApplicationId/ContextId tuple contained
     * in a Dlt_SendLogMessage or Dlt_SendTraceMessage call, the Dlt module shall do the following steps*/
#if ((DLT_SWC_NUM > 0) && (DLT_SWC_MAX_CONTEXT_NUM > 0))
    boolean Find = FALSE;
    const Dlt_SwcInfoType* SwcInfoPtr = &(Dlt_RunTime.SwcInfo[SwcIndex]);
    const Dlt_SwcContextInofType* SwcContextInfoPtr;
    uint16 SwcContextNum = SwcInfoPtr->DltSwcContextNum;
    Dlt_ApplicationIDType SwcApplicationId;
    Dlt_ContextIDType SwcContextId;
    /*1. Check whether a mapping element exists, where ApplicationId/ContextId tuple of mapping element
     * equals to the ApplicationId/ContextId tuple of the log/trace message. If such a mapping element
     * exists, the matching mapping element is found.*/
    for (uint16 iloop = 0U; iloop < SwcContextNum; iloop++)
    {
        SwcContextInfoPtr = &(SwcInfoPtr->SwcContextInfo[iloop]);
        SwcApplicationId = SwcContextInfoPtr->SwcContext.SwcApplicationId;
        SwcContextId = SwcContextInfoPtr->SwcContext.SwcContextId;
        if ((logInfo->appId == SwcApplicationId) && (logInfo->contextId == SwcContextId))
        {
            ret = SwcContextInfoPtr->DltThreshold;
            Find = TRUE;
            break;
        }
    }
    /*2. In case no match has been found in step 1, check whether a mapping element exists, where the
     * ApplicationID equals the ApplicationID of the log/trace message and the ContextId of mapping
     * element equals wildcard (value 0x00000000). If such a mapping element exists,
     * the matching mapping element is found.*/
    for (uint16 iloop = 0; (Find == FALSE) && (iloop < SwcContextNum); iloop++)
    {
        SwcContextInfoPtr = &(SwcInfoPtr->SwcContextInfo[iloop]);
        SwcApplicationId = SwcContextInfoPtr->SwcContext.SwcApplicationId;
        if ((logInfo->appId == SwcApplicationId) && (logInfo->contextId == 0u))
        {
            ret = SwcContextInfoPtr->DltThreshold;
            Find = TRUE;
        }
    }
#endif /* DLT_SWC_NUM > 0 && DLT_SWC_MAX_CONTEXT_NUM > 0 */
    return ret;
}

static FUNC(boolean, DLT_CODE)
    Dlt_GetSwcTraceStatus(uint16 SwcIndex, P2CONST(Dlt_MessageTraceInfoType, AUTOMATIC, DLT_APPL_CONST) traceInfo)
{
    boolean ret = Dlt_RunTime.DefaultTraceStatus;
    /*To find the matching LogChannels for the ApplicationId/ContextId tuple contained
     * in a Dlt_SendLogMessage or Dlt_SendTraceMessage call, the Dlt module shall do the following steps*/
#if ((DLT_SWC_NUM > 0) && (DLT_SWC_MAX_CONTEXT_NUM > 0))
    boolean Find = FALSE;
    const Dlt_SwcInfoType* SwcInfoPtr = &(Dlt_RunTime.SwcInfo[SwcIndex]);
    const Dlt_SwcContextInofType* SwcContextInfoPtr;
    uint16 SwcContextNum = SwcInfoPtr->DltSwcContextNum;
    Dlt_ApplicationIDType SwcApplicationId;
    Dlt_ContextIDType SwcContextId;
    /*1. Check whether a mapping element exists, where ApplicationId/ContextId tuple of mapping element
     * equals to the ApplicationId/ContextId tuple of the log/trace message. If such a mapping element
     * exists, the matching mapping element is found.*/
    for (uint16 iloop = 0U; iloop < SwcContextNum; iloop++)
    {
        SwcContextInfoPtr = &(SwcInfoPtr->SwcContextInfo[iloop]);
        SwcApplicationId = SwcContextInfoPtr->SwcContext.SwcApplicationId;
        SwcContextId = SwcContextInfoPtr->SwcContext.SwcContextId;
        if ((traceInfo->appId == SwcApplicationId) && (traceInfo->contextId == SwcContextId))
        {
            ret = SwcContextInfoPtr->TraceStatus;
            Find = TRUE;
            break;
        }
    }
    /*2. In case no match has been found in step 1, check whether a mapping element exists, where the
     * ApplicationID equals the ApplicationID of the log/trace message and the ContextId of mapping
     * element equals wildcard (value 0x00000000). If such a mapping element exists,
     * the matching mapping element is found.*/
    for (uint16 iloop = 0U; (Find == FALSE) && (iloop < SwcContextNum); iloop++)
    {
        SwcContextInfoPtr = &(SwcInfoPtr->SwcContextInfo[iloop]);
        SwcApplicationId = SwcContextInfoPtr->SwcContext.SwcApplicationId;
        if ((traceInfo->appId == SwcApplicationId) && (0u == traceInfo->contextId))
        {
            ret = SwcContextInfoPtr->TraceStatus;
            Find = TRUE;
        }
    }
#endif /* DLT_SWC_NUM > 0 && DLT_SWC_MAX_CONTEXT_NUM > 0 */
    return ret;
}

static FUNC(boolean, DLT_CODE) Dlt_GetChannelTraceStatus(uint16 ChannelIndex)
{
    boolean retVal = FALSE;
#if (DLT_CHANNEL_NUM > 0)
    retVal = Dlt_RunTime.ChannelInfo[ChannelIndex].DltLogTraceStatusFlag;
#endif
    return retVal;
}

static FUNC(Dlt_MessageLogLevelType, DLT_CODE) Dlt_GetChannelLogLevel(uint16 ChannelIndex)
{
    Dlt_MessageLogLevelType retVal = DLT_LOG_OFF;
#if (DLT_CHANNEL_NUM > 0)
    retVal = Dlt_RunTime.ChannelInfo[ChannelIndex].DltLogChannelThreshold;
#endif
    return retVal;
}

FUNC(uint16, DLT_CODE) Dlt_GetSwcSessionIdIndex(Dlt_SessionIDType sessionId)
{
    uint16 index = 0U;

#if (1u < DLT_SWC_NUM)
    for (; index < DLT_SWC_NUM; index++)
#endif /* 1u < DLT_SWC_NUM */
    {
#if (DLT_SWC_NUM > 0)
        if (sessionId == Dlt_RunTime.SwcInfo[index].DltSwcSessionId)
        {
#if (1u < DLT_SWC_NUM)
            break;
#endif /* 1u < DLT_SWC_NUM */
        }
#endif /* (DLT_SWC_NUM > 0 */
    }
    return index;
}

static FUNC(void, DLT_CODE) Dlt_MoveWriteIndexCopy(uint16 ChannelIndex, uint8 value)
{
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    uint32 logChannelBufferSize = Dlt_ConfigPtr->LogOutput->LogChannel[ChannelIndex].DltLogChannelBufferSize;
    ChannelPtr->buffer[ChannelPtr->WriteIndex] = value;
    if ((ChannelPtr->WriteIndex + (uint32)1) >= logChannelBufferSize)
    {
        ChannelPtr->WriteIndex = 0u;
    }
    else
    {
        ChannelPtr->WriteIndex += 1u;
    }
#endif
}

FUNC(void, DLT_CODE) Dlt_MoveReadIndex(uint16 ChannelIndex, uint32 length) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (DLT_CHANNEL_NUM > 0)
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    uint32 logChannelBufferSize = Dlt_ConfigPtr->LogOutput->LogChannel[ChannelIndex].DltLogChannelBufferSize;
    for (uint32 iloop = 0; iloop < length; iloop++)
    {
        if ((ChannelPtr->ReadIndex + (uint32)1) >= logChannelBufferSize)
        {
            ChannelPtr->ReadIndex = 0u;
        }
        else
        {
            ChannelPtr->ReadIndex += 1u;
        }
    }
#endif
}

static FUNC(boolean, DLT_CODE) Dlt_CompareTraceStatus(boolean CurrentLogLevel)
{
    boolean ret = CurrentLogLevel;
    if (Dlt_RunTime.MessageFilterEnable == FALSE)
    {
        ret = TRUE;
    }
    return ret;
}

static FUNC(boolean, DLT_CODE)
    Dlt_CompareLogLevel(Dlt_MessageLogLevelType CurrentLogLevel, Dlt_MessageLogLevelType MessageLogLevel)
{
    boolean ret = FALSE;
    const Dlt_RunTimeType* RunTimePtr = &Dlt_RunTime;
    if ((RunTimePtr->MessageFilterEnable == FALSE) || (RunTimePtr->MessageFilterPass == DLT_MESSAGE_PASS)
        || ((RunTimePtr->MessageFilterPass != DLT_MESSAGE_BLOCKD)
            && ((CurrentLogLevel != DLT_LOG_OFF) && (MessageLogLevel != DLT_LOG_OFF)
                && (CurrentLogLevel >= MessageLogLevel))))
    {
        ret = TRUE;
    }
    return ret;
}

FUNC(uint32, DLT_CODE) Dlt_GetEcuId(void)
{
    uint32 EcuId = 0;
    if ((Dlt_ConfigPtr->Protocol != NULL_PTR) && (Dlt_ConfigPtr->Protocol->DltEcuId != NULL_PTR))
    {
        Dlt_EcuIdCalloutTypes DltEcuIdCallout = Dlt_ConfigPtr->Protocol->DltEcuId->DltEcuIdCallout;
        if (DltEcuIdCallout != NULL_PTR)
        {
            EcuId = DltEcuIdCallout();
        }
        else
        {
            EcuId = Dlt_ConfigPtr->Protocol->DltEcuId->DltEcuIdValue;
        }
    }
    return EcuId;
}

uint16 Dlt_GetMessageLength(boolean VerboseMode, uint16 PayloadLength)
{
    uint16 Headerlength;
    uint16 messageLength;
    /* Check if the DLT use to verbose mode */
    if (FALSE == VerboseMode)
    {
        /* Get the number of bytes in the message header */
        Headerlength = DLT_STANDARD_HEADER_LENGTH;
    }
    else
    {
        /* Get the number of bytes in the message header */
        Headerlength = DLT_STANDARD_HEADER_LENGTH + DLT_EXTENED_HEADER_LENGTH;
    }
    /* Get the total number of bytes of the message */
    messageLength = Headerlength + PayloadLength;
    return messageLength;
}

/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,Rule 20.7 */
FUNC(void, DLT_CODE)
Dlt_CopyArrayToLongLittleEndian(
    P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData,
    P2CONST(uint8, AUTOMATIC, DLT_CONST) SourceData)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,Rule 20.7 */
{
    (*DestData) = (uint32)((uint32)(SourceData[3]) << 24u);
    (*DestData) |= (uint32)((uint32)(SourceData[2]) << 16u);
    (*DestData) |= (uint32)((uint32)(SourceData[1]) << 8u);
    (*DestData) |= (uint32)((uint32)(SourceData[0]));
}

#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(void, DLT_CODE) Dlt_CopyLongToArrayLittleEndian(uint32 SourceData, P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    DestData[0] = (uint8)(SourceData);
    DestData[1] = (uint8)(SourceData >> 8u);
    DestData[2] = (uint8)(SourceData >> 16u);
    DestData[3] = (uint8)(SourceData >> 24u);
}

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
FUNC(void, DLT_CODE) Dlt_CopyIntToArrayLittleEndian(uint16 SourceData, P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    DestData[0] = (uint8)(SourceData);
    DestData[1] = (uint8)(SourceData >> 8u);
}
#endif

/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,Rule 20.7 */
FUNC(void, DLT_CODE) Dlt_CopyLongToArrayBigEndian(uint32 SourceData, P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,Rule 20.7 */
{
    DestData[0] = (uint8)(SourceData >> 24u);
    DestData[1] = (uint8)(SourceData >> 16u);
    DestData[2] = (uint8)(SourceData >> 8u);
    DestData[3] = (uint8)(SourceData);
}

/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,Rule 20.7 */
FUNC(void, DLT_CODE) Dlt_CopyIntToArrayBigEndian(uint16 SourceData, P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,Rule 20.7 */
{
    DestData[0] = (uint8)(SourceData >> 8u);
    DestData[1] = (uint8)(SourceData);
}

/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,Rule 20.7 */
FUNC(void, DLT_CODE)
Dlt_CopyArrayToLongBigEndian(
    P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData,
    P2CONST(uint8, AUTOMATIC, DLT_CONST) SourceData)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,Rule 20.7 */
{
    (*DestData) = (uint32)((uint32)(SourceData[0]) << 24u);
    (*DestData) |= (uint32)((uint32)(SourceData[1]) << 16u);
    (*DestData) |= (uint32)((uint32)(SourceData[2]) << 8u);
    (*DestData) |= (uint32)((uint32)(SourceData[3]));
}

/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,Rule 20.7 */
FUNC(void, DLT_CODE)
Dlt_CopyArrayToIntBigEndian(
    P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) DestData,
    P2CONST(uint8, AUTOMATIC, DLT_CONST) SourceData)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,Rule 20.7 */
{
    (*DestData) = (uint16)((uint16)(SourceData[0]) << 8u);
    (*DestData) |= (uint16)((uint16)(SourceData[1]));
}

static FUNC(void, DLT_CODE) Dlt_SendOverFlow(uint16 ChannelIndex)
{
    /* "BufferOverflowNotification" Request message format: StandardHeader(16), ExternHeader(10), Payload(4) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)}, {MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4) } */

    /* "BufferOverflowNotification" Response message format: standardHeader(16), externHeader(10), Payload(9) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1),overflowCounter(4) */

    /* Check if the DLT message extension header is support verbose mode */
    boolean VerboseMode = FALSE;
    if (Dlt_ConfigPtr->Protocol != NULL_PTR)
    {
        VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
    }
    /* Calculate the byte length of the DLT service response message */
    uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_BUFFER_OVER_FLOW_LENGTH);

#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);

    Dlt_CreateExtendedHeaderInfoTypes CreateExtendedHeaderInfo;
    Dlt_CreateStandardHeaderInfoTypes CreateStandardHeaderInfo;

    /* Create standard and extended headers for DLT */
    CreateStandardHeaderInfo.ChannelIndex = ChannelIndex;
    CreateStandardHeaderInfo.Messagelength = Messagelength;
    CreateStandardHeaderInfo.SessionId = ChannelPtr->OverFlowInfo.SessionId;
    CreateStandardHeaderInfo.timestamp = ChannelPtr->OverFlowInfo.timestamp;
    uint8 offset = 0;
    SchM_Enter_Dlt_MsgChannel();
#if (STD_ON == DLT_RX_DATA_PATH_SUPPORT)
    /* Specify field fill "Standard Header" to the DLT message buffer */
    Dlt_CreateControlStandardHeader(&CreateStandardHeaderInfo, &(ChannelPtr->SendControlBuffer[offset]));
#endif /* STD_ON == DLT_RX_DATA_PATH_SUPPORT */
    offset = DLT_STANDARD_HEADER_LENGTH;
    /* Check if the extended header is support verbose mode */
    if (VerboseMode == TRUE)
    {
        Dlt_SwcContextType SwcContext;
        CreateExtendedHeaderInfo.ChannelIndex = ChannelIndex;
        CreateExtendedHeaderInfo.BufferDataType = ChannelPtr->OverFlowInfo.BufferDataType;
        SwcContext.SwcApplicationId = ChannelPtr->OverFlowInfo.SwcContext.SwcApplicationId;
        SwcContext.SwcContextId = ChannelPtr->OverFlowInfo.SwcContext.SwcContextId;
        CreateExtendedHeaderInfo.logInfo = NULL_PTR;
        CreateExtendedHeaderInfo.traceInfo = NULL_PTR;
        CreateExtendedHeaderInfo.SwcContext = SwcContext;
#if (STD_ON == DLT_RX_DATA_PATH_SUPPORT)
        /* Specify field fill "Extended Header" to the DLT message buffer */
        Dlt_CreateControlExtendedHeader(&CreateExtendedHeaderInfo, &(ChannelPtr->SendControlBuffer[offset]));
#endif /* STD_ON == DLT_RX_DATA_PATH_SUPPORT */
        offset += DLT_EXTENED_HEADER_LENGTH;
    }
    /* Specify field fill "Service Identifier" to the DLT message buffer */
    Dlt_CopyLongToArrayBigEndian((uint32)0x00000023, &(ChannelPtr->SendControlBuffer[offset]));
    offset += 4u;
    /* Specify field fill "Status Word" to the DLT message buffer */
    ChannelPtr->SendControlBuffer[offset] = (uint8)DLT_STATUS_OK;
    offset += 1u;
    /* Specify field fill "Overflow Counter" to the DLT message buffer */
    Dlt_CopyLongToArrayBigEndian(ChannelPtr->OverFlowInfo.overflowCounter, &(ChannelPtr->SendControlBuffer[offset]));

    PduInfoType PduInfo;
    boolean txPduUsesTp = Dlt_ConfigPtr->LogOutput->LogChannel[ChannelIndex].DltTxPdu->DltTxPduUsesTp;
    /* Check if the DLT module is support TP transmission */
    if (txPduUsesTp == TRUE)
    {
        PduInfo.SduDataPtr = NULL_PTR;
    }
    else
    {
        PduInfo.SduDataPtr = ChannelPtr->SendControlBuffer;
    }
    SchM_Exit_Dlt_MsgChannel();
    PduInfo.SduLength = Messagelength;
    PduIdType txPduRPduId = Dlt_ConfigPtr->LogOutput->LogChannel[ChannelIndex].DltTxPdu->DltTxPduRPduId;
    /* Call the "PduR_DltTransmit" function to start sending DLT messages */
    if (E_OK == PduR_DltTransmit(txPduRPduId, &PduInfo))
    {
        SchM_Enter_Dlt_MsgChannel();
        ChannelPtr->OverFlowInfo.OverFlowSend = TRUE;
        ChannelPtr->LastSendStatus = DLT_CONTROL_SENDED;
        ChannelPtr->ControlSendOffset = 0U;
        ChannelPtr->LastControlSendLength = Messagelength;
        SchM_Exit_Dlt_MsgChannel();
    }
#endif
}

/*======================================================================================================
  Response is build like this:
  1. Number of Application IDs
  2. Application ID + Number of Context IDs
     1. Context ID + log level;
     2. Context ID + log level;
     3. .........
  3. Application ID + Number of Context IDs
     1. Context ID + log level; 2.
  4. .........
======================================================================================================*/
/* "GetLogInfo" Request message format: StandardHeader(16), ExternHeader(10), Payload(17) */
/* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
/* Payload format: ServiceID(4),{Options(1),ApplicationID(4),ContextID(4),Reserved(4)} */

/* Response message format: standardHeader(16), externHeader(10), Payload(5 + n + 4) */
/* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
/* Payload format: ServiceID(4),Status(1),LogInfo(n),Reserved(4) */
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, DLT_CODE) Dlt_SendGetLogInfo(uint8 options, uint16 SwcIndex, uint16 SwcContextIndex)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_SWC_NUM > 0) && (DLT_SWC_MAX_CONTEXT_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint16 info_length;
    const Dlt_SwcContextInofType* SwcContextInfoPtr = &(Dlt_RunTime.SwcInfo[SwcIndex].SwcContextInfo[SwcContextIndex]);

    Dlt_CreateExtendedHeaderInfoTypes CreateExtendedHeaderInfo;
    Dlt_CreateStandardHeaderInfoTypes CreateStandardHeaderInfo;
    /* Generating the timestamp */
    if (Dlt_ConfigPtr->Protocol->DltHeaderUseTimestamp == TRUE)
    {
        CreateStandardHeaderInfo.timestamp = Dlt_GetTimeElapsed();
    }
    CreateStandardHeaderInfo.SessionId = Dlt_RunTime.SwcInfo[SwcIndex].DltSwcSessionId;
    Dlt_ApplicationIDType SwcApplicationId = SwcContextInfoPtr->SwcContext.SwcApplicationId;
    Dlt_ContextIDType SwcContextId = SwcContextInfoPtr->SwcContext.SwcContextId;

    for (uint16 iloop = 0; iloop < SwcContextInfoPtr->DltLogChannelRefNum; iloop++)
    {
        /* Check if the option word for the given log message is 7 */
        if (options == 7u)
        {
            /* options about registered ApplicationID/ContextID with logLevel/traceStatus information and all textual
             * descriptions of each Application ID and Context ID */
            info_length = 23u;
        }
        /* Check if the option word for the given log message is 6 */
        else if (options == 6u)
        {
            /* options about registered ApplicationID/ContextID with logLevel/traceStatus information */
            info_length = 19u;
        }
        else
        {
            info_length = 17u;
        }

        /* Calculate the byte length of the application description in the log message */
        uint16 lenAppDescription = (uint16)SwcContextInfoPtr->lenAppDescription;
        /* Calculate the byte length of the context description in the log message */
        uint16 lenContextDescription = (uint16)SwcContextInfoPtr->lenContextDescription;

        /* Check if the DLT message extension header is support verbose mode */
        boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
        /* Calculate the byte length of the DLT payload content */
        uint16 Payloadlength = lenAppDescription + lenContextDescription + info_length;
        /* Calculate the byte length of the DLT service response message */
        uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, Payloadlength);

        /* Set the index number of the log channel */
        uint16 ChannelIndex = SwcContextInfoPtr->DltLogChannelRef[iloop];
        Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
        /* Check if the last send status of the given log channel was "DLT_WAIT_SEND". */
        if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
        {
            SchM_Enter_Dlt_MsgChannel();
            /* Create standard header for the message of the DLT given log channel */
            CreateStandardHeaderInfo.ChannelIndex = ChannelIndex;
            CreateStandardHeaderInfo.Messagelength = Messagelength;
#if (STD_ON == DLT_RX_DATA_PATH_SUPPORT)
            Dlt_CreateControlStandardHeader(&CreateStandardHeaderInfo, &(ChannelPtr->SendControlBuffer[0]));
#endif /* STD_ON == DLT_RX_DATA_PATH_SUPPORT */
            uint16 offset = DLT_STANDARD_HEADER_LENGTH;
            /* Check if the DLT message extension header is support verbose mode */
            if (VerboseMode == TRUE)
            {
                /* Create extended header for the message of the DLT given log channel */
                CreateExtendedHeaderInfo.ChannelIndex = ChannelIndex;
                CreateExtendedHeaderInfo.BufferDataType = DLT_CONTROL_DATA;
                CreateExtendedHeaderInfo.logInfo = NULL_PTR;
                CreateExtendedHeaderInfo.traceInfo = NULL_PTR;
                CreateExtendedHeaderInfo.SwcContext.SwcApplicationId = SwcApplicationId;
                CreateExtendedHeaderInfo.SwcContext.SwcContextId = SwcContextId;
#if (STD_ON == DLT_RX_DATA_PATH_SUPPORT)
                Dlt_CreateControlExtendedHeader(&CreateExtendedHeaderInfo, &(ChannelPtr->SendControlBuffer[offset]));
#endif /* STD_ON == DLT_RX_DATA_PATH_SUPPORT */
                offset += DLT_EXTENED_HEADER_LENGTH;
            }
            /* Specify field fill "ServiceID" to the DLT message buffer */
            Dlt_CopyLongToArrayBigEndian((uint32)0x00000003, &(ChannelPtr->SendControlBuffer[offset]));
            offset += 4u;
            /* Check if the option word for the given log message not is 5 */
            if (options != 5u)
            {
                /* Check if the total length of DLT messages is greater than the maximum limit size */
                if (Messagelength > DLT_CHANNEL_MAX_BUFFER_LENGTH)
                {
                    /* Response data OverFlow 鈥� If the generated response is too large. */
                    /* Specify field fill "STATUS" to the DLT message buffer */
                    ChannelPtr->SendControlBuffer[offset] = 9;
                }
                else
                {
                    /* Specify field fill "STATUS" to the DLT message buffer */
                    ChannelPtr->SendControlBuffer[offset] = options;
                    offset += 1u;
                    /* Specify field fill "appIdCount" to the DLT message buffer */
                    Dlt_CopyIntToArrayBigEndian((uint16)1, &(ChannelPtr->SendControlBuffer[offset]));
                    offset += 2u;
                    /* Specify field fill "appIdInfo.appID" to the DLT message buffer */
                    Dlt_CopyLongToArrayBigEndian(SwcApplicationId, &(ChannelPtr->SendControlBuffer[offset]));
                    offset += 4u;
                    /* Specify field fill "contextIdCount" to the DLT message buffer */
                    Dlt_CopyIntToArrayBigEndian((uint16)1, &(ChannelPtr->SendControlBuffer[offset]));
                    offset += 2u;
                    /* Specify field fill "contextIdInfoList.contextId" to the DLT message buffer */
                    Dlt_CopyLongToArrayBigEndian(SwcContextId, &(ChannelPtr->SendControlBuffer[offset]));
                    offset += 4u;
                    /* Specify field fill "contextIdInfoList.logLevel" to the DLT message buffer */
                    ChannelPtr->SendControlBuffer[offset] = (uint8)SwcContextInfoPtr->DltThreshold;
                    offset += 1u;
                    /* Specify field fill "contextIdInfoList.traceStatus" to the DLT message buffer */
                    /* PRQA S 4304++ */ /* MISRA Rule 10.5 */
                    ChannelPtr->SendControlBuffer[offset] = (uint8)SwcContextInfoPtr->TraceStatus;
                    /* PRQA S 4304-- */ /* MISRA Rule 10.5 */
                    offset += 1u;
                    /* Check if the option word for the given log message is 7 */
                    if (options == 7u)
                    {
                        lenAppDescription = (uint16)SwcContextInfoPtr->lenAppDescription;
                        lenContextDescription = (uint16)SwcContextInfoPtr->lenContextDescription;
                        /* Specify field fill "contextIdInfoList.lenContextDescription" to the DLT message buffer */
                        Dlt_CopyIntToArrayBigEndian(lenContextDescription, &(ChannelPtr->SendControlBuffer[offset]));
                        offset += 2u;
                        /* Specify field fill "contextIdInfoList.contextDesc" to the DLT message buffer */
                        Dlt_MemCopy(
                            &(ChannelPtr->SendControlBuffer[offset]),
                            SwcContextInfoPtr->contextDescription,
                            (uint32)lenContextDescription);
                        offset += (uint16)lenContextDescription;
                        /* Specify field fill "contextIdInfoList.appDescLen" to the DLT message buffer */
                        Dlt_CopyIntToArrayBigEndian(lenContextDescription, &(ChannelPtr->SendControlBuffer[offset]));
                        offset += 2u;
                        /* Specify field fill "contextIdInfoList.appDesc" to the DLT message buffer */
                        Dlt_MemCopy(
                            &(ChannelPtr->SendControlBuffer[offset]),
                            SwcContextInfoPtr->appDescription,
                            lenAppDescription);
                    }
                }
            }
            else
            {
                /* Specify field fill "STATUS" to the DLT message buffer */
                ChannelPtr->SendControlBuffer[offset] = options;
                offset += 1u;
                /* Specify field fill "appIdCount" to the DLT message buffer */
                Dlt_CopyIntToArrayBigEndian((uint16)1, &(ChannelPtr->SendControlBuffer[offset]));
                offset += 2u;
                /* Specify field fill "appIdInfo.appID" to the DLT message buffer */
                Dlt_CopyLongToArrayBigEndian(SwcApplicationId, &(ChannelPtr->SendControlBuffer[offset]));
                offset += 4u;
                /* Specify field fill "contextIdCount" to the DLT message buffer */
                Dlt_CopyIntToArrayBigEndian((uint16)1, &(ChannelPtr->SendControlBuffer[offset]));
                offset += 2u;
                /* Specify field fill "contextIdInfoList.contextId" to the DLT message buffer */
                Dlt_CopyLongToArrayBigEndian(SwcContextId, &(ChannelPtr->SendControlBuffer[offset]));
            }
            SchM_Exit_Dlt_MsgChannel();

            /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
            const Dlt_LogChannelType* logChannelPtr =
                (Dlt_LogChannelType*)&(Dlt_ConfigPtr->LogOutput->LogChannel[ChannelIndex]);
            boolean txPduUsesTp = logChannelPtr->DltTxPdu->DltTxPduUsesTp;
            PduInfoType PduInfo;
            /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
            /* Check if the PDU transmission mode is "TP" */
            if (txPduUsesTp == TRUE)
            {
                PduInfo.SduDataPtr = NULL_PTR;
            }
            /* Check if the PDU transmission mode is "IF" */
            else
            {
                PduInfo.SduDataPtr = ChannelPtr->SendControlBuffer;
            }
            PduInfo.SduLength = Messagelength;
            PduIdType txPduRPduId = logChannelPtr->DltTxPdu->DltTxPduRPduId;
            /* Call "PduR_DltTransmit" to start sending PDU data */
            ret = PduR_DltTransmit(txPduRPduId, &PduInfo);
            if (E_OK == ret)
            {
                SchM_Enter_Dlt_MsgChannel();
                /* Set the last send status of the given log channel to "DLT_CONTROL_SENDED" */
                ChannelPtr->LastSendStatus = DLT_CONTROL_SENDED;
                /* Set the control send buffer offset of the given log channel to 0 */
                ChannelPtr->ControlSendOffset = 0U;
                /* Set the control send message length of the given log channel to "Messagelength" */
                ChannelPtr->LastControlSendLength = Messagelength;
                SchM_Exit_Dlt_MsgChannel();
            }
        }
    }
#endif /* DLT_SWC_NUM > 0 && DLT_SWC_MAX_CONTEXT_NUM > 0 */
    return ret;
}

FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_InterSetLogLevel(
    uint16 ChannelIndex,
    Dlt_ApplicationIDType appId,
    Dlt_ContextIDType contextId,
    Dlt_MessageLogLevelType newLogLevel)
{
    Std_ReturnType ret = E_NOT_OK;
    uint16 index = 0u;
#if (1u < DLT_SWC_NUM)
    for (; (index < DLT_SWC_NUM) && (ret == E_NOT_OK); index++)
#endif /* 1u < DLT_SWC_NUM */
    {
#if ((DLT_SWC_NUM > 0) && (DLT_SWC_MAX_CONTEXT_NUM > 0))
        uint16 SwcContextNum = Dlt_RunTime.SwcInfo[index].DltSwcContextNum;
        for (uint16 iloop = 0; (iloop < SwcContextNum) && (ret == E_NOT_OK); iloop++)
        {
            Dlt_SwcContextInofType* SwcContextInfoPtr = &(Dlt_RunTime.SwcInfo[index].SwcContextInfo[iloop]);
            if ((appId == SwcContextInfoPtr->SwcContext.SwcApplicationId)
                && (contextId == SwcContextInfoPtr->SwcContext.SwcContextId))
            {
                uint16 logChannelRefNum = SwcContextInfoPtr->DltLogChannelRefNum;
                for (uint16 Cnt = 0; (Cnt < logChannelRefNum) && (ret == E_NOT_OK); Cnt++)
                {
                    if (ChannelIndex == SwcContextInfoPtr->DltLogChannelRef[Cnt])
                    {
                        /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
                        const Dlt_SwcTypes* SwcPtr = (Dlt_SwcTypes*)&(Dlt_Swc.DltSwc[index]);
                        /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
                        if ((SwcContextInfoPtr->DltThreshold != newLogLevel)
                            && (TRUE == SwcPtr->DltSwcSupportLogLevelAndTraceStatusChangeNotification)
                            && (NULL_PTR != SwcPtr->LogLevelChangedNotification))
                        {
                            SwcPtr->LogLevelChangedNotification(appId, contextId, newLogLevel);
                        }
                        SchM_Enter_Dlt_SwcContext();
                        SwcContextInfoPtr->DltThreshold = newLogLevel;
                        SchM_Exit_Dlt_SwcContext();
                        ret = E_OK;
                    }
                }
            }
        }
#endif /* DLT_SWC_NUM > 0 && DLT_SWC_MAX_CONTEXT_NUM > 0 */
    }
    return ret;
}

FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_InterSetTraceStatus(
    uint16 ChannelIndex,
    Dlt_ApplicationIDType appId,
    Dlt_ContextIDType contextId,
    boolean newTraceStatus)
{
    Std_ReturnType ret = E_NOT_OK;
    uint16 index = 0u;
#if (1u < DLT_SWC_NUM)
    for (; (index < DLT_SWC_NUM) && (ret == E_NOT_OK); index++)
#endif /* 1u < DLT_SWC_NUM */
    {
#if ((DLT_SWC_NUM > 0) && (DLT_SWC_MAX_CONTEXT_NUM > 0))
        uint16 SwcContextNum = Dlt_RunTime.SwcInfo[index].DltSwcContextNum;
        for (uint16 iloop = 0; (iloop < SwcContextNum) && (ret == E_NOT_OK); iloop++)
        {
            Dlt_SwcContextInofType* SwcContextInfoPtr = &(Dlt_RunTime.SwcInfo[index].SwcContextInfo[iloop]);
            if ((appId == SwcContextInfoPtr->SwcContext.SwcApplicationId)
                && (contextId == SwcContextInfoPtr->SwcContext.SwcContextId))
            {
                uint16 logChannelRefNum = SwcContextInfoPtr->DltLogChannelRefNum;
                for (uint16 Cnt = 0; (Cnt < logChannelRefNum) && (ret == E_NOT_OK); Cnt++)
                {
                    if (ChannelIndex == SwcContextInfoPtr->DltLogChannelRef[Cnt])
                    {
                        /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
                        const Dlt_SwcTypes* SwcPtr = (Dlt_SwcTypes*)&(Dlt_Swc.DltSwc[index]);
                        /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
                        if ((SwcContextInfoPtr->TraceStatus != newTraceStatus)
                            && (TRUE == SwcPtr->DltSwcSupportLogLevelAndTraceStatusChangeNotification)
                            && (NULL_PTR != SwcPtr->TraceStatusChangedNotification))
                        {
                            SwcPtr->TraceStatusChangedNotification(appId, contextId, newTraceStatus);
                        }
                        SchM_Enter_Dlt_SwcContext();
                        SwcContextInfoPtr->TraceStatus = newTraceStatus;
                        SchM_Exit_Dlt_SwcContext();
                        ret = E_OK;
                    }
                }
            }
        }
#endif /* DLT_SWC_NUM > 0 && DLT_SWC_MAX_CONTEXT_NUM > 0*/
    }
    return ret;
}

FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_InterSetLogChannelAssignment(
    Dlt_ApplicationIDType appId,
    Dlt_ContextIDType contextId,
    Dlt_LogChannelNameType logChannelName,
    Dlt_AssignmentOperation addRemoveOp)
{
    Std_ReturnType ret = E_NOT_OK;

    boolean Find = FALSE;
    uint16 logChannelRefNum;

    /* The index number of the log channel is extracted by the log channel name */
    uint16 DltLogChannelIndex = Dlt_GetChannelIndexByChannelName(logChannelName);
#if (1u < DLT_CHANNEL_NUM)
    /* Check if the log channel index number is valid */
    if (DltLogChannelIndex != DLT_CHANNEL_NUM)
#endif /* 1u < DLT_CHANNEL_NUM */
    {
        uint16 SwcIndex = 0U;
        /* Traverse the list of all log channel index */
#if (1u < DLT_SWC_NUM)
        for (; (SwcIndex < DLT_SWC_NUM) && (ret == E_NOT_OK); SwcIndex++)
#endif /* 1u < DLT_SWC_NUM */
        {
#if ((DLT_SWC_NUM > 0) && (DLT_SWC_MAX_CONTEXT_NUM > 0))
            uint16 SwcContextNum = Dlt_RunTime.SwcInfo[SwcIndex].DltSwcContextNum;
            for (uint16 SwcContextIndex = 0; (SwcContextIndex < SwcContextNum) && (ret == E_NOT_OK); SwcContextIndex++)
            {
                Dlt_SwcContextInofType* SwcContextInfoPtr =
                    &(Dlt_RunTime.SwcInfo[SwcIndex].SwcContextInfo[SwcContextIndex]);
                logChannelRefNum = SwcContextInfoPtr->DltLogChannelRefNum;
                /* Check if the incoming appId/contextId parameter is matche */
                if ((appId == SwcContextInfoPtr->SwcContext.SwcApplicationId)
                    && (contextId == SwcContextInfoPtr->SwcContext.SwcContextId))
                {
                    if (addRemoveOp == DLT_ASSIGN_ADD)
                    {
                        for (uint16 iloop = 0; iloop < logChannelRefNum; iloop++)
                        {
                            /* Check if the incoming logChannelIndex parameter is matche */
                            if (SwcContextInfoPtr->DltLogChannelRef[iloop] == DltLogChannelIndex)
                            {
                                Find = TRUE;
                                break;
                            }
                        }
                        if (Find == FALSE)
                        {
                            SchM_Enter_Dlt_SwcContext();
                            SwcContextInfoPtr->DltLogChannelRef[logChannelRefNum] = DltLogChannelIndex;
                            SwcContextInfoPtr->DltLogChannelRefNum++;
                            SchM_Exit_Dlt_SwcContext();
                        }
                        ret = E_OK;
                    }
                    else
                    {
                        for (uint16 iloop = 0; (iloop < logChannelRefNum) && (ret == E_NOT_OK); iloop++)
                        {
                            /* Check if the incoming logChannelIndex parameter is matche */
                            if (SwcContextInfoPtr->DltLogChannelRef[iloop] == DltLogChannelIndex)
                            {
                                SchM_Enter_Dlt_SwcContext();
                                Dlt_MoveChannelRef(
                                    &(SwcContextInfoPtr->DltLogChannelRef[iloop]),
                                    (logChannelRefNum - iloop));
                                SwcContextInfoPtr->DltLogChannelRefNum--;
                                SchM_Exit_Dlt_SwcContext();
                                ret = E_OK;
                            }
                        }
                    }
                }
            }
#endif /* DLT_SWC_NUM > 0 && DLT_SWC_MAX_CONTEXT_NUM > 0 */
        }
    }
    return ret;
}

static FUNC(void, DLT_APPL_CODE) Dlt_MoveChannelRef(uint16* DltLogChannelRef, uint16 Length)
{
    for (uint16 Cnt = 0; Cnt < Length; Cnt++)
    {
        DltLogChannelRef[Cnt] = DltLogChannelRef[Cnt + 1u];
    }
}

/* "SendTraceMessage" Send trace message format: StandardHeader(16), ExternHeader(10), Payload((4+n)*m) */
/* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)}, {MSIN(1),NOAR(1),APID(4),CTID(4)} */
/* Payload format: {PayloadType(4),PayloadData(n)},{PayloadType(4),PayloadData(n)}, .... } */
static FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_InterSendTraceMessage(
    Dlt_SessionIDType sessionId,
    P2CONST(Dlt_MessageTraceInfoType, AUTOMATIC, DLT_APPL_CONST) traceInfo,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) traceData,
    uint16 traceDataLength)
{
    Std_ReturnType ret;
    uint32 timestamp = 0;
#if (DLT_CHANNEL_NUM > 0)
    uint16 ChannelIndexList[DLT_CHANNEL_NUM];

    Dlt_MemSet((uint8*)ChannelIndexList, 0xFF, (DLT_CHANNEL_NUM + DLT_CHANNEL_NUM));
    /*Generating the timestamp*/
    if ((Dlt_ConfigPtr->Protocol != NULL_PTR) && (Dlt_ConfigPtr->Protocol->DltHeaderUseTimestamp == TRUE))
    {
        timestamp = Dlt_GetTimeElapsed();
    }
    /* The Swc index number is extracted by the session identifier */
    uint16 SwcIndex = Dlt_GetSwcSessionIdIndex(sessionId);
#if (1u < DLT_SWC_NUM)
    /* Check if the Swc index number is valid*/
    if (SwcIndex == DLT_SWC_NUM)
    {
        /* Return unknown session identifier */
        ret = DLT_E_UNKNOWN_SESSION_ID;
    }
    else
#endif /* 1u < DLT_SWC_NUM */
    {
        /* The Swc index and trace information are used to extract the current trace status */
        boolean CurrentTraceStatus = Dlt_GetSwcTraceStatus(SwcIndex, traceInfo);
        /*Message filtering*/
        CurrentTraceStatus = Dlt_CompareTraceStatus(CurrentTraceStatus);
        ret = E_OK;
        /* Check if the current trace status is TRUE */
        if (CurrentTraceStatus == TRUE)
        {
            /* Select target LogChannel */
            Dlt_GetChannelIndex(SwcIndex, traceInfo->appId, traceInfo->contextId, ChannelIndexList);

            uint16 iloop = 0u;
            /* Traverse the list of all log channel index */
#if (1u < DLT_CHANNEL_NUM)
            for (; ((ChannelIndexList[iloop] != 0xFFFFu) && (iloop < DLT_CHANNEL_NUM)); iloop++)
#else  /* 1u < DLT_CHANNEL_NUM */
            if (ChannelIndexList[iloop] != 0xFFFFu)
#endif /* 1u < DLT_CHANNEL_NUM */
            {
                /* Set the index number of the log channel */
                uint16 ChannelIndex = ChannelIndexList[iloop];

                /* Check if the DLT message extension header is support verbose mode */
                boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
                /* Calculate the byte length of the DLT service response message */
                uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, traceDataLength);
                uint16 logChannelMaxMessageLength =
                    Dlt_ConfigPtr->LogOutput->LogChannel[ChannelIndex].DltLogChannelMaxMessageLength;
#if (DLT_SWC_NUM > 0)
                uint16 MaxSwcTraceMessageLength = Dlt_Swc.DltSwc[SwcIndex].MaxSwcTraceMessageLength;
#endif
                /*Check message length*/
                if ((logChannelMaxMessageLength < Messagelength)
#if (DLT_SWC_NUM > 0)
                    || (MaxSwcTraceMessageLength < traceDataLength)
#endif /* DLT_SWC_NUM > 0*/
                )
                {
                    /* The return message length is too large */
                    ret = DLT_E_MSG_TOO_LARGE;
                }
                else
                {
                    /* The current trace status is extracted by the log channel index */
                    CurrentTraceStatus = Dlt_GetChannelTraceStatus(ChannelIndex);
                    /* Message filtering */
                    CurrentTraceStatus = Dlt_CompareTraceStatus(CurrentTraceStatus);
                    ret = E_OK;
                    /* Check if the current trace status is TRUE */
                    if (CurrentTraceStatus == TRUE)
                    {
                        Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
                        /* Copying Dlt message to the LogChannel buffer */
                        /* BufferOverflow check */
                        if (ChannelPtr->UnusedLength < (uint32)Messagelength)
                        {
                            ret = DLT_E_NO_BUFFER;

                            Dlt_OverFlowInfoTypes* OverFlowInfoPtr = &(ChannelPtr->OverFlowInfo);
                            SchM_Enter_Dlt_MsgChannel();
                            OverFlowInfoPtr->OverFlow = TRUE;
                            OverFlowInfoPtr->OverFlowSend = FALSE;
                            OverFlowInfoPtr->BufferOverflowTimer = 0;
                            OverFlowInfoPtr->overflowCounter = (uint32)Messagelength;
                            OverFlowInfoPtr->BufferDataType = DLT_CONTROL_DATA;
                            OverFlowInfoPtr->SessionId = sessionId;
                            OverFlowInfoPtr->timestamp = timestamp;
                            OverFlowInfoPtr->SwcContext.SwcApplicationId = traceInfo->appId;
                            OverFlowInfoPtr->SwcContext.SwcContextId = traceInfo->contextId;
                            SchM_Exit_Dlt_MsgChannel();
                        }
                        else
                        {
                            Dlt_CreateExtendedHeaderInfoTypes CreateExtendedHeaderInfo;
                            Dlt_CreateStandardHeaderInfoTypes CreateStandardHeaderInfo;

                            SchM_Enter_Dlt_MsgChannel();
                            /* Create standard header for the message of the DLT given log channel */
                            CreateStandardHeaderInfo.ChannelIndex = ChannelIndex;
                            CreateStandardHeaderInfo.Messagelength = Messagelength;
                            CreateStandardHeaderInfo.SessionId = sessionId;
                            CreateStandardHeaderInfo.timestamp = timestamp;
                            Dlt_CreateStandardHeader(&CreateStandardHeaderInfo);
                            /* Check if the DLT message extension header is support verbose mode */
                            if (VerboseMode == TRUE)
                            {
                                /* Create extended header for the message of the DLT given log channel */
                                CreateExtendedHeaderInfo.ChannelIndex = ChannelIndex;
                                CreateExtendedHeaderInfo.BufferDataType = DLT_TRACE_DATA;
                                CreateExtendedHeaderInfo.logInfo = NULL_PTR;
                                /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
                                CreateExtendedHeaderInfo.traceInfo = (Dlt_MessageTraceInfoType*)traceInfo;
                                /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
                                CreateExtendedHeaderInfo.SwcContext.SwcApplicationId = traceInfo->appId;
                                CreateExtendedHeaderInfo.SwcContext.SwcContextId = traceInfo->contextId;
                                Dlt_CreateExtendedHeader(&CreateExtendedHeaderInfo);
                            }
                            /* Copies the user's trace data into the buffer of the given log channel */
                            for (uint16 jloop = 0; jloop < traceDataLength; jloop++)
                            {
                                Dlt_MoveWriteIndexCopy(ChannelIndex, traceData[jloop]);
                            }
                            /* Re-calculate the resources of the DLT given log channel buffer */
                            ChannelPtr->UnusedLength -= (uint32)Messagelength;
                            /* Performs increment of 1 action on the counter of the DLT message */
                            ChannelPtr->MessageCounter++;
                            SchM_Exit_Dlt_MsgChannel();
                            ret = DLT_OK;
                        }
                    }
                }
            }
        }
    }
#endif
    return ret;
}

/* "SendLogMessage" Send log message format: StandardHeader(16), ExternHeader(10), Payload((4+n)*m) */
/* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)}, {MSIN(1),NOAR(1),APID(4),CTID(4)} */
/* Payload format: {PayloadType(4),PayloadData(n)},{PayloadType(4),PayloadData(n)}, .... } */
static FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_InterSendLogMessage(
    Dlt_SessionIDType sessionId,
    P2CONST(Dlt_MessageLogInfoType, AUTOMATIC, DLT_APPL_CONST) logInfo,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) logData,
    uint16 logDataLength)
{
    Std_ReturnType ret;
    uint32 timestamp = 0;
    /*Generating the timestamp*/
    if ((Dlt_ConfigPtr->Protocol != NULL_PTR) && (Dlt_ConfigPtr->Protocol->DltHeaderUseTimestamp == TRUE))
    {
        timestamp = Dlt_GetTimeElapsed();
    }
    /* The Swc index number is extracted by the session identifier */
    uint16 SwcIndex = Dlt_GetSwcSessionIdIndex(sessionId);
#if (1u < DLT_SWC_NUM)
    /* Check if the Swc index number is valid*/
    if (SwcIndex == DLT_SWC_NUM)
    {
        /* Return unknown session identifier */
        ret = DLT_E_UNKNOWN_SESSION_ID;
    }
    else
#endif /* 1u < DLT_CHANNEL_NUM */
    {
#if (DLT_CHANNEL_NUM > 0)
        boolean Filter;
        uint16 ChannelIndexList[DLT_CHANNEL_NUM];
        Dlt_MemSet((uint8*)ChannelIndexList, 0xFF, (DLT_CHANNEL_NUM + DLT_CHANNEL_NUM));
        /* The Swc index and log information are used to extract the current log level */
        Dlt_MessageLogLevelType CurrentLogLevel = Dlt_GetSwcLogLevel(SwcIndex, logInfo);
        /* Message filtering */
        Filter = Dlt_CompareLogLevel(CurrentLogLevel, logInfo->logLevel);
        ret = E_OK;
        /* Check if the message filtering status is passed */
        if (Filter == TRUE)
        {
            /* Select target LogChannel */
            Dlt_GetChannelIndex(SwcIndex, logInfo->appId, logInfo->contextId, ChannelIndexList);

            uint16 iloop = 0u;
            /* Traverse the list of all log channel index */
#if (1u < DLT_CHANNEL_NUM)
            for (; ((ChannelIndexList[iloop] != 0xFFFFu) && (iloop < DLT_CHANNEL_NUM)); iloop++)
#else  /* 1u < DLT_CHANNEL_NUM */
            if (ChannelIndexList[iloop] != 0xFFFFu)
#endif /* 1u < DLT_CHANNEL_NUM */
            {
                /* Set the index number of the log channel */
                uint16 ChannelIndex = ChannelIndexList[iloop];

                /* Check if the DLT message extension header is support verbose mode */
                boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
                /* Calculate the byte length of the DLT service response message */
                uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, logDataLength);
                uint16 logChannelMaxMessageLength =
                    Dlt_ConfigPtr->LogOutput->LogChannel[ChannelIndex].DltLogChannelMaxMessageLength;
#if (DLT_SWC_NUM > 0)
                uint16 MaxSwcLogMessageLength = Dlt_Swc.DltSwc[SwcIndex].MaxSwcLogMessageLength;
#endif
                /*Check message length*/
                if ((logChannelMaxMessageLength < Messagelength)
#if (DLT_SWC_NUM > 0)
                    || (MaxSwcLogMessageLength < logDataLength)
#endif
                )
                {
                    /* The return message length is too large */
                    ret = DLT_E_MSG_TOO_LARGE;
                }
                else
                {
                    /* The current log level is extracted by the log channel index */
                    CurrentLogLevel = Dlt_GetChannelLogLevel(ChannelIndex);
                    /* Message filtering */
                    Filter = Dlt_CompareLogLevel(CurrentLogLevel, logInfo->logLevel);
                    ret = E_OK;
                    /* Check if the message filtering status is passed */
                    if (Filter == TRUE)
                    {
                        Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
                        /*Copying Dlt message to the LogChannel buffer*/
                        /*BufferOverflow check*/
                        if (ChannelPtr->UnusedLength < (uint32)Messagelength)
                        {
                            ret = DLT_E_NO_BUFFER;

                            Dlt_OverFlowInfoTypes* OverFlowInfoPtr = &(ChannelPtr->OverFlowInfo);
                            SchM_Enter_Dlt_MsgChannel();
                            OverFlowInfoPtr->OverFlow = TRUE;
                            OverFlowInfoPtr->OverFlowSend = FALSE;
                            OverFlowInfoPtr->BufferOverflowTimer = 0;
                            OverFlowInfoPtr->overflowCounter = (uint32)Messagelength;
                            OverFlowInfoPtr->BufferDataType = DLT_CONTROL_DATA;
                            OverFlowInfoPtr->SessionId = sessionId;
                            OverFlowInfoPtr->timestamp = timestamp;
                            OverFlowInfoPtr->SwcContext.SwcApplicationId = logInfo->appId;
                            OverFlowInfoPtr->SwcContext.SwcContextId = logInfo->contextId;
                            SchM_Exit_Dlt_MsgChannel();
                        }
                        else
                        {
                            Dlt_CreateExtendedHeaderInfoTypes CreateExtendedHeaderInfo;
                            Dlt_CreateStandardHeaderInfoTypes CreateStandardHeaderInfo;

                            SchM_Enter_Dlt_MsgChannel();
                            /* Create standard header for the message of the DLT given log channel */
                            CreateStandardHeaderInfo.ChannelIndex = ChannelIndex;
                            CreateStandardHeaderInfo.Messagelength = Messagelength;
                            CreateStandardHeaderInfo.SessionId = sessionId;
                            CreateStandardHeaderInfo.timestamp = timestamp;
                            Dlt_CreateStandardHeader(&CreateStandardHeaderInfo);
                            /* Check if the DLT message extension header is support verbose mode */
                            if (VerboseMode == TRUE)
                            {
                                /* Create extended header for the message of the DLT given log channel */
                                CreateExtendedHeaderInfo.ChannelIndex = ChannelIndex;
                                CreateExtendedHeaderInfo.BufferDataType = DLT_LOG_DATA;
                                /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
                                CreateExtendedHeaderInfo.logInfo = (Dlt_MessageLogInfoType*)logInfo;
                                /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
                                CreateExtendedHeaderInfo.traceInfo = NULL_PTR;
                                CreateExtendedHeaderInfo.SwcContext.SwcApplicationId = logInfo->appId;
                                CreateExtendedHeaderInfo.SwcContext.SwcContextId = logInfo->contextId;
                                Dlt_CreateExtendedHeader(&CreateExtendedHeaderInfo);
                            }
                            /* Copies the user's log data into the buffer of the given log channel */
                            for (uint16 jloop = 0; jloop < logDataLength; jloop++)
                            {
                                Dlt_MoveWriteIndexCopy(ChannelIndex, logData[jloop]);
                            }
                            /* Re-calculate the resources of the DLT given log channel buffer */
                            ChannelPtr->UnusedLength -= (uint32)Messagelength;
                            /* Performs increment of 1 action on the counter of the DLT message */
                            ChannelPtr->MessageCounter++;
                            SchM_Exit_Dlt_MsgChannel();
                            ret = DLT_OK;
                        }
                    }
                }
            }
        }
#endif
    }
    return ret;
}

static FUNC(void, DLT_CODE) Dlt_Tx_RemoveBuffer(uint16 ChannelIndex, uint32 RemoveOffset, uint32 Length)
{
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint32 Offset = RemoveOffset;
    uint32 logChannelBufferSize = Dlt_ConfigPtr->LogOutput->LogChannel[ChannelIndex].DltLogChannelBufferSize;
    /* PRQA S 3396++ */ /* MISRA Rule 12.1 */
    uint32 tmp_len = ((Offset + Length) > logChannelBufferSize) ? (logChannelBufferSize - Offset) : Length;
    /* PRQA S 3396-- */ /* MISRA Rule 12.1 */
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* Check if the data length is 1 */
    if (Length == 1u) /* PRQA S 3396 */ /* MISRA Rule 12.1 */
    {
        /* PRQA S 3440,3432++ */ /* MISRA Rule 13.3,Rule 20.7 */
        Offset++;
        ChannelPtr->buffer[Offset] = 0u;
        if (tmp_len != Length)
        {
            Offset = 0u;
            tmp_len = Length - tmp_len;
            Offset++;
            ChannelPtr->buffer[Offset] = 0u;
        }
        /* PRQA S 3440,3432-- */ /* MISRA Rule 13.3,Rule 20.7 */
    }
    else
    {
        for (uint32 iloop = 0; iloop < Length; iloop++)
        {
            Offset++;
            ChannelPtr->buffer[Offset] = 0u; /* PRQA S 3440 */ /* MISRA Rule 13.3 */
        }
        if (tmp_len != Length)
        {
            Offset = 0u;
            tmp_len = Length - tmp_len;
            for (uint32 jloop = 0; jloop < tmp_len; jloop++)
            {
                Offset++;
                ChannelPtr->buffer[Offset] = 0u; /* PRQA S 3440 */ /* MISRA Rule 13.3 */
            }
        }
    }
#endif
}

static FUNC(void, DLT_CODE) Dlt_MemSet(uint8* Dest, uint8 Value, uint32 Size)
{
    uint32 cnt = Size;
    while (cnt > 0u)
    {
        cnt--;
        Dest[cnt] = Value;
    }
}

/* Header Type format: UEH(0),MSBF(1),WEID(2),WSID(3),WTMS(4),VERS(5-7) */
/* Standard Header format: HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4) */
static FUNC(void, DLT_CODE) Dlt_CreateStandardHeader(const Dlt_CreateStandardHeaderInfoTypes* CreateStandardHeader)
{
#if (DLT_CHANNEL_NUM > 0)
    uint8 HeaderType = 0;

    uint16 ChannelIndex = CreateStandardHeader->ChannelIndex;
    uint16 Messagelength = CreateStandardHeader->Messagelength;

    const Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);

    uint8 MessageCounter = ChannelPtr->MessageCounter;

    /* Specify field fill "UEH" to the DLT message buffer */
    if (Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode == TRUE)
    {
        HeaderType = 0x01u;
    }
    /* Specify field fill "MSBF" to the DLT message buffer */
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
    HeaderType |= 0x02u;
#endif
    /* Specify field fill "WEID" to the DLT message buffer */
    if (Dlt_ConfigPtr->Protocol->DltHeaderUseEcuId == TRUE)
    {
        HeaderType |= 0x04u;
    }
    /* Specify field fill "WSID" to the DLT message buffer */
    if (Dlt_ConfigPtr->Protocol->DltHeaderUseSessionID == TRUE)
    {
        HeaderType |= 0x08u;
    }
    /* Specify field fill "WTMS" to the DLT message buffer */
    if (Dlt_ConfigPtr->Protocol->DltHeaderUseTimestamp == TRUE)
    {
        HeaderType |= 0x10u;
    }

    /* Specify field fill "VERS" to the DLT message buffer */
    HeaderType |= 0x20u;
    /* Specify field fill "Header Type" to the DLT message buffer */
    Dlt_MoveWriteIndexCopy(ChannelIndex, HeaderType);
    /* Specify field fill "MCNT" to the DLT message buffer */
    Dlt_MoveWriteIndexCopy(ChannelIndex, MessageCounter);
    /* Specify field fill "LEN" to the DLT message buffer */
    Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(Messagelength >> 8u));
    Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(Messagelength));
    /* Specify field fill "WEID" to the DLT message buffer */
    if (Dlt_ConfigPtr->Protocol->DltHeaderUseEcuId == TRUE)
    {
        /*ECU ID*/
        uint32 EcuId = Dlt_GetEcuId();
        Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(EcuId >> 24u));
        Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(EcuId >> 16u));
        Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(EcuId >> 8u));
        Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(EcuId));
    }
    else
    {
        Dlt_MoveWriteIndexCopy(ChannelIndex, 0);
        Dlt_MoveWriteIndexCopy(ChannelIndex, 0);
        Dlt_MoveWriteIndexCopy(ChannelIndex, 0);
        Dlt_MoveWriteIndexCopy(ChannelIndex, 0);
    }
    /* Specify field fill "WSID" to the DLT message buffer */
    if (Dlt_ConfigPtr->Protocol->DltHeaderUseSessionID == TRUE)
    {
        Dlt_SessionIDType SessionId = CreateStandardHeader->SessionId;
        Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(SessionId >> 24u));
        Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(SessionId >> 16u));
        Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(SessionId >> 8u));
        Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(SessionId));
    }
    else
    {
        Dlt_MoveWriteIndexCopy(ChannelIndex, 0);
        Dlt_MoveWriteIndexCopy(ChannelIndex, 0);
        Dlt_MoveWriteIndexCopy(ChannelIndex, 0);
        Dlt_MoveWriteIndexCopy(ChannelIndex, 0);
    }
    /* Specify field fill "WTMS" to the DLT message buffer */
    if (Dlt_ConfigPtr->Protocol->DltHeaderUseTimestamp == TRUE)
    {
        uint32 timestamp = CreateStandardHeader->timestamp;
        Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(timestamp >> 24u));
        Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(timestamp >> 16u));
        Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(timestamp >> 8u));
        Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(timestamp));
    }
    else
    {
        Dlt_MoveWriteIndexCopy(ChannelIndex, 0);
        Dlt_MoveWriteIndexCopy(ChannelIndex, 0);
        Dlt_MoveWriteIndexCopy(ChannelIndex, 0);
        Dlt_MoveWriteIndexCopy(ChannelIndex, 0);
    }
#endif
}

/* Data Type format: DLT_TYPE_LOG(0),DLT_TYPE_APP_TRACE(1),DLT_TYPE_NW_TRACE(2),DLT_TYPE_CONTROL(3) */
/* Message Type format: DLT_CONTROL_REQUEST(0),DLT_CONTROL_RESPONSE(1) */
/* Log Message Type format:
 * DLT_LOG_OFF(0),DLT_LOG_FATAL(1),DLT_LOG_ERROR(2),DLT_LOG_WARN(3),DLT_LOG_INFO(4),DLT_LOG_DEBUG(5),DLT_LOG_VERBOSE(6)
 */
/* Trace Message Type format:
 * DLT_TRACE_VARIABLE(1),DLT_TRACE_FUNCTION_IN(2),DLT_TRACE_FUNCTION_OUT(3),DLT_TRACE_STATE(4),DLT_TRACE_VFB(5)
 */
/* Message Option format: MSIN = {VerboseMode(0), MessageType(1-3), DataType(4-7) } */
/* Extended Header format: MSIN(1),NOAR(1),APID(4),CTID(4) */
static FUNC(void, DLT_CODE) Dlt_CreateExtendedHeader(const Dlt_CreateExtendedHeaderInfoTypes* CreateExtendedHeader)
{
#if (DLT_CHANNEL_NUM > 0)
    uint8 MessageInfo = 0;
    uint8 message_type = 0;
    uint16 ChannelIndex = CreateExtendedHeader->ChannelIndex;
    Dlt_BufferDataTypeTypes BufferDataType = CreateExtendedHeader->BufferDataType;
    boolean useVerboseMode = Dlt_ConfigPtr->Protocol->DltUseVerboseMode;
    /* Specify field fill "VERB" to the DLT message buffer */
    if (useVerboseMode == TRUE)
    {
        MessageInfo = 0x01u;
    }
    /* Check if the data type of the DLT message buffer is "DLT_TRACE_DATA" */
    if (BufferDataType == DLT_TRACE_DATA)
    {
        if (NULL_PTR != CreateExtendedHeader->traceInfo)
        {
            message_type = (uint8)(CreateExtendedHeader->traceInfo->options.Dlt_Bits.message_type);
            /* Specify field fill "MSTP" to the DLT message buffer */
            MessageInfo |= (uint8)((uint8)message_type << 1u);
            /* PRQA S 4442++ */ /* MISRA Rule 10.3 */
            Dlt_MessageTraceType traceInfo = (uint8)(CreateExtendedHeader->traceInfo->traceInfo);
            /* PRQA S 4442-- */ /* MISRA Rule 10.3 */
            /* Specify field fill "MTIN" to the DLT message buffer */
            MessageInfo |= (uint8)((uint8)traceInfo << 4u);
        }
    }
    /* Check if the data type of the DLT message buffer is "DLT_CONTROL_DATA" */
    else if (BufferDataType == DLT_CONTROL_DATA)
    {
        /* Specify field fill "MSTP" to the DLT message buffer */
        MessageInfo |= (uint8)((uint8)DLT_TYPE_CONTROL << 1u);
        /* Specify field fill "MTIN" to the DLT message buffer */
        MessageInfo |= (uint8)((uint8)DLT_CONTROL_RESPONSE << 4u);
    }
    /* Check if the data type of the DLT message buffer is "DLT_LOG_DATA" */
    else
    {
        if (NULL_PTR != CreateExtendedHeader->logInfo)
        {
            /* PRQA S 4442++ */ /* MISRA Rule 10.3 */
            message_type = (uint8)(CreateExtendedHeader->logInfo->options.Dlt_Bits.message_type);
            /* Specify field fill "MSTP" to the DLT message buffer */
            MessageInfo |= (uint8)((uint8)message_type << 1u);
            /* Specify field fill "MTIN" to the DLT message buffer */
            MessageInfo |= (uint8)((uint8)CreateExtendedHeader->logInfo->logLevel << 4u);
            /* PRQA S 4442-- */ /* MISRA Rule 10.3 */
        }
    }
    /* Specify field fill "MSIN" to the DLT message buffer */
    Dlt_MoveWriteIndexCopy(ChannelIndex, MessageInfo);
    /* Specify field fill "NOAR" to the DLT message buffer */
    if ((useVerboseMode == TRUE) && (NULL_PTR != CreateExtendedHeader->logInfo))
    {
        Dlt_MessageArgumentCountType argCount = CreateExtendedHeader->logInfo->argCount;
        Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)argCount);
    }
    else
    {
        Dlt_MoveWriteIndexCopy(ChannelIndex, 0);
    }
    /* Specify field fill "APID" to the DLT message buffer */
    Dlt_ApplicationIDType SwcApplicationId = CreateExtendedHeader->SwcContext.SwcApplicationId;
    Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(SwcApplicationId >> 24u));
    Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(SwcApplicationId >> 16u));
    Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(SwcApplicationId >> 8u));
    Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(SwcApplicationId));
    /* Specify field fill "CTID" to the DLT message buffer */
    Dlt_ContextIDType SwcContextId = CreateExtendedHeader->SwcContext.SwcContextId;
    Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(SwcContextId >> 24u));
    Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(SwcContextId >> 16u));
    Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(SwcContextId >> 8u));
    Dlt_MoveWriteIndexCopy(ChannelIndex, (uint8)(SwcContextId));
#endif
}

#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
