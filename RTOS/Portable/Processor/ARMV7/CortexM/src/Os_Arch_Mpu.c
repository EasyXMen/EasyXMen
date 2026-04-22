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
#include "Os_Arch_Mpu.h"
#include "Std_Types.h"
#include "Os_Arch_Processor.h"
#include "Os_Appl_CfgData.h"
#include "Os_Task_CfgData.h"
#include "Os_Interrupt_CfgData.h"
/*=======[M A C R O S]=======================================================*/
#define MPU_RASR_ENABLE_MASK 0xFFFFFFFEu

/* Get the memory protection type of application.*/
/* PRQA S 3472,3458++ */ /* MISRA Rule 4.9,PRE10 */
#define OS_MPU_GET_APP_MP_TYPE(trust, protection)  \
    ((TRUE == (trust))?((TRUE == (protection))?OS_MP_SET_TRUST_WITH_PROT:OS_MP_SET_TRUST_WITHOUT_PROT):OS_MP_SET_NON_TRUST)
/* PRQA S 3472,3458-- */ /* MISRA Rule 4.9,PRE10 */

#define OS_MAKE_MPU_CONFIG(region, acc, start, end, config)\
        config.u8RegionNum = (region);\
        config.u32StartAddr = (uint32)(start); /* PRQA S 0306 */ /* MISRA Rule 11.4 */\
        config.u32EndAddr = (uint32)(end); /* PRQA S 0306 */     /* MISRA Rule 11.4 */\
        config.size = Os_CaculateMemSize((uint8*)(start), (uint8*)(end));\
        config.u32access = (acc);

#define OS_MPU_SET_REGION(region, ref_region_type, idx)  \
        MPU_RNR_REG = (region);\
        MPU_RBAR_REG = ref_region_type->RegionRbarPtr[(idx)][(region)];\
        MPU_RASR_REG = ref_region_type->RegionRasrPtr[(idx)][(region)];

/*=======[E X T E R N A L   D A T A]=========================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/* PRQA S 0791++ */ /* MISRA Rule 5.4 */
#define OS_START_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
static Os_MpSetType Os_PreAppTrustState;
#define OS_STOP_SEC_VAR_CLONE_8
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
static Os_MpSetType Os_CurAppTrustState;
#define OS_STOP_SEC_VAR_CLONE_8
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
static uint8 Os_PublicAreaCfg;
#define OS_STOP_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
#endif              /*TRUE == CFG_MEMORY_PROTECTION_ENABLE*/

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
Os_MpuRegionType Os_TaskMpuRegion;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
Os_MpuRegionType Os_Isr2MpuRegion;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

/* PRQA S 0791-- */ /* MISRA Rule 5.4 */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static const uint32 MpuRegionSize[] = /* PRQA S 3218 */ /* MISRA Rule 8.9 */ /*ARCH_MPU_DATA_DEFINE_001*/
    {
        /* obyte, 32byte, 64byte, 128byte, 256byte, 512byte, 1KB */
        0u,
        32u,
        64u,
        128u,
        256u,
        512u,
        1024u,
        /* 2KB, 4KB, 8KB, 16KB, 32KB */
        2048u,
        4096u,
        8192u,
        16384u,
        32768u,
        /* 64KB, 128KB, 256KB, 512KB, 1MB */
        65536u,
        131072u,
        262144u,
        524288u,
        1048576u,
        /* 2MB, 4MB, 8MB, 16MB*/
        2097152u,
        4194304u,
        8388608u,
        16777216u,
        /* 32MB, 64MB, 128MB, 256MB */
        33554432u,
        67108864u,
        134217728u,
        268435456u,
        /* 512MB, 1GB, 2GB, 4GB */
        536870912u,
        1073741824u,
        2147483648u,
        4294967295u};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static const uint32 MpuRegionBitMap[] = /* PRQA S 3218 */ /* MISRA Rule 8.9 */ /*ARCH_MPU_DATA_DEFINE_001*/
    {
        /*obyte, 32byte, 64byte, 128byte, 256byte */
        0u,
        MPU_RGN_SIZE_32B,
        MPU_RGN_SIZE_64B,
        MPU_RGN_SIZE_128B,
        MPU_RGN_SIZE_256B,
        /* 512byte, 1KB, 2KB, 4KB */
        MPU_RGN_SIZE_512B,
        MPU_RGN_SIZE_1KB,
        MPU_RGN_SIZE_2KB,
        MPU_RGN_SIZE_4KB,
        /* 8KB, 16KB, 32KB, 64KB */
        MPU_RGN_SIZE_8KB,
        MPU_RGN_SIZE_16KB,
        MPU_RGN_SIZE_32KB,
        MPU_RGN_SIZE_64KB,
        /* 128KB, 256KB, 512KB, 1MB */
        MPU_RGN_SIZE_128KB,
        MPU_RGN_SIZE_256KB,
        MPU_RGN_SIZE_512KB,
        MPU_RGN_SIZE_1MKB,
        /* 2MB, 4MB, 8MB, 16MB */
        MPU_RGN_SIZE_2MKB,
        MPU_RGN_SIZE_4MKB,
        MPU_RGN_SIZE_8MKB,
        MPU_RGN_SIZE_16MKB,
        /* 32MB, 64MB, 128MB, 256MB */
        MPU_RGN_SIZE_32MKB,
        MPU_RGN_SIZE_64MKB,
        MPU_RGN_SIZE_128MKB,
        MPU_RGN_SIZE_256MKB,
        /* 512MB, 1GB, 2GB, 4GB */
        MPU_RGN_SIZE_512MKB,
        MPU_RGN_SIZE_1GB,
        MPU_RGN_SIZE_2GB,
        MPU_RGN_SIZE_4GB};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#endif
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_ArchMpuPriDefMemMap(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void MPU_SetRegionConfig(const mpu_config* pUserConfigPtr, uint16 threadId, Os_CallLevelType threadLevel);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Memory protection settings Region register.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-region[in]     <Region Number>
 * param-base[in]       <Region Base Address>
 * param-size[in]       <Region size>
 * param-access[in]     <Region access>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_SetMpuRegion(uint8 region, uint32 base, uint32 size, uint32 access)
{
    /* PRQA S 0303 ++*/ /* MISRA Rule 11.4 */
    MPU_RNR_REG = region;
    MPU_RBAR_REG = base;
    MPU_RASR_REG = (size | access);
    /* PRQA S 0303 --*/ /* MISRA Rule 11.4 */
    OS_ARCH_DSYNC();
    OS_ARCH_ISYNC();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Memory protection disable region>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-region[in]     <Region Number>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_ArchDisableMpuRegion(uint8 region)
{
    uint32 tpRasr = 0U;
    /* PRQA S 0303 ++*/ /* MISRA Rule 11.4 */
    MPU_RNR_REG = region;
    tpRasr = MPU_RASR_REG;
    tpRasr = tpRasr & (uint32)(MPU_RASR_ENABLE_MASK);
    MPU_RASR_REG = tpRasr;
    /* PRQA S 0303 --*/ /* MISRA Rule 11.4 */
    OS_ARCH_DSYNC();
    OS_ARCH_ISYNC();
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
 * CallByAPI            <Os_Hal_InitKnMemMap>
 * REQ ID               <None>
 */
/******************************************************************************/
static void Os_ArchMpuPriDefMemMap(void)
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
static void Os_MpuEnable(void)
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
void Os_ArchMpuDisable(void)
{
    MPU_CTRL_REG &= OS_MPU_CTRL_DISABLE_MASK; /* PRQA S 0303,3442 */ /* MISRA Rule 11.4,CWE-398 */
    OS_ARCH_DSYNC();
    OS_ARCH_ISYNC();
}

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Caculate memory protection region size>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-startAddr[in]  <start address>
 * param-endAddr[in]    <end address>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchMpTrustIsrThreadSwitch and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
static uint32 Os_CaculateMemSize(const uint8* startAddr, const uint8* endAddr)
{
    uint32 size = 0u;
    uint32 memSize = 0u;
    uint32 idex = 0u;

    size = (uint32)(endAddr - startAddr); /* PRQA S 0488 */ /* MISRA Rule 18.4 */ /*ARCH_MPU_PTR_OPERATIONS_003*/
    while (TRUE) /*PRQA S 2740*/                                                  /* MISRA CWE-569,CWE-571 */
    {
        if (MpuRegionSize[idex] >= size)
        {
            memSize = MpuRegionBitMap[idex];
            break;
        }
        idex++;
    }
    return memSize;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif
/*=======[E X C I P T I O N]==================================================*/
/* For memory protection. */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                < Switch MPU settings for task/isr >
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
static void Os_MpuRegionSet(Os_CallLevelType Level, uint16 Id)
{
    Os_MpuRegionType* RefRegeionType = (Level == OS_LEVEL_TASK) ? &Os_TaskMpuRegion : &Os_Isr2MpuRegion;
    Os_ArchMpuDisable();
    /* PRQA S 0303 ++*/                              /* MISRA Rule 11.4*/
    if (Os_PublicAreaCfg == FALSE) /* PRQA S 2109 */ /* MISRA CWE-192 */
    {
        OS_MPU_SET_REGION(OS_MPU_REGION_NUM_ALL_PFLASH_SPACE, RefRegeionType, Id);
        OS_MPU_SET_REGION(OS_MPU_REGION_NUM_ALL_SRAM_SPACE, RefRegeionType, Id);
        OS_MPU_SET_REGION(OS_MPU_REGION_NUM_SHARED_SPACE, RefRegeionType, Id);
        Os_PublicAreaCfg = TRUE;
    }
    if (Os_CurAppTrustState != Os_PreAppTrustState)
    {

#if (REGION_16 == CFG_REGION_NUM)
        OS_MPU_SET_REGION(OS_MPU_REGION_NUM_ALL_APP_CODE, RefRegeionType, Id);
#endif

        OS_MPU_SET_REGION(OS_MPU_REGION_NUM_ALL_APP_DATA, RefRegeionType, Id);
        OS_MPU_SET_REGION(OS_MPU_REGION_NUM_ALL_PERI_SPACE, RefRegeionType, Id);
    }

#if (REGION_16 == CFG_REGION_NUM)
    OS_MPU_SET_REGION(OS_MPU_REGION_NUM_CUR_APP_CODE, RefRegeionType, Id);
#endif

    OS_MPU_SET_REGION(OS_MPU_REGION_NUM_CUR_APP_PRI_DATA, RefRegeionType, Id);
    OS_MPU_SET_REGION(OS_MPU_REGION_NUM_CUR_TASK_DATA, RefRegeionType, Id);
    /* PRQA S 0303 --*/ /* MISRA Rule 11.4*/
    if (OS_MP_SET_NON_TRUST == Os_CurAppTrustState)
    {
        Os_PreAppTrustState = OS_MP_SET_NON_TRUST;
    }
    else
    {
        Os_PreAppTrustState = OS_MP_SET_TRUST_WITH_PROT;
    }
    Os_MpuEnable();
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
void Os_ArchMemProtMap(uint16 Id, Os_CallLevelType Level)
{
    ApplicationType HostAppId = Os_Kernel_GetSysRunningAppID();
    /* PRQA S 2109 ++*/ /* MISRA CWE-192 */
    if ((Os_AppSysId[Os_Hal_GetCoreID()] == HostAppId)
        || ((TRUE == Os_AppCfg[HostAppId].Trusted) && (TRUE != Os_AppCfg[HostAppId].TrustedAppWithProtection)))
    {
        /*SYS_APP, as OS kernel, have all access rights*/
        Os_CurAppTrustState = OS_MP_SET_TRUST_WITHOUT_PROT;
        Os_ArchMpDefaultConfigSwitch();
        Os_PreAppTrustState = OS_MP_SET_TRUST_WITHOUT_PROT;
    }
    else if (FALSE == Os_AppCfg[HostAppId].Trusted)
    {
        /*Non trusted apps or trusted apps protected by MPU*/
        Os_CurAppTrustState = OS_MP_SET_NON_TRUST;
        Os_MpuRegionSet(Level, Id);
    }
    else
    {
        /*Non trusted apps or trusted apps protected by MPU*/
        Os_CurAppTrustState = OS_MP_SET_TRUST_WITH_PROT;
        Os_MpuRegionSet(Level, Id);
    }
    /* PRQA S 2109-- */ /* MISRA CWE-192 */
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
void Os_Hal_InitKnMemMap(void)
{
    Os_ArchMpuConfigTableInit();
    Os_ArchMpuPriDefMemMap();
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
 * Brief                <Configure the region attributes of the MPU and store
 *                       them in a configuration table>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchMpTrustIsrThreadSwitch and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
static void MPU_SetRegionConfig(const mpu_config* pUserConfigPtr, uint16 threadId, Os_CallLevelType threadLevel)
{
    uint32 regionAttributes = 0U;
    uint32 regionRbar = 0U;

    regionRbar = pUserConfigPtr->u32StartAddr;

    /* Set the region size */
    regionAttributes |= pUserConfigPtr->size;

    regionAttributes |= pUserConfigPtr->u32access;

    if (pUserConfigPtr->u32EndAddr > pUserConfigPtr->u32StartAddr)
    {
        /* Enable Region */
        regionAttributes |= MPU_REGION_ENABLE;
    }
    else
    {
        regionAttributes |= MPU_REGION_DISABLE;
    }

    /* PRQA S 0303 ++*/ /* MISRA Rule 11.4 */
    if (threadLevel == OS_LEVEL_TASK)
    {
        Os_TaskMpuRegion.RegionRbarPtr[threadId][pUserConfigPtr->u8RegionNum] = regionRbar;
        Os_TaskMpuRegion.RegionRasrPtr[threadId][pUserConfigPtr->u8RegionNum] = regionAttributes;
    }
    else
    {
        Os_Isr2MpuRegion.RegionRbarPtr[threadId][pUserConfigPtr->u8RegionNum] = regionRbar;
        Os_Isr2MpuRegion.RegionRasrPtr[threadId][pUserConfigPtr->u8RegionNum] = regionAttributes;
    }
    /* PRQA S 0303 --*/ /* MISRA Rule 11.4 */ /*ARCH_MPU_TYPE_CAST_002*/
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Configure the memory protection unit for the task of
 *                       application>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchSetTaskMemMap>
 * REQ ID               <None>
 */
/******************************************************************************/
static void Os_MpuRegionCalc(Os_CallLevelType Level, uint16 Id, ApplicationType HostAppId, Os_MpSetType Set)
{
    mpu_config Mpu_Config;
    if (OS_MP_SET_TRUST_WITHOUT_PROT != Set)
    {
        /*By default, all code segments allow both privileged and user modes to read and execute.*/
        OS_MAKE_MPU_CONFIG(OS_MPU_REGION_NUM_ALL_PFLASH_SPACE, MPU_PRIV_RO_USER_RO_EXEC | MPU_NORMAL_OIWTNOWA_NONSHARED, 
            CODE_ADDR_START, CODE_ADDR_END, Mpu_Config);
        MPU_SetRegionConfig(&Mpu_Config, Id, Level);

        /*By default, all RAM space is read-write accessible in privileged mode and read-only in user mode.*/
        OS_MAKE_MPU_CONFIG(OS_MPU_REGION_NUM_ALL_SRAM_SPACE, MPU_PRIV_RW_USER_RO_EXEC | MPU_NORMAL_OIWTNOWA_NONSHARED, 
            SRAM_START, SRAM_END, Mpu_Config);
        MPU_SetRegionConfig(&Mpu_Config, Id, Level);

        /*The shared space has read-write access in privileged mode and is read-only in user mode.*/
        OS_MAKE_MPU_CONFIG(OS_MPU_REGION_NUM_SHARED_SPACE, MPU_PRIV_RW_USER_RO_EXEC | MPU_NORMAL_OINC_SHARED, 
            SHARED_START, SHARED_END, Mpu_Config);
        MPU_SetRegionConfig(&Mpu_Config, Id, Level);

        /*The App private space belonging to the thread has read-write access in both privileged and user modes.*/
        OS_MAKE_MPU_CONFIG(OS_MPU_REGION_NUM_CUR_APP_PRI_DATA, MPU_PRIV_RW_USER_RW_NOEXEC | MPU_NORMAL_OIWTNOWA_NONSHARED, 
            Os_AppPriDataAddr[HostAppId].AppAddrStart, Os_AppPriDataAddr[HostAppId].AppAddrEnd, Mpu_Config);
        MPU_SetRegionConfig(&Mpu_Config, Id, Level);

        /*The thread own private space has read-write permissions in both privileged and user modes.*/
        if (Level == OS_LEVEL_TASK) 
        {
            OS_MAKE_MPU_CONFIG(OS_MPU_REGION_NUM_CUR_TASK_DATA, MPU_PRIV_RW_USER_RW_NOEXEC | MPU_NORMAL_OIWTNOWA_NONSHARED, 
                Os_TaskDAddr[Id].TaskAddrStart, Os_TaskDAddr[Id].TaskAddrEnd, Mpu_Config);
        } 
        else 
        {
            OS_MAKE_MPU_CONFIG(OS_MPU_REGION_NUM_CUR_TASK_DATA, MPU_PRIV_RW_USER_RW_NOEXEC | MPU_NORMAL_OIWTNOWA_NONSHARED, 
                Os_IsrDAddr[Id].IsrAddrStart, Os_IsrDAddr[Id].IsrAddrEnd, Mpu_Config);
        }

        MPU_SetRegionConfig(&Mpu_Config, Id, Level);

        if (OS_MP_SET_NON_TRUST == Set)
        {
#if (REGION_16 == CFG_REGION_NUM)
            /*By default, threads belonging to an untrusted partition have read and execute permissions for all app code
             * spaces in privileged mode, and no permissions in user mode.*/
            OS_MAKE_MPU_CONFIG(OS_MPU_REGION_NUM_ALL_APP_CODE, MPU_PRIV_RO_USER_NA_EXEC | MPU_NORMAL_OIWTNOWA_NONSHARED, 
                Os_Core_App_CAddr[0u].AppAddrStart, Os_Core_App_CAddr[OS_AUTOSAR_CORES - 1].AppAddrEnd, Mpu_Config);
            MPU_SetRegionConfig(&Mpu_Config, Id, Level);

            /*Threads belonging to an untrusted partition have read and execute permissions for their own code space in
             * both privileged and user modes.*/
            OS_MAKE_MPU_CONFIG(OS_MPU_REGION_NUM_CUR_APP_CODE, MPU_PRIV_RO_USER_RO_EXEC | MPU_NORMAL_OIWTNOWA_NONSHARED, 
                Os_App_CAddr[HostAppId].AppAddrStart, Os_App_CAddr[HostAppId].AppAddrEnd, Mpu_Config);
            MPU_SetRegionConfig(&Mpu_Config, Id, Level);
#endif

            /*By default, threads belonging to an untrusted partition have read and write permissions for all app data
             * segments in privileged mode, and no permissions in user mode.*/
            OS_MAKE_MPU_CONFIG(OS_MPU_REGION_NUM_ALL_APP_DATA, 
                (Level == OS_LEVEL_TASK) ? (MPU_PRIV_RW_USER_NA_NOEXEC | MPU_NORMAL_OIWTNOWA_NONSHARED) : (MPU_PRIV_NA_USER_NA_NOEXEC | MPU_NORMAL_OIWTNOWA_NONSHARED), 
                Os_Core_App_DAddr[0u].AppAddrStart, Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1].AppAddrEnd, Mpu_Config);
            MPU_SetRegionConfig(&Mpu_Config, Id, Level);

            /*Threads belonging to an untrusted partition have read and write permissions for peripheral regions in
             * privileged mode, and no permissions in user mode.*/
            OS_MAKE_MPU_CONFIG(OS_MPU_REGION_NUM_ALL_PERI_SPACE,
                (Level == OS_LEVEL_TASK) ? (MPU_PRIV_RW_USER_NA_EXEC | MPU_DEVICE_NONSHAREABLE) : (MPU_PRIV_NA_USER_NA_EXEC | MPU_DEVICE_NONSHAREABLE), 
                PERIPHERAL_START, PERIPHERAL_END, Mpu_Config);
            MPU_SetRegionConfig(&Mpu_Config, Id, Level);
        }
        else
        {
#if (REGION_16 == CFG_REGION_NUM)
            /*The code space access permissions for a trusted app with a protected partition inherit the permissions of
             * `OS_MPU_REGION_NUM_ALL_PFLASH_SPACE`.*/
            OS_MAKE_MPU_CONFIG(OS_MPU_REGION_NUM_ALL_APP_CODE, MPU_PRIV_RO_USER_RO_EXEC | MPU_NORMAL_OIWTNOWA_NONSHARED, 
                Os_Core_App_CAddr[0u].AppAddrStart, Os_Core_App_CAddr[OS_AUTOSAR_CORES - 1].AppAddrEnd, Mpu_Config);
            MPU_SetRegionConfig(&Mpu_Config, Id, Level);
#endif

            /*a trusted app with a protected partition have read and write permissions for all app data segments in
             * privileged mode, and read permissions in user mode.*/
            OS_MAKE_MPU_CONFIG(OS_MPU_REGION_NUM_ALL_APP_DATA,
                (Level == OS_LEVEL_TASK) ?  (MPU_PRIV_RW_USER_RO_NOEXEC | MPU_NORMAL_OIWTNOWA_NONSHARED) : (MPU_PRIV_RO_USER_RO_NOEXEC | MPU_NORMAL_OIWTNOWA_NONSHARED), 
                Os_Core_App_DAddr[0u].AppAddrStart, Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1].AppAddrEnd, Mpu_Config);
            MPU_SetRegionConfig(&Mpu_Config, Id, Level);

            /*Threads belonging to a trusted and protected partition have read and write permissions for peripheral
             * regions in privileged mode, and read-only permissions in user mode.*/
            OS_MAKE_MPU_CONFIG(OS_MPU_REGION_NUM_ALL_PERI_SPACE,
                (Level == OS_LEVEL_TASK) ?  (MPU_PRIV_RW_USER_RO_EXEC | MPU_DEVICE_NONSHAREABLE) : (MPU_PRIV_RO_USER_RO_EXEC | MPU_DEVICE_NONSHAREABLE), 
                PERIPHERAL_START, PERIPHERAL_END, Mpu_Config);
            MPU_SetRegionConfig(&Mpu_Config, Id, Level);
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
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_ArchMpuConfigTableInit(void)
{
    uint8 region = 0u;
    Os_MpSetType set;
    Os_ApplicationType curAppId;
    uint16 id;
    Os_PublicAreaCfg = FALSE;
    Os_PreAppTrustState = OS_MP_SET_INIT;
    Os_CurAppTrustState = OS_MP_SET_INIT;
    Os_ArchMpuDisable();

    for (region = 0U; region < CFG_REGION_NUM; region++)
    { /* PRQA S 0303 ++*/ /* MISRA Rule 11.4 */
        MPU_RNR_REG = (uint32)region;
        MPU_RASR_REG = 0U;
        MPU_RBAR_REG = 0U;
        /* PRQA S 0303 --*/ /* MISRA Rule 11.4 */
    }

    for (id = 0u; id < CFG_TASK_MAX; id++)
    {
        curAppId = Os_TaskCfg[id].ObjAppCfg->HostApp;
        /* PRQA S 3138,2109 ++*/ /* MISRA CWE-398,CWE-192 */
        set = OS_MPU_GET_APP_MP_TYPE(Os_AppCfg[curAppId].Trusted, Os_AppCfg[curAppId].TrustedAppWithProtection);
        /* PRQA S 3138,2109 --*/ /* MISRA CWE-398,CWE-192 */
        Os_MpuRegionCalc(OS_LEVEL_TASK, id, curAppId, set);
    }

    for (id = 0u; id < CFG_ISR2_MAX; id++)
    {
        curAppId = Os_IsrCfg[id].ObjAppCfg->HostApp;
        /* PRQA S 3138,2109 ++*/ /* MISRA CWE-398,CWE-192 */
        set = OS_MPU_GET_APP_MP_TYPE(Os_AppCfg[curAppId].Trusted, Os_AppCfg[curAppId].TrustedAppWithProtection);
        /* PRQA S 3138,2109 --*/ /* MISRA CWE-398,CWE-192 */
        Os_MpuRegionCalc(OS_LEVEL_ISR2, id, curAppId, set);
    }
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
    Os_ArchMpuDisable();
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
