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
 **  @file               : Crypto_62_HmacDrbg.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_HMACDRBG_H
#define CRYPTO_HMACDRBG_H

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMMODE_HMAC == STD_ON)
#include "Crypto_62_Random.h"
#include "Crypto_62_Hash.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
typedef struct
{
    Crypto_Md_Context_t md_ctx;
    unsigned char       V[CRYPTO_CONST_64];
    int                 reseed_counter;
    uint32              entropy_len;
    int                 prediction_resistance;
    int                 reseed_interval;
    Std_ReturnType (*f_entropy)(void*, unsigned char*, uint32);
    void* p_entropy;
} Crypto_hmac_drbg_context;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief   Processes the HMAC_DRBG algorithm for a given object ID and algorithm family.
 *
 * @param[in] objectId         The object ID for the HMAC_DRBG process.
 * @param[in] algorithmfamily  The algorithm family type.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_HmacDrbgProcess(uint32 objectId, Crypto_AlgorithmFamilyType algorithmfamily);
#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_ALGORITHMMODE_HMAC == STD_ON */

#endif /* CRYPTO_HMACDRBG_H */
