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
**  FILENAME    : Xcp_Ram.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Defined array variables for XCP                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*=======[I N C L U D E S]====================================================*/
#include "Xcp_Internal.h"
#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
VAR(uint8, XCP_VAR_NO_INIT_8) Xcp_CmdBuffer[XCP_MAX_CTO];
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
VAR(uint8, XCP_VAR_NO_INIT_8) Xcp_RespBuffer[XCP_MAX_CTO];
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
VAR(uint8, XCP_VAR) Xcp_EvBuffer[XCP_CTO_BUFFER_SIZE];
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
VAR(uint8, XCP_VAR) Xcp_ServBuffer[XCP_CTO_BUFFER_SIZE]; /*PRQA S 1533*/
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
#define XCP_START_SEC_VAR_SAVED_ZONE_16
#include "Xcp_MemMap.h"
VAR(Xcp_DaqType, XCP_VAR) Xcp_Daq[XCP_MAX_DAQ]; /* PRQA S 1531 */ /* MISRA Rule 8.7 */
#define XCP_STOP_SEC_VAR_SAVED_ZONE_16
#include "Xcp_MemMap.h"
#endif /*XCP_PL_DAQ == XCP_PL_DAQ & XCP_RESOURCE*/

/*
 * Block transfer
 * */
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
VAR(Xcp_AGType, XCP_VAR) Xcp_BlockBuffer[XCP_BLOCK_BUFFER_SIZE];
#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
#endif

/*
 * CAL
 * */
#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
#if (XCP_PAG_SUPPORT == STD_ON)
#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
VAR(uint8, XCP_VAR) Xcp_ActivPagNum[XCP_MAX_SEGMENT];
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
#endif
#endif
