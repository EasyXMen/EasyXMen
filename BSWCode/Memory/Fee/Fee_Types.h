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
**  FILENAME    : Fee_Types.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Type definitions of FEE                                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19_11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
#ifndef FEE_TYPES_H
#define FEE_TYPES_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Std_Types.h"
#include "MemIf_Types.h"
#include "Fls.h"
/*******************************************************************************
**                      STRUCTURES AND OTHER TYPEDEFS                         **
*******************************************************************************/
#if (0)
typedef struct
{
    P2FUNC(Std_ReturnType, FEE_APPL_CODE, Fls_WriteApi)
    (Fls_AddressType TargetAddress, const uint8* SourceAddressPtr, Fls_LengthType Length);
    P2FUNC(Std_ReturnType, FEE_APPL_CODE, Fls_ReadApi)
    (Fls_AddressType SourceAddress, uint8* TargetAddressPtr, Fls_LengthType Length);
    P2FUNC(Std_ReturnType, FEE_APPL_CODE, Fls_EraseApi)(Fls_AddressType TargetAddress, Fls_LengthType Length);
    P2FUNC(void, FEE_APPL_CODE, Fls_CancelApi)(void);
    P2FUNC(Std_ReturnType, FEE_APPL_CODE, Fls_CompareApi)
    (Fls_AddressType SourceAddress, const uint8* TargetAddressPtr, Fls_LengthType Length);
    P2FUNC(MemIf_JobResultType, FEE_APPL_CODE, Fls_GetJobResultApi)(void);
    P2FUNC(MemIf_StatusType, FEE_APPL_CODE, Fls_GetStatusApi)(void);
    P2FUNC(void, FEE_APPL_CODE, Fls_SetModeApi)(MemIf_ModeType Mode);
    P2FUNC(Std_ReturnType, FEE_APPL_CODE, Fls_BlankCheckApi)(Fls_AddressType TargetAddress, Fls_LengthType Length);
} Fee_FlashDrvApiType;
#endif

typedef struct
{
    uint16 FeeBlockNumber;    /*Block identifier (handle)*/
    uint16 FeeBlockSize;      /*Size of a logical block in bytes*/
    boolean FeeImmediateData; /*Marker for high priority data*/
#if (0)
    uint8 FeeDeviceIndex; /*Reference to the device this block is stored in*/
#endif
} Fee_BlockConfigType;

typedef struct
{
    uint16 FeeBlockSum;                                                  /*Block identifier (handle)*/
    P2CONST(Fee_BlockConfigType, AUTOMATIC, FEE_APPL_CONST) FeeBlockRef; /*Every block configuration*/
#if (0)
    P2CONST(Fee_FlashDrvApiType, AUTOMATIC, FEE_APPL_CONST) FeeFlashDrvApiRef;
#endif
} Fee_ConfigType;

typedef struct
{
    Fls_AddressType FeeBankStartAddr;
    Fls_LengthType FeeBankLength;
} Fee_BankConfigType;

#endif /* FEE_TYPES_H */
