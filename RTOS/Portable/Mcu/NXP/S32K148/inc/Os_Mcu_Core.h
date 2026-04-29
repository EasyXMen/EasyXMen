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
/*
**                                                                            **
**  FILENAME    :  Os_Mcu_Core.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      :  i-soft-os                                                  **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/
#ifndef ARCH_PROCESSOR_H
#define ARCH_PROCESSOR_H

#include "Os_Types.h"
#include "Os_Arch_Processor.h"
#include "Os_Arch_Define.h"

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
/*=======[I N C L U D E S]====================================================*/

/*=======[M A C R O S]========================================================*/
#define CHAR_BIT                   8u /*PRQA S 4600*/ /* MISRA Rule 21.1 */
#define CFG_REGION_NUM             (16u)

typedef struct
{
    uint32* sarmstart;
    uint32* romstart;
    uint32* romend;
} Os_MemCopyType;

typedef enum
{
    OS_CORE_MODE_HALT = 0U,
    OS_CORE_MODE_RUN = 1U,
    OS_CORE_MODE_IDLE = 2U,
    OS_CORE_MODE_SLEEP = 3U,
    OS_CORE_MODE_STANDBY = 4U,
    OS_CORE_MODE_UNKNOWN = 5U
} Os_CoreModeType;

/* RPC */
#define E_BUSY      0x02u
#define RPC_TIMEOUT (100)
#define TIMER_FRE   (100)
/* stimate the number of instructions that will run */
#define RPC_INS_NUM   (5)
#define RPC_WAIT_TIME ((RPC_TIMEOUT * TIMER_FRE * 1000 / RPC_INS_NUM))
/*=======[Porting Macro]=====================================================*/


/*============[MEMORY CHECK]=====================*/
#define OS_HAL_ADDRESS_IS_IN_ALL_RAM(Address, Size)                                       \
    ((((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pSRamStart <= (Address))              \
      && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pSRamEnd >= ((Address) + (Size)))))

#define OS_HAL_ADDRESS_IS_NOT_IN_OTHER_APP(Address, Size, coreId, appsuborID)                             \
    (((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pSRamStart <= (Address))                           \
     && ((uint32)Os_Core_App_DAddr[coreId].AppAddrStart >= ((Address) + (Size))))               \
        || (((uint32)Os_Core_App_DAddr[coreId].AppAddrEnd <= (Address))                         \
            && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pPeripheralEnd >= ((Address) + (Size)))) \
        || (((uint32)Os_App_DAddr[appsuborID].AppAddrStart <= (Address))                        \
            && ((uint32)Os_App_DAddr[appsuborID].AppAddrEnd >= ((Address) + (Size))))

/*=======[I N T E R N A L   M A C R O]=======================================*/


/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/*=======[E X T E R N A L   D A T A]==========================================*/
extern const Os_MemProtKnCfgType Os_MemProtKnAddrCfg;
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
extern Os_CoreIdType Os_Hal_GetCoreID(void);
extern void Os_Hal_StartCore(Os_CoreIdType coreId);
extern Os_CoreModeType Os_Hal_GetCoreMode(Os_CoreIdType core);
extern boolean Os_Hal_SetCoreMode(Os_CoreIdType core, Os_CoreModeType coreMode);
extern void Os_Hal_MultiCoreInit(Os_CoreIdType sysCore);

extern void Os_Hal_InitCPU(void);

#if (CFG_TASK_MAX_CORE0 > 0U)
extern void Os_TaskEntry_IdleCore0(void);
extern void IdleHook_Core0(void);
#endif
#if (CFG_TASK_MAX_CORE1 > 0U)
extern void Os_TaskEntry_IdleCore1(void);
extern void IdleHook_Core1(void);
#endif

#endif
/*=======[E N D   O F   F I L E]==============================================*/
