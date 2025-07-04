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
**  FILENAME    : Arch_Define.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/
#ifndef ARCH_DEFINE_H
#define ARCH_DEFINE_H
/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
typedef struct
{
    uint8* pSRamStart; /* SRAM */
    uint8* pSRamEnd;

    uint8* pPeripheralStart; /* PERIPHERAL */
    uint8* pPeripheralEnd;

    uint8* pRomStart; /* ROM */
    uint8* pRomEnd;

    uint8* pDtcmStart; /* DTCM */
    uint8* pDtcmEnd;
} Os_MemProtKnAddr;

typedef struct
{
    CONST(Os_MemProtKnAddr, OS_VAR) OsKernelAddr;
} Os_MemProtKnCfgType;
#endif /* ARCH_DEFINE_H */
/*=======[E N D   O F   F I L E]==============================================*/
