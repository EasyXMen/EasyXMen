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
 **  @file               : Os_Mcu_Timer.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : MCU source file for Timer API implementations
 **
 ***********************************************************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Arch_Processor.h"
#include "Os_Mcu_Timer.h"
#include "Os_Core.h"
#include "Os_Monitor.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/
/* PRQA S 3132 ++ */ /* VL_QAC_MagicNum */
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static const uint32 OS_STM_CMP_SET[2][6] = {
    {OS_STM0_CMP0, OS_STM1_CMP0, OS_STM2_CMP0, OS_STM3_CMP0, OS_STM4_CMP0, OS_STM5_CMP0},
    {OS_STM0_CMP1, OS_STM1_CMP1, OS_STM2_CMP1, OS_STM3_CMP1, OS_STM4_CMP1, OS_STM5_CMP1}};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static const uint32 OS_STM_ABS_SET[6] =
    {OS_STM0_TIM0, OS_STM1_TIM0, OS_STM2_TIM0, OS_STM3_TIM0, OS_STM4_TIM0, OS_STM5_TIM0};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 3218 ++ */ /* VL_Os_3218 */
static const uint32 OS_STM_CMCON_SET[6] =
    {OS_STM0_CMCON, OS_STM1_CMCON, OS_STM2_CMCON, OS_STM3_CMCON, OS_STM4_CMCON, OS_STM5_CMCON};
/* PRQA S 3218 -- */
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static const uint32 OS_STM_ICR_SET[6] = {OS_STM0_ICR, OS_STM1_ICR, OS_STM2_ICR, OS_STM3_ICR, OS_STM4_ICR, OS_STM5_ICR};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static const uint32 OS_STM_VALUE_SET[OS_AUTOSAR_CORES] = {
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
    CFG_REG_OSTIMER_VALUE_CORE0,
#endif /* TRUE == CFG_CORE0_AUTOSAROS_ENABLE */
#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    CFG_REG_OSTIMER_VALUE_CORE1,
#endif /* TRUE == CFG_CORE1_AUTOSAROS_ENABLE */
#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    CFG_REG_OSTIMER_VALUE_CORE2,
#endif /* TRUE == CFG_CORE2_AUTOSAROS_ENABLE */
#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    CFG_REG_OSTIMER_VALUE_CORE3,
#endif /* TRUE == CFG_CORE3_AUTOSAROS_ENABLE */
#if (TRUE == CFG_CORE4_AUTOSAROS_ENABLE)
    CFG_REG_OSTIMER_VALUE_CORE4,
#endif /* TRUE == CFG_CORE4_AUTOSAROS_ENABLE */
#if (TRUE == CFG_CORE5_AUTOSAROS_ENABLE)
    CFG_REG_OSTIMER_VALUE_CORE5
#endif /* TRUE == CFG_CORE5_AUTOSAROS_ENABLE */
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 3218 ++ */ /* VL_Os_3218 */
static const uint32 OS_STM_ISCR_SET[6] =
    {OS_STM0_ISCR, OS_STM1_ISCR, OS_STM2_ISCR, OS_STM3_ISCR, OS_STM4_ISCR, OS_STM5_ISCR};
/* PRQA S 3218 -- */
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 3132 -- */

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
static uint32 Os_ArchTpLastReadValue[CFG_CORE_MAX];
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
#endif

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Os_McuSetSystemClock>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_McuSetSystemClock(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    /*only logic master core need Running this code.*/
    /* PRQA S 0303 */ /* VL_Os_0303 */
    Os_McuResetENDINIT(); /* PRQA S 3469 */ /* VL_Os_3469 */
    /* PRQA S 3442, 3120 ++ */ /* VL_Os_3442, VL_QAC_MagicNum */
    while (0U != (OS_SCU_CCUCON1 >> 31u)) /* PRQA S 0303 */ /* VL_Os_0303 */
    /* PRQA S 3442, 3120 -- */
    {
        /* Wait till ccucon registers can be written with new value */
        /* No "timeout" required, because if it hangs, Safety Endinit will
            give a trap */
    }
    /* set .UP to 1,.INSEL to 1,fstm = fsource/2*/
    /* PRQA S 0303, 3120, 1532, 3442 ++ */ /* VL_Os_0303, VL_QAC_MagicNum, VL_QAC_OneFunRef, VL_Os_3442 */
    OS_SCU_CCUCON0 &= 0xFFFFFFF0u;
    OS_SCU_CCUCON0 |= 0x50000003u;
    /* PRQA S 0303, 3120, 1532, 3442 -- */

    Os_McuSetENDINIT(); /* PRQA S 3469 */ /* VL_Os_3469 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Os_McuInitSystemTimer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_McuInitSystemTimer(Os_CoreIdType coreId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    /* PRQA S 0303, 3442 ++ */ /* VL_Os_0303, VL_Os_3442 */
    OS_REG32(OS_STM_CMCON_SET[coreId]) |= 0x0000001Fu; /* PRQA S 3120 */ /* VL_QAC_MagicNum*/
    OS_REG32(OS_STM_ICR_SET[coreId]) |= 0x00000001u;
    /* PRQA S 3345, 2842 ++ */ /* VL_Os_3345, VL_Os_2842 */
    OS_REG32(OS_STM_CMP_SET[0][coreId]) = OS_REG32(OS_STM_ABS_SET[coreId]) + OS_STM_VALUE_SET[coreId];
    /* PRQA S 3345, 2842 -- */
    /* PRQA S 0303, 3442 -- */

#if (TRUE == CFG_OS_MONITOR_ENABLE) && (TRUE == OS_HAL_SYSTEM_TIMER_IS_USED_CMP)/* PRQA S 3332 */ /* VL_Os_3332 */
    if (0u == coreId)
    {
        /* PRQA S 3442, 3345, 0303 ++ */ /* VL_Os_3442, VL_Os_3345, VL_Os_0303 */
        Os_TimerStamp.baseCmpValue = OS_REG32(OS_STM_CMP_SET[0][coreId]) - OS_STM_VALUE_SET[coreId];
        /* PRQA S 3442, 3345, 0303 -- */
    }
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <updata timer compare value>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_McuClearSysTimer(Os_CoreIdType coreId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    /* PRQA S 0303, 3442 ++ */ /* VL_Os_0303, VL_Os_3442 */
    /* PRQA S 1277, 3120, 1259 ++ */ /* VL_Os_1277, VL_QAC_MagicNum, VL_Os_1259 */
    if ((uint32)0x00000002 == (OS_REG32(OS_STM_ICR_SET[coreId]) & ((uint32)0x00000002)))
    /* PRQA S 1277, 3120, 1259 -- */
    {
	/* PRQA S 1277, 1259 ++ */ /* VL_Os_1277, VL_Os_1259 */
        OS_REG32(OS_STM_ISCR_SET[coreId]) |= (uint32)0x00000001;
	/* PRQA S 1277, 1259 -- */

        OS_REG32(OS_STM_CMP_SET[0][coreId]) += OS_STM_VALUE_SET[coreId]; /* PRQA S 2842 */ /* VL_Os_2842 */

        /* PRQA S 3345 ++ */ /* VL_Os_3345 */
        /* PRQA S 0404 ++ */ /* VL_Os_0404 */
        if (OS_REG32(OS_STM_CMP_SET[0][coreId]) < OS_REG32(OS_STM_ABS_SET[coreId]))
        /* PRQA S 0404 -- */
        {
            OS_REG32(OS_STM_CMP_SET[0][coreId]) = OS_STM_VALUE_SET[coreId] + OS_REG32(OS_STM_ABS_SET[coreId]); /* PRQA S 2842 */ /* VL_Os_2842 */
        }
        /* PRQA S 3345 -- */

        if (OS_CORE_ID_MASTER == coreId)
        {
#if (TRUE == CFG_OS_MONITOR_ENABLE)/* PRQA S 3332 */ /* VL_Os_3332 */
            /* PRQA S 3387, 3442 ++ */ /* VL_Os_3387, VL_Os_3442 */
            ++Os_TimerStamp.softCounter;
            /* PRQA S 3387, 3442 -- */
#if (TRUE == OS_HAL_SYSTEM_TIMER_IS_USED_CMP)
            /* PRQA S 1891, 3345, 4461, 3442 ++ */ /* VL_Os_1891, VL_Os_3345, VL_Os_4461, VL_Os_3442 */
            /* PRQA S 1894, 1840, 0404 ++ */ /* VL_Os_1894, VL_Os_1840, VL_Os_0404 */
            Os_TimerStamp.baseCmpValue = OS_TIMER_GET_BASE_TICKS(OS_REG32(OS_STM_CMP_SET[0][coreId]), Os_TimerStamp.baseCmpValue);
            /* PRQA S 1894, 1840, 0404 -- */ 
            /* PRQA S 1891, 3345, 4461, 3442 -- */
#endif
#endif
        }
    }
    /* PRQA S 0303, 3442 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_SYSTEM_TIMER_ENABLE */

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialize a time-protected clock for each CPU running OS>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_McuInitTimingProtTimer(Os_CoreIdType coreId)/* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    /* PRQA S 0303 ++ */ /* VL_Os_0303 */
    /* PRQA S 3442, 3120 ++ */ /* VL_Os_3442, VL_QAC_MagicNum */
    OS_REG32(OS_STM_CMCON_SET[coreId]) |= 0x001F0000u;
    OS_REG32(OS_STM_ICR_SET[coreId]) |= 0x00000050u;
    /* PRQA S 3442, 3120 -- */
    Os_ArchTpLastReadValue[coreId]      = OS_REG32(OS_STM_ABS_SET[coreId]);
    OS_REG32(OS_STM_CMP_SET[1][coreId]) = Os_ArchTpLastReadValue[coreId] + OS_HAL_TP_MAX_INCREMENT_VALUE;
    /* PRQA S 0303 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <SystemClock set>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_MultiCoreInit>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_McuClearTmProtTimer(Os_CoreIdType coreId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    /* PRQA S 3120, 3442, 0303, 1259 ++ */ /* VL_QAC_MagicNum, VL_Os_3442, VL_Os_0303, VL_Os_1259 */
    /* PRQA S 1841, 4542, 1277 ++ */ /* VL_Os_1841, VL_Os_4542, VL_Os_1277 */
    if ((uint32)0x00000020 == (OS_REG32(OS_STM_ICR_SET[coreId]) & ((uint32)0x00000020)))
    {
        
        OS_REG32(OS_STM_ISCR_SET[coreId]) |= 0x00000004;
    }
    /* PRQA S 1841, 4542, 1277 -- */
    /* PRQA S 3120, 3442, 0303, 1259 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Time protection get time pass, since last read>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
uint64 Os_Hal_TpGetTimePass(Os_CoreIdType coreId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    uint64 TimePass;
    uint32 CounterTime;

    CounterTime = OS_REG32(OS_STM_ABS_SET[coreId]);/* PRQA S 0303 */ /* VL_Os_0303 */

    if (CounterTime > Os_ArchTpLastReadValue[coreId])
    {
        TimePass = (uint64)CounterTime - (uint64)Os_ArchTpLastReadValue[coreId];
    }
    else
    {
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        TimePass = OS_SYSTEM_TIMER_MAX_TICK_VALUE - (uint64)Os_ArchTpLastReadValue[coreId] + (uint64)CounterTime;
        /* PRQA S 1258 -- */ 
    }

    Os_ArchTpLastReadValue[coreId] = CounterTime;
    return TimePass;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Time protection set compare register>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_TpSetCompare(Os_CoreIdType coreId, uint32 Increment)/* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    /* PRQA S 0303, 3345, 3442 ++ */ /* VL_Os_0303, VL_Os_3345, VL_Os_3442*/
    OS_REG32(OS_STM_CMP_SET[1][coreId]) = OS_REG32(OS_STM_ABS_SET[coreId]) + Increment;
    /* PRQA S 0303, 3345, 3442 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* (TRUE == CFG_TIMING_PROTECTION_ENABLE) */ /* PRQA S 2053 */ /*VL_Os_2053*/

#if (TRUE == CFG_OS_MONITOR_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get current time in the OS.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_GetCurrentTime>
 * REQ ID               <None>
 */
/******************************************************************************/
Os_TimerTickType Os_Hal_GetTimeTicks(void)
{
    /* Get the elapsed ticks of the current hardware timer */
    /* PRQA S 4461, 0404, 3446, 1840 ++ */ /* VL_Os_4461, VL_Os_0404, VL_Os_3446, VL_Os_1840 */
    /* PRQA S 3442, 3345, 0303, 1894 ++ */ /* VL_Os_3442, VL_Os_3345, VL_Os_0303, VL_Os_1894 */
    Os_TickType elapsedTicks = OS_TIMER_GET_ELAPSED_TICKS();
    /* PRQA S 3442, 3345, 0303, 1894 -- */
    /* PRQA S 4461, 0404, 3446, 1840 -- */

    /* Get the global time (ms) and calculate the current timestamp(ns). */
    /* PRQA S 3442 ++ */ /* VL_Os_3442 */
    Os_TimerTickType totalNs = Os_TimerStamp.softCounter * OS_NS_PER_MS;
    /* PRQA S 3442 -- */
    totalNs += OS_TIMER_TICKS_TO_NS(elapsedTicks);

    return totalNs;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_OS_MONITOR_ENABLE */
/*=======[E N D   O F   F I L E]==============================================*/
