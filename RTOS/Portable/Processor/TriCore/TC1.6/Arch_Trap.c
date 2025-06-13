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
**  FILENAME    : Arch_Trap.c                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : functions related to the processor exception                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"
#include "Arch_Trap.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
/* TriCore trap table */
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/* This array holds the functions to be called when a trap occurs. */
VAR(Os_traphnd, OS_VAR)
Tdisptab[OS_MAX_TRAPS];
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/* This is the default trap vector table, which consists of eight
   entries, each consisting of eight words (32 bytes).  The table
   must be put into the section ".traptab", must be aligned to a
   256-byte boundary, and the entry symbol (trap class 0 -- reset)
   must be called "TriCore_trap_table", as it is referenced below
   in order to program the BTV register.  */
OS_ASM(".section .traptab, \"ax\", @progbits\n"
       ".align 8\n"
       ".global TriCore_trap_table\n"
       "TriCore_trap_table:\n");

/*change calli to ji*/
/* Assembly language, define trap. */
#define DEFINE_TRAP(i)                               \
    OS_ASM(".global __trap_" #i);                    \
    OS_ASM("__trap_" #i ":");                        \
    OS_ASM("svlcx");                                 \
    OS_ASM("movh.a %a15,hi:Tdisptab+(4*" #i ")");    \
    OS_ASM("ld.w %d4,[%a15]lo:Tdisptab+(4*" #i ")"); \
    OS_ASM("mov.a %a15, %d4");                       \
    OS_ASM("mov %d4,%d15");                          \
    OS_ASM("ji %a15");                               \
    OS_ASM(".align 5")

DEFINE_TRAP(0); /* trap class 0 (Reset) */
DEFINE_TRAP(1); /* trap class 1 (Internal Protection Traps) */
DEFINE_TRAP(2); /* trap class 2 (Instruction Errors) */
DEFINE_TRAP(3); /* trap class 3 (Context Management) */
DEFINE_TRAP(4); /* trap class 4 (System Bus and Peripheral Errors) */
DEFINE_TRAP(5); /* trap class 5 (Assertion Traps) */
DEFINE_TRAP(6); /* trap class 6 (System Call) */
DEFINE_TRAP(7); /* trap class 7 (Non-Maskable Interrupt) */
OS_ASM(".text");

/*=======[E X C I P T I O N]==================================================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"

/******************************************************************************/
/*
 * Brief                <trap 6>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Os_SystemCallTrap(uint16 Os_Tin) /* PRQA S 3006 */ /* MISRA Dir 4.3 */ /*ARCH_TRAP_TYPE_CAST_001*/
{
#if (TRUE == CFG_SRV_SHELLOS)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* ARCH_TRAP_COMPILER_003 */
    /* PRQA S 3678 ++*/ /* MISRA Rule 8.13 */  /*ARCH_TRAP_COMPILER_004*/
    P2VAR(Os_ArchCsaType, AUTOMATIC, OS_VAR)
    lowCsa;
    /* PRQA S 3678 --*/ /* MISRA Rule 8.13 */
    P2VAR(Os_ArchCsaType, AUTOMATIC, OS_VAR)
    upperCsa;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /*ARCH_TRAP_TYPE_CAST_002*/
    /* PRQA S 3469 ++*/ /* MISRA Dir 4.9 */    /*ARCH_TRAP_MACRO_001*/
    lowCsa = OS_PCX_TO_EA(OS_ARCH_MFCR(OS_REG_PCX));
    /* PRQA S 3469 --*/                                                 /* MISRA Dir 4.9 */
    upperCsa = OS_CSA_PREV(lowCsa); /*PRQA S 3432*/ /*MISRA Rule 20.7*/ /* ARCH_TRAP_COMPILER_003 */
/* PRQA S 0306 -- */                                                    /* MISRA Rule 11.4 */
#endif                                                                  /* TRUE == CFG_SRV_SHELLOS */

    switch (Os_Tin)
    {
    case 0:
        /*Tasking should save lower csa, GNU save the lower csa before in the OS_ASM where regedit exception*/
        OS_ASM("j Os_ArchDispatcher");
        break;

#if (TRUE == CFG_SRV_SHELLOS)
    case 1:
        upperCsa->reg[1] = 0x00000981u; /* PSW  */
        OS_ARCH_DSYNC();
        OS_ASM("rslcx");
        OS_ASM("nop");
        OS_ASM("rfe");
        break;
#endif /* TRUE == CFG_SRV_SHELLOS */
#if (TRUE == CFG_SRV_SHELLOS)
    case 2:
        upperCsa->reg[1] &= 0xffffc3ffu;  /* PSW  */
        upperCsa->reg[1] |= 0x000000800u; /* PSW  */
        OS_ARCH_DSYNC();
        OS_ASM("rslcx");
        OS_ASM("nop");
        OS_ASM("rfe");
        break;
#endif /* TRUE == CFG_SRV_SHELLOS */
    /*add comments to pass QAC.*/
    default:
        /* Nothing to do. */
        break;
    }
}

/******************************************************************************/
/*
 * Brief                <trap 0>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Trap0(uint16 Os_Tin)
{
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    VAR(uint32, OS_VAR)
    whoHook = OS_TMPROT_HOOK_OSAPP;

    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        whoHook = OS_TMPROT_HOOK_TASK;
    }
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        whoHook = OS_TMPROT_HOOK_ISR;
    }
    else
    {
        /* Do nothing. */
    }
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */

    switch (Os_Tin)
    {
    /* Virtual Address Fill */
    case 0:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Virtual_Address_Fill);
#endif
#if (TRUE == CFG_PROTECTIONHOOK)
        /* OS245:  protection exception hook. */
        (void)Os_CallProtectionHook(E_OS_Virtual_Address_Fill, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Virtual_Address_Fill);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Virtual Address Protection */
    case 1:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Virtual_Address_Protection);
#endif
#if (TRUE == CFG_PROTECTIONHOOK)
        /* OS245:  protection exception hook. */
        (void)Os_CallProtectionHook(E_OS_Virtual_Address_Protection, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Virtual_Address_Protection);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    default:
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_PROTECTION_EXCEPTION);
#endif

        while (1)
        {
            /* Nothing to do. */
        }
        break;
    }
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    UNUSED_PARAMETER(whoHook);
#endif
}

/******************************************************************************/
/*
 * Brief                <trap 1>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Trap1(uint16 Os_Tin)
{
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_PROTECTIONHOOK)
    VAR(uint32, OS_VAR)
    whoHook = OS_TMPROT_HOOK_OSAPP;

    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        whoHook = OS_TMPROT_HOOK_TASK;
    }
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        whoHook = OS_TMPROT_HOOK_ISR;
    }
    else
    {
        /* Do nothing. */
    }
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/

    switch (Os_Tin)
    {
    case 1:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_PROTECTION_EXCEPTION);
#endif
#if (TRUE == CFG_PROTECTIONHOOK)
        /* OS245:  protection exception hook. */
        (void)Os_CallProtectionHook(E_OS_PROTECTION_EXCEPTION, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_PROTECTION_EXCEPTION);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    case 2: /* Memory Protection Read */
    case 3: /* Memory Protection Write */
    case 4: /* Memory Protection Execution */
    case 5: /* Memory Protection Peripheral Access */
    case 6: /* Memory Protection Peripheral Null Access */
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_PROTECTION_MEMORY);
#endif
#if (TRUE == CFG_PROTECTIONHOOK)
        /* OS044: Memory protection hook. */
        (void)Os_CallProtectionHook(E_OS_PROTECTION_MEMORY, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_PROTECTION_EXCEPTION);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    case 7: /* Global Register Write Protection */
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_PROTECTION_EXCEPTION);
#endif
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    default:
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_PROTECTION_EXCEPTION);
#endif
        while (1)
        {
            /* Nothing to do. */
        }
        break;
    }
}

/******************************************************************************/
/*
 * Brief                <trap 2>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Trap2(uint16 Os_Tin)
{
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_PROTECTIONHOOK)
    VAR(uint32, OS_VAR)
    whoHook = OS_TMPROT_HOOK_OSAPP;

    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        whoHook = OS_TMPROT_HOOK_TASK;
    }
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        whoHook = OS_TMPROT_HOOK_ISR;
    }
    else
    {
        /* Do nothing. */
    }
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/

    switch (Os_Tin)
    {
    /* Illegal Opcode */
    case 1:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Illegal_Opcode);
#endif
#if (TRUE == CFG_PROTECTIONHOOK)
        /* OS245:  protection exception hook. */
        (void)Os_CallProtectionHook(E_OS_Illegal_Opcode, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Illegal_Opcode);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Unimplemented Opcode */
    case 2:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Unimplemented_Opcode);
#endif
#if (TRUE == CFG_PROTECTIONHOOK)
        /* OS245:  protection exception hook. */
        (void)Os_CallProtectionHook(E_OS_Unimplemented_Opcode, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Unimplemented_Opcode);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Invalid Operand specification */
    case 3:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Invalid_Operand);
#endif
#if (TRUE == CFG_PROTECTIONHOOK)
        /* OS245:  protection exception hook. */
        (void)Os_CallProtectionHook(E_OS_Invalid_Operand, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Invalid_Operand);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Data Address Alignment */
    case 4:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Data_Address_Align);
#endif
#if (TRUE == CFG_PROTECTIONHOOK)
        /* OS245:  protection exception hook. */
        (void)Os_CallProtectionHook(E_OS_Data_Address_Align, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Data_Address_Align);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Invalid Local Memory Address */
    case 5:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Invalid_Local_Memory);
#endif
#if (TRUE == CFG_PROTECTIONHOOK)
        /* OS245:  protection exception hook. */
        (void)Os_CallProtectionHook(E_OS_Invalid_Local_Memory, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Invalid_Local_Memory);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    default:
        while (1)
        {
            /* Nothing to do. */
        }
        break;
    }
}

/******************************************************************************/
/*
 * Brief                <trap 3>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Trap3(uint16 Os_Tin)
{
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_PROTECTIONHOOK)
    VAR(uint32, OS_VAR)
    whoHook = OS_TMPROT_HOOK_OSAPP;

    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        whoHook = OS_TMPROT_HOOK_TASK;
    }
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        whoHook = OS_TMPROT_HOOK_ISR;
    }
    else
    {
        /* Do nothing. */
    }
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/

    switch (Os_Tin)
    {
    /* Free Context List Depletion(FCX = LCX)*/
    case 1:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Free_Context_List_Depletion);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_Free_Context_List_Depletion, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Free_Context_List_Depletion);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Call Depth Overflow */
    case 2:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Call_Depth_Overflow);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_Call_Depth_Overflow, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Call_Depth_Overflow);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Call Depth Underflow */
    case 3:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Call_Depth_Underflow);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_Call_Depth_Underflow, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Call_Depth_Underflow);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/

        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Free Context List Underflow(FCX = 0) */
    case 4:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Free_Context_List_Underflow);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_Free_Context_List_Underflow, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Free_Context_List_Underflow);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Call Stack Underflow(PCX = 0) */
    case 5:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Call_Stack_Underflow);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_Call_Stack_Underflow, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Call_Stack_Underflow);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Context Type(PCXI.UL wrong) */
    case 6:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Context_Type);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_Context_Type, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Context_Type);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Nesting Error: RFE with non-zero call depth */
    case 7:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Nesting_Error);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_Nesting_Error, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Nesting_Error);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    default:
        while (1)
        {
            /* Nothing to do. */
        }
        break;
    }
}

/******************************************************************************/
/*
 * Brief                <trap 4>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Trap4(uint16 Os_Tin)
{
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_PROTECTIONHOOK)
    VAR(uint32, OS_VAR)
    whoHook = OS_TMPROT_HOOK_OSAPP;

    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        whoHook = OS_TMPROT_HOOK_TASK;
    }
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        whoHook = OS_TMPROT_HOOK_ISR;
    }
    else
    {
        /* Do nothing. */
    }
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/

    switch (Os_Tin)
    {
    /* Program Fetch Synchronous Error */
    case 1:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Program_Fetch_Synchronous);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_Program_Fetch_Synchronous, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Program_Fetch_Synchronous);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Data Access Synchronous Error */
    case 2:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Data_Access_Synchronous);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_Data_Access_Synchronous, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Data_Access_Synchronous);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Data Acess Asynchronous Error */
    case 3:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Data_Acess_Asynchronous);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_Data_Acess_Asynchronous, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Data_Acess_Asynchronous);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* nothing to do. */
        }
        break;

    /* Coprocessor Trap Asynchronous Error. (TriCore 1.3.1)*/
    case 4:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Coprocessor_Trap_Asynchronous);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_Coprocessor_Trap_Asynchronous, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Coprocessor_Trap_Asynchronous);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Program Memory Integrity Error. (TriCore 1.3.1) */
    case 5:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Program_Memory_Integrity);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_Program_Memory_Integrity, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Program_Memory_Integrity);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Data Memory Integrity Error. (TriCore 1.3.1) */
    case 6:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Data_Memory_Integrity);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_Data_Memory_Integrity, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Data_Memory_Integrity);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Temporal Asynchronous Error. (TriCore 1.6)*/
    case 7:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Temporal_Asynchronous);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_Temporal_Asynchronous, whoHook);
#endif

#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Temporal_Asynchronous);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    default:
        while (1)
        {
            /* Nothing to do. */
        }
        break;
    }
}

/******************************************************************************/
/*
 * Brief                <trap 5>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Trap5(uint16 Os_Tin)
{
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_PROTECTIONHOOK)
    VAR(uint32, OS_VAR)
    whoHook = OS_TMPROT_HOOK_OSAPP;

    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        whoHook = OS_TMPROT_HOOK_TASK;
    }
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        whoHook = OS_TMPROT_HOOK_ISR;
    }
    else
    {
        /* Do nothing. */
    }
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/

    switch (Os_Tin)
    {
    /* Arithmetic Overflow */
    case 1:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Arithmetic_Overflow);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_Arithmetic_Overflow, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Arithmetic_Overflow);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    /* Sticky Arithmetic Overflow */
    case 2:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Sticky_Arithmetic_Overflow);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_Sticky_Arithmetic_Overflow, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_Sticky_Arithmetic_Overflow);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    default:
        while (1)
        {
            /* Nothing to do. */
        }
        break;
    }
}

/******************************************************************************/
/*
 * Brief                <trap 7>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Trap7(uint16 Os_Tin) /* Non-Maskable Interrupt */
{
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_PROTECTIONHOOK)
    VAR(uint32, OS_VAR)
    whoHook = OS_TMPROT_HOOK_OSAPP;

    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        whoHook = OS_TMPROT_HOOK_TASK;
    }
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        whoHook = OS_TMPROT_HOOK_ISR;
    }
    else
    {
        /* Do nothing. */
    }
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/

    switch (Os_Tin)
    {
    /* NMI - Non-Maskable Interrupt (TIN 0) */
    case 0:
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_NMI);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_NMI, whoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_NMI);
#endif
#endif /* OS_SC2 == CFG_SC || OS_SC3 == CFG_SC || OS_SC4 == CFG_SC*/
        while (1)
        {
            /* Nothing to do. */
        }
        break;

    default:
        while (1)
        {
            /* Nothing to do. */
        }
        break;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
