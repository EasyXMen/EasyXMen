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
 *
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : EthIf.h                                                     **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : YangBo                                                      **
 **  Vendor      :                                                             **
 **  DESCRIPTION :                                                             **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

#ifndef ETHIF_CBK_H
#define ETHIF_CBK_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "ComStack_Types.h"
#include "EthIf_Types.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
/* @req SWS_EthIf_00085 */
extern FUNC(void, ETHIF_CODE) EthIf_RxIndication(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    VAR(Eth_FrameType, ETHIF_VAR) FrameType,
    VAR(boolean, ETHIF_VAR) IsBroadcast,
    P2CONST(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PhysAddrPtr,
    P2CONST(Eth_DataType, ETHIF_APPL_DATA, ETHIF_VAR) DataPtr,
    VAR(uint16, ETHIF_VAR) LenByte);
/* @req SWS_EthIf_00091 */
#if (42u == ETHIF_ETH_MAIJORVERSION)
extern FUNC(void, ETHIF_CODE)
    EthIf_TxConfirmation(VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx);
#else  /* 42u != ETHIF_ETH_MAIJORVERSION ,R19 4.4 */
extern FUNC(void, ETHIF_CODE) EthIf_TxConfirmation(
    VAR(uint8, ETHIF_VAR) CtrlIdx,
    VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx,
    VAR(Std_ReturnType, ETHIF_VAR) Result);
#endif /* 42u == ETHIF_ETH_MAIJORVERSION */

/* @req SWS_EthIf_00231 */
extern FUNC(void, ETHIF_CODE)
    EthIf_CtrlModeIndication(VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(Eth_ModeType, ETHIF_VAR) CtrlMode);
/* @req SWS_EthIf_00232 */
#if (42u == ETHIF_ETH_MAIJORVERSION)
extern FUNC(void, ETHIF_CODE)
    EthIf_TrcvModeIndication(VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(EthTrcv_ModeType, ETHIF_VAR) CtrlMode);
#elif (44u == ETHIF_ETH_MAIJORVERSION)
extern FUNC(void, ETHIF_CODE)
    EthIf_TrcvModeIndication(VAR(uint8, ETHIF_VAR) TrcvIdx, VAR(EthTrcv_ModeType, ETHIF_VAR) CtrlMode);

#elif (19u == ETHIF_ETH_MAIJORVERSION)
extern FUNC(void, ETHIF_CODE)
    EthIf_TrcvModeIndication(VAR(uint8, ETHIF_VAR) TrcvIdx, VAR(Eth_ModeType, ETHIF_VAR) CtrlMode);
#endif /* 42u == ETHIF_ETH_MAIJORVERSION */
#endif
