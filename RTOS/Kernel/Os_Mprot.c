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
**  FILENAME    : Os_Mprot.c                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : AutoSar Memory Protection Managment                         **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"

/*=======[M A C R O S]========================================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This function find out the according access right of
 *                       ISR on the assigned memory area>
 * Service ID           <0x03>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <ISRID>
 *                      <Address>
 *                      <Size>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return value         <AccessType>
 * Caveats              <None>
 * Configuration        <SC3 and SC4>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(AccessType, OS_CODE) CheckISRMemoryAccess(ISRType ISRID, MemoryStartAddressType Address, MemorySizeType Size)
{
    OS_ENTER_KERNEL();
    VAR(AccessType, OS_VAR) Access = (AccessType)NO_PERMISSION;
    VAR(StatusType, OS_VAR) Status = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_CheckISRMemoryAccess);
#endif /* TRUE == CFG_TRACE_ENABLE */

    /* OS268 */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3469,3432 ++ */ /* MISRA  Dir-4.9, Rule-20.7*/ /* OS_MPROT_MACRO_002 */
    if (CHECK_ID_INVALID(ISRID, Os_CfgIsrMax_Inf))
    /* PRQA S 3469,3432 -- */ /* MISRA  Dir-4.9, Rule-20.7*/
    {
        Status = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_CHECK_TASK_MEMORY_ACCESS) != TRUE)
    {
        Status = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        Status = E_OS_DISABLEDINT;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
        Access = Os_CheckISRMemoryAccess(ISRID, Address, Size);
    }

#if (CFG_ERRORHOOK == TRUE)
    if (Status != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_CheckISRMemoryAccess(ISRID, Address, Size),
            OSServiceId_CheckISRMemoryAccess,
            Status);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_CheckISRMemoryAccess);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_MPROT_MACRO_002 */
    UNUSED_PARAMETER(Status);
    return Access;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This function find out the checked space whether has
 *                      nothing to do with stack> the checked space is one part
 *                      of the stack  or eaual to the stack or the checked space
 *                      include one whole stack,but not equeal to the stack or
 *                      the checked space cross the stack or the stack and
 *                      checked space are not mixed
 * Service ID           <Os_StackAndCheckedSpace>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <Address>
 *                      <Size>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return value         Os_StackAndCheckedSpaceStatus
 * Caveats              <None>
 * Configuration        <the address area is on the RAM>
 * CallByAPI            <Os_CheckTaskAccess>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(Os_StackAndCheckedSpaceStatus, OS_CODE) Os_StackAndCheckedSpace(
    const Os_StackType* Stack,
    uint16 StackNumber,
    MemoryStartAddressType Address,
    MemorySizeType Size)
{
    VAR(uint16, OS_VAR) i;
    /* PRQA S 3679 ++ */ /* MISRA  Rule-8.13*/ /* OS_MPROT_CONST_003 */
    VAR(Os_StackPtrType, OS_VAR) stackBottom;
    VAR(Os_StackPtrType, OS_VAR) stackTop;
    /* PRQA S 3679 -- */ /* MISRA  Rule-8.13*/ /* OS_MPROT_CONST_003 */
    VAR(Os_StackAndCheckedSpaceStatus, OS_VAR) Status = STACK_NOTMIXED_CHEKEDSPACE;

    /*  if the checked space is the stackspace*/
    for (i = 0u; i < StackNumber; i++) /* PRQA S 0771 */ /* MISRA  Rule-15.4*/ /* OS_MPROT_BREAK_004 */
    {
        /* MISRA-C: 17.4  Array subscripting applied to an object of pointer
         * type. Stack is pointor of static_cfg_array, StackNumber is the size
         * of array, it must be used like below. */
        stackBottom = Stack[i].stackBottom;
        stackTop = Stack[i].stackTop;

        /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_MPROT_TYPE_CAST_001 */
        if ((Address >= (MemoryStartAddressType)stackBottom) && (Address <= (MemoryStartAddressType)stackTop))
        {
            /*the checked space is one part of the stack*/
            /*                         <    checked space     >          */
            /*                  <            stack                  >    */
            if ((Address + Size) <= (MemoryStartAddressType)stackTop)
            {
                Status = STACK_INCLUDE_CHEKEDSPACE;
                break;
            }
            /*the checked space cross upper the stack*/
            /*                 <    checked space     >                 */
            /*                         <       stack       >            */
            else
            {
                Status = STACK_CROSS_CHEKEDSPACE;
                break;
            }
        }
        else if (Address < (MemoryStartAddressType)stackBottom)
        {
            /*the checked space include one whole stack, but not equeal to
             * the stack*/
            /*                         <    checked space     >         */
            /*                                < stack   >               */
            if ((Address + Size) > (MemoryStartAddressType)stackTop)
            {
                Status = CHEKEDSPACE_INCLUDE_STACK;
                break;
            }
            /*the checked space cross lower the stack*/
            /*                         <    checked space     >         */
            /*                      <       stack    >                  */
            else if ((Address + Size) >= (MemoryStartAddressType)stackBottom)
            {
                Status = STACK_CROSS_CHEKEDSPACE;
                break;
            }
            else
            {
                /* Intentionally Empty */
            }
        }
        else
        {
            /* Intentionally Empty */
        }
        /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */

        /*if the code step to here, the stack and checked space are not mixed            */
        /*                                          <checked space>                    */
        /*                                                                       <Stack >   */
        /*                           or                                                                            */
        /*                                          <checked space>                    */
        /*              <Stack >                                                            */
        /*exclude the conditions before ,in this condition, should return no
         * access this should be checked at last,because there are lots of
         * stacks need to be check */
        Status = STACK_NOTMIXED_CHEKEDSPACE;
    }

    return Status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This function find out the according access right of
 *                      task on the assigned memory area>
                        <called only by CheckTaskMemoryAccess>
 * Service ID           <Os_CheckTaskAccess>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <TaskID>
 *                      <Address>
 *                      <Size>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return value         AccessType
 * Caveats              <None>
 * Configuration        <memory protect is on>
 * CallByAPI            <CheckTaskMemoryAccess>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(StatusType, OS_CODE)
    Os_CheckAddrNotInStackSpace(AccessRefType Access, MemoryStartAddressType Address, MemorySizeType Size)
{
    VAR(StatusType, OS_VAR) StackSpaceErr = E_OK;
    VAR(Os_StackAndCheckedSpaceStatus, OS_VAR) StackStatus;
    /*if the checked space is the system stackspace*/
    StackStatus = Os_StackAndCheckedSpace(Os_SystemStack, CFG_SYSTEM_STACK_MAX, Address, Size);
    if (STACK_INCLUDE_CHEKEDSPACE == StackStatus)
    {
        /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9*/ /* OS_MPROT_MACRO_002 */
        *Access = OSMEMORY_SET_STACKSPACE(*Access);
        /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/
        StackSpaceErr = E_NOT_OK;
    }
    else if ((CHEKEDSPACE_INCLUDE_STACK == StackStatus) || (STACK_CROSS_CHEKEDSPACE == StackStatus))
    {
        *Access = (StatusType)OSMEMORY_SET_NOACCESS(*Access);
        StackSpaceErr = E_NOT_OK;
    }
    else
    {
        /* nothing to do */
    }
    if ((StatusType)E_OK == StackSpaceErr)
    {
        /*if the checked space is the task stackspace*/
        StackStatus = Os_StackAndCheckedSpace(Os_TaskStack, Os_SCB.sysTaskMax, Address, Size);
        if (STACK_INCLUDE_CHEKEDSPACE == StackStatus)
        {
            /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9*/ /* OS_MPROT_MACRO_002 */
            *Access = OSMEMORY_SET_STACKSPACE(*Access);
            /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/
            StackSpaceErr = E_NOT_OK;
        }
        else if ((CHEKEDSPACE_INCLUDE_STACK == StackStatus) || (STACK_CROSS_CHEKEDSPACE == StackStatus))
        {
            *Access = (StatusType)OSMEMORY_SET_NOACCESS(*Access);
            StackSpaceErr = E_NOT_OK;
        }
        else
        {
            /* nothing to do */
        }
    }
    if ((StatusType)E_OK == StackSpaceErr)
    {
/*if the checked space is the ISR stackspace*/
#if (CFG_ISR2_MAX > 0U)
        StackStatus = Os_StackAndCheckedSpace(Os_ISR2Stack, Os_CfgIsr2Max, Address, Size);
#endif /* CFG_ISR2_MAX > 0U */

        if (STACK_INCLUDE_CHEKEDSPACE == StackStatus)
        {
            /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9*/ /* OS_MPROT_MACRO_002 */
            *Access = OSMEMORY_SET_STACKSPACE(*Access);
            /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/
            StackSpaceErr = E_NOT_OK;
        }
        else if ((CHEKEDSPACE_INCLUDE_STACK == StackStatus) || (STACK_CROSS_CHEKEDSPACE == StackStatus))
        {
            *Access = (StatusType)OSMEMORY_SET_NOACCESS(*Access);
            StackSpaceErr = E_NOT_OK;
        }
        else
        {
            /*nothing to do*/
        }
    }

    return StackSpaceErr;
}

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This function find out the according access right of
 *                      task on the assigned memory area>
                        <called only by CheckISRMemoryAccess>
 * Service ID           <Os_CheckISR2Access>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <ISRID>
 *                      <Address>
 *                      <Size>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return value         AccessType
 * Caveats              <None>
 * Configuration        <memory protect is on>
 * CallByAPI            <CheckISRMemoryAccess>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(AccessType, OS_CODE) Os_CheckISR2Access(ISRType ISRID, MemoryStartAddressType Address, MemorySizeType Size)
{
    /*Indicate the ISR belong to which app*/
    VAR(uint16, OS_VAR) appsuborID;
    VAR(boolean, OS_VAR) apptrust;
    VAR(Os_CoreIdType, OS_VAR) coreId;
    VAR(Os_IsrType, OS_VAR) localIsrId;
    VAR(AccessType, OS_VAR) Access = (AccessType)NO_PERMISSION;

    /*the access area is on the ram*/
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_MPROT_TYPE_CAST_001 */
    /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9*/   /* OS_MPROT_MACRO_002 */
    if (OS_ADDRESS_IS_IN_ALL_RAM(Address, Size))
    {
        if ((StatusType)E_OK == Os_CheckAddrNotInStackSpace(&Access, Address, Size))
        {
            localIsrId = Os_GetObjLocalId(ISRID);
            /*find out which app the isr is belong under,and the app is trusted
             * or not */
            appsuborID = (uint16)Os_ObjectAppCfg[OBJECT_ISR][localIsrId].hostApp;
            apptrust = Os_AppCfg[appsuborID].OsTrusted;

            /*trusted app's ISRs and tasks have both read and write access on the
             *  whole ram*/
            if (TRUE == apptrust)
            {
                Access = OSMEMORY_SET_READABLE(Access);
                if (Os_AppCfg[appsuborID].OsTrustedAppWithProtection != TRUE)
                {
                    Access = OSMEMORY_SET_WRITEABLE(Access);
                }
                else
                {
                    /*SWS_Os_00795*/
                    /*write access only for father app's private data and the Isr's
                     * data*/
                    if ((((uint32)Os_AppPriDataAddr[appsuborID].APP_ADDR_START <= Address)
                         && ((uint32)Os_AppPriDataAddr[appsuborID].APP_ADDR_END >= (Address + Size)))
                        || (((uint32)Os_IsrDAddr[localIsrId].ISR_ADDR_START <= Address)
                            && ((uint32)Os_IsrDAddr[localIsrId].ISR_ADDR_END >= (Address + Size))))
                    {
                        Access = OSMEMORY_SET_WRITEABLE(Access);
                    }
                }
            }
            /*hand on non-trusted app's ISR*/
            /*read access for all the ram,write access only for father app's private
             *  data and the ISR's data*/
            else
            {
                coreId = Os_GetCoreLogID(Os_ArchGetCoreID());

                /*read access for all the ram, except for ohter app's data*/
                if (OS_ADDRESS_IS_IN_OTHER_APP(Address, Size, coreId, appsuborID))
                {
                    Access = OSMEMORY_SET_READABLE(Access);
                }
                /*write access only for father app's private data and the Task's
                 * data*/
                if ((((uint32)Os_AppPriDataAddr[appsuborID].APP_ADDR_START <= Address)
                     && ((uint32)Os_AppPriDataAddr[appsuborID].APP_ADDR_END >= (Address + Size)))
                    || (((uint32)Os_IsrDAddr[localIsrId].ISR_ADDR_START <= Address)
                        && ((uint32)Os_IsrDAddr[localIsrId].ISR_ADDR_END >= (Address + Size))))
                {
                    Access = OSMEMORY_SET_WRITEABLE(Access);
                }
            }
        }
    }
    /*the access area is on the flash*/
    else if (
        ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomStart <= Address)
        && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomEnd >= (Address + Size)))
    {
        /*need to access RoData area,read right*/
        if (((uint32)Os_RoData_DAddr.RoData_ADDR_START <= Address)
            && ((uint32)Os_RoData_DAddr.RoData_ADDR_END >= (Address + Size)))
        {
            Access = OSMEMORY_SET_READABLE(Access);
        }
        /*Rodata is on the right or left side of the checked space*/
        else if (
            ((uint32)Os_RoData_DAddr.RoData_ADDR_START >= (Address + Size))
            || ((uint32)Os_RoData_DAddr.RoData_ADDR_END <= Address))
        {
            Access = OSMEMORY_SET_EXECUTABLE(Access);
        }
        else /* the area cross Rodata and others,do not allow to access*/
        {
            Access = (AccessType)OSMEMORY_SET_NOACCESS(Access);
        }
    }
    else
    {
        Access = (AccessType)OSMEMORY_SET_NOACCESS(Access);
    }
    /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */

    return Access;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:CheckISRMemoryAccess>
 * Service ID           <CheckISRMemoryAccess>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <ISRID>
 *                      <Address>
 *                      <Size>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return value         <AccessType>
 * Caveats              <None>
 * Configuration        <SC3 and SC4>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(AccessType, OS_CODE) Os_CheckISRMemoryAccess(ISRType ISRID, MemoryStartAddressType Address, MemorySizeType Size)
{
    VAR(AccessType, OS_VAR) Access = (AccessType)NO_PERMISSION;
    OS_ARCH_DECLARE_CRITICAL();
    /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9*/ /* OS_MPROT_MACRO_002 */
    OS_ARCH_ENTRY_CRITICAL();

    if (Os_GetObjCoreId(ISRID) != Os_SCB.sysCore)
    {
        Access = (AccessType)OSMEMORY_SET_NOACCESS(Access);
    }
    else
    {
        /*get the access right*/
        Access = Os_CheckISR2Access(ISRID, Address, Size);
    }
    OS_ARCH_EXIT_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/
    return Access;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This function find out the according access right of
 *                      task on the assigned memory area>
 * Service ID           <0x04>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <TaskID>
 *                      <Address>
 *                      <Size>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return value         AccessType
 * Caveats              <None>
 * Configuration        <SC3 and SC4>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(AccessType, OS_CODE) CheckTaskMemoryAccess(TaskType TaskID, MemoryStartAddressType Address, MemorySizeType Size)
{
    OS_ENTER_KERNEL();
    VAR(AccessType, OS_VAR) Access = (AccessType)NO_PERMISSION;
    VAR(StatusType, OS_VAR) Status = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_CheckTaskMemoryAccess);
#endif /* TRUE == CFG_TRACE_ENABLE */

    /* OS268 */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3469,3432 ++ */ /* MISRA  Dir-4.9, Rule-20.7*/ /* OS_MPROT_MACRO_002 */
    if (CHECK_ID_INVALID(TaskID, Os_CfgTaskMax_Inf))
    /* PRQA S 3469,3432 ++ */ /* MISRA  Dir-4.9, Rule-20.7*/
    {
        Status = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_CHECK_TASK_MEMORY_ACCESS) != TRUE)
    {
        Status = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        Status = E_OS_DISABLEDINT;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
        Access = Os_CheckTaskMemoryAccess(TaskID, Address, Size);
    }

#if (CFG_ERRORHOOK == TRUE)
    if (Status != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_CheckTaskMemoryAccess(TaskID, Address, Size),
            OSServiceId_CheckTaskMemoryAccess,
            Status);
    }
#endif
    /* PRQA S 3469 ++*/ /* MISRA  Dir-4.9*/ /* OS_MPROT_MACRO_002 */
    OS_EXIT_KERNEL();
    /* PRQA S 3469 --*/ /* MISRA  Dir-4.9*/
    UNUSED_PARAMETER(Status);
    return Access;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This function find out the according access right of
 *                      task on the assigned memory area>
                        <called only by CheckTaskMemoryAccess>
 * Service ID           <Os_CheckTaskAccess>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <TaskID>
 *                      <Address>
 *                      <Size>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return value         AccessType
 * Caveats              <None>
 * Configuration        <memory protect is on>
 * CallByAPI            <CheckTaskMemoryAccess>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(AccessType, OS_CODE)
    Os_CheckTaskAccess(TaskType TaskID, MemoryStartAddressType Address, MemorySizeType Size)
{
    /*Indicate the Task belong to which app*/
    VAR(uint16, OS_VAR) appsuborID;
    VAR(boolean, OS_VAR) apptrust;
    VAR(Os_CoreIdType, OS_VAR) coreId;
    VAR(Os_TaskType, OS_VAR) localTaskId;
    VAR(AccessType, OS_VAR) Access = (AccessType)NO_PERMISSION;

    /*the access area is on the ram*/
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_MPROT_TYPE_CAST_001 */
    /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9*/   /* OS_MPROT_MACRO_002 */
    if (OS_ADDRESS_IS_IN_ALL_RAM(Address, Size))
    {
        if ((StatusType)E_OK == Os_CheckAddrNotInStackSpace(&Access, Address, Size))
        {
            /*find out which app the isr is belong under,and the app is trusted
             * or not */
            localTaskId = Os_GetObjLocalId(TaskID);
            appsuborID = (uint16)Os_ObjectAppCfg[OBJECT_TASK][localTaskId].hostApp;
            apptrust = Os_AppCfg[appsuborID].OsTrusted;

            /*trusted app's Tasks and tasks have both read and write access on the
             * whole ram*/
            if (TRUE == apptrust)
            {
                Access = OSMEMORY_SET_READABLE(Access);
                if (Os_AppCfg[appsuborID].OsTrustedAppWithProtection != TRUE)
                {
                    Access = OSMEMORY_SET_WRITEABLE(Access);
                }
                else
                {
                    /*SWS_Os_00795*/
                    /*write access only for father app's private data and the
                     * Task's data*/
                    if ((((uint32)Os_AppPriDataAddr[appsuborID].APP_ADDR_START <= Address)
                         && ((uint32)Os_AppPriDataAddr[appsuborID].APP_ADDR_END >= (Address + Size)))
                        || (((uint32)Os_TaskDAddr[localTaskId].Task_ADDR_START <= Address)
                            && ((uint32)Os_TaskDAddr[localTaskId].Task_ADDR_END >= (Address + Size))))
                    {
                        Access = OSMEMORY_SET_WRITEABLE(Access);
                    }
                }
            }
            /*hand on non-trusted app's Task*/
            else
            {
                coreId = Os_GetCoreLogID(Os_ArchGetCoreID());
                /*read access for all the ram, except for ohter app's data*/
                if (OS_ADDRESS_IS_IN_OTHER_APP(Address, Size, coreId, appsuborID))
                {
                    Access = OSMEMORY_SET_READABLE(Access);
                }
                /*write access only for father app's private data and the Task's
                 * data*/
                if ((((uint32)Os_AppPriDataAddr[appsuborID].APP_ADDR_START <= Address)
                     && ((uint32)Os_AppPriDataAddr[appsuborID].APP_ADDR_END >= (Address + Size)))
                    || (((uint32)Os_TaskDAddr[localTaskId].Task_ADDR_START <= Address)
                        && ((uint32)Os_TaskDAddr[localTaskId].Task_ADDR_END >= (Address + Size))))
                {
                    Access = OSMEMORY_SET_WRITEABLE(Access);
                }
            }
        }
    }
    /*the access area is on the flash*/
    else if (
        ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomStart <= Address)
        && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomEnd >= (Address + Size)))
    {
        /*need to access RoData area,read right*/
        if (((uint32)Os_RoData_DAddr.RoData_ADDR_START <= Address)
            && ((uint32)Os_RoData_DAddr.RoData_ADDR_END >= (Address + Size)))
        {
            Access = OSMEMORY_SET_READABLE(Access);
        }
        /*Rodata is on the right or left side of the checked space*/
        else if (
            ((uint32)Os_RoData_DAddr.RoData_ADDR_START >= (Address + Size))
            || ((uint32)Os_RoData_DAddr.RoData_ADDR_END <= Address))
        {
            Access = OSMEMORY_SET_EXECUTABLE(Access);
        }
        else /* the area cross Rodata and others,do not allow to access*/
        {
            Access = (AccessType)OSMEMORY_SET_NOACCESS(Access);
        }
    }
    else
    {
        Access = (AccessType)OSMEMORY_SET_NOACCESS(Access);
    }
    /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */

    return Access;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:CheckTaskMemoryAccess>
 * Service ID           <CheckTaskMemoryAccess>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <TaskID>
 *                      <Address>
 *                      <Size>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return value         AccessType
 * Caveats              <None>
 * Configuration        <SC3 and SC4>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(AccessType, OS_CODE) Os_CheckTaskMemoryAccess(TaskType TaskID, MemoryStartAddressType Address, MemorySizeType Size)
{
    VAR(AccessType, OS_VAR) Access = (AccessType)NO_PERMISSION;
    OS_ARCH_DECLARE_CRITICAL();
    /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9*/ /* OS_MPROT_MACRO_002 */
    OS_ARCH_ENTRY_CRITICAL();
    if (Os_GetObjCoreId(TaskID) != Os_SCB.sysCore)
    {
        Access = (AccessType)OSMEMORY_SET_NOACCESS(Access);
    }
    else
    {
        /*get the access right*/
        Access = Os_CheckTaskAccess(TaskID, Address, Size);
    }
    OS_ARCH_EXIT_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/

    return Access;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <Init function by memory protection>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <none>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/********************************************************************/
FUNC(void, OS_CODE) Os_InitMemProt(void)
{
    /* Set memory protection map for os_kernel. */
    Os_ArchInitKnMemMap();

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
