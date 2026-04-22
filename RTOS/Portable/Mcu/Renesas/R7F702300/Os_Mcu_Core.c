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
/* PRQA S 3108-- */

/*=======[I N C L U D E S]====================================================*/
#include "Os_Arch_Processor.h"
#include "Os_Core_Cfg.h"
#include "Os_Task_Cfg.h"
#include "Os_Interrupt_CfgData.h"
#include "Os_Core_Cfg.h"
#include "Os_Core_CfgData.h"
#include "Os_Task_CfgData.h"
/*=======[M A C R O S]========================================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/* Address space of kernel view. */
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 0306, 1531 ++*/ /* MISRA Rule 11.4, VL_QAC_OneRefSymbol */
const Os_MemProtKnCfgType Os_MemProtKnAddrCfg = {{
    /*Data*/
    (uint8*)OS_ARCH_RAM_ADDR_START,
    (uint8*)OS_ARCH_RAM_ADDR_END,

    (uint8*)OS_ARCH_PERIPH_ADDR_START,
    (uint8*)OS_ARCH_PERIPH_ADDR_END,

    (uint8*)OS_ARCH_FLASH_ADDR_START,
    (uint8*)OS_ARCH_FLASH_ADDR_END,
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
void Os_Hal_MultiCoreInit(Os_CoreIdType sysCore)
{

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
/* PRQA S 3006, 1532 ++ */ /* MISRA Dir 4.3, VL_QAC_OneFunRef */
void Os_Hal_InitCPU(void)
/* PRQA S 3006, 1532 -- */ /* MISRA Dir 4.3 */
{
    Os_CoreIdType            coreId = Os_Hal_GetCoreID();
    Os_IsrType               i;
    const Os_IsrCfgType*     pIsrCfg;
    const Os_IntInstallType* pIntInstall;
    Os_IsrType               idStartRange = Os_IsrIdRange[coreId].AllIsr.IsrStart;
    Os_IsrType               idEndRange   = Os_IsrIdRange[coreId].AllIsr.IsrEnd;

    /*Platform specific interrupt controller initialization*/
    Os_InterruptInit();
    /* masked all FE interrupt */
    /* PRQA S 0303 ++ */ /* VL_Os_0303 */
    OS_FEINTMSK(coreId) = OS_FEINTFMSK_MASK;
    /* PRQA S 0303 -- */

#if (OS_AUTOSAR_CORES > 1)
    /* Initialize the multil cores interrupt. */
    OS_IPIR_IPInFCLRS(0) = 0xF;

    /* Write 1 to the x-th bit to enable the issuance of inter-PE interrupt requests from
    PEx to PEm. Write 0 to the x-th bit to disable the issuance of inter-PE interrupt
    requests from PEx to PEm */
    OS_IPIR_IPInENS(0) |= (uint8)(~(1U << coreId));
#endif /* OS_AUTOSAR_CORES > 1 */

    for (i = idStartRange; i < idEndRange; i++)
    {
        pIsrCfg     = &Os_IsrCfg[i];
        pIntInstall = &Os_IntInstall[i];
        Os_InterruptInstall(pIsrCfg, pIntInstall);
    }
    /* Enables write access of protected registers */
    /* PRQA S 0303 ++ */ /* VL_Os_0303 */
    OS_MSRKCPROT = OS_KCPORT_ENABLE;

    /* The clocks of OSTM0 - OSTM9 Setting are supplied */
    OS_MSR_OSTM = 0x00U;
    /* PRQA S 0303 -- */
    ASM("SYNCM"); /* PRQA S 1006 */      /* VL_Os_1006 */

/*Initialize system timer for system counter */
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
    Os_ArchInitSystemTimer(coreId);
        /*notes: the code of start system timer must be lightweight, otherwise the
     * previous step 'synchronization' will not make sense*/
    OS_ARCH_ENABLE_SYSTIMER(coreId); /* PRQA S 0303, 2743 */ /* VL_Os_0303 */ /* VL_Os_2743 */
#endif

/*Initialize system timer for time protection */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_ArchInitTimingProtTimer(coreId);
    OS_ARCH_ENABLE_TPTIMER(coreId); /* PRQA S 0303, 2743 */ /* VL_Os_0303 */ /* VL_Os_2743 */
#endif

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
    if(0u == coreId)
    {
        Os_Hal_LoadRatioInit();
    }
#endif
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
    Os_CoreIdType phyCoreId = Os_ArchGetCorePhyID(coreId);
#if (OS_AUTOSAR_CORES > 1)
#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    if (OS_PHYSICAL_CORE_1 == phyCoreId)
    {
        OS_BOOTCTRL_ADDR |= (1 << 1);
    }
#endif
#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    else if (OS_PHYSICAL_CORE_2 == phyCoreId)
    {
        OS_BOOTCTRL_ADDR |= (1 << 2);
    }
#endif
#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    else if (OS_PHYSICAL_CORE_3 == phyCoreId)
    {
        OS_BOOTCTRL_ADDR |= (1 << 3);
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
 * Return               <Os_CoreModeType,the core mode>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_SetCoreMode>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Os_CoreModeType Os_Hal_GetCoreMode(Os_CoreIdType core)
/* PRQA S 1532 -- */
{
    (void)core;
    Os_CoreModeType coreMode = OS_CORE_MODE_UNKNOWN;
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
/* PRQA S 3006, 1532 ++ */ /* MISRA Dir 4.3 */ /* VL_QAC_OneFunRef */
boolean Os_Hal_SetCoreMode(Os_CoreIdType core, Os_CoreModeType coreMode)
/* PRQA S 3006, 1532 -- */ /* MISRA Dir 4.3 */
{
    boolean ret = TRUE; /* PRQA S 1290 */ /* VL_Os_1290 */

    (void)core;
    /* PRQA S 1006++ */ /* VL_Os_1006*/
    switch (coreMode)
    {
    case OS_CORE_MODE_HALT:
        ASM_HALT();
        ASM_NOP();
        break;

    case OS_CORE_MODE_IDLE:
        ASM_HALT();
        ASM_NOP();
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
    uint32 coreId = OS_ARCH_REG_READ(OS_REG_PEID) & 0x001FUL; /* PRQA S 3120 */ /* VL_QAC_MagicNum*/

    return (Os_CoreIdType)coreId;
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
 * return               <CoreIdType, The return value is the unique logical
 *                       CoreID map to the input physical core.>
 * PreCondition         <None>
 * CallByAPI            <StartCore>
 * REQ ID               <DD_1_0779, DD_1_0780, DD_1_1543, DD_1_1544>
 */
/******************************************************************************/
CoreIdType Os_ArchGetCoreLogID(CoreIdType phyCoreId)
{
    CoreIdType logCoreId;

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
 * return               <CoreIdType, The return value is the unique physical
 *                       CoreID map to the input logical core.>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <DD_1_0781, DD_1_0782>
 */
/******************************************************************************/
/* PRQA S 1505 ++ */ /* VL_Os_1505 */
CoreIdType Os_ArchGetCorePhyID(CoreIdType logCoreId)
/* PRQA S 1505 -- */
{
    CoreIdType phyCoreId;

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

#if (CFG_TASK_MAX_CORE0 > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The free task entry for core 0.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <DD_1_0838, DD_1_1217, DD_1_0839>
 */
/******************************************************************************/
void Os_TaskEntry_IdleCore0(void)
{
    while (1) /* PRQA S 2740 */ /* VL_Os_2740 */
    {
        IdleHook_Core0();
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_TASK_MAX_CORE0 > 0U */

#if (CFG_TASK_MAX_CORE1 > 0U) /* PRQA S 3332 */ /* VL_Os_3332 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The free task entry for core 1.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <DD_1_0838, DD_1_1217, DD_1_0839>
 */
/******************************************************************************/
void Os_TaskEntry_IdleCore1(void)
{
    while (1)
    {
        IdleHook_Core1();
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_TASK_MAX_CORE1 > 0U */

#if (CFG_TASK_MAX_CORE2 > 0U) /* PRQA S 3332 */ /* VL_Os_3332 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The free task entry for core 2.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <DD_1_0838, DD_1_1217, DD_1_0839>
 */
/******************************************************************************/
void Os_TaskEntry_IdleCore2(void)
{
    while (1)
    {
        IdleHook_Core2();
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_TASK_MAX_CORE2 > 0U */

#if (CFG_TASK_MAX_CORE3 > 0U) /* PRQA S 3332 */ /* VL_Os_3332 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The free task entry for core 3.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <DD_1_0838, DD_1_1217, DD_1_0839>
 */
/******************************************************************************/
void Os_TaskEntry_IdleCore3(void)
{
    while (1)
    {
        IdleHook_Core3();
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_TASK_MAX_CORE3 > 0U */
/*=======[E N D   O F   F I L E]==============================================*/
