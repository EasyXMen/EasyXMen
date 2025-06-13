
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
**  FILENAME    : CanTrcv_TLE9255W.h                                          **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Xinrun.Wang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Private interfaces declared by CanTrcv driver               **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef CANTRCV_TLE9255W_H_
#define CANTRCV_TLE9255W_H_

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "CanTrcv.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
typedef enum CanTrcv_TLE9255W_StatusType_E
{
    CANTRCV_TLE9255W_POR,
    CANTRCV_TLE9255W_SYSERR,
    CANTRCV_TLE9255W_WK,
    CANTRCV_TLE9255W_LWU,
    CANTRCV_TLE9255W_WUF,
    CANTRCV_TLE9255W_WUP,
    CANTRCV_TLE9255W_CANTO,
    CANTRCV_TLE9255W_CANSIL,
    CANTRCV_TLE9255W_CFG_VAL,
    CANTRCV_TLE9255W_SWK_EN,
    CANTRCV_TLE9255W_TXD_TO
} CanTrcv_TLE9255W_StatusType;

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_EnableWakeup(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_SetOpMode(uint8 Transceiver, CanTrcv_TrcvModeType OpMode);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_SetSleepMode(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_SetStandbyMode(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_SetNormalMode(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_ClearWakeupFlags(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_ReadWakeupFlags(uint8 Transceiver);

#if (STD_ON == CANTRCV_HWPN_SUPPORT)
#if (STD_ON == CANTRCV_POWER_ON_FLAG)

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_CheckPorFlag(
    uint8 Transceiver,
    P2VAR(CanTrcv_TrcvFlagStateType, AUTOMATIC, CANTRCV_APPL_DATA) FlagState);

#endif /* (STD_ON == CANTRCV_POWER_ON_FLAG) */

#if (STD_ON == CANTRCV_BUS_ERR_FLAG)

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_CheckSyserrFlag(
    uint8 Transceiver,
    P2VAR(CanTrcv_TrcvFlagStateType, AUTOMATIC, CANTRCV_APPL_DATA) FlagState);

#endif /* (STD_ON == CANTRCV_BUS_ERR_FLAG) */

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_ReadTrcvSilenceFlag(
    uint8 Transceiver,
    P2VAR(CanTrcv_TrcvFlagStateType, AUTOMATIC, CANTRCV_APPL_DATA) FlagState);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_ClearTrcvTimeoutFlag(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_DisablePNWK(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_ReadTrcvTimeoutFlag(
    uint8 Transceiver,
    P2VAR(CanTrcv_TrcvFlagStateType, AUTOMATIC, CANTRCV_APPL_DATA) FlagState);

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_TLE9255W_GetTrcvSystemData(uint8 Transceiver, P2VAR(uint32, AUTOMATIC, CANTRCV_APPL_DATA) TrcvSysData);

#endif /* (STD_ON == CANTRCV_HWPN_SUPPORT) */

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_TLE9255W_ClearStatus(uint8 Transceiver, CanTrcv_TLE9255W_StatusType StatusType);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_ReadStatus(
    uint8 Transceiver,
    CanTrcv_TLE9255W_StatusType StatusType,
    P2VAR(CanTrcv_TrcvFlagStateType, AUTOMATIC, CANTRCV_APPL_DATA) FlagState);

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_TLE9255W_GetOpMode(uint8 Transceiver, P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANTRCV_APPL_DATA) OpMode);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_DeInit(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_PNWKSetup(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_TLE9255W_DisableWakeup(uint8 Transceiver);

#endif /* CANTRCV_TLE9255W_H_ */
