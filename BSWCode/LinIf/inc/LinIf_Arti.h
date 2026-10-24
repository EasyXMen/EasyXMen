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
 **  @file               : LinIf_Arti.h
 **  @author             : darren.zhang
 **  @date               :
 **  @vendor             : isoft
 **  @description        : header file for Arti
 **
 ***********************************************************************************************************************/

#ifndef LINIF_ARTI_H_
#define LINIF_ARTI_H_

/* =================================================== inclusions =================================================== */
#include "Arti.h"

/* ================================================== trace macros ================================================== */

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x01_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x01_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x01_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x01_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x03_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x03_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x03_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x03_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x04_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x04_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x04_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x04_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x05_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x05_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x05_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x05_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x06_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x06_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x06_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x06_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x07_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x07_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x07_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x07_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x08_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x08_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x08_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x08_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x09_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x09_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x09_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x09_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x0A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x0A_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x0A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x0A_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x0B_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x0B_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x0B_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x0B_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x60_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x60_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x60_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x60_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x61_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x61_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x61_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x61_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x70_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x70_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x70_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x70_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x71_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x71_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x71_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x71_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x72_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x72_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x72_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x72_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x73_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x73_Bsw_Start_USER(instanceParameter, param1, param2, param3) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x73_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x73_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x78_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x78_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x78_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x78_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x79_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x79_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x79_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x79_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x7A_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x7A_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x7A_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x7A_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x7B_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x7B_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x7B_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x7B_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x7F_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x7F_Bsw_Start_USER(instanceParameter, param1, param2) \
    ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x7F_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x7F_Bsw_Return_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x80_Bsw_Start_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x80_Bsw_Start_USER(instanceParameter, param1) ((void)(instanceParameter))
#endif

#ifndef ARTI_TRACE_AR_CP_BSW_API_LinIf_0x80_Bsw_Return_USER
#define ARTI_TRACE_AR_CP_BSW_API_LinIf_0x80_Bsw_Return_USER(instanceParameter) ((void)(instanceParameter))
#endif

#endif
