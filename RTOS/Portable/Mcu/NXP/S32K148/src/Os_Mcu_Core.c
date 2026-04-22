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
**  FILENAME    : Os_Mcu_Core.c                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Processor Request Managment                                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]===================================================*/
#include "Os.h"
#include "Os_Arch_Processor.h"
#include "Os_Core_CfgData.h"

/*=======[V E R S I O N  C H E C K]==========================================*/

/*=======[M A C R O S]=======================================================*/

/*=======[E X T E R N A L   D A T A]=========================================*/

/*=======[I N T E R N A L   D A T A]=========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]=======*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]===================*/
/******************************************************************************/
/*
 * Brief                <Register interrupt.>
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
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static void Os_McuInitInt(void)
{
    uint16               i;
    uint16               idStartRange = (uint16)Os_IsrIdRange[0].AllIsr.IsrStart;
    uint16               idEndRange   = (uint16)Os_IsrIdRange[0].AllIsr.IsrEnd;
    for (i = idStartRange; i < idEndRange; i++)
    {
        Os_ArchInterruptInstall(Os_IsrCfg[i].IsrSrc, Os_IntInstall[i].IntPrio, Os_IntInstall[i].Isr);
    }
}
#define OS_STOP_SEC_CODE
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
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void  Os_Hal_MultiCoreInit(Os_CoreIdType sysCore)
/* PRQA S 1532 -- */
{
    UNUSED_PARAMETER(sysCore);
    Os_Isr2_Ipl_Limit = OS_NVIC_CONVERT_SET_PRIO(CFG_ISR2_IPL_MAX_CORE0);

    Os_ArchInterruptInit();
    Os_McuInitInt();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Os_Hal_GetCoreID>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <CoreId>
 * PreCondition         <None>
 * CallByAPI            <GetCoreID>
 */
/*****************************************************************************/
Os_CoreIdType Os_Hal_GetCoreID(void)
{
    /* S32K148 just have only one core */
    return (Os_CoreIdType)0;
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
void Os_Hal_StartCore(Os_CoreIdType coreId)
{
    return;
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
 */
/******************************************************************************/
Os_CoreModeType Os_Hal_GetCoreMode(Os_CoreIdType coreId)
{
    UNUSED_PARAMETER(coreId);
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
boolean Os_Hal_SetCoreMode(Os_CoreIdType coreId, Os_CoreModeType coreMode)
{
    boolean returnCode = TRUE;
    UNUSED_PARAMETER(coreId);
    UNUSED_PARAMETER(coreMode);
    return returnCode;
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
#if (CFG_TASK_MAX_CORE0 > 0U)
void Os_TaskEntry_IdleCore0(void)
{
    while (1)
    {
        IdleHook_Core0();
    }
}
#endif
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
#endif
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Os_Hal_InitCPU>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 */
/******************************************************************************/
void Os_Hal_InitCPU(void)
{
#if defined(OS_ENABLE_FPU)
    Os_ArchFPU_Init();
#endif

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
    Os_ArchInitSystemTimer(0);
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_ArchInitTimingProtTimer(0);
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Disable All interrupt But Timing Protect interrupt>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <None Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_DisableAllInt_ButTimingProtInt(Os_CoreIdType CoreId) /* PRQA S 3006 */ /* MISRA Rule 4.3 */
{
    Os_IsrType           i;
    const Os_IsrCfgType* pIsrCfg;
    uint32               isrSrc;
    Os_IsrType           idStartRange = Os_IsrIdRange[CoreId].AllIsr.IsrStart;
    Os_IsrType           idEndRange   = Os_IsrIdRange[CoreId].AllIsr.IsrEnd;
    for (i = idStartRange; i < idEndRange; i++)
    {
        pIsrCfg        = &Os_IsrCfg[i];
        isrSrc = pIsrCfg->IsrSrc;
        if (OS_ISR_SysTick_ADDR != isrSrc)
        {
            OS_INTERRUPT_DISABLE(isrSrc);
            /* PRQA S 0303, 3345, 3442, 1252*/ /* VL_Os_0303 */ /* VL_Os_3345 */ /* VL_Os_3442 */ /* VL_Os_1252 */
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Enable All interrupt But Timing Protect interrupt>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <None Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_EnableAllInt_ButTimingProtInt(Os_CoreIdType CoreId) /* PRQA S 3006 */ /* MISRA Rule 4.3 */
{
    Os_IsrType           i;
    const Os_IsrCfgType* pIsrCfg;
    uint32               isrSrc;
    Os_IsrType           idStartRange = Os_IsrIdRange[CoreId].AllIsr.IsrStart;
    Os_IsrType           idEndRange   = Os_IsrIdRange[CoreId].AllIsr.IsrEnd;

    for (i = idStartRange; i < idEndRange; i++)
    {
        pIsrCfg        = &Os_IsrCfg[i];
        isrSrc = pIsrCfg->IsrSrc;
        if (OS_ISR_SysTick_ADDR != isrSrc)
        {
            OS_INTERRUPT_ENABLE(isrSrc);
            /* PRQA S 0303, 3345, 3442, 1252*/ /* VL_Os_0303 */ /* VL_Os_3345 */ /* VL_Os_3442 */ /* VL_Os_1252 */
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
