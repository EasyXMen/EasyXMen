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
 ************************************************************************************************************************
 ** **
 **  @file               : ComM_Version.h **
 **  @version            : V1.0.0 **
 **  @author             : darren.zhang **
 **  @date               : 2022/01/07 **
 **  @vendor             : isoft **
 **  @description        : Communication Manager **
 **  @specification(s)   : AUTOSAR classic Platform R19-11 **
 ** **
 ***********************************************************************************************************************/

#ifndef COMM_VERSION_H_
#define COMM_VERSION_H_

/*=================================================[inclusions]=======================================================*/

/*====================================================[macros]========================================================*/

/*------------------------------------------[AUTOSAR vendor identification]-------------------------------------------*/

#if (defined COMM_VENDOR_ID)
#error COMM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: isoft software */
#define COMM_VENDOR_ID 62u

/*-----------------------------------------[AUTOSAR module identification]--------------------------------------------*/

#if (defined COMM_MODULE_ID)
#error COMM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define COMM_MODULE_ID 12U

#if (defined COMM_INSTANCE_ID)
#error COMM_INSTANCE_ID already defined
#else
#define COMM_INSTANCE_ID 0u
#endif
/*--------------------------------------[AUTOSAR release version identification]--------------------------------------*/

#if (defined COMM_AR_RELEASE_MAJOR_VERSION)
#error COMM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define COMM_AR_RELEASE_MAJOR_VERSION 4u

#if (defined COMM_AR_RELEASE_MINOR_VERSION)
#error COMM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define COMM_AR_RELEASE_MINOR_VERSION 5u

#if (defined COMM_AR_RELEASE_REVISION_VERSION)
#error COMM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define COMM_AR_RELEASE_REVISION_VERSION 0u

/*-------------------------------------[AUTOSAR module version identification]----------------------------------------*/

#if (defined COMM_SW_MAJOR_VERSION)
#error COMM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define COMM_SW_MAJOR_VERSION 2u

#if (defined COMM_SW_MINOR_VERSION)
#error COMM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define COMM_SW_MINOR_VERSION 1u

#if (defined COMM_SW_PATCH_VERSION)
#error COMM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define COMM_SW_PATCH_VERSION 14u

/*----------------------------------[AUTOSAR config depend on version identification]---------------------------------*/
#if (defined COMM_SW_CFG_DEP_MAJOR_VERSION)
#error COMM_SW_CFG_DEP_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module depend on configure major version */
#define COMM_SW_CFG_DEP_MAJOR_VERSION 2u

#if (defined COMM_SW_CFG_DEP_MINOR_VERSION)
#error COMM_SW_CFG_DEP_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module depend on configure minor version */
#define COMM_SW_CFG_DEP_MINOR_VERSION 1u

#if (defined COMM_SW_CFG_DEP_PATCH_VERSION)
#error COMM_SW_CFG_DEP_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module depend on configure minor patch version */
#define COMM_SW_CFG_DEP_PATCH_VERSION 10u

/*=============================================[type definitions]=====================================================*/

/*========================================[external function declarations]============================================*/

/*=========================================[internal function declarations]===========================================*/

/*==============================================[external constants]==================================================*/

/*==============================================[internal constants]==================================================*/

/*=================================================[external data]====================================================*/

/*=================================================[internal data]====================================================*/

/*==========================================[external function definitions]===========================================*/

/*==========================================[internal function definitions]===========================================*/

#endif /* COMM_VERSION_H_ */
