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
#define CM7_SCS_SYST_CSR_ADDRESS 0xE000E010U
#define CM7_SCS_SYST_CVR_ADDRESS 0xE000E018U
#define CM7_SCB_CCR_ADDRESS      0xE000ED14U
#define CM7_SCB_CFSR_ADDRESS     0xE000ED28U
#define CM7_CPUSS_PWRCTL_ADDRESS 0x40201200U
/* MPU Register. */
/* Indicates the MPU region size. */
#define MPU_RASR_SIZE_POS            (0x01U)
#define MPU_RGN_SIZE_32B             ((uint32)0x04U << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_64B             ((uint32)0x05U << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_128B            ((uint32)0x06U << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_256B            ((uint32)0x07U << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_512B            ((uint32)0x08U << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_1KB             ((uint32)0x09U << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_2KB             ((uint32)0x0AU << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_4KB             ((uint32)0x0BU << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_8KB             ((uint32)0x0CU << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_16KB            ((uint32)0x0DU << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_32KB            ((uint32)0x0EU << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_64KB            ((uint32)0x0FU << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_128KB           ((uint32)0x10U << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_256KB           ((uint32)0x11U << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_512KB           ((uint32)0x12U << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_1MKB            ((uint32)0x13U << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_2MKB            ((uint32)0x14U << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_4MKB            ((uint32)0x15U << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_8MKB            ((uint32)0x16U << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_16MKB           ((uint32)0x17U << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_32MKB           ((uint32)0x18U << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_64MKB           ((uint32)0x19U << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_128MKB          ((uint32)0x1AU << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_256MKB          ((uint32)0x1BU << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_512MKB          ((uint32)0x1CU << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_1GB             ((uint32)0x1DU << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_2GB             ((uint32)0x1EU << MPU_RASR_SIZE_POS)
#define MPU_RGN_SIZE_4GB             ((uint32)0x1FU << MPU_RASR_SIZE_POS)

#define MPU_RASR_AP_POS              (0x10U)
#define MPU_RASR_Encd_POS            (0x10U)
#define SCB_CCR_DC_POS               (16U)
#define SCB_CCSIDR_NUMSETS_POS       (13U) /*!< SCB CCSIDR: NumSets Position */
#define SCB_CCSIDR_NUMSETS_MSK       (0x7FFFUL << SCB_CCSIDR_NUMSETS_POS)
#define SCB_CCSIDR_ASSOCIATIVITY_POS (3U) /*!< SCB CCSIDR: Associativity Position */
#define SCB_CCSIDR_ASSOCIATIVITY_MSK (0x3FFUL << SCB_CCSIDR_ASSOCIATIVITY_POS)
/* SCB D-Cache Invalidate by Set-way Register Definitions */
#define SCB_DCISW_WAY_POS (30U)                      /*!< SCB DCISW: Way Position */
#define SCB_DCISW_WAY_MSK (3UL << SCB_DCISW_WAY_POS) /*!< SCB DCISW: Way Mask */

#define SCB_DCISW_SET_POS (5U) /*!< SCB DCISW: Set Position */
#define SCB_DCISW_SET_MSK (0x1FFUL << SCB_DCISW_SET_POS)
/* SCB D-Cache Clean and Invalidate by Set-way Register Definitions */
#define SCB_DCCISW_WAY_POS (30U)                       /*!< SCB DCCISW: Way Position */
#define SCB_DCCISW_WAY_MSK (3UL << SCB_DCCISW_WAY_POS) /*!< SCB DCCISW: Way Mask */

#define SCB_DCCISW_SET_POS (5U) /*!< SCB DCCISW: Set Position */
#define SCB_DCCISW_SET_MSK (0x1FFUL << SCB_DCCISW_SET_POS)

/* MPU register address. */
#define MPU_TYPE_ADDRESS        (0xE000ED90U)
#define MPU_CTRL_ADDRESS        (0xE000ED94U)
#define MPU_RNR_ADDRESS         (0xE000ED98U)
#define MPU_RBAR_ADDRESS        (0xE000ED9CU)
#define MPU_RASR_ADDRESS        (0xE000EDA0U)
#define MPU_RBAR_A1_ADDRESS     (0xE000EDA4U)
#define MPU_RASR_A1_ADDRESS     (0xE000EDA8U)
#define MPU_RBAR_A2_ADDRESS     (0xE000EDACU)
#define MPU_RASR_A2_ADDRESS     (0xE000EDB0U)
#define MPU_RBAR_A3_ADDRESS     (0xE000EDB4U)
#define MPU_RASR_A3_ADDRESS     (0xE000EDB8U)

#define CM7_SCB_ICIALLU_ADDRESS (0xE000EF50U)
#define CM7_SCB_CSSELR_ADDRESS  (0xE000ED84U)
#define CM7_SCB_CCSIDR_ADDRESS  (0xE000ED80U)
#define CM7_SCB_DCISW_ADDRESS   (0xE000EF60U)
#define CM7_SCB_DCCISW_ADDRESS  (0xE000EF74U)
#define MPU_TYPE_REG            OS_REG32(MPU_TYPE_ADDRESS)
#define MPU_CTRL_REG            OS_REG32(MPU_CTRL_ADDRESS)
#define MPU_RNR_REG             OS_REG32(MPU_RNR_ADDRESS)
#define MPU_RBAR_REG            OS_REG32(MPU_RBAR_ADDRESS)
#define MPU_RASR_REG            OS_REG32(MPU_RASR_ADDRESS)
#define MPU_RBAR_A1_REG         OS_REG32(MPU_RBAR_A1_ADDRESS)
#define MPU_RASR_A1_REG         OS_REG32(MPU_RASR_A1_ADDRESS)
#define MPU_RBAR_A2_REG         OS_REG32(MPU_RBAR_A2_ADDRESS)
#define MPU_RASR_A2_REG         OS_REG32(MPU_RASR_A2_ADDRESS)
#define MPU_RBAR_A3_REG         OS_REG32(MPU_RBAR_A3_ADDRESS)
#define MPU_RASR_A3_REG         OS_REG32(MPU_RASR_A3_ADDRESS)

#define CM7_SCB_CCR_REG         OS_REG32(CM7_SCB_CCR_ADDRESS)
#define CM7_SCB_ICIALLU_REG     OS_REG32(CM7_SCB_ICIALLU_ADDRESS)
#define CM7_SCB_CSSELR_REG      OS_REG32(CM7_SCB_CSSELR_ADDRESS)
#define CM7_SCB_CCSIDR_REG      OS_REG32(CM7_SCB_CCSIDR_ADDRESS)
#define CM7_SCB_DCISW_REG       OS_REG32(CM7_SCB_DCISW_ADDRESS)
#define CM7_SCB_DCCISW_REG      OS_REG32(CM7_SCB_DCCISW_ADDRESS)
/* The permission of the region. */
#define MPU_PRIV_NA_USER_NA_EXEC \
    (0x0000UL << MPU_RASR_AP_POS) /**< Alias no access in privileged mode, no access in user mode and execute */
#define MPU_PRIV_RW_USER_NA_EXEC \
    (0x0100UL << MPU_RASR_AP_POS) /**< Alias no read/write in privileged mode, no access in user mode and execute */
#define MPU_PRIV_RW_USER_RO_EXEC \
    (0x0200UL << MPU_RASR_AP_POS) /**< Alias no read/write in privileged mode, read only in user mode and execute */
#define MPU_PRIV_RW_USER_RW_EXEC \
    (0x0300UL << MPU_RASR_AP_POS) /**< Alias no read/write in privileged mode, read/write in user mode and execute */
#define MPU_PRIV_RO_USER_NA_EXEC \
    (0x0500UL << MPU_RASR_AP_POS) /**< Alias no read only in privileged mode, no access in user mode and execute */
#define MPU_PRIV_RO_USER_RO_EXEC \
    (0x0600UL << MPU_RASR_AP_POS) /**< Alias no read only in privileged mode, read only in user mode and execute */
#define MPU_PRIV_NA_USER_NA_NOEXEC \
    (0x1000UL << MPU_RASR_AP_POS) /**< Alias no access in privileged mode, no access in user mode and no execution */
#define MPU_PRIV_RW_USER_NA_NOEXEC \
    (0x1100UL                      \
     << MPU_RASR_AP_POS) /**< Alias no read/write in privileged mode, no access in user mode and no execution */
#define MPU_PRIV_RW_USER_RO_NOEXEC \
    (0x1200UL                      \
     << MPU_RASR_AP_POS) /**< Alias no read/write in privileged mode, read only in user mode and no execution */
#define MPU_PRIV_RW_USER_RW_NOEXEC \
    (0x1300UL                      \
     << MPU_RASR_AP_POS) /**< Alias no read/write in privileged mode, read/write in user mode and no execution */
#define MPU_PRIV_RO_USER_NA_NOEXEC                                                                                    \
    (0x1500UL << MPU_RASR_AP_POS) /**< Alias no read only in privileged mode, no access in user mode and no execution \
                                   */
#define MPU_PRIV_RO_USER_RO_NOEXEC                                                                                    \
    (0x1600UL << MPU_RASR_AP_POS) /**< Alias no read only in privileged mode, read only in user mode and no execution \
                                   */

#define MPU_REGION_ENABLE             (0x01UL)
#define MPU_REGION_DISABLE            (0x00UL)
#define MPU_STRONGLYORDERED_SHAREABLE (0x0000UL << MPU_RASR_Encd_POS)
#define MPU_DEVICE_SHAREABLE          (0x0001UL << MPU_RASR_Encd_POS) /**< Memory type device and sharable */
#define MPU_NORMAL_OIWTNOWA_NONSHARED \
    (0x0002UL                         \
     << MPU_RASR_Encd_POS) /**< Memory type normal outer and inner write-through, no write allocate and non shared */
#define MPU_NORMAL_OIWBNOWA_NONSHARED \
    (0x0003UL                         \
     << MPU_RASR_Encd_POS) /**< Memory type normal outer and inner write-back, no write allocate and non shared */
#define MPU_NORMAL_OIWTNOWA_SHARED \
    (0x0006UL                      \
     << MPU_RASR_Encd_POS) /**< Memory type normal outer and inner write-through, no write allocate and shared */
#define MPU_NORMAL_OIWBNOWA_SHARED                                                                                   \
    (0x0007UL << MPU_RASR_Encd_POS) /**< Memory type normal outer and inner write-back, no write allocate and shared \
                                     */
#define MPU_NORMAL_OINC_NONSHARED \
    (0x0008UL << MPU_RASR_Encd_POS) /**< Memory type normal outer and inner non-cachable and non shared */
#define MPU_NORMAL_OIWBWA_NONSHARED                                                                                   \
    (0x000BUL << MPU_RASR_Encd_POS) /**< Memory type normal outer and inner write-back, write allocate and non shared \
                                     */
#define MPU_NORMAL_OINC_SHARED \
    (0x000CUL << MPU_RASR_Encd_POS) /**< Memory type normal outer and inner non-cachable and shared */
#define MPU_NORMAL_OIWBWA_SHARED \
    (0x000FUL << MPU_RASR_Encd_POS) /**< Memory type normal outer and inner write-back, write allocate and shared */
#define MPU_DEVICE_NONSHAREABLE (0x0010UL << MPU_RASR_Encd_POS) /**< Memory type device and non sharable */

/* the region of mpu. */
#define MPU_REGION_0 (0U)
#define MPU_REGION_1 (1U)
#define MPU_REGION_2 (2U)
#define MPU_REGION_3 (3U)
#define MPU_REGION_4 (4U)
#define MPU_REGION_5 (5U)
#define MPU_REGION_6 (6U)
#define MPU_REGION_7 (7U)

/* MPU region number definitions.*/
#define OS_MPU_REGION_NUM_ALL_PFLASH_SPACE (0u)
#define OS_MPU_REGION_NUM_ALL_APP_CODE     (1u)
#define OS_MPU_REGION_NUM_CUR_APP_CODE     (2u)
#define OS_MPU_REGION_NUM_ALL_SRAM_SPACE   (3u)
#define OS_MPU_REGION_NUM_ALL_APP_DATA     (4u)
#define OS_MPU_REGION_NUM_CUR_APP_PRI_DATA (5u)
#define OS_MPU_REGION_NUM_CUR_TASK_DATA    (6u)
#define OS_MPU_REGION_NUM_CUR_ISR_DATA     (6u)
#define OS_MPU_REGION_NUM_SHARED_SPACE     (7u)
#define OS_MPU_REGION_NUM_ALL_PERI_SPACE   (8u)
#define OS_MPU_REGION_USEMAX_NUM           (9u)

/*=======[Hardfault behavior]======================================================*/
#define HARD_FAULT_VECTOR_READ_ERR                      (0U)
#define HARD_FAULT_ESCALATION_ERR                       (1U)
#define HARD_FAULT_BKPT_ESCALATION_ERR                  (2U)
#define BUS_FAULT_EXCEPTION_ENTRY_STACK_ERR             (3U)
#define MEMM_FAULT_EXCEPTION_ENTRY_STACK_ERR            (4U)
#define BUS_FAULT_EXCEPTION_RET_STACK_ERR               (5U)
#define MEMM_FAULT_EXCEPTION_RET_STACK_ERR              (6U)
#define MEMM_FAULT_DATA_ACCESSS_ERR                     (7U)
#define MEMM_FAULT_INSTRUCTION_ACCESS_ERR               (8U)
#define BUS_FAULT_INSTRUCTION_FETCH_ERR                 (9U)
#define BUS_FAULT_DATA_ACCESS_PRECISE_ERR               (10U)
#define BUS_FAULT_BUS_ERR_DATA_BUS_ERR                  (11U)
#define USAFEFAULT_NO_COPROCESSOR_ERR                   (12U)
#define USAGEFAULT_UNDEFINED_INSTRUCTION                (13U)
#define USAGEFAULT_EXC_INSTRCTION_IN_EPSRT_EQU_0        (14U)
#define USAGEFAULT_EXC_RET_INTEGRITY_CHECK_FAILURE_ERR  (15U)
#define USAGEFAULT_ILLEGAL_UNAGLIGNED_LOAD_OR_STORE_ERR (16U)
#define USAGEFAULT_DIVIDE_BY_0_ERR                      (17U)
#define MEMM_FAULT_DELAYED_FALOT_POINT_PRESERVATION_ERR (18U)
#define BUSFAULT_DELAYED_FALOT_POINT_PRESERVATION_ERR   (19U)
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
#define PERIPHERAL_START __PERIPHERAL_START
#define PERIPHERAL_END   __PERIPHERAL_END

#define CODE_DAAR_START  __CODE_START
#define CODE_ADDR_END    __CODE_END

#define SHARED_START     __SHARED_START
#define SHARED_END       __SHARED_END

#define SRAM_START       __SRAM_START
#define SRAM_END         __SRAM_END

/* Get the memory protection type of application.*/
/* PRQA S 3472,3458++ */ /* MISRA Rule 4.9,PRE10 */
#define OS_MPU_GET_APP_MP_TYPE(trust, protection)   \
    {                                               \
        if (TRUE == (trust))                        \
        {                                           \
            if (TRUE == (protection))               \
            {                                       \
                set = OS_MP_SET_TRUST_WITH_PROT;    \
            }                                       \
            else                                    \
            {                                       \
                set = OS_MP_SET_TRUST_WITHOUT_PROT; \
            }                                       \
        }                                           \
        else                                        \
        {                                           \
            set = OS_MP_SET_NON_TRUST;              \
        }                                           \
    }
/* PRQA S 3472,3458-- */ /* MISRA Rule 4.9,PRE10 */

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* This enum define the memory protection properties type. */
typedef enum
{
    OS_MP_SET_INIT = 0u, /*PRQA S 1271*/               /* MISRA CWE-682 */
    OS_MP_SET_NON_TRUST = 1u, /*PRQA S 1271*/          /* MISRA CWE-682 */
    OS_MP_SET_TRUST_WITH_PROT = 2u, /*PRQA S 1271*/    /* MISRA CWE-682 */
    OS_MP_SET_TRUST_WITHOUT_PROT = 3u, /*PRQA S 1271*/ /* MISRA CWE-682 */
} Os_MpSetType;

typedef struct
{
    uint8 u8RegionNum;   /*!< Region number                    */
    uint32 u32StartAddr; /*!< Memory region start address      */
    uint32 u32EndAddr;   /*!< Memory region end address        */
    uint32 size;
    uint32 u32access; /*!< Access permission for region     */
} mpu_config;

typedef struct
{
    uint32 (*RegionRbarPtr)[OS_MPU_REGION_USEMAX_NUM];
    uint32 (*RegionRasrPtr)[OS_MPU_REGION_USEMAX_NUM];
} Os_MpuRegionType;
/*=======[E X T E R N A L   D A T A]==========================================*/
/* PRQA S 3684 ++ */ /* MISRA Rule 8.11 */
extern uint8 __PERIPHERAL_START[];
extern uint8 __PERIPHERAL_END[];

extern uint32 __CODE_START[];
extern uint32 __CODE_END[];

extern uint32 __SHARED_START[];
extern uint32 __SHARED_END[];

extern uint8 __SRAM_START[];
extern uint8 __SRAM_END[];
/* PRQA S 3684 -- */ /* MISRA Rule 8.11 */ /*ARCH_MPU_H_DATA_DEFINE_001*/
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/* For memory protection. */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
extern FUNC(void, OS_CODE) Os_SetMpuRegion(uint8 region, uint32 base, uint32 size, uint32 access);
extern FUNC(void, OS_CODE) Os_DisableMpuRegion(uint8 region);
extern FUNC(void, OS_CODE) Os_MpuEnable(void);
extern FUNC(void, OS_CODE) Os_MpuDisable(void);
extern FUNC(void, OS_CODE) Os_ArchInitKnMemMap(void);
extern FUNC(void, OS_CODE) Os_ArchMemProtEnable(void);
extern FUNC(void, OS_CODE) Os_MpuConfigTableInit(void);
extern FUNC(void, OS_CODE) Os_MpuIsrSwitch(ISRType IsrId, ApplicationType HostAppId);
extern FUNC(void, OS_CODE) Os_MpuTaskSwitch(TaskType TaskId, ApplicationType HostAppId);
extern FUNC(void, OS_CODE) Os_ArchMpDefaultConfigSwitch(void);
extern FUNC(uint8, OS_CODE) Os_GetFault(void);
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
extern FUNC(uint32, OS_CODE) Os_CaculateMemSize(const uint8* startAddr, const uint8* endAddr);
#endif
#endif
/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#endif /* ARCH_MPU_H_ */

/*=======[E N D   O F   F I L E]==============================================*/
