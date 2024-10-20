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
 **  FILENAME    : Arch_Processor.c                                            **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : Platform dependence                                         **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLEARED_CLONE_32
#include "Os_MemMap.h"
VAR(uint32, OS_VAR)               Os_ArchTempSp;
#define OS_STOP_SEC_VAR_CLEARED_CLONE_32
#include "Os_MemMap.h"

#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
VAR(Os_TaskCBExtType, OS_VAR)   Os_TaskCBExtCore0[CFG_TASK_MAX_CORE0];
#define OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
#endif

#if (CFG_ISR_MAX > 0U)
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
VAR(uint32, OS_VAR)             Os_IsrNestPcxStackCore0[CFG_ISR_MAX_CORE0];
#define OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_VAR_CLEARED_CLONE_32
#include "Os_MemMap.h"
VAR(Os_CallLevelType, OS_VAR)   Os_SaveLevelISR1;
#define OS_STOP_SEC_VAR_CLEARED_CLONE_32
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLEARED_CLONE_32
#include "Os_MemMap.h"
VAR(volatile uint32, OS_VAR)                   Os_TprotTerminateIsr;
#define OS_STOP_SEC_VAR_CLEARED_CLONE_32
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_VAR_CLEARED_CLONE_PTR
#include "Os_MemMap.h"
P2VAR(Os_TaskCBExtType, AUTOMATIC, OS_VAR) Os_TaskCBExt;/* PRQA S 3432 */ /* MISRA Rule 20.7 */
#define OS_STOP_SEC_VAR_CLEARED_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CLONE_PTR
#include "Os_MemMap.h"
P2VAR(uint32, AUTOMATIC, OS_VAR)       Os_IsrNestPcxStack;/* PRQA S 3432 */ /* MISRA Rule 20.7 */
#define OS_STOP_SEC_VAR_CLEARED_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CLONE_PTR
#include "Os_MemMap.h"
P2VAR(uint64, AUTOMATIC, OS_VAR)            Os_ArchTopStkPtr;
#define OS_STOP_SEC_VAR_CLEARED_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
VAR(uint32, OS_VAR)       Os_Isr1NestStack[CFG_ISR_MAX_CORE0];
#define OS_STOP_SEC_VAR_CLEARED_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#if ((CFG_OSAPPLICATION_MAX > 0) && (CFG_ISR_MAX > 0) && (TRUE == CFG_INT_NEST_ENABLE))
static FUNC(StatusType, OS_CODE) Os_ArchTerminateOneNestIsr(Os_IsrType OsIsrID);
#endif

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
 * REQ ID               <DD_1_0713, DD_1_0714>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_MultiCoreInitProcessor(void)
{
    switch (Os_SCB.sysCore)
    {
        #if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
        case 0:
            Os_TaskCBExt            = Os_TaskCBExtCore0;
            Os_IsrNestPcxStack        = Os_IsrNestPcxStackCore0;
            #if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
            Os_IsrDAddr             = Os_Isr_DAddr_Core0;
            Os_TaskDAddr            = Os_Task_DAddr_Core0;
            #endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
            break;
        #endif
            
        /*add comments to pass QAC.*/
        default:
            while (1) 
            { 
                /*endless loop*/ 
            }
            break;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/******************************************************************************/
/*
 * Brief                <first entry task>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_SwitchTask>
 * REQ ID               <DD_1_0723, DD_1_0724, DD_1_1306>
 */
/******************************************************************************/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
FUNC(void, OS_CODE) Os_ArchFirstEnterTask(void)
{
    Os_ArchTopStkPtr = (uint64 *)Os_SCB.sysRunningTCB->taskTop;

    /* spp | fs | spie */
    *(--Os_ArchTopStkPtr) = 0x00000100 | 0x00002000 | 0x00000020; /* supervisor mode */

    Os_ArchTopStkPtr -= 28;
    *(Os_ArchTopStkPtr) = (uint64)(uint32)Os_TaskErrBack;    /*LP*/
    *(--Os_ArchTopStkPtr) = (uint64)(uint32)Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskEntry;    /*PC*/

    Os_TaskCBExt[Os_SCB.sysRunningTaskID] = (uint32)Os_ArchTopStkPtr;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Triggering a system call exception.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
FUNC(void, OS_CODE) Os_ArchSyscall(void)
{
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Os_ArchDispatch>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
FUNC(void, OS_CODE) Os_ArchDispatch(void)
{
    csr_set(CSR_IP, 0x2U);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <OS_GetSystemStackPointer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
FUNC(void, OS_CODE) OS_GetSystemStackPointer(void)
{
	Os_ArchTempSp = (uint32)Os_SystemStack->stackTop; 
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <OS_GetTaskStackPointer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
FUNC(void, OS_CODE) OS_GetTaskStackPointer(void)
{
    Os_ArchTempSp = (uint32)Os_TaskCBExt[Os_SCB.sysRunningTaskID];  
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <OS_SaveTaskStackPointer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
FUNC(void, OS_CODE) OS_SaveTaskStackPointer(void)
{
    Os_TaskCBExt[Os_SCB.sysRunningTaskID] = Os_ArchTempSp;   
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <OS_SaveIsr2StackPointer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
FUNC(void, OS_CODE) OS_SaveIsr2StackPointer(void)
{
    Os_IsrNestPcxStack[Os_IntNestISR2] = Os_ArchTempSp;       
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The OS starts scheduling.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 * REQ ID               <DD_1_0727, DD_1_0728>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchStartScheduler(void)
{
    Os_SwitchTask();
    OS_GetTaskStackPointer();
    Os_StartUpResumeContext();
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
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 * REQ ID               <DD_1_0732, DD_1_0733>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchInitCPU(void)
{
    Os_InterruptInit();
    Os_ArchInitIntPrio();

    #if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
    Os_ArchInitSystemTimer();
    #endif /* #if (TRUE == CFG_SYSTEM_TIMER_ENABLE) */
    
    #if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_ArchInitTimingProtTimer();
    #endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (CFG_ISR_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <TryToGetSpinlock has the same functionality as GetSpinlock with the difference
                         that if the spinlock is already occupied by a TASK on a different core the function
                         sets the OUT parameter "Success" and returns with E_OK>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <TerminateApplication>
 * REQ ID               <DD_1_1076>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchDisableIntInApp
(
    P2CONST(Os_ApplicationCfgType, AUTOMATIC, OS_VAR) posCurAppCfg
)
{   
    VAR(uint16,OS_VAR)     i;
    VAR(uint16,OS_VAR)     osIsrRefCnt;
    VAR(Os_IsrType,OS_VAR) osIsrId;
    VAR(uint32,OS_VAR)     osIsrRegVal;
    
    osIsrRefCnt = posCurAppCfg->OsAppIsrRefCnt;
    for (i = 0u; i < osIsrRefCnt; i++)
    {
        osIsrId = posCurAppCfg->OsAppObjectRef[OBJECT_ISR][i];
        osIsrRegVal  = Os_IsrCfg[Os_GetObjLocalId(osIsrId)].OsIsrSrc;
        OS_INTERRUPT_CLEAR_PENDING(osIsrRegVal);
        OS_INTERRUPT_DISABLE(osIsrRegVal);
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (TRUE == CFG_INT_NEST_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Terminate an interrupt>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <OsIsrID>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <StatusType>
 * CallByAPI            <Os_ArchAppTerminateIsrProc>
 * REQ ID               <DD_1_0901>
 */
/******************************************************************************/
static FUNC(StatusType, OS_CODE) Os_ArchTerminateOneNestIsr(Os_IsrType OsIsrID)
{
    VAR(uint8,OS_VAR) i;
    VAR(StatusType,OS_VAR) Status = E_OK;

    for (i = 0u; i < Os_IntNestISR2; i++)
    {
        /* find out the Isr nested position and reclaim its context saving area */
        if (OsIsrID == Os_SCB.sysIsrNestQueue[i])
        {
            /* Reclaim csa list used by this isr. */
            /* if OsIsrChkindex == Os_IntNestISR2, the isr is the innermost_isr,
             * can not reclaim csas here */
            break;
        }
    }

    /* Not find out the Isr Id in the IsrStack from the SCB,
        * not include outermost_isr. */
    if (i >= Os_IntNestISR2)
    {
        Status = E_NOT_OK;
    }
    else
    {
        /* Moving the following nested ISR2s to shift one position left */
        while (i < (Os_IntNestISR2 - 1u))
        {
            Os_SCB.sysIsrNestQueue[i] = Os_SCB.sysIsrNestQueue[i + 1u];

            if (i < (Os_IntNestISR2 - 2u))
            {
                Os_IsrNestPcxStack[i + 1u] = Os_IsrNestPcxStack[i + 2u];
            }

            i++;
        }
    }

    return Status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Terminate the ISR objects of the calling application>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <OsIsrID>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_TerminateISRObjectHandler>
 * REQ ID               <DD_1_1077>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchAppTerminateIsrProc(Os_IsrType OsIsrID)
{
    VAR(uint32,OS_VAR) ulRet;

    /* Isr_nest process. */
    /* Just find out the nested corresponding ISR2s(excluding the running one) */
    if (OsIsrID != Os_SCB.sysRunningIsrCat2Id)
    {
        /* Terminate one isr */
        ulRet = Os_ArchTerminateOneNestIsr(OsIsrID);
        if ((uint32)E_OK == ulRet)
        {
            if (Os_IntNestISR2 > 0u)
            {
                Os_IntNestISR2--;
            }

            if (Os_SCB.sysDispatchLocker > 0u)
            {
                /*Os_SCB.sysDispatchLocker--*/
                Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker - 1u;
            }
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif     /* TRUE == CFG_INT_NEST_ENABLE */
#endif     /* #if (CFG_ISR_MAX > 0) */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <TerminateTask arch_process in TerminateApplication>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_TerminateTaskObjectHandler>
 * REQ ID               <DD_1_1078>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchAppTerminateTaskProc(Os_TaskType OsTaskID)
{
    if ((OS_LEVEL_TASK == Os_SCB.sysOsLevel) ||
        ((OS_LEVEL_ERRORHOOK_APP == Os_SCB.sysOsLevel) && (TRUE != Os_SCB.sysInIsrCat2)))
    {
        /* If TerminateApplication is called by task or isr2 or app_error_hook,
         * the csa of running task is reclaimed in the dispatcher at the end of TerminateApplication. */
        if (OsTaskID != Os_SCB.sysRunningTaskID)
        {
            /* nothing to do. */
        }
    }
    else
    {
       /* nothing to do. */
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* #if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC)) */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Change the state of the spin lock.>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <GetSpinlock and so on>
 * REQ ID               <DD_1_1079>
 */
/******************************************************************************/
FUNC(uint32, OS_CODE) Os_CmpSwapW(uint32* address, uint32 compareVal, uint32 exchangedVal)
{
    return 0;
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
 * REQ ID               <DD_1_0763, DD_1_0764>
 */
/******************************************************************************/
FUNC(Os_CoreIdType, OS_CODE) Os_ArchGetCoreID(void)
{
    return (Os_CoreIdType)0;
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
 * REQ ID               <DD_1_0765, DD_1_0766>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchStartCore(Os_CoreIdType coreId)
{

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
 * REQ ID               <DD_1_0767, DD_1_0768>
 */
/******************************************************************************/
FUNC(Os_CoreModeType, OS_CODE) Os_GetCoreMode(Os_CoreIdType core)
{
    return OS_CORE_MODE_HALT;
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
 * REQ ID               <DD_1_0769, DD_1_0770>
 */
/******************************************************************************/
FUNC(boolean, OS_CODE) Os_SetCoreMode(Os_CoreIdType core,  Os_CoreModeType coreMode)
{
    (void)core;
    (void)coreMode;
    /* nothing to do. */
    
    return TRUE;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the CPU Mode>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-core[in]       <Selects the core which mode is set>
 * Return               <Os_CPUModeType,the Thread mode>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/******************************************************************************/
FUNC(Os_CPUModeType, OS_CODE) Os_GetCpuMode(void) /* PRQA S 3006 */ /* MISRA Rule 4.3 */
{
    return OS_CPUMODE_USER1;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Set the CPU Mode.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-core[in]       <Os_CPUModeType>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchInitCPU>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_SetCpuMode(Os_CPUModeType mode) /* PRQA S 3006 */ /* MISRA Rule 4.3 */
{

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
 * REQ ID               <DD_1_0779, DD_1_0780, DD_1_1543, DD_1_1544>
 */
/******************************************************************************/
FUNC(CoreIdType, OS_CODE) Os_GetCoreLogID(CoreIdType phyCoreId)
{
    VAR(CoreIdType,OS_VAR) logCoreId;

    switch (phyCoreId)
    {
        #if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
        case OS_CORE_ID_0:
            logCoreId = 0u;
            break;
        #endif /* CFG_CORE0_AUTOSAROS_ENABLE */

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
 * REQ ID               <DD_1_0781, DD_1_0782>
 */
/******************************************************************************/
FUNC(CoreIdType, OS_CODE) Os_GetCorePhyID(CoreIdType logCoreId)
{
    VAR(CoreIdType,OS_VAR) phyCoreId;

    switch (logCoreId)
    {
        #if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
        case 0:
            phyCoreId = OS_CORE_ID_0;
            break;
        #endif /* TRUE == CFG_CORE0_AUTOSAROS_ENABLE */

        default:
            phyCoreId = OS_CORE_INVALID;
            break;
    }

    return phyCoreId;
}
#define OS_STOP_SEC_CODE
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
 * REQ ID               <DD_1_0838, DD_1_1217, DD_1_0839>
 */
/******************************************************************************/
#define OS_START_SEC_CODE
#include "Os_MemMap.h" 
FUNC(void, OS_CODE) Os_TaskEntry_IdleCore0(void)
{
    while (1)
    {
        IdleHook_Core0();
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
