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
**  FILENAME    : Dem.h                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : Type definition and API declaration of DEM                  **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

#ifndef DEM_H
#define DEM_H

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>     <AUTHOR>        <REVISION LOG>
 *  V1.0.0       2018-4-20   tao.yu         Initial version
 *  V1.0.1       2019-9-17   tao.yu         fix some bug,change event callback
 *  V1.0.2       2019-12-25  tao.yu         QAC fix
 *  V1.0.3       2020-1-7    tao.yu         Commercial project problem modification
 *  V1.0.4       2020-3-10   tao.yu         enable condition has no impact on Dem_ResetEventDebounceStatus,
 *                                          Dem_ResetEventStatus and Dem_<...>ClearDTC.
 *  V2.0.0       2020-07-15  pengfei.zhu    fix bugs by PIM; optimized code
 *  V2.0.1       2020-09-15  pengfei.zhu    QAC test
 *  V2.1.0       2020-09-15  pengfei.zhu    QAC test
 *  V2.1.0       2022-8-18   tao.yu         change to PBPC mode
 *  V2.1.1       2023-7-26   xue.han        fix CPT-6195:DCTSetting reponse NRC
 *  V2.1.2       2023-8-10   xue.han        QAC change
 *  V2.1.3       2023-8-7    peng.wu        fix CPT-6021 CPT-6375 CPT-6373 Modify macro judgment in Dem_DebounceFreeze,
 *                                          Update Cfg Req
 *  V2.1.4       2023-8-18   tao.yu         fix Dem_FreezeFrameGetFromEntry/Dem_InterDcmGetFreezeFrameDataByDTC
 *  V2.1.5       2023-08-21  peng.wu        fix CPT-6470, Update DTC status in Dem_EventQueueAddDebounceFailed
 *  V2.1.6       2023-8-18   tao.yu         fix Dem_DeleteRelatedData init FF
 *  V2.1.7       2023-9-5    peng.wu        1. Update version info and Dem_GetVersionInfo
 *                                          2. Fix compiler error CPT-6909 CPT-6911
 *               2023-9-18   tao.yu         remove warning
 *  V2.1.8       2023-10-8   peng.wu        CPT-7100, Add DEM_IMMEDITE_CHILD_COMPONENT_MAX_NUM macro switch
 *               2023-10-9   xue.han        Fix compiler error CPT-6599
 *               2023-10-11  xue.han        CPT-6958, Fix Operation Cycle Autostart stratrgy during Dem init
 *               2023-10-16  tao.yu         CPT-7189, Fix compilation error when reset CD on overflow is not enabled
 *               2023-10-19  peng.wu        1.CPT-7101, Fix the macro definition value of OCCURRENCE/FAULT
 *                                          2.CPT-7217, Fix requests for DTC index values
 *               2023-10-23  tao.yu         Enter queue to add protection
 *               2023-10-24  peng.wu        CPT-7160, Fix data misalignment when reading snapshot data
 *               2023-10-25  peng.wu        CPT-7383, Resets the the obtained time variable in Dem_DcmGetNextFilteredDTC
 *
 *               2023-10-25  xue.han        CPT-7129, Fix Dem_IntReadNvRAM Array overflow
 *  V2.1.9       2023-11-06  xue.han        CPT-7491, Fix Dem_DcmSubClearDTC() and internal func Array overflow
 *               2023-11-10  xue.han        CPT-7501, Fix Dem Compile warning
 *               2023-11-15  peng.wu        CPT-7614, Fix AllGroupIsEnabled to TRUE in Dem_InterDcmEnableDTCSetting
 *               2023-11-16  xue.han        CPT-7554, Change DemPbCfgPtr used by Dem_ReportErrorStatus to DemPbCfg.
 *               2023-11-14  haibin.shao    CPT-7597, Changed Dem_J1939DcmGetNextFilteredDTC() and clear flag
 *               2023-11-16  tao.yu         CPT-7638, fix time order Param error
 *               2023-11-29  xue.han        CPT-7723, fix internal eventId usage error
 *               2023-11-30  tao.yu         CPT-7597, fix 1939DTC and MIL
 *  V2.1.10      2023-12-01  xue.han        1��CPT-7736��fix OCC4,0CC6 increase since DTC information was last cleared
 *                                          2��CPT-7742, add macro DemClearDTCLimitation in Dem_DcmCheckClearParameter()
 *               2023-12-11  xue.han        CPT-7818, fix DTC suppression
 *               2023-12-19  tao.yu         CPT-7899, fix combined DTC Aged clear other event bit3
 *               2023-12-19  tao.yu         Fix Compile warning,Modify the environment variable to get on storage
 * V2.1.11       2024-01-15  xue.han        Sync performence optimize
 *                                          CPT-7918,CPT-7915,fix Event Abstime on aging/EventPass/Displacement/Allocate
 *                                          CPT-7870,CPT-7881,fix 1902 Dtc timeorder wrong
 *                                          CPT-7916,fix DebounceTimer compilation errors
 *               2024-02-23  xue.han        QAC
 *               2024-03-20  xue.han        CPT-7786,add Global Freeze Frame process
 * V2.1.12       2024-04-08  tao.yu         CPT-8691,add clear noti fim
 *               2024-04-16  peng.wu        CPT-7787,CPT-7788,CPT-7789, Add SAIC requirements
 *               2024-04-17  xue.han        CPT-8762,fix NRC31 when read ExtData
 *                                          CPT-8766,fix InternalDebounceCounter when jump up/down
 *                                          CPT-8648,remove Redundant variable in Dem_SetDTCSuppression
 *                                          CPT-8637,add DEM_MAX_FDC_SINCE_LAST_CLEAR storage
 *               2024-04-28  xue.han        1. CPT-8858, Clean MaxFDCDuringCurrentCycle when UDS0x14
 *                                          2. CPT-8888, Add FDC Storage Trigger in Dem_DebounceTimerMain
 *               2024-04-30  xue.han        QAC
 *               2024-05-06  peng.wu        1. CPT-8732,CPT-8734 Fixed some macro inclusion issues
 *                                          2. CPT-8816, Fix TFBITCHANGE triggered correctly when entering pass
 *               2024-05-07  xue.han        fix IntId in Dem_DebounceTimerMain
 *               2024-05-09  peng.wu        Update on 16-bit platform
 *               2024-05-11  xue.han        CPT-8823, Return DEM_CLEAR_OK after the erasing of non-volatile-block
 *               2024-05-16  peng.wu        1.CPT-9029, Fix redefined Api
 *               2024-05-21  xue.han        fix DTC reading error when DTC number is greater than 255
 *               2024-06-17  xue.han        CPT-8888,fix event storage which configed FDC Trigger and time debounce
 *               2024-06-20  peng.wu        CPT-9052,CPT-8623, Fix warning about Det while compiling
 *               2024-06-27  peng.wu        CPT-9357, Fix compiler error when Det is off
 *               2024-07-10  xue.han        CPT-9396, Fix Dem_TriggerOnEventStatusType
 *               2024-07-18  xue.han        CPT-8823, Fix clearDTC when pending
 *               2024-07-26  peng.wu        1. CPT-8226, Update Geely requirements
 *                                          2. CPT-8227, Update the clearing mechanism of OCC6
 *               2024-08-07  peng.wu        CPT-10180, Fix multiple fetch of FreezeFrame data
 *               2024-08-13  peng.wu        Append QAC remarks and remove main function declarations from header files
 *               2024-08-14  peng.wu        Update config file, delete copy buffer operation when calling NvM
 * V2.1.13       2024-09-19  tao.yu         fix DemTypeOfFreezeFrameRecordNumeration is DEM_FF_RECNUM_CALCULATED,
 *                                          FF Dem_InterDcmGetFreezeFrameDataByDTC return value errer
 *               2024-11-02  peng.wu        CPT-10893, Fixed error in passing the attribute parameter of the DTC
 *               2024-11-06  peng.wu        CPT-11153, Fixed max J1939 DTCOC value to 0x7E
 *               2024-11-18  peng.wu        CPT-11405, Fixed data read by NvM is deleted during Dem initialization
 *               2024-11-22  xue.han        QAC
 *               2024-12-18  peng.wu        CPT-12007, Fixed null pointer judgment
 *                                          CPT-12011, Fixed Dem_InterDcmGetSizeOfFreezeFrameByDTC return value
 *               2024-12-24  peng.wu        CPT-12101, add DTC reference judgment
 *                                          CPT-11960,CPT-11959, Fixd compile problem
 *               2025-02-26  peng.wu        CPT-12316, Fixd the return result judgment of NvM during initialization
 *               2025-03-8   tao.yu         fix error of use iloop in Dem_TriggerOnEventStatus
 * V2.1.14       2024-09-19  tao.yu         remove Dem_EventQueueAddDebounceFailed and Dem_EventQueueAddDebouncePass,
 *                                          change debounce counter to sys
 * V2.1.15       2025-04-14  peng.wu        Fixd error data type definition
 *               2025-04-21  peng.wu        CPT-13982, Add the judgment that eventid is not zero
 *                           peng.wu        CPT-13926, The problem of reading frozen frames in OBD
 *                                          CPT-13913, Reset DemMemDestInfo when reading fails
 *                                          CPT-13904, Do not set UDS bit6 in Dem_SubInit
 *******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Dem_Ext.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define DEM_VENDOR_ID                62U
#define DEM_MODULE_ID                54U
#define DEM_INSTANCE_ID              1U
#define DEM_AR_RELEASE_MAJOR_VERSION 4U
#define DEM_AR_RELEASE_MINOR_VERSION 2U
#define DEM_AR_RELEASE_PATCH_VERSION 2U
#define DEM_SW_MAJOR_VERSION         2U
#define DEM_SW_MINOR_VERSION         1U
#define DEM_SW_PATCH_VERSION         15U

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

extern CONST(Dem_ConfigType, AUTOMATIC) DemPbCfg; /*for Dem_ReportErrorStatus*/
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
/******************************************************************************/
/*
 * Brief               Initializes the internal states necessary to process events reported by
 * BSW-modules. ServiceId           0x01 Sync/Async          Synchronous Reentrancy          Non
 * Reentrant Param-Name[in]      ConfigPtr: Pointer to Post build configuration data.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
extern FUNC(void, DEM_CODE) Dem_PreInit(void);
/*************************************************************************/
/*
 * Brief               Initializes or reinitializes this module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr:Pointer to the configuration set in VARIANT-POSTBUILD.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_Init(P2CONST(Dem_ConfigType, AUTOMATIC, DEM_CONST_PBCFG) ConfigPtr);

#if (DEM_VERSION_INFO_API == STD_ON)
/*************************************************************************/
/*
 * Brief               Provide Version information to other BSWs
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     versionInfo
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(void, DEM_CODE) Dem_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DEM_APPL_DATA) versionInfo);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* DEM_VERSION_INFO_API == STD_ON */

/*************************************************************************/
/*
 * Brief               Shuts down this module.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 3449,3451++ */ /* MISRA Rule 8.5*/
extern FUNC(void, DEM_CODE) Dem_Shutdown(void);
/* PRQA S 3449,3451-- */ /* MISRA Rule 8.5*/

/*************************************************************************/
/*
 * Brief               Queues the reported events from the BSW modules (API is only used by
                        BSW modules). The interface has an asynchronous behavior, because
                        the processing of the event is done within the Dem main function.
                        OBD Events Suppression shall be ignored for this computation.
 * ServiceId           0x0f
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant for different EventIds. Non reentrant for the same EventId.
 * Param-Name[in]      EventId: Identification of an event by assigned Event ID.
 *                       EventStatus: Monitor test result
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_ReportErrorStatus(Dem_EventIdType EventId, Dem_EventStatusType EventStatus);

#if (DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY) /*SWS_Dem_00687]*/
/*************************************************************************/
/*
 * Brief               Set the available status of a specific Event.
 * ServiceId           0x37
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      EventId : Identification of an event by assigned EventId.
 *                       AvailableStatus: This parameter specifies whether the respective
 *                       Event shall be available (TRUE) or not (FALSE).
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful.
 *                       E_NOT_OK:change of available status not accepted
 */
/*************************************************************************/
/* req SWS_Dem_01106 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetEventAvailable(Dem_EventIdType EventId, boolean AvailableStatus);
#endif /* DEM_AVAILABILITY_SUPPORT == DEM_EVENT_AVAILABILITY */

/*************************************************************************/
/*
 * Brief               Processes the events reported by SW-Cs via RTE.
 * ServiceId           0x04
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
/* PRQA S 0624,3449,3451++ */ /* MISRA Rule 8.3,8.5,8.5 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetEventStatus(Dem_EventIdType EventId, Dem_EventStatusType EventStatus);

/*************************************************************************/
/*
 * Brief               Control the internal debounce counter/timer by BSW modules and SW-Cs.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.
 * Param-Name[in]      EventId:Identification of an event by assigned EventId.
 *                       DebounceResetStatus :  Freeze or reset the internal debounce counter
 *                       /timer of the specified event.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Reset debounce counter/timer status was successful
 *                       E_NOT_OK: Reset debounce counter/timer status failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    Dem_ResetEventDebounceStatus(Dem_EventIdType EventId, Dem_DebounceResetStatusType DebounceResetStatus);

/*************************************************************************/
/*
 * Brief               Resets the event failed status.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.
 * Param-Name[in]      EventId Identification of an event by assigned EventId.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: reset of event status was successful
 *                       E_NOT_OK: reset of event status failed or is not allowed,
 *                       because the event is already tested in this operation cycle
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_ResetEventStatus(Dem_EventIdType EventId);

/*************************************************************************/
/*
 * Brief               Captures the freeze frame data for a specific event.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the sameEventId.
 * Param-Name[in]      EventId:Identification of an event by assigned EventId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Store freeze frame successfully
 *                       E_NOT_OK: Store freeze frame failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_PrestoreFreezeFrame(Dem_EventIdType EventId);

/*************************************************************************/
/*
 * Brief               Clears a prestored freeze frame of a specific event.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.
 * Param-Name[in]      EventId : Identification of an event by assigned EventId.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Clear Prestored FreezeFrame successfully
 *                       E_NOT_OK: Clear Prestored FreezeFrame failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_ClearPrestoredFreezeFrame(Dem_EventIdType EventId);

/*************************************************************************/
/*
 * Brief               Queues the reported events from the BSW modules (API is only used by
                        BSW modules). The interface has an asynchronous behavior, because
                        the processing of the event is done within the Dem main function.
                        OBD Events Suppression shall be ignored for this computation.
 * ServiceId           0x2a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ComponentId: Identification of ComponentId.
 * Param-Name[out]     ComponentFailed: TRUE: failed
 *                     FALSE: not failed
 * Param-Name[in/out]  None
 * Return              E_OK: get ComponentFailed was successful
 *                       E_NOT_OK: get ComponentFailed failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_GetComponentFailed(Dem_ComponentIdType ComponentId, P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) ComponentFailed);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the current extended event status of an event.
 * ServiceId           0x0a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId:Identification of an event by assigned EventId.
 * Param-Name[out]     EventStatusByte:UDS DTC status byte of the requested event
 * Param-Name[in/out]  None
 * Return              E_OK: get of event status was successful
 *                       E_NOT_OK: get of event status failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_GetEventStatus(Dem_EventIdType EventId, P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) EventStatusByte);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the event failed status of an event.
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId: Identification of an event by assigned EventId.
 * Param-Name[out]     EventFailed: TRUE - Last Failed FALSE - not Last Failed
 * Param-Name[in/out]  None
 * Return              E_OK: get of "EventFailed" was successful
 *                       E_NOT_OK: get of "EventFailed" was not successful
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_GetEventFailed(Dem_EventIdType EventId, P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) EventFailed);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the event tested status of an event.
 * ServiceId           0x0c
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId:Identification of an event by assigned EventId.
 * Param-Name[out]     EventTested: TRUE - event tested this cycle
 *                                    FALSE - event not tested this cycle
 * Param-Name[in/out]  None
 * Return              E_OK: get of event state "tested" successful
 *                       E_NOT_OK: get of event state "tested" failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_GetEventTested(Dem_EventIdType EventId, P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) EventTested);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Sets an operation cycle state.
 * ServiceId           0x08
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      OperationCycleId: Identification of operation cycle, like power cycle,
 *                                         driving cycle.
 *                       CycleState: New operation cycle state: (re-)start or end
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: set of operation cycle was accepted and will be handled asynchronously
 *                       E_NOT_OK: set of operation cycle was rejected>
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    Dem_SetOperationCycleState(uint8 OperationCycleId, Dem_OperationCycleStateType CycleState);

/*************************************************************************/
/*
 * Brief               Gets information about the status of a specific operation cycle.
 * ServiceId           0x9e
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      OperationCycleId : Identification of operation cycle, like power cycle,
 *                                          driving cycle.
 * Param-Name[out]     CycleState : Cycle status information
 * Param-Name[in/out]  None
 * Return              E_OK: read out of operation cycle was successful
 *                       E_NOT_OK: read out of operation cycle failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetOperationCycleState(
    uint8 OperationCycleId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_OperationCycleStateType, AUTOMATIC, DEM_APPL_DATA) CycleState);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Triggers the next aging cycle state.
 * ServiceId           0x11
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      OperationCycleId:Identification of aging cycle.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: set of aging cycle was successful
 *                       E_NOT_OK: set of aging cycle failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetAgingCycleState(uint8 OperationCycleId);

/*************************************************************************/
/*
 * Brief               Sets the WIR status bit via failsafe SW-Cs.
 * ServiceId           0x7a
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds.
 *                       Non reentrant for the same EventId.
 * Param-Name[in]      EventId: Identification of an event by assigned EventId.
 *                       WIRStatus: Requested status of event related WIR-bit
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: The request is accepted
 *                       E_NOT_OK: not be accepted
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetWIRStatus(Dem_EventIdType EventId, boolean WIRStatus);

/*************************************************************************/
/*
 * Brief               Gets the debouncing status of an event.
 * ServiceId           0x9f
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId:Identification of an event by assigned EventId.
 * Param-Name[out]     DebouncingState
 * Param-Name[in/out]  None
 * Return              E_OK: get of debouncing status per event state successful
 *                       E_NOT_OK: get of debouncing per event state failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetDebouncingOfEvent(
    Dem_EventIdType EventId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_DebouncingStateType, AUTOMATIC, DEM_APPL_DATA) DebouncingState);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the DTC of an event.
 * ServiceId           0x0d
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId:Identification of an event by assigned EventId.
 *                       DTCFormat: Defines the output-format of the requested DTCvalue.
 * Param-Name[out]     DTCOfEvent: Receives the DTC value in respective format returned
 *                                   by this function.
 * Param-Name[in/out]  None
 * Return              E_OK: get of DTC was successful
 *                       E_NOT_OK: the call was not successful
 *                       DEM_E_NO_DTC_AVAILABLE: there is no DTC configured in the requested format
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetDTCOfEvent(
    Dem_EventIdType EventId,
    Dem_DTCFormatType DTCFormat,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTCOfEvent);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Sets an enable condition.
 * ServiceId           0x39
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
/* PRQA S 1330++ */ /* MISRA Rule 8.3 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetEnableCondition(uint8 EnableConditionID, boolean ConditionFulfilled);
/* PRQA S 1330-- */ /* MISRA Rule 8.3 */

/*************************************************************************/
/*
 * Brief               Sets a storage condition.
 * ServiceId           0x38
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetStorageCondition(uint8 StorageConditionID, boolean ConditionFulfilled);

/*************************************************************************/
/*
 * Brief               Gets the fault detection counter of an event.
 * ServiceId           0x3e
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      EventId : Identification of an event by assigned EventId.>
 * Param-Name[out]     FaultDetectionCounter: This parameter receives the Fault Detection Counter
 *                      information of the requested EventId. If the return value of the function
 *                      call is other than E_OK this parameter does not contain valid data.
 *                      -128dec...127dec PASSED... FAILED according to ISO 14229-1
 * Param-Name[in/out]  None
 * Return              E_OK: request was successful
 *                       E_NOT_OK: request failed
 *                       DEM_E_NO_FDC_AVAILABLE: there is no fault detection counter available
 *                       for the requested event
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_GetFaultDetectionCounter(Dem_EventIdType EventId, P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA) FaultDetectionCounter);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the indicator status derived from the event status.
 * ServiceId           0x29
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IndicatorId: Number of indicator
 * Param-Name[out]     IndicatorStatus: Status of the indicator, like off, on, or blinking.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful E_NOT_OK: Operation failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_GetIndicatorStatus(uint8 IndicatorId, P2VAR(Dem_IndicatorStatusType, AUTOMATIC, DEM_APPL_DATA) IndicatorStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Sets the indicator status included in the event status.
 * ServiceId           0xa1
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
extern FUNC(Std_ReturnType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_SetIndicatorStatus(
        uint8 IndicatorId,
        P2CONST(Dem_IndicatorStatusType, AUTOMATIC, DEM_APPL_DATA) IndicatorStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the data of a freeze frame by event.
 * ServiceId           0x6e
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetEventFreezeFrameDataEx(
    Dem_EventIdType EventId,
    uint8 RecordNumber,
    uint16 DataId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    uint16* BufSize);

/*************************************************************************/
/*
 * Brief               Clears single DTCs, as well as groups of DTCs. This API is intended
                       for complex device driver. It can only be used through the RTE (due
                       to work-around described below SWS_Dem_00659), and therefore no
                       declaration is exported via Dem.h.
 * ServiceId           0x23
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTC:Defines the DTC in respective format, that shall be
 *                          cleared from the event memory. If the DTC fits to a
 *                          DTC group number, all DTCs of the group shall be cleared.
 *                       DTCFormat:Defines the input-format of the provided DTC value.
 *                       DTCOrigin:If the Dem supports more than one event memory
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnClearDTCType.
 */
/*************************************************************************/
extern FUNC(Dem_ReturnClearDTCType, DEM_CODE)
    Dem_ClearDTC(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin);

/*************************************************************************/
/*
 * Brief               Gets the data of an extended data record by event.
 * ServiceId           0x6d
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      EventId : Identification of an event by assigned EventId.
 *                       RecordNumber: Identification of requested Extended data record.
 *                       Valid values are between 0x01 and 0xEF as defined in ISO14229-1.
 * Param-Name[out]     DestBuffer: This parameter contains a byte pointer that points
 *                          to the buffer, to which the extended data shall bewritten to.
 *                          The format is raw hexadecimal values and contains no header-information.
 * Param-Name[in/out]  BufSize:When the function is called this parameter contains
                            the maximum number of data bytes that can be written to the buffer.
                            The function returns the actual number of written
                            data bytes in this parameter.
 * Return              E_OK: Operation was successful
 *                       DEM_E_NODATAAVAILABLE: The requested event data is not currently
 *                              stored (but the request was valid)
 *                       DEM_E_WRONG_RECORDNUMBER: The requested record number is not supported by
 the event
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetEventExtendedDataRecordEx(
    Dem_EventIdType EventId,
    uint8 RecordNumber,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    /* PRQA S 1330++ */ /* MISRA Rule 8.3 */
    uint16* BufSize);
/* PRQA S 1330-- */ /* MISRA Rule 8.3 */

/*************************************************************************/
/*
 * Brief               Gets the event memory overflow indication status.
 * ServiceId           0x32
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTCOrigin: If the Dem supports more than one event memory this parameter
 *                          is used to select the source memory the overflow indication shall be
 * read from.> Param-Name[out]     OverflowIndication: This parameter returns TRUE if the according
 *                          event memory was overflowed, otherwise it returns FALSE.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
 *                       E_NOT_OK: Operation failed or is not supported
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetEventMemoryOverflow(
    Dem_DTCOriginType DTCOrigin,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) OverflowIndication);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Returns the number of entries currently stored in the requested event memory.
 * ServiceId           0x35
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTCOrigin: If the Dem supports more than one event memory
 *                          this parameter is used to select the source memory
 *                          the number of entries shall be read from.
 * Param-Name[out]     NumberOfEventMemoryEntries: Number of entries currently stored in the
 *                          requested event memory.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful E_NOT_OK: Operation failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetNumberOfEventMemoryEntries(
    Dem_DTCOriginType DTCOrigin,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) NumberOfEventMemoryEntries);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Set the availability of a specific DemComponent.
 * ServiceId           0x2b
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
    Dem_SetComponentAvailable(Dem_ComponentIdType ComponentId, boolean AvailableStatus);

#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
/*************************************************************************/
/*
 * Brief               Set the suppression status of a specific DTC.
 * ServiceId           0x33
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC: Diagnostic Trouble Code
 *                       DTCFormat: Defines the input-format of the provided DTC value.
 *                       SuppressionStatus: This parameter specifies whether the respective
 *                       DTC shall be disabled (TRUE) or enabled (FALSE).
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK (Operation was successful),
 *                       E_NOT_OK (operation failed or event entry for this DTC still exists)
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    Dem_SetDTCSuppression(uint32 DTC, Dem_DTCFormatType DTCFormat, boolean SuppressionStatus);
#endif /* DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION */

/*************************************************************************/
/*
 * Brief               Gets the data of an most recent freeze frame record by event.
                        The OBD-II freeze frame is not returned by this function
 * ServiceId           0x41
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      EventId: Identification of an event by assigned EventId.
 *                          this parameter is used to select the source memory
 *                          the number of entries shall be read from.
 * Param-Name[out]     DestBuffer: This parameter contains a byte pointer that points
                        to the buffer, to which the freeze frame record shall
                        be written to. The format is raw hexadecimal values
                        and contains no header-information.
 *                          requested event memory.
 * Param-Name[in/out]  BufSize:When the function is called this parameter contains
                        the maximum number of data bytes that can be written to the buffer.
 * Return              E_OK: Operation was successful.
                        DEM_E_NODATAAVAILABLE: The requested event
                        data is not currently stored (but the request wasvalid).
                        DEM_E_PENDING: The requested data is currently
                        transported from NvM and needs to be requestedagain
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    Dem_DltGetMostRecentFreezeFrameRecordData(Dem_EventIdType EventId, uint8* DestBuffer, uint16* BufSize);

/*************************************************************************/
/*
 * Brief               Gets the data of all extended data records of an event.
 * ServiceId           0x40
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      EventId: Identification of an event by assigned EventId.
 *                          this parameter is used to select the source memory
 *                          the number of entries shall be read from.
 * Param-Name[out]     DestBuffer:This parameter contains a byte pointer that points
                            to the buffer, to which the extended data shall be
                            written to. The format is raw hexadecimal values
                            and contains no header-information.
 * Param-Name[in/out]  BufSize:When the function is called this parameter contains
                        the maximum number of data bytes that can be written to the buffer.
 * Return              E_OK: Operation was successful.
                        DEM_E_NODATAAVAILABLE: The requested event
                        data is not currently stored (but the request was valid).
                        DEM_E_PENDING: The requested data is currently
                        transported from NvM and needs to be requested again.
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    Dem_DltGetAllExtendedDataRecords(Dem_EventIdType EventId, uint8* DestBuffer, uint16* BufSize);

/* PRQA S 0624,3449,3451-- */ /* MISRA Rule 8.3,8.5,8.5 */

#if (DEM_J1939_SUPPORT == STD_ON)
#if (DEM_J1939_READING_DTC_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               The function sets the DTC filter for a specific node and returns the
 composite lamp status of the filtered DTCs
 * ServiceId           0x90
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTCStatusFilter: The following types are available:
                        DEM_J1939DTC_ACTIVE
                        DEM_J1939DTC_PREVIOUSLY_ACTIVE
                        DEM_J1939DTC_PENDING
                        DEM_J1939DTC_PERMANENT
                        DEM_J1939DTC_CURRENTLY_ACTIVE
 *                      DTCKind: Defines the functional group of DTCs to be reported
 *                      node:Nm node Id
 * Param-Name[out]     LampStatus:Receives the commulated lamp status
 * Param-Name[in/out]  none
 * Return              Status of the operation to (re-)set a DTC filter.
 */
/*************************************************************************/
extern FUNC(Dem_ReturnSetFilterType, DEM_CODE) Dem_J1939DcmSetDTCFilter(
    Dem_J1939DcmDTCStatusFilterType DTCStatusFilter,
    Dem_DTCKindType DTCKind,
    uint8 node,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_APPL_DATA) LampStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the number of currently filtered DTCs set by the function Dem_J1939DcmSetDTCFilter
 * ServiceId           0x91
 * Sync/Async          Asynchronous
 * Reentrancy Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     NumberOfFilteredDTC: The number of DTCs matching the defined status mask.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a number of DTC from the Dem
 */
/*************************************************************************/
extern FUNC(Dem_ReturnGetNumberOfFilteredDTCType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_J1939DcmGetNumberOfFilteredDTC(P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredDTC);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the next filtered J1939 DTC.
 * ServiceId           0x92
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
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_J1939DcmGetNextFilteredDTC(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) J1939DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) OccurenceCounter);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* DEM_J1939_READING_DTC_SUPPORT == STD_ON */

#if (DEM_J1939_DM31_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               The function sets the filter to the first applicable DTC for the DM31 response for a specific
 * node ServiceId           0x93 Sync/Async          Synchronous Reentrancy Non Reentrant Param-Name[in] Nm node Id of
 * requesting client Param-Name[out]     None Param-Name[in/out]  none Return              Status of the operation to
 * retrieve a DTC from the Dem.
 */
/*************************************************************************/
extern FUNC(void, DEM_CODE) Dem_J1939DcmFirstDTCwithLampStatus(uint8 node);

/*************************************************************************/
/*
 * Brief               Gets the next filtered J1939 DTC for DM31 including current LampStatus.
 * ServiceId           0x94
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     LampStatus:Receives the lamp status returned by this function.
 *                      If the return value of the function is other than
                        DEM_FILTERED_OK this parameter does not contain valid data.
                        J1939DTC:Receives the J1939DTC value. If the return value of
                        the function is other than DEM_FILTERED_OK this
                        parameter does not contain valid data.
                        OccurenceCounter:This parameter receives the corresponding occurrence
 counter. If the return value of the function call is other than DEM_FILTERED_OK this parameter does
 not contain valid data.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC from the Dem.
 */
/*************************************************************************/
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_J1939DcmGetNextDTCwithLampStatus(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_APPL_DATA) LampStatus,
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) J1939DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) OccurenceCounter);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* DEM_J1939_DM31_SUPPORT == STD_ON */

#if (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Clears active DTCs as well as previously active DTCs
 * ServiceId           0x95
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTCTypeFilter:The following types are available:
                        DEM_J1939DTC_CLEAR_ALL
                        DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE
 *                      node: Nm node Id of requesting client
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnClearDTCType.
 */
/*************************************************************************/
extern FUNC(Dem_ReturnClearDTCType, DEM_CODE)
    Dem_J1939DcmClearDTC(Dem_J1939DcmSetClearFilterType DTCTypeFilter, uint8 node);
#endif /* DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON */

#if (DEM_J1939_EXPANDED_FREEZE_FRAME_SUPPORT == STD_ON)
#if (DEM_J1939_FREEZE_FRAME_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               The function sets the FreezeFrame filter for a specific node
 * ServiceId           0x96
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      FreezeFrameKind: The following types are available:
                        DEM_J1939DCM_FREEZEFRAME
                        DEM_J1939DCM_EXPANDED_FREEZEFRAME
                        DEM_J1939DCM_SPNS_IN_EXPANDED_
                        FREEZEFRAME
                        node:Nm node Id of requesting client
 * Param-Name[out]     None
 * Param-Name[in/out]  none
 * Return              Status of the operation to (re-)set a FreezeFrame filter.
 */
/*************************************************************************/
/* PRQA S 0779++ */ /* MISRA Rule 5.2 */
extern FUNC(Dem_ReturnSetFilterType, DEM_CODE)
    Dem_J1939DcmSetFreezeFrameFilter(Dem_J1939DcmSetFreezeFrameFilterType FreezeFrameKind, uint8 node);
/* PRQA S 0779-- */ /* MISRA Rule 5.2 */

/*************************************************************************/
/*
 * Brief               Gets next freeze frame data. The function stores the data in the provided
 DestBuffer
 * ServiceId           0x97
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
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_J1939DcmGetNextFreezeFrame(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) J1939DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) OccurenceCounter,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* DEM_J1939_FREEZE_FRAME_SUPPORT == STD_ON */

/*************************************************************************/
/*
 * Brief               Gets next SPN.
 * ServiceId           0x98
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
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_J1939DcmGetNextSPNInFreezeFrame(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) SPNSupported,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) SPNDataLength);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* DEM_J1939_EXPANDED_FREEZE_FRAME_SUPPORT == STD_ON */

#if (DEM_J1939_RATIO_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               The function sets the Ratio filter for a specific node and returns the
 corresponding Ignition Cycle Counter and General Denominator.
 * ServiceId           0x99
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      node:Nm node Id of requesting client
 * Param-Name[out]     IgnitionCycleCounter: Ignition Cycle Counter
                        OBDMonitoringConditionsEncountered: OBD Monitoring Conditions Encountered
 * Param-Name[in/out]  None
 * Return              Status of the operation to (re-)set a DTC filter.
 */
/*************************************************************************/
extern FUNC(Dem_ReturnSetFilterType, DEM_CODE) Dem_J1939DcmSetRatioFilter(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) IgnitionCycleCounter,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) OBDMonitoringConditionsEncountered,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    uint8 node);

/*************************************************************************/
/*
 * Brief               The function sets the Ratio filter for a specific node and returns the
 corresponding Ignition Cycle Counter and General Denominator.
 * ServiceId           0x9a
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
extern FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_J1939DcmGetNextFilteredRatio(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SPN,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Numerator,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Denominator);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* DEM_J1939_RATIO_SUPPORT == STD_ON */

#if (DEM_J1939_READINESS1_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Service to report the value of Diagnostic Readiness 1 (DM05) computed by the
 Dem.
 * ServiceId           0x9b
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_J1939DcmReadDiagnosticReadiness1(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_J1939DcmDiagnosticReadiness1Type, AUTOMATIC, DEM_APPL_DATA) DataValue,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    uint8 node);
#endif /* DEM_J1939_READINESS1_SUPPORT == STD_ON */

#if (DEM_J1939_READINESS2_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Service to report the value of Diagnostic Readiness 2 (DM21) computed by the
 Dem.
 * ServiceId           0x9c
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      node:Nm node Id of requesting client
 * Param-Name[out]     DataValue:Buffer of 8 bytes containing the contents of Diagnostic Readiness 2
 (DM21) computed by the Dem.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
                        E_NOT_OK: Operation failed
 */
/*************************************************************************/
/* PRQA S 0777,0779,3432++ */ /* MISRA Rule 5.1,5.2,20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_J1939DcmReadDiagnosticReadiness2(
    P2VAR(Dem_J1939DcmDiagnosticReadiness2Type, AUTOMATIC, DEM_APPL_DATA) DataValue,
    uint8 node);
/* PRQA S 0777,0779,3432-- */ /* MISRA Rule 5.1,5.2,20.7 */
#endif                        /* DEM_J1939_READINESS2_SUPPORT == STD_ON */

#if (DEM_J1939_READINESS3_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Service to report the value of Diagnostic Readiness 3 (DM26) computed by the
 Dem.
 * ServiceId           0x9d
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      node:Nm node Id of requesting client
 * Param-Name[out]     DataValue:Buffer of 8 bytes containing the contents of Diagnostic Readiness 3
 (DM26) computed by the Dem.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
                        E_NOT_OK: Operation failed
 */
/*************************************************************************/
/* PRQA S 0777,0779,3432++ */ /* MISRA Rule 5.1,5.2,20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_J1939DcmReadDiagnosticReadiness3(
    P2VAR(Dem_J1939DcmDiagnosticReadiness3Type, AUTOMATIC, DEM_APPL_DATA) DataValue,
    uint8 node);
/* PRQA S 0777,0779,3432-- */ /* MISRA Rule 5.1,5.2,20.7 */
#endif                        /* DEM_J1939_READINESS3_SUPPORT == STD_ON */
#endif                        /*DEM_J1939_SUPPORT == STD_ON */

#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
/*************************************************************************/
/*
 * Brief               Service is used for requesting IUMPR data according to InfoType 08.
 * ServiceId           0x6b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      OpStatus: Only DCM_INITIAL will appear, because this API
                        behaves synchronous.
 * Param-Name[out]     Iumprdata08: Buffer containing the number of data elements (as
                        defined in ISO-15031-5) and contents of InfoType
                        08. The buffer is provided by the Dcm.
 * Param-Name[in/out]  Iumprdata08BufferSize: The maximum number of data bytes that can be
                        written to the Iumprdata08 Buffer.
 * Return              Always E_OK is returned.
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    Dem_DcmGetInfoTypeValue08(Dcm_OpStatusType OpStatus, uint8* Iumprdata08, uint8* Iumprdata08BufferSize);

/*************************************************************************/
/*
 * Brief               Service is used for requesting IUMPR data according to InfoType 0B.
 * ServiceId           0x6c
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      OpStatus: Only DCM_INITIAL will appear, because this API
                        behaves synchronous.
 * Param-Name[out]     Iumprdata0B: Buffer containing the number of data elements (as
                        defined in ISO-15031-5) and contents of InfoType
                        08. The buffer is provided by the Dcm.
 * Param-Name[in/out]  Iumprdata0BBufferSize: The maximum number of data bytes that can be
                        written to the Iumprdata08 Buffer.
 * Return              Always E_OK is returned.
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_DcmGetInfoTypeValue0B(
    Dcm_OpStatusType OpStatus,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata0B,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata0BBufferSize);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Reports the value of a requested "availability-OBDMID" to the DCM upon a
 Service 06 request.
 * ServiceId           0xa3
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Obdmid Availablity OBDMID (00,20, 40...)
 * Param-Name[out]     Obdmidvalue Bit coded information on the support of OBDMIDs.
 * Param-Name[in/out]  None
 * Return              E_OK: Report of DTR result successful
                        E_NOT_OK: Report of DTR result failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_DcmGetAvailableOBDMIDs(uint8 Obdmid, P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) Obdmidvalue);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets the number of TIDs per (functional) OBDMID.
 * ServiceId           0xa4
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Obdmid OBDMID subject of the request to identify the number of assigned TIDs
 * Param-Name[out]     numberOfTIDs Number of assigned TIDs for the requested OBDMID.
 *                      Used as loop value for the DCM to retrieve all OBD/TID result data
 * Param-Name[in/out]  None
 * Return              E_OK: get number of TIDs successful
                        E_NOT_OK: get number of TIDs failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dem_DcmGetNumTIDsOfOBDMID(uint8 Obdmid, P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) numberOfTIDs);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 01 computed by the Dem.
 * ServiceId           0x61
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 01 computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* Monitor status since DTCs cleared 4 byte*/
/* SWS_Dem_01103 Unavailable events shall not be considered for computation of service 01 PID 41.*/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID01(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID01value);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 1C computed by the Dem.
 * ServiceId           0x63
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 1C computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/*OBD requirements to which vehicle is designed*/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID1C(
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID1Cvalue
#else
    P2CONST(uint8, AUTOMATIC, DEM_APPL_DATA) PID1Cvalue
#endif
);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 21 computed by the Dem.
 * ServiceId           0x64
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 21 computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID21(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID21value);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 30 computed by the Dem.
 * ServiceId           0x65
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 30 computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID30(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID30value);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 31 computed by the Dem.
 * ServiceId           0x66
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 31 computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID31(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID31value);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 41 computed by the Dem.
 * ServiceId           0x67
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 41 computed by the Dem.>
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID41(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID41value);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 4D computed by the Dem.
 * ServiceId           0x68
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 4D computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID4D(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID4Dvalue);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 4E computed by the Dem.
 * ServiceId           0x69
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID4Evalue Buffer containing the contents of PID 4E
                        computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID4E(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID4Evalue);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 4E computed by the Dem.
 * ServiceId           0x6a
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID4Evalue Buffer containing the contents of PID 91
                        computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID91(
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID91value
#else
    P2CONST(uint8, AUTOMATIC, DEM_APPL_DATA) PID91value
#endif
);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
extern Std_ReturnType DemReadPID91(uint8* Buffer);
#endif

/*************************************************************************/
/*
 * Brief               Gets data element per PID and index of the most important freeze frame
                        being selected for the output of service 02. The function stores the data
                        in the provided DestBuffer.
 * ServiceId           0x52
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfOBDFreezeFrame(
    uint8 PID,
    uint8 DataElementIndexOfPID,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Gets DTC by freeze frame record number.
 * ServiceId           0x53
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      FrameNumber: Unique identifier for a freeze frame
                        record as defined in ISO 15031-5. The value 0x00
                        indicates the complete OBD freeze frame. Other values
                        are reserved for future functionality.
                        DTCFormat: Output format of the DTC value.
 * Param-Name[out]     DTC: Diagnostic Trouble Code in ODB format. If the return
                        value of the function is other than E_OK this
                        parameter does not contain valid data.
 * Param-Name[in/out]  None
 * Return              E_OK: operation was successful E_NOT_OK: no DTC available
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_DcmGetDTCOfOBDFreezeFrame(
    uint8 FrameNumber,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    Dem_DTCFormatType DTCFormat);

/*************************************************************************/
/*
 * Brief               Reports a DTR data along with TID-value, UaSID, test result with lower and
 upper limit.
 * ServiceId           0xa5
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_DcmGetDTRData(
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
 * Brief               Service for reporting the event as disabled to the
                        Dem for the PID 41 computation.
 * ServiceId           0x51
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds. Non reentrant for the same EventId.
 * Param-Name[in]      Identification of an event by assigned EventId.>
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK set of event to disabled was successfull.
                         E_NOT_OK set of event disabled failed
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetEventDisabled(Dem_EventIdType EventId);

/*************************************************************************/
/*
 * Brief               Service for reporting that faults are possibly found
                        because all conditions are fullfilled.
 * ServiceId           0x73
 * Sync/Async          Reentrant for different RatioIDs. Non reentrant for the same RatioID
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Ratio Identifier reporting that a respective monitor
                        could have found a fault
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK report of IUMPR result was successfully reported
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_RepIUMPRFaultDetect(Dem_RatioIdType RatioID);

#if (DEM_RATIO_NUM > 0)
/*************************************************************************/
/*
 * Brief               In order to communicate the status of the (additional) denominator
                        conditions among the OBD relevant ECUs, the API is used to forward
                        the condition status to a Dem of a particular ECU
 * ServiceId           0xae
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
    Dem_SetIUMPRDenCondition(Dem_IumprDenomCondIdType ConditionId, Dem_IumprDenomCondStatusType ConditionStatus);

/*************************************************************************/
/*
 * Brief               In order to communicate the status of the (additional) denominator
                        conditions among the OBD relevant ECUs, the API is used to retrieve
                        the condition status from the Dem of the ECU where the conditions are
                        computed.
 * ServiceId           0xaf
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetIUMPRDenCondition(
    Dem_IumprDenomCondIdType ConditionId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dem_IumprDenomCondStatusType, AUTOMATIC, DEM_APPL_DATA) ConditionStatus);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /*DEM_RATIO_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Service is used to lock a denominator of a specific monitor.
 * ServiceId           0x71
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_RepIUMPRDenLock(Dem_RatioIdType RatioID);

/*************************************************************************/
/*
 * Brief               Service is used to release a denominator of a
                        specific monitor.
 * ServiceId           0x72
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
extern FUNC(Std_ReturnType, DEM_CODE) Dem_RepIUMPRDenRelease(Dem_RatioIdType RatioID);

/*************************************************************************/
/*
 * Brief               Set the status of the PTO.
 * ServiceId           0x79
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PtoStatus sets the status of the PTO
                       TRUE==active;FALSE==inactive
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType Returns E_OK when the new PTO-status has been
                        adopted by the Dem; returns E_NOT_OK in all other cases.
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetPtoStatus(boolean PtoStatus);

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 01 computed by the Dem.
 * ServiceId           0xb3
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID01value:Buffer containing the contents of PID 01 computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_ReadDataOfPID01(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID01value);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#if ((DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU) && (DEM_OBD_CENTRALIZED_PID21_HANDLING == STD_ON))
/*************************************************************************/
/*
 * Brief               Service to get the value of PID 21 from the Dem by a software component.
 * ServiceId           0xb1
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID21value:Content of PID 21 as raw hex value.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetDataOfPID21(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID21value);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#endif /* DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU && DEM_OBD_CENTRALIZED_PID21_HANDLING == \
          STD_ON */

/*************************************************************************/
/*
 * Brief               Service to set the value of PID 31 in the Dem by a software component
 * ServiceId           0xa7
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID31value:Buffer containing the contents of PID 31.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetDataOfPID31(P2CONST(uint8, AUTOMATIC, DEM_APPL_DATA) PID31value);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Service to set the value of PID 4D in the Dem by a software component.
 * ServiceId           0xa8
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID4Dvalue:Buffer containing the contents of PID 4D.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetDataOfPID4D(P2CONST(uint8, AUTOMATIC, DEM_APPL_DATA) PID4Dvalue);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Service to set the value of PID 4E in the Dem by a software component.
 * ServiceId           0xa9
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID4Evalue:Buffer containing the contents of PID 4E.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetDataOfPID4E(P2CONST(uint8, AUTOMATIC, DEM_APPL_DATA) PID4Evalue);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Marks the current OBD driving cycle as having met the criteria for the PFC
 * cycle. ServiceId           0xaa Sync/Async          Synchronous Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetPfcCycleQualified(void);

/*************************************************************************/
/*
 * Brief               Returns TRUE if the criteria for the PFC cycle have been met during the
 current OBD driving cycle.
 * ServiceId           0xab
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     isqualified:TRUE: During the current OBD driving cycle the criteria for the
 PFC cycle have been met. FALSE: During the current OBD driving cycle the criteria for the PFC cycle
 have not been met.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetPfcCycleQualified(P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) isqualified);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               API to inform the Dem (of a Dependend / Secondary ECU) about the
                       reception of service 04 execution by a software component.
                       API is needed in OBD Dependend / Secondary ECUs only.
 * ServiceId           0xac
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC:Defines the DTC in respective format, that has been
 *                     cleared from the event memory.
 *                       DTCFormat:Format of the provided DTC value.
 *                       DTCOrigin:Event memory (e.g. MIRROR)
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Return value unused - only for compatibility with according RTE operation.
 */
/*************************************************************************/
extern FUNC(Dem_ReturnClearDTCType, DEM_CODE)
    Dem_SetClearDTC(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin);

#if (DEM_DTR_NUM > 0)
/*************************************************************************/
/*
 * Brief              Reports a DTR result with lower and upper limit. The internal eventstatus
                        serves as master whether the DTR values are forwarded or ignored,
                        also taking the DTRUpdateKind into account.
 * ServiceId           0xa2
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
    Dem_SetDTR(uint16 DTRId, sint32 TestResult, sint32 LowerLimit, sint32 UpperLimit, Dem_DTRControlType Ctrlval);
#endif /*DEM_DTR_NUM > 0 */

/*************************************************************************/
/*
 * Brief               Service to report the value of the B1 counter computed by the Dem.
 * ServiceId           0xb4
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     B1Counter:Buffer containing the B1 counter.
 * Param-Name[in/out]  none
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_GetB1Counter(P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) B1Counter);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*************************************************************************/
/*
 * Brief               Service to set the value of PID 21 in the Dem by a software component.
 * ServiceId           0xa6
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID21value:Buffer containing the contents of PID 21.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_SetDataOfPID21(P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID21value);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#endif /* DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT */

#endif /* DEM_H_ */

/*******************************************************************************
**                      end of file                                           **
*******************************************************************************/
