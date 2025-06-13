/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : sha256.h                                                    **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuzhe.zhang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for sha256                                   **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef SHA256_H
#define SHA256_H
#if (CRYPTO_ALGORITHM_HASH == STD_ON)
#include "Crypto_Types.h"

typedef struct
{
    uint32 total[2];  /*!< The number of Bytes processed.  */
    uint32 state[8];  /*!< The intermediate digest state.  */
    uint8 buffer[64]; /*!< The data block being processed. */
} Crypto_Sha256Data;
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_sha256_starts_ret(Crypto_Sha256Data* ctx);

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_sha256_update_ret(
    Crypto_Sha256Data* ctx,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) input,
    VAR(uint32, AUTOMATIC) ilen);

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_sha256_finish_ret(Crypto_Sha256Data* ctx, VAR(uint8, AUTOMATIC) output[32]);

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_sha256(
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) input,
    VAR(uint32, AUTOMATIC) ilen,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) output);

#endif /* CRYPTO_ALGORITHM_HASH == STD_ON */
#endif /* Crypto_sha256.h */
