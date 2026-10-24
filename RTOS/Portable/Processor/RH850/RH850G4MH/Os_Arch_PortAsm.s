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
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : Os_Arch_PortAsm.s                                           **
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
#define OS_ARCH_PORTASM_S
/*=======[I N C L U D E S]====================================================*/
#include "Os_Hal_Compiler.h"
#include "Os_Cfg_S.h"
/*=======[E X T E R N A L   D A T A]==========================================*/
    OS_ASM_EXTERN _OS_ArchGetSystemStackPointer
    OS_ASM_EXTERN _OS_ArchGetTaskStackPointer
    OS_ASM_EXTERN _Os_ArchTempSp
    OS_ASM_EXTERN _OS_ArchSaveTaskStackPointer
    OS_ASM_EXTERN _Os_SwitchTask
    OS_ASM_EXTERN OS_ASM_RESET
    OS_ASM_EXTERN _Os_Arch_FEExceptionHandler
    
    OS_ASM_EXTERN _Os_Vector_Handler
    OS_ASM_EXTERN _Os_FE_Handler
    OS_ASM_EXTERN _Os_ArchFENMI
    OS_ASM_EXTERN _Os_ArchMipMdp

OS_FEINC_BASE_ADDR_PE0 OS_ASM_EQU 0xFF9A3B00U
OS_FEINC_OFFEST        OS_ASM_EQU 0x0100U

/*=======[M A C R O S]========================================================*/

/*--------------------------return PE number in r10----------------------*/
OS_ASM_MACRO(GETPEID)
    stsr	0, r10, 2
    andi	0x7, r10, r10
    OS_ASM_MACRO_END

/*--------------------------Load and Switch to system stack----------------------*/
OS_ASM_MACRO(LOAD_SYSTEM_STACK)
    jarl _OS_ArchGetSystemStackPointer,lp
    mov    _Os_ArchTempSp, r7
    ld.w   0[r7], r7
    mov    r7, sp
    OS_ASM_MACRO_END
/*-------------------------------------------------------------------------------*/

/*--------------------------Load and Switch to task stack------------------------*/
OS_ASM_MACRO(LOAD_TASK_STACK)
    jarl _OS_ArchGetTaskStackPointer, lp
    mov _Os_ArchTempSp, r6
    ld.w 0[r6], sp
    OS_ASM_MACRO_END
/*-------------------------------------------------------------------------------*/

/*--------------------------Disable FE&EI interrupt------------------------------*/
OS_ASM_MACRO(DISABLE_FEANDEI_INT)
    pushsp r1-r2
    stsr psw, r2
    ori 0xa0, r2, r2
    ldsr r2, psw
    popsp r1-r2
    OS_ASM_MACRO_END
/*-------------------------------------------------------------------------------*/

/*--------------------------Enable FE&EI interrupt-------------------------------*/
OS_ASM_MACRO(ENABLE_FEANDEI_INT)
    pushsp r1-r2
    stsr psw, r2
    mov 0xFFFFFF5FU,r1
    and  r1, r2
    ldsr r2, psw
    popsp r1-r2
    OS_ASM_MACRO_END
/*-------------------------------------------------------------------------------*/

/*--------------------------Resum fpu Context---------------------------------*/
OS_ASM_MACRO(OS_ARCH_RESUME_FPU_CONTEXT)
    popsp r8-r13
    ldsr  r8,6,0      /*---resume fpsr--*/
    ldsr  r9,7,0      /*---resume fpepc--*/
    ldsr  r10,8,0     /*---resume fpst--*/
    ldsr  r11,9,0     /*---resume fpcc--*/
    ldsr  r12,10,0    /*---resume fpcfg--*/
    ldsr  r13,11,0    /*---resume fpec--*/
    OS_ASM_MACRO_END
/*-------------------------------------------------------------------------------*/

/*--------------------------Save fpu Context---------------------------------*/
OS_ASM_MACRO(OS_ARCH_SAVE_FPU_CONTEXT)
    stsr  6,r8,0      /*---save fpsr--*/
    stsr  7,r9,0      /*---save fpepc--*/
    stsr  8,r10,0     /*---save fpst--*/
    stsr  9,r11,0     /*---save fpcc--*/
    stsr  10,r12,0    /*---save fpcfg--*/
    stsr  11,r13,0    /*---save fpec--*/
    pushsp r8-r13
    OS_ASM_MACRO_END
/*-------------------------------------------------------------------------------*/


/*--------------------------resum thread Context---------------------------------*/
OS_ASM_MACRO(OS_ARCH_RESUME_CONTEXT_1)
#if ((1U) == CFG_FPU_ENABLE)
    OS_ARCH_RESUME_FPU_CONTEXT
#endif
    popsp r6-r7
    ldsr  r6,0,0    /*---resume eipc--*/
    ldsr  r7,1,0    /*---resume eipsw--*/
    popsp r6-r31  
    popsp r1-r2
    OS_ASM_MACRO_END
/*-------------------------------------------------------------------------------*/

/*--------------------------Save thread Context----------------------------------*/
OS_ASM_MACRO(OS_ARCH_SAVE_CONTEXT_1)
    pushsp r1-r2               /*-- Push register to sp */
    pushsp r6-r31
    stsr 0, r6,0               /*-- save eipc to r6*/
    stsr 1, r7,0               /*-- save eipsw to r7*/
    pushsp r6-r7               /*-- push eipc and eipsw*/
#if ((1U) == CFG_FPU_ENABLE)
    OS_ARCH_SAVE_FPU_CONTEXT
#endif
    mov _Os_ArchTempSp, r21   /* -- Save Current sp */
    st.w sp, 0[r21]             
    OS_ASM_MACRO_END
/*-------------------------------------------------------------------------------*/

/*--------------------------Save FEENTER Context----------------------------------*/
OS_ASM_MACRO(OS_ARCH_SAVE_CONTEXT_FEENTER)
    PUSHSP r1-r2              /*-- Push register to sp*/
    PUSHSP r6-r31             
    stsr 2, r6,0              /*--save fepc to r6*/
    stsr 3, r7,0              /*--save fepsw to r7*/
    pushsp r6-r7              /*--push fepc and fepsw*/
#if ((1U) == CFG_FPU_ENABLE)
    OS_ARCH_SAVE_FPU_CONTEXT
#endif
    mov _Os_ArchTempSp, r21   /*--Save Current sp*/
    st.w sp, 0[r21]        
    OS_ASM_MACRO_END
/*-------------------------------------------------------------------------------*/

/*--------------------------RESUME FEENTER Context----------------------------------*/
OS_ASM_MACRO(OS_ARCH_RESUME_CONTEXT_FEENTER)
#if ((1U) == CFG_FPU_ENABLE)
    OS_ARCH_RESUME_FPU_CONTEXT
#endif
    popsp r6-r7
    ldsr  r6,2,0    /*--resume fepc--*/
    ldsr  r7,3,0    /*--resume fepsw--*/
    popsp r6-r31
    popsp r1-r2
    OS_ASM_MACRO_END
/*-------------------------------------------------------------------------------*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

/*-------------------------------------------------------------------------------*/
/*                                                                               */
/*                           Switch to privileged mode                           */
/*                                                                               */
/*-------------------------------------------------------------------------------*/
    OS_ASM_ALIGN(2)
_Os_ArchSetSvMode:
    pushsp r1-r2
    stsr 1, r2, 0      /* r2 = EIPSW */
    mov 0xBFFFFFFF, r1
    and r1, r2         /* r2.bit30 = 0 */
    ldsr r2, 1, 0      /* EIPSW = r2(UM=0) */
    stsr psw, R2       /* PSW = r2(UM=0) */
    popsp r1-r2
    eiret

/*-------------------------------------------------------------------------------*/
/*                                   Trap 1                                      */
/*                   Terminate thread  &   Promote privileges                    */
/*                                                                               */
/*-------------------------------------------------------------------------------*/
    OS_ASM_GLOBAL Os_SystemCall_Trap1
    OS_ASM_TEXT_ALIGN(2)
Os_SystemCall_Trap1:
    /*--------disable interrupt---------*/
    DISABLE_FEANDEI_INT

    pushsp r1-r2
    stsr eiic, r2
    andi 0xF, r2, r2
    cmp 0, r2
    bz _StartScheduler
    cmp 1, r2
    bz _SetSvMode

/*---Terminate thread:switch and restore the context without saving the context--*/
_StartScheduler:
    popsp r1-r2
    LOAD_SYSTEM_STACK        /*--load system stack and switch sp--*/
    jarl _Os_SwitchTask, lp  /*--Switch thread--*/
    LOAD_TASK_STACK          /*--load Task stack and switch sp--*/
    OS_ARCH_RESUME_CONTEXT_1 /*--resume thread--*/

    /*------Return-----*/
    eiret

/*----- Promote privileges----*/
_SetSvMode:
    popsp r1-r2
    jr _Os_ArchSetSvMode

/*-------------------------------------------------------------------------------*/
/*                                   Trap 0                                      */
/*                  system scheduling:Save and restore context                   */
/*                                                                               */
/*-------------------------------------------------------------------------------*/
    OS_ASM_GLOBAL Os_SystemCall_Trap0
    OS_ASM_TEXT_ALIGN(2)
Os_SystemCall_Trap0:
    /*--------disable interrupt---------*/
   DISABLE_FEANDEI_INT

   /*---save context-----*/
   OS_ARCH_SAVE_CONTEXT_1

   jarl _OS_ArchSaveTaskStackPointer, lp

   /*--load system stack and switch sp--*/
   LOAD_SYSTEM_STACK

   /*--Switch thread--*/
   jarl _Os_SwitchTask, lp

   /*--load Task stack and switch sp--*/
   LOAD_TASK_STACK

    /*--resume thread--*/
   OS_ARCH_RESUME_CONTEXT_1
    
   /*------Return-----  */
   eiret

/*-------------------------------------------------------------------------------*/
/*                                                                               */
/*                                 Os_ISR1Handler                                */
/*                                                                               */
/*-------------------------------------------------------------------------------*/
    OS_ASM_GLOBAL _Os_ISR1Handler
    OS_ASM_TEXT_ALIGN(2)
_Os_ISR1Handler:
    /*--------disable interrupt---------*/
    DISABLE_FEANDEI_INT

    /*----save context-----*/
    OS_ARCH_SAVE_CONTEXT_1

    /*-----Get exception number-----*/
    stsr eiic, r2
    andi 0xFFF, r2, r2
    shl 2,r2

    /*-----Get interrupt entry address----- */
    mov _Os_Vector_Handler, r1
    add r2, r1
    ld.w 0[r1],r1

    /*----jump to interrupt service function---*/
    jmp [r1]

/*-------------------------------------------------------------------------------*/
/*                                                                               */
/*                            EI interrupt operations                            */
/*                                                                               */
/*-------------------------------------------------------------------------------*/
    OS_ASM_GLOBAL _Os_ISR2Handler
    OS_ASM_TEXT_ALIGN(2)
_Os_ISR2Handler:
    /*--------disable interrupt---------*/
    DISABLE_FEANDEI_INT

    /*----save context-----*/
    OS_ARCH_SAVE_CONTEXT_1

    /*-----Get exception number-----*/
    stsr eiic, r2
    andi 0xFFF, r2, r2
    shl 2,r2

    /*-----Get interrupt entry address----- */
    mov _Os_Vector_Handler, r1
    add r2, r1
    ld.w 0[r1],r1

    /*----jump to interrupt service function---*/
    jmp [r1]


/*-------------------------------------------------------------------------------*/
/*                                                                               */
/*                          EI interrupt exit processing                         */
/*                                                                               */
/*-------------------------------------------------------------------------------*/
    OS_ASM_GLOBAL _Os_ArchISR2Exit
    OS_ASM_TEXT_ALIGN(2)
_Os_ArchISR2Exit:
    /*------load sp-----*/
    mov _Os_ArchTempSp, r6
    ld.w 0[r6], sp

    /*------resume thread context---*/
    OS_ARCH_RESUME_CONTEXT_1

    /*------Return-----  */
    eiret

/*-------------------------------------------------------------------------------*/
/*                                                                               */
/*                          EI interrupt exit processing                         */
/*                                                                               */
/*-------------------------------------------------------------------------------*/
    OS_ASM_GLOBAL _Os_ArchISR1Exit
    OS_ASM_TEXT_ALIGN(2)
_Os_ArchISR1Exit:
    /*------load sp-----*/
    mov _Os_ArchTempSp, r6  
    ld.w 0[r6], sp 
    OS_ARCH_RESUME_CONTEXT_1

    /*------Return-----  */
    eiret

/*-------------------------------------------------------------------------------*/
/*                                                                               */
/*                             FE interrupt operations                           */
/*                                                                               */
/*-------------------------------------------------------------------------------*/
    OS_ASM_GLOBAL Os_FEHandler
    OS_ASM_TEXT_ALIGN(2)
Os_FEHandler:
    /*--------disable interrupt---------*/
    DISABLE_FEANDEI_INT

    /*----save context-----*/
    OS_ARCH_SAVE_CONTEXT_FEENTER

    /*---Get PEID---*/
    stsr 0, r1, 2
    andi 0x7, r1, r1

    mov OS_FEINC_OFFEST, r6
    mul r6, r1, r1 

    /*-----Get exception number-----*/
    mov OS_FEINC_BASE_ADDR_PE0, r2
    add r1, r2
    ld.w 0[r2], r2
    sch1r r2, r2
    add -1, r2
    shl 2,r2

    mov _Os_FE_Handler, r1
    add r2, r1
    ld.w 0[r1],r1
    jmp [r1]

/*-------------------------------------------------------------------------------*/
/*                                                                               */
/*                           EI interrupt exit processing                        */
/*                                                                               */
/*-------------------------------------------------------------------------------*/
    OS_ASM_GLOBAL _Os_ArchFeISRExit
    OS_ASM_TEXT_ALIGN(2)
_Os_ArchFeISRExit:
    /*------load sp-----*/
    mov _Os_ArchTempSp, r6
    ld.w 0[r6], sp

    /*------resume thread context---*/
    OS_ARCH_RESUME_CONTEXT_FEENTER

    /*------Return-----  */
    feret

/*-------------------------------------------------------------------------------*/
/*                                                                               */
/*                           FE exception enter processing                       */
/*                                                                               */
/*-------------------------------------------------------------------------------*/
    OS_ASM_GLOBAL _Os_FEExceptionEnter
    OS_ASM_TEXT_ALIGN(2)
_Os_FEExceptionEnter:
    OS_ARCH_SAVE_CONTEXT_FEENTER
    jarl _Os_Arch_FEExceptionHandler, lp
Os_Arch_FEExceptionEnter_Hang:
    jr Os_Arch_FEExceptionEnter_Hang      /* hang forever, FE handler never returns */


/*-------------------------------------------------------------------------------*/
/*                                                                               */
/*                               Os_ArchExceptionTable                           */
/*                                                                               */
/*-------------------------------------------------------------------------------*/
    OS_ASM_GLOBAL _Os_ArchExceptionTable
    OS_ASM_TEXT_ALIGN(9)
_Os_ArchExceptionTable:
    /* 0x00 */
    jr OS_ASM_RESET

    /* 0x10 SYSERR */
    OS_ASM_ALIGN(4)
_SYSERR:
    jr _Os_FEExceptionEnter

    /* 0x20 Reserve */
    OS_ASM_ALIGN(4)
_HVTRAP:
    jr _HVTRAP

    /* 0x30 FETRAP */
    OS_ASM_ALIGN(4)
_FETRAP:
    jr _FETRAP

    /* 0x40	TRAP0 : System Dispatch*/
    OS_ASM_ALIGN(4)
    jr Os_SystemCall_Trap0
_SYSTEMCALL_ERROR:
    jr _SYSTEMCALL_ERROR

    /* 0x50	TRAP1 : System StartScheduler / Privilege Change */
    OS_ASM_ALIGN(4)
    jr Os_SystemCall_Trap1
    jr _SYSTEMCALL_ERROR

    /* 0x60	RIE :  */
    OS_ASM_ALIGN(4)
_RIE:
    jr _Os_FEExceptionEnter

    /* 0x70	FPE_FXE :  */
    OS_ASM_ALIGN(4)
_FPE_FXE:
    jr _FPE_FXE

    /* 0x80	UCPOP :  */
    OS_ASM_ALIGN(4)
_UCPOP:
    jr _Os_FEExceptionEnter

    /* 0x90 MDP/MIP */
    OS_ASM_ALIGN(4)
    jr _Os_ArchMipMdp

    /* 0xA0 PIE */
    OS_ASM_ALIGN(4)
_PIE:
    jr _Os_FEExceptionEnter

    /* 0xB0 Reserve */
    OS_ASM_ALIGN(4)
_none_ERROR2:
    jr _none_ERROR2

    /* 0xC0	MAE */
    OS_ASM_ALIGN(4)
_MAE:
    jr _Os_FEExceptionEnter

    /* 0xD0 Reserve */
    OS_ASM_ALIGN(4)
_BGFEINT_BGEIINT:
    jr _BGFEINT_BGEIINT

    /* 0xE0	Os_ArchFENMI :  */
    OS_ASM_ALIGN(4)
    jr _Os_ArchFENMI

    /* 0xF0	FEINT :  */
    OS_ASM_ALIGN(4)
    jr Os_FEHandler

    /* 0x100	EIINT0-2047	Direct Method priority0*/
    OS_ASM_ALIGN(4)
    jr _Os_ISR1Handler

    /* 0x110	EIINT0-2047	Direct Method priority1*/
    OS_ASM_ALIGN(4)
    jr _Os_ISR1Handler

    /* 0x120	EIINT0-2047	Direct Method priority2*/
    OS_ASM_ALIGN(4)
    jr _Os_ISR1Handler

    /* 0x130	EIINT0-2047	Direct Method priority3*/
    OS_ASM_ALIGN(4)
    jr _Os_ISR1Handler

    /* 0x140	EIINT0-2047	Direct Method priority4*/
    OS_ASM_ALIGN(4)
    jr _Os_ISR1Handler

    /* 0x150	EIINT0-2047	Direct Method priority5*/
    OS_ASM_ALIGN(4)
    jr _Os_ISR2Handler

    /* 0x160	EIINT0-2047	Direct Method priority6*/
    OS_ASM_ALIGN(4)
    jr _Os_ISR2Handler

    /* 0x170	EIINT0-2047	Direct Method priority7*/
    OS_ASM_ALIGN(4)
    jr _Os_ISR2Handler

    /* 0x180	EIINT0-2047	Direct Method priority8*/
    OS_ASM_ALIGN(4)
    jr _Os_ISR2Handler

    /* 0x190	EIINT0-2047	Direct Method priority9*/
    OS_ASM_ALIGN(4)
    jr _Os_ISR2Handler

    /* 0x1A0	EIINT0-2047	Direct Method priority10*/
    OS_ASM_ALIGN(4)
    jr _Os_ISR2Handler

    /* 0x1B0	EIINT0-2047	Direct Method priority11*/
    OS_ASM_ALIGN(4)
    jr _Os_ISR2Handler

    /* 0x1C0	EIINT0-2047	Direct Method priority12*/
    OS_ASM_ALIGN(4)
    jr _Os_ISR2Handler

    /* 0x1D0	EIINT0-2047	Direct Method priority13*/
    OS_ASM_ALIGN(4)
    jr _Os_ISR2Handler

    /* 0x1E0	EIINT0-2047	Direct Method priority14*/
    OS_ASM_ALIGN(4)
    jr _Os_ISR2Handler

    /* 0x1F0	EIINT0-2047	Direct Method priority15*/
    OS_ASM_ALIGN(4)
    jr _Os_ISR2Handler

/*=======[E N D   O F   F I L E]==============================================*/
