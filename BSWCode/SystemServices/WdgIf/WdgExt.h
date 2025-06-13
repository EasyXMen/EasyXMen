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
**  FILENAME    : WdgExt.h                                                    **
**                                                                            **
**  Created on  :                                                             **
**  Author      : haibin.shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S):   AUTOSAR classic Platform R19-11                       **
**                                                                            **
*******************************************************************************/

#ifndef WDGEXT_H_
#define WDGEXT_H_

/*===========================[R E V I S I O N   H I S T O R Y]====================================*/
/*================================================================================================*/

/*===========================[V E R S I O N  I N F O R M A T I O N]===============================*/
/*===========================[I N C L U D E S]====================================================*/
#include "Std_Types.h"
#include "WdgIf_Types.h"

/*===========================[T Y P E   D E F I N I T I O N S]====================================*/

#if !defined(WDGEXT_PARA_UNUSED)
#define WDGEXT_PARA_UNUSED(x) ((void)(x))
#endif /* WDGEXT_PARA_UNUSED */

/**************************************************************************************************/
/*
 * Brief               <Service for triggers the watchdog hardware>
 * ServiceId           <WDGIF_TRIGGER_ID>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <The WDGIF driver module must not be initialled>
 * CallByAPI           <None>
 */
/**************************************************************************************************/
extern void WdgExt_SetTriggerCondition(const uint16 timeout);
/**************************************************************************************************/
/*
 * Brief               <Service for switches the watchdog into the mode>
 * ServiceId           <WDGIF_SETMODE_ID>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <mode: 1.WDGIFIF_OFF_MODE; 2.WDGIFIF_SLOW_MODE; 3.WDGIFIF_FAST_MODE>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType: E_NOT_OK, or E_OK>
 * PreCondition        <The WDGIF driver module must not be initialled>
 * CallByAPI           <None>
 */
/**************************************************************************************************/
extern Std_ReturnType WdgExt_SetMode(const WdgIf_ModeType Mode);

#endif
