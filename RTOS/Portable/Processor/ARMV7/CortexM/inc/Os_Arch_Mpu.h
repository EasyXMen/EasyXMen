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
**  SPECIFICATION(S) :   AUTOSAR classic Platform r23                         **
**  Version :   AUTOSAR classic Platform R23--Function Safety                 **
**                                                                            **
*******************************************************************************/
#ifndef OS_ARCH_MPU_H
#define OS_ARCH_MPU_H
/*=======[I N C L U D E S]====================================================*/

#include "Std_Types.h"
#include "Os_Types.h"
#include "Os_Mcu_Config.h"

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#if (FALSE == OS_MCU_EXTERNAL_MPU_USED)
/*=======[M A C R O S]========================================================*/
/* ========================================
* Functional Macro Defination Local Used *
======================================== */

#define OS_ARCH_CM7_SCS_SYST_CSR_ADDRESS        0xE000E010U
#define OS_ARCH_CM7_SCS_SYST_CVR_ADDRESS        0xE000E018U
#define OS_ARCH_CM7_CPUSS_PWRCTL_ADDRESS        0x40201200U

/* MPU Register. */
/* Indicates the MPU region size. */
#define OS_ARCH_MPU_RASR_SIZE_POS               (OS_NUM_1)
#define OS_ARCH_MPU_RGN_SIZE_32B                ((uint32)(OS_NUM_4 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_64B                ((uint32)(OS_NUM_5 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_128B               ((uint32)(OS_NUM_6 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_256B               ((uint32)(OS_NUM_7 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_512B               ((uint32)(OS_NUM_8 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_1KB                ((uint32)(OS_NUM_9 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_2KB                ((uint32)(OS_NUM_10 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_4KB                ((uint32)(OS_NUM_11 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_8KB                ((uint32)(OS_NUM_12 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_16KB               ((uint32)(OS_NUM_13 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_32KB               ((uint32)(OS_NUM_14 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_64KB               ((uint32)(OS_NUM_15 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_128KB              ((uint32)(OS_NUM_16 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_256KB              ((uint32)(OS_NUM_17 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_512KB              ((uint32)(OS_NUM_18 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_1MKB               ((uint32)(OS_NUM_19 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_2MKB               ((uint32)(OS_NUM_20 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_4MKB               ((uint32)(OS_NUM_21 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_8MKB               ((uint32)(OS_NUM_22 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_16MKB              ((uint32)(OS_NUM_23 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_32MKB              ((uint32)(OS_NUM_24 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_64MKB              ((uint32)(OS_NUM_25 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_128MKB             ((uint32)(OS_NUM_26 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_256MKB             ((uint32)(OS_NUM_27 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_512MKB             ((uint32)(OS_NUM_28 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_1GB                ((uint32)(OS_NUM_29 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_2GB                ((uint32)(OS_NUM_30 << OS_ARCH_MPU_RASR_SIZE_POS))
#define OS_ARCH_MPU_RGN_SIZE_4GB                ((uint32)(OS_NUM_31 << OS_ARCH_MPU_RASR_SIZE_POS))

#define OS_ARCH_MPU_RASR_AP_POS                 (OS_NUM_16)
#define OS_ARCH_MPU_RASR_Encd_POS               (OS_NUM_16)
#define OS_ARCH_SCB_CCR_DC_POS                  (OS_NUM_16)
#define OS_ARCH_SCB_CCSIDR_NUMSETS_POS          (OS_NUM_13)
#define OS_ARCH_SCB_CCSIDR_NUMSETS_MSK          ((uint32)(0x7FFFUL << OS_ARCH_SCB_CCSIDR_NUMSETS_POS))
#define OS_ARCH_SCB_CCSIDR_ASSOCIATIVITY_POS    (OS_NUM_3)
#define OS_ARCH_SCB_CCSIDR_ASSOCIATIVITY_MSK    ((uint32)(0x3FFUL << OS_ARCH_SCB_CCSIDR_ASSOCIATIVITY_POS))

/* SCB D-Cache Invalidate by Set-way Register Definitions */
#define OS_ARCH_SCB_DCISW_WAY_POS               (OS_NUM_30)
#define OS_ARCH_SCB_DCISW_WAY_MSK               ((uint32)(OS_3BIT_MASK << OS_ARCH_SCB_DCISW_WAY_POS))

#define OS_ARCH_SCB_DCISW_SET_POS               (OS_NUM_5)
#define OS_ARCH_SCB_DCISW_SET_MSK               ((uint32)(0x1FFUL << OS_ARCH_SCB_DCISW_SET_POS))

/* SCB D-Cache Clean and Invalidate by Set-way Register Definitions */
#define OS_ARCH_SCB_DCCISW_WAY_POS              (OS_NUM_30)
#define OS_ARCH_SCB_DCCISW_WAY_MSK              ((uint32)(OS_3BIT_MASK << OS_ARCH_SCB_DCCISW_WAY_POS))

#define OS_ARCH_SCB_DCCISW_SET_POS              (OS_NUM_5)
#define OS_ARCH_SCB_DCCISW_SET_MSK              ((uint32)(0x1FFUL << OS_ARCH_SCB_DCCISW_SET_POS))

/* MPU register address. */
#define OS_ARCH_MPU_TYPE_ADDRESS                (0xE000ED90U)
#define OS_ARCH_MPU_CTRL_ADDRESS                (0xE000ED94U)
#define OS_ARCH_MPU_RNR_ADDRESS                 (0xE000ED98U)
#define OS_ARCH_MPU_RBAR_ADDRESS                (0xE000ED9CU)
#define OS_ARCH_MPU_RASR_ADDRESS                (0xE000EDA0U)
#define OS_ARCH_MPU_RBAR_A1_ADDRESS             (0xE000EDA4U)
#define OS_ARCH_MPU_RASR_A1_ADDRESS             (0xE000EDA8U)
#define OS_ARCH_MPU_RBAR_A2_ADDRESS             (0xE000EDACU)
#define OS_ARCH_MPU_RASR_A2_ADDRESS             (0xE000EDB0U)
#define OS_ARCH_MPU_RBAR_A3_ADDRESS             (0xE000EDB4U)
#define OS_ARCH_MPU_RASR_A3_ADDRESS             (0xE000EDB8U)

#define OS_ARCH_CM7_SCB_ICIALLU_ADDRESS         (0xE000EF50U)
#define OS_ARCH_CM7_SCB_CSSELR_ADDRESS          (0xE000ED84U)
#define OS_ARCH_CM7_SCB_CCSIDR_ADDRESS          (0xE000ED80U)
#define OS_ARCH_CM7_SCB_DCISW_ADDRESS           (0xE000EF60U)
#define OS_ARCH_CM7_SCB_DCCISW_ADDRESS          (0xE000EF74U)

#define OS_ARCH_MPU_TYPE_REG                    OS_ARCH_REG32(OS_ARCH_MPU_TYPE_ADDRESS)
#define OS_ARCH_MPU_CTRL_REG                    OS_ARCH_REG32(OS_ARCH_MPU_CTRL_ADDRESS)
#define OS_ARCH_MPU_RNR_REG                     OS_ARCH_REG32(OS_ARCH_MPU_RNR_ADDRESS)
#define OS_ARCH_MPU_RBAR_REG                    OS_ARCH_REG32(OS_ARCH_MPU_RBAR_ADDRESS)
#define OS_ARCH_MPU_RASR_REG                    OS_ARCH_REG32(OS_ARCH_MPU_RASR_ADDRESS)
#define OS_ARCH_MPU_RBAR_A1_REG                 OS_ARCH_REG32(OS_ARCH_MPU_RBAR_A1_ADDRESS)
#define OS_ARCH_MPU_RASR_A1_REG                 OS_ARCH_REG32(OS_ARCH_MPU_RASR_A1_ADDRESS)
#define OS_ARCH_MPU_RBAR_A2_REG                 OS_ARCH_REG32(OS_ARCH_MPU_RBAR_A2_ADDRESS)
#define OS_ARCH_MPU_RASR_A2_REG                 OS_ARCH_REG32(OS_ARCH_MPU_RASR_A2_ADDRESS)
#define OS_ARCH_MPU_RBAR_A3_REG                 OS_ARCH_REG32(OS_ARCH_MPU_RBAR_A3_ADDRESS)
#define OS_ARCH_MPU_RASR_A3_REG                 OS_ARCH_REG32(OS_ARCH_MPU_RASR_A3_ADDRESS)

#define OS_ARCH_CM7_SCB_ICIALLU_REG             OS_ARCH_REG32(OS_ARCH_CM7_SCB_ICIALLU_ADDRESS)
#define OS_ARCH_CM7_SCB_CSSELR_REG              OS_ARCH_REG32(OS_ARCH_CM7_SCB_CSSELR_ADDRESS)
#define OS_ARCH_CM7_SCB_CCSIDR_REG              OS_ARCH_REG32(OS_ARCH_CM7_SCB_CCSIDR_ADDRESS)
#define OS_ARCH_CM7_SCB_DCISW_REG               OS_ARCH_REG32(OS_ARCH_CM7_SCB_DCISW_ADDRESS)
#define OS_ARCH_CM7_SCB_DCCISW_REG              OS_ARCH_REG32(OS_ARCH_CM7_SCB_DCCISW_ADDRESS)

#define OS_ARCH_MPU_CTRL_ENABLE_MASK            ((uint32)(OS_1BIT_MASK))
#define OS_ARCH_MPU_CTRL_HFNMIENA_MASK          ((uint32)(OS_1BIT_MASK << OS_NUM_1))
#define OS_ARCH_MPU_CTRL_PRIVDEFENA_MASK        ((uint32)(OS_1BIT_MASK << OS_NUM_2))
#define OS_ARCH_MPU_CTRL_ENABLE                 (OS_ARCH_MPU_CTRL_ENABLE_MASK | OS_ARCH_MPU_CTRL_HFNMIENA_MASK)
#define OS_ARCH_MPU_CTRL_DISABLE_MASK           (~OS_ARCH_MPU_CTRL_ENABLE_MASK)

#define OS_ARCH_MPU_RASR_ENABLE_MASK            ((uint32)(OS_1BIT_MASK))

/* The permission of the region. */
#define OS_ARCH_MPU_PRIV_NA_USER_NA_EXEC    \
    (0x0000UL << OS_ARCH_MPU_RASR_AP_POS) /* Alias no access in privileged mode, no access in user mode and execute */
#define OS_ARCH_MPU_PRIV_RW_USER_NA_EXEC    \
    (0x0100UL << OS_ARCH_MPU_RASR_AP_POS) /* Alias no read/write in privileged mode, no access in user mode and execute */
#define OS_ARCH_MPU_PRIV_RW_USER_RO_EXEC    \
    (0x0200UL << OS_ARCH_MPU_RASR_AP_POS) /* Alias no read/write in privileged mode, read only in user mode and execute */
#define OS_ARCH_MPU_PRIV_RW_USER_RW_EXEC    \
    (0x0300UL << OS_ARCH_MPU_RASR_AP_POS) /* Alias no read/write in privileged mode, read/write in user mode and execute */
#define OS_ARCH_MPU_PRIV_RO_USER_NA_EXEC    \
    (0x0500UL << OS_ARCH_MPU_RASR_AP_POS) /* Alias no read only in privileged mode, no access in user mode and execute */
#define OS_ARCH_MPU_PRIV_RO_USER_RO_EXEC    \
    (0x0600UL << OS_ARCH_MPU_RASR_AP_POS) /* Alias no read only in privileged mode, read only in user mode and execute */
#define OS_ARCH_MPU_PRIV_NA_USER_NA_NOEXEC  \
    (0x1000UL << OS_ARCH_MPU_RASR_AP_POS) /* Alias no access in privileged mode, no access in user mode and no execution */
#define OS_ARCH_MPU_PRIV_RW_USER_NA_NOEXEC  \
    (0x1100UL << OS_ARCH_MPU_RASR_AP_POS) /* Alias no read/write in privileged mode, no access in user mode and no execution */
#define OS_ARCH_MPU_PRIV_RW_USER_RO_NOEXEC  \
    (0x1200UL << OS_ARCH_MPU_RASR_AP_POS) /* Alias no read/write in privileged mode, read only in user mode and no execution */
#define OS_ARCH_MPU_PRIV_RW_USER_RW_NOEXEC  \
    (0x1300UL << OS_ARCH_MPU_RASR_AP_POS) /* Alias no read/write in privileged mode, read/write in user mode and no execution */
#define OS_ARCH_MPU_PRIV_RO_USER_NA_NOEXEC  \
    (0x1500UL << OS_ARCH_MPU_RASR_AP_POS) /* Alias no read only in privileged mode, no access in user mode and no execution */
#define OS_ARCH_MPU_PRIV_RO_USER_RO_NOEXEC  \
    (0x1600UL << OS_ARCH_MPU_RASR_AP_POS) /* Alias no read only in privileged mode, read only in user mode and no execution */

#define OS_ARCH_MPU_REGION_ENABLE               ((uint32)OS_1BIT_MASK)
#define OS_ARCH_MPU_REGION_DISABLE              (~OS_ARCH_MPU_REGION_ENABLE)
#define OS_ARCH_MPU_STRONGLYORDERED_SHAREABLE   (OS_NUM_0 << OS_ARCH_MPU_RASR_Encd_POS)
#define OS_ARCH_MPU_DEVICE_SHAREABLE            (OS_1BIT_MASK << OS_ARCH_MPU_RASR_Encd_POS) /**< Memory type device and sharable */
#define OS_ARCH_MPU_NORMAL_OIWTNOWA_NONSHARED   \
    (0x0002UL << OS_ARCH_MPU_RASR_Encd_POS) /* Memory type normal outer and inner write-through, no write allocate and non shared */
#define OS_ARCH_MPU_NORMAL_OIWBNOWA_NONSHARED   \
    (0x0003UL << OS_ARCH_MPU_RASR_Encd_POS) /* Memory type normal outer and inner write-back, no write allocate and non shared */
#define OS_ARCH_MPU_NORMAL_OIWTNOWA_SHARED      \
    (0x0006UL << OS_ARCH_MPU_RASR_Encd_POS) /* Memory type normal outer and inner write-through, no write allocate and shared */
#define OS_ARCH_MPU_NORMAL_OIWBNOWA_SHARED      \
    (0x0007UL << OS_ARCH_MPU_RASR_Encd_POS) /* Memory type normal outer and inner write-back, no write allocate and shared  */
#define OS_ARCH_MPU_NORMAL_OINC_NONSHARED       \
    (0x0008UL << OS_ARCH_MPU_RASR_Encd_POS) /* Memory type normal outer and inner non-cachable and non shared */
#define OS_ARCH_MPU_NORMAL_OIWBWA_NONSHARED     \
    (0x000BUL << OS_ARCH_MPU_RASR_Encd_POS) /* Memory type normal outer and inner write-back, write allocate and non shared */
#define OS_ARCH_MPU_NORMAL_OINC_SHARED          \
    (0x000CUL << OS_ARCH_MPU_RASR_Encd_POS) /* Memory type normal outer and inner non-cachable and shared */
#define OS_ARCH_MPU_NORMAL_OIWBWA_SHARED        \
    (0x000FUL << OS_ARCH_MPU_RASR_Encd_POS) /* Memory type normal outer and inner write-back, write allocate and shared */
#define OS_ARCH_MPU_DEVICE_NONSHAREABLE         \
    (0x0010UL << OS_ARCH_MPU_RASR_Encd_POS) /*  Memory type device and non sharable */

/* the region of mpu. */
#define OS_ARCH_MPU_REGION_0                    (OS_NUM_0)
#define OS_ARCH_MPU_REGION_1                    (OS_NUM_1)
#define OS_ARCH_MPU_REGION_2                    (OS_NUM_2)
#define OS_ARCH_MPU_REGION_3                    (OS_NUM_3)
#define OS_ARCH_MPU_REGION_4                    (OS_NUM_4)
#define OS_ARCH_MPU_REGION_5                    (OS_NUM_5)
#define OS_ARCH_MPU_REGION_6                    (OS_NUM_6)
#define OS_ARCH_MPU_REGION_7                    (OS_NUM_7)

/* MPU region number definitions.*/
#define OS_ARCH_MPU_REGION_NUM_ALL_SRAM_SPACE   (OS_ARCH_MPU_REGION_0)
#define OS_ARCH_MPU_REGION_NUM_ALL_APP_DATA     (OS_ARCH_MPU_REGION_1)
#define OS_ARCH_MPU_REGION_NUM_CUR_APP_PRI_DATA (OS_ARCH_MPU_REGION_2)
#define OS_ARCH_MPU_REGION_NUM_CUR_TASK_DATA    (OS_ARCH_MPU_REGION_3)
#define OS_ARCH_MPU_REGION_NUM_CUR_ISR_DATA     (OS_ARCH_MPU_REGION_3)
#define OS_ARCH_MPU_REGION_NUM_SHARED_SPACE     (OS_ARCH_MPU_REGION_4)
#define OS_ARCH_MPU_REGION_NUM_ALL_PERI_SPACE   (OS_ARCH_MPU_REGION_5)
#define OS_ARCH_MPU_REGION_NUM_ALL_PFLASH_SPACE (OS_ARCH_MPU_REGION_6)
/* The configuration of the following two areas can be added according to the hardware environment */
#define OS_ARCH_MPU_REGION_NUM_ALL_APP_CODE     (OS_NUM_7)
#define OS_ARCH_MPU_REGION_NUM_CUR_APP_CODE     (OS_NUM_8)

#define OS_ARCH_MPU_NUM_OF_REGION_8             (OS_NUM_8)
#define OS_ARCH_MPU_NUM_OF_REGION_16            (OS_NUM_16)

/*=======[Hardfault behavior]======================================================*/

#define PERIPHERAL_START __PERIPHERAL_START
#define PERIPHERAL_END   __PERIPHERAL_END

#define PFLASH_START __CODE_START
#define PFLASH_END   __CODE_END

#define SHARED_START __SHARED_START
#define SHARED_END   __SHARED_END

#define SRAM_START __SRAM_START
#define SRAM_END   __SRAM_END

/* ========================================
* Functional Macro Provided to Kernel     *
======================================== */

#define Os_Hal_MemProtTaskMap(taskId, appId)     Os_Arch_MemProtMap(taskId, appId, OS_LEVEL_TASK)
#define Os_Hal_MemProtIsrMap(isrId, appId)       Os_Arch_MemProtMap(isrId, appId, OS_LEVEL_ISR2)
#define Os_Hal_MemProtKernelMap()                Os_Arch_MpuDisable()

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/* This enum define the memory protection properties type. */
/*PRQA S 1271 ++ */ /* VL_Os_1271 */
typedef enum
{
    OS_MP_SET_INIT                  = 0u,
    OS_MP_SET_TASK_NON_TRUST        = 1u,
    OS_MP_SET_ISR_NON_TRUST         = 2u,
    OS_MP_SET_TASK_TRUST_WITH_PROT  = 3u,
    OS_MP_SET_ISR_TRUST_WITH_PROT   = 4u,
    OS_MP_SET_TRUST_WITHOUT_PROT    = 5u
} Os_Arch_MpSetType;
/*PRQA S 1271 -- */

typedef struct
{
    uint8* pSRamStart; /* SRAM */
    uint8* pSRamEnd;

    uint8* pPeripheralStart; /* PERIPHERAL */
    uint8* pPeripheralEnd;

    uint8* pRomStart; /* ROM */
    uint8* pRomEnd;

    uint8* pDtcmStart; /* DTCM */
    uint8* pDtcmEnd;
} Os_Arch_MemProtKnAddr;

typedef struct
{
    const Os_Arch_MemProtKnAddr OsKernelAddr;
} Os_Arch_MemProtKnCfgType;

typedef struct
{
    uint8  u8RegionNum;  /*!< Region number                    */
    uint32 u32StartAddr; /*!< Memory region start address      */
    uint32 u32EndAddr;   /*!< Memory region end address        */
    uint32 size;
    uint32 u32access; /*!< Access permission for region     */
} Os_Arch_Mpu_ConfigType;

#if (OS_MPROT_REGION_CFG_ENABLE == TRUE)
typedef struct
{
    uint32 (*RegionRbarPtr)[OS_MCU_CFG_REGION_NUM];
    uint32 (*RegionRasrPtr)[OS_MCU_CFG_REGION_NUM];
} Os_MpuRegionType;
#else
typedef struct
{
    uint32 (*RegionRbarPtr)[OS_MCU_MPU_REGION_USEMAX_NUM];
    uint32 (*RegionRasrPtr)[OS_MCU_MPU_REGION_USEMAX_NUM];
} Os_MpuRegionType;
#endif /* OS_MPROT_REGION_CFG_ENABLE == TRUE */

/*=======[E X T E R N A L   D A T A]==========================================*/

/* PRQA S 3684 ++ */ /* VL_Os_3684 */
extern uint8 __PERIPHERAL_START[];
extern uint8 __PERIPHERAL_END[];

extern uint32 __CODE_START[];
extern uint32 __CODE_END[];

extern uint32 __SHARED_START[];
extern uint32 __SHARED_END[];

extern uint8 __SRAM_START[];
extern uint8 __SRAM_END[];
/* PRQA S 3684 -- */ /*ARCH_MPU_H_DATA_DEFINE_001*/
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
extern Os_MpuRegionType Os_TaskMpuRegion;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
extern Os_MpuRegionType Os_Isr2MpuRegion;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/* =====================
* Function Local Used  *
===================== */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void  Os_Arch_DisableMpuRegion(uint8 region);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void  Os_Arch_MpuDisable(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void  Os_Arch_MpuConfigTableInit(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void  Os_Arch_MpuDefaultConfigSwitch(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void  Os_Arch_MemProtMap(uint16 Id, Os_ApplicationType appId, Os_CallLevelType Level);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/* ============================
* Function Provided to Kernel *
============================ */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_InitKnMemMap(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_MemProtEnable(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* FALSE == OS_MCU_EXTERNAL_MPU_USED */
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */
#endif /* OS_ARCH_MPU_H */
/*=======[E N D   O F   F I L E]==============================================*/
