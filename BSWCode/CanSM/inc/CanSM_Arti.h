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
 **  @file               : CanSM_Arti.h
 **  @author             : xiaojian.liang
 **  @date               : 2026/06/03
 **  @vendor             : isoft
 **  @description        : ARTI trace interface of CanSM
 **
 ***********************************************************************************************************************/

#ifndef CANSM_ARTI_H_
#define CANSM_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* ===================================================== macros ===================================================== */

/* SERVICE_ID_CANSM_INIT = 0x00 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x00u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x00u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x00u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x00u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SERVICE_ID_CANSM_GETVERSIONINFO = 0x01 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x01u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x01u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x01u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x01u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SERVICE_ID_CANSM_REQUESTCOMMODE = 0x02 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x02u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x02u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x02u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x02u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SERVICE_ID_CANSM_GETCURRENTCOMMODE = 0x03 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x03u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x03u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x03u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x03u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SERVICE_ID_CANSM_CONTROLLERBUSOFF = 0x04 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x04u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x04u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x04u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x04u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SERVICE_ID_CANSM_MAINFUNCTION = 0x05 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x05u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x05u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x05u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x05u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SERVICE_ID_CANSM_CONFIRMPNAVAILABILITY = 0x06 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x06u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x06u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x06u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x06u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SERVICE_ID_CANSM_CONTROLLERMODEINDICATION = 0x07 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x07u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x07u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x07u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x07u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SERVICE_ID_CANSM_CLEARTRCVWUFFLAGINDICATION = 0x08 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x08u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x08u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x08u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x08u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SERVICE_ID_CANSM_TRANCEIVERMODEINDICATION = 0x09 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x09u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x09u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x09u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x09u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SERVICE_ID_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION = 0x0a */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x0au_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x0au_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x0au_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x0au_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SERVICE_ID_CANSM_TXTIMEOUTEXCEPTION = 0x0b */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x0bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x0bu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x0bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x0bu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SERVICE_ID_CANSM_SETBAUDRATE = 0x0d */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x0du_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x0du_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x0du_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x0du_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SERVICE_ID_CANSM_SETICOMCONFIGURATION = 0x0f */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x0fu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x0fu_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x0fu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x0fu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SERVICE_ID_CANSM_CURRENTICOMCONFIGURATION = 0x10 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x10u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x10u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x10u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x10u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* SERVICE_ID_CANSM_STARTWAKEUPSOURCE = 0x11 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x11u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x11u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x11u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x11u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SERVICE_ID_CANSM_STOPWAKEUPSOURCE = 0x12 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x12u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x12u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x12u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x12u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SERVICE_ID_CANSM_SETECUPASSIVE = 0x13 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x13u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x13u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x13u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x13u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* SERVICE_ID_CANSM_DEINIT = 0x14 */
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x14u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x14u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_CanSM_0x14u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_CanSM_0x14u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* Wrapper macros */
#define CanSM_ArtiApiStart(numEventParameters, ...) \
    ARTI_TRACE_N(USER, AR_CP_BSW_API, CanSM, 0, Bsw_Start, numEventParameters, __VA_ARGS__)
#define CanSM_ArtiApiReturn(numEventParameters, ...) \
    ARTI_TRACE_N(USER, AR_CP_BSW_API, CanSM, 0, Bsw_Return, numEventParameters, __VA_ARGS__)

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */

#endif /* CANSM_ARTI_H_ */
