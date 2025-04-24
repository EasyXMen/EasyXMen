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
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Mcu_Core.h                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Core Request Managment                                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

#ifndef MCU_CORE_H
#define MCU_CORE_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_Types.h"

/*=======[M A C R O S]=====================================================*/
/* Mask for OS_CORE_ID */
#define OS_CORE_ID_MSK (0x7u) /* Maximum value of core ID. */
#define OS_REG_CUS_ID  (0xFE50U)

/* Csa Management */
#define CFG_CSA_MAX_CORE0  128U
#define CFG_CSA_MAX_CORE1  128U
#define CFG_CSA_MAX_CORE2  128U
#define CFG_CSA_MAX_CORE3  128U
#define CFG_CSA_MAX_CORE4  128U
#define CFG_CSA_MAX_CORE5  128U

#define OS_CPU0_PC         OS_REG32(0xF881FE08u)
#define OS_CPU1_PC         OS_REG32(0xF883FE08u)
#define OS_CPU2_PC         OS_REG32(0xF885FE08u)
#define OS_CPU3_PC         OS_REG32(0xF887FE08u)
#define OS_CPU4_PC         OS_REG32(0xF889FE08u)
#define OS_CPU5_PC         OS_REG32(0xF88DFE08u)

#define OS_CPU0_ISP        OS_REG32(0xF881FE28u)
#define OS_CPU1_ISP        OS_REG32(0xF883FE28u)
#define OS_CPU2_ISP        OS_REG32(0xF885FE28u)
#define OS_CPU3_ISP        OS_REG32(0xF887FE28u)
#define OS_CPU4_ISP        OS_REG32(0xF889FE28u)
#define OS_CPU5_ISP        OS_REG32(0xF88DFE28u)

#define OS_CPU0_SYSCON     OS_REG32(0xF881FE14u)
#define OS_CPU1_SYSCON     OS_REG32(0xF883FE14u)
#define OS_CPU2_SYSCON     OS_REG32(0xF885FE14u)
#define OS_CPU3_SYSCON     OS_REG32(0xF887FE14u)
#define OS_CPU4_SYSCON     OS_REG32(0xF889FE14u)
#define OS_CPU5_SYSCON     OS_REG32(0xF88DFE14u)

#define OS_CPU0_DBGSR_ADDR (0xF881FD00u)
#define OS_CPU1_DBGSR_ADDR (0xF883FD00u)
#define OS_CPU2_DBGSR_ADDR (0xF885FD00u)
#define OS_CPU3_DBGSR_ADDR (0xF887FD00u)
#define OS_CPU4_DBGSR_ADDR (0xF889FD00u)
#define OS_CPU5_DBGSR_ADDR (0xF88DFD00u)

/*
 * SCU register
 */
#define OS_SCU_PMCSR0              OS_REG32(0xF00360D4u)
#define OS_SCU_PMCSR1              OS_REG32(0xF00360D8u)
#define OS_SCU_PMCSR2              OS_REG32(0xF00360DCu)
#define OS_SCU_PMCSR3              OS_REG32(0xF00360C8u)
#define OS_SCU_PMCSR4              OS_REG32(0xF00360CCu)
#define OS_SCU_PMCSR5              OS_REG32(0xF00360D0u)

#define PERIPHERAL_VALID_ADDRESS   (0xF8030000u)
#define PERIPHERAL_INVALID_ADDRESS (0xF802A000u)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* Type of the core mode. */
typedef enum
{
    OS_CORE_MODE_HALT = 0U,
    OS_CORE_MODE_RUN = 1U,
    OS_CORE_MODE_IDLE = 2U,
    OS_CORE_MODE_SLEEP = 3U,
    OS_CORE_MODE_STANDBY = 4U,
    OS_CORE_MODE_UNKNOWN = 5U
} Os_CoreModeType;

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*Define the startup address for each core.*/
extern void _START1(void);
extern void _START2(void);
extern void _START3(void);
extern void _START4(void);
extern void _START5(void);

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
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(Os_CoreModeType, OS_CODE) Os_GetCoreMode(Os_CoreIdType core);

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
extern FUNC(void, OS_CODE) Os_ArchStartCore(Os_CoreIdType coreId);

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
extern FUNC(boolean, OS_CODE) Os_SetCoreMode(Os_CoreIdType core, Os_CoreModeType coreMode);

/******************************************************************************/
/*
 * Brief                <Get the core ID.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <CoreId>
 * PreCondition         <None>
 * CallByAPI            <GetCoreID>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(Os_CoreIdType, OS_CODE) Os_ArchGetCoreID(void);

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

extern FUNC(void, OS_CODE) Arch_CheckCPUInformation(void);
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
extern FUNC(void, OS_CODE) Os_TaskEntry_IdleCore0(void);
#endif
#if (CFG_TASK_MAX_CORE1 > 0U)
extern FUNC(void, OS_CODE) Os_TaskEntry_IdleCore1(void);
#endif
#if (CFG_TASK_MAX_CORE2 > 0U)
extern FUNC(void, OS_CODE) Os_TaskEntry_IdleCore2(void);
#endif
#if (CFG_TASK_MAX_CORE3 > 0U)
extern FUNC(void, OS_CODE) Os_TaskEntry_IdleCore3(void);
#endif
#if (CFG_TASK_MAX_CORE4 > 0U)
extern FUNC(void, OS_CODE) Os_TaskEntry_IdleCore4(void);
#endif
#if (CFG_TASK_MAX_CORE5 > 0U)
extern FUNC(void, OS_CODE) Os_TaskEntry_IdleCore5(void);
#endif
/* PRQA S 3449,3451 --*/ /* MISRA Rule 8.5 */

#if (CFG_TASK_MAX_CORE0 > 0U)
extern FUNC(void, OS_APPL_CODE) IdleHook_Core0(void);
#endif
#if (CFG_TASK_MAX_CORE1 > 0U)
extern FUNC(void, OS_APPL_CODE) IdleHook_Core1(void);
#endif
#if (CFG_TASK_MAX_CORE2 > 0U)
extern FUNC(void, OS_APPL_CODE) IdleHook_Core2(void);
#endif

#if (CFG_TASK_MAX_CORE3 > 0U)
extern FUNC(void, OS_APPL_CODE) IdleHook_Core3(void);
#endif
#if (CFG_TASK_MAX_CORE4 > 0U)
extern FUNC(void, OS_APPL_CODE) IdleHook_Core4(void);
#endif
#if (CFG_TASK_MAX_CORE5 > 0U)
extern FUNC(void, OS_APPL_CODE) IdleHook_Core5(void);
#endif

#endif /* MCU_CORE_H */
/*=======[E N D   O F   F I L E]==============================================*/
