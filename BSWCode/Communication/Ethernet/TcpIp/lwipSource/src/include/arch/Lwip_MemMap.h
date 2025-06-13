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
************************************************************************************************************************
**                                                                                                                    **
**  FILENAME    : Lwip_MemMap.h                                                                                       **
**                                                                                                                    **
**  Created on  : 10/03/21                                                                                            **
**  Author      : darren.zhang                                                                                        **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : lwip large memory mapping                                                                           **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/

/***********************************************************************************************************************
**                                          REVISION   HISTORY                                                        **
***********************************************************************************************************************/
/* <VERSION>    <DATE>       <AUTHOR>        <REVISION LOG>
 *  V1.0.0    [20190813]  [darren.zhang]   Initial version
 **********************************************************************************************************************/

#if defined(LWIP_HEAP_START_SEC_VAR_NO_INIT_UNSPECIFIED)
#undef LWIP_HEAP_START_SEC_VAR_NO_INIT_UNSPECIFIED
#define LWIP_START_SEC_VAR_NO_INIT_UNSPECIFIED
#elif defined(LWIP_HEAP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED)
#undef LWIP_HEAP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#define LWIP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED

#elif defined(LWIP_MEMP_START_SEC_VAR_INIT_UNSPECIFIED)
#undef LWIP_MEMP_START_SEC_VAR_INIT_UNSPECIFIED
#define LWIP_START_SEC_VAR_INIT_UNSPECIFIED
#elif defined(LWIP_MEMP_STOP_SEC_VAR_INIT_UNSPECIFIED)
#undef LWIP_MEMP_STOP_SEC_VAR_INIT_UNSPECIFIED
#define LWIP_STOP_SEC_VAR_INIT_UNSPECIFIED

#else
#error "lwip not include this section"
#endif

#include "MemMap.h"