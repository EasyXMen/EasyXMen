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
 **  @file               : Crypto_62_Rsa.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_RSA_H
#define CRYPTO_RSA_H

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_RSA == STD_ON)
#include <stdint.h>

#include "Crypto_62_Bignum.h"
#include "Crypto_62_Asn1.h"

#include "Crypto_62_CtrDrbg.h"
#include "Crypto_62_Random.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#define CRYPTO_RSA_SIGN  1 /**< Identifier for RSA signature operations. */
#define CRYPTO_RSA_CRYPT 2 /**< Identifier for RSA encryption and decryption operations. */

#define CRYPTO_RSA_SALT_LEN_ANY (-1)
#define CRYPTO_RSA_NO_CRT       1

/*
 * Convert between bits/chars and number of limbs
 * Divide first in order to avoid potential overflows
 */
#define BITS_TO_LIMBS(i)  ((i) / biL + ((i) % biL != 0))
#define CHARS_TO_LIMBS(i) ((i) / ciL + ((i) % ciL != 0))

/*
 * Digest algorithms
 */
#define CRYPTO_OID_DIGEST_ALG_MD5                                                                       \
    "\x2a\x86\xf7\x0d\x02\x05" /**< id-CRYPTO_md5 OBJECT IDENTIFIER ::= { iso(1) member-body(2) us(840) \
                                  rsadsi(113549) digestAlgorithm(2) 5 } */
#define CRYPTO_OID_DIGEST_ALG_SHA1                                                                               \
    "\x2b\x0e\x03\x02\x1a" /**< id-CRYPTO_sha1 OBJECT IDENTIFIER ::= { iso(1) identified-organization(3) oiw(14) \
                  secsig(3) algorithms(2) 26 } */
#define CRYPTO_OID_DIGEST_ALG_SHA224                                                                                 \
    "\x03\x04\x02\x04" /**< id-sha224 OBJECT IDENTIFIER ::= { joint-iso-itu-t(2) country(16) us(840) organization(1) \
                          gov(101) csor(3) nistalgorithm(4) hashalgs(2) 4 } */
#define CRYPTO_OID_DIGEST_ALG_SHA256                                                                        \
    "\x60\x86\x48\x01\x65\x03\x04\x02\x01" /**< id-CRYPTO_sha256 OBJECT IDENTIFIER ::= { joint-iso-itu-t(2) \
                              country(16) us(840) organization(1) gov(101) csor(3) nistalgorithm(4) hashalgs(2) 1 } */

#define CRYPTO_OID_DIGEST_ALG_SHA384                                                                                 \
    "\x03\x04\x02\x02" /**< id-sha384 OBJECT IDENTIFIER ::= { joint-iso-itu-t(2) country(16) us(840) organization(1) \
                          gov(101) csor(3) nistalgorithm(4) hashalgs(2) 2 } */

#define CRYPTO_OID_DIGEST_ALG_SHA512                                                                        \
    "\x03\x04\x02\x03" /**< id-CRYPTO_sha512 OBJECT IDENTIFIER ::= { joint-iso-itu-t(2) country(16) us(840) \
                          organization(1) gov(101) csor(3) nistalgorithm(4) hashalgs(2) 3 } */

#define CRYPTO_OID_DIGEST_ALG_RIPEMD160                                                                             \
    "\x2b\x24\x03\x02\x01" /**< id-ripemd160 OBJECT IDENTIFIER :: { iso(1) identified-organization(3) teletrust(36) \
                              algorithm(3) hashAlgorithm(2) ripemd160(1) } */

#define CRYPTO_OID_DIGEST_ALG_SHA3_224                                                                                 \
    "\x03\x04\x02\x07" /**< id-sha3-224 OBJECT IDENTIFIER ::= { joint-iso-itu-t(2) country(16) us(840) organization(1) \
                          gov(101) csor(3) nistAlgorithms(4) hashalgs(2) sha3-224(7) } */

#define CRYPTO_OID_DIGEST_ALG_SHA3_256                                                                                 \
    "\x03\x04\x02\x08" /**< id-sha3-256 OBJECT IDENTIFIER ::= { joint-iso-itu-t(2) country(16) us(840) organization(1) \
                          gov(101) csor(3) nistAlgorithms(4) hashalgs(2) sha3-256(8) } */

#define CRYPTO_OID_DIGEST_ALG_SHA3_384                                                                                 \
    "\x03\x04\x02\x09" /**< id-sha3-384 OBJECT IDENTIFIER ::= { joint-iso-itu-t(2) country(16) us(840) organization(1) \
                          gov(101) csor(3) nistAlgorithms(4) hashalgs(2) sha3-384(9) } */

#define CRYPTO_OID_DIGEST_ALG_SHA3_512                                                                                 \
    "\x03\x04\x02\x0a" /**< id-sha3-512 OBJECT IDENTIFIER ::= { joint-iso-itu-t(2) country(16) us(840) organization(1) \
                          gov(101) csor(3) nistAlgorithms(4) hashalgs(2) sha3-512(10) } */

#define CRYPTO_OID_HMAC_SHA1                                                                                      \
    "\x2a\x86\x48\x86\xf7\x0d\x02\x07" /**< id-hmacWithSHA1 OBJECT IDENTIFIER ::= { iso(1) member-body(2) us(840) \
                                          rsadsi(113549) digestAlgorithm(2) 7 } */

#define CRYPTO_OID_HMAC_SHA224                                                                                      \
    "\x2a\x86\x48\x86\xf7\x0d\x02\x08" /**< id-hmacWithSHA224 OBJECT IDENTIFIER ::= { iso(1) member-body(2) us(840) \
                                          rsadsi(113549) digestAlgorithm(2) 8 } */

#define CRYPTO_OID_HMAC_SHA256                                                                                      \
    "\x2a\x86\x48\x86\xf7\x0d\x02\x09" /**< id-hmacWithSHA256 OBJECT IDENTIFIER ::= { iso(1) member-body(2) us(840) \
                                          rsadsi(113549) digestAlgorithm(2) 9 } */

#define CRYPTO_OID_HMAC_SHA384                                                                                      \
    "\x2a\x86\x48\x86\xf7\x0d\x02\x0A" /**< id-hmacWithSHA384 OBJECT IDENTIFIER ::= { iso(1) member-body(2) us(840) \
                                          rsadsi(113549) digestAlgorithm(2) 10 } */

#define CRYPTO_OID_HMAC_SHA512                                                                                      \
    "\x2a\x86\x48\x86\xf7\x0d\x02\x0B" /**< id-hmacWithSHA512 OBJECT IDENTIFIER ::= { iso(1) member-body(2) us(840) \
                                          rsadsi(113549) digestAlgorithm(2) 11 } */

#define CRYPTO_OID_HMAC_SHA3_224                                                                               \
    "\x03\x04\x02\x0d" /**< id-hmacWithSHA3-512 OBJECT IDENTIFIER ::= { joint-iso-itu-t(2) country(16) us(840) \
                          organization(1) gov(101) csor(3) nistAlgorithms(4) hashalgs(2) hmacWithSHA3-224(13) } */

#define CRYPTO_OID_HMAC_SHA3_256                                                                               \
    "\x03\x04\x02\x0e" /**< id-hmacWithSHA3-512 OBJECT IDENTIFIER ::= { joint-iso-itu-t(2) country(16) us(840) \
                          organization(1) gov(101) csor(3) nistAlgorithms(4) hashalgs(2) hmacWithSHA3-256(14) } */

#define CRYPTO_OID_HMAC_SHA3_384                                                                               \
    "\x03\x04\x02\x0f" /**< id-hmacWithSHA3-512 OBJECT IDENTIFIER ::= { joint-iso-itu-t(2) country(16) us(840) \
                          organization(1) gov(101) csor(3) nistAlgorithms(4) hashalgs(2) hmacWithSHA3-384(15) } */

#define CRYPTO_OID_HMAC_SHA3_512                                                                               \
    "\x03\x04\x02\x10" /**< id-hmacWithSHA3-512 OBJECT IDENTIFIER ::= { joint-iso-itu-t(2) country(16) us(840) \
                          organization(1) gov(101) csor(3) nistAlgorithms(4) hashalgs(2) hmacWithSHA3-512(16) } */

#define CRYPTO_OID_HMAC_RIPEMD160                                                                                     \
    "\x2b\x06\x01\x05\x05\x08\x01\x04" /**< id-hmacWithSHA1 OBJECT IDENTIFIER ::= {iso(1)                             \
                                          iso-identified-organization(3) dod(6) internet(1) security(5) mechanisms(5) \
                                          ipsec(8) isakmpOakley(1) hmacRIPEMD160(4)} */

#define CRYPTO_RSA_GEN_KEY_MIN_BITS 1024

/* ================================================ type definitions ================================================ */

/** @brief ASN.1 buffer structure. */
typedef struct Crypto_asn1_buf_tag
{
    int    tag; /**< ASN1 type, e.g. TLS_ASN1_UTF8_STRING. */
    uint32 len; /**< ASN1 length, in octets. */
    uint8* p;   /**< ASN1 data, e.g. in ASCII. */
} Crypto_asn1_buf;

/** @brief ASN.1 Sequence Item Structure*/
typedef struct Crypto_asn1_sequence_tag
{
    Crypto_asn1_buf                  buf;  /**< Buffer containing the given ASN.1 item. */
    struct Crypto_asn1_sequence_tag* next; /**< The next entry in the sequence. */
} Crypto_asn1_sequence;

/** @brief RSA Context Structure*/
typedef struct
{
    int ver;    /*!<  Reserved for internal purposes.
                 *    Do not set this field in application
                 *    code. Its meaning might change without
                 *    notice. */
    uint32 len; /*!<  The size of \p N in Bytes. */

    Crypto_mpi N; /*!<  The public modulus. */
    Crypto_mpi E; /*!<  The public exponent. */

    Crypto_mpi D;  /*!<  The private exponent. */
    Crypto_mpi P;  /*!<  The first prime factor. */
    Crypto_mpi Q;  /*!<  The second prime factor. */
    Crypto_mpi RN; /*!<  cached <code>R^2 mod N</code>. */

#if !defined(CRYPTO_RSA_NO_CRT)
    Crypto_mpi DP; /*!<  <code>D % (P - 1)</code>. */
    Crypto_mpi DQ; /*!<  <code>D % (Q - 1)</code>. */
    Crypto_mpi QP; /*!<  <code>1 / (Q % P)</code>. */
    Crypto_mpi RP; /*!<  cached <code>R^2 mod P</code>. */
    Crypto_mpi RQ; /*!<  cached <code>R^2 mod Q</code>. */
#endif
    Crypto_mpi Vi; /*!<  The cached blinding value. */
    Crypto_mpi Vf; /*!<  The cached un-blinding value. */

    Crypto_AlgorithmModeType padding;   /*!< Selects padding mode:
                                        #CRYPTO_RSA_PKCS_V15 for 1.5 padding and
                                        #CRYPTO_RSA_PKCS_V21 for OAEP or PSS. */
    Crypto_AlgorithmFamilyType hash_id; /*!< Hash identifier of Crypto_AlgorithmFamilyType type,
                                     as specified in md.h for use in the MGF
                                     mask generating function used in the
                                     EME-OAEP and EMSA-PSS encodings. */

} Crypto_rsa_context;

/** @brief Base OID Descriptor Structure*/
typedef struct
{
    const char* asn1;        /*!< OID ASN.1 representation       */
    uint32      asn1_len;    /*!< length of asn1                 */
    const char* name;        /*!< official name (e.g. from RFC)  */
    const char* description; /*!< human friendly description     */

} Crypto_oid_descriptor_t;

/** @brief Digest Algorithm Structure*/
typedef struct
{
    Crypto_oid_descriptor_t    descriptor;
    Crypto_AlgorithmFamilyType md_alg;
} Crypto_oid_md_alg_t;
/** @brief ASN.1 Sequence Header Structure*/
typedef struct
{
    int                   tag;
    Crypto_asn1_sequence* cur;
} Crypto_asn1_get_sequence_of_cb_ctx_t;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief   Calculates the number of working limbs required for Montgomery multiplication.
 *
 * @param[in] AN_limbs  Number of limbs in the modulus N.
 *
 * @return  Number of working limbs required.
 */
CRYPTO_62_LOCAL inline uint32 Crypto_mpi_core_montmul_working_limbs(uint32 AN_limbs)
{
    return (2u * AN_limbs) + 1u;
}

/**
 * @brief   Processes RSA encryption for a given object ID, mode, and secondary family.
 *
 * @param[in] objectId         The object ID for the RSA encryption process.
 * @param[in] mode             The algorithm mode type.
 * @param[in] secondaryFam     The secondary algorithm family type.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    Crypto_Rsa_EncryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Processes RSA decryption for a given object ID, mode, and secondary family.
 *
 * @param[in] objectId         The object ID for the RSA decryption process.
 * @param[in] mode             The algorithm mode type.
 * @param[in] secondaryFam     The secondary algorithm family type.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    Crypto_Rsa_DecryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Generates an RSA signature for a given object ID, mode, and secondary family.
 *
 * @param[in] objectId         The object ID for the RSA signature generation process.
 * @param[in] mode             The algorithm mode type.
 * @param[in] secondaryFam     The secondary algorithm family type.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_Rsa_SignatureGenerate_Process(
    uint32                     objectId,
    Crypto_AlgorithmModeType   mode,
    Crypto_AlgorithmFamilyType secondaryFam);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Verifies an RSA signature for a given object ID, mode, and secondary family.
 *
 * @param[in] objectId         The object ID for the RSA signature verification process.
 * @param[in] mode             The algorithm mode type.
 * @param[in] secondaryFam     The secondary algorithm family type.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_Rsa_SignatureVerifyProcess(
    uint32                     objectId,
    Crypto_AlgorithmModeType   mode,
    Crypto_AlgorithmFamilyType secondaryFam);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Initializes an RSA context.
 *
 * @param[in,out] ctx            Pointer to the RSA context to be initialized.
 * @param[in]     mode           The algorithm mode type.
 * @param[in]     secondaryFam   The secondary algorithm family type.
 *
 * @return  None.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void Crypto_rsa_init(Crypto_rsa_context* ctx, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Sets the padding mode and hash algorithm for an RSA context.
 *
 * @param[in,out] ctx            Pointer to the RSA context.
 * @param[in]     padding        The padding mode to set.
 * @param[in]     hash_id        The hash algorithm family type.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_set_padding(Crypto_rsa_context* ctx, int padding, Crypto_AlgorithmFamilyType hash_id);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Retrieves the current padding mode from an RSA context.
 *
 * @param[in] ctx  Pointer to the RSA context.
 *
 * @return  The current padding mode.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_get_padding_mode(const Crypto_rsa_context* ctx);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Retrieves the current hash algorithm from an RSA context.
 *
 * @param[in] ctx  Pointer to the RSA context.
 *
 * @return  The current hash algorithm family type.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_get_md_alg(const Crypto_rsa_context* ctx);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Imports RSA parameters into an RSA context.
 *
 * @param[in,out] ctx  Pointer to the RSA context.
 * @param[in]     N    Pointer to the public modulus.
 * @param[in]     P    Pointer to the first prime factor.
 * @param[in]     Q    Pointer to the second prime factor.
 * @param[in]     D    Pointer to the private exponent.
 * @param[in]     E    Pointer to the public exponent.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_import(
    Crypto_rsa_context* ctx,
    const Crypto_mpi*   N,
    const Crypto_mpi*   P,
    const Crypto_mpi*   Q,
    const Crypto_mpi*   D,
    const Crypto_mpi*   E);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Imports raw RSA parameters into an RSA context.
 *
 * @param[in,out] ctx    Pointer to the RSA context.
 * @param[in]     N      Pointer to the public modulus.
 * @param[in]     N_len  Length of the public modulus in bytes.
 * @param[in]     P      Pointer to the first prime factor.
 * @param[in]     P_len  Length of the first prime factor in bytes.
 * @param[in]     Q      Pointer to the second prime factor.
 * @param[in]     Q_len  Length of the second prime factor in bytes.
 * @param[in]     D      Pointer to the private exponent.
 * @param[in]     D_len  Length of the private exponent in bytes.
 * @param[in]     E      Pointer to the public exponent.
 * @param[in]     E_len  Length of the public exponent in bytes.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_import_raw(
    Crypto_rsa_context*  ctx,
    const unsigned char* N,
    uint32               N_len,
    const unsigned char* P,
    uint32               P_len,
    const unsigned char* Q,
    uint32               Q_len,
    const unsigned char* D,
    uint32               D_len,
    const unsigned char* E,
    uint32               E_len);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Completes the initialization of an RSA context.
 *
 * This function finalizes the setup of an RSA context, ensuring that all necessary
 * parameters are correctly initialized and validated.
 *
 * @param[in,out] ctx  Pointer to the RSA context to be completed.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_complete(Crypto_rsa_context* ctx);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Exports RSA parameters from an RSA context.
 *
 * This function retrieves the RSA parameters (N, P, Q, D, E) from the provided RSA context
 * and stores them in the provided MPI structures.
 *
 * @param[in]  ctx  Pointer to the RSA context.
 * @param[out] N    Pointer to the MPI structure to store the public modulus.
 * @param[out] P    Pointer to the MPI structure to store the first prime factor.
 * @param[out] Q    Pointer to the MPI structure to store the second prime factor.
 * @param[out] D    Pointer to the MPI structure to store the private exponent.
 * @param[out] E    Pointer to the MPI structure to store the public exponent.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_export(
    const Crypto_rsa_context* ctx,
    Crypto_mpi*               N,
    Crypto_mpi*               P,
    Crypto_mpi*               Q,
    Crypto_mpi*               D,
    Crypto_mpi*               E);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Exports raw RSA parameters from an RSA context.
 *
 * This function retrieves the RSA parameters (N, P, Q, D, E) from the provided RSA context
 * and stores them in the provided raw byte arrays.
 *
 * @param[in]  ctx    Pointer to the RSA context.
 * @param[out] N      Pointer to the byte array to store the public modulus.
 * @param[in]  N_len  Length of the byte array for the public modulus.
 * @param[out] P      Pointer to the byte array to store the first prime factor.
 * @param[in]  P_len  Length of the byte array for the first prime factor.
 * @param[out] Q      Pointer to the byte array to store the second prime factor.
 * @param[in]  Q_len  Length of the byte array for the second prime factor.
 * @param[out] D      Pointer to the byte array to store the private exponent.
 * @param[in]  D_len  Length of the byte array for the private exponent.
 * @param[out] E      Pointer to the byte array to store the public exponent.
 * @param[in]  E_len  Length of the byte array for the public exponent.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_export_raw(
    const Crypto_rsa_context* ctx,
    unsigned char*            N,
    uint32                    N_len,
    unsigned char*            P,
    uint32                    P_len,
    unsigned char*            Q,
    uint32                    Q_len,
    unsigned char*            D,
    uint32                    D_len,
    unsigned char*            E,
    uint32                    E_len);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Exports the CRT parameters from an RSA context.
 *
 * This function retrieves the Chinese Remainder Theorem (CRT) parameters (DP, DQ, QP)
 * from the provided RSA context and stores them in the provided MPI structures.
 *
 * @param[in]  ctx  Pointer to the RSA context.
 * @param[out] DP   Pointer to the MPI structure to store <code>D % (P - 1)</code>.
 * @param[out] DQ   Pointer to the MPI structure to store <code>D % (Q - 1)</code>.
 * @param[out] QP   Pointer to the MPI structure to store <code>1 / (Q % P)</code>.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_export_crt(const Crypto_rsa_context* ctx, Crypto_mpi* DP, Crypto_mpi* DQ, Crypto_mpi* QP);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Retrieves the bit length of the RSA modulus.
 *
 * This function calculates and returns the bit length of the RSA modulus (N) from the provided RSA context.
 *
 * @param[in] ctx  Pointer to the RSA context.
 *
 * @return  The bit length of the RSA modulus.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
uint32 Crypto_rsa_get_bitlen(const Crypto_rsa_context* ctx);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/**
 * @brief   Retrieves the length of the RSA modulus in bytes.
 *
 * This function returns the length of the RSA modulus (N) in bytes from the provided RSA context.
 *
 * @param[in] ctx  Pointer to the RSA context.
 *
 * @return  The length of the RSA modulus in bytes.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
uint32 Crypto_rsa_get_len(const Crypto_rsa_context* ctx);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/**
 * @brief   Generates an RSA key pair.
 *
 * This function generates a new RSA key pair based on the specified number of bits and public exponent.
 * It uses the provided random number generator function to ensure the randomness of the generated keys.
 *
 * @param[in,out] ctx      Pointer to the RSA context where the generated key pair will be stored.
 * @param[in]     f_rng    Pointer to the random number generator function.
 * @param[in]     p_rng    Pointer to the random number generator context.
 * @param[in]     nbits    Number of bits for the RSA modulus.
 * @param[in]     exponent Public exponent value.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
int Crypto_rsa_gen_key(
    Crypto_rsa_context* ctx,
    /* PRQA S 1336 ++ */ /* VL_Crypto_62_1336 */
    /* PRQA S 3672 ++ */ /* VL_Crypto_62_3672 */
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    /* PRQA S 3672 -- */ /* VL_Crypto_62_3672 */
    /* PRQA S 1336 -- */ /* VL_Crypto_62_1336 */
    void*        p_rng,
    unsigned int nbits,
    int          exponent);
/**
 * @brief   Checks the validity of an RSA public key.
 *
 * This function verifies the correctness of the RSA public key stored in the provided RSA context.
 *
 * @param[in] ctx  Pointer to the RSA context containing the public key.
 *
 * @return  Status of the operation.
 *          - 0: Public key is valid.
 *          - Non-zero: Public key is invalid.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_check_pubkey(const Crypto_rsa_context* ctx);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/**
 * @brief   Checks the validity of an RSA private key.
 *
 * This function verifies the correctness of the RSA private key stored in the provided RSA context.
 *
 * @param[in] ctx  Pointer to the RSA context containing the private key.
 *
 * @return  Status of the operation.
 *          - 0: Private key is valid.
 *          - Non-zero: Private key is invalid.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_check_privkey(const Crypto_rsa_context* ctx);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/**
 * @brief   Checks the consistency between an RSA public and private key pair.
 *
 * This function verifies that the provided RSA public and private key pairs are consistent with each other.
 *
 * @param[in] pub  Pointer to the RSA context containing the public key.
 * @param[in] prv  Pointer to the RSA context containing the private key.
 *
 * @return  Status of the operation.
 *          - 0: Key pair is consistent.
 *          - Non-zero: Key pair is inconsistent.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_check_pub_priv(const Crypto_rsa_context* pub, const Crypto_rsa_context* prv);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/**
 * @brief   Performs RSA public key encryption or signature verification.
 *
 * This function uses the RSA public key stored in the provided RSA context to perform encryption or signature
 * verification.
 *
 * @param[in,out] ctx    Pointer to the RSA context containing the public key.
 * @param[in]     input  Pointer to the input data to be encrypted or verified.
 * @param[out]    output Pointer to the buffer where the encrypted data or verification result will be stored.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_public(Crypto_rsa_context* ctx, const unsigned char* input, unsigned char* output);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/**
 * @brief   Performs RSA private key decryption or signature generation.
 *
 * This function uses the RSA private key stored in the provided RSA context to perform decryption or signature
 * generation. It uses the provided random number generator function to ensure the randomness of the generated
 * signatures.
 *
 * @param[in,out] ctx    Pointer to the RSA context containing the private key.
 * @param[in]     f_rng  Pointer to the random number generator function.
 * @param[in]     p_rng  Pointer to the random number generator context.
 * @param[in]     input  Pointer to the input data to be decrypted or signed.
 * @param[out]    output Pointer to the buffer where the decrypted data or signature will be stored.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
int Crypto_rsa_private(
    Crypto_rsa_context* ctx,
    /* PRQA S 1336,3672 ++ */ /* VL_Crypto_62_1336 */
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    /* PRQA S 1336,3672 -- */ /* VL_Crypto_62_1336 */
    void*                p_rng,
    const unsigned char* input,
    unsigned char*       output);

/**
 * @brief   Encrypts data using the PKCS#1 v1.5 scheme.
 *
 * This function encrypts data using the RSAES-PKCS1-v1_5 scheme. It uses the public key stored in the provided RSA
 * context. The encrypted data is written to the output buffer.
 *
 * @param[in]  ctx              Pointer to the RSA context containing the public key.
 * @param[in]  f_rng            Pointer to the random number generator function (not used in this scheme but included
 * for consistency).
 * @param[in]  p_rng            Pointer to the random number generator context (not used in this scheme but included for
 * consistency).
 * @param[in]  ilen             Length of the input data in bytes.
 * @param[in]  input            Pointer to the input data to be encrypted.
 * @param[out] output           Pointer to the buffer where the encrypted data will be stored.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
int Crypto_rsa_pkcs1_encrypt(
    Crypto_rsa_context* ctx,
    /* PRQA S 1336 ++ */ /* VL_Crypto_62_1336 */
    /* PRQA S 3672 ++ */ /* VL_Crypto_62_3672 */
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    /* PRQA S 3672 -- */ /* VL_Crypto_62_3672 */
    /* PRQA S 1336 -- */ /* VL_Crypto_62_1336 */
    void*                p_rng,
    uint32               ilen,
    const unsigned char* input,
    unsigned char*       output);

/**
 * @brief   Encrypts data using the RSAES-PKCS1-v1_5 scheme.
 *
 * This function encrypts data using the RSAES-PKCS1-v1_5 scheme. It uses the public key stored in the provided RSA
 * context. The encrypted data is written to the output buffer.
 *
 * @param[in]  ctx              Pointer to the RSA context containing the public key.
 * @param[in]  f_rng            Pointer to the random number generator function (not used in this scheme but included
 * for consistency).
 * @param[in]  p_rng            Pointer to the random number generator context (not used in this scheme but included for
 * consistency).
 * @param[in]  ilen             Length of the input data in bytes.
 * @param[in]  input            Pointer to the input data to be encrypted.
 * @param[out] output           Pointer to the buffer where the encrypted data will be stored.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
int Crypto_rsa_rsaes_pkcs1_v15_encrypt(
    Crypto_rsa_context* ctx,
    /* PRQA S 1336 ++ */ /* VL_Crypto_62_1336 */
    /* PRQA S 3672 ++ */ /* VL_Crypto_62_3672 */
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    /* PRQA S 3672 -- */ /* VL_Crypto_62_3672 */
    /* PRQA S 1336 -- */ /* VL_Crypto_62_1336 */
    void*                p_rng,
    uint32               ilen,
    const unsigned char* input,
    unsigned char*       output);

/**
 * @brief   Encrypts data using the RSAES-OAEP scheme.
 *
 * This function encrypts data using the RSAES-OAEP (Optimal Asymmetric Encryption Padding) scheme.
 * It uses the public key stored in the provided RSA context. A label can optionally be provided to customize the
 * encryption process.
 *
 * @param[in]  ctx              Pointer to the RSA context containing the public key.
 * @param[in]  f_rng            Pointer to the random number generator function used for generating random padding.
 * @param[in]  p_rng            Pointer to the random number generator context.
 * @param[in]  label            Pointer to the optional label used during encryption.
 * @param[in]  label_len        Length of the label in bytes.
 * @param[in]  ilen             Length of the input data in bytes.
 * @param[in]  input            Pointer to the input data to be encrypted.
 * @param[out] output           Pointer to the buffer where the encrypted data will be stored.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
int Crypto_rsa_rsaes_oaep_encrypt(
    Crypto_rsa_context* ctx,
    /* PRQA S 1336 ++ */ /* VL_Crypto_62_1336 */
    /* PRQA S 3672 ++ */ /* VL_Crypto_62_3672 */
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    /* PRQA S 3672 -- */ /* VL_Crypto_62_3672 */
    /* PRQA S 1336 -- */ /* VL_Crypto_62_1336 */
    void*                p_rng,
    const unsigned char* label,
    uint32               label_len,
    uint32               ilen,
    const unsigned char* input,
    unsigned char*       output);

/**
 * @brief   Decrypts data using the PKCS#1 v1.5 scheme.
 *
 * This function decrypts data using the RSAES-PKCS1-v1_5 scheme. It uses the private key stored in the provided RSA
 * context. The decrypted data is written to the output buffer, and the length of the decrypted data is returned via the
 * `olen` parameter.
 *
 * @param[in]  ctx              Pointer to the RSA context containing the private key.
 * @param[in]  f_rng            Pointer to the random number generator function (not used in this scheme but included
 * for consistency).
 * @param[in]  p_rng            Pointer to the random number generator context (not used in this scheme but included for
 * consistency).
 * @param[out] olen             Pointer to a variable that will store the length of the decrypted data.
 * @param[in]  input            Pointer to the encrypted input data.
 * @param[in]  ilen             Length of the input data in bytes.
 * @param[out] output           Pointer to the buffer where the decrypted data will be stored.
 * @param[in]  output_max_len   Maximum length of the output buffer.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
int Crypto_rsa_pkcs1_decrypt(
    Crypto_rsa_context* ctx,
    /* PRQA S 1336 ++ */ /* VL_Crypto_62_1336 */
    /* PRQA S 3672 ++ */ /* VL_Crypto_62_3672 */
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    /* PRQA S 3672 -- */ /* VL_Crypto_62_3672 */
    /* PRQA S 1336 -- */ /* VL_Crypto_62_1336 */
    void*                p_rng,
    uint32*              olen,
    const unsigned char* input,
    unsigned char*       output,
    uint32               output_max_len);

/**
 * @brief   Decrypts data using the RSAES-PKCS1-v1_5 scheme.
 *
 * This function decrypts data using the RSAES-PKCS1-v1_5 scheme. It uses the private key stored in the provided RSA
 * context. The decrypted data is written to the output buffer, and the length of the decrypted data is returned via the
 * `olen` parameter.
 *
 * @param[in]  ctx              Pointer to the RSA context containing the private key.
 * @param[in]  f_rng            Pointer to the random number generator function (not used in this scheme but included
 * for consistency).
 * @param[in]  p_rng            Pointer to the random number generator context (not used in this scheme but included for
 * consistency).
 * @param[out] olen             Pointer to a variable that will store the length of the decrypted data.
 * @param[in]  input            Pointer to the encrypted input data.
 * @param[out] output           Pointer to the buffer where the decrypted data will be stored.
 * @param[in]  output_max_len   Maximum length of the output buffer.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
int Crypto_rsa_rsaes_pkcs1_v15_decrypt(
    Crypto_rsa_context* ctx,
    /* PRQA S 1336 ++ */ /* VL_Crypto_62_1336 */
    /* PRQA S 3672 ++ */ /* VL_Crypto_62_3672 */
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    /* PRQA S 3672 -- */ /* VL_Crypto_62_3672 */
    /* PRQA S 1336 -- */ /* VL_Crypto_62_1336 */
    void*                p_rng,
    uint32*              olen,
    const unsigned char* input,
    unsigned char*       output,
    uint32               output_max_len);

/**
 * @brief   Decrypts data using the RSAES-OAEP scheme.
 *
 * This function decrypts data using the RSAES-OAEP scheme. It uses the private key stored in the provided RSA context.
 * The decrypted data is written to the output buffer, and the length of the decrypted data is returned via the `olen`
 * parameter. A label can optionally be provided to customize the decryption process.
 *
 * @param[in]  ctx              Pointer to the RSA context containing the private key.
 * @param[in]  f_rng            Pointer to the random number generator function (not used in this scheme but included
 * for consistency).
 * @param[in]  p_rng            Pointer to the random number generator context (not used in this scheme but included for
 * consistency).
 * @param[in]  label            Pointer to the optional label used during encryption.
 * @param[in]  label_len        Length of the label in bytes.
 * @param[out] olen             Pointer to a variable that will store the length of the decrypted data.
 * @param[in]  input            Pointer to the encrypted input data.
 * @param[out] output           Pointer to the buffer where the decrypted data will be stored.
 * @param[in]  output_max_len   Maximum length of the output buffer.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
int Crypto_rsa_rsaes_oaep_decrypt(
    Crypto_rsa_context* ctx,
    /* PRQA S 1336 ++ */ /* VL_Crypto_62_1336 */
    /* PRQA S 3672 ++ */ /* VL_Crypto_62_3672 */
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    /* PRQA S 3672 -- */ /* VL_Crypto_62_3672 */
    /* PRQA S 1336 -- */ /* VL_Crypto_62_1336 */
    void*                p_rng,
    const unsigned char* label,
    uint32               label_len,
    uint32*              olen,
    const unsigned char* input,
    unsigned char*       output,
    uint32               output_max_len);

/**
 * @brief   Generates an RSA signature using the PKCS#1 v1.5 scheme.
 *
 * This function generates an RSA signature using the PKCS#1 v1.5 scheme.
 * It uses the provided random number generator function to ensure the randomness of the padding.
 *
 * @param[in] ctx              Pointer to the RSA context containing the private key.
 * @param[in] f_rng            Pointer to the random number generator function.
 * @param[in] p_rng            Pointer to the random number generator context.
 * @param[in] md_alg           Hash algorithm family type used for the message.
 * @param[in] hashlen          Length of the hash in bytes.
 * @param[in] hash             Pointer to the hash of the message.
 * @param[out] sig             Pointer to the buffer where the generated signature will be stored.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
int Crypto_rsa_pkcs1_sign(
    Crypto_rsa_context* ctx,
    /* PRQA S 1336 ++ */ /* VL_Crypto_62_1336 */
    /* PRQA S 3672 ++ */ /* VL_Crypto_62_3672 */
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    /* PRQA S 3672 -- */ /* VL_Crypto_62_3672 */
    /* PRQA S 1336 -- */ /* VL_Crypto_62_1336 */
    void*                      p_rng,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    unsigned char*             sig);

/**
 * @brief   Generates an RSA signature using the RSASSA-PKCS1-v1_5 scheme.
 *
 * This function generates an RSA signature using the RSASSA-PKCS1-v1_5 scheme.
 * It uses the provided random number generator function to ensure the randomness of the padding.
 *
 * @param[in] ctx              Pointer to the RSA context containing the private key.
 * @param[in] f_rng            Pointer to the random number generator function.
 * @param[in] p_rng            Pointer to the random number generator context.
 * @param[in] md_alg           Hash algorithm family type used for the message.
 * @param[in] hashlen          Length of the hash in bytes.
 * @param[in] hash             Pointer to the hash of the message.
 * @param[out] sig             Pointer to the buffer where the generated signature will be stored.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
int Crypto_rsa_rsassa_pkcs1_v15_sign(
    Crypto_rsa_context* ctx,
    /* PRQA S 1336 ++ */ /* VL_Crypto_62_1336 */
    /* PRQA S 3672 ++ */ /* VL_Crypto_62_3672 */
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    /* PRQA S 3672 -- */ /* VL_Crypto_62_3672 */
    /* PRQA S 1336 -- */ /* VL_Crypto_62_1336 */
    void*                      p_rng,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    unsigned char*             sig);

/**
 * @brief   Generates an RSA signature using the RSASSA-PSS scheme with extended parameters.
 *
 * This function generates an RSA signature using the RSASSA-PSS scheme with extended parameters,
 * including the hash algorithm for the message and the mask generation function (MGF1).
 *
 * @param[in] ctx              Pointer to the RSA context containing the private key.
 * @param[in] f_rng            Pointer to the random number generator function.
 * @param[in] p_rng            Pointer to the random number generator context.
 * @param[in] md_alg           Hash algorithm family type used for the message.
 * @param[in] hashlen          Length of the hash in bytes.
 * @param[in] hash             Pointer to the hash of the message.
 * @param[in] saltlen          Length of the salt in bytes.
 * @param[out] sig             Pointer to the buffer where the generated signature will be stored.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
int Crypto_rsa_rsassa_pss_sign_ext(
    Crypto_rsa_context* ctx,
    /* PRQA S 1336 ++ */ /* VL_Crypto_62_1336 */
    /* PRQA S 3672 ++ */ /* VL_Crypto_62_3672 */
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    /* PRQA S 3672 -- */ /* VL_Crypto_62_3672 */
    /* PRQA S 1336 -- */ /* VL_Crypto_62_1336 */
    void*                      p_rng,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    int                        saltlen,
    unsigned char*             sig);

/**
 * @brief   Generates an RSA signature using the RSASSA-PSS scheme.
 *
 * This function generates an RSA signature using the RSASSA-PSS scheme.
 * It uses the provided random number generator function to ensure the randomness of the salt.
 *
 * @param[in] ctx              Pointer to the RSA context containing the private key.
 * @param[in] f_rng            Pointer to the random number generator function.
 * @param[in] p_rng            Pointer to the random number generator context.
 * @param[in] md_alg           Hash algorithm family type used for the message.
 * @param[in] hashlen          Length of the hash in bytes.
 * @param[in] hash             Pointer to the hash of the message.
 * @param[out] sig             Pointer to the buffer where the generated signature will be stored.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
int Crypto_rsa_rsassa_pss_sign(
    Crypto_rsa_context* ctx,
    /* PRQA S 1336 ++ */ /* VL_Crypto_62_1336 */
    /* PRQA S 3672 ++ */ /* VL_Crypto_62_3672 */
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    /* PRQA S 3672 -- */ /* VL_Crypto_62_3672 */
    /* PRQA S 1336 -- */ /* VL_Crypto_62_1336 */
    void*                      p_rng,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    unsigned char*             sig);

/**
 * @brief   Verifies an RSA signature using the PKCS#1 v1.5 scheme.
 *
 * This function verifies an RSA signature using the PKCS#1 v1.5 scheme.
 * It checks if the provided signature matches the hash of the message using the RSA public key.
 *
 * @param[in] ctx     Pointer to the RSA context containing the public key.
 * @param[in] md_alg  Hash algorithm family type used for the message.
 * @param[in] hashlen Length of the hash in bytes.
 * @param[in] hash    Pointer to the hash of the message.
 * @param[in] sig     Pointer to the signature to verify.
 *
 * @return  Status of the operation.
 *          - 0: Signature is valid.
 *          - Non-zero: Signature is invalid.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_pkcs1_verify(
    Crypto_rsa_context*        ctx,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    const unsigned char*       sig);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Verifies an RSA signature using the RSASSA-PKCS1-v1_5 scheme.
 *
 * This function verifies an RSA signature using the RSASSA-PKCS1-v1_5 scheme.
 * It checks if the provided signature matches the hash of the message using the RSA public key.
 *
 * @param[in] ctx     Pointer to the RSA context containing the public key.
 * @param[in] md_alg  Hash algorithm family type used for the message.
 * @param[in] hashlen Length of the hash in bytes.
 * @param[in] hash    Pointer to the hash of the message.
 * @param[in] sig     Pointer to the signature to verify.
 *
 * @return  Status of the operation.
 *          - 0: Signature is valid.
 *          - Non-zero: Signature is invalid.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_rsassa_pkcs1_v15_verify(
    Crypto_rsa_context*        ctx,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    const unsigned char*       sig);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Verifies an RSA signature using the RSASSA-PSS scheme.
 *
 * This function verifies an RSA signature using the RSASSA-PSS scheme.
 * It checks if the provided signature matches the hash of the message using the RSA public key.
 *
 * @param[in] ctx     Pointer to the RSA context containing the public key.
 * @param[in] md_alg  Hash algorithm family type used for the message.
 * @param[in] hashlen Length of the hash in bytes.
 * @param[in] hash    Pointer to the hash of the message.
 * @param[in] sig     Pointer to the signature to verify.
 *
 * @return  Status of the operation.
 *          - 0: Signature is valid.
 *          - Non-zero: Signature is invalid.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_rsassa_pss_verify(
    Crypto_rsa_context*        ctx,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    const unsigned char*       sig);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Verifies an RSA signature using the RSASSA-PSS scheme with extended parameters.
 *
 * This function verifies an RSA signature using the RSASSA-PSS scheme with extended parameters,
 * including the hash algorithm for the message and the mask generation function (MGF1).
 *
 * @param[in] ctx              Pointer to the RSA context containing the public key.
 * @param[in] md_alg           Hash algorithm family type used for the message.
 * @param[in] hashlen          Length of the hash in bytes.
 * @param[in] hash             Pointer to the hash of the message.
 * @param[in] mgf1_hash_id     Hash algorithm family type used for the MGF1.
 * @param[in] expected_salt_len Expected length of the salt in bytes.
 * @param[in] sig              Pointer to the signature to verify.
 *
 * @return  Status of the operation.
 *          - 0: Signature is valid.
 *          - Non-zero: Signature is invalid.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_rsassa_pss_verify_ext(
    Crypto_rsa_context*        ctx,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    Crypto_AlgorithmFamilyType mgf1_hash_id,
    int                        expected_salt_len,
    const unsigned char*       sig);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief   Deduces the CRT (Chinese Remainder Theorem) parameters from given RSA primes and private exponent.
 *
 * This function calculates the CRT parameters (DP, DQ, QP) based on the provided RSA primes (P, Q) and private exponent
 * (D).
 *
 * @param[in]  P   Pointer to the first prime factor.
 * @param[in]  Q   Pointer to the second prime factor.
 * @param[in]  D   Pointer to the private exponent.
 * @param[out] DP  Pointer to the MPI structure to store <code>D % (P - 1)</code>.
 * @param[out] DQ  Pointer to the MPI structure to store <code>D % (Q - 1)</code>.
 * @param[out] QP  Pointer to the MPI structure to store <code>1 / (Q % P)</code>.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_deduce_crt(
    const Crypto_mpi* P,
    const Crypto_mpi* Q,
    const Crypto_mpi* D,
    Crypto_mpi*       DP,
    Crypto_mpi*       DQ,
    Crypto_mpi*       QP);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/**
 * @brief   Deduces the RSA prime factors from the modulus and private exponent.
 *
 * This function calculates the RSA prime factors (P, Q) based on the provided RSA modulus (N) and private exponent (D).
 *
 * @param[in]  N   Pointer to the RSA modulus.
 * @param[in]  E   Pointer to the public exponent.
 * @param[in]  D   Pointer to the private exponent.
 * @param[out] P   Pointer to the MPI structure to store the first prime factor.
 * @param[out] Q   Pointer to the MPI structure to store the second prime factor.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_deduce_primes(
    const Crypto_mpi* N,
    const Crypto_mpi* E,
    const Crypto_mpi* D,
    Crypto_mpi*       P,
    Crypto_mpi*       Q);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/**
 * @brief   Deduces the RSA private exponent from the prime factors and public exponent.
 *
 * This function calculates the RSA private exponent (D) based on the provided RSA prime factors (P, Q) and public
 * exponent (E).
 *
 * @param[in]  P   Pointer to the first prime factor.
 * @param[in]  Q   Pointer to the second prime factor.
 * @param[in]  E   Pointer to the public exponent.
 * @param[out] D   Pointer to the MPI structure to store the private exponent.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_deduce_private_exponent(const Crypto_mpi* P, const Crypto_mpi* Q, const Crypto_mpi* E, Crypto_mpi* D);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/**
 * @brief   Validates the RSA key parameters.
 *
 * This function verifies the correctness of the provided RSA key parameters (N, P, Q, D, E).
 * It uses the provided random number generator function to ensure the randomness of any necessary computations.
 *
 * @param[in]  N      Pointer to the RSA modulus.
 * @param[in]  P      Pointer to the first prime factor.
 * @param[in]  Q      Pointer to the second prime factor.
 * @param[in]  D      Pointer to the private exponent.
 * @param[in]  E      Pointer to the public exponent.
 * @param[in]  f_rng  Pointer to the random number generator function.
 * @param[in]  p_rng  Pointer to the random number generator context.
 *
 * @return  Status of the operation.
 *          - 0: Parameters are valid.
 *          - Non-zero: Parameters are invalid.
 */
int Crypto_rsa_validate_params(
    const Crypto_mpi* N,
    const Crypto_mpi* P,
    const Crypto_mpi* Q,
    const Crypto_mpi* D,
    const Crypto_mpi* E,
    /* PRQA S 1336 ++ */ /* VL_Crypto_62_1336 */
    /* PRQA S 3672 ++ */ /* VL_Crypto_62_3672 */
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    /* PRQA S 3672 -- */ /* VL_Crypto_62_3672 */
    /* PRQA S 1336 -- */ /* VL_Crypto_62_1336 */
    void* p_rng);
/**
 * @brief   Writes the RSA key to a buffer.
 *
 * This function serializes the provided RSA key parameters (N, P, Q, D, E) into a buffer in a specific format.
 *
 * @param[in]  rsa    Pointer to the RSA context containing the key parameters.
 * @param[out] start  Pointer to the start of the buffer where the key will be written.
 * @param[out] p      Pointer to a pointer to the current position in the buffer.
 *
 * @return  Status of the operation.
 *          - 0: Operation successful.
 *          - Non-zero: Operation failed.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_write_key(const Crypto_rsa_context* rsa, unsigned char* start, unsigned char** p);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_ALGORITHMFAM_RSA == STD_ON */

#endif /* CRYPTO_RSA_H */
