/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
**************************************************************************** **
**                                                                           **
**  FILENAME    : LdCom.h                                                    **
**                                                                           **
**  Created on  :                                                            **
**  Author      : zhengfei.li                                                **
**  Vendor      :                                                            **
**  DESCRIPTION :                                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#ifndef LDCOM_H_
#define LDCOM_H_
/******************************************************************************
**                      Includes                                              *
******************************************************************************/
#include "LdCom_Types.h"
/******************************************************************************
**                      Global Symbols                                       **
******************************************************************************/
#define LDCOM_MODULE_ID   49u
#define LDCOM_VENDOR_ID   62u
#define LDCOM_INSTANCE_ID 0u
/*Published information elements*/
#define LDCOM_H_AR_MAJOR_VERSION 4u
#define LDCOM_H_AR_MINOR_VERSION 5u
#define LDCOM_H_AR_PATCH_VERSION 0u
#define LDCOM_H_SW_MAJOR_VERSION 2u
#define LDCOM_H_SW_MINOR_VERSION 0u
#define LDCOM_H_SW_PATCH_VERSION 6u

#define LDCOM_PDU_ID_INVALID     (PduIdType)(-1)
#define LDCOM_NOUSED(paramter)   (void)(paramter)
/*******************************************************************************
**                      Public API Declarations                               **
*******************************************************************************/

extern FUNC(void, LDCOM_CODE) LdCom_Init(P2CONST(LdCom_ConfigType, AUTOMATIC, LDCOM_APPL_DATA) config);

extern FUNC(void, LDCOM_CODE) LdCom_DeInit(void);

#if (STD_ON == LDCOM_VERSION_INFO_API)
extern FUNC(void, LDCOM_CODE) LdCom_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, LDCOM_APPL_DATA) versioninfo);
#endif /* STD_ON == LDCOM_VERSION_INFO_API */

extern FUNC(Std_ReturnType, LDCOM_CODE)
    LdCom_Transmit(PduIdType Id, P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr);

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

extern CONST(LdCom_ConfigType, LDCOM_CONST) LdCom_InitCfgSet;

#endif /* LDCOM_H_ */
