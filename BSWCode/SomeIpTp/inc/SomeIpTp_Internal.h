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
 **  @file               : SomeIpTp_Types.h
 **  @author             : HuRongbo
 **  @date               : 2020/07/07
 **  @vendor             : isoft
 **  @description        : SOMEIP Transport Protocol
 **
 ***********************************************************************************************************************/

#ifndef SOMEIPTP_INTERNAL_H
#define SOMEIPTP_INTERNAL_H

/* =================================================== inclusions =================================================== */
#include "ComStack_Types.h"
#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

#define SOMEIPTP_TPFLAG_MASK       0x20u
#define SOMEIPTP_OFFSETFIELD_LSB   16UL
#define SOMEIPTP_TOTAL_HEADER_LEN  12UL
#define SOMEIPTP_MAX_PAYLOAD_LEN   1400u
#define SOMEIPTP_SOMEIP_HEADER_LEN 8u
#define SOMEIPTP_TP_HEADER_LEN     (4UL)
/* SOMEIPTP_MAX_PAYLOAD_LEN + SOMEIPTP_SOMEIP_HEADER_LEN + SOMEIPTP_TP_HEADER_LEN */
#define SOMEIPTP_MAX_PDU_LEN       1404UL

#define SOMEIPTP_HEADER_MESSAGE_TYPE_INDEX 6u

#define SOMEIPTP_DET_REPORT_RUNTIMEERROR(ApiId, ErrorId) \
    ((void)Det_ReportRuntimeError(SOMEIPTP_MODULE_ID, SOMEIPTP_INSTANCE_ID, (ApiId), (ErrorId)))

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif
