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
 **  @file               : EcuM_Shutdown.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : ECU state management
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "EcuM_Internal.h"
#include "BswM.h"
/* ===================================================== macros ===================================================== */
#define ECUM_U8_MAX_VALUE 256u
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/**
 * @brief Sets the shutdown target and mode.
 * @details This function updates the runtime data structure with the new shutdown target and mode. It also saves the
 * current target and mode as the last known values.
 * @param[in] pRt : Pointer to the runtime data structure.
 * @param[in] target : The new shutdown target.
 * @param[in] mode : The new shutdown mode.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-64871
 */
ECUM_LOCAL void EcuM_SetSdtg(EcuM_RunTimeLcType* pRt, EcuM_ShutdownTargetType target, EcuM_ShutdownModeType mode);

/**
 * @brief Retrieves the current shutdown target and mode.
 * @details This function reads the current shutdown target and mode from the runtime data structure and stores them in
 * the provided pointers.
 * @param[in] pRt : Pointer to the runtime data structure.
 * @param[out] pTarget : Pointer to store the current shutdown target.
 * @param[out] pMode : Pointer to store the current shutdown mode.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace -
 */
ECUM_LOCAL void
    EcuM_GetSdtg(const EcuM_RunTimeLcType* pRt, EcuM_ShutdownTargetType* pTarget, EcuM_ShutdownModeType* pMode);

/**
 * @brief Handles pending wakeup events.
 * @details This function checks for pending or validated wakeup events and, if any are found, selects a reset shutdown
 * target with the default shutdown mode.
 * @param None
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-69016
 */
ECUM_LOCAL void EcuM_HandlePendingWakeupEvents(void);

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/**
 * @brief Handles the master-slave shutdown process.
 * @details This function manages the shutdown sequence for both the master and slave cores. The master core sets a
 * shutdown flag and waits for all slave cores to complete their de-initialization and shutdown. The slave cores signal
 * their readiness to shut down by setting a bit in the `EcuM_SlaveShutdownEvent` variable.
 * @param[in] coreId : Identifier of the core (master or slave) that is initiating the shutdown.
 * @return None
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-69309
 */
ECUM_LOCAL void EcuM_HandleMasterSalveShutdown(CoreIdType coreId);
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

#if (ECUM_MAX_MCU_CORE_NUM > 1)
#define ECUM_START_SEC_VAR_FAST_CLEARED_BOOLEAN
#include "EcuM_MemMap.h"
/*Shutdown request flag, set by master core, check by slave main function*/
volatile boolean EcuM_ShutdownFlag;
#define ECUM_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_VAR_FAST_CLEARED_BOOLEAN
#include "EcuM_MemMap.h"
ECUM_LOCAL uint32 EcuM_SlaveShutdownEvent; /* PRQA S 3218 */ /* VL_EcuM_3218 */
#define ECUM_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
#include "EcuM_MemMap.h"
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
/* ========================================== external function definitions ========================================= */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"

/* PRQA S 1503, 2814 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_MultiReturn */
/**
 * Perform the shutdown sequence based on the shutdown target.
 * Notify integration code about going into OFF II step.
 * Call EcuM_AL_Reset or EcuM_AL_SwitchOff based on shutdown target.
 * Report error if ECU Manager is not initialized (if error detection is enabled).
 */
void EcuM_Shutdown(void)
{
    EcuM_RunTimeLcType* pRt;
    if (EcuM_isInitState(&pRt))
    {
        /*notify integration code ongoing into OFF II step*/
        EcuM_OnGoOffTwo();

#if (ECUM_MAX_MCU_CORE_NUM > 1)
        /*This instance is running on master or slave*/
        CoreIdType coreId = pRt->coreId;

        /*This ECUM instance is running on master or slaves*/
        if (ECUM_MASTER_CORE_ID == coreId)
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
        {
            /*Callout EcuM_AL_Reset or Callout EcuM_AL_SwitchOff, depends on the
             *selected shutdown target (RESET or OFF)*/
            if (ECUM_SHUTDOWN_TARGET_RESET == (pRt)->SdtgNext.Target)
            {
                EcuM_AL_Reset((EcuM_ResetType)((pRt->SdtgNext.Mode) - ECUM_U8_MAX_VALUE));
            }
            else if (ECUM_SHUTDOWN_TARGET_OFF == (pRt->SdtgNext.Target))
            {
                EcuM_AL_SwitchOff();
            }
            else
            {
                /*nothing*/
            }
        }
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SHUTDOWN, ECUM_E_UNINIT);
    }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
}
/* PRQA S 1503, 2814 -- */

/**
 * Select the shutdown target for the ECU.
 * This function allows the ECU to select the target for shutdown, such as sleep, reset, or off state.
 * It performs error checks and sets the shutdown target if no errors are detected.
 */
Std_ReturnType EcuM_SelectShutdownTarget(EcuM_ShutdownTargetType shutdownTarget, EcuM_ShutdownModeType shutdownMode)
{
    Std_ReturnType      ret = E_NOT_OK;
    EcuM_RunTimeLcType* pRt;
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    uint8 detErrorId = ECUM_E_OK;
    if (!EcuM_isInitState(&pRt))
    {
        detErrorId = ECUM_E_UNINIT;
    }
    else
    {
        switch (shutdownTarget)
        {
#if (ECUM_SLEEP_MODE_ENABLED == STD_ON)
        case ECUM_SHUTDOWN_TARGET_SLEEP:
            if (shutdownMode >= (EcuM_ShutdownModeType)ECUM_MAX_SLEEP_MODE_NUM)
            {
                detErrorId = ECUM_E_INVALID_PAR;
            }
            else
            {
                ret = E_OK;
            }
            break;
#endif
        case ECUM_SHUTDOWN_TARGET_RESET:
            if (shutdownMode >= (EcuM_ShutdownModeType)ECUM_MAX_RESET_MODE_NUM + (EcuM_ShutdownModeType)256)
            {
                detErrorId = ECUM_E_INVALID_PAR;
            }
            else
            {
                ret = E_OK;
            }
            break;
        case ECUM_SHUTDOWN_TARGET_OFF:
            /*The OFF state requires the capability of the ECU to switch off itself. This is not granted for all
              hardware designs.*/
            ret = E_OK;
            break;
        default:
            break;
        }
    }

    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SELECTSHUTDOWNTARGET, detErrorId);
    }
    if ((Std_ReturnType)E_OK == ret)
#else
    if (EcuM_isInitState(&pRt))
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        EcuM_SetSdtg(pRt, shutdownTarget, shutdownMode);
        ret = E_OK;
    }
    return ret;
}

/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
/*
 * Get the shutdown target and mode for the ECU.
 * This function retrieves the shutdown target and mode for the ECU.
 */
Std_ReturnType EcuM_GetShutdownTarget(EcuM_ShutdownTargetType* shutdownTarget, EcuM_ShutdownModeType* shutdownMode)
{
    Std_ReturnType      ret = E_NOT_OK;
    EcuM_RunTimeLcType* pRt;
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    uint8 detErrorId = ECUM_E_OK;

    /* Init Check */
    if (!EcuM_isInitState(&pRt))
    {
        detErrorId = ECUM_E_UNINIT;
    }
    else if ((NULL_PTR == shutdownTarget) || (NULL_PTR == shutdownMode))
    {
        detErrorId = ECUM_E_INVALID_PAR;
    }
    else
#else
    if (EcuM_isInitState(&pRt))
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        EcuM_GetSdtg(pRt, shutdownTarget, shutdownMode);
        ret = E_OK;
    }

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETSHUTDOWNTARGET, detErrorId);
    }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */
    return ret;
}
/* PRQA S 1503 -- */

/* PRQA S 1503, 2814 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_MultiReturn */
/**
 * Retrieves the last shutdown target and mode.
 */
Std_ReturnType EcuM_GetLastShutdownTarget(EcuM_ShutdownTargetType* shutdownTarget, EcuM_ShutdownModeType* shutdownMode)
{
    Std_ReturnType      ret = E_NOT_OK;
    EcuM_RunTimeLcType* pRt;
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    uint8 detErrorId = ECUM_E_OK;
    /* Init Check */
    if (!EcuM_isInitState(&pRt))
    {
        detErrorId = ECUM_E_UNINIT;
    }
    else if ((NULL_PTR == shutdownTarget) || (NULL_PTR == shutdownMode))
    {
        detErrorId = ECUM_E_NULL_POINTER;
    }
    else
#else
    if (EcuM_isInitState(&pRt))
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        *(shutdownTarget) = pRt->SdtgLast.Target;
        *(shutdownMode)   = pRt->SdtgLast.Mode;
        ret               = E_OK;
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETLASTSHUTDOWNTARGET, detErrorId);
    }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */
    return ret;
}
/* PRQA S 1503, 2814 -- */

/* PRQA S 1503, 2814 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_MultiReturn */
/**
 * Selects the shutdown cause for the ECU module.
 *
 * This function sets the shutdown cause for the ECU module. It first checks if the ECU module is in the initialized
 * state, then validates the shutdown cause. If an error is detected, it reports the corresponding error ID.
 */
Std_ReturnType EcuM_SelectShutdownCause(EcuM_ShutdownCauseType cause)
{
    EcuM_RunTimeLcType* pRt;

    Std_ReturnType ret = E_NOT_OK;
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    uint8 detErrorId = ECUM_E_OK;
    /* Init Check */
    if (!EcuM_isInitState(&pRt))
    {
        detErrorId = ECUM_E_UNINIT;
    }
    else if (cause >= (EcuM_ShutdownCauseType)ECUM_MAX_SHUTDOWN_CAUSE_NUM)
    {
        detErrorId = ECUM_E_INVALID_PAR;
    }
    else
#else
    if (EcuM_isInitState(&pRt))
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        (pRt)->cause = cause;
        ret          = E_OK;
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SELECTSHUTDOWNCAUSE, detErrorId);
    }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */
    return ret;
}
/* PRQA S 1503, 2814 -- */

/* PRQA S 1503, 2814 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_MultiReturn */
/**
 * Retrieves the current shutdown cause for the ECU module.
 *
 * This function gets the shutdown cause that has been set for the ECU module. It performs initialization checks and
 * validates the pointer before returning the shutdown cause.
 */
Std_ReturnType EcuM_GetShutdownCause(EcuM_ShutdownCauseType* shutdownCause)
{
    Std_ReturnType      ret = E_NOT_OK;
    EcuM_RunTimeLcType* pRt;
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    uint8 detErrorId = ECUM_E_OK;
    /* Init Check */
    if (!EcuM_isInitState(&pRt))
    {
        detErrorId = ECUM_E_UNINIT;
    }
    else if (NULL_PTR == shutdownCause)
    {
        detErrorId = ECUM_E_NULL_POINTER;
    }
    else
#else
    if (EcuM_isInitState(&pRt))
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        *(shutdownCause) = pRt->cause;
        ret              = E_OK;
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETSHUTDOWNCAUSE, detErrorId);
    }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */

    return ret;
}
/* PRQA S 1503, 2814 -- */

/* PRQA S 1503,2814,1532,2844 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_MultiReturn,VL_QAC_OneFunRef,VL_EcuM_DerefInvalidPtr */
/**
 * Executes the shutdown phase for the specified ECU user.
 *
 * This function searches for the user by the caller ID and checks if the user is allowed to go down.
 * If allowed, it performs the pre-OS shutdown sequence. If an error is detected, it reports the corresponding error ID.
 */
Std_ReturnType EcuM_DoShutDownPhase(EcuM_UserType caller)
{
    Std_ReturnType ret;
    uint8          userIndex;

    ret = EcuM_SearchUserByCaller(caller, &userIndex);
    if ((Std_ReturnType)E_OK == ret)
    {
        if (EcuM_UserCfgs[userIndex].goDownAllowed)
        {
            EcuM_OffPreOS();
        }
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GODOWNHALTPOLL, ECUM_E_INVALID_PAR);
        ret = E_NOT_OK;
    }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    return ret;
}
/* PRQA S 1503,2814,1532,2844 -- */

/* PRQA S 1503, 1505, 2814 ++ */ /* VL_QAC_NoUsedApi, VL_EcuM_1505, VL_QAC_MultiReturn */
/**
 * Prepares the system for shutdown before the OS is turned off.
 *
 * This function performs various shutdown tasks, including canceling alarms, calling out to `EcuM_OnGoOffOne`,
 * transitioning the master core to the shutdown state, deinitializing BSW mode manager and scheduler,
 * handling pending wakeup events, and finally shutting down the OS.
 */
void EcuM_OffPreOS(void)
{
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
    /*Cancelled alarms.*/
    EcuM_CancellAlarms();
#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/
    /*call callout OnGoOffOne*/
    EcuM_OnGoOffOne();

#if (ECUM_MAX_MCU_CORE_NUM > 1)
    /*get core ID for this running core*/
    coreId = GetCoreID();
    if (ECUM_MASTER_CORE_ID == coreId)
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    {
        /*Master-core enter in SHUTDOWN mode.*/
        BswM_EcuM_CurrentState(ECUM_STATE_SHUTDOWN);
    }

    /*De-init BSW Mode manager*/
    BswM_Deinit();
    /*De-init BSW scheduler*/
    SchM_Deinit();
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    if (ECUM_MASTER_CORE_ID == coreId)
#endif /* ECUM_MAX_MCU_CORE_NUM > 1 */
    {
        EcuM_HandlePendingWakeupEvents();
    }

#if (ECUM_MAX_MCU_CORE_NUM > 1)
    EcuM_HandleMasterSalveShutdown(coreId);
#else  /*Single core system.*/
    ShutdownOS(E_OK);
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
}
/* PRQA S 1503, 1505, 2814 -- */
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Sets the shutdown target and mode for the ECU module.
 *
 * This function updates the current shutdown target and mode, and stores the previous values.
 */
ECUM_LOCAL void EcuM_SetSdtg(EcuM_RunTimeLcType* pRt, EcuM_ShutdownTargetType target, EcuM_ShutdownModeType mode)
{
    (pRt)->SdtgLast.Target = ((pRt)->SdtgNext.Target);
    (pRt)->SdtgLast.Mode   = ((pRt)->SdtgNext.Mode);
    (pRt)->SdtgNext.Target = (target);
    (pRt)->SdtgNext.Mode   = (mode);
    return;
}
/* PRQA S 2814 -- */

/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Retrieves the current shutdown target and mode for the ECU module.
 *
 * This function reads the current shutdown target and mode from the runtime data structure and stores them in the
 * provided pointers.
 */
ECUM_LOCAL void
    EcuM_GetSdtg(const EcuM_RunTimeLcType* pRt, EcuM_ShutdownTargetType* pTarget, EcuM_ShutdownModeType* pMode)
{
    *(pTarget) = (pRt)->SdtgNext.Target;
    *(pMode)   = (pRt)->SdtgNext.Mode;
    return;
}
/* PRQA S 2814 -- */

/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Handles pending wakeup events during the shutdown process.
 *
 * This function checks for pending or validated wakeup events. If any are found, it selects a reset as the shutdown
 * target. The behavior depends on whether wakeup events are ignored during the pre-OS shutdown phase.
 */
ECUM_LOCAL void EcuM_HandlePendingWakeupEvents(void)
{
    uint32 WkSrcsPending   = 0u;
    uint32 WkSrcsValidated = 0u;

    /* IgnoreWakeupEvValOffPreOS is True, then consider wakeup event which do not need validation. */
    WkSrcsValidated = EcuM_GetValidatedWakeupEvents();

    /* IgnoreWakeupEvValOffPreOS is False, then consider wakeup event which do not need validation. */
    if (EcuM_ConfigPtr->IgnoreWakeupEvValOffPreOS == FALSE)
    {
        WkSrcsPending = EcuM_GetPendingWakeupEvents();
    }
    if (((EcuM_ConfigPtr->allWkSource & WkSrcsPending) != 0uL)
        || ((EcuM_ConfigPtr->allWkSource & WkSrcsValidated) != 0uL))
    {
        (void)EcuM_SelectShutdownTarget(ECUM_SHUTDOWN_TARGET_RESET, EcuM_ConfigPtr->defaultShutdownMode);
    }
}
/* PRQA S 2814 -- */

/**
 * @brief Handling the shutdown process for master and slave cores
 *
 */
#if (ECUM_MAX_MCU_CORE_NUM > 1)
/**
 * Manages the shutdown process for both master and slave cores.
 *
 * This function ensures that the master core waits for all slave cores to complete their de-initialization and
 * shutdown. It uses spinlocks to synchronize the shutdown process and sets flags to indicate the shutdown status. If
 * the core is a slave, it updates the slave shutdown event and releases the spinlock. If the core is the master, it
 * waits for all slaves to signal completion before shutting down all cores.
 */
ECUM_LOCAL void EcuM_HandleMasterSalveShutdown(CoreIdType coreId)
{
    StatusType osRet = E_OK;
    do /* PRQA S 0771 */ /* VL_EcuM_0771 */
    {
        if (ECUM_MASTER_CORE_ID == coreId)
        {
            osRet = GetSpinlock(EcuM_RefSpinLockCfg);
            if (E_OK != osRet)
            {
                break;
            }

            /* Set a shutdown flag that can be read by all from the core */
            EcuM_ShutdownFlag = TRUE;

            osRet = ReleaseSpinlock(EcuM_RefSpinLockCfg);
            if (E_OK != osRet)
            {
                break;
            }

            /* Wait for all modules to complete de-initialisation and shutdown successfully */
            boolean lShutdownTag = EcuM_ShutdownFlag;
            while (lShutdownTag)
            {
                /* PRQA S 4397 ++ */ /* VL_EcuM_4397 */
                if (EcuM_SlaveShutdownEvent == ((1uL << ECUM_SLAVE_CORE_NUM) - 1uL))
                /* PRQA S 4397 -- */
                {
                    (void)GetSpinlock(EcuM_RefSpinLockCfg);

                    EcuM_SlaveShutdownEvent = 0u;
                    EcuM_ShutdownFlag       = FALSE;

                    (void)ReleaseSpinlock(EcuM_RefSpinLockCfg);
                }
                lShutdownTag = EcuM_ShutdownFlag;
            }

            ShutdownAllCores(E_OK);
        }
        else
        {
            uint8 slaveIdx = EcuM_GetSlaveCoreIndex(coreId);
            osRet          = GetSpinlock(EcuM_RefSpinLockCfg);
            if (E_OK != osRet)
            {
                break;
            }
            if (ECUM_MAX_MCU_CORE_NUM != slaveIdx)
            {
                EcuM_SlaveShutdownEvent |= ((uint32)(1uL << slaveIdx));
            }
            osRet = ReleaseSpinlock(EcuM_RefSpinLockCfg);
            if (E_OK != osRet)
            {
                break;
            }
        }
    } while (0u); /* PRQA S 2743 */ /* VL_EcuM_2743 */

    if (E_OK != osRet)
    {
        EcuM_ErrorHook(ECUM_E_CALL_OS_FAILED);
    }
}
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
