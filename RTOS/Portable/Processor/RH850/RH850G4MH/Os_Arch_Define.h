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
 **  FILENAME    : Os_Arch_Define.h                                            **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : Variable definition                                         **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/

#ifndef ARCH_DEFINE_H
#define ARCH_DEFINE_H

/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"
#include "Os_Hal_Compiler.h"
/*=======[M A C R O S]========================================================*/
/* Basic system register */
#define OS_ARCH_EIPC_NUM   (0U), (0U)  /* SR0,0 */
#define OS_ARCH_EIPSW_NUM  (1U), (0U)  /* SR1,0 */
#define OS_ARCH_FEPC_NUM   (2U), (0U)  /* SR2,0 */
#define OS_ARCH_FEPSW_NUM  (3U), (0U)  /* SR3,0 */
#define OS_ARCH_PSW_NUM    (5U), (0U)  /* SR5,0 */
#define OS_ARCH_FPSR_NUM   (6U), (0U)  /* SR6,0 */
#define OS_ARCH_FPEPC_NUM  (7U), (0U)  /* SR7,0 */
#define OS_ARCH_FPST_NUM   (8U), (0U)  /* SR8,0 */
#define OS_ARCH_FPCC_NUM   (9U), (0U)  /* SR9,0 */
#define OS_ARCH_FPCFG_NUM  (10U), (0U) /* SR10,0 */
#define OS_ARCH_EIIC_NUM   (13U), (0U) /* SR13,0 */
#define OS_ARCH_FEIC_NUM   (14U), (0U) /* SR14,0 */
#define OS_ARCH_CTPC_NUM   (16U), (0U) /* SR16,0 */
#define OS_ARCH_CTPSW_NUM  (17U), (0U) /* SR17,0 */
#define OS_ARCH_CTBP_NUM   (20U), (0U) /* SR20,0 */
#define OS_ARCH_SNZCFG_NUM (21U), (0U) /* SR21,0 */
#define OS_ARCH_EIWR_NUM   (28U), (0U) /* SR28,0 */
#define OS_ARCH_FEWR_NUM   (29U), (0U) /* SR29,0 */
#define OS_ARCH_MCFG0_NUM  (0U), (1U)  /* SR0,1 */
#define OS_ARCH_RBASE_NUM  (2U), (1U)  /* SR2,1 */
#define OS_ARCH_EBASE_NUM  (3U), (1U)  /* SR3,1 */
#define OS_ARCH_INTBP_NUM  (4U), (1U)  /* SR4,1 */
#define OS_ARCH_MCTL_NUM   (5U), (1U)  /* SR5,1 */
#define OS_ARCH_PID_NUM    (6U), (1U)  /* SR6,1 */
#define OS_ARCH_SVLOCK_NUM (8U), (1U)  /* SR8,1 */
#define OS_ARCH_SCCFG_NUM  (11U), (1U) /* SR11,1 */
#define OS_ARCH_SCBP_NUM   (12U), (1U) /* SR12,1 */
#define OS_ARCH_REG_PEID   (0U), (2U)  /* SR0,2 */
#define OS_ARCH_BMID_NUM   (1U), (2U)  /* SR1,2 */
#define OS_ARCH_EMA_NUM    (6U), (2U)  /* SR6,2 */
#define OS_ARCH_MEI_NUM    (8U), (2U)  /* SR8,2 */
#define OS_ARCH_PBCR0_NUM  (15U), (2U) /* SR15,2 */
#define OS_ARCH_PBCR1_NUM  (16U), (2U) /* SR16,2 */
#define OS_ARCH_PBNR_NUM   (17U), (2U) /* SR17,2 */
#define OS_ARCH_PBIP_NUM   (18U), (2U) /* SR18,2 */

/* MPU Function System Register */
#define OS_ARCH_MPM_NUM   (0U), (5U)
#define OS_ARCH_MPCFG_NUM (2U), (5U)
#define OS_ARCH_MCA_NUM   (8U), (5U)
#define OS_ARCH_MCS_NUM   (9U), (5U)
#define OS_ARCH_MCC_NUM   (10U), (5U)
#define OS_ARCH_MCR_NUM   (11U), (5U)
#define OS_ARCH_MCI_NUM   (12U), (5U)
#define OS_ARCH_MPIDX_NUM (16U), (5U)
#define OS_ARCH_MPBX_NUM  (17U), (5U)
#define OS_ARCH_MPLA_NUM  (20U), (5U)
#define OS_ARCH_MPUA_NUM  (21U), (5U)
#define OS_ARCH_MPAT_NUM  (22U), (5U)
#define OS_ARCH_MPID0_NUM (24U), (5U)
#define OS_ARCH_MPID1_NUM (25U), (5U)
#define OS_ARCH_MPID2_NUM (26U), (5U)
#define OS_ARCH_MPID3_NUM (27U), (5U)
#define OS_ARCH_MPID4_NUM (28U), (5U)
#define OS_ARCH_MPID5_NUM (29U), (5U)
#define OS_ARCH_MPID6_NUM (30U), (5U)
#define OS_ARCH_MPID7_NUM (31U), (5U)

#define OS_ARCH_MPU_REGION_0  (0U)
#define OS_ARCH_MPU_REGION_1  (1U)
#define OS_ARCH_MPU_REGION_2  (2U)
#define OS_ARCH_MPU_REGION_3  (3U)
#define OS_ARCH_MPU_REGION_4  (4U)
#define OS_ARCH_MPU_REGION_5  (5U)
#define OS_ARCH_MPU_REGION_6  (6U)
#define OS_ARCH_MPU_REGION_7  (7U)
#define OS_ARCH_MPU_REGION_8  (8U)
#define OS_ARCH_MPU_REGION_9  (9U)
#define OS_ARCH_MPU_REGION_10 (10U)
#define OS_ARCH_MPU_REGION_11 (11U)
#define OS_ARCH_MPU_REGION_12 (12U)
#define OS_ARCH_MPU_REGION_13 (13U)
#define OS_ARCH_MPU_REGION_14 (14U)
#define OS_ARCH_MPU_REGION_15 (15U)
#define OS_ARCH_MPU_REGION_16 (16U)
#define OS_ARCH_MPU_REGION_17 (17U)
#define OS_ARCH_MPU_REGION_18 (18U)
#define OS_ARCH_MPU_REGION_19 (19U)
#define OS_ARCH_MPU_REGION_20 (20U)
#define OS_ARCH_MPU_REGION_21 (21U)
#define OS_ARCH_MPU_REGION_22 (22U)
#define OS_ARCH_MPU_REGION_23 (23U)
#define OS_ARCH_MPU_REGION_24 (24U)
#define OS_ARCH_MPU_REGION_25 (25U)
#define OS_ARCH_MPU_REGION_26 (26U)
#define OS_ARCH_MPU_REGION_27 (27U)
#define OS_ARCH_MPU_REGION_28 (28U)
#define OS_ARCH_MPU_REGION_29 (29U)
#define OS_ARCH_MPU_REGION_30 (30U)
#define OS_ARCH_MPU_REGION_31 (31U)

/* Interrupt Function Register */
#define OS_ARCH_ISPR_NUM   (10U), (2U) /* Priority of interrupt being serviced */
#define OS_ARCH_IMSR_NUM   (11U), (2U) /* Interrupt mask status */
#define OS_ARCH_ICSR_NUM   (12U), (2U) /* Interrupt control status */
#define OS_ARCH_INTCFG_NUM (13U), (2U) /* Interrupt function setting */
#define OS_ARCH_PLMR_NUM   (14U), (2U) /* Interrupt priority masking */

/* Hazard Control */
#define OS_ARCH_SYNCP() ASM("SYNCP") /* Synchronize pipeline */
#define OS_ARCH_SYNCM() ASM("SYNCM") /* Synchronize memory */
#define OS_ARCH_SYNCI() ASM("SYNCI") /* Synchronize instruction fetch */
#define OS_ARCH_SYNCE() ASM("SYNCE") /* Synchronize exceptions */

/* Inline assembler support for HALT instruction */
#define OS_ARCH_HALT() ASM("halt")
/* Inline assembler support for NOP instruction */
#define OS_ARCH_NOP() ASM("nop")

/* Bit manipulation instruction */
#define OS_ARCH_SET1_BIT_SPECIFY(addr, bitnum) \
    ASM("SET1 %1, 0[%0]" ::"r"(addr), "i"(bitnum) : "memory") /* Clear bit by "Format VIII" */
#define OS_ARCH_CLR1_BIT_SPECIFY(addr, bitnum) \
    ASM("CLR1 %1, 0[%0]" ::"r"(addr), "i"(bitnum) : "memory") /* Not bit by "Format VIII"  */
#define OS_ARCH_SET1_GP_REGISTER(addr, bitnum) \
    ASM("SET1 %1, [%0]" ::"r"(addr), "r"(bitnum) : "memory") /* Clear bit by "Format IX" */
#define OS_ARCH_CLR1_GP_REGISTER(addr, bitnum) \
    ASM("CLR1 %1, [%0]" ::"r"(addr), "r"(bitnum) : "memory") /* Not bit by "Format IX" */

/* Interrupt mode---Direct&Table */
#define OS_ARCH_INT_DIRECT ((uint16)0x0000U)
#define OS_ARCH_INT_TABLE  ((uint16)0x0040U)

/* The platform can reserve configuration, such as reserving stack traceback space*/
#define OS_HAL_STACK_RESERVE 0

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
typedef uint32 Os_ArchTaskCBExtType;
typedef void (*Os_ArchIsrhnd)(void); /* PRQA S 5004 */ /* VL_QAC_FuncPtr */

#endif /* ARCH_DEFINE_H */
/*=======[E N D   O F   F I L E]==============================================*/
