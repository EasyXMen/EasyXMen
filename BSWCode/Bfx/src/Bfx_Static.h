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
/*
 ************************************************************************************************************************
 **
 **  @file               : Bfx_static.h
 **  @author             : ShenXu,Yb,QinmeiChen
 **  @date               : 2024/09/09
 **  @vendor             : isoft
 **  @description        : Crc Routlines
 **
 ***********************************************************************************************************************/
/* ================================================ misar justifications ============================================ */
#ifndef BFX_STATIC_H
#define BFX_STATIC_H
/* ===================================================== macros ===================================================== */
#define BFX_CONST_BIT8_ONE       (uint8)1
#define BFX_CONST_BIT8_TWO       (uint8)2
#define BFX_CONST_BIT8_BITLEN    (uint8)8
#define BFX_CONST_BIT8_LSB_BITPN (uint8)0
#define BFX_CONST_BIT8_MSB_BITPN (uint8)7

#define BFX_CONST_BIT16_ONE       (uint16)1
#define BFX_CONST_BIT16_BITLEN    (uint8)16
#define BFX_CONST_BIT16_LSB_BITPN (uint8)0
#define BFX_CONST_BIT16_MSB_BITPN (uint8)15

#define BFX_CONST_BIT32_ONE       1uL
#define BFX_CONST_BIT32_BITLEN    (uint8)32
#define BFX_CONST_BIT32_LSB_BITPN (uint8)0
#define BFX_CONST_BIT32_MSB_BITPN (uint8)31

#define BFX_CONST_BIT64_ONE       1uLL
#define BFX_CONST_BIT64_BITLEN    (uint8)64
#define BFX_CONST_BIT64_LSB_BITPN (uint8)0
#define BFX_CONST_BIT64_MSB_BITPN (uint8)63

/*PRQA S 3412,0342 ++ */ /*VL_Bfx_macro*/
#define BFX_DEF(typeMn)                                                                                                \
    void Bfx_SetBit_u##typeMn##u8(uint##typeMn* data, uint8 bitPn)                                                     \
    /*PRQA S 3412,0342 -- */                                                                                           \
    {                                                                                                                  \
        *data = *data | (uint##typeMn)(BFX_CONST_BIT##typeMn##_ONE << bitPn);                                          \
    }                                                                                                                  \
    void Bfx_ClrBit_u##typeMn##u8(uint##typeMn* data, uint8 bitPn)                                                     \
    {                                                                                                                  \
        *data = (uint##typeMn)(*data & (uint##typeMn)(~(uint##typeMn)(BFX_CONST_BIT##typeMn##_ONE << bitPn)));         \
    }                                                                                                                  \
    boolean Bfx_GetBit_u##typeMn##u8_u8(uint##typeMn data, uint8 bitPn)                                                \
    {                                                                                                                  \
        return (0u != (data & (uint##typeMn)(BFX_CONST_BIT##typeMn##_ONE << bitPn)));                                  \
    }                                                                                                                  \
    void Bfx_SetBits_u##typeMn##u8u8u8(uint##typeMn* data, uint8 bitStartPn, uint8 bitLn, uint8 status)                \
    {                                                                                                                  \
        uint8 bitLnTmp = bitLn;                                                                                        \
        if (0u == status)                                                                                              \
        {                                                                                                              \
            /*clear bits*/                                                                                             \
            while (bitLnTmp > 0u)                                                                                      \
            {                                                                                                          \
                bitLnTmp--;                                                                                            \
                *data = (uint##typeMn)(                                                                                \
                    *data & (uint##typeMn)(~(uint##typeMn)(BFX_CONST_BIT##typeMn##_ONE << (bitStartPn + bitLnTmp))));  \
            }                                                                                                          \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            /*set bits as 1*/                                                                                          \
            while (bitLnTmp > 0u)                                                                                      \
            {                                                                                                          \
                bitLnTmp--;                                                                                            \
                *data = *data | (uint##typeMn)(BFX_CONST_BIT##typeMn##_ONE << (bitStartPn + bitLnTmp));                \
            }                                                                                                          \
        }                                                                                                              \
    }                                                                                                                  \
    uint##typeMn Bfx_GetBits_u##typeMn##u8u8_u##typeMn(uint##typeMn data, uint8 bitStartPn, uint8 bitLn)               \
    {                                                                                                                  \
        uint##typeMn result;                                                                                           \
        uint##typeMn mask = 0u;                                                                                        \
                                                                                                                       \
        /*calculate the mask value*/                                                                                   \
        Bfx_SetBits_u##typeMn##u8u8u8(&mask, bitStartPn, bitLn, (uint8)BFX_CONST_BIT##typeMn##_ONE);                   \
        /*mask the data and shift*/                                                                                    \
        result = (uint##typeMn)((uint##typeMn)(data & mask) >> bitStartPn);                                            \
                                                                                                                       \
        return result;                                                                                                 \
    }                                                                                                                  \
    void Bfx_SetBitMask_u##typeMn##u##typeMn(uint##typeMn* data, uint##typeMn mask)                                    \
    {                                                                                                                  \
        *data = (uint##typeMn)(*data | mask);                                                                          \
    }                                                                                                                  \
    void Bfx_ClrBitMask_u##typeMn##u##typeMn(uint##typeMn* data, uint##typeMn mask)                                    \
    {                                                                                                                  \
        *data = (uint##typeMn)(*data & (uint##typeMn) ~mask);                                                          \
    }                                                                                                                  \
    boolean Bfx_TstBitMask_u##typeMn##u##typeMn##_u8(uint##typeMn data, uint##typeMn mask)                             \
    {                                                                                                                  \
        return ((uint##typeMn)(data & mask) == mask);                                                                  \
    }                                                                                                                  \
    boolean Bfx_TstBitLnMask_u##typeMn##u##typeMn##_u8(uint##typeMn data, uint##typeMn mask)                           \
    {                                                                                                                  \
        boolean result = FALSE;                                                                                        \
                                                                                                                       \
        if ((data & mask) != 0u)                                                                                       \
        {                                                                                                              \
            result = TRUE;                                                                                             \
        }                                                                                                              \
        return result;                                                                                                 \
    }                                                                                                                  \
    boolean Bfx_TstParityEven_u##typeMn##_u8(uint##typeMn data)                                                        \
    {                                                                                                                  \
        boolean result;                                                                                                \
        uint8   setCount = 0u;                                                                                         \
        uint8   bitPn    = 0u;                                                                                         \
                                                                                                                       \
        while (bitPn < BFX_CONST_BIT##typeMn##_BITLEN)                                                                 \
        {                                                                                                              \
            result = Bfx_GetBit_u##typeMn##u8_u8(data, bitPn);                                                         \
            if (result)                                                                                                \
            {                                                                                                          \
                setCount++;                                                                                            \
            }                                                                                                          \
            bitPn++;                                                                                                   \
        }                                                                                                              \
                                                                                                                       \
        if (0u == (setCount % BFX_CONST_BIT8_TWO))                                                                     \
        {                                                                                                              \
            result = TRUE;                                                                                             \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            result = FALSE;                                                                                            \
        }                                                                                                              \
                                                                                                                       \
        return result;                                                                                                 \
    }                                                                                                                  \
    void Bfx_ToggleBits_u##typeMn(uint##typeMn* data)                                                                  \
    {                                                                                                                  \
        *data = (uint##typeMn)(~(*data));                                                                              \
    }                                                                                                                  \
    void Bfx_ToggleBitMask_u##typeMn##u##typeMn(uint##typeMn* data, uint##typeMn mask)                                 \
    {                                                                                                                  \
        *data = (uint##typeMn)(*data ^ mask);                                                                          \
    }                                                                                                                  \
    void Bfx_ShiftBitRt_u##typeMn##u8(uint##typeMn* data, uint8 shiftCnt)                                              \
    {                                                                                                                  \
        *data = (uint##typeMn)(*data >> shiftCnt);                                                                     \
    }                                                                                                                  \
    void Bfx_ShiftBitLt_u##typeMn##u8(uint##typeMn* data, uint8 shiftCnt)                                              \
    {                                                                                                                  \
        *data = (uint##typeMn)(*data << shiftCnt);                                                                     \
    }                                                                                                                  \
    void Bfx_RotBitRt_u##typeMn##u8(uint##typeMn* data, uint8 shiftCnt)                                                \
    {                                                                                                                  \
        boolean bit0Value;                                                                                             \
        uint8   shiftCntTmp = shiftCnt;                                                                                \
                                                                                                                       \
        while (shiftCntTmp > 0u)                                                                                       \
        {                                                                                                              \
            bit0Value = Bfx_GetBit_u##typeMn##u8_u8(*data, BFX_CONST_BIT##typeMn##_LSB_BITPN);                         \
            *data     = (uint##typeMn)(*data >> BFX_CONST_BIT8_ONE);                                                   \
            if (bit0Value)                                                                                             \
            {                                                                                                          \
                Bfx_SetBit_u##typeMn##u8(data, BFX_CONST_BIT##typeMn##_MSB_BITPN);                                     \
            }                                                                                                          \
            shiftCntTmp--;                                                                                             \
        }                                                                                                              \
    }                                                                                                                  \
    void Bfx_RotBitLt_u##typeMn##u8(uint##typeMn* data, uint8 shiftCnt)                                                \
    {                                                                                                                  \
        boolean bit0Value;                                                                                             \
        uint8   shiftCntTmp = shiftCnt;                                                                                \
                                                                                                                       \
        while (shiftCntTmp > 0u)                                                                                       \
        {                                                                                                              \
            bit0Value = Bfx_GetBit_u##typeMn##u8_u8(*data, BFX_CONST_BIT##typeMn##_MSB_BITPN);                         \
            *data     = (uint##typeMn)(*data << BFX_CONST_BIT8_ONE);                                                   \
            if (bit0Value)                                                                                             \
            {                                                                                                          \
                Bfx_SetBit_u##typeMn##u8(data, BFX_CONST_BIT##typeMn##_LSB_BITPN);                                     \
            }                                                                                                          \
            shiftCntTmp--;                                                                                             \
        }                                                                                                              \
    }                                                                                                                  \
    void Bfx_CopyBit_u##typeMn##u8u##typeMn##u8(                                                                       \
        uint##typeMn* destData,                                                                                        \
        uint8         destPn,                                                                                          \
        uint##typeMn  srcData,                                                                                         \
        uint8         srcPn)                                                                                           \
    {                                                                                                                  \
        boolean bitSet;                                                                                                \
                                                                                                                       \
        bitSet = Bfx_GetBit_u##typeMn##u8_u8(srcData, srcPn);                                                          \
                                                                                                                       \
        if (bitSet)                                                                                                    \
        {                                                                                                              \
            Bfx_SetBit_u##typeMn##u8(destData, destPn);                                                                \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            Bfx_ClrBit_u##typeMn##u8(destData, destPn);                                                                \
        }                                                                                                              \
    }                                                                                                                  \
    void Bfx_PutBits_u##typeMn##u8u8u##typeMn(uint##typeMn* data, uint8 bitStartPn, uint8 bitLn, uint##typeMn pattern) \
    {                                                                                                                  \
        uint##typeMn mask       = 0u;                                                                                  \
        uint##typeMn patternTmp = pattern;                                                                             \
                                                                                                                       \
        Bfx_SetBits_u##typeMn##u8u8u8(&mask, bitStartPn, bitLn, BFX_CONST_BIT8_ONE);                                   \
        Bfx_ClrBitMask_u##typeMn##u##typeMn(data, mask);                                                               \
        Bfx_ShiftBitLt_u##typeMn##u8(&patternTmp, bitStartPn);                                                         \
        Bfx_SetBitMask_u##typeMn##u##typeMn(data, patternTmp);                                                         \
    }                                                                                                                  \
    void Bfx_PutBitsMask_u##typeMn##u##typeMn##u##typeMn(uint##typeMn* data, uint##typeMn pattern, uint##typeMn mask)  \
    {                                                                                                                  \
        uint##typeMn patternTmp = pattern;                                                                             \
        Bfx_ClrBitMask_u##typeMn##u##typeMn(data, mask);                                                               \
        patternTmp = (uint##typeMn)(patternTmp & mask);                                                                \
        Bfx_SetBitMask_u##typeMn##u##typeMn(data, patternTmp);                                                         \
    }                                                                                                                  \
    void Bfx_PutBit_u##typeMn##u8u8(uint##typeMn* data, uint8 bitPn, boolean status)                                   \
    {                                                                                                                  \
        if (status)                                                                                                    \
        {                                                                                                              \
            Bfx_SetBit_u##typeMn##u8(data, bitPn);                                                                     \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            Bfx_ClrBit_u##typeMn##u8(data, bitPn);                                                                     \
        }                                                                                                              \
    }                                                                                                                  \
    uint8 Bfx_CountLeadingSigns_s##typeMn(sint##typeMn data)                                                           \
    {                                                                                                                  \
        uint8 count = 0u;                                                                                              \
        uint8 msb   = (uint8)(data >> BFX_CONST_BIT##typeMn##_MSB_BITPN) & 0x01u;                                      \
        uint8 i     = 0u;                                                                                              \
                                                                                                                       \
        for (i = BFX_CONST_BIT##typeMn##_MSB_BITPN - 1u; i > 0u; i--)                                                  \
        {                                                                                                              \
            if (((uint8)(data >> i) & 0x01u) == msb)                                                                   \
            {                                                                                                          \
                count++;                                                                                               \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                break;                                                                                                 \
            }                                                                                                          \
        }                                                                                                              \
        if ((i == 0u) && (((uint8)(data) & 0x01u) == msb))                                                             \
        {                                                                                                              \
            count++;                                                                                                   \
        }                                                                                                              \
        return count;                                                                                                  \
    }                                                                                                                  \
    uint8 Bfx_CountLeadingOnes_u##typeMn(uint##typeMn data)                                                            \
    {                                                                                                                  \
        uint8 count = 0u;                                                                                              \
        uint8 i     = 0u;                                                                                              \
        for (i = BFX_CONST_BIT##typeMn##_MSB_BITPN; i > 0u; i--)                                                       \
        {                                                                                                              \
            if (((uint8)(data >> i) & 0x01u) == 1u)                                                                    \
            {                                                                                                          \
                count++;                                                                                               \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                break;                                                                                                 \
            }                                                                                                          \
        }                                                                                                              \
        if ((i == 0u) && (((uint8)(data) & 0x01u) == 1u))                                                              \
        {                                                                                                              \
            count++;                                                                                                   \
        }                                                                                                              \
        return count;                                                                                                  \
    }                                                                                                                  \
    uint8 Bfx_CountLeadingZeros_u##typeMn(uint##typeMn data)                                                           \
    {                                                                                                                  \
        uint8 count = 0u;                                                                                              \
        uint8 i     = 0u;                                                                                              \
        for (i = BFX_CONST_BIT##typeMn##_MSB_BITPN; i > 0u; i--)                                                       \
        {                                                                                                              \
            if (((uint8)(data >> i) & 0x01u) == 0u)                                                                    \
            {                                                                                                          \
                count++;                                                                                               \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                break;                                                                                                 \
            }                                                                                                          \
        }                                                                                                              \
        if ((i == 0u) && (((uint8)(data) & 0x01u) == 0u))                                                              \
        {                                                                                                              \
            count++;                                                                                                   \
        }                                                                                                              \
        return count;                                                                                                  \
    }
/* ================================================ type definitions ================================================ */
/* ========================================== internal function definitions ========================================= */
/* =========================================== external data declarations =========================================== */
/* ========================================= external function declarations ========================================= */
#endif
