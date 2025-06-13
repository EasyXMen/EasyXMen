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
**  FILENAME    : Arch_Irq.c                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Deal with operations related to processor interrupts        **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/
/*=======[I N C L U D E S]===================================================*/
#include "Arch_irq.h"
#include "Os_Internal.h"
/*=======[V E R S I O N  C H E C K]==========================================*/

/*=======[M A C R O S]=======================================================*/
#define OS_ISR_EXTERNAL_NUM 16u
/*=======[E X T E R N A L   D A T A]=========================================*/
/* PRQA S 0791++ */ /* MISRA Rule 5.4 */
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
VAR(uint32, OS_VAR) Os_ArchMasterSp_ARRAY[OS_ISR_MAXNUM];
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
VAR(uint32, OS_VAR) Os_ISRxPSRStack[OS_ISR_MAXNUM];
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#if (CFG_ISR_MAX > 0U)
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
VAR(Os_CallLevelType, OS_VAR) Os_SaveLevelISR1;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
VAR(volatile uint32, OS_VAR) Os_TprotTerminateIsr;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
VAR(volatile uint32, OS_VAR) Os_TprotTerminateTask;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
VAR(uint32, OS_VAR) Os_ArchTempSysSp;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
VAR(uint32, OS_VAR) Os_IsrTempIPSR = 0U;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
VAR(uint32, OS_VAR) Os_Isr2_Ipl_Limit;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/* PRQA S 0791-- */ /* MISRA Rule 5.4 */
/*=======[I N T E R N A L   D A T A]=========================================*/
/* PRQA S 0305,0428 ++ */ /* MISRA Rule 11.1,CWE-398 */
#define OS_START_SEC_VAR_INTVECTOR_GLOBAL_32
#include "Os_MemMap.h"
uint32 Os_GlobalVector[OS_NVIC_NUM + 1u] = {
    [0u] = (uint32)0u,
    [1] = (uint32)((uint32)(&Reset_Handler) + (1u)),
    [2] = (uint32)(&NMI_Handler),
    [3] = (uint32)(&HardFault_Handler),
    [4] = (uint32)(&MemManage_Handler),
    [5] = (uint32)(&BusFault_Handler),
    [6] = (uint32)(&UsageFault_Handler),
    [7] = (uint32)0u,  /* Reserved */
    [8] = (uint32)0u,  /* Reserved */
    [9] = (uint32)0u,  /* Reserved */
    [10] = (uint32)0u, /* Reserved */
    [11] = (uint32)((uint32)(&SVC_Handler) + 1u),
    [12] = (uint32)(&DebugMon_Handler),
    [13] = (uint32)0u,
    [14] = (uint32)((uint32)(&PendSV_Handler) + 1u),
    [15] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [16] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [17] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [18] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [19] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [20] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [21] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [22] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [23] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [24] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [25] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [26] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [27] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [28] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [29] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [30] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [31] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [32] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [33] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [34] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [35] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [36] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [37] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [38] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [39] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [40] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [41] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [42] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [43] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [44] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [45] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [46] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [47] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [48] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [49] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [50] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [51] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [52] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [53] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [54] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [55] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [56] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [57] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [58] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [59] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [60] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [61] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [62] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [63] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [64] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [65] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [66] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [67] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [68] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [69] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [70] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [71] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [72] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [73] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [74] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [75] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [76] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [77] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [78] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [79] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [80] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [81] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [82] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [83] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [84] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [85] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [86] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [87] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [88] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [89] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [90] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [91] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [92] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [93] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [94] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [95] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [96] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [97] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [98] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [99] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [100] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [101] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [102] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [103] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [104] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [105] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [106] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [107] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [108] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [109] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [110] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [111] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [112] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [113] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [114] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [115] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [116] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [117] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [118] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [119] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [120] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [121] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [122] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [123] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [124] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [125] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [126] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [127] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [128] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [129] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [130] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [131] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [132] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [133] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [134] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [135] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [136] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [137] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [138] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [139] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [140] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [141] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [142] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [143] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [144] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [145] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [146] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [147] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [148] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [149] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [150] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [151] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [152] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [153] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [154] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [155] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [156] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [157] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [158] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [159] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [160] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [161] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [162] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [163] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [164] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [165] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [166] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [167] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [168] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [169] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [170] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [171] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [172] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [173] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [174] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [175] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [176] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [177] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [178] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [179] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [180] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [181] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [182] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [183] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [184] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [185] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [186] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [187] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [188] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [189] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [190] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [191] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [192] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [193] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [194] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [195] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [196] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [197] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [198] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [199] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [200] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [201] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [202] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [203] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [204] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [205] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [206] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [207] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [208] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [209] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [210] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [211] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [212] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [213] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [214] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [215] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [216] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [217] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [218] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [219] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [220] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [221] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [222] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [223] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [224] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [225] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [226] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [227] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [228] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [229] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [230] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [231] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [232] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [233] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [234] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [235] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [236] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [237] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [238] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [239] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [240] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [241] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [242] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [243] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [244] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [245] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [246] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [247] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [248] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [249] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [250] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [251] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [252] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [253] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [254] = (uint32)((uint32)(&armv7_default_isr) + 1u),
    [255] = (uint32)((uint32)(&armv7_default_isr) + 1u),
};
#define OS_STOP_SEC_VAR_INTVECTOR_GLOBAL_32
#include "Os_MemMap.h"
/* PRQA S 0305,0428 -- */ /* MISRA Rule 11.1,CWE-398 */

#define OS_START_SEC_VAR_INTVECTOR_LOCAL_32
#include "Os_MemMap.h"
uint32 Os_LocalVector[OS_NVIC_NUM + 1u] = {0u};
#define OS_STOP_SEC_VAR_INTVECTOR_LOCAL_32
#include "Os_MemMap.h"

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]=======*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]===================*/
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (CFG_ISR_MAX > 0)
#if (TRUE == CFG_INT_NEST_ENABLE)
static FUNC(StatusType, OS_CODE) Os_ArchTerminateOneNestIsr(Os_IsrType OsIsrID);
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Terminate an interrupt>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <OsIsrID>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <StatusType>
 * CallByAPI            <Os_ArchAppTerminateIsrProc>
 * REQ ID               <DD_1_0901>
 */
/******************************************************************************/
static FUNC(StatusType, OS_CODE) Os_ArchTerminateOneNestIsr(Os_IsrType OsIsrID)
{
    uint8 i;
    StatusType Status = (uint8)E_OK; /* PRQA S 1277 */ /* CWE-398 */
    for (i = 0u; i < Os_IntNestISR2; i++)
    {
        /* find out the Isr nested position and reclaim its context saving area */
        if (OsIsrID == Os_SCB.sysIsrNestQueue[i])
        {
            Os_IsrTempIPSR = Os_ISRxPSRStack[i];
            TERMINATEISR_ISR();
            break;
        }
    }

    /* Not find out the Isr Id in the IsrStack from the SCB,
     * not include outermost_isr. */
    if (i >= Os_IntNestISR2)
    {
        Status = (uint8)E_NOT_OK; /* PRQA S 1277 */ /* CWE-398 */
    }
    else
    {
        /* Moving the following nested ISR2s to shift one position left */
        while (i < (uint8)(Os_IntNestISR2 - (uint8)1u))
        {
            Os_SCB.sysIsrNestQueue[i] = Os_SCB.sysIsrNestQueue[i + (uint8)1u];

            if (i < (Os_IntNestISR2 - 2U)) /* PRQA S 3120 */ /* MISRA CWE-398 */
            {
                Os_ArchMasterSp_ARRAY[i + 1U] = Os_ArchMasterSp_ARRAY[i + 2U]; /* PRQA S 3120 */ /* MISRA CWE-398 */
            }

            i++;
        }
    }
    return Status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Terminate Isr2 platform process in TerminateApplication>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchInitCPU>
 */
/*****************************************************************************/
FUNC(void, OS_CODE) Os_ArchAppTerminateIsrProc(Os_IsrType OsIsrID)
{
    StatusType ulRet = (uint8)E_OK; /* PRQA S 1277 */ /* CWE-398 */
    /* Isr_nest process. */
    /* Just find out the nested corresponding ISR2s(excluding the running one) */
    if (OsIsrID != Os_SCB.sysRunningIsrCat2Id)
    {
        /* Terminate one isr */
        ulRet = Os_ArchTerminateOneNestIsr(OsIsrID);

        if ((uint8)E_OK == ulRet) /* PRQA S 1277 */ /* CWE-398 */
        {
            if (Os_IntNestISR2 > (uint8)0u)
            {
                Os_IntNestISR2--;
            }

            if (Os_SCB.sysDispatchLocker > (uint8)0u)
            {
                Os_SCB.sysDispatchLocker--;
            }
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* #if (TRUE == CFG_INT_NEST_ENABLE) */ /* PRQA S 2053 */              /* MISRA Rule 18.8 */
#endif /* #if (CFG_ISR_MAX > 0) */ /* PRQA S 2053 */                          /* MISRA Rule 18.8 */
#endif /* #if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC)) */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Save ccr to variable msr and disable maskable interrupt>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <SuspendAllInterrupts>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Os_ArchSuspendOsInt(P2VAR(Os_ArchMsrType, AUTOMATIC, OS_VAR) msr) /*PRQA S 3006,3432*/ /*MISRA Dir 4.3,Rule 20.7*/
{
    register Os_ArchMsrType result; /* PRQA S 2011 */ /* MISRA CWE-398 */
    OS_ASM("MRS %0, basepri_max" : "=r"(result));
    result = OS_NVIC_CONVERT_GET_PRIO(result);
    *msr = result;
    result = Os_Isr2_Ipl_Limit;
    OS_ASM("msr basepri, %0" : : "r"(result));
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <transfer variable msr back to ccr>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ResumeAllInterrupts>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchRestoreOsInt(Os_ArchMsrType msr) /*PRQA S 3006*/ /*MISRA Dir 4.3*/
{
    register Os_ArchMsrType converted_prio = OS_NVIC_CONVERT_SET_PRIO(msr); /* PRQA S 2011 */ /* MISRA CWE-398 */
    OS_ASM("msr basepri, %0" : : "r"(converted_prio));
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Save ccr to variable msr and disable maskable interrupt>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <SuspendAllInterrupts>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Os_ArchSuspendInt(P2VAR(Os_ArchMsrType, AUTOMATIC, OS_VAR) msr) /*PRQA S 3006,3432*/ /*MISRA Dir 4.3,Rule 20.7*/
{
    register Os_ArchMsrType result; /* PRQA S 2011 */ /* MISRA CWE-398 */
    OS_ASM("MRS %0, primask" : "=r"(result));
    *msr = result;
    OS_ASM("MSR primask, %0" : : "r"(OS_MSR_PRIMASK_BIT0));
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <transfer variable msr back to ccr>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ResumeAllInterrupts>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchRestoreInt(Os_ArchMsrType msr) /*PRQA S 3006*/ /*MISRA Dir 4.3*/
{
    UNUSED_PARAMETER(msr);
    OS_ARCH_DSYNC();
    OS_ASM("MSR primask, %0 " : : "r"(msr));
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Installation interrupted>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchInitCPU>
 */
/*****************************************************************************/
/*PRQA S 3006,3672 ++*/ /*MISRA Dir 4.3,CWE-398*/
FUNC(void, OS_CODE) Os_InterruptInstall(uint8 id, uint8 prio, uint32 srcType, Os_isrhnd isrProc)
{
    (void)srcType;
    /* PRQA S 0303,3345,3442 ++*/ /* MISRA Rule 11.4,CWE-398 */
    OS_INTERRUPT_INSTALL(id, prio, srcType);
    /* PRQA S 0303,3345,3442 --*/ /* MISRA Rule 11.4,CWE-398 */

    /* Save the former handler pointer */
    if (isrProc != NULL_PTR)
    {
        /* Set handler into vector table */
        Os_LocalVector[id] = (uint32)isrProc; /* PRQA S 0305 */ /* MISRA Rule 11.1 */
    }
    OS_ASM("dsb");
    OS_ASM("isb");
}
/*PRQA S 3006,3672 --*/ /*MISRA Dir 4.3,CWE-398*/
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Remove interruption>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchInitCPU>
 */
/*****************************************************************************/
/*PRQA S 3006,3672 ++*/ /*MISRA Dir 4.3,CWE-398*/
FUNC(void, OS_CODE) Os_InterruptUnload(uint8 id, uint8 prio, uint32 srcType, Os_isrhnd isrProc)
{
    UNUSED_PARAMETER(prio);
    UNUSED_PARAMETER(srcType);
    /* PRQA S 3345,0303,0428,3442 ++*/ /* MISRA CWE-398,Rule-11.4,EXP16,CWE-398 */
    OS_INTERRUPT_UNLOAD(id, prio, srcType);
    /* PRQA S 3345,0303,0428,3442 --*/ /* MISRA CWE-398,Rule-11.4,EXP16,CWE-398 */
    /* Clean the former handler pointer */
    if (isrProc != NULL_PTR)
    {
        /* Set handler into vector table */
        Os_LocalVector[id] = (uint32)(Os_IntHandler); /* PRQA S 0305,0428 */ /* MISRA Rule 11.1,EXP16 */
    }
    OS_ASM("dsb");
    OS_ASM("isb");
}
/*PRQA S 3006,3672 --*/ /*MISRA Dir 4.3,CWE-398*/
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Installation interrupt by default>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchInitCPU>
 */
/*****************************************************************************/
FUNC(void, OS_CODE) Os_InterruptInit(void)
{
    uint32 index;

    OS_REG_VTOR = (uint32)&Os_GlobalVector[0u]; /* PRQA S 0303,0306*/ /*Rule-11.4*/
    /* PRQA S 0303 ++ */                                              /* MISRA Rule-11.4 */
    OS_INTERRUPT_SYS_SET_PRIO(OS_SVCall_IRQn, OS_NVIC_CONVERT_SET_PRIO(OS_NVIC_PRIO_MAX));
    OS_INTERRUPT_SYS_SET_PRIO(OS_PendSV_IRQn, OS_NVIC_CONVERT_SET_PRIO(OS_NVIC_PRIO_MAX));
    /* PRQA S 0303 -- */ /* MISRA Rule-11.4 */
    for (index = OS_ISR_EXTERNAL_NUM; index < OS_NVIC_NUM; index++)
    {
        OS_INTERRUPT_DISABLE(index); /* PRQA S 0303,3345,3442 */ /* MISRA Rule 11.4,CWE-398 */
        OS_INTERRUPT_CLEAR_PENDING(index); /* PRQA S 0303 */     /* MISRA Rule 11.4 */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Ipl greater than 0, disable interrupt otherwise enable interrupt>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ipl>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ResumeOSInterrupts and so on>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchSetIpl(Os_IPLType ipl, Os_IsrDescriptionType isrdesc) /*PRQA S 3006*/ /*MISRA Dir 4.3*/
{
    /*add comments to pass QAC.*/
    (void)isrdesc;
    /* Disable interrupt prio - 1,CAN open the max OSinterrupt self*/
    register Os_IPLType converted_prio = OS_NVIC_CONVERT_SET_PRIO(ipl); /* PRQA S 2011 */ /* MISRA CWE-398 */

    OS_ASM("msr basepri, %0" : : "r"(converted_prio));
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get current IPL>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <0>
 * PreCondition         <None>
 * CallByAPI            <SuspendOSInterrupts and so on>
 */
/******************************************************************************/
FUNC(Os_IPLType, OS_CODE) Os_ArchGetIpl(void) /*PRQA S 3006*/ /*MISRA Dir 4.3*/
{
    register Os_IPLType result; /* PRQA S 2011 */ /* MISRA CWE-398 */

    OS_ASM("MRS %0, basepri_max" : "=r"(result));

    result = OS_NVIC_CONVERT_GET_PRIO(result);

    return (result);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Disable the given interrupt source>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-vIsrSrc[in]    <Interrupt source>
 * param-vIsrSrcType[in]<Type of Interrupt source>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <DisableInterruptSource>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_DisableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    UNUSED_PARAMETER(vIsrSrcType);
    OS_INTERRUPT_DISABLE(vIsrSrc); /* PRQA S 0303,3345,3442 */ /* MISRA Rule 11.4,CWE-398 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Clear the pending status of the given interrupt>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-vIsrSrc[in]    <Interrupt source>
 * param-vIsrSrcType[in]<Type of Interrupt source>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <EnableInterruptSource><ClearPendingInterrupt>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ClearPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    UNUSED_PARAMETER(vIsrSrcType);
    OS_INTERRUPT_CLEAR_PENDING(vIsrSrc); /* PRQA S 0303 */ /* MISRA Rule 11.4 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the status of the given interrupt.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <srcAddr,prio,srcType,isrProc>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <>
 */
/******************************************************************************/
FUNC(Os_IsrStateType, OS_CODE) Os_GetIsrSourceState(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    UNUSED_PARAMETER(vIsrSrcType);
    Os_IsrStateType isrSourceState = OS_ISR_DISABLED;
    if (1U == OS_INTERRUPT_CHECK_STATUS(vIsrSrc)) /* PRQA S 0303,3442 */ /* MISRA Rule 11.4,CWE-398 */
    {
        isrSourceState = OS_ISR_ENABLED;
    }
    return isrSourceState;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Enable the given interrupt source vIsrSrc.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <srcAddr,prio,srcType,isrProc>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <>
 */
FUNC(void, OS_CODE) Os_EnableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    UNUSED_PARAMETER(vIsrSrcType);
    OS_INTERRUPT_ENABLE(vIsrSrc); /* PRQA S 0303,3345,3442 */ /* MISRA Rule 11.4,CWE-398 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (CFG_ISR_MAX > 0)
/*****************************************************************************/
/*
 * Brief                <Disable all interrupts in os_app during TerminateApplication.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchInitCPU>
 */
/*****************************************************************************/
FUNC(void, OS_CODE) Os_ArchDisableIntInApp(P2CONST(Os_ApplicationCfgType, AUTOMATIC, OS_VAR) posCurAppCfg)
{
    uint16 i;
    uint16 osIsrRefCnt;
    Os_IsrType osIsrId;
    uint32 osIsrRegVal;

    osIsrRefCnt = posCurAppCfg->OsAppIsrRefCnt;

    for (i = 0u; i < osIsrRefCnt; i++)
    {
        osIsrId = Os_GetObjLocalId(posCurAppCfg->OsAppObjectRef[OBJECT_ISR][i]);

        osIsrRegVal = Os_IsrCfg[osIsrId].OsIsrSrc;

        OS_INTERRUPT_CLEAR_PENDING(osIsrRegVal); /* PRQA S 0303 */     /* MISRA Rule 11.4 */
        OS_INTERRUPT_DISABLE(osIsrRegVal); /* PRQA S 0303,3345,3442 */ /* MISRA Rule 11.4,CWE-398 */
    }
}

#endif /* #if (CFG_ISR_MAX > 0) */ /* PRQA S 2053 */                          /* MISRA Rule 18.8 */
#endif /* #if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC)) */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Os_ArchDispatch_ISR>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
FUNC(void, OS_CODE) Os_ArchDispatch_ISR(void)
{
    /* Call PendSV to switch task */
    OS_ASM("cpsid i                            \n"
           ".set NVIC_INT_CTRL, 0xE000ED04     \n"
           ".set NVIC_PENDSVSET, 0x10000000    \n"
           "ldr r0, =NVIC_INT_CTRL             \n"
           "ldr r1, =NVIC_PENDSVSET            \n"
           "str r1, [r0]                       \n"
           "dsb                                \n");
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
