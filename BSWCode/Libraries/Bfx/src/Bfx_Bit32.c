/**
 * Copyright (C) 2008-2026 isoft Infrastructure Software Co., Ltd.
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
/*
 ************************************************************************************************************************
 **
 **  @file               : Bfx_Bit32.c
 **  @author             : ShenXu,Yb,QinmeiChen
 **  @date               : 2024/09/09
 **  @vendor             : isoft
 **  @description        : Bfx 32-bit bit manipulation functions
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Bfx_Static.h"
/* ================================================== version check ================================================= */
/* ===================================================== macros ===================================================== */
/* ================================================ type definitions ================================================ */
/* ========================================== internal function declarations ======================================== */
/* ============================================ internal data definitions =========================================== */
/* ============================================ external data definitions =========================================== */
/* ========================================== external function definitions ========================================= */
/*PRQA S 1503,1505,3408 ++ */ /*VL_Bfx_macro*/
/*PRQA S 1512 ++ */           /*VL_Bfx_ODR*/
#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
void Bfx_SetBit_u32u8(uint32* data, uint8 bitPn) /*PRQA S 4533,4393,2985*/ /*VL_Bfx_Signed*/
{
    *data = *data | (uint32)(BFX_CONST_BIT32_ONE << bitPn);
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
void Bfx_ClrBit_u32u8(uint32* data, uint8 bitPn)
{
    *data = (uint32)(*data & (uint32)(~(uint32)(BFX_CONST_BIT32_ONE << bitPn)));
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
boolean Bfx_GetBit_u32u8_u8(uint32 data, uint8 bitPn)
{
    return (0u != (data & (uint32)(BFX_CONST_BIT32_ONE << bitPn)));
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
void Bfx_SetBits_u32u8u8u8(uint32* data, uint8 bitStartPn, uint8 bitLn, uint8 status)
{
    uint8 bitLnTmp = bitLn;
    if (0u == status)
    {
        /*clear bits*/
        while (bitLnTmp > 0u)
        {
            bitLnTmp--;
            *data = (uint32)(*data & (uint32)(~(uint32)(BFX_CONST_BIT32_ONE << (bitStartPn + bitLnTmp))));
        }
    }
    else
    {
        /*set bits as 1*/
        while (bitLnTmp > 0u)
        {
            bitLnTmp--;
            *data = *data | (uint32)(BFX_CONST_BIT32_ONE << (bitStartPn + bitLnTmp));
        }
    }
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
uint32 Bfx_GetBits_u32u8u8_u32(uint32 data, uint8 bitStartPn, uint8 bitLn)
{
    uint32 result;
    uint32 mask = 0u;

    /*calculate the mask value*/
    Bfx_SetBits_u32u8u8u8(&mask, bitStartPn, bitLn, (uint8)BFX_CONST_BIT32_ONE);
    /*mask the data and shift*/
    result = (uint32)((uint32)(data & mask) >> bitStartPn);

    return result;
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
void Bfx_SetBitMask_u32u32(uint32* data, uint32 mask)
{
    *data = (uint32)(*data | mask);
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
void Bfx_ClrBitMask_u32u32(uint32* data, uint32 mask)
{
    *data = (uint32)(*data & (uint32)~mask);
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
boolean Bfx_TstBitMask_u32u32_u8(uint32 data, uint32 mask)
{
    return ((uint32)(data & mask) == mask);
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
boolean Bfx_TstBitLnMask_u32u32_u8(uint32 data, uint32 mask)
{
    boolean result = FALSE;

    if ((data & mask) != 0u)
    {
        result = TRUE;
    }
    return result;
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
boolean Bfx_TstParityEven_u32_u8(uint32 data)
{
    boolean result;
    uint8   setCount = 0u;
    uint8   bitPn    = 0u;

    while (bitPn < BFX_CONST_BIT32_BITLEN)
    {
        result = Bfx_GetBit_u32u8_u8(data, bitPn);
        if (result)
        {
            setCount++;
        }
        bitPn++;
    }

    if (0u == (setCount % BFX_CONST_BIT8_TWO))
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    return result;
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
void Bfx_ToggleBits_u32(uint32* data)
{
    *data = (uint32)(~(*data));
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
void Bfx_ToggleBitMask_u32u32(uint32* data, uint32 mask)
{
    *data = (uint32)(*data ^ mask);
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
void Bfx_ShiftBitRt_u32u8(uint32* data, uint8 shiftCnt)
{
    *data = (uint32)(*data >> shiftCnt);
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
void Bfx_ShiftBitLt_u32u8(uint32* data, uint8 shiftCnt)
{
    *data = (uint32)(*data << shiftCnt);
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
void Bfx_RotBitRt_u32u8(uint32* data, uint8 shiftCnt)
{
    boolean bit0Value;
    uint8   shiftCntTmp = shiftCnt;

    while (shiftCntTmp > 0u)
    {
        bit0Value = Bfx_GetBit_u32u8_u8(*data, BFX_CONST_BIT32_LSB_BITPN);
        *data     = (uint32)(*data >> BFX_CONST_BIT8_ONE);
        if (bit0Value)
        {
            Bfx_SetBit_u32u8(data, BFX_CONST_BIT32_MSB_BITPN);
        }
        shiftCntTmp--;
    }
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
void Bfx_RotBitLt_u32u8(uint32* data, uint8 shiftCnt)
{
    boolean bit0Value;
    uint8   shiftCntTmp = shiftCnt;

    while (shiftCntTmp > 0u)
    {
        bit0Value = Bfx_GetBit_u32u8_u8(*data, BFX_CONST_BIT32_MSB_BITPN);
        *data     = (uint32)(*data << BFX_CONST_BIT8_ONE);
        if (bit0Value)
        {
            Bfx_SetBit_u32u8(data, BFX_CONST_BIT32_LSB_BITPN);
        }
        shiftCntTmp--;
    }
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
void Bfx_CopyBit_u32u8u32u8(uint32* destData, uint8 destPn, uint32 srcData, uint8 srcPn)
{
    boolean bitSet;

    bitSet = Bfx_GetBit_u32u8_u8(srcData, srcPn);

    if (bitSet)
    {
        Bfx_SetBit_u32u8(destData, destPn);
    }
    else
    {
        Bfx_ClrBit_u32u8(destData, destPn);
    }
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
void Bfx_PutBits_u32u8u8u32(uint32* data, uint8 bitStartPn, uint8 bitLn, uint32 pattern)
{
    uint32 mask       = 0u;
    uint32 patternTmp = pattern;

    Bfx_SetBits_u32u8u8u8(&mask, bitStartPn, bitLn, BFX_CONST_BIT8_ONE);
    Bfx_ClrBitMask_u32u32(data, mask);
    Bfx_ShiftBitLt_u32u8(&patternTmp, bitStartPn);
    Bfx_SetBitMask_u32u32(data, patternTmp);
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
void Bfx_PutBitsMask_u32u32u32(uint32* data, uint32 pattern, uint32 mask)
{
    uint32 patternTmp = pattern;
    Bfx_ClrBitMask_u32u32(data, mask);
    patternTmp = (uint32)(patternTmp & mask);
    Bfx_SetBitMask_u32u32(data, patternTmp);
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
void Bfx_PutBit_u32u8u8(uint32* data, uint8 bitPn, boolean status)
{
    if (status)
    {
        Bfx_SetBit_u32u8(data, bitPn);
    }
    else
    {
        Bfx_ClrBit_u32u8(data, bitPn);
    }
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
uint8 Bfx_CountLeadingSigns_s32(sint32 data)
{
    uint8 count = 0u;
    uint8 msb   = (uint8)(data >> BFX_CONST_BIT32_MSB_BITPN) & 0x01u; /* PRQA S 4393,4533,2105,2985 */ /*VL_Bfx_Signed*/
    uint8 i     = 0u;

    for (i = BFX_CONST_BIT32_MSB_BITPN - 1u; i > 0u; i--)
    {
        if (((uint8)(data >> i) & 0x01u) == msb) /* PRQA S 4533,4393,2105,2985 */ /*VL_Bfx_Signed*/
        {
            count++;
        }
        else
        {
            break;
        }
    }
    if ((i == 0u) && (((uint8)(data) & 0x01u) == msb))
    {
        count++;
    }
    return count;
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
uint8 Bfx_CountLeadingOnes_u32(uint32 data)
{
    uint8 count = 0u;
    uint8 i     = 0u;
    for (i = BFX_CONST_BIT32_MSB_BITPN; i > 0u; i--)
    {
        if (((uint8)(data >> i) & 0x01u) == 1u)
        {
            count++;
        }
        else
        {
            break;
        }
    }
    if ((i == 0u) && (((uint8)(data) & 0x01u) == 1u))
    {
        count++;
    }
    return count;
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
uint8 Bfx_CountLeadingZeros_u32(uint32 data)
{
    uint8 count = 0u;
    uint8 i     = 0u;
    for (i = BFX_CONST_BIT32_MSB_BITPN; i > 0u; i--)
    {
        if (((uint8)(data >> i) & 0x01u) == 0u)
        {
            count++;
        }
        else
        {
            break;
        }
    }
    if ((i == 0u) && (((uint8)(data) & 0x01u) == 0u))
    {
        count++;
    }
    return count;
}
/*PRQA S 1503,1505,3408 -- */ /*VL_Bfx_macro*/
/*PRQA S 1512 -- */           /*VL_Bfx_ODR*/
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"

/* ========================================== internal function definitions ========================================= */
