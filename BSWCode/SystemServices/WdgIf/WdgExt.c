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
/*********************************************************************************
**                                                                            **
**  FILENAME    : WdgExt.c                                                    **
**                                                                            **
**  Created on  :                                                             **
**  Author      : haibin.shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S):   AUTOSAR classic Platform R19-11                       **
**                                                                            **
*******************************************************************************/

/*===========================[R E V I S I O N   H I S T O R Y]====================================*/
/*================================================================================================*/

/*===========================[V E R S I O N  I N F O R M A T I O N]===============================*/
/*===========================[I N C L U D E S]====================================================*/
#include "Std_Types.h"
#include "WdgIf_Types.h"
#include "WdgExt.h"
#include "WdgIf.h"

/*===========================[V E R S I O N  C H E C K]===========================================*/

#define WDGIF_START_SEC_CODE
#include "WdgIf_MemMap.h"
/**************************************************************************************************/
/*
 * Brief               <Service for triggers the external watchdog hardware>
 * ServiceId           --
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <The WDGIF driver module must not be initialled>
 * CallByAPI           <None>
 * REQ ID              <None>
 */
/**************************************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
void WdgExt_SetTriggerCondition(const uint16 timeout)
{
    (void)(timeout);
    return;
}

/**************************************************************************************************/
/*
 * Brief               <Service for switches the external watchdog into the mode>
 * ServiceId           --
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <mode: 1.WDGIFIF_OFF_MODE; 2.WDGIFIF_SLOW_MODE; 3.WDGIFIF_FAST_MODE>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType: E_NOT_OK, or E_OK>
 * PreCondition        <The WDGIF driver module must not be initialled>
 * CallByAPI           <None>
 * REQ ID              <None>
 */
/**************************************************************************************************/
Std_ReturnType WdgExt_SetMode(const WdgIf_ModeType Mode)
{
    (void)(Mode);
    return E_OK;
}
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
#define WDGIF_STOP_SEC_CODE
#include "WdgIf_MemMap.h"
