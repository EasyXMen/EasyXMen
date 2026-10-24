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
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : Os_Arch_Mpu.h                                               **
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
/*=======[I N C L U D E S]=================================================*/
#include "Os_Arch_Define.h"
#include "Os_Mprot_CfgData.h"
#include "Os_Hal_Compiler.h"
/*=======[M A C R O S]=====================================================*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_ARCH_MPU_MPAT_E(val)  (uint32)(((uint32)(val)) << 7U)
#define OS_ARCH_MPU_MPAT_RG(val) (uint32)(((uint32)(val)) << 14U)
#define OS_ARCH_MPU_MPAT_WG(val) (uint32)(((uint32)(val)) << 15U)
/* Set memory protection map for trusted app */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define Os_ArchSetTruReg() \
    do                     \
    {                      \
    } while (0)
#define Os_Hal_MemProtKernelMap() Os_ArchSetTruReg()

#define Os_Hal_MemProtEnable()                                                               \
    do                                                                                       \
    {                                                                                        \
        OS_ARCH_REG_WRITE(OS_ARCH_MPM_NUM, OS_ARCH_REG_READ(OS_ARCH_MPM_NUM) | 0x00000001U); \
    } while (0)

#define Os_Hal_MemProtDisable()                                                              \
    do                                                                                       \
    {                                                                                        \
        OS_ARCH_REG_WRITE(OS_ARCH_MPM_NUM, OS_ARCH_REG_READ(OS_ARCH_MPM_NUM) & 0xFFFFFFFEU); \
    } while (0)

#define OS_ARCH_MPU_DISABLE_REGION(region)                           \
    do                                                               \
    {                                                                \
        OS_ARCH_REG_WRITE(OS_ARCH_MPIDX_NUM, region);                \
        OS_ARCH_REG_WRITE(OS_ARCH_MPAT_NUM, OS_ARCH_MPU_MPAT_E(0U)); \
    } while (0)

#define OS_ARCH_MPU_ENABLE_REGION(region, access, start, end)                                           \
    do                                                                                                  \
    {                                                                                                   \
        if ((start) <= (end))                                                                           \
        {                                                                                               \
            OS_ARCH_REG_WRITE(OS_ARCH_MPIDX_NUM, (region));                                             \
            OS_ARCH_REG_WRITE(OS_ARCH_MPLA_NUM, (start) & 0xFFFFFFFCU);                                 \
            OS_ARCH_REG_WRITE(OS_ARCH_MPUA_NUM, (end) & 0xFFFFFFFCU);                                   \
            OS_ARCH_REG_WRITE(                                                                          \
                OS_ARCH_MPAT_NUM,                                                                       \
                OS_ARCH_MPU_MPAT_RG(1U) | OS_ARCH_MPU_MPAT_WG(1U) | OS_ARCH_MPU_MPAT_E(1U) | (access)); \
        }                                                                                               \
        else                                                                                            \
        {                                                                                               \
            OS_ARCH_MPU_DISABLE_REGION(region);                                                         \
        }                                                                                               \
    } while (0)
/* PRQA S 3472 -- */
#else
#define Os_Hal_MemProtEnable() /* PRQA S 2053 */ /*VL_Os_2053*/
#define Os_Hal_MemProtDisable()
#endif /* (TRUE == CFG_MEMORY_PROTECTION_ENABLE) */ /* PRQA S 2053 */ /*VL_Os_2053*/

/* Memory protection. */
/* PRQA S 3684 ++ */ /* VL_Os_3684 */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/* PRQA S 3684 -- */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_InitKnMemMap(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_MemProtTaskMap(Os_TaskType taskId, Os_ApplicationType appId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_MemProtIsrMap(Os_IsrType isrId, Os_ApplicationType appId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

#endif
/*=======[E N D   O F   F I L E]==============================================*/
