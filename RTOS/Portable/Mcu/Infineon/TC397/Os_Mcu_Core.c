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
 **  @file               : Os_Mcu_Core.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : MCU source file for Core API implementations
 **
 ***********************************************************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Arch_Processor.h"
#include "Os_Core.h"
#include "Os_Task.h"
#include "Os_Interrupt.h"

/*=======[M A C R O S]========================================================*/
#define DeviceNumber (11UL)
/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#if (CFG_ISR_MAX > 0U)
/* PRQA S 1514 ++ */ /* VL_Os_1514 */
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE0_32
#include "Os_MemMap.h"
uint32 Os_IsrNestPcxStackCore0[CFG_ISR_MAX_CORE0];
#define OS_STOP_SEC_VAR_CLEARED_CORE0_32
#include "Os_MemMap.h"
#endif /* TRUE == CFG_CORE0_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE1_32
#include "Os_MemMap.h"
uint32 Os_IsrNestPcxStackCore1[CFG_ISR_MAX_CORE1];
#define OS_STOP_SEC_VAR_CLEARED_CORE1_32
#include "Os_MemMap.h"
#endif /* TRUE == CFG_CORE1_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE2_32
#include "Os_MemMap.h"
uint32 Os_IsrNestPcxStackCore2[CFG_ISR_MAX_CORE2];
#define OS_STOP_SEC_VAR_CLEARED_CORE2_32
#include "Os_MemMap.h"
#endif /* TRUE == CFG_CORE2_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE3_32
#include "Os_MemMap.h"
uint32 Os_IsrNestPcxStackCore3[CFG_ISR_MAX_CORE3];
#define OS_STOP_SEC_VAR_CLEARED_CORE3_32
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_CORE4_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE4_32
#include "Os_MemMap.h"
uint32 Os_IsrNestPcxStackCore4[CFG_ISR_MAX_CORE4];
#define OS_STOP_SEC_VAR_CLEARED_CORE4_32
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_CORE5_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE5_32
#include "Os_MemMap.h"
uint32 Os_IsrNestPcxStackCore5[CFG_ISR_MAX_CORE5];
#define OS_STOP_SEC_VAR_CLEARED_CORE5_32
#include "Os_MemMap.h"
#endif
/* PRQA S 1514 -- */
#endif

#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 3408, 1514 ++ */ /* VL_Os_3408, VL_Os_1514 */
Os_TaskCBExtType Os_TaskCBExtCores[CFG_TASK_MAX];
/* PRQA S 3408, 1514 -- */
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"

/* PRQA S 1514 ++ */ /* VL_Os_1514 */
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core0[CFG_CSA_MAX_CORE0];
#define OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE1_UNSPECIFIED
#include "Os_MemMap.h"
OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core1[CFG_CSA_MAX_CORE1];
#define OS_STOP_SEC_VAR_CLEARED_CORE1_UNSPECIFIED
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE2_UNSPECIFIED
#include "Os_MemMap.h"
OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core2[CFG_CSA_MAX_CORE2];
#define OS_STOP_SEC_VAR_CLEARED_CORE2_UNSPECIFIED
#include "Os_MemMap.h"
#endif /* TRUE == CFG_CORE2_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE3_UNSPECIFIED
#include "Os_MemMap.h"
OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core3[CFG_CSA_MAX_CORE3];
#define OS_STOP_SEC_VAR_CLEARED_CORE3_UNSPECIFIED
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_CORE4_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE4_UNSPECIFIED
#include "Os_MemMap.h"
OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core4[CFG_CSA_MAX_CORE4];
#define OS_STOP_SEC_VAR_CLEARED_CORE4_UNSPECIFIED
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_CORE5_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE5_UNSPECIFIED
#include "Os_MemMap.h"
OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core5[CFG_CSA_MAX_CORE5];
#define OS_STOP_SEC_VAR_CLEARED_CORE5_UNSPECIFIED
#include "Os_MemMap.h"
#endif
/* PRQA S 1514 -- */

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
void Os_Hal_MultiCoreInit(Os_CoreIdType sysCore) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    Os_ArchInterruptInit();

    Os_IsrType i;
    /* PRQA S 2842 ++ */ /* VL_Os_2842 */
    const Os_IsrCfgType*     pIsrCfg;
    const Os_IntInstallType* pIntInstall;
    Os_IsrType               idStartRange = Os_IsrIdRange[sysCore].AllIsr.IsrStart;
    Os_IsrType               idEndRange   = Os_IsrIdRange[sysCore].AllIsr.IsrEnd;
    /* PRQA S 2842 -- */

    for (i = idStartRange; i < idEndRange; i++) /* PRQA S 4527 */ /* VL_Os_4527 */
    {
        pIsrCfg     = &Os_IsrCfg[i];
        pIntInstall = &Os_IntInstall[i];
        Os_McuInterruptInstall(pIsrCfg, pIntInstall);
    }
    Os_TaskCBExt = Os_TaskCBExtCores;

    switch (sysCore)
    {
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
    case OS_CORE_ID_0:
        Os_Csas            = Os_Csas_Core0;
        Os_CfgCsaMax       = CFG_CSA_MAX_CORE0;
        Os_IsrNestPcxStack = Os_IsrNestPcxStackCore0;
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
        Os_McuSetSystemClock();
#endif
        break;
#endif

#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    case OS_CORE_ID_1:
        Os_Csas            = Os_Csas_Core1;
        Os_CfgCsaMax       = CFG_CSA_MAX_CORE1;
        Os_IsrNestPcxStack = Os_IsrNestPcxStackCore1;
        break;
#endif

#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    case OS_CORE_ID_2:
        Os_Csas            = Os_Csas_Core2;
        Os_CfgCsaMax       = CFG_CSA_MAX_CORE2;
        Os_IsrNestPcxStack = Os_IsrNestPcxStackCore2;
        break;
#endif

#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    case OS_CORE_ID_3:
        Os_Csas            = Os_Csas_Core3;
        Os_CfgCsaMax       = CFG_CSA_MAX_CORE3;
        Os_IsrNestPcxStack = Os_IsrNestPcxStackCore3;
        break;
#endif

#if (TRUE == CFG_CORE4_AUTOSAROS_ENABLE)
    case OS_CORE_ID_4:
        Os_Csas            = Os_Csas_Core4;
        Os_CfgCsaMax       = CFG_CSA_MAX_CORE4;
        Os_IsrNestPcxStack = Os_IsrNestPcxStackCore4;
        break;
#endif

#if (TRUE == CFG_CORE5_AUTOSAROS_ENABLE)
    case OS_CORE_ID_5:
        Os_Csas            = Os_Csas_Core5;
        Os_CfgCsaMax       = CFG_CSA_MAX_CORE5;
        Os_IsrNestPcxStack = Os_IsrNestPcxStackCore5;
        break;
#endif

    /*add comments to pass QAC.*/
    default:
        /* Nothing to do */
        break;
    }

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
    Os_McuInitSystemTimer(sysCore);
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_McuInitTimingProtTimer(sysCore);
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Start the core.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-coreId[in]     <selects the core which to start>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartCore>
 * REQ ID               <None>
 */
/******************************************************************************/
static void Os_McuStartCore(Os_CoreIdType coreId) /* PRQA S 3450 */ /* VL_Os_3450 */
{
#if ((TRUE == CFG_CORE1_AUTOSAROS_ENABLE) || \
     (TRUE == CFG_CORE2_AUTOSAROS_ENABLE) || \
     (TRUE == CFG_CORE3_AUTOSAROS_ENABLE) || \
     (TRUE == CFG_CORE4_AUTOSAROS_ENABLE) || \
     (TRUE == CFG_CORE5_AUTOSAROS_ENABLE))
    Os_CoreIdType phyCoreId = Os_McuGetCorePhyID(coreId);

    switch (phyCoreId)
    {
        /* PRQA S 0303, 3345, 3442 ++ */ /* VL_Os_0303, VL_Os_3345, VL_Os_3442*/
        /* PRQA S 3120, 4544, 1259 ++ */ /* VL_QAC_MagicNum, VL_Os_4544, VL_Os_1259*/
        /* PRQA S 0428, 0305 ++ */       /* VL_Os_0428 , VL_Os_0305*/
#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_1:
        OS_CPU1_PC = (uint32)_START1;
        Os_McuResetENDINIT();
        OS_CPU1_SYSCON = OS_CPU1_SYSCON & (~((uint32)1 << 24));
        Os_McuSetENDINIT();
        break;
#endif

#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)    
    case OS_PHYSICAL_CORE_2:
        OS_CPU2_PC = (uint32)_START2;
        Os_McuResetENDINIT();
        OS_CPU2_SYSCON = OS_CPU2_SYSCON & (~((uint32)1 << 24));
        Os_McuSetENDINIT();
        break;
#endif

#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_3:
        OS_CPU3_PC = (uint32)_START3;
        Os_McuResetENDINIT();
        OS_CPU3_SYSCON = OS_CPU3_SYSCON & (~((uint32)1 << 24));
        Os_McuSetENDINIT();
        break;
#endif

#if (TRUE == CFG_CORE4_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_4:
        OS_CPU4_PC = (uint32)_START4;
        Os_McuResetENDINIT();
        OS_CPU4_SYSCON = OS_CPU4_SYSCON & (~((uint32)1 << 24));
        Os_McuSetENDINIT();
        break;
#endif

#if (TRUE == CFG_CORE5_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_5:
        OS_CPU5_PC = (uint32)_START5;
        Os_McuResetENDINIT();
        OS_CPU5_SYSCON = OS_CPU5_SYSCON & (~((uint32)1 << 24));
        Os_McuSetENDINIT();
        break;
#endif
        /* PRQA S 0303, 3345, 3442 -- */
        /* PRQA S 3120, 4544, 1259 -- */
        /* PRQA S 0428, 0305 -- */

    default: /* Nothing to do. */
        break;
    }
#endif
    (void)coreId;
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
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6080, 6030, 6050 ++ */ /* VL_MTR_Os_STPTH, VL_MTR_Os_STMIF, VL_MTR_Os_STST3 */
/* PRQA S 6010, 1505, 3450 ++ */ /* VL_MTR_Os_STCYC, VL_Os_1505, VL_Os_3450 */
static Os_CoreModeType Os_McuGetCoreMode(Os_CoreIdType core)
/* PRQA S 6010, 1505, 3450 -- */
/* PRQA S 6080, 6030, 6050 -- */
{
    Os_CoreIdType phyCoreId = Os_McuGetCorePhyID(core);
    Os_CoreModeType  coreMode = OS_CORE_MODE_UNKNOWN;
    StatusType       status   = E_OK;
    volatile uint32* pmcsr; /* PRQA S 3678 */ /* VL_Os_3678 */
    uint32           tempRegValue;

    /* PRQA S 0303 ++ */ /* VL_Os_0303 */
    if (phyCoreId != Os_McuGetPhyCoreID())
    {
        switch (phyCoreId)
        {
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_0:
            tempRegValue = OS_REG32(OS_CPU0_DBGSR_ADDR);
            pmcsr        = &OS_SCU_PMCSR0;
            break;
#endif

#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_1:
            tempRegValue = OS_REG32(OS_CPU1_DBGSR_ADDR);
            pmcsr        = &OS_SCU_PMCSR1;
            break;
#endif

#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_2:
            tempRegValue = OS_REG32(OS_CPU2_DBGSR_ADDR);
            pmcsr        = &OS_SCU_PMCSR2;
            break;
#endif

#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_3:
            tempRegValue = OS_REG32(OS_CPU3_DBGSR_ADDR);
            pmcsr        = &OS_SCU_PMCSR3;
            break;
#endif

#if (TRUE == CFG_CORE4_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_4:
            tempRegValue = OS_REG32(OS_CPU4_DBGSR_ADDR);
            pmcsr        = &OS_SCU_PMCSR4;
            break;
#endif

#if (TRUE == CFG_CORE5_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_5:
            tempRegValue = OS_REG32(OS_CPU5_DBGSR_ADDR);
            pmcsr        = &OS_SCU_PMCSR5;
            break;
#endif

            /*add comments to pass QAC.*/
        default:
            status = E_NOT_OK;
            break;
        }
    }
    else
    {
        switch (core)
        {
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_0:
            tempRegValue = OS_ARCH_MFCR(OS_REG_DBGSR);
            pmcsr        = &OS_SCU_PMCSR0;
            break;
#endif

#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_1:
            tempRegValue = OS_ARCH_MFCR(OS_REG_DBGSR);
            pmcsr        = &OS_SCU_PMCSR1;
            break;
#endif

#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_2:
            tempRegValue = OS_ARCH_MFCR(OS_REG_DBGSR);
            pmcsr        = &OS_SCU_PMCSR2;
            break;
#endif

#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_3:
            tempRegValue = OS_ARCH_MFCR(OS_REG_DBGSR);
            pmcsr        = &OS_SCU_PMCSR3;
            break;
#endif

#if (TRUE == CFG_CORE4_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_4:
            tempRegValue = OS_ARCH_MFCR(OS_REG_DBGSR);
            pmcsr        = &OS_SCU_PMCSR4;
            break;
#endif

#if (TRUE == CFG_CORE5_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_5:
            tempRegValue = OS_ARCH_MFCR(OS_REG_DBGSR);
            pmcsr        = &OS_SCU_PMCSR5;
            break;
#endif

            /*add comments to pass QAC.*/
        default:
            status = E_NOT_OK;
            break;
        }
    }
    /* PRQA S 0303 -- */

    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum*/
    if ((StatusType)E_OK == status)
    {
        /*get the DBGSR.HALT status */
        tempRegValue &= 0x00000006u;

        /*Check if the requested CPU is in DBGSR HALT mode */
        if (0x00000002U == tempRegValue)
        {
            coreMode = OS_CORE_MODE_HALT;
        }
        else if (0x00000000U == tempRegValue) /*CPU is in DBGSR RUNNING mode*/
        {
            /* Get PMCSR.PMST status */
            tempRegValue = *pmcsr;
            tempRegValue &= 0x00000700U;

            if (0x00000100U == tempRegValue)
            {
                coreMode = OS_CORE_MODE_RUN;
            }
            else if (0x00000300U == tempRegValue)
            {
                coreMode = OS_CORE_MODE_IDLE;
            }
            else
            {
                coreMode = OS_CORE_MODE_UNKNOWN;
            }
        }
        else
        {
            coreMode = OS_CORE_MODE_UNKNOWN;
        }
    }
    /* PRQA S 3120 -- */
    return coreMode;
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
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6010, 1505, 1532 ++ */ /* VL_MTR_Os_STCYC, VL_Os_1505, VL_QAC_OneFunRef */
Os_CoreModeType Os_Hal_GetCoreMode(Os_CoreIdType core)
/* PRQA S 6010, 1505, 1532 -- */
{
    return Os_McuGetCoreMode(core);
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
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6030, 6010, 1532 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_QAC_OneFunRef */
boolean Os_Hal_SetCoreMode(Os_CoreIdType core, Os_CoreModeType coreMode)
/* PRQA S 6030, 6010, 1532 -- */
{
    Os_CoreIdType phyCoreId = Os_McuGetCorePhyID(core);
    boolean          retVal      = TRUE;
    Os_CoreModeType  curCoreMode = OS_CORE_MODE_UNKNOWN;
    volatile uint32* pmcsr;
    /* Check the mode the Core is in */
    curCoreMode = Os_McuGetCoreMode(core);

    /* PRQA S 0303 ++ */ /* VL_Os_0303 */
    if (coreMode != curCoreMode)
    {
        switch (phyCoreId)
        {
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_0:
            pmcsr = &OS_SCU_PMCSR0;
            break;
#endif

#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_1:
            pmcsr = &OS_SCU_PMCSR1;
            break;
#endif

#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_2:
            pmcsr = &OS_SCU_PMCSR2;
            break;
#endif

#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_3:
            pmcsr = &OS_SCU_PMCSR3;
            break;
#endif

#if (TRUE == CFG_CORE4_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_4:
            pmcsr = &OS_SCU_PMCSR4;
            break;
#endif

#if (TRUE == CFG_CORE5_AUTOSAROS_ENABLE)
        case OS_PHYSICAL_CORE_5:
            pmcsr = &OS_SCU_PMCSR5;
            break;
#endif

        /*add comments to pass QAC.*/
        default:
            retVal = FALSE;
            break;
        }

        if ((boolean)TRUE == retVal)
        {
            /* transition from halt to Run */
            if (OS_CORE_MODE_HALT == curCoreMode)
            {
                if (OS_CORE_MODE_RUN == coreMode)
                {
                    Os_McuStartCore(core);
                }
                else
                {
                    /* cannot go to any other mode e.g. IfxCpu_CoreMode_idle */
                    retVal = FALSE;
                }
            }
            /* From Run to Idle */
            else if (OS_CORE_MODE_RUN == curCoreMode)
            {
                if (OS_CORE_MODE_IDLE == coreMode)
                {
                    Os_McuResetENDINIT();
                    volatile uint32 temp = *pmcsr;
                    /* PRQA S 3442 ++ */ /* VL_Os_3442 */
                    temp &= 0xfffffffcu; /* PRQA S 3120 */ /* VL_QAC_MagicNum*/
                    temp |= 0x00000001u;
                    /* PRQA S 3442 -- */
                    *pmcsr = temp; /* PRQA S 3345 */ /* VL_Os_3345 */
                    Os_McuSetENDINIT();
                }
                else
                {
                    retVal = FALSE;
                }
            }
            /* idle to Run */
            else if (OS_CORE_MODE_IDLE == curCoreMode)
            {
                if (OS_CORE_MODE_RUN == coreMode)
                {
                    Os_McuResetENDINIT();
                    /* PRQA S 3442, 3120 ++ */ /* VL_Os_3442, VL_QAC_MagicNum */
                    *pmcsr &= 0xfffffffcu;
                    /* PRQA S 3442, 3120 -- */
                    Os_McuSetENDINIT();
                }
                else
                {
                    retVal = FALSE;
                }
            }
            else
            {
                retVal = FALSE;
            }
        }
    }
    /* PRQA S 0303 -- */

    return retVal;
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
void Os_Hal_CheckCPUInformation(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    uint32     index = 0U;
    StatusType osRet = TRUE; /* PRQA S 4404 */ /* VL_Os_4404 */

    /* PRQA S 3678, 3120 ++ */              /* VL_Os_3678, VL_QAC_MagicNum */
    const uint32 DeviceIDAddress[DeviceNumber] = {/*CPU1_CPU_ID,CPU2_CPU_ID,CPU3_CPU_ID, CPU4_CPU_ID, CPU5_CPU_ID, STM0_ID*/
                                            0xF883FE18UL,
                                            0xF885FE18UL,
                                            0xF887FE18UL,
                                            0xF889FE18UL,
                                            0xF88DFE18UL,
                                            0xF0000008UL,
                                            /* STM1_ID,    STM2_ID,   PMU0_ID,  SMU_ID,   LMU_MODID*/
                                            0xF0000108UL,
                                            0xF0000208UL,
                                            0xF8038508UL,
                                            0xF0036808UL,
                                            0xF8700808UL};
    const uint32 DeviceIDValue[DeviceNumber]   = {
        0x00C0C021UL,
        0x00C0C021UL,
        0x00C0C021UL,
        0x00C0C021UL,
        0x00C0C021UL,
        0x0000C007UL,
        0x0000C007UL,
        0x0000C007UL,
        0x00E6C001UL,
        0x0089C001UL,
        0x0088C003UL};
    /* PRQA S 3678, 3120 -- */

    if (Os_Kernel_GetSysLevel() != OS_LEVEL_MAIN)
    {
        /*nothing to do*/
    }
    else
    {
        /* PRQA S 0771, 1259 ++ */ /* VL_Os_0771, VL_Os_1259 */
        for (index = 0u; index < (uint32)DeviceNumber; index++)
        /* PRQA S 0771, 1259 -- */
        {
            if (0u == index)
            {
                /*CPU0_CPU_ID shall be checked*/
                if (OS_ARCH_MFCR(0xFE18u) != (uint32)0x00C0C021UL)/* PRQA S 3120 */ /* VL_QAC_MagicNum*/
                {
                    osRet = FALSE; /* PRQA S 4404 */ /* VL_Os_4404 */
                    break;
                }
            }
            /* PRQA S 0303, 3442 ++ */ /* VL_Os_0303, VL_Os_3442 */
            if (OS_REG32(DeviceIDAddress[index]) != DeviceIDValue[index])
            /* PRQA S 0303, 3442 -- */
            {
                osRet = FALSE; /* PRQA S 4404 */ /* VL_Os_4404 */
                break;
            }
        }
        while (osRet == FALSE) /* PRQA S 1881, 2872, 2487 */ /* VL_Os_1881, VL_Os_2872, VL_Os_2487 */
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
 * Brief                <Start the core for thr kennel.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-coreId[in]     <selects the core which to start>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartCore>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_StartCore(Os_CoreIdType coreId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    Os_McuStartCore(coreId);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
