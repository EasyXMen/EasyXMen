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
 **  @file               : Crypto_62_Hash.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* PRQA S 6050,6060,6070,6080,6010,6030,6040,0342,2743,2889,2784,1532 ++ */ /* VL_QAC_Crypto */
/* =================================================== inclusions =================================================== */

#include "Crypto_62_Internal.h"
#if (CRYPTO_SERVICE_HASH == STD_ON)

#include "Crypto_62_Hash.h"
#include "Crypto_62_Ripemd160.h"
#include "Crypto_62_Sha256.h"
#include "Crypto_62_Sha512.h"
#include "Crypto_62_Sha1.h"
#include "Crypto_62_Sha3.h"
#include "Crypto_62_Sm3.h"
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"

/* ===================================================== macros ===================================================== */
/* PRQA S 3410 ++ */ /* VL_QAC_Crypto */
#define MD_INFO(type, out_size, block_size) type, out_size, block_size,
/* PRQA S 3410 -- */

#define ALLOC(type)                                                                                  \
    do                                                                                               \
    {                                                                                                \
        ctx->md_ctx = IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1U, sizeof(Crypto_##type##_Context)); \
        if (ctx->md_ctx == NULL_PTR)                                                                 \
            return E_NOT_OK;                                                                         \
        (void)IStdLib_MemSet(ctx->md_ctx, 0, sizeof(ctx->md_ctx));                                   \
    } while (0)
#define FREE(type)                                                             \
    do                                                                         \
    {                                                                          \
        (void)IStdLib_MemSet(ctx->md_ctx, 0, sizeof(Crypto_##type##_Context)); \
        IStdLib_MemHeapFree(Crypto_62_MemPool, ctx->md_ctx);                   \
    } while (0)
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */
/* PRQA S 1290,3432,3418,3218,3412 ++ */ /* VL_QAC_Crypto */
CRYPTO_62_LOCAL const Crypto_Md_Info_t Crypto_ripemd160_info = {MD_INFO(CRYPTO_ALGOFAM_RIPEMD160, 20, 64)};

CRYPTO_62_LOCAL const Crypto_Md_Info_t Crypto_sha1_info = {MD_INFO(CRYPTO_ALGOFAM_SHA1, 20, 64)};

CRYPTO_62_LOCAL const Crypto_Md_Info_t Crypto_sha224_info = {MD_INFO(CRYPTO_ALGOFAM_SHA2_224, 28, 64)};

CRYPTO_62_LOCAL const Crypto_Md_Info_t Crypto_sha256_info = {MD_INFO(CRYPTO_ALGOFAM_SHA2_256, 32, 64)};

CRYPTO_62_LOCAL const Crypto_Md_Info_t Crypto_sha384_info = {MD_INFO(CRYPTO_ALGOFAM_SHA2_384, 48, 128)};

CRYPTO_62_LOCAL const Crypto_Md_Info_t Crypto_sha512_info = {MD_INFO(CRYPTO_ALGOFAM_SHA2_512, 64, 128)};

CRYPTO_62_LOCAL const Crypto_Md_Info_t Crypto_sha3_224_info = {MD_INFO(CRYPTO_ALGOFAM_SHA3_224, 28, 144)};

CRYPTO_62_LOCAL const Crypto_Md_Info_t Crypto_sha3_256_info = {MD_INFO(CRYPTO_ALGOFAM_SHA3_256, 32, 136)};

CRYPTO_62_LOCAL const Crypto_Md_Info_t Crypto_sha3_384_info = {MD_INFO(CRYPTO_ALGOFAM_SHA3_384, 48, 104)};

CRYPTO_62_LOCAL const Crypto_Md_Info_t Crypto_sha3_512_info = {MD_INFO(CRYPTO_ALGOFAM_SHA3_512, 64, 72)};

#if (CRYPTO_ALGORITHMFAM_SM3 == STD_ON)
CRYPTO_62_LOCAL const Crypto_Md_Info_t Crypto_sm3_info = {MD_INFO(CRYPTO_ALGOFAM_SM3, 32, 64)};
#endif
/* PRQA S 1290,3432,3418,3218,3412 -- */
/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/******************************************************************************/
/*
 * Brief               This function returns the message-digest information
 *                       associated with the given digest type.
 *
 * Param-Name[in]      algorithmfamily: The type of digest to search for.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              const Crypto_Md_Info_t*: The message-digest information
 *                                              associated with algorithmfamily.
 */
/******************************************************************************/
/* PRQA S 2023,2024++ */ /* VL_QAC_Crypto */
const Crypto_Md_Info_t* Crypto_Md_info_from_type(Crypto_AlgorithmFamilyType algorithmfamily)
{
    switch (algorithmfamily)
    {
    case CRYPTO_ALGOFAM_RIPEMD160:
        return &Crypto_ripemd160_info;

    case CRYPTO_ALGOFAM_SHA1:
        return &Crypto_sha1_info;

    case CRYPTO_ALGOFAM_SHA2_224:
        return &Crypto_sha224_info;

    case CRYPTO_ALGOFAM_SHA2_256:
        return &Crypto_sha256_info;

    case CRYPTO_ALGOFAM_SHA2_384:
        return &Crypto_sha384_info;

    case CRYPTO_ALGOFAM_SHA2_512:
        return &Crypto_sha512_info;

    case CRYPTO_ALGOFAM_SHA3_224:
        return &Crypto_sha3_224_info;

    case CRYPTO_ALGOFAM_SHA3_256:
        return &Crypto_sha3_256_info;

    case CRYPTO_ALGOFAM_SHA3_384:
        return &Crypto_sha3_384_info;

    case CRYPTO_ALGOFAM_SHA3_512:
        return &Crypto_sha3_512_info;
    case CRYPTO_ALGOFAM_SM3:
#if (CRYPTO_ALGORITHMFAM_SM3 == STD_ON)
        return &Crypto_sm3_info;
#endif
        return NULL_PTR;
    default:
        return NULL_PTR;
    }
}
/* PRQA S 2023,2024 -- */
/******************************************************************************/
/*
 * Brief               This function extracts the message-digest size from the
 *                     message-digest information structure.
 *
 * Param-Name[in]      md_info: The information structure of the message-digest
 *                               algorithm to use.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              unsigned char: The size of the message-digest output in Bytes.
 */
/******************************************************************************/
unsigned char Crypto_md_get_size(const Crypto_Md_Info_t* md_info)
{
    if (md_info == NULL_PTR)
    {
        return E_NOT_OK;
    }

    return md_info->size;
}

/******************************************************************************/
/*
 * Brief               This function selects the message digest algorithm to use,
 *                     and allocates internal structures.
 * Param-Name[in]      md_info: The information structure of the message-digest
 *                                 algorithm to use.
 * Param-Name[in/out]  ctx: The context to set up.
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_md_setup(Crypto_Md_Context_t* ctx, const Crypto_Md_Info_t* md_info)
{
    Std_ReturnType ret = E_OK;
    if (ctx == NULL_PTR)
    {
        return E_NOT_OK;
    }

    if (md_info == NULL_PTR)
    {
        return E_NOT_OK;
    }

    ctx->md_info  = md_info;
    ctx->md_ctx   = NULL_PTR;
    ctx->hmac_ctx = IStdLib_MemHeapCalloc(Crypto_62_MemPool, CRYPTO_CONST_2, md_info->block_size);

    switch (md_info->algorithmfamily)
    {
    case CRYPTO_ALGOFAM_RIPEMD160:
#if (CRYPTO_ALGORITHMFAM_RIPEMD160 == STD_ON)
        ALLOC(Ripemd160);
#endif
        break;

    case CRYPTO_ALGOFAM_SHA1:
#if (CRYPTO_ALGORITHMFAM_SHA1 == STD_ON)
        ALLOC(Sha1);
#endif
        break;

    case CRYPTO_ALGOFAM_SHA2_224:
#if (CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON)
        ALLOC(Sha256);
#endif
        break;

    case CRYPTO_ALGOFAM_SHA2_256:
#if (CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON)
        ALLOC(Sha256);
#endif
        break;

    case CRYPTO_ALGOFAM_SHA2_384:
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
        ALLOC(Sha512);
#endif
        break;

    case CRYPTO_ALGOFAM_SHA2_512:
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
        ALLOC(Sha512);
#endif
        break;

    case CRYPTO_ALGOFAM_SHA3_224:
    case CRYPTO_ALGOFAM_SHA3_256:
    case CRYPTO_ALGOFAM_SHA3_384:
    case CRYPTO_ALGOFAM_SHA3_512:
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
        ALLOC(Sha3);
#endif
        break;
    case CRYPTO_ALGOFAM_SM3:
#if (CRYPTO_ALGORITHMFAM_SM3 == STD_ON)
        ALLOC(SM3);
#endif
        break;
    default:
        ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
        break;
    }

    return ret;
}
/******************************************************************************/
/*
 * Brief               This function frees and clears the internal message-digest context.
 *
 * Param-Name[in]      ctx: Pointer to the generic message-digest context structure.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              void
 */
/******************************************************************************/
void Crypto_Md_Free(Crypto_Md_Context_t* ctx)
{
    if (ctx == NULL_PTR || ctx->md_info == NULL_PTR)
    {
        return;
    }

    if (ctx->md_ctx != NULL_PTR)
    {
        switch (ctx->md_info->algorithmfamily)
        {
        case CRYPTO_ALGOFAM_RIPEMD160:
#if (CRYPTO_ALGORITHMFAM_RIPEMD160 == STD_ON)
            FREE(Ripemd160);
#endif
            break;

        case CRYPTO_ALGOFAM_SHA1:
#if (CRYPTO_ALGORITHMFAM_SHA1 == STD_ON)
            FREE(Sha1);
#endif
            break;

        case CRYPTO_ALGOFAM_SHA2_224:
#if (CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON)
            FREE(Sha256);
#endif
            break;

        case CRYPTO_ALGOFAM_SHA2_256:
#if (CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON)
            FREE(Sha256);
#endif
            break;

        case CRYPTO_ALGOFAM_SHA2_384:
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
            FREE(Sha512);
#endif
            break;

        case CRYPTO_ALGOFAM_SHA2_512:
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
            FREE(Sha512);
#endif
            break;

        case CRYPTO_ALGOFAM_SHA3_224:
        case CRYPTO_ALGOFAM_SHA3_256:
        case CRYPTO_ALGOFAM_SHA3_384:
        case CRYPTO_ALGOFAM_SHA3_512:
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
            FREE(Sha3);
#endif
            break;
        case CRYPTO_ALGOFAM_SM3:
#if (CRYPTO_ALGORITHMFAM_SM3 == STD_ON)
            FREE(SM3);
#endif
            break;
        default:
            break;
        }
    }
}
/******************************************************************************/
/*
 * Brief               This function starts a message-digest computation.
 *
 * Param-Name[in]      None
 * Param-Name[in/out]  ctx: The generic message-digest context.
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
/* PRQA S 3673,0317,1294 ++ */ /* VL_QAC_Crypto */
Std_ReturnType Crypto_Md_Start(Crypto_Md_Context_t* ctx)
{
    Std_ReturnType result = E_OK;

    if ((ctx != NULL_PTR) && (ctx->md_info != NULL_PTR))
    {
        switch (ctx->md_info->algorithmfamily)
        {
        case CRYPTO_ALGOFAM_RIPEMD160:
#if (CRYPTO_ALGORITHMFAM_RIPEMD160 == STD_ON)
            Crypto_Ripemd160_Starts(ctx->md_ctx);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA1:
#if (CRYPTO_ALGORITHMFAM_SHA1 == STD_ON)
            Crypto_Sha1_Starts(ctx->md_ctx);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_224:
#if (CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON)
            Crypto_Sha256_Starts(ctx->md_ctx, 1);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_256:
#if (CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON)
            Crypto_Sha256_Starts(ctx->md_ctx, 0);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_384:
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
            Crypto_Sha512_Starts(ctx->md_ctx, 1);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_512:
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
            Crypto_Sha512_Starts(ctx->md_ctx, 0);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA3_224:
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
            Crypto_Sha3_Starts(ctx->md_ctx, CRYPTO_SHA3_224);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA3_256:
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
            Crypto_Sha3_Starts(ctx->md_ctx, CRYPTO_SHA3_256);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA3_384:
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
            Crypto_Sha3_Starts(ctx->md_ctx, CRYPTO_SHA3_384);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA3_512:
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
            Crypto_Sha3_Starts(ctx->md_ctx, CRYPTO_SHA3_512);
#endif
            break;
        case CRYPTO_ALGOFAM_SM3:
#if (CRYPTO_ALGORITHMFAM_SM3 == STD_ON)
            Crypto_Sm3_Start(ctx->md_ctx);
#endif
            break;
        default:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        }
    }
    return result;
}
/* PRQA S 3673,0317,1294 -- */

/******************************************************************************/
/*
 * Brief               This function feeds an input buffer into an ongoing
 *                     message-digest computation.
 *
 * Param-Name[in]      input: The buffer holding the input data.
 *                     ilen: The length of the input data.
 * Param-Name[in/out]  ctx: The generic message-digest context.
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Md_Update(Crypto_Md_Context_t* ctx, const unsigned char* input, uint32 ilen)
{
    Std_ReturnType result = E_NOT_OK;
    if ((ctx != NULL_PTR) && (ctx->md_info != NULL_PTR))
    {
        switch (ctx->md_info->algorithmfamily)
        {
        case CRYPTO_ALGOFAM_RIPEMD160:
#if (CRYPTO_ALGORITHMFAM_RIPEMD160 == STD_ON)
            Crypto_Ripemd160_Update(ctx->md_ctx, input, ilen);
            result = E_OK;
#endif
            break;
        case CRYPTO_ALGOFAM_SHA1:
#if (CRYPTO_ALGORITHMFAM_SHA1 == STD_ON)
            result = Crypto_Sha1_Update(ctx->md_ctx, input, ilen);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_224:
#if (CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON)
            result = Crypto_Sha256_Update(ctx->md_ctx, input, ilen);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_256:
#if (CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON)
            result = Crypto_Sha256_Update(ctx->md_ctx, input, ilen);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_384:
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
            result = Crypto_Sha512_Update(ctx->md_ctx, input, ilen);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_512:
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
            result = Crypto_Sha512_Update(ctx->md_ctx, input, ilen);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA3_224:
        case CRYPTO_ALGOFAM_SHA3_256:
        case CRYPTO_ALGOFAM_SHA3_384:
        case CRYPTO_ALGOFAM_SHA3_512:
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
            result = Crypto_Sha3_Update(ctx->md_ctx, input, ilen);
#endif
            break;

        case CRYPTO_ALGOFAM_SM3:
#if (CRYPTO_ALGORITHMFAM_SM3 == STD_ON)
            result = Crypto_Sm3_Update(ctx->md_ctx, input, ilen);
#endif
            break;
        default:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        }
    }
    return result;
}

/******************************************************************************/
/*
 * Brief               This function finishes the digest operation, and writes
 *                     the result to the output buffer.
 *
 * Param-Name[in]      ctx: The generic message-digest context.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The buffer for the generic message-digest checksum
 *                             result.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Md_Finish(Crypto_Md_Context_t* ctx, uint8* output)
{
    Std_ReturnType ret = E_NOT_OK;

    if ((ctx != NULL_PTR) && (ctx->md_info != NULL_PTR))
    {
        switch (ctx->md_info->algorithmfamily)
        {
        case CRYPTO_ALGOFAM_RIPEMD160:
#if (CRYPTO_ALGORITHMFAM_RIPEMD160 == STD_ON)
            Crypto_Ripemd160_Finish(ctx->md_ctx, output);
            ret = E_OK;
#endif
            break;
        case CRYPTO_ALGOFAM_SHA1:
#if (CRYPTO_ALGORITHMFAM_SHA1 == STD_ON)
            ret = Crypto_Sha1_Finish(ctx->md_ctx, output);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_224:
#if (CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON)
            ret = Crypto_Sha256_Finish(ctx->md_ctx, output);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_256:
#if (CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON)
            ret = Crypto_Sha256_Finish(ctx->md_ctx, output);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_384:
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
            ret = Crypto_Sha512_Finish(ctx->md_ctx, output);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_512:
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
            ret = Crypto_Sha512_Finish(ctx->md_ctx, output);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA3_224:
        case CRYPTO_ALGOFAM_SHA3_256:
        case CRYPTO_ALGOFAM_SHA3_384:
        case CRYPTO_ALGOFAM_SHA3_512:
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
            ret = Crypto_Sha3_Finish(ctx->md_ctx, output, ctx->md_info->size);
#endif
            break;

        case CRYPTO_ALGOFAM_SM3:
#if (CRYPTO_ALGORITHMFAM_SM3 == STD_ON)
            Crypto_Sm3_Finish(ctx->md_ctx, output);
#endif
            ret = E_OK;
            break;
        default:
            ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        }
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function calculates the message-digest of a buffer,
 *                     with respect to a configurable message-digest algorithm
 *                     in a single call.
 *
 * Param-Name[in]      md_info: This function calculates the message-digest of a
 *                              buffer, with respect to a configurable message-digest
 *                              algorithm in a single call.
 *                     input: The buffer holding the data.
 *                     ilen: The length of the input data.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The generic message-digest checksum result.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType
    Crypto_md(const Crypto_Md_Info_t* md_info, const unsigned char* input, uint32 ilen, unsigned char* output)
{
    Std_ReturnType ret = E_NOT_OK;
    if (md_info == NULL_PTR)
    {
        return E_NOT_OK;
    }
    switch (md_info->algorithmfamily)
    {
    case CRYPTO_ALGOFAM_RIPEMD160:
#if (CRYPTO_ALGORITHMFAM_RIPEMD160 == STD_ON)
        ret = Crypto_Ripemd160(input, ilen, output);
#endif
        break;
    case CRYPTO_ALGOFAM_SHA1:
#if (CRYPTO_ALGORITHMFAM_SHA1 == STD_ON)
        ret = Crypto_Sha1(input, ilen, output);
#endif
        break;
    case CRYPTO_ALGOFAM_SHA2_224:
#if (CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON)
        ret = Crypto_Sha256(input, ilen, output, true);
#endif
        break;
    case CRYPTO_ALGOFAM_SHA2_256:
#if (CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON)
        ret = Crypto_Sha256(input, ilen, output, false);
#endif
        break;
    case CRYPTO_ALGOFAM_SHA2_384:
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
        ret = Crypto_Sha512(input, ilen, output, true);
#endif
        break;
    case CRYPTO_ALGOFAM_SHA2_512:
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
        ret = Crypto_Sha512(input, ilen, output, false);
#endif
        break;
    case CRYPTO_ALGOFAM_SHA3_224:
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
        ret = Crypto_Sha3(CRYPTO_SHA3_224, input, ilen, output, md_info->size);
#endif
        break;
    case CRYPTO_ALGOFAM_SHA3_256:
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
        ret = Crypto_Sha3(CRYPTO_SHA3_256, input, ilen, output, md_info->size);
#endif
        break;
    case CRYPTO_ALGOFAM_SHA3_384:
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
        ret = Crypto_Sha3(CRYPTO_SHA3_384, input, ilen, output, md_info->size);
#endif
        break;
    case CRYPTO_ALGOFAM_SHA3_512:
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
        ret = Crypto_Sha3(CRYPTO_SHA3_512, input, ilen, output, md_info->size);
#endif
        break;

    default:
        ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
        break;
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function do the HASH Service for CSM.
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: algorithmMode configured in crypto,
 *                     algorithmfamily: familly configured in crypto,
 *                     operateMode: operateMode choose from modetype,
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
Std_ReturnType Crypto_62_Hash_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode)
{
    Std_ReturnType result = E_NOT_OK;
    (void)mode;
    if ((CRYPTO_OPERATIONMODE_START == operateMode) || (CRYPTO_OPERATIONMODE_UPDATE == operateMode))
    {
        result = E_OK;
    }
    else
    {
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_SHA1:
#if (CRYPTO_ALGORITHMFAM_SHA1 == STD_ON)
            result = Crypto_Sha1_Process(objectId);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_224:
#if (CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON)
            result = Crypto_Sha256_Process(objectId, TRUE);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_256:
#if (CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON)
            result = Crypto_Sha256_Process(objectId, FALSE);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_384:
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
            result = Crypto_Sha512_Process(objectId, TRUE);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_512:
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
            result = Crypto_Sha512_Process(objectId, FALSE);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_512_224:
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
            result = Crypto_Sha512_224_Process(objectId);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA2_512_256:
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
            result = Crypto_Sha512_256_Process(objectId);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA3_224:
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
            result = Crypto_Sha3_Process(objectId, CRYPTO_SHA3_224);

#endif
            break;
        case CRYPTO_ALGOFAM_SHA3_256:
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
            result = Crypto_Sha3_Process(objectId, CRYPTO_SHA3_256);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA3_384:
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
            result = Crypto_Sha3_Process(objectId, CRYPTO_SHA3_384);
#endif
            break;
        case CRYPTO_ALGOFAM_SHA3_512:
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
            result = Crypto_Sha3_Process(objectId, CRYPTO_SHA3_512);
#endif
            break;
        case CRYPTO_ALGOFAM_RIPEMD160:
#if (CRYPTO_ALGORITHMFAM_RIPEMD160 == STD_ON)
            result = Crypto_Ripemd160_Process(objectId);
#endif
            break;
        case CRYPTO_ALGOFAM_SHAKE128:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        case CRYPTO_ALGOFAM_SHAKE256:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
            /*RFC not support*/
        case CRYPTO_ALGOFAM_BLAKE_1_256:
        case CRYPTO_ALGOFAM_BLAKE_1_512:
        case CRYPTO_ALGOFAM_BLAKE_2s_512:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        case CRYPTO_ALGOFAM_BLAKE_2s_256:
#if (CRYPTO_ALGORITHMFAM_BLAKE_2s_256 == STD_ON)
            result = Crypto_Blake2s_Process(objectId);
#endif
            break;
        case CRYPTO_ALGOFAM_SM3:
#if (CRYPTO_ALGORITHMFAM_SM3 == STD_ON)
            result = Crypto_Sm3_Process(objectId);
#endif
            break;
        default:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        }
    }

    return result;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif

/* PRQA S 6050,6060,6070,6080,6010,6030,6040,0342,2743,2889,2784,1532 -- */
