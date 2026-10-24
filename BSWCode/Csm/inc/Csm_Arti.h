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
 **  @file               : Csm_Arti.h
 **  @author             : mengbo.li
 **  @date               : 2026/06/04
 **  @vendor             : isoft
 **  @description        : ARTI trace interface of Csm
 **
 ***********************************************************************************************************************/

#ifndef CSM_ARTI_H_
#define CSM_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* ===================================================== macros ===================================================== */

#define Csm_ArtiApiStart(num, ...) ARTI_TRACE_N(USER, AR_CP_BSW_API, Csm, 0, Bsw_Start, num, __VA_ARGS__)
#define Csm_ArtiApiStop(num, ...)  ARTI_TRACE_N(USER, AR_CP_BSW_API, Csm, 0, Bsw_Return, num, __VA_ARGS__)

/* CSM_SID_INIT = 0x00u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x00u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x00u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x00u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x00u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* CSM_SID_MAINFUNCTION = 0x01u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x01u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x01u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x01u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x01u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* CSM_SID_CUSTOM_SEV = 0x13u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x13u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x13u_Bsw_Start_USER( \
    instanceParameter,                                     \
    param1,                                                \
    param2,                                                \
    param3,                                                \
    param4,                                                \
    param5,                                                \
    param6,                                                \
    param7,                                                \
    param8,                                                \
    param9,                                                \
    param10,                                               \
    param11,                                               \
    param12,                                               \
    param13,                                               \
    param14)                                               \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x13u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x13u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_GETVERSIONINFO = 0x3Bu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x3Bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x3Bu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x3Bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x3Bu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* CSM_SID_HASH = 0x5Du */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x5Du_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x5Du_Bsw_Start_USER( \
    instanceParameter,                                     \
    param1,                                                \
    param2,                                                \
    param3,                                                \
    param4,                                                \
    param5,                                                \
    param6)                                                \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x5Du_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x5Du_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_ENCRYPT = 0x5Eu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x5Eu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x5Eu_Bsw_Start_USER( \
    instanceParameter,                                     \
    param1,                                                \
    param2,                                                \
    param3,                                                \
    param4,                                                \
    param5,                                                \
    param6)                                                \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x5Eu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x5Eu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_DECRYPT = 0x5Fu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x5Fu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x5Fu_Bsw_Start_USER( \
    instanceParameter,                                     \
    param1,                                                \
    param2,                                                \
    param3,                                                \
    param4,                                                \
    param5,                                                \
    param6)                                                \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x5Fu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x5Fu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_MAC_GEN = 0x60u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x60u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x60u_Bsw_Start_USER( \
    instanceParameter,                                     \
    param1,                                                \
    param2,                                                \
    param3,                                                \
    param4,                                                \
    param5,                                                \
    param6)                                                \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x60u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x60u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_MAC_VERIFY = 0x61u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x61u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x61u_Bsw_Start_USER( \
    instanceParameter,                                     \
    param1,                                                \
    param2,                                                \
    param3,                                                \
    param4,                                                \
    param5,                                                \
    param6,                                                \
    param7)                                                \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x61u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x61u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_AEADENCRYPT = 0x62u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x62u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x62u_Bsw_Start_USER( \
    instanceParameter,                                     \
    param1,                                                \
    param2,                                                \
    param3,                                                \
    param4,                                                \
    param5,                                                \
    param6,                                                \
    param7,                                                \
    param8,                                                \
    param9,                                                \
    param10)                                               \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x62u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x62u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_AEADDECRYPT = 0x63u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x63u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x63u_Bsw_Start_USER( \
    instanceParameter,                                     \
    param1,                                                \
    param2,                                                \
    param3,                                                \
    param4,                                                \
    param5,                                                \
    param6,                                                \
    param7,                                                \
    param8,                                                \
    param9,                                                \
    param10,                                               \
    param11)                                               \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x63u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x63u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_SIG_VERIFY = 0x64u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x64u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x64u_Bsw_Start_USER( \
    instanceParameter,                                     \
    param1,                                                \
    param2,                                                \
    param3,                                                \
    param4,                                                \
    param5,                                                \
    param6,                                                \
    param7)                                                \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x64u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x64u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_KEYSET_VALID = 0x67u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x67u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x67u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x67u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x67u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_KEYELEMENTGET = 0x68u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x68u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x68u_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x68u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x68u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_RANDOMSEED = 0x69u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x69u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x69u_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x69u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x69u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_KEY_GENERATE = 0x6Au */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Au_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Au_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Au_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Au_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_KEY_DERIVE = 0x6Bu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Bu_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Bu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_KEY_EX_PUB = 0x6Cu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Cu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Cu_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Cu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Cu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_KEY_EX_SEC = 0x6Du */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Du_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Du_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Du_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Du_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_CANCELJOB = 0x6Fu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Fu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Fu_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Fu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x6Fu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_CBK_NOTIFY = 0x70u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x70u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x70u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x70u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x70u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* CSM_SID_KEYELEMENTCPY = 0x71u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x71u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x71u_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x71u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x71u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_RANDOM_GEN = 0x72u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x72u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x72u_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x72u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x72u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_KEYCPY = 0x73u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x73u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x73u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x73u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x73u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_SIG_GEN = 0x76u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x76u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x76u_Bsw_Start_USER( \
    instanceParameter,                                     \
    param1,                                                \
    param2,                                                \
    param3,                                                \
    param4,                                                \
    param5,                                                \
    param6)                                                \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x76u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x76u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_KEYELEMENTSET = 0x78u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x78u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x78u_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x78u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x78u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_KEYELEMENTCPYPARTIAL = 0x79u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x79u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x79u_Bsw_Start_USER( \
    instanceParameter,                                     \
    param1,                                                \
    param2,                                                \
    param3,                                                \
    param4,                                                \
    param5,                                                \
    param6,                                                \
    param7)                                                \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x79u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x79u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_JOBKEYSETVALID = 0x7Au */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Au_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Au_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Au_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Au_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_JOBRANDOMSEED = 0x7Bu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Bu_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Bu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_JOBKEYGENERATE = 0x7Cu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Cu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Cu_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Cu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Cu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_JOBKEYDERIVE = 0x7Du */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Du_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Du_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Du_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Du_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_JOBKEY_EX_PUB = 0x7Eu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Eu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Eu_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Eu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Eu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_JOBKEY_EX_SEC = 0x7Fu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Fu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Fu_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Fu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x7Fu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_KEY_GETSTATUS = 0x83u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x83u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x83u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x83u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x83u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_JOBKEYSETINVALID = 0x84u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x84u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x84u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x84u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x84u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_KEYSET_INVALID = 0x85u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x85u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x85u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x85u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x85u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_SAVE_CONTEXT = 0x86u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x86u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x86u_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x86u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x86u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CSM_SID_RESTORE_CONTEXT = 0x87u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x87u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x87u_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_Csm_0x87u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_Csm_0x87u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */

#endif /* CSM_ARTI_H_ */
