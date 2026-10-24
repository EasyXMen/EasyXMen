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
 **  FILENAME    : Os_Mcu_Core.c                                               **
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

/*=======[I N C L U D E S]====================================================*/
#include "Os_Cfg.h"
#include "Os_Types.h"
#include "Os_Mcu_Core.h"
#include "Os_Mcu_Timer.h"
#include "Os_Mcu_Irq.h"
#include "Os_Arch_Define.h"
#include "Os_Arch_Processor.h"
#include "Os_Arch_Irq.h"
#include "Os_Interrupt_CfgData.h"
#include "Os_Core_Cfg.h"
#include "Os_Core_CfgData.h"
#include "Os_Task_Cfg.h"
#include "Os_Task_CfgData.h"
#include "Os_Tprot_Cfg.h"
#include "Os_Osm_Cfg.h"
/*=======[M A C R O S]========================================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/* Address space of kernel view. */
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 0306, 1531 ++*/ /* VL_Os_0306,VL_QAC_OneRefSymbol */
const Os_McuMemProtKnCfgType Os_MemProtKnAddrCfg = {{
    /*Data*/
    (uint8*)OS_MCU_RAM_ADDR_START,
    (uint8*)OS_MCU_RAM_ADDR_END,

    (uint8*)OS_MCU_PERIPH_ADDR_START,
    (uint8*)OS_MCU_PERIPH_ADDR_END,

    (uint8*)OS_MCU_FLASH_ADDR_START,
    (uint8*)OS_MCU_FLASH_ADDR_END,
}};
/* PRQA S 0306, 1531 --*/
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
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_Hal_MultiCoreInit(Os_CoreIdType sysCore)
/* PRQA S 1532 -- */
{
    Os_SystickTimerArray[sysCore]->CNT = CFG_REG_OSTIMER_VALUE_CORE0;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialization of the CPU in the OS.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <void>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 */
/******************************************************************************/
/* PRQA S 3006, 1532 ++ */ /* VL_Os_3006,VL_QAC_OneFunRef */
void Os_Hal_InitCPU(void)
/* PRQA S 3006, 1532 -- */
{
    Os_CoreIdType            coreId = Os_Hal_GetCoreID();
    Os_IsrType               i;
    const Os_IsrCfgType*     pIsrCfg;
    const Os_IntInstallType* pIntInstall;
    Os_IsrType               idStartRange = Os_IsrIdRange[coreId].AllIsr.IsrStart;
    Os_IsrType               idEndRange   = Os_IsrIdRange[coreId].AllIsr.IsrEnd;

    /*Platform specific interrupt controller initialization*/
    Os_ArchInterruptInit();
    /* masked all FE interrupt */
    /* PRQA S 0303 ++ */ /* VL_Os_0303 */
    OS_MCU_FEINTMSK(coreId) = OS_ARCH_FEINTFMSK_MASK;
    /* PRQA S 0303 -- */

#if (OS_AUTOSAR_CORES > 1)
    /* Initialize the multil cores interrupt. */
    /* PRQA S 3120,1290,0303,1840,1277 ++ */ /* VL_QAC_MagicNum,VL_Os_1290,VL_Os_0303,VL_Os_1840,VL_Os_1277 */
    OS_MCU_IPIR_IPInFCLRS(0) = 0xF; 
    /* PRQA S 3120,1290,0303,1840,1277 -- */
    /* Write 1 to the x-th bit to enable the issuance of inter-PE interrupt requests from
    PEx to PEm. Write 0 to the x-th bit to disable the issuance of inter-PE interrupt
    requests from PEx to PEm */
    /* PRQA S 3442,0303,1840,4397 ++ */ /* VL_Os_3442,VL_Os_0303,VL_Os_1840,VL_Os_4397 */
    OS_MCU_IPIR_IPInENS(0) |= (uint8)(~(1U << coreId));
    /* PRQA S 3442,0303,1840,4397 -- */
#endif /* OS_AUTOSAR_CORES > 1 */

    for (i = idStartRange; i < idEndRange; i++) /* PRQA S 4527 */ /* VL_Os_4527 */
    {
        pIsrCfg     = &Os_IsrCfg[i];
        pIntInstall = &Os_IntInstall[i];
        Os_McuInterruptInstall(pIsrCfg, pIntInstall);
    }
    /* Enables write access of protected registers */
    /* PRQA S 0303 ++ */ /* VL_Os_0303 */
    OS_MCU_MSRKCPROT = OS_MCU_KCPORT_ENABLE;

    /* The clocks of OSTM0 - OSTM9 Setting are supplied */
    OS_MCU_MSR_OSTM = 0x00U;
    /* PRQA S 0303 -- */
    ASM("SYNCM"); /* PRQA S 1006 */ /* VL_Os_1006 */

/*Initialize system timer for system counter */
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
    Os_McuInitSystemTimer(coreId);
    /*notes: the code of start system timer must be lightweight, otherwise the
     * previous step 'synchronization' will not make sense*/
    OS_MCU_ENABLE_SYSTIMER(coreId); /* PRQA S 0303, 2743 */ /* VL_Os_0303 */ /* VL_Os_2743 */
#endif

/*Initialize system timer for time protection */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_McuInitTimingProtTimer(coreId);
    OS_MCU_ENABLE_TPTIMER(coreId); /* PRQA S 0303, 2743 */ /* VL_Os_0303 */ /* VL_Os_2743 */
#endif

    /* Monitor timer: shares the per-core TP timer instance (OSTM4..7); each
     * core initializes only its own baseline. When TP is disabled, the
     * monitor starts the instance itself (see Os_Mcu_InitMonitorTimer). */
    Os_Mcu_InitMonitorTimer(coreId);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Os_Hal_StartCore>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-coreId[in]     <selects the core which to start>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartCore>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_Hal_StartCore(Os_CoreIdType coreId)
{
    Os_CoreIdType phyCoreId = Os_Mcu_GetPhyCoreIdFromLog(coreId);
#if (OS_AUTOSAR_CORES > 1)
#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    if (OS_PHYSICAL_CORE_1 == phyCoreId)
    {
        /* PRQA S 4544,0303,4543,3442,1841,4542 ++ */ /* VL_Os_4544,VL_Os_0303,VL_Os_4543,VL_Os_3442,VL_Os_1841,VL_Os_4542 */
        OS_MCU_BOOTCTRL_ADDR |= (1 << 1);
        /* PRQA S 4544,0303,4543,3442,1841,4542 -- */
    }
#endif
#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    else if (OS_PHYSICAL_CORE_2 == phyCoreId)
    {
        /* PRQA S 4544,0303,4543,3442,1841,4542,3120 ++ */ /* VL_Os_4544,VL_Os_0303,VL_Os_4543,VL_Os_3442,VL_Os_1841,VL_Os_4542,VL_QAC_MagicNum */
        OS_MCU_BOOTCTRL_ADDR |= (1 << 2);
        /* PRQA S 4544,0303,4543,3442,1841,4542,3120 -- */
    }
#endif
#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    else if (OS_PHYSICAL_CORE_3 == phyCoreId)
    {
        /* PRQA S 4544,0303,4543,3442,1841,4542,3120 ++ */ /* VL_Os_4544,VL_Os_0303,VL_Os_4543,VL_Os_3442,VL_Os_1841,VL_Os_4542,VL_QAC_MagicNum */
        OS_MCU_BOOTCTRL_ADDR |= (1 << 3);
        /* PRQA S 4544,0303,4543,3442,1841,4542,3120 -- */
    }
#endif
    else
    {
        /* Nothing to do. */
    }
#endif
}
/* PRQA S 1532 -- */
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
 * Return               <Os_McuCoreModeType,the core mode>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_SetCoreMode>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Os_McuCoreModeType Os_Hal_GetCoreMode(Os_CoreIdType core)
/* PRQA S 1532 -- */
{
    (void)core;
    Os_McuCoreModeType coreMode = OS_CORE_MODE_UNKNOWN;
    return coreMode;
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
 */
/******************************************************************************/
/* PRQA S 3006, 1532 ++ */ /* VL_Os_3006,VL_QAC_OneFunRef */
boolean Os_Hal_SetCoreMode(Os_CoreIdType core, Os_McuCoreModeType coreMode)
/* PRQA S 3006, 1532 -- */
{
    boolean ret = TRUE; /* PRQA S 1290 */ /* VL_Os_1290 */

    (void)core;
    /* PRQA S 1006++ */ /* VL_Os_1006*/
    switch (coreMode)
    {
    case OS_CORE_MODE_HALT:
        OS_ARCH_HALT();
        OS_ARCH_NOP();
        break;

    case OS_CORE_MODE_IDLE:
        OS_ARCH_HALT();
        OS_ARCH_NOP();
        break;
    /* PRQA S 1006-- */
    case OS_CORE_MODE_RUN:
        /*When an exception, such as an interrupt or a TRAP for scheduling, occurs,
        the CPU exits the Halt mode automatically, without requiring additional setup. */
        break;

    default:
        ret = FALSE; /* PRQA S 1290 */ /* VL_Os_1290 */
        break;
    }

    return ret; /* PRQA S 1290 */ /* VL_Os_1290 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Get Core ID>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <CoreId>
 * PreCondition         <None>
 * CallByAPI            <GetCoreID>
 */
/******************************************************************************/
Os_CoreIdType Os_Hal_GetCoreID(void)
{
    uint32 coreId = OS_ARCH_REG_READ(OS_ARCH_REG_PEID) & 0x001FUL; /* PRQA S 3120 */ /* VL_QAC_MagicNum*/

    return Os_Mcu_GetLogCoreIdFromPhy((Os_CoreIdType)coreId);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The function returns a unique logical Core ID.>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-phyCoreId[in]  <A valid physical Core ID defined by the AUTOSAR OS.>
 * return               <Os_CoreIdType, The return value is the unique logical
 *                       CoreID map to the input physical core.>
 * PreCondition         <None>
 * CallByAPI            <StartCore>
 * REQ ID               <DD_1_0779, DD_1_0780, DD_1_1543, DD_1_1544>
 */
/******************************************************************************/
Os_CoreIdType Os_Mcu_GetLogCoreIdFromPhy(Os_CoreIdType phyCoreId) /* PRQA S 1505 */ /* VL_Os_1505 */
{
    Os_CoreIdType logCoreId;

    switch (phyCoreId)
    {
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_0:
        logCoreId = OS_CORE_ID_0;
        break;
#endif /* CFG_CORE0_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_1:
        logCoreId = OS_CORE_ID_1;
        break;
#endif /* CFG_CORE1_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_2:
        logCoreId = OS_CORE_ID_2;
        break;
#endif /* CFG_CORE2_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    case OS_PHYSICAL_CORE_3:
        logCoreId = OS_CORE_ID_3;
        break;
#endif /* CFG_CORE3_AUTOSAROS_ENABLE */

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
 * Brief                <The function returns a unique physical Core ID.>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-logCoreId[in]  <A valid logical Core ID from GetCoreID,Os_GetCoreLogID.>
 * return               <Os_CoreIdType, The return value is the unique physical
 *                       CoreID map to the input logical core.>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <DD_1_0781, DD_1_0782>
 */
/******************************************************************************/
/* PRQA S 1505 ++ */ /* VL_Os_1505 */
Os_CoreIdType Os_Mcu_GetPhyCoreIdFromLog(Os_CoreIdType logCoreId)
/* PRQA S 1505 -- */
{
    Os_CoreIdType phyCoreId;

    switch (logCoreId)
    {
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
    case OS_CORE_ID_0:
        phyCoreId = OS_PHYSICAL_CORE_0;
        break;
#endif /* TRUE == CFG_CORE0_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    case OS_CORE_ID_1:
        phyCoreId = OS_PHYSICAL_CORE_1;
        break;
#endif /* TRUE == CFG_CORE1_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    case OS_CORE_ID_2:
        phyCoreId = OS_PHYSICAL_CORE_2;
        break;
#endif /* TRUE == CFG_CORE2_AUTOSAROS_ENABLE */

#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    case OS_CORE_ID_3:
        phyCoreId = OS_PHYSICAL_CORE_3;
        break;
#endif /* TRUE == CFG_CORE3_AUTOSAROS_ENABLE */

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
 * Brief                <Check whether CPU information is correct.>
 * Service ID           <none>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_Hal_CheckCPUInformation(void)
/* PRQA S 1532 -- */
{
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
