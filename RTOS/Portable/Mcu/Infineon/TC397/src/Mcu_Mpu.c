/*******************************************************************************
**                                                                            **
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
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME    :  Mcu_Mpu.c                                                  **
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
#include "Os_Internal.h"
/*=======[M A C R O S]========================================================*/
#define PPRS_MASK      (0x00003000U)
#define ISR_PPRS_SET   (0x02U)
#define TASK_PPRS_SET  (0x01U)
#define ISR_DPR_START  (7U)
#define ISR_CPR_START  (4U)
#define TASK_DPR_START (1U)
#define TASK_CPR_START (1U)

/*=======[E X T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/* Address space of kernel view. */
const Os_MemProtKnCfgType Os_MemProtKnAddrCfg = {{
    /*Data*/
    (uint8*)dsram5_START,
    (uint8*)dsram5_END,
    (uint8*)dsram4_START,
    (uint8*)dsram4_END,
    (uint8*)dsram3_START,
    (uint8*)dsram3_END,
    (uint8*)dsram2_START,
    (uint8*)dsram2_END,
    (uint8*)dsram1_START,
    (uint8*)dsram1_END,
    (uint8*)dsram0_START,
    (uint8*)dsram0_END,
    (uint8*)LMU_SRAM_START,
    (uint8*)LMU_SRAM_END,
    (uint8*)DFLASH_START,
    (uint8*)DFLASH_END,
    (uint8*)DMI_DSPR_START,
    (uint8*)DMI_DSPR_END,
    (uint8*)PERIPHERAL_START,
    (uint8*)PERIPHERAL_END,
    /*Code*/
    (uint8*)PFLASH_START,
    (uint8*)PFLASH_END,
}};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
/* PRQA S 1532 ++ */                             /* VL_QAC_OneFunRef */
void Os_ArchInitKnMemMap(void) /* PRQA S 3006 */ /* VL_Os_3006 */
/* PRQA S 1532 -- */
{
    /* Set tc397 reg_set, use set_0 */
    /* Set-up the Data regions for the Os_Kernel. */
    /* PRQA S 3138, 1006 ++ */ /* VL_Os_3138, VL_Os_1006 */
    /* PRQA S 0306 ++ */  /* VL_Os_0306 */
    OS_ARCH_MTCR(OS_DPR_0L, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pCPU5DataRamStart);
    OS_ARCH_MTCR(OS_DPR_0U, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pPeripheralEnd);

    /* Set-up the Code regions for the Kernel. */
    OS_ARCH_MTCR(OS_CPR_0L, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomStart);
    OS_ARCH_MTCR(OS_CPR_0U, (uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomEnd);
    /* PRQA S 0306 -- */

    /* Set privileges of data and code sections. */
    OS_ARCH_MTCR(OS_DPRE0, 0x0001u); /* All data regions are R */
    OS_ARCH_MTCR(OS_DPWE0, 0x0001u); /* All data regions are W*/
    OS_ARCH_MTCR(OS_CPXE0, 0x0001u); /* All code regions are E */
    /* PRQA S 3138, 1006 -- */

    Os_ArchSetMemProtSet(0x00UL);
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
/* PRQA S 1505 ++ */                                                                                    /* VL_Os_1505 */
void Os_ArchSetIsrMemMap(ISRType IsrId, ApplicationType HostAppId, boolean isTrusted) /* PRQA S 3006 */ /* VL_Os_3006 */
/* PRQA S 1505 -- */
{
    CoreIdType coreId;
    uint32     temp = (Os_ArchGetMemProtSet() & PPRS_MASK) >> PPRS_SET_BITS;
    uint8      OsDPRStart;
    uint8      OsCPRStart;

    coreId = Os_SCB.sysCore;

    /* Find out which register set in use  register 0 should not be used here */
    if (ISR_PPRS_SET == temp)
    {
        /*set 2: OS_DPR_7~OS_DPR_12, OS_CPR_4~OS_CPR_6*/
        OsDPRStart = ISR_DPR_START;
        OsCPRStart = ISR_CPR_START;
        /* PRQA S 3432, 1259, 1255, 1258 ++ */ /* VL_Os_3432, VL_Os_1259, VL_Os_1255, VL_Os_1258 */
        if (TRUE != isTrusted)                 /* Non-trusted app running. */
        {
            /* PRQA S 3138, 3141, 0306, 1006 ++ */ /* VL_Os_3138, VL_Os_3141, VL_Os_0306, VL_Os_1006 */
            Os_ArchSetNonTruIsrReg(
                OS_DPR_7L,
                OS_CPR_4L,
                OsDPRStart,
                OsCPRStart,
                OS_DPRE2,
                OS_DPWE2,
                OS_CPXE2,
                IsrId,
                HostAppId,
                coreId);
            /* PRQA S 1006 -- */
            /* PRQA S 3138, 3141, 0306, 1006 -- */
        }
        else /* Trusted app or os_kernel running. */
        {
            /*OSTrustedApplicationWithProtection != TRUE*/
            if (Os_AppCfg[HostAppId].OsTrustedAppWithProtection != TRUE)
            {
                /* PRQA S 3138, 0306, 1006 ++*/ /* VL_Os_3138, VL_Os_0306, VL_Os_1006 */
                Os_ArchSetTruReg(OS_DPR_7L, OS_CPR_4L, OsDPRStart, OsCPRStart, OS_DPRE2, OS_DPWE2, OS_CPXE2);
                /* PRQA S 3138, 0306, 1006 --*/
            }
            else
            {
                /* PRQA S 3138, 3141, 0306, 1006 ++*/ /* VL_Os_3138, VL_Os_3141, VL_Os_0306, VL_Os_1006 */
                Os_ArchSetTruIsrProtReg(
                    OS_DPR_7L,
                    OS_CPR_4L,
                    OsDPRStart,
                    OsCPRStart,
                    OS_DPRE2,
                    OS_DPWE2,
                    OS_CPXE2,
                    IsrId,
                    HostAppId);
                /* PRQA S 3138, 3141, 0306, 1006 --*/
            }
        }
        /* PRQA S 3432, 1259, 1255, 1258 -- */
    }
    else
    {
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_PROTECTION_MEMORY, OS_TMPROT_HOOK_OSAPP);
#endif /* TRUE == CFG_PROTECTIONHOOK */
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
/* PRQA S 3006, 1505 ++*/ /* VL_Os_3006, VL_Os_1505 */
void Os_ArchSetTaskMemMap(TaskType TaskId, ApplicationType HostAppId, boolean isTrusted)
/* PRQA S 3006, 1505 --*/
{
    CoreIdType coreId;
    uint32     temp = (Os_ArchGetMemProtSet() & PPRS_MASK) >> PPRS_SET_BITS;
    uint8      OsDPRStart;
    uint8      OsCPRStart;

    coreId = Os_SCB.sysCore;

    /* Find out which register set in use  register 0 should not be used here */
    if (TASK_PPRS_SET == temp)
    {
        /*set 1: OS_DPR_1~OS_DPR_6, OS_CPR_1~OS_CPR_3*/
        OsDPRStart = 1u;
        OsCPRStart = 1u;
        /* PRQA S 3432, 1259, 1255, 1258 ++ */ /* VL_Os_3432, VL_Os_1259, VL_Os_1255, VL_Os_1258 */
        if (TRUE != isTrusted)                 /* Non-trusted app running. */
        {
            /* PRQA S 3138, 3141, 0306 ++*/ /* VL_Os_3138, VL_Os_3141, VL_Os_0306 */
            /* PRQA S 1006 ++ */            /* VL_Os_1006 */
            Os_ArchSetNonTruTskReg(
                OS_DPR_1L,
                OS_CPR_1L,
                OsDPRStart,
                OsCPRStart,
                OS_DPRE1,
                OS_DPWE1,
                OS_CPXE1,
                TaskId,
                HostAppId,
                coreId);
            /* PRQA S 1006 -- */
            /* PRQA S 3138, 3141, 0306 -- */
        }
        else /* Trusted app or os_kernel running. */
        {
            /*OSTrustedApplicationWithProtection != TRUE*/
            if (Os_AppCfg[HostAppId].OsTrustedAppWithProtection != TRUE)
            {
                /* PRQA S 3138, 0306, 1006 ++*/ /* VL_Os_3138, VL_Os_0306, VL_Os_1006 */
                Os_ArchSetTruReg(OS_DPR_1L, OS_CPR_1L, OsDPRStart, OsCPRStart, OS_DPRE1, OS_DPWE1, OS_CPXE1);
                /* PRQA S 3138, 0306, 1006 --*/
            }
            else
            {
                /* PRQA S 3138, 3141, 0306, 1006 ++*/ /* VL_Os_3138, VL_Os_3141, VL_Os_0306, VL_Os_1006 */
                Os_ArchSetTruTskProtReg(
                    OS_DPR_1L,
                    OS_CPR_1L,
                    OsDPRStart,
                    OsCPRStart,
                    OS_DPRE1,
                    OS_DPWE1,
                    OS_CPXE1,
                    TaskId,
                    HostAppId);
                /* PRQA S 3138, 3141, 0306, 1006 --*/
            }
        }
        /* PRQA S 3432, 1259, 1255, 1258 -- */
    }
    else
    {
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_PROTECTION_MEMORY, OS_TMPROT_HOOK_OSAPP);
#endif /* TRUE == CFG_PROTECTIONHOOK */
    }

    return;
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
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_SwitchTask>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_MemProtTaskCat1Map(void)
/* PRQA S 1532 -- */
{
    Os_TaskType runTaskID = Os_GetObjLocalId(Os_SCB.sysRunningTaskID);
    Os_ApplicationType runAppId = Os_ObjectAppCfg[OBJECT_TASK][runTaskID].hostApp;
    if (Os_SCB.sysAppId != runAppId)
    {
        /* Memory protection: Set memory map according to new running task. */
        Os_ArchSetMemProtSet(0x01U);
        /* PRQA S 3469 ++*/ /*  VL_Os_3469 */
        Os_ArchSetTaskMemMap(
            runTaskID,
            runAppId,
            Os_AppCfg[runAppId].OsTrusted);
        /* PRQA S 3469 --*/
    }
    else
    {
        /*SYS_APP, as OS kernel, have all access rights*/
        Os_ArchSetMemProtSet(0x00U);
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
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_SwitchTask,Os_ExitISR2>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_MemProtTaskCat2Map(void)
{
    Os_TaskType runTaskID = Os_GetObjLocalId(Os_SCB.sysRunningTaskID);
    Os_ApplicationType runAppId = Os_ObjectAppCfg[OBJECT_TASK][runTaskID].hostApp;
    if (Os_SCB.sysAppId != runAppId)
    {
        /* Memory protection: Set memory map according to new running task. */
        Os_ArchSetMemProtSet(TASK_PPRS_SET);
        /* PRQA S 3469 ++*/ /* VL_Os_3469 */
        Os_ArchSetTaskMemMap(
            runTaskID,
            runAppId,
            Os_AppCfg[runAppId].OsTrusted);
        /* PRQA S 3469 --*/
    }
    else
    {
        /*SYS_APP, as OS kernel, have all access rights*/
        Os_ArchSetMemProtSet(0x00U);
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
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_EnterISR2,Os_ExitISR2>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_MemProtIsrMap(void)
/* PRQA S 1532 -- */
{
    if (Os_SCB.sysAppId != Os_SCB.sysRunningAppID)
    {
        /*Preparing to enter the ISR2 routine defined by user with its memory region table*/
        /*New ISR, defualt set register 2*/
        Os_ArchSetMemProtSet(ISR_PPRS_SET);
        Os_ArchSetIsrMemMap(
            Os_SCB.sysRunningIsrCat2Id,
            Os_SCB.sysRunningAppID,
            Os_AppCfg[Os_SCB.sysRunningAppID].OsTrusted);
    }
    else
    {
        /*SYS_APP, as OS kernel, have all access rights*/
        Os_ArchSetMemProtSet(0x00U);
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif
/* PRQA S 6530 EOF */ /* VL_MTR_Os_STECT */
