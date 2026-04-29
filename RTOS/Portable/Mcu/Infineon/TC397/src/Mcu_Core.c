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
/*******************************************************************************
**                                                                            **
**  FILENAME    :  Mcu_Core.c                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      :  i-soft-os                                                  **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_CoreCfg.h"
/*=======[M A C R O S]========================================================*/
#define CPU_SYSCON_BITS (24U)
#define REQSLP_MASK     (0xfffffffcu)
#define REQSLP_IDLE_VALUE  (0x01u)
#define DeviceNumber    (8u)
#define OS_CPU_ID_RST   (0x00C0C021U)
#define PMU0_ID_RST     (0x00E6C001U)
#define SMU_ID_RST      (0x0089C001U)
#define LMU0_MODID_RST  (0x0088C002U)

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
static Os_CoreModeType Os_CpuCoreState[CFG_CORE_MAX];
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"

/* PRQA S 3218 ++ */ /* VL_Os_3218 */
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE0_32
#include "Os_MemMap.h"
static uint32 Os_IsrNestPcxStackCore0[CFG_ISR_MAX_CORE0];
#define OS_STOP_SEC_VAR_CLEARED_CORE0_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
static Os_TaskCBExtType Os_TaskCBExtCore0[CFG_TASK_MAX_CORE0];
#define OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
static OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core0[CFG_CSA_MAX_CORE0];
#define OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
#endif /* TRUE == CFG_CORE0_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE1_32
#include "Os_MemMap.h"
static uint32 Os_IsrNestPcxStackCore1[CFG_ISR_MAX_CORE1];
#define OS_STOP_SEC_VAR_CLEARED_CORE1_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CORE1_UNSPECIFIED
#include "Os_MemMap.h"
static Os_TaskCBExtType Os_TaskCBExtCore1[CFG_TASK_MAX_CORE1];
#define OS_STOP_SEC_VAR_CLEARED_CORE1_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CORE1_UNSPECIFIED
#include "Os_MemMap.h"
static OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core1[CFG_CSA_MAX_CORE1];
#define OS_STOP_SEC_VAR_CLEARED_CORE1_UNSPECIFIED
#include "Os_MemMap.h"
#endif /* TRUE == CFG_CORE1_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE2_32
#include "Os_MemMap.h"
static uint32 Os_IsrNestPcxStackCore2[CFG_ISR_MAX_CORE2];
#define OS_STOP_SEC_VAR_CLEARED_CORE2_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CORE2_UNSPECIFIED
#include "Os_MemMap.h"
static Os_TaskCBExtType Os_TaskCBExtCore2[CFG_TASK_MAX_CORE2];
#define OS_STOP_SEC_VAR_CLEARED_CORE2_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CORE2_UNSPECIFIED
#include "Os_MemMap.h"
static OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core2[CFG_CSA_MAX_CORE2];
#define OS_STOP_SEC_VAR_CLEARED_CORE2_UNSPECIFIED
#include "Os_MemMap.h"
#endif /* TRUE == CFG_CORE2_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE3_32
#include "Os_MemMap.h"
static uint32 Os_IsrNestPcxStackCore3[CFG_ISR_MAX_CORE3];
#define OS_STOP_SEC_VAR_CLEARED_CORE3_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CORE3_UNSPECIFIED
#include "Os_MemMap.h"
static Os_TaskCBExtType Os_TaskCBExtCore3[CFG_TASK_MAX_CORE3];
#define OS_STOP_SEC_VAR_CLEARED_CORE3_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CORE3_UNSPECIFIED
#include "Os_MemMap.h"
static OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core3[CFG_CSA_MAX_CORE3];
#define OS_STOP_SEC_VAR_CLEARED_CORE3_UNSPECIFIED
#include "Os_MemMap.h"
#endif /* TRUE == CFG_CORE3_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE4_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE4_32
#include "Os_MemMap.h"
static uint32 Os_IsrNestPcxStackCore4[CFG_ISR_MAX_CORE4];
#define OS_STOP_SEC_VAR_CLEARED_CORE4_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CORE4_UNSPECIFIED
#include "Os_MemMap.h"
static Os_TaskCBExtType Os_TaskCBExtCore4[CFG_TASK_MAX_CORE4];
#define OS_STOP_SEC_VAR_CLEARED_CORE4_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CORE4_UNSPECIFIED
#include "Os_MemMap.h"
static OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core4[CFG_CSA_MAX_CORE4];
#define OS_STOP_SEC_VAR_CLEARED_CORE4_UNSPECIFIED
#include "Os_MemMap.h"
#endif /* TRUE == CFG_CORE4_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE5_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE5_32
#include "Os_MemMap.h"
static uint32 Os_IsrNestPcxStackCore5[CFG_ISR_MAX_CORE5];
#define OS_STOP_SEC_VAR_CLEARED_CORE5_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CORE5_UNSPECIFIED
#include "Os_MemMap.h"
static Os_TaskCBExtType Os_TaskCBExtCore5[CFG_TASK_MAX_CORE5];
#define OS_STOP_SEC_VAR_CLEARED_CORE5_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CORE5_UNSPECIFIED
#include "Os_MemMap.h"
static OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core5[CFG_CSA_MAX_CORE5];
#define OS_STOP_SEC_VAR_CLEARED_CORE5_UNSPECIFIED
#include "Os_MemMap.h"
#endif /* TRUE == CFG_CORE5_AUTOSAROS_ENABLE */
/* PRQA S 3218 -- */

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
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_MultiCoreInitProcessor(void)
/* PRQA S 1532 -- */
{
    Os_InterruptInit();

    Os_IsrType               i;
    uint16                   vCoreId = Os_SCB.sysCore;
    const Os_IsrCfgType*     pIsrCfg;
    const Os_IntInstallType* pIntInstall;
    /* PRQA S 2842,1290 ++ */ /* VL_Os_2842, VL_Os_1290 */
    for (i = 0; i < Os_CfgIsrMax_Inf[vCoreId]; i++)
    {
        pIsrCfg     = &(Os_IsrCfg_Inf[vCoreId][i]);
        pIntInstall = &(Os_IntInstall_Inf[vCoreId][i]);
        Os_InterruptInstall(pIsrCfg, pIntInstall);
    }
    /* PRQA S 2842,1290 -- */
    switch (vCoreId)
    {
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
    case OS_CORE_ID_0:
        Os_Csas            = Os_Csas_Core0;
        Os_CfgCsaMax       = CFG_CSA_MAX_CORE0;
        Os_TaskCBExt       = Os_TaskCBExtCore0;
        Os_IsrNestPcxStack = Os_IsrNestPcxStackCore0;
        Os_CpuCoreState[0] = OS_CORE_MODE_RUN;
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_IsrDAddr  = Os_Isr_DAddr_Core0;
        Os_TaskDAddr = Os_Task_DAddr_Core0;
#endif
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
        Os_SetSystemClock();
#endif
        break;
#endif

#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    case OS_CORE_ID_1:
        Os_Csas            = Os_Csas_Core1;
        Os_CfgCsaMax       = CFG_CSA_MAX_CORE1;
        Os_TaskCBExt       = Os_TaskCBExtCore1;
        Os_IsrNestPcxStack = Os_IsrNestPcxStackCore1;
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_IsrDAddr  = Os_Isr_DAddr_Core1;
        Os_TaskDAddr = Os_Task_DAddr_Core1;
#endif
        break;
#endif

#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    case OS_CORE_ID_2:
        Os_Csas            = Os_Csas_Core2;
        Os_CfgCsaMax       = CFG_CSA_MAX_CORE2;
        Os_TaskCBExt       = Os_TaskCBExtCore2;
        Os_IsrNestPcxStack = Os_IsrNestPcxStackCore2;
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_IsrDAddr  = Os_Isr_DAddr_Core2;
        Os_TaskDAddr = Os_Task_DAddr_Core2;
#endif
        break;
#endif

#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    case OS_CORE_ID_3:
        Os_Csas            = Os_Csas_Core3;
        Os_CfgCsaMax       = CFG_CSA_MAX_CORE3;
        Os_TaskCBExt       = Os_TaskCBExtCore3;
        Os_IsrNestPcxStack = Os_IsrNestPcxStackCore3;
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_IsrDAddr  = Os_Isr_DAddr_Core3;
        Os_TaskDAddr = Os_Task_DAddr_Core3;
#endif
        break;
#endif

#if (TRUE == CFG_CORE4_AUTOSAROS_ENABLE)
    case OS_CORE_ID_4:
        Os_Csas            = Os_Csas_Core4;
        Os_CfgCsaMax       = CFG_CSA_MAX_CORE4;
        Os_TaskCBExt       = Os_TaskCBExtCore4;
        Os_IsrNestPcxStack = Os_IsrNestPcxStackCore4;
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_IsrDAddr  = Os_Isr_DAddr_Core4;
        Os_TaskDAddr = Os_Task_DAddr_Core4;
#endif
        break;
#endif

#if (TRUE == CFG_CORE5_AUTOSAROS_ENABLE)
    case OS_CORE_ID_5:
        Os_Csas            = Os_Csas_Core5;
        Os_CfgCsaMax       = CFG_CSA_MAX_CORE5;
        Os_TaskCBExt       = Os_TaskCBExtCore5;
        Os_IsrNestPcxStack = Os_IsrNestPcxStackCore5;
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_IsrDAddr  = Os_Isr_DAddr_Core5;
        Os_TaskDAddr = Os_Task_DAddr_Core5;
#endif
        break;
#endif

    /*add comments to pass QAC.*/
    default:
        /* Nothing to do */
        break;
    }
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
    Os_InitSystemTimer();
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_InitTimingProtTimer();
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the core ID.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <CoreId>
 * PreCondition         <None>
 * CallByAPI            <GetCoreID>
 * REQ ID               <None>
 */
/******************************************************************************/
Os_CoreIdType Os_ArchGetCoreID(void)
{
    return ((Os_CoreIdType)OS_ARCH_MFCR(OS_REG_CUS_ID) & OS_CORE_ID_MSK);
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
void Os_ArchStartCore(Os_CoreIdType coreId)
{
    /* PRQA S 0428, 3469, 3345 ++ */ /* VL_Os_0428, VL_Os_3469, VL_Os_3345 */
    /* PRQA S 3442, 1259, 1258 ++ */ /* VL_Os_3442, VL_Os_1259, VL_Os_1258 */
    /* PRQA S 0303, 0305 ++ */       /* VL_Os_0303, VL_Os_0303 */
    switch (coreId)
    {
#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_1:
        OS_CPU1_PC = (uint32)_START1;
        Os_ArchResetENDINIT();
        OS_CPU1_SYSCON = OS_CPU1_SYSCON & (~((uint32)1u << CPU_SYSCON_BITS));
        Os_ArchSetENDINIT();
        break;
#endif
#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_2:
        OS_CPU2_PC = (uint32)_START2;
        Os_ArchResetENDINIT();
        OS_CPU2_SYSCON = OS_CPU2_SYSCON & (~((uint32)1u << CPU_SYSCON_BITS));
        Os_ArchSetENDINIT();
        break;
#endif
#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_3:
        OS_CPU3_PC = (uint32)_START3;
        Os_ArchResetENDINIT();
        OS_CPU3_SYSCON = OS_CPU3_SYSCON & (~((uint32)1u << CPU_SYSCON_BITS));
        Os_ArchSetENDINIT();
        break;
#endif
#if (TRUE == CFG_CORE4_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_4:
        OS_CPU4_PC = (uint32)_START4;
        Os_ArchResetENDINIT();
        OS_CPU4_SYSCON = OS_CPU4_SYSCON & (~((uint32)1u << CPU_SYSCON_BITS));
        Os_ArchSetENDINIT();
        break;
#endif
#if (TRUE == CFG_CORE5_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_5:
        OS_CPU5_PC = (uint32)_START5;
        Os_ArchResetENDINIT();
        OS_CPU5_SYSCON = OS_CPU5_SYSCON & (~((uint32)1u << CPU_SYSCON_BITS));
        Os_ArchSetENDINIT();
        break;
#endif
    default:
        /* Nothing to do */
        break;
    }
    /* PRQA S 0303, 0305 -- */
    /* PRQA S 3442, 1259, 1258 -- */
    /* PRQA S 0428, 3469, 3345 -- */
    Os_CpuCoreState[coreId] = OS_CORE_MODE_RUN;
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
 * CallByAPI            <Os_SetCoreMode>
 * REQ ID               <None>
 */
/******************************************************************************/
Os_CoreModeType Os_GetCoreMode(Os_CoreIdType core)
{
    return Os_CpuCoreState[core]; /* PRQA S 2842 */ /* VL_Os_2842 */
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
/* PRQA S 6030, 6010, 1532 ++*/ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_QAC_OneFunRef */
boolean Os_SetCoreMode(Os_CoreIdType core, Os_CoreModeType coreMode)
/* PRQA S 6030, 6010, 1532 --*/
{
    boolean         retVal      = TRUE;
    Os_CoreModeType curCoreMode = OS_CORE_MODE_UNKNOWN;
    VAR(volatile uint32*, OS_VAR) pmcsr;
    VAR(volatile uint32, OS_VAR) temp;
    /* Check the mode the Core is in */
    curCoreMode = Os_GetCoreMode(core);

    /* PRQA S 0303 ++ */ /* VL_Os_0303 */
    if (coreMode != curCoreMode)
    {
        switch (core)
        {
        case OS_PHYSICAL_CORE_0:
            pmcsr = &OS_SCU_PMCSR0;
            break;
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
                    Os_ArchStartCore(core);
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
                    Os_ArchResetENDINIT(); /* PRQA S 3469 */ /* VL_Os_3469 */
                    temp = *pmcsr; /* PRQA S 3345 */         /* VL_Os_3345 */
                    temp &= REQSLP_MASK; /* PRQA S 3442 */   /* VL_Os_3442 */
                    temp |= REQSLP_IDLE_VALUE; /* PRQA S 3442 */   /* VL_Os_3442 */
                    *pmcsr = temp; /* PRQA S 3345 */         /* VL_Os_3345 */
                    Os_ArchSetENDINIT(); /* PRQA S 3469 */   /* VL_Os_3469 */
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
                    Os_ArchResetENDINIT(); /* PRQA S 3469 */ /* VL_Os_3469 */
                    *pmcsr &= REQSLP_MASK; /* PRQA S 3442 */ /* VL_Os_3442 */
                    Os_ArchSetENDINIT(); /* PRQA S 3469 */   /* VL_Os_3469 */
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
    Os_CpuCoreState[core] = coreMode;
    /* PRQA S 0303 -- */ /* VL_Os_0303 */

    return retVal;
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
 * return               <CoreIdType, The return value is the unique logical
 *                       CoreID map to the input physical core.>
 * PreCondition         <None>
 * CallByAPI            <StartCore>
 * REQ ID               <None>
 */
/******************************************************************************/
CoreIdType Os_GetCoreLogID(CoreIdType phyCoreId)
{
    CoreIdType logCoreId;

    switch (phyCoreId)
    {
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_0:
        logCoreId = OS_CORE_ID_0;
        break;
#endif

#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_1:
        logCoreId = OS_CORE_ID_1;
        break;
#endif

#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_2:
        logCoreId = OS_CORE_ID_2;
        break;
#endif
#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_3:
        logCoreId = OS_CORE_ID_3;
        break;
#endif

#if (TRUE == CFG_CORE4_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_4:
        logCoreId = OS_CORE_ID_4;
        break;
#endif

#if (TRUE == CFG_CORE5_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_5:
        logCoreId = OS_CORE_ID_5;
        break;
#endif

    default:
        logCoreId = OS_CORE_INVALID;
        break;
    }

    return logCoreId;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The function returns a unique physical Core ID .>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-logCoreId[in]  <A valid logical Core ID from GetCoreID,Os_GetCoreLogID.>
 * return               <CoreIdType, The return value is the unique physical
 *                       CoreID map to the input logical core.>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
CoreIdType Os_GetCorePhyID(CoreIdType logCoreId)
/* PRQA S 1532 -- */
{
    CoreIdType phyCoreId;

    switch (logCoreId)
    {
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
    case OS_CORE_ID_0:
        phyCoreId = OS_PHYSICAL_CORE_0;
        break;
#endif

#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    case OS_CORE_ID_1:
        phyCoreId = OS_PHYSICAL_CORE_1;
        break;
#endif

#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    case OS_CORE_ID_2:
        phyCoreId = OS_PHYSICAL_CORE_2;
        break;
#endif

#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    case OS_CORE_ID_3:
        phyCoreId = OS_PHYSICAL_CORE_3;
        break;
#endif

#if (TRUE == CFG_CORE4_AUTOSAROS_ENABLE)
    case OS_CORE_ID_4:
        phyCoreId = OS_PHYSICAL_CORE_4;
        break;
#endif

#if (TRUE == CFG_CORE5_AUTOSAROS_ENABLE)
    case OS_CORE_ID_5:
        phyCoreId = OS_PHYSICAL_CORE_5;
        break;
#endif

    default:
        phyCoreId = OS_CORE_INVALID;
        break;
    }

    return phyCoreId;
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
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Arch_CheckCPUInformation(void)
/* PRQA S 1532 -- */
{
    uint32  index = 0U;
    boolean osRet = TRUE;
    /* PRQA S 3678 ++*/                     /* VL_Os_3678 */
    uint32 DeviceIDAddress[DeviceNumber] = {/*CPU1_CPU_ID,CPU2_CPU_ID,CPU3_CPU_ID, CPU4_CPU_ID, CPU5_CPU_ID*/
                                            OS_CPU1_CPU_ID,
                                            OS_CPU2_CPU_ID,
                                            OS_CPU3_CPU_ID,
                                            OS_CPU4_CPU_ID,
                                            OS_CPU5_CPU_ID,
                                            /*  PMU0_ID,  SMU_ID,     LMU0_MODID*/
                                            PMU0_ID,
                                            SMU_ID,
                                            LMU0_MODID};
    uint32 DeviceIDValue[DeviceNumber]   = {
        OS_CPU_ID_RST,
        OS_CPU_ID_RST,
        OS_CPU_ID_RST,
        OS_CPU_ID_RST,
        OS_CPU_ID_RST,
        PMU0_ID_RST,
        SMU_ID_RST,
        LMU0_MODID_RST};
    /* PRQA S 3678 -- */
    if (Os_SCB.sysOsLevel != OS_LEVEL_MAIN)
    {
        /*nothing to do*/
    }
    else
    {
        /* PRQA S 0771 ++ */                                                      /* VL_Os_0771 */
        for (index = 0u; index < (uint32)DeviceNumber; index++) /* PRQA S 1258 */ /* VL_Os_1258 */
        /* PRQA S 0771 -- */
        {
            if (0u == index)
            {
                /*CPU0_CPU_ID shall be checked*/
                if (OS_ARCH_MFCR(CPU_ID_REG) != (uint32)OS_CPU_ID_RST) /* PRQA S 1258 */ /* VL_Os_1258 */
                {
                    osRet = FALSE;
                    break;
                }
            }
            /* PRQA S 0303, 3469 ++*/                                                       /* VL_Os_0303, VL_Os_3469 */
            if (OS_REG32(DeviceIDAddress[index]) != DeviceIDValue[index]) /* PRQA S 3442 */ /* VL_Os_3442 */
            /* PRQA S 0303, 3469 --*/
            {
                osRet = FALSE;
                break;
            }
        }
        while (osRet == FALSE) /* PRQA S 2872, 2487 */ /* VL_Os_2872, VL_Os_2487 */
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
 * Brief                <idle task>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 2870, 2740, 1532 ++ */ /* VL_Os_2870, VL_Os_2740, VL_QAC_OneFunRef */
#if (CFG_TASK_MAX_CORE0 > 0U)
void Os_TaskEntry_IdleCore0(void)
{
    while (1)
    {
        IdleHook_Core0();
    }
}
#endif /* CFG_TASK_MAX_CORE0 > 0U */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <idle task>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <None>
 */
/******************************************************************************/
#if (CFG_TASK_MAX_CORE1 > 0U)
void Os_TaskEntry_IdleCore1(void)
{
    while (1)
    {
        IdleHook_Core1();
    }
}
#endif /* CFG_TASK_MAX_CORE1 > 0U */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <idle task>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <None>
 */
/******************************************************************************/
#if (CFG_TASK_MAX_CORE2 > 0U)
void Os_TaskEntry_IdleCore2(void)
{
    while (1)
    {
        IdleHook_Core2();
    }
}
#endif /* CFG_TASK_MAX_CORE2 > 0U */

/******************************************************************************/
/*
 * Brief                <idle task>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <None>
 */
/******************************************************************************/
#if (CFG_TASK_MAX_CORE3 > 0U)
void Os_TaskEntry_IdleCore3(void)
{
    while (1)
    {
        IdleHook_Core3();
    }
}
#endif /* CFG_TASK_MAX_CORE2 > 0U */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <idle task>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <None>
 */
/******************************************************************************/
#if (CFG_TASK_MAX_CORE4 > 0U)
void Os_TaskEntry_IdleCore4(void)
{
    while (1)
    {
        IdleHook_Core4();
    }
}
#endif /* CFG_TASK_MAX_CORE2 > 0U */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"

/******************************************************************************/
/*
 * Brief                <idle task>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <None>
 */
/******************************************************************************/
#if (CFG_TASK_MAX_CORE5 > 0U)
void Os_TaskEntry_IdleCore5(void)
{
    while (1)
    {
        IdleHook_Core5();
    }
}
#endif /* CFG_TASK_MAX_CORE2 > 0U */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 2870, 2740, 1532 -- */
/*=======[E N D   O F   F I L E]==============================================*/
