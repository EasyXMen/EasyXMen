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
 **  @file               : EcuM_Arti.h
 **  @author             : mengbo.li
 **  @date               : 2026/06/04
 **  @vendor             : isoft
 **  @description        : ARTI trace interface of EcuM
 **
 ***********************************************************************************************************************/

#ifndef ECUM_ARTI_H_
#define ECUM_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* ===================================================== macros ===================================================== */

#define EcuM_ArtiApiStart(num, ...) ARTI_TRACE_N(USER, AR_CP_BSW_API, EcuM, 0, Bsw_Start, num, __VA_ARGS__)
#define EcuM_ArtiApiStop(num, ...)  ARTI_TRACE_N(USER, AR_CP_BSW_API, EcuM, 0, Bsw_Return, num, __VA_ARGS__)

/* ECUM_SID_GETVERSION = 0x00u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x00u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x00u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x00u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x00u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* ECUM_SID_INIT = 0x01u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x01u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x01u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x01u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x01u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* ECUM_SID_SHUTDOWN = 0x02u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x02u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x02u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x02u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x02u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* ECUM_SID_REQUESTRUN = 0x03u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x03u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x03u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x03u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x03u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_RELEASERUN = 0x04u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x04u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x04u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x04u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x04u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_SELECTSHUTDOWNTARGET = 0x06u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x06u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x06u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x06u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x06u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_GETLASTSHUTDOWNTARGET = 0x08u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x08u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x08u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x08u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x08u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_GETSHUTDOWNTARGET = 0x09u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x09u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x09u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x09u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x09u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_REQUESTPOST_RUN = 0x0au */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x0au_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x0au_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x0au_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x0au_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_RELEASEPOST_RUN = 0x0bu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x0bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x0bu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x0bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x0bu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_SETWAKEUPEVENT = 0x0cu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x0cu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x0cu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x0cu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x0cu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* ECUM_SID_GETPENDINGWAKEUPEVENT = 0x0du */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x0du_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x0du_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x0du_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x0du_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_SELECTBOOTTARGET = 0x12u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x12u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x12u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x12u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x12u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_GETBOOTTARGET = 0x13u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x13u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x13u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x13u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x13u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_VALIDATEWAKEUPEVENT = 0x14u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x14u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x14u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x14u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x14u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* ECUM_SID_GETVALIDATEDWAKEUPEVENT = 0x15u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x15u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x15u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x15u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x15u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_CLEARWAKEUPEVENT = 0x16u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x16u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x16u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x16u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x16u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* ECUM_SID_MAINFUNCTION = 0x18u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x18u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x18u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x18u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x18u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* ECUM_SID_GETEXPIREDWAKEUPEVENT = 0x19u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x19u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x19u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x19u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x19u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_STARTUPTWO = 0x1au */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x1au_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x1au_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x1au_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x1au_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* ECUM_SID_SELECTSHUTDOWNCAUSE = 0x1bu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x1bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x1bu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x1bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x1bu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_GETSHUTDOWNCAUSE = 0x1cu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x1cu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x1cu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x1cu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x1cu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_SETRELWAKEUPALARM = 0x22u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x22u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x22u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x22u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x22u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_SETABSWAKEUPALARM = 0x23u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x23u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x23u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x23u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x23u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_ABORTWAKEUPALARM = 0x24u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x24u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x24u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x24u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x24u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_GETCURRENTTIME = 0x25u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x25u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x25u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x25u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x25u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_GETWAKEUPTIME = 0x26u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x26u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x26u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x26u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x26u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_SETCLOCK = 0x27u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x27u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x27u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x27u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x27u_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_SETSTATE = 0x2bu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x2bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x2bu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x2bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x2bu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* ECUM_SID_GODOWNHALTPOLL = 0x2cu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x2cu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x2cu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x2cu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x2cu_Bsw_Return_USER(instanceParameter, param1) (void)(0)
#endif

/* ECUM_SID_CHECKWAKEUP = 0x42u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x42u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x42u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_EcuM_0x42u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_EcuM_0x42u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */

#endif /* ECUM_ARTI_H_ */
