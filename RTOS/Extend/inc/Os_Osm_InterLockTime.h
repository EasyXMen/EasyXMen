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
 **  FILENAME    : Os_InterLockTime.h                                              **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION :                                                             **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
#ifndef OS_INTERLOCKTIME_H
#define OS_INTERLOCKTIME_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_Osm_CfgData.h"

/*=======[M A C R O S]========================================================*/
#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE) /* PRQA S 3332 */ /* VL_Os_3332 */

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief          Init the Interrupt Lock Monitor block control.
 * @param[in]      coreId: The id of core.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          -
 */
extern void Os_InterMonitorInit(Os_CoreIdType coreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief          Record the Interrupt Lock Monitor Start Time.
 * @param[in]      ApiType: The type of interrupt API.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          -
 */
extern void Os_InterMonitorStartRecord(Os_InterApiType ApiType);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief          Record the Interrupt Lock Monitor Lock Time to Record Table.
 * @param[in]      ApiType: The type of interrupt API.
 * @return         StatusType
 * @retval         E_OK: Get the Interrupt Lock Monitor Lock Time successfully.
 * @retval         E_NOT_OK: Get the Interrupt Lock Monitor Lock Time failed.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          -
 */
/* PRQA S 3209 ++ */ /* VL_Os_3209 */
extern StatusType Os_InterMonitorEndRecord(Os_InterApiType ApiType);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 3209 -- */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Record the Interrupt Lock Monitor RATIO.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <SuspendAllInterrupts/SuspendOsInterrupts>
 * REQ ID               <None>
 */
/******************************************************************************/
extern void Os_InterMonitorRatio(Os_SCBType* pScb);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#endif
/*=======[E N D   O F   F I L E]==============================================*/
