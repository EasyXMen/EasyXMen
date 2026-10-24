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
 **  @file               : CryIf_Arti.h
 **  @author             : mengbo.li
 **  @date               : 2026/06/04
 **  @vendor             : isoft
 **  @description        : ARTI trace interface of CryIf
 **
 ***********************************************************************************************************************/

#ifndef CRYIF_ARTI_H_
#define CRYIF_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* ===================================================== macros ===================================================== */

#define CryIf_ArtiApiStart(num, ...) ARTI_TRACE_N(USER, AR_CP_BSW_API, CryIf, 0, Bsw_Start, num, __VA_ARGS__)
#define CryIf_ArtiApiStop(num, ...)  ARTI_TRACE_N(USER, AR_CP_BSW_API, CryIf, 0, Bsw_Return, num, __VA_ARGS__)

/* CRYIF_SID_INIT = 0x00u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x00u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x00u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x00u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x00u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* CRYIF_SID_GETVERSIONINFO = 0x01u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x01u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x01u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x01u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x01u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* CRYIF_SID_PROCESSJOB = 0x03u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x03u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x03u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x03u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x03u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYIF_SID_KEYELEMENT_SET = 0x04u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x04u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x04u_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x04u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x04u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYIF_SID_KEYSETVALID = 0x05u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x05u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x05u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x05u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x05u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYIF_SID_KEYELEMENT_GET = 0x06u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x06u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x06u_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x06u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x06u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYIF_SID_RANDOM_SEED = 0x07u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x07u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x07u_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x07u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x07u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYIF_SID_KEYGENERATE = 0x08u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x08u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x08u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x08u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x08u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYIF_SID_KEYDERIVE = 0x09u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x09u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x09u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x09u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x09u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYIF_SID_KEYEXCHANGECALCPUBVAL = 0x0Au */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Au_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Au_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Au_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Au_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYIF_SID_KEYEXCHANGECALCSECRET = 0x0Bu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Bu_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Bu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYIF_SID_CBNOTIFICATION = 0x0Du */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Du_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Du_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Du_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Du_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* CRYIF_SID_CANCELJOB = 0x0Eu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Eu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Eu_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Eu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Eu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYIF_SID_KEYSETINVALID = 0x14u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x14u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x14u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x14u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x14u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYIF_SID_KEYELEMENT_COPY = 0x0Fu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Fu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Fu_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Fu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x0Fu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYIF_SID_KEYCOPY = 0x10u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x10u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x10u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x10u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x10u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYIF_SID_KEYELEMENT_COPYPARTIAL = 0x12u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x12u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x12u_Bsw_Start_USER( \
    instanceParameter,                                       \
    param1,                                                  \
    param2,                                                  \
    param3,                                                  \
    param4,                                                  \
    param5,                                                  \
    param6,                                                  \
    param7)                                                  \
    (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x12u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x12u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* CRYIF_SID_KEYGETSTATUS = 0x13u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x13u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x13u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CryIf_0x13u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CryIf_0x13u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */

#endif /* CRYIF_ARTI_H_ */
