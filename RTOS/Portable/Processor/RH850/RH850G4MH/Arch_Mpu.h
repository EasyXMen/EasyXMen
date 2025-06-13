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
/*=======[I N C L U D E S]=================================================*/

#include "Os_Internal.h"
#include "Os_MprotCfg.h"
/*=======[M A C R O S]=====================================================*/
#define OS_ARCH_MPU_ISR2_SUPPORTED
/*
Bit25-Bit16: These bits indicate the ASID value to be used as the area match condition.
Bit7: E, Area n is enabled.
Bit6: G, If this bit is 0, MPATn.ASID = ASID.ASID is used as the area match condition.
         If this bit is 1, the values of MPATn.ASID and ASID.ASID are not used as the area match condition.
Bit5: Supervisor Execution
Bit4: Supervisor Write
Bit3: Supervisor Read
Bit2: User Execution
Bit1: User Write
Bit0: User Read
*/
#if defined(OS_ARCH_MPU_ISR2_SUPPORTED)
#define MPU_RDP_ACCESS_R    0x49U /* 0100_1001, Supervisor = User = R */
#define MPU_RDP_ACCESS_W    0x52U /* 0101_0010, Supervisor = User = W */
#define MPU_RDP_ACCESS_RW   0x5BU /* 0101_1011, Supervisor = User = RW */
#define MPU_RDP_ACCESS_E    0x64U /* 0110_0100, Supervisor = User = E */
#define MPU_RDP_ACCESS_N    0xCU  /* 0000_1100, Area Disable */
#define MPU_SV_NONE_UM_NONE 0x40u /* 0100_0000 SV has none permissions,UM has none*/
#else
#define MPU_RDP_ACCESS_R  0x41U /* 0100_0001, Supervisor = None, User = R */
#define MPU_RDP_ACCESS_W  0x42U /* 0100_0010, Supervisor = None, User = W */
#define MPU_RDP_ACCESS_RW 0x43U /* 0100_0011, Supervisor = None, User = RW */
#define MPU_RDP_ACCESS_E  0x44U /* 0100_0100, Supervisor = None, User = E */
#define MPU_RDP_ACCESS_N  0xCU  /* 0000_1100, Area Disable */
#endif

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define Os_ArchMemProtEnable()                                                     \
    {                                                                              \
        OS_ARCH_REG_WRITE(OS_MPM_NUM, OS_ARCH_REG_READ(OS_MPM_NUM) | 0x00000001U); \
    }
#define Os_ArchMemProtDisable()                                                    \
    {                                                                              \
        OS_ARCH_REG_WRITE(OS_MPM_NUM, OS_ARCH_REG_READ(OS_MPM_NUM) & 0xFFFFFFFEU); \
    }
#else
#define Os_ArchMemProtEnable()
#define Os_ArchMemProtDisable()
#endif /* (TRUE == CFG_MEMORY_PROTECTION_ENABLE) */

/* Memory protection. */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
extern FUNC(void, OS_CODE) Os_ArchInitKnMemMap(void);
#endif /* (TRUE == CFG_MEMORY_PROTECTION_ENABLE) */

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
extern P2CONST(OS_ISR_ADDR, AUTOMATIC, OS_VAR) Os_IsrDAddr;
extern P2CONST(OS_TASK_ADDR, AUTOMATIC, OS_VAR) Os_TaskDAddr;
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

#endif
/*=======[E N D   O F   F I L E]==============================================*/
