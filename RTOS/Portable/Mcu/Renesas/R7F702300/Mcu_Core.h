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
 **  FILENAME    : Mcu_Core.h                                                  **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : MCU dependence                                              **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
#ifndef MCU_CORE_H__
#define MCU_CORE_H__

/*=======[I N C L U D E S]====================================================*/
#include "Os_Types.h"

/*=======[M A C R O S]========================================================*/
#define CHAR_BIT               (3U)

#define OS_ARCH_MPU_REGION_NUM (16U)

/* Flash address area */
/* Code Flash */
#define OS_ARCH_FLASH_ADDR_START (0U)
#define OS_ARCH_FLASH_ADDR_END   (0x0FFFFFFFU)

/* Local RAM and Cluster RAM */
#define OS_ARCH_RAM_ADDR_START (0xFD600000U)
#define OS_ARCH_RAM_ADDR_END   (0xFE83FFFFU)

/* Peripheral address area */
#define OS_ARCH_PERIPH_ADDR_START (0xFF000000U)
#define OS_ARCH_PERIPH_ADDR_END   (0xFFFD3FFFU)
/* Provided for external use */
#define PERIPHERAL_START (uint8*)OS_ARCH_PERIPH_ADDR_START
#define PERIPHERAL_END   (uint8*)OS_ARCH_PERIPH_ADDR_END

#define RODATA_START     (uint8*)OS_ARCH_RAM_ADDR_START
#define RODATA_END       (uint8*)OS_ARCH_FLASH_ADDR_END

/* Provided for external use */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_ADDRESS_IS_IN_ALL_RAM(Address, Size)                        \
    (((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRamStart <= (Address)) \
     && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRamEnd >= ((Address) + (Size))))

/* Provided for external use */
#define OS_ADDRESS_IS_IN_OTHER_APP(Address, Size, coreId, appsuborID)                          \
    ((((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRamStart <= (Address))                        \
      && ((uint32)Os_Core_App_DAddr[coreId].APP_ADDR_START >= ((Address) + (Size))))           \
     || (((uint32)Os_Core_App_DAddr[coreId].APP_ADDR_END <= (Address))                         \
         && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pPeripheralEnd >= ((Address) + (Size)))) \
     || (((uint32)Os_App_DAddr[appsuborID].APP_ADDR_START <= (Address))                        \
         && ((uint32)Os_App_DAddr[appsuborID].APP_ADDR_END >= ((Address) + (Size)))))
#endif /* (TRUE == CFG_MEMORY_PROTECTION_ENABLE) */

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* Core Mode */
typedef enum
{
    OS_CORE_MODE_HALT = 0U,
    OS_CORE_MODE_RUN = 1U,
    OS_CORE_MODE_IDLE = 2U,
    OS_CORE_MODE_SLEEP = 3U,
    OS_CORE_MODE_STANDBY = 4U,
    OS_CORE_MODE_UNKNOWN = 5U
} Os_CoreModeType;

/*=======[E X T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
extern CONST(Os_MemProtKnCfgType, OS_CONST) Os_MemProtKnAddrCfg;
#endif

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
extern FUNC(void, OS_CODE) Os_ArchStartCore(Os_CoreIdType coreId);
extern FUNC(Os_CoreModeType, OS_CODE) Os_GetCoreMode(Os_CoreIdType core);
extern FUNC(boolean, OS_CODE) Os_SetCoreMode(Os_CoreIdType core, Os_CoreModeType coreMode);
extern FUNC(Os_CoreIdType, OS_CODE) Os_ArchGetCoreID(void);

#if (CFG_TASK_MAX_CORE0 > 0U)
extern FUNC(void, OS_APPL_CODE) IdleHook_Core0(void);
extern FUNC(void, OS_CODE) Os_TaskEntry_IdleCore0(void);
#endif /* CFG_TASK_MAX_CORE0 > 0U */
#if (CFG_TASK_MAX_CORE1 > 0U)
extern FUNC(void, OS_APPL_CODE) IdleHook_Core1(void);
extern FUNC(void, OS_CODE) Os_TaskEntry_IdleCore1(void);
#endif /* CFG_TASK_MAX_CORE1 > 0U */
#if (CFG_TASK_MAX_CORE2 > 0U)
extern FUNC(void, OS_APPL_CODE) IdleHook_Core2(void);
extern FUNC(void, OS_CODE) Os_TaskEntry_IdleCore2(void);
#endif /* CFG_TASK_MAX_CORE2 > 0U */
#if (CFG_TASK_MAX_CORE3 > 0U)
extern FUNC(void, OS_APPL_CODE) IdleHook_Core3(void);
extern FUNC(void, OS_CODE) Os_TaskEntry_IdleCore3(void);
#endif /* CFG_TASK_MAX_CORE3 > 0U */

#endif /* ARCH_MCU_H__ */
/*=======[E N D   O F   F I L E]==============================================*/
