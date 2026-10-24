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
 **  @file               : Crypto_62_Sm2.h
 **  @author             : Lanyan.wang
 **  @date               : 2025/07/16
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/
/* PRQA S 3207,1536 ++ */ /* VL_QAC_Crypto */

#ifndef CRYPTO_SM2_H
#define CRYPTO_SM2_H

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_SM2 == STD_ON)

#include "Crypto_62_Sm3.h"
#include "Platform_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ================================================ type definitions ================================================ */

/**
 * @brief The type representing a 256-bit big number in finite prime field.
 * @note The number is stored as a big-endian byte array for upper-layer output and network transmission
 */
typedef uint8 sm2_bn[32];

/**
 * @brief The type representing a 256-bit big number in finite prime field.
 * @note The number is stored in little-endian order and used for underlying calculations.
 */
typedef uint32 sm2_fp[8];

/**
 * @brief The type representing a 512-bit number to store the multiplication product of sm2_fp.
 */
typedef uint32 sm2_dfp[16];

/**
 * @brief The type representing a Non-Adjacent Form (NAF) for efficient scalar multiplication
 */
typedef sint8 sm2_naf[257];

typedef uint8 sm3_digest[32];

/**
 * @brief The structure representing a point of SM2.
 * @note The point is stored as two sm2_bn, i.e., the big-endian byte array and used for upper-layer output and network
 * transmission
 */
typedef struct SM2Point
{
    sm2_bn x; ///< x-coordinate of the point
    sm2_bn y; ///< y-coordinate of the point
} sm2_point;

typedef sm2_point sm2_pubkey;

typedef struct SM2PrivateKey
{
    sm2_bn     prikey; ///< Private key
    sm2_pubkey pubkey; ///< Public key
    sm2_fp     d;      ///< Private key in the present of fp
    sm2_fp     d_inv;  ///< d_ = (1 + d)^(-1)
} sm2_prikey;

typedef struct SM2_Signature
{
    sm2_bn r; ///< r value of the signature
    sm2_bn s; ///< s value of the signature
} sm2_signature;

typedef struct SM2SignContext
{
    const sm2_prikey* prikey; ///< Private key used for signing
#if (CRYPTO_ALGORITHMFAM_SM3 == STD_ON)
    Crypto_SM3_Context* sm3_ctx; ///< Hash context for SM3
#endif
    sm3_digest    dgst; ///< Message digest
    sm2_signature sig;  ///< SM2 signature
} sm2_sign_ctx;

typedef struct SM2VerifyContext
{
    const sm2_pubkey* pubkey; ///< Public key used for verification
#if (CRYPTO_ALGORITHMFAM_SM3 == STD_ON)
    Crypto_SM3_Context* sm3_ctx; ///< Hash context for SM3
#endif
    sm3_digest dgst; ///< Message digest
} sm2_verify_ctx;

typedef struct
{
    sm2_point  C1;   ///< Pointer to the ciphertext data C1
    sm3_digest C3;   ///< Pointer to the ciphertext data C3
    uint8*     C2;   ///< Pointer to the ciphertext data C2
    uint32     clen; ///< Length of the ciphertext
} sm2_ciphertext;

/**
 * @brief The structure representing an affine point on the elliptic curve.
 * @note This structure is used for underlying calculations.
 */
typedef struct SM2AffinePoint
{
    sm2_fp x; ///< x-coordinate of the affine point
    sm2_fp y; ///< y-coordinate of the affine point
} sm2_apoint;

/**
 * @brief The structure representing a jacobian point on the elliptic curve.
 * @note This structure is used for underlying calculations.
 */
typedef struct SM2JacobianPoint
{
    sm2_fp x; ///< x-coordinate of the jacobian point
    sm2_fp y; ///< y-coordinate of the jacobian point
    sm2_fp z; ///< z-coordinate of the jacobian point
} sm2_jpoint;

/* ===================================================== macros ===================================================== */

#define SM2_KEY_LEN (uint32)65U

// #define GET32(m)        ((uint32)(m)[0] << 24 | (uint32)(m)[1] << 16 | (uint32)(m)[2] <<  8 | (uint32)(m)[3])
#define GET32(m) ((uint32)(m)[0] << 24 | (uint32)(m)[1] << 16 | (uint32)(m)[2] << 8 | (uint32)(m)[3])
#define PUT32(ptr, a)       \
    (ptr)[0] = ((a) >> 24); \
    (ptr)[1] = ((a) >> 16); \
    (ptr)[2] = ((a) >> 8);  \
    (ptr)[3] = (a)
void bytes_from_hexstr(void* b, const char* s, uint32 nbytes);

#define Sign_sequence_max_size 75
#define Sm3_Digest_Len         32

#define Large_lookup_table_allowed 0

#if Large_lookup_table_allowed == 1
#define CRYPTO_62_START_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"
extern const sm2_apoint sm2_pct[32][256];
#define CRYPTO_62_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"
#else
#define CRYPTO_62_START_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"
extern const sm2_apoint fpt[52];
#define CRYPTO_62_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"
#endif

// The constant value representing zero.
#define CRYPTO_62_START_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"
static const sm2_fp sm2_zero = {0};
#define CRYPTO_62_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"

// The constant value representing one.
#define CRYPTO_62_START_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"
static const sm2_fp sm2_one = {1};
#define CRYPTO_62_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"

// The constant value representing the prime number p.
#define CRYPTO_62_START_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"
static const sm2_fp sm2_p =
    {0xFFFFFFFFU, 0xFFFFFFFFU, 0x00000000U, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFEU};
#define CRYPTO_62_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"

// The constant value representing the order n of the base point G.
#define CRYPTO_62_START_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"
static const sm2_fp sm2_n =
    {0x39D54123U, 0x53BBF409U, 0x21C6052BU, 0x7203DF6BU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFEU};
#define CRYPTO_62_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"

// The constant value representing the coefficient a of the elliptic curve.
#define CRYPTO_62_START_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"
static const sm2_fp sm2_a =
    {0xFFFFFFFCU, 0xFFFFFFFFU, 0x00000000U, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFEU};
#define CRYPTO_62_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"

// The constant value representing the coefficient b of the elliptic curve.
#define CRYPTO_62_START_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"
static const sm2_fp sm2_b =
    {0x4D940E93U, 0xDDBCBD41U, 0x15AB8F92U, 0xF39789F5U, 0xCF6509A7U, 0x4D5A9E4BU, 0x9D9F5E34U, 0x28E9FA9EU};
#define CRYPTO_62_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"

// The constant value representing the value (p + 1) / 2.
#define CRYPTO_62_START_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"
static const sm2_fp sm2_s =
    {0x00000000U, 0x80000000U, 0x80000000U, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0x7FFFFFFFU, 0x7FFFFFFFU};
#define CRYPTO_62_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"

/* ========================================== internal function definitions ========================================= */
/**
 * @brief Convert an SM2 ciphertext to DER-encoded format.
 * @param ct The ciphertext to be converted.
 * @param dst The output buffer to store the DER-encoded ciphertext.
 * @param dstl The length of the output buffer.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_ciphertext_to_der(const sm2_ciphertext* ct, uint8** dst, uint32* dstl);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Adds two Jacobian points.
 * @param [out] r Pointer to the resulting SM2JacobianPoint.
 * @param [in] p Pointer to the first SM2JacobianPoint.
 * @param [in] q Pointer to the second SM2JacobianPoint.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_jpoint_add(sm2_jpoint* r, const sm2_jpoint* p, const sm2_jpoint* q);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/******************************************************************************/
/*
 * Brief               This function do the SM2 Encrypt process and output cipher text.
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: AlgorithmMode configured in crypto,
 *                     secondaryFam: familly for the hash
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    Crypto_SM2_EncryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function do the SM2 Decrypt process and output cipher text.
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: AlgorithmMode configured in crypto,
 *                     secondaryFam: familly for the hash
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    Crypto_SM2_DecryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function do the SM2 signature generate process and output signature.
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: AlgorithmMode configured in crypto,
 *                     secondaryFam: familly for the hash
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_Sm2_SignatureGenerate_Process(
    uint32                     objectId,
    Crypto_AlgorithmModeType   mode,
    Crypto_AlgorithmFamilyType secondaryFam);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function do the SM2 signature verify process and output the result.
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: AlgorithmMode configured in crypto,
 *                     secondaryFam: familly for the hash
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_Sm2_SignatureVerifyProcess(
    uint32                     objectId,
    Crypto_AlgorithmModeType   mode,
    Crypto_AlgorithmFamilyType secondaryFam);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_ALGORITHMFAM_SM2 == STD_ON */

#endif

/* PRQA S 3207,1536 -- */
