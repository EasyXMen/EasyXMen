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
**  FILENAME    : EcuM_Sleep.c                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Implement code for SLEEP phase of EcuM.                     **
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
 */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "EcuM_Internal.h"
#if (ECUM_COMM_PNC_ENABLED == STD_ON) || (ECUM_COMM_CHANNEL_ENABLED == STD_ON)
#include "ComM_EcuM.h"
#endif /* ECUM_COMM_PNC_ENABLED == STD_ON */
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
/*GoSleep sequence the ECU Manager module configures hardware for the
upcoming sleep phase and sets the ECU up for the next wakeup event.*/
static FUNC(void, ECUM_CODE) EcuM_GoSleep(void);

/*Activity sequence for HALT*/
static FUNC(void, ECUM_CODE) EcuM_HaltSequence(void);

/*Activity sequence for POLL.*/
static FUNC(void, ECUM_CODE) EcuM_PollSequence(void);

/*Resume form halt sequence*/
static FUNC(void, ECUM_CODE) EcuM_HaltSequenceExit(void);

/*In sleep phase,notify Ecum wake up status to BSWM.*/
static FUNC(void, ECUM_CODE) EcuM_SleepNotifyBswM(void);

/*WakeUpSequence from sleep phase.*/
static FUNC(void, ECUM_CODE) EcuM_WakeupRestart(void);

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/*Used to determine whether the slave core is all ready for sleep*/
static FUNC(boolean, ECUM_CODE) EcuM_JudgeSleep(void);
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/*Clear slave ready sleep flag.*/
static FUNC(void, ECUM_CODE) EcuM_ClrSlaveReadySleepFlag(void);

/*Set slave ready sleep flag.*/
static FUNC(void, ECUM_CODE) EcuM_SetSlaveReadySleepFlag(void);
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

/*After the wake-up event is verified, the action of other modules needs to be notified*/
static FUNC(void, ECUM_CODE) EcuM_ValidateWkEvAction(uint8 wkSrcIdx);

/*Handle SLEEP Phase*/
static FUNC(Std_ReturnType, ECUM_CODE) EcuM_DoSleepPhase(void);
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#if (ECUM_MAX_MCU_CORE_NUM > 1)
/*Shutdown request flag, set by master core, check by slave main function*/
#define ECUM_START_SEC_VAR_CLEARED_SHARE_BOOLEAN
#include "EcuM_MemMap.h"
static volatile VAR(boolean, ECUM_CLEARED) EcuM_SlaveReadySleepFlag[ECUM_SLAVE_CORE_NUM];
#define ECUM_STOP_SEC_VAR_CLEARED_SHARE_BOOLEAN
#include "EcuM_MemMap.h"
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/**
 * Instructs the ECU State Manager module to go into a sleep mode, Reset or OFF
 * depending on the previously selected shutdown target.
 * Service ID: 0x2C
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): caller:Module ID of the calling module. Only special modules are allowed to
 *                      call this function and only valid when shutdown target is RESET or OFF.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_NOT_OK: The request was not accepted.
 *               E_OK:  If the ShutdownTargetType is SLEEP the call successfully returns, the
 *                       ECU has left the sleep again.
 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_GoDownHaltPoll(uint16 caller)
{
    Std_ReturnType ret;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if ((boolean)FALSE == EcuM_IsInit)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GODOWNHALTPOLL, ECUM_E_UNINIT);
        ret = E_NOT_OK;
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        switch (EcuMRunData.SdtgNext.Target)
        {
        case ECUM_SHUTDOWN_TARGET_RESET:
            /*Do nothing*/
        case ECUM_SHUTDOWN_TARGET_OFF:
            /*EcuM_GoDownHaltPoll with shutdown target RESET or OFF initiates the SHUTDOWN Phase.*/
            ret = EcuM_DoShutDownPhase(caller);
            break;
        case ECUM_SHUTDOWN_TARGET_SLEEP:
            /*EcuM_GoDownHaltPoll with shutdown target SLEEP initiate the SLEEP phase.*/
            ret = EcuM_DoSleepPhase();
            break;
        default:
            ret = E_NOT_OK;
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
            (void)Det_ReportError(
                ECUM_MODULE_ID,
                ECUM_INSTANCE_ID,
                ECUM_SID_GODOWNHALTPOLL,
                ECUM_E_STATE_PAR_OUT_OF_RANGE);
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
            break;
        }
    }
    return ret;
}

/**
 * Gets pending wakeup events.
 * Service ID[hex]: 0x0d
 * Sync/Async: Synchronous
 * Reentrancy: Non-Reentrant, Non-Interruptible
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: All wakeup events
 */
FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetPendingWakeupEvents(void)
{
    EcuM_WakeupSourceType wks = ECUM_WKSOURCE_NONE;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if ((boolean)FALSE == EcuM_IsInit)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETPENDINGWAKEUPEVENT, ECUM_E_UNINIT);
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        /*Enter critical protection zone*/
        SchM_Enter_EcuM_WkEv();
        wks = EcuMWksPending;
        SchM_Exit_EcuM_WkEv();
    }
    return wks;
}

/**
 * Clears wakeup events.
 * Service ID[hex]: 0x16
 * Sync/Async: Synchronous
 * Reentrancy: Non-Reentrant, Non-Interruptible
 * Parameters (in): sources, events to be cleared
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_CODE) EcuM_ClearWakeupEvent(EcuM_WakeupSourceType sources)
{
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if ((boolean)FALSE == EcuM_IsInit)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_CLEARWAKEUPEVENT, ECUM_E_UNINIT);
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        /*Enter critical protection zone*/
        SchM_Enter_EcuM_WkEv();
        EcuMWksPending = (EcuM_WakeupSourceType)(EcuMWksPending & (~sources));
        EcuMWksValidated = (EcuM_WakeupSourceType)(EcuMWksValidated & (~sources));
        EcuMWksExpired = (EcuM_WakeupSourceType)(EcuMWksExpired & (~sources));
        SchM_Exit_EcuM_WkEv();
        BswM_EcuM_CurrentWakeup(sources, ECUM_WKSTATUS_NONE);
    }
    return;
}

/**
 * Gets validated wakeup events.
 * Service ID[hex]: 0x15
 * Sync/Async: Synchronous
 * Reentrancy: Non-Reentrant, Non-Interruptible
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: All wakeup events
 */
FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetValidatedWakeupEvents(void)
{
    EcuM_WakeupSourceType wks = ECUM_WKSOURCE_NONE;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if ((boolean)FALSE == EcuM_IsInit)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETVALIDATEDWAKEUPEVENT, ECUM_E_UNINIT);
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        /*Enter critical protection zone*/
        SchM_Enter_EcuM_WkEv();
        wks = (*EcuMRunData.Wks.Validated);
        SchM_Exit_EcuM_WkEv();
    }
    return wks;
}

/**
 * Gets expired wakeup events.
 * Service ID[hex]: 0x19
 * Sync/Async: Synchronous
 * Reentrancy: Non-Reentrant, Non-Interruptible
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: All wakeup events:Returns all events that have been set and
 *               for which validation has failed. Events which do not need
 *               validation must never be reported by this function.
 */
FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetExpiredWakeupEvents(void)
{
    EcuM_WakeupSourceType wks = ECUM_WKSOURCE_NONE;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if ((boolean)FALSE == EcuM_IsInit)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETEXPIREDWAKEUPEVENT, ECUM_E_UNINIT);
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        /*Enter critical protection zone*/
        SchM_Enter_EcuM_WkEv();
        wks = (*EcuMRunData.Wks.Expired);
        SchM_Exit_EcuM_WkEv();
    }
    return wks;
}
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_SETWAKEUPEVENT_CALLBACK_CODE
#include "EcuM_MemMap.h"
/**
 * Sets the wakeup event.
 * Sets (OR-operation) all events passed as a bit set in the sources in parameter
 * (EcuM_WakeupSourceType bitmask) in the internal pending wakeup events variable.
 * Start the wakeup validation timeout timer.
 * Shall ignore all events passed in the sources parameter not associated to the
 * selected sleep mode.
 * Must be callable from interrupt context, from OS context and an OS-free context.
 * Service ID[hex]: 0x0c
 * Sync/Async: Synchronous
 * Reentrancy: Non-Reentrant, Non-Interruptible
 * Parameters (in): sources, value to be set
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_SETWAKEUPEVENT_CODE) EcuM_SetWakeupEvent(EcuM_WakeupSourceType sources)
{
    P2VAR(EcuM_RunTimeType, AUTOMATIC, ECUM_VAR) pRt = &EcuMRunData; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    uint32 spanTime;
    EcuM_ShutdownModeType sleepModeId;
    uint8 wksIdx;
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
    Std_ReturnType ret;
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if ((boolean)FALSE == EcuM_IsInit)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SETWAKEUPEVENT, ECUM_E_UNINIT);
    }
    /*SWS_EcuM_02867*/
    else if (
        (ECUM_WKSOURCE_NONE == sources)
        || (ECUM_WKSOURCE_NONE != (EcuM_WakeupSourceType)(sources & (~((EcuM_WakeupSourceType)ECUM_ALL_WKSOURCE)))))
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SETWAKEUPEVENT, ECUM_E_UNKNOWN_WAKEUP_SOURCE);
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        /*SWS_EcuM_04138
         * Ignore all events passed in the sources parameter that are not associated
         * to the selected sleep mode*/
        sleepModeId = pRt->SdtgNext.Mode;
        if ((sources & EcuM_SleepModeCfgs[sleepModeId].wkMask) != ECUM_WKSOURCE_NONE)
        {
            SchM_Enter_EcuM_WkEv();
            (*pRt->Wks.Pending) |= sources;
            SchM_Exit_EcuM_WkEv();

            /*Calculate config wake up index.*/
            wksIdx = EcuM_WkSrcMap2CfgWkIdx(sources);
            if (wksIdx != (uint8)ECUM_MAX_WAKE_UP_SOURCE_NUM)
            {
                if (EcuM_WkSourceCfgs[wksIdx].checkWkupTimeout != 0u)
                {
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
                    ret = Tm_GetTimeSpan100us32bit(&(pRt->Wks.wkTime[wksIdx]), &spanTime);
#else
                    spanTime = EcuM_CalculateElapsedMS(pRt->Wks.wkTime[wksIdx]);
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
                    if ((EcuM_WkSourceCfgs[wksIdx].checkWkupTimeout < spanTime)
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
                        || (ret != E_OK)
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
                    )
                    {
                        /*Check wake-up timeout.*/
                        (*pRt->Wks.Pending) &= ~(sources);
                        /*Update internal variable(record expired wake up source.)*/
                        (*pRt->Wks.Expired) |= sources;
                        /*notify BSWM for expired wake up source*/
                        BswM_EcuM_CurrentWakeup(sources, ECUM_WKSTATUS_EXPIRED);
                        /*stop wake up source*/
                        EcuM_StopWakeupSources(sources);
                        /* PRQA S 2001 ++ */ /* MISRA Rule 15.1 */
                        goto SET_WAKEUP_EVENT;
                        /* PRQA S 2001 -- */ /* MISRA Rule 15.1 */
                    }
                }
                if (EcuM_WkSourceCfgs[wksIdx].validationTimeout != 0u)
                {
                    /* Modify the process, put EcuM_StartWakeupSources into
                     * EcuM_SetWakeupEvent and execute(For pending wake up source).*/
                    EcuM_StartWakeupSources(sources);
#if (ECUM_USE_TIMER == ECUM_TIMER_USE_GPT)
                    /*SWS_EcuM_02707
                     * Start validation timer*/
                    (void)Tm_ResetTimer100us32bit(&(pRt->Wks.wkTime[wksIdx]));
#else  /*Use Os counter.*/
                    pRt->Wks.wkTime[wksIdx] = EcuM_CurrentTimestampMS();
#endif /*ECUM_USE_TIMER == ECUM_TIMER_USE_GPT*/
                }
                else /*No validation.*/
                {
                    (*pRt->Wks.Pending) &= ~sources;
                    (*pRt->Wks.Validated) |= sources;
                    if (EcuMRunData.State >= ECUM_STATE_STARTUP)
                    {
                        EcuM_ValidateWkEvAction(wksIdx);
                    }
                }
            }
        }
    }
SET_WAKEUP_EVENT:
    return;
}
#define ECUM_STOP_SEC_SETWAKEUPEVENT_CALLBACK_CODE
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_VALIDATEWAKEUPEVENT_CALLBACK_CODE
#include "EcuM_MemMap.h"
/**
 * After wakeup, the ECU State Manager will stop the process during the WAKEUP
 * VALIDATION state/sequence to wait for validation of the wakeup event.
 * This API service is used to indicate to the ECU Manager module that the wakeup
 * events indicated in the sources parameter have been validated.
 * Service ID[hex]: 0x14
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): sources, events that have been validated
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: NA
 */
FUNC(void, ECUM_VALIDATEWAKEUPEVENT_CODE) EcuM_ValidateWakeupEvent(EcuM_WakeupSourceType sources)
{
    P2CONST(EcuM_RunTimeType, AUTOMATIC, ECUM_CONST) pRt = &EcuMRunData; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    uint8 wkIdx;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if ((boolean)FALSE == EcuM_IsInit)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_VALIDATEWAKEUPEVENT, ECUM_E_UNINIT);
    }
    /* SWS_EcuM_02868 */
    else if (
        (ECUM_WKSOURCE_NONE == sources)
        || (ECUM_WKSOURCE_NONE != (EcuM_WakeupSourceType)(sources & (~((EcuM_WakeupSourceType)ECUM_ALL_WKSOURCE)))))
    {
        (void)Det_ReportError(
            ECUM_MODULE_ID,
            ECUM_INSTANCE_ID,
            ECUM_SID_VALIDATEWAKEUPEVENT,
            ECUM_E_UNKNOWN_WAKEUP_SOURCE);
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        /*SWS_EcuM_02790*/
        if (ECUM_STATE_RUN == (pRt->State & ECUM_STATE_RUN))
        {
            SchM_Enter_EcuM_WkEv();
            (*pRt->Wks.Pending) &= ~(sources);
            (*pRt->Wks.Validated) |= sources;
            SchM_Exit_EcuM_WkEv();
            wkIdx = EcuM_WkSrcMap2CfgWkIdx(sources);
            if (wkIdx != (uint8)ECUM_MAX_WAKE_UP_SOURCE_NUM)
            {
                EcuM_ValidateWkEvAction(wkIdx);
            }
        }
    }
}

FUNC(uint8, ECUM_CODE)
EcuM_WkSrcMap2CfgWkIdx(EcuM_WakeupSourceType wkSrc)
{
    uint8 wkIdx = ECUM_MAX_WAKE_UP_SOURCE_NUM;
    uint8 idx;

    for (idx = 0; idx < (uint8)ECUM_MAX_WAKE_UP_SOURCE_NUM; idx++)
    {
        if ((EcuM_WkSourceCfgs[idx].wkSource & wkSrc) != 0u)
        {
            wkIdx = idx;
        }
    }
    return wkIdx;
}
#define ECUM_STOP_SEC_VALIDATEWAKEUPEVENT_CALLBACK_CODE
#include "EcuM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/*GoSleep sequence the ECU Manager module configures hardware for the
upcoming sleep phase and sets the ECU up for the next wakeup event.*/
static FUNC(void, ECUM_CODE) EcuM_GoSleep(void)
{
    P2CONST(EcuM_RunTimeType, AUTOMATIC, ECUM_VAR) pRt = &EcuMRunData;
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;
    uint8 slaveIdx;
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    StatusType osRet = E_OK;
    uint8 index;
    EcuM_ShutdownModeType sleepModeId;
    uint8 wksNum;
    uint8 refWkSrcIdx;

    sleepModeId = pRt->SdtgNext.Mode;
    if (sleepModeId <= (EcuM_ShutdownModeType)ECUM_MAX_SLEEP_MODE_NUM)
    {
        wksNum = EcuM_SleepModeCfgs[sleepModeId].numberOfWakeSource;
#if (ECUM_MAX_MCU_CORE_NUM > 1)
        /*get core ID for this running core*/
        coreId = GetCoreID();
        if (ECUM_MASTER_CORE_ID == coreId)
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
        {
            for (index = 0; index < wksNum; index++)
            {
                refWkSrcIdx = EcuM_SleepModeCfgs[sleepModeId].wkSrcIdx[index];
                /*To set the wakeup sources up for the next sleep mode, the ECU Manager
                 *module shall execute the EcuM_EnableWakeupSources callout for each wakeup
                 *source that is configured for the target sleep mode*/
                EcuM_EnableWakeupSources(EcuM_WkSourceCfgs[refWkSrcIdx].wkSource);
                /*Notify BswM about wake up state.*/
                BswM_EcuM_CurrentWakeup(EcuM_WkSourceCfgs[refWkSrcIdx].wkSource, ECUM_WKSTATUS_NONE);
            }
#if (ECUM_MAX_MCU_CORE_NUM > 1)
            osRet = GetResource(EcuM_OSResource[ECUM_MASTER_CORE_ID]);
#else  /*Single core.*/
            osRet = GetResource(EcuM_OSResource);
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
        }
#if (ECUM_MAX_MCU_CORE_NUM > 1)
        else /*Slave core sleep phase.*/
        {
            for (index = 0; index < wksNum; index++)
            {
                refWkSrcIdx = EcuM_SleepModeCfgs[sleepModeId].wkSrcIdx[index];
                EcuM_EnableWakeupSources(EcuM_WkSourceCfgs[refWkSrcIdx].wkSource);
            }
            /*Get slave core Index in configuration*/
            slaveIdx = EcuM_GetSlaveCoreIndex(coreId);
            osRet = GetResource(EcuM_OSResource[slaveIdx]);
        }
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
        /*Enter in SLEEP mode.*/
        EcuMRunData.State = ECUM_STATE_SLEEP;
        BswM_EcuM_CurrentState(ECUM_STATE_SLEEP);
    }
    if ((StatusType)E_OK != osRet)
    {
        EcuM_ErrorHook(ECUM_E_CALL_OS_FAILED);
    }
}

/*Activity sequence for HALT*/
static FUNC(void, ECUM_CODE) EcuM_HaltSequence(void)
{
    EcuM_ShutdownModeType sleepModeId;
    EcuM_WakeupSourceType pendWks;
    EcuM_WakeupSourceType ValidatedWks;

#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;
    uint8 slaveIdx;
    StatusType osRet = E_OK;

    /*The master core could issue the MCU halt before all slave cores are finished processing,
     * the cores must be synchronized before entering GoHalt.*/
    coreId = GetCoreID();
    if (ECUM_MASTER_CORE_ID == coreId)
    {
        /*Wait for all Slave cores to be ready to sleep()*/
        while (FALSE == EcuM_JudgeSleep())
        {
            /* Waiting for a wake-up to occur from when the core is dormant */
            pendWks = EcuM_GetPendingWakeupEvents();
            ValidatedWks = EcuM_GetValidatedWakeupEvents();
            if (((ECUM_ALL_WKSOURCE & pendWks) != 0u) || ((ECUM_ALL_WKSOURCE & ValidatedWks) != 0u))
            {
                break;
            }
        }
    }
    else /*Slave core*/
    {
        slaveIdx = EcuM_GetSlaveCoreIndex(coreId);
        osRet = GetSpinlock(EcuM_RefSpinLockCfg);
        if (E_OK != osRet)
        {
            goto ECUM_HALT_SEQ;
        }
        EcuM_SlaveReadySleepFlag[slaveIdx] = TRUE;
        osRet = ReleaseSpinlock(EcuM_RefSpinLockCfg);
    }
ECUM_HALT_SEQ:
    if (E_OK != osRet)
    {
        EcuM_ErrorHook(ECUM_E_CALL_OS_FAILED);
    }
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    /*Disable all interrupts*/
    DisableAllInterrupts();
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    if (ECUM_MASTER_CORE_ID == coreId)
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    {
        EcuM_GenerateRamHash();
    }
    /* When a wakeup occurs and is not pending or validated, EcuM continues to sleep */
    while (TRUE)
    {
        pendWks = EcuM_GetPendingWakeupEvents();
        ValidatedWks = EcuM_GetValidatedWakeupEvents();
        if (((ECUM_ALL_WKSOURCE & pendWks) == 0u) && ((ECUM_ALL_WKSOURCE & ValidatedWks) == 0u))
        {
            sleepModeId = EcuMRunData.SdtgLast.Mode;
            /*Enter in halt mode,ECU Manager module does not execute any code.*/
            EcuM_McuSetMode(EcuM_SleepModeCfgs[sleepModeId].mcuMode);
            /*Enable all interrupts*/
            EnableAllInterrupts();
        }
        else
        {
            /*Enable all interrupts*/
            EnableAllInterrupts();
            break;
        }
    }
    /*Wait resume form MCU Halt mode.*/
    EcuM_HaltSequenceExit();
}

/*Activity sequence for POLL.*/
static FUNC(void, ECUM_CODE) EcuM_PollSequence(void)
{
    EcuM_ShutdownModeType sleepModeId;
    uint8 index;
    EcuM_WakeupSourceType pendingWkup;
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;
    uint8 slaveIdx;
    StatusType osRet = E_OK;
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

    DisableAllInterrupts();
    sleepModeId = EcuMRunData.SdtgNext.Mode;
    /*EcuM_McuSetMode() puts the microcontroller in some power saving mode.
     * In this mode software execution continues, but with reduced clock speed. */
    EcuM_McuSetMode(EcuM_SleepModeCfgs[sleepModeId].mcuMode);
    EnableAllInterrupts();
    do
    {
        /*Sleep activity*/
        EcuM_SleepActivity();

        /*loop FOR all wakeup sources that need polling*/
        for (index = 0; index < (uint8)ECUM_MAX_WAKE_UP_SOURCE_NUM; index++)
        {
            if ((boolean)TRUE == EcuM_WkSourceCfgs[index].isPolling)
            {
                /*check the wakeup source*/
                EcuM_CheckWakeup(EcuM_WkSourceCfgs[index].wkSource);
            }
        }
        pendingWkup = EcuM_GetPendingWakeupEvents();
#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
        /*Get the expired wakeup sources*/
        if ((ECUM_WKSOURCE_ALARMCLOCK == (((*EcuMRunData.Wks.Validated)) & ECUM_WKSOURCE_ALARMCLOCK))
            || (EcuMRunData.GlobalClock >= EcuMRunData.MasterAlarm))
        {
            /*Additional Confidition to Loop: While (AlarmClockService Present AND EcuM_AlarmClock
             *  only pending event AND Alarm not expired)*/
            break;
        }
#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/
    } while (0u == (ECUM_ALL_WKSOURCE & pendingWkup));
    EcuM_SleepNotifyBswM();
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    coreId = GetCoreID();
    /*Signal all slave cores to continue.*/
    if (ECUM_MASTER_CORE_ID == coreId)
    {
        EcuM_SetSlaveReadySleepFlag();
    }
    else /*Slave core*/
    {
        slaveIdx = EcuM_GetSlaveCoreIndex(coreId);
        /*Wait for master core signal to continue.*/
        while (FALSE == EcuM_SlaveReadySleepFlag[slaveIdx])
            ;
        osRet = GetSpinlock(EcuM_RefSpinLockCfg);
        if (E_OK != osRet)
        {
            goto ECUM_POLL_SEQ;
        }
        /*Clear slave core ready sleep flag.*/
        EcuM_SlaveReadySleepFlag[slaveIdx] = FALSE;
        osRet = ReleaseSpinlock(EcuM_RefSpinLockCfg);
    ECUM_POLL_SEQ:
        if (E_OK != osRet)
        {
            EcuM_ErrorHook(ECUM_E_CALL_OS_FAILED);
        }
    }
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
}

/*Resume form halt sequence*/
static FUNC(void, ECUM_CODE) EcuM_HaltSequenceExit(void)
{
#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
    uint8 sleepModeId;
#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/
    uint8 ramHash;
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;
    uint8 slaveIdx;
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

#if (ECUM_MAX_MCU_CORE_NUM > 1)
    coreId = GetCoreID();
    if (ECUM_MASTER_CORE_ID == coreId)
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    {
#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
        /*Condition:
         * EcuM_AlarmClock only pending event AND Alarm not expired*/
        if (EcuMRunData.MasterAlarm > EcuMRunData.GlobalClock)
        {
            /*Disable all interrupts*/
            DisableAllInterrupts();
            EcuM_GenerateRamHash();
            sleepModeId = EcuMRunData.SdtgNext.Mode;
            /*Enter in halt mode,ECU Manager module does not execute any code.*/
            EcuM_McuSetMode(EcuM_SleepModeCfgs[sleepModeId].mcuMode);
            EnableAllInterrupts();
        }
#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/

        /*returned from halt, check RAM hash*/
        ramHash = EcuM_CheckRamHash();
        if (ramHash == 0u)
        {
            /*RAM check failed,EcuM_ErrorHook will not return.*/
            EcuM_ErrorHook(ECUM_E_RAM_CHECK_FAILED);
        }
    }
    /*RAM check success*/
    EcuM_SleepNotifyBswM();
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    /*Signal all slave cores to continue.*/
    if (ECUM_MASTER_CORE_ID == coreId)
    {
        EcuM_ClrSlaveReadySleepFlag();
    }
    else /*Slave core*/
    {
        slaveIdx = EcuM_GetSlaveCoreIndex(coreId);
        /*Wait for master core signal to continue.*/
        while (TRUE == EcuM_SlaveReadySleepFlag[slaveIdx])
            ;
    }
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
}

/*In sleep phase,notify Ecum wake up status to BSWM.*/
static FUNC(void, ECUM_CODE) EcuM_SleepNotifyBswM(void)
{
    P2CONST(EcuM_RunTimeType, AUTOMATIC, ECUM_VAR) pRt = &EcuMRunData;
    uint8 index;

    for (index = 0; index < (uint8)ECUM_MAX_WAKE_UP_SOURCE_NUM; index++)
    {
        if ((EcuM_WkSourceCfgs[index].wkSource & (*pRt->Wks.Pending)) != 0u)
        {
            /*wakeup validation needed or not*/
            if (((EcuM_WkSourceCfgs[index].wkSource & (*pRt->Wks.Validated)) != 0u)
                && (EcuM_WkSourceCfgs[index].validationTimeout > 0u))
            {
                /*validation needed*/
                BswM_EcuM_CurrentWakeup(EcuM_WkSourceCfgs[index].wkSource, ECUM_WKSTATUS_PENDING);
            }
            else
            {
                /*validation not need*/
                if (EcuM_WkSourceCfgs[index].wkSource < ECUM_WKSOURCE_EXTERNAL_WDG)
                {
                    BswM_EcuM_CurrentWakeup(EcuM_WkSourceCfgs[index].wkSource, ECUM_WKSTATUS_VALIDATED);
                }
            }
        }
    }
}

/*WakeUpSequence from sleep phase.*/
static FUNC(void, ECUM_CODE) EcuM_WakeupRestart(void)
{
    EcuM_WakeupSourceType pendingWkSrc;
    EcuM_WakeupSourceType validatedWkSrc;
    StatusType osRet;
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    CoreIdType coreId;
    uint8 slaveIdx;
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
    /*Cancelled alarms.*/
    EcuM_CancellAlarms();
#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/
    /*Edit by yqc,
     * 2020/08/28
     * Some MCU don't need to call EcuM_McuSetMode to reach normal mode.*/
    if (EcuM_NormalMcuModeCfg != ECUM_DAFULT_MCU_NORMAL_MODE)
    {
        DisableAllInterrupts();
        /*Restore MCU normal mode
         * Selected MCU mode is configured in the configuration parameter EcuMNormalMcuModeRef*/
        EcuM_McuSetMode(EcuM_NormalMcuModeCfg);
        EnableAllInterrupts();
    }

    pendingWkSrc = EcuM_GetPendingWakeupEvents();

    /*Edit by yqc,
     * 2020/08/28
     * Some wakeup source don't need validate,also need to call EcuM_DisableWakeupSources.
     * */
    validatedWkSrc = EcuM_GetValidatedWakeupEvents();

    /*Disable currently pending wakeup source but leave the others armed so
     *that later wakeups are possible.*/
    EcuM_DisableWakeupSources(pendingWkSrc | validatedWkSrc);

#if (ECUM_MAX_MCU_CORE_NUM > 1)
    coreId = GetCoreID();
    if (ECUM_MASTER_CORE_ID == coreId)
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    {
#if (ECUM_DRIVER_RESTART_LIST == STD_ON)
        /*Initialize drivers that need restarting*/
        EcuM_AL_DriverRestart();
#endif
#if (ECUM_MAX_MCU_CORE_NUM > 1)
        osRet = ReleaseResource(EcuM_OSResource[ECUM_MASTER_CORE_ID]);
#else  /*Single core.*/
        osRet = ReleaseResource(EcuM_OSResource);
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    }
#if (ECUM_MAX_MCU_CORE_NUM > 1)
    else /*Slave Core*/
    {
        /*Get slave core Index in configuration*/
        slaveIdx = EcuM_GetSlaveCoreIndex(coreId);
        osRet = ReleaseResource(EcuM_OSResource[slaveIdx]);
    }
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/
    if ((StatusType)E_OK != osRet)
    {
        EcuM_ErrorHook(ECUM_E_CALL_OS_FAILED);
    }
}

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/*Used to determine whether the slave core is all ready for sleep*/
static FUNC(boolean, ECUM_CODE) EcuM_JudgeSleep(void)
{
    CoreIdType cIdx;
    CoreIdType slaveCoreNum;
    boolean ret = TRUE;

    slaveCoreNum = ECUM_MAX_MCU_CORE_NUM - 1u;
    for (cIdx = 0; cIdx < slaveCoreNum; cIdx++)
    {
        if (TRUE != EcuM_SlaveReadySleepFlag[cIdx])
        {
            ret = FALSE;
            break;
        }
    }
    return ret;
}
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

#if (ECUM_MAX_MCU_CORE_NUM > 1)
/*Clear slave ready sleep flag.*/
static FUNC(void, ECUM_CODE) EcuM_ClrSlaveReadySleepFlag(void)
{
    StatusType osRet = E_OK;
    uint8 slaveIdx;

    osRet = GetSpinlock(EcuM_RefSpinLockCfg);
    if (E_OK != osRet)
    {
        goto ECUM_CLR_SLEEP_FLAG;
    }
    for (slaveIdx = 0; slaveIdx < ECUM_MAX_MCU_CORE_NUM - 1u; slaveIdx++)
    {
        EcuM_SlaveReadySleepFlag[slaveIdx] = FALSE;
    }
    osRet = ReleaseSpinlock(EcuM_RefSpinLockCfg);
ECUM_CLR_SLEEP_FLAG:
    if (E_OK != osRet)
    {
        EcuM_ErrorHook(ECUM_E_CALL_OS_FAILED);
    }
}

/*Set slave ready sleep flag.*/
static FUNC(void, ECUM_CODE) EcuM_SetSlaveReadySleepFlag(void)
{
    StatusType osRet = E_OK;
    uint8 slaveIdx;

    osRet = GetSpinlock(EcuM_RefSpinLockCfg);
    if (E_OK != osRet)
    {
        goto ECUM_SET_SLEEP_FLAG;
    }
    for (slaveIdx = 0; slaveIdx < ECUM_MAX_MCU_CORE_NUM - 1u; slaveIdx++)
    {
        EcuM_SlaveReadySleepFlag[slaveIdx] = TRUE;
    }
    osRet = ReleaseSpinlock(EcuM_RefSpinLockCfg);
ECUM_SET_SLEEP_FLAG:
    if (E_OK != osRet)
    {
        EcuM_ErrorHook(ECUM_E_CALL_OS_FAILED);
    }
}
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

/*After the wake-up event is verified, the action of other modules needs to be notified*/
static FUNC(void, ECUM_CODE) EcuM_ValidateWkEvAction(uint8 wkSrcIdx)
{
    P2CONST(EcuM_WakeupSourceCfgType, AUTOMATIC, ECUM_CONST) pWks;
#if (ECUM_COMM_PNC_ENABLED == STD_ON)
    PNCHandleType pncIdx;
#endif /*ECUM_COMM_PNC_ENABLED == STD_ON*/

    pWks = &(EcuM_WkSourceCfgs[wkSrcIdx]);
#if (ECUM_COMM_CHANNEL_ENABLED == STD_ON)
    if (NULL_PTR != pWks->comMChnl)
    {
        ComM_EcuM_WakeUpIndication(*(pWks->comMChnl));
    }
#endif /* ECUM_COMM_CHANNEL_ENABLED == STD_ON */
#if (ECUM_COMM_PNC_ENABLED == STD_ON)
    if (NULL_PTR != pWks->pnc)
    {
        /*notify COMM for PNC wake up*/
        for (pncIdx = 0; pncIdx < pWks->pncNum; pncIdx++)
        {
            ComM_EcuM_PNCWakeUpIndication(pWks->pnc[pncIdx]);
        }
    }
#endif /*ECUM_COMM_PNC_ENABLED == STD_ON*/
    BswM_EcuM_CurrentWakeup(pWks->wkSource, ECUM_WKSTATUS_VALIDATED);
}

/*Handle SLEEP Phase*/
static FUNC(Std_ReturnType, ECUM_CODE) EcuM_DoSleepPhase(void)
{
    Std_ReturnType ret = E_NOT_OK;
    EcuM_ShutdownModeType sleepModeId;
    boolean doHalt;

    /*prepare to go sleep*/
    EcuM_GoSleep();

    /*Determine which control streams to use*/
    sleepModeId = EcuMRunData.SdtgNext.Mode;
    if (sleepModeId <= (EcuM_ShutdownModeType)ECUM_MAX_SLEEP_MODE_NUM)
    {
        doHalt = EcuM_SleepModeCfgs[sleepModeId].sleepSuspend;
        if ((boolean)TRUE == doHalt)
        {
            /*activities for halt MCU*/
            EcuM_HaltSequence();
        }
        else
        {
            /*Activities for POLL*/
            EcuM_PollSequence();
        }
        /*Restart activities for wakeup from POLL*/
        EcuM_WakeupRestart();
        ret = E_OK;
    }

    return ret;
}
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
