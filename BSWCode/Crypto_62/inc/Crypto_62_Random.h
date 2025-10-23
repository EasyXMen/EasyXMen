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
 **  @file               : Crypto_62_Random.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_RANDOM_H
#define CRYPTO_RANDOM_H

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_SERVICE_RANDOM == STD_ON)
#include "Crypto_62_Sha512.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#define CRYPTO_ENTROPY_MAX_SOURCES 20 /**< Maximum number of sources supported */

/* ================================================ type definitions ================================================ */
/******************************************************************/
/**
 * \brief           Entropy poll callback pointer
 *
 * \param data      Callback-specific data pointer
 * \param output    Data to fill
 * \param len       Maximum size to provide
 * \param olen      The actual amount of bytes put into the buffer (Can be 0)
 *
 * \return          0 if no critical failures occurred,
 *                  CRYPTO_ERR_ENTROPY_SOURCE_FAILED otherwise
 */
/******************************************************************/
/** @brief Entropy source callback function pointer */
typedef int (*crypto_entropy_f_source_ptr)(void* data, unsigned char* output, uint32 len, uint32* olen);
/** @brief Entropy Source State Structure*/
typedef struct
{
    crypto_entropy_f_source_ptr f_source;  /**< The entropy source callback */
    void*                       p_source;  /**< The callback data pointer */
    uint64                      size;      /**< Amount received in bytes */
    uint64                      threshold; /**< Minimum bytes required before release */
    uint32                      strong;    /**< Is the source strong? */
} Crypto_entropy_source_state;

/** @brief Entropy Context Structure*/
typedef struct
{
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
    Crypto_Sha512_Context accumulator;
    uint32                accumulator_started; /* 0 after init.
                                                * 1 after the first update.
                                                * -1 after free. */
#endif
    uint32                      source_count; /* Number of entries used in source. */
    Crypto_entropy_source_state source[CRYPTO_ENTROPY_MAX_SOURCES];

} Crypto_entropy_context;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief   Entropy function to gather entropy data.
 *
 * @param[in]  data    Callback-specific data pointer.
 * @param[out] output  Data buffer to fill with entropy.
 * @param[in]  len     Maximum size of the buffer in bytes.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_entropy_func(void* data, uint8* output, uint32 len);

/**
 * @brief   Initializes the entropy context.
 *
 * @param[in,out] ctx  Pointer to the entropy context structure.
 *
 * @return  None.
 */
void Crypto_entropy_init(Crypto_entropy_context* ctx);

/**
 * @brief   Polls the platform entropy source for entropy data.
 *
 * @param[in]  data    Callback-specific data pointer.
 * @param[out] output  Data buffer to fill with entropy.
 * @param[in]  len     Maximum size of the buffer in bytes.
 * @param[out] olen    Actual amount of bytes put into the buffer.
 *
 * @return  Status of the operation.
 *          - 0: No critical failures occurred.
 *          - CRYPTO_ERR_ENTROPY_SOURCE_FAILED: Critical failure occurred.
 */
int Crypto_platform_entropy_poll(void* data, unsigned char* output, uint32 len, uint32* olen);
#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_SERVICE_RANDOM == STD_ON */

#endif /* CRYPTO_RANDOM_H */
