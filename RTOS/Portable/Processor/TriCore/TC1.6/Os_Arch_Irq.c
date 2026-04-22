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
 *
 ************************************************************************************************************************
 **
 **  @file               : Os_Arch_Irq.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Arch source file for Irq API implementations
 **
 ***********************************************************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Arch_Processor.h"
#include "Os_Arch_Irq.h"
#include "Os_Interrupt.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/
/* PRQA S 0791 ++ */ /* VL_Os_0791 */
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
Os_isrhnd Cdisptab[MAX_INTRS]; /* PRQA S 3672 */ /* VL_Os_3672 */
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
uint32* Os_IsrNestPcxStack; /* PRQA S 3432, 1533 */ /* VL_Os_3432, VL_Os_1533 */
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
volatile uint32 Os_ArchSp;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

/* PRQA S 0791-- */


/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/* This is the default interrupt vector table, which consists of 32
   entries, each consisting of eight words (32 bytes).  The table
   must be put into the section ".inttab", must be aligned to a
   8 KB boundary, and the entry symbol (virtual interrupt 0)
   must be called "TriCore_int_table", as it is referenced below
   in order to program the BIV register.  Note: This is only a
   sample implementation which can handle up to 31 interrupts.
   If you need more interrupts, you need to expand the table in the
   obvious way and adjust the alignment as explained in the "TriCore
   uC-DSP Architecture Manual".  */
/* PRQA S 1006++ */                                              /* VL_Os_1006 */
OS_ARCH_DECLARE_INT_TAB()

OS_ARCH_DEFINE_INT_0()

DEFINE_INT(1);
DEFINE_INT(2);
DEFINE_INT(3);
DEFINE_INT(4);
DEFINE_INT(5);
DEFINE_INT(6);
DEFINE_INT(7);
DEFINE_INT(8);
DEFINE_INT(9);
DEFINE_INT(10);
DEFINE_INT(11);
DEFINE_INT(12);
DEFINE_INT(13);
DEFINE_INT(14);
DEFINE_INT(15);
DEFINE_INT(16);
DEFINE_INT(17);
DEFINE_INT(18);
DEFINE_INT(19);
DEFINE_INT(20);
DEFINE_INT(21);
DEFINE_INT(22);
DEFINE_INT(23);
DEFINE_INT(24);
DEFINE_INT(25);
DEFINE_INT(26);
DEFINE_INT(27);
DEFINE_INT(28);
DEFINE_INT(29);
DEFINE_INT(30);
DEFINE_INT(31);
#if (MAX_INTRS > 32)
DEFINE_INT(32);
DEFINE_INT(33);
DEFINE_INT(34);
DEFINE_INT(35);
DEFINE_INT(36);
DEFINE_INT(37);
DEFINE_INT(38);
DEFINE_INT(39);
DEFINE_INT(40);
DEFINE_INT(41);
DEFINE_INT(42);
DEFINE_INT(43);
DEFINE_INT(44);
DEFINE_INT(45);
DEFINE_INT(46);
DEFINE_INT(47);
DEFINE_INT(48);
DEFINE_INT(49);
DEFINE_INT(50);
DEFINE_INT(51);
DEFINE_INT(52);
DEFINE_INT(53);
DEFINE_INT(54);
DEFINE_INT(55);
DEFINE_INT(56);
DEFINE_INT(57);
DEFINE_INT(58);
DEFINE_INT(59);
DEFINE_INT(60);
DEFINE_INT(61);
DEFINE_INT(62);
DEFINE_INT(63);
DEFINE_INT(64);
DEFINE_INT(65);
DEFINE_INT(66);
DEFINE_INT(67);
DEFINE_INT(68);
DEFINE_INT(69);
DEFINE_INT(70);
DEFINE_INT(71);
DEFINE_INT(72);
DEFINE_INT(73);
DEFINE_INT(74);
DEFINE_INT(75);
DEFINE_INT(76);
DEFINE_INT(77);
DEFINE_INT(78);
DEFINE_INT(79);
DEFINE_INT(80);
DEFINE_INT(81);
DEFINE_INT(82);
DEFINE_INT(83);
DEFINE_INT(84);
DEFINE_INT(85);
DEFINE_INT(86);
DEFINE_INT(87);
DEFINE_INT(88);
DEFINE_INT(89);
DEFINE_INT(90);
DEFINE_INT(91);
DEFINE_INT(92);
DEFINE_INT(93);
DEFINE_INT(94);
DEFINE_INT(95);
DEFINE_INT(96);
DEFINE_INT(97);
DEFINE_INT(98);
DEFINE_INT(99);
DEFINE_INT(100);
DEFINE_INT(101);
DEFINE_INT(102);
DEFINE_INT(103);
DEFINE_INT(104);
DEFINE_INT(105);
DEFINE_INT(106);
DEFINE_INT(107);
DEFINE_INT(108);
DEFINE_INT(109);
DEFINE_INT(110);
DEFINE_INT(111);
DEFINE_INT(112);
DEFINE_INT(113);
DEFINE_INT(114);
DEFINE_INT(115);
DEFINE_INT(116);
DEFINE_INT(117);
DEFINE_INT(118);
DEFINE_INT(119);
DEFINE_INT(120);
DEFINE_INT(121);
DEFINE_INT(122);
DEFINE_INT(123);
DEFINE_INT(124);
DEFINE_INT(125);
DEFINE_INT(126);
DEFINE_INT(127);
DEFINE_INT(128);
DEFINE_INT(129);
DEFINE_INT(130);
DEFINE_INT(131);
DEFINE_INT(132);
DEFINE_INT(133);
DEFINE_INT(134);
DEFINE_INT(135);
DEFINE_INT(136);
DEFINE_INT(137);
DEFINE_INT(138);
DEFINE_INT(139);
DEFINE_INT(140);
DEFINE_INT(141);
DEFINE_INT(142);
DEFINE_INT(143);
DEFINE_INT(144);
DEFINE_INT(145);
DEFINE_INT(146);
DEFINE_INT(147);
DEFINE_INT(148);
DEFINE_INT(149);
DEFINE_INT(150);
DEFINE_INT(151);
DEFINE_INT(152);
DEFINE_INT(153);
DEFINE_INT(154);
DEFINE_INT(155);
DEFINE_INT(156);
DEFINE_INT(157);
DEFINE_INT(158);
DEFINE_INT(159);
DEFINE_INT(160);
DEFINE_INT(161);
DEFINE_INT(162);
DEFINE_INT(163);
DEFINE_INT(164);
DEFINE_INT(165);
DEFINE_INT(166);
DEFINE_INT(167);
DEFINE_INT(168);
DEFINE_INT(169);
DEFINE_INT(170);
DEFINE_INT(171);
DEFINE_INT(172);
DEFINE_INT(173);
DEFINE_INT(174);
DEFINE_INT(175);
DEFINE_INT(176);
DEFINE_INT(177);
DEFINE_INT(178);
DEFINE_INT(179);
DEFINE_INT(180);
DEFINE_INT(181);
DEFINE_INT(182);
DEFINE_INT(183);
DEFINE_INT(184);
DEFINE_INT(185);
DEFINE_INT(186);
DEFINE_INT(187);
DEFINE_INT(188);
DEFINE_INT(189);
DEFINE_INT(190);
DEFINE_INT(191);
DEFINE_INT(192);
DEFINE_INT(193);
DEFINE_INT(194);
DEFINE_INT(195);
DEFINE_INT(196);
DEFINE_INT(197);
DEFINE_INT(198);
DEFINE_INT(199);
DEFINE_INT(200);
DEFINE_INT(201);
DEFINE_INT(202);
DEFINE_INT(203);
DEFINE_INT(204);
DEFINE_INT(205);
DEFINE_INT(206);
DEFINE_INT(207);
DEFINE_INT(208);
DEFINE_INT(209);
DEFINE_INT(210);
DEFINE_INT(211);
DEFINE_INT(212);
DEFINE_INT(213);
DEFINE_INT(214);
DEFINE_INT(215);
DEFINE_INT(216);
DEFINE_INT(217);
DEFINE_INT(218);
DEFINE_INT(219);
DEFINE_INT(220);
DEFINE_INT(221);
DEFINE_INT(222);
DEFINE_INT(223);
DEFINE_INT(224);
DEFINE_INT(225);
DEFINE_INT(226);
DEFINE_INT(227);
DEFINE_INT(228);
DEFINE_INT(229);
DEFINE_INT(230);
DEFINE_INT(231);
DEFINE_INT(232);
DEFINE_INT(233);
DEFINE_INT(234);
DEFINE_INT(235);
DEFINE_INT(236);
DEFINE_INT(237);
DEFINE_INT(238);
DEFINE_INT(239);
DEFINE_INT(240);
DEFINE_INT(241);
DEFINE_INT(242);
DEFINE_INT(243);
DEFINE_INT(244);
DEFINE_INT(245);
DEFINE_INT(246);
DEFINE_INT(247);
DEFINE_INT(248);
DEFINE_INT(249);
DEFINE_INT(250);
DEFINE_INT(251);
DEFINE_INT(252);
DEFINE_INT(253);
DEFINE_INT(254);
DEFINE_INT(255);
#endif /* MAX_INTRS > 32 */

OS_ARCH_DECLARE_TEXT_SECTION();

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 1006-- */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Ipl greater than 0, disable interrupt otherwise enable
 *                       interrupt>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ipl>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ResumeOSInterrupts and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_SetIpl(Os_IPLType ipl, Os_IsrDescriptionType isrDesc) /* PRQA S 3006 */ /* VL_Os_3006 */
{
    uint32 temp = (Os_ArchMsrType)OS_ARCH_MFCR(OS_REG_ICR);
    temp        = temp & ARCH_IPI_MASK; /* PRQA S 1252 */ /* VL_Os_1252 */
    temp        = temp | (uint32)ipl;
    OS_ARCH_MTCR(OS_REG_ICR, (uint32)temp) /* PRQA S 1006 */ /* VL_Os_1006 */

    (void)isrDesc;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <get the IPL>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <0>
 * PreCondition         <None>
 * CallByAPI            <SuspendOSInterrupts and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
Os_IPLType Os_Hal_GetIpl(void)
{
    uint32 temp = (uint32)OS_ARCH_MFCR(OS_REG_ICR);

    return (Os_IPLType)(temp & ARCH_ICR_IPI_MASK); /* PRQA S 1252 */ /* VL_Os_1252 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Save ccr to variable msr and disable maskable interrupt>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <SuspendAllInterrupts>
 * REQ ID               <None>
 */
/******************************************************************************/
Os_ArchMsrType Os_Hal_SuspendInt(void)
{
    Os_ArchMsrType temp;
    uint32         low_bit = 1UL;

    temp = (Os_ArchMsrType)OS_ARCH_MFCR(OS_REG_ICR) & (Os_ArchMsrType)(low_bit << ARCH_ICR_CCR_BIT);
    Os_Hal_DisableInt();
    return temp;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <transfer variable msr back to ccr>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ResumeAllInterrupts>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_RestoreInt(Os_ArchMsrType msr) /* PRQA S 3006 */ /* VL_Os_3006 */
{
    Os_ArchMsrType temp = OS_ARCH_MFCR(OS_REG_ICR);
    temp                = temp | msr;
    OS_ARCH_MTCR(OS_REG_ICR, (uint32)temp) /* PRQA S 1006 */ /* VL_Os_1006 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialize interrupts and traps>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <>
 */
/******************************************************************************/
void Os_ArchInterruptInit(void) /* PRQA S 3006, 1532 */ /* VL_Os_3006, VL_QAC_OneFunRef */
{
    uint32* vptr; /* PRQA S 3203, 3678 */ /* VL_Os_3203 */ /* VL_Os_3678 */
    uint32  vecno;

    /* Set BTV and BIV registers.  */
    unlock_wdtcon();
    /* PRQA S 1006, 2983, 3138 ++ */ /* VL_Os_1006, VL_Os_2983, VL_Os_3138 */
    OS_ARCH_SET_BTV();
    OS_ARCH_SET_BIV();
    /* PRQA S 1006, 2983, 3138 -- */
    lock_wdtcon();

    /* Initialize the trap handlers.  */
    Tdisptab[TRAP_0_NUM]  = Trap0;
    Tdisptab[TRAP_1_NUM]  = Trap1;
    Tdisptab[TRAP_2_NUM]  = Trap2;
    Tdisptab[TRAP_3_NUM]  = Trap3;
    Tdisptab[TRAP_4_NUM]  = Trap4;
    Tdisptab[TRAP_5_NUM]  = Trap5;
    Tdisptab[SYSCALL_NUM] = Os_SystemCallTrap;
    Tdisptab[TRAP_7_NUM]  = Trap7;

    /* Initialize the interrupt handlers.  */
    for (vecno = 0u; vecno < MAX_INTRS; vecno++) /* PRQA S 1252 */ /* VL_Os_1252 */
    {
        Cdisptab[vecno] = Os_IntHandler;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 3219, 3006, 3450 ++ */ /* VL_Os_3219, VL_Os_3006, VL_Os_3450 */
OS_LOCAL void OS_USED Os_ArchTprotTerminateIsrProcess(void)
/* PRQA S 3219, 3006, 3450 -- */
{
    /* PRQA S 0310, 3442, 0306 ++ */ /* VL_Os_0310, VL_Os_3442, VL_Os_0306 */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1006, 2743 ++ */ /* VL_Os_1006, VL_Os_2743*/
    Os_ArchTprotTerminateIsr();
    /* PRQA S 1006, 2743 -- */
    /* PRQA S 3138, 3141 -- */
    /* PRQA S 0310, 3442, 0306 -- */
    Os_ArchReturnFromTrapOrISR();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 3219, 3006, 3450, 6030 ++ */ /* VL_Os_3219, VL_Os_3006, VL_Os_3450, VL_MTR_Os_STMIF */
OS_LOCAL void OS_USED Os_ArchProtectionHookTerminateNestedIsrProcess(void)
/* PRQA S 3219, 3006, 3450, 6030 -- */
{
    /* PRQA S 0310, 3442, 0306 ++ */ /* VL_Os_0310, VL_Os_3442, VL_Os_0306 */
    /* PRQA S 3138,3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 2995, 1006, 2743 ++ */ /* VL_Os_2995, VL_Os_1006, VL_Os_2743 */
    Os_ArchProtectionHookTerminateNestedIsr();
    /* PRQA S 2995, 1006, 2743 -- */
    /* PRQA S 3138,3141 -- */               
    /* PRQA S 0310, 3442, 0306 -- */
    Os_ArchReturnFromTrapOrISR();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 6030, 6070, 3006 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCAL, VL_Os_3006 */
void Os_ArchResumeContextFromISR2(void)
/* PRQA S 6030, 6070, 3006 -- */
{
    if (OS_PROTECTION_TERMINATE_TPROT_ISR == Os_Kernel_GetAndComparTerminateObject(OS_PROTECTION_TERMINATE_TPROT_ISR))
    {
        OS_ASM("j Os_ArchTprotTerminateIsrProcess");/* PRQA S 1006 */ /* VL_Os_1006 */
    }
    else if (OS_PROTECTION_TERMINATE_NESTED_ISR == Os_Kernel_GetAndComparTerminateObject(OS_PROTECTION_TERMINATE_NESTED_ISR))
    {
        OS_ASM("j Os_ArchProtectionHookTerminateNestedIsrProcess"); /* PRQA S 1006 */ /* VL_Os_1006 */
    }
    /* Normal return to Task */
    else if (0U == Os_Kernel_GetIntNestNumISR2()) /* PRQA S 2004 */ /* VL_Os_2004 */
    {
        /* PRQA S 1006, 3138, 3141 ++ */ /* VL_Os_1006, VL_Os_3138, VL_Os_3141 */
        Os_CLEAR_CDC_ISR2_EPILOGUE();
        /* PRQA S 1006, 3138, 3141 -- */
        /* PRQA S 0310, 0306, 3455 ++ *//* VL_Os_0310, VL_Os_0306, VL_Os_3455 */
        /* PRQA S 3138, 3442, 2743 ++ *//* VL_Os_3138, VL_Os_3442, VL_Os_2743 */
        /* PRQA S 2995, 1006 ++ */ /* VL_Os_2995, VL_Os_1006 */
        OS_ARCH_RESUME_CONTEXT(Os_Kernel_GetSysRunningTaskID());
        /* PRQA S 2995, 1006 -- */
        /* PRQA S 0310, 0306, 3455 -- */
        /* PRQA S 3138, 3442, 2743 -- */
    }
    Os_ArchReturnFromTrapOrISR();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
