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
 **  FILENAME    : Arch_Define.h                                               **
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

/*=======[M A C R O S]========================================================*/
/* Basic system register */
#define OS_EIPC_NUM   (0), (0)  /* SR0,0 */
#define OS_EIPSW_NUM  (1), (0)  /* SR1,0 */
#define OS_FEPC_NUM   (2), (0)  /* SR2,0 */
#define OS_FEPSW_NUM  (3), (0)  /* SR3,0 */
#define OS_PSW_NUM    (5), (0)  /* SR5,0 */
#define OS_FPSR_NUM   (6), (0)  /* SR6,0 */
#define OS_FPEPC_NUM  (7), (0)  /* SR7,0 */
#define OS_FPST_NUM   (8), (0)  /* SR8,0 */
#define OS_FPCC_NUM   (9), (0)  /* SR9,0 */
#define OS_FPCFG_NUM  (10), (0) /* SR10,0 */
#define OS_FPEC_NUM   (11), (0) /* SR11,0 */
#define OS_EIIC_NUM   (12), (0) /* SR13,0 */
#define OS_FEIC_NUM   (14), (0) /* SR14,0 */
#define OS_CTPC_NUM   (16), (0) /* SR16,0 */
#define OS_CTPSW_NUM  (17), (0) /* SR17,0 */
#define OS_CTBP_NUM   (20), (0) /* SR20,0 */
#define OS_EIWR_NUM   (28), (0) /* SR28,0 */
#define OS_FEWR_NUM   (29), (0) /* SR29,0 */
#define OS_MCFG0_NUM  (0), (1)  /* SR0,1 */
#define OS_RBASE_NUM  (2), (1)  /* SR2,1 */
#define OS_EBASE_NUM  (3), (1)  /* SR3,1 */
#define OS_INTBP_NUM  (4), (1)  /* SR4,1 */
#define OS_MCTL_NUM   (5), (1)  /* SR5,1 */
#define OS_PID_NUM    (6), (1)  /* SR6,1 */
#define OS_SCCFG_NUM  (11), (1) /* SR11,1 */
#define OS_SCBP_NUM   (12), (1) /* SR12,1 */
#define OS_HTCFG0_NUM (0), (2)  /* SR0,2 */
#define OS_EMA_NUM    (6), (2)  /* SR6,2 */
#define OS_ASID_NUM   (7), (2)  /* SR7,2 */
#define OS_MEI_NUM    (8), (2)  /* SR8,2 */
#define OS_REG_PEID   (0), (2)

/* MPU Function System Register */
#define OS_MPM_NUM       (0), (5)
#define OS_MPCFG_NUM     (2), (5)
#define OS_MCA_NUM       (8), (5)
#define OS_MCS_NUM       (9), (5)
#define OS_MCC_NUM       (10), (5)
#define OS_MCR_NUM       (11), (5)
#define OS_MCI_NUM       (12), (5)
#define OS_MPIDX_NUM     (16), (5)
#define OS_MPBX_NUM      (17), (5)
#define OS_MPLA_NUM      (20), (5)
#define OS_MPUA_NUM      (21), (5)
#define OS_MPAT_NUM      (22), (5)
#define OS_MPID0_NUM     (24), (5)
#define OS_MPID1_NUM     (25), (5)
#define OS_MPID2_NUM     (26), (5)
#define OS_MPID3_NUM     (27), (5)
#define OS_MPID4_NUM     (28), (5)
#define OS_MPID5_NUM     (29), (5)
#define OS_MPID6_NUM     (30), (5)
#define OS_MPID7_NUM     (31), (5)

#define OS_MPU_REGION_0  (0U)
#define OS_MPU_REGION_1  (1U)
#define OS_MPU_REGION_2  (2U)
#define OS_MPU_REGION_3  (3U)
#define OS_MPU_REGION_4  (4U)
#define OS_MPU_REGION_5  (5U)
#define OS_MPU_REGION_6  (6U)
#define OS_MPU_REGION_7  (7U)
#define OS_MPU_REGION_8  (8U)
#define OS_MPU_REGION_9  (9U)
#define OS_MPU_REGION_10 (10U)
#define OS_MPU_REGION_11 (11U)
#define OS_MPU_REGION_12 (12U)
#define OS_MPU_REGION_13 (13U)
#define OS_MPU_REGION_14 (14U)
#define OS_MPU_REGION_15 (15U)
#define OS_MPU_REGION_16 (16U)
#define OS_MPU_REGION_17 (17U)
#define OS_MPU_REGION_18 (18U)
#define OS_MPU_REGION_19 (19U)
#define OS_MPU_REGION_20 (20U)
#define OS_MPU_REGION_21 (21U)
#define OS_MPU_REGION_22 (22U)
#define OS_MPU_REGION_23 (23U)
#define OS_MPU_REGION_24 (24U)
#define OS_MPU_REGION_25 (25U)
#define OS_MPU_REGION_26 (26U)
#define OS_MPU_REGION_27 (27U)
#define OS_MPU_REGION_28 (28U)
#define OS_MPU_REGION_29 (29U)
#define OS_MPU_REGION_30 (30U)
#define OS_MPU_REGION_31 (31U)

/* Interrupt Function Register */
#define OS_ISPR_NUM   (10), (2) /* Priority of interrupt being serviced */
#define OS_IMSR_NUM   (11), (2) /* Interrupt mask status */
#define OS_ICSR_NUM   (12), (2) /* Interrupt control status */
#define OS_INTCFG_NUM (13), (2) /* Interrupt function setting */
#define OS_PLMR_NUM   (14), (2) /* Interrupt priority masking */

/* Interrupt mode---Direct&Table */
#define OS_ARCH_INT_DIRECT ((uint16)0x0000u)
#define OS_ARCH_INT_TABLE  ((uint16)0x0040u)

#define OS_BEGIN_NO_OPTIMIZE
#define OS_END_NO_OPTIMIZE
/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* Memory protection Kernel Address */
/* Provided for external use */
typedef struct
{
    uint8* pRamStart; /* RAM */
    uint8* pRamEnd;

    uint8* pPeripheralStart; /* PERIPHERAL */
    uint8* pPeripheralEnd;

    uint8* pRomStart; /* PFLASH */
    uint8* pRomEnd;
} Os_MemProtKnAddr;

/* Type of memory protection Kernel */
/* Provided for external use */
typedef struct
{
    CONST(Os_MemProtKnAddr, OS_VAR) OsKernelAddr;
} Os_MemProtKnCfgType;
#endif /* ARCH_DEFINE_H */
/*=======[E N D   O F   F I L E]==============================================*/
