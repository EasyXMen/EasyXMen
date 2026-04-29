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
/*******************************************************************************
**                                                                            **
**  FILENAME    : Mcu_Timer.c                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Interrupt Request Managment                                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Mcu_Timer.h"
#include "Os_CfgData.h"
/*=======[M A C R O S]========================================================*/
#define CLOCK_STAT_BITS   (31U)
#define STMDIV_MASK       (0xFFFFFFF0u)
#define CLOCK_UP_VALUE    (0x50000003u)
#define SYSTIMER_CMCON_SET_VALUE   (0x0000001Fu)
#define TPTIMER_CMCON_SET_VALUE   (0x001F0000u)
#define CMP0IR_VALUE (0x00000002u)
#define CMP1IR_VALUE (0x00000020u)
#define CMP0OS_CMP0EN_VALUE (0x00000001u)
#define CMP1OS_CMP1EN_VALUE (0x00000050u)
#define CMP0IRR_BIT (0)
#define CMP1IRR_BIT (2U)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static const uint32 OS_STM_CMP_SET[][CFG_CORE_MAX] = {
    {OS_STM0_CMP0, OS_STM1_CMP0, OS_STM2_CMP0, OS_STM3_CMP0, OS_STM4_CMP0, OS_STM5_CMP0},
    {OS_STM0_CMP1, OS_STM1_CMP1, OS_STM2_CMP1, OS_STM3_CMP1, OS_STM4_CMP1, OS_STM5_CMP1}};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static const uint32 OS_STM_ABS_SET[CFG_CORE_MAX] =
    {OS_STM0_TIM0, OS_STM1_TIM0, OS_STM2_TIM0, OS_STM3_TIM0, OS_STM4_TIM0, OS_STM5_TIM0};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 3218 ++ */ /* VL_Os_3218 */
static const uint32 OS_STM_CMCON_SET[CFG_CORE_MAX] =
    {OS_STM0_CMCON, OS_STM1_CMCON, OS_STM2_CMCON, OS_STM3_CMCON, OS_STM4_CMCON, OS_STM5_CMCON};
/* PRQA S 3218 -- */
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static const uint32 OS_STM_ICR_SET[CFG_CORE_MAX] =
    {OS_STM0_ICR, OS_STM1_ICR, OS_STM2_ICR, OS_STM3_ICR, OS_STM4_ICR, OS_STM5_ICR};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static const uint32 OS_STM_VALUE_SET[][CFG_CORE_MAX] = {
    {CFG_REG_OSTIMER_VALUE_CORE0,
     CFG_REG_OSTIMER_VALUE_CORE1,
     CFG_REG_OSTIMER_VALUE_CORE2,
     CFG_REG_OSTIMER_VALUE_CORE3,
     CFG_REG_OSTIMER_VALUE_CORE4,
     CFG_REG_OSTIMER_VALUE_CORE5},
    {CFG_REG_TP_TIMER_VALUE_CORE0,
     CFG_REG_TP_TIMER_VALUE_CORE1,
     CFG_REG_TP_TIMER_VALUE_CORE2,
     CFG_REG_TP_TIMER_VALUE_CORE3,
     CFG_REG_TP_TIMER_VALUE_CORE4,
     CFG_REG_TP_TIMER_VALUE_CORE5}};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 3218 ++ */ /* VL_Os_3218 */
static const uint32 OS_STM_ISCR_SET[CFG_CORE_MAX] =
    {OS_STM0_ISCR, OS_STM1_ISCR, OS_STM2_ISCR, OS_STM3_ISCR, OS_STM4_ISCR, OS_STM5_ISCR};
/* PRQA S 3218 -- */
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
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
 * CallByAPI            <Os_ArchInitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_ClearSysTimer(void)
/* PRQA S 1532 -- */
{
    /* PRQA S 1259, 0303, 1258 ++ */ /* VL_Os_1259, VL_Os_0303, VL_Os_1258 */
    /* PRQA S 3442, 0404, 3345 ++ */ /* VL_Os_3442, VL_Os_0404, VL_Os_3345 */
    if ((uint32)CMP0IR_VALUE == (OS_REG32(OS_STM_ICR_SET[Os_SCB.sysCore]) & ((uint32)CMP0IR_VALUE)))
    {
        OS_REG32(OS_STM_ISCR_SET[Os_SCB.sysCore]) |= (uint32)(1u << CMP0IRR_BIT);

        OS_REG32(OS_STM_CMP_SET[0][Os_SCB.sysCore]) += OS_STM_VALUE_SET[0][Os_SCB.sysCore];

        if (OS_REG32(OS_STM_CMP_SET[0][Os_SCB.sysCore]) < OS_REG32(OS_STM_ABS_SET[Os_SCB.sysCore]))
        {
            OS_REG32(OS_STM_CMP_SET[0][Os_SCB.sysCore]) =
                OS_STM_VALUE_SET[0][Os_SCB.sysCore] + OS_REG32(OS_STM_ABS_SET[Os_SCB.sysCore]);
        }
    }
    /* PRQA S 3442, 0404, 3345 -- */
    /* PRQA S 1259, 0303, 1258 -- */
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
 * CallByAPI            <Os_MultiCoreInitProcessor>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_SetSystemClock(void)
/* PRQA S 1532 -- */
{
    /* PRQA S 0303 ++ */ /* VL_Os_0303 */
    if (0u == Os_SCB.sysCore)
    {
        Os_ArchResetENDINIT(); /* PRQA S 3469 */ /* VL_Os_3469 */
        /* PRQA S 3442 ++ */                     /* VL_Os_3442 */
        while (0U != (OS_SCU_CCUCON1 >> CLOCK_STAT_BITS))
        {
            /* Wait till ccucon registers can be written with new value */
            /* No "timeout" required, because if it hangs, Safety Endinit will
               give a trap */
        }
        /* set .UP to 1,.INSEL to 1,fstm = fsource/2*/
        OS_SCU_CCUCON0 &= STMDIV_MASK;
        OS_SCU_CCUCON0 |= CLOCK_UP_VALUE;

        Os_ArchSetENDINIT(); /* PRQA S 3469 */ /* VL_Os_3469 */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Os_InitSystemTimer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchInitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitSystemTimer(void)
/* PRQA S 1532 -- */
{
    OS_REG32(OS_STM_CMCON_SET[Os_SCB.sysCore]) |= SYSTIMER_CMCON_SET_VALUE;
    OS_REG32(OS_STM_ICR_SET[Os_SCB.sysCore]) |= CMP0OS_CMP0EN_VALUE;
    OS_REG32(OS_STM_CMP_SET[0][Os_SCB.sysCore]) = /* PRQA S 3345 */ /* VL_Os_3345 */
                                                  OS_REG32(OS_STM_ABS_SET[Os_SCB.sysCore])
                                                  + OS_STM_VALUE_SET[0][Os_SCB.sysCore];
    /* PRQA S 3442 -- */
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
 * CallByAPI            <Os_ArchInitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_InitTimingProtTimer(void)
{
    OS_REG32(OS_STM_CMCON_SET[Os_SCB.sysCore]) |= TPTIMER_CMCON_SET_VALUE;
    OS_REG32(OS_STM_ICR_SET[Os_SCB.sysCore]) |= CMP1OS_CMP1EN_VALUE;
    OS_REG32(OS_STM_CMP_SET[1][Os_SCB.sysCore]) =
        OS_REG32(OS_STM_ABS_SET[Os_SCB.sysCore]) + OS_STM_VALUE_SET[1][Os_SCB.sysCore];
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
 * CallByAPI            <Os_MultiCoreInitProcessor>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_ClearTmProtTimer(void)
{
    if ((uint32)CMP1IR_VALUE == (OS_REG32(OS_STM_ICR_SET[Os_SCB.sysCore]) & ((uint32)CMP1IR_VALUE)))
    {
        OS_REG32(OS_STM_ISCR_SET[Os_SCB.sysCore]) |= (uint32)(1u << CMP1IRR_BIT);

        OS_REG32(OS_STM_CMP_SET[1][Os_SCB.sysCore]) =
            OS_STM_VALUE_SET[1][Os_SCB.sysCore] + OS_REG32(OS_STM_ABS_SET[Os_SCB.sysCore]);
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

#if (TRUE == CFG_OS_MONITOR_ENABLE)
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
uint32 Os_ArchGetTimeTicks(void)
{
    /* TIM0 as global base time(us) */
    return OS_REG32(OS_STM0_TIM0);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_OS_MONITOR_ENABLE */

/*=======[E N D   O F   F I L E]==============================================*/
