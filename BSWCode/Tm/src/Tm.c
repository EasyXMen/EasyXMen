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
#include "Gpt.h" /* PRQA S 0380 */ /* VL_Tm_0380 */
#include "Det.h"

/* ===================================================== macros ===================================================== */
/**
 * @name TM_VERSION_C_INFO_GROUP
 * @{
 * @brief Tm module version infomation macros in Tm source file.
 */
#define TM_C_AR_MAJOR_VERSION 4U /**< Tm C ar major version*/
#define TM_C_AR_MINOR_VERSION 9U /**< Tm C ar minor version*/
#define TM_C_AR_PATCH_VERSION 0U /**< Tm C ar patch version*/
#define TM_C_SW_MAJOR_VERSION 2U /**< Tm C sw major version*/
#define TM_C_SW_MINOR_VERSION 2U /**< Tm C sw minor version*/
#define TM_C_SW_PATCH_VERSION 0U /**< Tm C sw patch version*/
/** @} */

#if (STD_ON == TM_DEV_ERROR_DETECT)
/* PRQA S 3472 ++ */ /* VL_Tm_3472 */
/** TM_DET_REPORT macro api in Tm */
#define TM_DET_REPORT(ApiId, ErrorId) ((void)Det_ReportError(TM_MODULE_ID, TM_INSTANCE_ID, (ApiId), (ErrorId)))
/* PRQA S 3472 -- */
#endif /* STD_ON == TM_DEV_ERROR_DETECT */

/* ------------------------------------------------- Version  Check ------------------------------------------------- */
#if (                                                                                                       \
    (TM_C_AR_MAJOR_VERSION != TM_H_AR_MAJOR_VERSION) || (TM_C_AR_MINOR_VERSION != TM_H_AR_MINOR_VERSION)    \
    || (TM_C_AR_PATCH_VERSION != TM_H_AR_PATCH_VERSION) || (TM_C_SW_MAJOR_VERSION != TM_H_SW_MAJOR_VERSION) \
    || (TM_C_SW_MINOR_VERSION != TM_H_SW_MINOR_VERSION) || (TM_C_SW_PATCH_VERSION != TM_H_SW_PATCH_VERSION))

#error "Tm.c : Mismatch in Specification Major Version"
#endif

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#if (STD_ON == TM_VERSIONINFO_API)
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
void Tm_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect null_ptr */
    if (NULL_PTR == versioninfo)
    {
        TM_DET_REPORT(TM_GETVERSIONINFO_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* update versioninfo */
        versioninfo->vendorID         = TM_VENDOR_ID;
        versioninfo->moduleID         = TM_MODULE_ID;
        versioninfo->sw_major_version = TM_H_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = TM_H_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = TM_H_SW_PATCH_VERSION;
    }
}
/* PRQA S 1503 -- */
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"
#endif /* STD_ON == TM_VERSIONINFO_API */

#if (STD_ON == TM_1US16BIT)
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
Std_ReturnType Tm_ResetTimer1us16bit(Tm_PredefTimer1us16bitType* TimerPtr)
{
    Std_ReturnType ret            = E_NOT_OK;
    uint32         CurrentTime    = 0u;
    uint32*        CurrentTimePtr = &CurrentTime;

#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect null_ptr */
    if (NULL_PTR == TimerPtr)
    {
        TM_DET_REPORT(TM_RESETTIMER1US16BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* get current gpt real time and assign to reference time of given clock */
        ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_16BIT, CurrentTimePtr);
        if ((Std_ReturnType)E_OK == ret)
        {
            TimerPtr->ui16RefTime = (uint16)CurrentTime;
        }
        else
        {
            /* gpt clock error */
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_RESETTIMER1US16BIT_ID, TM_E_HARDWARE_TIMER);
        }
    }
    return ret;
}
/* PRQA S 1503 -- */

/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
Std_ReturnType Tm_GetTimeSpan1us16bit(const Tm_PredefTimer1us16bitType* TimerPtr, uint16* TimeSpanPtr)
{
    Std_ReturnType ret                = E_NOT_OK;
    uint32         ui16CurrentTime    = 0u;
    uint32*        ui16CurrentTimePtr = &ui16CurrentTime;

#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect null_ptr */
    if ((NULL_PTR == TimerPtr) || (NULL_PTR == TimeSpanPtr))
    {
        TM_DET_REPORT(TM_GETTIMESPAN1US16BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* get current gpt time, subtract it from reference time */
        ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_16BIT, ui16CurrentTimePtr);
        if ((Std_ReturnType)E_OK == ret)
        {
            *TimeSpanPtr = (uint16)(ui16CurrentTime - TimerPtr->ui16RefTime);
        }
        else
        {
            /* gpt clock error, timespan=0 */
            *TimeSpanPtr = 0U;
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_GETTIMESPAN1US16BIT_ID, TM_E_HARDWARE_TIMER);
        }
    }
    return ret;
}
/* PRQA S 1503 -- */

/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
void Tm_ShiftTimer1us16bit(Tm_PredefTimer1us16bitType* TimerPtr, uint16 TimeValue)
{
#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect null_ptr */
    if (NULL_PTR == TimerPtr)
    {
        TM_DET_REPORT(TM_SHIFTTIMER1US16BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* simply add TimeValue to refernce time */
        TimerPtr->ui16RefTime = (uint16)(TimerPtr->ui16RefTime + TimeValue);
    }
}
/* PRQA S 1503 -- */

/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
void Tm_SyncTimer1us16bit(Tm_PredefTimer1us16bitType* TimerDstPtr, const Tm_PredefTimer1us16bitType* TimerSrcPtr)
{
#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect null_ptr */
    if ((NULL_PTR == TimerDstPtr) || (NULL_PTR == TimerSrcPtr))
    {
        TM_DET_REPORT(TM_SYNCTIMER1US16BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* Sync two clocks */
        TimerDstPtr->ui16RefTime = TimerSrcPtr->ui16RefTime;
    }
}
/* PRQA S 1503 -- */

/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
Std_ReturnType Tm_BusyWait1us16bit(uint8 WaitingTimeMin)
{
    Std_ReturnType ret;
    uint32         RefTime        = 0u;
    uint32*        RefTimePtr     = &RefTime;
    uint32         CurrentTime    = 0u;
    uint32*        CurrentTimePtr = &CurrentTime;

    /* get the start time */
    ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_16BIT, RefTimePtr);

    /* keep getting the current time and subtract it from start time until over timemin */
    do
    {
        if ((Std_ReturnType)E_OK == ret)
        {
            ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_16BIT, CurrentTimePtr);
        }
        else
        {
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_BUSYWAIT1US16BIT_ID, TM_E_HARDWARE_TIMER);
            break;
        }
    } while ((uint16)(CurrentTime - RefTime) <= WaitingTimeMin);

    return ret;
}
/* PRQA S 1503 -- */

#endif /* STD_ON == TM_1US16BIT */

#if (STD_ON == TM_1US24BIT)
/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
Std_ReturnType Tm_ResetTimer1us24bit(Tm_PredefTimer1us24bitType* TimerPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    uint32         CurrentTime;

#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect null_ptr */
    if (NULL_PTR == TimerPtr)
    {
        TM_DET_REPORT(TM_RESETTIMER1US24BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* get current gpt real time and assign to reference time of given clock */
        ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_24BIT, &CurrentTime);
        if ((Std_ReturnType)E_OK == ret)
        {
            TimerPtr->ui24RefTime = CurrentTime & (uint32)TM_1US24BIT_MAX_MASK;
        }
        else
        {
            /* gpt clock error */
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_RESETTIMER1US24BIT_ID, TM_E_HARDWARE_TIMER);
        }
    }
    return ret;
}
/* PRQA S 1503 -- */

/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
Std_ReturnType Tm_GetTimeSpan1us24bit(const Tm_PredefTimer1us24bitType* TimerPtr, uint32* TimeSpanPtr)
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981 */ /* VL_Tm_2981 */
    uint32         ui32CurrentTime;

#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if ((NULL_PTR == TimerPtr) || (NULL_PTR == TimeSpanPtr))
    {
        TM_DET_REPORT(TM_GETTIMESPAN1US24BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* get current gpt time, subtract it from reference time */
        ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_24BIT, &ui32CurrentTime);
        if ((Std_ReturnType)E_OK == ret)
        {
            *TimeSpanPtr = (uint32)(ui32CurrentTime - TimerPtr->ui24RefTime) & (uint32)TM_1US24BIT_MAX_MASK;
        }
        else
        {
            /* gpt error, TimeSpan = 0 */
            *TimeSpanPtr = 0U;
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_GETTIMESPAN1US24BIT_ID, TM_E_HARDWARE_TIMER);
        }
    }
    return ret;
}
/* PRQA S 1503 -- */

/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
void Tm_ShiftTimer1us24bit(Tm_PredefTimer1us24bitType* TimerPtr, uint32 TimeValue)
{
#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if (NULL_PTR == TimerPtr)
    {
        TM_DET_REPORT(TM_SHIFTTIMER1US24BIT_ID, TM_E_PARAM_POINTER);
    }
    else if (TimeValue > TM_1US24BIT_MAX_MASK)
    {
        TM_DET_REPORT(TM_SHIFTTIMER1US24BIT_ID, TM_E_PARAM_VALUE);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* simply add TimeValue to refernce time */
        TimerPtr->ui24RefTime = (uint32)(TimerPtr->ui24RefTime + TimeValue) & (uint32)TM_1US24BIT_MAX_MASK;
    }
}
/* PRQA S 1503 -- */

/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
void Tm_SyncTimer1us24bit(Tm_PredefTimer1us24bitType* TimerDstPtr, const Tm_PredefTimer1us24bitType* TimerSrcPtr)
{
#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if ((NULL_PTR == TimerDstPtr) || (NULL_PTR == TimerSrcPtr))
    {
        TM_DET_REPORT(TM_SYNCTIMER1US24BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* Sync two clocks */
        TimerDstPtr->ui24RefTime = TimerSrcPtr->ui24RefTime;
    }
}
/* PRQA S 1503 -- */

/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
Std_ReturnType Tm_BusyWait1us24bit(uint8 WaitingTimeMin)
{
    Std_ReturnType ret;
    uint32         RefTime;
    uint32         CurrentTime;

    /* get the start time */
    ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_24BIT, &RefTime);

    /* keep getting the current time and subtract it from start time until over timemin */
    do
    {
        if ((Std_ReturnType)E_OK == ret)
        {
            ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_24BIT, &CurrentTime);
        }
        else
        {
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_BUSYWAIT1US24BIT_ID, TM_E_HARDWARE_TIMER);
            break;
        }
    } while (((uint32)(CurrentTime - RefTime) & (uint32)TM_1US24BIT_MAX_MASK) <= WaitingTimeMin);

    return ret;
}
/* PRQA S 1503 -- */

#endif /* STD_ON == TM_1US24BIT */

#if (STD_ON == TM_1US32BIT)

/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
Std_ReturnType Tm_ResetTimer1us32bit(Tm_PredefTimer1us32bitType* TimerPtr)
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981 */ /* VL_Tm_2981 */
    uint32         CurrentTime;

#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if (NULL_PTR == TimerPtr)
    {
        TM_DET_REPORT(TM_RESETTIMER1US32BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* get current gpt real time and assign to reference time of given clock */
        ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_32BIT, &CurrentTime);
        if ((Std_ReturnType)E_OK == ret)
        {
            TimerPtr->ui32RefTime = CurrentTime;
        }
        else
        {
            /* gpt clock error */
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_RESETTIMER1US32BIT_ID, TM_E_HARDWARE_TIMER);
        }
    }
    return ret;
}
/* PRQA S 1503 -- */

/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
Std_ReturnType Tm_GetTimeSpan1us32bit(const Tm_PredefTimer1us32bitType* TimerPtr, uint32* TimeSpanPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    uint32         ui32CurrentTime;

#if (TM_DEV_ERROR_DETECT == STD_ON)
    /* detect NULL_PTR */
    if ((NULL_PTR == TimerPtr) || (NULL_PTR == TimeSpanPtr))
    {
        TM_DET_REPORT(TM_GETTIMESPAN1US32BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* get current gpt time, subtract it from reference time */
        ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_32BIT, &ui32CurrentTime);
        if ((Std_ReturnType)E_OK == ret)
        {
            *TimeSpanPtr = (uint32)(ui32CurrentTime - TimerPtr->ui32RefTime);
        }
        else
        {
            /* gpt error, TimeSpan = 0 */
            *TimeSpanPtr = 0U;
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_GETTIMESPAN1US32BIT_ID, TM_E_HARDWARE_TIMER);
        }
    }
    return ret;
}
/* PRQA S 1503 -- */

/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
void Tm_ShiftTimer1us32bit(Tm_PredefTimer1us32bitType* TimerPtr, uint32 TimeValue)
{
#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if (NULL_PTR == TimerPtr)
    {
        TM_DET_REPORT(TM_SHIFTTIMER1US32BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* simply add TimeValue to refernce time */
        TimerPtr->ui32RefTime = (uint32)(TimerPtr->ui32RefTime + TimeValue);
    }
}
/* PRQA S 1503 -- */

/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
void Tm_SyncTimer1us32bit(Tm_PredefTimer1us32bitType* TimerDstPtr, const Tm_PredefTimer1us32bitType* TimerSrcPtr)
{
/* detect NULL_PTR */
#if (STD_ON == TM_DEV_ERROR_DETECT)
    if ((NULL_PTR == TimerDstPtr) || (NULL_PTR == TimerSrcPtr))
    {
        TM_DET_REPORT(TM_SYNCTIMER1US32BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* Sync two clocks */
        TimerDstPtr->ui32RefTime = TimerSrcPtr->ui32RefTime;
    }
}
/* PRQA S 1503 -- */

/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
Std_ReturnType Tm_BusyWait1us32bit(uint8 WaitingTimeMin)
{
    Std_ReturnType ret;
    uint32         RefTime;
    uint32         CurrentTime;

    /* get the start time */
    ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_32BIT, &RefTime);

    /* keep getting the current time and subtract it from start time until over timemin */
    do
    {
        if ((Std_ReturnType)E_OK == ret)
        {
            ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_32BIT, &CurrentTime);
        }
        else
        {
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_BUSYWAIT1US32BIT_ID, TM_E_HARDWARE_TIMER);
            break;
        }
    } while (((uint32)(CurrentTime - RefTime)) <= WaitingTimeMin);

    return ret;
}
/* PRQA S 1503 -- */
#endif /* STD_ON == TM_1US32BIT */

#if (STD_ON == TM_100US32BIT)
/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
Std_ReturnType Tm_ResetTimer100us32bit(Tm_PredefTimer100us32bitType* TimerPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    uint32         CurrentTime;

#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if (NULL_PTR == TimerPtr)
    {
        TM_DET_REPORT(TM_RESETTIMER100US32BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* get current gpt real time and assign to reference time of given clock */
        ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_100US_32BIT, &CurrentTime);
        if ((Std_ReturnType)E_OK == ret)
        {
            TimerPtr->ui32RefTime = CurrentTime;
        }
        else
        {
            /* gpt clock error */
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_RESETTIMER100US32BIT_ID, TM_E_HARDWARE_TIMER);
        }
    }
    return ret;
}
/* PRQA S 1503 -- */

/* PRQA S 1503,1505 ++ */ /* VL_Tm_1503, VL_Tm_1505 */
Std_ReturnType Tm_GetTimeSpan100us32bit(const Tm_PredefTimer100us32bitType* TimerPtr, uint32* TimeSpanPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    uint32         ui32CurrentTime;

#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if ((NULL_PTR == TimerPtr) || (NULL_PTR == TimeSpanPtr))
    {
        TM_DET_REPORT(TM_GETTIMESPAN100US32BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* get current gpt time, subtract it from reference time */
        ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_100US_32BIT, &ui32CurrentTime);
        if ((Std_ReturnType)E_OK == ret)
        {
            *TimeSpanPtr = (uint32)(ui32CurrentTime - TimerPtr->ui32RefTime);
        }
        else
        {
            /* gpt error, timeSpan = 0 */
            *TimeSpanPtr = 0U;
            (void)
                Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_GETTIMESPAN100US32BIT_ID, TM_E_HARDWARE_TIMER);
        }
    }
    return ret;
}
/* PRQA S 1503,1505 -- */

/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
void Tm_ShiftTimer100us32bit(Tm_PredefTimer100us32bitType* TimerPtr, uint32 TimeValue)
{
#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if (NULL_PTR == TimerPtr)
    {
        TM_DET_REPORT(TM_SHIFTTIMER100US32BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* simply add TimeValue to refernce time */
        TimerPtr->ui32RefTime = (uint32)(TimerPtr->ui32RefTime + TimeValue);
    }
}
/* PRQA S 1503 -- */

/* PRQA S 1503 ++ */ /* VL_Tm_1503 */
void Tm_SyncTimer100us32bit(Tm_PredefTimer100us32bitType* TimerDstPtr, const Tm_PredefTimer100us32bitType* TimerSrcPtr)
{
#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if ((NULL_PTR == TimerDstPtr) || (NULL_PTR == TimerSrcPtr))
    {
        TM_DET_REPORT(TM_SYNCTIMER100US32BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* sync two clocks */
        TimerDstPtr->ui32RefTime = TimerSrcPtr->ui32RefTime;
    }
}
/* PRQA S 1503 -- */
#endif /* STD_ON == TM_100US32BIT */

#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

/* ========================================== internal function definitions ========================================= */
