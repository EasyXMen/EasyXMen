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
********************************************************************************
**                                                                            **
**  FILENAME    : Xcp_Interface.h                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration and type definitions of interface functions **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*=======[I N C L U D E S]====================================================*/
#ifndef XCP_INTERFACE_H_
#define XCP_INTERFACE_H_

/*=======[I N C L U D E S]====================================================*/
#include "Xcp_Cfg.h"
/*=======[M A C R O S]========================================================*/
typedef struct
{
    uint8* seed;
    uint8 lengthS;
} Xcp_SeedVerifyType;

typedef struct
{
    uint8* key;
    uint8 lengthK;
} Xcp_KeyVerifyType;
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#if (STD_ON == XCP_SEED_AND_UNLOCK)
extern FUNC(Std_ReturnType, XCP_CODE) Xcp_CreateSeed(uint8* length, uint8* seed, const uint8 resource);
extern boolean Xcp_IsKeyRight(
    const Xcp_SeedVerifyType seedVerify,
    const Xcp_KeyVerifyType keyVerify,
    const uint8 resource);
#endif /*STD_ON == XCP_SEED_AND_UNLOCK*/

#if (STD_ON == XCP_BUILD_CHECKSUM)
extern void Xcp_InitCrc16CcittTable(void);
extern FUNC(void, XCP_CODE)
    Xcp_ChecksumCompute(const uint32 checksumStartAddress, const uint32 blockSize, uint32* crcResult);
#endif /*STD_ON == XCP_BUILD_CHECKSUM*/

#if (STD_ON == XCP_PAG_SUPPORT)
extern void Xcp_InitCalMem(void);
#endif /*STD_ON == XCP_PAG_SUPPORT*/

#endif /* XCP_INTERFACE_H_ */
