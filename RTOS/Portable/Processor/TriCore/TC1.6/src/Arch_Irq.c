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
/*******************************************************************************
**                                                                            **
**  FILENAME    : Arch_Irq.c                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Interrupt Request Managment                                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"
#include "Arch_Trap.h"
/* PRQA S 1006 EOF */ /* VL_Os_1006 */
/*=======[M A C R O S]========================================================*/
#define ICR_MASK      (0xFFFFFF00u)
#define IPL_MASK      (0xFFu)
#define OS_ICR_IE_BIT (15U)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
/* PRQA S 0791++ */ /* VL_Os_0791 */

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/* PRQA S 1514 ++ */                                /* VL_Os_1514 */
Os_isrhnd Cdisptab[OS_MAX_INTRS]; /* PRQA S 3672 */ /* VL_Os_3672 */
/* PRQA S 1514 -- */
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
volatile boolean Os_TprotTerminateIsr;
#define OS_STOP_SEC_VAR_CLONE_8
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
volatile boolean Os_TprotTerminateTask;
#define OS_STOP_SEC_VAR_CLONE_8
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
uint32* Os_IsrNestPcxStack; /* PRQA S 3432 */ /* VL_Os_3432 */
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 1751, 1514 ++ */ /* VL_Os_1751, VL_Os_1514 */
Os_CallLevelType Os_SaveLevelISR1;
/* PRQA S 1751, 1514 -- */
#define OS_STOP_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
volatile uint32 Os_ArchSp;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/* PRQA S 0791-- */

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#if (((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC)) && (CFG_ISR_MAX > 0) && (TRUE == CFG_INT_NEST_ENABLE))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static StatusType Os_ArchTerminateOneNestIsr(Os_IsrType OsIsrID);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

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
OS_ASM(".section .inttab, \"ax\", @progbits\n" /* PRQA S 0286 */ /* VL_Os_0286 */
       ".align 13\n"
       ".global TriCore_int_table\n"
       "TriCore_int_table:\n");

OS_ASM("debug        # int 0\n"
       ".align 5\n");

/* Assembly language, defining interrupts. */
/* PRQA S 3412, 0884, 0341 ++ */ /* VL_Os_3412, VL_Os_0884, VL_Os_0341 */
#define DEFINE_INT(i)                           \
    OS_ASM(".global __interrupt_" #i);          \
    OS_ASM("__interrupt_" #i ":");              \
    OS_ASM("svlcx");                            \
    OS_ASM("movh.a %a15,hi:Cdisptab+4*" #i);    \
    OS_ASM("lea %a15,[%a15]lo:Cdisptab+4*" #i); \
    OS_ASM("ld.a %a14,[%a15+]");                \
    OS_ASM("ld.w %d4,[%a15]");                  \
    OS_ASM("ji %a14");                          \
    OS_ASM(".align 5")
/* PRQA S 3412, 0884, 0341 -- */
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
#if (OS_MAX_INTRS > 32)
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
#endif /* OS_MAX_INTRS > 32 */
OS_ASM(".text");
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

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
void Os_ArchSetIpl(Os_IPLType ipl, Os_IsrDescriptionType isrdesc) /* PRQA S 3006 */ /* VL_Os_3006 */
{
    uint32     temp     = (Os_ArchMsrType)OS_ARCH_MFCR(OS_REG_ICR);
    Os_IPLType temp_ipl = (uint16)(temp & (uint32)IPL_MASK); /* PRQA S 1258 */ /* VL_Os_1258 */
    temp                = temp & ICR_MASK;
    temp                = temp | (uint32)ipl;
    OS_ARCH_MTCR(OS_REG_ICR, (uint32)temp); /* PRQA S 3138 */ /* VL_Os_3138 */
    Os_SaveOsIntNested = temp_ipl;

    (void)isrdesc;
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
Os_IPLType Os_ArchGetIpl(void)
{
    uint32 temp = (uint32)OS_ARCH_MFCR(OS_REG_ICR);

    return (Os_IPLType)(temp & IPL_MASK);
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
void Os_ArchSuspendInt(
    /* PRQA S 3432 ++*/ /* VL_Os_3432 */
    Os_ArchMsrType* msr
    /* PRQA S 3432 --*/
)
{
    Os_ArchMsrType temp;
    /* PRQA S 1258 ++ */ /* VL_Os_1258 */
    temp = (Os_ArchMsrType)OS_ARCH_MFCR(OS_REG_ICR) & (Os_ArchMsrType)((uint32)1U << (uint32)OS_ICR_IE_BIT);
    /* PRQA S 1258 -- */
    Os_ArchDisableInt();
    *msr = temp;
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
void Os_ArchRestoreInt(Os_ArchMsrType msr) /* PRQA S 3006 */ /* VL_Os_3006 */
{
    Os_ArchMsrType temp = OS_ARCH_MFCR(OS_REG_ICR);
    temp                = temp | msr;
    OS_ARCH_MTCR(OS_REG_ICR, (uint32)temp); /* PRQA S 3138 */ /* VL_Os_3138 */
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
 * CallByAPI            <Os_ArchInitCPU>
 * REQ ID               <>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */                          /* VL_QAC_OneFunRef */
void Os_InterruptInit(void) /* PRQA S 3006 */ /* VL_Os_3006 */
/* PRQA S 1532 -- */
{
    /* PRQA S 3432, 3678, 3203 ++*/ /* VL_Os_3432, VL_Os_3678, VL_Os_3203 */
    uint32* vptr;
    /* PRQA S 3432, 3678, 3203 --*/
    uint32 vecno;

    /* Set BTV and BIV registers.  */
    unlock_wdtcon();
    vptr = TriCore_trap_table;
    OS_ASM volatile("mtcr $btv,%0" : : "d"(vptr)); /* PRQA S 0286 */ /* VL_Os_0286 */
    vptr = TriCore_int_table; /* PRQA S 2983 */                      /* VL_Os_2983 */
    OS_ASM volatile("mtcr $biv,%0" : : "d"(vptr)); /* PRQA S 0286 */ /* VL_Os_0286 */
    lock_wdtcon();

    /* Initialize the trap handlers.  */
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    Tdisptab[0] = Trap0;
    Tdisptab[1] = Trap1;
    Tdisptab[2] = Trap2;
    Tdisptab[3] = Trap3;
    Tdisptab[4] = Trap4;
    Tdisptab[5] = Trap5;
    Tdisptab[6] = Os_SystemCallTrap;
    Tdisptab[7] = Trap7;
    /* PRQA S 3120 -- */

    Os_IntNestISR2 = 0u;

    /* Initialize the interrupt handlers.  */
    for (vecno = 0u; vecno < OS_MAX_INTRS; vecno++)
    {
        Cdisptab[vecno] = Os_IntHandler;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (CFG_ISR_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
#if (TRUE == CFG_INT_NEST_ENABLE)
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
 * REQ ID               <None>
 */
/******************************************************************************/
static StatusType Os_ArchTerminateOneNestIsr(Os_IsrType OsIsrID)
{
    uint8      i;
    StatusType Status = E_OK;
    for (i = 0u; i < Os_IntNestISR2; i++)
    {
        /* find out the Isr nested position and reclaim its context saving area */
        if (OsIsrID == Os_SCB.sysIsrNestQueue[i])
        {
            /* Reclaim csa list used by this isr. */
            /* if OsIsrChkindex == Os_IntNestISR2, the isr is the innermost_isr,
             * can not reclaim csas here */
            /* PRQA S 0306,0310 ++*/ /* VL_Os_0306, VL_Os_0310 */
            Os_ArchReclaimIsrCsasTermApp(Os_IsrNestPcxStack[i + 1u], Os_IsrNestPcxStack[i]);
            /* PRQA S 0306,0310 --*/

            break;
        }
    }

    /* Not find out the Isr Id in the IsrStack from the SCB,
     * not include outermost_isr. */
    if (i >= Os_IntNestISR2)
    {
        Status = E_NOT_OK;
    }
    else
    {
        /* Moving the following nested ISR2s to shift one position left */
        while (i < (Os_IntNestISR2 - 1u))
        {
            Os_SCB.sysIsrNestQueue[i] = Os_SCB.sysIsrNestQueue[i + 1u];

            if (i < (Os_IntNestISR2 - 2u))
            {
                Os_IsrNestPcxStack[i + 1u] = Os_IsrNestPcxStack[i + 2u];
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
/******************************************************************************/
/*
 * Brief                <Terminate the ISR objects of the calling application>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <OsIsrID>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_TerminateISRObjectHandler>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_ArchAppTerminateIsrProc(Os_IsrType OsIsrID)
{
    uint32 ulRet = E_OK;

    /* Isr_nest process. */
    /* Just find out the nested corresponding ISR2s(excluding the running one) */
    if (OsIsrID != Os_SCB.sysRunningIsrCat2Id)
    {
        /* Terminate one isr */
        ulRet = Os_ArchTerminateOneNestIsr(OsIsrID);

        if ((uint32)E_OK == ulRet)
        {
            if (Os_IntNestISR2 > 0u)
            {
                Os_IntNestISR2--;
            }

            if (Os_SCB.sysDispatchLocker > 0u)
            {
                /*Os_SCB.sysDispatchLocker--*/
                Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker - 1u;
            }
        }
    }

    return;
}
#endif /* TRUE == CFG_INT_NEST_ENABLE */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_ISR_MAX > 0 */
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Return from ISR1>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1503 ++ */                           /* VL_Os_1503 */
void Os_ReturnFromISR1(void) /* PRQA S 3006 */ /* VL_Os_3006 */
/* PRQA S 1503 -- */
{
#if (TRUE == CFG_INT_NEST_ENABLE)
    if (0U == Os_IntNestISR1)
#endif
    {
        Os_SCB.sysOsLevel = Os_SaveLevelISR1;
    }
    OS_ARCH_DSYNC();
    OS_ASM("rslcx");
    OS_ASM("nop");
    OS_ASM("rfe");
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Return from ISR2>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_ReturnFromISR2(void)
{
    if (TRUE == Os_TprotTerminateTask)
    {
        Os_ArchReclaimCsas(Os_SCB.sysRunningTaskID);
        Os_TprotTerminateTask = FALSE;
    }

    Os_ExitISR2();
    if (0U == Os_IntNestISR2)
    {
        Os_TprotTerminateIsr = FALSE;
        Os_CLEAR_CDC_ISR2_EPILOGUE();
        OS_ARCH_RESUME_CONTEXT();
    }
    else
    {
        if (TRUE == Os_TprotTerminateIsr)
        {
            Os_IntNestISR2--;
            Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker - 1u;
            Os_TprotTerminateIsr     = FALSE;
            if (0U == Os_IntNestISR2)
            {
                Os_SCB.sysOsLevel = OS_LEVEL_TASK;
            }
            Os_ArchReclaimIsrCsas(Os_IsrNestPcxStack[Os_IntNestISR2 + 1u], Os_IsrNestPcxStack[Os_IntNestISR2]);
            OS_ARCH_MTCR(OS_REG_PCX, Os_IsrNestPcxStack[Os_IntNestISR2]);
        }
    }
    OS_ARCH_DSYNC();
    OS_ASM("rslcx");
    OS_ASM("nop");
    OS_ASM("rfe");
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Resume context>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1503 ++ */                          /* VL_Os_1503 */
void Os_ResumeContext(void) /* PRQA S 3006 */ /* VL_Os_3006 */
/* PRQA S 1503 -- */
{
    OS_ARCH_MTCR(OS_REG_PCX, Os_TaskCBExt[Os_SCB.sysRunningTaskID].pcx); /* PRQA S 3138 */ /* VL_Os_3138 */
    OS_ARCH_DSYNC();
    OS_ASM("rslcx");
    OS_ASM("nop");
    OS_ASM("rfe");
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Update Psw>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_UpdatePsw(void)
/* PRQA S 1532 -- */
{
    Os_ArchCsaType* csa = OS_PCX_TO_EA(OS_ARCH_MFCR(OS_REG_PCX)); /* PRQA S 0306 */ /* VL_Os_0306 */
/* PRQA S 3120 ++ */                                                                /* VL_QAC_MagicNum */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
    csa->reg[1] &= 0xFFFFCFFFu;
    csa->reg[1] |= Os_PSW_PRS; /* SET PSW.PRS  */
#endif

    if (TRUE != Os_AppCfg[Os_SCB.sysRunningAppID].OsTrusted)
    {
        csa->reg[1] &= 0xfffff3ffu; /* CLEAR PSW.IO  */
        csa->reg[1] |= 0x00000400u; /* SET PSW.IO  */
    }
    /* PRQA S 3120 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

/*=======[E N D   O F   F I L E]==============================================*/
