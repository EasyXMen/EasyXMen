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
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/
#ifndef OS_ARCH_TRAP_H
#define OS_ARCH_TRAP_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_Compiler.h"
#include "Platform_Types.h"
#include "Os_Cfg.h"
#include "Os_Types.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/

/******************************************************************************/
/*
 * Brief                <Call PendSV to switch task.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_SystemCallTrap>
 * REQ ID               <None>
 */
/******************************************************************************/
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
extern void Os_Hal_ExceptionInformationParse(Os_TrapContextType* TrapContext);
#endif /* TRUE == CFG_FAULT_MANAGEMENT_ENABLE */

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
#endif /* OS_ARCH_TRAP_H */
/*=======[E N D   O F   F I L E]==============================================*/
