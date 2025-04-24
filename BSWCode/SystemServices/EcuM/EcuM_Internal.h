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
**  FILENAME    : EcuM_Internal.h                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Internal type definition and function declaration           **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11.                     **
**                                                                            **
*******************************************************************************/

/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0       2020-06-28  qinchun.yang    R19_11 EcuM initial version.
 *  V2.0.0       2021-04-08  qinchun.yang    R1911 stable version.
 */
#ifndef ECUM_INTERNAL_H
#define ECUM_INTERNAL_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "EcuM.h"
#include "BswM_EcuM.h"
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
#include "Tm.h"
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
#include "SchM_EcuM.h"
#include "Rte_Main.h"
#include "EcuM_Externals.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define ECUM_WKSOURCE_NONE          ((uint32)0u)

#define ECUM_MS_TO_SECOND           (1000u)

#define ECUM_DAFULT_MCU_NORMAL_MODE ((Mcu_ModeType)0xffu)

#define ECUM_AVOID_WARNNING(par)    ((void)par)

#define ECUM_SLAVE_CORE_NUM         (ECUM_MAX_MCU_CORE_NUM - 1u)
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*Pre-compile configuration type definition of ECUM sleep mode. The data index in
 * the array of the modes is used as Sleep mode Identifier used in service like
 * EcuM_SelectShutdownTarget*/
typedef struct
{
    boolean sleepSuspend;                         /*Flag, which is set true, if the CPU is suspended,
                                                   halted, or powered off in the sleep mode. If the
                                                   CPU keeps running in this sleep mode, then this
                                                   flag must be set to false.*/
    Mcu_ModeType mcuMode;                         /*the corresponding MCU mode for this sleep mode*/
    uint8 numberOfWakeSource;                     /*No. of wake up source shall be enabled*/
    P2CONST(uint8, TYPEDEF, ECUM_CONST) wkSrcIdx; /*Reference wake sources index*/
    EcuM_WakeupSourceType wkMask;                 /*Reference all wake sources mask*/
} EcuM_SleepModeCfgType;

/*Pre-compile configuration type definition of ECUM wake up source.*/
typedef struct
{
    uint32 checkWkupTimeout;        /*Time of the EcuM to delay shut down of the
                                      ECU if the check of the Wake up Source is done
                                      asynchronously,time uint of tool is sec, and
                                      generate in c file is ms*/
    uint32 validationTimeout;       /*the timeout is zero, there is no validation
                                       routine and the ECU Manager shall not validate
                                       the wake up source,time uint of tool is sec, and
                                      generate in c file is ms*/
    EcuM_WakeupSourceType wkSource; /*wake up source id bit representation*/
    boolean isPolling;              /*describes if the wake up source needs polling*/
    P2CONST(NetworkHandleType, TYPEDEF, ECUM_CONST)
    comMChnl; /*reference to a Network in COMM, INVALID value
indicates wake up source not a communication channel*/
#if (ECUM_COMM_PNC_ENABLED == STD_ON)
    PNCHandleType pncNum;                            /*number of PNC for this wake up source*/
    P2CONST(PNCHandleType, TYPEDEF, ECUM_CONST) pnc; /*PNC of this wake up source*/
#endif                                               /*ECUM_COMM_PNC_ENABLED == STD_ON*/
    uint32 resetResasonNum;
    P2CONST(Mcu_ResetType, TYPEDEF, ECUM_CONST)
    resetResasonPtr; /*describes the mapping of reset reasons detected
                             by the MCU driver into wake up sources*/
} EcuM_WakeupSourceCfgType;

#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
/*Pre-compile configuration type definition of ECUM alarm clock*/
typedef struct
{
    uint32 alarmClkTimeout;     /*Time out for this alarm clock*/
    EcuM_UserType alarmClkUser; /*the user this alarm clock assigned to*/
} EcuM_AlarmClockCfgType;
#endif

/*Pre-compile configuration type definition of ECUM user*/
typedef struct
{
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId; /*The core id the user of ECUM is in*/
#endif
    boolean goDownAllowed; /*the user is allowed to call the EcuM_GoDown API*/
#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
    boolean setClkAllowed; /*the user is allowed to call the EcuM_SetClock API*/
#endif
    EcuM_UserType usrId; /*the bit related with this user*/
} EcuM_UserCfgType;

/*Runtime status types*/
#if (ECUM_MAX_USER_NUM > 0u)
typedef struct
{
    uint8 RunNum;
    uint8 PostRunNum;
    boolean IsRun[ECUM_MAX_USER_NUM];
    boolean IsPostRun[ECUM_MAX_USER_NUM];
} EcuM_UserStateType;
#endif /*ECUM_USER_NUM > 0u*/

#if (ECUM_MAX_WAKE_UP_SOURCE_NUM > 0u)
/* Wakeup Source */
typedef struct
{
    P2VAR(EcuM_WakeupSourceType, AUTOMATIC, ECUM_VAR) Pending;
    P2VAR(EcuM_WakeupSourceType, AUTOMATIC, ECUM_VAR) Validated;
    P2VAR(EcuM_WakeupSourceType, AUTOMATIC, ECUM_VAR) Expired;
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
    Tm_PredefTimer100us32bitType wkTime[ECUM_MAX_WAKE_UP_SOURCE_NUM];
#else
    TickType wkTime[ECUM_MAX_WAKE_UP_SOURCE_NUM];
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
} EcuM_WksStateType;
#endif /*ECUM_MAX_WAKE_UP_SOURCE_NUM > 0u*/

typedef struct
{
    EcuM_ShutdownTargetType Target;
    EcuM_ShutdownModeType Mode; /* If target is 'SLEEP', otherwise this parameter may ignored. */
} EcuM_TargetType;

typedef struct
{
    EcuM_StateType State;
    EcuM_StateType rqstState;
#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
    EcuM_TimeType UserAlarm[ECUM_MAX_ALARM_CLOCK_NUM];
    EcuM_TimeType MasterAlarm;
    EcuM_TimeType GlobalClock;
#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/
#if (ECUM_MAX_USER_NUM > 0u)
    EcuM_UserStateType Users;
#endif /*ECUM_USER_NUM > 0u*/

#if (ECUM_MAX_WAKE_UP_SOURCE_NUM > 0u)
    EcuM_WksStateType Wks;
#endif /*ECUM_MAX_WAKE_UP_SOURCE_NUM > 0u*/

    EcuM_TargetType SdtgNext; /* Next shutdown target */
    EcuM_TargetType SdtgLast; /* Last shutdown target */

    EcuM_ShutdownCauseType cause;
    EcuM_BootTargetType BootTarget; /* BootLoader Target*/
} EcuM_RunTimeType;
/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/
/***********configure data generated in pre-compile*************/

/*EcuMOSResource, reference to a OS resource which is used to bring the ECU into
 * sleep mode. In case of multi core each core shall have an own OsResource.*/
#if (1 >= ECUM_MAX_MCU_CORE_NUM)
extern CONST(ResourceType, ECUM_CONST) EcuM_OSResource;
#else
extern CONST(ResourceType, ECUM_CONST) EcuM_OSResource[ECUM_MAX_MCU_CORE_NUM];
#endif

/*EcuM Sleep modes configured by pre-compile configure*/
extern CONST(EcuM_SleepModeCfgType, ECUM_CONST) EcuM_SleepModeCfgs[ECUM_MAX_SLEEP_MODE_NUM];

/*EcuM Wake up source configurations by pre-compile configure*/
extern CONST(EcuM_WakeupSourceCfgType, ECUM_CONST) EcuM_WkSourceCfgs[ECUM_MAX_WAKE_UP_SOURCE_NUM];

/*The normal MCU mode to be restored after a sleep.*/
extern CONST(Mcu_ModeType, ECUM_CONST) EcuM_NormalMcuModeCfg;

/*ECUM user configuration*/
extern CONST(EcuM_UserCfgType, ECUM_CONST) EcuM_UserCfgs[ECUM_MAX_USER_NUM];

#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)

/*ECUM user alarm configuration*/
extern CONST(EcuM_AlarmClockCfgType, ECUM_CONST) EcuM_AlarmClkCfgs[ECUM_MAX_ALARM_CLOCK_NUM];

#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/

#if (ECUM_MAX_MCU_CORE_NUM > 1)

extern CONST(CoreIdType, ECUM_CONST) EcuM_SlaveCoreIds[ECUM_MAX_MCU_CORE_NUM - 1];

extern CONST(SpinlockIdType, ECUM_CONST) EcuM_RefSpinLockCfg;

#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

/*Runtime variable*/
#if (ECUM_DEV_ERROR_DETECT == STD_ON)

extern VAR(boolean, ECUM_CLEARED) EcuM_IsInit;

#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/

extern VAR(EcuM_RunTimeType, ECUM_CLEARED) EcuMRunData;

extern VAR(EcuM_WakeupSourceType, ECUM_CLEARED) EcuMWksPending;
extern VAR(EcuM_WakeupSourceType, ECUM_CLEARED) EcuMWksValidated;
extern VAR(EcuM_WakeupSourceType, ECUM_CLEARED) EcuMWksExpired;

/*Pointer variable to store the determined PB configuration data*/
extern P2CONST(EcuM_ConfigType, ECUM_VAR_CLEARED_PTR, ECUM_APPL_CONST) EcuM_ConfigPtr;

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/*Shutdown request flag, set by master core, check by slave main function*/

extern volatile VAR(boolean, ECUM_CLEARED) EcuM_ShutdownFlag;

#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

#if (ECUM_MODE_HANDING == STD_ON)
/* Current Rte State to notify during MainFunction */
extern VAR(Rte_ModeType_EcuM_Mode, ECUM_CLEARED) EcuM_RteState;
#endif

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
FUNC(uint8, ECUM_CODE)
EcuM_WkSrcMap2CfgWkIdx(EcuM_WakeupSourceType wkSrc);

/*Handle SHUTDOWN Phase*/
FUNC(Std_ReturnType, ECUM_CODE)
EcuM_DoShutDownPhase(uint16 caller);

/*OffPreOS Sequence*/
FUNC(void, ECUM_CODE)
EcuM_OffPreOS(void);

/*Update Ecum global clock.(Use EcuMMainFunctionPeriod)*/
FUNC(void, ECUM_CODE)
EcuM_UpdateEcuMClock(void);

FUNC(void, ECUM_CODE)
EcuM_CancellAlarms(void);

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/*Get slave core index in configuration*/
FUNC(uint8, ECUM_CODE)
EcuM_GetSlaveCoreIndex(CoreIdType coreId);
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

/*Query the corresponding configured user id*/
FUNC(Std_ReturnType, ECUM_CODE)
EcuM_SearchUserByCaller(uint16 caller, uint8* userIndex);
#endif /*ECUM_INTERNAL_H*/
