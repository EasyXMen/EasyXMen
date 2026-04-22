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
 **  @file               : CanSM.c
 **  @author             : Wanglili
 **  @date               : 2020/08/17
 **  @vendor             : isoft
 **  @description        : CAN State Manage
 **
 ***********************************************************************************************************************/
/* PRQA S 6060, 1532 ++ */ /* VL_MTR_CanSM_STM19, VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "CanSM.h"
#include "CanSM_CanIf.h"
#include "CanSM_SchM.h"
#include "CanIf.h"
#include "SchM_CanSM.h"
#include "BswM_CanSM.h"
#if (CANSM_CANNM_CONF_PN_AVA == STD_ON)
#include "CanNm.h"
#endif
#include "Det.h"

#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
#include "ComM.h"
#include "Os.h"
#endif

/* ===================================================== macros ===================================================== */
#ifndef CANSM_LOCAL
#define CANSM_LOCAL static /* PRQA S 3414 */ /* VL_QAC_KeyWord */
#endif

#ifndef CANSM_LOCAL_INLINE
#define CANSM_LOCAL_INLINE static inline
#endif

#define CANSM_UNUSED(x) (void)(x)

#define BUS_OFF_COUNTER_UP_LIMIT 255u

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
/**
 * @brief        Handles the request and indication timer periodically.
 * @param[inout] varPtr  Runtime pointer of the CAN network.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73301
 */
CANSM_LOCAL void CanSM_TimerHandler(CanSM_NetWorkRunTimeType* varPtr);

/**
 * @brief        Finds the index of the network in the CanSM configurations.
 * @param[in]    networkHandle  Handle of the addressed CAN network.
 * @return       Pointer to the network instance that matches the networkHandle, or NULL_PTR if not found.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73302
 */
CANSM_LOCAL const CanSM_ManagerNetworkType* CanSM_FindNetworkByNetworkHandle(NetworkHandleType networkHandle);

/**
 * @brief        Calls the API ComM_BusSM_ModeIndication with the specified ComMode.
 * @param[in]    network           Affected CAN network.
 * @param[in]    comMode           Notified communication mode.
 * @param[in]    requestedComMode  Requested communication mode.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73303
 */
CANSM_LOCAL void
    CanSM_ComMModeIndication(NetworkHandleType network, ComM_ModeType comMode, CanSM_RequestModeType requestedComMode);

/**
 * @brief        Handles state transition in DEINITPNNOT_S_CC_STOPPED.
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73304
 */
CANSM_LOCAL void CanSM_PnNot_CC_Stopped(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

/**
 * @brief        Handle state transition in DEINITPNNOT_S_CC_STOPPED_WAIT
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73305
 */
CANSM_LOCAL void
    CanSM_PnNot_CC_Stopped_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

/**
 * @brief        Handle state transition in DEINITPNNOT_S_CC_SLEEP
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73306
 */
CANSM_LOCAL void CanSM_PnNot_CC_Sleep(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

/**
 * @brief        Handle state transition in DEINITPNNOT_S_CC_SLEEP_WAIT
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73307
 */
CANSM_LOCAL void CanSM_PnNot_CC_Sleep_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

#if (CANSM_TRCV_ENABLED == STD_ON)
/**
 * @brief        Handle state transition in DEINITPNNOT_S_TRCV_NORMAL
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73308
 */
CANSM_LOCAL void CanSM_PnNot_Trcv_Normal(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

/**
 * @brief        Handle state transition in DEINITPNNOT_S_TRCV_NORMAL_WAIT
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73309
 */

CANSM_LOCAL void CanSM_PnNot_Trcv_Normal_Wait(CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in DEINITPNNOT_S_TRCV_STANDBY
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73310
 */
CANSM_LOCAL void CanSM_PnNot_Trcv_Standby(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

/**
 * @brief        Handle state transition in DEINITPNNOT_S_TRCV_STANDBY_WAIT
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73311
 */
CANSM_LOCAL void
    CanSM_PnNot_Trcv_Standby_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
#endif

/**
 * @brief        Handle state transition in DeinitPnNotSupported
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @return       boolean
 * @retval       TRUE if the DeinitPnNotSupported is finished, FALSE otherwise
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73313
 */
CANSM_LOCAL boolean
    CanSM_DeinitPnNotSupported(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

#if (STD_ON == CANSM_PNC_SUPPORT)
/**
 * @brief        Handle state transition in DEINITPN_S_PN_CLEAR_WUF
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73314
 */
CANSM_LOCAL void CanSM_Pn_S_Pn_Clear_WUF(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in DEINITPN_S_PN_CLEAR_WUF_WAIT
 * @param[inout] varPtr  RuntimePtr of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73315
 */
CANSM_LOCAL void CanSM_Pn_S_Pn_Clear_WUF_Wait(CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in DEINITPN_S_PN_CC_STOPPED
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73316
 */
CANSM_LOCAL void CanSM_Pn_S_Pn_CC_Stopped(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in DEINITPN_S_CC_STOPPED_WAIT
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73317
 */
CANSM_LOCAL void
    CanSM_Pn_S_Pn_CC_Stopped_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in DEINITPN_S_TRCV_NORMAL
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73318
 */
CANSM_LOCAL void CanSM_Pn_S_Trcv_Normal(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in DEINITPN_S_TRCV_NORMAL_WAIT
 * @param[inout] varPtr  RuntimePtr of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73319
 */
CANSM_LOCAL void CanSM_Pn_S_Trcv_Normal_Wait(CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in DEINITPN_S_TRCV_STANDBY
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73320
 */
CANSM_LOCAL void CanSM_Pn_S_Trcv_Standby(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in DEINITPN_S_TRCV_STANDBY_WAIT
 * @param[inout] varPtr  RuntimePtr of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73321
 */
CANSM_LOCAL void CanSM_Pn_S_Trcv_Standby_Wait(CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in DEINITPN_S_CC_SLEEP
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73322
 */
CANSM_LOCAL void CanSM_Pn_S_CC_Sleep(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

/**
 * @brief        Handle state transition in DEINITPN_S_CC_SLEEP_WAIT
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73323
 */
CANSM_LOCAL void CanSM_Pn_S_CC_Sleep_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

/**
 * @brief        Handle state transition in DEINITPN_S_CHECK_WFLAG_IN_CC_SLEEP
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73324
 */
CANSM_LOCAL void
    CanSM_Pn_Check_Wflag_InCCSleep(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

/**
 * @brief        Handle state transition in DEINITPN_S_CHECK_WUF_IN_CC_SLEEP_WAIT
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73325
 */
CANSM_LOCAL void
    CanSM_Pn_Check_Wflag_InCCSleepWait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in DEINITPN_S_CHECK_WFLAG_IN_NOT_CC_SLEEP
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73326
 */
CANSM_LOCAL void
    CanSM_Pn_CheckWflag_InNotCCSleep(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in DEINITPN_S_CHECK_WFLAG_IN_NOT_CC_SLEEP
 * @param[inout] varPtr    RuntimePtr of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73327
 */
CANSM_LOCAL void CanSM_Pn_CheckWFlag_InNotCCSleepWait(CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in DeinitPnSupported
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @return       boolean
 * @retval       TRUE if the DeinitPnSupported is finished, FALSE otherwise
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73328
 */
CANSM_LOCAL boolean
    CanSM_DeinitPnSupported(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
#endif

/**
 * @brief        Handle state transition in Bsm_S_NoCom
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @return       boolean
 * @retval       TRUE if the Bsm_S_NoCom is finished, FALSE otherwise
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73329
 */
CANSM_LOCAL boolean CanSM_BsmSNoCom(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in WUVALIDATION_S_CC_STOPPED
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73330
 */
CANSM_LOCAL void CanSM_WUVAL_S_CC_Stopped(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in WUVALIDATION_S_CC_STOPPED_WAIT
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73331
 */
CANSM_LOCAL void
    CanSM_WUVAL_S_CC_Stopped_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in WUVALIDATION_S_CC_STARTED
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73332
 */
CANSM_LOCAL void CanSM_WUVAL_S_CC_Started(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in WUVALIDATION_S_CC_STARTED_WAIT
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73333
 */
CANSM_LOCAL void
    CanSM_WUVAL_S_CC_Started_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in Bsm_WuValidation
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @return       boolean
 * @retval       TRUE if the Bsm_WuValidation is finished, FALSE otherwise
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73334
 */
CANSM_LOCAL boolean CanSM_BsmWuValidation(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

#if (CANSM_TRCV_ENABLED == STD_ON)
/**
 * @brief        Handle state transition in WUVALIDATION_S_TRCV_NORMAL
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73335
 */
CANSM_LOCAL void CanSM_WUVAL_S_Trcv_Normal(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in WUVALIDATION_S_TRCV_NORMAL_WAIT
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73336
 */
CANSM_LOCAL void CanSM_WUVAL_S_Trcv_Normal_Wait(CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in PREFULLCOM_S_TRCV_NORMAL
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73337
 */
CANSM_LOCAL void
    CanSM_Prefullcom_S_Trcv_Normal(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in PREFULLCOM_S_TRCV_NORMAL_WAIT
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73338
 */
CANSM_LOCAL void CanSM_Prefullcom_S_Trcv_Normal_Wait(CanSM_NetWorkRunTimeType* varPtr);
#endif

/**
 * @brief        Handle state transition in PREFULLCOM_S_CC_STOPPED
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73339
 */
CANSM_LOCAL void
    CanSM_Prefullcom_S_CC_Stopped(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in PREFULLCOM_S_CC_STOPPED_WAIT
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73340
 */
CANSM_LOCAL void
    CanSM_Prefullcom_S_CC_Stopped_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in PREFULLCOM_S_CC_STARTED
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73341
 */
CANSM_LOCAL void
    CanSM_Prefullcom_S_CC_Started(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in PREFULLCOM_S_CC_STARTED_WAIT
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73342
 */
CANSM_LOCAL void
    CanSM_Prefullcom_S_CC_Started_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in Bsm_S_Pre_FullCom
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @return       boolean
 * @retval       TRUE if the Bsm_S_Pre_FullCom is finished, FALSE otherwise
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73343
 */
CANSM_LOCAL boolean CanSM_BsmSPreFullCom(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in FULLCOM_S_BUS_OFF_CHECK
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73344
 */
CANSM_LOCAL void
    CanSM_FullCom_S_Bus_off_Check(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in FULLCOM_S_NO_BUS_OFF
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73345
 */
CANSM_LOCAL void
    CanSM_FullCom_S_No_Bus_Off(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in FULLCOM_S_CC_STOPPED
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73346
 */
CANSM_LOCAL void
    CanSM_FullCom_S_CC_Stopped(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in FULLCOM_S_CC_STOPPED_WAIT
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73347
 */
CANSM_LOCAL void
    CanSM_FullCom_S_CC_Stopped_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in FULLCOM_S_CC_STARTED
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73348
 */
CANSM_LOCAL void
    CanSM_FullCom_S_CC_Started(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in FULLCOM_S_CC_STARTED_WAIT
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73349
 */
CANSM_LOCAL void
    CanSM_FullCom_S_CC_Started_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in FULLCOM_S_RESTART_CC
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73350
 */
CANSM_LOCAL void
    CanSM_FullCom_S_Restart_CC(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in FULLCOM_S_RESTART_CC_WAIT
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73351
 */
CANSM_LOCAL void
    CanSM_FullCom_S_Restart_CC_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in Substate CHANGEBR_CHANGE_BR_SYNC
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73352
 */
CANSM_LOCAL void CanSM_FullCom_S_Tx_Off(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in Bsm_S_FullCom
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @return       boolean
 * @retval       TRUE if the Bsm_S_FullCom is finished, FALSE otherwise
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73353
 */
CANSM_LOCAL boolean CanSM_BsmSFullCom(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

#if (STD_ON == CANSM_SET_BAUDRATE_API)
/**
 * @brief        Handle state transition in Substate CHANGEBR_CHANGE_BR_SYNC
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73354
 */
CANSM_LOCAL void
    CanSM_ChangeBR_Change_BR_Sync(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in Substate CHANGEBR_S_CC_STOPPED
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73355
 */
CANSM_LOCAL void
    CanSM_ChangeBR_S_CC_Stopped(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in Substate CHANGEBR_S_CC_STOPPED_WAIT
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73356
 */
CANSM_LOCAL void
    CanSM_ChangeBR_S_CC_Stopped_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

/**
 * @brief        Handle state transition in Substate CHANGEBR_S_CC_STARTED
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73357
 */
CANSM_LOCAL void
    CanSM_ChangeBR_S_CC_Started(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

/**
 * @brief        Handle state transition in Substate CHANGEBR_S_CC_STARTED_WAIT
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73358
 */
CANSM_LOCAL void
    CanSM_ChangeBR_S_CC_Started_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

/**
 * @brief        Handle state transition in Bsm_S_Change_Baudrate
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @return       boolean
 * @retval       TRUE if the BSM_S_CHANGE_BAUDRATE is finished, FALSE otherwise
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73359
 */
CANSM_LOCAL boolean
    CanSM_BsmSChangeBaudrate(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
#endif

/**
 * @brief        Handle state transition in Bsm_S_SilentCom
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @return       boolean
 * @retval       TRUE if the Bsm_S_SilentCom is finished, FALSE otherwise
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73360
 */
CANSM_LOCAL boolean CanSM_BsmSSilentCom(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        Handle state transition in Bsm_S_SilentCom_Bor
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @return       boolean
 * @retval       TRUE if the BSM_S_SILENTCOM_BOR is finished, FALSE otherwise
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73361
 */
CANSM_LOCAL boolean CanSM_BsmSSilentComBor(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        action of effect E_FULL_COM
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73362
 */
CANSM_LOCAL void CanSM_EFullComAction(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

/**
 * @brief        action of effect E_FULL_TO_SILENT_COM
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73363
 */
CANSM_LOCAL void
    CanSM_EFullToSilentComAction(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

/**
 * @brief        action of effect E_PRE_NOCOM
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73364
 */
CANSM_LOCAL void CanSM_EPreNoComAction(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);
/**
 * @brief        action of trigger T_BUS_OFF
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73365
 */
CANSM_LOCAL void CanSM_EBusOffAction(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

/**
 * @brief        Check all controller mode of the networkindex
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[out]   varPtr    RuntimePtr of the CAN network
 * @param[in]    requestContorllerMode   : CAN controller mode
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73366
 */
CANSM_LOCAL void CanSM_SetControllerModeRepeat(
    const CanSM_ManagerNetworkType* configPtr,
    CanSM_NetWorkRunTimeType*       varPtr,
    Can_ControllerStateType         requestContorllerMode);

/**
 * @brief        Check all controller mode of the networkindex
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[in]    controllerMode   : CAN controller mode
 * @return       boolean
 * @retval       TRUE if all controllers mode of the network is same of controllerMode, FALSE otherwise
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73367
 */
CANSM_LOCAL_INLINE boolean CanSM_CheckNetworkAllControllerMode(
    const CanSM_ManagerNetworkType* configPtr,
    Can_ControllerStateType         controllerMode);

/**
 * @brief        The mode request to CanIf time-out handle
 * @param[in]    configPtr  Pointer to the network configuration
 * @param[inout] varPtr  Runtime pointer of the CAN network
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73368
 */
CANSM_LOCAL void CanSM_ModeRequestTimeout(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr);

/**
 * @brief        Find the index of the partition in the CanSM involved partition according to ApplicationID.
 * @return       uint16
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73369
 */
CANSM_LOCAL_INLINE uint16 CanSM_GetPartitionIndex(void);

#if (STD_ON == CANSM_DEV_ERROR_DETECT)
#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS CanSM_ValidateInitStatus
#pragma PRQA_NO_SIDE_EFFECTS CanSM_ValidateAllNetworksNoCom
#pragma PRQA_NO_SIDE_EFFECTS CanSM_ValidateNetworkRange
#endif
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS CanSM_ValidatePartitionContext
#pragma PRQA_NO_SIDE_EFFECTS CanSM_ValidateNetworkParition
#endif
/**
 * @brief        Check that the current running partition is valid in range.
 * @param[in]    apiId   : The ID of the API to use when calling this function.
 * @return       boolean
 * @retval       TRUE if the currently running partition is valid in range, FALSE otherwise
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73370
 */
CANSM_LOCAL_INLINE boolean CanSM_ValidatePartitionContext(uint8 apiId);

/**
 * @brief        Checks if the current running partition is valid in configuration
 * @param[in]    apiId   : The ID of the API to use when calling this function.
 * @param[in]    network : Handle of the addressed CAN network
 * @return       boolean
 * @retval       TRUE if the current running partition is valid for the network, FALSE otherwise
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73371
 */
CANSM_LOCAL_INLINE boolean CanSM_ValidateNetworkParition(uint8 apiId, NetworkHandleType network);
#endif
/**
 * @brief        Check that the function parameter pointer is valid.
 * @param[in]    apiId   : The ID of the API to use when calling this function.
 * @param[in]    pointer : Input parameter pointer.
 * @return       boolean
 * @retval       TRUE if the parameter pointer is not NULL_PTR, FALSE otherwise
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73372
 */
CANSM_LOCAL_INLINE boolean CanSM_ValidatePointer(uint8 apiId, const void* pointer);
/**
 * @brief        Checks if the current initialization state is valid
 * @param[in]    apiId   : The ID of the API to use when calling this function.
 * @return       boolean
 * @retval       TRUE if the current initialization state is valid, FALSE otherwise
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73373
 */
CANSM_LOCAL_INLINE boolean CanSM_ValidateInitStatus(uint8 apiId);
/**
 * @brief        Checks if the network handle is valid in range
 * @param[in]    apiId   : The ID of the API to use when calling this function.
 * @param[in]    network : Handle of the addressed CAN network
 * @return       boolean
 * @retval       TRUE if the network handle id is valid, FALSE otherwise
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73374
 */
CANSM_LOCAL_INLINE boolean CanSM_ValidateNetworkRange(uint8 apiId, NetworkHandleType network);

/**
 * @brief        Checks if all network communication mode is NoCom state currently
 * @return       boolean
 * @retval       TRUE if all network is in BSM_S_NOCOM, FALSE otherwise
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73375
 */
CANSM_LOCAL_INLINE boolean CanSM_ValidateAllNetworksNoCom(void);
#endif

/**
 * @brief        Forward pdu mode request to CanIf, if ecu is in passive mode, using CANIF_TX_OFFLINE_ACTIVE instead of
 * CANIF_ONLINE.
 * @reentrant    TRUE (only for different controllers)
 * @synchronous  TRUE
 * @trace        CPD-54754
 */
CANSM_LOCAL void CanSM_SetPduMode(uint8 controllerId, CanIf_PduModeType pduModeRequest);

/* ============================================ internal data definitions =========================================== */
#define CANSM_START_SEC_VAR_CLEARED_8
#include "CanSM_MemMap.h"
/**
 *  @brief Module initialize status, TRUE initialized, FALSE not initialized.
 *  @range CANSM_UNINITED, CANSM_INITED
 */
CANSM_LOCAL CanSM_ModuleStatusType CanSM_Status[CANSM_PARTITION_NUM];
#define CANSM_STOP_SEC_VAR_CLEARED_8
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_VAR_CLEARED_PTR
#include "CanSM_MemMap.h"
/**
 * @brief Pointer to the configuration data of the CanSM module.
 * @range Pointer
 */
CANSM_LOCAL const CanSM_ConfigType* CanSM_ConfigPtr;
#define CANSM_STOP_SEC_VAR_CLEARED_PTR
#include "CanSM_MemMap.h"

#if CANSM_TX_OFFLINE_ACTIVE_SUPPORT == STD_ON
#define CANSM_START_SEC_VAR_CLEARED_BOOLEAN
#include "CanSM_MemMap.h"
/**
 * @brief ECU passive mode status(SWS_CanSM_00646), TRUE ECU passive mode, FALSE ECU active mode.
 * @range TRUE, FALSE
 */
CANSM_LOCAL boolean CanSM_EcuPassive = FALSE;
#define CANSM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "CanSM_MemMap.h"
#endif

#define CANSM_START_SEC_CONST_PTR
#include "CanSM_MemMap.h"
/* PRQA S 3218 ++ */ /* VL_CanSM_3218 */
/**
 * @brief The function pointer array of the CanSM behaviors.
 * @range Pointer
 */
/* clang-format off */
CANSM_LOCAL boolean (*const CanSM_DoBehaviors[])(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr) = {
    CanSM_DeinitPnNotSupported,
#if (STD_ON == CANSM_PNC_SUPPORT)
    CanSM_DeinitPnSupported,
#endif
    CanSM_BsmSNoCom,
    CanSM_BsmWuValidation,
    CanSM_BsmSPreFullCom,
    CanSM_BsmSFullCom,
#if (STD_ON == CANSM_SET_BAUDRATE_API)
    CanSM_BsmSChangeBaudrate,
#endif
    CanSM_BsmSSilentCom,
    CanSM_BsmSSilentComBor,
};
/* clang-format on */
/* PRQA S 3218 -- */ /* VL_CanSM_3218 */
#define CANSM_STOP_SEC_CONST_PTR
#include "CanSM_MemMap.h"
/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
/**
 * This service initializes the CanSM module
 */
void CanSM_Init(const CanSM_ConfigType* ConfigPtr) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
#if CANSM_DEV_ERROR_DETECT == STD_ON
    if (CanSM_ValidatePointer(SERVICE_ID_CANSM_INIT, ConfigPtr)
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        && CanSM_ValidatePartitionContext(SERVICE_ID_CANSM_INIT)
#endif
        && CanSM_ValidateInitStatus(SERVICE_ID_CANSM_INIT))
#endif
    {
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        ApplicationType applicationID = GetApplicationID();
#endif
        uint16 partitionIndex = CanSM_GetPartitionIndex();
        CanSM_ConfigPtr       = ConfigPtr;
        /* initialize run time structure variable for every network */
        for (uint8 netLoop = 0u; netLoop < CANSM_NETWORK_NUM; netLoop++)
        {
            const CanSM_ManagerNetworkType* configPtr = &CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop];

#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
            if (configPtr->ApplicationID != applicationID)
            {
                continue; /* PRQA S 0770 */ /* VL_CanSM_0770 */
            }
#endif
            CanSM_NetWorkRunTimeType* varPtr = configPtr->RuntimeNetPtr;

#if (STD_ON == CANSM_PNC_SUPPORT)
            if ((configPtr->TrcvRef != NULL_PTR) && (configPtr->TrcvRef->CanTrcvPnEnabled))
            {
                varPtr->CurBsmState            = CANSM_BSM_DEINITPNSUPPORTED;
                varPtr->DeinitPnSupportedState = DEINITPN_S_PN_CLEAR_WUF;
            }
            else
#endif /*STD_ON == CANSM_PNC_SUPPORT*/
            {
                varPtr->CurBsmState               = CANSM_BSM_DEINITPNNOTSUPPORTED;
                varPtr->DeinitPnNotSupportedState = DEINITPNNOT_S_CC_STOPPED;
            }
            varPtr->ModeRequestRepetitionTime   = 0u;
            varPtr->WaitCanIfIndicatedStartTime = 0u;
            varPtr->BusOffEventStartTime        = 0u;
#if CANSM_BOR_TIME_TX_ENSURED_ENABLED == STD_ON
            varPtr->BorTimeTxEnsuredTime = 0u;
#endif
            varPtr->BusOffCounter  = 0u;
            varPtr->RepeatCounter  = 0u;
            varPtr->BusOffEvent    = FALSE;
            varPtr->CanIfIndicated = T_NO_INDICATED;
            varPtr->RequestComMode = CANSM_NO_REQUEST;
            varPtr->CurComMode     = COMM_NO_COMMUNICATION;
#if (STD_ON == CANSM_SET_BAUDRATE_API)
            varPtr->BaudRateConfigID = 0u;
#endif
#if (STD_ON == CANSM_GET_BUSOFF_DELAY_FUNCTION_USED)
            varPtr->BusOffDelayTime = 0u;
#endif
            const CanSM_ControllerRefType* controllerRef = configPtr->ControllerRef;
            uint8                          controllerNum = controllerRef->NetWorkRefControllerNumber;
            for (uint8 controllerIndex = 0u; controllerIndex < controllerNum; controllerIndex++)
            {
                controllerRef->RuntimeControlModePtr[controllerIndex] = CAN_CS_UNINIT;
            }
        }
        CanSM_Status[partitionIndex] = CANSM_INITED;
    }
}

/**
 * This service de-initializes the CanSM module.
 */
void CanSM_DeInit(void) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
#if CANSM_DEV_ERROR_DETECT == STD_ON
    if (
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        CanSM_ValidatePartitionContext(SERVICE_ID_CANSM_DEINIT) &&
#endif
        CanSM_ValidateInitStatus(SERVICE_ID_CANSM_DEINIT) && CanSM_ValidateAllNetworksNoCom())
#endif
    {
        uint16 partitionIndex        = CanSM_GetPartitionIndex();
        CanSM_Status[partitionIndex] = CANSM_UNINITED;
    }
}

#if CANSM_VERSION_INFO_API == STD_ON
/**
 * This service puts out the version information of this module (module ID, vendor ID, vendor specific version numbers
 * related to BSW00407)
 */
void CanSM_GetVersionInfo(Std_VersionInfoType* VersionInfo)
{
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    if (CanSM_ValidatePointer(SERVICE_ID_CANSM_GETVERSIONINFO, VersionInfo))
#endif
    {
        VersionInfo->moduleID         = CANSM_MODULE_ID;
        VersionInfo->sw_major_version = CANSM_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = CANSM_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = CANSM_SW_PATCH_VERSION;
        VersionInfo->vendorID         = CANSM_VENDOR_ID;
    }
}
#endif

#if (STD_ON == CANSM_SET_BAUDRATE_API)
/**
 * This service shall start an asynchronous process to change the baud rate
 * for the configured CAN controllers of a certain CAN network.
 * Depending on necessary baud rate modifications the controllers might have to reset.
 */
Std_ReturnType CanSM_SetBaudrate(NetworkHandleType Network, uint16 BaudRateConfigID)
{
    Std_ReturnType ret = E_NOT_OK;
#if CANSM_DEV_ERROR_DETECT == STD_ON
    if (
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        CanSM_ValidatePartitionContext(SERVICE_ID_CANSM_SETBAUDRATE) &&
#endif
        CanSM_ValidateInitStatus(SERVICE_ID_CANSM_SETBAUDRATE)
        && CanSM_ValidateNetworkRange(SERVICE_ID_CANSM_SETBAUDRATE, Network)
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        && CanSM_ValidateNetworkParition(SERVICE_ID_CANSM_SETBAUDRATE, Network)
#endif
    )
#endif
    {
        const CanSM_ManagerNetworkType* configPtr = CanSM_FindNetworkByNetworkHandle(Network);
        CanSM_NetWorkRunTimeType*       varPtr    = configPtr->RuntimeNetPtr;
        SchM_Enter_CanSM_Context();
        if ((CANSM_BSM_S_FULLCOM == varPtr->CurBsmState) && (FULLCOM_S_NO_BUS_OFF == varPtr->FullComState))
        {
            varPtr->CurBsmState         = CANSM_BSM_S_CHANGE_BAUDRATE;
            varPtr->ChangeBaudrateState = CHANGEBR_CHANGE_BR_SYNC;
            SchM_Exit_CanSM_Context();
            BswM_CanSM_CurrentState(Network, CANSM_BSWM_CHANGE_BAUDRATE);
            varPtr->BaudRateConfigID = BaudRateConfigID;
            ret                      = E_OK;
        }
        else
        {
            SchM_Exit_CanSM_Context();
        }
    }
    return ret;
}
#endif /*STD_ON == CANSM_SET_BAUDRATE_API*/

#if CANSM_TX_OFFLINE_ACTIVE_SUPPORT == STD_ON
/**
 * This function can be used to set all CanSM channels of the ECU to a receive only mode.
 */
Std_ReturnType CanSM_SetEcuPassive(boolean CanSM_Passive)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    if (
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        CanSM_ValidatePartitionContext(SERVICE_ID_CANSM_SETECUPASSIVE) &&
#endif
        CanSM_ValidateInitStatus(SERVICE_ID_CANSM_SETECUPASSIVE))
#endif
    {
        if (CanSM_Passive != CanSM_EcuPassive)
        {
            CanSM_EcuPassive                               = CanSM_Passive;
            const CanSM_ManagerNetworkType* networkConfigs = CanSM_ConfigPtr->CanSMManagerNetworkRef;
            for (uint8 netLoop = 0u; netLoop < CANSM_NETWORK_NUM; netLoop++)
            {
                const CanSM_ManagerNetworkType* networkConfigPtr           = &networkConfigs[netLoop];
                const CanSM_ControllerRefType*  controllerRef              = networkConfigPtr->ControllerRef;
                uint8                           netWorkRefControllerNumber = controllerRef->NetWorkRefControllerNumber;
                networkConfigPtr->SetNetworkPassive(controllerRef->ControllerId, netWorkRefControllerNumber);
            }
        }
        ret = E_OK;
    }
    return ret;
}

/** The implementation of ComM_ChannelGetCurrentComMode. */
void CanSM_SetNetworkPassive(const uint8* controllerIds, uint8 networkControllerNumber)
{
    for (uint8 controllerIndex = 0u; controllerIndex < networkControllerNumber; controllerIndex++)
    {
        CanIf_PduModeType pduModeReceive;
        uint8             controllerId = controllerIds[controllerIndex];
        SchM_Enter_CanSM_Context();
        if (CanIf_GetPduMode(controllerId, &pduModeReceive) == E_OK)
        {
            if (CanSM_EcuPassive && (pduModeReceive == CANIF_ONLINE))
            {
                (void)CanIf_SetPduMode(controllerId, CANIF_TX_OFFLINE_ACTIVE);
            }
            else if (!CanSM_EcuPassive && (pduModeReceive == CANIF_TX_OFFLINE_ACTIVE))
            {
                (void)CanIf_SetPduMode(controllerId, CANIF_ONLINE);
            }
            else
            {
                ; /* do nothing */
            }
        }
        SchM_Exit_CanSM_Context();
    }
}
#endif /* CANSM_TX_OFFLINE_ACTIVE_SUPPORT == STD_ON */

/**
 * Scheduled function of the CanSM
 */
void CanSM_MainFunction(CanSM_NetworkIndexType netIndex) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
#if CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON
    if (CanSM_ValidatePartitionContext(SERVICE_ID_CANSM_MAINFUNCTION))
#endif
    {
        if (CanSM_Status[CanSM_GetPartitionIndex()] == CANSM_INITED)
        {
            const CanSM_ManagerNetworkType* configPtr = &CanSM_ConfigPtr->CanSMManagerNetworkRef[netIndex];

#if CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON
            if (GetApplicationID() != configPtr->ApplicationID)
            {
                (void)Det_ReportError(
                    CANSM_MODULE_ID,
                    CANSM_INSTANCE_ID,
                    SERVICE_ID_CANSM_MAINFUNCTION,
                    CANSM_E_INVALID_PARTITION_CONTEXT);
            }
            else
#endif
            {
                CanSM_NetWorkRunTimeType* varPtr = configPtr->RuntimeNetPtr;
                CanSM_TimerHandler(varPtr);
                while (CanSM_DoBehaviors[varPtr->CurBsmState](configPtr, varPtr))
                {
                }
            }
        }
    }
}

/**
 * This service shall change the communication mode of a CAN network to the requested one.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType CanSM_RequestComMode(NetworkHandleType network, ComM_ModeType ComM_Mode)
/* PRQA S 1532 -- */ /* VL_QAC_OneFunRef */
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    if (
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        CanSM_ValidatePartitionContext(SERVICE_ID_CANSM_REQUESTCOMMODE) &&
#endif
        CanSM_ValidateInitStatus(SERVICE_ID_CANSM_REQUESTCOMMODE)
        && CanSM_ValidateNetworkRange(SERVICE_ID_CANSM_REQUESTCOMMODE, network)
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        && CanSM_ValidateNetworkParition(SERVICE_ID_CANSM_REQUESTCOMMODE, network)
#endif
    )
#endif
    {
        const CanSM_ManagerNetworkType* configPtr = CanSM_FindNetworkByNetworkHandle(network);
        CanSM_NetWorkRunTimeType*       varPtr    = configPtr->RuntimeNetPtr;
        switch (ComM_Mode)
        {
        case COMM_NO_COMMUNICATION:
            if ((CANSM_BSM_S_FULLCOM == varPtr->CurBsmState)
#if (STD_ON == CANSM_SET_BAUDRATE_API)
                || (CANSM_BSM_S_CHANGE_BAUDRATE == varPtr->CurBsmState)
#endif
                || (CANSM_BSM_S_SILENTCOM == varPtr->CurBsmState) || (CANSM_BSM_S_SILENTCOM_BOR == varPtr->CurBsmState))
            {
                varPtr->RequestComMode = CANSM_NO_COMMUNICATION;
                ret                    = E_OK;
            }
            break;
        case COMM_SILENT_COMMUNICATION:
            if (CANSM_BSM_S_FULLCOM == varPtr->CurBsmState)
            {
                varPtr->RequestComMode = CANSM_SILENT_COMMUNICATION;
                CanSM_EFullToSilentComAction(configPtr, varPtr);
                ret = E_OK;
            }
#if (STD_ON == CANSM_SET_BAUDRATE_API)
            else if (CANSM_BSM_S_CHANGE_BAUDRATE == varPtr->CurBsmState)
            {
                varPtr->RequestComMode = CANSM_SILENT_COMMUNICATION;
                ret                    = E_OK;
            }
#endif
            else
            {
                /* do nothing */
            }
            break;
        case COMM_FULL_COMMUNICATION:
            if (CANSM_BSM_S_SILENTCOM == varPtr->CurBsmState)
            {
                varPtr->RequestComMode = CANSM_FULL_COMMUNICATION;
                CanSM_EFullComAction(configPtr, varPtr);
                ret = E_OK;
            }
            else if (
                (CANSM_BSM_S_NOCOM == varPtr->CurBsmState) || (CANSM_BSM_WUVALIDATION == varPtr->CurBsmState) ||
#if (STD_ON == CANSM_PNC_SUPPORT)
                (CANSM_BSM_DEINITPNSUPPORTED == varPtr->CurBsmState) ||
#endif
                (CANSM_BSM_DEINITPNNOTSUPPORTED == varPtr->CurBsmState))
            {
                varPtr->RequestComMode = CANSM_FULL_COMMUNICATION;
                ret                    = E_OK;
            }
            else
            {
                /* do nothing */
            }
            break;
        default:
            /* do nothing */
            break;
        }
    }
    return ret;
}

/**
 * This service shall put out the current communication mode of a CAN network
 */
/* PRQA S 1503,1532 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef */
Std_ReturnType CanSM_GetCurrentComMode(NetworkHandleType network, ComM_ModeType* ComM_ModePtr)
/* PRQA S 1503,1532 -- */ /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef */
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    if (CanSM_ValidatePointer(SERVICE_ID_CANSM_GETCURRENTCOMMODE, ComM_ModePtr)
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        && CanSM_ValidatePartitionContext(SERVICE_ID_CANSM_GETCURRENTCOMMODE)
#endif
        && CanSM_ValidateInitStatus(SERVICE_ID_CANSM_GETCURRENTCOMMODE)
        && CanSM_ValidateNetworkRange(SERVICE_ID_CANSM_GETCURRENTCOMMODE, network)
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        && CanSM_ValidateNetworkParition(SERVICE_ID_CANSM_GETCURRENTCOMMODE, network)
#endif
    )
#endif
    {
        const CanSM_ManagerNetworkType* configPtr = CanSM_FindNetworkByNetworkHandle(network);
        const CanSM_NetWorkRunTimeType* varPtr    = configPtr->RuntimeNetPtr;
        *ComM_ModePtr                             = varPtr->CurComMode;
        ret                                       = E_OK;
    }
    return ret;
}

/**
 * This function shall be called by EcuM when a wakeup source shall be started.
 */
/* PRQA S 1503,1532 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef */
Std_ReturnType CanSM_StartWakeupSource(NetworkHandleType network)
/* PRQA S 1503,1532 -- */ /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef */
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    if (
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        CanSM_ValidatePartitionContext(SERVICE_ID_CANSM_STARTWAKEUPSOURCE) &&
#endif
        CanSM_ValidateInitStatus(SERVICE_ID_CANSM_STARTWAKEUPSOURCE)
        && CanSM_ValidateNetworkRange(SERVICE_ID_CANSM_STARTWAKEUPSOURCE, network)
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        && CanSM_ValidateNetworkParition(SERVICE_ID_CANSM_STARTWAKEUPSOURCE, network)
#endif
    )
#endif
    {
        const CanSM_ManagerNetworkType* configPtr = CanSM_FindNetworkByNetworkHandle(network);
        CanSM_NetWorkRunTimeType*       varPtr    = configPtr->RuntimeNetPtr;
        if (varPtr->CurBsmState <= CANSM_BSM_S_NOCOM)
        {
#if (CANSM_TRCV_ENABLED == STD_ON)
            if (configPtr->TrcvRef != NULL_PTR)
            {
                varPtr->WuValidationState = WUVALIDATION_S_TRCV_NORMAL;
            }
            else
#endif
            {
                varPtr->WuValidationState = WUVALIDATION_S_CC_STOPPED;
            }
            varPtr->RepeatCounter = 0u;
            varPtr->CurBsmState   = CANSM_BSM_WUVALIDATION;
            ret                   = E_OK;
        }
    }
    return ret;
}

/**
 * This function shall be called by EcuM when a wakeup source shall be stopped
 */
/* PRQA S 1503,1532 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef */
Std_ReturnType CanSM_StopWakeupSource(NetworkHandleType network)
/* PRQA S 1503,1532 -- */ /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef */
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    if (
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        CanSM_ValidatePartitionContext(SERVICE_ID_CANSM_STOPWAKEUPSOURCE) &&
#endif
        CanSM_ValidateInitStatus(SERVICE_ID_CANSM_STOPWAKEUPSOURCE)
        && CanSM_ValidateNetworkRange(SERVICE_ID_CANSM_STOPWAKEUPSOURCE, network)
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        && CanSM_ValidateNetworkParition(SERVICE_ID_CANSM_STOPWAKEUPSOURCE, network)
#endif
    )
#endif
    {
        const CanSM_ManagerNetworkType* configPtr = CanSM_FindNetworkByNetworkHandle(network);
        CanSM_NetWorkRunTimeType*       varPtr    = configPtr->RuntimeNetPtr;
        if (CANSM_BSM_WUVALIDATION == varPtr->CurBsmState)
        {
            varPtr->RepeatCounter = 0u;
#if (STD_ON == CANSM_PNC_SUPPORT)
            if ((configPtr->TrcvRef != NULL_PTR) && (configPtr->TrcvRef->CanTrcvPnEnabled))
            {
                varPtr->DeinitPnSupportedState = DEINITPN_S_PN_CLEAR_WUF;
                varPtr->CurBsmState            = CANSM_BSM_DEINITPNSUPPORTED;
            }
            else
#endif
            {
                varPtr->DeinitPnNotSupportedState = DEINITPNNOT_S_CC_STOPPED;
                varPtr->CurBsmState               = CANSM_BSM_DEINITPNNOTSUPPORTED;
            }
            ret = E_OK;
        }
    }
    return ret;
}

/**
 * This callback function notifies the CanSM about a bus-off event on a certain CAN controller,
 * which needs to be considered with the specified bus-off recovery handling for the impacted
 * CAN network.
 */
void CanSM_ControllerBusOff(uint8 ControllerId) /* PRQA S 1532, 6030 */ /* VL_QAC_OneFunRef, VL_MTR_CanSM_STMIF */
{
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    if (
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        CanSM_ValidatePartitionContext(SERVICE_ID_CANSM_CONTROLLERBUSOFF) &&
#endif
        CanSM_ValidateInitStatus(SERVICE_ID_CANSM_CONTROLLERBUSOFF))
#endif
    {
        boolean findController = FALSE;
        for (CanSM_NetworkIndexType netLoop = 0u; (netLoop < CANSM_NETWORK_NUM) && !findController; netLoop++)
        {
            const CanSM_ManagerNetworkType* configPtr     = &CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop];
            const CanSM_ControllerRefType*  controllerRef = configPtr->ControllerRef;
            uint8                           netWorkRefControllerNumber = controllerRef->NetWorkRefControllerNumber;
            for (uint8 controllerIndex = 0u; controllerIndex < netWorkRefControllerNumber; ++controllerIndex)
            {
                if (ControllerId == controllerRef->ControllerId[controllerIndex])
                {
                    findController = TRUE;
#if CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON
                    if (CanSM_ValidateNetworkParition(
                            SERVICE_ID_CANSM_CONTROLLERBUSOFF,
                            configPtr->ComMNetworkHandleRef))
#endif
                    {
                        controllerRef->RuntimeControlModePtr[controllerIndex] = CAN_CS_STOPPED;

                        CanSM_NetWorkRunTimeType* varPtr = configPtr->RuntimeNetPtr;
                        varPtr->BusOffEvent              = TRUE;
                    }
                    break;
                }
            }
        }
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
        if (!findController)
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_CONTROLLERBUSOFF,
                CANSM_E_PARAM_CONTROLLER);
        }
#endif
    }
}

/**
 * This callback shall notify the CanSM module about a CAN controller mode change.
 */
/* PRQA S 1532, 6030 ++ */ /* VL_QAC_OneFunRef, VL_MTR_CanSM_STMIF */
void CanSM_ControllerModeIndication(uint8 ControllerId, Can_ControllerStateType ControllerMode)
/* PRQA S 1532, 6030 -- */ /* VL_QAC_OneFunRef, VL_MTR_CanSM_STMIF */
{
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    if (
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        CanSM_ValidatePartitionContext(SERVICE_ID_CANSM_CONTROLLERMODEINDICATION) &&
#endif
        CanSM_ValidateInitStatus(SERVICE_ID_CANSM_CONTROLLERMODEINDICATION))
#endif
    {
        boolean findController = FALSE;
        for (CanSM_NetworkIndexType networkIndex = 0u; (networkIndex < CANSM_NETWORK_NUM) && !findController;
             ++networkIndex)
        {
            const CanSM_ManagerNetworkType* configPtr     = &CanSM_ConfigPtr->CanSMManagerNetworkRef[networkIndex];
            const CanSM_ControllerRefType*  controllerRef = configPtr->ControllerRef;
            uint8                           netWorkRefControllerNumber = controllerRef->NetWorkRefControllerNumber;
            for (uint8 controllerIndex = 0u; controllerIndex < netWorkRefControllerNumber; ++controllerIndex)
            {
                if (ControllerId == controllerRef->ControllerId[controllerIndex])
                {
                    findController = TRUE;
#if CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON
                    if (CanSM_ValidateNetworkParition(
                            SERVICE_ID_CANSM_CONTROLLERMODEINDICATION,
                            configPtr->ComMNetworkHandleRef))
#endif
                    {
                        controllerRef->RuntimeControlModePtr[controllerIndex] = ControllerMode;
                    }
                    break;
                }
            }
        }
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
        if (!findController)
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_CONTROLLERMODEINDICATION,
                CANSM_E_PARAM_CONTROLLER);
        }
#endif
    }
}

/**
 * This callback shall notify the CanSM module about a CAN transceiver mode change
 */
/* PRQA S 1503,1532, 6030 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef, VL_MTR_CanSM_STMIF */
void CanSM_TransceiverModeIndication(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode)
/* PRQA S 1503,1532, 6030 -- */ /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef, VL_MTR_CanSM_STMIF */
{
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    if (
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        CanSM_ValidatePartitionContext(SERVICE_ID_CANSM_TRANCEIVERMODEINDICATION) &&
#endif
        CanSM_ValidateInitStatus(SERVICE_ID_CANSM_TRANCEIVERMODEINDICATION))
#endif
    {
        CanSM_NetWorkRunTimeType* varPtr = NULL_PTR;
        for (CanSM_NetworkIndexType netLoop = 0u; netLoop < CANSM_NETWORK_NUM; netLoop++)
        {
            const CanSM_ManagerNetworkType* configPtr = &CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop];
            const CanSM_TrcvRefType*        trcvRef   = configPtr->TrcvRef;
            if (trcvRef != NULL_PTR)
            {
                if (TransceiverId == trcvRef->TransceiverId)
                {
#if CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON
                    if (CanSM_ValidateNetworkParition(
                            SERVICE_ID_CANSM_TRANCEIVERMODEINDICATION,
                            configPtr->ComMNetworkHandleRef))
#endif
                    {
                        varPtr = configPtr->RuntimeNetPtr;
                    }
                    break;
                }
            }
        }

        if (varPtr != NULL_PTR)
        {
            switch (TransceiverMode)
            {
            case CANTRCV_TRCVMODE_NORMAL:
                varPtr->CanIfIndicated = T_TRCV_NORMAL_INDICATED;
                break;
            case CANTRCV_TRCVMODE_STANDBY:
                varPtr->CanIfIndicated = T_TRCV_STANDBY_INDICATED;
                break;
            default:
                /*This logic does not require any processing*/
                break;
            }
        }
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
        else
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_TRANCEIVERMODEINDICATION,
                CANSM_E_PARAM_TRANSCEIVER);
        }
#endif
    }
}

/**
 * This function shall notify the CanSM module, that the CanNm has detected for the affected
 * partial CAN network a tx timeout exception, which shall be recovered within the respective
 * network state machine of the CanSM module
 */
/* PRQA S 1503,1532 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef */
void CanSM_TxTimeoutException(NetworkHandleType Channel)
/* PRQA S 1503,1532 -- */ /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef */
{
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    if (
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        CanSM_ValidatePartitionContext(SERVICE_ID_CANSM_TXTIMEOUTEXCEPTION) &&
#endif
        CanSM_ValidateInitStatus(SERVICE_ID_CANSM_TXTIMEOUTEXCEPTION))
#endif
    {
#if CANSM_DEV_ERROR_DETECT == STD_ON
        if (CanSM_ValidateNetworkRange(SERVICE_ID_CANSM_TXTIMEOUTEXCEPTION, Channel)
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
            && CanSM_ValidateNetworkParition(SERVICE_ID_CANSM_TXTIMEOUTEXCEPTION, Channel)
#endif
        )
#endif
        {
            const CanSM_ManagerNetworkType* configPtr = CanSM_FindNetworkByNetworkHandle(Channel);
            CanSM_NetWorkRunTimeType*       varPtr    = configPtr->RuntimeNetPtr;
            SchM_Enter_CanSM_Context();
            if ((CANSM_BSM_S_FULLCOM == varPtr->CurBsmState) && (FULLCOM_S_NO_BUS_OFF == varPtr->FullComState))
            {
                varPtr->FullComState  = FULLCOM_S_CC_STOPPED;
                varPtr->RepeatCounter = 0u;
            }
            SchM_Exit_CanSM_Context();
        }
    }
}

#if (STD_ON == CANSM_PNC_SUPPORT)
/**
 * This callback function shall indicate the CanIf_ClearTrcvWufFlag API process end for the
 * notified CAN Transceiver.
 */
void CanSM_ClearTrcvWufFlagIndication(uint8 Transceiver) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    if (
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        CanSM_ValidatePartitionContext(SERVICE_ID_CANSM_CLEARTRCVWUFFLAGINDICATION) &&
#endif
        CanSM_ValidateInitStatus(SERVICE_ID_CANSM_CLEARTRCVWUFFLAGINDICATION))
#endif
    {
        CanSM_NetWorkRunTimeType* varPtr = NULL_PTR;
        for (CanSM_NetworkIndexType netLoop = 0u; netLoop < CANSM_NETWORK_NUM; netLoop++)
        {
            const CanSM_ManagerNetworkType* configPtr = &CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop];
            const CanSM_TrcvRefType*        trcvRef   = configPtr->TrcvRef;

            if (trcvRef != NULL_PTR)
            {
                if (Transceiver == trcvRef->TransceiverId)
                {
#if CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON
                    if (GetApplicationID() != configPtr->ApplicationID)
                    {
                        (void)Det_ReportError(
                            CANSM_MODULE_ID,
                            CANSM_INSTANCE_ID,
                            SERVICE_ID_CANSM_CLEARTRCVWUFFLAGINDICATION,
                            CANSM_E_INVALID_PARTITION_CONTEXT);
                    }
                    else
#endif
                    {
                        varPtr = configPtr->RuntimeNetPtr;
                    }
                    break;
                }
            }
        }
        if (varPtr != NULL_PTR)
        {
            varPtr->CanIfIndicated = T_CLEAR_WUF_INDICATED;
        }
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
        else
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_CLEARTRCVWUFFLAGINDICATION,
                CANSM_E_PARAM_TRANSCEIVER);
        }
#endif
    }
}

/**
 * This callback function indicates the CanIf_CheckTrcvWakeFlag API process end for the notified
 * CAN Transceiver.
 */
void CanSM_CheckTransceiverWakeFlagIndication(uint8 Transceiver) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    if (
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        CanSM_ValidatePartitionContext(SERVICE_ID_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION) &&
#endif
        CanSM_ValidateInitStatus(SERVICE_ID_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION))
#endif
    {
        CanSM_NetWorkRunTimeType* varPtr = NULL_PTR;
        for (CanSM_NetworkIndexType netLoop = 0u; netLoop < CANSM_NETWORK_NUM; netLoop++)
        {
            const CanSM_ManagerNetworkType* configPtr = &CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop];
            const CanSM_TrcvRefType*        trcvRef   = configPtr->TrcvRef;
            if (trcvRef != NULL_PTR)
            {
                if (Transceiver == trcvRef->TransceiverId)
                {
#if CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON
                    if (CanSM_ValidateNetworkParition(
                            SERVICE_ID_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION,
                            configPtr->ComMNetworkHandleRef))
#endif
                    {
                        varPtr = configPtr->RuntimeNetPtr;
                    }
                    break;
                }
            }
        }
        if (varPtr != NULL_PTR)
        {
            varPtr->CanIfIndicated = T_CHECK_WFLAG_INDICATED;
        }
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
        else
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION,
                CANSM_E_PARAM_TRANSCEIVER);
        }
#endif
    }
}

/**
 * This callback function indicates that the transceiver is running in PN communication mode
 */
void CanSM_ConfirmPnAvailability(uint8 TransceiverId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
#if (STD_ON == CANSM_DEV_ERROR_DETECT)
    if (
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
        CanSM_ValidatePartitionContext(SERVICE_ID_CANSM_CONFIRMPNAVAILABILITY) &&
#endif
        CanSM_ValidateInitStatus(SERVICE_ID_CANSM_CONFIRMPNAVAILABILITY))
#endif
    {
        CanSM_NetworkIndexType netLoop;
        for (netLoop = 0u; netLoop < CANSM_NETWORK_NUM; netLoop++)
        {
            const CanSM_ManagerNetworkType* configPtr = &CanSM_ConfigPtr->CanSMManagerNetworkRef[netLoop];
            const CanSM_TrcvRefType*        trcvRef   = configPtr->TrcvRef;
            if (trcvRef != NULL_PTR)
            {
                if (TransceiverId == trcvRef->TransceiverId)
                {
#if CANSM_CANNM_CONF_PN_AVA == STD_ON
#if CANSM_DEV_ERROR_DETECT == STD_ON && CANSM_MULTIPLE_PARTITION_USED == STD_ON
                    if (CanSM_ValidateNetworkParition(
                            SERVICE_ID_CANSM_CONFIRMPNAVAILABILITY,
                            configPtr->ComMNetworkHandleRef))
#endif
                    {
                        CanNm_ConfirmPnAvailability(configPtr->ComMNetworkHandleRef);
                    }
#endif
                    break;
                }
            }
        }

#if (STD_ON == CANSM_DEV_ERROR_DETECT)
        if (netLoop >= CANSM_NETWORK_NUM)
        {
            (void)Det_ReportError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_CONFIRMPNAVAILABILITY,
                CANSM_E_PARAM_TRANSCEIVER);
        }
#endif
    }
}

#endif

/* ========================================== internal function definitions ========================================= */
/**
 * Function to handle the request and indication timer periodically
 */
CANSM_LOCAL void CanSM_TimerHandler(CanSM_NetWorkRunTimeType* varPtr)
{
    if (varPtr->ModeRequestRepetitionTime > 0UL)
    {
        varPtr->ModeRequestRepetitionTime--;
    }
    if (varPtr->WaitCanIfIndicatedStartTime > 0UL)
    {
        varPtr->WaitCanIfIndicatedStartTime--;
    }
}

/**
 * The implementation of CanSM_FindNetworkByNetworkHandle.
 */
CANSM_LOCAL const CanSM_ManagerNetworkType* CanSM_FindNetworkByNetworkHandle(NetworkHandleType networkHandle)
{
    const CanSM_ManagerNetworkType* configPtr      = NULL_PTR;
    const CanSM_ManagerNetworkType* networkConfigs = CanSM_ConfigPtr->CanSMManagerNetworkRef;
    for (CanSM_NetworkIndexType networkIndex = 0u; networkIndex < CANSM_NETWORK_NUM; networkIndex++)
    {
        const CanSM_ManagerNetworkType* tConfigPtr = &networkConfigs[networkIndex];
        if (networkHandle == tConfigPtr->ComMNetworkHandleRef)
        {
            configPtr = tConfigPtr;
            break;
        }
    }
    return configPtr;
}

/**
 * To call the API ComM_BusSM_ModeIndication with ComMode
 */
CANSM_LOCAL void
    CanSM_ComMModeIndication(NetworkHandleType network, ComM_ModeType comMode, CanSM_RequestModeType requestedComMode)
{
    if (requestedComMode != CANSM_NO_REQUEST)
    {
        ComM_BusSM_ModeIndication(network, comMode);
    }
}

/**
 * Handle state transition in DEINITPNNOT_S_CC_STOPPED
 */
CANSM_LOCAL void CanSM_PnNot_CC_Stopped(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(configPtr, varPtr, CAN_CS_STOPPED);

    /*enter DEINITPNNOT_S_CC_STOPPED_WAIT state*/
    varPtr->DeinitPnNotSupportedState   = DEINITPNNOT_S_CC_STOPPED_WAIT;
    varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
}

/**
 * Handle state transition in DEINITPNNOT_S_CC_STOPPED_WAIT
 */
CANSM_LOCAL void
    CanSM_PnNot_CC_Stopped_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    if (CanSM_CheckNetworkAllControllerMode(configPtr, CAN_CS_STOPPED))
    {
        varPtr->DeinitPnNotSupportedState   = DEINITPNNOT_S_CC_SLEEP;
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_CC_STOPPED_TIMEOUT*/
        if (0UL == varPtr->WaitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(configPtr, varPtr);
            varPtr->DeinitPnNotSupportedState = DEINITPNNOT_S_CC_STOPPED;
        }
    }
}

/**
 * Handle state transition in DEINITPNNOT_S_CC_SLEEP
 */
CANSM_LOCAL void CanSM_PnNot_CC_Sleep(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(configPtr, varPtr, CAN_CS_SLEEP);

    /*enter DEINITPNNOT_S_CC_SLEEP_WAIT state*/
    varPtr->DeinitPnNotSupportedState   = DEINITPNNOT_S_CC_SLEEP_WAIT;
    varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
}

/**
 * Handle state transition in DEINITPNNOT_S_CC_SLEEP_WAIT
 */
CANSM_LOCAL void CanSM_PnNot_CC_Sleep_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    if (CanSM_CheckNetworkAllControllerMode(configPtr, CAN_CS_SLEEP))
    {
#if (CANSM_TRCV_ENABLED == STD_ON)
        if (configPtr->TrcvRef != NULL_PTR)
        {
            varPtr->DeinitPnNotSupportedState = DEINITPNNOT_S_TRCV_NORMAL;
        }
        else
#endif
        {
            varPtr->CurBsmState = CANSM_BSM_S_NOCOM;
            /*do E_NOCOM action*/
            CanSM_ComMModeIndication(configPtr->ComMNetworkHandleRef, COMM_NO_COMMUNICATION, varPtr->RequestComMode);
        }
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_CC_SLEEP_TIMEOUT*/
        if (0UL == varPtr->WaitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(configPtr, varPtr);
            varPtr->DeinitPnNotSupportedState = DEINITPNNOT_S_CC_SLEEP;
        }
    }
}

#if (CANSM_TRCV_ENABLED == STD_ON)
/**
 * Handle state transition in DEINITPNNOT_S_TRCV_NORMAL
 */
CANSM_LOCAL void CanSM_PnNot_Trcv_Normal(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    if ((0u == varPtr->ModeRequestRepetitionTime)
        && (varPtr->RepeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        ret = CanIf_SetTrcvMode(configPtr->TrcvRef->TransceiverId, CANTRCV_TRCVMODE_NORMAL);
        if (E_NOT_OK == ret)
        {
            varPtr->ModeRequestRepetitionTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
        }
        varPtr->RepeatCounter++;
    }

    /*enter DEINITPNNOT_S_TRCV_STANDBY state*/
    if (T_TRCV_NORMAL_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->DeinitPnNotSupportedState = DEINITPNNOT_S_TRCV_STANDBY;
        varPtr->RepeatCounter             = 0u;
        varPtr->ModeRequestRepetitionTime = 0u;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (varPtr->RepeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            varPtr->RepeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            const CanSM_DemEventParameterRefType* demEventParameterRef = configPtr->DemEventParameterRefs;
            if (demEventParameterRef != NULL_PTR)
            {
                if (demEventParameterRef->ModeReqTimeoutPara != NULL_PTR)
                {
                    Dem_ReportErrorStatus(*demEventParameterRef->ModeReqTimeoutPara, DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter DEINITPNNOT_S_TRCV_NORMAL_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                varPtr->DeinitPnNotSupportedState   = DEINITPNNOT_S_TRCV_NORMAL_WAIT;
                varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
            }
        }
    }
}

/**
 * Handle state transition in DEINITPNNOT_S_TRCV_NORMAL_WAIT
 */
CANSM_LOCAL void CanSM_PnNot_Trcv_Normal_Wait(CanSM_NetWorkRunTimeType* varPtr)
{
    if (T_TRCV_NORMAL_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->DeinitPnNotSupportedState   = DEINITPNNOT_S_TRCV_STANDBY;
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_TRCV_NORMAL_TIMEOUT*/
        if (0u == varPtr->WaitCanIfIndicatedStartTime)
        {
            varPtr->DeinitPnNotSupportedState = DEINITPNNOT_S_TRCV_NORMAL;
        }
    }
}

/**
 * Handle state transition in DEINITPNNOT_S_TRCV_STANDBY
 */
CANSM_LOCAL void CanSM_PnNot_Trcv_Standby(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    Std_ReturnType ret = E_NOT_OK;

    if ((0u == varPtr->ModeRequestRepetitionTime)
        && (varPtr->RepeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        ret = CanIf_SetTrcvMode(configPtr->TrcvRef->TransceiverId, CANTRCV_TRCVMODE_STANDBY);
        if (E_NOT_OK == ret)
        {
            varPtr->ModeRequestRepetitionTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
        }
        varPtr->RepeatCounter++;
    }

    /*enter CANSM_BSM_S_NOCOM state*/
    if (T_TRCV_STANDBY_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->CurBsmState = CANSM_BSM_S_NOCOM;
        /*do E_NOCOM action*/
        CanSM_ComMModeIndication(configPtr->ComMNetworkHandleRef, COMM_NO_COMMUNICATION, varPtr->RequestComMode);
        varPtr->RepeatCounter             = 0u;
        varPtr->ModeRequestRepetitionTime = 0u;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (varPtr->RepeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            varPtr->RepeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            const CanSM_DemEventParameterRefType* demEventParameterRef = configPtr->DemEventParameterRefs;
            if (demEventParameterRef != NULL_PTR)
            {
                if (demEventParameterRef->ModeReqTimeoutPara != NULL_PTR)
                {
                    Dem_ReportErrorStatus(*demEventParameterRef->ModeReqTimeoutPara, DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter DEINITPNNOT_S_TRCV_STANDBY_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                varPtr->DeinitPnNotSupportedState   = DEINITPNNOT_S_TRCV_STANDBY_WAIT;
                varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
            }
        }
    }
}

/**
 * Handle state transition in DEINITPNNOT_S_TRCV_STANDBY_WAIT
 */
CANSM_LOCAL void
    CanSM_PnNot_Trcv_Standby_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    if (T_TRCV_STANDBY_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->CurBsmState = CANSM_BSM_S_NOCOM;
        /*do E_NOCOM action*/
        CanSM_ComMModeIndication(configPtr->ComMNetworkHandleRef, COMM_NO_COMMUNICATION, varPtr->RequestComMode);
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*CANSM_BSM_T_TRCV_STANDBY_TIMEOUT*/
        if (0u == varPtr->WaitCanIfIndicatedStartTime)
        {
            varPtr->DeinitPnNotSupportedState = DEINITPNNOT_S_TRCV_STANDBY;
        }
    }
}
#endif

/**
 * Handle state transition in DeinitPnNotSupported
 */
CANSM_LOCAL boolean
    CanSM_DeinitPnNotSupported(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    CanSM_DeinitPnNotSupportedStateType lastState = varPtr->DeinitPnNotSupportedState;

    switch (varPtr->DeinitPnNotSupportedState)
    {
    case DEINITPNNOT_S_CC_STOPPED:
        CanSM_PnNot_CC_Stopped(configPtr, varPtr);
        break;
    case DEINITPNNOT_S_CC_STOPPED_WAIT:
        /*enter DEINITPNNOT_S_CC_SLEEP state*/
        CanSM_PnNot_CC_Stopped_Wait(configPtr, varPtr);
        break;
    case DEINITPNNOT_S_CC_SLEEP:
        CanSM_PnNot_CC_Sleep(configPtr, varPtr);
        break;
    case DEINITPNNOT_S_CC_SLEEP_WAIT:
        /*enter DEINITPNNOT_S_TRCV_NORMAL or CANSM_BSM_S_NOCOM state*/
        CanSM_PnNot_CC_Sleep_Wait(configPtr, varPtr);
        break;
#if (CANSM_TRCV_ENABLED == STD_ON)
    case DEINITPNNOT_S_TRCV_NORMAL:
        CanSM_PnNot_Trcv_Normal(configPtr, varPtr);
        break;
    case DEINITPNNOT_S_TRCV_NORMAL_WAIT:
        /*enter DEINITPNNOT_S_TRCV_STANDBY state*/
        CanSM_PnNot_Trcv_Normal_Wait(varPtr);
        break;
    case DEINITPNNOT_S_TRCV_STANDBY:
        CanSM_PnNot_Trcv_Standby(configPtr, varPtr);
        break;
    case DEINITPNNOT_S_TRCV_STANDBY_WAIT:
        /*enter CANSM_BSM_S_NOCOM state*/
        CanSM_PnNot_Trcv_Standby_Wait(configPtr, varPtr);
        break;
#endif
    default:
        /*This logic does not require any processing*/
        break;
    }
    return (varPtr->DeinitPnNotSupportedState != lastState) || (varPtr->CurBsmState != CANSM_BSM_DEINITPNNOTSUPPORTED);
}

#if (STD_ON == CANSM_PNC_SUPPORT)
/**
 * Handle state transition in DEINITPN_S_PN_CLEAR_WUF
 */
CANSM_LOCAL void CanSM_Pn_S_Pn_Clear_WUF(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    Std_ReturnType ret = E_NOT_OK;

    if ((0u == varPtr->ModeRequestRepetitionTime)
        && (varPtr->RepeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        const CanSM_TrcvRefType* trcvRef = configPtr->TrcvRef;
        ret                              = CanIf_ClearTrcvWufFlag(trcvRef->TransceiverId);
        if (E_NOT_OK == ret)
        {
            varPtr->ModeRequestRepetitionTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
        }
        varPtr->RepeatCounter++;
    }
    /*enter DEINITPN_S_PN_CC_STOPPED state*/
    if (T_CLEAR_WUF_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->DeinitPnSupportedState    = DEINITPN_S_PN_CC_STOPPED;
        varPtr->RepeatCounter             = 0u;
        varPtr->ModeRequestRepetitionTime = 0u;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (varPtr->RepeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            varPtr->RepeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            const CanSM_DemEventParameterRefType* demEventParameterRef = configPtr->DemEventParameterRefs;
            if (demEventParameterRef != NULL_PTR)
            {
                if (demEventParameterRef->ModeReqTimeoutPara != NULL_PTR)
                {
                    Dem_ReportErrorStatus(*demEventParameterRef->ModeReqTimeoutPara, DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter DEINITPN_S_PN_CLEAR_WUF_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                varPtr->DeinitPnSupportedState      = DEINITPN_S_PN_CLEAR_WUF_WAIT;
                varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
            }
        }
    }
}

/**
 * Handle state transition in DEINITPN_S_PN_CLEAR_WUF_WAIT
 */
CANSM_LOCAL void CanSM_Pn_S_Pn_Clear_WUF_Wait(CanSM_NetWorkRunTimeType* varPtr)
{
    if (T_CLEAR_WUF_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->DeinitPnSupportedState      = DEINITPN_S_PN_CC_STOPPED;
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_CLEAR_WUF_TIMEOUT*/
        if (0u == varPtr->WaitCanIfIndicatedStartTime)
        {
            varPtr->DeinitPnSupportedState = DEINITPN_S_PN_CLEAR_WUF;
        }
    }
}

/**
 * Handle state transition in DEINITPN_S_PN_CC_STOPPED
 */
CANSM_LOCAL void CanSM_Pn_S_Pn_CC_Stopped(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(configPtr, varPtr, CAN_CS_STOPPED);

    /*enter DEINITPN_S_CC_STOPPED_WAIT state*/
    varPtr->DeinitPnSupportedState      = DEINITPN_S_CC_STOPPED_WAIT;
    varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
}

/**
 * Handle state transition in DEINITPN_S_CC_STOPPED_WAIT
 */
CANSM_LOCAL void
    CanSM_Pn_S_Pn_CC_Stopped_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    if (CanSM_CheckNetworkAllControllerMode(configPtr, CAN_CS_STOPPED))
    {
        varPtr->DeinitPnSupportedState      = DEINITPN_S_TRCV_NORMAL;
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_CC_STOPPED_TIMEOUT*/
        if (0u == varPtr->WaitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(configPtr, varPtr);
            varPtr->DeinitPnSupportedState = DEINITPN_S_PN_CC_STOPPED;
        }
    }
}

/**
 * Handle state transition in DEINITPN_S_TRCV_NORMAL
 */
CANSM_LOCAL void CanSM_Pn_S_Trcv_Normal(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    Std_ReturnType ret = E_NOT_OK;

    if ((0u == varPtr->ModeRequestRepetitionTime)
        && (varPtr->RepeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        const CanSM_TrcvRefType* trcvRef = configPtr->TrcvRef;
        ret                              = CanIf_SetTrcvMode(trcvRef->TransceiverId, CANTRCV_TRCVMODE_NORMAL);
        if (E_NOT_OK == ret)
        {
            varPtr->ModeRequestRepetitionTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
        }
        varPtr->RepeatCounter++;
    }
    /*enter DEINITPN_S_TRCV_STANDBY state*/
    if (T_TRCV_NORMAL_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->DeinitPnSupportedState    = DEINITPN_S_TRCV_STANDBY;
        varPtr->RepeatCounter             = 0u;
        varPtr->ModeRequestRepetitionTime = 0u;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (varPtr->RepeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            varPtr->RepeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            const CanSM_DemEventParameterRefType* demEventParameterRef = configPtr->DemEventParameterRefs;
            if (demEventParameterRef != NULL_PTR)
            {
                if (demEventParameterRef->ModeReqTimeoutPara != NULL_PTR)
                {
                    Dem_ReportErrorStatus(*demEventParameterRef->ModeReqTimeoutPara, DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter DEINITPN_S_TRCV_NORMAL_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                varPtr->DeinitPnSupportedState      = DEINITPN_S_TRCV_NORMAL_WAIT;
                varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
            }
        }
    }
}

/**
 * Handle state transition in DEINITPN_S_TRCV_NORMAL_WAIT
 */
CANSM_LOCAL void CanSM_Pn_S_Trcv_Normal_Wait(CanSM_NetWorkRunTimeType* varPtr)
{
    if (T_TRCV_NORMAL_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->DeinitPnSupportedState      = DEINITPN_S_TRCV_STANDBY;
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_TRCV_NORMAL_TIMEOUT*/
        if (0u == varPtr->WaitCanIfIndicatedStartTime)
        {
            varPtr->DeinitPnSupportedState = DEINITPN_S_TRCV_NORMAL;
        }
    }
}

/**
 * Handle state transition in DEINITPN_S_TRCV_STANDBY
 */
CANSM_LOCAL void CanSM_Pn_S_Trcv_Standby(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    Std_ReturnType ret = E_NOT_OK;

    if ((0u == varPtr->ModeRequestRepetitionTime)
        && (varPtr->RepeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        const CanSM_TrcvRefType* trcvRef = configPtr->TrcvRef;
        ret                              = CanIf_SetTrcvMode(trcvRef->TransceiverId, CANTRCV_TRCVMODE_STANDBY);
        if (E_NOT_OK == ret)
        {
            varPtr->ModeRequestRepetitionTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
            varPtr->RepeatCounter++;
        }
    }
    /*enter DEINITPN_S_CC_SLEEP state*/
    if (T_TRCV_STANDBY_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->DeinitPnSupportedState    = DEINITPN_S_CC_SLEEP;
        varPtr->RepeatCounter             = 0u;
        varPtr->ModeRequestRepetitionTime = 0u;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (varPtr->RepeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            varPtr->RepeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            const CanSM_DemEventParameterRefType* demEventParameterRef = configPtr->DemEventParameterRefs;
            if (demEventParameterRef != NULL_PTR)
            {
                if (demEventParameterRef->ModeReqTimeoutPara != NULL_PTR)
                {
                    Dem_ReportErrorStatus(*demEventParameterRef->ModeReqTimeoutPara, DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter DEINITPN_S_TRCV_STANDBY_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                varPtr->DeinitPnSupportedState      = DEINITPN_S_TRCV_STANDBY_WAIT;
                varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
            }
        }
    }
}

/**
 * Handle state transition in DEINITPN_S_TRCV_STANDBY_WAIT
 */
CANSM_LOCAL void CanSM_Pn_S_Trcv_Standby_Wait(CanSM_NetWorkRunTimeType* varPtr)
{
    if (T_TRCV_STANDBY_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->DeinitPnSupportedState      = DEINITPN_S_CC_SLEEP;
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*CANSM_BSM_T_TRCV_STANDBY_TIMEOUT*/
        if (0u == varPtr->WaitCanIfIndicatedStartTime)
        {
            varPtr->DeinitPnSupportedState = DEINITPN_S_TRCV_STANDBY;
        }
    }
}

/**
 * Handle state transition in DEINITPN_S_CC_SLEEP
 */
CANSM_LOCAL void CanSM_Pn_S_CC_Sleep(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(configPtr, varPtr, CAN_CS_SLEEP);

    /*enter DEINITPN_S_CC_SLEEP_WAIT state*/
    varPtr->DeinitPnSupportedState      = DEINITPN_S_CC_SLEEP_WAIT;
    varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
}

/**
 * Handle state transition in DEINITPN_S_CC_SLEEP_WAIT
 */
CANSM_LOCAL void CanSM_Pn_S_CC_Sleep_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    if (CanSM_CheckNetworkAllControllerMode(configPtr, CAN_CS_SLEEP))
    {
        varPtr->DeinitPnSupportedState      = DEINITPN_S_CHECK_WFLAG_IN_CC_SLEEP;
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*CANSM_BSM_T_CC_SLEEP_TIMEOUT*/
        if (0u == varPtr->WaitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(configPtr, varPtr);
            varPtr->DeinitPnSupportedState = DEINITPN_S_CHECK_WFLAG_IN_NOT_CC_SLEEP;
            varPtr->RepeatCounter          = 0u;
        }
    }
}

/**
 * Handle state transition in DEINITPN_S_CHECK_WFLAG_IN_CC_SLEEP
 */
CANSM_LOCAL void
    CanSM_Pn_Check_Wflag_InCCSleep(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    Std_ReturnType ret = E_NOT_OK;

    if ((0u == varPtr->ModeRequestRepetitionTime)
        && (varPtr->RepeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        ret = CanIf_CheckTrcvWakeFlag(configPtr->TrcvRef->TransceiverId);
        if (E_NOT_OK == ret)
        {
            varPtr->ModeRequestRepetitionTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
        }
        varPtr->RepeatCounter++;
    }
    /*enter CANSM_BSM_S_NOCOM state*/
    if (T_CHECK_WFLAG_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->CurBsmState = CANSM_BSM_S_NOCOM;
        /*do E_NOCOM action*/
        CanSM_ComMModeIndication(configPtr->ComMNetworkHandleRef, COMM_NO_COMMUNICATION, varPtr->RequestComMode);
        varPtr->RepeatCounter             = 0u;
        varPtr->ModeRequestRepetitionTime = 0u;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (varPtr->RepeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            varPtr->RepeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            const CanSM_DemEventParameterRefType* demEventParameterRef = configPtr->DemEventParameterRefs;
            if (demEventParameterRef != NULL_PTR)
            {
                if (demEventParameterRef->ModeReqTimeoutPara != NULL_PTR)
                {
                    Dem_ReportErrorStatus(*demEventParameterRef->ModeReqTimeoutPara, DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter DEINITPN_S_CHECK_WUF_IN_CC_SLEEP_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                varPtr->DeinitPnSupportedState      = DEINITPN_S_CHECK_WUF_IN_CC_SLEEP_WAIT;
                varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
            }
        }
    }
}

/**
 * Handle state transition in DEINITPN_S_CHECK_WUF_IN_CC_SLEEP_WAIT
 */
CANSM_LOCAL void
    CanSM_Pn_Check_Wflag_InCCSleepWait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    if (T_CHECK_WFLAG_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->CurBsmState = CANSM_BSM_S_NOCOM;
        /*do E_NOCOM action*/
        CanSM_ComMModeIndication(configPtr->ComMNetworkHandleRef, COMM_NO_COMMUNICATION, varPtr->RequestComMode);
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_CHECK_WFLAG_TIMEOUT*/
        if (0u == varPtr->WaitCanIfIndicatedStartTime)
        {
            varPtr->DeinitPnSupportedState = DEINITPN_S_CHECK_WFLAG_IN_CC_SLEEP;
        }
    }
}

/**
 * Handle state transition in DEINITPN_S_CHECK_WFLAG_IN_NOT_CC_SLEEP
 */
CANSM_LOCAL void
    CanSM_Pn_CheckWflag_InNotCCSleep(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    Std_ReturnType ret = E_NOT_OK;

    if ((0u == varPtr->ModeRequestRepetitionTime)
        && (varPtr->RepeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        const CanSM_TrcvRefType* trcvRef = configPtr->TrcvRef;
        ret                              = CanIf_CheckTrcvWakeFlag(trcvRef->TransceiverId);
        if (E_NOT_OK == ret)
        {
            varPtr->ModeRequestRepetitionTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
        }
        varPtr->RepeatCounter++;
    }
    /*enter DEINITPN_S_PN_CLEAR_WUF state*/
    if (T_CHECK_WFLAG_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->RepeatCounter             = 0u;
        varPtr->ModeRequestRepetitionTime = 0u;
        varPtr->DeinitPnSupportedState    = DEINITPN_S_PN_CLEAR_WUF;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (varPtr->RepeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            varPtr->RepeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            const CanSM_DemEventParameterRefType* demEventParameterRef = configPtr->DemEventParameterRefs;
            if (demEventParameterRef != NULL_PTR)
            {
                if (demEventParameterRef->ModeReqTimeoutPara != NULL_PTR)
                {
                    Dem_ReportErrorStatus(*demEventParameterRef->ModeReqTimeoutPara, DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter DEINITPN_S_CHECK_WUF_IN_NOT_CC_SLEEP_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                varPtr->DeinitPnSupportedState      = DEINITPN_S_CHECK_WUF_IN_NOT_CC_SLEEP_WAIT;
                varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
            }
        }
    }
}

/**
 * Handle state transition in DEINITPN_S_CHECK_WFLAG_IN_NOT_CC_SLEEP
 */
CANSM_LOCAL void CanSM_Pn_CheckWFlag_InNotCCSleepWait(CanSM_NetWorkRunTimeType* varPtr)
{
    if (T_CHECK_WFLAG_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->DeinitPnSupportedState      = DEINITPN_S_PN_CLEAR_WUF;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_CHECK_WFLAG_TIMEOUT*/
        if (0u == varPtr->WaitCanIfIndicatedStartTime)
        {
            varPtr->DeinitPnSupportedState = DEINITPN_S_CHECK_WFLAG_IN_NOT_CC_SLEEP;
        }
    }
}

/**
 * Handle state transition in DeinitPnSupported
 */
CANSM_LOCAL boolean CanSM_DeinitPnSupported(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    CanSM_DeinitPnSupportedStateType lastState = varPtr->DeinitPnSupportedState;

    switch (varPtr->DeinitPnSupportedState)
    {
    case DEINITPN_S_PN_CLEAR_WUF:
        CanSM_Pn_S_Pn_Clear_WUF(configPtr, varPtr);
        break;
    case DEINITPN_S_PN_CLEAR_WUF_WAIT:
        /*enter DEINITPN_S_PN_CC_STOPPED state*/
        CanSM_Pn_S_Pn_Clear_WUF_Wait(varPtr);
        break;
    case DEINITPN_S_PN_CC_STOPPED:
        CanSM_Pn_S_Pn_CC_Stopped(configPtr, varPtr);
        break;
    case DEINITPN_S_CC_STOPPED_WAIT:
        /*enter DEINITPN_S_TRCV_NORMAL state*/
        CanSM_Pn_S_Pn_CC_Stopped_Wait(configPtr, varPtr);
        break;
    case DEINITPN_S_TRCV_NORMAL:
        CanSM_Pn_S_Trcv_Normal(configPtr, varPtr);
        break;
    case DEINITPN_S_TRCV_NORMAL_WAIT:
        /*enter DEINITPN_S_TRCV_STANDBY state*/
        CanSM_Pn_S_Trcv_Normal_Wait(varPtr);
        break;
    case DEINITPN_S_TRCV_STANDBY:
        CanSM_Pn_S_Trcv_Standby(configPtr, varPtr);
        break;
    case DEINITPN_S_TRCV_STANDBY_WAIT:
        /*enter DEINITPN_S_CC_SLEEP state*/
        CanSM_Pn_S_Trcv_Standby_Wait(varPtr);
        break;
    case DEINITPN_S_CC_SLEEP:
        CanSM_Pn_S_CC_Sleep(configPtr, varPtr);
        break;
    case DEINITPN_S_CC_SLEEP_WAIT:
        /*enter DEINITPN_S_CHECK_WFLAG_IN_CC_SLEEP state*/
        CanSM_Pn_S_CC_Sleep_Wait(configPtr, varPtr);
        break;
    case DEINITPN_S_CHECK_WFLAG_IN_CC_SLEEP:
        CanSM_Pn_Check_Wflag_InCCSleep(configPtr, varPtr);
        break;
    case DEINITPN_S_CHECK_WUF_IN_CC_SLEEP_WAIT:
        /*enter CANSM_BSM_S_NOCOM state*/
        CanSM_Pn_Check_Wflag_InCCSleepWait(configPtr, varPtr);
        break;
    case DEINITPN_S_CHECK_WFLAG_IN_NOT_CC_SLEEP:
        CanSM_Pn_CheckWflag_InNotCCSleep(configPtr, varPtr);
        break;
    case DEINITPN_S_CHECK_WUF_IN_NOT_CC_SLEEP_WAIT:
        /*enter DEINITPN_S_PN_CLEAR_WUF state*/
        CanSM_Pn_CheckWFlag_InNotCCSleepWait(varPtr);
        break;
    default:
        /*nothing*/
        break;
    }
    return (varPtr->DeinitPnSupportedState != lastState) || (varPtr->CurBsmState != CANSM_BSM_DEINITPNSUPPORTED);
}
#endif

/**
 * Handle state transition in Bsm_S_NoCom
 */
CANSM_LOCAL boolean CanSM_BsmSNoCom(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    if (CANSM_FULL_COMMUNICATION == varPtr->RequestComMode)
    {
        /*enter CANSM_BSM_S_PRE_FULLCOM state*/
        varPtr->CurBsmState = CANSM_BSM_S_PRE_FULLCOM;
#if (CANSM_TRCV_ENABLED == STD_ON)
        /*enter sub PREFULLCOM_S_TRCV_NORMAL state*/
        if (configPtr->TrcvRef != NULL_PTR)
        {
            varPtr->PreFullComState = PREFULLCOM_S_TRCV_NORMAL;
        }
        /*enter sub PREFULLCOM_S_CC_STOPPED state*/
        else
#else
        CANSM_UNUSED(configPtr);
#endif
        {
            varPtr->PreFullComState = PREFULLCOM_S_CC_STOPPED;
        }
        varPtr->RepeatCounter = 0u;
    }
    return varPtr->CurBsmState != CANSM_BSM_S_NOCOM;
}

#if (CANSM_TRCV_ENABLED == STD_ON)
/**
 * Handle state transition in WUVALIDATION_S_TRCV_NORMAL
 */
CANSM_LOCAL void CanSM_WUVAL_S_Trcv_Normal(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    if ((0u == varPtr->ModeRequestRepetitionTime)
        && (varPtr->RepeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        ret = CanIf_SetTrcvMode(configPtr->TrcvRef->TransceiverId, CANTRCV_TRCVMODE_NORMAL);
        if (E_NOT_OK == ret)
        {
            varPtr->ModeRequestRepetitionTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
        }
        varPtr->RepeatCounter++;
    }
    /*enter WUVALIDATION_S_CC_STOPPED state*/
    if (T_TRCV_NORMAL_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->WuValidationState         = WUVALIDATION_S_CC_STOPPED;
        varPtr->RepeatCounter             = 0u;
        varPtr->ModeRequestRepetitionTime = 0u;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (varPtr->RepeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            varPtr->RepeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            const CanSM_DemEventParameterRefType* demEventParameterRef = configPtr->DemEventParameterRefs;
            if (demEventParameterRef != NULL_PTR)
            {
                if (demEventParameterRef->ModeReqTimeoutPara != NULL_PTR)
                {
                    Dem_ReportErrorStatus(*demEventParameterRef->ModeReqTimeoutPara, DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter WUVALIDATION_S_TRCV_NORMAL_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                varPtr->WuValidationState           = WUVALIDATION_S_TRCV_NORMAL_WAIT;
                varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
            }
        }
    }
}

/**
 * Handle state transition in WUVALIDATION_S_TRCV_NORMAL_WAIT
 */
CANSM_LOCAL void CanSM_WUVAL_S_Trcv_Normal_Wait(CanSM_NetWorkRunTimeType* varPtr)
{
    if (T_TRCV_NORMAL_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->WuValidationState           = WUVALIDATION_S_CC_STOPPED;
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_TRCV_NORMAL_TIMEOUT*/
        if (0u == varPtr->WaitCanIfIndicatedStartTime)
        {
            varPtr->WuValidationState = WUVALIDATION_S_TRCV_NORMAL;
        }
    }
}
#endif

/**
 * Handle state transition in WUVALIDATION_S_CC_STOPPED
 */
CANSM_LOCAL void CanSM_WUVAL_S_CC_Stopped(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    CanSM_SetControllerModeRepeat(configPtr, varPtr, CAN_CS_STOPPED);

    /*enter WUVALIDATION_S_CC_STOPPED_WAIT state*/
    varPtr->WuValidationState           = WUVALIDATION_S_CC_STOPPED_WAIT;
    varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
}

/**
 * Handle state transition in WUVALIDATION_S_CC_STOPPED_WAIT
 */
CANSM_LOCAL void
    CanSM_WUVAL_S_CC_Stopped_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    if (CanSM_CheckNetworkAllControllerMode(configPtr, CAN_CS_STOPPED))
    {
        varPtr->WuValidationState           = WUVALIDATION_S_CC_STARTED;
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_CC_STOPPED_TIMEOUT*/
        if (0UL == varPtr->WaitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(configPtr, varPtr);
            varPtr->WuValidationState = WUVALIDATION_S_CC_STOPPED;
        }
    }
}

/**
 * Handle state transition in WUVALIDATION_S_CC_STARTED
 */
CANSM_LOCAL void CanSM_WUVAL_S_CC_Started(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(configPtr, varPtr, CAN_CS_STARTED);

    /*enter WUVALIDATION_S_CC_STARTED_WAIT state*/
    varPtr->WuValidationState           = WUVALIDATION_S_CC_STARTED_WAIT;
    varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
}

/**
 * Handle state transition in WUVALIDATION_S_CC_STARTED_WAIT
 */
CANSM_LOCAL void
    CanSM_WUVAL_S_CC_Started_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    if (CanSM_CheckNetworkAllControllerMode(configPtr, CAN_CS_STARTED))
    {
        varPtr->WuValidationState           = WUVALIDATION_WAIT_WUVALIDATION_LEAVE;
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_CC_STARTED_TIMEOUT*/
        if (0UL == varPtr->WaitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(configPtr, varPtr);
            varPtr->WuValidationState = WUVALIDATION_S_CC_STARTED;
        }
    }
}

/**
 * Handle state transition in Bsm_WuValidation
 */
CANSM_LOCAL boolean CanSM_BsmWuValidation(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    CanSM_WUValidationStateType lastState = varPtr->WuValidationState;

    if (CANSM_FULL_COMMUNICATION == varPtr->RequestComMode)
    {
        /*enter CANSM_BSM_S_PRE_FULLCOM state*/
        varPtr->CurBsmState = CANSM_BSM_S_PRE_FULLCOM;
#if (CANSM_TRCV_ENABLED == STD_ON)
        /*enter sub PREFULLCOM_S_TRCV_NORMAL state*/
        if (configPtr->TrcvRef != NULL_PTR)
        {
            varPtr->PreFullComState = PREFULLCOM_S_TRCV_NORMAL;
            varPtr->RepeatCounter   = 0u;
        }
        /*enter sub PREFULLCOM_S_CC_STOPPED state*/
        else
#endif
        {
            varPtr->PreFullComState = PREFULLCOM_S_CC_STOPPED;
            varPtr->RepeatCounter   = 0u;
        }
    }
    else
    {
        switch (varPtr->WuValidationState)
        {
#if (CANSM_TRCV_ENABLED == STD_ON)
        case WUVALIDATION_S_TRCV_NORMAL:
            CanSM_WUVAL_S_Trcv_Normal(configPtr, varPtr);
            break;
        case WUVALIDATION_S_TRCV_NORMAL_WAIT:
            /*enter WUVALIDATION_S_CC_STOPPED state*/
            CanSM_WUVAL_S_Trcv_Normal_Wait(varPtr);
            break;
#endif
        case WUVALIDATION_S_CC_STOPPED:
            /*set all controllers of the network to request mode repeat*/
            CanSM_WUVAL_S_CC_Stopped(configPtr, varPtr);
            break;
        case WUVALIDATION_S_CC_STOPPED_WAIT:
            /*enter WUVALIDATION_S_CC_STARTED state*/
            CanSM_WUVAL_S_CC_Stopped_Wait(configPtr, varPtr);
            break;
        case WUVALIDATION_S_CC_STARTED:
            CanSM_WUVAL_S_CC_Started(configPtr, varPtr);
            break;
        case WUVALIDATION_S_CC_STARTED_WAIT:
            /*enter WUVALIDATION_WAIT_WUVALIDATION_LEAVE state*/
            CanSM_WUVAL_S_CC_Started_Wait(configPtr, varPtr);
            break;
        default:
            /*WUVALIDATION_WAIT_WUVALIDATION_LEAVE state will do nothing*/
            break;
        }
    }
    return (varPtr->WuValidationState != lastState) || (varPtr->CurBsmState != CANSM_BSM_WUVALIDATION);
}

#if (CANSM_TRCV_ENABLED == STD_ON)
/**
 * Handle state transition in PREFULLCOM_S_TRCV_NORMAL
 */
CANSM_LOCAL void
    CanSM_Prefullcom_S_Trcv_Normal(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    if ((0u == varPtr->ModeRequestRepetitionTime)
        && (varPtr->RepeatCounter <= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax))
    {
        ret = CanIf_SetTrcvMode(configPtr->TrcvRef->TransceiverId, CANTRCV_TRCVMODE_NORMAL);
        if (E_NOT_OK == ret)
        {
            varPtr->ModeRequestRepetitionTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
        }
        varPtr->RepeatCounter++;
    }
    /*enter PREFULLCOM_S_CC_STOPPED state*/
    if (T_TRCV_NORMAL_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->PreFullComState           = PREFULLCOM_S_CC_STOPPED;
        varPtr->RepeatCounter             = 0u;
        varPtr->ModeRequestRepetitionTime = 0u;
    }
    else
    {
        /*T_REPEAT_MAX*/
        if (varPtr->RepeatCounter > CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
        {
            varPtr->RepeatCounter = 0u;
#if (STD_ON == CANSM_DEM_SUPPORT)
            const CanSM_DemEventParameterRefType* demEventParameterRef = configPtr->DemEventParameterRefs;
            if (demEventParameterRef != NULL_PTR)
            {
                if (demEventParameterRef->ModeReqTimeoutPara != NULL_PTR)
                {
                    Dem_ReportErrorStatus(*demEventParameterRef->ModeReqTimeoutPara, DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif
            (void)Det_ReportRuntimeError(
                CANSM_MODULE_ID,
                CANSM_INSTANCE_ID,
                SERVICE_ID_CANSM_MAINFUNCTION,
                CANSM_E_MODE_REQUEST_TIMEOUT);
        }
        /*enter PREFULLCOM_S_TRCV_NORMAL_WAIT state*/
        else
        {
            if (E_OK == ret)
            {
                varPtr->PreFullComState             = PREFULLCOM_S_TRCV_NORMAL_WAIT;
                varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
            }
        }
    }
}

/**
 * Handle state transition in PREFULLCOM_S_TRCV_NORMAL_WAIT
 */
CANSM_LOCAL void CanSM_Prefullcom_S_Trcv_Normal_Wait(CanSM_NetWorkRunTimeType* varPtr)
{
    /*enter PREFULLCOM_S_CC_STOPPED state*/
    if (T_TRCV_NORMAL_INDICATED == varPtr->CanIfIndicated)
    {
        varPtr->PreFullComState             = PREFULLCOM_S_CC_STOPPED;
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_TRCV_NORMAL_TIMEOUT*/
        if (0u == varPtr->WaitCanIfIndicatedStartTime)
        {
            varPtr->PreFullComState = PREFULLCOM_S_TRCV_NORMAL;
        }
    }
}
#endif

/**
 * Handle state transition in PREFULLCOM_S_CC_STOPPED
 */
CANSM_LOCAL void
    CanSM_Prefullcom_S_CC_Stopped(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(configPtr, varPtr, CAN_CS_STOPPED);

    /*enter PREFULLCOM_S_CC_STOPPED_WAIT state*/
    varPtr->PreFullComState             = PREFULLCOM_S_CC_STOPPED_WAIT;
    varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
}

/**
 * Handle state transition in PREFULLCOM_S_CC_STOPPED_WAIT
 */
CANSM_LOCAL void
    CanSM_Prefullcom_S_CC_Stopped_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*enter PREFULLCOM_S_CC_STARTED state*/
    if (CanSM_CheckNetworkAllControllerMode(configPtr, CAN_CS_STOPPED))
    {
        varPtr->PreFullComState             = PREFULLCOM_S_CC_STARTED;
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_CC_STOPPED_TIMEOUT*/
        if (0UL == varPtr->WaitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(configPtr, varPtr);
            varPtr->PreFullComState = PREFULLCOM_S_CC_STOPPED;
        }
    }
}

/**
 * Handle state transition in PREFULLCOM_S_CC_STARTED
 */
CANSM_LOCAL void
    CanSM_Prefullcom_S_CC_Started(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(configPtr, varPtr, CAN_CS_STARTED);

    /*enter PREFULLCOM_S_CC_STARTED_WAIT state*/
    varPtr->PreFullComState             = PREFULLCOM_S_CC_STARTED_WAIT;
    varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
}

/**
 * Handle state transition in PREFULLCOM_S_CC_STARTED_WAIT
 */
CANSM_LOCAL void
    CanSM_Prefullcom_S_CC_Started_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*do E_FULL_COM action,enter CANSM_BSM_S_FULLCOM,sub FULLCOM_S_BUS_OFF_CHECK state*/
    if (CanSM_CheckNetworkAllControllerMode(configPtr, CAN_CS_STARTED))
    {
        CanSM_EFullComAction(configPtr, varPtr);
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_CC_STARTED_TIMEOUT*/
        if (0UL == varPtr->WaitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(configPtr, varPtr);
            varPtr->PreFullComState = PREFULLCOM_S_CC_STARTED;
        }
    }
}

/**
 * Handle state transition in Bsm_S_Pre_FullCom
 */
CANSM_LOCAL boolean CanSM_BsmSPreFullCom(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    CanSM_PreFullComStateType lastState = varPtr->PreFullComState;

    switch (varPtr->PreFullComState)
    {
#if (CANSM_TRCV_ENABLED == STD_ON)
    case PREFULLCOM_S_TRCV_NORMAL:
        CanSM_Prefullcom_S_Trcv_Normal(configPtr, varPtr);
        break;
    case PREFULLCOM_S_TRCV_NORMAL_WAIT:
        CanSM_Prefullcom_S_Trcv_Normal_Wait(varPtr);
        break;
#endif
    case PREFULLCOM_S_CC_STOPPED:
        CanSM_Prefullcom_S_CC_Stopped(configPtr, varPtr);
        break;
    case PREFULLCOM_S_CC_STOPPED_WAIT:
        CanSM_Prefullcom_S_CC_Stopped_Wait(configPtr, varPtr);
        break;
    case PREFULLCOM_S_CC_STARTED:
        CanSM_Prefullcom_S_CC_Started(configPtr, varPtr);
        break;
    case PREFULLCOM_S_CC_STARTED_WAIT:
        CanSM_Prefullcom_S_CC_Started_Wait(configPtr, varPtr);
        break;
    default:
        /*This logic does not require any processing*/
        break;
    }
    return (varPtr->PreFullComState != lastState) || (varPtr->CurBsmState != CANSM_BSM_S_PRE_FULLCOM);
}

/**
 * Handle state transition in FULLCOM_S_BUS_OFF_CHECK
 */
CANSM_LOCAL void
    CanSM_FullCom_S_Bus_off_Check(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    if (varPtr->BusOffEvent)
    {
        CanSM_EBusOffAction(configPtr, varPtr);
    }
    else
    {
        boolean busOffPassive = TRUE;

#if CANSM_BOR_TIME_TX_ENSURED_ENABLED == STD_ON
#if CANSM_BOR_TX_CONFIRMATION_POLLING_ENABLED == STD_ON
        if (!configPtr->BorTxConfirmationPolling)
#endif
        {
            if (varPtr->BorTimeTxEnsuredTime > 0UL)
            {
                varPtr->BorTimeTxEnsuredTime--;
            }
            /*the time duration since the effect E_TX_ON is greater or equal the configuration parameter
             * CANSM_BOR_TIME_TX_ENSURED*/
            if (varPtr->BorTimeTxEnsuredTime != 0UL)
            {
                busOffPassive = FALSE;
            }
        }
#endif

#if CANSM_BOR_TX_CONFIRMATION_POLLING_ENABLED == STD_ON
#if CANSM_BOR_TIME_TX_ENSURED_ENABLED == STD_ON
        if (configPtr->BorTxConfirmationPolling)
#endif
        {
            const CanSM_ControllerRefType* controllerRef              = configPtr->ControllerRef;
            uint8                          netWorkRefControllerNumber = controllerRef->NetWorkRefControllerNumber;
            for (uint8 controllerIndex = 0u; controllerIndex < netWorkRefControllerNumber; ++controllerIndex)
            {
                uint8 controllerId = controllerRef->ControllerId[controllerIndex];
                if (CANIF_TX_RX_NOTIFICATION != CanIf_GetTxConfirmationState(controllerId))
                {
                    busOffPassive = FALSE;
                    break;
                }
            }
        }
#endif
        if (busOffPassive)
        {
#if (STD_ON == CANSM_DEM_SUPPORT)
            const CanSM_DemEventParameterRefType* demEventParameterRef = configPtr->DemEventParameterRefs;
            if (demEventParameterRef != NULL_PTR)
            {
                if (demEventParameterRef->BusOffPara != NULL_PTR)
                {
                    Dem_ReportErrorStatus(*demEventParameterRef->BusOffPara, DEM_EVENT_STATUS_PASSED);
                }
            }
#endif
            varPtr->FullComState  = FULLCOM_S_NO_BUS_OFF;
            varPtr->BusOffCounter = 0u;
        }
    }
}

/**
 * Handle state transition in FULLCOM_S_NO_BUS_OFF
 */
CANSM_LOCAL void CanSM_FullCom_S_No_Bus_Off(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    if (varPtr->BusOffEvent)
    {
        CanSM_EBusOffAction(configPtr, varPtr);
    }
}

/**
 * Handle state transition in FULLCOM_S_CC_STOPPED
 */
CANSM_LOCAL void CanSM_FullCom_S_CC_Stopped(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(configPtr, varPtr, CAN_CS_STOPPED);

    /*enter FULLCOM_S_CC_STOPPED_WAIT state*/
    varPtr->FullComState                = FULLCOM_S_CC_STOPPED_WAIT;
    varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
}

/**
 * Handle state transition in FULLCOM_S_CC_STOPPED_WAIT
 */
CANSM_LOCAL void
    CanSM_FullCom_S_CC_Stopped_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*enter FULLCOM_S_CC_STARTED state*/
    if (CanSM_CheckNetworkAllControllerMode(configPtr, CAN_CS_STOPPED))
    {
        varPtr->FullComState                = FULLCOM_S_CC_STARTED;
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_CC_STOPPED_TIMEOUT*/
        if (0UL == varPtr->WaitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(configPtr, varPtr);
            varPtr->FullComState = FULLCOM_S_CC_STOPPED;
        }
    }
}

/**
 * Handle state transition in FULLCOM_S_CC_STARTED
 */
CANSM_LOCAL void CanSM_FullCom_S_CC_Started(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(configPtr, varPtr, CAN_CS_STARTED);

    /*enter FULLCOM_S_CC_STARTED_WAIT state*/
    varPtr->FullComState                = FULLCOM_S_CC_STARTED_WAIT;
    varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
}

/**
 * Handle state transition in FULLCOM_S_CC_STARTED_WAIT
 */
CANSM_LOCAL void
    CanSM_FullCom_S_CC_Started_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*enter FULLCOM_S_BUS_OFF_CHECK state,clear BusOff counter*/
    if (CanSM_CheckNetworkAllControllerMode(configPtr, CAN_CS_STARTED))
    {
        varPtr->FullComState                = FULLCOM_S_NO_BUS_OFF;
        varPtr->BusOffCounter               = 0u;
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;

        const CanSM_ControllerRefType* controllerRef              = configPtr->ControllerRef;
        uint8                          netWorkRefControllerNumber = controllerRef->NetWorkRefControllerNumber;
        for (uint8 controllerIndex = 0u; controllerIndex < netWorkRefControllerNumber; ++controllerIndex)
        {
            uint8 controllerId = controllerRef->ControllerId[controllerIndex];
            CanSM_SetPduMode(controllerId, CANIF_ONLINE);
        }
    }
    else
    {
        /*T_CC_STARTED_TIMEOUT*/
        if (0UL == varPtr->WaitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(configPtr, varPtr);
            varPtr->FullComState = FULLCOM_S_CC_STARTED;
        }
    }
}

/**
 * Handle state transition in FULLCOM_S_RESTART_CC
 */
CANSM_LOCAL void CanSM_FullCom_S_Restart_CC(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(configPtr, varPtr, CAN_CS_STARTED);

    /*enter FULLCOM_S_RESTART_CC_WAIT state*/
    varPtr->FullComState                = FULLCOM_S_RESTART_CC_WAIT;
    varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
}

/**
 * Handle state transition in FULLCOM_S_RESTART_CC_WAIT
 */
CANSM_LOCAL void
    CanSM_FullCom_S_Restart_CC_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*enter FULLCOM_S_TX_OFF state*/
    if (CanSM_CheckNetworkAllControllerMode(configPtr, CAN_CS_STARTED))
    {
        varPtr->FullComState                = FULLCOM_S_TX_OFF;
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_RESTART_CC_TIMEOUT*/
        if (0UL == varPtr->WaitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(configPtr, varPtr);
            varPtr->FullComState = FULLCOM_S_RESTART_CC;
        }
    }
}

/**
 * Handle state transition in FULLCOM_S_TX_OFF
 */
CANSM_LOCAL void CanSM_FullCom_S_Tx_Off(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    uint32 busOffContinueTime;
    varPtr->BusOffEventStartTime++;
    if (varPtr->BusOffCounter >= configPtr->BorCounterL1ToL2)
    {
        busOffContinueTime = configPtr->BorTimeL2
#if (STD_ON == CANSM_GET_BUSOFF_DELAY_FUNCTION_USED)
                             + (uint32)varPtr->BusOffDelayTime
#endif
            ;
    }
    else
    {
        busOffContinueTime = configPtr->BorTimeL1
#if (STD_ON == CANSM_GET_BUSOFF_DELAY_FUNCTION_USED)
                             + (uint32)varPtr->BusOffDelayTime
#endif
            ;
    }
    /*G_TX_ON state is OK*/
    if (varPtr->BusOffEventStartTime >= busOffContinueTime)
    {
        const CanSM_ControllerRefType* controllerRef              = configPtr->ControllerRef;
        uint8                          netWorkRefControllerNumber = controllerRef->NetWorkRefControllerNumber;
        /*do the E_TX_ON action*/
        for (uint8 controllerIndex = 0u; controllerIndex < netWorkRefControllerNumber; ++controllerIndex)
        {
            uint8 controllerId = controllerRef->ControllerId[controllerIndex];
            CanSM_SetPduMode(controllerId, CANIF_ONLINE);
        }
        BswM_CanSM_CurrentState(configPtr->ComMNetworkHandleRef, CANSM_BSWM_FULL_COMMUNICATION);
        varPtr->CurComMode = COMM_FULL_COMMUNICATION;
        CanSM_ComMModeIndication(configPtr->ComMNetworkHandleRef, varPtr->CurComMode, varPtr->RequestComMode);
        /*enter FULLCOM_S_BUS_OFF_CHECK state*/
        varPtr->FullComState = FULLCOM_S_BUS_OFF_CHECK;
        if (varPtr->BusOffCounter < BUS_OFF_COUNTER_UP_LIMIT)
        {
            varPtr->BusOffCounter++;
        }
#if CANSM_BOR_TIME_TX_ENSURED_ENABLED == STD_ON
#if CANSM_BOR_TX_CONFIRMATION_POLLING_ENABLED == STD_ON
        if (!configPtr->BorTxConfirmationPolling)
#endif
        {
            varPtr->BorTimeTxEnsuredTime = configPtr->BorTimeTxEnsured;
        }
#endif
    }
}

/**
 * Handle state transition in Bsm_S_FullCom
 */
/* PRQA S 5017 ++ */       /* VL_CanSM_5017 */
/* PRQA S 6010, 6070 ++ */ /* VL_MTR_CanSM_STCYC, VL_MTR_CanSM_STCAL */
CANSM_LOCAL boolean CanSM_BsmSFullCom(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
/* PRQA S 6010, 6070 -- */
{
    CanSM_FullComStateType lastState = varPtr->FullComState;

    if (CANSM_NO_COMMUNICATION == varPtr->RequestComMode)
    {
        CanSM_EPreNoComAction(configPtr, varPtr);
    }
    else if (CANSM_SILENT_COMMUNICATION == varPtr->RequestComMode)
    {
        CanSM_EFullToSilentComAction(configPtr, varPtr);
    }
    else
    {
        switch (varPtr->FullComState)
        {
        case FULLCOM_S_BUS_OFF_CHECK:
            CanSM_FullCom_S_Bus_off_Check(configPtr, varPtr);
            break;
        case FULLCOM_S_NO_BUS_OFF:
            /* keep the FULLCOM_S_NO_BUS_OFF state until T_TX_TIMEOUT_EXCEPTION,T_BUS_OFF,T_CHANGE_BR_REQUEST happen */
            CanSM_FullCom_S_No_Bus_Off(configPtr, varPtr);
            break;
        case FULLCOM_S_CC_STOPPED:
            CanSM_FullCom_S_CC_Stopped(configPtr, varPtr);
            break;
        case FULLCOM_S_CC_STOPPED_WAIT:
            CanSM_FullCom_S_CC_Stopped_Wait(configPtr, varPtr);
            break;
        case FULLCOM_S_CC_STARTED:
            CanSM_FullCom_S_CC_Started(configPtr, varPtr);
            break;
        case FULLCOM_S_CC_STARTED_WAIT:
            CanSM_FullCom_S_CC_Started_Wait(configPtr, varPtr);
            break;
        case FULLCOM_S_RESTART_CC:
            CanSM_FullCom_S_Restart_CC(configPtr, varPtr);
            break;
        case FULLCOM_S_RESTART_CC_WAIT:
            CanSM_FullCom_S_Restart_CC_Wait(configPtr, varPtr);
            break;
        case FULLCOM_S_TX_OFF:
            CanSM_FullCom_S_Tx_Off(configPtr, varPtr);
            break;
        default:
            /*This logic does not require any processing*/
            break;
        }
    }
    return (varPtr->FullComState != lastState) || (varPtr->CurBsmState != CANSM_BSM_S_FULLCOM);
}
/* PRQA S 5017 -- */ /* VL_CanSM_5017 */
#if (STD_ON == CANSM_SET_BAUDRATE_API)
/**
 * Handle state transition in Substate CHANGEBR_CHANGE_BR_SYNC
 */
CANSM_LOCAL void
    CanSM_ChangeBR_Change_BR_Sync(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    Std_ReturnType                 ret                        = E_OK;
    const CanSM_ControllerRefType* controllerRef              = configPtr->ControllerRef;
    uint8                          netWorkRefControllerNumber = controllerRef->NetWorkRefControllerNumber;
    for (uint8 controllerIndex = 0u; controllerIndex < netWorkRefControllerNumber; ++controllerIndex)
    {
        uint8 controllerId = controllerRef->ControllerId[controllerIndex];
        if (E_NOT_OK == CanIf_SetBaudrate(controllerId, varPtr->BaudRateConfigID))
        {
            ret = E_NOT_OK;
            break;
        }
    }

    /*all CanIf_SetBaudrate requests returned with E_OK*/
    if (E_OK == ret)
    {
        if (CANSM_NO_COMMUNICATION == varPtr->RequestComMode)
        {
            CanSM_EPreNoComAction(configPtr, varPtr);
        }
        else if (CANSM_SILENT_COMMUNICATION == varPtr->RequestComMode)
        {
            CanSM_EFullToSilentComAction(configPtr, varPtr);
        }
        else
        {
            CanSM_EFullComAction(configPtr, varPtr);
        }
    }
    /*not all CanIf_SetBaudrate requests returned with E_OK,
     * enter CHANGEBR_S_CC_STOPPED state*/
    else
    {
        varPtr->ChangeBaudrateState = CHANGEBR_S_CC_STOPPED;
        varPtr->RepeatCounter       = 0u;
    }
}

/**
 * Handle state transition in Substate CHANGEBR_S_CC_STOPPED
 */
CANSM_LOCAL void
    CanSM_ChangeBR_S_CC_Stopped(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(configPtr, varPtr, CAN_CS_STOPPED);

    /*enter CHANGEBR_S_CC_STOPPED_WAIT state*/
    varPtr->ChangeBaudrateState         = CHANGEBR_S_CC_STOPPED_WAIT;
    varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
}

/**
 * Handle state transition in Substate CHANGEBR_S_CC_STOPPED_WAIT
 */
CANSM_LOCAL void
    CanSM_ChangeBR_S_CC_Stopped_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*enter CHANGEBR_S_CC_STARTED state*/
    if (CanSM_CheckNetworkAllControllerMode(configPtr, CAN_CS_STOPPED))
    {
        varPtr->ChangeBaudrateState = CHANGEBR_S_CC_STARTED;
        /*do E_CHANGE_BAUDRATE action*/
        CanSM_ComMModeIndication(configPtr->ComMNetworkHandleRef, COMM_NO_COMMUNICATION, varPtr->RequestComMode);

        const CanSM_ControllerRefType* controllerRef              = configPtr->ControllerRef;
        uint8                          netWorkRefControllerNumber = controllerRef->NetWorkRefControllerNumber;
        for (uint8 controllerIndex = 0u; controllerIndex < netWorkRefControllerNumber; ++controllerIndex)
        {
            uint8 controllerId = controllerRef->ControllerId[controllerIndex];
            (void)CanIf_SetBaudrate(controllerId, varPtr->BaudRateConfigID);
        }

        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_CC_STOPPED_TIMEOUT*/
        if (0u == varPtr->WaitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(configPtr, varPtr);
            varPtr->ChangeBaudrateState = CHANGEBR_S_CC_STOPPED;
        }
    }
}

/**
 * Handle state transition in Substate CHANGEBR_S_CC_STARTED
 */
CANSM_LOCAL void
    CanSM_ChangeBR_S_CC_Started(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*set all controllers of the network to request mode repeat*/
    CanSM_SetControllerModeRepeat(configPtr, varPtr, CAN_CS_STARTED);

    /*enter CHANGEBR_S_CC_STARTED_WAIT state*/
    varPtr->ChangeBaudrateState         = CHANGEBR_S_CC_STARTED_WAIT;
    varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
}

/**
 * Handle state transition in Substate CHANGEBR_S_CC_STARTED_WAIT
 */
CANSM_LOCAL void
    CanSM_ChangeBR_S_CC_Started_Wait(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*exit CANSM_BSM_S_CHANGE_BAUDRATE state*/
    if (CanSM_CheckNetworkAllControllerMode(configPtr, CAN_CS_STARTED))
    {
        if (CANSM_NO_COMMUNICATION == varPtr->RequestComMode)
        {
            CanSM_EPreNoComAction(configPtr, varPtr);
        }
        else if (CANSM_SILENT_COMMUNICATION == varPtr->RequestComMode)
        {
            CanSM_EFullToSilentComAction(configPtr, varPtr);
        }
        else
        {
            CanSM_EFullComAction(configPtr, varPtr);
        }
        varPtr->WaitCanIfIndicatedStartTime = 0u;
        varPtr->RepeatCounter               = 0u;
    }
    else
    {
        /*T_CC_STARTED_TIMEOUT*/
        if (0u == varPtr->WaitCanIfIndicatedStartTime)
        {
            CanSM_ModeRequestTimeout(configPtr, varPtr);
            varPtr->ChangeBaudrateState = CHANGEBR_S_CC_STARTED;
        }
    }
}

/**
 * Handle state transition in Bsm_S_Change_Baudrate
 */
CANSM_LOCAL boolean
    CanSM_BsmSChangeBaudrate(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    CanSM_ChangeBaudrateStateType lastState = varPtr->ChangeBaudrateState;

    switch (varPtr->ChangeBaudrateState)
    {
    case CHANGEBR_CHANGE_BR_SYNC:
        CanSM_ChangeBR_Change_BR_Sync(configPtr, varPtr);
        break;
    case CHANGEBR_S_CC_STOPPED:
        CanSM_ChangeBR_S_CC_Stopped(configPtr, varPtr);
        break;
    case CHANGEBR_S_CC_STOPPED_WAIT:
        CanSM_ChangeBR_S_CC_Stopped_Wait(configPtr, varPtr);
        break;
    case CHANGEBR_S_CC_STARTED:
        CanSM_ChangeBR_S_CC_Started(configPtr, varPtr);
        break;
    case CHANGEBR_S_CC_STARTED_WAIT:
        CanSM_ChangeBR_S_CC_Started_Wait(configPtr, varPtr);
        break;
    default:
        /*nothing*/
        break;
    }
    return (varPtr->ChangeBaudrateState != lastState) || (varPtr->CurBsmState != CANSM_BSM_S_CHANGE_BAUDRATE);
}
#endif

/**
 * Handle state transition in Bsm_S_SilentCom
 */
CANSM_LOCAL boolean CanSM_BsmSSilentCom(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    if (varPtr->BusOffEvent)
    {
        CanSM_EBusOffAction(configPtr, varPtr);
    }
    else if (CANSM_NO_COMMUNICATION == varPtr->RequestComMode)
    {
        CanSM_EPreNoComAction(configPtr, varPtr);
    }
    else if (CANSM_FULL_COMMUNICATION == varPtr->RequestComMode)
    {
        CanSM_EFullComAction(configPtr, varPtr);
    }
    else
    {
        /*This logic does not require any processing*/
    }
    return varPtr->CurBsmState != CANSM_BSM_S_SILENTCOM;
}

/**
 * Handle state transition in Bsm_S_SilentCom_Bor
 */
CANSM_LOCAL boolean CanSM_BsmSSilentComBor(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    CanSM_SilentComBORStateType lastState = varPtr->SilentComBORState;

    if (CANSM_NO_COMMUNICATION == varPtr->RequestComMode)
    {
        CanSM_EPreNoComAction(configPtr, varPtr);
    }
    else
    {
        switch (varPtr->SilentComBORState)
        {
        case SILENTBOR_S_RESTART_CC:
            /*set all controllers of the network to request mode repeat*/
            CanSM_SetControllerModeRepeat(configPtr, varPtr, CAN_CS_STARTED);

            /*enter SILENTBOR_S_RESTART_CC_WAIT state*/
            varPtr->SilentComBORState           = SILENTBOR_S_RESTART_CC_WAIT;
            varPtr->WaitCanIfIndicatedStartTime = CanSM_ConfigPtr->CanSMModeRequestRepetitionTime;
            break;
        case SILENTBOR_S_RESTART_CC_WAIT:
            /*exit CANSM_BSM_S_SILENTCOM_BOR state,enter CANSM_BSM_S_SILENTCOM state*/
            if (CanSM_CheckNetworkAllControllerMode(configPtr, CAN_CS_STARTED))
            {
                varPtr->CurBsmState                 = CANSM_BSM_S_SILENTCOM;
                varPtr->WaitCanIfIndicatedStartTime = 0u;
                varPtr->RepeatCounter               = 0u;
            }
            else
            {
                /*T_RESTART_CC_TIMEOUT*/
                if (0UL == varPtr->WaitCanIfIndicatedStartTime)
                {
                    CanSM_ModeRequestTimeout(configPtr, varPtr);
                    varPtr->SilentComBORState = SILENTBOR_S_RESTART_CC;
                }
            }
            break;
        default:
            /*This logic does not require any processing*/
            break;
        }
    }
    return (varPtr->SilentComBORState != lastState) || (varPtr->CurBsmState != CANSM_BSM_S_SILENTCOM_BOR);
}

/**
 * action of effect E_FULL_COM
 */
CANSM_LOCAL void CanSM_EFullComAction(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    const CanSM_ControllerRefType* controllerRef              = configPtr->ControllerRef;
    uint8                          netWorkRefControllerNumber = controllerRef->NetWorkRefControllerNumber;
    /*do E_FULL_COM action*/
    for (uint8 controllerIndex = 0u; controllerIndex < netWorkRefControllerNumber; ++controllerIndex)
    {
        uint8 controllerId = controllerRef->ControllerId[controllerIndex];
        CanSM_SetPduMode(controllerId, CANIF_ONLINE);
    }

    CanSM_ComMModeIndication(configPtr->ComMNetworkHandleRef, COMM_FULL_COMMUNICATION, varPtr->RequestComMode);
    BswM_CanSM_CurrentState(configPtr->ComMNetworkHandleRef, CANSM_BSWM_FULL_COMMUNICATION);
    /*enter CANSM_BSM_S_FULLCOM,sub FULLCOM_S_BUS_OFF_CHECK state*/
    varPtr->CurBsmState  = CANSM_BSM_S_FULLCOM;
    varPtr->FullComState = FULLCOM_S_BUS_OFF_CHECK;
#if CANSM_BOR_TIME_TX_ENSURED_ENABLED == STD_ON
#if CANSM_BOR_TX_CONFIRMATION_POLLING_ENABLED == STD_ON
    if (!configPtr->BorTxConfirmationPolling)
#endif
    {
        varPtr->BorTimeTxEnsuredTime = configPtr->BorTimeTxEnsured;
    }
#endif
    /*Com Mode change to COMM_FULL_COMMUNICATION*/
    varPtr->CurComMode = COMM_FULL_COMMUNICATION;
}

/**
 * action of effect E_FULL_TO_SILENT_COM
 */
CANSM_LOCAL void
    CanSM_EFullToSilentComAction(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    /*enter CANSM_BSM_S_SILENTCOM state*/
    varPtr->CurBsmState = CANSM_BSM_S_SILENTCOM;
    /*do the E_FULL_TO_SILENT_COM action*/
    BswM_CanSM_CurrentState(configPtr->ComMNetworkHandleRef, CANSM_BSWM_SILENT_COMMUNICATION);

    const CanSM_ControllerRefType* controllerRef              = configPtr->ControllerRef;
    uint8                          netWorkRefControllerNumber = controllerRef->NetWorkRefControllerNumber;
    for (uint8 controllerIndex = 0u; controllerIndex < netWorkRefControllerNumber; ++controllerIndex)
    {
        uint8 controllerId = controllerRef->ControllerId[controllerIndex];
        CanSM_SetPduMode(controllerId, CANIF_TX_OFFLINE);
    }

    CanSM_ComMModeIndication(configPtr->ComMNetworkHandleRef, COMM_SILENT_COMMUNICATION, varPtr->RequestComMode);
    varPtr->CurComMode = COMM_SILENT_COMMUNICATION;
}

/**
 * action of effect E_PRE_NOCOM
 */
CANSM_LOCAL void CanSM_EPreNoComAction(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
/*enter CANSM_BSM_S_PRE_NOCOM state*/
#if (STD_ON == CANSM_PNC_SUPPORT)
    const CanSM_TrcvRefType* trcvRef = configPtr->TrcvRef;
    if ((trcvRef != NULL_PTR) && (trcvRef->CanTrcvPnEnabled))
    {
        varPtr->CurBsmState            = CANSM_BSM_DEINITPNSUPPORTED;
        varPtr->DeinitPnSupportedState = DEINITPN_S_PN_CLEAR_WUF;
    }
    else
#endif
    {
        varPtr->CurBsmState               = CANSM_BSM_DEINITPNNOTSUPPORTED;
        varPtr->DeinitPnNotSupportedState = DEINITPNNOT_S_CC_STOPPED;
    }
    varPtr->RepeatCounter = 0u;
    /*do the E_PRE_NOCOM action*/
    BswM_CanSM_CurrentState(configPtr->ComMNetworkHandleRef, CANSM_BSWM_NO_COMMUNICATION);
    varPtr->CurComMode = COMM_NO_COMMUNICATION;
}

/**
 * action of trigger T_BUS_OFF
 */
CANSM_LOCAL void CanSM_EBusOffAction(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    if (varPtr->CurBsmState == CANSM_BSM_S_FULLCOM)
    {
        BswM_CanSM_CurrentState(configPtr->ComMNetworkHandleRef, CANSM_BSWM_BUS_OFF);
        varPtr->CurComMode = COMM_SILENT_COMMUNICATION;
        CanSM_ComMModeIndication(configPtr->ComMNetworkHandleRef, varPtr->CurComMode, varPtr->RequestComMode);
        varPtr->FullComState = FULLCOM_S_RESTART_CC;

#if (STD_ON == CANSM_GET_BUSOFF_DELAY_FUNCTION_USED)
        if (configPtr->EnableBusOffDelay)
        {
            uint8 delayCycleNum = 0u;
            CANSM_GET_BUSOFF_DELAY_FUNCTION(configPtr->ComMNetworkHandleRef, &delayCycleNum);
            varPtr->BusOffDelayTime = delayCycleNum;
        }
#endif
        varPtr->BusOffEventStartTime = 0u;
    }
    else /* varPtr->CurBsmState == CANSM_BSM_S_SILENTCOM */
    {
        varPtr->CurBsmState       = CANSM_BSM_S_SILENTCOM_BOR;
        varPtr->SilentComBORState = SILENTBOR_S_RESTART_CC;
    }

    varPtr->RepeatCounter = 0u;

#if (STD_ON == CANSM_DEM_SUPPORT)
    const CanSM_DemEventParameterRefType* demEventParameterRef = configPtr->DemEventParameterRefs;
    if (demEventParameterRef != NULL_PTR)
    {
        if (demEventParameterRef->BusOffPara != NULL_PTR)
        {
            Dem_ReportErrorStatus(*demEventParameterRef->BusOffPara, DEM_EVENT_STATUS_PREFAILED);
        }
    }
#endif
}

/**
 * Switch the state of the controller
 */
CANSM_LOCAL void CanSM_SetControllerModeRepeat(
    const CanSM_ManagerNetworkType* configPtr,
    CanSM_NetWorkRunTimeType*       varPtr,
    Can_ControllerStateType         requestContorllerMode)
{
    varPtr->BusOffEvent                                       = FALSE;
    const CanSM_ControllerRefType* controllerRef              = configPtr->ControllerRef;
    uint8                          netWorkRefControllerNumber = controllerRef->NetWorkRefControllerNumber;
    for (uint8 controllerIndex = 0u; controllerIndex < netWorkRefControllerNumber; ++controllerIndex)
    {
        uint8 controllerId = controllerRef->ControllerId[controllerIndex];
        if (requestContorllerMode != controllerRef->RuntimeControlModePtr[controllerIndex])
        {
            (void)CanIf_SetControllerMode(controllerId, requestContorllerMode);
        }
    }
}

/**
 * Check all controller mode of the networkindex
 */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
CANSM_LOCAL_INLINE boolean CanSM_CheckNetworkAllControllerMode(
    const CanSM_ManagerNetworkType* configPtr,
    Can_ControllerStateType         controllerMode)
{
    const CanSM_ControllerRefType* controllerRef = configPtr->ControllerRef;
    uint8                          controllerNum = controllerRef->NetWorkRefControllerNumber;
    for (uint8 index = 0u; index < controllerNum; ++index)
    {
        if (controllerRef->RuntimeControlModePtr[index] != controllerMode)
        {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * The mode request to CanIf time-out handle
 */
CANSM_LOCAL void CanSM_ModeRequestTimeout(const CanSM_ManagerNetworkType* configPtr, CanSM_NetWorkRunTimeType* varPtr)
{
    if (varPtr->RepeatCounter >= CanSM_ConfigPtr->CanSMModeRequestRepetitionMax)
    {
        varPtr->RepeatCounter = 0u;
#if CANSM_DEM_SUPPORT == STD_ON
        const CanSM_DemEventParameterRefType* demEventParameterRef = configPtr->DemEventParameterRefs;
        if (demEventParameterRef != NULL_PTR)
        {
            if (demEventParameterRef->ModeReqTimeoutPara != NULL_PTR)
            {
                Dem_ReportErrorStatus(*demEventParameterRef->ModeReqTimeoutPara, DEM_EVENT_STATUS_PREFAILED);
            }
        }
#else
        CANSM_UNUSED(configPtr);
#endif
        (void)Det_ReportRuntimeError(
            CANSM_MODULE_ID,
            CANSM_INSTANCE_ID,
            SERVICE_ID_CANSM_MAINFUNCTION,
            CANSM_E_MODE_REQUEST_TIMEOUT);
    }
    else
    {
        ++varPtr->RepeatCounter;
    }
}

/**
 * Find the index of the partition in the CanSM involved partition according to ApplicationID.
 */
CANSM_LOCAL_INLINE uint16 CanSM_GetPartitionIndex(void)
{
    uint16 index = 0u;
#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
    ApplicationType applicationID = GetApplicationID();
    for (; index < CANSM_PARTITION_NUM; ++index)
    {
        if (CanSM_OsApplicationList[index] == applicationID)
        {
            break;
        }
    }
#endif
    return index;
}

/** The implementation of CanSM_SetPduMode. */
CANSM_LOCAL void CanSM_SetPduMode(uint8 controllerId, CanIf_PduModeType pduModeRequest)
{
    SchM_Enter_CanSM_Context();
#if CANSM_TX_OFFLINE_ACTIVE_SUPPORT == STD_ON
    (void)CanIf_SetPduMode(
        controllerId,
        ((pduModeRequest == CANIF_ONLINE) && CanSM_EcuPassive) ? CANIF_TX_OFFLINE_ACTIVE : pduModeRequest);
#else
    (void)CanIf_SetPduMode(controllerId, pduModeRequest);
#endif
    SchM_Exit_CanSM_Context();
}

#if CANSM_DEV_ERROR_DETECT == STD_ON
/**
 * Check that the function parameter pointer is valid.
 */
CANSM_LOCAL_INLINE boolean CanSM_ValidatePointer(uint8 apiId, const void* pointer)
{
    if (pointer == NULL_PTR)
    {
        (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, apiId, CANSM_E_PARAM_POINTER);
        return FALSE;
    }
    return TRUE;
}

#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
/**
 * Check that the current running partition is valid in range.
 */
CANSM_LOCAL_INLINE boolean CanSM_ValidatePartitionContext(uint8 apiId)
{
    if (CanSM_GetPartitionIndex() >= CANSM_PARTITION_NUM)
    {
        (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, apiId, CANSM_E_INVALID_PARTITION_CONTEXT);
        return FALSE;
    }
    return TRUE;
}
#endif

/**
 * Checks if the current initialization state is valid
 */
CANSM_LOCAL_INLINE boolean CanSM_ValidateInitStatus(uint8 apiId)
{
    CanSM_ModuleStatusType status = CanSM_Status[CanSM_GetPartitionIndex()];
    if ((status == CANSM_INITED) && (apiId == SERVICE_ID_CANSM_INIT))
    {
        (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, apiId, CANSM_E_ALREADY_INITIALIZED);
        return FALSE;
    }
    else if ((status == CANSM_UNINITED) && (apiId != SERVICE_ID_CANSM_INIT))
    {
        (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, apiId, CANSM_E_UNINIT);
        return FALSE;
    }
    else
    {
        /* do nothing */
    }
    return TRUE;
}

/**
 * Checks if the network handle is valid in range
 */
CANSM_LOCAL_INLINE boolean CanSM_ValidateNetworkRange(uint8 apiId, NetworkHandleType network)
{
    const CanSM_ManagerNetworkType* configPtr = CanSM_FindNetworkByNetworkHandle(network);
    if (configPtr == NULL_PTR)
    {
        (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, apiId, CANSM_E_INVALID_NETWORK_HANDLE);
        return FALSE;
    }
    return TRUE;
}

#if CANSM_MULTIPLE_PARTITION_USED == STD_ON
/**
 * Checks if the current running partition is valid in configuration
 */
CANSM_LOCAL_INLINE boolean CanSM_ValidateNetworkParition(uint8 apiId, NetworkHandleType network)
{
    const CanSM_ManagerNetworkType* configPtr = CanSM_FindNetworkByNetworkHandle(network);
    if (GetApplicationID() != configPtr->ApplicationID)
    {
        (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, apiId, CANSM_E_INVALID_PARTITION_CONTEXT);
        return FALSE;
    }
    return TRUE;
}
#endif

/**
 * Checks if all network communication mode is NoCom state currently
 */
CANSM_LOCAL_INLINE boolean CanSM_ValidateAllNetworksNoCom(void)
{
    for (CanSM_NetworkIndexType networkIndex = 0u; networkIndex < CANSM_NETWORK_NUM; ++networkIndex)
    {
        const CanSM_ManagerNetworkType* configPtr = &CanSM_ConfigPtr->CanSMManagerNetworkRef[networkIndex];
        const CanSM_NetWorkRunTimeType* varPtr    = configPtr->RuntimeNetPtr;
        if (CANSM_BSM_S_NOCOM != varPtr->CurBsmState)
        {
            (void)Det_ReportError(CANSM_MODULE_ID, CANSM_INSTANCE_ID, SERVICE_ID_CANSM_DEINIT, CANSM_E_NOT_IN_NO_COM);
            return FALSE;
        }
    }
    return TRUE;
}
#endif
/* PRQA S 2889 -- */

#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"
/* PRQA S 6060, 1532 -- */
