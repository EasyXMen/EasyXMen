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
#ifndef OS_MCU_CORE_H
#define OS_MCU_CORE_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_Cfg.h"
#include "Os_Mprot_Cfg.h"
#include "Os_Types.h"
#include "Os_Mcu_Config.h"
/*=======[M A C R O S]========================================================*/
#define PERIPHERAL_START (uint8*)OS_MCU_PERIPH_ADDR_START
#define PERIPHERAL_END   (uint8*)OS_MCU_PERIPH_ADDR_END

#define RODATA_START (uint8*)OS_MCU_RAM_ADDR_START
#define RODATA_END   (uint8*)OS_MCU_FLASH_ADDR_END

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_HAL_ADDRESS_IS_IN_ALL_RAM(Address, Size)                    \
    (((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRamStart <= (Address)) \
     && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRamEnd >= ((Address) + (Size))))
/* PRQA S 3472 -- */
/* PRQA S 3472,2053 ++ */ /* VL_Os_3472,VL_Os_2053 */
#define OS_HAL_ADDRESS_IS_NOT_IN_OTHER_APP(Address, Size, coreId, appsuborID)           \
    ((((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRamStart <= (Address))                 \
      && ((uint32)Os_Core_App_DAddr[0u].AppAddrStart >= ((Address) + (Size))))          \
     || (((uint32)Os_Core_App_DAddr[OS_AUTOSAR_CORES - 1u].AppAddrEnd <= (Address))     \
         && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRamEnd >= ((Address) + (Size)))) \
     || (((uint32)Os_App_DAddr[appsuborID].AppAddrStart <= (Address))                   \
         && ((uint32)Os_App_DAddr[appsuborID].AppAddrEnd >= ((Address) + (Size)))))
/* PRQA S 3472,2053 -- */
#endif /* (TRUE == CFG_MEMORY_PROTECTION_ENABLE) */ /* PRQA S 2053 */ /* VL_Os_2053 */

#if (OS_AUTOSAR_CORES > 1U)
#define Os_Hal_RemoteCall(remoteCoreId) /* PRQA S 3458,3472 */ /* VL_Os_3458, VL_Os_3472 */   \
    {                                                                                            \
        OS_MCU_IPIR_IPInREQm(Os_Hal_GetCoreID(), 0U) = (1U << Os_Mcu_GetPhyCoreIdFromLog(remoteCoreId)); \
    }

#define Os_Hal_RemoteClear() /* PRQA S 3458,3472 */ /* VL_Os_3458, VL_Os_3472 */ \
    {                                                              \
        uint8 flags              = OS_MCU_IPIR_IPInFLGS(0U);       \
        uint8 flagMask           = flags - ((flags - 1U) & flags); \
        OS_MCU_IPIR_IPInFCLRS(0) = flagMask;                       \
    }
/* Boot Control Register */
#define OS_MCU_BOOTCTRL_ADDR (*((volatile uint32*)(0xFFFB2000U)))
#endif

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* Core Mode */ /* PRQA S 1271 ++ */ /* VL_Os_1271 */
typedef enum
{
    OS_CORE_MODE_HALT    = 0U,
    OS_CORE_MODE_RUN     = 1U,
    OS_CORE_MODE_IDLE    = 2U,
    OS_CORE_MODE_SLEEP   = 3U,
    OS_CORE_MODE_STANDBY = 4U,
    OS_CORE_MODE_UNKNOWN = 5U
} Os_McuCoreModeType;
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
} Os_McuMemProtKnAddr;

/* Type of memory protection Kernel */
typedef struct
{
    const Os_McuMemProtKnAddr OsKernelAddr;
} Os_McuMemProtKnCfgType;

/*=======[E X T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
extern const Os_McuMemProtKnCfgType Os_MemProtKnAddrCfg;
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#endif
/* PRQA S 3451, 3449 ++ */ /* VL_Os_3451 */ /* VL_Os_3449 */
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void               Os_Hal_MultiCoreInit(Os_CoreIdType sysCore);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void               Os_Hal_InitCPU(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void               Os_Hal_StartCore(Os_CoreIdType coreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern Os_McuCoreModeType Os_Hal_GetCoreMode(Os_CoreIdType core);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern boolean            Os_Hal_SetCoreMode(Os_CoreIdType core, Os_McuCoreModeType coreMode);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern Os_CoreIdType      Os_Hal_GetCoreID(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern Os_CoreIdType         Os_Mcu_GetLogCoreIdFromPhy(Os_CoreIdType phyCoreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern Os_CoreIdType         Os_Mcu_GetPhyCoreIdFromLog(Os_CoreIdType logCoreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void               Os_Hal_CheckCPUInformation(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 3451, 3449 -- */
#endif /* MCU_CORE_H */
/*=======[E N D   O F   F I L E]==============================================*/
