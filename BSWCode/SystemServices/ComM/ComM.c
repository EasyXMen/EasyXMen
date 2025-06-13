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
 ************************************************************************************************************************
 ** **
 **  @file               : ComM.c **
 **  @version            : V2.1.4 **
 **  @author             : darren.zhang **
 **  @date               : 2017/01/01 **
 **  @vendor             : isoft **
 **  @description        : Communication Manager **
 **  @specification(s)   : AUTOSAR classic Platform R19-11 **
 ** **
 ***********************************************************************************************************************/

/*======================================================================================================================
 *                                       REVISION HISTORY
======================================================================================================================*/
/** <VERSION>  <DATE>    <AUTHOR>      <REVISION LOG>
 *  V1.0.0    20200113  zhengfei.li
        Initial version(link ff 1.30)
 *  V1.0.1    20200114  lili.wang
 *      1.Modify the function that calls network management in ComM_Internal.c is not encapsulated by the network
 * management macro switch. 2.Ethernet compatible. 3.Modified the problem that the 3e80 service request is cleared
 * before the network is successfully opened.
 *  V1.0.2    20200521  lili.wang
 *      1. QAC check code modify
 *  V1.0.3    20200522  lili.wang
 *      1. modify the bug:when one user ref to multiple channel or pnc,ComM_RequestComMode only set the first channel or
 * pnc's UserFullCounter++.It will lead to the other channel or pnc failed to switch modes.
 *  V2.0.0    20200723 lili.wang
 *      1. Update code according AUTOSAR R19-11 Specification
 *      2. Modify code with the new coding rule
 *      3. Fix bugs find in V1.0.3
 *  V2.0.1    20201019  lili.wang
 *      1. QAC check,code update
 *  V2.0.2    20201211  lili.wang
 *      1. cantata check,code update
 *  V2.0.3    20210201  lili.wang
 *      1. delete the macro switch COMM_PNC_SUPPORT of ComM_ManagedChannelHandler and ComM_ManagingChannelHandler
 *      2. Modify the timing mechanism for MainFunction timing.
 *      3. ComM_BusSM_ModeIndication parameter ComMode is converted from pointer to variable
 *      4. Add "Rte_ComM_Type.h"
 *  V2.0.4    20210510  lili.wang
 *      1. Code update after integration test
 *  V2.0.5    20210629  lili.wang
 *      1. Solve the bug that ComMPncPrepareSleepTimer is 0 and the Pnc state machine is maintained in the state of
 * COMM_PNC_PREPARE_SLEEP
 *  V2.0.6 20210707 lili.wang
 *      1. ComM_EnterNoComPending modify.
 *  V2.0.7    20210715  lili.wang
 *      1. Solve the bug if ComMTMinFullComModeDuration timeout,the channel state machine is maintained in the state of
 * COMM_SPEC_FULL_COM_REQ. 2.Improve the code logic for accessing the configuration parameters of the gateway when the
 * gateway and non- gateway exist at the same time.
 *  V2.0.8 20210805  lili.wang
 *      1. ComM.Internal.c grammatical errors modify.
 *  V2.0.9    20210809  lili.wang
 *      1. When the ComM type is set to LINSLAVE, NO_COM is requested, but the request cannot be passed down.
 *  V2.1.0    20210927 darren.zhang
 *      1. modify bus nm notify comm used var cache,to solve bus nm notify slinet and normal but comm not had time to
 * process it.then comm clear all bus notfiy state.
 *  V2.1.1    20211130 darren.zhang
 *      1.Code Refactor
 *  V2.1.2    20220711 darren.zhang
 *      1.modify passive wakeup channel handle error and silent switch to ready sleep handle(ComM_Ch)
 *      2.Qac MISRA C modify
 *      3.modify configure version macros check
 *      4.modify pnc user requst handle(ComM_Pnc)
 *      5.modify memmap
 *  V2.1.3    20221021 darren.zhang
 *      1.modify channel state COMM_SILENT_COMMUNICATION wakeup and user requat logic handle
 *  V2.1.4    20230109 darren.zhang
 *      1.Generate code integration RTE
 *      2.In a non-gateway scenario, ComM PncSendIraByCh parameter correction is invoked
 *      3.ComM_Ch.c Fix compilation warnings
 *   V2.1.5   20230206 darren.zhang
 *      1. ComM_Ch.c modify ComMTMinFullComModeDuration logic handle
 *      2. ComM_Pnc.c modify code comment and delete invalid code
 *   V2.1.6   20230530 darren.zhang
 *      1.support pnc user mode switch notify
 *      2.add ComM_InhibitionStatusType local typedef
 *      3.modify pnc precompiled macro
 *      4.macro COMMM_ modify COMM_
 *      5.modify limit nocom request handle
 *      6.ComM_GetCurrentComMode support indirect user mapping channel and ComM_RequestComMode handle silent request
 *       modify
 *      7.modify ComM_GetMaxComMode logic handle
 *      8.ECU Group Class add initialization of variable
 *   V2.1.7   20230616 darren.zhang
 *      1.modify limit nocom clear user request handle
 *   V2.1.8   20230630 xiaojian.liang
 *      1.fixed CPT-5873 - Function ComM_ChGetMaxAllowMode declaration is missing
 *      2.ComM_Pnc.c Cross mapping scenario ERA receiving processing intercept
 *      3.Function inhibition enables position adjustment
 *   V2.1.9   20230804 xiaojian.liang
 *      1.fixed CPT-6210 - Function limit to COMM_NO_COMMUNICATION mode is not available when ComMNoCom is not set
 *      2.fixed CPT-5627 - Compile failed when ComMModeLimitationEnabled is set but ComMWakeupInhibitionEnabled is not
 *   V2.1.10  20230821 xiaojian.liang
 *      1.fixed CPT-6601 - Limit to COMM_NO_COMMUNICATION mode initialization error other than channel 0
 *      2.fixed CPT-6233 - COMM_FULL_COMMUNICATION request was not reevaluated after wake up inhibition is switched off
 *   V2.1.11  20231114 xiaojian.liang
 *      1.fixed CPT-6569 - Ignore user request in PASSIVE channel and fixed passive wake-up failure
 *      2.fixed CPT-7054 - Undefined ComM_InhibitionStatusType is used when communication limitation is disabled
 *      3.fixed CPT-7636 - ComMTMinFullComModeDuration should not be skipped after request COMM_NO_COMMUNICATION
 *   V2.1.12  20231207 xiaojian.liang
 *      1.fixed CPT-7703 - The definition of missing macros COMM_DYN_PNC_TO_CH_MAPP_SUPPORT and
 *                         COMM_SYNC_PNC_SHUTDOWN_ENABLED causes compilation failure When ComMPncSupport is not set.
 *      2.Added guard condition [no passive wakeup] for transition NO_COM_REQUEST_PENDING -> NO_COM_NO_PENDING_REQUEST
 *   V2.1.13  20240122 xiaojian.liang
 *      1.CPT-8068 - Crash in function ComM_PncProcessTimer if ComMPncSupport is true but ComMPncEnabled is false.
 *      2.CPT-7917 - Compile error using GHS compiler with [-Werror, -Wparentheses-equality].
 *      3.Support multiple partition feature.
 *   V2.1.14  20241127 shuangyang.fu
 *      1.CPT-8386 - reconstruct the mainfunction of pnc.
 *      2.CPT-10596 - Ensure that reqCnt does not change during the state transition process.
 *      3.CPT-10864 - reconstruct the implementation of ComM_ChGetMaxAllowMode.
 *      4.CPT-11330 - Remove the state transition that was originally performed in the interface, and instead,
 *  centralize the state transition in the main function.
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:ComM<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 1531, 1532 MISRA Rule 8.7 .<br>
    Reason:The exception is global configuration data(1531) and interfaces within and between modules(1532).

    \li PRQA S 1504, 1505 MISRA Rule 8.7 .<br>
    Reason:ComM_UserReqMode and ComM_HandleUserReqest will be used in ComM_Ch.c when enable wake up inhibition.

    \li PRQA S 0777 MISRA Rule 5.4 .<br>
    Reason:Function name, design needs, namelength set to 31 in C99.
 */

/*=================================================[inclusions]=======================================================*/
#include "ComM.h"
#include "ComM_Internal.h"
#include "ComM_PBCfg.h"
#include "ComM_Version.h"
#if (COMM_PNC_SUPPORT == STD_ON)
#include "ComM_Pnc.h"
#endif /* COMM_PNC_SUPPORT == STD_ON */
#include "ComM_Ch.h"
#include "ComM_Nm.h"
#include "ComM_EcuM.h"
#include "ComM_Dcm.h"
#include "ComM_BusSM.h"
#include "ComM_BswM.h"
#include "SchM_ComM.h"

#if COMM_DEV_ERROR_DETECT == STD_ON
#include "Det.h"
#endif

#if defined(COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR)
#include "NvM.h"
#endif

/*======================================================================================================================
 *                                                      Version
======================================================================================================================*/

/*======================================================================================================================
 *                                                  Version check
======================================================================================================================*/
#if !defined(COMM_VENDOR_ID)
#error "COMM_VENDOR_ID must be defined"
#endif /* !defined(COMM_VENDOR_ID */
#if (COMM_VENDOR_ID != 62u)
#error "COMM_VENDOR_ID has wrong vendor id"
#endif /* COMM_VENDOR_ID!=62u */

#if !defined COMM_AR_RELEASE_MAJOR_VERSION
#error "COMM_AR_RELEASE_MAJOR_VERSION must be defined"
#endif /* !defined COMM_AR_RELEASE_MAJOR_VERSION */

#if !defined COMM_AR_RELEASE_MINOR_VERSION
#error "COMM_AR_RELEASE_MINOR_VERSION must be defined"
#endif /* !defined COMM_AR_RELEASE_MINOR_VERSION */

#if !defined COMM_AR_RELEASE_REVISION_VERSION
#error "COMM_AR_RELEASE_REVISION_VERSION must be defined"
#endif /* !defined COMM_AR_RELEASE_REVISION_VERSION */

#if !defined COMM_SW_MAJOR_VERSION
#error "COMM_SW_MAJOR_VERSION must be defined"
#endif /* !defined COMM_SW_MAJOR_VERSION */

#if !defined COMM_SW_MINOR_VERSION
#error "COMM_SW_MINOR_VERSION must be defined"
#endif /* !defined COMM_SW_MINOR_VERSION */

#if !defined COMM_SW_PATCH_VERSION
#error "COMM_SW_PATCH_VERSION must be defined"
#endif /* !defined COMM_SW_PATCH_VERSION */

#if (                                                            \
    (COMM_SW_CFG_DEP_MAJOR_VERSION != COMM_CFG_MAJOR_VERSION)    \
    || (COMM_SW_CFG_DEP_MINOR_VERSION != COMM_CFG_MINOR_VERSION) \
    || (COMM_SW_CFG_DEP_PATCH_VERSION != COMM_CFG_PATCH_VERSION))
#error "soruce code version is not match configuration"
#endif

/*=================================================[internal data]====================================================*/
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
/** \brief comm store nvm data */
COMM_LOCAL ComM_StoryDataType ComM_StoreData;
#endif /* (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON */

/** \brief  Initialization state of ComM */
#define COMM_START_SEC_VAR_INIT_8
#include "ComM_MemMap.h"
ComM_InitStatusType ComM_InitStatus = COMM_UNINIT;
#define COMM_STOP_SEC_VAR_INIT_8
#include "ComM_MemMap.h"

/** \brief ComM channel configure post build data point */
#define COMM_START_SEC_VAR_NO_INIT_PTR
#include "ComM_MemMap.h"
const ComM_ConfigType* ComM_ConfigPtr;
#define COMM_STOP_SEC_VAR_NO_INIT_PTR
#include "ComM_MemMap.h"

#if (COMM_USER_NUMBER > 0u)
/** \brief ComM user request mode */
#define COMM_START_SEC_VAR_NO_INIT_8
#include "ComM_MemMap.h"
ComM_ModeType ComM_UserReqMode[COMM_USER_NUMBER]; /* PRQA S 1504 */ /* MISRA Rule 8.7 */
#define COMM_STOP_SEC_VAR_NO_INIT_8
#include "ComM_MemMap.h"
#endif /* COMM_USER_NUMBER > 0u */

/*=========================================[internal function declarations]===========================================*/
#if COMM_DEV_ERROR_DETECT == STD_ON
#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS ComM_ValidatePointer
#endif

COMM_LOCAL boolean ComM_ValidateInitStatus(uint8 apiId);
COMM_LOCAL boolean ComM_ValidatePointer(uint8 apiId, const void* pointer);
COMM_LOCAL boolean ComM_ValidateUser(uint8 apiId, ComM_UserHandleType user);
COMM_LOCAL boolean ComM_ValidateUserAndComMode(uint8 apiId, ComM_UserHandleType user, ComM_ModeType comMode);
COMM_LOCAL boolean ComM_ValidateUserAndPointer(uint8 apiId, ComM_UserHandleType user, const void* pointer);
COMM_LOCAL boolean ComM_ValidateChannel(uint8 apiId, NetworkHandleType channel);
#if (COMM_PNC_SUPPORT == STD_ON && COMM_SYNC_PNC_SHUTDOWN_ENABLED == STD_ON) \
    || (COMM_MODE_LIMITATION_ENABLED == STD_ON || COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
COMM_LOCAL boolean ComM_ValidateChannelAndPointer(uint8 apiId, NetworkHandleType channel, const void* pointer);
#endif
#if COMM_PNC_SUPPORT == STD_ON
COMM_LOCAL boolean ComM_ValidatePnc(uint8 apiId, PNCHandleType pnc);
#endif
#endif

/*==========================================[external function definitions]===========================================*/
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/**
 * @ingroup ComM
 * @brief Requesting of a Communication Mode by a user
 * @param[in] User Handle of the user who requests a mode
 * @param[in] ComMode requst mode
 * @return E_OK :Successfully changed to the new mode
 *         E_NOT_OK:Changing to the new mode failed
 *          COMM_E_MODE_LIMITATION: Mode can not be granted because of mode inhibition.
 * @note autosar standard @ref SWS_ComM_00110
 */
/* PRQA S 1505 ++ */ /* MISRA Rule 8.7 */
Std_ReturnType ComM_HandleUserReqest(ComM_UserHandleType User, ComM_ModeType ComMode)
/* PRQA S 1505 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
    const ComM_UserConfigType* userCfgPtr = &ComM_ConfigPtr->userCfgPtr[User];
#if (COMM_PNC_SUPPORT == STD_ON)

    if (ComM_ConfigPtr->ComMPncEnabled == TRUE)
    {
#if (COMM_PNC_NUMBER > 0)
        for (uintx index = 0u; index < (uintx)(userCfgPtr->pncListNum); index++)
        {
            ComM_PncRequestComMode(userCfgPtr->pncList[index], userCfgPtr->userInPncIdex[index], ComMode);
        }
        ret = E_OK;
#endif /* COMM_PNC_NUMBER > 0 */
    }
#endif /* COMM_PNC_SUPPORT == STD_ON */
    for (uintx index = 0u; index < userCfgPtr->directChListNum; index++)
    {
#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
        ret =
            ComM_ChRequstCommModeLimitCheck(userCfgPtr->directChList[index], userCfgPtr->userInChIdex[index], ComMode);
        if (ret == E_OK)
#endif
        {
            ret = ComM_ChRequstCommMode(userCfgPtr->directChList[index], userCfgPtr->userInChIdex[index], ComMode);
        }
        if (ret != E_OK)
        {
            break;
        }
    }
    return ret;
}

/**
 * @ingroup ComM
 * @brief Initializes ComM Module
 * @param[in] ConfigPtr Pointer to post-build configuration data
 * @return NA
 * @note support postbuild @ref SWS_ComM_00146
 */
FUNC(void, COMM_CODE)
ComM_Init(P2CONST(ComM_ConfigType, AUTOMATIC, COMM_APPL_CONST) ConfigPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    COMM_PARA_UNUSED(ConfigPtr);

    COMM_FUNC_TRACE_ENTRY();
    ComM_ConfigPtr = &ComM_Config;
#if (COMM_PNC_SUPPORT == STD_ON)
    if (ComM_ConfigPtr->ComMPncEnabled == TRUE)
    {
#if (COMM_PNC_NUMBER > 0)
        ComM_PncInit(ComM_ConfigPtr->pncCfgPtr);
#endif /* COMM_PNC_NUMBER > 0 */
    }
#endif /* COMM_PNC_SUPPORT == STD_ON */
    ComM_ChInit(ComM_ConfigPtr->chCfgPtr, COMM_CHANNEL_NUMBER);

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
    ComM_StoreData.ecuGroupClass = COMM_ECU_GROUP_CLASSIFICATION;
    ComM_StoreData.inhibitCnt = 0u;
#if defined(COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR)
#if COMM_FUNC_CHECK
    Std_ReturnType ret = NvM_ReadBlock(COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR, &ComM_StoreData);
    if (E_OK != ret)
    {
        COMM_CHECK_OUTPUT("NvM_ReadBlock block:%d return:%d\n", COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR, ret);
    }
#else
    (void)NvM_ReadBlock(COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR, &ComM_StoreData);
#endif /* COMM_FUNC_CHECK */
#endif /* defined(COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR */
#endif /* (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON */

#if (COMM_USER_NUMBER > 0u)
    for (uintx index = 0u; index < COMM_USER_NUMBER; index++)
    {
        ComM_UserReqMode[index] = COMM_NO_COMMUNICATION;
    }
#endif /* COMM_USER_NUMBER > 0u */
    ComM_InitStatus = COMM_INIT;
    COMM_FUNC_TRACE_EXIT();
}

/**
 * @ingroup ComM
 * @brief  de-initializes ComM Module
 * @param NA
 * @return NA
 * @note NA @ref SWS_ComM_00147
 */
FUNC(void, COMM_CODE) ComM_DeInit(void)
{
    COMM_FUNC_TRACE_ENTRY();
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateInitStatus(COMM_SID_DEINIT))
#endif
    {
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
#if defined(COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR)
#if COMM_FUNC_CHECK
        Std_ReturnType ret = NvM_WriteBlock(COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR, &ComM_StoreData);
        if (E_OK != ret)
        {
            COMM_CHECK_OUTPUT("NvM_WriteBlock block:%d return:%d\n", COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR, ret);
        }
#else
        (void)NvM_WriteBlock(COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR, &ComM_StoreData);
#endif /* COMM_FUNC_CHECK */
#endif /* defined(COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR */
#endif /* (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON */

        uintx chIdx;
        for (chIdx = 0; chIdx < COMM_CHANNEL_NUMBER; chIdx++)
        {
            ComM_StateType chStu;
            ComM_ChGetState(chIdx, &chStu);
            if (chStu != COMM_SPEC_NOCOM_NOPENDING_REQ)
            {
                break;
            }
        }
        if (chIdx >= COMM_CHANNEL_NUMBER)
        {
            ComM_InitStatus = COMM_UNINIT;
        }
    }
    COMM_FUNC_TRACE_EXIT();
}

#if (COMM_VERSION_INFO_API == STD_ON)
/**
 * @ingroup ComM
 * @brief This function returns the version information of this module
 * @param[in] Versioninfo network channel
 * @param[out] State network channel state machine
 * @return NA
 * @note autosar standard @ref SWS_ComM_00370
 */

FUNC(void, COMM_CODE)
ComM_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr)
{
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePointer(COMM_SID_GETVERSIONINFO, VersionInfoPtr))
#endif
    {
        VersionInfoPtr->vendorID = COMM_VENDOR_ID;
        VersionInfoPtr->moduleID = COMM_MODULE_ID;
        VersionInfoPtr->sw_major_version = COMM_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = COMM_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = COMM_SW_PATCH_VERSION;
    }
}
#endif /* COMM_VERSION_INFO_API == STD_ON */

/**
 * @ingroup ComM
 * @brief Returns the initialization status of ComM
 * @param[out] Status network channel state machine
 * @return E_OK :Success return
 *         E_NOT_OK:get state failed
 * @note support 4.2.2 r19 r21 @ref SWS_ComM_00242
 */
FUNC(Std_ReturnType, COMM_CODE)
ComM_GetStatus(ComM_InitStatusType* Status)
{
    Std_ReturnType ret = E_NOT_OK;
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePointer(COMM_SID_GETSTATUS, Status))
#endif
    {
        *Status = ComM_InitStatus;
        ret = E_OK;
    }
    return ret;
}

/**
 * @ingroup ComM
 * @brief Requesting of a Communication Mode by a user
 * @param[in] User Handle of the user who requests a mode
 * @param[in] ComMode requst mode
 * @return E_OK :Successfully changed to the new mode
 *         E_NOT_OK:Changing to the new mode failed
 *          COMM_E_MODE_LIMITATION: Mode can not be granted because of mode inhibition.
 * @note autosar standard @ref SWS_ComM_00110
 */
FUNC(Std_ReturnType, COMM_CODE)
ComM_RequestComMode(ComM_UserHandleType User, ComM_ModeType ComMode)
{
    Std_ReturnType ret = E_NOT_OK;
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateUserAndComMode(COMM_SID_REQUESTCOMMODE, User, ComMode))
#endif
    {
#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
        ComM_UserReqMode[User] = ComMode;
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
#else
        ComM_UserReqMode[User] = ComMode;
#endif
        ret = ComM_HandleUserReqest(User, ComMode);
#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
        if ((ret == COMM_E_MODE_LIMITATION) && (ComM_StoreData.inhibitCnt < 0xffffu))
        {
            ComM_StoreData.inhibitCnt++;
        }
#endif
    }
    return ret;
}

/**
 * @ingroup ComM
 * @brief Function to query the maximum allowed Communication Mode of the corresponding user
 * @param[in] User Handle of the user who requests a mode
 * @param[in] ComMode requst mode
 * @return E_OK :Successfully returned maximum allowed Communication Mode
 *         E_NOT_OK:Return of maximum allowed Communication Mode failed
 * @note autosar standard @ref SWS_ComM_00085
 */
FUNC(Std_ReturnType, COMM_CODE)
ComM_GetMaxComMode(ComM_UserHandleType User, ComM_ModeType* ComMode)
{
    Std_ReturnType ret = E_NOT_OK;
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateUserAndPointer(COMM_SID_GETMAXCOMMODE, User, ComMode))
#endif
    {
#if COMM_USER_NUMBER > 0u
        ret = E_OK;
        const ComM_UserConfigType* userCfgPtr = &ComM_ConfigPtr->userCfgPtr[User];
        *ComMode = COMM_FULL_COMMUNICATION;

        for (uintx index = 0u; index < userCfgPtr->mapAllChListNum; index++)
        {
            ComM_ModeType chMode = ComM_ChGetMaxAllowMode(userCfgPtr->mapAllChList[index]);
            *ComMode = chMode;
            if (chMode == COMM_NO_COMMUNICATION)
            {
                break;
            }
        }
#endif /* COMM_USER_NUMBER > 0u */
    }
    return ret;
}

/**
 * @ingroup ComM
 * @brief Function to query the currently requested Communication Mode of the corresponding user.
 * @param[in] User Handle of the user who requests a mode
 * @param[in] ComMode requst mode
 * @return E_OK :Successfully returned requested Communication Mode
 *         E_NOT_OK:Return of requested Communication Mode failed
 * @note autosar standard @ref SWS_ComM_00079
 */
FUNC(Std_ReturnType, COMM_CODE)
ComM_GetRequestedComMode(ComM_UserHandleType User, ComM_ModeType* ComMode)
{
    Std_ReturnType ret = E_NOT_OK;
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateUserAndPointer(COMM_SID_GETREQUESTEDCOMMODE, User, ComMode))
#endif
    {
        *ComMode = ComM_UserReqMode[User];
        ret = E_OK;
    }
    return ret;
}

/**
 * @ingroup ComM
 * @brief Function to query the currently requested Communication Mode of the corresponding user.
 * @param[in] User Handle of the user who requests a mode
 * @param[in] ComMode requst mode
 * @return E_OK :Successfully returned Communication Mode from Bus State Manager
 *         E_NOT_OK:Return of Communication Mode from Bus State Manager failed
 * @note autosar standard @ref SWS_ComM_00083
 */
FUNC(Std_ReturnType, COMM_CODE)
ComM_GetCurrentComMode(ComM_UserHandleType User, ComM_ModeType* ComMode) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateUserAndPointer(COMM_SID_GETCURRENTCOMMODE, User, ComMode))
#endif
    {
        const ComM_UserConfigType* usrCfgPtr = &ComM_ConfigPtr->userCfgPtr[User];
        ComM_ModeType localMode = 0xffu;
        for (uintx index = 0; index < usrCfgPtr->mapAllChListNum; index++)
        {
            ComM_ModeType modeTmp;
            ret = ComM_ChGetCurrentComMode(usrCfgPtr->mapAllChList[index], &modeTmp);
            if ((ret == E_OK) && (modeTmp < localMode))
            {
                localMode = modeTmp;
            }
            if ((ret != E_OK) || (modeTmp == COMM_NO_COMMUNICATION))
            {
                break;
            }
        }
        if (E_OK == ret)
        {
            *ComMode = localMode;
        }
    }
    return ret;
}

/**
 * @ingroup ComM
 * @brief The function returns the current Communication Mode of the corresponding PNC the affected user is assigned to
 * @param[in] User Handle of the user who requests a mode
 * @param[in] ComMode requst mode
 * @return E_OK :Successfully returned Communication Mode from Bus State Manager
 *         E_NOT_OK:Return of Communication Mode from Bus State Manager failed
 * @note autosar standard R21 @ref SWS_ComM_00083
 */
FUNC(Std_ReturnType, COMM_CODE)
ComM_GetCurrentPNCComMode(ComM_UserHandleType User, ComM_ModeType* ComMode)
{
    Std_ReturnType ret = E_NOT_OK;
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateUserAndPointer(COMM_SID_GETCURRENTPNCCOMMODE, User, ComMode))
#endif
    {
#if (COMM_PNC_SUPPORT == STD_ON)
        const ComM_UserConfigType* usrCfgPtr = &ComM_ConfigPtr->userCfgPtr[User];

        if (ComM_ConfigPtr->ComMPncEnabled == TRUE)
        {
#if (COMM_PNC_NUMBER > 0)
            if (usrCfgPtr->pncListNum == 1u)
            {
                ret = E_OK;
                /** @ref SWS_ComM_01024
                 * pnc state mapping ComM_ModeType
                 */
                ComM_PncModeType pncMode = ComM_PncGetStatus(usrCfgPtr->pncList[0u]);
                *ComMode = (pncMode != COMM_PNC_NO_COMMUNICATION) ? COMM_FULL_COMMUNICATION : COMM_NO_COMMUNICATION;
            }
            else if (usrCfgPtr->pncListNum > 1u)
            {
                /** @ref SWS_ComM_01022
                 * not support more than one PNC is assigned to the affected user
                 */
                ret = COMM_E_MULTIPLE_PNC_ASSIGNED;
            }
            else
            {
                /** @ref SWS_ComM_01023
                 * not support no PNC is assigned to the affected user
                 */
                ret = COMM_E_NO_PNC_ASSIGNED;
            }
#endif /* COMM_PNC_NUMBER > 0 */
        }
#endif /* COMM_PNC_SUPPORT == STD_ON */
    }
    return ret;
}

#if (COMM_PNC_SUPPORT == STD_ON) && (COMM_PNC_GATEWAY_ENABLED == STD_ON) && (COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON)
/**
 * @ingroup ComM
 * @brief returns the current configuration of the ECUs PNC-to-channelmapping.
 * @param[out] MappingTable Pointer to two-dimensional array with the current Pnc-to-channelmapping of the PNC Gateway
 * where the first dimension covers all relevant channels and the second all relevant PNCs.
 * @param[out] ChannelCnt Pointer to number of ComM channels that are passed in the Mapping Table parameter
 * @param[out] PncCnt Pointer to number of PNCs, that are passed in the MappingTable parameter
 * @return E_OK: Successfully get PNC-to-channel-mapping entry
 *         E_NOT_OK: Getting of PNC-to-channel-mapping entry failed
 *         COMM_E_LEARNING_ACTIVE: Functionality cannot be executed because PNC learning phase is active
 * @note autosar standard R21 draft @ref SWS_ComM_91015
 */
FUNC(Std_ReturnType, COMM_CODE)
ComM_GetPncToChannelMapping(
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(boolean, AUTOMATIC, COMM_APPL_DATA) MappingTable,
    P2VAR(uint8, AUTOMATIC, COMM_APPL_DATA) ChannelCnt,
    P2VAR(uint8, AUTOMATIC, COMM_APPL_DATA) PncCnt
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
)
{
    Std_ReturnType ret = E_NOT_OK;
#if (COMM_DEV_ERROR_DETECT == STD_ON)
    if (ComM_InitStatus != COMM_INIT)
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SID_GETPNCTOCHANNELMAPPING, COMM_E_UNINIT);
    }
    else if ((NULL_PTR == MappingTable) || (NULL_PTR == ChannelCnt) || (NULL_PTR == PncCnt))
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SID_GETPNCTOCHANNELMAPPING, COMM_E_PARAM_POINTER);
    }
    else
#endif /* COMM_DEV_ERROR_DETECT == STD_ON */
    {
        ret = ComM_PncGetPncMap((uint8**)MappingTable, ChannelCnt, PncCnt);
    }
    return ret;
}
/**
 * @ingroup ComM
 * @brief This function can be used to set entries within the the ECUs PNC-to-channelmapping
 * @param[in] MappingTable Pointer to two-dimensional array with the current Pnc-to-channelmapping of the PNC Gateway
 * where the first dimension covers all relevant channels and the second all relevant PNCs.
 * @param[in] ChannelCnt Pointer to number of ComM channels that are passed in the Mapping Table parameter
 * @param[in] PncCnt Pointer to number of PNCs, that are passed in the MappingTable parameter
 * @return E_OK: Successfully set PNC-to-channel-mapping entry
 *         E_NOT_OK: Set of PNC-to-channel-mapping entry failed
 *         COMM_E_LEARNING_ACTIVE: Functionality cannot be executed because PNC learning phase is active
 */
FUNC(Std_ReturnType, COMM_CODE)
ComM_UpdatePncToChannelMapping(
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, COMM_APPL_DATA) MappingTable,
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(uint8, COMM_APPL_DATA) ChannelCnt,
    VAR(uint8, COMM_APPL_DATA) PncCnt)
{
    Std_ReturnType ret = E_NOT_OK;
#if (COMM_DEV_ERROR_DETECT == STD_ON)
    if (ComM_InitStatus != COMM_INIT)
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SID_UPDATEPNCTOCHANNELMAPPING, COMM_E_UNINIT);
    }
    else if ((NULL_PTR == MappingTable))
    {
        (void)
            Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SID_UPDATEPNCTOCHANNELMAPPING, COMM_E_PARAM_POINTER);
    }
    /** @ref SWS_ComM_01038 */
    else if (
        (NULL_PTR == ComM_ConfigPtr->pncCfgPtr) || (ChannelCnt >= ComM_ConfigPtr->pncCfgPtr->pncMapTotalChannelNum)
        || (PncCnt >= ComM_ConfigPtr->pncCfgPtr->pncListNum))
    {
        (void)Det_ReportError(
            COMM_MODULE_ID,
            COMM_INSTANCE_ID,
            COMM_SID_UPDATEPNCTOCHANNELMAPPING,
            COMM_E_WRONG_PARAMETERS);
    }
    else
#endif /* COMM_DEV_ERROR_DETECT == STD_ON */
    {
        ret = ComM_PncSetPncMap((uint8**)MappingTable, ChannelCnt, PncCnt);
    }
    return ret;
}
/**
 * @ingroup ComM
 * @brief This function resets dynamic entries within the ECUs PNC-to-channel-mapping to default values
 * @param[in] NA
 * @return E_OK: Successfully reset PNC-to-channel-mapping to default
 *         E_NOT_OK: Reset of PNC-to-channel-mapping to default failed
 *         COMM_E_LEARNING_ACTIVE: Functionality cannot be executed because PNC learning phase is active
 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ResetPncToChannelMapping(void)
{
    Std_ReturnType ret = E_NOT_OK;
#if (COMM_DEV_ERROR_DETECT == STD_ON)
    if (ComM_InitStatus != COMM_INIT)
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SID_UPDATEPNCTOCHANNELMAPPING, COMM_E_UNINIT);
    }
    else if (NULL_PTR == ComM_ConfigPtr->pncCfgPtr)
    {
        (void)Det_ReportError(
            COMM_MODULE_ID,
            COMM_INSTANCE_ID,
            COMM_SID_UPDATEPNCTOCHANNELMAPPING,
            COMM_E_WRONG_PARAMETERS);
    }
    else
#endif /* COMM_DEV_ERROR_DETECT == STD_ON */
    {
        ret = ComM_PncResetPncMap();
    }
    return ret;
}

#endif /* COMM_PNC_SUPPORT == STD_ON && COMM_PNC_GATEWAY_ENABLED == STD_ON && COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == \
          STD_ON */

#if ((COMM_PNC_SUPPORT == STD_ON) && (COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON))

/**
 * @ingroup ComM
 * @brief Triggers the NM to return into NM Repeat Message State and send the PNI RMR used for the optional Dynamic
 * PNC-to-channelmapping feature
 * @param[in] NA
 * @return E_OK: Successfully started PNC Learning algorithm
 *         E_NOT_OK: PNC Learning algorithm could not be started
 *         COMM_E_LEARNING_ACTIVE: Functionality cannot be executed because PNC learning phase is active
 */
FUNC(Std_ReturnType, COMM_CODE) ComM_PnLearningRequest(void)
{
    Std_ReturnType ret = E_NOT_OK;
#if (COMM_DEV_ERROR_DETECT == STD_ON)
    if (ComM_InitStatus != COMM_INIT)
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SID_PNLEARNREQUEST, COMM_E_UNINIT);
    }
    else if ((ComM_ConfigPtr->ComMPncEnabled == TRUE))
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SID_PNLEARNREQUEST, COMM_E_WRONG_PARAMETERS);
    }
    else
#endif /* COMM_DEV_ERROR_DETECT == STD_ON */
    {
        ret = ComM_PncLearningRequest();
    }
    return ret;
}
/**
 * @ingroup ComM
 * @brief This function is used by SWCs to update the PNC membership which is transmitted during PNC Learning
 * @param[in] Control Boolean Parameter: 0 = Unset the corresponding Bits in PncBit Mask
 * 1 = Set the corresponding Bits in PncBitMask
 * @param[in] PncMembership Array of uint8 with <PNC Vector Length> Elements that holds the current PNC Membership of
 * the node
 * @return E_OK: ComM_PncMembership successfully updated
 *         E_NOT_OK: Error occurred while updating the PNC membership.
 *         COMM_E_LEARNING_ACTIVE: Functionality cannot be executed because PNC learning phase is active
 */
FUNC(Std_ReturnType, COMM_CODE)
ComM_UpdatePncMembership(boolean Control, P2CONST(uint8, AUTOMATIC, COMM_APPL_DATA) PncMembership)
{
    Std_ReturnType ret = E_NOT_OK;
#if (COMM_DEV_ERROR_DETECT == STD_ON)
    if (ComM_InitStatus != COMM_INIT)
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SID_UPPNCMENBERSHIP, COMM_E_UNINIT);
    }
    else if (NULL_PTR == PncMembership)
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SID_UPPNCMENBERSHIP, COMM_E_PARAM_POINTER);
    }
    else if ((ComM_ConfigPtr->ComMPncEnabled == TRUE))
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SID_UPPNCMENBERSHIP, COMM_E_WRONG_PARAMETERS);
    }
    else
#endif /* COMM_DEV_ERROR_DETECT == STD_ON */
    {
        ret = ComM_PncUpdateMembership(Control, PncMembership);
    }
    return ret;
}
#endif /* COMM_PNC_SUPPORT == STD_ON && COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON */

#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
/**
 * @ingroup ComM
 * @brief Changes the inhibition status COMM_NO_WAKEUP for the corresponding channel.
 * @param[in] Channel See NetworkHandleType
 * @param[in] Status FALSE: Wake up inhibition is switched off TRUE: Wake up inhibition is switched on
 * @return E_OK: Successfully changed wake up status for the channel
 *         E_NOT_OK: Change of wake up status for the channel failed
 */
FUNC(Std_ReturnType, COMM_CODE) ComM_PreventWakeUp(NetworkHandleType Channel, boolean Status)
{
    Std_ReturnType ret = E_NOT_OK;
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannel(COMM_SID_PREVENTWAKEUP, Channel))
#endif
    {
        if (0u != (ComM_StoreData.ecuGroupClass & COMM_WAKEUP_INHIBITION_ACTIVE_BflMask))
        {
            ComM_ChLimitWakeUp(Channel, Status);
            ret = E_OK;
        }
    }
    return ret;
}

#endif /* COMM_WAKEUP_INHIBITION_ENABLED == STD_ON */

#if COMM_MODE_LIMITATION_ENABLED == STD_ON
/**
 * @ingroup ComM
 * @brief Changes the inhibition status for the channel for changing from COMM_NO_COMMUNICATION to a higher
 * Communication Mode.
 * @param[in] Channel See NetworkHandleType
 * @param[in] Status FALSE: Limit channel to COMM_NO_COMMUNICATION disabled TRUE: Limit channel to COMM_NO_COMMUNICATION
 * enabled
 * @return E_OK: Successfully changed inhibition status for the channel
 *         E_NOT_OK: Change of inhibition status for the channel failed
 */
Std_ReturnType ComM_LimitChannelToNoComMode(NetworkHandleType Channel, boolean Status)
{
    Std_ReturnType ret = E_NOT_OK;
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannel(COMM_SID_LIMITCHANNELTONOCOMMODE, Channel))
#endif
    {
        if (0u != (ComM_StoreData.ecuGroupClass & COMM_LIMITED_TO_NO_COM_BflMask))
        {
            ComM_ChLimitNoCom(Channel, Status);
            ret = E_OK;
        }
    }
    return ret;
}
/**
 * @ingroup ComM
 * @brief Changes the inhibition status for the ECU (=all channels) for changing from COMM_NO_COMMUNICATION to a higher
 * Communication Mode.
 * @param[in] Status FALSE: Limit ECU to COMM_NO_COMMUNICATION disabled TRUE: Limit ECU to COMM_NO_COMMUNICATION
 * enabled
 * @return E_OK: Successfully changed inhibition status for the ECU
 *         E_NOT_OK: Change of inhibition status for the channel ECU
 */
Std_ReturnType ComM_LimitECUToNoComMode(boolean Status)
{
    Std_ReturnType ret = E_NOT_OK;
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateInitStatus(COMM_SID_LIMITECUTONOCOMMODE))
#endif
    {
        if (0u != (ComM_StoreData.ecuGroupClass & COMM_LIMITED_TO_NO_COM_BflMask))
        {
            for (uintx chIdex = 0u; chIdex < COMM_CHANNEL_NUMBER; chIdex++)
            {
                ComM_ChLimitNoCom(chIdex, Status);
            }
            ret = E_OK;
        }
    }
    return ret;
}
#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) && defined(COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR)
/**
 * @ingroup ComM
 * @brief This function returns the amount of rejected COMM_FULL_COMMUNICATION user requests
 * @param[out] CounterValue Amount of rejected COMM_FULL_COMMUNICATION user requests
 * enabled
 * @return E_OK: Successfully returned Inhibition Counter
 *         E_NOT_OK: Return of Inhibition Counter failed
 */
FUNC(Std_ReturnType, COMM_CODE)
ComM_ReadInhibitCounter(P2VAR(uint16, AUTOMATIC, COMM_APPL_DATA) CounterValue /* PRQA S 3432 */ /* MISRA Rule 20.7 */)
{
    Std_ReturnType stdReturn = E_NOT_OK;
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePointer(COMM_SID_READINHIBITCOUNTER, CounterValue))
#endif
    {
        *CounterValue = ComM_StoreData.inhibitCnt;
        stdReturn = E_OK;
    }
    return stdReturn;
}

/**
 * @ingroup ComM
 * @brief This function resets the Inhibited COMM_FULL_COMMUNICATION request Counter
 * @param[in] NA
 * @return E_OK: Successfully reset of Inhibit COMM_FULL_COMMUNICATION Counter
 *         E_NOT_OK: Reset of Inhibit COMM_FULL_COMMUNICATION Counter failed
 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ResetInhibitCounter(void)
{
    Std_ReturnType stdReturn = E_NOT_OK;
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateInitStatus(COMM_SID_RESETINHIBITCOUNTER))
#endif
    {
        ComM_StoreData.inhibitCnt = 0u;
        stdReturn = E_OK;
    }
    return stdReturn;
}
#endif /* COMM_MODE_LIMITATION_ENABLED == STD_ON && defined COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR */

#if (COMM_USED_MODULE_NM == STD_ON)
/**
 * @ingroup ComM
 * @brief Indication that a NM-message has been received in the Bus Sleep Mode
 * @param[in] Channel  NetworkHandle
 * @return NA
 */
FUNC(void, COMM_CODE) ComM_Nm_NetworkStartIndication(NetworkHandleType Channel) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannel(COMM_SID_NM_NETWORKSTARTINDICATION, Channel))
#endif
    {
        ComM_ChPassiveWakeup(Channel);
    }
}

/**
 * @ingroup ComM
 * @brief Notification that the network management has entered Network Mode.
 * @param[in] Channel  NetworkHandle
 * @return NA
 */
FUNC(void, COMM_CODE) ComM_Nm_NetworkMode(NetworkHandleType Channel) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannel(COMM_SID_NM_NETWORKMODE, Channel))
#endif
    {
        ComM_ChannelNmModeIndication(Channel, COMM_NM_IND_NETWORK_MODE);
    }
}

/**
 * @ingroup ComM
 * @brief Notification that the network management has entered Prepare Bus-Sleep Mode.
 * @param[in] Channel  NetworkHandle
 * @return NA
 */
FUNC(void, COMM_CODE) ComM_Nm_PrepareBusSleepMode(NetworkHandleType Channel) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannel(COMM_SID_NM_PREPAREBUSSLEEPMODE, Channel))
#endif
    {
        ComM_ChannelNmModeIndication(Channel, COMM_NM_IND_PREPARE_BUSSLEEP);
    }
}

/**
 * @ingroup ComM
 * @brief Notification that the network management has entered Bus-Sleep Mode.
 * @param[in] Channel  NetworkHandle
 * @return NA
 */
FUNC(void, COMM_CODE) ComM_Nm_BusSleepMode(NetworkHandleType Channel) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannel(COMM_SID_NM_BUSSLEEPMODE, Channel))
#endif
    {
        ComM_ChannelNmModeIndication(Channel, COMM_NM_IND_BUS_SLEEP);
    }
}
/**
 * @ingroup ComM
 * @brief NmIf start shut doen the coordinated busses not all coordinated have indicated bus sleep state
 * @param[in] Channel  NetworkHandle
 * @return NA
 */
FUNC(void, COMM_CODE) ComM_Nm_RestartIndication(NetworkHandleType Channel)
{
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannel(COMM_SID_NM_RESTARTINDICATION, Channel))
#endif
    {
        ComM_ChPassiveWakeup(Channel);
    }
}

#if (COMM_PNC_SUPPORT == STD_ON) && (COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON)
/**
 * @ingroup ComM
 * @brief Notification that the state of all <BusNm> has left RepeatMessage.
 * @param[in] Channel  NetworkHandle
 * @return NA
 * @note CP R21-11 @ref SWS_ComM_91026 {draft}
 */
FUNC(void, COMM_CODE) ComM_Nm_RepeatMessageLeftIndication(NetworkHandleType Channel)
{
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannel(COMM_SID_NM_REPEATMSGLEFTIND, Channel))
#endif
    {
#if defined(COMM_DYN_PNC_MAP_EN)
        if (ComM_CommonGetBit(ComM_ConfigPtr->chCfgPtr[Channel].cfgMask, COMM_DYN_PNC_MAP_EN))
        {
            ComM_PncLearnInd(Channel, FALSE);
        }
#endif
    }
}

/**
 * @ingroup ComM
 * @brief Service to indicate that an NM message with set PNC Learning Bit has been received.
 * @param[in] Channel  NetworkHandle
 * @return NA
 * @note CP R21-11 @ref SWS_ComM_91003 {draft}
 */
FUNC(void, COMM_CODE) ComM_Nm_PncLearningBitIndication(NetworkHandleType Channel)
{
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannel(COMM_SID_NM_PNCLEARNBITINDICATION, Channel))
#endif
    {
#if defined(COMM_DYN_PNC_MAP_EN)
        if (ComM_CommonGetBit(ComM_ConfigPtr->chCfgPtr[Channel].cfgMask, COMM_DYN_PNC_MAP_EN))
        {
            ComM_PncLearnInd(Channel, TRUE);
        }
#endif /* defined(COMM_DYN_PNC_MAP_EN */
    }
}

#endif /* COMM_PNC_SUPPORT == STD_ON && COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON */

#if (COMM_PNC_SUPPORT == STD_ON) && (COMM_SYNC_PNC_SHUTDOWN_ENABLED == STD_ON)
/**
 * @ingroup ComM
 * @brief pnc coordinator receiver a PN shutdown message，comm shall forward
 * @param[in] Channel  NetworkHandle
 * @return NA
 * @note CP R21-11 @ref SWS_ComM_91003 {draft}
 */
FUNC(void, COMM_CODE) ComM_Nm_ForwardSynchronizedPncShutdown(NetworkHandleType Channel, const uint8* PncBitVectorPtr)
{
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannelAndPointer(COMM_SID_FORWARDSYNCPNCSHUTDOWN, Channel, PncBitVectorPtr))
#endif
    {
#if (COMM_PNC_SUPPORT == STD_ON) && (COMM_PNC_GATEWAY_ENABLED == STD_ON)
#if (COMM_PNC_NUMBER > 0)
        ComM_PncForwardSyncPncShutdown(Channel, PncBitVectorPtr);
#endif /* COMM_PNC_NUMBER > 0 */
#endif /* COMM_PNC_SUPPORT == STD_ON && COMM_PNC_GATEWAY_ENABLED == STD_ON */
    }
}
#endif /* COMM_PNC_SUPPORT == STD_ON) && (COMM_SYNC_PNC_SHUTDOWN_ENABLED == STD_ON */

#if (COMM_PNC_SUPPORT == STD_ON) && (COMM_PNC_USED_COM != STD_ON)
/**
 * @ingroup ComM
 * @brief Function to indicate the current aggregated external / internal PNC request called by Nm.
 * @param[in] PncBitVectorPtr  Pointer to the PNC bit vector which contain the current aggregated EIRA
 * @return NA
 * @note CP R21-11 @ref SWS_ComM_91003 {draft}
 */
FUNC(void, COMM_CODE) ComM_Nm_UpdateEIRA(P2CONST(uint8, AUTOMATIC, COMM_APPL_DATA) PncBitVectorPtr)
{
}

/**
 * @ingroup ComM
 * @brief pnc coordinator receiver a PN shutdown message，comm shall forward
 * @param[in] Channel  NetworkHandle
 * @param[in] PncBitVectorPtr  PNC bit vector which contain the current ERA received on the given channel
 * @return NA
 * @note CP R21-11 @ref SWS_ComM_91029 {draft}
 */
FUNC(void, COMM_CODE)
ComM_Nm_UpdateERA(NetworkHandleType Channel, P2CONST(uint8, AUTOMATIC, COMM_APPL_DATA) PncBitVectorPtr)
{
}
#endif /* COMM_PNC_SUPPORT == STD_ON) && (COMM_PNC_USED_COM != STD_ON */

#endif /* COMM_USED_MODULE_NM == STD_ON */

#if (COMM_DCM_INDICATION == STD_ON)
/**
 * @ingroup ComM
 * @brief Indication of active diagnostic by the DCM.
 * @param[in] Channel  Channel needed for Diagnostic communication
 * @return NA
 * @note CP standard @ref SWS_ComM_00873
 */
FUNC(void, COMM_CODE) ComM_DCM_ActiveDiagnostic(NetworkHandleType Channel)
{
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannel(COMM_SID_DCM_ACTIVEDIAGNOSTIC, Channel))
#endif
    {
        ComM_ChDiagIndication(Channel, TRUE);
    }
}
/**
 * @ingroup ComM
 * @brief Indication of inactive diagnostic by the DCM.
 * @param[in] Channel Channel no longer needed for Diagnostic communication
 * @return NA
 * @note CP standard @ref SWS_ComM_00874
 */
FUNC(void, COMM_CODE) ComM_DCM_InactiveDiagnostic(NetworkHandleType Channel)
{
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannel(COMM_SID_DCM_INACTIVEDIAGNOSTIC, Channel))
#endif
    {
        ComM_ChDiagIndication(Channel, FALSE);
    }
}
#endif /* COMM_DCM_INDICATION == STD_ON */

#if (COMM_ECUM_ENABLE == STD_ON)
/**
 * @ingroup ComM
 * @brief Notification of a wake up on the corresponding channel.
 * @param[in] Channel Networkchannel
 * @return NA
 * @note CP standard @ref SWS_ComM_00275
 */
FUNC(void, COMM_CODE) ComM_EcuM_WakeUpIndication(NetworkHandleType Channel) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannel(COMM_SID_ECUM_WAKEUPINDICATION, Channel))
#endif
    {
#if (COMM_SYNC_WAKEUP == STD_ON)
#if (COMM_PNC_SUPPORT == STD_ON) && (COMM_PNC_NUMBER > 0u)
        if (ComM_ConfigPtr->ComMPncEnabled == TRUE)
        {
            ComM_PncWakupByCh(Channel);
        }
#endif  /* COMM_PNC_SUPPORT == STD_ON) && (COMM_PNC_NUMBER > 0u */
        /** @ref SWS_ComM_00990
        ComMSynchronousWakeUp is set to FALSE, the PNC main state COMM_PNC_NO_COMMUNICATION shall be the current state
      */
#endif  /* COMM_SYNC_WAKEUP == STD_ON */
#if (COMM_SYNC_WAKEUP == STD_ON)
        /** @ref SWS_ComM_00694
         * ComMSynchronousWakeUp is set to TRUE comm shall switch all comm channel
         */
        for (uintx chIdx = 0u; chIdx < COMM_CHANNEL_NUMBER; chIdx++)
#else
        uintx chIdx = Channel;
#endif /* COMM_SYNC_WAKEUP == STD_ON */
        {
            ComM_ChEcuMWakeup(chIdx);
        }
    }
}

#if (COMM_PNC_SUPPORT == STD_ON)
/**
 * @ingroup ComM
 * @brief Notification of a wake up on the corresponding partial network cluster.
 * @param[in] PNCid Identifier of the partial network cluster
 * @return NA
 * @note CP standard @ref SWS_ComM_91001
 */
FUNC(void, COMM_CODE) ComM_EcuM_PNCWakeUpIndication(PNCHandleType PNCid) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidatePnc(COMM_SID_ECUM_PNCWAKEUPINDICATION, PNCid))
#endif
    {
        /** @ref SWS_ComM_01014 */
#if (COMM_PNC_NUMBER > 0u)
        /* pncid to pncindex */
        uintx pncIndx = PNCid;
        ComM_PncWakupIndPnc(pncIndx);

#endif  /* COMM_PNC_NUMBER > 0u */
        /** @ref SWS_ComM_01015
         * ComMSynchronousWakeUp is set to TRUE comm shall switch all comm channel ecum wakeup
         */
#if COMM_SYNC_WAKEUP == STD_ON
        for (uintx chIdx = 0u; chIdx < COMM_CHANNEL_NUMBER; chIdx++)
        {
            ComM_ChEcuMWakeup(chIdx);
        }
#endif /* COMM_SYNC_WAKEUP == STD_ON */
    }
}
#endif /* COMM_PNC_SUPPORT == STD_ON */

#endif /* COMM_ECUM_ENABLE == STD_ON */

#if (COMM_BSWM_ENABLE == STD_ON)
/**
 * @ingroup ComM
 * @brief EcuM or BswM shall indicate to ComM when communication is allowed
 * @param[in] Channel network channel
 * @param[in] Allowed TRUE: Communication is allowed FALSE: Communication is not allowed
 * @return NA
 * @note CP standard @ref SWS_ComM_00871
 */
FUNC(void, COMM_CODE) ComM_CommunicationAllowed(NetworkHandleType Channel, boolean Allowed)
{
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannel(COMM_SID_COMMUNICATIONALLOWED, Channel))
#endif
    {
        ComM_ChComAllow(Channel, Allowed);
    }
}
#endif /* COMM_BSWM_ENABLE == STD_ON */

/**
 * @ingroup ComM
 * @brief EcuM or BswM shall indicate to ComM when communication is allowed
 * @param[in] Channel network channel
 * @param[in] ComMode mode status
 * @return NA
 * @note CP standard @ref SWS_ComM_00675
 */
FUNC(void, COMM_CODE)
ComM_BusSM_ModeIndication(NetworkHandleType Channel, ComM_ModeType ComMode)
{
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannel(COMM_SID_BUSSM_MODEINDICATION, Channel))
#endif
    {
        ComM_ChBusSmModeInd(Channel, ComMode);
    }
}

/**
 * @ingroup ComM
 * @brief Notification of the corresponding Bus State Manager that the actual bus mode is Bus-Sleep.
 * @param[in] Channel network channel
 * @return NA
 * @note CP standard @ref SWS_ComM_91000 (ComMNmVariant set to SLAVE_ACTIVE or SLAVE_PASSIVE)
 */
FUNC(void, COMM_CODE) ComM_BusSM_BusSleepMode(NetworkHandleType Channel) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannel(COMM_SID_BUSSM_BUSSLEEPMODE, Channel))
#endif
    {
        ComM_ChBusSmModeInd(Channel, (ComM_ModeType)COMM_BUS_SLEEP);
    }
}

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
Std_ReturnType ComM_GetInhibitionStatus(NetworkHandleType Channel, ComM_InhibitionStatusType* Status)
{
    Std_ReturnType ret = E_NOT_OK;
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannelAndPointer(COMM_SID_GETINHIBITIONSTATUS, Channel, Status))
#endif
    {
        ComM_ChGetInhibitionStu(Channel, Status);
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType ComM_SetECUGroupClassification(ComM_InhibitionStatusType Status)
{
    Std_ReturnType ret = E_NOT_OK;
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateInitStatus(COMM_SID_SETECUGROUPCLASSIFICATION))
#endif
    {
        ret = E_OK;
        ComM_StoreData.ecuGroupClass = Status;
    }
    return ret;
}
#endif /* COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON */

#if COMM_DEV_ERROR_DETECT == STD_ON
COMM_LOCAL boolean ComM_ValidateInitStatus(uint8 apiId)
{
    boolean valid = ComM_InitStatus == COMM_INIT;
    if (!valid)
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, apiId, COMM_E_UNINIT);
    }
    return valid;
}

COMM_LOCAL boolean ComM_ValidatePointer(uint8 apiId, const void* pointer)
{
    boolean valid = TRUE;

    switch (apiId)
    {
    case COMM_SID_GETVERSIONINFO:
    case COMM_SID_GETSTATUS:
        /* do nothing */;
        break;
    default:
        valid = ComM_ValidateInitStatus(apiId);
        break;
    }

    if (valid && (pointer == NULL_PTR))
    {
        valid = FALSE;
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, apiId, COMM_E_PARAM_POINTER);
    }
    return valid;
}

COMM_LOCAL boolean ComM_ValidateUser(uint8 apiId, ComM_UserHandleType user)
{
    boolean valid = FALSE;
    if (ComM_ValidateInitStatus(apiId))
    {
        if (user >= COMM_USER_NUMBER)
        {
            (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, apiId, COMM_E_WRONG_PARAMETERS);
        }
        else
        {
            valid = TRUE;
        }
    }
    return valid;
}

COMM_LOCAL boolean ComM_ValidateUserAndComMode(uint8 apiId, ComM_UserHandleType user, ComM_ModeType comMode)
{
    boolean valid = FALSE;
    if (ComM_ValidateUser(apiId, user))
    {
        /* @req SWS_ComM_00151 */
        if ((comMode != COMM_NO_COMMUNICATION) && (comMode != COMM_FULL_COMMUNICATION))
        {
            (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, apiId, COMM_E_WRONG_PARAMETERS);
        }
        else
        {
            valid = TRUE;
        }
    }
    return valid;
}

COMM_LOCAL boolean ComM_ValidateUserAndPointer(uint8 apiId, ComM_UserHandleType user, const void* pointer)
{
    return ComM_ValidateUser(apiId, user) && ComM_ValidatePointer(apiId, pointer);
}

COMM_LOCAL boolean ComM_ValidateChannel(uint8 apiId, NetworkHandleType channel)
{
    boolean valid = FALSE;
    if (ComM_ValidateInitStatus(apiId))
    {
        if (channel >= COMM_CHANNEL_NUMBER)
        {
            (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, apiId, COMM_E_WRONG_PARAMETERS);
        }
        else
        {
            valid = TRUE;
        }
    }
    return valid;
}

#if (COMM_PNC_SUPPORT == STD_ON && COMM_SYNC_PNC_SHUTDOWN_ENABLED == STD_ON) \
    || (COMM_MODE_LIMITATION_ENABLED == STD_ON || COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
COMM_LOCAL boolean ComM_ValidateChannelAndPointer(uint8 apiId, NetworkHandleType channel, const void* pointer)
{
    return ComM_ValidateChannel(apiId, channel) && ComM_ValidatePointer(apiId, pointer);
}
#endif

#if COMM_PNC_SUPPORT == STD_ON
COMM_LOCAL boolean ComM_ValidatePnc(uint8 apiId, PNCHandleType pnc)
{
    boolean valid = FALSE;
    if (ComM_ValidateInitStatus(apiId))
    {
        if (pnc >= COMM_PNC_NUMBER)
        {
            (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, apiId, COMM_E_WRONG_PARAMETERS);
        }
        else
        {
            valid = TRUE;
        }
    }
    return valid;
}
#endif
#endif

#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
/**
 * @brief         Returns the ApplicationID of a ComM channel.
 * @id            0x70
 * @synchronous   Synchronous
 * @reentrancy    Reentrant
 * @retval        The ApplicationID of a ComM channel
 */
ApplicationType ComM_GetChannelApplicationID(NetworkHandleType Channel)
{
    ApplicationType applicationID = INVALID_OSAPPLICATION;
#if COMM_DEV_ERROR_DETECT == STD_ON
    if (ComM_ValidateChannel(COMM_SID_GETCHANNELAPPLICATIONID, Channel))
#endif
    {
        applicationID = ComM_ChannelApplicationID[Channel];
    }
    return applicationID;
}
#endif
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
