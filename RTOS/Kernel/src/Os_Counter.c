/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
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
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : Os_Counter.c                                                **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : counter manager                                             **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"

#if (CFG_COUNTER_MAX > 0U)
/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]=========================================*/
#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
static Os_CounterType Os_CfgCounterMax; /* PRQA S 3218 */ /* VL_Os_3218 */
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Init the counter control block>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_InitCounter(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    Os_CounterType i;
    Os_CCBType*    pCCB;
    uint16         vCoreId = Os_SCB.sysCore;

    Os_CounterCfg    = Os_CounterCfg_Inf[vCoreId];
    Os_CCB           = Os_CCB_Inf[vCoreId];
    Os_CfgCounterMax = Os_CfgCounterMax_Inf[vCoreId];

    for (i = 0U; i < Os_CfgCounterMax; i++)
    {
        pCCB                    = &Os_CCB[i];
        pCCB->counterCurVal     = 0u;
        pCCB->counterLastVal    = 0u;
        pCCB->counterAlmQue     = OS_ALARM_INVALID;
        pCCB->counterStListHead = NULL_PTR;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Calculate the absolute ticks that baseTick add the
 *                       offset>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-baseTick[in]   <the begin point>
 * param-offset[in]     <the offset from begin point>
 * param-CounterID[in]  <counter id>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <Os_TickType>
 * PreCondition         <None>
 * CallByAPI            <SetRelAlarm and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
Os_TickType Os_CalcAbsTicks(Os_TickType baseTick, Os_TickType offset, Os_CounterType CounterID)
{
    Os_TickType counterTwiceMaxTicks;
    Os_TickType counterAbsTicks;

    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    counterTwiceMaxTicks = (Os_CounterCfg[CounterID].osCounterMaxAllowedValue * 2u) + 1u;
    /* PRQA S 3120 -- */

    if ((baseTick + offset) <= counterTwiceMaxTicks)
    {
        counterAbsTicks = baseTick + offset;
    }
    else
    {
        counterAbsTicks = (baseTick + offset) - (counterTwiceMaxTicks + 1u);
    }

    return counterAbsTicks;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the distance from baseTick to destTick>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-baseTick[in]   <the begin point >
 * param-destTick[in]   <the end point>
 * param-CounterID[in]  <counter id>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <Os_TickType>
 * PreCondition         <None>
 * CallByAPI            <GetAlarm and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Os_TickType Os_GetDistance(Os_TickType baseTick, Os_TickType destTick, Os_CounterType CounterID)
/* PRQA S 1532 -- */
{
    Os_TickType counterTwiceMaxTicks;
    Os_TickType counterDistanceTicks;

    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    counterTwiceMaxTicks = (Os_CounterCfg[CounterID].osCounterMaxAllowedValue * 2u) + 1u;
    /* PRQA S 3120 -- */
    if (baseTick <= destTick)
    {
        counterDistanceTicks = destTick - baseTick;
    }
    else
    {
        /* PRQA S 2911 ++ */ /* VL_Os_2911 */
        counterDistanceTicks = (destTick - baseTick) + (counterTwiceMaxTicks + 1u);
        /* PRQA S 2911 -- */
    }

    return counterDistanceTicks;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Increment Hardware counter>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-CounterID[in]  <reference to counter>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
StatusType Os_IncrementHardCounter(CounterType CounterID)
/* PRQA S 1532 -- */
{
    Os_CCBType* pCcb;
    StatusType  err = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    const Os_CounterCfgType* pCounterCfgRef;
#endif

    /* SWS_Os_00629:A COUNTER belonging to an OS-Application shall be
     * incremented by the core on which the OS-Application resides. */
    if (Os_SCB.sysCore != Os_GetObjCoreId(CounterID))
    {
        err = E_OS_CORE;
    }
    else
    {
        CounterID = Os_GetObjLocalId(CounterID); /* PRQA S 1338 */ /* VL_Os_1338 */

/* OS285 :check CounterID */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        if (CounterID >= Os_CfgCounterMax)
        {
            err = E_OS_ID;
        }
        else
        {
            /* OS285 :check configuration, if the counter is a
             * software counter, E_OS_ID shall be returned */
            pCounterCfgRef = &Os_CounterCfg[CounterID];
            if (COUNTER_SOFTWARE == pCounterCfgRef->osCounterType)
            {
                err = E_OS_ID;
            }
        }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

        if ((StatusType)E_OK == err) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
        {
            /* update the Counter value. */
            pCcb                 = &Os_CCB[CounterID];
            pCcb->counterLastVal = pCcb->counterCurVal;
            /* PRQA S 1290 ++ */ /* VL_Os_1290 */
            pCcb->counterCurVal = Os_CalcAbsTicks(pCcb->counterCurVal, 1, CounterID);
            /* PRQA S 1290 -- */

/* Alarm. */
#if (CFG_ALARM_MAX > 0)
            Os_WorkAlarm(CounterID);
#endif

/* Schedule table. */
#if (CFG_SCHEDTBL_MAX > 0U)
            Os_WorkSchedTbl(CounterID);
#endif
        }
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get current counter value>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-CounterID[in]  <reference to counter>
 * Param-Value[out]     <current value of the counter>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_GetCounterValue(CounterType CounterID, TickRefType Value) /* PRQA S 1505 */ /* VL_Os_1505 */
{
    OS_ARCH_DECLARE_CRITICAL();

#if (OS_AUTOSAR_CORES > 1)
    CounterID = Os_GetObjLocalId(CounterID); /* PRQA S 1338 */ /* VL_Os_1338 */
#endif

    /* Timer type. */
    OS_ARCH_ENTRY_CRITICAL();
    /* PRQA S 2834 ++ */ /* VL_Os_2834 */
    *Value = Os_CCB[CounterID].counterCurVal % (Os_CounterCfg[CounterID].osCounterMaxAllowedValue + 1u);
    /* PRQA S 2834 -- */
    OS_ARCH_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                    <Get elapsed counter value>
 * Service ID               <None>
 * Sync/Async               <Synchronous>
 * Reentrancy               <Reentrant>
 * param-CounterID[in]      <reference to counter>
 * Param-Value[out]         <current value of the counter>
 * Param-ElapsedValue[out]  <Elapsed value of the counter>
 * Param-Name[in/out]       <None>
 * return                   <None>
 * PreCondition             <None>
 * CallByAPI                <None>
 * REQ ID                   <None>
 */
/******************************************************************************/
StatusType Os_GetElapsedValue(CounterType CounterID, TickRefType Value, TickRefType ElapsedValue) /* PRQA S 1505 */ /* VL_Os_1505 */
{
#if (OS_AUTOSAR_CORES > 1)
    CounterID = Os_GetObjLocalId(CounterID); /* PRQA S 1338 */ /* VL_Os_1338 */
#endif

    StatusType  err = E_OK;
    Os_TickType counterCurval;
    Os_TickType counterMaxAllowedValue;
    OS_ARCH_DECLARE_CRITICAL();

    if (*Value > Os_CounterCfg[CounterID].osCounterMaxAllowedValue)
    {
        err = E_OS_VALUE;
    }
    else
    {
        counterMaxAllowedValue = Os_CounterCfg[CounterID].osCounterMaxAllowedValue;

        OS_ARCH_ENTRY_CRITICAL();
        /* PRQA S 2834 ++ */ /* VL_Os_2834 */
        counterCurval = Os_CCB[CounterID].counterCurVal % (counterMaxAllowedValue + 1u);
        *ElapsedValue = ((counterCurval + counterMaxAllowedValue) - (*Value)) % counterMaxAllowedValue;
        /* PRQA S 2834 -- */
        *Value = counterCurval;
        OS_ARCH_EXIT_CRITICAL();
    }
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Increment counter>
 * Service ID           <0x0f>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-CounterID[in]  <reference to counter>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6010, 6030, 6070 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STMIF, VL_MTR_Os_STCAL */
/* PRQA S 3006, 1532, 1503 ++ */       /* VL_Os_3006, VL_QAC_OneFunRef, VL_QAC_NoUsedApi */
StatusType IncrementCounter(CounterType CounterID)
/* PRQA S 3006, 1532, 1503 -- */
/* PRQA S 6010, 6030, 6070 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    OS_ARCH_DECLARE_CRITICAL();
    Os_CCBType* pCcb;

    StatusType err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_IncrementCounter);
#endif /* TRUE == CFG_TRACE_ENABLE */

    /* SWS_Os_00629:A COUNTER belonging to an OS-Application shall be
     * incremented by the core on which the OS-Application resides. */
    if (Os_SCB.sysCore != Os_GetObjCoreId(CounterID))
    {
        err = E_OS_CORE;
    }
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    else if (Os_GetObjLocalId(CounterID) >= Os_CfgCounterMax)
    {
        err = E_OS_ID;
    }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    else if (Os_WrongContext(OS_CONTEXT_INCREMENT_COUNTER) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_COUNTER, CounterID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    else
    {
#if (OS_AUTOSAR_CORES > 1)

        CounterID = Os_GetObjLocalId(CounterID); /* PRQA S 1338 */ /* VL_Os_1338 */

#endif

#if (OS_STATUS_EXTENDED == CFG_STATUS)
        const Os_CounterCfgType* pCounterCfgRef;
        /* SWS_Os_00285:If the input parameter <CounterID> in a call of
         * IncrementCounter() is not valid OR the counter is a hardware
         * counter,IncrementCounter() shall return E_OS_ID.  */

        pCounterCfgRef = &Os_CounterCfg[CounterID];
        if (COUNTER_HARDWARE == pCounterCfgRef->osCounterType)
        {
            err = E_OS_ID;
        }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

        if ((StatusType)E_OK == err) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
        {
            pCcb = &Os_CCB[CounterID];

            OS_ARCH_ENTRY_CRITICAL();

            pCcb->counterLastVal = pCcb->counterCurVal;
            /* PRQA S 1290 ++ */ /* VL_Os_1290 */
            pCcb->counterCurVal = Os_CalcAbsTicks(pCcb->counterCurVal, 1, CounterID);
            /* PRQA S 1290 -- */
            OS_ARCH_EXIT_CRITICAL();

/* Alarm. */
#if (CFG_ALARM_MAX > 0)
            Os_WorkAlarm(CounterID);
#endif

/* Schedule table. */
#if (CFG_SCHEDTBL_MAX > 0U)
            Os_WorkSchedTbl(CounterID);
#endif
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_IncrementCounter(CounterID), OSServiceId_IncrementCounter, err);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get current counter value>
 * Service ID           <0x10>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-CounterID[in]  <reference to counter>
 * Param-Value[out]     <current value of the counter>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6010, 6030, 6070 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STMIF, VL_MTR_Os_STCAL */
/* PRQA S 3006, 1532, 1503 ++ */ /* VL_Os_3006, VL_QAC_OneFunRef, VL_QAC_NoUsedApi */
StatusType GetCounterValue(CounterType CounterID, TickRefType Value)
/* PRQA S 3006, 1532, 1503 -- */
/* PRQA S 6010, 6030, 6070 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_GetCounterValue);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == Value)
    {
        err = E_OS_PARAM_POINTER;
    }
    /* PRQA S 3432 ++ */ /* VL_Os_3432 */
    else if (CHECK_ID_INVALID(CounterID, Os_CfgCounterMax_Inf))
    /* PRQA S 3432 -- */
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_GET_COUNTER_VALUE) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_AddressWritable((uint32)Value) != TRUE) /* PRQA S 0306 */ /* VL_Os_0306 */
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_COUNTER, CounterID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
/* Cross core use shall be supported. */
#if (OS_AUTOSAR_CORES > 1)
        Os_CoreIdType coreId = Os_GetObjCoreId(CounterID);
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync         = RPC_SYNC,
                .remoteCoreId = coreId,
                .serviceId    = OSServiceId_GetCounterValue,
                .srvPara0     = (uint32)CounterID,
                /* PRQA S 0306 ++ */ /* VL_Os_0306 */
                .srvPara1 = (uint32)Value,
                /* PRQA S 0306 -- */
                /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral  */
                .srvPara2 = (uint32)NULL_PARA,
                /* PRQA S 1258 -- */
            };
            err = Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            Os_GetCounterValue(CounterID, Value);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_GetCounterValue(CounterID, Value), OSServiceId_GetCounterValue, err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_GetCounterValue);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                    <Get elapsed counter value>
 * Service ID               <0x11>
 * Sync/Async               <Synchronous>
 * Reentrancy               <Reentrant>
 * param-CounterID[in]      <reference to counter>
 * Param-Value[out]         <current value of the counter>
 * Param-ElapsedValue[out]  <Elapsed value of the counter>
 * Param-Name[in/out]       <None>
 * return                   <StatusType>
 * PreCondition             <None>
 * CallByAPI                <None>
 * REQ ID                   <None>
 */
/******************************************************************************/
/* PRQA S 6010, 6030, 6070 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STMIF, VL_MTR_Os_STCAL */
/* PRQA S 3006, 1532, 1503 ++ */ /* VL_Os_3006, VL_QAC_OneFunRef, VL_QAC_NoUsedApi */
StatusType GetElapsedValue(CounterType CounterID, TickRefType Value, TickRefType ElapsedValue)
/* PRQA S 3006, 1532, 1503 -- */
/* PRQA S 6010, 6030, 6070 -- */
{
    /* PRQA S 2741, 2742, 2880, 3138, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2741, 2742, 2880, 3138, 3141 -- */
    StatusType err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_GetElapsedValue);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if ((NULL_PTR == Value) || (NULL_PTR == ElapsedValue))
    {
        err = E_OS_PARAM_POINTER;
    }
    /* PRQA S 3432 ++ */ /* VL_Os_3432 */
    else if (CHECK_ID_INVALID(CounterID, Os_CfgCounterMax_Inf))
    /* PRQA S 3432 -- */
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_GET_ELAPSED_COUNTER_VALUE) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    /* PRQA S 0306 ++ */ /* VL_Os_0306 */
    else if (Os_AddressWritable((uint32)Value) != TRUE)
    /* PRQA S 0306 -- */
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    /* PRQA S 0306 ++ */ /* VL_Os_0306 */
    else if (Os_AddressWritable((uint32)ElapsedValue) != TRUE)
    /* PRQA S 0306 -- */
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_COUNTER, CounterID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
/* Cross core use shall be supported. */
#if (OS_AUTOSAR_CORES > 1)
        Os_CoreIdType coreId = Os_GetObjCoreId(CounterID);
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync         = RPC_SYNC,
                .remoteCoreId = coreId,
                .serviceId    = OSServiceId_GetElapsedValue,
                .srvPara0     = (uint32)CounterID,
                /* PRQA S 0306 ++ */ /* VL_Os_0306 */
                .srvPara1 = (uint32)Value,
                .srvPara2 = (uint32)ElapsedValue,
                /* PRQA S 0306 -- */
            };
            err = Os_RpcCallService(&rpcData); /* PRQA S 0306 */ /* VL_Os_0306 */
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            err = Os_GetElapsedValue(CounterID, Value, ElapsedValue);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_GetElapsedValue(CounterID, Value, ElapsedValue),
            OSServiceId_GetElapsedValue,
            err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_GetElapsedValue);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_COUNTER_MAX > 0U */

/*=======[E N D   O F   F I L E]==============================================*/
