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
 **  @file               : Crypto_62_Sm3.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_SM3_H
#define CRYPTO_SM3_H

#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_SM3 == STD_ON)

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/** @brief for Sm3 struct */
typedef struct
{
    uint32 A, B, C, D, E, F, G, H;
    uint32 Nl, Nh;
    uint32 data[16];
    uint32 num;
} Crypto_SM3_Context;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief   Initializes the SM3 context.
 *
 * This function initializes the SM3 context structure, preparing it for a new SM3 hash computation.
 *
 * @param[out] ctx  Pointer to the SM3 context structure to be initialized.
 */
void Crypto_Sm3_Start(Crypto_SM3_Context* ctx);

/**
 * @brief   Updates the SM3 context with a new block of data.
 *
 * This function updates the SM3 context with a new block of data. It can be called multiple times to process
 * large amounts of data in chunks.
 *
 * @param[in,out] c     Pointer to the SM3 context structure.
 * @param[in]     data_ Pointer to the input data to be processed.
 * @param[in]     len   Length of the input data in bytes.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sm3_Update(Crypto_SM3_Context* c, const void* data_, uint32 len);

/**
 * @brief   Finalizes the SM3 hash computation.
 *
 * This function finalizes the SM3 hash computation and writes the resulting hash to the output buffer.
 *
 * @param[in,out] c  Pointer to the SM3 context structure.
 * @param[out]    md Pointer to the buffer where the resulting hash will be stored.
 */
void Crypto_Sm3_Finish(Crypto_SM3_Context* c, uint8* md);

/**
 * @brief   Computes the SM3 hash of a given input data.
 *
 * This function computes the SM3 hash of a given input data and writes the resulting hash to the output buffer.
 *
 * @param[in]  input  Pointer to the input data to be hashed.
 * @param[in]  ilen   Length of the input data in bytes.
 * @param[out] output Pointer to the buffer where the resulting hash will be stored.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sm3(uint8* input, uint32 ilen, uint8* output);

/**
 * @brief   Processes the SM3 hash computation for a given object ID.
 *
 * This function processes the SM3 hash computation for a given object ID. It is intended for internal use.
 *
 * @param[in] objectId  The object ID for the SM3 hash computation.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sm3_Process(uint32 objectId);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_ALGORITHMFAM_SM3 == STD_ON */

#endif /* CRYPTO_SM3_H */
