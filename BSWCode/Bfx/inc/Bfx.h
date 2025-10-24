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
 **  @file               : Bfx.h
 **  @author             : stanleyluo,QinmeiChen
 **  @date               : 2024/09/09
 **  @vendor             : isoft
 **  @description        : Public function prototypes and types defined by the BFX library specification
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 * V01.00.00 2024-04-16  qinmei.chen  fix CPT-8741 the definition of some variables is wrong
 * V02.00.00 2024-09-11  qinmei.chen  Refactor and update to R23-11
 *
===================================================================================================================== */
/* ================================================ misar justifications ============================================ */
/**
 \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
   ModeName:Crc<br>
 RuleSorce:puhua_rules-2024.12.rcf

  \li VL_Bfx_Signed
   Reason: The specification requires such a design
   Risk: None
   Prevention: Functional reliability guaranteed by design.

  \li VL_Bfx_uint32
   Reason: For the sake of convenience, we adopt this writing method,
   so we cannot consider variables above uint32 and variables below uint32
   Risk: None
   Prevention: Functional reliability guaranteed by design.

   \li VL_Bfx_macro
   Reason: the implement is almost the same depending on different bit input
   Risk: None
   Prevention: Functional reliability guaranteed by design.

*/
#ifndef BFX_H
#define BFX_H
/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "Bfx_Static.h"
#ifdef __cplusplus
extern "C" {
#endif
/* =============================================== version information ============================================== */
/*Published information elements*/
#define BFX_H_VENDOR_ID                   (uint16)62
#define BFX_H_MODULE_ID                   (uint16)205
#define BFX_H_AR_RELEASE_MAJOR_VERSION    (uint8)4
#define BFX_H_AR_RELEASE_MINOR_VERSION    (uint8)2
#define BFX_H_AR_RELEASE_REVISION_VERSION (uint8)2

#define BFX_H_SW_MAJOR_VERSION (uint8)1
#define BFX_H_SW_MINOR_VERSION (uint8)0
#define BFX_H_SW_PATCH_VERSION (uint8)0

/* ===================================================== macros ===================================================== */
/* ================================================ type definitions ================================================ */
/* ========================================== internal function definitions ========================================= */
/* =========================================== external data declarations =========================================== */
/* ========================================= external function declarations ========================================= */
/*PRQA S 1501,1753 ++ */ /*VL_Bfx_macro*/
/**
 * @brief         set logical status of input data as '1' at the requested bit position.
 * @param[in]     bitPn : Bit position
 * @param[inout]  data  : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58519
 */
void Bfx_SetBit_u8u8(uint8* data, uint8 bitPn);

/**
 * @brief         clear the logical status of the input data to '0' at the requested bit position.
 * @param[in]     bitPn : Bit position
 * @param[inout]  data  : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58518
 */
void Bfx_ClrBit_u8u8(uint8* data, uint8 bitPn);

/**
 * @brief         return the logical status of the input data for the requested bit position.
 * @param[in]     data  : input data
 * @param[in]     bitPn : Bit position
 * @return        boolean
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58517
 */
boolean Bfx_GetBit_u8u8_u8(uint8 data, uint8 bitPn);

/**
 * @brief         set the input data as '1' or '0' as per 'Status' value starting from
 *                'BitStartPn' for the length 'BitLn'.
 * @param[in]     bitStartPn : Start bit position
 * @param[in]     bitLn      : Bit field length
 * @param[in]     status     : Status value
 * @param[inout]  data       : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58516
 */
void Bfx_SetBits_u8u8u8u8(uint8* data, uint8 bitStartPn, uint8 bitLn, uint8 status);

/**
 * @brief         return the Bits of the input data starting from 'BitStartPn' for the length
 *                of 'BitLn'.
 * @param[in]     data       : input data
 * @param[in]     bitStartPn : Start bit position
 * @param[in]     bitLn      : Bit field length
 * @return        uint8
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58515
 */
uint8 Bfx_GetBits_u8u8u8_u8(uint8 data, uint8 bitStartPn, uint8 bitLn);

/**
 * @brief         set the data to logical status '1' as per the corresponding Mask bits when
 *                set to value 1 and remaining bits will retain their original values.
 * @param[in]     mask : Mask used to set bits
 * @param[inout]  data : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58514
 */
void Bfx_SetBitMask_u8u8(uint8* data, uint8 mask);

/**
 * @brief         clear the logical status to '0' for the input data for all the bit positions
 *                as per the mask.
 * @param[in]     mask : mask value
 * @param[inout]  data : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58539
 */
void Bfx_ClrBitMask_u8u8(uint8* data, uint8 mask);

/**
 * @brief         return TRUE, if all bits defined in Mask value are set in the input Data
 *                value. In all other cases this function shall return FALSE.
 * @param[in]     data : input data
 * @param[in]     mask : mask value
 * @return        boolean
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58538
 */
boolean Bfx_TstBitMask_u8u8_u8(uint8 data, uint8 mask);

/**
 * @brief         makes a test on the input data and if at least one bit is set as per the
 *                mask, then the function shall return TRUE, otherwise it shall return FALSE.
 * @param[in]     data, input data
 * @param[in]     mask, mask value
 * @return        boolean
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58537
 */
boolean Bfx_TstBitLnMask_u8u8_u8(uint8 data, uint8 mask);

/**
 * @brief         tests the number of bits set to 1. If this number is even, it shall return
 *                TRUE, otherwise it returns FALSE.
 * @param[in]     data : input data
 * @return        boolean
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58536
 */
boolean Bfx_TstParityEven_u8_u8(uint8 data);

/**
 * @brief         toggles all the bits of data (1's Complement Data).
 * @param[inout]  data :  Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58535
 */
void Bfx_ToggleBits_u8(uint8* data);

/**
 * @brief         toggles the bits of data when the corresponding bit of the mask is enabled
 *                and set to 1.
 * @param[in]     mask : mask value
 * @param[inout]  data : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58534
 */
void Bfx_ToggleBitMask_u8u8(uint8* data, uint8 mask);

/**
 * @brief         shift data to the right by ShiftCnt. The most significant bit (left-most bit)
 *                is replaced by a '0' bit and the least significant bit (right-most bit) is
 *                discarded for every single bit shift cycle.
 * @param[in]     shiftCnt : Shift right count
 * @param[inout]  data     : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58533
 */
void Bfx_ShiftBitRt_u8u8(uint8* data, uint8 shiftCnt);

/**
 * @brief         shift data to the left by ShiftCnt. The least significant bit (right-most bit)
 *                is replaced by a '0' bit and the most significant bit (left-most bit) is
 *                discarded for every single bit shift cycle.
 * @param[in]     shiftCnt : Shift right count
 * @param[inout]  data     : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58532
 */
void Bfx_ShiftBitLt_u8u8(uint8* data, uint8 shiftCnt);

/**
 * @brief         rotate data to the right by ShiftCnt. The least significant bit is rotated
 *                to the most significant bit location for every single bit shift cycle.
 * @param[in]     shiftCnt : Shift count
 * @param[inout]  data     : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58531
 */
void Bfx_RotBitRt_u8u8(uint8* data, uint8 shiftCnt);

/**
 * @brief         rotate data to the left by ShiftCnt. The most significant bit is rotated to
 *                the least significant bit location for every single bit shift cycle.
 * @param[in]     shiftCnt : Shift count
 * @param[inout]  data     : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58530
 */
void Bfx_RotBitLt_u8u8(uint8* data, uint8 shiftCnt);

/**
 * @brief         copy a bit from source data from bit position to destination data at bit
 *                position.
 * @param[in]     destPn   : Destination position
 * @param[in]     srcData  : Source data
 * @param[in]     srcPn    : Source position
 * @param[inout]  destData : Pointer to destination data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58529
 */
void Bfx_CopyBit_u8u8u8u8(uint8* destData, uint8 destPn, uint8 srcData, uint8 srcPn);

/**
 * @brief         put bits as mentioned in Pattern to the input Data from the specified bit
 *                position.
 * @param[in]     bitStartPn : Start bit position
 * @param[in]     bitLn      : Bit field length
 * @param[in]     pattern    : Pattern to be set
 * @param[inout]  data       : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58528
 */
void Bfx_PutBits_u8u8u8u8(uint8* data, uint8 bitStartPn, uint8 bitLn, uint8 pattern);

/**
 * @brief         put all bits defined in Pattern and for which the corresponding Mask bit is
 *                set to 1 in the input Data.
 * @param[in]     pattern : Pattern to be set
 * @param[in]     mask    : mask value
 * @param[inout]  data    : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58527
 */
void Bfx_PutBitsMask_u8u8u8(uint8* data, uint8 pattern, uint8 mask);

/**
 * @brief         update the bit specified by BitPn of input data as '1' or '0' as per
 *                'Status' value.
 * @param[in]     bitPn  : Bit position
 * @param[in]     status : status value
 * @param[inout]  data   : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58526
 */
void Bfx_PutBit_u8u8u8(uint8* data, uint8 bitPn, boolean status);

/**
 * @brief         Arithmetic shift with saturation.
 * @param[in]     ShiftCnt : Shift count
 * @param[inout]  data     : input data
 * @return        sint8
 * @reentrant     FALSE
 * @synchronous   FALSE
 * @trace         CPD-58525
 */
sint8 Bfx_ShiftBitSat_s8s8_s8(sint8 ShiftCnt, uint8* data);

/**
 * @brief         Arithmetic shift with saturation.
 * @param[in]     ShiftCnt : Shift count
 * @param[inout]  data     : Pointer to input data
 * @return        uint8
 * @reentrant     FALSE
 * @synchronous   FALSE
 * @trace         CPD-58525
 */
uint8 Bfx_ShiftBitSat_u8s8_u8(sint8 ShiftCnt, uint8* data);

/**
 * @brief         Count the number of consecutive bits which have
 *                the same value as most significant bit in Data,starting with bit
 *                at position msb minus one. Put the result in Data. It is the number of leading
 *                sign bits minus one, giving the number of redundant sign bits in Data.
 * @param[in]     data : input data
 * @return        uint8
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58523
 */
uint8 Bfx_CountLeadingSigns_s8(sint8 data);

/**
 * @brief         Count the number of consecutive ones in Data starting with
 *                the most significant bit and return the result.
 * @param[in]     data : input data
 * @return        uint8
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58524
 */
uint8 Bfx_CountLeadingOnes_u8(uint8 data);

/**
 * @brief         Count the number of consecutive zeros in Data starting with
 *                the most significant bit and return the result.
 * @param[in]     data : input data
 * @return        uint8
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58522
 */
uint8 Bfx_CountLeadingZeros_u8(uint8 data);

/**
 * @brief         set logical status of input data as '1' at the requested bit position.
 * @param[in]     bitPn : Bit position
 * @param[inout]  data  : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58519
 */
void Bfx_SetBit_u16u8(uint16* data, uint8 bitPn);

/**
 * @brief         clear the logical status of the input data to '0' at the requested bit position.
 * @param[in]     bitPn : Bit position
 * @param[inout]  data  : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58518
 */
void Bfx_ClrBit_u16u8(uint16* data, uint8 bitPn);

/**
 * @brief         return the logical status of the input data for the requested bit position.
 * @param[in]     data  : input data
 * @param[in]     bitPn : Bit position
 * @return        boolean
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58517
 */
boolean Bfx_GetBit_u16u8_u8(uint16 data, uint8 bitPn);

/**
 * @brief         set the input data as '1' or '0' as per 'Status' value starting from
 *                'BitStartPn' for the length 'BitLn'.
 * @param[in]     bitStartPn : Start bit position
 * @param[in]     bitLn      : Bit field length
 * @param[in]     status     : Status value
 * @param[inout]  data       : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58516
 */
void Bfx_SetBits_u16u8u8u8(uint16* data, uint8 bitStartPn, uint8 bitLn, uint8 status);

/**
 * @brief         return the Bits of the input data starting from 'BitStartPn' for the length
 *                of 'BitLn'.
 * @param[in]     data       : input data
 * @param[in]     bitStartPn : Start bit position
 * @param[in]     bitLn      : Bit field length
 * @return        uint16
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58515
 */
uint16 Bfx_GetBits_u16u8u8_u16(uint16 data, uint8 bitStartPn, uint8 bitLn);

/**
 * @brief         set the data to logical status '1' as per the corresponding Mask bits when
 *                set to value 1 and remaining bits will retain their original values.
 * @param[in]     mask : Mask used to set bits
 * @param[inout]  data : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58514
 */
void Bfx_SetBitMask_u16u16(uint16* data, uint16 mask);

/**
 * @brief         clear the logical status to '0' for the input data for all the bit positions
 *                as per the mask.
 * @param[in]     mask : mask value
 * @param[inout]  data : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58539
 */
void Bfx_ClrBitMask_u16u16(uint16* data, uint16 mask);

/**
 * @brief         return TRUE, if all bits defined in Mask value are set in the input Data
 *                value. In all other cases this function shall return FALSE.
 * @param[in]     data : input data
 * @param[in]     mask : mask value
 * @return        boolean
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58538
 */
boolean Bfx_TstBitMask_u16u16_u8(uint16 data, uint16 mask);

/**
 * @brief         makes a test on the input data and if at least one bit is set as per the
 *                mask, then the function shall return TRUE, otherwise it shall return FALSE.
 * @param[in]     data, input data
 * @param[in]     mask, mask value
 * @return        boolean
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58537
 */
boolean Bfx_TstBitLnMask_u16u16_u8(uint16 data, uint16 mask);

/**
 * @brief         tests the number of bits set to 1. If this number is even, it shall return
 *                TRUE, otherwise it returns FALSE.
 * @param[in]     data : input data
 * @return        boolean
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58536
 */
boolean Bfx_TstParityEven_u16_u8(uint16 data);

/**
 * @brief         toggles all the bits of data (1's Complement Data).
 * @param[inout]  data : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58535
 */
void Bfx_ToggleBits_u16(uint16* data);

/**
 * @brief         toggles the bits of data when the corresponding bit of the mask is enabled
 *                and set to 1.
 * @param[in]     mask : mask value
 * @param[inout]  data : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58534
 */
void Bfx_ToggleBitMask_u16u16(uint16* data, uint16 mask);

/**
 * @brief         shift data to the right by ShiftCnt. The most significant bit (left-most bit)
 *                is replaced by a '0' bit and the least significant bit (right-most bit) is
 *                discarded for every single bit shift cycle.
 * @param[in]     shiftCnt : Shift right count
 * @param[inout]  data     : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58533
 */
void Bfx_ShiftBitRt_u16u8(uint16* data, uint8 shiftCnt);

/**
 * @brief         shift data to the left by ShiftCnt. The least significant bit (right-most bit)
 *                is replaced by a '0' bit and the most significant bit (left-most bit) is
 *                discarded for every single bit shift cycle.
 * @param[in]     shiftCnt : Shift right count
 * @param[inout]  data     : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58532
 */
void Bfx_ShiftBitLt_u16u8(uint16* data, uint8 shiftCnt);

/**
 * @brief         rotate data to the right by ShiftCnt. The least significant bit is rotated
 *                to the most significant bit location for every single bit shift cycle.
 * @param[in]     shiftCnt : Shift count
 * @param[inout]  data     : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58531
 */
void Bfx_RotBitRt_u16u8(uint16* data, uint8 shiftCnt);

/**
 * @brief         rotate data to the left by ShiftCnt. The most significant bit is rotated to
 *                the least significant bit location for every single bit shift cycle.
 * @param[in]     shiftCnt : Shift count
 * @param[inout]  data     : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58530
 */
void Bfx_RotBitLt_u16u8(uint16* data, uint8 shiftCnt);

/**
 * @brief         copy a bit from source data from bit position to destination data at bit
 *                position.
 * @param[in]     destPn   : Destination position
 * @param[in]     srcData  : Source data
 * @param[in]     srcPn    : Source position
 * @param[inout]  destData : Pointer to destination data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58529
 */
void Bfx_CopyBit_u16u8u16u8(uint16* destData, uint8 destPn, uint16 srcData, uint8 srcPn);

/**
 * @brief         put bits as mentioned in Pattern to the input Data from the specified bit
 *                position.
 * @param[in]     bitStartPn : Start bit position
 * @param[in]     bitLn      : Bit field length
 * @param[in]     pattern    : Pattern to be set
 * @param[inout]  data       : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58528
 */
void Bfx_PutBits_u16u8u8u16(uint16* data, uint8 bitStartPn, uint8 bitLn, uint16 pattern);

/**
 * @brief         put all bits defined in Pattern and for which the corresponding Mask bit is
 *                set to 1 in the input Data.
 * @param[in]     pattern : Pattern to be set
 * @param[in]     mask    : mask value
 * @param[inout]  data    : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58527
 */
void Bfx_PutBitsMask_u16u16u16(uint16* data, uint16 pattern, uint16 mask);

/**
 * @brief         update the bit specified by BitPn of input data as '1' or '0' as per
 *                'Status' value.
 * @param[in]     bitPn  : Bit position
 * @param[in]     status : status value
 * @param[inout]  data   : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58526
 */
void Bfx_PutBit_u16u8u8(uint16* data, uint8 bitPn, boolean status);

/**
 * @brief         Arithmetic shift with saturation.
 * @param[in]     ShiftCnt : Shift count
 * @param[inout]  data     : input data
 * @return        sint16
 * @reentrant     FALSE
 * @synchronous   FALSE
 * @trace         CPD-58525
 */
sint16 Bfx_ShiftBitSat_s16s8_s16(sint8 ShiftCnt, uint16* data);

/**
 * @brief         Arithmetic shift with saturation.
 * @param[in]     ShiftCnt : Shift count
 * @param[inout]  data     : Pointer to input data
 * @return        uint16
 * @reentrant     FALSE
 * @synchronous   FALSE
 * @trace         CPD-58525
 */
uint16 Bfx_ShiftBitSat_u16s8_u16(sint8 ShiftCnt, uint16* data);

/**
 * @brief         Count the number of consecutive bits which have
 *                the same value as most significant bit in Data,starting with bit
 *                at position msb minus one. Put the result in Data. It is the number of leading
 *                sign bits minus one, giving the number of redundant sign bits in Data.
 * @param[in]     data : input data
 * @return        uint8
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58523
 */
uint8 Bfx_CountLeadingSigns_s16(sint16 data);

/**
 * @brief         Count the number of consecutive ones in Data starting with
 *                the most significant bit and return the result.
 * @param[in]     data : input data
 * @return        uint8
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58524
 */
uint8 Bfx_CountLeadingOnes_u16(uint16 data);

/**
 * @brief         Count the number of consecutive zeros in Data starting with
 *                the most significant bit and return the result.
 * @param[in]     data : input data
 * @return        uint8
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58522
 */
uint8 Bfx_CountLeadingZeros_u16(uint16 data);

/**
 * @brief         set logical status of input data as '1' at the requested bit position.
 * @param[in]     bitPn : Bit position
 * @param[inout]  data  : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58519
 */
void Bfx_SetBit_u32u8(uint32* data, uint8 bitPn);

/**
 * @brief         clear the logical status of the input data to '0' at the requested bit position.
 * @param[in]     bitPn : Bit position
 * @param[inout]  data  : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58518
 */
void Bfx_ClrBit_u32u8(uint32* data, uint8 bitPn);

/**
 * @brief         return the logical status of the input data for the requested bit position.
 * @param[in]     data  : input data
 * @param[in]     bitPn : Bit position
 * @return        boolean
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58517
 */
boolean Bfx_GetBit_u32u8_u8(uint32 data, uint8 bitPn);

/**
 * @brief         set the input data as '1' or '0' as per 'Status' value starting from
 *                'BitStartPn' for the length 'BitLn'.
 * @param[in]     bitStartPn : Start bit position
 * @param[in]     bitLn      : Bit field length
 * @param[in]     status     : Status value
 * @param[inout]  data       : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58516
 */
void Bfx_SetBits_u32u8u8u8(uint32* data, uint8 bitStartPn, uint8 bitLn, uint8 status);

/**
 * @brief         return the Bits of the input data starting from 'BitStartPn' for the length
 *                of 'BitLn'.
 * @param[in]     data       : input data
 * @param[in]     bitStartPn : Start bit position
 * @param[in]     bitLn      : Bit field length
 * @return        uint32
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58515
 */
uint32 Bfx_GetBits_u32u8u8_u32(uint32 data, uint8 bitStartPn, uint8 bitLn);

/**
 * @brief         set the data to logical status '1' as per the corresponding Mask bits when
 *                set to value 1 and remaining bits will retain their original values.
 * @param[in]     mask : Mask used to set bits
 * @param[inout]  data : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58514
 */
void Bfx_SetBitMask_u32u32(uint32* data, uint32 mask);

/**
 * @brief         clear the logical status to '0' for the input data for all the bit positions
 *                as per the mask.
 * @param[in]     mask : mask value
 * @param[inout]  data : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58539
 */
void Bfx_ClrBitMask_u32u32(uint32* data, uint32 mask);

/**
 * @brief         return TRUE, if all bits defined in Mask value are set in the input Data
 *                value. In all other cases this function shall return FALSE.
 * @param[in]     data : input data
 * @param[in]     mask : mask value
 * @return        boolean
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58538
 */
boolean Bfx_TstBitMask_u32u32_u8(uint32 data, uint32 mask);

/**
 * @brief         makes a test on the input data and if at least one bit is set as per the
 *                mask, then the function shall return TRUE, otherwise it shall return FALSE.
 * @param[in]     data, input data
 * @param[in]     mask, mask value
 * @return        boolean
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58537
 */
boolean Bfx_TstBitLnMask_u32u32_u8(uint32 data, uint32 mask);

/**
 * @brief         tests the number of bits set to 1. If this number is even, it shall return
 *                TRUE, otherwise it returns FALSE.
 * @param[in]     data : input data
 * @return        boolean
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58536
 */
boolean Bfx_TstParityEven_u32_u8(uint32 data);

/**
 * @brief         toggles all the bits of data (1's Complement Data).
 * @param[inout]  data : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58535
 */
void Bfx_ToggleBits_u32(uint32* data);

/**
 * @brief         toggles the bits of data when the corresponding bit of the mask is enabled
 *                and set to 1.
 * @param[in]     mask : mask value
 * @param[inout]  data : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58534
 */
void Bfx_ToggleBitMask_u32u32(uint32* data, uint32 mask);

/**
 * @brief         shift data to the right by ShiftCnt. The most significant bit (left-most bit)
 *                is replaced by a '0' bit and the least significant bit (right-most bit) is
 *                discarded for every single bit shift cycle.
 * @param[in]     shiftCnt : Shift right count
 * @param[inout]  data     : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58533
 */
void Bfx_ShiftBitRt_u32u8(uint32* data, uint8 shiftCnt);

/**
 * @brief         shift data to the left by ShiftCnt. The least significant bit (right-most bit)
 *                is replaced by a '0' bit and the most significant bit (left-most bit) is
 *                discarded for every single bit shift cycle.
 * @param[in]     shiftCnt : Shift right count
 * @param[inout]  data     : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58532
 */
void Bfx_ShiftBitLt_u32u8(uint32* data, uint8 shiftCnt);

/**
 * @brief         rotate data to the right by ShiftCnt. The least significant bit is rotated
 *                to the most significant bit location for every single bit shift cycle.
 * @param[in]     shiftCnt : Shift count
 * @param[inout]  data     : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58531
 */
void Bfx_RotBitRt_u32u8(uint32* data, uint8 shiftCnt);

/**
 * @brief         rotate data to the left by ShiftCnt. The most significant bit is rotated to
 *                the least significant bit location for every single bit shift cycle.
 * @param[in]     shiftCnt : Shift count
 * @param[inout]  data     : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58530
 */
void Bfx_RotBitLt_u32u8(uint32* data, uint8 shiftCnt);

/**
 * @brief         copy a bit from source data from bit position to destination data at bit
 *                position.
 * @param[in]     destPn   : Destination position
 * @param[in]     srcData  : Source data
 * @param[in]     srcPn    : Source position
 * @param[inout]  destData : Pointer to destination data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58529
 */
void Bfx_CopyBit_u32u8u32u8(uint32* destData, uint8 destPn, uint32 srcData, uint8 srcPn);

/**
 * @brief         put bits as mentioned in Pattern to the input Data from the specified bit
 *                position.
 * @param[in]     bitStartPn : Start bit position
 * @param[in]     bitLn      : Bit field length
 * @param[in]     pattern    : Pattern to be set
 * @param[inout]  data       : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58528
 */
void Bfx_PutBits_u32u8u8u32(uint32* data, uint8 bitStartPn, uint8 bitLn, uint32 pattern);

/**
 * @brief         put all bits defined in Pattern and for which the corresponding Mask bit is
 *                set to 1 in the input Data.
 * @param[in]     pattern : Pattern to be set
 * @param[in]     mask    : mask value
 * @param[inout]  data    : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58527
 */
void Bfx_PutBitsMask_u32u32u32(uint32* data, uint32 pattern, uint32 mask);

/**
 * @brief         update the bit specified by BitPn of input data as '1' or '0' as per
 *                'Status' value.
 * @param[in]     bitPn  : Bit position
 * @param[in]     status : status value
 * @param[inout]  data   : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58526
 */
void Bfx_PutBit_u32u8u8(uint32* data, uint8 bitPn, boolean status);

/**
 * @brief         Arithmetic shift with saturation.
 * @param[in]     ShiftCnt : Shift count
 * @param[inout]  data     : input data
 * @return        sint32
 * @reentrant     FALSE
 * @synchronous   FALSE
 * @trace         CPD-58525
 */
sint32 Bfx_ShiftBitSat_s32s8_s32(sint8 ShiftCnt, uint32* data);

/**
 * @brief         Arithmetic shift with saturation.
 * @param[in]     ShiftCnt : Shift count
 * @param[inout]  data     : Pointer to input data
 * @return        uint32
 * @reentrant     FALSE
 * @synchronous   FALSE
 * @trace         CPD-58525
 */
uint32 Bfx_ShiftBitSat_u32s8_u32(sint8 ShiftCnt, uint32* data);

/**
 * @brief         Count the number of consecutive bits which have
 *                the same value as most significant bit in Data,starting with bit
 *                at position msb minus one. Put the result in Data. It is the number of leading
 *                sign bits minus one, giving the number of redundant sign bits in Data.
 * @param[in]     data : input data
 * @return        uint8
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58523
 */
uint8 Bfx_CountLeadingSigns_s32(sint32 data);

/**
 * @brief         Count the number of consecutive ones in Data starting with
 *                the most significant bit and return the result.
 * @param[in]     data : input data
 * @return        uint8
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58524
 */
uint8 Bfx_CountLeadingOnes_u32(uint32 data);

/**
 * @brief         Count the number of consecutive zeros in Data starting with
 *                the most significant bit and return the result.
 * @param[in]     data : input data
 * @return        uint8
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58522
 */
uint8 Bfx_CountLeadingZeros_u32(uint32 data);

/**
 * @brief         set logical status of input data as '1' at the requested bit position.
 * @param[in]     bitPn : Bit position
 * @param[inout]  data  : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58519
 */
void Bfx_SetBit_u64u8(uint64* data, uint8 bitPn);

/**
 * @brief         clear the logical status of the input data to '0' at the requested bit position.
 * @param[in]     bitPn : Bit position
 * @param[inout]  data  : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58518
 */
void Bfx_ClrBit_u64u8(uint64* data, uint8 bitPn);

/**
 * @brief         return the logical status of the input data for the requested bit position.
 * @param[in]     data  : input data
 * @param[in]     bitPn : Bit position
 * @return        boolean
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58517
 */
boolean Bfx_GetBit_u64u8_u8(uint64 data, uint8 bitPn);

/**
 * @brief         set the input data as '1' or '0' as per 'Status' value starting from
 *                'BitStartPn' for the length 'BitLn'.
 * @param[in]     bitStartPn : Start bit position
 * @param[in]     bitLn      : Bit field length
 * @param[in]     status     : Status value
 * @param[inout]  data       : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58516
 */
void Bfx_SetBits_u64u8u8u8(uint64* data, uint8 bitStartPn, uint8 bitLn, uint8 status);

/**
 * @brief         return the Bits of the input data starting from 'BitStartPn' for the length
 *                of 'BitLn'.
 * @param[in]     data       : input data
 * @param[in]     bitStartPn : Start bit position
 * @param[in]     bitLn      : Bit field length
 * @return        uint64
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58515
 */
uint64 Bfx_GetBits_u64u8u8_u64(uint64 data, uint8 bitStartPn, uint8 bitLn);

/**
 * @brief         set the data to logical status '1' as per the corresponding Mask bits when
 *                set to value 1 and remaining bits will retain their original values.
 * @param[in]     mask : Mask used to set bits
 * @param[inout]  data : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58514
 */
void Bfx_SetBitMask_u64u64(uint64* data, uint64 mask);

/**
 * @brief         clear the logical status to '0' for the input data for all the bit positions
 *                as per the mask.
 * @param[in]     mask : mask value
 * @param[inout]  data : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58539
 */
void Bfx_ClrBitMask_u64u64(uint64* data, uint64 mask);

/**
 * @brief         return TRUE, if all bits defined in Mask value are set in the input Data
 *                value. In all other cases this function shall return FALSE.
 * @param[in]     data : input data
 * @param[in]     mask : mask value
 * @return        boolean
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58538
 */
boolean Bfx_TstBitMask_u64u64_u8(uint64 data, uint64 mask);

/**
 * @brief         makes a test on the input data and if at least one bit is set as per the
 *                mask, then the function shall return TRUE, otherwise it shall return FALSE.
 * @param[in]     data, input data
 * @param[in]     mask, mask value
 * @return        boolean
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58537
 */
boolean Bfx_TstBitLnMask_u64u64_u8(uint64 data, uint64 mask);

/**
 * @brief         tests the number of bits set to 1. If this number is even, it shall return
 *                TRUE, otherwise it returns FALSE.
 * @param[in]     data : input data
 * @return        boolean
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58536
 */
boolean Bfx_TstParityEven_u64_u8(uint64 data);

/**
 * @brief         toggles all the bits of data (1's Complement Data).
 * @param[inout]  data : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58535
 */
void Bfx_ToggleBits_u64(uint64* data);

/**
 * @brief         toggles the bits of data when the corresponding bit of the mask is enabled
 *                and set to 1.
 * @param[in]     mask : mask value
 * @param[inout]  data : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58534
 */
void Bfx_ToggleBitMask_u64u64(uint64* data, uint64 mask);

/**
 * @brief         shift data to the right by ShiftCnt. The most significant bit (left-most bit)
 *                is replaced by a '0' bit and the least significant bit (right-most bit) is
 *                discarded for every single bit shift cycle.
 * @param[in]     shiftCnt : Shift right count
 * @param[inout]  data     : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58533
 */
void Bfx_ShiftBitRt_u64u8(uint64* data, uint8 shiftCnt);

/**
 * @brief         shift data to the left by ShiftCnt. The least significant bit (right-most bit)
 *                is replaced by a '0' bit and the most significant bit (left-most bit) is
 *                discarded for every single bit shift cycle.
 * @param[in]     shiftCnt : Shift right count
 * @param[inout]  data     : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58532
 */
void Bfx_ShiftBitLt_u64u8(uint64* data, uint8 shiftCnt);

/**
 * @brief         rotate data to the right by ShiftCnt. The least significant bit is rotated
 *                to the most significant bit location for every single bit shift cycle.
 * @param[in]     shiftCnt : Shift count
 * @param[inout]  data     : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58531
 */
void Bfx_RotBitRt_u64u8(uint64* data, uint8 shiftCnt);

/**
 * @brief         rotate data to the left by ShiftCnt. The most significant bit is rotated to
 *                the least significant bit location for every single bit shift cycle.
 * @param[in]     shiftCnt : Shift count
 * @param[inout]  data     : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58530
 */
void Bfx_RotBitLt_u64u8(uint64* data, uint8 shiftCnt);

/**
 * @brief         copy a bit from source data from bit position to destination data at bit
 *                position.
 * @param[in]     destPn   : Destination position
 * @param[in]     srcData  : Source data
 * @param[in]     srcPn    : Source position
 * @param[inout]  destData : Pointer to destination data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58529
 */
void Bfx_CopyBit_u64u8u64u8(uint64* destData, uint8 destPn, uint64 srcData, uint8 srcPn);

/**
 * @brief         put bits as mentioned in Pattern to the input Data from the specified bit
 *                position.
 * @param[in]     bitStartPn : Start bit position
 * @param[in]     bitLn      : Bit field length
 * @param[in]     pattern    : Pattern to be set
 * @param[inout]  data       : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58528
 */
void Bfx_PutBits_u64u8u8u64(uint64* data, uint8 bitStartPn, uint8 bitLn, uint64 pattern);

/**
 * @brief         put all bits defined in Pattern and for which the corresponding Mask bit is
 *                set to 1 in the input Data.
 * @param[in]     pattern : Pattern to be set
 * @param[in]     mask    : mask value
 * @param[inout]  data    : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58527
 */
void Bfx_PutBitsMask_u64u64u64(uint64* data, uint64 pattern, uint64 mask);

/**
 * @brief         update the bit specified by BitPn of input data as '1' or '0' as per
 *                'Status' value.
 * @param[in]     bitPn  : Bit position
 * @param[in]     status : status value
 * @param[inout]  data   : Pointer to input data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58526
 */
void Bfx_PutBit_u64u8u8(uint64* data, uint8 bitPn, boolean status);

/**
 * @brief         Arithmetic shift with saturation.
 * @param[in]     ShiftCnt : Shift count
 * @param[inout]  data     : input data
 * @return        sint64
 * @reentrant     FALSE
 * @synchronous   FALSE
 * @trace         CPD-58525
 */
sint64 Bfx_ShiftBitSat_s64s8_s64(sint8 ShiftCnt, uint64* data);

/**
 * @brief         Arithmetic shift with saturation.
 * @param[in]     ShiftCnt : Shift count
 * @param[inout]  data     : Pointer to input data
 * @return        uint64
 * @reentrant     FALSE
 * @synchronous   FALSE
 * @trace         CPD-58525
 */
uint64 Bfx_ShiftBitSat_u64s8_u64(sint8 ShiftCnt, uint64* data);

/**
 * @brief         Count the number of consecutive bits which have
 *                the same value as most significant bit in Data,starting with bit
 *                at position msb minus one. Put the result in Data. It is the number of leading
 *                sign bits minus one, giving the number of redundant sign bits in Data.
 * @param[in]     data : input data
 * @return        uint8
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58523
 */
uint8 Bfx_CountLeadingSigns_s64(sint64 data);

/**
 * @brief         Count the number of consecutive ones in Data starting with
 *                the most significant bit and return the result.
 * @param[in]     data : input data
 * @return        uint8
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58524
 */
uint8 Bfx_CountLeadingOnes_u64(uint64 data);

/**
 * @brief         Count the number of consecutive zeros in Data starting with
 *                the most significant bit and return the result.
 * @param[in]     data : input data
 * @return        uint8
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-58522
 */
uint8 Bfx_CountLeadingZeros_u64(uint64 data);
/*PRQA S 1501,1753 -- */
/**
 * @brief         Returns the version information of this library.
 * @param[out]    versionInfo : version information
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-77763
 */
void Bfx_GetVersionInfo(Std_VersionInfoType* versionInfo);
#ifdef __cplusplus
}

#endif
#endif
