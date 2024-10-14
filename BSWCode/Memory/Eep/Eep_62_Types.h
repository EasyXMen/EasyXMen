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
 **  FILENAME    : Eep_62_Types.h                                              **
 **                                                                            **
 **  Created on  : 2022/02/15                                                  **
 **  Author      : Haibin.Shao                                                 **
 **  Vendor      :                                                             **
 **  DESCRIPTION :                                                             **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */

/*=========[R E V I S I O N   H I S T O R Y]==================================*/
/*  <VERSION>   <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0      20210123    Haibin.Shao     Initial Version

 */
/*============================================================================*/

#ifndef EEP_62_TYPES_H_
#define EEP_62_TYPES_H_

#include "Std_Types.h"
#include "MemIf_Types.h"

typedef uint32 Eep_62_AddressType;
typedef Eep_62_AddressType Eep_62_Write_AddressType;
typedef Eep_62_AddressType Eep_62_Compare_AddressType;
typedef Eep_62_AddressType Eep_62_Erase_AddressType;
typedef Eep_62_AddressType Eep_62_Read_AddressType;
/* Specifies the number of bytes to read/write/erase/compare. */
typedef uint16 Eep_62_LengthType;
typedef Eep_62_LengthType Eep_62_Write_LengthType;
typedef Eep_62_LengthType Eep_62_Compare_LengthType;
typedef Eep_62_LengthType Eep_62_Erase_LengthType;
typedef Eep_62_LengthType Eep_62_Read_LengthType;

/* define EEprom module initiation status */
typedef enum
{
    EEP_62_JOB_NONE = 0,
    EEP_62_JOB_ERASE = 1,
    EEP_62_JOB_WRITE = 2,
    EEP_62_JOB_READ = 3,
    EEP_62_JOB_CANCEL = 4,
    EEP_62_JOB_COMPARE = 5
} Eep_62_RequestJobType;

/* prototype of EEprom driver routine */
typedef struct
{
    Eep_62_AddressType operateAddr;
    uint8* dataBufferPtr;
    const uint8* WriteBuffer;
    Eep_62_LengthType length;
    Eep_62_LengthType CompareLength;
    Eep_62_RequestJobType currentJob;
    Eep_62_LengthType maxReadSize;
    Eep_62_LengthType maxWriteSize;
    MemIf_ModeType currentMode;
    boolean StateForRunMainFunction;
} Eep_62_RuntimeType;

#endif
