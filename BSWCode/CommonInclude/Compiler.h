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
**   $FILENAME   : Compiler.h $                                               **
**                                                                            **
**   $CC VERSION : highTec 1.6.1                                              **
**                                                                            **
**   VENDOR      :                                                            **
**                                                                            **
**   DESCRIPTION :Provider of compiler specific (non-ANSI) keywords.          **
**              All mappings of keywords which are  not standardized and/or   **
**              compiler specific shall be placed  and organized in this      **
**              compiler specific header. The file is  needed to decouple     **
**              the code of all standard core components from compiler        **
**              topics and thus enhance portability.                          **
**                                                                            **
**                                                                            **
**   SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                            **
**                                                                            **
*******************************************************************************/
#ifndef COMPILER_H
#define COMPILER_H

#include "Compiler_Cfg.h"

/*target compiler*/
#define _GCC_C_TRICORE_

/*
 * File version information
 */
#define COMPILER_MAJOR_VERSION 1
#define COMPILER_MINOR_VERSION 6
#define COMPILER_PATCH_VERSION 1

/* The compiler abstraction shall provide the NULL_PTR
   define with a void pointer to zero definition.
 */
#ifndef NULL_PTR
#define NULL_PTR ((void*)0)
#endif

/* The compiler abstraction shall provide the INLINE define
   for abstraction of the keyword inline.
 */
#define INLINE

/* The compiler abstraction shall provide the LOCAL_INLINE define for abstraction
   of the keyword inline in functions with 鈥渟tatic鈥� scope.
 */
#define LOCAL_INLINE

/* used for the declaration of local pointers */
#define AUTOMATIC

/* used for the declaration of type pointers */
#define TYPEDEF

/* @req COMPILER049 */
#define STATIC static

/* The compiler abstraction shall define the FUNC macro for the declaration and
   definition of functions, that ensures correct syntax of function declarations
   as required by a specific compiler. - used for API functions
   rettype     return type of the function
   memclass    classification of the function itself
               (not used on 32bit platforms)
 */
#define FUNC(rettype, memclass) rettype

/* The compiler abstraction shall define the FUNC_P2CONST macro for the declaration
   and definition of functions returning a pointer to a constant. This shall ensure
   the correct syntax of function declarations as required by a specific compiler.
   rettype     return type of the function
   ptrclass    defines the classification of the pointer's distance
               (not used on 32bit platforms)
   memclass    classification of the function itself
               (not used on 32bit platforms)
 */
#define FUNC_P2CONST(rettype, ptrclass, memclass) const rettype*

/* The compiler abstraction shall define the FUNC_P2VAR macro for the declaration
   and definition of functions returning a pointer to a variable. This shall ensure
   the correct syntax of function declarations as required by a specific compiler.
   rettype     return type of the function
   ptrclass    defines the classification of the pointer's distance
               (not used on 32bit platforms)
   memclass    classification of the function itself
               (not used on 32bit platforms)
 */
#define FUNC_P2VAR(rettype, ptrclass, memclass) rettype*

/* Pointer to variable data
   ptrtype     type of the referenced data
   memclass    classification of the pointer's variable itself
               (not used on 32bit platforms)
   ptrclass    defines the classification of the pointer's distance
               (not used on 32bit platforms)
 */
#define P2VAR(ptrtype, memclass, ptrclass) ptrtype*

/* Pointer to constant data
   ptrtype     type of the referenced data
   memclass    classification of the pointer's variable itself
               (not used on 32bit platforms)
   ptrclass    defines the classification of the pointer's distance
               (not used on 32bit platforms)
 */
#define P2CONST(ptrtype, memclass, ptrclass) const ptrtype*

/* Const pointer to variable data
   ptrtype     type of the referenced data
   memclass    classification of the pointer's variable itself
               (not used on 32bit platforms)
   ptrclass    defines the classification of the pointer's distance
               (not used on 32bit platforms)
 */
#define CONSTP2VAR(ptrtype, memclass, ptrclass) ptrtype* const

/* Const pointer to constant data
   ptrtype     type of the referenced data
   memclass    classification of the pointer's variable itself
               (not used on 32bit platforms)
   ptrclass    defines the classification of the pointer's distance
               (not used on 32bit platforms)
 */
#define CONSTP2CONST(ptrtype, memclass, ptrclass) const ptrtype* const

/* Type definition of pointers to functions
   rettype     return type of the function
   ptrclass    defines the classification of the pointer's distance
               (not used on 32bit platforms)
   fctname     function name respectivly name of the defined type
 */
#define P2FUNC(rettype, ptrclass, fctname) rettype(*fctname)

/* Type definition of constant pointers to functions
   rettype     return type of the function
   ptrclass    defines the classification of the pointer's distance
               (not used on 32bit platforms)
   fctname     function name respectivly name of the defined type
 */
#define CONSTP2FUNC(rettype, ptrclass, fctname) rettype(*const fctname)

/* ROM constant
   consttype   type of the constant
   memclass    classification of the constant
               (not used on 32bit platforms)
 */
#define CONST(consttype, memclass) const consttype

/* RAM variables
   vartype     type of the variable
   memclass    classification of the variable
               (not used on 32bit platforms)
 */
#define VAR(vartype, memclass) vartype

#endif /* end of COMPILER_H */

/*=======[E N D   O F   F I L E]==============================================*/
