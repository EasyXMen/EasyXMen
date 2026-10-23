/*******************************************************************************
**                                                                            **
** Copyright (C) iSOFT   (2023)                                               **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to iSOFT.         **
** Passing on and copying of this document, and communication                 **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME    : Os_Arch_Trap.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Interrupt Request Managment                                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r23                         **
**  Version :   AUTOSAR classic Platform r23--Function Safety                 **
**                                                                            **
*******************************************************************************/

#ifndef OS_ARCH_TRAP_H
#define OS_ARCH_TRAP_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_Types.h"

/*=======[M A C R O S]========================================================*/

/* Configurable Fault Status Register */
#define OS_REG_SCB_CFSR                         OS_ARCH_REG32(0xE000ED28U)
#define OS_REG_SCB_HFSR                         OS_ARCH_REG32(0xE000ED2CU)

/* Fault Message Mask */
#define OS_ARCH_CFSR_MMFSR_IACCVIOL_MASK        (OS_1BIT_MASK << OS_NUM_0)   /* Instruction access violation */
#define OS_ARCH_CFSR_MMFSR_DACCVIOL_MASK        (OS_1BIT_MASK << OS_NUM_1)   /* Data access violation */
#define OS_ARCH_CFSR_MMFSR_MUNSTKERR_MASK       (OS_1BIT_MASK << OS_NUM_3)   /* Memory manage fault on unstacking */
#define OS_ARCH_CFSR_MMFSR_MSTKERR_MASK         (OS_1BIT_MASK << OS_NUM_4)   /* Memory manage fault on stacking */
#define OS_ARCH_CFSR_MMFSR_MLSPERR_MASK         (OS_1BIT_MASK << OS_NUM_5)   /* MemManage fault during lazy FP save */
#define OS_ARCH_CFSR_MMFSR_MMARVALID_MASK       (OS_1BIT_MASK << OS_NUM_7)   /* MMFAR holds valid address */
#define OS_ARCH_CFSR_BFSR_IBUSERR_MASK          (OS_1BIT_MASK << OS_NUM_8)   /* Instruction bus error */
#define OS_ARCH_CFSR_BFSR_PRECISERR_MASK        (OS_1BIT_MASK << OS_NUM_9)   /* Precise data bus error */
#define OS_ARCH_CFSR_BFSR_IMPRECISERR_MASK      (OS_1BIT_MASK << OS_NUM_10)  /* Imprecise data bus error */
#define OS_ARCH_CFSR_BFSR_UNSTKERR_MASK         (OS_1BIT_MASK << OS_NUM_11)  /* Bus fault on unstacking */
#define OS_ARCH_CFSR_BFSR_STKERR_MASK           (OS_1BIT_MASK << OS_NUM_12)  /* Bus fault on stacking */
#define OS_ARCH_CFSR_BFSR_LSPERR_MASK           (OS_1BIT_MASK << OS_NUM_13)  /* Bus fault during lazy FP save */
#define OS_ARCH_CFSR_BFSR_BFARVALID_MASK        (OS_1BIT_MASK << OS_NUM_15)  /* BFAR holds valid address */
#define OS_ARCH_CFSR_UFSR_UNDEFINSTR_MASK       (OS_1BIT_MASK << OS_NUM_16)  /* Undefined instruction */
#define OS_ARCH_CFSR_UFSR_INVSTATE_MASK         (OS_1BIT_MASK << OS_NUM_17)  /* Invalid state (EPSR.T=0) */
#define OS_ARCH_CFSR_UFSR_INVPC_MASK            (OS_1BIT_MASK << OS_NUM_18)  /* Invalid PC load */
#define OS_ARCH_CFSR_UFSR_NOCP_MASK             (OS_1BIT_MASK << OS_NUM_19)  /* No coprocessor (FPU not enabled?) */
#define OS_ARCH_CFSR_UFSR_UNALIGNED_MASK        (OS_1BIT_MASK << OS_NUM_24)  /* Unaligned access trap */
#define OS_ARCH_CFSR_UFSR_DIVBYZERO_MASK        (OS_1BIT_MASK << OS_NUM_25)  /* Divide by zero */

/* Fault category */
/* Memory fault mask */
#define OS_ARCH_CFSR_MEM_FAULT_MASK                   \
        (OS_ARCH_CFSR_MMFSR_IACCVIOL_MASK       |     \
         OS_ARCH_CFSR_MMFSR_DACCVIOL_MASK       |     \
         OS_ARCH_CFSR_MMFSR_MSTKERR_MASK        |     \
         OS_ARCH_CFSR_MMFSR_MLSPERR_MASK        |     \
         OS_ARCH_CFSR_MMFSR_MMARVALID_MASK      |     \
         OS_ARCH_CFSR_MMFSR_MUNSTKERR_MASK)

/* Bus fault mask */
#define OS_ARCH_CFSR_BUS_FAULT_MASK                   \
        (OS_ARCH_CFSR_BFSR_IBUSERR_MASK         |     \
         OS_ARCH_CFSR_BFSR_PRECISERR_MASK       |     \
         OS_ARCH_CFSR_BFSR_STKERR_MASK          |     \
         OS_ARCH_CFSR_BFSR_IMPRECISERR_MASK     |     \
         OS_ARCH_CFSR_BFSR_LSPERR_MASK          |     \
         OS_ARCH_CFSR_BFSR_BFARVALID_MASK       |     \
         OS_ARCH_CFSR_BFSR_UNSTKERR_MASK)

/* Usage falut mask */
#define OS_ARCH_CFSR_USAGE_FAULT_MASK                \
        (OS_ARCH_CFSR_UFSR_UNDEFINSTR_MASK      |    \
         OS_ARCH_CFSR_UFSR_INVSTATE_MASK        |    \
         OS_ARCH_CFSR_UFSR_INVPC_MASK           |    \
         OS_ARCH_CFSR_UFSR_NOCP_MASK            |    \
         OS_ARCH_CFSR_UFSR_UNALIGNED_MASK       |    \
         OS_ARCH_CFSR_UFSR_DIVBYZERO_MASK)

/* hard fault force mask */
#define OS_ARCH_ERROR_HARDFORCE_MASK            (OS_1BIT_MASK << OS_NUM_30)

/* OS Error Non-Autosar Specified */
#define OS_ARCH_ERROR_HARDFORCE                 (OS_1BIT_MASK << OS_NUM_7)
#define OS_ARCH_ERROR_USAGEFAULT                (OS_1BIT_MASK << OS_NUM_4)
#define OS_ARCH_ERROR_BUSFAULT                  (OS_1BIT_MASK << OS_NUM_5)
#define OS_ARCH_ERROR_MEMORYFALUT               (OS_1BIT_MASK << OS_NUM_6)
#define OS_ARCH_ERROR_HARDFAULT                 (OS_7BIT_MASK)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/* ============================
* Function Provided to Kernel *
============================ */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern uint32 Os_Hal_GetCurrentExc(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((TRUE == CFG_FAULT_MANAGEMENT_ENABLE) && (TRUE == CFG_HARDWARE_FAULT_ENABLE))

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_ExceptionInformationParse(Os_TrapContextType* TrapContext);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_HalExceptionSaveContext(const void* TrapMsg);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_HalExceptionResumeContext(const void* trapMsg);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* (TRUE == CFG_FAULT_MANAGEMENT_ENABLE) && (TRUE == CFG_HARDWARE_FAULT_ENABLE) */ /* PRQA S 2053 */ /* VL_Os_2053 */ 

/* ======================
* Functional Other Used *
====================== */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_HardFault_Process(uint32 TrapMsg);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* OS_TRAP_H */

/*=======[E N D   O F   F I L E]==============================================*/
