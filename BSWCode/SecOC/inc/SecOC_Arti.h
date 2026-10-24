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
 **  @file               : SecOC_Arti.h
 **  @author             : mengbo.li
 **  @date               : 2026/06/04
 **  @vendor             : isoft
 **  @description        : ARTI trace interface of SecOC
 **
 ***********************************************************************************************************************/

#ifndef SECOC_ARTI_H_
#define SECOC_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* ===================================================== macros ===================================================== */

#define SecOC_ArtiApiStart(num, ...) ARTI_TRACE_N(USER, AR_CP_BSW_API, SecOC, 0, Bsw_Start, num, __VA_ARGS__)
#define SecOC_ArtiApiStop(num, ...)  ARTI_TRACE_N(USER, AR_CP_BSW_API, SecOC, 0, Bsw_Return, num, __VA_ARGS__)

/* SECOC_INIT_ID = 0x01u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x01u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x01u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x01u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x01u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SECOC_GETVERSIONINFO_ID = 0x02u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x02u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x02u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x02u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x02u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SECOC_MAINFUNCTIONTX_ID = 0x03u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x03u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x03u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x03u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x03u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SECOC_SENDDEFAULTAUTHENTICATIONINFORMATION_ID = 0x04u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x04u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x04u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x04u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x04u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SECOC_DEINIT_ID = 0x05u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x05u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x05u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x05u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x05u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SECOC_MAINFUNCTIONRX_ID = 0x06u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x06u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x06u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x06u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x06u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SECOC_VERIFYSTATUSOVERRIDE_ID = 0x0bu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x0bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x0bu_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x0bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x0bu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SECOC_TXCONFIRMATION_ID = 0x40u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x40u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x40u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x40u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x40u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SECOC_TRIGGERTRANSMIT_ID = 0x41u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x41u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x41u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x41u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x41u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SECOC_RXINDICATION_ID = 0x42u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x42u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x42u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x42u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x42u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SECOC_COPYTXDATA_ID = 0x43u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x43u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x43u_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x43u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x43u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SECOC_COPYRXDATA_ID = 0x44u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x44u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x44u_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x44u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x44u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SECOC_TPRXINDICATION_ID = 0x45u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x45u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x45u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x45u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x45u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SECOC_STARTOFRECEPTION_ID = 0x46u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x46u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x46u_Bsw_Start_USER(instanceParameter, param1, param2, param3, param4) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x46u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x46u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SECOC_TPTXCONFIRMATION_ID = 0x48u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x48u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x48u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x48u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x48u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SECOC_IFTRANSMIT_ID = 0x49u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x49u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x49u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x49u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x49u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SECOC_TPTRANSMIT_ID = 0x53u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x53u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x53u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x53u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x53u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SECOC_IFCANCELTRANSMIT_ID = 0x4au */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x4au_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x4au_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x4au_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x4au_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SECOC_TPCANCELTRANSMIT_ID = 0x54u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x54u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x54u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x54u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x54u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SECOC_TPCANCELRECEIVE_ID = 0x4cu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x4cu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x4cu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x4cu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x4cu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SECOC_CSMGENERATEJOBFINISHEDINDICATION_ID = 0x50u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x50u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x50u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x50u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x50u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SECOC_CSMVERIFYJOBFINISHEDINDICATION_ID = 0x51u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x51u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x51u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_SecOC_0x51u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_SecOC_0x51u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */

#endif /* SECOC_ARTI_H_ */
