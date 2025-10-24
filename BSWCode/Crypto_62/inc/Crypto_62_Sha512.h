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
 **  @file               : Crypto_62_Sha512.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_SHA512_H
#define CRYPTO_SHA512_H

#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#define UL64(x) x##ULL

/* ================================================ type definitions ================================================ */
/** @brief SHA512 Context*/
typedef struct
{
    uint64 total[CRYPTO_CONST_2];    /*!< The number of Bytes processed. */
    uint64 state[CRYPTO_CONST_8];    /*!< The intermediate digest state. */
    uint8  buffer[CRYPTO_CONST_128]; /*!< The data block being processed. */
} Crypto_Sha512_Context;

/* ========================================== internal function definitions ========================================= */
/*
 * Round constants
 */
CRYPTO_62_LOCAL const uint64 K[CRYPTO_CONST_80] = {
    UL64(0x428A2F98D728AE22), UL64(0x7137449123EF65CD), UL64(0xB5C0FBCFEC4D3B2F), UL64(0xE9B5DBA58189DBBC),
    UL64(0x3956C25BF348B538), UL64(0x59F111F1B605D019), UL64(0x923F82A4AF194F9B), UL64(0xAB1C5ED5DA6D8118),
    UL64(0xD807AA98A3030242), UL64(0x12835B0145706FBE), UL64(0x243185BE4EE4B28C), UL64(0x550C7DC3D5FFB4E2),
    UL64(0x72BE5D74F27B896F), UL64(0x80DEB1FE3B1696B1), UL64(0x9BDC06A725C71235), UL64(0xC19BF174CF692694),
    UL64(0xE49B69C19EF14AD2), UL64(0xEFBE4786384F25E3), UL64(0x0FC19DC68B8CD5B5), UL64(0x240CA1CC77AC9C65),
    UL64(0x2DE92C6F592B0275), UL64(0x4A7484AA6EA6E483), UL64(0x5CB0A9DCBD41FBD4), UL64(0x76F988DA831153B5),
    UL64(0x983E5152EE66DFAB), UL64(0xA831C66D2DB43210), UL64(0xB00327C898FB213F), UL64(0xBF597FC7BEEF0EE4),
    UL64(0xC6E00BF33DA88FC2), UL64(0xD5A79147930AA725), UL64(0x06CA6351E003826F), UL64(0x142929670A0E6E70),
    UL64(0x27B70A8546D22FFC), UL64(0x2E1B21385C26C926), UL64(0x4D2C6DFC5AC42AED), UL64(0x53380D139D95B3DF),
    UL64(0x650A73548BAF63DE), UL64(0x766A0ABB3C77B2A8), UL64(0x81C2C92E47EDAEE6), UL64(0x92722C851482353B),
    UL64(0xA2BFE8A14CF10364), UL64(0xA81A664BBC423001), UL64(0xC24B8B70D0F89791), UL64(0xC76C51A30654BE30),
    UL64(0xD192E819D6EF5218), UL64(0xD69906245565A910), UL64(0xF40E35855771202A), UL64(0x106AA07032BBD1B8),
    UL64(0x19A4C116B8D2D0C8), UL64(0x1E376C085141AB53), UL64(0x2748774CDF8EEB99), UL64(0x34B0BCB5E19B48A8),
    UL64(0x391C0CB3C5C95A63), UL64(0x4ED8AA4AE3418ACB), UL64(0x5B9CCA4F7763E373), UL64(0x682E6FF3D6B2B8A3),
    UL64(0x748F82EE5DEFB2FC), UL64(0x78A5636F43172F60), UL64(0x84C87814A1F0AB72), UL64(0x8CC702081A6439EC),
    UL64(0x90BEFFFA23631E28), UL64(0xA4506CEBDE82BDE9), UL64(0xBEF9A3F7B2C67915), UL64(0xC67178F2E372532B),
    UL64(0xCA273ECEEA26619C), UL64(0xD186B8C721C0C207), UL64(0xEADA7DD6CDE0EB1E), UL64(0xF57D4F7FEE6ED178),
    UL64(0x06F067AA72176FBA), UL64(0x0A637DC5A2C898A6), UL64(0x113F9804BEF90DAE), UL64(0x1B710B35131C471B),
    UL64(0x28DB77F523047D84), UL64(0x32CAAB7B40C72493), UL64(0x3C9EBE0A15C9BEBC), UL64(0x431D67C49C100D4C),
    UL64(0x4CC5D4BECB3E42B6), UL64(0x597F299CFC657E2A), UL64(0x5FCB6FAB3AD6FAEC), UL64(0x6C44198C4A475817)};
/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief   Initializes the SHA-512 context.
 *
 * This function initializes the SHA-512 context structure, preparing it for a new SHA-512 or SHA-384 hash computation.
 *
 * @param[out] ctx    Pointer to the SHA-512 context structure to be initialized.
 * @param[in]  is384  Boolean flag indicating whether to compute SHA-384 (true) or SHA-512 (false).
 */
void Crypto_Sha512_Starts(Crypto_Sha512_Context* ctx, boolean is384);

/**
 * @brief   Initializes the SHA-512/224 context.
 *
 * This function initializes the SHA-512 context structure, preparing it for a new SHA-512/224 hash computation.
 *
 * @param[out] ctx  Pointer to the SHA-512 context structure to be initialized.
 */
void Crypto_Sha512_224_Starts(Crypto_Sha512_Context* ctx);

/**
 * @brief   Initializes the SHA-512/256 context.
 *
 * This function initializes the SHA-512 context structure, preparing it for a new SHA-512/256 hash computation.
 *
 * @param[out] ctx  Pointer to the SHA-512 context structure to be initialized.
 */
void Crypto_Sha512_256_Starts(Crypto_Sha512_Context* ctx);

/**
 * @brief   Updates the SHA-512 context with a new block of data.
 *
 * This function updates the SHA-512 context with a new block of data. It can be called multiple times to process
 * large amounts of data in chunks.
 *
 * @param[in,out] ctx   Pointer to the SHA-512 context structure.
 * @param[in]     input Pointer to the input data to be processed.
 * @param[in]     ilen  Length of the input data in bytes.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha512_Update(Crypto_Sha512_Context* ctx, const unsigned char* input, uint32 ilen);

/**
 * @brief   Finalizes the SHA-512 hash computation.
 *
 * This function finalizes the SHA-512 hash computation and writes the resulting hash to the output buffer.
 *
 * @param[in,out] ctx    Pointer to the SHA-512 context structure.
 * @param[out]    output Pointer to the buffer where the resulting hash will be stored.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha512_Finish(Crypto_Sha512_Context* ctx, unsigned char* output);

/**
 * @brief   Computes the SHA-512 or SHA-384 hash of a given input data.
 *
 * This function computes the SHA-512 or SHA-384 hash of a given input data and writes the resulting hash to the output
 * buffer.
 *
 * @param[in]  input  Pointer to the input data to be hashed.
 * @param[in]  ilen   Length of the input data in bytes.
 * @param[out] output Pointer to the buffer where the resulting hash will be stored.
 * @param[in]  is384  Boolean flag indicating whether to compute SHA-384 (true) or SHA-512 (false).
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha512(const unsigned char* input, uint32 ilen, unsigned char* output, boolean is384);

/**
 * @brief   Processes the SHA-512 or SHA-384 hash computation for a given object ID.
 *
 * This function processes the SHA-512 or SHA-384 hash computation for a given object ID. It is intended for internal
 * use.
 *
 * @param[in] objectId  The object ID for the SHA-512 or SHA-384 hash computation.
 * @param[in] is384     Boolean flag indicating whether to compute SHA-384 (true) or SHA-512 (false).
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha512_Process(uint32 objectId, boolean is384);

/**
 * @brief   Computes the SHA-512/224 hash of a given input data.
 *
 * This function computes the SHA-512/224 hash of a given input data and writes the resulting hash to the output buffer.
 *
 * @param[in]  input  Pointer to the input data to be hashed.
 * @param[in]  ilen   Length of the input data in bytes.
 * @param[out] output Pointer to the buffer where the resulting hash will be stored.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_sha512_224(const unsigned char* input, uint32 ilen, unsigned char* output);

/**
 * @brief   Processes the SHA-512/224 hash computation for a given object ID.
 *
 * This function processes the SHA-512/224 hash computation for a given object ID. It is intended for internal use.
 *
 * @param[in] objectId  The object ID for the SHA-512/224 hash computation.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha512_224_Process(uint32 objectId);

/**
 * @brief   Computes the SHA-512/256 hash of a given input data.
 *
 * This function computes the SHA-512/256 hash of a given input data and writes the resulting hash to the output buffer.
 *
 * @param[in]  input  Pointer to the input data to be hashed.
 * @param[in]  ilen   Length of the input data in bytes.
 * @param[out] output Pointer to the buffer where the resulting hash will be stored.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_sha512_256(const unsigned char* input, uint32 ilen, unsigned char* output);

/**
 * @brief   Processes the SHA-512/256 hash computation for a given object ID.
 *
 * This function processes the SHA-512/256 hash computation for a given object ID. It is intended for internal use.
 *
 * @param[in] objectId  The object ID for the SHA-512/256 hash computation.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha512_256_Process(uint32 objectId);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON */

#endif /* CRYPTO_SHA512_H */
