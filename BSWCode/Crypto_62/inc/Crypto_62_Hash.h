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
 **  @file               : Crypto_Hash.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_HASH_H
#define CRYPTO_HASH_H

/* PRQA S 1501,1753 EOF */ /* VL_Crypto_62_1501,VL_Crypto_62_1753 */

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_SERVICE_HASH == STD_ON)

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#define CRYPTO_MD_MAX_SIZE 144 /**< Maximum hash size  sha3-224*/

/* ================================================ type definitions ================================================ */
/** @brief Blake2s Context*/
typedef struct
{
    uint8  b[CRYPTO_CONST_64]; // input buffer
    uint32 h[CRYPTO_CONST_8];  // chained state
    uint32 t[CRYPTO_CONST_2];  // total number of bytes
    uint32 c;                  // pointer for b[]
    uint32 outlen;             // digest size
} Crypto_Blake2s_Context;

/** @brief
 * Message digest information.
 * Allows message digest functions to be called in a generic way.
 */

typedef struct
{
    /** Digest identifier */
    Crypto_AlgorithmFamilyType algorithmfamily;

    /** Output length of the digest function in bytes */
    uint8 size;

    /** Block length of the digest function in bytes */
    uint8 block_size;

} Crypto_Md_Info_t;
/** @brief Message Digest Context*/
typedef struct
{
    const Crypto_Md_Info_t* md_info;  /* Information about the associated message digest. */
    void*                   md_ctx;   /* The digest-specific context. */
    void*                   hmac_ctx; /** The HMAC part of the context. */
} Crypto_Md_Context_t;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief   Processes the hash operation for a given object ID, algorithm family, mode, and operation mode.
 *
 * @param[in] objectId         The object ID for the hash process.
 * @param[in] algorithmfamily  The algorithm family type.
 * @param[in] mode             The algorithm mode type.
 * @param[in] operateMode      The operation mode type.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_62_Hash_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode);

/**
 * @brief   Retrieves the message digest information for a given algorithm family.
 *
 * @param[in] algorithmfamily  The algorithm family type.
 *
 * @return  Pointer to the message digest information structure.
 */
const Crypto_Md_Info_t* Crypto_Md_info_from_type(Crypto_AlgorithmFamilyType algorithmfamily);

/**
 * @brief   Retrieves the size of the message digest for a given message digest information structure.
 *
 * @param[in] md_info  Pointer to the message digest information structure.
 *
 * @return  Size of the message digest in bytes.
 */
uint8 Crypto_md_get_size(const Crypto_Md_Info_t* md_info);

/**
 * @brief   Sets up the message digest context with the specified message digest information.
 *
 * @param[in,out] ctx      Pointer to the message digest context structure.
 * @param[in]     md_info  Pointer to the message digest information structure.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_md_setup(Crypto_Md_Context_t* ctx, const Crypto_Md_Info_t* md_info);

/**
 * @brief   Initializes the message digest context for a new hash operation.
 *
 * @param[in,out] ctx  Pointer to the message digest context structure.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Md_Start(Crypto_Md_Context_t* ctx);

/**
 * @brief   Updates the message digest context with new input data.
 *
 * @param[in,out] ctx    Pointer to the message digest context structure.
 * @param[in]     input  Pointer to the input data.
 * @param[in]     ilen   Length of the input data in bytes.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Md_Update(Crypto_Md_Context_t* ctx, const uint8* input, uint32 ilen);

/**
 * @brief   Finalizes the message digest context and produces the hash output.
 *
 * @param[in,out] ctx    Pointer to the message digest context structure.
 * @param[out]    output Pointer to the buffer where the hash output will be stored.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Md_Finish(Crypto_Md_Context_t* ctx, uint8* output);
/**
 * @brief   Frees and clears the internal message-digest context.
 *
 * @details This function is responsible for securely freeing and clearing
 *          the internal context data associated with a message-digest operation.
 *          It ensures no sensitive data remains in memory after execution.
 *
 * @param[in] ctx  Pointer to the generic message-digest context structure.
 *
 * @return  void
 */
void Crypto_Md_Free(Crypto_Md_Context_t* ctx);
/**
 * @brief   Computes the hash of the input data using the specified message digest information.
 *
 * @param[in]     md_info  Pointer to the message digest information structure.
 * @param[in]     input    Pointer to the input data.
 * @param[in]     ilen     Length of the input data in bytes.
 * @param[out]    output   Pointer to the buffer where the hash output will be stored.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_md(const Crypto_Md_Info_t* md_info, const uint8* input, uint32 ilen, uint8* output);

/**
 * @brief   Processes the Blake2s hash operation for a given object ID.
 *
 * @param[in] objectId  The object ID for the Blake2s process.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Blake2s_Process(uint32 objectId);

/**
 * @brief   Retrieves the size of the message digest for a given algorithm family.
 *
 * @param[in] algorithmfamily  The algorithm family type.
 *
 * @return  Size of the message digest in bytes.
 */
CRYPTO_62_LOCAL inline uint8 Crypto_md_get_size_from_type(Crypto_AlgorithmFamilyType algorithmfamily)
{
    return Crypto_md_get_size(Crypto_Md_info_from_type(algorithmfamily));
}

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_SERVICE_HASH == STD_ON */

#endif /* CRYPTO_HASH_H */
