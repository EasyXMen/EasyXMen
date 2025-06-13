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
 **  @file               : ComM.h **
 **  @version            : V2.1.1 **
 **  @author             : darren.zhang **
 **  @date               : 2017/01/01 **
 **  @vendor             : isoft **
 **  @description        : Communication Manager **
 **  @specification(s)   : AUTOSAR classic Platform R19-11 **
 ** **
 ***********************************************************************************************************************/

#ifndef COMM_H_
#define COMM_H_

/*=================================================[inclusions]=======================================================*/
#include "ComM_Cfg.h"
#include "ComM_PBCfg.h"
#include "ComM_Types.h"

#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
#include "Os.h"
#endif

/*=================================================[macros]===========================================================*/
#define COMM_E_NO_ERROR                  0u
#define COMM_E_UNINIT                    1u
#define COMM_E_WRONG_PARAMETERS          2u
#define COMM_E_PARAM_POINTER             3u
#define COMM_E_INIT_FAILED               4u
#define COMM_E_INVALID_PARTITION_CONTEXT 5u

/*-------------------------------------------------[Service Id]-------------------------------------------------------*/
#define COMM_SID_INIT                      0x01u
#define COMM_SID_DEINIT                    0x02u
#define COMM_SID_GETSTATUS                 0x03u
#define COMM_SID_GETINHIBITIONSTATUS       0x04u
#define COMM_SID_REQUESTCOMMODE            0x05u
#define COMM_SID_GETMAXCOMMODE             0x06u
#define COMM_SID_GETREQUESTEDCOMMODE       0x07u
#define COMM_SID_GETCURRENTCOMMODE         0x08u
#define COMM_SID_PREVENTWAKEUP             0x09u
#define COMM_SID_LIMITCHANNELTONOCOMMODE   0x0bu
#define COMM_SID_LIMITECUTONOCOMMODE       0x0cu
#define COMM_SID_READINHIBITCOUNTER        0x0du
#define COMM_SID_RESETINHIBITCOUNTER       0x0eu
#define COMM_SID_SETECUGROUPCLASSIFICATION 0x0fu
#define COMM_SID_GETVERSIONINFO            0x10u
#define COMM_SID_NM_NETWORKSTARTINDICATION 0x15u
#define COMM_SID_NM_NETWORKMODE            0x18u
#define COMM_SID_NM_PREPAREBUSSLEEPMODE    0x19u
#define COMM_SID_NM_BUSSLEEPMODE           0x1au
#define COMM_SID_NM_RESTARTINDICATION      0x1bu
#define COMM_SID_DCM_ACTIVEDIAGNOSTIC      0x1fu
#define COMM_SID_DCM_INACTIVEDIAGNOSTIC    0x20u
#define COMM_SID_COMMUNICATIONALLOWED      0x35u
#define COMM_SID_ECUM_WAKEUPINDICATION     0x2au
#define COMM_SID_ECUM_PNCWAKEUPINDICATION  0x37u
#define COMM_SID_BUSSM_MODEINDICATION      0x33u
#define COMM_SID_BUSSM_BUSSLEEPMODE        0x34u
#define COMM_SID_MAINFUNCATION             0x60u
#define COMM_SID_PNLEARNREQUEST            0x64u
#define COMM_SID_UPPNCMENBERSHIP           0x65u
#define COMM_SID_NM_REPEATMSGLEFTIND       0x66u
#define COMM_SID_GETPNCTOCHANNELMAPPING    0x68u
#define COMM_SID_UPDATEPNCTOCHANNELMAPPING 0x62u
#define COMM_SID_RESETPNCTOCHANNELMAPPING  0x63u
#define COMM_SID_NM_PNCLEARNBITINDICATION  0x69u
#define COMM_SID_GETCURRENTPNCCOMMODE      0x6au
#define COMM_SID_FORWARDSYNCPNCSHUTDOWN    0x6bu
#define COMM_SID_GETCHANNELAPPLICATIONID   0x70u

/*========================================[external function declarations]============================================*/
BEGIN_C_DECLS

FUNC(void, COMM_CODE)
ComM_Init(P2CONST(ComM_ConfigType, AUTOMATIC, COMM_APPL_CONST) ConfigPtr);

FUNC(void, COMM_CODE) ComM_DeInit(void);

#if (COMM_VERSION_INFO_API == STD_ON)
FUNC(void, COMM_CODE)
ComM_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr);
#endif

FUNC(Std_ReturnType, COMM_CODE)
ComM_GetStatus(ComM_InitStatusType* Status);

FUNC(Std_ReturnType, COMM_CODE)
ComM_RequestComMode(ComM_UserHandleType User, ComM_ModeType ComMode);

FUNC(Std_ReturnType, COMM_CODE)
ComM_GetMaxComMode(ComM_UserHandleType User, ComM_ModeType* ComMode);

FUNC(Std_ReturnType, COMM_CODE)
ComM_GetRequestedComMode(ComM_UserHandleType User, ComM_ModeType* ComMode);

FUNC(Std_ReturnType, COMM_CODE)
ComM_GetCurrentComMode(ComM_UserHandleType User, ComM_ModeType* ComMode);

FUNC(Std_ReturnType, COMM_CODE)
ComM_GetCurrentPNCComMode(ComM_UserHandleType User, ComM_ModeType* ComMode);

#if (COMM_PNC_SUPPORT == STD_ON)
#if (COMM_PNC_GATEWAY_ENABLED == STD_ON) && (COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON)
FUNC(Std_ReturnType, COMM_CODE)
ComM_GetPncToChannelMapping(
    P2VAR(boolean, AUTOMATIC, COMM_APPL_DATA) MappingTable,
    P2VAR(uint8, AUTOMATIC, COMM_APPL_DATA) ChannelCnt,
    P2VAR(uint8, AUTOMATIC, COMM_APPL_DATA) PncCnt);
FUNC(Std_ReturnType, COMM_CODE)
ComM_UpdatePncToChannelMapping(
    P2VAR(uint8, AUTOMATIC, COMM_APPL_DATA) MappingTable,
    VAR(uint8, COMM_APPL_DATA) ChannelCnt,
    VAR(uint8, COMM_APPL_DATA) PncCnt);
FUNC(Std_ReturnType, COMM_CODE) ComM_ResetPncToChannelMapping(void);
#endif
#endif

#if (COMM_PNC_SUPPORT == STD_ON) && (COMM_DYN_PNC_TO_CH_MAPP_SUPPORT == STD_ON)
FUNC(Std_ReturnType, COMM_CODE) ComM_PnLearningRequest(void);
FUNC(Std_ReturnType, COMM_CODE)
ComM_UpdatePncMembership(boolean Control, P2CONST(uint8, AUTOMATIC, COMM_APPL_DATA) PncMembership);
#endif

#if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)
FUNC(Std_ReturnType, COMM_CODE) ComM_PreventWakeUp(NetworkHandleType Channel, boolean Status);
#endif

#if COMM_MODE_LIMITATION_ENABLED == STD_ON
Std_ReturnType ComM_LimitChannelToNoComMode(NetworkHandleType Channel, boolean Status);
Std_ReturnType ComM_LimitECUToNoComMode(boolean Status);
#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) && defined(COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR)
FUNC(Std_ReturnType, COMM_CODE)
ComM_ReadInhibitCounter(P2VAR(uint16, AUTOMATIC, COMM_APPL_DATA) CounterValue);
FUNC(Std_ReturnType, COMM_CODE) ComM_ResetInhibitCounter(void);
#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
/**
 * @brief         Returns the inhibition status of a ComM channel.
 * @id            0x04
 * @synchronous   Synchronous
 * @reentrancy    Non Reentrant
 * @param[in]     Channel   See NetworkHandleType
 * @param[out]    Status    See ComM_InhibitionStatusType
 * @retval        E_OK      Successfully returned Inhibition Status
 * @retval        E_NOT_OK  Return of Inhibition Status failed
 */
extern Std_ReturnType ComM_GetInhibitionStatus(NetworkHandleType Channel, ComM_InhibitionStatusType* Status);

/**
 * @brief         Changes the ECU Group Classification status.
 * @id            0x0f
 * @synchronous   Synchronous
 * @reentrancy    Non Reentrant
 * @param[in]     Status    See ComM_InhibitionStatusType
 * @retval        E_OK      Successfully change the ECU Group Classification Status
 * @retval        E_NOT_OK  Change of the ECU Group Classification Status failed
 */
extern Std_ReturnType ComM_SetECUGroupClassification(ComM_InhibitionStatusType Status);
#endif

#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
/**
 * @brief         Returns the ApplicationID of a ComM channel.
 * @id            0x70
 * @synchronous   Synchronous
 * @reentrancy    Reentrant
 * @retval        The ApplicationID of a ComM channel
 */
extern ApplicationType ComM_GetChannelApplicationID(NetworkHandleType Channel);
#endif

END_C_DECLS

#endif /* COMM_H_ */
