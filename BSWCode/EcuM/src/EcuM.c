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
 **  @file               : EcuM.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : ECU state management
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "EcuM_Internal.h"
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */
#if (ECUM_MODE_HANDING == STD_ON)
#define ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"
/* Current Rte State to notify during MainFunction */
Rte_ModeType_EcuM_Mode EcuM_RteState;
#define ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"
#endif
#if (ECUM_CLONE_VARIABLE_SUPPORT_ENABLED == STD_ON)
#define ECUM_START_SEC_VAR_CLONE_UNSPECIFIED
#include "EcuM_MemMap.h"
EcuM_RunTimeLcType EcuMRunData;
#define ECUM_STOP_SEC_VAR_CLONE_UNSPECIFIED
#include "EcuM_MemMap.h"
#endif /* ECUM_CLONE_VARIABLE_SUPPORT_ENABLED == STD_ON */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#if (ECUM_VERSION_INFO_API == STD_ON)
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/**
 * Retrieves version information for the ECU Manager.
 *
 * This function fills the provided `versioninfo` structure with the module ID, vendor ID, and software version numbers.
 * If error detection is enabled, it checks for a null pointer and reports an error if the input is invalid.
 */
void EcuM_GetVersionInfo(Std_VersionInfoType* versioninfo) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    EcuM_ArtiApiStart(2, ECUM_SID_GETVERSION, versioninfo);

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == versioninfo)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETVERSION, ECUM_E_PARAM_POINTER);
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        versioninfo->moduleID         = ECUM_MODULE_ID;
        versioninfo->vendorID         = ECUM_VENDOR_ID;
        versioninfo->sw_major_version = ECUM_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = ECUM_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = ECUM_SW_PATCH_VERSION;
    }

    EcuM_ArtiApiStop(1, ECUM_SID_GETVERSION);
}
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
#endif /*ECUM_VERSION_INFO_API == STD_ON*/

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/**
 * Sets the state of the ECU Manager.
 *
 * This function checks if the ECU Manager is initialized and then sets the requested state.
 * If mode handling is enabled, it maps the requested state to the corresponding RTE mode.
 * For multi-core systems, it handles the state change on the master and slave cores appropriately.
 * If the requested state is out of range or the ECU Manager is not initialized, it reports an error.
 */
void EcuM_SetState(EcuM_StateType state) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    EcuM_ArtiApiStart(2, ECUM_SID_SETSTATE, state);

#if (ECUM_MODE_HANDING == STD_ON)
    Rte_ModeType_EcuM_Mode mode;
    boolean                errFlag = FALSE; /*PRQA S 2995*/ /* VL_EcuM_2995 */
#endif
    EcuM_RunTimeLcType* pRt;
    uint8               detErrorId = ECUM_E_OK;
    /* Init Check */
    if (EcuM_isInitState(&pRt))
    {
#if (ECUM_MODE_HANDING == STD_ON)
        switch (state)
        {
        case ECUM_STATE_STARTUP:
            mode = RTE_MODE_EcuM_Mode_STARTUP;
            break;

        case ECUM_STATE_APP_RUN:
            mode = RTE_MODE_EcuM_Mode_RUN;
            break;

        case ECUM_STATE_APP_POST_RUN:
            mode = RTE_MODE_EcuM_Mode_POST_RUN;
            break;

        case ECUM_STATE_SHUTDOWN:
            /*Same with sleep.*/
        case ECUM_STATE_SLEEP:
            if (pRt->SdtgNext->Target == ECUM_SHUTDOWN_TARGET_SLEEP)
            {
                mode = RTE_MODE_EcuM_Mode_SLEEP;
            }
            else
            {
                mode = RTE_MODE_EcuM_Mode_SHUTDOWN;
            }
            break;

        default:
            detErrorId = ECUM_E_STATE_PAR_OUT_OF_RANGE;
            errFlag    = TRUE;
            break;
        }
        if ((boolean)errFlag == FALSE) /* PRQA S 2991,2995 */ /* VL_EcuM_2991,VL_EcuM_2995 */
#endif                                                        /*ECUM_MODE_HANDING == STD_ON*/
        {
#if (ECUM_MODE_HANDING == STD_ON)
#if (ECUM_MAX_MCU_CORE_NUM > 1)
            /*get core ID for this running core*/
            CoreIdType coreId = GetCoreID();
            if (ECUM_MASTER_CORE_ID != coreId)
            {
                pRt->State = state;
                BswM_EcuM_CurrentState(pRt->State);
            }
            else
#endif /* ECUM_MAX_MCU_CORE_NUM > 1 */
            {
                EcuM_RteState  = mode;
                pRt->rqstState = state;
            }
#else
            if ((state == ECUM_STATE_STARTUP) || (state == ECUM_STATE_APP_RUN) || (state == ECUM_STATE_APP_POST_RUN)
                || (state == ECUM_STATE_SHUTDOWN) || (state == ECUM_STATE_SLEEP))
            {
                pRt->State = state;
                BswM_EcuM_CurrentState(pRt->State);
            }
            else
            {
                detErrorId = ECUM_E_STATE_PAR_OUT_OF_RANGE;
            }
#endif
        }
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        detErrorId = ECUM_E_UNINIT;
    }

    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SETSTATE, detErrorId);
    }
#else
    (void)detErrorId;
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */

    EcuM_ArtiApiStop(1, ECUM_SID_SETSTATE);
}
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#if (STD_ON == ECUM_MODE_HANDING)
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/* PRQA S 1503, 2844 ++ */ /* VL_QAC_NoUsedApi, VL_EcuM_DerefInvalidPtr */
/**
 * Requests the ECU Manager to enter the RUN state.
 *
 * This function checks if the ECU Manager is initialized and then searches for the specified user.
 * If the user is found and not already in the RUN state, it sets the user to the RUN state and increments the run
 * count. If the run count reaches zero, it requests the ECU Manager to enter the APP_RUN state. If the user is already
 * in the RUN state or the user is invalid, it reports an error. If the ECU Manager is not initialized, it also reports
 * an error.
 */
Std_ReturnType EcuM_RequestRUN(EcuM_UserType user)
{
    EcuM_ArtiApiStart(2, ECUM_SID_REQUESTRUN, user);

    uint8               uid;
    Std_ReturnType      ret = E_OK;
    EcuM_RunTimeLcType* pRt;
    uint8               detErrorId = ECUM_E_OK;

    if (EcuM_isInitState(&pRt))
    {
        ret = EcuM_SearchUserByCaller(user, &uid);

        if (E_OK == ret)
        {
            if (FALSE == pRt->Users.IsRun[uid])
            {
                pRt->Users.IsRun[uid] = TRUE;
                if (pRt->Users.RunNum == 0u)
                {
                    BswM_EcuM_RequestedState(ECUM_STATE_APP_RUN, ECUM_RUNSTATUS_REQUESTED);
                }
                pRt->Users.RunNum = pRt->Users.RunNum + 1u;
            }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
            else
            {
                detErrorId = ECUM_E_MULTIPLE_RUN_REQUESTS;
                ret        = E_NOT_OK;
            }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
        }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
        else
        {
            detErrorId = ECUM_E_INVALID_PAR;
            ret        = E_NOT_OK;
        }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        detErrorId = ECUM_E_UNINIT;
        ret        = E_NOT_OK;
    }

    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_REQUESTRUN, detErrorId);
    }
#else
    (void)detErrorId;
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */

    EcuM_ArtiApiStop(2, ECUM_SID_REQUESTRUN, ret);

    return ret;
}
/* PRQA S 1503, 2844 -- */
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/* PRQA S 1503, 2844 ++ */ /* VL_QAC_NoUsedApi, VL_EcuM_DerefInvalidPtr */
/**
 * Releases the RUN state for the specified user in the ECU Manager.
 *
 * This function checks if the ECU Manager is initialized and then searches for the specified user.
 * If the user is found and currently in the RUN state, it sets the user to the non-RUN state and decrements the run
 * count. If the run count reaches zero, it requests the ECU Manager to release the APP_RUN state. If the user is not in
 * the RUN state or the user is invalid, it reports an error. If the ECU Manager is not initialized, it also reports an
 * error.
 */
Std_ReturnType EcuM_ReleaseRUN(EcuM_UserType user)
{
    EcuM_ArtiApiStart(2, ECUM_SID_RELEASERUN, user);

    uint8               uid;
    Std_ReturnType      ret = E_OK;
    EcuM_RunTimeLcType* pRt;
    uint8               detErrorId = ECUM_E_OK;

    if (EcuM_isInitState(&pRt))
    {
        ret = EcuM_SearchUserByCaller(user, &uid);

        if (E_OK == ret)
        {
            if (TRUE == pRt->Users.IsRun[uid])
            {
                pRt->Users.IsRun[uid] = FALSE;
                pRt->Users.RunNum     = pRt->Users.RunNum - 1u;
                if (pRt->Users.RunNum == 0u)
                {
                    BswM_EcuM_RequestedState(ECUM_STATE_APP_RUN, ECUM_RUNSTATUS_RELEASED);
                }
            }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
            else
            {
                detErrorId = ECUM_E_MISMATCHED_RUN_RELEASE;
                ret        = E_NOT_OK;
            }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
        }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
        else
        {
            detErrorId = ECUM_E_INVALID_PAR;
            ret        = E_NOT_OK;
        }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        detErrorId = ECUM_E_UNINIT;
        ret        = E_NOT_OK;
    }

    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_RELEASERUN, detErrorId);
    }
#else
    (void)detErrorId;
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */

    EcuM_ArtiApiStop(2, ECUM_SID_RELEASERUN, ret);

    return ret;
}
/* PRQA S 1503, 2844 -- */
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/* PRQA S 1503, 2844 ++ */ /* VL_QAC_NoUsedApi, VL_EcuM_DerefInvalidPtr */
/**
 * Requests the ECU Manager to enter the POST_RUN state for the specified user.
 *
 * This function checks if the ECU Manager is initialized and then searches for the specified user.
 * If the user is found and not already in the POST_RUN state, it sets the user to the POST_RUN state and increments the
 * post-run count. If the post-run count reaches one, it requests the ECU Manager to enter the APP_POST_RUN state. If
 * the user is already in the POST_RUN state or the user is invalid, it reports an error. If the ECU Manager is not
 * initialized, it also reports an error.
 */
Std_ReturnType EcuM_RequestPOST_RUN(EcuM_UserType user)
{
    EcuM_ArtiApiStart(2, ECUM_SID_REQUESTPOST_RUN, user);

    uint8               uid;
    Std_ReturnType      ret = E_OK;
    EcuM_RunTimeLcType* pRt;
    uint8               detErrorId = ECUM_E_OK;

    if (EcuM_isInitState(&pRt))
    {
        ret = EcuM_SearchUserByCaller(user, &uid);

        if (E_OK == ret)
        {
            if (FALSE == pRt->Users.IsPostRun[uid])
            {
                pRt->Users.IsPostRun[uid] = TRUE;
                if (pRt->Users.PostRunNum == 0u)
                {
                    BswM_EcuM_RequestedState(ECUM_STATE_APP_POST_RUN, ECUM_RUNSTATUS_REQUESTED);
                }
                pRt->Users.PostRunNum = pRt->Users.PostRunNum + 1u;
            }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
            else
            {
                detErrorId = ECUM_E_MULTIPLE_RUN_REQUESTS;
                ret        = E_NOT_OK;
            }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
        }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
        else
        {
            detErrorId = ECUM_E_INVALID_PAR;
            ret        = E_NOT_OK;
        }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        detErrorId = ECUM_E_UNINIT;
        ret        = E_NOT_OK;
    }

    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_REQUESTPOST_RUN, detErrorId);
    }
#else
    (void)detErrorId;
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */

    EcuM_ArtiApiStop(2, ECUM_SID_REQUESTPOST_RUN, ret);

    return ret;
}
/* PRQA S 1503, 2844 -- */
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/* PRQA S 1503, 2844 ++ */ /* VL_QAC_NoUsedApi, VL_EcuM_DerefInvalidPtr */
/**
 * Releases the POST_RUN state for the specified user in the ECU Manager.
 *
 * This function checks if the ECU Manager is initialized and then searches for the specified user.
 * If the user is found and currently in the POST_RUN state, it sets the user to the non-POST_RUN state and decrements
 * the post-run count. If the post-run count reaches zero, it requests the ECU Manager to release the APP_POST_RUN
 * state. If the user is not in the POST_RUN state or the user is invalid, it reports an error. If the ECU Manager is
 * not initialized, it also reports an error.
 */
Std_ReturnType EcuM_ReleasePOST_RUN(EcuM_UserType user)
{
    EcuM_ArtiApiStart(2, ECUM_SID_RELEASEPOST_RUN, user);

    uint8               uid;
    Std_ReturnType      ret = E_OK;
    EcuM_RunTimeLcType* pRt;
    uint8               detErrorId = ECUM_E_OK;

    /* Init Check */
    if (EcuM_isInitState(&pRt))
    {
        ret = EcuM_SearchUserByCaller(user, &uid);

        if (E_OK == ret)
        {
            if (TRUE == pRt->Users.IsPostRun[uid])
            {
                pRt->Users.IsPostRun[uid] = FALSE;
                pRt->Users.PostRunNum     = pRt->Users.PostRunNum - 1u;
                if (pRt->Users.PostRunNum == 0u)
                {
                    BswM_EcuM_RequestedState(ECUM_STATE_APP_POST_RUN, ECUM_RUNSTATUS_RELEASED);
                }
            }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
            else
            {
                detErrorId = ECUM_E_MISMATCHED_RUN_RELEASE;
                ret        = E_NOT_OK;
            }
#endif /* ECUM_DEV_ERROR_DETECT == STD_ON */
        }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
        else
        {
            detErrorId = ECUM_E_INVALID_PAR;
            ret        = E_NOT_OK;
        }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        detErrorId = ECUM_E_UNINIT;
        ret        = E_NOT_OK;
    }
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_RELEASEPOST_RUN, detErrorId);
    }

#else
    (void)detErrorId;
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */

    EcuM_ArtiApiStop(2, ECUM_SID_RELEASEPOST_RUN, ret);

    return ret;
}
/* PRQA S 1503, 2844 -- */
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
#endif /*STD_ON == ECUM_MODE_HANDING*/

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/**
 * Selects the boot target for the ECU Manager.
 *
 * This function checks if the ECU Manager is initialized and then sets the specified boot target.
 * The boot target information is stored in a safe location to be evaluated by the boot manager after a reset.
 * If the ECU Manager is not initialized, it reports an error.
 */
Std_ReturnType EcuM_SelectBootTarget(EcuM_BootTargetType target) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    EcuM_ArtiApiStart(2, ECUM_SID_SELECTBOOTTARGET, target);

    Std_ReturnType      ret = E_OK;
    EcuM_RunTimeLcType* tempRunData;
    uint8               detErrorId = ECUM_E_OK;

    /* Init Check */
    if (EcuM_isInitState(&tempRunData))
    {
        /*EcuM2247 EcuM2835 */
        /*The implementer must ensure that the boot target information is placed at a safe location
          which then can be evaluated by the boot manager after a reset. */
        /* Check if target is within valid range */
        if (target <= ECUM_BOOT_TARGET_SYS_BOOTLOADER) /* Valid values are 0, 1, 2 */
        {
            tempRunData->BootTarget = target;
        }
        else
        {
            detErrorId = ECUM_E_INVALID_PAR;
            ret        = E_NOT_OK;
        }
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    else
    {
        detErrorId = ECUM_E_UNINIT;
        ret        = E_NOT_OK;
    }

    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SELECTBOOTTARGET, detErrorId);
    }
#else
    (void)detErrorId;
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */

    EcuM_ArtiApiStop(2, ECUM_SID_SELECTBOOTTARGET, ret);

    return ret;
}
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/**
 * Retrieves the current boot target set in the ECU Manager.
 *
 * This function checks if the ECU Manager is initialized and then retrieves the boot target.
 * If the ECU Manager is not initialized or the provided pointer is null, it reports an error.
 * Otherwise, it sets the provided pointer to the current boot target.
 */
Std_ReturnType EcuM_GetBootTarget(EcuM_BootTargetType* target) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    EcuM_ArtiApiStart(2, ECUM_SID_GETBOOTTARGET, target);

    Std_ReturnType      ret = E_NOT_OK;
    EcuM_RunTimeLcType* tempRunData;
    uint8               detErrorId = ECUM_E_OK;

    /* Init Check */

    if (!EcuM_isInitState(&tempRunData))
    {
        detErrorId = ECUM_E_UNINIT;
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    else if (NULL_PTR == target)
    {
        detErrorId = ECUM_E_PARAM_POINTER;
    }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */
    else
    {
        *target = tempRunData->BootTarget;
        ret     = E_OK;
    }

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETBOOTTARGET, detErrorId);
    }
#else
    (void)detErrorId;
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */

    EcuM_ArtiApiStop(2, ECUM_SID_GETBOOTTARGET, ret);

    return ret;
}
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#if (ECUM_MAX_MCU_CORE_NUM > 1)
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/**
 * Retrieves the index of the specified slave core ID.
 *
 * This function iterates through the list of slave core IDs and returns the index of the specified core ID.
 * If the core ID is not found, it returns the maximum core number.
 */
uint8 EcuM_GetSlaveCoreIndex(CoreIdType coreId)
{
    uint8 coreIdx = ECUM_MAX_MCU_CORE_NUM;
    uint8 index;

    for (index = 0u; index < (ECUM_MAX_MCU_CORE_NUM - 1u); index++)
    {
        if (EcuM_SlaveCoreIds[index] == coreId)
        {
            coreIdx = index;
            break;
        }
    }

    return coreIdx;
}
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
#endif /*ECUM_MAX_MCU_CORE_NUM > 1*/

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/* PRQA S 2814, 1532 ++ */ /* VL_QAC_MultiReturn, VL_QAC_OneFunRef */
/**
 * Searches for the user index based on the caller ID.
 *
 * This function iterates through the user configurations and returns the index of the user with the specified caller
 * ID. If the caller ID is not found, it returns an error.
 */
Std_ReturnType EcuM_SearchUserByCaller(EcuM_UserType caller, uint8* userIndex)
{
    uint8          index;
    boolean        flag = FALSE;
    Std_ReturnType ret;

    for (index = 0u; index < (uint8)ECUM_MAX_USER_NUM; index++) /* PRQA S 2877 */ /* VL_EcuM_2877 */
    {
        if ((uint16)EcuM_UserCfgs[index].usrId == caller)
        {
            flag = TRUE;
            break;
        }
    }
    if (flag)
    {
        *userIndex = index;
        ret        = E_OK;
    }
    else
    {
        ret = E_NOT_OK;
    }

    return ret;
}
/* PRQA S 2814, 1532 -- */
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Checks if the ECU Manager is in the initialized state.
 *
 * This function retrieves the runtime data pointer and checks if the ECU Manager has been initialized.
 * If the ECU Manager is initialized, it sets the provided pointer to the runtime data and returns true.
 * Otherwise, it returns false.
 */
boolean EcuM_isInitState(EcuM_RunTimeLcType** EcuMRunDataPtr)
{
    boolean             status = FALSE;
    EcuM_RunTimeLcType* tempRunDataPtr;
#if (ECUM_CLONE_VARIABLE_SUPPORT_ENABLED == STD_ON)
    status          = EcuMRunData.EcuM_IsInit;
    *EcuMRunDataPtr = &EcuMRunData;
#else  /* ECUM_CLONE_VARIABLE_SUPPORT_ENABLED == STD_OFF */
    tempRunDataPtr = EcuM_GetLcRunDataPtr();

    if (NULL_PTR != tempRunDataPtr)
    {
        *EcuMRunDataPtr = tempRunDataPtr;
        status          = tempRunDataPtr->EcuM_IsInit;
    }
#endif /* ECUM_CLONE_VARIABLE_SUPPORT_ENABLED == STD_ON */
    return status;
}
/* PRQA S 2814 -- */
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

/* ========================================== internal function definitions ========================================= */
