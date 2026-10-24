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
#if (CRYPTO_SERVICE_RANDOM == STD_ON)
#if (CRYPTO_ALGORITHMMODE_HMAC == STD_ON)
#include "Crypto_62_Hash.h"
#include "Crypto_62_Random.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
typedef struct Crypto_hmac_drbg_context_Tag
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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_HmacDrbgProcess(uint32 objectId, Crypto_AlgorithmFamilyType algorithmfamily);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               HMAC_DRBG random function.
 *
 * Param-Name[in]      p_rng: The HMAC_DRBG context.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The buffer to fill.
 *                     out_len: The length of the buffer in bytes.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_hmac_drbg_random(void* p_rng, unsigned char* output, uint32 out_len);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function resets HMAC_DRBG context to the initialized
 *                     state.
 *
 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The HMAC_DRBG context to free.
 * Return              None
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void Crypto_hmac_drbg_free(Crypto_hmac_drbg_context* ctx);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/**
 * \brief               HMAC_DRBG context initialization.
 *
 * This function makes the context ready for Crypto_hmac_drbg_seed(),
 * Crypto_hmac_drbg_seed_buf() or Crypto_hmac_drbg_free().
 *
 * \note                The reseed interval is #Crypto_HMAC_DRBG_RESEED_INTERVAL
 *                      by default. Override this value by calling
 *                      Crypto_hmac_drbg_set_reseed_interval().
 *
 * \param ctx           HMAC_DRBG context to be initialized.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void Crypto(Crypto_hmac_drbg_context* ctx);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * \param ctx           HMAC_DRBG context to be initialised.
 * \param md_info       MD algorithm to use for HMAC_DRBG.
 * \param data          Concatenation of the initial entropy string and
 *                      the additional data.
 * \param data_len      Length of \p data in bytes.
 *
 * \return              \c 0 if successful. or
 * \return              #MBEDTLS_ERR_MD_BAD_INPUT_DATA if \p md_info is
 *                      invalid.
 * \return              #MBEDTLS_ERR_MD_ALLOC_FAILED if there was not enough
 *                      memory to allocate context data.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_hmac_drbg_seed_buf(
    Crypto_hmac_drbg_context* ctx,
    const Crypto_Md_Info_t*   md_info,
    const unsigned char*      data,
    uint32                    data_len);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               HMAC_DRBG context initialization.
 *
 * Param-Name[in]      ctx: HMAC_DRBG context to be initialized.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void Crypto_hmac_drbg_init(Crypto_hmac_drbg_context* ctx);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_ALGORITHMMODE_HMAC == STD_ON */
#endif /* CRYPTO_SERVICE_RANDOM == STD_ON */

#endif /* CRYPTO_HMACDRBG_H */
