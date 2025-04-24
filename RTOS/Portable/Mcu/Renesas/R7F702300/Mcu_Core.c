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
 *
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : Mcu_Core.c                                                  **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : MCU dependence                                              **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"
#include "Os_CoreCfg.h"
/*=======[M A C R O S]========================================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
/*Save the task context stack pointer address.*/
/* PRQA S 3218++ */ /* MISRA Rule 8.9 */
static VAR(Os_TaskCBExtType, OS_VAR) Os_TaskCBExtCore0[CFG_TASK_MAX_CORE0];
/*Save the ISR2 context stack pointer address of core0.*/
static VAR(uint32, OS_VAR) Os_IsrNestPcxStackCore0[CFG_ISR_MAX_CORE0];
/*Save the ISR1context stack pointer address of core0.*/
static VAR(uint32, OS_VAR) Os_ISR1SPCore0[OS_ISR1_MAX_NESTED_DEPTH];
/* PRQA S 3218-- */ /* MISRA Rule 8.9 */
#endif              /* TRUE == CFG_CORE0_AUTOSAROS_ENABLE */
#define OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CORE1_UNSPECIFIED
#include "Os_MemMap.h"
#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
/*Save the task context stack pointer address.*/
/* PRQA S 3218++ */ /* MISRA Rule 8.9 */
static VAR(Os_TaskCBExtType, OS_VAR) Os_TaskCBExtCore1[CFG_TASK_MAX_CORE1];
/*Save the ISR2 context stack pointer address of core1.*/
static VAR(uint32, OS_VAR) Os_IsrNestPcxStackCore1[CFG_ISR_MAX_CORE1];
/*Save the ISR1context stack pointer address of core1.*/
static VAR(uint32, OS_VAR) Os_ISR1SPCore1[OS_ISR1_MAX_NESTED_DEPTH];
/* PRQA S 3218-- */ /* MISRA Rule 8.9 */
#endif              /* TRUE == CFG_CORE1_AUTOSAROS_ENABLE */
#define OS_STOP_SEC_VAR_CLEARED_CORE1_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CORE2_UNSPECIFIED
#include "Os_MemMap.h"
#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
/*Save the task context stack pointer address.*/
/* PRQA S 3218++ */ /* MISRA Rule 8.9 */
static VAR(Os_TaskCBExtType, OS_VAR) Os_TaskCBExtCore2[CFG_TASK_MAX_CORE2];
/*Save the ISR2 context stack pointer address of core2.*/
static VAR(uint32, OS_VAR) Os_IsrNestPcxStackCore2[CFG_ISR_MAX_CORE2];
/*Save the ISR1context stack pointer address of core2.*/
static VAR(uint32, OS_VAR) Os_ISR1SPCore2[OS_ISR1_MAX_NESTED_DEPTH];
/* PRQA S 3218-- */ /* MISRA Rule 8.9 */
#endif              /* TRUE == CFG_CORE2_AUTOSAROS_ENABLE */
#define OS_STOP_SEC_VAR_CLEARED_CORE2_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CORE3_UNSPECIFIED
#include "Os_MemMap.h"
#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
/*Save the task context stack pointer address.*/
/* PRQA S 3218++ */ /* MISRA Rule 8.9 */
static VAR(Os_TaskCBExtType, OS_VAR) Os_TaskCBExtCore3[CFG_TASK_MAX_CORE3];
/*Save the ISR2 context stack pointer address of core3.*/
static VAR(uint32, OS_VAR) Os_IsrNestPcxStackCore3[CFG_ISR_MAX_CORE3];
/*Save the ISR1context stack pointer address of core3.*/
static VAR(uint32, OS_VAR) Os_ISR1SPCore3[OS_ISR1_MAX_NESTED_DEPTH];
/* PRQA S 3218-- */ /* MISRA Rule 8.9 */
#endif              /* TRUE == CFG_CORE3_AUTOSAROS_ENABLE */
#define OS_STOP_SEC_VAR_CLEARED_CORE3_UNSPECIFIED
#include "Os_MemMap.h"

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/* Address space of kernel view. */
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
CONST(Os_MemProtKnCfgType, OS_CONST)
Os_MemProtKnAddrCfg = {{
    /*Data*/
    /* PRQA S 0306 ++*/ /* MISRA Rule 11.4 */
    (uint8*)OS_ARCH_RAM_ADDR_START,
    (uint8*)OS_ARCH_RAM_ADDR_END,

    (uint8*)OS_ARCH_PERIPH_ADDR_START,
    (uint8*)OS_ARCH_PERIPH_ADDR_END,

    (uint8*)OS_ARCH_FLASH_ADDR_START,
    (uint8*)OS_ARCH_FLASH_ADDR_END,
    /* PRQA S 0306 --*/ /* MISRA Rule 11.4 */
}};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#endif
/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialize the multicore processor.>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ipl>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <void>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_MultiCoreInitProcessor(void)
{
    VAR(Os_IsrType, OS_VAR) i;
    VAR(uint16, OS_VAR) vCoreId = Os_SCB.sysCore;
    P2CONST(Os_IsrCfgType, AUTOMATIC, OS_VAR) pIsrCfg;
    P2CONST(Os_IntInstallType, AUTOMATIC, OS_VAR) pIntInstall;

    /*Platform specific interrupt controller initialization*/
    Os_InterruptInit();

    for (i = 0; i < Os_CfgIsrMax_Inf[vCoreId]; i++)
    {
        pIsrCfg = &(Os_IsrCfg_Inf[vCoreId][i]);
        pIntInstall = &(Os_IntInstall_Inf[vCoreId][i]);
        Os_InterruptInstall(pIsrCfg, pIntInstall);
    }

    InitInterruptFlag = TRUE;

    switch (Os_SCB.sysCore)
    {
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
    case 0:
        Os_TaskCBExt = Os_TaskCBExtCore0;
        Os_IsrNestPcxStack = Os_IsrNestPcxStackCore0;
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_IsrDAddr = Os_Isr_DAddr_Core0;
        Os_TaskDAddr = Os_Task_DAddr_Core0;
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
        Os_ISR1SP = Os_ISR1SPCore0;
        break;
#endif /* TRUE == CFG_CORE0_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    case 1:
        Os_TaskCBExt = Os_TaskCBExtCore1;
        Os_IsrNestPcxStack = Os_IsrNestPcxStackCore1;
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_IsrDAddr = Os_Isr_DAddr_Core1;
        Os_TaskDAddr = Os_Task_DAddr_Core1;
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
        Os_ISR1SP = Os_ISR1SPCore1;
        break;
#endif /* TRUE == CFG_CORE1_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    case 2:
        Os_TaskCBExt = Os_TaskCBExtCore2;
        Os_IsrNestPcxStack = Os_IsrNestPcxStackCore2;
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_IsrDAddr = Os_Isr_DAddr_Core2;
        Os_TaskDAddr = Os_Task_DAddr_Core2;
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
        Os_ISR1SP = Os_ISR1SPCore2;
        break;
#endif /* TRUE == CFG_CORE2_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    case 3:
        Os_TaskCBExt = Os_TaskCBExtCore3;
        Os_IsrNestPcxStack = Os_IsrNestPcxStackCore3;
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_IsrDAddr = Os_Isr_DAddr_Core3;
        Os_TaskDAddr = Os_Task_DAddr_Core3;
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
        Os_ISR1SP = Os_ISR1SPCore3;
        break;
#endif /* TRUE == CFG_CORE3_AUTOSAROS_ENABLE */

    /*add comments to pass QAC.*/
    default:
        /* Nothing to do */
        break;
    }
}

/******************************************************************************/
/*
 * Brief                <Os_ArchStartCore>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-coreId[in]     <selects the core which to start>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartCore>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchStartCore(Os_CoreIdType coreId)
{
#if (OS_AUTOSAR_CORES > 1)
#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    if (1u == coreId)
    {
        OS_BOOTCTRL_ADDR |= (1 << 1);
    }
#endif
#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    else if (2u == coreId)
    {
        OS_BOOTCTRL_ADDR |= (1 << 2);
    }
#endif
#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    else if (3u == coreId)
    {
        OS_BOOTCTRL_ADDR |= (1 << 3);
    }
#endif
    else
    {
        /* Nothing to do. */
    }
#endif
}

/******************************************************************************/
/*
 * Brief                <Get the Core Mode>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-core[in]       <selects the core which to get mode>
 * Return               <Os_CoreModeType,the core mode>
 * PreCondition         <None>
 * CallByAPI            <Os_SetCoreMode>
 */
/******************************************************************************/
FUNC(Os_CoreModeType, OS_CODE) Os_GetCoreMode(Os_CoreIdType core)
{
    (void)core;
    Os_CoreModeType coreMode = OS_CORE_MODE_UNKNOWN;
    return coreMode;
}
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
FUNC(boolean, OS_CODE) Os_SetCoreMode(Os_CoreIdType core, Os_CoreModeType coreMode)
{
    boolean Os_Ret = TRUE;

    (void)core;
    switch (coreMode)
    {
    case OS_CORE_MODE_HALT:
        ASM_HALT();
        ASM_NOP();
        break;

    case OS_CORE_MODE_IDLE:
        ASM_HALT();
        ASM_NOP();
        break;

    case OS_CORE_MODE_RUN:
        /*When an exception, such as an interrupt or a TRAP for scheduling, occurs,
        the CPU exits the Halt mode automatically, without requiring additional setup. */
        break;

    default:
        Os_Ret = FALSE;
        break;
    }

    return Os_Ret;
}
/*****************************************************************************/
/*
 * Brief                <Os_ArchGetCoreID>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <CoreId>
 * PreCondition         <None>
 * CallByAPI            <GetCoreID>
 */
/*****************************************************************************/
FUNC(Os_CoreIdType, OS_CODE) Os_ArchGetCoreID(void)
{
    uint32 coreid = OS_ARCH_REG_READ(OS_REG_PEID) & 0x001F;

    return (Os_CoreIdType)coreid;
}
/******************************************************************************/
/*
 * Brief                <The function returns a unique logical Core ID.>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-phyCoreId[in]  <A valid physical Core ID defined by the AUTOSAR OS.>
 * return               <CoreIdType, The return value is the unique logical
 *                       CoreID map to the input physical core.>
 * PreCondition         <None>
 * CallByAPI            <StartCore>
 * REQ ID               <DD_1_0779, DD_1_0780, DD_1_1543, DD_1_1544>
 */
/******************************************************************************/
FUNC(CoreIdType, OS_CODE) Os_GetCoreLogID(CoreIdType phyCoreId)
{
    VAR(CoreIdType, OS_VAR) logCoreId;

    switch (phyCoreId)
    {
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
    case OS_CORE_ID_0:
        logCoreId = 0u;
        break;
#endif /* CFG_CORE0_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    case OS_CORE_ID_1:
        logCoreId = 1u;
        break;
#endif /* CFG_CORE1_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    case OS_CORE_ID_2:
        logCoreId = 2u;
        break;
#endif /* CFG_CORE2_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    case OS_CORE_ID_3:
        logCoreId = 3u;
        break;
#endif /* CFG_CORE3_AUTOSAROS_ENABLE */

    default:
        logCoreId = OS_CORE_INVALID;
        break;
    }

    return logCoreId;
}

/******************************************************************************/
/*
 * Brief                <The function returns a unique physical Core ID.>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-logCoreId[in]  <A valid logical Core ID from GetCoreID,Os_GetCoreLogID.>
 * return               <CoreIdType, The return value is the unique physical
 *                       CoreID map to the input logical core.>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <DD_1_0781, DD_1_0782>
 */
/******************************************************************************/
FUNC(CoreIdType, OS_CODE) Os_GetCorePhyID(CoreIdType logCoreId)
{
    VAR(CoreIdType, OS_VAR) phyCoreId;

    switch (logCoreId)
    {
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
    case 0:
        phyCoreId = OS_CORE_ID_0;
        break;
#endif /* TRUE == CFG_CORE0_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    case 1:
        phyCoreId = OS_CORE_ID_1;
        break;
#endif /* TRUE == CFG_CORE1_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    case 2:
        phyCoreId = OS_CORE_ID_2;
        break;
#endif /* TRUE == CFG_CORE2_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    case 3:
        phyCoreId = OS_CORE_ID_3;
        break;
#endif /* TRUE == CFG_CORE3_AUTOSAROS_ENABLE */

    default:
        phyCoreId = OS_CORE_INVALID;
        break;
    }

    return phyCoreId;
}

/******************************************************************************/
/*
 * Brief                <The free task entry for core 0.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <DD_1_0838, DD_1_1217, DD_1_0839>
 */
/******************************************************************************/
#if (CFG_TASK_MAX_CORE0 > 0U)
FUNC(void, OS_CODE) Os_TaskEntry_IdleCore0(void)
{
    while (1)
    {
        IdleHook_Core0();
    }
}
#endif /* CFG_TASK_MAX_CORE0 > 0U */
/******************************************************************************/
/*
 * Brief                <The free task entry for core 1.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <DD_1_0838, DD_1_1217, DD_1_0839>
 */
/******************************************************************************/
#if (CFG_TASK_MAX_CORE1 > 0U)
FUNC(void, OS_CODE) Os_TaskEntry_IdleCore1(void)
{
    while (1)
    {
        IdleHook_Core1();
    }
}
#endif /* CFG_TASK_MAX_CORE1 > 0U */
/******************************************************************************/
/*
 * Brief                <The free task entry for core 2.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <DD_1_0838, DD_1_1217, DD_1_0839>
 */
/******************************************************************************/
#if (CFG_TASK_MAX_CORE2 > 0U)
FUNC(void, OS_CODE) Os_TaskEntry_IdleCore2(void)
{
    while (1)
    {
        IdleHook_Core2();
    }
}
#endif /* CFG_TASK_MAX_CORE2 > 0U */
/******************************************************************************/
/*
 * Brief                <The free task entry for core 3.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <DD_1_0838, DD_1_1217, DD_1_0839>
 */
/******************************************************************************/
#if (CFG_TASK_MAX_CORE3 > 0U)
FUNC(void, OS_CODE) Os_TaskEntry_IdleCore3(void)
{
    while (1)
    {
        IdleHook_Core3();
    }
}
#endif /* CFG_TASK_MAX_CORE3 > 0U */

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
