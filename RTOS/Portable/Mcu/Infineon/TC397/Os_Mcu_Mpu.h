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
 ************************************************************************************************************************
 **
 **  @file               : Os_Mcu_Mpu.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Mcu header file for Mpu api declarations
 **
 ***********************************************************************************************************************/

#ifndef ARCH_MPU_H
#define ARCH_MPU_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_Arch_Mpu.h"
#include "Os_Mprot.h"
/*=======[M A C R O S]========================================================*/
/* Memory protection register. CPU Data Protection Range */
#define OS_DPR_0L  (0xC000U)
#define OS_DPR_0U  (0xC004U)
#define OS_DPR_1L  (0xC008U)
#define OS_DPR_1U  (0xC00CU)
#define OS_DPR_2L  (0xC010U)
#define OS_DPR_2U  (0xC014U)
#define OS_DPR_3L  (0xC018U)
#define OS_DPR_3U  (0xC01CU)
#define OS_DPR_4L  (0xC020U)
#define OS_DPR_4U  (0xC024U)
#define OS_DPR_5L  (0xC028U)
#define OS_DPR_5U  (0xC02CU)
#define OS_DPR_6L  (0xC030U)
#define OS_DPR_6U  (0xC034U)
#define OS_DPR_7L  (0xC038U)
#define OS_DPR_7U  (0xC03CU)
#define OS_DPR_8L  (0xC040U)
#define OS_DPR_8U  (0xC044U)
#define OS_DPR_9L  (0xC048U)
#define OS_DPR_9U  (0xC04CU)
#define OS_DPR_10L (0xC050U)
#define OS_DPR_10U (0xC054U)
#define OS_DPR_11L (0xC058U)
#define OS_DPR_11U (0xC05CU)
#define OS_DPR_12L (0xC060U)
#define OS_DPR_12U (0xC064U)
#define OS_DPR_13L (0xC068U)
#define OS_DPR_13U (0xC06CU)
#define OS_DPR_14L (0xC070U)
#define OS_DPR_14U (0xC074U)
#define OS_DPR_15L (0xC078U)
#define OS_DPR_15U (0xC07CU)
#define OS_DPR_16L (0xC080U)
#define OS_DPR_16U (0xC084U)
#define OS_DPR_17L (0xC088U)
#define OS_DPR_17U (0xC08CU)

/*CPU Data Protection Read Enable*/
#define OS_DPRE0 (0xE010u)
#define OS_DPRE1 (0xE014u)
#define OS_DPRE2 (0xE018u)
#define OS_DPRE3 (0xE01Cu)
#define OS_DPRE4 (0xE050u)
#define OS_DPRE5 (0xE054u)

/*CPU Data Protection Write Enable*/
#define OS_DPWE0 (0xE020u)
#define OS_DPWE1 (0xE024u)
#define OS_DPWE2 (0xE028u)
#define OS_DPWE3 (0xE02Cu)
#define OS_DPWE4 (0xE060u)
#define OS_DPWE5 (0xE064u)

/*CPU Code Protection Range */
#define OS_CPR_0L (0xD000U)
#define OS_CPR_0U (0xD004U)
#define OS_CPR_1L (0xD008U)
#define OS_CPR_1U (0xD00CU)
#define OS_CPR_2L (0xD010U)
#define OS_CPR_2U (0xD014U)
#define OS_CPR_3L (0xD018U)
#define OS_CPR_3U (0xD01CU)
#define OS_CPR_4L (0xD020U)
#define OS_CPR_4U (0xD024U)
#define OS_CPR_5L (0xD028U)
#define OS_CPR_5U (0xD02CU)
#define OS_CPR_6L (0xD030U)
#define OS_CPR_6U (0xD034U)
#define OS_CPR_7L (0xD038U)
#define OS_CPR_7U (0xD03CU)
#define OS_CPR_8L (0xD040U)
#define OS_CPR_8U (0xD044U)
#define OS_CPR_9L (0xD048U)
#define OS_CPR_9U (0xD04CU)

/*CPU Code Protection Execute Enable*/
#define OS_CPXE0 (0xE000u)
#define OS_CPXE1 (0xE004u)
#define OS_CPXE2 (0xE008u)
#define OS_CPXE3 (0xE00Cu)
#define OS_CPXE4 (0xE040u)
#define OS_CPXE5 (0xE044u)

/* Set access rights for memory protection. */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
/* PRQA S 3410 ++ */ /* VL_Os_3410 */
#define OS_ARCH_SET_MP_ACCESS_RIGHT(rights, offset) (((uint32)rights) << (offset))
/* PRQA S 3410 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
#define OS_HAL_ADDRESS_IS_IN_ALL_RAM(address, size)                                          \
    (((((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pCPU5DataRamStart) <= (address))            \
      && (((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pGlobleRamEnd) >= ((address) + (size)))) \
     || ((((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pLocalDRamStart) <= (address))           \
         && (((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pLocalDRamEnd) >= ((address) + (size)))))

#define OS_HAL_ADDRESS_IS_NOT_IN_OTHER_APP(address, size, coreId, appId)                           \
    ((((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pCPU5DataRamStart <= (address))                    \
     && ((uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1u].AppAddrStart >= ((address) + (size))))                      \
        || (((uint32)Os_Core_App_DAddr[0u].AppAddrEnd <= (address))             \
            && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pCPU0DataRamEnd >= ((address) + (size)))) \
        || (((uint32)Os_App_DAddr[appId].AppAddrStart <= (address))                                \
            && ((uint32)Os_App_DAddr[appId].AppAddrEnd >= ((address) + (size)))))
#endif
/* PRQA S 3472 -- */

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/* PRQA S 3458 ++ *//* VL_Os_3458 */
/* Set up areas that can be accessed by a trusted APP. */
#define Os_ArchSetTruReg(D_Addrbase, C_Addrbase, DBitPos, CBitPos, D_RModeAddr, D_WModeAddr, C_ModeAddr) \
    {                                                                                                    \
        OS_ARCH_MTCR((D_Addrbase), (uint32)0x10000000);                                                  \
        OS_ARCH_MTCR((D_Addrbase) + 0x04U, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pPeripheralEnd);     \
        OS_ARCH_MTCR((C_Addrbase), (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomStart);                  \
        OS_ARCH_MTCR((C_Addrbase) + 0x04U, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomEnd);            \
        OS_ARCH_MTCR((D_RModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, DBitPos));                         \
        OS_ARCH_MTCR((D_WModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, DBitPos));                         \
        OS_ARCH_MTCR((C_ModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, CBitPos));                          \
    }

/* Set memory protection map for isr of non-trusted app */
#define Os_ArchSetNonTruIsrReg(                                                                                \
    D_Addrbase,                                                                                                \
    C_Addrbase,                                                                                                \
    DBitPos,                                                                                                   \
    CBitPos,                                                                                                   \
    D_RModeAddr,                                                                                               \
    D_WModeAddr,                                                                                               \
    C_ModeAddr,                                                                                                \
    Isr,                                                                                                       \
    HostApp,                                                                                                   \
    CoreId)                                                                                                    \
    {                                                                                                          \
        OS_ARCH_MTCR((D_Addrbase), (uint32)0x10000000);                                                        \
        OS_ARCH_MTCR((D_Addrbase) + 0x04U, (uint32)Os_Core_App_DAddr[CoreId].AppAddrStart);                    \
        OS_ARCH_MTCR((D_Addrbase) + 0x08U, (uint32)Os_Core_App_DAddr[CoreId].AppAddrEnd);                      \
        OS_ARCH_MTCR((D_Addrbase) + 0x0CU, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pPeripheralEnd);           \
        OS_ARCH_MTCR((D_Addrbase) + 0x10U, (uint32)Os_App_DAddr[HostApp].AppAddrStart);                        \
        OS_ARCH_MTCR((D_Addrbase) + 0x14U, (uint32)Os_App_DAddr[HostApp].AppAddrEnd);                          \
        OS_ARCH_MTCR((D_Addrbase) + 0x18U, (uint32)Os_AppPriDataAddr[HostApp].AppAddrStart);                   \
        OS_ARCH_MTCR((D_Addrbase) + 0x1CU, (uint32)Os_AppPriDataAddr[HostApp].AppAddrEnd);                     \
        OS_ARCH_MTCR((D_Addrbase) + 0x20U, (uint32)Os_IsrDAddr[Isr].IsrAddrStart);                             \
        OS_ARCH_MTCR((D_Addrbase) + 0x24U, (uint32)Os_IsrDAddr[Isr].IsrAddrEnd);                               \
        OS_ARCH_MTCR((D_Addrbase) + 0x28U, (uint32)Os_AppCfg[HostApp].AppAssignedPeripheralAddr.AppAddrStart); \
        OS_ARCH_MTCR((D_Addrbase) + 0x2CU, (uint32)Os_AppCfg[HostApp].AppAssignedPeripheralAddr.AppAddrEnd);   \
        OS_ARCH_MTCR((D_Addrbase) + 0x30U, (uint32)0x80000000u);                                               \
        OS_ARCH_MTCR((D_Addrbase) + 0x34U, (uint32)0x81000000u);                                               \
        OS_ARCH_MTCR((C_Addrbase), (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomStart);                        \
        OS_ARCH_MTCR((C_Addrbase) + 0x04U, (uint32)Os_Core_App_CAddr[CoreId].AppAddrStart);                    \
        OS_ARCH_MTCR((C_Addrbase) + 0x08U, (uint32)Os_Core_App_CAddr[CoreId].AppAddrEnd);                      \
        OS_ARCH_MTCR((C_Addrbase) + 0x0CU, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomEnd);                  \
        OS_ARCH_MTCR((C_Addrbase) + 0x10U, (uint32)Os_App_CAddr[HostApp].AppAddrStart);                        \
        OS_ARCH_MTCR((C_Addrbase) + 0x14U, (uint32)Os_App_CAddr[HostApp].AppAddrEnd);                          \
        OS_ARCH_MTCR((D_RModeAddr), (uint32)OS_ARCH_SET_MP_ACCESS_RIGHT(0xFU, (DBitPos) + 3U));                \
        OS_ARCH_MTCR((D_WModeAddr), (uint32)OS_ARCH_SET_MP_ACCESS_RIGHT(0x7U, (DBitPos) + 3U));                \
        OS_ARCH_MTCR((C_ModeAddr), (uint32)OS_ARCH_SET_MP_ACCESS_RIGHT(0x7U, (CBitPos)));                      \
    }

/* Set memory protection map for isr of trusted app */
#define Os_ArchSetTruIsrProtReg(                                                                     \
    D_Addrbase,                                                                                      \
    C_Addrbase,                                                                                      \
    DBitPos,                                                                                         \
    CBitPos,                                                                                         \
    D_RModeAddr,                                                                                     \
    D_WModeAddr,                                                                                     \
    C_ModeAddr,                                                                                      \
    Isr,                                                                                             \
    HostApp)                                                                                         \
    {                                                                                                \
        OS_ARCH_MTCR((D_Addrbase), (uint32)0x10000000);                                              \
        OS_ARCH_MTCR((D_Addrbase) + 0x04U, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pPeripheralEnd); \
        OS_ARCH_MTCR((D_Addrbase) + 0x08U, (uint32)Os_AppPriDataAddr[HostApp].AppAddrStart);         \
        OS_ARCH_MTCR((D_Addrbase) + 0x0CU, (uint32)Os_AppPriDataAddr[HostApp].AppAddrEnd);           \
        OS_ARCH_MTCR((D_Addrbase) + 0x10U, (uint32)Os_IsrDAddr[Isr].IsrAddrStart);                   \
        OS_ARCH_MTCR((D_Addrbase) + 0x14U, (uint32)Os_IsrDAddr[Isr].IsrAddrEnd);                     \
        OS_ARCH_MTCR((C_Addrbase), (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomStart);              \
        OS_ARCH_MTCR((C_Addrbase) + 0x04U, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomEnd);        \
        OS_ARCH_MTCR((D_RModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, (DBitPos)));                   \
        OS_ARCH_MTCR((D_WModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x3U, (DBitPos) + 1U));              \
        OS_ARCH_MTCR((C_ModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, (CBitPos)));                    \
    }

/* Set memory protection map for task of non-trusted app. */
#define Os_ArchSetNonTruTskReg(                                                                                \
    D_Addrbase,                                                                                                \
    C_Addrbase,                                                                                                \
    DBitPos,                                                                                                   \
    CBitPos,                                                                                                   \
    D_RModeAddr,                                                                                               \
    D_WModeAddr,                                                                                               \
    C_ModeAddr,                                                                                                \
    Task,                                                                                                      \
    HostApp,                                                                                                   \
    CoreId)                                                                                                    \
    {                                                                                                          \
        OS_ARCH_MTCR((D_Addrbase), (uint32)0x10000000);                                                        \
        OS_ARCH_MTCR((D_Addrbase) + 0x04U, (uint32)Os_Core_App_DAddr[CoreId].AppAddrStart);                    \
        OS_ARCH_MTCR((D_Addrbase) + 0x08U, (uint32)Os_Core_App_DAddr[CoreId].AppAddrEnd);                      \
        OS_ARCH_MTCR((D_Addrbase) + 0x0CU, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pPeripheralEnd);           \
        OS_ARCH_MTCR((D_Addrbase) + 0x10U, (uint32)Os_App_DAddr[HostApp].AppAddrStart);                        \
        OS_ARCH_MTCR((D_Addrbase) + 0x14U, (uint32)Os_App_DAddr[HostApp].AppAddrEnd);                          \
        OS_ARCH_MTCR((D_Addrbase) + 0x18U, (uint32)Os_AppPriDataAddr[HostApp].AppAddrStart);                   \
        OS_ARCH_MTCR((D_Addrbase) + 0x1CU, (uint32)Os_AppPriDataAddr[HostApp].AppAddrEnd);                     \
        OS_ARCH_MTCR((D_Addrbase) + 0x20U, (uint32)Os_TaskDAddr[Task].TaskAddrStart);                          \
        OS_ARCH_MTCR((D_Addrbase) + 0x24U, (uint32)Os_TaskDAddr[Task].TaskAddrEnd);                            \
        OS_ARCH_MTCR((D_Addrbase) + 0x28U, (uint32)Os_AppCfg[HostApp].AppAssignedPeripheralAddr.AppAddrStart); \
        OS_ARCH_MTCR((D_Addrbase) + 0x2CU, (uint32)Os_AppCfg[HostApp].AppAssignedPeripheralAddr.AppAddrEnd);   \
        OS_ARCH_MTCR((D_Addrbase) + 0x30U, (uint32)0x80000000u);                                               \
        OS_ARCH_MTCR((D_Addrbase) + 0x34U, (uint32)0x81000000u);                                               \
        OS_ARCH_MTCR((C_Addrbase), (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomStart);                        \
        OS_ARCH_MTCR((C_Addrbase) + 0x04U, (uint32)Os_Core_App_CAddr[CoreId].AppAddrStart);                    \
        OS_ARCH_MTCR((C_Addrbase) + 0x08U, (uint32)Os_Core_App_CAddr[CoreId].AppAddrEnd);                      \
        OS_ARCH_MTCR((C_Addrbase) + 0x0CU, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomEnd);                  \
        OS_ARCH_MTCR((C_Addrbase) + 0x10U, (uint32)Os_App_CAddr[HostApp].AppAddrStart);                        \
        OS_ARCH_MTCR((C_Addrbase) + 0x14U, (uint32)Os_App_CAddr[HostApp].AppAddrEnd);                          \
        OS_ARCH_MTCR((D_RModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0xFU, (DBitPos) + 3U));                        \
        OS_ARCH_MTCR((D_WModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x7U, (DBitPos) + 3U));                        \
        OS_ARCH_MTCR((C_ModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x7U, (CBitPos)));                              \
    }

/* Set memory protection map for task of trusted app. */
#define Os_ArchSetTruTskProtReg(                                                                     \
    D_Addrbase,                                                                                      \
    C_Addrbase,                                                                                      \
    DBitPos,                                                                                         \
    CBitPos,                                                                                         \
    D_RModeAddr,                                                                                     \
    D_WModeAddr,                                                                                     \
    C_ModeAddr,                                                                                      \
    Task,                                                                                            \
    HostApp)                                                                                         \
    {                                                                                                \
        OS_ARCH_MTCR((D_Addrbase), (uint32)0x10000000);                                              \
        OS_ARCH_MTCR((D_Addrbase) + 0x04U, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pPeripheralEnd); \
        OS_ARCH_MTCR((D_Addrbase) + 0x08U, (uint32)Os_AppPriDataAddr[HostApp].AppAddrStart);         \
        OS_ARCH_MTCR((D_Addrbase) + 0x0CU, (uint32)Os_AppPriDataAddr[HostApp].AppAddrEnd);           \
        OS_ARCH_MTCR((D_Addrbase) + 0x10U, (uint32)Os_TaskDAddr[Task].TaskAddrStart);                \
        OS_ARCH_MTCR((D_Addrbase) + 0x14U, (uint32)Os_TaskDAddr[Task].TaskAddrEnd);                  \
        OS_ARCH_MTCR((C_Addrbase), (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomStart);              \
        OS_ARCH_MTCR((C_Addrbase) + 0x04U, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomEnd);        \
        OS_ARCH_MTCR((D_RModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, (DBitPos)));                   \
        OS_ARCH_MTCR((D_WModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x3U, (DBitPos) + 1U));              \
        OS_ARCH_MTCR((C_ModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, (CBitPos)));                    \
    }
/* PRQA S 3458 -- */

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
/* PRQA S 3684, 1752 ++ */ /* VL_Os_3684, VL_Os_1752 */
/*Data*/
extern uint8 dsram5_START[];
extern uint8 dsram5_END[];
extern uint8 dsram4_START[];
extern uint8 dsram4_END[];
extern uint8 dsram3_START[];
extern uint8 dsram3_END[];
extern uint8 dsram2_START[];
extern uint8 dsram2_END[];
extern uint8 dsram1_START[];
extern uint8 dsram1_END[];
extern uint8 dsram0_START[];
extern uint8 dsram0_END[];
extern uint8 LMU_SRAM_START[];
extern uint8 LMU_SRAM_END[];
extern uint8 DFLASH_START[];
extern uint8 DFLASH_END[];
extern uint8 DMI_DSPR_START[];
extern uint8 DMI_DSPR_END[];
extern uint8 PERIPHERAL_START[];
extern uint8 PERIPHERAL_END[];
extern uint8 RODATA_START[];
extern uint8 RODATA_END[];
/*Code*/
/* PRQA S 1500 ++ */ /* VL_Os_1500 */
extern uint8 psram2_START[];
extern uint8 psram2_END[];
extern uint8 psram1_START[];
extern uint8 psram1_END[];
extern uint8 psram0_START[];
extern uint8 psram0_END[];
extern uint8 PFLASH_START[];
extern uint8 PFLASH_END[];
extern uint8 PMI_PSPR_START[];
extern uint8 PMI_PSPR_END[];
/* PRQA S 1500 -- */
/* PRQA S 3684, 1752 -- */
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/******************************************************************************/
/*
 * Brief                <Initialize memory protection related registers>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_SwitchTask,Os_InitMemProt>
 * REQ ID               <None>
 */
/******************************************************************************/
extern void Os_Hal_InitKnMemMap(void);

/******************************************************************************/
/*
 * Brief                <MemMap for Memory Protection Settings Task>
 *
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <>
 */
/******************************************************************************/
extern void Os_Hal_MemProtTaskMap(Os_TaskType taskId);

/******************************************************************************/
/*
 * Brief                <MemMap for Memory Protection Settings ISR>
 *
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <>
 */
/******************************************************************************/
extern void Os_Hal_MemProtIsrMap(Os_IsrType isrId);

/******************************************************************************/
/*
 * Brief                <MemMap for Memory Protection Settings Kernel>
 *
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <>
 */
/******************************************************************************/
extern void Os_Hal_MemProtKernelMap(void);
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/

#endif /* OS_MPU_H */
/*=======[E N D   O F   F I L E]==============================================*/
