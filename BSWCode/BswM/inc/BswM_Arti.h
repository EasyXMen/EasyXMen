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
 **  @file               : BswM_Arti.h
 **  @author             : mengbo.li
 **  @date               : 2026/06/04
 **  @vendor             : isoft
 **  @description        : ARTI trace interface of BswM
 **
 ***********************************************************************************************************************/

#ifndef BSWM_ARTI_H_
#define BSWM_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* ===================================================== macros ===================================================== */

#define BswM_ArtiApiStart(num, ...) ARTI_TRACE_N(USER, AR_CP_BSW_API, BswM, 0, Bsw_Start, num, __VA_ARGS__)
#define BswM_ArtiApiStop(num, ...)  ARTI_TRACE_N(USER, AR_CP_BSW_API, BswM, 0, Bsw_Return, num, __VA_ARGS__)

/* BSWM_API_ID_INIT = 0u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_GET_VERSION_INFO = 0x01u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x01u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x01u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x01u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x01u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_REQUEST_MODE = 0x02u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x02u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x02u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x02u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x02u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_MAINFUNCTION = 0x03u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x03u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x03u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x03u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x03u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_CANSM_CURRENT_STATE = 0x05u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x05u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x05u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x05u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x05u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_DCM_COM_MODE_CURRENT_STATE = 0x06u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x06u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x06u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x06u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x06u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_LINSM_CURRENT_STATE = 0x09u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x09u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x09u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x09u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x09u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_LINSM_CURRENT_SCHEDULE = 0x0au */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x0au_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x0au_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x0au_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x0au_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_LINTP_REQUEST_MODE = 0x0bu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x0bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x0bu_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x0bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x0bu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_FRSM_CURRENT_STATE = 0x0cu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x0cu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x0cu_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x0cu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x0cu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_ETHSM_CURRENT_STATE = 0x0du */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x0du_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x0du_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x0du_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x0du_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_COMM_CURRENT_MODE = 0x0eu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x0eu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x0eu_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x0eu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x0eu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_ECUM_CURRENT_STATE = 0x0fu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x0fu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x0fu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x0fu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x0fu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_ECUM_CURRENT_WAKEUP = 0x10u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x10u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x10u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x10u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x10u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_DCM_APPLICATION_UPDATED = 0x14u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x14u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x14u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x14u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x14u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_COMM_CURRENT_PNC_MODE = 0x15u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x15u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x15u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x15u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x15u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_NVM_CURRENT_BLOCK_MODE = 0x16u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x16u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x16u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x16u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x16u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_NVM_CURRENT_JOB_MODE = 0x17u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x17u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x17u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x17u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x17u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_J1939NM_STATE_CHANGE_NOTIFY = 0x18u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x18u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x18u_Bsw_Start_USER(instanceParameter, param1, param2, param3) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x18u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x18u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_J1939DCM_BROADCAST_STATUS = 0x1bu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x1bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x1bu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x1bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x1bu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_BSWM_PARTITION_RESTARTED = 0x1eu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x1eu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x1eu_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x1eu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x1eu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_SD_CLIENT_SERVICE_CURRENT_STATE = 0x1fu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x1fu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x1fu_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x1fu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x1fu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_SD_EVENT_HANDLER_CURRENT_STATE = 0x20u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x20u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x20u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x20u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x20u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_SD_CONSUMED_EVENT_GROUP_CURRENT_STATE = 0x21u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x21u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x21u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x21u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x21u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_COMM_INITIATE_RESET = 0x22u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x22u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x22u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x22u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x22u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_ECUM_REQUESTED_STATE = 0x23u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x23u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x23u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x23u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x23u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_NMIF_CAR_WAKEUP_INDICATION = 0x24u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x24u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x24u_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x24u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x24u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_ETHIF_PORT_GRU_LINK_STATE = 0x26u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x26u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x26u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x26u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x26u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_NM_STATECHANGENOTIFICATION = 0x27u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x27u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x27u_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x27u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x27u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_SOAD_SOCONMODECHG = 0x2au */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x2au_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x2au_Bsw_Start_USER(instanceParameter, param1, param2) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x2au_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x2au_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_MODE_NOTIFICATION_PORT = 0xfdu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0xfdu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0xfdu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0xfdu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0xfdu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_MODE_REQUEST_PORT = 0xfeu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0xfeu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0xfeu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0xfeu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0xfeu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_MODE_SWITCH_ERROR_NOTIFICATION = 0xffu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0xffu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0xffu_Bsw_Start_USER(instanceParameter, param1) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0xffu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0xffu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_INITCANSMREQUESTPORTS = 0x30u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x30u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x30u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x30u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x30u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_INITCOMMREQUESTPORTS = 0x31u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x31u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x31u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x31u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x31u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_INITDCMREQUESTPORTS = 0x32u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x32u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x32u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x32u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x32u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_INITECUMREQUESTPORTS = 0x33u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x33u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x33u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x33u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x33u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_INITETHIFREQUESTPORTS = 0x34u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x34u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x34u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x34u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x34u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_INITETHSMREQUESTPORTS = 0x35u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x35u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x35u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x35u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x35u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_INITFRSMREQUESTPORTS = 0x36u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x36u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x36u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x36u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x36u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_INITJ1939NMREQUESTPORTS = 0x37u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x37u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x37u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x37u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x37u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_INITLINSMREQUESTPORTS = 0x38u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x38u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x38u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x38u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x38u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_INITLINTPREQUESTPORTS = 0x39u */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x39u_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x39u_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x39u_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x39u_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_INITNMREQUESTPORTS = 0x3Au */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Au_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Au_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Au_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Au_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_INITNVMREQUESTPORTS = 0x3Bu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Bu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Bu_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Bu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Bu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_INITSDREQUESTPORTS = 0x3Cu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Cu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Cu_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Cu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Cu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_INITSOADREQUESTPORTS = 0x3Du */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Du_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Du_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Du_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Du_Bsw_Return_USER(instanceParameter) (void)(0)
#endif

/* BSWM_API_ID_INITBSWMTIMER = 0x3Eu */
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Eu_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Eu_Bsw_Start_USER(instanceParameter) (void)(0)
#endif
#ifndef ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Eu_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_BswM_0x3Eu_Bsw_Return_USER(instanceParameter) (void)(0)
#endif
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */

#endif /* BSWM_ARTI_H_ */
