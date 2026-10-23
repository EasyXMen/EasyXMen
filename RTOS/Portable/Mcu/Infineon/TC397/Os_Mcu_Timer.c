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
#include "Os_Mcu_Timer.h"
#include "Os_Mcu_Core.h"
#include "Os_Arch_Irq.h"
#include "Os_Cfg.h"
#include "Os_CfgData.h"
#include "Os_Counter_CfgData.h"

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
/* PRQA S 3218, 3207 ++ */ /* VL_Os_3218, VL_Os_3207 */
static const uint32 OS_STM_CAP_SET[6] =
    {OS_STM0_CAP, OS_STM1_CAP, OS_STM2_CAP, OS_STM3_CAP, OS_STM4_CAP, OS_STM5_CAP};
/* PRQA S 3218, 3207 -- */
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
    /* PRQA S 0303 */                                       /* VL_Os_0303 */
    Os_McuResetENDINIT(); /* PRQA S 3469 */                 /* VL_Os_3469 */
    /* PRQA S 3442, 3120 ++ */                              /* VL_Os_3442, VL_QAC_MagicNum */
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
    /* PRQA S 0303, 3442 ++ */                                           /* VL_Os_0303, VL_Os_3442 */
    OS_REG32(OS_STM_CMCON_SET[coreId]) |= 0x0000001Fu; /* PRQA S 3120 */ /* VL_QAC_MagicNum*/
    OS_REG32(OS_STM_ISCR_SET[coreId]) |= 0x00000001u;
    OS_REG32(OS_STM_ICR_SET[coreId]) |= 0x00000001u;
    /* PRQA S 3345, 2842 ++ */ /* VL_Os_3345, VL_Os_2842 */
    OS_REG32(OS_STM_CMP_SET[0][coreId]) = OS_REG32(OS_STM_ABS_SET[coreId]) + OS_STM_VALUE_SET[coreId];
    /* PRQA S 3345, 2842 -- */
    /* PRQA S 0303, 3442 -- */
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
void Os_Hal_ClearSysTimer(Os_CoreIdType coreId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    /* PRQA S 0303, 3442 ++ */       /* VL_Os_0303, VL_Os_3442 */
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
            OS_REG32(OS_STM_CMP_SET[0][coreId]) = OS_STM_VALUE_SET[coreId] + OS_REG32(OS_STM_ABS_SET[coreId]);
            /* PRQA S 2842 */ /* VL_Os_2842 */
        }
        /* PRQA S 3345 -- */
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
void Os_McuInitTimingProtTimer(Os_CoreIdType coreId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    /* PRQA S 0303 ++ */       /* VL_Os_0303 */
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
void Os_Hal_ClearTmProtTimer(Os_CoreIdType coreId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    /* PRQA S 3120, 3442, 0303, 1259 ++ */ /* VL_QAC_MagicNum, VL_Os_3442, VL_Os_0303, VL_Os_1259 */
    /* PRQA S 1841, 4542, 1277 ++ */       /* VL_Os_1841, VL_Os_4542, VL_Os_1277 */
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

    CounterTime = OS_REG32(OS_STM_ABS_SET[coreId]); /* PRQA S 0303 */ /* VL_Os_0303 */

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
void Os_Hal_TpSetCompare(Os_CoreIdType coreId, uint32 Increment) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    /* PRQA S 0303, 3345, 3442 ++ */ /* VL_Os_0303, VL_Os_3345, VL_Os_3442*/
    OS_REG32(OS_STM_CMP_SET[1][coreId]) = OS_REG32(OS_STM_ABS_SET[coreId]) + Increment;
    /* PRQA S 0303, 3345, 3442 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the native 64-bit hardware timer ticks>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <Os_TimerTickType> Native 64-bit hardware ticks (TIM0 low 32 bits | CAP frozen high 32 bits)
 * PreCondition         <None>
 * CallByAPI            <OSM monitor modules (Os_Osm_*)>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Os_TimerTickType Os_Hal_GetHwTimerTicks(Os_CoreIdType coreId)
/* PRQA S 1532 -- */
{
    Os_TimerTickType ticks;
    uint32           lo;
    uint32           hi;

    /* Read TIM0 then CAP: reading TIM0 freezes the current high 32 bits into CAP, composing a consistent 64-bit value.
     * The critical section is required because any nested read of TIM0 (e.g. from an ISR calling this function)
     * would re-latch CAP between the two reads below, breaking the lo/hi consistency. */
    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();
    /* PRQA S 0303 ++ */ /* VL_Os_0303 */
    lo = OS_REG32(OS_STM_ABS_SET[coreId]); /* TIM0: low 32 bits */
    hi = OS_REG32(OS_STM_CAP_SET[coreId]); /* CAP: frozen high 32 bits */
    /* PRQA S 0303 -- */
    OS_HAL_EXIT_CRITICAL();

    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    ticks = (((Os_TimerTickType)hi) << 32U) | ((Os_TimerTickType)lo);
    /* PRQA S 3120 -- */
    return ticks;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
