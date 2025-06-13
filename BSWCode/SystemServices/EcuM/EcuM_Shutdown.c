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
**  FILENAME    : EcuM_Shutdown.c                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Implement code for EcuM shutdown phase.                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0       2020-06-28  qinchun.yang    R19_11 EcuM initial version.
 *  V2.0.0       2021-04-08  qinchun.yang    R1911 stable version.
 *  V2.1.0       2021-11-25  qinchun.yang    Shutdown process logic modification.
 */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "EcuM_Internal.h"
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
static FUNC(void, ECUM_CODE) EcuM_SetSdtg(
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(EcuM_RunTimeType, AUTOMATIC, ECUM_VAR) pRt,
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
    EcuM_ShutdownTargetType target,
    EcuM_ShutdownModeType mode);

static FUNC(void, ECUM_CODE) EcuM_GetSdtg(
    P2CONST(EcuM_RunTimeType, AUTOMATIC, ECUM_CONST) pRt,
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(EcuM_ShutdownTargetType, AUTOMATIC, ECUM_VAR) pTarget,
    P2VAR(EcuM_ShutdownModeType, AUTOMATIC, ECUM_VAR) pMode);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/
#if (ECUM_MAX_MCU_CORE_NUM > 1)
/*Shutdown task-ID in master core*/
#define ECUM_START_SEC_VAR_CLEARED_SHARE_16
#include "EcuM_MemMap.h"
static VAR(TaskType, ECUM_CLEARED) EcuM_McShutdownTaskId;
#define ECUM_STOP_SEC_VAR_CLEARED_SHARE_16
#include "EcuM_MemMap.h"
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/*Shutdown request flag, set by master core, check by slave main function*/
#define ECUM_START_SEC_VAR_CLEARED_SHARE_BOOLEAN
#include "EcuM_MemMap.h"
volatile VAR(boolean, ECUM_CLEARED) EcuM_ShutdownFlag;
#define ECUM_STOP_SEC_VAR_CLEARED_SHARE_BOOLEAN
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_VAR_CLEARED_SHARE_BOOLEAN
#include "EcuM_MemMap.h"
static VAR(uint32, ECUM_CLEARED) EcuM_SlaveShutdownEvent;
#define ECUM_STOP_SEC_VAR_CLEARED_SHARE_BOOLEAN
#include "EcuM_MemMap.h"
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/**
 * Typically called from the shutdown hook, this function takes over execution
 * control and will carry out GO OFF II activities.
 * Service ID[hex]: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, ECUM_CODE) EcuM_Shutdown(void)
{
    P2CONST(EcuM_RunTimeType, AUTOMATIC, ECUM_VAR) pRt = &EcuMRunData;
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;

    /*This ECUM instance is running on master or slaves*/
    /*get the core running this ECUM instance*/
    coreId = GetCoreID();
#endif
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if ((boolean)FALSE == EcuM_IsInit)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SHUTDOWN, ECUM_E_UNINIT);
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        /*notify integration code ongoing into OFF II step*/
        EcuM_OnGoOffTwo();

#if (ECUM_MAX_MCU_CORE_NUM > 1)
        /*This instance is running on master or slave*/
        if (ECUM_MASTER_CORE_ID == coreId)
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
        {
            /*Callout EcuM_AL_Reset or Callout EcuM_AL_SwitchOff, depends on the
             *selected shutdown target (RESET or OFF)*/
            if (ECUM_SHUTDOWN_TARGET_RESET == (pRt)->SdtgNext.Target)
            {
                EcuM_AL_Reset((EcuM_ResetType)((pRt->SdtgNext.Mode) - 256u));
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
}

/**
 * EcuM_SelectShutdownTarget selects the shutdown target.
 * EcuM_SelectShutdownTarget is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x06
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): shutdownTarget The selected shutdown target
 *                  shutdownMode   The identifier of a sleep mode (if target is
 *                                 ECUM_STATE_SLEEP) or a reset mechanism
 *                                 (if target is ECUM_STATE_RESET) as defined by configuration.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: The new shutdown target was set
 *               E_NOT_OK: The new shutdown target was not set
 */
FUNC(Std_ReturnType, ECUM_CODE)
EcuM_SelectShutdownTarget(EcuM_ShutdownTargetType shutdownTarget, EcuM_ShutdownModeType shutdownMode)
{
    Std_ReturnType ret = E_OK;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if ((boolean)FALSE == EcuM_IsInit)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SELECTSHUTDOWNTARGET, ECUM_E_UNINIT);
        ret = E_NOT_OK;
    }
    else
    {
        switch (shutdownTarget)
        {
        case ECUM_SHUTDOWN_TARGET_SLEEP:
            if (shutdownMode >= (EcuM_ShutdownModeType)ECUM_MAX_SLEEP_MODE_NUM)
            {
                (void)Det_ReportError(
                    ECUM_MODULE_ID,
                    ECUM_INSTANCE_ID,
                    ECUM_SID_SELECTSHUTDOWNTARGET,
                    ECUM_E_INVALID_PAR);
                ret = E_NOT_OK;
            }
            break;
        case ECUM_SHUTDOWN_TARGET_RESET:
            if (shutdownMode >= (EcuM_ShutdownModeType)ECUM_MAX_RESET_MODE_NUM)
            {
                (void)Det_ReportError(
                    ECUM_MODULE_ID,
                    ECUM_INSTANCE_ID,
                    ECUM_SID_SELECTSHUTDOWNTARGET,
                    ECUM_E_INVALID_PAR);
                ret = E_NOT_OK;
            }
            break;
        case ECUM_SHUTDOWN_TARGET_OFF:
            /*The OFF state requires the capability of the ECU to switch off itself. This is not granted for all
              hardware designs.*/
            break;
        default:
            ret = E_NOT_OK;
            break;
        }
    }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    if ((Std_ReturnType)E_OK == ret)
    {
        EcuM_SetSdtg(&EcuMRunData, shutdownTarget, shutdownMode);
    }
    return ret;
}

/**
 * Returns the currently selected shutdown target as set by EcuM_SelectShutdownTarget.
 * EcuM_GetShutdownTarget is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x09
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): shutdownTarget One of these values is returned:
 *                                 ECUM_STATE_SLEEP/ECUM_STATE_RESET/ECUM_STATE_OFF
 *                  shutdownMode  If the out parameter "shutdownTarget" is
 *                                ECUM_STATE_SLEEP, sleepMode tells which of the
 *                                configured sleep modes was actually chosen.
 *                                If "shutdownTarget" is ECUM_STATE_RESET,
 *                                sleepMode tells which of the configured reset
 *                                modes was actually chosen.
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: The service has failed, e.g. due to NULL pointer being passed
 */
FUNC(Std_ReturnType, ECUM_CODE)
EcuM_GetShutdownTarget(
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(EcuM_ShutdownTargetType, AUTOMATIC, ECUM_APPL_DATA) shutdownTarget,
    P2VAR(EcuM_ShutdownModeType, AUTOMATIC, ECUM_APPL_DATA) shutdownMode)
/* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if ((boolean)FALSE == EcuM_IsInit)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETSHUTDOWNTARGET, ECUM_E_UNINIT);
        ret = E_NOT_OK;
    }
    else if ((NULL_PTR == shutdownTarget) || (NULL_PTR == shutdownMode))
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETSHUTDOWNTARGET, ECUM_E_NULL_POINTER);
        ret = E_NOT_OK;
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        EcuM_GetSdtg(&EcuMRunData, shutdownTarget, shutdownMode);
    }
    return ret;
}

/**
 * Returns the shutdown target of the previous shutdown process.
 * EcuM_GetLastShutdownTarget is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x08
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): shutdownTarget One of these values is returned:
 *                                 ECUM_STATE_SLEEP/ECUM_STATE_RESET/ECUM_STATE_OFF
 *                  shutdownMode  If the out parameter "shutdownTarget" is
 *                                ECUM_STATE_SLEEP, sleepMode tells which of the
 *                                configured sleep modes was actually chosen.
 *                                If "shutdownTarget" is ECUM_STATE_RESET,
 *                                sleepMode tells which of the configured reset
 *                                modes was actually chosen.
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: The service has failed, e.g. due to NULL pointer being passed
 */
FUNC(Std_ReturnType, ECUM_CODE)
EcuM_GetLastShutdownTarget(
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(EcuM_ShutdownTargetType, AUTOMATIC, ECUM_APPL_DATA) shutdownTarget,
    P2VAR(EcuM_ShutdownModeType, AUTOMATIC, ECUM_APPL_DATA) shutdownMode)
/* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if ((boolean)FALSE == EcuM_IsInit)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETLASTSHUTDOWNTARGET, ECUM_E_UNINIT);
        ret = E_NOT_OK;
    }
    else if ((NULL_PTR == shutdownTarget) || (NULL_PTR == shutdownMode))
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETLASTSHUTDOWNTARGET, ECUM_E_NULL_POINTER);
        ret = E_NOT_OK;
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        *(shutdownTarget) = EcuMRunData.SdtgLast.Target;
        *(shutdownMode) = EcuMRunData.SdtgLast.Mode;
    }
    return ret;
}

/**
 * Elects the cause for a shutdown.
 * EcuM_SelectShutdownCause is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x1b
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): target, the selected shutdown cause
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: E_OK: The new shutdown cause was set
 *               E_NOT_OK: The new shutdown cause was not set
 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_SelectShutdownCause(EcuM_ShutdownCauseType cause)
{
    P2VAR(EcuM_RunTimeType, AUTOMATIC, ECUM_VAR) pRt = &EcuMRunData; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    Std_ReturnType ret = E_OK;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if ((boolean)FALSE == EcuM_IsInit)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SELECTSHUTDOWNCAUSE, ECUM_E_UNINIT);
        ret = E_NOT_OK;
    }
    else if (
        (cause != ECUM_CAUSE_ECU_STATE) && (cause != ECUM_CAUSE_WDGM) && (cause != ECUM_CAUSE_DCM)
        && (cause != ECUM_CAUSE_UNKNOWN))
    {
        ret = E_NOT_OK;
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        (pRt)->cause = cause;
    }

    return ret;
}

/**
 * Returns the selected shutdown cause as set by EcuM_SelectShutdownCause.
 * EcuM_GetShutdownCause is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x1c
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  shutdownCause, the selected cause of the next shutdown.
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: The service has failed
 */
/* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
FUNC(Std_ReturnType, ECUM_CODE)
EcuM_GetShutdownCause(P2VAR(EcuM_ShutdownCauseType, AUTOMATIC, ECUM_APPL_DATA) shutdownCause)
/* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if ((boolean)FALSE == EcuM_IsInit)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETSHUTDOWNCAUSE, ECUM_E_UNINIT);
        ret = E_NOT_OK;
    }
    else if (NULL_PTR == shutdownCause)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETSHUTDOWNCAUSE, ECUM_E_NULL_POINTER);
        ret = E_NOT_OK;
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        *(shutdownCause) = EcuMRunData.cause;
    }

    return ret;
}

/*Handle SHUTDOWN Phase*/
FUNC(Std_ReturnType, ECUM_CODE)
EcuM_DoShutDownPhase(uint16 caller)
{
    Std_ReturnType ret;
    uint8 userIndex;

    ret = EcuM_SearchUserByCaller(caller, &userIndex);
    if ((Std_ReturnType)E_OK == ret)
    {
        if ((boolean)TRUE == EcuM_UserCfgs[userIndex].goDownAllowed)
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

/*OffPreOS Sequence*/
FUNC(void, ECUM_CODE)
EcuM_OffPreOS(void)
{
    EcuM_WakeupSourceType pendingWkSrcs;
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;
    uint8 slaveIdx;
    StatusType osRet = E_OK;
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
        /*Master-core enter in SHUTDOWM mode.*/
        BswM_EcuM_CurrentState(ECUM_STATE_SHUTDOWN);
    }

    /*De-init BSW Mode manager*/
    BswM_Deinit();
    /*De-init BSW scheduler*/
    SchM_Deinit();
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    if (ECUM_MASTER_CORE_ID == coreId)
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    {
        /*check for pending wake up events and update shutdown target accordingly*/
        pendingWkSrcs = EcuM_GetPendingWakeupEvents();
        if (0U != pendingWkSrcs)
        {
            (void)EcuM_SelectShutdownTarget(ECUM_SHUTDOWN_TARGET_RESET, EcuM_ConfigPtr->defaultShutdownMode);
        }
    }

#if (ECUM_MAX_MCU_CORE_NUM > 1)
    if (ECUM_MASTER_CORE_ID == coreId)
    {
        osRet = GetSpinlock(EcuM_RefSpinLockCfg);
        if (E_OK != osRet)
        {
            goto ECUM_OS_ERROR_HANDLE;
        }
        /*Get the ID of the task that initiated the shutdown process.(Master core)*/
        osRet = GetTaskID(&EcuM_McShutdownTaskId);
        if (E_OK != osRet)
        {
            goto ECUM_OS_ERROR_HANDLE;
        }
        /*Set a shutdown flag which can be read by all EcuMs of all slave cores*/
        EcuM_ShutdownFlag = TRUE;
        osRet = ReleaseSpinlock(EcuM_RefSpinLockCfg);
        if (E_OK != osRet)
        {
            goto ECUM_OS_ERROR_HANDLE;
        }
        /*Wait until all modules have de-initialized the BSW modules for which they are
         * responsible and successfully shutdown*/
        while (EcuM_ShutdownFlag == TRUE)
        {
            if (EcuM_SlaveShutdownEvent == ((1u << ECUM_SLAVE_CORE_NUM)) - 1u)
            {
                GetSpinlock(EcuM_RefSpinLockCfg);

                EcuM_SlaveShutdownEvent = 0;
                EcuM_ShutdownFlag = FALSE;

                ReleaseSpinlock(EcuM_RefSpinLockCfg);
            }
        }
        /*Unset the shutdown flag*/
        ShutdownAllCores(E_OK);
    }
    else /*Slave core.*/
    {
        /*Get slave core Index in configuration*/
        slaveIdx = EcuM_GetSlaveCoreIndex(coreId);
        if (ECUM_MAX_MCU_CORE_NUM != slaveIdx)
        {
            /*Set event to notify master core using the event of this core*/
            EcuM_SlaveShutdownEvent |= (1u << slaveIdx);
        }
        if (E_OK != osRet)
        {
            goto ECUM_OS_ERROR_HANDLE;
        }
    }
ECUM_OS_ERROR_HANDLE:
    if (E_OK != osRet)
    {
        EcuM_ErrorHook(ECUM_E_CALL_OS_FAILED);
    }
#else  /*Single core system.*/
    ShutdownOS(E_OK);
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
}
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"

static FUNC(void, ECUM_CODE) EcuM_SetSdtg(
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(EcuM_RunTimeType, AUTOMATIC, ECUM_VAR) pRt,
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
    EcuM_ShutdownTargetType target,
    EcuM_ShutdownModeType mode)
{
    (pRt)->SdtgLast.Target = ((pRt)->SdtgNext.Target);
    (pRt)->SdtgLast.Mode = ((pRt)->SdtgNext.Mode);
    (pRt)->SdtgNext.Target = (target);
    (pRt)->SdtgNext.Mode = (mode);
    return;
}

static FUNC(void, ECUM_CODE) EcuM_GetSdtg(
    P2CONST(EcuM_RunTimeType, AUTOMATIC, ECUM_CONST) pRt,
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(EcuM_ShutdownTargetType, AUTOMATIC, ECUM_VAR) pTarget,
    P2VAR(EcuM_ShutdownModeType, AUTOMATIC, ECUM_VAR) pMode)
/* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
{
    *(pTarget) = (pRt)->SdtgNext.Target;
    *(pMode) = (pRt)->SdtgNext.Mode;
    return;
}
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
