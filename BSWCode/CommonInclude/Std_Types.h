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
 ************************************************************************************************************************
 **
 **  @file               : Std_Types.h
 **  @author             : darren.zhang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Specification of Standard Types
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
*  REVISION HISTORY
*  -------------------------------------------------------------------------------------------------------------------
*  Version   Date        Author         Description
*  -------------------------------------------------------------------------------------------------------------------
*  V01.00.00 2025-03-05  darren.zhang        1. Update code according AUTOSAR R23-11 Specification
*
==================================================================================================================== */

#ifndef STD_TYPES_H
#define STD_TYPES_H

/* =================================================== inclusions =================================================== */
#if defined(BSW_USED_COMPILER_ABSTRACTION)
#include "Compiler.h"
#endif

#include "Platform_Types.h"

/* ===================================================== macros ===================================================== */

/* @req SWS_Std_00007 */
/**
 * @brief mcaro define
 */
#define STD_HIGH 0x01u /* Physical state 5V or 3.3V */
#define STD_LOW  0x00u /* Physical state 0V */

/* @req SWS_Std_00013 */
/**
 * @brief mcaro define
 */
#define STD_ACTIVE 0x01u /* Logical state active */
#define STD_IDLE   0x00u /* Logical state idle */

/* @req SWS_Std_00010 */
/**
 * @brief mcaro define
 */
#define STD_ON  0x01u
#define STD_OFF 0x00u

/* @req SWS_Std_00031 */
/**
 * @brief The implementation shall provide the NULL_PTR define with a void pointer to zero definition.
 */
#ifndef NULL_PTR
#define NULL_PTR ((void*)0)
#endif

#ifndef STATUSTYPEDEFINED
#define STATUSTYPEDEFINED
#define E_OK 0x00u
typedef unsigned char StatusType; /* OSEK compliance */
#endif
#if !defined(E_NOT_OK)
#define E_NOT_OK 0x01u
#endif
#if !defined(E_BUSY)
#define E_BUSY 0x02u
#endif

/* ================================================ type definitions ================================================ */

/* @req SWS_Std_00005 */
/**
 * @brief This type can be used as standard API return type which is shared between the RTE and the BSW modules
 */
typedef uint8 Std_ReturnType;

/* @req SWS_Std_00015 */
/**
 * @brief This type shall be used to request the version of a BSW module using the <Module name>_GetVersionInfo()
 * function.
 */
typedef struct
{
    uint16 vendorID;
    uint16 moduleID;

    uint8 sw_major_version;
    uint8 sw_minor_version;
    uint8 sw_patch_version;
} Std_VersionInfoType;

/* @req SWS_Std_00022 */
/**
 * @brief     The values are specified for eachtransformer class in ASWS_TransformerGeneral.
 */
typedef uint8 Std_TransformerErrorCode;

/* @req SWS_Std_00024 */
/**
 * @brief Std_TransformerError represents a transformer error in the context of a certain transformer chain
 */
typedef uint8 Std_TransformerClass;
#define STD_TRANSFORMER_UNSPECIFIED 0x00
#define STD_TRANSFORMER_SERIALIZER  0x01
#define STD_TRANSFORMER_SAFETY      0x02
#define STD_TRANSFORMER_SECURITY    0x03
#define STD_TRANSFORMER_CUSTOM      0xFF

/* @req SWS_Std_00021 */
/**
 * @brief Std_TransformerError represents a transformer error in the context of a certain transformer chain.
 */
typedef struct
{
    Std_TransformerErrorCode errorCode;
    Std_TransformerClass     transformerClass;
} Std_TransformerError;

/* @req SWS_Std_00028 SWS_Std_00029 */
/**
 * @brief     The data type Std_TransformerForwardCode represents a forwarded transformer
 *            codein the context of a certain transformer chain
 */
typedef uint8 Std_TransformerForwardCode;
#define E_SAFETY_INVALID_REP 0x01
#define E_SAFETY_INVALID_CRC 0x02
#define E_SAFETY_INVALID_SEQ 0x03

/* @req SWS_Std_91001 */
/**
 * @brief Type of the message (request/response)
 */
typedef uint8 Std_MessageTypeType;
#define STD_MESSAGETYPE_REQUEST  0x0u /* Message type for a request message */
#define STD_MESSAGETYPE_RESPONSE 0x1u /* Message type for a response message */

/* @req SWS_Std_91002 */
/**
 * @brief This type is used to encode different types of results for response messages. - Currently this encoding is
 * limited to the distinction between OK and ERROR responses.
 */
typedef uint8 Std_MessageResultType;
#define STD_MESSAGERESULT_OK    0x0u
#define STD_MESSAGERESULT_ERROR 0x1u

/* @req SWS_Std_91003 */
/**
 * @brief TType for the function pointer to extract the relevant protocol header fields of the message and the type of
 * the message result of a transformer.
 */
typedef Std_ReturnType (*Std_ExtractProtocolHeaderFieldsType)(
    const uint8*           buffer,
    uint32                 bufferLength,
    Std_MessageTypeType*   messageType,
    Std_MessageResultType* messageResult);

#endif /* STD_TYPES_H */
