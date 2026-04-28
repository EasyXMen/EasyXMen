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

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:LdCom<br>
  RuleSorce:puhua_rules-2024.12.rcf

  \li PRQA S 3432 VL_LdCom_MacrosFunctionUsed
    Reason: Function-like macros are used to allow more efficient code.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li PRQA S 3209 VL_LdCom_3209
    Reason: Designed in accordance with the C standard library.
    Risk: No risk.
    Prevention: The functional coverage test has passed.

  \li PRQA S 3673 VL_LdCom_ConstPointer .<br>
    Reason: Due to the macro being enabled, the pointer was not modified.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

  \li VL_LdCom_1513
    Reason: Identifier '${name}' with external linkage has separate non-defining declarations in more than one
            location.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.
*/

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

#define LDCOM_PDU_ID_INVALID   (PduIdType)(~(PduIdType)0)
#define LDCOM_NOUSED(paramter) (void)(paramter)
/*******************************************************************************
**                      Public API Declarations                               **
*******************************************************************************/

extern FUNC(void, LDCOM_CODE) LdCom_Init(P2CONST(LdCom_ConfigType, AUTOMATIC, LDCOM_APPL_DATA) config);

extern FUNC(void, LDCOM_CODE) LdCom_DeInit(void);

/* PRQA S 3432 ++ */ /* VL_LdCom_MacrosFunctionUsed */
#if (STD_ON == LDCOM_VERSION_INFO_API)
extern FUNC(void, LDCOM_CODE) LdCom_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, LDCOM_APPL_DATA) versioninfo);
#endif /* STD_ON == LDCOM_VERSION_INFO_API */
/* PRQA S 3432 -- */

/* PRQA S 3209 ++ */ /* VL_LdCom_3209 */
extern FUNC(Std_ReturnType, LDCOM_CODE)
    LdCom_Transmit(PduIdType Id, P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr);
/* PRQA S 3209 -- */

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

extern CONST(LdCom_ConfigType, LDCOM_CONST) LdCom_InitCfgSet;

#endif /* LDCOM_H_ */
