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
/*
 ************************************************************************************************************************
 **
 **  @file               : ComM.h
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : ECU state management
 **
 ***********************************************************************************************************************/

#ifndef ECUM_INTERNAL_H
#define ECUM_INTERNAL_H

/* PRQA S 1753 EOF */ /* VL_EcuM_1753 */

/* =================================================== inclusions =================================================== */
#include "EcuM.h"
#include "EcuM_Cfg.h"
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

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
/** No wakeup source */
#define ECUM_WKSOURCE_NONE ((uint32)0uL)

/** Milliseconds to seconds conversion */
#define ECUM_MS_TO_SECOND (1000u)

/** Default MCU normal mode */
#define ECUM_DAFULT_MCU_NORMAL_MODE ((Mcu_ModeType)0xffu)

/** Number of slave cores */
#define ECUM_SLAVE_CORE_NUM (ECUM_MAX_MCU_CORE_NUM - 1u)

/** Maximum alarm values */
#define ECUM_MAX_ALARM_VALUES 0xFFFFFFFFu

/* PRQA S 3472, 3414 ++ */ /* VL_EcuM_3472 */
#if !defined(ECUM_LOCAL)
/** Local static definition */
#define ECUM_LOCAL static
#endif

#if !defined(ECUM_LOCAL_INLINE)
/** Local static inline definition */
#define ECUM_LOCAL_INLINE static inline
#endif
/* PRQA S 3472, 3414 -- */
/* ================================================ type definitions ================================================ */

/**
 * @brief  Timer state type definition
 *
 */
typedef enum
{
    ECUM_TIMER_STOP     = 0, /**< Timer is stopped. @range 0x00 */
    ECUM_TIMER_CHECKING = 1, /**< Timer is running and the check of the wake up source is
                               requested. @range 0x01*/
    ECUM_TIMER_VALID = 2,    /**< Timer is running and the check of the wake up source is
                               done synchronously. @range 0x02*/
} EcuM_CheckTimerStateType;

/**
 * @brief  Pre-compile configuration type definition of ECUM sleep mode. The data index in the array of the modes is
 * used as Sleep mode Identifier used in service like EcuM_SelectShutdownTarget
 *
 */
typedef struct
{
    boolean sleepSuspend;                     /**< Flag, which is set true, if the CPU is suspended,
                                               halted, or powered off in the sleep mode. If the
                                               CPU keeps running in this sleep mode, then this
                                               flag must be set to false. . @range 0x00 or 0x01 */
    Mcu_ModeType          mcuMode;            /**< the corresponding MCU mode for this sleep mode. @range 0x00 ~ 0xFF */
    uint8                 numberOfWakeSource; /**< No. of wake up source shall be enabled. @range 0x00 ~ 0xFF  */
    const uint8*          wkSrcIdx;           /**< Reference wake sources index. @range 0x00 ~ 0xFF  */
    EcuM_WakeupSourceType wkMask;             /**< Reference all wake sources mask. @range 0x00 ~ 0xFFFFFFFF
                                               */
} EcuM_SleepModeCfgType;

#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
/**
 * @brief  Pre-compile configuration type definition of ECUM alarm clock
 *
 */
typedef struct
{
    uint32        alarmClkTimeout; /**< Time out for this alarm clock. @range 0x00 ~ 0xFFFFFFFF */
    EcuM_UserType alarmClkUser;    /**< the user this alarm clock assigned to. @range 0x00 ~ 0xFF */
} EcuM_AlarmClockCfgType;
#endif

/**
 * @brief  Pre-compile configuration type definition of ECUM user
 *
 */
typedef struct
{
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId; /**< The core id the user of ECUM is in. @raneg 0x00 ~ 0xFF */
#endif
    boolean goDownAllowed; /**< the user is allowed to call the EcuM_GoDown API. @range 0x00 or 0x01 */
#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
    boolean setClkAllowed; /**< the user is allowed to call the EcuM_SetClock API. @range 0x00 or 0x01 */
#endif
    EcuM_UserType usrId; /**< the bit related with this user. @range 0x00 ~ 0xFF */
} EcuM_UserCfgType;

#if (ECUM_MAX_USER_NUM > 0u)
/**
 * @brief type definition of ECUM user state
 *
 */
typedef struct
{
    uint8 RunNum;                         /**< number of times the user is in Run state. @range 0x00 ~ 0xFF */
    uint8 PostRunNum;                     /**< number of times the user is in PostRun state. @range 0x00 ~ 0xFF */
    /* PRQA S 1039 ++ */                  /* VL_EcuM_FlexibleArray */
    boolean IsRun[ECUM_MAX_USER_NUM];     /**< the user is in Run state. @range 0x00 or 0x01 */
    boolean IsPostRun[ECUM_MAX_USER_NUM]; /**< number of times the user is in PostRun state. @range 0x00 or 0x01 */
    /* PRQA S 1039 -- */
} EcuM_UserStateType;
#endif /*ECUM_USER_NUM > 0u*/

#if (ECUM_MAX_WAKE_UP_SOURCE_NUM > 0u)
/**
 * @brief  type definition of ECUM wake up source
 *
 */
typedef struct
{
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId; /**< The core id the user of ECUM is in. @raneg 0x00 ~ 0xFF */
#endif
    EcuM_WakeupSourceType* Pending;   /**< wake up source which is pending to be validated. @range 0x00 ~ 0xFFFFFFFF */
    EcuM_WakeupSourceType* Validated; /**< wake up source which is validated. @range 0x00 ~ 0xFFFFFFFF */
    EcuM_WakeupSourceType* Expired;   /**< wake up source which is expired. @range 0x00 ~ 0xFFFFFFFF */
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
    Tm_PredefTimer100us32bitType
        wkTime[ECUM_MAX_WAKE_UP_SOURCE_NUM]; /**< wake up source time. @range 0x00 ~ 0xFFFFFFFF */
#else
    TickType wkTime[ECUM_MAX_WAKE_UP_SOURCE_NUM]; /**< wake up source time. @range 0x00 ~ 0xFFFFFFFF */
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
    EcuM_CheckTimerStateType
        timerState[ECUM_MAX_WAKE_UP_SOURCE_NUM]; /**< wake up source timer state. @range 0x00 ~ 0x02 */
} EcuM_WksStateType;
#endif /*ECUM_MAX_WAKE_UP_SOURCE_NUM > 0u*/

/**
 * @brief  type definition of ECUM runtime variable
 *
 */
typedef struct
{
    boolean                EcuM_IsInit; /**< initialization flag. @range 0x00 or 0x01 */
    EcuM_StateType         State;       /**< current state of ECUM. @range 0x00 ~ 0x33  */
    EcuM_StateType         rqstState;   /**< requested state of ECUM. @range 0x00 ~ 0x33 */
    EcuM_ShutdownCauseType cause;       /**< shutdown cause.                             @range 0x00 ~ 0xFF */
    EcuM_BootTargetType    BootTarget;  /**< BootLoader Target. @range 0x00 ~ 0xFF */
#if (ECUM_MAX_MCU_CORE_NUM > 1u)
    CoreIdType coreId;       /**< the core id the runtime variable is in. @range 0x00 ~ 0xFF */
    boolean    rqstShutFlag; /**< flag to indicate if shutdown request is received. @range 0x00 or 0x01 */
#endif                       /**< ECUM_MAX_MCU_CORE_NUM > 1u */
#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
    EcuM_TimeType UserAlarm[ECUM_MAX_ALARM_CLOCK_NUM]; /**< user defined alarm clock. @range 0x00 ~ 0xFFFFFFFF */
    EcuM_TimeType MasterAlarm;                         /**< master alarm clock. @range 0x00 ~ 0xFFFFFFFF */
    EcuM_TimeType GlobalClock;                         /**< global alarm clock. @range 0x00 ~ 0xFFFFFFFF */
    uint16        EcuM_TimeRecord;                     /**<  time record for the current alarm. @range 0x00 ~ 0xFFFF */
#endif                                                 /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/
#if (ECUM_MAX_USER_NUM > 0u)
    EcuM_UserStateType Users; /* user state. @range 0x00 ~ 0xFF */
#endif                        /*ECUM_USER_NUM > 0u*/

#if (ECUM_MAX_WAKE_UP_SOURCE_NUM > 0u)
    EcuM_WksStateType Wks; /* wake up source state. @range 0x00 ~ 0xFF */
#endif                     /*ECUM_MAX_WAKE_UP_SOURCE_NUM > 0u*/

    EcuM_TargetType SdtgNext; /* Next shutdown target. @range 0x00 ~ 0xFF */
    EcuM_TargetType SdtgLast; /* Last shutdown target. @range 0x00 ~ 0xFF */
} EcuM_RunTimeLcType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
/***********configure data generated in pre-compile*************/

/*EcuMOSResource, reference to a OS resource which is used to bring the ECU into
 * sleep mode. In case of multi core each core shall have an own OsResource.*/
#if (1u >= ECUM_MAX_MCU_CORE_NUM)
extern const ResourceType EcuM_OSResource;
#else
extern const ResourceType EcuM_OSResource[ECUM_MAX_MCU_CORE_NUM];
#endif

#if (ECUM_SLEEP_MODE_ENABLED == STD_ON)
/*EcuM Sleep modes configured by pre-compile configure*/
extern const EcuM_SleepModeCfgType EcuM_SleepModeCfgs[ECUM_MAX_SLEEP_MODE_NUM];
#endif

/*The normal MCU mode to be restored after a sleep.*/
extern const Mcu_ModeType EcuM_NormalMcuModeCfg;

/*ECUM user configuration*/
extern const EcuM_UserCfgType EcuM_UserCfgs[ECUM_MAX_USER_NUM];

#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)

/*ECUM user alarm configuration*/
extern const EcuM_AlarmClockCfgType EcuM_AlarmClkCfgs[ECUM_MAX_ALARM_CLOCK_NUM];

#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/* Slave cores IDs */
extern const CoreIdType EcuM_SlaveCoreIds[ECUM_MAX_MCU_CORE_NUM - 1u];
/* Reference spinlock configuration */
extern const SpinlockIdType EcuM_RefSpinLockCfg;

#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
/* Wake up source Pending, Validated and Expired */
extern EcuM_WakeupSourceType EcuMWksPending;
extern EcuM_WakeupSourceType EcuMWksValidated;
extern EcuM_WakeupSourceType EcuMWksExpired;

/*Pointer variable to store the determined PB configuration data*/
extern const EcuM_ConfigType* EcuM_ConfigPtr;

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/*Shutdown request flag, set by master core, check by slave main function*/
extern volatile boolean EcuM_ShutdownFlag;

#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

#if (ECUM_MODE_HANDING == STD_ON)
/* Current Rte State to notify during MainFunction */
extern Rte_ModeType_EcuM_Mode EcuM_RteState;
#endif

/* ========================================= external function declarations ========================================= */
/**
 * @brief       Maps a wakeup source to its corresponding configuration index.
 * @param[in]   wkSrc : Wakeup source type to be mapped.
 * @return      uint8: Configuration index of the wakeup source.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-69010
 */
uint8 EcuM_WkSrcMap2CfgWkIdx(EcuM_WakeupSourceType wkSrc);

/**
 * @brief       Executes the shutdown phase for the specified user.
 * @param[in]   caller : User requesting the shutdown phase.
 * @return      Std_ReturnType: Status of the shutdown operation.
 * @retval      E_OK: Shutdown phase executed successfully.
 * @retval      E_NOT_OK: Shutdown phase failed.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64772
 */
Std_ReturnType EcuM_DoShutDownPhase(EcuM_UserType caller);

/**
 * @brief       Performs the necessary actions to turn off the ECU before the OS starts.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64803
 */
void EcuM_OffPreOS(void);

/**
 * @brief       Updates the ECU Manager's clock with the current runtime information.
 * @param[in]   pRt : Pointer to the runtime information structure.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64875
 */
/* PRQA S 1753,1501 ++ */ /* VL_EcuM_1753,VL_EcuM_1501 */
void EcuM_UpdateEcuMClock(EcuM_RunTimeLcType* pRt);
/* PRQA S 1753,1501 -- */

/**
 * @brief       Cancels all active alarms managed by the ECU Manager.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-69011
 */
/* PRQA S 1501 ++ */ /* VL_EcuM_1501 */
void EcuM_CancellAlarms(void);
/* PRQA S 1501 -- */

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/**
 * @brief       Retrieves the index of the slave core based on the provided core ID.
 * @param[in]   coreId : Identifier of the core.
 * @return      uint8_t: Index of the slave core.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64783
 */
uint8 EcuM_GetSlaveCoreIndex(CoreIdType coreId);
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

/**
 * @brief       Searches for the user index based on the provided caller.
 * @param[in]   caller : User requesting the search.
 * @param[out]  userIndex : Pointer to store the found user index.
 * @return      Std_ReturnType: Status of the search operation.
 * @retval      E_OK: User index found successfully.
 * @retval      E_NOT_OK: User index not found or an error occurred.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64870
 */
Std_ReturnType EcuM_SearchUserByCaller(EcuM_UserType caller, uint8* userIndex);

/**
 * @brief       Checks if the ECU Manager is in the initialization state.
 * @param[out]   EcuMRunDataPtr : Double pointer to the ECU Manager runtime data.
 * @return      boolean: True if the ECU Manager is in the initialization state, otherwise false.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-69012
 */
boolean EcuM_isInitState(EcuM_RunTimeLcType** EcuMRunDataPtr);

/**
 * @brief       Retrieves the pointer to the ECU Manager's runtime data.
 * @return      EcuM_RunTimeLcType : Pointer to the ECU Manager's runtime data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-69013
 */
/* PRQA S 5016 ++ */ /* VL_EcuM_5016 */
EcuM_RunTimeLcType* EcuM_GetLcRunDataPtr(void);
/* PRQA S 5016 -- */

/**
 * @brief       Retrieves the configuration pointer for a specific wakeup source.
 * @param[in]   index : Index of the wakeup source configuration to retrieve.
 * @return      const EcuM_WakeupSourceCfgType*: Pointer to the wakeup source configuration.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       -
 */
/* PRQA S 5016 ++ */ /* VL_EcuM_5016 */
ECUM_LOCAL_INLINE const EcuM_WakeupSourceCfgType* EcuM_GetWakeupSourceCfgPtr(uint8 index)
{
    return &EcuM_ConfigPtr->wkSourceCfgPtr[index];
}
/* PRQA S 5016 -- */

#ifdef __cplusplus
}
#endif
#endif /*ECUM_INTERNAL_H*/
