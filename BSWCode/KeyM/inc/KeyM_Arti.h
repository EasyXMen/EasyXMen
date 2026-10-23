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
 **  @file               : KeyM_Arti.h
 **  @author             : mengbo.li
 **  @date               : 2026/06/04
 **  @vendor             : isoft
 **  @description        : ARTI trace interface of KeyM
 **
 ***********************************************************************************************************************/

#ifndef KEYM_ARTI_H_
#define KEYM_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* ===================================================== macros ===================================================== */

#define KeyM_ArtiApiStart(num, ...) ARTI_TRACE_N(USER, AR_CP_BSW_API, KeyM, 0, Bsw_Start, num, __VA_ARGS__)
#define KeyM_ArtiApiStop(num, ...)  ARTI_TRACE_N(USER, AR_CP_BSW_API, KeyM, 0, Bsw_Return, num, __VA_ARGS__)

/* KEYM_INIT_ID = 0x01U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x01U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x01U_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x01U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x01U_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* KEYM_DEINIT_ID = 0x02U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x02U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x02U_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x02U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x02U_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* KEYM_GETVERSIONINFO_ID = 0x03U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x03U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x03U_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x03U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x03U_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* KEYM_START_ID = 0x04U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x04U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x04U_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4, param5) \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x04U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x04U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KEYM_PREPARE_ID = 0x05U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x05U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x05U_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x05U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x05U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KEYM_UPDATE_ID = 0x06U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x06U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x06U_Bsw_Start_USER( \
    instanceParameter,                                      \
    param1,                                                 \
    param2,                                                 \
    param3,                                                 \
    param4,                                                 \
    param5,                                                 \
    param6)                                                 \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x06U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x06U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KEYM_FINAL_ID = 0x07U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x07U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x07U_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x07U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x07U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KEYM_VERIFY_ID = 0x08U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x08U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x08U_Bsw_Start_USER( \
    instanceParameter,                                      \
    param1,                                                 \
    param2,                                                 \
    param3,                                                 \
    param4,                                                 \
    param5,                                                 \
    param6)                                                 \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x08U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x08U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KEYM_SERV_CERTIFICATE_ID = 0x09U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x09U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x09U_Bsw_Start_USER( \
    instanceParameter,                                      \
    param1,                                                 \
    param2,                                                 \
    param3,                                                 \
    param4,                                                 \
    param5,                                                 \
    param6,                                                 \
    param7)                                                 \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x09U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x09U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KEYM_SET_CERTIFICATE_ID = 0x0AU */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0AU_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0AU_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0AU_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0AU_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KEYM_GET_CERTIFICATE_ID = 0x0BU */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0BU_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0BU_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0BU_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0BU_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KEYM_VERIFY_CERTIFICATES_ID = 0x0CU */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0CU_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0CU_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0CU_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0CU_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KEYM_VERIFY_CERTIFICATE_ID = 0x0DU */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0DU_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0DU_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0DU_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0DU_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KEYM_VERIFY_CERTIFICATE_CHAIN_ID = 0x0EU */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0EU_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0EU_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0EU_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0EU_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KEYM_CERT_ELEMENT_GET_ID = 0x0FU */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0FU_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0FU_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0FU_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x0FU_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KEYM_CERT_ELEMENT_GET_FIRST_ID = 0x10U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x10U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x10U_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4, param5) \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x10U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x10U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KEYM_CERT_ELEMENT_GET_NEXT_ID = 0x11U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x11U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x11U_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x11U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x11U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KEYM_CERT_GET_STATUS_ID = 0x12U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x12U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x12U_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x12U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x12U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KEYM_SERV_CERTIFICATE_BY_CERT_ID = 0x13U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x13U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x13U_Bsw_Start_USER( \
    instanceParameter,                                      \
    param1,                                                 \
    param2,                                                 \
    param3,                                                 \
    param4,                                                 \
    param5,                                                 \
    param6)                                                 \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x13U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x13U_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KEYM_MAINFUNCTION_ID = 0x19U */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x19U_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x19U_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x19U_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x19U_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* KEYM_CERT_ELEMENT_GET_BY_INDEX = 0x1BU */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x1BU_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x1BU_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4, param5) \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x1BU_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x1BU_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* KeyM_CertElementGetCount_ID = 0x1CU */
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x1CU_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x1CU_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_KeyM_0x1CU_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_KeyM_0x1CU_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */

#endif /* KEYM_ARTI_H_ */
