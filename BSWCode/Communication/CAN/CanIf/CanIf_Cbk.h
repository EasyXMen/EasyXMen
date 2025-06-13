/* PRQA S 3108++ */
/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : CanIf_Cbk.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Callback declaration supplied by CANIF                      **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR Classic Platform 4.2.2 and R19-11              **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* Refer to CanIf.h */

#ifndef CANIF_CBK_H
#define CANIF_CBK_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "CanIf.h"
#include "CanIf_Can.h"
#include "CanIf_CanTrcv.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/* Published information */
#if !defined(CANIF_PUBLISHED_INFORMATION)
#define CANIF_PUBLISHED_INFORMATION
#define CANIF_MODULE_ID                   60u
#define CANIF_VENDOR_ID                   62u
#define CANIF_AR_RELEASE_MAJOR_VERSION    4u
#define CANIF_AR_RELEASE_MINOR_VERSION    5u
#define CANIF_AR_RELEASE_REVISION_VERSION 0u
#define CANIF_SW_MAJOR_VERSION            2u
#define CANIF_SW_MINOR_VERSION            2u
#define CANIF_SW_PATCH_VERSION            3u
#elif ((CANIF_SW_MAJOR_VERSION != 2u) || (CANIF_SW_MINOR_VERSION != 2u) || (CANIF_SW_PATCH_VERSION != 3u))
#error "CanIf: Mismatch in Software Version"
#endif

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

#endif /* CANIF_CBK_H */
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
