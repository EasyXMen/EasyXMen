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
**  FILENAME    : Mcu_Core.h                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Core Request Managment                                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/
#ifndef ARCH_PROCESSOR_H
#define ARCH_PROCESSOR_H

/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"
#include "Arch_Mpu.h"

/*=======[M A C R O S]========================================================*/
#define OS_ADDRESS_IS_IN_ALL_RAM(Address, Size)                                       \
    ((((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pSRamStart <= (Address))              \
      && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pSRamEnd >= ((Address) + (Size)))) \
     || (((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pDtcmStart <= (Address))           \
         && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pDtcmEnd >= ((Address) + (Size)))))

#define OS_ADDRESS_IS_IN_OTHER_APP(Address, Size, coreId, appsuborID)                             \
    (((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pSRamStart <= (Address))                           \
     && ((uint32)Os_Core_App_DAddr[coreId].APP_ADDR_START >= ((Address) + (Size))))               \
        || (((uint32)Os_Core_App_DAddr[coreId].APP_ADDR_END <= (Address))                         \
            && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pPeripheralEnd >= ((Address) + (Size)))) \
        || (((uint32)Os_App_DAddr[appsuborID].APP_ADDR_START <= (Address))                        \
            && ((uint32)Os_App_DAddr[appsuborID].APP_ADDR_END >= ((Address) + (Size))))

#define CFG_REGION_NUM             (16u)
/* RPC */
#define E_BUSY      0x02u
#define RPC_TIMEOUT (100)
#define TIMER_FRE   (100)
/* stimate the number of instructions that will run */
#define RPC_INS_NUM   (5)
#define RPC_WAIT_TIME ((RPC_TIMEOUT * TIMER_FRE * 1000 / RPC_INS_NUM))

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* Core Mode */
typedef enum
{
    OS_CORE_MODE_HALT = 0U, /*PRQA S 1271*/    /* MISRA CWE-682 */
    OS_CORE_MODE_RUN = 1U, /*PRQA S 1271*/     /* MISRA CWE-682 */
    OS_CORE_MODE_IDLE = 2U, /*PRQA S 1271*/    /* MISRA CWE-682 */
    OS_CORE_MODE_SLEEP = 3U, /*PRQA S 1271*/   /* MISRA CWE-682 */
    OS_CORE_MODE_STANDBY = 4U, /*PRQA S 1271*/ /* MISRA CWE-682 */
    OS_CORE_MODE_UNKNOWN = 5U /*PRQA S 1271*/  /* MISRA CWE-682 */
} Os_CoreModeType;

extern uint32 Os_Isr2_Ipl_Limit;
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

extern void Os_ArchInitCPU(void);
extern Os_CoreIdType Os_ArchGetCoreID(void);
extern void Os_ArchStartCore(Os_CoreIdType coreId);
extern Os_CoreModeType Os_GetCoreMode(Os_CoreIdType core);
extern boolean Os_SetCoreMode(Os_CoreIdType core, Os_CoreModeType coreMode);
extern void Os_MultiCoreInitProcessor(void); /*PRQA S 3449,3451*/             /* MISRA Rule 8.5 */
extern CoreIdType Os_GetCoreLogID(CoreIdType phyCoreId); /*PRQA S 3449,3451*/ /* MISRA Rule 8.5 */
extern CoreIdType Os_GetCorePhyID(CoreIdType logCoreId); /*PRQA S 3449,3451*/ /* MISRA Rule 8.5 */

extern void IdleHook_Core0(void);
extern void Os_TaskEntry_IdleCore0(void);

#endif