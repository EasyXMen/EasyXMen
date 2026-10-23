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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Os_Mcu_Core.c                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Core Request Managment                                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R23                         **
**  Version :   AUTOSAR classic Platform R23--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]===================================================*/

#include "Std_Types.h"
#include "Os_Arch_Define.h"
#include "Os_Arch_Irq.h"
#include "Os_Arch_Timer.h"
#include "Os_Mcu_Core.h"
#include "Os_Mcu_Config.h"
#include "Os_Mcu_Timer.h"

/*=======[E X T E R N A L   D A T A]=========================================*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]===================*/

/******************************************************************************/
/*
 * Brief                <Initialize the multicore processor>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_Hal_MultiCoreInit(Os_CoreIdType coreId)
{
    /* The ISR nesting stacks (Os_ArchMainSpNest/Os_ArchIsrxPSRNest) are owned
     * and sized by the ARCH layer; nothing to bind per core here. */
    UNUSED_PARAMETER(coreId);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <check whether CPU information is correct.>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <Yes>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
void Os_Hal_CheckCPUInformation(void)
{
    if (OS_MCU_SCB_CPUID_VALUE != OS_REG_SCB_CPUID)
    {
        while (1)
        {
            /*endless loop*/
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Os_Hal_StartCore>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-coreId[in]     <selects the core which to start>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartCore>
 */
/******************************************************************************/
void Os_Hal_StartCore(Os_CoreIdType coreId)
{
    /* Nothing need to do */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The function returns a unique logical Core ID .>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-phyCoreId[in]  <A valid physical Core ID defined by the AUTOSAR OS.>
 * return               <Os_CoreIdType, The return value is the unique logical
 *                       CoreID map to the input physical core.>
 * PreCondition         <None>
 * CallByAPI            <StartCore>
 * REQ ID               <DD_1_0779, DD_1_0780, DD_1_1543, DD_1_1544>
 */
/******************************************************************************/
Os_CoreIdType Os_Hal_GetCoreID(void)
{
    /* Single core */
    return (Os_CoreIdType)(OS_NUM_0);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the Core Mode>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-core[in]       <selects the core which to get mode>
 * Return               <Os_CoreModeType,the core mode>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_SetCoreMode>
 */
/******************************************************************************/
Os_CoreModeType Os_Hal_GetCoreMode(Os_CoreIdType coreId)
{
    UNUSED_PARAMETER(coreId);

    return OS_CORE_MODE_UNKNOWN;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Set the Core Mode>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-core[in]       <Selects the core which mode is set>
 * Return               <boolean,the result of setting>
 * PreCondition         <None>
 * CallByAPI            <ControlIdle>
 */
/******************************************************************************/
boolean Os_Hal_SetCoreMode(Os_CoreIdType core, Os_CoreModeType coreMode)
{
    UNUSED_PARAMETER(core);
    UNUSED_PARAMETER(coreMode);

    return TRUE;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Os_Hal_InitCPU>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 */
/******************************************************************************/
void Os_Hal_InitCPU(void)
{
    /* Enable return thread in any case  */
    OS_REG_SCB_CCR |= OS_SCB_CCR_NONBASETHDENA_MASK;
#if (TRUE == OS_CFG_ASM_FPU_ENABLE)
    Os_Arch_FpuInit();
#endif
    Os_Arch_InterruptInit(OS_CORE_ID_0);
    Os_Arch_InitIntPrio(OS_CORE_ID_0);

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
    Os_Arch_InitSysTimer(OS_CORE_ID_0);
#endif

    /* OSM and OS_GetGlobalTime must stay available in every scalability class,
     * so the monitor timer is always initialized here; whether the OS takes
     * over the DWT cycle counter or merely adopts an already-running one is
     * selected by OS_MCU_MONITOR_TIMER_INIT_ENABLE inside the function. It
     * shares the core clock with the SysTick above, so ordering between the two
     * does not matter. */
    Os_Mcu_InitMonitorTimer(OS_CORE_ID_0);

    /* The memory-protection region table is initialized by the kernel via
     * Os_Hal_InitKnMemMap() during StartOS, not here. S32K148 is an SC1-only
     * port: the timing-protection timer path is not implemented. */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

