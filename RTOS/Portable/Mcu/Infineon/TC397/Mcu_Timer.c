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
**  FILENAME    : Mcu_Timer.c                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : functions related to the processor timers                   **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Mcu_Timer.h"
#include "Os_CfgData.h"
/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint32, OS_CONST) OS_STM_CMP_SET[2][6] = {
    {OS_STM0_CMP0, OS_STM1_CMP0, OS_STM2_CMP0, OS_STM3_CMP0, OS_STM4_CMP0, OS_STM5_CMP0},
    {OS_STM0_CMP1, OS_STM1_CMP1, OS_STM2_CMP1, OS_STM3_CMP1, OS_STM4_CMP1, OS_STM5_CMP1}};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint32, OS_CONST)
    OS_STM_ABS_SET[6] = {OS_STM0_TIM0, OS_STM1_TIM0, OS_STM2_TIM0, OS_STM3_TIM0, OS_STM4_TIM0, OS_STM5_TIM0};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint32, OS_CONST)
    OS_STM_CMCON_SET[6] = {OS_STM0_CMCON, OS_STM1_CMCON, OS_STM2_CMCON, OS_STM3_CMCON, OS_STM4_CMCON, OS_STM5_CMCON};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint32, OS_CONST)
    OS_STM_ICR_SET[6] = {OS_STM0_ICR, OS_STM1_ICR, OS_STM2_ICR, OS_STM3_ICR, OS_STM4_ICR, OS_STM5_ICR};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static CONST(uint32, OS_CONST) OS_STM_VALUE_SET[2][6] = {
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
static CONST(uint32, OS_CONST)
    OS_STM_ISCR_SET[6] = {OS_STM0_ISCR, OS_STM1_ISCR, OS_STM2_ISCR, OS_STM3_ISCR, OS_STM4_ISCR, OS_STM5_ISCR};
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
FUNC(void, OS_CODE) Os_ClearSysTimer(void)
{
    if ((uint32)0x00000002 == (OS_REG32(OS_STM_ICR_SET[Os_SCB.sysCore]) & ((uint32)0x00000002)))
    {
        OS_REG32(OS_STM_ISCR_SET[Os_SCB.sysCore]) |= (uint32)0x00000001;

        OS_REG32(OS_STM_CMP_SET[0][Os_SCB.sysCore]) += OS_STM_VALUE_SET[0][Os_SCB.sysCore];

        if (OS_REG32(OS_STM_CMP_SET[0][Os_SCB.sysCore]) < OS_REG32(OS_STM_ABS_SET[Os_SCB.sysCore]))
        {
            OS_REG32(OS_STM_CMP_SET[0][Os_SCB.sysCore]) =
                OS_STM_VALUE_SET[0][Os_SCB.sysCore] + OS_REG32(OS_STM_ABS_SET[Os_SCB.sysCore]);
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Os_ArchInitSystemTimer>
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
FUNC(void, OS_CODE) Os_ArchInitSystemTimer(void)
{
    /*only logic master core need Running this code.*/
    /* PRQA S 0303 ++ */ /* MISRA Rule 11.4 */ /*ARCH_TIMER_VTYPE_CAST_001*/
    if (0u == Os_SCB.sysCore)
    {
        Os_ArchResetENDINIT(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /*ARCH_TIMER_MACRO_001*/

        while (0U != (OS_SCU_CCUCON1 >> 31u))
        {
            /* Wait till ccucon registers can be written with new value */
            /* No "timeout" required, because if it hangs, Safety Endinit will
               give a trap */
        }
        /* set .UP to 1,.INSEL to 1,fstm = fsource/2*/
        OS_SCU_CCUCON0 &= 0xFFFFFFF0u;
        OS_SCU_CCUCON0 |= 0x50000003u;

        Os_ArchSetENDINIT(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /*ARCH_TIMER_MACRO_001*/
    }

    OS_REG32(OS_STM_CMCON_SET[Os_SCB.sysCore]) |= 0x0000001Fu;
    OS_REG32(OS_STM_ICR_SET[Os_SCB.sysCore]) |= 0x00000001u;
    OS_REG32(OS_STM_CMP_SET[0][Os_SCB.sysCore]) =
        OS_REG32(OS_STM_ABS_SET[Os_SCB.sysCore]) + OS_STM_VALUE_SET[0][Os_SCB.sysCore];
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
FUNC(void, OS_CODE) Os_ArchInitTimingProtTimer(void)
{
    OS_REG32(OS_STM_CMCON_SET[Os_SCB.sysCore]) |= 0x001F0000u;
    OS_REG32(OS_STM_ICR_SET[Os_SCB.sysCore]) |= 0x00000050u;
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
FUNC(void, OS_CODE) Os_ClearTmProtTimer()
{
    if ((uint32)0x00000020 == (OS_REG32(OS_STM_ICR_SET[Os_SCB.sysCore]) & ((uint32)0x00000020)))
    {
        OS_REG32(OS_STM_ISCR_SET[Os_SCB.sysCore]) |= (uint32)0x00000004;

        OS_REG32(OS_STM_CMP_SET[1][Os_SCB.sysCore]) =
            OS_STM_VALUE_SET[1][Os_SCB.sysCore] + OS_REG32(OS_STM_ABS_SET[Os_SCB.sysCore]);
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
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
FUNC(uint32, OS_CODE) Os_ArchGetTimeTicks(void)
{
    /* TIM0 as global base time(us) */
    return OS_REG32(OS_STM0_TIM0);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_LOAD_RATIO_CALC_ENABLE */

/*=======[E N D   O F   F I L E]==============================================*/
