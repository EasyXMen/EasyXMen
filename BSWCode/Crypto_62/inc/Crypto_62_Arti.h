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
 **  @file               : Crypto_62_Arti.h
 **  @author             : mengbo.li
 **  @date               : 2026/06/04
 **  @vendor             : isoft
 **  @description        : ARTI trace interface of Crypto_62
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_62_ARTI_H_
#define CRYPTO_62_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* ===================================================== macros ===================================================== */

#define Crypto_62_ArtiApiStart(num, ...) ARTI_TRACE_N(USER, AR_CP_BSW_API, Crypto_62, 0, Bsw_Start, num, __VA_ARGS__)
#define Crypto_62_ArtiApiStop(num, ...)  ARTI_TRACE_N(USER, AR_CP_BSW_API, Crypto_62, 0, Bsw_Return, num, __VA_ARGS__)

/* CRYPTO_SID_INIT = 0x00U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x00U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x00U_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x00U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x00U_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* CRYPTO_SID_GETVERSIONINFO = 0x01U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x01U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x01U_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x01U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x01U_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* CRYPTO_SID_PROCESSJOB = 0x03U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x03U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x03U_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x03U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x03U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYPTO_SID_KEYELEMENTSET = 0x04U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x04U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x04U_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x04U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x04U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYPTO_SID_KEYSETVALID = 0x05U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x05U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x05U_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x05U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x05U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYPTO_SID_KEYELEMENTGET = 0x06U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x06U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x06U_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x06U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x06U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYPTO_SID_KEYGENERATE = 0x07U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x07U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x07U_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x07U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x07U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYPTO_SID_KEYDERIVE = 0x08U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x08U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x08U_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x08U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x08U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYPTO_SID_KEYEXCHANGECALCPUBVAL = 0x09U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x09U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x09U_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x09U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x09U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYPTO_SID_KEYEXCHANGECALCSECRET = 0x0AU */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0AU_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0AU_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0AU_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0AU_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYPTO_SID_MAINFUNCTION = 0x0CU */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0CU_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0CU_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0CU_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0CU_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* CRYPTO_SID_RANDOMSEED = 0x0DU */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0DU_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0DU_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0DU_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0DU_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYPTO_SID_CANCELJOB = 0x0EU */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0EU_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0EU_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0EU_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0EU_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYPTO_SID_KEYELEMENTCOPY = 0x0FU */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0FU_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0FU_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0FU_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x0FU_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYPTO_SID_KEYCOPY = 0x10U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x10U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x10U_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x10U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x10U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYPTO_SID_KEYELEMEMTIDSGET = 0x11U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x11U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x11U_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x11U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x11U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYPTO_SID_KEYELEMENTCOPYPARTIAL = 0x13U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x13U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x13U_Bsw_Start_USER( \
    instanceParameter,                                           \
    param1,                                                      \
    param2,                                                      \
    param3,                                                      \
    param4,                                                      \
    param5,                                                      \
    param6,                                                      \
    param7)                                                      \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x13U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x13U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYPTO_SID_KEYGETSTATUS = 0x14U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x14U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x14U_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x14U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x14U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYPTO_SID_KEYSETINVALID = 0x15U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x15U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x15U_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x15U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x15U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYPTO_SID_CustomSync = 0x1aU */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x1aU_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x1aU_Bsw_Start_USER( \
    instanceParameter,                                           \
    param1,                                                      \
    param2,                                                      \
    param3,                                                      \
    param4,                                                      \
    param5,                                                      \
    param6,                                                      \
    param7,                                                      \
    param8,                                                      \
    param9,                                                      \
    param10,                                                     \
    param11)                                                     \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x1aU_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Crypto_62_0x1aU_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */

#endif /* CRYPTO_62_ARTI_H_ */
