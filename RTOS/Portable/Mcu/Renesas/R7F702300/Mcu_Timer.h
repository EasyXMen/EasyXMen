/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : Mcu_Timer.h                                                 **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : Timer driver                                                **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
#ifndef MCU_TIMER_H
#define MCU_TIMER_H
#include "Os.h"
/* MSRKCPROT register is used for protection against writing operation to the registers which may have
a material effect on the system so that the application system is not incorrectly stopped due to program
malfunction and the like */
#define OS_MSRKCPROT     (*(volatile uint32*)(0xFF981710U))
#define OS_KCPORT_DIABLE (0xA5A5A500U) /* Disables write access of protected registers */
#define OS_KCPORT_ENABLE (0xA5A5A501U) /* Enables write access of protected registers */

/* This register is used to control the stop modes of the OSTM. */
#define OS_MSR_OSTM (*(volatile uint32*)(0xFF981180U))

/* OSTM0 - OSTM9 register */
#define OS_OSTM0_BASE_ADDR 0xFFBF0000U
#define OS_OSTM1_BASE_ADDR 0xFFBF0100U
#define OS_OSTM2_BASE_ADDR 0xFFBF0200U
#define OS_OSTM3_BASE_ADDR 0xFFBF0300U
#define OS_OSTM4_BASE_ADDR 0xFFBF0400U
#define OS_OSTM5_BASE_ADDR 0xFFBF0500U
#define OS_OSTM6_BASE_ADDR 0xFFBF0600U
#define OS_OSTM7_BASE_ADDR 0xFFBF0700U
#define OS_OSTM8_BASE_ADDR 0xFFBF0800U
#define OS_OSTM9_BASE_ADDR 0xFFBF0900U

/* OSTMn instance */
#define OS_STM0_INSTANCE ((volatile STMn_t*)(OS_OSTM0_BASE_ADDR))
#define OS_STM1_INSTANCE ((volatile STMn_t*)(OS_OSTM1_BASE_ADDR))
#define OS_STM2_INSTANCE ((volatile STMn_t*)(OS_OSTM2_BASE_ADDR))
#define OS_STM3_INSTANCE ((volatile STMn_t*)(OS_OSTM3_BASE_ADDR))
#define OS_STM4_INSTANCE ((volatile STMn_t*)(OS_OSTM4_BASE_ADDR))
#define OS_STM5_INSTANCE ((volatile STMn_t*)(OS_OSTM5_BASE_ADDR))
#define OS_STM6_INSTANCE ((volatile STMn_t*)(OS_OSTM6_BASE_ADDR))
#define OS_STM7_INSTANCE ((volatile STMn_t*)(OS_OSTM7_BASE_ADDR))
#define OS_STM8_INSTANCE ((volatile STMn_t*)(OS_OSTM8_BASE_ADDR))
#define OS_STM9_INSTANCE ((volatile STMn_t*)(OS_OSTM9_BASE_ADDR))

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
/* enable OSTMx */
#define OS_ARCH_ENABLE_SYSTIMER(Pen) systickTimerArray[Pen]->TS = (uint8)0x01
#define Os_ClearSysTimer()
/* empty for U2Ax */
#define OS_ARCH_CLEAR_SYSTIMER(Pen)
#endif /* TRUE == CFG_SYSTEM_TIMER_ENABLE */

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
/* enable STM0 CNT3 Channel A */
#define OS_ARCH_ENABLE_TPTIMER(Pen) tpTimerkArray[Pen]->TS = (uint8)0x01

/* empty for U2Ax */
#define OS_ARCH_CLEAR_TPTIMER(Pen)
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

#if (TRUE == CFG_OS_MONITOR_ENABLE)
#define OS_SYSTEM_TIMER_MAX_TICK_VALUE      0xFFFFFFFFU
#define OS_SYSTEM_TIMER_REG_PER_MS_VALUE    400000U
#define Os_ArchTptmPtr                      ((Os_ArchTptmTypeRef)(0xFFFBB000U))
#endif /* TRUE == CFG_OS_MONITOR_ENABLE */

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
typedef struct
{
    // Interval Timer Control Registers
    volatile uint32 TPTMSIRUN;         // 0x000: Counter Start Register
    volatile uint32 TPTMSIRRUN;        // 0x004: Counter Restart Register
    volatile uint32 TPTMSISTP;         // 0x008: Counter Stop Register
    volatile uint32 TPTMSISTR;         // 0x00C: Counter Status Register
    volatile uint32 TPTMSIIEN;         // 0x010: Interrupt Enable Register
    volatile uint32 TPTMSIUSTR;        // 0x014: Underflow Status Register
    volatile uint32 TPTMSIDIV;         // 0x018: Divider Register
    volatile uint32 RESERVED1;         // 0x01C: Reserved
    // Free-run Timer Control Registers
    volatile uint32 TPTMSFRUN;         // 0x020: Counter Start Register
    volatile uint32 TPTMSFRRUN;        // 0x024: Counter Restart Register
    volatile uint32 TPTMSFSTP;         // 0x028: Counter Stop Register
    volatile uint32 TPTMSFSTR;         // 0x02C: Counter Status Register
    volatile uint32 TPTMSFDIV;         // 0x030: Divider Register
    volatile uint32 RESERVED2[3];      // 0x034-0x03F: Reserved
    // Up Timer Control Registers
    volatile uint32 TPTMSURUN;         // 0x040: Counter Start Register
    volatile uint32 TPTMSURRUN;        // 0x044: Counter Restart Register
    volatile uint32 TPTMSUSTP;         // 0x048: Counter Stop Register
    volatile uint32 TPTMSUSTR;         // 0x04C: Counter Status Register
    volatile uint32 TPTMSUIEN;         // 0x050: Interrupt Enable Register
    volatile uint32 RESERVED3;         // 0x054: Reserved
    volatile uint32 TPTMSUDIV;         // 0x058: Divider Register
    volatile uint32 TPTMSUTRG;         // 0x05C: Trigger Select Register
    volatile uint32 RESERVED4[8];      // 0x060-0x07F: Reserved
    // Interval Timer Counter/Load Registers
    volatile uint32 TPTMSICNT0;        // 0x080: Channel 0 Counter Register
    volatile uint32 TPTMSILD0;         // 0x084: Channel 0 Load Register
    volatile uint32 TPTMSICNT1;        // 0x088: Channel 1 Counter Register
    volatile uint32 TPTMSILD1;         // 0x08C: Channel 1 Load Register
    volatile uint32 RESERVED5[4];      // 0x090-0x09F: Reserved
    // Free-run Timer Counter Register
    volatile uint32 TPTMSFCNT;         // 0x0A0: Counter Register
    volatile uint32 RESERVED6[7];      // 0x0A4-0x0BF: Reserved
    // Up Timer 0 Counter/Compare Registers
    volatile uint32 TPTMSUCNT0;        // 0x0C0: Channel 0 Counter Register
    volatile uint32 TPTMSUCMP00;       // 0x0C4: Compare Value 0 Register
    volatile uint32 TPTMSUCMP01;       // 0x0C8: Compare Value 1 Register
    volatile uint32 TPTMSUCMP02;       // 0x0CC: Compare Value 2 Register
    volatile uint32 TPTMSUCMP03;       // 0x0D0: Compare Value 3 Register
    volatile uint32 RESERVED7[3];      // 0x0D4-0x0DF: Reserved
    // Up Timer 1 Counter/Compare Registers
    volatile uint32 TPTMSUCNT1;        // 0x0E0: Channel 1 Counter Register
    volatile uint32 TPTMSUCMP10;       // 0x0E4: Compare Value 0 Register
    volatile uint32 TPTMSUCMP11;       // 0x0E8: Compare Value 1 Register
    volatile uint32 TPTMSUCMP12;       // 0x0EC: Compare Value 2 Register
    volatile uint32 TPTMSUCMP13;       // 0x0F0: Compare Value 3 Register
} Os_ArchTptmType,*Os_ArchTptmTypeRef;

typedef struct __STMn
{
    volatile uint32 CMP;         /* CMP              */
    volatile uint32 CNT;         /* CNT              */
    volatile uint8 TO;           /* TO               */
    volatile uint8 dummy1101[3]; /* Reserved         */
    volatile uint8 TOE;          /* TOE              */
    volatile uint8 dummy1102[3]; /* Reserved         */
    const volatile uint8 TE;     /* TE               */
    volatile uint8 dummy1103[3]; /* Reserved         */
    volatile uint8 TS;           /* TS               */
    volatile uint8 dummy1104[3]; /* Reserved         */
    volatile uint8 TT;           /* TT               */
    volatile uint8 dummy1105[7]; /* Reserved         */
    volatile uint8 CTL;          /* CTL              */
} STMn_t;

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
extern volatile STMn_t* systickTimerArray[OS_AUTOSAR_CORES];
#endif
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
extern volatile STMn_t* tpTimerkArray[OS_AUTOSAR_CORES];
#endif
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
FUNC(void, OS_CODE) Os_ArchInitCPU(void);

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
extern FUNC(void, OS_CODE) Os_ArchInitSystemTimer(void);
#endif /* #if (TRUE == CFG_SYSTEM_TIMER_ENABLE) */

#if (TRUE == CFG_OS_MONITOR_ENABLE)
extern FUNC(uint32, OS_CODE) Os_ArchLoadRatioInit(void);
extern FUNC(uint32, OS_CODE) Os_ArchGetTimeTicks(void);
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
extern FUNC(void, OS_CODE) Os_ArchInitTimingProtTimer(void);
#endif /* #if (TRUE == CFG_SYSTEM_TIMER_ENABLE) */

#endif
/*=======[E N D   O F   F I L E]==============================================*/
