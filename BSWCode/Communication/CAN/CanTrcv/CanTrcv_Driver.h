
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
**  FILENAME    : CanTrcv_Driver.h                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Xinrun.Wang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Private interfaces declared by CanTrcv driver               **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef CANTRCV_DRIVER_H_
#define CANTRCV_DRIVER_H_

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
typedef enum CanTrcv_Driverr_StatusType_E
{
    CANTRCV_DRIVER_POR,
    CANTRCV_DRIVER_SYSERR,
    CANTRCV_DRIVER_WK,
    CANTRCV_DRIVER_LWU,
    CANTRCV_DRIVER_WUF,
    CANTRCV_DRIVER_WUP,
    CANTRCV_DRIVER_CANTO,
    CANTRCV_DRIVER_CANSIL,
    CANTRCV_DRIVER_BUSERR
} CanTrcv_Driver_StatusType;

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_Init(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_EnableWakeup(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_SetOpMode(uint8 Transceiver, CanTrcv_TrcvModeType OpMode);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_SetSleepMode(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_SetStandbyMode(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_SetNormalMode(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_ClearWakeupFlags(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_ReadWakeupFlags(uint8 Transceiver);

#if (STD_ON == CANTRCV_HWPN_SUPPORT)
#if (STD_ON == CANTRCV_POWER_ON_FLAG)

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_CheckPorFlag(
    uint8 Transceiver,
    P2VAR(CanTrcv_TrcvFlagStateType, AUTOMATIC, CANTRCV_APPL_DATA) FlagState);

#endif /* (STD_ON == CANTRCV_POWER_ON_FLAG) */

#if (STD_ON == CANTRCV_BUS_ERR_FLAG)

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_CheckSyserrFlag(
    uint8 Transceiver,
    P2VAR(CanTrcv_TrcvFlagStateType, AUTOMATIC, CANTRCV_APPL_DATA) FlagState);

#endif /* (STD_ON == CANTRCV_BUS_ERR_FLAG) */

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_ReadTrcvSilenceFlag(
    uint8 Transceiver,
    P2VAR(CanTrcv_TrcvFlagStateType, AUTOMATIC, CANTRCV_APPL_DATA) FlagState);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_ClearTrcvTimeoutFlag(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_DisablePNWK(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_ReadTrcvTimeoutFlag(
    uint8 Transceiver,
    P2VAR(CanTrcv_TrcvFlagStateType, AUTOMATIC, CANTRCV_APPL_DATA) FlagState);

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_Driver_GetTrcvSystemData(uint8 Transceiver, P2VAR(uint32, AUTOMATIC, CANTRCV_APPL_DATA) TrcvSysData);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_PNWKSetup(uint8 Transceiver);

#endif /* (STD_ON == CANTRCV_HWPN_SUPPORT) */

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_Driver_ClearStatus(uint8 Transceiver, CanTrcv_Driver_StatusType StatusType);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_ReadStatus(
    uint8 Transceiver,
    CanTrcv_Driver_StatusType StatusType,
    P2VAR(CanTrcv_TrcvFlagStateType, AUTOMATIC, CANTRCV_APPL_DATA) FlagState);

extern FUNC(Std_ReturnType, CANTRCV_CODE)
    CanTrcv_Driver_GetOpMode(uint8 Transceiver, P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANTRCV_APPL_DATA) OpMode);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_DeInit(uint8 Transceiver);

extern FUNC(Std_ReturnType, CANTRCV_CODE) CanTrcv_Driver_DisableWakeup(uint8 Transceiver);

#endif /* CANTRCV_Driver_H_ */
