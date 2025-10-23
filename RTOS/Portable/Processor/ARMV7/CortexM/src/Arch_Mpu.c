/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
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

/*=======[I N C L U D E S]===================================================*/
#include "Arch_Mpu.h"
#include "Std_Types.h"
#include "Os_Internal.h"

/*=======[M A C R O S]=======================================================*/
#define MPU_RASR_ENABLE_MASK 0xFFFFFFFEu
/*=======[E X T E R N A L   D A T A]=========================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/* PRQA S 0791++ */ /* MISRA Rule 5.4 */
#define OS_START_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
Os_ArchMpuRgnType* Os_ArchTaskMpuRegion; /* PRQA S 3432 */ /* MISRA Rule 20.7 */ /* ARCH_MPU_MACRO_DEFINE_003 */
#define OS_STOP_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
Os_ArchMpuRgnType* Os_ArchIsrMpuRegion; /* PRQA S 3432 */ /* MISRA Rule 20.7 */ /* ARCH_MPU_MACRO_DEFINE_003 */
#define OS_STOP_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"

#if (0 < CFG_OSAPPLICATION_MAX)
#define OS_START_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
static Os_ArchMpuRgnType Os_ArchAppMpuRegion[CFG_OSAPPLICATION_MAX][3];
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
#endif

#endif              /*TRUE == CFG_MEMORY_PROTECTION_ENABLE*/
/* PRQA S 0791-- */ /* MISRA Rule 5.4 */
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_MpuPriDefMemMap(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
uint32 Os_ArchMpuCalculateRasr(uint32 RegionLength, uint32 AcessRight, uint32 MemType)
{
    uint32 vRasr;
    uint32 vSizeEncode = 0;
    static CONST(uint8, OS_CONST) Os_MpSizeMpuTable[16] =
        {0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3};
    if(RegionLength >= 32u)
    {
        /* [33:64] ==> 0x5 */
        RegionLength -= 1u;
        if ((RegionLength & 0xFFFF0000U) != 0u)
        {
            vSizeEncode += 16u;
            RegionLength >>= 16u;
        }
        if ((RegionLength & 0xFF00U) != 0u)
        {
            vSizeEncode += 8u;
            RegionLength >>= 8u;
        }
        if ((RegionLength & 0xF0U) != 0u)
        {
            vSizeEncode += 4u;
            RegionLength >>= 4u;
        }
        vSizeEncode += Os_MpSizeMpuTable[RegionLength];
    }
    else if(RegionLength > 0u)
    {
        vSizeEncode = 4u;
    }
    else
    {
        while(1)
        {
        }
        /* DO NOTHING */
    }

    vRasr = AcessRight | MemType | (vSizeEncode << 1) | OS_MPU_RASR_ENABLE;
    return vRasr;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Default memory map configuration>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchInitKnMemMap>
 * REQ ID               <None>
 */
/******************************************************************************/
static void Os_MpuPriDefMemMap(void)
{
    MPU_CTRL_REG |= OS_MPU_CTRL_PRIVDEFENA_MASK; /* PRQA S 0303,3442 */ /* MISRA Rule 11.4,CWE-398 */
    OS_ARCH_DSYNC();
    OS_ARCH_ISYNC();
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
void Os_MpuEnable(void)
{
    MPU_CTRL_REG |= OS_MPU_CTRL_ENABLE; /* PRQA S 0303,3442 */ /* MISRA Rule 11.4,CWE-398 */
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
void Os_MpuDisable(void)
{
    MPU_CTRL_REG &= OS_MPU_CTRL_DISABLE_MASK; /* PRQA S 0303,3442 */ /* MISRA Rule 11.4,CWE-398 */
    OS_ARCH_DSYNC();
    OS_ARCH_ISYNC();
}

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
/*=======[E X C I P T I O N]==================================================*/
/* For memory protection. */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <In MPU Trust or NO Task Configuration, All permissions are
 *                      allowed in privilege mode ,but in user mode,Peripheral is restrict>
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
    OS_ARCH_DSYNC();
    OS_ARCH_ISYNC();

    // /* PRQA S 0303 ++ */ /* MISRA Rule 11.4 */ /* ARCH_MPU_VTYPE_CAST_006 */
    if (TRUE == Os_AppCfg[HostAppId].OsTrustedAppWithProtection)
    {
        /* PERIPHERAL */
        OS_REG32(OS_MPU_RNR_ADDRESS) = MPU_REGION_12;
        OS_REG32(OS_MPU_RASR_ADDRESS) = OS_MPU_RASR_RESET_VALUE;    /* Disable Region */
        OS_REG32(OS_MPU_RBAR_ADDRESS) = (uint32)PERIPHERAL_START;
        OS_REG32(OS_MPU_RASR_ADDRESS) = Os_ArchMpuCalculateRasr((uint32)PERIPHERAL_END - (uint32)PERIPHERAL_START, OS_MPU_AR_RW_R, OS_MPU_TYPE_AHB);
        /* other app data */
        OS_REG32(OS_MPU_RNR_ADDRESS) = MPU_REGION_13;
        OS_REG32(OS_MPU_RASR_ADDRESS) = OS_MPU_RASR_RESET_VALUE;    /* Disable Region */
        OS_REG32(OS_MPU_RBAR_ADDRESS) = (uint32)(Os_Core_App_DAddr[Os_SCB.sysCore].APP_ADDR_START);
        OS_REG32(OS_MPU_RASR_ADDRESS) = Os_ArchMpuCalculateRasr((uint32)(Os_Core_App_DAddr[Os_SCB.sysCore].APP_ADDR_END) - (uint32)(Os_Core_App_DAddr[Os_SCB.sysCore].APP_ADDR_START), OS_MPU_AR_RW_R, OS_MPU_TYPE_SRAMCACHE);
    }
    /* App's PriData */
    OS_REG32(OS_MPU_RNR_ADDRESS) = MPU_REGION_14;
    OS_REG32(OS_MPU_RASR_ADDRESS) = OS_MPU_RASR_RESET_VALUE;    /* Disable Region */
    OS_REG32(OS_MPU_RBAR_ADDRESS) = Os_ArchAppMpuRegion[HostAppId][0].RBAR;
    OS_REG32(OS_MPU_RASR_ADDRESS) = Os_ArchAppMpuRegion[HostAppId][0].RASR;
    /* Task Data */
    OS_REG32(OS_MPU_RNR_ADDRESS) = MPU_REGION_15;
    OS_REG32(OS_MPU_RASR_ADDRESS) = OS_MPU_RASR_RESET_VALUE;    /* Disable Region */
    OS_REG32(OS_MPU_RBAR_ADDRESS) = Os_ArchTaskMpuRegion[TaskId].RBAR;
    OS_REG32(OS_MPU_RASR_ADDRESS) = Os_ArchTaskMpuRegion[TaskId].RASR;
    /* PRQA S 0303 -- */ /* MISRA Rule 11.4 */ /* ARCH_MPU_VTYPE_CAST_006 */
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
 *                      allowed in privilege mode ,but in user mode,Peripheral is restrict>
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
static FUNC(void, OS_CODE) Os_ArchMpuIsrSwitch(ISRType IsrId, ApplicationType HostAppId)
{
    OS_ARCH_DSYNC();
    OS_ARCH_ISYNC();
    /* PRQA S 0303 ++ */ /* MISRA Rule 11.4 */ /* ARCH_MPU_VTYPE_CAST_006 */
    if (TRUE == Os_AppCfg[HostAppId].OsTrustedAppWithProtection)
    {
        /* PERIPHERAL */
        OS_REG32(OS_MPU_RNR_ADDRESS) = MPU_REGION_12;
        OS_REG32(OS_MPU_RASR_ADDRESS) = OS_MPU_RASR_RESET_VALUE;    /* Disable Region */
        OS_REG32(OS_MPU_RBAR_ADDRESS) = (uint32)PERIPHERAL_START;
        OS_REG32(OS_MPU_RASR_ADDRESS) = Os_ArchMpuCalculateRasr((uint32)PERIPHERAL_END - (uint32)PERIPHERAL_START, OS_MPU_AR_RW_R, OS_MPU_TYPE_AHB);
        /* other app data */
        OS_REG32(OS_MPU_RNR_ADDRESS) = MPU_REGION_13;
        OS_REG32(OS_MPU_RASR_ADDRESS) = OS_MPU_RASR_RESET_VALUE;    /* Disable Region */
        OS_REG32(OS_MPU_RBAR_ADDRESS) = (uint32)(Os_Core_App_DAddr[Os_SCB.sysCore].APP_ADDR_START);
        OS_REG32(OS_MPU_RASR_ADDRESS) = Os_ArchMpuCalculateRasr((uint32)(Os_Core_App_DAddr[Os_SCB.sysCore].APP_ADDR_END) - (uint32)(Os_Core_App_DAddr[Os_SCB.sysCore].APP_ADDR_START), OS_MPU_AR_RW_R, OS_MPU_TYPE_SRAMCACHE);
    }

    /* App's PriData */
    OS_REG32(OS_MPU_RNR_ADDRESS) = MPU_REGION_14;
    OS_REG32(OS_MPU_RASR_ADDRESS) = OS_MPU_RASR_RESET_VALUE;    /* Disable Region */
    OS_REG32(OS_MPU_RBAR_ADDRESS) = Os_ArchAppMpuRegion[HostAppId][1].RBAR;
    OS_REG32(OS_MPU_RASR_ADDRESS) = Os_ArchAppMpuRegion[HostAppId][1].RASR;
    /* Isr Data */
    OS_REG32(OS_MPU_RNR_ADDRESS) = MPU_REGION_15;
    OS_REG32(OS_MPU_RASR_ADDRESS) = OS_MPU_RASR_RESET_VALUE;    /* Disable Region */
    OS_REG32(OS_MPU_RBAR_ADDRESS) = Os_ArchIsrMpuRegion[IsrId].RBAR;
    OS_REG32(OS_MPU_RASR_ADDRESS) = Os_ArchIsrMpuRegion[IsrId].RASR;

    /* PRQA S 0303 -- */ /* MISRA Rule 11.4 */ /* ARCH_MPU_VTYPE_CAST_006 */
    OS_ARCH_DSYNC();
    OS_ARCH_ISYNC();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE) Os_MemProtTaskCat1Map(void)
{
    TaskType TaskId = Os_GetObjLocalId(Os_SCB.sysRunningTaskID);
    ApplicationType HostAppId = Os_SCB.sysRunningAppID;

    if ((Os_SCB.sysAppId == Os_SCB.sysRunningAppID) || ((TRUE == Os_AppCfg[HostAppId].OsTrusted) && (TRUE != Os_AppCfg[HostAppId].OsTrustedAppWithProtection)))
    {
        /*SYS_APP, as OS kernel, have all access rights*/ /*Trusted APP and no memory protection*/
        Os_ArchMpDefaultConfigSwitch();
    }
    else
    {
        /*Non trusted apps or trusted apps protected by MPU*/
        Os_ArchMpuTaskSwitch(TaskId, HostAppId);
    }
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
FUNC(void, OS_CODE) Os_MemProtTaskCat2Map(void)
{
    TaskType TaskId = Os_GetObjLocalId(Os_SCB.sysRunningTaskID); /* PRQA S 3469 */ /* MISRA Dir 4.9 */
    ApplicationType HostAppId = Os_SCB.sysRunningAppID;

    if ((Os_SCB.sysAppId == Os_SCB.sysRunningAppID) || ((TRUE == Os_AppCfg[HostAppId].OsTrusted) && (TRUE != Os_AppCfg[HostAppId].OsTrustedAppWithProtection)))
    {
        /*SYS_APP, as OS kernel, have all access rights*/ /*Trusted APP and no memory protection*/
        Os_ArchMpDefaultConfigSwitch();
    }
    else
    {
        /*Non trusted apps or trusted apps protected by MPU*/
        Os_ArchMpuTaskSwitch(TaskId, HostAppId);
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
void Os_MemProtIsrMap(void)
{
    ISRType IsrId = Os_SCB.sysRunningIsrCat2Id;
    ApplicationType HostAppId = Os_SCB.sysRunningAppID;
    /* PRQA S 2109++ */ /* MISRA CWE-192 */
    if ((Os_SCB.sysAppId == Os_SCB.sysRunningAppID) || (TRUE == Os_AppCfg[HostAppId].OsTrusted) && (TRUE != Os_AppCfg[HostAppId].OsTrustedAppWithProtection))
    {
        /*SYS_APP, as OS kernel, have all access rights*/ /*Trusted APP and no memory protection*/
        Os_ArchMpDefaultConfigSwitch();
    }
    else
    {
        /*Non trusted apps or trusted apps protected by MPU*/
        Os_ArchMpuIsrSwitch(IsrId, HostAppId);
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static uint32 GetMaxAlignment(uint32 size) 
{
    uint32 base = size >> 10;
 
    base |= base >> 1;
    base |= base >> 2;
    base |= base >> 4;
    base |= base >> 8;
    base |= base >> 16;
    base = (base >> 1) + 1;
    return base << 10;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_ArchSetDefaultMpu(uint32 startAddr, uint32 endAddr, uint32* curRegion, uint32 access, uint32 regionType)
{
    if (startAddr >= endAddr)
    {
        return;
    }
    else
    {
        uint32 size = endAddr - startAddr + 1;
        uint32 remainSize = size;
        uint32 curAddr = startAddr;
        while (remainSize > 0)
        {
            if (*curRegion > 15)
            {
                return;
            }
            uint32 alignSize = GetMaxAlignment(remainSize);
            while ((curAddr % alignSize) != 0)
            {
                alignSize >>= 1;
            }

            OS_REG32(OS_MPU_RNR_ADDRESS) = *curRegion;
            OS_REG32(OS_MPU_RBAR_ADDRESS) = curAddr;
            OS_REG32(OS_MPU_RASR_ADDRESS) = Os_ArchMpuCalculateRasr(alignSize, access, regionType);

            curAddr += alignSize;
            remainSize -= alignSize;
            (*curRegion)++;
        }
    }
    return;
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
void Os_ArchInitKnMemMap(void)
{
    /* PRQA S 0303,0306 ++*/ /* MISRA Rule 11.4 */ /* ARCH_MPU_MULTI_WARNING_004 */
    uint32 vLength;
    uint32 curRegion;
    Os_MpuDisable();
    /* 0.Background */
    vLength = 0xFFFFFFFFu;
    OS_REG32(OS_MPU_RNR_ADDRESS) = MPU_REGION_0;
    OS_REG32(OS_MPU_RBAR_ADDRESS) = (uint32)0u;
    OS_REG32(OS_MPU_RASR_ADDRESS) = Os_ArchMpuCalculateRasr(vLength, OS_MPU_AR_NONE_NONE, OS_MPU_TYPE_DEFAULT);

    /* 1.ITCM */ 
    curRegion = MPU_REGION_0 + 1;
    Os_ArchSetDefaultMpu((uint32)ITCM_START, (uint32)ITCM_END, &curRegion, OS_MPU_AR_RX_RX, OS_MPU_TYPE_ITCM);

    /* 2.PFLASH() */ 
    Os_ArchSetDefaultMpu((uint32)PFLASH_START, (uint32)PFLASH_END, &curRegion, OS_MPU_AR_RX_RX, OS_MPU_TYPE_PFALSH);

    /* 3.DTCM */ 
    Os_ArchSetDefaultMpu((uint32)DTCM_START, (uint32)DTCM_END, &curRegion, OS_MPU_AR_RW_RW, OS_MPU_TYPE_DTCM);

    /* 4.SRAM */ 
    Os_ArchSetDefaultMpu((uint32)SRAM_START, (uint32)SRAM_END, &curRegion, OS_MPU_AR_RW_RW, OS_MPU_TYPE_SRAMCACHE);

    /* 5.SRAM SHAREABLE(in SRAM) */
    Os_ArchSetDefaultMpu((uint32)SHARED_START, (uint32)SHARED_END, &curRegion, OS_MPU_AR_RW_RW, OS_MPU_TYPE_SRAMSHARE);

    /* 6.PERIPHERAL */
    Os_ArchSetDefaultMpu((uint32)PERIPHERAL_START, (uint32)PERIPHERAL_END, &curRegion, OS_MPU_AR_RW_NONE, OS_MPU_TYPE_AHB);

    /* 7.DFLASH */
    Os_ArchSetDefaultMpu((uint32)DATA_FLASH_START, (uint32)DATA_FLASH_END, &curRegion, OS_MPU_AR_R_R, OS_MPU_TYPE_DFALSH);

    /* 8.COREx DATA */
    vLength = (uint32)(Os_Core_App_DAddr[Os_SCB.sysCore].APP_ADDR_END) - (uint32)(Os_Core_App_DAddr[Os_SCB.sysCore].APP_ADDR_START);
    OS_REG32(OS_MPU_RNR_ADDRESS) = curRegion;
    OS_REG32(OS_MPU_RBAR_ADDRESS) = (uint32)Os_Core_App_DAddr[Os_SCB.sysCore].APP_ADDR_START;
    OS_REG32(OS_MPU_RASR_ADDRESS) = Os_ArchMpuCalculateRasr(vLength, OS_MPU_AR_RW_NONE, OS_MPU_TYPE_SRAMCACHE);

    Os_MpuEnable();
    /* PRQA S 0303,0306 --*/ /* MISRA Rule 11.4 */ /* ARCH_MPU_MULTI_WARNING_004 */
    Os_MpuConfigTableInit();
    Os_MpuPriDefMemMap();
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
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_ArchMemProtEnable(void)
{
    /* Reserved */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_ArchMpuAppRegionInit(void)
{
    ApplicationType vAppId;
    uint32 vLength;
    for(vAppId = 0;vAppId < CFG_OSAPPLICATION_MAX; vAppId++)
    {
        /* App Private Data */
        vLength = (uint32)(Os_AppPriDataAddr[vAppId].APP_ADDR_END) - (uint32)(Os_AppPriDataAddr[vAppId].APP_ADDR_START);
        if(vLength > 0u)
        {
            Os_ArchAppMpuRegion[vAppId][0].RBAR = (uint32)Os_AppPriDataAddr[vAppId].APP_ADDR_START;
            Os_ArchAppMpuRegion[vAppId][0].RASR = Os_ArchMpuCalculateRasr(vLength, OS_MPU_AR_RW_RW, OS_MPU_TYPE_SRAMCACHE);
        }
        else
        {
            Os_ArchAppMpuRegion[vAppId][0].RBAR = OS_MPU_RBAR_RESET_VALUE;
            Os_ArchAppMpuRegion[vAppId][0].RASR = OS_MPU_RASR_RESET_VALUE;
        }

    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_ArchMpuTaskRegionInit(void)
{
    TaskType vTaskId;
    uint32 vLength;
    for(vTaskId = 0;vTaskId < Os_SCB.sysTaskMax; vTaskId++)
    {
        /* Task's All Data */
        vLength = (uint32)(Os_TaskDAddr[vTaskId].Task_ADDR_END) - (uint32)(Os_TaskDAddr[vTaskId].Task_ADDR_START);
        if(vLength > 0u)
        {
            Os_ArchTaskMpuRegion[vTaskId].RBAR = (uint32)Os_TaskDAddr[vTaskId].Task_ADDR_START;
            Os_ArchTaskMpuRegion[vTaskId].RASR = Os_ArchMpuCalculateRasr(vLength, OS_MPU_AR_RW_RW, OS_MPU_TYPE_SRAMCACHE);
        }
        else
        {
            Os_ArchTaskMpuRegion[vTaskId].RBAR = OS_MPU_RBAR_RESET_VALUE;
            Os_ArchTaskMpuRegion[vTaskId].RASR = OS_MPU_RASR_RESET_VALUE;
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_ArchMpuIsrRegionInit(void)
{
    ISRType isrId;
    uint32 vLength;
    for(isrId = 0;isrId < Os_CfgIsr2Max; isrId++)
    {
        /* Task's All Data */
        vLength = (uint32)(Os_IsrDAddr[isrId].ISR_ADDR_END) - (uint32)(Os_IsrDAddr[isrId].ISR_ADDR_START);
        if(vLength > 0u)
        {
            Os_ArchIsrMpuRegion[isrId].RBAR = (uint32)Os_IsrDAddr[isrId].ISR_ADDR_START;
            Os_ArchIsrMpuRegion[isrId].RASR = Os_ArchMpuCalculateRasr(vLength, OS_MPU_AR_RW_RW, OS_MPU_TYPE_SRAMCACHE);
        }
        else
        {
            Os_ArchIsrMpuRegion[isrId].RBAR = OS_MPU_RBAR_RESET_VALUE;
            Os_ArchIsrMpuRegion[isrId].RASR = OS_MPU_RASR_RESET_VALUE;
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialize the MPU configuration and calculate the
 *               configuration table for each OsApplication>
 * Service ID           <Os_ArchMpTrustIsrThreadSwitch>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchInitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_MpuConfigTableInit(void)
{
    if (0 == Os_SCB.sysCore)
    {
        Os_ArchMpuAppRegionInit();
    }
    Os_ArchMpuTaskRegionInit();
    Os_ArchMpuIsrRegionInit();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <In MPU default Configuration, All permissions are
 *                      allowed in privilege mode and User mode>
 * Service ID           <Os_ArchMpDefaultConfigSwitch>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchSetIsrMemMap><Os_ArchSetTaskMemMap>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_ArchMpDefaultConfigSwitch(void)
{
    /* do nothing */
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
uint8 Os_GetFault(void)
{
    uint8 retErr = NO_FAULT_BEHAVIOR;
    uint32 reg = 0u;
    reg = OS_REG32(CM7_SCB_CFSR_ADDRESS); /* PRQA S 0303*/ /*Rule-11.4*/
    if (reg != 0u)
    {
        if (OS_MPU_FAULT_IS_DATA_ACCESS(reg) != 0u)
        {
            retErr = MEMM_FAULT_DATA_ACCESSS_ERR;
        }
        else if (OS_MPU_FAULT_IS_INSTRUCTION_ACCESS(reg) != 0u)
        {

            retErr = MEMM_FAULT_INSTRUCTION_ACCESS_ERR;
        }
        else
        {
            /* Note, I only analyze data and instruction access error violating MPU,
             * so i think that other error is non-mpu-error.
             * The user shall analyze specific reason. */
            retErr = NON_MPU_RELATED_ERR;
        }
    }
    else
    {
        retErr = NO_FAULT_BEHAVIOR;
    }

    return retErr;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* PRQA S 0862*/ /*CWE-398*/
