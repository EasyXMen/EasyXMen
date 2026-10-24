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
**  FILENAME    : Os_Mcu_Core.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Core Request Managment                                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r23                         **
**  Version :   AUTOSAR classic Platform R23--Function Safety                 **
**                                                                            **
*******************************************************************************/
#ifndef OS_MCU_CORE_H
#define OS_MCU_CORE_H

/*=======[I N C L U D E S]====================================================*/

#include "Os_Types.h"
#include "Os_Arch_Mpu.h"

/*=======[M A C R O S]========================================================*/
#define OS_ADDRESS_IS_IN_ALL_RAM(Address, Size)                                                     \
    ((((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pSRamStart <= (Address))                            \
      && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pSRamEnd >= ((Address) + (Size))))               \
     || (((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pDtcmStart <= (Address))                         \
         && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pDtcmEnd >= ((Address) + (Size)))))

#define OS_ADDRESS_IS_IN_OTHER_APP(Address, Size, coreId, appsuborID)                               \
    (((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pSRamStart <= (Address))                             \
     && ((uint32)Os_Core_App_DAddr[coreId].APP_ADDR_START >= ((Address) + (Size))))                 \
        || (((uint32)Os_Core_App_DAddr[coreId].APP_ADDR_END <= (Address))                           \
            && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pPeripheralEnd >= ((Address) + (Size))))   \
        || (((uint32)Os_App_DAddr[appsuborID].APP_ADDR_START <= (Address))                          \
            && ((uint32)Os_App_DAddr[appsuborID].APP_ADDR_END >= ((Address) + (Size))))

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/* Core Mode */
typedef enum
{
    OS_CORE_MODE_HALT    = 0U, /*PRQA S 1271*/ /* MISRA CWE-682 */
    OS_CORE_MODE_RUN     = 1U, /*PRQA S 1271*/ /* MISRA CWE-682 */
    OS_CORE_MODE_IDLE    = 2U, /*PRQA S 1271*/ /* MISRA CWE-682 */
    OS_CORE_MODE_SLEEP   = 3U, /*PRQA S 1271*/ /* MISRA CWE-682 */
    OS_CORE_MODE_STANDBY = 4U, /*PRQA S 1271*/ /* MISRA CWE-682 */
    OS_CORE_MODE_UNKNOWN = 5U /*PRQA S 1271*/  /* MISRA CWE-682 */
} Os_CoreModeType;

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/* ============================
* Function Provided to Kernel *
============================ */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_CheckCPUInformation(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_StartCore(Os_CoreIdType coreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern Os_CoreModeType Os_Hal_GetCoreMode(Os_CoreIdType core);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern boolean Os_Hal_SetCoreMode(Os_CoreIdType core, Os_CoreModeType coreMode);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_MultiCoreInit(Os_CoreIdType coreId); 
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern Os_CoreIdType Os_Hal_GetCoreID(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_InitCPU(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* OS_MCU_CORE_H */

/*=======[E N D   O F   F I L E]==============================================*/
