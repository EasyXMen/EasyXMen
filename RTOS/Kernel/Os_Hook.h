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
**  FILENAME    : Os_Hook.h                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : AutoSar Hook Managment                                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

#ifndef OS_HOOK_H
#define OS_HOOK_H

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/

/*=======[I N C L U D E S]====================================================*/
#include "Os.h"

/*=======[M A C R O S]========================================================*/
#if (CFG_ERRORHOOK == TRUE)
#define Os_TraceErrorHook(x, OSSrvID, err) \
    {                                      \
        x;                                 \
        OSError_Save_ServiceId(OSSrvID);   \
        Os_CallErrorHook(err);             \
    }
#else
#define Os_TraceErrorHook(x, OSSrvID, err)
#endif

#if (CFG_ERRORHOOK == TRUE)
/* PRQA S 3472 ++*/ /* MISRA  Dir-4.9*/ /* OS_HOOK_MACRO_001 */
#define Os_ErrorHook(err)      \
    {                          \
        Os_CallErrorHook(err); \
    }
/* PRQA S 3472 --*/ /* MISRA  Dir-4.9*/
#else
#define Os_ErrorHook(err)
#endif

#if (TRUE == CFG_SHUTDOWNHOOK)
#define Os_ShutdownHook(err)      \
    {                             \
        Os_CallShutdownHook(err); \
    }
#else
#define Os_ShutdownHook(err)
#endif

#if (TRUE == CFG_POSTTASKHOOK)
#define Os_PostTaskHook()      \
    {                          \
        Os_CallPostTaskHook(); \
    } /* PRQA S 3472 */ /* MISRA  Dir-4.9*/ /* OS_HOOK_MACRO_001 */
#else
#define Os_PostTaskHook()
#endif

#if (TRUE == CFG_PRETASKHOOK)
#define Os_PreTaskHook()      \
    {                         \
        Os_CallPreTaskHook(); \
    } /* PRQA S 3472 */ /* MISRA  Dir-4.9*/ /* OS_HOOK_MACRO_001 */
#else
#define Os_PreTaskHook()
#endif

#if (TRUE == CFG_STARTUPHOOK)
#define Os_StartupHook()      \
    {                         \
        Os_CallStartupHook(); \
    } /* PRQA S 3472 */ /* MISRA  Dir-4.9*/ /* OS_HOOK_MACRO_004 */
#else
#define Os_StartupHook()
#endif

#if (TRUE == CFG_PROTECTIONHOOK)
#define Os_ProtectionHook(err, who) Os_CallProtectionHook(err, who)
#else
#define Os_ProtectionHook(err, who)
#endif

#endif /* OS_HOOK_H */

/*=======[E N D   O F   F I L E]==============================================*/
