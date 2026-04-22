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

/*=======[I N C L U D E S]===================================================*/
#include "Os_MCu_Mpu.h"
#include "Os_Arch_Processor.h"
#include "Os_Appl_CfgData.h"
#include "Os_Task_CfgData.h"
/*=======[V E R S I O N  C H E C K]==========================================*/


/*=======[M A C R O S]=======================================================*/


/*=======[E X T E R N A L   D A T A]=========================================*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/* Address space of kernel view. */
#define OS_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Os_MemMap.h"
CONST(Os_MemProtKnCfgType, OS_CONST) Os_MemProtKnAddrCfg =
{
    {
        /*Code*/
        (uint8 *)OS_FLASH_START,
        (uint8 *)OS_FLASH_END,
        /*Data*/
        /* PRQA S 0306 ++*/ /* MISRA Rule 11.4 */
        (uint8 *)OS_SRAM_START,
        (uint8 *)OS_SRAM_END,
        (uint8 *)PERIPHERAL_START,
        (uint8 *)PERIPHERAL_END,
        /* PRQA S 0306 --*/ /* MISRA Rule 11.4 */
    }
};
#define OS_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
static VAR(Os_ApplicationType, OS_VAR)      Os_ArchMpCurAppId = INVALID_OSAPPLICATION;
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(void, OS_CODE) Os_ArchMpuTaskSwitch(TaskType TaskId, ApplicationType HostAppId);
static FUNC(void, OS_CODE) Os_ArchMpuEnable(void);
static FUNC(void, OS_CODE) Os_ArchMpuDisable(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E X C I P T I O N]==================================================*/
/* For memory protection. */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialize memory protection related registers>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_MemProtEnable(void)
{
    /* Reserved */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <In MPU default Configuration, All permissions are
 *  					allowed in privilege mode and User mode>
 * Service ID           <Os_ArchMpDefaultConfigSwitch>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchMpDefaultConfigSwitch(void)
{
    /* Do Nothing */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialize memory protection related registers>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_MemProtTaskCat1Map and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_Hal_InitKnMemMap(void)
{
    Os_ArchMpuDisable();
    /* 0:Background */
    OS_REG32(OS_MPU_RGDAAC0_ADDR) = (OS_MPU_AP_RWX_NONE | OS_MPU_M23_AP_RW);

    /* 1:FLASH,FlexNVM */
    OS_REG32(OS_MPU_RGDAAC1_ADDR) = (OS_MPU_AP_RX_NONE | OS_MPU_M23_AP_RW);
    OS_REG32(OS_MPU_RGD1_WORD0_ADDR) = OS_FLASH_START;
    OS_REG32(OS_MPU_RGD1_WORD1_ADDR) = OS_FLEXNVM_END;
    OS_REG32(OS_MPU_RGD1_WORD3_ADDR) = OS_MPU_REGION_ENABLE;

    /* 2:FlexRAM,SRAM */
    OS_REG32(OS_MPU_RGDAAC2_ADDR) = (OS_MPU_AP_RWX_NONE | OS_MPU_M23_AP_RW);
    OS_REG32(OS_MPU_RGD2_WORD0_ADDR) = (uint32)OS_FLEXRAM_START;
    OS_REG32(OS_MPU_RGD2_WORD1_ADDR) = (uint32)OS_SRAM_END;
    OS_REG32(OS_MPU_RGD2_WORD3_ADDR) = OS_MPU_REGION_ENABLE;

    /* 3:Peripheral */
    OS_REG32(OS_MPU_RGDAAC3_ADDR) = (OS_MPU_AP_RWX_RWX | OS_MPU_M23_AP_RW);
    OS_REG32(OS_MPU_RGD3_WORD0_ADDR) = (uint32)PERIPHERAL_START;
    OS_REG32(OS_MPU_RGD3_WORD1_ADDR) = (uint32)PERIPHERAL_END;
    OS_REG32(OS_MPU_RGD3_WORD3_ADDR) = OS_MPU_REGION_ENABLE;

    /* 4:APP CODE*/
    OS_REG32(OS_MPU_RGDAAC4_ADDR) = (OS_MPU_AP_RX_RX | OS_MPU_M23_AP_RW);
    OS_REG32(OS_MPU_RGD4_WORD0_ADDR) = (uint32)OS_CODE_START;
    OS_REG32(OS_MPU_RGD4_WORD1_ADDR) = (uint32)OS_CODE_END;
    OS_REG32(OS_MPU_RGD4_WORD3_ADDR) = OS_MPU_REGION_ENABLE;


    Os_ArchMpuEnable();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Memory protection unit enable>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchMpTrustIsrThreadSwitch and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_ArchMpuEnable(void)
{
    /* Enable Global MPU,Clear Slave Port Error */
    OS_REG32(OS_MPU_CESR_ADDR) |= OS_MPU_VALID_BIT;
    OS_ARCH_DSYNC();
    OS_ARCH_ISYNC();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Memory protection unit disable>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchMpTrustIsrThreadSwitch and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_ArchMpuDisable(void)
{
    OS_ARCH_DSYNC();
    OS_ARCH_ISYNC();
    /* Disable Global MPU,Clear Slave Port Error */
    OS_REG32(OS_MPU_CESR_ADDR) &= (~OS_MPU_VALID_BIT);
    OS_ARCH_DSYNC();
    OS_ARCH_ISYNC();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <In MPU Trust or NO Task Configuration, All permissions are
 *  					allowed in privilege mode ,but in user mode,Peripheral is restrict>
 * Service ID           <Os_ArchMpuTaskSwitch>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_ArchMpuTaskSwitch(TaskType TaskId, ApplicationType HostAppId)
{
    Os_ArchMpuDisable();
    /* Task Data */
    OS_REG32(OS_MPU_RGDAAC5_ADDR) = (OS_MPU_AP_RWX_RWX | OS_MPU_M23_AP_RW);
    OS_REG32(OS_MPU_RGD5_WORD0_ADDR) = (uint32)Os_TaskDAddr[TaskId].TaskAddrStart;
    OS_REG32(OS_MPU_RGD5_WORD1_ADDR) = (uint32)Os_TaskDAddr[TaskId].TaskAddrEnd;
    OS_REG32(OS_MPU_RGD5_WORD3_ADDR) = OS_MPU_REGION_ENABLE;
    if (Os_ArchMpCurAppId != HostAppId)
    {
        /* Application All Code */
        if(Os_App_CAddr[HostAppId].AppAddrEnd - Os_App_CAddr[HostAppId].AppAddrStart > 0)
        {
            OS_REG32(OS_MPU_RGDAAC6_ADDR) = (OS_MPU_AP_RX_RX | OS_MPU_M23_AP_RW);
            OS_REG32(OS_MPU_RGD6_WORD0_ADDR) = (uint32)Os_App_CAddr[HostAppId].AppAddrStart;
            OS_REG32(OS_MPU_RGD6_WORD1_ADDR) = ((uint32)Os_App_CAddr[HostAppId].AppAddrEnd);
            OS_REG32(OS_MPU_RGD6_WORD3_ADDR) = OS_MPU_REGION_ENABLE;
        }
        else
        {
        	OS_REG32(OS_MPU_RGDAAC6_ADDR) = (OS_MPU_AP_RX_NONE | OS_MPU_M23_AP_RW);
        }
        /* Application Private Data */
        if(Os_AppPriDataAddr[HostAppId].AppAddrEnd - Os_AppPriDataAddr[HostAppId].AppAddrStart > 0)
        {
            OS_REG32(OS_MPU_RGDAAC7_ADDR) = (OS_MPU_AP_RWX_RWX | OS_MPU_M23_AP_RW);
            OS_REG32(OS_MPU_RGD7_WORD0_ADDR) = (uint32)Os_AppPriDataAddr[HostAppId].AppAddrStart;
            OS_REG32(OS_MPU_RGD7_WORD1_ADDR) = ((uint32)Os_AppPriDataAddr[HostAppId].AppAddrEnd);
            OS_REG32(OS_MPU_RGD7_WORD3_ADDR) = OS_MPU_REGION_ENABLE;
        }
        else
        {
        	OS_REG32(OS_MPU_RGDAAC7_ADDR) = (OS_MPU_AP_RWX_NONE | OS_MPU_M23_AP_RW);
        }
    }
    Os_ArchMpCurAppId = HostAppId;
    Os_ArchMpuEnable();
}

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
void Os_Hal_MemProtTaskMap(Os_TaskType taskId)
{
    ApplicationType HostAppId = Os_TaskCfg[taskId].ObjAppCfg->HostApp;

    if ((TRUE == Os_AppCfg[HostAppId].Trusted) && (TRUE != Os_AppCfg[HostAppId].TrustedAppWithProtection))
    {
        Os_ArchMpDefaultConfigSwitch();
    }
    else
    {
        /*Non trusted apps or trusted apps protected by MPU*/
        Os_ArchMpuTaskSwitch(taskId, HostAppId);
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
void Os_Hal_MemProtIsrMap(Os_IsrType isrId)
{
    ApplicationType HostAppId = Os_IsrCfg[isrId].ObjAppCfg->HostApp;

    if ((TRUE == Os_AppCfg[HostAppId].Trusted) && (TRUE != Os_AppCfg[HostAppId].TrustedAppWithProtection))
    {
        Os_ArchMpDefaultConfigSwitch();
    }
    else
    {
        /*Nothing to do*/
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The function get hard fault.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <None>
 * Param-Name[in/out]   <None>
 * Param-Name[out]      <None>
 * Return               <uint8>
 * PreCondition         <None>
 * CallByAPI            <HardFault_Handler>
 * REQ ID               <None>
 */
/******************************************************************************/
uint8 Os_ArchGetFault(void)
{
    if ((OS_REG32(OS_MPU_CESR_ADDR) & OS_MPU_SPERR_BIT) != 0u)
    {
        return E_NOT_OK;
    }

    return E_OK;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* (TRUE == CFG_MEMORY_PROTECTION_ENABLE) */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */ 
