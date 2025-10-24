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
 **  @file               : Crypto_MAC.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_MAC_H
#define CRYPTO_MAC_H

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (CRYPTO_SERVICE_MAC == STD_ON)

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief   Performs a left shift operation on a byte array.
 *
 * @param[in]  len  Length of the byte array.
 * @param[in]  add  Pointer to the source byte array to be shifted.
 * @param[out] des  Pointer to the destination byte array where the shifted result will be stored.
 *
 * @return  None.
 */
void Crypto_Leftshift(uint32 len, uint8* add, uint8* des);
#if (CRYPTO_ALGORITHMMODE_CMAC == STD_ON)
/**
 * @brief   Verifies a CMAC (Cipher-based Message Authentication Code) for a given object ID and algorithm family.
 *
 * @param[in] objectId         The object ID for the CMAC verification process.
 * @param[in] algorithmfamily  The algorithm family type.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_VerifyCmac(uint32 objectId, Crypto_AlgorithmFamilyType algorithmfamily);

/**
 * @brief   Generates a CMAC for a given object ID and algorithm family.
 *
 * @param[in] objectId         The object ID for the CMAC generation process.
 * @param[in] algorithmfamily  The algorithm family type.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_GenerateCmac_Process(uint32 objectId, Crypto_AlgorithmFamilyType algorithmfamily);
#endif
#if (CRYPTO_ALGORITHMMODE_HMAC == STD_ON)
/**
 * @brief   Initializes the HMAC (Hash-based Message Authentication Code) generation process.
 *
 * @param[in,out] ctx    Pointer to the message digest context structure.
 * @param[in]     key    Pointer to the HMAC key.
 * @param[in]     keylen Length of the HMAC key in bytes.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_GenerateHmac_Start(Crypto_Md_Context_t* ctx, uint8* key, uint32 keylen);

/**
 * @brief   Updates the HMAC context with new input data.
 *
 * @param[in,out] ctx    Pointer to the message digest context structure.
 * @param[in]     input  Pointer to the input data.
 * @param[in]     ilen   Length of the input data in bytes.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_GenerateHmac_Update(Crypto_Md_Context_t* ctx, const uint8* input, uint32 ilen);

/**
 * @brief   Finalizes the HMAC generation process and produces the HMAC output.
 *
 * @param[in,out] ctx    Pointer to the message digest context structure.
 * @param[out]    output Pointer to the buffer where the HMAC output will be stored.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_GenerateHmac_Finish(Crypto_Md_Context_t* ctx, uint8* output);

/**
 * @brief   Resets the HMAC context.
 *
 * @param[in,out] ctx  Pointer to the message digest context structure.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_md_hmac_reset(Crypto_Md_Context_t* ctx);

/**
 * @brief   Generates an HMAC for a given object ID and algorithm family.
 *
 * @param[in] objectId         The object ID for the HMAC generation process.
 * @param[in] algorithmfamily  The algorithm family type.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_GenerateHmac(uint32 objectId, Crypto_AlgorithmFamilyType algorithmfamily);

/**
 * @brief   Verifies an HMAC for a given object ID, algorithm family, and operation mode.
 *
 * @param[in] objectId         The object ID for the HMAC verification process.
 * @param[in] algorithmfamily  The algorithm family type.
 * @param[in] operateMode      The operation mode type.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_VerifyHmac(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_OperationModeType   operateMode);
#endif
/**
 * @brief   Processes the MAC generation for a given object ID, algorithm family, mode, and operation mode.
 *
 * @param[in] objectId         The object ID for the MAC generation process.
 * @param[in] algorithmfamily  The algorithm family type.
 * @param[in] mode             The algorithm mode type.
 * @param[in] operateMode      The operation mode type.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_62_MacGenerate_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode);

/**
 * @brief   Processes the MAC verification for a given object ID, algorithm family, mode, and operation mode.
 *
 * @param[in] objectId         The object ID for the MAC verification process.
 * @param[in] algorithmfamily  The algorithm family type.
 * @param[in] mode             The algorithm mode type.
 * @param[in] operateMode      The operation mode type.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_62_MacVerify_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode);

#endif /* CRYPTO_SERVICE_MAC == STD_ON */

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_MAC_H */
