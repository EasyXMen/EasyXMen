/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : EthTrcv_Cbk.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : wanglili                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V2.0.0       2021-02-24  wanglili        R19_11 initial version.
 */
#ifndef ETHTRCV_CBK_H_
#define ETHTRCV_CBK_H_
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "EthTrcv.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

#if (STD_ON == ETHTRCV_DEFINE_MIIRXINDICATION)

/******************************************************************************/
/*
 * Brief               Called when information has been read out via MII interface.
 *                     Triggered by previous Eth_ReadMii call. Can directly be
 *                     called within Eth_ReadMii.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant for the same CtrlIdx, reentrant for different
 * Param-Name[in]      CtrlIdx, Index of the controller within the context of the
 *                     Ethernet Driver
 *                     TrcvIdx, Index of the transceiver on the MII
 *                     RegIdx, Index of the transceiver register on the MII
 *                     RegVal, Value contained in the indexed register
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
extern FUNC(void, ETHTRCV_CODE) EthTrcv_ReadMiiIndication(uint8 CtrlIdx, uint8 TrcvIdx, uint8 RegIdx, uint8 RegVal);

/******************************************************************************/
/*
 * Brief               Called when information has been written via MII interface.
 *                     Triggered by previous Eth_WriteMii call. Can directly be
 *                     called within Eth_WriteMii.
 * ServiceId           0x0a
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant for the same CtrlIdx, reentrant for different
 * Param-Name[in]      CtrlIdx, Index of the controller within the context of the
 *                     Ethernet Driver
 *                     TrcvIdx, Index of the transceiver on the MII
 *                     RegIdx, Index of the transceiver register on the MII
 * Param-Name[out]     NA
 * Param-Name[in/out]  NA
 * Return              NA
 */
/******************************************************************************/
extern FUNC(void, ETHTRCV_CODE) EthTrcv_WriteMiiIndication(uint8 CtrlIdx, uint8 TrcvIdx, uint8 RegIdx);

#endif /*STD_ON == ETHTRCV_DEFINE_MIIRXINDICATION*/

#endif /* ETHTRCV_CBK_H_ */
