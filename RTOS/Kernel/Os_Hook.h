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
/* PRQA S 3108-- */

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
/* PRQA S 3472 ++ */ /* MISRA  Dir-4.9*/ /* OS_HOOK_MACRO_001 */
#define Os_ErrorHook(err)      \
    {                          \
        Os_CallErrorHook(err); \
    }
/* PRQA S 3472 -- */ /* MISRA  Dir-4.9*/
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
/* PRQA S 3472 ++ */ /* MISRA  Dir-4.9*/ /* OS_HOOK_MACRO_001 */
#define Os_PostTaskHook()      \
    {                          \
        Os_CallPostTaskHook(); \
    }
/* PRQA S 3472 -- */ /* MISRA  Dir-4.9*/
#else
#define Os_PostTaskHook()
#endif

#if (TRUE == CFG_PRETASKHOOK)
/* PRQA S 3472 ++ */ /* MISRA  Dir-4.9*/ /* OS_HOOK_MACRO_001 */
#define Os_PreTaskHook()      \
    {                         \
        Os_CallPreTaskHook(); \
    }
/* PRQA S 3472 -- */ /* MISRA  Dir-4.9*/
#else
#define Os_PreTaskHook()
#endif

#if (TRUE == CFG_STARTUPHOOK)
/* PRQA S 3472 ++ */ /* MISRA  Dir-4.9*/ /* OS_HOOK_MACRO_004 */
#define Os_StartupHook()      \
    {                         \
        Os_CallStartupHook(); \
    }
/* PRQA S 3472 -- */ /* MISRA  Dir-4.9*/
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
