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
/*********************************************************************************
**                                                                            **
**  FILENAME    : WdgIf_Types.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : haibin.shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S):   AUTOSAR classic Platform R19-11                       **
**                                                                            **
*******************************************************************************/
#ifndef WDGIF_TYPES_H
#define WDGIF_TYPES_H

/*===========================[R E V I S I O N   H I S T O R Y]====================================*/
/*================================================================================================*/

/*===========================[V E R S I O N  I N F O R M A T I O N]===============================*/
/*===========================[I N C L U D E S]====================================================*/
/*===========================[M A C R O S]========================================================*/
/*===========================[T Y P E   D E F I N I T I O N S]====================================*/
/* Type of WDG Mode */
typedef enum
{
    /* OFF Mode */
    WDGIF_OFF_MODE = 0u,
    /* SLOW Mode */
    WDGIF_SLOW_MODE,
    /* FAST Mode */
    WDGIF_FAST_MODE
} WdgIf_ModeType;

/*===========================[E X T E R N A L   D A T A]==========================================*/
/*===========================[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

#endif /* WDGIF_TYPES_H */
