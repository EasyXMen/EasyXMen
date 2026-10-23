/*******************************************************************************
**                                                                            **
** Copyright (C) iSOFT   (2023)                                               **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to iSOFT.         **
** Passing on and copying of this document, and communication                 **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME    : Os_Mcu_Timer.c                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : functions related to the processor timers                   **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R23                         **
**  Version :   AUTOSAR classic Platform R23--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/

#include "Os_Mcu_Timer.h"
#include "Os_Arch_Irq.h"
#include "Os_CfgData.h"

/*=======[M A C R O S]=======================================================*/

/*=======[E X T E R N A L   D A T A]=========================================*/

/*=======[I N T E R N A L   D A T A]=========================================*/
#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
static Os_MonitorTimerType Os_MonitorTimer[OS_AUTOSAR_CORES];
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]===================*/

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <init STM Mode>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_InitCPU>
 */
/*****************************************************************************/
FUNC(void, OS_CODE) Os_Hal_ClearTmProtTimer(Os_CoreIdType CoreId)
{
    /* SC2 fixed */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <init STM Mode>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_InitCPU>
 */
/*****************************************************************************/
FUNC(void, OS_CODE) Os_Arch_InitTmProtTimer(Os_CoreIdType CoreId)
{
    /* SC2 fixed */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Time protection get time pass, since last read>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
uint32 Os_Hal_TpGetTimePass(Os_CoreIdType CoreId)
{
    /* SC2 fixed */
    return (uint32)OS_NUM_0;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Time protection set compare register>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_TpSetCompare(Os_CoreIdType CoreId, uint32 Increment)
{
    /* SC2 fixed */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* #if (TRUE == CFG_TIMING_PROTECTION_ENABLE) */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialize the monitor timer (DWT CYCCNT free-running, no interrupt)>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <coreId> Core identifier
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Mcu_InitMonitorTimer(Os_CoreIdType coreId)
{
#if (TRUE == OS_MCU_MONITOR_TIMER_INIT_ENABLE)
    /* Order matters: DEMCR.TRCENA is the CoreSight global trace enable and has
     * to be set before CYCCNTENA, otherwise the counter enable never takes
     * effect. The DSB/ISB pair guarantees the DEMCR write has completed before
     * DWT_CTRL is written. Do not reorder. */
    OS_ARCH_REG32(OS_MCU_DEBUG_DEMCR_ADDR) |= OS_MCU_DEBUG_DEMCR_TRCENA;
    OS_ASM("DSB");
    OS_ASM("ISB");
    OS_ARCH_REG32(OS_MCU_DWT_CTRL_ADDR) |= OS_MCU_DWT_CTRL_CYCCNTENA;
    OS_ARCH_REG32(OS_MCU_DWT_CYCCNT_ADDR) = OS_NUM_0;

    Os_MonitorTimer[coreId].lastTicks = OS_NUM_0;
    Os_MonitorTimer[coreId].totalTicks = OS_NUM_0;
#else
    /* CYCCNT is already owned and running: take the current counter value as
     * baseline only. The counter is neither cleared nor enabled here, so the
     * time base becomes an offset of the owner's cycle count instead of a count
     * from OS start. */
    Os_MonitorTimer[coreId].lastTicks =
        (Os_TickType)OS_ARCH_REG32(OS_MCU_DWT_CYCCNT_ADDR);
    Os_MonitorTimer[coreId].totalTicks =
        (Os_TimerTickType)Os_MonitorTimer[coreId].lastTicks;
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the extended 64-bit hardware timer ticks>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <coreId> Core identifier
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <Os_TimerTickType> Monotonic 64-bit accumulated ticks of the monitor timer
 * PreCondition         <Os_Mcu_InitMonitorTimer has run; call interval < 32-bit CYCCNT wrap>
 * CallByAPI            <OSM monitor modules (Os_Osm_*)>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Os_TimerTickType Os_Hal_GetHwTimerTicks(Os_CoreIdType coreId)
/* PRQA S 1532 -- */
{
    Os_TimerTickType curTicks;
    uint32 tempTick;
    uint32 elapseTick;

    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();
    tempTick = OS_ARCH_REG32(OS_MCU_DWT_CYCCNT_ADDR);
    elapseTick = tempTick - Os_MonitorTimer[coreId].lastTicks;

    curTicks = Os_MonitorTimer[coreId].totalTicks + (Os_TimerTickType)elapseTick;
    Os_MonitorTimer[coreId].lastTicks = tempTick;
    Os_MonitorTimer[coreId].totalTicks = curTicks;
    OS_HAL_EXIT_CRITICAL();

    return curTicks;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
