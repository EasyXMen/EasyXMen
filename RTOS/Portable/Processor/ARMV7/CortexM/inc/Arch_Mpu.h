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
#ifndef ARCH_MPU_H_
#define ARCH_MPU_H_

/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"
#include "Os_Types.h"

/*=======[M A C R O S]========================================================*/
#define CM7_SCB_CCR_ADDRESS      0xE000ED14U
#define CM7_SCB_CFSR_ADDRESS     0xE000ED28U

/* MPU register address. */
#define OS_MPU_RNR_ADDRESS          (0xE000ED98u)
#define OS_MPU_RBAR_ADDRESS         (0xE000ED9Cu)
#define OS_MPU_RASR_ADDRESS         (0xE000EDA0u)
#define MPU_CTRL_ADDRESS        (0xE000ED94U)
#define MPU_CTRL_REG            OS_REG32(MPU_CTRL_ADDRESS)
#define OS_MPU_RASR_ENABLE          ((uint32)0x1)
#define OS_MPU_RBAR_RESET_VALUE     (0x00000000u)
#define OS_MPU_RASR_RESET_VALUE     (0x00000000u)

/* Region Type */
#define OS_MPU_TYPE_DEFAULT         ((uint32)0x4u << 16) /* Shareable ,Strongly Ordered*/
#define OS_MPU_TYPE_ITCM            ((uint32)0x8u << 16) /* Non-Shareable, Normal*/
#define OS_MPU_TYPE_PFALSH          ((uint32)0xBu << 16) /* Non-Shareable, Normal, Write-Back Write-Allocate cacheable*/
#define OS_MPU_TYPE_DTCM            ((uint32)0x8u << 16) /* Non-Shareable, Normal*/
#define OS_MPU_TYPE_SRAMCACHE       ((uint32)0xBu << 16) /* Non-Shareable, Normal, Write-Back Write-Allocate cacheable*/
#define OS_MPU_TYPE_SRAMSHARE       ((uint32)0x4u << 16) /* Shareable ,Strongly Ordered*/
#define OS_MPU_TYPE_AHB             ((uint32)0x4u << 16) /* Shareable ,Strongly Ordered*/
#define OS_MPU_TYPE_DFALSH          ((uint32)0x4u << 16) /* Shareable ,Strongly Ordered*/

/* The permission of the region. */
#define OS_MPU_RACR_AP_SHIFT        (24u)
#define OS_MPU_RACR_XN_SHIFT        (28u)
#define OS_MPU_AR_RWX_RWX           ((uint32)3 << OS_MPU_RACR_AP_SHIFT)|((uint32)0 << OS_MPU_RACR_XN_SHIFT) /* Privileged?? Unprivileged RWX*/
#define OS_MPU_AR_RWX_RX            ((uint32)2 << OS_MPU_RACR_AP_SHIFT)|((uint32)0 << OS_MPU_RACR_XN_SHIFT)
#define OS_MPU_AR_NONE_NONE         ((uint32)0 << OS_MPU_RACR_AP_SHIFT)|((uint32)1 << OS_MPU_RACR_XN_SHIFT)
#define OS_MPU_AR_RX_RX             ((uint32)7 << OS_MPU_RACR_AP_SHIFT)|((uint32)0 << OS_MPU_RACR_XN_SHIFT)
#define OS_MPU_AR_RW_NONE           ((uint32)1 << OS_MPU_RACR_AP_SHIFT)|((uint32)1 << OS_MPU_RACR_XN_SHIFT)
#define OS_MPU_AR_RWX_X             ((uint32)1 << OS_MPU_RACR_AP_SHIFT)|((uint32)0 << OS_MPU_RACR_XN_SHIFT)
#define OS_MPU_AR_R_R               ((uint32)7 << OS_MPU_RACR_AP_SHIFT)|((uint32)1 << OS_MPU_RACR_XN_SHIFT)
#define OS_MPU_AR_RW_RW             ((uint32)3 << OS_MPU_RACR_AP_SHIFT)|((uint32)1 << OS_MPU_RACR_XN_SHIFT)
#define OS_MPU_AR_RW_R              ((uint32)2 << OS_MPU_RACR_AP_SHIFT)|((uint32)1 << OS_MPU_RACR_XN_SHIFT)

/* the region of mpu. */
#define MPU_REGION_0                (0U)
#define MPU_REGION_1                (1U)
#define MPU_REGION_2                (2U)
#define MPU_REGION_3                (3U)
#define MPU_REGION_4                (4U)                    
#define MPU_REGION_5                (5U)
#define MPU_REGION_6                (6U)
#define MPU_REGION_7                (7U)
#define MPU_REGION_8                (8U)
#define MPU_REGION_9                (9U)
#define MPU_REGION_10               (10U)
#define MPU_REGION_11               (11U)
#define MPU_REGION_12               (12U)
#define MPU_REGION_13               (13U)
#define MPU_REGION_14               (14U)
#define MPU_REGION_15               (15U)

/*=======[Hardfault behavior]======================================================*/
#define MEMM_FAULT_DATA_ACCESSS_ERR                     (7U)
#define MEMM_FAULT_INSTRUCTION_ACCESS_ERR               (8U)
#define NO_FAULT_BEHAVIOR                               (20U)
#define NON_MPU_RELATED_ERR                             (21U)

/* The macro below is get fault. */
#define FAULT_DATA_ACCESS        (2u)
#define FAULT_INSTRUCTION_ACCESS (1u)
#define FAULT_BIT_GET(reg, pos)  ((reg) & (pos))
/* PRQA S 3472++ */ /* MISRA Rule 4.9 */
#define OS_MPU_FAULT_IS_DATA_ACCESS(reg)        (FAULT_BIT_GET(reg, FAULT_DATA_ACCESS))
#define OS_MPU_FAULT_IS_INSTRUCTION_ACCESS(reg) (FAULT_BIT_GET(reg, FAULT_INSTRUCTION_ACCESS))
/* PRQA S 3472-- */ /* MISRA Rule 4.9 */

#define OS_MPU_CTRL_ENABLE_MASK (0x01u)
#define OS_MPU_CTRL_HFNMIENA_MASK (0x02u)
#define OS_MPU_CTRL_ENABLE (OS_MPU_CTRL_ENABLE_MASK | OS_MPU_CTRL_HFNMIENA_MASK)
#define OS_MPU_CTRL_PRIVDEFENA_MASK (0x04u)
#define OS_MPU_CTRL_DISABLE_MASK (0xFFFFFFFCu)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
typedef struct 
{
    uint32 RBAR;
    uint32 RASR;
}Os_ArchMpuRgnType,*Os_ArchMpuRgnRefType; /* fixme */
/*=======[E X T E R N A L   D A T A]==========================================*/
/* PRQA S 3684 ++ */ /* MISRA Rule 8.11 */

extern uint32 SHARED_START[];
extern uint32 SHARED_END[];

extern uint8 SRAM_START[];
extern uint8 SRAM_END[];

extern uint8 ITCM_START[];
extern uint8 ITCM_END[];

extern uint8 PFLASH_START[];
extern uint8 PFLASH_END[];

extern uint8 DTCM_START[];
extern uint8 DTCM_END[];

extern uint32 DATA_FLASH_START[];
extern uint32 DATA_FLASH_END[];

extern uint8 PERIPHERAL_START[];
extern uint8 PERIPHERAL_END[];

extern uint8 RODATA_START[];
extern uint8 RODATA_END[];

/* PRQA S 3684 -- */ /* MISRA Rule 8.11 */ /*ARCH_MPU_H_DATA_DEFINE_001*/
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/* For memory protection. */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
extern FUNC(void, OS_CODE) Os_MpuEnable(void);
extern FUNC(void, OS_CODE) Os_MpuDisable(void);
extern FUNC(void, OS_CODE) Os_ArchInitKnMemMap(void);
extern FUNC(void, OS_CODE) Os_ArchMemProtEnable(void);
extern FUNC(void, OS_CODE) Os_MpuConfigTableInit(void);
extern FUNC(void, OS_CODE) Os_ArchMpDefaultConfigSwitch(void);
extern FUNC(uint8, OS_CODE) Os_GetFault(void);
#endif
/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#endif /* ARCH_MPU_H_ */

/*=======[E N D   O F   F I L E]==============================================*/
