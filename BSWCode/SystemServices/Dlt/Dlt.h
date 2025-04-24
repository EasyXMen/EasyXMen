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
**  DESCRIPTION : Public extern FUNCtions declared of Dlt                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef RTE_DLT_H_
#define RTE_DLT_H_

#include "Dlt_Cbk.h"
#include "Dlt_internal.h"

/*=======[R E V I S I O N   H I S T O R Y]====================================*
*  <VERSION>    <DATE>       <AUTHOR>    <REVISION LOG>
*  V2.0.0       2021-4-8    tao.yu      Initial version
*  V2.0.1       2023-5-30   tao.yu      fix DLT_TIME_STAMP_SUPPORTp configuration item and hange version info
*  V2.0.2       2023-6-30   haibin.shao fix The array overboundary and Pdu signal route configuration item generation
*  V2.0.3       2023-7-19   haibin.shao Modify the parameter detection processing logic.
*  V2.0.4       2023-10-18  haibin.shao Modify the Macro compiler switch detection logic(Dlt_ServicesSyncTimeStamp).
*  V2.0.5       2023-10-18  haibin.shao Modify "If DLT is not initialized, there are no need to report an error to DET".
rules caused by SWC are not configured
 * V2.0.6       2023-8-3    tao.yu       QAC change
 * V2.0.7       2023-10-18  haibin.shao Modify the Macro compiler switch detection logic(Dlt_ServicesSyncTimeStamp).
 * V2.0.8       2023-12-10  haibin.shao Modify the optimize for DLT and upload CUnit.
 * V2.0.9       2023-12-22  haibin.shao Improve the handling of insufficient buffers and update CUnit.
 * V2.0.10      2023-12-27  haibin.shao     Added QAC rule violation remarks and adj NvM interaction
 * V2.0.11      2024-02-02  xue.han     QAC
 * V2.0.12      2024-07-04  haibin.shao update data type for cunit-big-end
 * V2.0.13      2024-08-13  haibin.shao Append QAC remarks and remove main function declarations from header files
 * V2.0.14      2024-09-12  haibin.shao Adjust the interface condition judgment

============================================================================*/
/*==============================================================================
*                              SOURCE FILE VERSION INFORMATION
==============================================================================*/
#define DLT_VENDOR_ID                62U
#define DLT_MODULE_ID                55U
#define DLT_INSTANCE_ID              0U
#define DLT_AR_RELEASE_MAJOR_VERSION 4U
#define DLT_AR_RELEASE_MINOR_VERSION 5U
#define DLT_AR_RELEASE_PATCH_VERSION 0U
#define DLT_SW_MAJOR_VERSION         2U
#define DLT_SW_MINOR_VERSION         0U
#define DLT_SW_PATCH_VERSION         14U

#if (DLT_SWC_NUM > 0)
extern CONST(Dlt_SwcType, DLT_CONST) Dlt_Swc;
#endif /* DLT_SWC_NUM > 0 */
extern CONST(Dlt_ConfigType, DLT_CONST) Dlt_Config;

/*
 * Brief               Dlt is using the NVRamManager and is to be initialized very
 *                     late in the ECU startup phase. The Dlt_Init() extern FUNCtion should
 *                     be called after the NVRamManager is initialized.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      config: Pointer to a DLT configuration structure
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 */
extern FUNC(void, DLT_APPL_CODE) Dlt_Init(P2CONST(Dlt_ConfigType, AUTOMATIC, DLT_APPL_CONST) config);

#if (DLT_VERSION_INFO_API == STD_ON)

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
extern FUNC(void, DLT_APPL_CODE) Dlt_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DLT_APPL_DATA) versioninfo);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#endif

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
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_SendTraceMessage(
    Dlt_SessionIDType sessionId,
    P2CONST(Dlt_MessageTraceInfoType, AUTOMATIC, DLT_APPL_CONST) traceInfo,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) traceData,
    uint16 traceDataLength);

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
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_SendLogMessage(
    Dlt_SessionIDType sessionId,
    P2CONST(Dlt_MessageLogInfoType, AUTOMATIC, DLT_APPL_CONST) logInfo,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) logData,
    uint16 logDataLength);

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
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_RegisterContext(
    Dlt_SessionIDType sessionId,
    Dlt_ApplicationIDType appId,
    Dlt_ContextIDType contextId,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) appDescription,
    uint8 lenAppDescription,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) contextDescription,
    uint8 lenContextDescription);

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
extern FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_UnregisterContext(Dlt_SessionIDType sessionId, Dlt_ApplicationIDType appId, Dlt_ContextIDType contextId);

#if (DLT_DET_FORWARD_TO_DET_SUPPORT == STD_ON)

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
extern FUNC(void, DLT_APPL_CODE)
    Dlt_DetForwardErrorTrace(uint16 moduleId, uint8 instanceId, uint8 apiId, uint8 errorId);

#endif

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
extern FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_SetLogLevel(Dlt_ApplicationIDType appId, Dlt_ContextIDType contextId, Dlt_MessageLogLevelType newLogLevel);

/*
 * Brief               The service Dlt_SetTraceStatus sets the trace status for a specific tuple of ApplicationID and
 * ContextID. ServiceId           0x09 Sync/Async          Synchronous Reentrancy          Non Reentrant Param-Name[in]
 * appId:ID of the SW-C contextId:ID of the context newLogLevel:new log level to set Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType:E_OK: No error occurred
 *                     E_NOT_OK: LogLevel could not be changed
 */
extern FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_SetTraceStatus(Dlt_ApplicationIDType appId, Dlt_ContextIDType contextId, boolean newTraceStatus);

/*
 * Brief               Called to request information about registered ApplicationIds, their ContextIds and the
 * corresponding log level. ServiceId           0x0a Sync/Async          Synchronous Reentrancy          Non Reentrant
 * Param-Name[in]      options: Used to filter the response in respect to the ApplicationId, Context Id and Trace Status
 * information appId:Representation of the ApplicationId contextId:Representation of the ContextId Param-Name[out]
 * status logInfo:Details about the returned Application IDs Param-Name[in/out]  N/A Return Std_ReturnType:E_OK: No
 * error occurred E_NOT_OK:  LogInfo could not be returned
 */
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_GetLogInfo(
    uint8 options,
    Dlt_ApplicationIDType appId,
    Dlt_ContextIDType contextId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) status,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    Dlt_LogInfoType* logInfo);

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
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_GetDefaultLogLevel(Dlt_MessageLogLevelType* defaultLogLevel);

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
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_StoreConfiguration(void);

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
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_ResetToFactoryDefault(void);

/*
 * Brief               Switches on/off the message filtering extern FUNCtionality of the Dlt module.
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
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_SetMessageFiltering(boolean status);

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
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_SetDefaultLogLevel(Dlt_MessageLogLevelType newLogLevel);

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
extern FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_SetDefaultTraceStatus(boolean newTraceStatus, Dlt_LogChannelNameType logChannelName);

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
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_GetDefaultTraceStatus(
    Dlt_LogChannelNameType logChannelName,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(boolean, AUTOMATIC, DLT_APPL_DATA) traceStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*
 * Brief               The caller provides the number of logChannelNames to be returned.
 *                     The extern FUNCtion returns the requested amount of LogChannelNames and updates
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
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_GetLogChannelNames(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) numberOfLogChannels,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    Dlt_LogChannelNameType* logChannelNames);

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
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_GetTraceStatus(
    Dlt_ApplicationIDType appId,
    Dlt_ContextIDType contextId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(boolean, AUTOMATIC, DLT_APPL_DATA) traceStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

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
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_SetLogChannelAssignment(
    Dlt_ApplicationIDType appId,
    Dlt_ContextIDType contextId,
    Dlt_LogChannelNameType logChannelName,
    Dlt_AssignmentOperation addRemoveOp);

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
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_SetLogChannelThreshold(
    Dlt_LogChannelNameType logChannelName,
    Dlt_MessageLogLevelType newThreshold,
    boolean newTraceStatus);

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
extern FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_GetLogChannelThreshold(
    Dlt_LogChannelNameType logChannelName,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dlt_MessageLogLevelType, AUTOMATIC, DLT_APPL_DATA) logChannelThreshold,
    P2VAR(boolean, AUTOMATIC, DLT_APPL_DATA) traceStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#endif /* RTE_DLT_H_ */
