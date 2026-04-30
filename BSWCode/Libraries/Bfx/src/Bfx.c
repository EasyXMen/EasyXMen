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
 **  @file               : Bfx.c
 **  @author             : ShenXu,Yb,QinmeiChen
 **  @date               : 2024/09/09
 **  @vendor             : isoft
 **  @description        : Bfx Routlines
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Bfx.h"
/* ================================================== version check ================================================= */
/* ===================================================== macros ===================================================== */
/* ================================================ type definitions ================================================ */
/* ========================================== internal function declarations ======================================== */
/* ============================================ internal data definitions =========================================== */
/* ============================================ external data definitions =========================================== */
/* ========================================== external function definitions ========================================= */
#define BFX_START_SEC_CODE
#include "Bfx_MemMap.h"
/*PRQA S 1503,1505 ++ */                                                     /*VL_Bfx_macro*/
BFX_DEF(8) /*PRQA S 4533,4393,2144*/                                         /*VL_Bfx_Signed*/
BFX_DEF(16) /*PRQA S 4533,4393,2105*/                                        /*VL_Bfx_Signed*/
BFX_DEF(32) /*PRQA S 4533,4393,2985*/ /*VL_Bfx_Signed*/ /*PRQA S 1252,4464*/ /*VL_Bfx_uint32*/
BFX_DEF(64) /*PRQA S 4533,4393,2985*/ /*VL_Bfx_Signed*/ /*PRQA S 1252*/      /*VL_Bfx_uint32*/
/*PRQA S 1503,1505 -- */
/**
 * retrive version info from macros
 */
void Bfx_GetVersionInfo(Std_VersionInfoType* versionInfo) /*PRQA S 1503*/ /*VL_QAC_NoUsedApi*/
{
    if (versionInfo != NULL_PTR)
    {
        versionInfo->vendorID         = BFX_H_VENDOR_ID;
        versionInfo->moduleID         = BFX_H_MODULE_ID;
        versionInfo->sw_major_version = BFX_H_SW_MAJOR_VERSION;
        versionInfo->sw_minor_version = BFX_H_SW_MINOR_VERSION;
        versionInfo->sw_patch_version = BFX_H_SW_PATCH_VERSION;
    }
}
#define BFX_STOP_SEC_CODE
#include "Bfx_MemMap.h"
/* ========================================== internal function definitions ========================================= */
