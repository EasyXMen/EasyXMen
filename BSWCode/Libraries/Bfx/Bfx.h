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
**  FILENAME    : Bfx.h                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : stanleyluo                                                  **
**  Vendor      :                                                             **
**  DESCRIPTION : Public function prototypes and types defined by the BFX     **
**                library specification                                       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
/*  <VERSION>    <DATE>      <AUTHOR>       <REVISION LOG>
 *  V1.0.0       2024-04-16  chenqinmei      fix CPT-8741 the definition of some variables is wrong
 *
*******************************************************************************/

#ifndef BFX_H
#define BFX_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Std_Types.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*Published information elements*/
#define BFX_H_VENDOR_ID                   (uint16)62
#define BFX_H_MODULE_ID                   (uint16)205
#define BFX_H_AR_RELEASE_MAJOR_VERSION    (uint8)4
#define BFX_H_AR_RELEASE_MINOR_VERSION    (uint8)2
#define BFX_H_AR_RELEASE_REVISION_VERSION (uint8)2

#define BFX_H_SW_MAJOR_VERSION            (uint8)1
#define BFX_H_SW_MINOR_VERSION            (uint8)0
#define BFX_H_SW_PATCH_VERSION            (uint8)0

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/**
 * set logical status of input data as '1' at the requested bit position.
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): bitPn, Bit position
 * Parameters(INOUT): data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_SetBit_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitPn);

/**
 * clear the logical status of the input data to '0' at the requested bit position.
 * Service ID: 0x06
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): bitPn, Bit position
 * Parameters(INOUT): data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_ClrBit_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitPn);

/**
 * return the logical status of the input data for the requested bit position.
 * Service ID: 0x0a
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): data, input data
 *                 bitPn, Bit position
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: Bit Status
 */
FUNC(boolean, BFX_BIT8_CODE)
Bfx_GetBit_u8u8_u8(uint8 data, uint8 bitPn);

/**
 * set the input data as '1' or '0' as per 'Status' value starting from
 * 'BitStartPn' for the length 'BitLn'.
 * Service ID: 0x20
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): bitStartPn, Start bit position
 *                 bitLn, Bit field length
 *                 status, Status value
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_SetBits_u8u8u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitStartPn, uint8 bitLn, uint8 status);

/**
 * return the Bits of the input data starting from 'BitStartPn' for the length
 *  of 'BitLn'.
 * Service ID: 0x26
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): data, input data
 *                 bitStartPn, Start bit position
 *                 bitLn, Bit field length
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: Bit field sequence
 */
FUNC(uint8, BFX_BIT8_CODE)
Bfx_GetBits_u8u8u8_u8(uint8 data, uint8 bitStartPn, uint8 bitLn);

/**
 * set the data to logical status '1' as per the corresponding Mask bits when
 * set to value 1 and remaining bits will retain their original values.
 * Service ID: 0x2a
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): mask, Mask used to set bits
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_SetBitMask_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 mask);

/**
 * clear the logical status to '0' for the input data for all the bit positions
 *  as per the mask.
 * Service ID: 0x30
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): mask, mask value
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_ClrBitMask_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 mask);

/**
 * return TRUE, if all bits defined in Mask value are set in the input Data
 * value. In all other cases this function shall return FALSE.
 * Service ID: 0x36
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):data, input data
 *                mask, mask value
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: value
 */
FUNC(boolean, BFX_BIT8_CODE)
Bfx_TstBitMask_u8u8_u8(uint8 data, uint8 mask);

/**
 * makes a test on the input data and if at least one bit is set as per the
 * mask, then the function shall return TRUE, otherwise it shall return FALSE.
 * Service ID: 0x3a
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):data, input data
 *                mask, mask value
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: data
 */
FUNC(boolean, BFX_BIT8_CODE)
Bfx_TstBitLnMask_u8u8_u8(uint8 data, uint8 mask);

/**
 * tests the number of bits set to 1. If this number is even, it shall return
 * TRUE, otherwise it returns FALSE.
 * Service ID: 0x40
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):data, input data
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: status
 */
FUNC(boolean, BFX_BIT8_CODE)
Bfx_TstParityEven_u8_u8(uint8 data);

/**
 * toggles all the bits of data (1's Complement Data).
 * Service ID: 0x46
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):NA
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_ToggleBits_u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data);

/**
 * toggles the bits of data when the corresponding bit of the mask is enabled
 * and set to 1.
 * Service ID: 0x4a
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):mask, mask value
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_ToggleBitMask_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 mask);

/**
 * shift data to the right by ShiftCnt. The most significant bit (left-most bit)
 * is replaced by a '0' bit and the least significant bit (right-most bit) is
 * discarded for every single bit shift cycle.
 * Service ID: 0x50
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):shiftCnt, Shift right count
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_ShiftBitRt_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 shiftCnt);

/**
 * shift data to the left by ShiftCnt. The least significant bit (right-most bit)
 * is replaced by a '0' bit and the most significant bit (left-most bit) is
 * discarded for every single bit shift cycle.
 * Service ID: 0x56
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):shiftCnt, Shift left count
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_ShiftBitLt_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 shiftCnt);

/**
 * rotate data to the right by ShiftCnt. The least significant bit is rotated
 * to the most significant bit location for every single bit shift cycle.
 * Service ID: 0x5a
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):shiftCnt, Shift count
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_RotBitRt_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 shiftCnt);

/**
 * rotate data to the left by ShiftCnt. The most significant bit is rotated to
 * the least significant bit location for every single bit shift cycle.
 * Service ID: 0x60
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):shiftCnt, Shift count
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_RotBitLt_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 shiftCnt);

/**
 * copy a bit from source data from bit position to destination data at bit
 * position.
 * Service ID: 0x66
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):destPn, Destination position
 *                srcData, Source data
 *                srcPn, Source position
 * Parameters(INOUT):destData, Pointer to destination data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_CopyBit_u8u8u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) destData, uint8 destPn, uint8 srcData, uint8 srcPn);

/**
 * put bits as mentioned in Pattern to the input Data from the specified bit
 * position.
 * Service ID: 0x70
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):bitStartPn, Start bit position
 *                bitLn, Bit field length
 *                pattern, Pattern to be set
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_PutBits_u8u8u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitStartPn, uint8 bitLn, uint8 pattern);

/**
 * put all bits defined in Pattern and for which the corresponding Mask bit is
 *  set to 1 in the input Data.
 * Service ID: 0x80
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):pattern, Pattern to be set
 *                mask, mask value
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_PutBitsMask_u8u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 pattern, uint8 mask);

/**
 * update the bit specified by BitPn of input data as '1' or '0' as per
 * 'Status' value.
 * Service ID: 0x85
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):bitPn, Bit position
 *                status, status value
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_PutBit_u8u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitPn, boolean status);

/**
 * set logical status of input data as '1' at the requested bit position.
 * Service ID: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): bitPn, Bit position
 * Parameters(INOUT): data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT16_CODE)
Bfx_SetBit_u16u8(P2VAR(uint16, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitPn);

/**
 * clear the logical status of the input data to '0' at the requested bit position.
 * Service ID: 0x07
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): bitPn, Bit position
 * Parameters(INOUT): data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT16_CODE)
Bfx_ClrBit_u16u8(P2VAR(uint16, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitPn);

/**
 * return the logical status of the input data for the requested bit position.
 * Service ID: 0x0b
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): data, input data
 *                 bitPn, Bit position
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: Bit Status
 */
FUNC(boolean, BFX_BIT16_CODE)
Bfx_GetBit_u16u8_u8(uint16 data, uint8 bitPn);

/**
 * set the input data as '1' or '0' as per 'Status' value starting from
 * 'BitStartPn' for the length 'BitLn'.
 * Service ID: 0x21
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): bitStartPn, Start bit position
 *                 bitLn, Bit field length
 *                 status, Status value
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT16_CODE)
Bfx_SetBits_u16u8u8u8(P2VAR(uint16, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitStartPn, uint8 bitLn, uint8 status);

/**
 * return the Bits of the input data starting from 'BitStartPn' for the length
 *  of 'BitLn'.
 * Service ID: 0x27
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): data, input data
 *                 bitStartPn, Start bit position
 *                 bitLn, Bit field length
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: Bit field sequence
 */
FUNC(uint16, BFX_BIT16_CODE)
Bfx_GetBits_u16u8u8_u16(uint16 data, uint8 bitStartPn, uint8 bitLn);

/**
 * set the data to logical status '1' as per the corresponding Mask bits when
 * set to value 1 and remaining bits will retain their original values.
 * Service ID: 0x2b
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): mask, Mask used to set bits
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT16_CODE)
Bfx_SetBitMask_u16u16(P2VAR(uint16, AUTOMATIC, BFX_APPL_DATA) data, uint16 mask);

/**
 * clear the logical status to '0' for the input data for all the bit positions
 *  as per the mask.
 * Service ID: 0x31
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): mask, mask value
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT16_CODE)
Bfx_ClrBitMask_u16u16(P2VAR(uint16, AUTOMATIC, BFX_APPL_DATA) data, uint16 mask);

/**
 * return TRUE, if all bits defined in Mask value are set in the input Data
 * value. In all other cases this function shall return FALSE.
 * Service ID: 0x37
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):data, input data
 *                mask, mask value
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: value
 */
FUNC(boolean, BFX_BIT16_CODE)
Bfx_TstBitMask_u16u16_u8(uint16 data, uint16 mask);

/**
 * makes a test on the input data and if at least one bit is set as per the
 * mask, then the function shall return TRUE, otherwise it shall return FALSE.
 * Service ID: 0x3b
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):data, input data
 *                mask, mask value
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: data
 */
FUNC(boolean, BFX_BIT16_CODE)
Bfx_TstBitLnMask_u16u16_u8(uint16 data, uint16 mask);

/**
 * tests the number of bits set to 1. If this number is even, it shall return
 * TRUE, otherwise it returns FALSE.
 * Service ID: 0x41
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):data, input data
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: status
 */
FUNC(boolean, BFX_BIT16_CODE)
Bfx_TstParityEven_u16_u8(uint16 data);

/**
 * toggles all the bits of data (1's Complement Data).
 * Service ID: 0x47
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):NA
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT16_CODE)
Bfx_ToggleBits_u16(P2VAR(uint16, AUTOMATIC, BFX_APPL_DATA) data);

/**
 * toggles the bits of data when the corresponding bit of the mask is enabled
 * and set to 1.
 * Service ID: 0x4b
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):mask, mask value
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT16_CODE)
Bfx_ToggleBitMask_u16u16(P2VAR(uint16, AUTOMATIC, BFX_APPL_DATA) data, uint16 mask);

/**
 * shift data to the right by ShiftCnt. The most significant bit (left-most bit)
 * is replaced by a '0' bit and the least significant bit (right-most bit) is
 * discarded for every single bit shift cycle.
 * Service ID: 0x51
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):shiftCnt, Shift right count
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT16_CODE)
Bfx_ShiftBitRt_u16u8(P2VAR(uint16, AUTOMATIC, BFX_APPL_DATA) data, uint8 shiftCnt);

/**
 * shift data to the left by ShiftCnt. The least significant bit (right-most bit)
 * is replaced by a '0' bit and the most significant bit (left-most bit) is
 * discarded for every single bit shift cycle.
 * Service ID: 0x57
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):shiftCnt, Shift left count
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT16_CODE)
Bfx_ShiftBitLt_u16u8(P2VAR(uint16, AUTOMATIC, BFX_APPL_DATA) data, uint8 shiftCnt);

/**
 * rotate data to the right by ShiftCnt. The least significant bit is rotated
 * to the most significant bit location for every single bit shift cycle.
 * Service ID: 0x5b
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):shiftCnt, Shift count
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT16_CODE)
Bfx_RotBitRt_u16u8(P2VAR(uint16, AUTOMATIC, BFX_APPL_DATA) data, uint8 shiftCnt);

/**
 * rotate data to the left by ShiftCnt. The most significant bit is rotated to
 * the least significant bit location for every single bit shift cycle.
 * Service ID: 0x61
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):shiftCnt, Shift count
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT16_CODE)
Bfx_RotBitLt_u16u8(P2VAR(uint16, AUTOMATIC, BFX_APPL_DATA) data, uint8 shiftCnt);

/**
 * copy a bit from source data from bit position to destination data at bit
 * position.
 * Service ID: 0x67
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):destPn, Destination position
 *                srcData, Source data
 *                srcPn, Source position
 * Parameters(INOUT):destData, Pointer to destination data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT16_CODE)
Bfx_CopyBit_u16u8u16u8(P2VAR(uint16, AUTOMATIC, BFX_APPL_DATA) destData, uint8 destPn, uint16 srcData, uint8 srcPn);

/**
 * put bits as mentioned in Pattern to the input Data from the specified bit
 * position.
 * Service ID: 0x71
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):bitStartPn, Start bit position
 *                bitLn, Bit field length
 *                pattern, Pattern to be set
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT16_CODE)
Bfx_PutBits_u16u8u8u16(P2VAR(uint16, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitStartPn, uint8 bitLn, uint16 pattern);

/**
 * put all bits defined in Pattern and for which the corresponding Mask bit is
 *  set to 1 in the input Data.
 * Service ID: 0x81
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):pattern, Pattern to be set
 *                mask, mask value
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT16_CODE)
Bfx_PutBitsMask_u16u16u16(P2VAR(uint16, AUTOMATIC, BFX_APPL_DATA) data, uint16 pattern, uint16 mask);

/**
 * update the bit specified by BitPn of input data as '1' or '0' as per
 * 'Status' value.
 * Service ID: 0x86
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):bitPn, Bit position
 *                status, status value
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT16_CODE)
Bfx_PutBit_u16u8u8(P2VAR(uint16, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitPn, boolean status);

/**
 * set logical status of input data as '1' at the requested bit position.
 * Service ID: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): bitPn, Bit position
 * Parameters(INOUT): data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT32_CODE)
Bfx_SetBit_u32u8(P2VAR(uint32, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitPn);

/**
 * clear the logical status of the input data to '0' at the requested bit position.
 * Service ID: 0x08
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): bitPn, Bit position
 * Parameters(INOUT): data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT32_CODE)
Bfx_ClrBit_u32u8(P2VAR(uint32, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitPn);

/**
 * return the logical status of the input data for the requested bit position.
 * Service ID: 0x0c
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): data, input data
 *                 bitPn, Bit position
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: Bit Status
 */
FUNC(boolean, BFX_BIT32_CODE)
Bfx_GetBit_u32u8_u8(uint32 data, uint8 bitPn);

/**
 * set the input data as '1' or '0' as per 'Status' value starting from
 * 'BitStartPn' for the length 'BitLn'.
 * Service ID: 0x22
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): bitStartPn, Start bit position
 *                 bitLn, Bit field length
 *                 status, Status value
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT32_CODE)
Bfx_SetBits_u32u8u8u8(P2VAR(uint32, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitStartPn, uint8 bitLn, uint8 status);

/**
 * return the Bits of the input data starting from 'BitStartPn' for the length
 *  of 'BitLn'.
 * Service ID: 0x28
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): data, input data
 *                 bitStartPn, Start bit position
 *                 bitLn, Bit field length
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: Bit field sequence
 */
FUNC(uint32, BFX_BIT32_CODE)
Bfx_GetBits_u32u8u8_u32(uint32 data, uint8 bitStartPn, uint8 bitLn);

/**
 * set the data to logical status '1' as per the corresponding Mask bits when
 * set to value 1 and remaining bits will retain their original values.
 * Service ID: 0x2c
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): mask, Mask used to set bits
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT32_CODE)
Bfx_SetBitMask_u32u32(P2VAR(uint32, AUTOMATIC, BFX_APPL_DATA) data, uint32 mask);

/**
 * clear the logical status to '0' for the input data for all the bit positions
 *  as per the mask.
 * Service ID: 0x32
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): mask, mask value
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT32_CODE)
Bfx_ClrBitMask_u32u32(P2VAR(uint32, AUTOMATIC, BFX_APPL_DATA) data, uint32 mask);

/**
 * return TRUE, if all bits defined in Mask value are set in the input Data
 * value. In all other cases this function shall return FALSE.
 * Service ID: 0x38
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):data, input data
 *                mask, mask value
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: value
 */
FUNC(boolean, BFX_BIT32_CODE)
Bfx_TstBitMask_u32u32_u8(uint32 data, uint32 mask);

/**
 * makes a test on the input data and if at least one bit is set as per the
 * mask, then the function shall return TRUE, otherwise it shall return FALSE.
 * Service ID: 0x3c
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):data, input data
 *                mask, mask value
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: data
 */
FUNC(boolean, BFX_BIT32_CODE)
Bfx_TstBitLnMask_u32u32_u8(uint32 data, uint32 mask);

/**
 * tests the number of bits set to 1. If this number is even, it shall return
 * TRUE, otherwise it returns FALSE.
 * Service ID: 0x42
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):data, input data
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: status
 */
FUNC(boolean, BFX_BIT32_CODE)
Bfx_TstParityEven_u32_u8(uint32 data);

/**
 * toggles all the bits of data (1's Complement Data).
 * Service ID: 0x48
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):NA
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT32_CODE)
Bfx_ToggleBits_u32(P2VAR(uint32, AUTOMATIC, BFX_APPL_DATA) data);

/**
 * toggles the bits of data when the corresponding bit of the mask is enabled
 * and set to 1.
 * Service ID: 0x4c
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):mask, mask value
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT32_CODE)
Bfx_ToggleBitMask_u32u32(P2VAR(uint32, AUTOMATIC, BFX_APPL_DATA) data, uint32 mask);

/**
 * shift data to the right by ShiftCnt. The most significant bit (left-most bit)
 * is replaced by a '0' bit and the least significant bit (right-most bit) is
 * discarded for every single bit shift cycle.
 * Service ID: 0x52
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):shiftCnt, Shift right count
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT32_CODE)
Bfx_ShiftBitRt_u32u8(P2VAR(uint32, AUTOMATIC, BFX_APPL_DATA) data, uint8 shiftCnt);

/**
 * shift data to the left by ShiftCnt. The least significant bit (right-most bit)
 * is replaced by a '0' bit and the most significant bit (left-most bit) is
 * discarded for every single bit shift cycle.
 * Service ID: 0x58
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):shiftCnt, Shift left count
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT32_CODE)
Bfx_ShiftBitLt_u32u8(P2VAR(uint32, AUTOMATIC, BFX_APPL_DATA) data, uint8 shiftCnt);

/**
 * rotate data to the right by ShiftCnt. The least significant bit is rotated
 * to the most significant bit location for every single bit shift cycle.
 * Service ID: 0x5c
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):shiftCnt, Shift count
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT32_CODE)
Bfx_RotBitRt_u32u8(P2VAR(uint32, AUTOMATIC, BFX_APPL_DATA) data, uint8 shiftCnt);

/**
 * rotate data to the left by ShiftCnt. The most significant bit is rotated to
 * the least significant bit location for every single bit shift cycle.
 * Service ID: 0x62
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):shiftCnt, Shift count
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT32_CODE)
Bfx_RotBitLt_u32u8(P2VAR(uint32, AUTOMATIC, BFX_APPL_DATA) data, uint8 shiftCnt);

/**
 * copy a bit from source data from bit position to destination data at bit
 * position.
 * Service ID: 0x68
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):destPn, Destination position
 *                srcData, Source data
 *                srcPn, Source position
 * Parameters(INOUT):destData, Pointer to destination data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT32_CODE)
Bfx_CopyBit_u32u8u32u8(P2VAR(uint32, AUTOMATIC, BFX_APPL_DATA) destData, uint8 destPn, uint32 srcData, uint8 srcPn);

/**
 * put bits as mentioned in Pattern to the input Data from the specified bit
 * position.
 * Service ID: 0x72
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):bitStartPn, Start bit position
 *                bitLn, Bit field length
 *                pattern, Pattern to be set
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT32_CODE)
Bfx_PutBits_u32u8u8u32(P2VAR(uint32, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitStartPn, uint8 bitLn, uint32 pattern);

/**
 * put all bits defined in Pattern and for which the corresponding Mask bit is
 *  set to 1 in the input Data.
 * Service ID: 0x82
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):pattern, Pattern to be set
 *                mask, mask value
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT32_CODE)
Bfx_PutBitsMask_u32u32u32(P2VAR(uint32, AUTOMATIC, BFX_APPL_DATA) data, uint32 pattern, uint32 mask);

/**
 * update the bit specified by BitPn of input data as '1' or '0' as per
 * 'Status' value.
 * Service ID: 0x87
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):bitPn, Bit position
 *                status, status value
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT32_CODE)
Bfx_PutBit_u32u8u8(P2VAR(uint32, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitPn, boolean status);

/**
 * Returns the version information of this library.
 * Service ID: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):bitPn, Bit position
 *                status, status value
 * Parameters(INOUT):data, Pointer to input data
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_CODE)
Bfx_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, BFX_APPL_DATA) Versioninfo);

#endif /* BFX_H */
