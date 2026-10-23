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
********************************************************************************
**                                                                            **
**  FILENAME    : Os_Arch_Mpu.c                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : functions related to the Memory Protection Unit (MPU)       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r23                         **
**  Version :   AUTOSAR classic Platform R23--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]===================================================*/

#include "Os_Arch_Mpu.h"
#include "Std_Types.h"
#include "Os_Arch_Processor.h"
#include "Os_Appl_CfgData.h"
#include "Os_Task_CfgData.h"
#include "Os_Interrupt_CfgData.h"
#include "Os_Mprot_CfgData.h"
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#if (FALSE == OS_MCU_EXTERNAL_MPU_USED)
/*=======[M A C R O S]=======================================================*/

/* Get the memory protection type of application.*/
/* PRQA S 3472,3458++ */ /* VL_Os_3472, VL_Os_3458 */
#define OS_MPU_TASK_GET_APP_MP_TYPE(trust, protection)                                                            \
    ((TRUE == (trust)) ? ((TRUE == (protection)) ? OS_MP_SET_TASK_TRUST_WITH_PROT : OS_MP_SET_TRUST_WITHOUT_PROT) \
                       : OS_MP_SET_TASK_NON_TRUST)

#define OS_MPU_ISR_GET_APP_MP_TYPE(trust, protection)                                                            \
    ((TRUE == (trust)) ? ((TRUE == (protection)) ? OS_MP_SET_ISR_TRUST_WITH_PROT : OS_MP_SET_TRUST_WITHOUT_PROT) \
                       : OS_MP_SET_ISR_NON_TRUST)
/* PRQA S 3472,3458-- */ 

#define OS_ARCH_MAKE_MPU_CONFIG(region, acc, start, end, config)                        \
    config.u8RegionNum  = (region);                                                     \
    config.u32StartAddr = (uint32)(start); /* PRQA S 0306 */ /* VL_Os_0306 */      \
    config.u32EndAddr   = (uint32)(end); /* PRQA S 0306 */   /* VL_Os_0306 */      \
    config.size         = Os_Arch_CaculateMemSize((uint8*)(start), (uint8*)(end));      \
    config.u32access    = (acc);

#define OS_ARCH_MPU_SET_REGION(region, ref_region_type, idx)                            \
    OS_ARCH_MPU_RNR_REG  = (region);                                                    \
    OS_ARCH_MPU_RBAR_REG = ref_region_type->RegionRbarPtr[(idx)][(region)];             \
    OS_ARCH_MPU_RASR_REG = ref_region_type->RegionRasrPtr[(idx)][(region)];

/*=======[E X T E R N A L   D A T A]=========================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/

#define OS_START_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
static Os_Arch_MpSetType Os_PreAppTrustState;
#define OS_STOP_SEC_VAR_CLONE_8
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
static Os_Arch_MpSetType Os_CurAppTrustState;
#define OS_STOP_SEC_VAR_CLONE_8
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
static uint8 Os_PublicAreaCfg;
#define OS_STOP_SEC_VAR_CLONE_8
#include "Os_MemMap.h"

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

#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static const uint32 MpuRegionSize[] = /* PRQA S 3218 */ /* VL_Os_3218 */ /*ARCH_MPU_DATA_DEFINE_001*/
    {
        /* 0byte, 32byte, 64byte, 128byte, 256byte, 512byte, 1KB */
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
static const uint32 MpuRegionBitMap[] = /* PRQA S 3218 */ /* VL_Os_3218 */ /*ARCH_MPU_DATA_DEFINE_001*/
    {
        /* 0byte, 32byte, 64byte, 128byte, 256byte */
        OS_NUM_0,
        OS_ARCH_MPU_RGN_SIZE_32B,
        OS_ARCH_MPU_RGN_SIZE_64B,
        OS_ARCH_MPU_RGN_SIZE_128B,
        OS_ARCH_MPU_RGN_SIZE_256B,
        /* 512byte, 1KB, 2KB, 4KB */
        OS_ARCH_MPU_RGN_SIZE_512B,
        OS_ARCH_MPU_RGN_SIZE_1KB,
        OS_ARCH_MPU_RGN_SIZE_2KB,
        OS_ARCH_MPU_RGN_SIZE_4KB,
        /* 8KB, 16KB, 32KB, 64KB */
        OS_ARCH_MPU_RGN_SIZE_8KB,
        OS_ARCH_MPU_RGN_SIZE_16KB,
        OS_ARCH_MPU_RGN_SIZE_32KB,
        OS_ARCH_MPU_RGN_SIZE_64KB,
        /* 128KB, 256KB, 512KB, 1MB */
        OS_ARCH_MPU_RGN_SIZE_128KB,
        OS_ARCH_MPU_RGN_SIZE_256KB,
        OS_ARCH_MPU_RGN_SIZE_512KB,
        OS_ARCH_MPU_RGN_SIZE_1MKB,
        /* 2MB, 4MB, 8MB, 16MB */
        OS_ARCH_MPU_RGN_SIZE_2MKB,
        OS_ARCH_MPU_RGN_SIZE_4MKB,
        OS_ARCH_MPU_RGN_SIZE_8MKB,
        OS_ARCH_MPU_RGN_SIZE_16MKB,
        /* 32MB, 64MB, 128MB, 256MB */
        OS_ARCH_MPU_RGN_SIZE_32MKB,
        OS_ARCH_MPU_RGN_SIZE_64MKB,
        OS_ARCH_MPU_RGN_SIZE_128MKB,
        OS_ARCH_MPU_RGN_SIZE_256MKB,
        /* 512MB, 1GB, 2GB, 4GB */
        OS_ARCH_MPU_RGN_SIZE_512MKB,
        OS_ARCH_MPU_RGN_SIZE_1GB,
        OS_ARCH_MPU_RGN_SIZE_2GB,
        OS_ARCH_MPU_RGN_SIZE_4GB};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_Arch_MpuPriDefMemMap(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_Arch_MpuSetRegionConfig(const Os_Arch_Mpu_ConfigType* pUserConfigPtr, uint16 threadId, Os_CallLevelType threadLevel);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_Arch_MpuRegionCalc(Os_CallLevelType Level, uint16 Id, ApplicationType HostAppId, Os_Arch_MpSetType Set);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_Arch_MpuEnable(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static uint32 Os_Arch_CaculateMemSize(const uint8* startAddr, const uint8* endAddr);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_Arch_MpuRegionSet(Os_CallLevelType Level, uint16 Id);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (OS_MPROT_REGION_CFG_ENABLE == TRUE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_Arch_MpuToolCfgRegionSet(Os_MemoryRegionObjType Object, uint32 RegionStartNum, uint16 Id, Os_CallLevelType Level);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_Arch_ToolConfigAddressRegion(Os_CallLevelType Level, uint16 Id, ApplicationType HostAppId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_MPROT_REGION_CFG_ENABLE == TRUE */

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

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
static void Os_Arch_MpuPriDefMemMap(void)
{
    OS_ARCH_MPU_CTRL_REG |= OS_ARCH_MPU_CTRL_PRIVDEFENA_MASK; /* PRQA S 0303,3442 */ /* VL_Os_0303, VL_Os_3442 */
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
static void Os_Arch_MpuEnable(void)
{
    OS_ARCH_MPU_CTRL_REG |= OS_ARCH_MPU_CTRL_ENABLE; /* PRQA S 0303,3442 */ /* VL_Os_0303, VL_Os_3442 */
    OS_ARCH_DSYNC();
    OS_ARCH_ISYNC();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

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
static uint32 Os_Arch_CaculateMemSize(const uint8* startAddr, const uint8* endAddr)
{
    uint32 size    = OS_NUM_0;
    uint32 memSize = OS_NUM_0;
    uint32 idex    = OS_NUM_0;

    size = (uint32)(endAddr - startAddr); /* PRQA S 0488 */ /* VL_Os_0488 */ /*ARCH_MPU_PTR_OPERATIONS_003*/
    while (TRUE) /*PRQA S 2740*/ /* VL_Os_2740 */
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
static void Os_Arch_MpuRegionSet(Os_CallLevelType Level, uint16 Id)
{
    Os_MpuRegionType* RefRegeionType = (Level == OS_LEVEL_TASK) ? &Os_TaskMpuRegion : &Os_Isr2MpuRegion;
    Os_Arch_MpuDisable();
    /* PRQA S 0303 ++*/                              /* VL_Os_0303 */
    if (Os_PublicAreaCfg == FALSE) 
    {
        OS_ARCH_MPU_SET_REGION(OS_ARCH_MPU_REGION_NUM_ALL_PFLASH_SPACE, RefRegeionType, Id);
        OS_ARCH_MPU_SET_REGION(OS_ARCH_MPU_REGION_NUM_ALL_SRAM_SPACE, RefRegeionType, Id);
        OS_ARCH_MPU_SET_REGION(OS_ARCH_MPU_REGION_NUM_SHARED_SPACE, RefRegeionType, Id);
        Os_PublicAreaCfg = TRUE;
    }
    if (Os_CurAppTrustState != Os_PreAppTrustState)
    {
#if (OS_ARCH_MPU_NUM_OF_REGION_16 == OS_MCU_CFG_REGION_NUM)
        OS_ARCH_MPU_SET_REGION(OS_ARCH_MPU_REGION_NUM_ALL_APP_CODE, RefRegeionType, Id);
#endif

        OS_ARCH_MPU_SET_REGION(OS_ARCH_MPU_REGION_NUM_ALL_APP_DATA, RefRegeionType, Id);
        OS_ARCH_MPU_SET_REGION(OS_ARCH_MPU_REGION_NUM_ALL_PERI_SPACE, RefRegeionType, Id);
    }

#if (OS_ARCH_MPU_NUM_OF_REGION_16 == OS_MCU_CFG_REGION_NUM)
    OS_ARCH_MPU_SET_REGION(OS_ARCH_MPU_REGION_NUM_CUR_APP_CODE, RefRegeionType, Id);
#endif

    OS_ARCH_MPU_SET_REGION(OS_ARCH_MPU_REGION_NUM_CUR_APP_PRI_DATA, RefRegeionType, Id);
    OS_ARCH_MPU_SET_REGION(OS_ARCH_MPU_REGION_NUM_CUR_TASK_DATA, RefRegeionType, Id);

#if (OS_MPROT_REGION_CFG_ENABLE == TRUE)
    for (uint8 Index = OS_MCU_MPU_REGION_USEMAX_NUM; Index < OS_MCU_CFG_REGION_NUM; Index++)
    {
        OS_ARCH_MPU_SET_REGION(Index, RefRegeionType, Id);
    }
#endif /* OS_MPROT_REGION_CFG_ENABLE == TRUE */

    /* PRQA S 0303 --*/
    Os_PreAppTrustState = Os_CurAppTrustState;
    Os_Arch_MpuEnable();
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
static void Os_Arch_MpuSetRegionConfig(const Os_Arch_Mpu_ConfigType* pUserConfigPtr, uint16 threadId, Os_CallLevelType threadLevel)
{
    uint32 regionAttributes = 0U;
    uint32 regionRbar       = 0U;

    regionRbar = pUserConfigPtr->u32StartAddr;

    /* Set the region size */
    regionAttributes |= pUserConfigPtr->size;

    regionAttributes |= pUserConfigPtr->u32access;

    if (pUserConfigPtr->u32EndAddr > pUserConfigPtr->u32StartAddr)
    {
        /* Enable Region */
        regionAttributes |= OS_ARCH_MPU_REGION_ENABLE;
    }
    else
    {
        regionAttributes &= OS_ARCH_MPU_REGION_DISABLE;
    }

    /* PRQA S 0303 ++*/ /* VL_Os_0303 */
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
    /* PRQA S 0303 --*/ /*ARCH_MPU_TYPE_CAST_002*/
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
static void Os_Arch_MpuRegionCalc(Os_CallLevelType Level, uint16 Id, ApplicationType HostAppId, Os_Arch_MpSetType Set)
{
    Os_Arch_Mpu_ConfigType Mpu_Config;
    if (OS_MP_SET_TRUST_WITHOUT_PROT != Set)
    {
        /*By default, all code segments allow both privileged and user modes to read and execute.*/
        OS_ARCH_MAKE_MPU_CONFIG(
            OS_ARCH_MPU_REGION_NUM_ALL_PFLASH_SPACE,
            OS_ARCH_MPU_PRIV_RO_USER_RO_EXEC | OS_ARCH_MPU_NORMAL_OIWTNOWA_NONSHARED,
            PFLASH_START,
            PFLASH_END,
            Mpu_Config);
        Os_Arch_MpuSetRegionConfig(&Mpu_Config, Id, Level);

        /*By default, all RAM space is read-write accessible in privileged mode and read-only in user mode.*/
        OS_ARCH_MAKE_MPU_CONFIG(
            OS_ARCH_MPU_REGION_NUM_ALL_SRAM_SPACE,
            OS_ARCH_MPU_PRIV_RW_USER_RO_EXEC | OS_ARCH_MPU_NORMAL_OIWTNOWA_NONSHARED,
            SRAM_START,
            SRAM_END,
            Mpu_Config);
        Os_Arch_MpuSetRegionConfig(&Mpu_Config, Id, Level);

        /*The shared space has read-write access in privileged mode and is read-only in user mode.*/
        OS_ARCH_MAKE_MPU_CONFIG(
            OS_ARCH_MPU_REGION_NUM_SHARED_SPACE,
            OS_ARCH_MPU_PRIV_RW_USER_RO_EXEC | OS_ARCH_MPU_NORMAL_OINC_SHARED,
            SHARED_START,
            SHARED_END,
            Mpu_Config);
        Os_Arch_MpuSetRegionConfig(&Mpu_Config, Id, Level);

        /*The App private space belonging to the thread has read-write access in both privileged and user modes.*/
        OS_ARCH_MAKE_MPU_CONFIG(
            OS_ARCH_MPU_REGION_NUM_CUR_APP_PRI_DATA,
            OS_ARCH_MPU_PRIV_RW_USER_RW_NOEXEC | OS_ARCH_MPU_NORMAL_OIWTNOWA_NONSHARED,
            Os_AppPriDataAddr[HostAppId].AppAddrStart,
            Os_AppPriDataAddr[HostAppId].AppAddrEnd,
            Mpu_Config);
        Os_Arch_MpuSetRegionConfig(&Mpu_Config, Id, Level);

        /*The thread own private space has read-write permissions in both privileged and user modes.*/
        if (Level == OS_LEVEL_TASK)
        {
            OS_ARCH_MAKE_MPU_CONFIG(
                OS_ARCH_MPU_REGION_NUM_CUR_TASK_DATA,
                OS_ARCH_MPU_PRIV_RW_USER_RW_NOEXEC | OS_ARCH_MPU_NORMAL_OIWTNOWA_NONSHARED,
                Os_TaskDAddr[Id].TaskAddrStart,
                Os_TaskDAddr[Id].TaskAddrEnd,
                Mpu_Config);
        }
        else
        {
            OS_ARCH_MAKE_MPU_CONFIG(
                OS_ARCH_MPU_REGION_NUM_CUR_TASK_DATA,
                OS_ARCH_MPU_PRIV_RW_USER_RW_NOEXEC | OS_ARCH_MPU_NORMAL_OIWTNOWA_NONSHARED,
                Os_IsrDAddr[Id].IsrAddrStart,
                Os_IsrDAddr[Id].IsrAddrEnd,
                Mpu_Config);
        }
        Os_Arch_MpuSetRegionConfig(&Mpu_Config, Id, Level);

        if ((OS_MP_SET_TASK_NON_TRUST == Set) || (OS_MP_SET_ISR_NON_TRUST == Set))
        {
#if (OS_ARCH_MPU_NUM_OF_REGION_16 == OS_MCU_CFG_REGION_NUM)
            /*By default, threads belonging to an untrusted partition have read and execute permissions for all app code
             * spaces in privileged mode, and no permissions in user mode.*/
            OS_ARCH_MAKE_MPU_CONFIG(
                OS_ARCH_MPU_REGION_NUM_ALL_APP_CODE,
                OS_ARCH_MPU_PRIV_RO_USER_NA_EXEC | OS_ARCH_MPU_NORMAL_OIWTNOWA_NONSHARED,
                Os_Core_App_CAddr[0u].AppAddrStart,
                Os_Core_App_CAddr[OS_AUTOSAR_CORES - 1].AppAddrEnd,
                Mpu_Config);
            Os_Arch_MpuSetRegionConfig(&Mpu_Config, Id, Level);

            /*Threads belonging to an untrusted partition have read and execute permissions for their own code space in
             * both privileged and user modes.*/
            OS_ARCH_MAKE_MPU_CONFIG(
                OS_ARCH_MPU_REGION_NUM_CUR_APP_CODE,
                OS_ARCH_MPU_PRIV_RO_USER_RO_EXEC | OS_ARCH_MPU_NORMAL_OIWTNOWA_NONSHARED,
                Os_App_CAddr[HostAppId].AppAddrStart,
                Os_App_CAddr[HostAppId].AppAddrEnd,
                Mpu_Config);
            Os_Arch_MpuSetRegionConfig(&Mpu_Config, Id, Level);
#endif

            /*By default, threads belonging to an untrusted partition have read and write permissions for all app data
             * segments in privileged mode, and no permissions in user mode.*/
            OS_ARCH_MAKE_MPU_CONFIG(
                OS_ARCH_MPU_REGION_NUM_ALL_APP_DATA,
                (Level == OS_LEVEL_TASK) ? (OS_ARCH_MPU_PRIV_RW_USER_NA_NOEXEC | OS_ARCH_MPU_NORMAL_OIWTNOWA_NONSHARED)
                                         : (OS_ARCH_MPU_PRIV_NA_USER_NA_NOEXEC | OS_ARCH_MPU_NORMAL_OIWTNOWA_NONSHARED),
                Os_Core_App_DAddr[OS_NUM_0].AppAddrStart,
                Os_Core_App_DAddr[OS_AUTOSAR_CORES - OS_NUM_1].AppAddrEnd,
                Mpu_Config);
            Os_Arch_MpuSetRegionConfig(&Mpu_Config, Id, Level);

            /*Threads belonging to an untrusted partition have read and write permissions for peripheral regions in
             * privileged mode, and no permissions in user mode.*/
            OS_ARCH_MAKE_MPU_CONFIG(
                OS_ARCH_MPU_REGION_NUM_ALL_PERI_SPACE,
                (Level == OS_LEVEL_TASK) ? (OS_ARCH_MPU_PRIV_RW_USER_NA_EXEC | OS_ARCH_MPU_DEVICE_NONSHAREABLE)
                                         : (OS_ARCH_MPU_PRIV_NA_USER_NA_EXEC | OS_ARCH_MPU_DEVICE_NONSHAREABLE),
                PERIPHERAL_START,
                PERIPHERAL_END,
                Mpu_Config);
            Os_Arch_MpuSetRegionConfig(&Mpu_Config, Id, Level);
        }
        else
        {
#if (OS_ARCH_MPU_NUM_OF_REGION_16 == OS_MCU_CFG_REGION_NUM)
            /*The code space access permissions for a trusted app with a protected partition inherit the permissions of
             * `OS_ARCH_MPU_REGION_NUM_ALL_PFLASH_SPACE`.*/
            OS_ARCH_MAKE_MPU_CONFIG(
                OS_ARCH_MPU_REGION_NUM_ALL_APP_CODE,
                OS_ARCH_MPU_PRIV_RO_USER_RO_EXEC | OS_ARCH_MPU_NORMAL_OIWTNOWA_NONSHARED,
                Os_Core_App_CAddr[0u].AppAddrStart,
                Os_Core_App_CAddr[OS_AUTOSAR_CORES - 1].AppAddrEnd,
                Mpu_Config);
            Os_Arch_MpuSetRegionConfig(&Mpu_Config, Id, Level);
#endif

            /*a trusted app with a protected partition have read and write permissions for all app data segments in
             * privileged mode, and read permissions in user mode.*/
            OS_ARCH_MAKE_MPU_CONFIG(
                OS_ARCH_MPU_REGION_NUM_ALL_APP_DATA,
                (Level == OS_LEVEL_TASK) ? (OS_ARCH_MPU_PRIV_RW_USER_RO_NOEXEC | OS_ARCH_MPU_NORMAL_OIWTNOWA_NONSHARED)
                                         : (OS_ARCH_MPU_PRIV_RO_USER_RO_NOEXEC | OS_ARCH_MPU_NORMAL_OIWTNOWA_NONSHARED),
                Os_Core_App_DAddr[OS_NUM_0].AppAddrStart,
                Os_Core_App_DAddr[OS_AUTOSAR_CORES - OS_NUM_1].AppAddrEnd,
                Mpu_Config);
            Os_Arch_MpuSetRegionConfig(&Mpu_Config, Id, Level);

            /*Threads belonging to a trusted and protected partition have read and write permissions for peripheral
             * regions in privileged mode, and read-only permissions in user mode.*/
            OS_ARCH_MAKE_MPU_CONFIG(
                OS_ARCH_MPU_REGION_NUM_ALL_PERI_SPACE,
                (Level == OS_LEVEL_TASK) ? (OS_ARCH_MPU_PRIV_RW_USER_RO_EXEC | OS_ARCH_MPU_DEVICE_NONSHAREABLE)
                                         : (OS_ARCH_MPU_PRIV_RO_USER_RO_EXEC | OS_ARCH_MPU_DEVICE_NONSHAREABLE),
                PERIPHERAL_START,
                PERIPHERAL_END,
                Mpu_Config);
            Os_Arch_MpuSetRegionConfig(&Mpu_Config, Id, Level);
        }

#if (OS_MPROT_REGION_CFG_ENABLE == TRUE)
        /* Tool Configuration region of specified start and end addresses */
        Os_Arch_ToolConfigAddressRegion(Level, Id, HostAppId);
#endif /* OS_MPROT_REGION_CFG_ENABLE == TRUE */
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (OS_MPROT_REGION_CFG_ENABLE == TRUE)

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Tool Configure memory protection region address>
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
static void Os_Arch_MpuToolCfgRegionSet(Os_MemoryRegionObjType Object, uint32 RegionStartNum, uint16 Id, Os_CallLevelType Level)
{
    uint32 Index;
    uint32 Access;
    Os_Arch_Mpu_ConfigType Mpu_Config;

    /*ignore stacksapce bit, cause hardware does not support it
      ignore only write , cause hardware does not support it 
      ignore execute diff from user and kernel, cause hardware does not support it */
    for (Index = OS_NUM_0; Index < Object.regionsNum; Index++)
    {
        if (Object.regionsRef[Index]->access & OS_EXECUTBIT)
        {
            if (Object.regionsRef[Index]->access & OS_READBIT)
            {
                Access = OS_ARCH_MPU_PRIV_RW_USER_RW_EXEC;
                if (Object.regionsRef[Index]->access & OS_WRITEBIT)
                {
                    Access = OS_ARCH_MPU_PRIV_RW_USER_RW_EXEC;
                }
                else
                {
                    Access = OS_ARCH_MPU_PRIV_RW_USER_RO_EXEC;
                }
            }
            else
            {
                Access = OS_ARCH_MPU_PRIV_NA_USER_NA_EXEC;
            }
        }
        else 
        {
            if (Object.regionsRef[Index]->access & OS_READBIT)
            {
                Access = OS_ARCH_MPU_PRIV_RW_USER_RW_NOEXEC;
                if (Object.regionsRef[Index]->access & OS_WRITEBIT)
                {
                    Access = OS_ARCH_MPU_PRIV_RW_USER_RW_NOEXEC;
                }
                else
                {
                    Access = OS_ARCH_MPU_PRIV_RW_USER_RO_NOEXEC;
                }
            }
            else
            {
                Access = OS_ARCH_MPU_PRIV_NA_USER_NA_NOEXEC;
            }
        }

        /* shared area and device not supported */
        Access |= OS_ARCH_MPU_NORMAL_OIWTNOWA_NONSHARED;

        OS_ARCH_MAKE_MPU_CONFIG(RegionStartNum + Index, Access,
                                Object.regionsRef[Index]->startAddr,
                                Object.regionsRef[Index]->endAddr,
                                Mpu_Config);
        Os_Arch_MpuSetRegionConfig(&Mpu_Config, Id, Level);
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Tool Configure memory protection region address>
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
static void Os_Arch_ToolConfigAddressRegion(Os_CallLevelType Level, uint16 Id, Os_ApplicationType appId)
{
    uint32 UsedRgNum;
    uint32 RegionStartNum;
    Os_CoreIdType CoreId = Os_Hal_GetCoreID();

    UsedRgNum = (uint32)OS_NUM_0;
    UsedRgNum += Os_Memory_Region_Core_Inf[CoreId].regionsNum;
    UsedRgNum += Os_Memory_Region_Application_Inf[appId].regionsNum;
    if (Level == OS_LEVEL_TASK)
    {
        UsedRgNum += Os_Memory_Region_Task_Inf[Id].regionsNum;
    }
    else
    {
        UsedRgNum += Os_Memory_Region_Isr_Inf[Id].regionsNum;
    }

    if (UsedRgNum > (OS_MCU_CFG_REGION_NUM - OS_MCU_MPU_REGION_USEMAX_NUM))
    {
        /* Stopped here cause chip's region num not match required ones */
        while(1);
    }

    RegionStartNum = OS_MCU_MPU_REGION_USEMAX_NUM;

    if (Os_Memory_Region_Core_Inf[CoreId].regionsNum > OS_NUM_0)
    {
        Os_Arch_MpuToolCfgRegionSet(Os_Memory_Region_Core_Inf[CoreId], RegionStartNum, Id, Level);
        RegionStartNum += Os_Memory_Region_Core_Inf[CoreId].regionsNum;
    }

    if (Os_Memory_Region_Application_Inf[appId].regionsNum > OS_NUM_0)
    {
        Os_Arch_MpuToolCfgRegionSet(Os_Memory_Region_Application_Inf[appId], RegionStartNum, Id, Level);
        RegionStartNum += Os_Memory_Region_Application_Inf[appId].regionsNum;
    }

    if (Level == OS_LEVEL_TASK)
    {
        if (Os_Memory_Region_Task_Inf[Id].regionsNum > OS_NUM_0)
        {
            Os_Arch_MpuToolCfgRegionSet(Os_Memory_Region_Task_Inf[Id], RegionStartNum, Id, Level);
        }
    }
    else
    {
        if (Os_Memory_Region_Isr_Inf[Id].regionsNum > OS_NUM_0)
        {
            Os_Arch_MpuToolCfgRegionSet(Os_Memory_Region_Isr_Inf[Id], RegionStartNum, Id, Level);
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* OS_MPROT_REGION_CFG_ENABLE == TRUE */

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
void Os_Arch_DisableMpuRegion(uint8 region)
{
    uint32 tpRasr = 0U;
    /* PRQA S 0303 ++*/ /* VL_Os_0303 */
    OS_ARCH_MPU_RNR_REG  = region;
    tpRasr = OS_ARCH_MPU_RASR_REG;
    tpRasr = tpRasr & (~OS_ARCH_MPU_RASR_ENABLE_MASK);
    OS_ARCH_MPU_RASR_REG = tpRasr;
    /* PRQA S 0303 --*/ 
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
void Os_Arch_MpuDisable(void)
{
    OS_ARCH_MPU_CTRL_REG &= OS_ARCH_MPU_CTRL_DISABLE_MASK; /* PRQA S 0303,3442 */ /* VL_Os_0303, VL_Os_3442 */
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
void Os_Arch_MemProtMap(uint16 Id, Os_ApplicationType appId, Os_CallLevelType Level)
{
    
    if ((Os_AppSysId[Os_Hal_GetCoreID()] == appId)
        || ((TRUE == Os_AppCfg[appId].Trusted) && (TRUE != Os_AppCfg[appId].TrustedAppWithProtection)))
    {
        /*SYS_APP, as OS kernel, have all access rights*/
        Os_CurAppTrustState = OS_MP_SET_TRUST_WITHOUT_PROT;
        Os_Arch_MpuDefaultConfigSwitch();
        Os_PreAppTrustState = OS_MP_SET_TRUST_WITHOUT_PROT;
    }
    else if (FALSE == Os_AppCfg[appId].Trusted)
    {
        /*Non trusted apps or trusted apps protected by MPU*/
        Os_CurAppTrustState = (Level == OS_LEVEL_TASK) ? OS_MP_SET_TASK_NON_TRUST : OS_MP_SET_ISR_NON_TRUST;
        Os_Arch_MpuRegionSet(Level, Id);
    }
    else
    {
        /*Non trusted apps or trusted apps protected by MPU*/
        Os_CurAppTrustState = (Level == OS_LEVEL_TASK) ? OS_MP_SET_TASK_TRUST_WITH_PROT : OS_MP_SET_ISR_TRUST_WITH_PROT;
        Os_Arch_MpuRegionSet(Level, Id);
    }
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
    Os_Arch_MpuConfigTableInit();
    Os_Arch_MpuPriDefMemMap();
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
    OS_ARCH_MPU_CTRL_REG |= OS_ARCH_MPU_CTRL_ENABLE; /* PRQA S 0303,3442 */ /* VL_Os_0303, VL_Os_3442 */
    OS_ARCH_DSYNC();
    OS_ARCH_ISYNC();
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
void Os_Arch_MpuConfigTableInit(void)
{
    uint8              region;
    Os_Arch_MpSetType       set;
    Os_ApplicationType curAppId;
    uint16             id;
    Os_PublicAreaCfg    = FALSE;
    Os_PreAppTrustState = OS_MP_SET_INIT;
    Os_CurAppTrustState = OS_MP_SET_INIT;
    Os_Arch_MpuDisable();

    for (region = OS_NUM_0; region < OS_MCU_CFG_REGION_NUM; region++)
    { /* PRQA S 0303 ++*/ /* VL_Os_0303 */
    /* Multi-core with cache: a non-cacheable sync region is pre-filled before StartOS, so do not clear it here. */
#if (OS_AUTOSAR_CORES > 1)
        if (region == OS_MCU_NONCACHEABLE_REGION_NUM)
        {
            continue;
        }
#endif
        OS_ARCH_MPU_RNR_REG  = (uint32)region;
        OS_ARCH_MPU_RASR_REG = OS_NUM_0;
        OS_ARCH_MPU_RBAR_REG = OS_NUM_0;
        /* PRQA S 0303 --*/ 
    }

    for (id = OS_NUM_0; id < CFG_TASK_MAX; id++)
    {
        curAppId = Os_TaskCfg[id].ObjAppCfg->HostApp;
        /* PRQA S 3138 ++*/ /* VL_Os_3138 */
        set = OS_MPU_TASK_GET_APP_MP_TYPE(Os_AppCfg[curAppId].Trusted, Os_AppCfg[curAppId].TrustedAppWithProtection);
        /* PRQA S 3138 --*/ 
        Os_Arch_MpuRegionCalc(OS_LEVEL_TASK, id, curAppId, set);
    }

    for (id = OS_NUM_0; id < CFG_ISR2_MAX; id++)
    {
        curAppId = Os_IsrCfg[id].ObjAppCfg->HostApp;
        /* PRQA S 3138 ++*/ /* VL_Os_3138 */
        set = OS_MPU_ISR_GET_APP_MP_TYPE(Os_AppCfg[curAppId].Trusted, Os_AppCfg[curAppId].TrustedAppWithProtection);
        /* PRQA S 3138 --*/ 
        Os_Arch_MpuRegionCalc(OS_LEVEL_ISR2, id, curAppId, set);
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
 * Service ID           <Os_Arch_MpuDefaultConfigSwitch>
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

void Os_Arch_MpuDefaultConfigSwitch(void)
{
    Os_Arch_MpuDisable();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* FALSE == OS_MCU_EXTERNAL_MPU_USED */
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
