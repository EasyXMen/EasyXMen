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
 **  @file               : Crypto_62_Signature.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
/* PRQA S 1532 EOF */ /* VL_QAC_OneFunRef */

#include "Crypto_62_Internal.h"

#if (CRYPTO_SERVICE_SIGNATURE == STD_ON)

#if (CRYPTO_ALGORITHMFAM_RSA == STD_ON)
#include "Crypto_62_Rsa.h"
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
/*
 * Brief               This function process signaturegenerate service with supported modes.
 *
 *
 * Param-Name[in]      objectId: The object ID of the crypto driver.
 *                     algorithmfamily: The algorithm family.
 *                     mode: The algorithm mode.
 *                     operateMode: The operation mode.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Std_ReturnType:
 *                            E_OK: Request successful
 *                            E_NOT_OK: Request failed
 */
/******************************************************************************/
Std_ReturnType Crypto_62_SignatureGenerate_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode)
{
    Std_ReturnType result = E_NOT_OK;

    Crypto_AlgorithmFamilyType secondaryFam =
        Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily;
    if ((CRYPTO_OPERATIONMODE_START == operateMode) || (CRYPTO_OPERATIONMODE_UPDATE == operateMode))
    {
        result = E_OK;
    }
    else
    {
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_RSA:
#if (CRYPTO_ALGORITHMFAM_RSA == STD_ON)
            result = Crypto_Rsa_SignatureGenerate_Process(objectId, mode, secondaryFam);
#endif
            break;
        case CRYPTO_ALGOFAM_BRAINPOOL:

            break;
        case CRYPTO_ALGOFAM_CUSTOM:

            break;
        case CRYPTO_ALGOFAM_ECCNIST:

            break;
        case CRYPTO_ALGOFAM_ECDSA:

            break;
        case CRYPTO_ALGOFAM_ED25519:

            break;
        case CRYPTO_ALGOFAM_SM2:

            break;
        default:
            /* do nothing*/
            break;
        }
    }
    return result;
}
/******************************************************************************/
/*
 * Brief               This function process signatureverify service with supported modes.
 *
 *
 * Param-Name[in]      objectId: The object ID of the crypto driver.
 *                     algorithmfamily: The algorithm family.
 *                     mode: The algorithm mode.
 *                     operateMode: The operation mode.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Std_ReturnType:
 *                            E_OK: Request successful
 *                            E_NOT_OK: Request failed
 */
/******************************************************************************/
Std_ReturnType Crypto_62_SignatureVerify_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode)
{
    Std_ReturnType result = E_NOT_OK;

    Crypto_AlgorithmFamilyType secondaryFam =
        Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily;
    if ((CRYPTO_OPERATIONMODE_START == operateMode) || (CRYPTO_OPERATIONMODE_UPDATE == operateMode))
    {
        result = E_OK;
    }
    else
    {
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_RSA:
#if (CRYPTO_ALGORITHMFAM_RSA == STD_ON)
            result = Crypto_Rsa_SignatureVerifyProcess(objectId, mode, secondaryFam);
#endif
            break;
        case CRYPTO_ALGOFAM_BRAINPOOL:

            break;
        case CRYPTO_ALGOFAM_CUSTOM:

            break;
        case CRYPTO_ALGOFAM_ECCNIST:

            break;
        case CRYPTO_ALGOFAM_ECDSA:

            break;
        case CRYPTO_ALGOFAM_ED25519:

            break;
        case CRYPTO_ALGOFAM_SM2:

            break;
        default:
            /* do nothing*/
            break;
        }
    }
    return result;
}

#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#endif /* CRYPTO_SERVICE_SIGNATURE == STD_ON */
