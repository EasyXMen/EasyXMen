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
 ************************************************************************************************************************
 **
 **  @file               : Os_Mcu_Core.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Mcu header file for Core api declarations
 **
 ***********************************************************************************************************************/

#ifndef MCU_CORE_H
#define MCU_CORE_H

/*=======[I N C L U D E S]=================================================*/
#include "Std_types.h"
#include "Os_Arch_Define.h"
#include "Os_Core_Cfg.h"
/*=======[M A C R O S]=====================================================*/
#if ((TRUE == CFG_SYSTEM_TIMER_ENABLE) || (TRUE == CFG_TIMING_PROTECTION_ENABLE))
/* PRQA S 3472 ++ */                                 /* VL_Os_3472 */
#define Os_McuSetENDINIT()   lock_safety_wdtcon()   /* Lock the safety watchdog. */
#define Os_McuResetENDINIT() unlock_safety_wdtcon() /* Unlock the safety watchdog. */
/* PRQA S 3472 -- */
/* PRQA S 2053 ++ */ /* VL_Os_2053 */
#endif               /* (TRUE == CFG_SYSTEM_TIMER_ENABLE) || (TRUE == CFG_TIMING_PROTECTION_ENABLE) */
/* PRQA S 2053 -- */
/*---------------------TC397-------------------------*/
/* Mask for OS_CORE_ID */
#define OS_CORE_ID_MSK (0x7u) /* Maximum value of core ID. */

/*
 * SCU register
 */
#define OS_SCU_PMCSR0 OS_REG32(0xF00360D4u)
#define OS_SCU_PMCSR1 OS_REG32(0xF00360D8u)
#define OS_SCU_PMCSR2 OS_REG32(0xF00360DCu)
#define OS_SCU_PMCSR3 OS_REG32(0xF00360C8u)
#define OS_SCU_PMCSR4 OS_REG32(0xF00360CCu)
#define OS_SCU_PMCSR5 OS_REG32(0xF00360D0u)

#define OS_SCU_CCUCON0 OS_REG32(0xF0036030u)
#define OS_SCU_CCUCON1 OS_REG32(0xF0036034u)

/* Csa Management */
#ifndef CFG_CSA_MAX_CORE0
#define CFG_CSA_MAX_CORE0 (256u)
#endif
#ifndef CFG_CSA_MAX_CORE1
#define CFG_CSA_MAX_CORE1 (256u)
#endif
#ifndef CFG_CSA_MAX_CORE2
#define CFG_CSA_MAX_CORE2 (256u)
#endif
#ifndef CFG_CSA_MAX_CORE3
#define CFG_CSA_MAX_CORE3 (256u)
#endif
#ifndef CFG_CSA_MAX_CORE4
#define CFG_CSA_MAX_CORE4 (256u)
#endif
#ifndef CFG_CSA_MAX_CORE5
#define CFG_CSA_MAX_CORE5 (256u)
#endif
/*
 * CSFR
 */
/* Define macros to represent memory addresses or offsets. */
/*-----------------------------start of register address----------------------------*/
#define OS_CPU0_PC OS_REG32(0xF881FE08u)
#define OS_CPU1_PC OS_REG32(0xF883FE08u)
#define OS_CPU2_PC OS_REG32(0xF885FE08u)
#define OS_CPU3_PC OS_REG32(0xF887FE08u)
#define OS_CPU4_PC OS_REG32(0xF889FE08u)
#define OS_CPU5_PC OS_REG32(0xF88DFE08u)

#define OS_CPU0_ISP OS_REG32(0xF881FE28u)
#define OS_CPU1_ISP OS_REG32(0xF883FE28u)
#define OS_CPU2_ISP OS_REG32(0xF885FE28u)
#define OS_CPU3_ISP OS_REG32(0xF887FE28u)
#define OS_CPU4_ISP OS_REG32(0xF889FE28u)
#define OS_CPU5_ISP OS_REG32(0xF88DFE28u)

#define OS_CPU0_SYSCON OS_REG32(0xF881FE14u)
#define OS_CPU1_SYSCON OS_REG32(0xF883FE14u)
#define OS_CPU2_SYSCON OS_REG32(0xF885FE14u)
#define OS_CPU3_SYSCON OS_REG32(0xF887FE14u)
#define OS_CPU4_SYSCON OS_REG32(0xF889FE14u)
#define OS_CPU5_SYSCON OS_REG32(0xF88DFE14u)

#define OS_CPU0_DBGSR_ADDR (0xF881FD00u)
#define OS_CPU1_DBGSR_ADDR (0xF883FD00u)
#define OS_CPU2_DBGSR_ADDR (0xF885FD00u)
#define OS_CPU3_DBGSR_ADDR (0xF887FD00u)
#define OS_CPU4_DBGSR_ADDR (0xF889FD00u)
#define OS_CPU5_DBGSR_ADDR (0xF88DFD00u)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* Type of the core mode. */
typedef enum
{
    OS_CORE_MODE_HALT    = 0,
    OS_CORE_MODE_RUN     = 1,
    OS_CORE_MODE_IDLE    = 2,
    OS_CORE_MODE_SLEEP   = 3,
    OS_CORE_MODE_STANDBY = 4,
    OS_CORE_MODE_UNKNOWN = 5
} Os_CoreModeType;
/*=======[E X T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
extern OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core0[CFG_CSA_MAX_CORE0];
#endif
#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
extern OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core1[CFG_CSA_MAX_CORE1];
#endif
#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
extern OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core2[CFG_CSA_MAX_CORE2];
#endif
#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
extern OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core3[CFG_CSA_MAX_CORE3];
#endif
#if (TRUE == CFG_CORE4_AUTOSAROS_ENABLE)
extern OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core4[CFG_CSA_MAX_CORE4];
#endif
#if (TRUE == CFG_CORE5_AUTOSAROS_ENABLE)
extern OS_ALIGN(64) Os_ArchCsaType Os_Csas_Core5[CFG_CSA_MAX_CORE5];
#endif

#if (CFG_ISR_MAX > 0U)
#if (TRUE == CFG_CORE0_AUTOSAROS_ENABLE)
extern uint32 Os_IsrNestPcxStackCore0[CFG_ISR_MAX_CORE0];
#endif
#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
extern uint32 Os_IsrNestPcxStackCore1[CFG_ISR_MAX_CORE1];
#endif
#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
extern uint32 Os_IsrNestPcxStackCore2[CFG_ISR_MAX_CORE2];
#endif
#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
extern uint32 Os_IsrNestPcxStackCore3[CFG_ISR_MAX_CORE3];
#endif
#if (TRUE == CFG_CORE4_AUTOSAROS_ENABLE)
extern uint32 Os_IsrNestPcxStackCore4[CFG_ISR_MAX_CORE4];
#endif
#if (TRUE == CFG_CORE5_AUTOSAROS_ENABLE)
extern uint32 Os_IsrNestPcxStackCore5[CFG_ISR_MAX_CORE5];
#endif
#endif /* CFG_ISR_MAX > 0U */
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/*Define the startup address for each core.*/
/* PRQA S 1753, 1501 ++ */ /* VL_Os_1753, VL_Os_1501 */
extern void _START1(void);
extern void _START2(void);
extern void _START3(void);
extern void _START4(void);
extern void _START5(void);
/* PRQA S 1753, 1501 -- */

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
extern void Os_Hal_MultiCoreInit(Os_CoreIdType sysCore);
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
extern void Os_Hal_StartCore(Os_CoreIdType coreId);
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
 * REQ ID               <None>
 */
/******************************************************************************/
extern Os_CoreModeType Os_Hal_GetCoreMode(Os_CoreIdType core);
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
extern boolean Os_Hal_SetCoreMode(Os_CoreIdType core, Os_CoreModeType coreMode);
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
extern void Os_Hal_CheckCPUInformation(void);

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the physical core ID.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <CoreId>
 * PreCondition         <None>
 * CallByAPI            <GetCoreID>
 * REQ ID               <None>
 */
/******************************************************************************/
static inline OS_ALWAYS_INLINE Os_CoreIdType Os_McuGetPhyCoreID(void)
{
    return ((Os_CoreIdType)OS_ARCH_MFCR(OS_REG_CUS_ID) & OS_CORE_ID_MSK);
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
static inline OS_ALWAYS_INLINE CoreIdType Os_McuGetCoreLogID(CoreIdType phyCoreId)
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
 * param-logCoreId[in]  <A valid logical Core ID from GetCoreID,Os_McuGetCoreLogID.>
 * return               <Os_CoreIdType, The return value is the unique physical
 *                       CoreID map to the input logical core.>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
static inline OS_ALWAYS_INLINE Os_CoreIdType Os_McuGetCorePhyID(Os_CoreIdType logCoreId)
{
    Os_CoreIdType phyCoreId;

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
/******************************************************************************/
/*
 * Brief                <Get the core ID.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <logical CoreId>
 * PreCondition         <None>
 * CallByAPI            <GetCoreID>
 * REQ ID               <None>
 */
/******************************************************************************/
static inline OS_ALWAYS_INLINE Os_CoreIdType Os_McuGetCoreID(void)
{
    return Os_McuGetCoreLogID(Os_McuGetPhyCoreID());
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the core ID for the kernel.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <CoreId>
 * PreCondition         <None>
 * CallByAPI            <GetCoreID>
 * REQ ID               <None>
 */
/******************************************************************************/
static inline OS_ALWAYS_INLINE Os_CoreIdType Os_Hal_GetCoreID(void)
{
#if (OS_AUTOSAR_CORES == 1)
    return (Os_CoreIdType)(0u);
#else
    return Os_McuGetCoreID();
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* MCU_CORE_H */
/*=======[E N D   O F   F I L E]==============================================*/
