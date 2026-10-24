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
#include "EcuM_Arti.h"

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
    CoreIdType coreId[ECUM_MAX_WAKE_UP_SOURCE_NUM]; /**< The core id the user of ECUM is in. @raneg 0x00 ~ 0xFF */
#endif
    EcuM_WakeupSourceType* Pending;   /**< wake up source which is pending to be validated. @range 0x00 ~ 0xFFFFFFFF */
    EcuM_WakeupSourceType* Validated; /**< wake up source which is validated. @range 0x00 ~ 0xFFFFFFFF */
    EcuM_WakeupSourceType* Expired;   /**< wake up source which is expired. @range 0x00 ~ 0xFFFFFFFF */
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
    uint32 wkTime[ECUM_MAX_WAKE_UP_SOURCE_NUM]; /**< wake up source time. @range 0x00 ~ 0xFFFFFFFF */
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

    EcuM_TargetType* SdtgNext; /* Next shutdown target. @range 0x00 ~ 0xFF */
    EcuM_TargetType* SdtgLast; /* Last shutdown target. @range 0x00 ~ 0xFF */
} EcuM_RunTimeLcType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
/***********configure data generated in pre-compile*************/

/*EcuMOSResource, reference to a OS resource which is used to bring the ECU into
 * sleep mode. In case of multi core each core shall have an own OsResource.*/
#if (1u >= ECUM_MAX_MCU_CORE_NUM)
#define ECUM_START_SEC_CONFIG_DATA_16
#include "EcuM_MemMap.h"
extern const ResourceType EcuM_OSResource;
#define ECUM_STOP_SEC_CONFIG_DATA_16
#include "EcuM_MemMap.h"
#else
#define ECUM_START_SEC_CONFIG_DATA_16
#include "EcuM_MemMap.h"
extern const ResourceType EcuM_OSResource[ECUM_MAX_MCU_CORE_NUM];
#define ECUM_STOP_SEC_CONFIG_DATA_16
#include "EcuM_MemMap.h"
#endif

#if (ECUM_SLEEP_MODE_ENABLED == STD_ON)
/*EcuM Sleep modes configured by pre-compile configure*/
#define ECUM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EcuM_MemMap.h"
extern const EcuM_SleepModeCfgType EcuM_SleepModeCfgs[ECUM_MAX_SLEEP_MODE_NUM];
#define ECUM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EcuM_MemMap.h"
#endif

/*The normal MCU mode to be restored after a sleep.*/
#define ECUM_START_SEC_CONFIG_DATA_8
#include "EcuM_MemMap.h"
extern const Mcu_ModeType EcuM_NormalMcuModeCfg;
#define ECUM_STOP_SEC_CONFIG_DATA_8
#include "EcuM_MemMap.h"

/*ECUM user configuration*/
#define ECUM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EcuM_MemMap.h"
extern const EcuM_UserCfgType EcuM_UserCfgs[ECUM_MAX_USER_NUM];
#define ECUM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EcuM_MemMap.h"

#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
/*ECUM user alarm configuration*/
#define ECUM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EcuM_MemMap.h"
extern const EcuM_AlarmClockCfgType EcuM_AlarmClkCfgs[ECUM_MAX_ALARM_CLOCK_NUM];
#define ECUM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EcuM_MemMap.h"

#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/* Slave cores IDs */
#define ECUM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EcuM_MemMap.h"
extern const CoreIdType EcuM_SlaveCoreIds[ECUM_MAX_MCU_CORE_NUM - 1u];
#define ECUM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EcuM_MemMap.h"

/* Reference spinlock configuration */
#define ECUM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EcuM_MemMap.h"
extern const SpinlockIdType EcuM_RefSpinLockCfg;
#define ECUM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EcuM_MemMap.h"
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

/* Wake up source Pending, Validated and Expired */
#define ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"
extern EcuM_WakeupSourceType EcuM_WksPending;
#define ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"
extern EcuM_WakeupSourceType EcuM_WksValidated;
#define ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"
extern EcuM_WakeupSourceType EcuM_WksExpired;
#define ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"

/*Pointer variable to store the determined PB configuration data*/
#define ECUM_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"
extern const EcuM_ConfigType* EcuM_ConfigPtr;
#define ECUM_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/*Shutdown request flag, set by master core, check by slave main function*/
#define ECUM_START_SEC_VAR_FAST_CLEARED_BOOLEAN
#include "EcuM_MemMap.h"
extern volatile boolean EcuM_ShutdownFlag;
#define ECUM_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
#include "EcuM_MemMap.h"

#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

#if (ECUM_MODE_HANDING == STD_ON)
/* Current Rte State to notify during MainFunction */
extern Rte_ModeType_EcuM_Mode EcuM_RteState;
#endif /* ECUM_MODE_HANDING == STD_ON */

#if (ECUM_CLONE_VARIABLE_SUPPORT_ENABLED == STD_ON)
#define ECUM_START_SEC_VAR_CLONE_UNSPECIFIED
#include "EcuM_MemMap.h"
extern EcuM_RunTimeLcType EcuMRunData;
#define ECUM_STOP_SEC_VAR_CLONE_UNSPECIFIED
#include "EcuM_MemMap.h"
#endif /* ECUM_CLONE_VARIABLE_SUPPORT_ENABLED == STD_ON */

/* ========================================= external function declarations ========================================= */
/**
 * @brief       Maps a wakeup source to its corresponding configuration index.
 * @param[in]   wkSrc : Wakeup source type to be mapped.
 * @return      uint8: Configuration index of the wakeup source.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-69010
 */
#define ECUM_START_SEC_CODE_FAST
#include "EcuM_MemMap.h"
uint8 EcuM_WkSrcMap2CfgWkIdx(EcuM_WakeupSourceType wkSrc);
#define ECUM_STOP_SEC_CODE_FAST
#include "EcuM_MemMap.h"

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
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
Std_ReturnType EcuM_DoShutDownPhase(EcuM_UserType caller);
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

/**
 * @brief       Performs the necessary actions to turn off the ECU before the OS starts.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64803
 */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
void EcuM_OffPreOS(void);
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
/**
 * @brief       Updates the ECU Manager's clock with the current runtime information.
 * @param[in]   pRt : Pointer to the runtime information structure.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64875
 */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
void EcuM_UpdateEcuMClock(EcuM_RunTimeLcType* pRt);
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

/**
 * @brief       Cancels all active alarms managed by the ECU Manager.
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-69011
 */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
void EcuM_CancellAlarms(void);
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/**
 * @brief       Retrieves the index of the slave core based on the provided core ID.
 * @param[in]   coreId : Identifier of the core.
 * @return      uint8_t: Index of the slave core.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-64783
 */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
uint8 EcuM_GetSlaveCoreIndex(CoreIdType coreId);
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
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
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
Std_ReturnType EcuM_SearchUserByCaller(EcuM_UserType caller, uint8* userIndex);
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

/**
 * @brief       Checks if the ECU Manager is in the initialization state.
 * @param[out]   EcuMRunDataPtr : Double pointer to the ECU Manager runtime data.
 * @return      boolean: True if the ECU Manager is in the initialization state, otherwise false.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-69012
 */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
boolean EcuM_isInitState(EcuM_RunTimeLcType** EcuMRunDataPtr);
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#if (ECUM_CLONE_VARIABLE_SUPPORT_ENABLED == STD_OFF)
/**
 * @brief       Retrieves the pointer to the ECU Manager's runtime data.
 * @return      EcuM_RunTimeLcType : Pointer to the ECU Manager's runtime data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-69013
 */
/* PRQA S 5016 ++ */ /* VL_EcuM_5016 */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
EcuM_RunTimeLcType* EcuM_GetLcRunDataPtr(void);
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/* PRQA S 5016 -- */
#endif /* ECUM_CLONE_VARIABLE_SUPPORT_ENABLED == STD_OFF */

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
