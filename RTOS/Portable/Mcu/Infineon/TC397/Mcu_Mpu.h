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
**  FILENAME    : Mcu_Mpu.h                                                   **
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
#ifndef MCU_MPU_H
#define MCU_MPU_H
/*=======[I N C L U D E S]=================================================*/
#include "Arch_Mpu.h"
#include "Os_MprotCfg.h"
/*=======[T Y P E   D E F I N I T I O N S]==================================*/
typedef struct
{
    uint8* pCPU5DataRamStart; /* address range of ram */
    uint8* pCPU5DataRamEnd;

    uint8* pCPU4DataRamStart; /* address range of ram */
    uint8* pCPU4DataRamEnd;

    uint8* pCPU3DataRamStart; /* address range of ram */
    uint8* pCPU3DataRamEnd;

    uint8* pCPU2DataRamStart; /* address range of ram */
    uint8* pCPU2DataRamEnd;

    uint8* pCPU1DataRamStart; /* address range of ram */
    uint8* pCPU1DataRamEnd;

    uint8* pCPU0DataRamStart; /* address range of ram */
    uint8* pCPU0DataRamEnd;

    uint8* pGlobleRamStart; /* LMU in Tricore */
    uint8* pGlobleRamEnd;

    uint8* pDflashStart; /* DFLASH in Tricore*/
    uint8* pDflashEnd;

    uint8* pLocalDRamStart; /* DMI in Tricore*/
    uint8* pLocalDRamEnd;

    uint8* pPeripheralStart; /* PERIPHERAL in Tricore*/
    uint8* pPeripheralEnd;

    uint8* pRomStart; /* PFLASH in Tricore*/
    uint8* pRomEnd;
} Os_MemProtKnAddr;

typedef struct
{
    CONST(Os_MemProtKnAddr, OS_VAR) OsKernelAddr;
} Os_MemProtKnCfgType;

/*=======[M A C R O S]=====================================================*/

/* PRQA S 3472 ++*/ /* MISRA  Dir-4.9 */ /* ARCH_DEFINE_MACRO_001 */
#define OS_ADDRESS_IS_IN_ALL_RAM(Address, Size)                                              \
    (((((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pCPU5DataRamStart) <= (Address))            \
      && (((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pGlobleRamEnd) >= ((Address) + (Size)))) \
     || ((((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pLocalDRamStart) <= (Address))           \
         && (((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pLocalDRamEnd) >= ((Address) + (Size)))))
/* PRQA S 3472 --*/ /* MISRA  Dir-4.9 */

#define OS_ADDRESS_IS_IN_OTHER_APP(Address, Size, coreId, appsuborID)                             \
    (((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pCPU5DataRamStart <= (Address))                    \
     && ((uint32)Os_Core_App_DAddr[coreId].APP_ADDR_START >= ((Address) + (Size))))               \
        || (((uint32)Os_Core_App_DAddr[coreId].APP_ADDR_END <= (Address))                         \
            && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pPeripheralEnd >= ((Address) + (Size)))) \
        || (((uint32)Os_App_DAddr[appsuborID].APP_ADDR_START <= (Address))                        \
            && ((uint32)Os_App_DAddr[appsuborID].APP_ADDR_END >= ((Address) + (Size))))

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/*=======[M A C R O S]========================================================*/
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
#define Os_ArchSetNonTruIsrReg(                                                                                    \
    D_Addrbase,                                                                                                    \
    C_Addrbase,                                                                                                    \
    DBitPos,                                                                                                       \
    CBitPos,                                                                                                       \
    D_RModeAddr,                                                                                                   \
    D_WModeAddr,                                                                                                   \
    C_ModeAddr,                                                                                                    \
    Isr,                                                                                                           \
    HostApp,                                                                                                       \
    CoreId)                                                                                                        \
    {                                                                                                              \
        OS_ARCH_MTCR((D_Addrbase), (uint32)0x10000000);                                                            \
        OS_ARCH_MTCR((D_Addrbase) + 0x04U, (uint32)Os_Core_App_DAddr[CoreId].APP_ADDR_START);                      \
        OS_ARCH_MTCR((D_Addrbase) + 0x08U, (uint32)Os_Core_App_DAddr[CoreId].APP_ADDR_END);                        \
        OS_ARCH_MTCR((D_Addrbase) + 0x0CU, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pPeripheralEnd);               \
        OS_ARCH_MTCR((D_Addrbase) + 0x10U, (uint32)Os_App_DAddr[HostApp].APP_ADDR_START);                          \
        OS_ARCH_MTCR((D_Addrbase) + 0x14U, (uint32)Os_App_DAddr[HostApp].APP_ADDR_END);                            \
        OS_ARCH_MTCR((D_Addrbase) + 0x18U, (uint32)Os_AppPriDataAddr[HostApp].APP_ADDR_START);                     \
        OS_ARCH_MTCR((D_Addrbase) + 0x1CU, (uint32)Os_AppPriDataAddr[HostApp].APP_ADDR_END);                       \
        OS_ARCH_MTCR((D_Addrbase) + 0x20U, (uint32)Os_IsrDAddr[Isr].ISR_ADDR_START);                               \
        OS_ARCH_MTCR((D_Addrbase) + 0x24U, (uint32)Os_IsrDAddr[Isr].ISR_ADDR_END);                                 \
        OS_ARCH_MTCR((D_Addrbase) + 0x28U, (uint32)Os_AppCfg[HostApp].OsAppAssignedPeripheralAddr.APP_ADDR_START); \
        OS_ARCH_MTCR((D_Addrbase) + 0x2CU, (uint32)Os_AppCfg[HostApp].OsAppAssignedPeripheralAddr.APP_ADDR_END);   \
        OS_ARCH_MTCR((D_Addrbase) + 0x30U, (uint32)0x80000000);                                                    \
        OS_ARCH_MTCR((D_Addrbase) + 0x34U, (uint32)0x81000000);                                                    \
        OS_ARCH_MTCR((C_Addrbase), (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomStart);                            \
        OS_ARCH_MTCR((C_Addrbase) + 0x04U, (uint32)Os_Core_App_CAddr[CoreId].APP_ADDR_START);                      \
        OS_ARCH_MTCR((C_Addrbase) + 0x08U, (uint32)Os_Core_App_CAddr[CoreId].APP_ADDR_END);                        \
        OS_ARCH_MTCR((C_Addrbase) + 0x0CU, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomEnd);                      \
        OS_ARCH_MTCR((C_Addrbase) + 0x10U, (uint32)Os_App_CAddr[HostApp].APP_ADDR_START);                          \
        OS_ARCH_MTCR((C_Addrbase) + 0x14U, (uint32)Os_App_CAddr[HostApp].APP_ADDR_END);                            \
        OS_ARCH_MTCR((D_RModeAddr), (uint32)OS_ARCH_SET_MP_ACCESS_RIGHT(0xFU, (DBitPos) + 3U));                    \
        OS_ARCH_MTCR((D_WModeAddr), (uint32)OS_ARCH_SET_MP_ACCESS_RIGHT(0x7U, (DBitPos) + 3U));                    \
        OS_ARCH_MTCR((C_ModeAddr), (uint32)OS_ARCH_SET_MP_ACCESS_RIGHT(0x7U, (CBitPos)));                          \
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
        OS_ARCH_MTCR((D_Addrbase) + 0x08U, (uint32)Os_AppPriDataAddr[HostApp].APP_ADDR_START);       \
        OS_ARCH_MTCR((D_Addrbase) + 0x0CU, (uint32)Os_AppPriDataAddr[HostApp].APP_ADDR_END);         \
        OS_ARCH_MTCR((D_Addrbase) + 0x10U, (uint32)Os_IsrDAddr[Isr].ISR_ADDR_START);                 \
        OS_ARCH_MTCR((D_Addrbase) + 0x14U, (uint32)Os_IsrDAddr[Isr].ISR_ADDR_END);                   \
        OS_ARCH_MTCR((C_Addrbase), (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomStart);              \
        OS_ARCH_MTCR((C_Addrbase) + 0x04U, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomEnd);        \
        OS_ARCH_MTCR((D_RModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, (DBitPos)));                   \
        OS_ARCH_MTCR((D_WModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x3U, (DBitPos) + 1U));              \
        OS_ARCH_MTCR((C_ModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, (CBitPos)));                    \
    }

/* Set memory protection map for task of non-trusted app. */
#define Os_ArchSetNonTruTskReg(                                                                                    \
    D_Addrbase,                                                                                                    \
    C_Addrbase,                                                                                                    \
    DBitPos,                                                                                                       \
    CBitPos,                                                                                                       \
    D_RModeAddr,                                                                                                   \
    D_WModeAddr,                                                                                                   \
    C_ModeAddr,                                                                                                    \
    Task,                                                                                                          \
    HostApp,                                                                                                       \
    CoreId)                                                                                                        \
    {                                                                                                              \
        OS_ARCH_MTCR((D_Addrbase), (uint32)0x10000000);                                                            \
        OS_ARCH_MTCR((D_Addrbase) + 0x04U, (uint32)Os_Core_App_DAddr[CoreId].APP_ADDR_START);                      \
        OS_ARCH_MTCR((D_Addrbase) + 0x08U, (uint32)Os_Core_App_DAddr[CoreId].APP_ADDR_END);                        \
        OS_ARCH_MTCR((D_Addrbase) + 0x0CU, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pPeripheralEnd);               \
        OS_ARCH_MTCR((D_Addrbase) + 0x10U, (uint32)Os_App_DAddr[HostApp].APP_ADDR_START);                          \
        OS_ARCH_MTCR((D_Addrbase) + 0x14U, (uint32)Os_App_DAddr[HostApp].APP_ADDR_END);                            \
        OS_ARCH_MTCR((D_Addrbase) + 0x18U, (uint32)Os_AppPriDataAddr[HostApp].APP_ADDR_START);                     \
        OS_ARCH_MTCR((D_Addrbase) + 0x1CU, (uint32)Os_AppPriDataAddr[HostApp].APP_ADDR_END);                       \
        OS_ARCH_MTCR((D_Addrbase) + 0x20U, (uint32)Os_TaskDAddr[Task].Task_ADDR_START);                            \
        OS_ARCH_MTCR((D_Addrbase) + 0x24U, (uint32)Os_TaskDAddr[Task].Task_ADDR_END);                              \
        OS_ARCH_MTCR((D_Addrbase) + 0x28U, (uint32)Os_AppCfg[HostApp].OsAppAssignedPeripheralAddr.APP_ADDR_START); \
        OS_ARCH_MTCR((D_Addrbase) + 0x2CU, (uint32)Os_AppCfg[HostApp].OsAppAssignedPeripheralAddr.APP_ADDR_END);   \
        OS_ARCH_MTCR((D_Addrbase) + 0x30U, (uint32)0x80000000);                                                    \
        OS_ARCH_MTCR((D_Addrbase) + 0x34U, (uint32)0x81000000);                                                    \
        OS_ARCH_MTCR((C_Addrbase), (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomStart);                            \
        OS_ARCH_MTCR((C_Addrbase) + 0x04U, (uint32)Os_Core_App_CAddr[CoreId].APP_ADDR_START);                      \
        OS_ARCH_MTCR((C_Addrbase) + 0x08U, (uint32)Os_Core_App_CAddr[CoreId].APP_ADDR_END);                        \
        OS_ARCH_MTCR((C_Addrbase) + 0x0CU, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomEnd);                      \
        OS_ARCH_MTCR((C_Addrbase) + 0x10U, (uint32)Os_App_CAddr[HostApp].APP_ADDR_START);                          \
        OS_ARCH_MTCR((C_Addrbase) + 0x14U, (uint32)Os_App_CAddr[HostApp].APP_ADDR_END);                            \
        OS_ARCH_MTCR((D_RModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0xFU, (DBitPos) + 3U));                            \
        OS_ARCH_MTCR((D_WModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x7U, (DBitPos) + 3U));                            \
        OS_ARCH_MTCR((C_ModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x7U, (CBitPos)));                                  \
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
        OS_ARCH_MTCR((D_Addrbase) + 0x08U, (uint32)Os_AppPriDataAddr[HostApp].APP_ADDR_START);       \
        OS_ARCH_MTCR((D_Addrbase) + 0x0CU, (uint32)Os_AppPriDataAddr[HostApp].APP_ADDR_END);         \
        OS_ARCH_MTCR((D_Addrbase) + 0x10U, (uint32)Os_TaskDAddr[Task].Task_ADDR_START);              \
        OS_ARCH_MTCR((D_Addrbase) + 0x14U, (uint32)Os_TaskDAddr[Task].Task_ADDR_END);                \
        OS_ARCH_MTCR((C_Addrbase), (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomStart);              \
        OS_ARCH_MTCR((C_Addrbase) + 0x04U, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomEnd);        \
        OS_ARCH_MTCR((D_RModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, (DBitPos)));                   \
        OS_ARCH_MTCR((D_WModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x3U, (DBitPos) + 1U));              \
        OS_ARCH_MTCR((C_ModeAddr), OS_ARCH_SET_MP_ACCESS_RIGHT(0x1U, (CBitPos)));                    \
    }
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

/*=======[E X T E R N A L   D A T A]========================================*/
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

extern CONST(Os_MemProtKnCfgType, OS_CONST) Os_MemProtKnAddrCfg;

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
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
extern FUNC(void, OS_CODE) Os_ArchInitKnMemMap(void);
/******************************************************************************/
/*
 * Brief                <MemMap for Memory Protection Settings Task>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ExitISR2, Os_SwitchTask>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchSetTaskMemMap(TaskType TaskId, ApplicationType HostAppId, boolean isTrusted);
/******************************************************************************/
/*
 * Brief                <Os_ArchSetIsrMemMap>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_EnterISR2,Os_ExitISR2>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchSetIsrMemMap(ISRType IsrId, ApplicationType HostAppId, boolean isTrusted);
#endif
#endif
