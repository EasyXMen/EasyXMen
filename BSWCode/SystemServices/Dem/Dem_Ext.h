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
**  FILENAME    : Dem_Ext.h                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : API declaration                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef DEM_EXT_H
#define DEM_EXT_H

#include "Dem_CfgTypes.h"
#include "Rte_Dcm_Type.h"

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
extern VAR(uint8, AUTOMATIC) DemOperationCycleStatus[DEM_OPERATION_CYCLE_NUM_BYTE]; /* OperationCycleStatus */

#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
#if (DEM_DTR_NUM > 0)
extern DTRInfoType DTRInfo[DEM_DTR_NUM];
#endif /* DEM_DTR_NUM > 0 */
#if (DEM_RATIO_NUM > 0)
extern IUMPRType IUMPRValue[DEM_RATIO_NUM];
#endif /* DEM_RATIO_NUM > 0 */
#endif /* DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT */

extern VAR(Dem_InternalDataType, AUTOMATIC) DemInternalData;

extern VAR(Dem_EventQueueType, AUTOMATIC) DemEventQueue;

#if (DEM_STORAGE_CONDITION_GROUP_NUM > 0)
/* DemGeneral/DemStorageConditionGroup 0-255 */
extern CONST(uint8, AUTOMATIC)
    DemStorageConditionGroup[DEM_STORAGE_CONDITION_GROUP_NUM][DEM_STORAGE_CONDITION_NUM_BYTE];
#endif /* DEM_STORAGE_CONDITION_GROUP_NUM > 0 */

#if (DEM_ENABLE_CONDITION_GROUP_NUM > 0)
/* DemGeneral/DemEnableConditionGroup 0-255 */
extern CONST(uint8, AUTOMATIC) DemEnableConditionGroup[DEM_ENABLE_CONDITION_GROUP_NUM][DEM_ENABLE_CONDITION_NUM_BYTE];
#endif /* DEM_ENABLE_CONDITION_GROUP_NUM > 0 */

#if (DEM_ENABLE_CONDITION_NUM > 0)
/* DemGeneral/DemEnableCondition 0-255 */
extern CONST(uint8, AUTOMATIC) DemEnableCondition[DEM_ENABLE_CONDITION_NUM_BYTE];
#endif /* DEM_ENABLE_CONDITION_NUM > 0 */

#if (DEM_CALLBACK_DTC_STATUS_CHANGED_NUM > 0)
extern CONST(Dem_TriggerOnDTCStatusType, AUTOMATIC) DemCallbackDTCStatusChanged[DEM_CALLBACK_DTC_STATUS_CHANGED_NUM];
#endif /* DEM_CALLBACK_DTC_STATUS_CHANGED_NUM > 0 */

#if (DEM_CALLBACK_OBD_DTC_STATUS_CHANGED_NUM > 0) /* PRQA S 3332 */ /* MISRA Rule 20.9 */
extern CONST(Dem_TriggerOnDTCStatusType, AUTOMATIC)
    DemCallbackObdDTCStatusChanged[DEM_CALLBACK_OBD_DTC_STATUS_CHANGED_NUM];
#endif /* DEM_CALLBACK_OBD_DTC_STATUS_CHANGED_NUM > 0 */

#if (DEM_CALLBACK_J1939_DTC_STATUS_CHANGED_NUM > 0)
extern CONST(Dem_TriggerOnDTCStatusType, AUTOMATIC)
    DemCallbackJ1939DTCStatusChanged[DEM_CALLBACK_J1939_DTC_STATUS_CHANGED_NUM];
#endif /* DEM_CALLBACK_J1939_DTC_STATUS_CHANGED_NUM > 0 */

#if (DEM_INDICATOR_NUM > 0)
extern VAR(Dem_IndicatorStatusType, AUTOMATIC) DemWIRStatus[DEM_INDICATOR_NUM];
#endif

#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
/* Debounce Counter Base Info Define */
extern VAR(Dem_DebounceCounterInfoType, AUTOMATIC) DemDebounceCounterInfo[DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM];
#endif /* DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0 */

#if (DEM_COMPONENT_NUM > 0)
extern VAR(DemComponentStatusType, AUTOMATIC) DemComponentStatus[DEM_COMPONENT_NUM];
#endif

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/*************************************************************************/
/*
 * Brief               Sets an enable condition.
 * ServiceId           None
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EnableConditionID:This parameter identifies the enable condition.
 *                       ConditionFulfilled: This parameter specifies whether the enable condition
 *                       assigned to the EnableConditionID is fulfilled
 *                       (TRUE) or not fulfilled (FALSE).
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              In case the enable condition could be set successfully the API call returns
 * E_OK. If the setting of the enable condition failed the return value of the function is E_NOT_OK.
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_InterSetEnableCondition(uint8 EnableConditionID, boolean ConditionFulfilled);

/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_SetEnableCondictionProcess(void);

/*************************************************************************/
/*
 * Brief               Dem_SetStorageCondictionProcess
 * ServiceId           --
 * Sync/Async          Synchronous-
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_SetStorageCondictionProcess(void);

/*************************************************************************/
/*
 * Brief               Sets a storage condition.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      StorageConditionID: This parameter identifies the storage condition.
 *                       ConditionFulfilled: This parameter specifies whether the storage condition
 *                       assigned to the StorageConditionID is fulfilled (TRUE) or not
 *                       fulfilled (FALSE).
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              In case the storage condition could be set successfully the API call
 *                      returns E_OK. If the setting of the storage condition failed the
 *                      return value of the function is E_NOT_OK.
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    Dem_InterSetStorageCondition(uint8 StorageConditionID, boolean ConditionFulfilled);

/******************************************************************************/
/*
 * Brief               Initializes the internal states necessary to process events reported by
 * BSW-modules. ServiceId           None Sync/Async          Synchronous Reentrancy          Non
 * Reentrant Param-Name[in]      None Param-Name[out]     None Param-Name[in/out]  None Return None
 */
/******************************************************************************/
extern FUNC(void, DEM_CODE) Dem_InterPreInit(void);

/*************************************************************************/
/*
 * Brief               Initializes or reinitializes this module.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr:Pointer to the configuration set in VARIANT-POSTBUILD.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_InterInit(void);

/*************************************************************************/
/*
 * Brief               Processes the events reported by SW-Cs via RTE.
 * ServiceId           None
 * Sync/Async          Synchronous/Asynchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.
 * Param-Name[in]      EventId:Identification of an event by assigned EventId.
 *                       EventStatus: Monitor test result
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: set of event status was successful
 *                       E_NOT_OK: set of event status failed or could not be accepted
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_InterSetEventStatus(Dem_EventIdType* IntId, Dem_EventStatusType EventStatus);

/*************************************************************************/
/*
 * Brief               Control the internal debounce counter/timer by BSW modules and SW-Cs.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.
 * Param-Name[in]      EventId:Identification of an event by assigned EventId.
 *                       DebounceResetStatus :  Freeze or reset the internal debounce counter
 *                       /timer of the specified event.
 * Param-Name[out]     None...
 * Param-Name[in/out]  None...
 * Return              None...
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE)
    Dem_InterResetEventDebounceStatus(Dem_EventIdType EventId, Dem_DebounceResetStatusType DebounceResetStatus);

/*************************************************************************/
/*
 * Brief               Captures the freeze frame data for a specific event.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the sameEventId.
 * Param-Name[in]      EventId:Identification of an event by assigned EventId
 * Param-Name[out]     None...
 * Param-Name[in/out]  None...
 * Return              None...
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_InterPrestoreFreezeFrame(Dem_EventIdType IntId);

/*************************************************************************/
/*
 * Brief               Clears a prestored freeze frame of a specific event.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.
 * Param-Name[in]      EventId : Identification of an event by assigned EventId.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_InterClearPrestoredFreezeFrame(Dem_EventIdType IntId);

/*************************************************************************/
/*
 * Brief               Gets the failed status of a DemComponent.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ComponentId:Identification of a DemComponent
 * Param-Name[out]     ComponentFailed: TRUE: failed FALSE: not failed
 * Param-Name[in/out]  None
 * Return              E_OK: getting "ComponentFailed" was successful
 *                       E_NOT_OK: getting "ComponentFailed" was not successful
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_InterGetComponentFailed(
    Dem_ComponentIdType ComponentId,
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
#if (DEM_COMPONENT_NUM > 0)
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) ComponentFailed
#else
    P2CONST(boolean, AUTOMATIC, DEM_APPL_DATA) ComponentFailed
#endif
);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the indicator status derived from the event status.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IndicatorId: Number of indicator
 * Param-Name[out]     IndicatorStatus: Status of the indicator, like off, on, or blinking.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful E_NOT_OK: Operation failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_InterGetIndicatorStatus(
    uint8 IndicatorId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_IndicatorStatusType, AUTOMATIC, DEM_APPL_DATA) IndicatorStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Sets the indicator status included in the event status.
 * ServiceId           None
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IndicatorId : Number of indicator
 *                      IndicatorStatus Status of the indicator, like off, on, or blinking.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
 *                       E_NOT_OK: Operation failed or is not supported
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_InterSetIndicatorStatus(
    uint8 IndicatorId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dem_IndicatorStatusType, AUTOMATIC, DEM_APPL_DATA) IndicatorStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the data of a freeze frame by event.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId: Identification of an event by assigned EventId.
 *                       RecordNumber: This parameter is a unique identifier for a freeze
 *                       frame record as defined in ISO15031-5 and ISO14229-1.
 *                       0xFF means most recent freeze frame record is returned.
 *                       DataId : This parameter specifies the PID (ISO15031-5 mapped in UDS range
 *                       0xF400 - 0xF4FF) or DID (ISO14229-1) that shall be copied to
 *                       the destination buffer.
 * Param-Name[out]     DestBuffer : This parameter contains a byte pointer that points to the
 *                       buffer, to which the freeze frame data record shall be written to.
 *                       The format is raw hexadecimal values and contains no header-information.
 * Param-Name[in/out]   BufSize: When the function is called this parameter contains
                            the maximum number of data bytes that can be written to the buffer.
                            The function returns the actual number of written
                            data bytes in this parameter
 * Return              E_OK: Operation was successful
 *                       DEM_E_NODATAAVAILABLE: The requested event data is not currently stored
 *                                              (but the request was valid)
 *                       DEM_E_WRONG_RECORDNUMBER: The requested record number is not supported
 *                                              by the event
 *                       DEM_E_WRONG_DIDNUMBER: The requested DID is not supported by the freeze
 frame
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_InterGetEventFreezeFrameDataEx(
    Dem_EventIdType IntId,
    uint8 RecordNumber,
    uint16 DataId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the data of an extended data record by event.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId : Identification of an event by assigned EventId.
 *                       RecordNumber: Identification of requested Extended data record.
 *                       Valid values are between 0x01 and 0xEF as defined in ISO14229-1.
 * Param-Name[out]     DestBuffer: This parameter contains a byte pointer that points
 *                          to the buffer, to which the extended data shall bewritten to.
 *                          The format is raw hexadecimal values and contains no header-information.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
 *                       DEM_E_NODATAAVAILABLE: The requested event data is not currently
 *                              stored (but the request was valid)
 *                       DEM_E_WRONG_RECORDNUMBER: The requested record number is not supported by
 * the event
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_InterGetEventExtendedDataRecordEx(
    Dem_EventIdType IntId,
    uint8 RecordNumber,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               GetInternalMemDest
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTCOrigin
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              uint8
 */
/*************************************************************************/
static inline FUNC(uint8, DEM_CODE) Dem_GetInternalMemDest(Dem_DTCOriginType DTCOrigin)
{
    uint8 iloop;
    uint8 ret = DEM_MEM_DEST_INVALID;
    for (iloop = 0; iloop < DEM_MEM_DEST_TOTAL_NUM; iloop++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        if (DemMemDestCfg[iloop].ExtId == DTCOrigin)
        {
            ret = iloop;
            break;
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               Set the availability of a specific DemComponent.
 * ServiceId           none
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ComponentId: Identification of a DemComponent.
 *                       AvailableStatus: This parameter specifies whether the respective
 *                          Component shall be available (TRUE) or not (FALSE).
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              E_OK: Operation was successful
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    Dem_InterSetComponentAvailable(Dem_ComponentIdType ComponentId, boolean AvailableStatus);

#if ((DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0u) || (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0u))
/*************************************************************************/
/*
 * Brief               Dem_DebounceFreeze
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/

extern FUNC(void, DEM_CODE) Dem_DebounceFreeze(Dem_EventIdType IntId);
#else
#define Dem_DebounceFreeze(x)
#endif /* DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0u) || (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0u */

#if ((DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) || (DEM_J1939_SUPPORT == STD_ON))
#if (DEM_RATIO_NUM > 0)
/*************************************************************************/
/*
 * Brief               In order to communicate the status of the (additional) denominator
                        conditions among the OBD relevant ECUs, the API is used to forward
                        the condition status to a Dem of a particular ECU
 * ServiceId           None
 * Sync/Async          Synchronous /Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConditionId: Identification of a IUMPR denominator condition ID
                        ConditionStatus:Status of the IUMPR denominator condition
                        (Notreached, reached, not reachable / inhibited)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: set of IUMPR denominator condition was successful
                        E_NOT_OK: set of IUMPR denominator condition
                        failed or could not be accepted.
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    Dem_InterSetIUMPRDenCondition(Dem_IumprDenomCondIdType ConditionId, Dem_IumprDenomCondStatusType ConditionStatus);

/*************************************************************************/
/*
 * Brief               In order to communicate the status of the (additional) denominator
                        conditions among the OBD relevant ECUs, the API is used to retrieve
                        the condition status from the Dem of the ECU where the conditions are
                        computed.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ConditionId: Identification of a IUMPR denominator condition ID
 * Param-Name[out]     ConditionStatus:Status of the IUMPR denominator condition
 *                      (Notreached, reached, not reachable / inhibited)
 * Param-Name[in/out]  None
 * Return              E_OK: get of IUMPR denominator condition status was successful
                        E_NOT_OK: get of condition status failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_InterGetIUMPRDenCondition(
    Dem_IumprDenomCondIdType ConditionId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_IumprDenomCondStatusType, AUTOMATIC, DEM_APPL_DATA) ConditionStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /*DEM_RATIO_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Calculate the OBD related data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_InterCalOBDRelatedValue(void);

/*************************************************************************/
/*
 * Brief               Service is used for requesting IUMPR data according to InfoType 08.
 * ServiceId           Non
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      OpStatus Only DCM_INITIAL will appear, because this API
                        behaves synchronous.
 * Param-Name[out]     Iumprdata08 Buffer containing the number of data elements (as
                        defined in ISO-15031-5) and contents of InfoType
                        08. The buffer is provided by the Dcm.
 * Param-Name[in/out]  Iumprdata08BufferSize: The maximum number of data bytes that can be
                        written to the Iumprdata08 Buffer.
 * Return              Non
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_InterDcmGetInfoTypeValue08(
    Dcm_OpStatusType OpStatus,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata08,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata08BufferSize);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Service is used for requesting IUMPR data according to InfoType 0B.
 * ServiceId           Non
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      OpStatus Only DCM_INITIAL will appear, because this API
                        behaves synchronous.
 * Param-Name[out]     Iumprdata0B Buffer containing the number of data elements (as
                        defined in ISO-15031-5) and contents of InfoType
                        08. The buffer is provided by the Dcm.
 * Param-Name[in/out]  Iumprdata0BBufferSize: The maximum number of data bytes that can be
                        written to the Iumprdata08 Buffer.
 * Return              Non
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_InterDcmGetInfoTypeValue0B(
    Dcm_OpStatusType OpStatus,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata0B,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata0BBufferSize);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Reports the value of a requested "availability-OBDMID" to the DCM upon a
 * Service 06 request. ServiceId           None Sync/Async          Synchronous Reentrancy Reentrant
 * Param-Name[in]      Obdmid Availablity OBDMID (00,20, 40...)
 * Param-Name[out]     Obdmidvalue Bit coded information on the support of OBDMIDs.
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_InterDcmGetAvailableOBDMIDs(uint8 Obdmid, P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) Obdmidvalue);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the number of TIDs per (functional) OBDMID.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Obdmid OBDMID subject of the request to identify the number of assigned TIDs
 * Param-Name[out]     numberOfTIDs Number of assigned TIDs for the requested OBDMID.
 *                      Used as loop value for the DCM to retrieve all OBD/TID result data
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_InterDcmGetNumTIDsOfOBDMID(uint8 Obdmid, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) numberOfTIDs);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets data element per PID and index of the most important freeze frame
                        being selected for the output of service 02. The function stores the data
                        in the provided DestBuffer.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PID: This parameter is an identifier for a PID as defined
                        in ISO15031-5.
                        DataElementIndexOfPID: Data element index of this PID according
                        to the Dcm configuration of service 02. It is zero-based and
                        consecutive, and ordered by the data element  positions
                        (configured in Dcm, refer to SWS_Dem_00597).>
 * Param-Name[out]     None
 * Param-Name[in/out]  DestBuffer: This parameter contains a byte pointer that points to
                        the buffer, to which the data element of the PID shall
                        be written to. The format is raw hexadecimal values
                        and contains no header-information.
                        BufSize: When the function is called this parameter contains
                        the maximum number of data bytes that can be written
                        to the buffer.
                        The function returns the actual number of written
                        data bytes in this parameter.
 * Return              E_OK Freeze frame data was successfully reported
                        E_NOT_OK Freeze frame data was not successfully reported
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_InterDcmReadDataOfOBDFreezeFrame(
    uint8 PID,
    uint8 DataElementIndexOfPID,
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
#if ((DEM_PID_CLASS_NUM > 0) && (DEM_DATA_ELEMENT_CLASS_NUM > 0))
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize
#else
    P2CONST(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2CONST(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize
#endif
);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Reports a DTR data along with TID-value, UaSID, test result with lower and
 upper limit.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Obdmid: Identification of a DTR element by assigned DTRId.
                        TIDindex: Index of the TID within the DEM. Runs from 0
                        to "numberOfTIDs" obtained in the call to Dem_
                        DcmGetNumTIDsOfOBDMID()
 * Param-Name[out]     TIDvalue: TID to be put on the tester reponse
                        UaSID: UaSID to be put on the tester reponse
                        Testvalue:Latest test result
                        Lowlimvalue:Lower limit value associated to the latest test result
                        Upplimvalue:Upper limit value associated to the latest test result
 * Param-Name[in/out]  None
 * Return              E_OK: Report of DTR result successful
                        E_NOT_OK: Report of DTR result failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_InterDcmGetDTRData(
    uint8 Obdmid,
    uint8 TIDindex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) TIDvalue,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) UaSID,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Testvalue,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Lowlimvalue,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Upplimvalue);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Service for reporting that faults are possibly found
                        because all conditions are fullfilled.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Ratio Identifier reporting that a respective monitor
                        could have found a fault
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK report of IUMPR result was successfully reported
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_InterRepIUMPRFaultDetect(Dem_RatioIdType RatioID);

/*************************************************************************/
/*
 * Brief               Service is used to lock a denominator of a specific monitor.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Ratio Identifier reporting that specific denominator
                        is locked
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK report of IUMPR denominator status was successfully reported
                        E_NOK report of IUMPR denominator status was
                        not successfully reported
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_InterRepIUMPRDenLock(Dem_RatioIdType RatioID);

/*************************************************************************/
/*
 * Brief               Service is used to release a denominator of a
                        specific monitor.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Ratio Identifier reporting that specific denominator
                        is released
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK report of IUMPR denominator status was successfully reported
                        E_NOK report of IUMPR denominator status was
                        not successfully reported
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_InterRepIUMPRDenRelease(Dem_RatioIdType RatioID);

#if (DEM_DTR_NUM > 0)
/*************************************************************************/
/*
 * Brief              Reports a DTR result with lower and upper limit. The internal eventstatus
                        serves as master whether the DTR values are forwarded or ignored,
                        also taking the DTRUpdateKind into account.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTRId:Identification of a DTR element by assigned DTRId
 *                      TestResult:Test result of DTR
 *                      LowerLimit:Lower limit of DTR
 *                      UpperLimit:Upper limit of DTR
 *                      Ctrlval:Control value of the DTR to support its interpretation
 Dem-internally.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Report of DTR result successful
                        E_NOT_OK: Report of DTR result failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    Dem_InterSetDTR(uint16 DTRId, sint32 TestResult, sint32 LowerLimit, sint32 UpperLimit, Dem_DTRControlType Ctrlval);
#endif /*DEM_DTR_NUM > 0 */

#endif /* DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT */

#if (DEM_J1939_SUPPORT == STD_ON)
#if (DEM_J1939_READING_DTC_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               The function returns the composite lamp status of the filtered DTCs
 * ServiceId           none
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     LampStatus:Receives the commulated lamp status
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_GetLampStatusOfJ1939DcmSetDTCFilter(P2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_APPL_DATA) LampStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the number of currently filtered DTCs set by the function
 * Dem_J1939DcmSetDTCFilter ServiceId           None Sync/Async          Asynchronous Reentrancy Non
 * Reentrant Param-Name[in]      None Param-Name[out]     NumberOfFilteredDTC: The number of DTCs
 * matching the defined status mask. Param-Name[in/out]  none Return              Status of the
 * operation to retrieve a number of DTC from the Dem
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_InterJ1939DcmGetNumberOfFilteredDTC(
    Dem_EventIdType IntId,
    uint8 entryId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) findSameDTC,
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) IsFind);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the next filtered J1939 DTC.
 * ServiceId           none
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     J1939DTC: Receives the J1939DTC value. If the return value of
                        the function is other than DEM_FILTERED_OK this
                        parameter does not contain valid data.
                        OccurenceCounter:This parameter receives the corresponding occurrence
 counter. If the return value of the function call is other than DEM_FILTERED_OK this parameter does
 not contain valid data.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC from the Dem.
 */
/*************************************************************************/
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_InterJ1939DcmGetNextFilteredDTC(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) J1939DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) OccurenceCounter);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#endif /* DEM_J1939_READING_DTC_SUPPORT == STD_ON */

#if (DEM_J1939_DM31_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               The function sets the filter to the first applicable DTC for the DM31
 * response for a specific node ServiceId           none Sync/Async          Synchronous Reentrancy
 * Non Reentrant Param-Name[in] Nm node Id of requesting client Param-Name[out]     None
 * Param-Name[in/out]  none Return              Status of the operation to retrieve a DTC from the
 * Dem.
 */
/*************************************************************************/
FUNC(boolean, DEM_CODE) Dem_InterJ1939DcmFirstDTCwithLampStatus(uint8 nodeId, uint16 dtcRef);

/*************************************************************************/
/*
 * Brief               Gets the next filtered J1939 DTC for DM31 including current LampStatus.
 * ServiceId           none
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     J1939DTC:Receives the J1939DTC value. If the return value of
                        the function is other than DEM_FILTERED_OK this
                        parameter does not contain valid data.
                        OccurenceCounter:This parameter receives the corresponding occurrence
 counter. If the return value of the function call is other than DEM_FILTERED_OK this parameter does
 not contain valid data.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC from the Dem.
 */
/*************************************************************************/
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_InterJ1939DcmGetNextDTCwithLampStatus(
    uint8 nodeId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) J1939DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) OccurenceCounter,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    Dem_EventIdType IntId);

/*************************************************************************/
/*
 * Brief               Gets the next filtered J1939 DTC for DM31 including current LampStatus.
 * ServiceId           none
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     LampStatus:Receives the lamp status returned by this function.
 *                      If the return value of the function is other than
                        DEM_FILTERED_OK this parameter does not contain valid data.
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_GetLampStatusOfJ1939DcmGetNextDTCwithLampStatus(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
    P2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_APPL_DATA) LampStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#endif /* DEM_J1939_DM31_SUPPORT == STD_ON */

#if (DEM_J1939_EXPANDED_FREEZE_FRAME_SUPPORT == STD_ON)
#if (DEM_J1939_FREEZE_FRAME_SUPPORT == STD_ON)

/*************************************************************************/
/*
 * Brief               Gets next freeze frame data. The function stores the data in the provided
 DestBuffer
 * ServiceId           None
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     J1939DTC:Receives the J1939DTC value. If the return value of
                        the function is other than DEM_FILTERED_OK this
                        parameter does not contain valid data
                        OccurenceCounter:This parameter receives the corresponding occurrence
 counter. If the return value of the function call is other than DEM_FILTERED_OK this parameter does
 not contain valid data.
 * Param-Name[in/out]  DestBuffer:This parameter contains a byte pointer that points
                        to the buffer, to which the freeze frame data record
                        shall be written to.
                        BufSize:When the function is called this parameter contains
                        the maximum number of data bytes that can be written to the buffer.
 * Return              Status of the operation to retrieve freeze frame data by DTC.
 */
/*************************************************************************/
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_InterJ1939DcmGetNextFreezeFrame(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) J1939DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) OccurenceCounter,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#endif /* DEM_J1939_FREEZE_FRAME_SUPPORT == STD_ON */

/*************************************************************************/
/*
 * Brief               Gets next SPN.
 * ServiceId           None
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     SPNSupported: This parameter contains the next SPN in the ExpandedFreezeFrame
                        SPNDataLength: This parameter contains the corresponding dataLength of the
 SPN
 * Param-Name[in/out]  None
 * Return              Status of the operation to retrieve freeze frame data by DTC.
 */
/*************************************************************************/
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_InterJ1939DcmGetNextSPNInFreezeFrame(
    uint16 dtcRef,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) SPNSupported,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) SPNDataLength);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#endif /* DEM_J1939_EXPANDED_FREEZE_FRAME_SUPPORT == STD_ON */

#if (DEM_J1939_RATIO_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               The function sets the Ratio filter for a specific node and returns the
 * corresponding Ignition Cycle Counter and General Denominator.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     SPN:Receives the SPN of the applicaple system monitor. If the return value of
 the function is other than DEM_FILTERED_OK this parameter does not contain valid data.
 Numerator:Receives the Numerator of the applicable system monitor. If the return value of the
 function is other than DEM_FILTERED_OK this parameter does not contain valid data.
                        Denominator:Receives the Denominator of the applicable system
                        monitor. If the return value of the function is other
                        than DEM_FILTERED_OK this parameter does not
                        contain valid data.
 * Param-Name[in/out]  None
 * Return              Status of the operation to (re-)set a DTC filter.
 */
/*************************************************************************/
/* PRQA S 3432,0777,0779++ */ /* MISRA Rule 20.7,5.1,5.2 */
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_InterJ1939DcmGetNextFilteredRatio(
    Dem_EventIdType IntId,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SPN,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Numerator,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Denominator);
/* PRQA S 3432,0777,0779-- */ /* MISRA Rule 20.7,5.1,5.2 */

#endif /* DEM_J1939_RATIO_SUPPORT == STD_ON */

#if (DEM_J1939_READINESS1_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Service to report the value of Diagnostic Readiness 1 (DM05) computed by the
 * Dem. ServiceId           None Sync/Async          Synchronous Reentrancy          Non Reentrant
 * Param-Name[in]      node:Nm node Id of requesting client
 * Param-Name[out]     DataValue:Buffer of 8 bytes containing the contents of Diagnostic Readiness 1
 * (DM05) computed by the Dem. Param-Name[in/out]  None Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_InterOneJ1939DcmReadDiagnosticReadiness1(
    Dem_EventIdType IntId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_J1939DcmDiagnosticReadiness1Type, AUTOMATIC, DEM_APPL_DATA) DataValue,
    uint8 node);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Service to report the value of Diagnostic Readiness 1 (DM05) computed by the
 Dem.
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      node:Nm node Id of requesting client
 * Param-Name[out]     DataValue:Buffer of 8 bytes containing the contents of Diagnostic Readiness 1
 (DM05) computed by the Dem.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
                        E_NOT_OK: Operation failed
 */
/*************************************************************************/
FUNC(void, DEM_CODE)
Dem_InterTwoJ1939DcmReadDiagnosticReadiness1(
    uint8 node,
    uint8 ReadinessGroupIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) supportedGroups,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) notCompletedGroups);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* DEM_J1939_READINESS1_SUPPORT == STD_ON */
#endif              /* DEM_J1939_SUPPORT == STD_ON */

/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE)
    Dem_TriggerOnEventStatus(Dem_EventIdType IntId, Dem_UdsStatusByteType OldStatus, Dem_UdsStatusByteType NewStatus);

/*************************************************************************/
/*
 * Brief               ClearAllDTC
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      memDest
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_ClearAllDTC(uint8 memDest);

#if (DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS)
/*************************************************************************/
/*
 * Brief               ClearGroupDTC
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_ClearGroupDTC(uint8 memDest, uint8 GroupIndex);
#endif /* DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS */

/*************************************************************************/
/*
 * Brief               Dem_ClearDTRInfoByEventId
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_ClearDTRInfoByEventId(Dem_EventIdType IntId);

#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
/*************************************************************************/
/*
 * Brief               Dem_GetFreezeFrame
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     pBuffer
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(void, DEM_CODE) Dem_GetFreezeFrame(Dem_EventIdType IntId, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Dem_GetGeneralFreezeFrame
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      tempRef
 * Param-Name[out]     pBuffer
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_GetGeneralFreezeFrame(uint16 tempRef, uint8* pBuffer);

/*************************************************************************/
/*
 * Brief               Dem_GetJ1939FreezeFrame
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      tempRef
 * Param-Name[out]     pBuffer
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(void, DEM_CODE) Dem_GetJ1939FreezeFrame(uint16 tempRef, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Dem_GetObdFreezeFrame
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     pBuffer
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(void, DEM_CODE) Dem_GetObdFreezeFrame(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Dem_CheckFreezeFrameIsStorage
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && Trigger
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/*************************************************************************/
extern FUNC(boolean, DEM_CODE) Dem_CheckFreezeFrameIsStorage(Dem_EventIdType IntId, uint8 Trigger);

/*************************************************************************/
/*
 * Brief               Dem_FreezeFrameCopy
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && pEventBuffer
 * Param-Name[out]     pFFBuffer
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_FreezeFrameCopy(
    Dem_EventIdType IntId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pFFBuffer
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */);
#endif /* DEM_FREEZE_FRAME_CLASS_NUM > 0 || DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0 || \
          DEM_PID_CLASS_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Dem_ExtendedDataGetFromEntry
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEntry && RecordNum
 * Param-Name[out]     pBuffer
 * Param-Name[in/out]  BufSize
 * Return              pBuffer
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_ExtendedDataGetFromEntry(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry,
    uint8 RecordNum,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    uint16* BufSize);

#if (DEM_EXTENDED_DATA_CLASS_NUM > 0)

/*************************************************************************/
/*
 * Brief               Dem_CheckExtendedDataIsStorage
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && Trigger
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/*************************************************************************/
extern FUNC(boolean, DEM_CODE) Dem_CheckExtendedDataIsStorage(Dem_EventIdType IntId, uint8 Trigger);
#endif /* DEM_EXTENDED_DATA_CLASS_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Dem_EventQueueProcessWithDebounce
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_EventQueueProcessWithDebounce(Dem_EventIdType IntId);

/*************************************************************************/
/*
 * Brief               Dem_EventTestFailedProcessOfComponent
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_EventTestFailedProcessOfComponent(
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
#if (DEM_COMPONENT_NUM > 0)
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
#else
    P2CONST(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
#endif
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg);

/*************************************************************************/
/*
 * Brief               Dem_GetMemDestMaxNumOfDtc
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/*************************************************************************/
extern FUNC(uint8, DEM_CODE) Dem_GetMemDestMaxNumOfDtc(void);

/*************************************************************************/
/*
 * Brief               Dem_GetDemWIRStatus
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/*************************************************************************/
extern FUNC(Dem_IndicatorStatusType, DEM_CODE) Dem_GetDemWIRStatus(uint8 DemIndicatorRef);

/*************************************************************************/
/*
 * Brief               Dem_CheckWIROn
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEvent && pEventCfg
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/*************************************************************************/
extern FUNC(boolean, DEM_CODE) Dem_CheckWIROn(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg);

/*************************************************************************/
/*
 * Brief               Dem_CheckWIROff
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEvent && pEventCfg
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/*************************************************************************/
extern FUNC(boolean, DEM_CODE) Dem_CheckWIROff(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg);

/*************************************************************************/
/*
 *  Brief               Dem_ResponseOnDtcStatusChange
 *  ServiceId           --
 *  Sync/Async          Synchronous
 *  Reentrancy          Reentrant
 *  Param-Name[in]      IntId
 *  Param-Name[out]     None
 *  Param-Name[in/out]  None
 *  Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_ResponseOnDtcStatusChange(Dem_EventIdType IntId);

/*************************************************************************/
/*
 * Brief               Dem_InterEventTestPassed
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && pEventBuffer
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_InterEventTestPassed(Dem_EventIdType IntId);

/*************************************************************************/
/*
 * Brief               Dem_InterOperationCycleStart
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_InterOperationCycleStart(Dem_EventIdType IntId);

/*************************************************************************/
/*
 * Brief               Dem_CallDataChangedOfMemEntryAllocate
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_CallDataChangedOfMemEntryAllocate(Dem_EventIdType IntId);

/*************************************************************************/
/*
 * Brief               Dem_ClearEventRelateInformation
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEvent
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern void Dem_ClearEventRelateInformation(P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Dem_DeleteRelatedData
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(void, DEM_CODE) Dem_DeleteRelatedData(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Dem_EventDataStorageTrigger
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && pEventBuffer && Trigger
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_EventDataStorageTrigger(Dem_EventIdType IntId, uint8 Trigger);

/*************************************************************************/
/*
 * Brief               Dem_DebounceInit
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_DebounceInit(void);

/*************************************************************************/
/*
 * Brief               Dem_DebounceReset
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_DebounceReset(Dem_EventIdType IntId);

/*************************************************************************/
/*
 * Brief               Dem_GetInteralFDC
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              sint8
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_GetInteralFDC(Dem_EventIdType IntId, P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA) FaultDetectionCounter);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Dem_DebounceProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEventBuffer
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Dem_EventStatusType
 */
/*************************************************************************/
extern FUNC(Dem_EventStatusType, DEM_CODE) Dem_DebounceProcess(Dem_EventDataBufferType pEventBuffer);

/*************************************************************************/
/*
 * Brief               Dem_DebounceTimerMain
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_DebounceTimerMain(void);

#if (DEM_NVRAM_BLOCKID_NUM > 0)
/*************************************************************************/
/*
 * Brief               Dem_IntWriteNvRAM
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_InterIntWriteNvRAM(
    uint16 iloop,
    /* PRQA S 3432,3673++ */ /* MISRA Rule 8.13,20.7 */
    P2VAR(Dem_EventRelateInformationStorageType, AUTOMATIC, DEM_APPL_DATA) InformationStorage);
/* PRQA S 3432,3673-- */ /* MISRA Rule 8.13,20.7 */

/*************************************************************************/
/*
 * Brief               Dem_InterIntReadNvRAM
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  pEvent
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_InterIntReadNvRAM(
    uint16 iloop,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dem_EventRelateInformationStorageType, AUTOMATIC, DEM_APPL_DATA) InformationStorage);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* DEM_NVRAM_BLOCKID_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Sets a freeze frame record filter.
 * ServiceId           none
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTCFormat: Defines the output-format of the requested DTC values for the
 *                           sub-sequent API calls.
 * Param-Name[out]     NumberOfFilteredRecords:Number of freeze frame records currently stored
 *                           in the event memory.
 * Param-Name[in/out]  none
 * Return              Status of the operation to (re-)set a freeze frame record filter.
 */
/*************************************************************************/
extern FUNC(Dem_ReturnSetFilterType, DEM_CODE) Dem_InterDcmSetFreezeFrameRecordFilter(
    Dem_DTCFormatType DTCFormat,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredRecords);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the next freeze frame record number and its associated DTC
 *                          stored in the event memory.
 * ServiceId           none
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     DTC:Receives the DTC value in respective format of the filter
 *                          returned by this function.
 *                       RecordNumber:Freeze frame record number of the reported DTC
 *                          (relative addressing).
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC and its associated
 *                          snapshot record number from the Dem.
 */
/*************************************************************************/
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_InterDcmGetNextFilteredRecord(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) RecordNumber);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets freeze frame data by DTC. The function stores the data
 *                          in the provided DestBuffer.
 * ServiceId           Non
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC : Diagnostic Trouble Code in UDS format.
 *                       DTCOrigin:If the Dem supports more than one event memory,
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 *                       RecordNumber:This parameter is a unique identifier for a freeze
 *                          frame record as defined in ISO 15031-5 and ISO 14229-1.
 * Param-Name[out]     DestBuffer:This parameter contains a byte pointer that points
 *                          to the buffer, to which the freeze frame data record
 *                          shall be written to.
 * Param-Name[in/out]  BufSize:When the function is called this parameter contains
 *                          the maximum number of data bytes that can be written to the buffer.
 * Return              Status of the operation to retrieve freeze frame data by DTC.
 */
/*************************************************************************/
extern FUNC(Dem_ReturnGetFreezeFrameDataByDTCType, DEM_CODE) Dem_InterDcmGetFreezeFrameDataByDTC(
    uint32 DTC,
    uint16 memDestAndRecordNumber,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the size of freeze frame data by DTC.
 * ServiceId           none
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC : Diagnostic Trouble Code in UDS format
 *                       DTCOrigin : If the Dem supports more than one event memory,
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 *                       RecordNumber: This parameter is a unique identifier for a freeze
 *                          frame record as defined in ISO 15031-5 and ISO 14229-1.
 * Param-Name[out]     SizeOfFreezeFrame:
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(Dem_ReturnGetSizeOfDataByDTCType, DEM_CODE) Dem_InterDcmGetSizeOfFreezeFrameByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 RecordNumber,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfFreezeFrame);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ExtendedDataNumber
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              const Dem_ExtendedDataRecordClassType*
 */
/*************************************************************************/
extern FUNC(P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST), DEM_CODE)
    Dem_GetExtendedDataRecordCfg(uint8 ExtendedDataNumber);

/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      IntId && Status
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(Dem_ReturnGetExtendedDataRecordByDTCType, DEM_CODE) Dem_GetExtendedRecordDataLoop(
    uint32 DTC,
    uint16 memDestAndExtendedDataNumber,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the size of extended data by DTC.
 * ServiceId           none
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC:Diagnostic Trouble Code in UDS format.
 *                      DTCOrigin:If the Dem supports more than one event memory ,
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 *                      ExtendedDataNumber:Identification/Number of requested extended
 *                          data record.
 * Param-Name[out]     SizeOfExtendedDataRecord:Size of the requested extended data record(s)
 *                          including record number size
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve the size of extended data.
 */
/*************************************************************************/
extern FUNC(Dem_ReturnGetSizeOfDataByDTCType, DEM_CODE) Dem_InterDcmGetSizeOfExtendedDataRecordByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 ExtendedDataNumber,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Disables the DTC setting for a DTC group.
 * ServiceId           none
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTCGroup:Defines the group of DTC that shall be disabled to
 *                          store in event memory.
 *                       DTCKind:This parameter defines the requested DTC kind, either
 *                          only OBD-relevant DTCs or all DTCs>
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Returns status of the operation>
 */
/*************************************************************************/
extern FUNC(Dem_ReturnControlDTCSettingType, DEM_CODE)
    Dem_InterDcmDisableDTCSetting(uint32 DTCGroup, Dem_DTCKindType DTCKind);

/*************************************************************************/
/*
 * Brief               Enables the DTC setting for a DTC group.
                        This API is intended for the Dcm. It can only be used through the RTE
                        (due to work-around described below SWS_Dem_00035), and therefore
                        no declaration is exported via Dem_Dcm.h.
 * ServiceId           none
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTCGroup:Defines the group of DTC that shall be enabled to
 *                          store in event memory.
 *                       DTCKind:This parameter defines the requested DTC kind,
 *                          either only OBD-relevant DTCs or all DTCs
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Returns the status of the operation
 */
/*************************************************************************/
extern FUNC(Dem_ReturnControlDTCSettingType, DEM_CODE)
    Dem_InterDcmEnableDTCSetting(uint32 DTCGroup, Dem_DTCKindType DTCKind);

/*************************************************************************/
/*
 * Brief               Dem_GetDemMalfunctionLampIndicator
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/*************************************************************************/
extern FUNC(uint8, DEM_CODE) Dem_GetDemMalfunctionLampIndicator(void);

#if (DEM_J1939_SUPPORT == STD_ON)
#if (DEM_J1939_READING_DTC_SUPPORT == STD_ON)
#if (DEM_INDICATOR_NUM > 0)
/*************************************************************************/
/*
 * Brief               The function returns the composite lamp status of the filtered DTCs
 * ServiceId           none
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     LampStatus:Receives the commulated lamp status
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_SubGetLampStatusOfJ1939DcmSetDTCFilter(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_APPL_DATA) LampStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#endif /* DEM_INDICATOR_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Gets the number of currently filtered DTCs set by the function
 * Dem_J1939DcmSetDTCFilter ServiceId           0x91 Sync/Async          Asynchronous Reentrancy Non
 * Reentrant Param-Name[in]      None Param-Name[out]     NumberOfFilteredDTC: The number of DTCs
 * matching the defined status mask. Param-Name[in/out]  none Return              Status of the
 * operation to retrieve a number of DTC from the Dem
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_SubInterJ1939DcmGetNumberOfFilteredDTC(
    Dem_EventIdType IntId,
    uint8 entryId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) findSameDTC,
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) IsFind);
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */

#endif /* DEM_J1939_SUPPORT == STD_ON */
#endif /* DEM_J1939_READING_DTC_SUPPORT == STD_ON */

#if (DEM_INDICATOR_NUM > 0)
/*************************************************************************/
/*
 * Brief               Gets the next filtered J1939 DTC for DM31 including current LampStatus.
 * ServiceId           none
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     LampStatus:Receives the lamp status returned by this function.
 *                      If the return value of the function is other than
                        DEM_FILTERED_OK this parameter does not contain valid data.
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_SubGetLampStatusOfJ1939DcmGetNextDTCwithLampStatus(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dem_IndicatorAttributeType, AUTOMATIC, DEM_APPL_DATA) pWIRAttr,
    P2CONST(Dem_EventRelateInformationType, AUTOMATIC, DEM_APPL_DATA) pEvent,
    P2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_APPL_DATA) LampStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#endif /* DEM_INDICATOR_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Dem_DebouceProcessPrePassedCounter
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#if (DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
extern FUNC(Dem_EventStatusType, DEM_CODE) Dem_DebouceProcessPrePassedCounter(
    P2CONST(Dem_DebounceCounterBasedClassType, AUTOMATIC, DEM_CONST) pCfgCounter,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_APPL_DATA) pCounter);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Dem_DebouceProcessPreFailedCounter
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(Dem_EventStatusType, DEM_CODE) Dem_DebouceProcessPreFailedCounter(
    P2CONST(Dem_DebounceCounterBasedClassType, AUTOMATIC, DEM_CONST) pCfgCounter,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_APPL_DATA) pCounter);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0 */

#if (DEM_RATIO_NUM > 0)
/*************************************************************************/
/*
 * Brief               Get the IUMPR Group Denomitor data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(uint16, DEM_CODE) Dem_GetIUMPRDenValueByGroup(Dem_IUMPRGroupType Dem_IUMPRGroup);
/*************************************************************************/
/*
 * Brief               Get the IUMPR calculated Numerator data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(uint16, DEM_CODE) Dem_GetIUMPRNumValueByGroup(Dem_IUMPRGroupType Dem_IUMPRGroup);
#endif

#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
/*************************************************************************/
/*
 * Brief               Dem_DcmGetFreezeFrameDataByDTCProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_DcmGetFreezeFrameDataByDTCProcess(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(uint8, AUTOMATIC, DEM_APPL_DATA) pFFData,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    Dem_EventIdType IntId,
    boolean isGlobalFF);
#endif /* DEM_FREEZE_FRAME_CLASS_NUM > 0 || DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0 || \
          DEM_PID_CLASS_NUM > 0 */

#if (DEM_EXTENDED_DATA_CLASS_NUM > 0)
/*************************************************************************/
/*
 * Brief               Dem_SetDTCSettingProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_DcmGetSizeOfExtDataRecLoop(
    uint16 dtcRef,
    uint8 memDest,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtDataCfg);
#endif

#if ((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
/*************************************************************************/
/*
 * Brief               Dem_FreezeFrameStorage
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && pEventBuffer && Trigger
 * Param-Name[out]     None
 * Param-Name[in/out]  pEntry
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_FreezeFrameStorage(
    Dem_EventIdType IntId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    uint8 Trigger);

/*************************************************************************/
/*
 * Brief               Dem_FreezeFrameGetByRecordNum
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEntry && RecordNum
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Dem_FreezeFrameInfoType*
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_APPL_DATA), DEM_CODE)
    Dem_FreezeFrameGetByRecordNum(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry, uint8 RecordNum);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* DEM_FREEZE_FRAME_CLASS_NUM > 0 || DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0 || \
                       DEM_PID_CLASS_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Dem_FreezeFrameGetFromEntry
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEntry && RecordNum && Did
 * Param-Name[out]     None
 * Param-Name[in/out]  pBuffer && BufSize
 * Return              Std_ReturnType
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_FreezeFrameGetFromEntry(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_APPL_DATA) pEntry,
    uint8 RecordNum,
    uint16 Did,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) pBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#endif /* ifndef DEM_EXT_H */
