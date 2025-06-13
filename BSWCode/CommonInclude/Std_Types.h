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
**  FILENAME  : Std_Types.h                                                   **
**                                                                            **
**                                                                            **
**  PLATFORM  : Infineon AURIX                                                **
**                                                                            **
**  AUTHOR    :                                                               **
**                                                                            **
**  VENDOR    :                                                               **
**                                                                            **
**  DESCRIPTION: Provision of Standard Types                                  **
**                                                                            **
**                                                                            **
**   SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                            **
**                                                                            **
*******************************************************************************/
#ifndef STD_TYPES_H
#define STD_TYPES_H

#include "Compiler.h"
#include "Platform_Types.h"

typedef uint8 Std_ReturnType;

#ifndef STATUSTYPEDEFINED
#define STATUSTYPEDEFINED
#define E_OK 0x00u
typedef unsigned char StatusType; /* OSEK compliance */
#endif
#define E_NOT_OK 0x01u
#define E_BUSY   0x02u

typedef struct
{
    uint16 vendorID;
    uint16 moduleID;

    uint8 sw_major_version;
    uint8 sw_minor_version;
    uint8 sw_patch_version;
} Std_VersionInfoType;

#define STD_HIGH   0x01u /* Physical state 5V or 3.3V */

#define STD_LOW    0x00u /* Physical state 0V */

#define STD_ACTIVE 0x01u /* Logical state active */

#define STD_IDLE   0x00u /* Logical state idle */

#define STD_ON     0x01u

#define STD_OFF    0x00u

/**
 * @brief     The values are specified for eachtransformer class in ASWS_TransformerGeneral.
 */
typedef uint8 Std_TransformerErrorCode;

/**
 * @brief     Std_TransformerError represents a transformer error in the context of a certain transformer chain
 */
typedef uint8 Std_TransformerClass;
#define STD_TRANSFORMER_UNSPECIFIED 0x00
#define STD_TRANSFORMER_SERIALIZER  0x01
#define STD_TRANSFORMER_SAFETY      0x02
#define STD_TRANSFORMER_SECURITY    0x03
#define STD_TRANSFORMER_CUSTOM      0xFF

/**
 * @brief     Std_TransformerError represents a transformer error in the context of a certain transformer chain.
 */
typedef struct
{
    Std_TransformerErrorCode errorCode;
    Std_TransformerClass transformerClass;
} Std_TransformerError;

/**
 * @brief     The data type Std_TransformerForwardCode represents a forwarded transformer
 *            codein the context of a certain transformer chain
 */
typedef uint8 Std_TransformerForwardCode;
#define E_SAFETY_INVALID_REP 0x01
#define E_SAFETY_INVALID_CRC 0x02
#define E_SAFETY_INVALID_SEQ 0x03

#endif /* STD_TYPES_H */
