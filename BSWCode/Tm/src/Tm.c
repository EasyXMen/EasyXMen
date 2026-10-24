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
 **  @file               : Tm.c
 **  @author             : xudong.guan
 **  @date               : 2024/08/20
 **  @vendor             : isoft
 **  @description        : Source file for Tm
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Tm.h"
#include "Det.h"
#include "Tm_Arti.h"

/* ===================================================== macros ===================================================== */

#define TM_START_SEC_VAR_FAST_CLEARED_PTR
#include "Tm_MemMap.h"
static const Tm_ConfigType* Tm_ConfigPtr;
#define TM_STOP_SEC_VAR_FAST_CLEARED_PTR
#include "Tm_MemMap.h"

#define TM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Tm_MemMap.h"
static boolean Tm_InitState;
#define TM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Tm_MemMap.h"

#define TM_START_SEC_VAR_CLEARED_32
#include "Tm_MemMap.h"
static uint32 Tm_PreDefTimerValue[TM_TIMER_INSTANCE_NUM];
#define TM_STOP_SEC_VAR_CLEARED_32
#include "Tm_MemMap.h"

#if (STD_ON == TM_DEV_ERROR_DETECT)
/* PRQA S 3472 ++ */ /* VL_Tm_3472 */
/** TM_DET_REPORT macro api in Tm */
#define TM_DET_REPORT(ApiId, ErrorId) ((void)Det_ReportError(TM_MODULE_ID, TM_INSTANCE_ID, (ApiId), (ErrorId)))
/* PRQA S 3472 -- */
#endif               /* STD_ON == TM_DEV_ERROR_DETECT */
/* PRQA S 3472 ++ */ /* VL_Tm_3472 */
#define TM_DET_REPORTRUNTIME(ApiId, ErrorId) \
    ((void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, (ApiId), (ErrorId)))
/* PRQA S 3472 -- */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
void Tm_Init(const Tm_ConfigType* config)
{
    Tm_ArtiApiStart(2, TM_INIT_ID, config);

    if (config != NULL_PTR)
    {
        if (!Tm_InitState)
        {
            for (uint8 idx = 0u; idx < TM_TIMER_INSTANCE_NUM; idx++)
            {
                Tm_PreDefTimerValue[idx] = TM_INVALID_TIMER_VALUE;
            }
        }
        Tm_InitState = TRUE;
        Tm_ConfigPtr = config;
    }
#if (STD_ON == TM_DEV_ERROR_DETECT)
    else
    {
        TM_DET_REPORT(TM_INIT_ID, TM_E_PARAM_POINTER);
    }
#endif /* STD_ON == TM_DEV_ERROR_DETECT */

    Tm_ArtiApiStop(1, TM_INIT_ID);
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

#if (STD_ON == TM_VERSIONINFO_API)
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
void Tm_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    Tm_ArtiApiStart(2, TM_GETVERSIONINFO_ID, versioninfo);

#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect null_ptr */
    if (versioninfo == NULL_PTR)
    {
        TM_DET_REPORT(TM_GETVERSIONINFO_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* update versioninfo */
        versioninfo->vendorID         = TM_VENDOR_ID;
        versioninfo->moduleID         = TM_MODULE_ID;
        versioninfo->sw_major_version = TM_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = TM_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = TM_SW_PATCH_VERSION;
    }

    Tm_ArtiApiStop(1, TM_GETVERSIONINFO_ID);
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"
#endif /* STD_ON == TM_VERSIONINFO_API */

#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
Std_ReturnType Tm_ResetTimer(uint32 timerId)
{
    Tm_ArtiApiStart(2, TM_RESETTIMER_ID, timerId);

    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TM_DEV_ERROR_DETECT)
    if (timerId >= TM_TIMER_INSTANCE_NUM)
    {
        TM_DET_REPORT(TM_RESETTIMER_ID, TM_E_PARAM_VALUE);
    }
    else
#endif /*STD_ON == TM_DEV_ERROR_DETECT*/
    {
        if (Tm_InitState)
        {
            ret = Gpt_GetPredefTimerValue(Tm_ConfigPtr->preDefTimer[timerId].timerType, &Tm_PreDefTimerValue[timerId]);
            if (ret == E_NOT_OK)
            {
                TM_DET_REPORTRUNTIME(TM_RESETTIMER_ID, TM_E_HARDWARE_TIMER);
            }
        }
#if (STD_ON == TM_DEV_ERROR_DETECT)
        else
        {
            TM_DET_REPORTRUNTIME(TM_RESETTIMER_ID, TM_E_UNINIT);
        }
#endif /*STD_ON == TM_DEV_ERROR_DETECT*/
    }

    Tm_ArtiApiStop(2, TM_RESETTIMER_ID, ret);

    return ret;
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 6010, 6030 ++ */ /* VL_MTR_Tm_STCYC, VL_MTR_Tm_STMIF */
Std_ReturnType Tm_GetTimeSpan(uint32 timerId, uint32* timeSpan)
{
    Tm_ArtiApiStart(3, TM_GETTIMESPAN_ID, timerId, timeSpan);

    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TM_DEV_ERROR_DETECT)
    if (timerId >= TM_TIMER_INSTANCE_NUM)
    {
        TM_DET_REPORT(TM_GETTIMESPAN_ID, TM_E_PARAM_VALUE);
    }
    else if (timeSpan == NULL_PTR)
    {
        TM_DET_REPORT(TM_GETTIMESPAN_ID, TM_E_PARAM_POINTER);
    }
    else if (Tm_PreDefTimerValue[timerId] == TM_INVALID_TIMER_VALUE)
    {
        TM_DET_REPORT(TM_GETTIMESPAN_ID, TM_E_STATE);
    }
    else
#endif
    {
        if (Tm_InitState)
        {
            uint32 currentTimerValue = 0u;
            ret = Gpt_GetPredefTimerValue(Tm_ConfigPtr->preDefTimer[timerId].timerType, &currentTimerValue);
            if (ret == E_OK)
            {
                if (currentTimerValue <= Tm_PreDefTimerValue[timerId])
                {
                    /* When current time tick is equal to reset time tick, it can be considered that after a period.
                     * Although many cycles may have passed, only one cycle can be considered. */
                    switch (Tm_ConfigPtr->preDefTimer[timerId].timerType)
                    {
                    case TM_PREDEF_TIMER_1US_16BIT:
                        *timeSpan = (uint32)(((uint16)TM_1US16BIT_MAX_VALUE - Tm_PreDefTimerValue[timerId])
                                             + currentTimerValue + 1u);
                        break;
                    case TM_PREDEF_TIMER_1US_24BIT:
                        *timeSpan = (uint32)(((uint32)TM_1US24BIT_MAX_VALUE - Tm_PreDefTimerValue[timerId])
                                             + currentTimerValue + 1u);
                        break;
                    case TM_PREDEF_TIMER_1US_32BIT:
                        *timeSpan = (uint32)(((uint32)TM_1US32BIT_MAX_VALUE - Tm_PreDefTimerValue[timerId])
                                             + currentTimerValue + 1u);
                        break;
                    case TM_PREDEF_TIMER_100US_32BIT:
                        *timeSpan = (uint32)(((uint32)TM_100US32BIT_MAX_VALUE - Tm_PreDefTimerValue[timerId])
                                             + currentTimerValue + 1u);
                        break;
                    default:
                        (void)timerId; /* to avoid compiler warning */
                        break;
                    }
                }
                else
                {
                    *timeSpan = currentTimerValue - Tm_PreDefTimerValue[timerId];
                }
            }
            else
            {
                *timeSpan = 0u;
                TM_DET_REPORTRUNTIME(TM_GETTIMESPAN_ID, TM_E_HARDWARE_TIMER);
            }
        }
#if (STD_ON == TM_DEV_ERROR_DETECT)
        else
        {
            TM_DET_REPORTRUNTIME(TM_GETTIMESPAN_ID, TM_E_UNINIT);
        }
#endif /*STD_ON == TM_DEV_ERROR_DETECT*/
    }

    Tm_ArtiApiStop(2, TM_GETTIMESPAN_ID, ret);

    return ret;
}
/* PRQA S 6010, 6030 -- */
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 6030 ++ */ /* VL_MTR_Tm_STMIF */
void Tm_ShiftTimer(uint32 timerId, uint32 timeValue)
{
    Tm_ArtiApiStart(3, TM_SHIFTTIMER_ID, timerId, timeValue);

    if (Tm_InitState)
    {
#if (STD_ON == TM_DEV_ERROR_DETECT)
        if (timerId >= TM_TIMER_INSTANCE_NUM)
        {
            TM_DET_REPORT(TM_SHIFTTIMER_ID, TM_E_PARAM_VALUE);
        }
        else if (
            (Tm_ConfigPtr->preDefTimer[timerId].timerType == TM_PREDEF_TIMER_1US_16BIT)
            && (timeValue > TM_1US16BIT_MAX_VALUE))
        {
            TM_DET_REPORT(TM_SHIFTTIMER_ID, TM_E_PARAM_VALUE);
        }
        else if (
            (Tm_ConfigPtr->preDefTimer[timerId].timerType == TM_PREDEF_TIMER_1US_24BIT)
            && (timeValue > TM_1US24BIT_MAX_VALUE))
        {
            TM_DET_REPORT(TM_SHIFTTIMER_ID, TM_E_PARAM_VALUE);
        }
        else if (Tm_PreDefTimerValue[timerId] == TM_INVALID_TIMER_VALUE)
        {
            TM_DET_REPORT(TM_SHIFTTIMER_ID, TM_E_STATE);
        }
        else
#endif /*STD_ON == TM_DEV_ERROR_DETECT*/
        {
            Tm_PreDefTimerValue[timerId] += timeValue;
        }
    }
#if (STD_ON == TM_DEV_ERROR_DETECT)
    else
    {
        TM_DET_REPORT(TM_SHIFTTIMER_ID, TM_E_UNINIT);
    }
#endif /*STD_ON == TM_DEV_ERROR_DETECT*/

    Tm_ArtiApiStop(1, TM_SHIFTTIMER_ID);
}
/* PRQA S 6030 -- */
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
void Tm_SyncTimer(uint32 timerIdSrc, uint32 timerIdDst)
{
    Tm_ArtiApiStart(3, TM_SYNCTIMER_ID, timerIdSrc, timerIdDst);

    if (Tm_InitState)
    {
#if (STD_ON == TM_DEV_ERROR_DETECT)
        if ((timerIdSrc >= TM_TIMER_INSTANCE_NUM) || (timerIdDst >= TM_TIMER_INSTANCE_NUM))
        {
            TM_DET_REPORT(TM_SYNCTIMER_ID, TM_E_PARAM_VALUE);
        }
        else if (Tm_ConfigPtr->preDefTimer[timerIdSrc].timerType != Tm_ConfigPtr->preDefTimer[timerIdDst].timerType)
        {
            TM_DET_REPORT(TM_SYNCTIMER_ID, TM_E_PARAM_SYNC);
        }
        else if (Tm_PreDefTimerValue[timerIdSrc] == TM_INVALID_TIMER_VALUE)
        {
            TM_DET_REPORT(TM_SYNCTIMER_ID, TM_E_STATE);
        }
        else
#endif /*STD_ON == TM_DEV_ERROR_DETECT*/
        {
            Tm_PreDefTimerValue[timerIdDst] = Tm_PreDefTimerValue[timerIdSrc];
        }
    }
#if (STD_ON == TM_DEV_ERROR_DETECT)
    else
    {
        TM_DET_REPORT(TM_SYNCTIMER_ID, TM_E_UNINIT);
    }
#endif /*STD_ON == TM_DEV_ERROR_DETECT*/

    Tm_ArtiApiStop(1, TM_SYNCTIMER_ID);
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
Std_ReturnType Tm_BusyWait1us(uint8 WaitingTimeMin)
{
    Tm_ArtiApiStart(2, TM_BUSYWAIT1US_ID, WaitingTimeMin);

    Std_ReturnType ret = E_NOT_OK;
    if (Tm_InitState)
    {
        uint32 startCurrentTime = 0u;
        ret                     = Gpt_GetPredefTimerValue(TM_PREDEF_TIMER_1US_32BIT, &startCurrentTime);
        if (ret == E_OK)
        {
            uint32 currentTime = 0u;
            do
            {
                ret = Gpt_GetPredefTimerValue(TM_PREDEF_TIMER_1US_32BIT, &currentTime);
                if (ret == E_NOT_OK)
                {
                    TM_DET_REPORTRUNTIME(TM_BUSYWAIT1US_ID, TM_E_HARDWARE_TIMER);
                    break;
                }

            } while ((currentTime - startCurrentTime) < WaitingTimeMin);
        }
        else
        {
            TM_DET_REPORTRUNTIME(TM_BUSYWAIT1US_ID, TM_E_HARDWARE_TIMER);
        }
    }
#if (STD_ON == TM_DEV_ERROR_DETECT)
    else
    {
        TM_DET_REPORT(TM_BUSYWAIT1US_ID, TM_E_UNINIT);
    }
#endif /*STD_ON == TM_DEV_ERROR_DETECT*/

    Tm_ArtiApiStop(2, TM_BUSYWAIT1US_ID, ret);

    return ret;
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

/* ========================================== internal function definitions ========================================= */
