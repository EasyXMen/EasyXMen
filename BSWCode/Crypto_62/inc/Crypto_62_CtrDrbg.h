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
 **  @file               : Crypto_CtrDrbg.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_CTRDRBG_H
#define CRYPTO_CTRDRBG_H

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMMODE_CTRDRBG == STD_ON)
#include "Crypto_62.h"
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
#include "Crypto_62_Aes.h"
#endif
#include "Crypto_62_Des.h"
#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#define CRYPTO_CTR_DRBG_BLOCKSIZE 16U /**< The block size used by the cipher. */

#define CRYPTO_CTR_DRBG_KEYSIZE 32U
/**< The key size in bytes used by the cipher.
 *
 * Compile-time choice: 32 bytes (256 bits)
 * because \c CRYPTO_CTR_DRBG_USE_128_BIT_KEY is disabled.
 */

#define CRYPTO_CTR_DRBG_KEYBITS (CRYPTO_CTR_DRBG_KEYSIZE * 8U) /**< The key size for the DRBG operation, in bits. */
#define CRYPTO_CTR_DRBG_SEEDLEN \
    (CRYPTO_CTR_DRBG_KEYSIZE + CRYPTO_CTR_DRBG_BLOCKSIZE) /**< The seed length, calculated as (counter + AES key). */

#define CRYPTO_CTR_DRBG_ENTROPY_LEN 48U

#define CRYPTO_CTR_DRBG_RESEED_INTERVAL 10000U

#define CRYPTO_CTR_DRBG_MAX_INPUT 256U

#define CRYPTO_CTR_DRBG_MAX_REQUEST 1024U

#define CRYPTO_CTR_DRBG_MAX_SEED_INPUT 384U

#define PSA_KEY_TYPE_NONE 0x0000u

#define PSA_KEY_LIFETIME_VOLATILE 0x00000000u

#define CRYPTO_SVC_KEY_ID_INIT ((uint32)0)

/* PRQA S 3457 ++ */ /* VL_Crypto_62_3457 */
#define PSA_KEY_POLICY_INIT {0, 0, 0}
#define PSA_KEY_ATTRIBUTES_INIT \
    {PSA_KEY_TYPE_NONE, 0u, PSA_KEY_LIFETIME_VOLATILE, PSA_KEY_POLICY_INIT, CRYPTO_SVC_KEY_ID_INIT}
/* PRQA S 3457 -- */
#define CRYPTO_CTR_DRBG_RESEED_INTERVAL 10000U

/* ================================================ type definitions ================================================ */
/** @brief the context for CTR_DRBG*/
typedef struct
{
    uint8  counter[CRYPTO_CONST_16]; /*!< The counter (V). */
    uint32 reseed_counter;           /*!< The reseed counter.
                                      * This is the number of requests that have
                                      * been made since the last (re)seeding,
                                      * minus one.
                                      * Before the initial seeding, this field
                                      * contains the amount of entropy in bytes
                                      * to use as a nonce for the initial seeding,
                                      * or -1 if no nonce length has been explicitly
                                      * set (see CRYPTO_ctr_drbg_set_nonce_len()).
                                      */
    uint32 prediction_resistance;    /*!< This determines whether prediction
                                                      resistance is enabled, that is
                                                      whether to systematically reseed before
                                                      each random generation. */
    uint32 entropy_len;              /*!< The amount of entropy grabbed on each
                                                         seed or reseed operation, in bytes. */
    uint32 reseed_interval;          /*!< The reseed interval.
                                      * This is the maximum number of requests
                                      * that can be made between reseedings. */
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
    Crypto_AESData aes_ctx; /*!< The AES context. */
#endif
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
    Crypto_Des3Data des3_ctx;
#endif

    /*
     * Callbacks (Entropy)
     */
    /* PRQA S 1336 ++ */ /* VL_Crypto_62_1336 */
    Std_ReturnType (*f_entropy)(void*, unsigned char*, uint32);
    /* PRQA S 1336 -- */
    /*!< The entropy callback function. */

    void*                      p_entropy; /*!< The context for the entropy function. */
    Crypto_AlgorithmFamilyType algorithmfamily;

} Crypto_Ctr_Drbg_Context;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief   Processes the CTR_DRBG algorithm for a given object ID and algorithm family.
 *
 * @param[in] objectId         The object ID for the CTR_DRBG process.
 * @param[in] algorithmfamily  The algorithm family type.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_CtrDrbgProcess(uint32 objectId, Crypto_AlgorithmFamilyType algorithmfamily);

/**
 * @brief   Seeds the CTR_DRBG context with entropy and an optional custom value.
 *
 * @param[in,out] ctx          Pointer to the CTR_DRBG context structure.
 * @param[in]     f_entropy    Pointer to the entropy callback function.
 * @param[in]     p_entropy    Pointer to the context for the entropy function.
 * @param[in]     custom       Pointer to the custom input data (can be NULL).
 * @param[in]     len          Length of the custom input data.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
/* PRQA S 3672,1336 ++ */ /* VL_Crypto_62_3672,VL_Crypto_62_1336 */
Std_ReturnType Crypto_ctr_drbg_seed(
    Crypto_Ctr_Drbg_Context* ctx,
    Std_ReturnType (*f_entropy)(void*, uint8*, uint32),
    void*        p_entropy,
    const uint8* custom,
    uint32       len);
/* PRQA S 3672,1336 -- */

/**
 * @brief   Generates random data using the CTR_DRBG context.
 *
 * @param[in]  p_rng        Pointer to the CTR_DRBG context structure.
 * @param[out] output       Pointer to the buffer where the random data will be stored.
 * @param[in]  output_len   Length of the random data to generate.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_ctr_drbg_random(void* p_rng, uint8* output, uint32 output_len);

/**
 * @brief   Initializes the CTR_DRBG context.
 *
 * @param[in,out] ctx          Pointer to the CTR_DRBG context structure.
 * @param[in]     algorithmfamily  The algorithm family type.
 *
 * @return  None.
 */
void Crypto_Ctr_Drbg_Init(Crypto_Ctr_Drbg_Context* ctx, Crypto_AlgorithmFamilyType algorithmfamily);

/**
 * @brief   Generates random data with additional input using the CTR_DRBG context.
 *
 * @param[in]  p_rng        Pointer to the CTR_DRBG context structure.
 * @param[out] output       Pointer to the buffer where the random data will be stored.
 * @param[in]  output_len   Length of the random data to generate.
 * @param[in]  additional   Pointer to the additional input data (can be NULL).
 * @param[in]  add_len      Length of the additional input data.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_ctr_drbg_random_with_add(
    void*        p_rng,
    uint8*       output,
    uint32       output_len,
    const uint8* additional,
    uint32       add_len);

/**
 * @brief   Reseeds the CTR_DRBG context with additional input.
 *
 * @param[in,out] ctx          Pointer to the CTR_DRBG context structure.
 * @param[in]     additional   Pointer to the additional input data (can be NULL).
 * @param[in]     len          Length of the additional input data.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_ctr_drbg_reseed(Crypto_Ctr_Drbg_Context* ctx, const uint8* additional, uint32 len);

/**
 * @brief   Updates the CTR_DRBG context with additional input.
 *
 * @param[in,out] ctx          Pointer to the CTR_DRBG context structure.
 * @param[in]     additional   Pointer to the additional input data (can be NULL).
 * @param[in]     add_len      Length of the additional input data.
 * @param[in]     algorithmfamily  The algorithm family type.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_ctr_drbg_update(
    Crypto_Ctr_Drbg_Context*   ctx,
    const uint8*               additional,
    uint32                     add_len,
    Crypto_AlgorithmFamilyType algorithmfamily);

/**
 * @brief   Sets the reseed interval for the CTR_DRBG context.
 *
 * @param[in,out] ctx          Pointer to the CTR_DRBG context structure.
 * @param[in]     interval     The reseed interval.
 *
 * @return  None.
 */
void Crypto_Ctr_Drbg_Set_reseed_interval(Crypto_Ctr_Drbg_Context* ctx, uint32 interval);

/**
 * @brief   Sets the nonce length for the CTR_DRBG context.
 *
 * @param[in,out] ctx          Pointer to the CTR_DRBG context structure.
 * @param[in]     len          The nonce length.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Ctr_Drbg_Set_Nonce_Len(Crypto_Ctr_Drbg_Context* ctx, uint32 len);

/**
 * @brief   Sets the entropy length for the CTR_DRBG context.
 *
 * @param[in,out] ctx          Pointer to the CTR_DRBG context structure.
 * @param[in]     len          The entropy length.
 *
 * @return  None.
 */
void Crypto_Ctr_Drbg_Set_Entropy_Len(Crypto_Ctr_Drbg_Context* ctx, uint32 len);

/**
 * @brief   Sets the prediction resistance flag for the CTR_DRBG context.
 *
 * @param[in,out] ctx          Pointer to the CTR_DRBG context structure.
 * @param[in]     resistance   The prediction resistance flag.
 *
 * @return  None.
 */
void Crypto_Ctr_Drbg_Set_Prediction_Resistance(Crypto_Ctr_Drbg_Context* ctx, uint32 resistance);
#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_ALGORITHMMODE_CTRDRBG == STD_ON */

#endif /* CRYPTO_CTRDRBG_H */
