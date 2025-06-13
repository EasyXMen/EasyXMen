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
**  FILENAME    : CSM_Type.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuzhe.zhang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : type declaration of KeyManager                              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0     2020-06-01    yuzhe.zhang     initial version.
 *  V2.0.0     2023-02-06    jie.gu          R19_11
 */
#ifndef CSM_TYPE_H
#define CSM_TYPE_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Crypto_GeneralTypes.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*Jobs PC-config type*/
typedef struct
{
    P2CONST(Crypto_JobPrimitiveInfoType, TYPEDEF, CSM_CONST) jobPrimitiveInfo;
    P2CONST(Crypto_JobInfoType, TYPEDEF, CSM_CONST) jobInfo;
    P2VAR(Crypto_JobRedirectionInfoType, TYPEDEF, CSM_CONST) inOutRedirtRef; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    uint32 queRef;
    boolean usePort;
} Csm_JobsCfgType;

/*configuration of a CSM key*/
typedef struct
{
    uint32 cryIfKey;
    boolean usePort;
} Csm_KeyCfgType;

/* CSM queue configurations*/
typedef struct
{
    uint32 queueSize;
    uint32 cryIfChRef;
} Csm_QueueCfgType;

/*Identification of a CSM service configuration via a numeric identifier*/
typedef uint16 Csm_ConfigIdType;
#endif
