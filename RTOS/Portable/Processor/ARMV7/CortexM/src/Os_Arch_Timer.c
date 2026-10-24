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
**  FILENAME    : Os_Arch_Timer.c                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Timer Relation Functionality                                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r23                         **
**  Version :   AUTOSAR classic Platform r23--Function Safety                 **
**                                                                            **
*******************************************************************************/

/* Peripheral Timer Used, Not realized here, move to Os_Mcu_Timer.c for more inforamtion */

/*=======[I N C L U D E S]====================================================*/

#include "Os_Arch_Define.h"
#include "Os_Arch_Irq.h"
#include "Os_Arch_Timer.h"
#include "Os_Mcu_Config.h"
#include "Os_Core_CfgData.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Os_Arch_InitSysTimer>
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
void Os_Arch_InitSysTimer(Os_CoreIdType coreId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    /* PRQA S 4397, 0303, 3442, 1258 ++ */ /* VL_Os_4397, VL_Os_0303, VL_Os_3442, VL_Os_ConstToIntegral */
    OS_ARCH_SYSTICK_CTRL_REG &= ~(OS_ARCH_SYSTICK_ENABLE_BIT);
    /* PRQA S 4397, 0303, 3442, 1258 -- */   
    /* PRQA S 0303 ++ */ /* VL_Os_0303 */                     
    OS_ARCH_SYSTICK_RELOAD_REG = Os_SysTimerValue[coreId] - OS_NUM_1;
    OS_ARCH_SYSTICK_COUNTER_REG = OS_NUM_0;  
    /* PRQA S 0303 -- */ 
    /* PRQA S 4397, 0303, 1258 ++ */ /* VL_Os_4397, VL_Os_0303, VL_Os_ConstToIntegral */                           
    OS_ARCH_SYSTICK_CTRL_REG = OS_ARCH_SYSTICK_CLK_BIT | OS_ARCH_SYSTICK_INT_BIT;
    /* PRQA S 4397, 0303, 1258 -- */   
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <start system timer>
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
void Os_Arch_StartSysTimer(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    /* PRQA S 4397, 0303, 3442, 1258 ++ */ /* VL_Os_4397, VL_Os_0303, VL_Os_3442, VL_Os_ConstToIntegral */
    OS_ARCH_SYSTICK_CTRL_REG |= OS_ARCH_SYSTICK_ENABLE_BIT;
    /* PRQA S 4397, 0303, 3442, 1258 -- */   
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Clear system timer>
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
void Os_Hal_ClearSysTimer(Os_CoreIdType coreId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    UNUSED_PARAMETER(coreId);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_SYSTEM_TIMER_ENABLE */

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 6610 EOF */ /* VL_MTR_Os_STCDN */
