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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_62_Hash_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Retrieves the message digest information for a given algorithm family.
 *
 * @param[in] algorithmfamily  The algorithm family type.
 *
 * @return  Pointer to the message digest information structure.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
const Crypto_Md_Info_t* Crypto_Md_info_from_type(Crypto_AlgorithmFamilyType algorithmfamily);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Retrieves the size of the message digest for a given message digest information structure.
 *
 * @param[in] md_info  Pointer to the message digest information structure.
 *
 * @return  Size of the message digest in bytes.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
uint8 Crypto_md_get_size(const Crypto_Md_Info_t* md_info);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_md_setup(Crypto_Md_Context_t* ctx, const Crypto_Md_Info_t* md_info, uint8 hmac);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * \brief           This function clears the internal structure of \p ctx and
 *                  frees any embedded internal structure, but does not free
 *                  \p ctx itself.
 *
 *                  If you have called Crypto_md_setup() on \p ctx, you must
 *                  call Crypto_md_free() when you are no longer using the
 *                  context.
 *                  Calling this function if you have previously
 *                  called Crypto_md_init() and nothing else is optional.
 *                  You must not call this function if you have not called
 *                  Crypto_md_init().
 */
void Crypto_md_free(Crypto_Md_Context_t* ctx); /*PRQA S 1501*/ /*VL_Crypto_62_1501*/
/**
 * @brief   Initializes the message digest context for a new hash operation.
 *
 * @param[in,out] ctx  Pointer to the message digest context structure.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_Md_Start(Crypto_Md_Context_t* ctx);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_Md_Update(Crypto_Md_Context_t* ctx, const uint8* input, uint32 ilen);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_Md_Finish(Crypto_Md_Context_t* ctx, uint8* output);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/* PRQA S 1501 ++ */ /* VL_Crypto_62_1501 */
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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_md(const Crypto_Md_Info_t* md_info, const uint8* input, uint32 ilen, uint8* output);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/* PRQA S 1501 -- */

#if (CRYPTO_ALGORITHMFAM_BLAKE_2s_256 == STD_ON)
/**
 * @brief   Processes the Blake2s hash operation for a given object ID.
 *
 * @param[in] objectId  The object ID for the Blake2s process.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_Blake2s_Process(uint32 objectId);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif /*CRYPTO_ALGORITHMFAM_BLAKE_2s_256 == STD_ON*/

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
