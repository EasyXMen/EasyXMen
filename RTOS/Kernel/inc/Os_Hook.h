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
 ************************************************************************************************************************
 **
 **  @file               : Os_Hook.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for hook api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_HOOK_H
#define OS_HOOK_H

/* =================================================== inclusions =================================================== */
#include "Os_Types.h"
#include "Os_Rti.h"

/* ===================================================== macros ===================================================== */
/**
 * @brief If ErrorHook is configured, save the service ID and call ErrorHook when an error occurs.
 *
 * Macro parameters:
 *  @param[in] x: Execute the incoming function.
 *  @param[in] serviceId: The service ID when an error occurs.
 *  @param[in] err: Error code.
 *  @param[in] pScb: Point to System Control Block.
 */
#if (CFG_ERRORHOOK == TRUE)
/* PRQA S 3458 ++ *//* VL_Os_3458 */
#define Os_TraceErrorHook(x, serviceId, err, pScb) \
    {                                              \
        x;                                         \
        OSErrorSaveServiceId(serviceId);           \
        OSRtiSaveErrorStatus(err);                 \
        Os_FaultSave(pScb, err);                   \
        Os_CallErrorHook(err);                     \
    }
/* PRQA S 3458 -- */
#else
#define Os_TraceErrorHook(x, serviceId, err, pScb)
#endif

/**
 * @brief If ErrorHook is configured, call ErrorHook when an error occurs.
 *
 * Macro parameters:
 *  @param[in] err: Error code.
 *  @param[in] pScb: Point to System Control Block.
 */
#if (CFG_ERRORHOOK == TRUE)
/* PRQA S 3458, 3472 ++ *//* VL_Os_3458, VL_Os_3472 */
#define Os_ErrorHook(err)            \
    {                                \
        Os_CallErrorHook(err);       \
    }
/* PRQA S 3458, 3472 -- */
#else
#define Os_ErrorHook(err)
#endif

/**
 * @brief If ShutdownHook is configured, call ShutdownHook when an error occurs.
 *
 * Macro parameters:
 *  @param[in] err: Error code.
 *  @param[in] pScb: Point to System Control Block.
 */
#if (TRUE == CFG_SHUTDOWNHOOK)
/* PRQA S 3458 ++ *//* VL_Os_3458 */
#define Os_ShutdownHook(pScb, err)      \
    {                                   \
        Os_CallShutdownHook(pScb, err); \
    }
/* PRQA S 3458 -- */
#else
#define Os_ShutdownHook(pScb, err)
#endif

/**
 * @brief If PostTaskHook is configured, call PostTaskHook after task scheduling.
 *
 * Macro parameters:
 *  @param[in] pScb: Point to System Control Block.
 */
#if (TRUE == CFG_POSTTASKHOOK)
/* PRQA S 3472, 3458 ++ */ /* VL_Os_3472, VL_Os_3458 */
#define Os_PostTaskHook(pScb)      \
    {                              \
        Os_CallPostTaskHook(pScb); \
    }
/* PRQA S 3472, 3458 -- */
#else
#define Os_PostTaskHook(pScb)
#endif

/**
 * @brief If PreTaskHook is configured, call PreTaskHook before task scheduling.
 *
 * Macro parameters:
 *  @param[in] pScb: Point to System Control Block.
 */
#if (TRUE == CFG_PRETASKHOOK)
/* PRQA S 3472, 3458 ++ */ /* VL_Os_3472, VL_Os_3458 */
#define Os_PreTaskHook(pScb)      \
    {                             \
        Os_CallPreTaskHook(pScb); \
    }
/* PRQA S 3472, 3458 -- */
#else
#define Os_PreTaskHook(pScb)
#endif

/**
 * @brief If StartupHook is configured, call StartupHook when starting the OS.
 *
 * Macro parameters:
 *  @param[in] pScb: Point to System Control Block.
 */
#if (TRUE == CFG_STARTUPHOOK)
/* PRQA S 3458, 3472 ++ *//* VL_Os_3458, VL_Os_3472 */
#define Os_StartupHook(pScb)      \
    {                             \
        Os_CallStartupHook(pScb); \
    }
/* PRQA S 3458, 3472 -- */
#else
#define Os_StartupHook(pScb)
#endif

/* ========================================== external function definitions ========================================= */
#if (TRUE == CFG_ERRORHOOK)
/**
 * @brief              Call the error hook function.
 * @param[in]          error: Error code.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_CallErrorHook(StatusType error);
#endif

#if (TRUE == CFG_PRETASKHOOK)
/**
 * @brief              Call the PreTask hook function.
 * @param[in]          pScb: Point to System Control Block.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_CallPreTaskHook(Os_SCBType *pScb);
#endif

#if (TRUE == CFG_POSTTASKHOOK)
/**
 * @brief              Call the PostTask hook function.
 * @param[in]          pScb: Point to System Control Block.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_CallPostTaskHook(Os_SCBType *pScb);
#endif

#if (TRUE == CFG_SHUTDOWNHOOK)
/**
 * @brief              Call the Shutdown hook function.
 * @param[in]          pScb: Point to System Control Block.
 * @param[in]          error: Error code.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_CallShutdownHook(Os_SCBType *pScb, StatusType error);
#endif

#if (TRUE == CFG_STARTUPHOOK)
/**
 * @brief              Call the Startup hook function.
 * @param[in]          pScb: Point to System Control Block.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_CallStartupHook(Os_SCBType *pScb);
#endif

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_APPL_SHUTDOWNHOOK)
/**
 * @brief              Call Appl's Shutdown hook function.
 * @param[in]          pScb: Point to System Control Block.
 * @param[in]          fatalError: Error code.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_ApplShutdownHook(Os_SCBType *pScb, StatusType fatalError);
#endif

#if (TRUE == CFG_APPL_STARTUPHOOK)
/**
 * @brief              Call Appl's startup hook function.
 * @param[in]          pScb: Point to System Control Block.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_ApplStartupHook(Os_SCBType *pScb);
#endif
#endif

#if (TRUE == CFG_ERRORHOOK)
/**
 * @brief           This hook routine is called  by the operating system  at
 *                  the end of a system service which return s StatusType not
 *                  equal E_OK. It is called before returning to the task level.
 * @param[in]       Error: Error code.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void ErrorHook(StatusType Error); /* PRQA S 1753 */ /* VL_Os_1753 */
#endif

#if (TRUE == CFG_PRETASKHOOK)
/**
 * @brief           This hook routine is  called by the operat ing system before
 *                  executing a new task, but after t he transition of the task to
 *                  the running state (to allow evaluation of the TaskID by GetTaskID).
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void PreTaskHook(void); /* PRQA S 1753 */ /* VL_Os_1753 */
#endif

/**
 * @brief           This hook routine is called by the operating system after
 *                  executing the current task, but before leaving the task's
 *                  running state (to allow evaluation of the TaskID by GetTaskID).
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
#if (TRUE == CFG_POSTTASKHOOK)
extern void PostTaskHook(void); /* PRQA S 1753 */ /* VL_Os_1753 */
#endif

#if (TRUE == CFG_STARTUPHOOK)
/**
 * @brief           This hook routine is called  by the operating system at
 *                  the end of the operating system in itialisation and before
 *                  the scheduler is running. At this time the application
 *                  can initialise device drivers etc.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void StartupHook(void); /* PRQA S 1753 */ /* VL_Os_1753 */
#endif

#if (TRUE == CFG_SHUTDOWNHOOK)
/**
 * @brief           This hook routine is ca lled by the operating system when
 *                  the OS service ShutdownOS  has been called. This routine is
 *                  called during the operating system shut down.
 * @param[in]       Error: Error code.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void ShutdownHook(StatusType Error); /* PRQA S 1753 */ /* VL_Os_1753 */
#endif

#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_PROTECTIONHOOK)
/**
 * @brief           The protection hook is always called if a serious error occurs. E.g. exceeding the worst case
                    execution time or violating against the memory protection.
 * @param[in]       Fatalerror: The error which caused the call to the protection hook.
 * @return          ProtectionReturnType
 * @retval          PRO_IGNORE: Ignore this error.
 * @retval          PRO_TERMINATETASKISR: Terminate task or Isr.
 * @retval          PRO_TERMINATEAPPL: Terminate application.
 * @retval          PRO_TERMINATEAPPL_RESTART: Terminate application and restart it.
 * @retval          PRO_SHUTDOWN: Shutdown Os.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           CPD-57666
 */
extern ProtectionReturnType ProtectionHook(StatusType Fatalerror); /* PRQA S 1753 */ /* VL_Os_1753 */
#endif
#endif

#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
/**
 * @brief           The FalutManager hook is HOOK for post-failure user handling (e.g., printing fault information).
 * @param[in]       Error: Points to the system's fault information.
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
extern void FaultManagerHook(const ExceptionMsgType *Error); /* PRQA S 1753 */ /* VL_Os_1753 */
#endif

#if (TRUE == CFG_EVENT_RESPONSE_TIME_HOOK) /* PRQA S 3332 */ /* VL_Os_3332 */
extern void EventResponseTimeHook(Os_TaskType taskId, Os_EventMaskType eventMask, Os_TickType respTime); /* PRQA S 1753 */ /* VL_Os_1753 */
#endif

#if (TRUE == CFG_EVENT_RESPONSE_RATE_HOOK) /* PRQA S 3332 */ /* VL_Os_3332 */
extern void EventResponseRateHook(Os_TaskType taskId, Os_EventMaskType eventMask, uint16 respNum); /* PRQA S 1753 */ /* VL_Os_1753 */
#endif

#endif

/*=======[E N D   O F   F I L E]==============================================*/
