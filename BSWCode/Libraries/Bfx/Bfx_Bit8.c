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
 **  FILENAME    : Bfx_Bit8.c                                                  **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : stanleyluo                                                  **
 **  Vendor      :                                                             **
 **  DESCRIPTION : Implementation for APIs of 8-bit data                       **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Bfx.h"
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define BFX_CONST_BIT8_ONE       (uint8)1
#define BFX_CONST_BIT8_TWO       (uint8)2
#define BFX_CONST_BIT8_BITLEN    (uint8)8
#define BFX_CONST_BIT8_LSB_BITPN (uint8)0
#define BFX_CONST_BIT8_MSB_BITPN (uint8)7

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
/**
 * Set the request bit position of the input value to 1.
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): bitPn, Bit position
 * Parameters(INOUT): data, Pointer to input value
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_SetBit_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitPn)
{
    *data = *data | (uint8)(BFX_CONST_BIT8_ONE << bitPn);
}

/**
 * Set the request bit position of the input value to 0.
 * Service ID: 0x06
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): bitPn, Bit position
 * Parameters(INOUT): data, Pointer to input value
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_ClrBit_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitPn)
{
    *data = (uint8)(*data & ~(BFX_CONST_BIT8_ONE << bitPn));
}

/**
 * return the logical level of the input value at the requested bit position.
 * Service ID: 0x0a
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): data, input value
 *                 bitPn, Bit position
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: Bit Status
 */
FUNC(boolean, BFX_BIT8_CODE)
Bfx_GetBit_u8u8_u8(uint8 data, uint8 bitPn)
{
    boolean result = FALSE;

    if (0 != (data & (BFX_CONST_BIT8_ONE << bitPn)))
    {
        result = TRUE;
    }
    return result;
}

/**
 * Set the input value to "1" or "0" based on the "Status"
 * data from "BitStartPn" to "BitStartPn+BitLn".
 * Service ID: 0x20
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): bitStartPn, Start bit position
 *                 bitLn, Bit field length
 *                 status, Status value
 * Parameters(INOUT):data, Pointer to input value
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_SetBits_u8u8u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitStartPn, uint8 bitLn, uint8 status)
{
    if (0 == status)
    {
        /*clear bits*/
        while (bitLn > 0)
        {
            bitLn--;
            *data = (uint8)(*data & ~(BFX_CONST_BIT8_ONE << (bitStartPn + bitLn)));
        }
    }
    else
    {
        /*set bits as 1*/
        while (bitLn > 0)
        {
            bitLn--;
            *data = *data | (uint8)(BFX_CONST_BIT8_ONE << (bitStartPn + bitLn));
        }
    }
}

/**
 * return the Bits of the input value starting from 'BitStartPn' for the length
 *  of 'BitLn'.
 * Service ID: 0x26
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): data, input value
 *                 bitStartPn, Start bit position
 *                 bitLn, Bit field length
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: Bit field sequence
 */
FUNC(uint8, BFX_BIT8_CODE)
Bfx_GetBits_u8u8u8_u8(uint8 data, uint8 bitStartPn, uint8 bitLn)
{
    uint8 result;
    uint8 mask = 0;

    /*calculate the mask value*/
    Bfx_SetBits_u8u8u8u8(&mask, bitStartPn, bitLn, BFX_CONST_BIT8_ONE);
    /*mask the data and shift*/
    result = (uint8)((uint8)(data & mask) >> bitStartPn);

    return result;
}

/**
 * set the data to logical status '1' as per the corresponding Mask bits when
 * set to value 1 and remaining bits will retain their original values.
 * Service ID: 0x2a
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): mask, Mask used to set bits
 * Parameters(INOUT):data, Pointer to input value
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_SetBitMask_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 mask)
{
    *data = (uint8)(*data | mask);
}

/**
 * clear the logical status to '0' for the input value for all the bit positions
 *  as per the mask.
 * Service ID: 0x30
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): mask, mask value
 * Parameters(INOUT):data, Pointer to input value
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_ClrBitMask_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 mask)
{
    *data = (uint8)(*data & ~mask);
}

/**
 * return TRUE, if all bits defined in Mask value are set in the input value
 * value. In all other cases this function shall return FALSE.
 * Service ID: 0x36
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):data, input value
 *                mask, mask value
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: value
 */
FUNC(boolean, BFX_BIT8_CODE)
Bfx_TstBitMask_u8u8_u8(uint8 data, uint8 mask)
{
    boolean result = FALSE;

    if ((uint8)(data & mask) == mask)
    {
        result = TRUE;
    }

    return result;
}

/**
 * makes a test on the input value and if at least one bit is set as per the
 * mask, then the function shall return TRUE, otherwise it shall return FALSE.
 * Service ID: 0x3a
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):data, input value
 *                mask, mask value
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: data
 */
FUNC(boolean, BFX_BIT8_CODE)
Bfx_TstBitLnMask_u8u8_u8(uint8 data, uint8 mask)
{
    boolean result = FALSE;

    if ((data & mask) != 0)
    {
        result = TRUE;
    }
    return result;
}

/**
 * tests the number of bits set to 1. If this number is even, it shall return
 * TRUE, otherwise it returns FALSE.
 * Service ID: 0x40
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):data, input value
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value: status
 */
FUNC(boolean, BFX_BIT8_CODE)
Bfx_TstParityEven_u8_u8(uint8 data)
{
    boolean result;
    uint8 setCount = 0;
    uint8 bitPn = 0;

    while (bitPn < BFX_CONST_BIT8_BITLEN)
    {
        result = Bfx_GetBit_u8u8_u8(data, bitPn);
        if (TRUE == result)
        {
            setCount++;
        }
        bitPn++;
    }

    if (0 == (setCount % BFX_CONST_BIT8_TWO))
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    return result;
}

/**
 * toggles all the bits of data (1's Complement Data).
 * Service ID: 0x46
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):NA
 * Parameters(INOUT):data, Pointer to input value
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_ToggleBits_u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data)
{
    *data = (uint8)(~(*data));
}

/**
 * toggles the bits of data when the corresponding bit of the mask is enabled
 * and set to 1.
 * Service ID: 0x4a
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):mask, mask value
 * Parameters(INOUT):data, Pointer to input value
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_ToggleBitMask_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 mask)
{
    *data = (uint8)(*data ^ mask);
}

/**
 * shift data to the right by ShiftCnt. The most significant bit (left-most bit)
 * is replaced by a '0' bit and the least significant bit (right-most bit) is
 * discarded for every single bit shift cycle.
 * Service ID: 0x50
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):shiftCnt, Shift right count
 * Parameters(INOUT):data, Pointer to input value
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_ShiftBitRt_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 shiftCnt)
{
    *data = (uint8)(*data >> shiftCnt);
}

/**
 * shift data to the left by ShiftCnt. The least significant bit (right-most bit)
 * is replaced by a '0' bit and the most significant bit (left-most bit) is
 * discarded for every single bit shift cycle.
 * Service ID: 0x56
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):shiftCnt, Shift left count
 * Parameters(INOUT):data, Pointer to input value
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_ShiftBitLt_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 shiftCnt)
{
    *data = (uint8)(*data << shiftCnt);
}

/**
 * rotate data to the right by ShiftCnt. The least significant bit is rotated
 * to the most significant bit location for every single bit shift cycle.
 * Service ID: 0x5a
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):shiftCnt, Shift count
 * Parameters(INOUT):data, Pointer to input value
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_RotBitRt_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 shiftCnt)
{
    boolean bit0Value;

    while (shiftCnt > 0)
    {
        bit0Value = Bfx_GetBit_u8u8_u8(*data, BFX_CONST_BIT8_LSB_BITPN);
        *data = (uint8)(*data >> BFX_CONST_BIT8_ONE);
        if (TRUE == bit0Value)
        {
            Bfx_SetBit_u8u8(data, BFX_CONST_BIT8_MSB_BITPN);
        }
        shiftCnt--;
    }
}

/**
 * rotate data to the left by ShiftCnt. The most significant bit is rotated to
 * the least significant bit location for every single bit shift cycle.
 * Service ID: 0x60
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):shiftCnt, Shift count
 * Parameters(INOUT):data, Pointer to input value
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_RotBitLt_u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 shiftCnt)
{
    boolean bit0Value;

    while (shiftCnt > 0)
    {
        bit0Value = Bfx_GetBit_u8u8_u8(*data, BFX_CONST_BIT8_MSB_BITPN);
        *data = (uint8)(*data << BFX_CONST_BIT8_ONE);
        if (TRUE == bit0Value)
        {
            Bfx_SetBit_u8u8(data, BFX_CONST_BIT8_LSB_BITPN);
        }
        shiftCnt--;
    }
}

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
Bfx_CopyBit_u8u8u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) destData, uint8 destPn, uint8 srcData, uint8 srcPn)
{
    boolean bitSet;

    bitSet = Bfx_GetBit_u8u8_u8(srcData, srcPn);

    if (TRUE == bitSet)
    {
        Bfx_SetBit_u8u8(destData, destPn);
    }
    else
    {
        Bfx_ClrBit_u8u8(destData, destPn);
    }
}

/**
 * put bits as mentioned in Pattern to the input value from the specified bit
 * position.
 * Service ID: 0x70
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):bitStartPn, Start bit position
 *                bitLn, Bit field length
 *                pattern, Pattern to be set
 * Parameters(INOUT):data, Pointer to input value
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_PutBits_u8u8u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitStartPn, uint8 bitLn, uint8 pattern)
{
    uint8 mask = 0;

    Bfx_SetBits_u8u8u8u8(&mask, bitStartPn, bitLn, BFX_CONST_BIT8_ONE);
    Bfx_ClrBitMask_u8u8(data, mask);
    Bfx_ShiftBitLt_u8u8(&pattern, bitStartPn);
    Bfx_SetBitMask_u8u8(data, pattern);
}

/**
 * put all bits defined in Pattern and for which the corresponding Mask bit is
 *  set to 1 in the input value.
 * Service ID: 0x80
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):pattern, Pattern to be set
 *                mask, mask value
 * Parameters(INOUT):data, Pointer to input value
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_PutBitsMask_u8u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 pattern, uint8 mask)
{
    Bfx_ClrBitMask_u8u8(data, mask);
    pattern = (uint8)(pattern & mask);
    Bfx_SetBitMask_u8u8(data, pattern);
}

/**
 * update the bit specified by BitPn of input value as '1' or '0' as per
 * 'Status' value.
 * Service ID: 0x85
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):bitPn, Bit position
 *                status, status value
 * Parameters(INOUT):data, Pointer to input value
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BFX_BIT8_CODE)
Bfx_PutBit_u8u8u8(P2VAR(uint8, AUTOMATIC, BFX_APPL_DATA) data, uint8 bitPn, boolean status)
{
    if (TRUE == status)
    {
        Bfx_SetBit_u8u8(data, bitPn);
    }
    else
    {
        Bfx_ClrBit_u8u8(data, bitPn);
    }
}

#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                            General Notes                                   **
*******************************************************************************/
