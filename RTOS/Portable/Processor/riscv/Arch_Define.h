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
 *
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
/* PRQA S 3108-- */
#ifndef ARCH_DEFINE_H
#define ARCH_DEFINE_H

/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"

/*=======[M A C R O S]========================================================*/
/* Register operations */
#define OS_WRITE_SP(val) {                 \
    uint32  v = (uint32)(val);             \
    __asm volatile("mv sp, %0"             \
            :                              \
            :"r"(v)                        \
            :"memory");                    \
}
#define OS_READ_SP(val) ({                 \
    __asm volatile("mv %0, sp"             \
            :"=r"(val)                     \
            :                              \
            :"memory");                    \
    val;                                   \
})

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
typedef struct
{
    uint8 *pCPU5DataRamStart;     /* address range of ram */
    uint8 *pCPU5DataRamEnd;

    uint8 *pCPU4DataRamStart;     /* address range of ram */
    uint8 *pCPU4DataRamEnd;

    uint8 *pCPU3DataRamStart;     /* address range of ram */
    uint8 *pCPU3DataRamEnd;

    uint8 *pCPU2DataRamStart;     /* address range of ram */
    uint8 *pCPU2DataRamEnd;

    uint8 *pCPU1DataRamStart;     /* address range of ram */
    uint8 *pCPU1DataRamEnd;

    uint8 *pCPU0DataRamStart;     /* address range of ram */
    uint8 *pCPU0DataRamEnd;

    uint8 *pGlobleRamStart;
    uint8 *pGlobleRamEnd;

    uint8 *pDflashStart;
    uint8 *pDflashEnd;

    uint8 *pLocalDRamStart;
    uint8 *pLocalDRamEnd;

    uint8 *pPeripheralStart;
    uint8 *pPeripheralEnd;

    uint8 *pRomStart;
    uint8 *pRomEnd;
}Os_MemProtKnAddr;

typedef struct
{
    CONST(Os_MemProtKnAddr, OS_VAR) OsKernelAddr;
}Os_MemProtKnCfgType;
#endif /* ARCH_DEFINE_H */
/*=======[E N D   O F   F I L E]==============================================*/
