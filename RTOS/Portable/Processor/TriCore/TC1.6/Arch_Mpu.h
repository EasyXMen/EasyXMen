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
**  FILENAME    : Arch_Mpu.h                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : functions related to the Memory Protection Unit (MPU)       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/
#ifndef ARCH_MPU_H
#define ARCH_MPU_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_CfgData.h"

#if ((OS_NOSC == CFG_SC) || (OS_SC1 == CFG_SC) || (OS_SC2 == CFG_SC))
#define Os_CLEAR_CDC_ISR2_EPILOGUE()
#else /* OS_SC4 == CFG_SC || OS_SC3 == CFG_SC */
/* Clear Call Depth Count Enable bit. */
#define Os_CLEAR_CDC_ISR2_EPILOGUE()     \
    do                                   \
    {                                    \
        uint32 temp = 0u;                \
        temp = OS_ARCH_MFCR(OS_REG_PSW); \
        temp &= 0XFFFFFF80u;             \
        OS_ARCH_MTCR(OS_REG_PSW, temp);  \
    } while (0)
#endif /* OS_NOSC == CFG_SC || OS_SC1 == CFG_SC || OS_SC2 == CFG_SC */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/*=======[M A C R O S]========================================================*/
/* Memory protection register. CPU Data Protection Range p187*/
#define OS_DPR_0L  (0xC000U)
#define OS_DPR_0U  (0xC004U)
#define OS_DPR_1L  (0xC008U)
#define OS_DPR_1U  (0xC00CU)
#define OS_DPR_2L  (0xC010U)
#define OS_DPR_2U  (0xC014U)
#define OS_DPR_3L  (0xC018U)
#define OS_DPR_3U  (0xC01CU)
#define OS_DPR_4L  (0xC020U)
#define OS_DPR_4U  (0xC024U)
#define OS_DPR_5L  (0xC028U)
#define OS_DPR_5U  (0xC02CU)
#define OS_DPR_6L  (0xC030U)
#define OS_DPR_6U  (0xC034U)
#define OS_DPR_7L  (0xC038U)
#define OS_DPR_7U  (0xC03CU)
#define OS_DPR_8L  (0xC040U)
#define OS_DPR_8U  (0xC044U)
#define OS_DPR_9L  (0xC048U)
#define OS_DPR_9U  (0xC04CU)
#define OS_DPR_10L (0xC050U)
#define OS_DPR_10U (0xC054U)
#define OS_DPR_11L (0xC058U)
#define OS_DPR_11U (0xC05CU)
#define OS_DPR_12L (0xC060U)
#define OS_DPR_12U (0xC064U)
#define OS_DPR_13L (0xC068U)
#define OS_DPR_13U (0xC06CU)
#define OS_DPR_14L (0xC070U)
#define OS_DPR_14U (0xC074U)
#define OS_DPR_15L (0xC078U)
#define OS_DPR_15U (0xC07CU)
#define OS_DPR_16L (0xC080U)
#define OS_DPR_16U (0xC084U)
#define OS_DPR_17L (0xC088U)
#define OS_DPR_17U (0xC08CU)
#define OS_DPR_18L (0xC090U)
#define OS_DPR_18U (0xC094U)
#define OS_DPR_19L (0xC098U)
#define OS_DPR_19U (0xC09CU)
#define OS_DPR_20L (0xC0A0U)
#define OS_DPR_20U (0xC0A4U)
#define OS_DPR_21L (0xC0A8U)
#define OS_DPR_21U (0xC0ACU)
#define OS_DPR_22L (0xC0B0U)
#define OS_DPR_22U (0xC0B4U)
#define OS_DPR_23L (0xC0B8U)
#define OS_DPR_23U (0xC0BCU)
#define OS_DPR_24L (0xC0C0U)
#define OS_DPR_24U (0xC0C4U)
#define OS_DPR_25L (0xC0C8U)
#define OS_DPR_25U (0xC0CCU)
#define OS_DPR_26L (0xC0D0U)
#define OS_DPR_26U (0xC0D4U)
#define OS_DPR_27L (0xC0D8U)
#define OS_DPR_27U (0xC0DCU)
#define OS_DPR_28L (0xC0E0U)
#define OS_DPR_28U (0xC0E4U)
#define OS_DPR_29L (0xC0E8U)
#define OS_DPR_29U (0xC0ECU)
#define OS_DPR_30L (0xC0F0U)
#define OS_DPR_30U (0xC0F4U)
#define OS_DPR_31L (0xC0F8U)
#define OS_DPR_31U (0xC0FCU)

/*CPU Data Protection Read Enable p190*/
#define OS_DPRE0 (0xE010u)
#define OS_DPRE1 (0xE014u)
#define OS_DPRE2 (0xE018u)
#define OS_DPRE3 (0xE01Cu)
#define OS_DPRE4 (0xE050u)
#define OS_DPRE5 (0xE054u)

/*CPU Data Protection Write Enable p190*/
#define OS_DPWE0 (0xE020u)
#define OS_DPWE1 (0xE024u)
#define OS_DPWE2 (0xE028u)
#define OS_DPWE3 (0xE02Cu)
#define OS_DPWE4 (0xE060u)
#define OS_DPWE5 (0xE064u)

/*CPU Code Protection Range p189*/
#define OS_CPR_0L  (0xD000U)
#define OS_CPR_0U  (0xD004U)
#define OS_CPR_1L  (0xD008U)
#define OS_CPR_1U  (0xD00CU)
#define OS_CPR_2L  (0xD010U)
#define OS_CPR_2U  (0xD014U)
#define OS_CPR_3L  (0xD018U)
#define OS_CPR_3U  (0xD01CU)
#define OS_CPR_4L  (0xD020U)
#define OS_CPR_4U  (0xD024U)
#define OS_CPR_5L  (0xD028U)
#define OS_CPR_5U  (0xD02CU)
#define OS_CPR_6L  (0xD030U)
#define OS_CPR_6U  (0xD034U)
#define OS_CPR_7L  (0xD038U)
#define OS_CPR_7U  (0xD03CU)
#define OS_CPR_8L  (0xD040U)
#define OS_CPR_8U  (0xD044U)
#define OS_CPR_9L  (0xD048U)
#define OS_CPR_9U  (0xD04CU)
#define OS_CPR_10L (0xD050U)
#define OS_CPR_10U (0xD054U)
#define OS_CPR_11L (0xD058U)
#define OS_CPR_11U (0xD05CU)
#define OS_CPR_12L (0xD060U)
#define OS_CPR_12U (0xD064U)
#define OS_CPR_13L (0xD068U)
#define OS_CPR_13U (0xD06CU)
#define OS_CPR_14L (0xD070U)
#define OS_CPR_14U (0xD074U)
#define OS_CPR_15L (0xD078U)
#define OS_CPR_15U (0xD07CU)
#define OS_CPR_16L (0xD080U)
#define OS_CPR_16U (0xD084U)
#define OS_CPR_17L (0xD088U)
#define OS_CPR_17U (0xD08CU)
#define OS_CPR_18L (0xD090U)
#define OS_CPR_18U (0xD094U)
#define OS_CPR_19L (0xD098U)
#define OS_CPR_19U (0xD09CU)
#define OS_CPR_20L (0xD0A0U)
#define OS_CPR_20U (0xD0A4U)
#define OS_CPR_21L (0xD0A8U)
#define OS_CPR_21U (0xD0ACU)
#define OS_CPR_22L (0xD0B0U)
#define OS_CPR_22U (0xD0B4U)
#define OS_CPR_23L (0xD0B8U)
#define OS_CPR_23U (0xD0BCU)
#define OS_CPR_24L (0xD0C0U)
#define OS_CPR_24U (0xD0C4U)
#define OS_CPR_25L (0xD0C8U)
#define OS_CPR_25U (0xD0CCU)
#define OS_CPR_26L (0xD0D0U)
#define OS_CPR_26U (0xD0D4U)
#define OS_CPR_27L (0xD0D8U)
#define OS_CPR_27U (0xD0DCU)
#define OS_CPR_28L (0xD0E0U)
#define OS_CPR_28U (0xD0E4U)
#define OS_CPR_29L (0xD0E8U)
#define OS_CPR_29U (0xD0ECU)
#define OS_CPR_30L (0xD0F0U)
#define OS_CPR_30U (0xD0F4U)
#define OS_CPR_31L (0xD0F8U)
#define OS_CPR_31U (0xD0FCU)

/*CPU Code Protection Execute Enable p189*/
#define OS_CPXE0 (0xE000u)
#define OS_CPXE1 (0xE004u)
#define OS_CPXE2 (0xE008u)
#define OS_CPXE3 (0xE00Cu)
#define OS_CPXE4 (0xE040u)
#define OS_CPXE5 (0xE044u)

/* Set access rights for memory protection. */
#define OS_ARCH_SET_MP_ACCESS_RIGHT(rights, offset) (((uint32)rights) << (offset))

/* Clear Protection register set bits for memory protection in PSW. */
#define Os_ArchMemProtSwithToSystem()    \
    {                                    \
        uint32 temp = 0u;                \
        temp = OS_ARCH_MFCR(OS_REG_PSW); \
        temp &= 0xFFFF4FFFu;             \
        OS_ARCH_MTCR(OS_REG_PSW, temp);  \
    }

#else /* FALSE == CFG_MEMORY_PROTECTION_ENABLE */
/* Enable the enable bits for memory protection in PSW. */
#define Os_ArchMemProtSwithToSystem()
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
extern VAR(uint32, OS_VAR) Os_PSW_PRS;
extern P2CONST(OS_ISR_ADDR, AUTOMATIC, OS_VAR) Os_IsrDAddr;
extern P2CONST(OS_TASK_ADDR, AUTOMATIC, OS_VAR) Os_TaskDAddr;

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/******************************************************************************/
/*
 * Brief                <Set the memory protection properties>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <MpSetIdx>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_ArchInitKnMemMap and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchSetMemProtSet(uint32 MpSetIdx);
/******************************************************************************/
/*
 * Brief                <Gets the memory protection property>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_ArchSetIsrMemMap,Os_ArchSetTaskMemMap>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(uint32, OS_CODE) Os_ArchGetMemProtSet(void);

/******************************************************************************/
/*
 * Brief                <Enable the enable bits for memory protection in PSW and CORECON>
 *
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
extern FUNC(void, OS_CODE) Os_ArchMemProtEnable(void);
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/

#endif /* OS_MPU_H */
/*=======[E N D   O F   F I L E]==============================================*/
