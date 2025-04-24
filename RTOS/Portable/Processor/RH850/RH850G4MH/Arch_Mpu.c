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
 **  FILENAME    : Arch_Mpu.c                                                  **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : functions related to the Memory Protection Unit (MPU)       **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"
/*=======[E X T E R N A L   D A T A]==========================================*/
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/*The address of the interrupt data.*/
P2CONST(OS_ISR_ADDR, AUTOMATIC, OS_VAR) Os_IsrDAddr;
/*The address of the task data.*/
P2CONST(OS_TASK_ADDR, AUTOMATIC, OS_VAR) Os_TaskDAddr;

#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/*=======[I N T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
static VAR(Os_ApplicationType, OS_VAR) Os_MpuCurrentAppId;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
static FUNC(void, OS_CODE) Os_MpuIsrSwitch(ISRType IsrId, ApplicationType HostAppId);

static FUNC(void, OS_CODE) Os_MpuTaskSwitch(TaskType TaskId, ApplicationType HostAppId);
/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialize the memory protection mapping for the os>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <xxx>
 * Reentrancy           <Non Reentrant>
 * param[in]            <xxx>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <xxx>
 * REQ ID               <xxx>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchInitKnMemMap(void)
{
    Os_MpuCurrentAppId = 0xFFU;

#if defined(OS_ARCH_MPU_ISR2_SUPPORTED)
    OS_ARCH_REG_WRITE(OS_MPM_NUM, OS_ARCH_REG_READ(OS_MPM_NUM) & ~(1U << 1U)); /* MPM.SVP = 0 */
#endif
    OS_ARCH_REG_WRITE(OS_MPRC_NUM, (uint32)0x00);

    /*fixed region 0-1 */
    /* RX*/
    OS_ARCH_REG_WRITE(OS_MPLA0_NUM, (uint32)OS_ARCH_FLASH_ADDR_START);
    OS_ARCH_REG_WRITE(OS_MPUA0_NUM, (uint32)OS_ARCH_FLASH_ADDR_END);
    OS_ARCH_REG_WRITE(OS_MPAT0_NUM, (MPU_RDP_ACCESS_E | MPU_RDP_ACCESS_R));

    /* RW*/
    OS_ARCH_REG_WRITE(OS_MPLA1_NUM, (uint32)OS_ARCH_PERIPH_ADDR_START);
    OS_ARCH_REG_WRITE(OS_MPUA1_NUM, (uint32)OS_ARCH_PERIPH_ADDR_END);
    OS_ARCH_REG_WRITE(OS_MPAT1_NUM, MPU_RDP_ACCESS_RW);

    /*Trusted  region 2*/
    /* RW*/
    OS_ARCH_REG_WRITE(OS_MPLA2_NUM, (uint32)OS_ARCH_RAM_ADDR_START);
    OS_ARCH_REG_WRITE(OS_MPUA2_NUM, (uint32)OS_ARCH_RAM_ADDR_END);
    OS_ARCH_REG_WRITE(OS_MPAT2_NUM, MPU_RDP_ACCESS_RW);

    /*NON-Trusted  3-7*/
    /* RW */
    /* PRQA S 0306++ */ /* MISRA Rule 11.4 */
    if ((uint32)OS_ARCH_RAM_ADDR_START != (uint32)Os_Core_App_DAddr[0].APP_ADDR_START)
    {
        OS_ARCH_REG_WRITE(OS_MPLA3_NUM, (uint32)OS_ARCH_RAM_ADDR_START);
        OS_ARCH_REG_WRITE(OS_MPUA3_NUM, (uint32)Os_Core_App_DAddr[0].APP_ADDR_START - 4U);
        OS_ARCH_REG_WRITE(OS_MPAT3_NUM, MPU_RDP_ACCESS_RW);
    }
    /* NONE */
    OS_ARCH_REG_WRITE(OS_MPLA4_NUM, (uint32)Os_Core_App_DAddr[0].APP_ADDR_START);
    OS_ARCH_REG_WRITE(OS_MPUA4_NUM, (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END - 4U);
    OS_ARCH_REG_WRITE(OS_MPAT4_NUM, MPU_SV_NONE_UM_NONE);

    /* RW */
    OS_ARCH_REG_WRITE(OS_MPLA5_NUM, (uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1U].APP_ADDR_END);
    OS_ARCH_REG_WRITE(OS_MPUA5_NUM, (uint32)_OS_SYSTEM_STACK_START - 4U);
    OS_ARCH_REG_WRITE(OS_MPAT5_NUM, MPU_RDP_ACCESS_RW);

    /* RW */
    OS_ARCH_REG_WRITE(OS_MPLA6_NUM, (uint32)_OS_SYSTEM_STACK_START);
    OS_ARCH_REG_WRITE(OS_MPUA6_NUM, (uint32)_OS_SYSTEM_STACK_END - 4U);
    OS_ARCH_REG_WRITE(OS_MPAT6_NUM, MPU_RDP_ACCESS_RW);

    /* RW */
    OS_ARCH_REG_WRITE(OS_MPLA7_NUM, (uint32)_OS_SYSTEM_STACK_END);
    OS_ARCH_REG_WRITE(OS_MPUA7_NUM, (uint32)OS_ARCH_RAM_ADDR_END);
    OS_ARCH_REG_WRITE(OS_MPAT7_NUM, MPU_RDP_ACCESS_RW);
    /* PRQA S 0306-- */ /* MISRA Rule 11.4 */

    /*0000 0000 0000 0111*/
    OS_ARCH_REG_WRITE(OS_MPRC_NUM, (uint32)0x0007);

#if defined(OS_ARCH_MPU_ISR2_SUPPORTED)
    OS_ARCH_REG_WRITE(OS_MPM_NUM, OS_ARCH_REG_READ(OS_MPM_NUM) | (1U << 1U)); /* MPM.SVP = 1 */
#endif
}

/******************************************************************************/
/*
 * Brief                <Set memory protection map for trusted app>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <xxx>
 * Reentrancy           <Non Reentrant>
 * param[in]            <xxx>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <xxx>
 * REQ ID               <xxx>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_ArchSetTruReg(void)
{
    /*0000 0000 0000 0111*/
    OS_ARCH_REG_WRITE(OS_MPRC_NUM, (uint32)0x0007);
}

/******************************************************************************/
/*
 * Brief                <Cat1 MemMap for Memory Protection Settings Task>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_SwitchTask>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_MemProtTaskCat1Map(void)
{
    TaskType TaskId = Os_GetObjLocalId(Os_SCB.sysRunningTaskID);
    /* PRQA S 3469 */ /* MISRA Rule 4.9 */ /*ARCH_MPU_C_MACRO_008*/
    ApplicationType HostAppId = Os_SCB.sysRunningAppID;

    if (Os_SCB.sysAppId == Os_SCB.sysRunningAppID)
    {
        /*SYS_APP, as OS kernel, have all access rights*/
        Os_ArchSetTruReg();
    }
    else if ((TRUE == Os_AppCfg[HostAppId].OsTrusted) && (TRUE != Os_AppCfg[HostAppId].OsTrustedAppWithProtection))
    {
        /*Trusted APP and no memory protection*/
        Os_ArchSetTruReg();
    }
    else
    {
        /*Non trusted apps or trusted apps protected by MPU*/
        Os_MpuTaskSwitch(TaskId, HostAppId);
    }
}

/******************************************************************************/
/*
 * Brief                <Cat2 MemMap for Memory Protection Settings Task>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_SwitchTask>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_MemProtTaskCat2Map(void)
{
    TaskType TaskId = Os_GetObjLocalId(Os_SCB.sysRunningTaskID);
    /* PRQA S 3469 */ /* MISRA Rule 4.9 */ /*ARCH_MPU_C_MACRO_008*/
    ApplicationType HostAppId = Os_SCB.sysRunningAppID;

    if (Os_SCB.sysAppId == Os_SCB.sysRunningAppID)
    {
        /*SYS_APP, as OS kernel, have all access rights*/
        Os_ArchSetTruReg();
    }
    else if ((TRUE == Os_AppCfg[HostAppId].OsTrusted) && (TRUE != Os_AppCfg[HostAppId].OsTrustedAppWithProtection))
    {
        /*Trusted APP and no memory protection*/
        Os_ArchSetTruReg();
    }
    else
    {
        /*Non trusted apps or trusted apps protected by MPU*/
        Os_MpuTaskSwitch(TaskId, HostAppId);
    }
}

/******************************************************************************/
/*
 * Brief                <MemMap for Memory Protection Settings ISR>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_EnterISR2><Os_ExitISR2>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_MemProtIsrMap(void)
{
    ISRType IsrId = Os_SCB.sysRunningIsrCat2Id;
    ApplicationType HostAppId = Os_SCB.sysRunningAppID;

    if (Os_SCB.sysAppId == Os_SCB.sysRunningAppID)
    {
        /*SYS_APP, as OS kernel, have all access rights*/
        Os_ArchSetTruReg();
    }
    else if ((TRUE == Os_AppCfg[HostAppId].OsTrusted) && (TRUE != Os_AppCfg[HostAppId].OsTrustedAppWithProtection))
    {
        /*Trusted APP and no memory protection*/
        Os_ArchSetTruReg();
    }
    else
    {
        /*Non trusted apps or trusted apps protected by MPU*/
        Os_MpuIsrSwitch(IsrId, HostAppId);
    }
}

/******************************************************************************/
/*
 * Brief                <In MPU Trust Isr Configuration, All permissions are
 *               allowed in privilege mode ,but in user mode,it has some restrict>
 * Service ID           <Os_ArchMpTrustIsrThreadSwitch>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_MpuIsrSwitch(ISRType IsrId, ApplicationType HostAppId)
{
    if (Os_MpuCurrentAppId != HostAppId)
    {
        Os_MpuCurrentAppId = HostAppId;

        OS_ARCH_REG_WRITE(OS_MPLA8_NUM, (uint32)Os_AppPriDataAddr[HostAppId].APP_ADDR_START);
        OS_ARCH_REG_WRITE(OS_MPUA8_NUM, (uint32)Os_AppPriDataAddr[HostAppId].APP_ADDR_END - 4U);
        /* PRQA S 0306-- */ /* MISRA Rule 11.4 */
        OS_ARCH_REG_WRITE(OS_MPAT8_NUM, MPU_RDP_ACCESS_RW);
    }
    OS_ARCH_REG_WRITE(OS_MPLA9_NUM, (uint32)Os_IsrDAddr[IsrId].ISR_ADDR_START);
    OS_ARCH_REG_WRITE(OS_MPUA9_NUM, (uint32)Os_IsrDAddr[IsrId].ISR_ADDR_END - 4U);
    /* PRQA S 0306-- */ /* MISRA Rule 11.4 */
    OS_ARCH_REG_WRITE(OS_MPAT9_NUM, MPU_RDP_ACCESS_RW);

    /*0000 0011 1111 1001*/
    OS_ARCH_REG_WRITE(OS_MPRC_NUM, (uint32)0x03f9);
}

/******************************************************************************/
/*
 * Brief                <In MPU No Trust Task Configuration, All permissions are
 *                      allowed in privilege mode ,but in user mode,it has some restrict>
 * Service ID           <Os_ArchMpNonTrustTaskThreadSwitch>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_MpuTaskSwitch(TaskType TaskId, ApplicationType HostAppId)
{
    if (Os_MpuCurrentAppId != HostAppId)
    {
        Os_MpuCurrentAppId = HostAppId;

        OS_ARCH_REG_WRITE(OS_MPLA8_NUM, (uint32)Os_AppPriDataAddr[HostAppId].APP_ADDR_START);
        OS_ARCH_REG_WRITE(OS_MPUA8_NUM, (uint32)Os_AppPriDataAddr[HostAppId].APP_ADDR_END - 4U);
        /* PRQA S 0306-- */ /* MISRA Rule 11.4 */
        OS_ARCH_REG_WRITE(OS_MPAT8_NUM, MPU_RDP_ACCESS_RW);
    }
    OS_ARCH_REG_WRITE(OS_MPLA9_NUM, (uint32)Os_TaskDAddr[TaskId].Task_ADDR_START);
    OS_ARCH_REG_WRITE(OS_MPUA9_NUM, (uint32)Os_TaskDAddr[TaskId].Task_ADDR_END - 4U);
    /* PRQA S 0306-- */ /* MISRA Rule 11.4 */
    OS_ARCH_REG_WRITE(OS_MPAT9_NUM, MPU_RDP_ACCESS_RW);

    /*0000 0011 1111 1001*/
    OS_ARCH_REG_WRITE(OS_MPRC_NUM, (uint32)0x03f9);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
/*=======[E N D   O F   F I L E]==============================================*/
