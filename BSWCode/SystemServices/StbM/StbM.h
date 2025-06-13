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
**  FILENAME    : StbM.h                                                      **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuzhe.zhang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : implementation header for StbM                              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
#ifndef STBM_H
#define STBM_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "StbM_Types.h"
#include "SchM_StbM.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

#define STBM_VENDOR_ID          62U
#define STBM_MODULE_ID          160U
#define STBM_INSTANCE_ID        0U
#define STBM_H_AR_MAJOR_VERSION 4U
#define STBM_H_AR_MINOR_VERSION 5U
#define STBM_H_AR_PATCH_VERSION 0U
#define STBM_H_SW_MAJOR_VERSION 2U
#define STBM_H_SW_MINOR_VERSION 0U
#define STBM_H_SW_PATCH_VERSION 12U
#if (STBM_DEV_ERROR_DETECT == STD_ON)
/*Development errors used by the STBM module*/
/* StbM_Init called with an invalid configuration pointer */
#define STBM_E_INIT_FAILED ((Std_ReturnType)0x11)
/* API called while StbM is not initialized */
#define STBM_E_UNINIT ((Std_ReturnType)0x0B)
/* API called with invalid time base ID */
#define STBM_E_PARAM ((Std_ReturnType)0x0A)
/* API called with invalid pointer in parameter list */
#define STBM_E_PARAM_POINTER ((Std_ReturnType)0x10)
/* API disabled by configuration */
#define STBM_E_SERVICE_DISABLED ((Std_ReturnType)0x12)
/* API called with invalid timestamp */
#define STBM_E_PARAM_TIMESTAMP ((Std_ReturnType)0x25)
/* API called with invalid user data */
#define STBM_E_PARAM_USERDATA ((Std_ReturnType)0x26)

/*service id*/
#define STBM_SID_INIT                       ((uint8)0x00)
#define STBM_SID_GETVERSIONINFO             ((uint8)0x05)
#define STBM_SID_GETCURRENTTIME             ((uint8)0x07)
#define STBM_SID_GETCURRENTVIRTUALLOCALTIME ((uint8)0x1E)
#define STBM_SID_SETGLOBALTIME              ((uint8)0x0B)
#define STBM_SID_UPDATEGLOBALTIME           ((uint8)0x10)
#define STBM_SID_SETUSERDATA                ((uint8)0x0C)
#define STBM_SID_SETOFFSET                  ((uint8)0x0D)
#define STBM_SID_GETOFFSET                  ((uint8)0x0E)
#define STBM_SID_BUSGETCURRENTTIME          ((uint8)0x1F)
#define STBM_SID_BUSSETGLOBALTIME           ((uint8)0x0F)
#define STBM_SID_GETRATEDEVIATION           ((uint8)0x11)
#define STBM_SID_SETRATECORRECTION          ((uint8)0x12)
#define STBM_SID_GETTIMELEAP                ((uint8)0x13)
#define STBM_SID_GETTIMEBASESTATUS          ((uint8)0x14)
#define STBM_SID_STARTTIMER                 ((uint8)0x15)
#define STBM_SID_GETSYNCTIMERECORDHEAD      ((uint8)0x16)
#define STBM_SID_GETOFFSETTIMERECORDHEAD    ((uint8)0x17)
#define STBM_SID_TRIGGERTIMETRANSMISSION    ((uint8)0x1C)
#define STBM_SID_GETTIMEBASEUPDATECOUNTER   ((uint8)0x1B)
#define STBM_SID_GETMASTERCONFIG            ((uint8)0x1D)
#define STBM_SID_MAINFUNCTION               ((uint8)0x04)
#endif

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
typedef struct
{
#if STBM_TRIGGERED_CUSTOMER_NUM > 0u
    const StbMTriggeredCustomerCfgType* StbMTriggeredCustomer;
#endif /* STBM_TRIGGERED_CUSTOMER_NUM > 0u */
    const StbMSynchronizedTimeBaseCfgType* StbMSynchronizedTimeBase;
} StbM_ConfigType;

/* Variables of this type store time stamps of the Virtual Local Time. The unit is nanoseconds */
typedef struct
{
    uint32 nanosecondsLo;
    uint32 nanosecondsHi;
} StbM_VirtualLocalTimeType;

/* Structure which contains additional measurement data */
typedef struct
{
    uint32 pathDelay;
} StbM_MeasurementType;
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
extern CONST(StbM_ConfigType, STBM_CONST_PBCFG) StbM_Config;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/**
 * Initializes the Synchronized Time-base Manager
 * Service ID: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @ConfigPtr, Pointer to the selected configuration set.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
void StbM_Init(const StbM_ConfigType* ConfigPtr);
#if (STBM_VERSION_INFO_API == STD_ON)
/**
 * Returns the version information of this module.
 * Service ID: 0x05
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): @versioninfo, Pointer to the memory location holding the version information of
the module.
 * Return value: NA
 */
void StbM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, STBM_APPL_DATA) versioninfo);
#endif /* STBM_VERSION_INFO_API = STD_ON */

/**
 * Returns a time value(Local Time Base derived from Global Time Base) in standard format.
 * Service ID: 0x07
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @timeStamp, Current time stamp that is valid at this time
 *                  @userData, User data of the Time Base
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_GetCurrentTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_TimeStampType* timeStamp,
    StbM_UserDataType* userData);

/**
 * Returns the Virtual Local Time of the referenced Time Base.
 * Service ID: 0x1e
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @localTimePtr, Current Virtual Local Time value
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_GetCurrentVirtualLocalTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_VirtualLocalTimeType* localTimePtr);

/**
 * Allows the Customers to set the new global time that has to be valid for the system,
   which will be sent to the busses. This function will be used if a Time Master is
   present in this ECU.
 * Service ID: 0x0b
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):  @timeBaseId, time base reference
 *                  @timeStamp, New time stamp
 *                  @userData, New user data(if not NULL)
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_SetGlobalTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeStampType* timeStamp,
    const StbM_UserDataType* userData);

/**
 * Allows the Customers to set the Global Time that will be sent to the buses. This
   function will be used if a Time Master is present in this ECU. Using UpdateGlobal
   Time will not lead to an immediate transmission of the Global Time.
 * Service ID: 0x10
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):  @timeBaseId, time base reference
 *                  @timeStamp, New time stamp
 *                  @userData, New user data(if not NULL)
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_UpdateGlobalTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeStampType* timeStamp,
    const StbM_UserDataType* userData);

/**
 * Allows the Customers to set the new User Data that has to be valid for the
   system, which will be sent to the busses.
 * Service ID: 0x0c
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):  @timeBaseId, time base reference
 *                  @userData, New user data
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_SetUserData(StbM_SynchronizedTimeBaseType timeBaseId, const StbM_UserDataType* userData);

/**
 * Allows the Customers and the Timesync Modules to set the Offset Time and the User Data.
 * Service ID: 0x0d
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):  @timeBaseId, time base reference
 *                  @timeStamp, New time stamp
 *                  @userData, New user data(Or NULL if no new user data is provided)
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_SetOffset(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeStampType* timeStamp,
    const StbM_UserDataType* userData);

/**
 * Allows the Timesync Modules to get the current Offset Time and User Data.
 * Service ID: 0x0e
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @timeStamp, Current Offset Time value
 *                  @userData, Current user data
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_GetOffset(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_TimeStampType* timeStamp,
    StbM_UserDataType* userData);

/**
 * Returns the current Time Tuple, status and User Data of the Time Base.
 * Service ID: 0x1f
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 *
 * Parameters(INOUT): NA
 * Parameters(OUT): @globalTimePtr Value of the local instance of the Global Time, which is sampled
                    when the function is called
                    @localTimePtr, Value of the Virtual Local Time, which is sampled when the function is called
                    @userDataPtr, User data of the Time Base
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_BusGetCurrentTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_TimeStampType* globalTimePtr,
    StbM_VirtualLocalTimeType* localTimePtr,
    StbM_UserDataType* userDataPtr);

/**
 * Allows the Time Base Provider Modules to forward a new Global Time tuple(i.e.,
   the Received Time Tuple) to the StbM.
 * Service ID: 0x0f
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 *                 @globalTimePtr New Global Time value
 *                 @userDataPtr New User Data(if not NULL)
                   @measureDataPtr New measurement data
                   @localTimePtr Value of the Virtual Local Time associated to the new Global Time
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_BusSetGlobalTime(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_TimeStampType* globalTimePtr,
    const StbM_UserDataType* userDataPtr,
    const StbM_MeasurementType* measureDataPtr,
    const StbM_VirtualLocalTimeType* localTimePtr);

/**
 * Returns value of the current rate deviation of a Time Base
 * Service ID: 0x11
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @rateDeviation, Value of the current rate deviation of a Time Base
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
/* PRQA S 1532, 3673 ++ */ /* VL_StbM_3673 */
Std_ReturnType StbM_GetRateDeviation(StbM_SynchronizedTimeBaseType timeBaseId, StbM_RateDeviationType* rateDeviation);
/* PRQA S 1532, 3673 -- */

/**
 * Allows to set the rate of a Synchronized Time Base(being either a Pure Local Time Base or not)
 * Service ID: 0x12
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 *                 @rateDeviation, value of the applied rate deviation
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_SetRateCorrection(StbM_SynchronizedTimeBaseType timeBaseId, StbM_RateDeviationType rateDeviation);

/**
 * Returns value of Time Leap.
 * Service ID: 0x13
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @timeJump, Time leap value
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_GetTimeLeap(StbM_SynchronizedTimeBaseType timeBaseId, StbM_TimeDiffType* timeJump);

/**
 * Returns detailed status information for a Synchronized(or Pure Local) Time Base
   and, if called for an Offset Time Base, for the Offset Time Base and the underlying
   Synchronized Time Base.
 * Service ID: 0x14
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @syncTimeBaseStatus Status of the Synchronized(or Pure Local) Time Base
                    @offsetTimeBaseStatus Status of the Offset Time Base
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_GetTimeBaseStatus(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_TimeBaseStatusType* syncTimeBaseStatus,
    StbM_TimeBaseStatusType* offsetTimeBaseStatus);

#if (STBM_NOTIFICATION_CUSTOMER_NUM > 0)
/**
 * Sets a time value, which the Time Base value is compared against
 * Service ID: 0x15
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 *                 @customerId, Status of the Synchronized Time Base
                   @expireTime, Time value relative to current Time Base value of the Notification
                    Customer, when the Timer shall expire
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_StartTimer(
    StbM_SynchronizedTimeBaseType timeBaseId,
    StbM_CustomerIdType customerId,
    const StbM_TimeStampType* expireTime);
#endif /* STBM_NOTIFICATION_CUSTOMER_NUM>0 */

#if (STBM_TIME_RECORDING_SUPPORT == STD_ON)
/**
 * Accesses to the recorded snapshot data Header of the table belonging to the Synchronized Time Base.
 * Service ID: 0x16
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @syncRecordTableHead, Header of the table
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_GetSyncTimeRecordHead(
    StbM_SynchronizedTimeBaseType timeBaseId,
    P2VAR(StbM_SyncRecordTableHeadType, AUTOMATIC, STBM_APPL_DATA) syncRecordTableHead);

/**
 * Accesses to the recorded snapshot data Header of the table belonging to the Offset Time Base.
 * Service ID: 0x17
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @offsetRecordTableHead, Header of the table
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_GetOffsetTimeRecordHead(
    StbM_SynchronizedTimeBaseType timeBaseId,
    P2VAR(StbM_OffsetRecordTableHeadType, AUTOMATIC, STBM_APPL_DATA) offsetRecordTableHead);
#endif

/**
 * Called by the <Upper Layer> to force the Timesync Modules to transmit the current
   Time Base again due to an incremented timeBaseUpdateCounter[timeBaseId]
 * Service ID: 0x1c
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_TriggerTimeTransmission(StbM_SynchronizedTimeBaseType timeBaseId);

/**
 * Allows the Timesync Modules to detect, whether a Time Base should be
   transmitted immediately in the subsequent <Bus>TSyn_MainFunction() cycle.
 * Service ID: 0x1b
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:    Counter value belonging to the Time Base, that indicates a Time Base
                    update to the Timesync Modules
 */
uint8 StbM_GetTimeBaseUpdateCounter(StbM_SynchronizedTimeBaseType timeBaseId);

/**
 * Indicates if the functionality for a system wide master(e.g. StbM_SetGlobalTime)
   for a given Time Base is available or not.
 * Service ID: 0x1d
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @timeBaseId, time base reference
 * Parameters(INOUT): NA
 * Parameters(OUT): @masterConfig, Indicates, if system wide master functionality is supported
 * Return value:    E_OK: successful
                    E_NOT_OK: failed
 */
Std_ReturnType StbM_GetMasterConfig(StbM_SynchronizedTimeBaseType timeBaseId, StbM_MasterConfigType* masterConfig);

#endif /* STBM_H */
