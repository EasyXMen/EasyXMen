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
/*
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : Os_Mcu_Core.h                                               **
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
/* PRQA S 3108-- */
#ifndef OS_MCU_CORE_H
#define OS_MCU_CORE_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_Types.h"

/*=======[M A C R O S]========================================================*/
#define CHAR_BIT (8U) /* PRQA S 4600 */ /* VL_Os_4600 */

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

#define PERIPHERAL_START (uint8*)OS_ARCH_PERIPH_ADDR_START
#define PERIPHERAL_END   (uint8*)OS_ARCH_PERIPH_ADDR_END

#define RODATA_START     (uint8*)OS_ARCH_RAM_ADDR_START
#define RODATA_END       (uint8*)OS_ARCH_FLASH_ADDR_END

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_HAL_ADDRESS_IS_IN_ALL_RAM(Address, Size)                    \
    (((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRamStart <= (Address)) \
     && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRamEnd >= ((Address) + (Size))))
/* PRQA S 3472 -- */
/* PRQA S 3472,2053 ++ */ /* VL_Os_3472,2053 */
#define OS_HAL_ADDRESS_IS_NOT_IN_OTHER_APP(Address, Size, coreId, appsuborID)                  \
    ((((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRamStart <= (Address))                        \
      && ((uint32)Os_Core_App_DAddr[0u].AppAddrStart >= ((Address) + (Size))))             \
     || (((uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1u].AppAddrEnd <= (Address))                           \
         && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRamEnd >= ((Address) + (Size)))) \
     || (((uint32)Os_App_DAddr[appsuborID].AppAddrStart <= (Address))                          \
         && ((uint32)Os_App_DAddr[appsuborID].AppAddrEnd >= ((Address) + (Size)))))
/* PRQA S 3472,2053 -- */
#endif /* (TRUE == CFG_MEMORY_PROTECTION_ENABLE) */ /* PRQA S 2053 */ /* VL_Os_2053 */

#if (OS_AUTOSAR_CORES > 1U)
#define Os_Hal_RemoteCall(remoteCoreId)                                \
    {                                                                  \
        OS_IPIR_IPInREQm(Os_Hal_GetCoreID(), 0U) = (1U << Os_ArchGetCorePhyID(remoteCoreId)); \
    }

#define Os_ArchRemoteClear()                                   \
    {                                                          \
        uint8 flags          = OS_IPIR_IPInFLGS(0U);           \
        uint8 flagMask       = flags - ((flags - 1U) & flags); \
        OS_IPIR_IPInFCLRS(0) = flagMask;                       \
    }
/* Boot Control Register */
#define OS_BOOTCTRL_ADDR (*((volatile uint32*)(0xFFFB2000U)))
#endif

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* Core Mode */  /* PRQA S 1271 ++ */ /* VL_NON_INT_TO_ENUM */
typedef enum
{
    OS_CORE_MODE_HALT    = 0U,
    OS_CORE_MODE_RUN     = 1U,
    OS_CORE_MODE_IDLE    = 2U,
    OS_CORE_MODE_SLEEP   = 3U,
    OS_CORE_MODE_STANDBY = 4U,
    OS_CORE_MODE_UNKNOWN = 5U
} Os_CoreModeType;
/* PRQA S 1271 -- */
/* Memory protection Kernel Address */
typedef struct
{
    uint8* pRamStart; /* RAM */
    uint8* pRamEnd;

    uint8* pPeripheralStart; /* PERIPHERAL */
    uint8* pPeripheralEnd;

    uint8* pRomStart; /* PFLASH */
    uint8* pRomEnd;
} Os_MemProtKnAddr;

/* Type of memory protection Kernel */
typedef struct
{
    const Os_MemProtKnAddr OsKernelAddr;
} Os_MemProtKnCfgType;

/*=======[E X T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
extern const Os_MemProtKnCfgType Os_MemProtKnAddrCfg;
#endif

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
extern void            Os_Hal_MultiCoreInit(Os_CoreIdType sysCore);
extern void            Os_Hal_InitCPU(void);
extern void            Os_Hal_StartCore(Os_CoreIdType coreId);
extern Os_CoreModeType Os_Hal_GetCoreMode(Os_CoreIdType core);
extern boolean         Os_Hal_SetCoreMode(Os_CoreIdType core, Os_CoreModeType coreMode);
extern Os_CoreIdType   Os_Hal_GetCoreID(void);
extern CoreIdType      Os_ArchGetCoreLogID(CoreIdType phyCoreId);
extern CoreIdType      Os_ArchGetCorePhyID(CoreIdType logCoreId);
extern void            Os_Hal_CheckCPUInformation(void);

/* PRQA S 3451, 3449 ++ */ /* VL_Os_3451 */ /* VL_Os_3449 */
#if (CFG_TASK_MAX_CORE0 > 0U)
extern void IdleHook_Core0(void);
extern void Os_TaskEntry_IdleCore0(void);
#endif /* CFG_TASK_MAX_CORE0 > 0U */
/* PRQA S 3451, 3449 -- */

#if (CFG_TASK_MAX_CORE1 > 0U) /* PRQA S 3332 */ /* VL_Os_3332 */
extern void IdleHook_Core1(void);/* PRQA S 3332 */ /* VL_Os_3332 */
extern void Os_TaskEntry_IdleCore1(void);
#endif /* CFG_TASK_MAX_CORE1 > 0U */
#if (CFG_TASK_MAX_CORE2 > 0U) /* PRQA S 3332 */ /* VL_Os_3332 */
extern void IdleHook_Core2(void);/* PRQA S 3332 */ /* VL_Os_3332 */
extern void Os_TaskEntry_IdleCore2(void);
#endif /* CFG_TASK_MAX_CORE2 > 0U */
#if (CFG_TASK_MAX_CORE3 > 0U) /* PRQA S 3332 */ /* VL_Os_3332 */
extern void IdleHook_Core3(void);
extern void Os_TaskEntry_IdleCore3(void);
#endif /* CFG_TASK_MAX_CORE3 > 0U */

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
static inline OS_ALWAYS_INLINE Os_CoreIdType Os_GetCoreID(void)
{
#if (OS_AUTOSAR_CORES == 1)
    return (Os_CoreIdType)(0U);
#else
    return Os_ArchGetCoreLogID(Os_Hal_GetCoreID());
#endif
}

#endif /* MCU_CORE_H */
/*=======[E N D   O F   F I L E]==============================================*/
