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
 */
/* PRQA S 3108-- */
/*
 ********************************************************************************
 ************************************************************************************************************************
 ** **
 **  @file               : ComM_Ch.h **
 **  @version            : V1.0.0 **
 **  @author             : darren.zhang **
 **  @date               : 022/01/07 **
 **  @vendor             : isoft **
 **  @description        : Communication Manager **
 **  @specification(s)   : AUTOSAR classic Platform R19-11 **
 ** **
 ***********************************************************************************************************************/

#ifndef COMM_CH_H_
#define COMM_CH_H_

/*=================================================[inclusions]=======================================================*/

/*====================================================[macros]========================================================*/
#define COMM_BUS_SLEEP 0xffu

/*=============================================[type definitions]=====================================================*/

typedef struct
{

#define COMM_SPEC_NOCOM_NOPENDING_REQ COMM_NO_COM_NO_PENDING_REQUEST
#define COMM_SPEC_NOCOM_REQ_PENDING   COMM_NO_COM_REQUEST_PENDING
#define COMM_SPEC_FULL_COM_REQ        COMM_FULL_COM_NETWORK_REQUESTED
#define COMM_SPEC_FULL_READY_SLEEP    COMM_FULL_COM_READY_SLEEP
#define COMM_SPEC_SILENT_COM          COMM_SILENT_COM
    uint8 specMode;
#define COMM_REQMODE_FULL_COMMUNICATION                     0x0u
#define COMM_REQMODE_FULL_COMMUNICATION_WITH_WAKEUP_REQUEST 0x1u
    uint8 reqMode;
    /** \brief indication of nmif mode */
#if (COMM_USED_MODULE_NM == STD_ON)
#define COMM_NM_IND_BUS_SLEEP        0x0u
#define COMM_NM_IND_PREPARE_BUSSLEEP 0x1u
#define COMM_NM_IND_NETWORK_MODE     0x3u
#define COMM_NM_MODE_EVENT_NONE      0x5u
#endif
    /** \brief indication of actual bus mode */
    ComM_ModeType busSmIndMode;
    /** \brief requested actual bus mode */
    ComM_ModeType busSmReqMode;
    /* function mask */
#define COMM_FUN_ALLOW_STU       0x0u
#define COMM_FUN_PASSIVE_WAKEUP  0x2u
#define COMM_FUN_LIMIT_WAKEUP    0x3u
#define COMM_FUN_LIMIT_NOCOM_REQ 0x4u
#if (COMM_RESET_AFTER_FORCING_NOCOMM == STD_ON)
#define COMM_FUN_LIMIT_NOCOM_FORCE 0x5u
#endif
#define COMM_FUN_NMIF_REQ  0x6u
#define COMM_FUN_BUSSM_IND 0x7u
    uint8 funcMask[1];
    uint32 reqCnt;
    uint8* reqMask;
#if (defined(COMM_NM_VARIANT_LIGHT) || defined(COMM_NM_VARIANT_NONE))
    uint32 durationTmr;
#endif
#if (COMM_USED_MODULE_NM == STD_ON)
    uint8 NmModeEvent;
#endif
} ComM_ChVarType;

/*========================================[external function declarations]============================================*/

BEGIN_C_DECLS
void ComM_ChInit(const ComM_ChannelConfigType* channelConfigPtr, uint32 chNum);

FUNC(void, COMM_CODE)
ComM_ChGetState(uintx chIdx, ComM_StateType* chStu);

ComM_ModeType ComM_ChGetMaxAllowMode(uintx chIdx);

FUNC(Std_ReturnType, COMM_CODE)
ComM_ChGetCurrentComMode(uintx chIdx, ComM_ModeType* ComMode);

FUNC(void, COMM_CODE) ComM_ChFuncSwitchHandle(uintx chIdx, uintx funcIdex, boolean flag);

FUNC(void, COMM_CODE) ComM_ChComAllow(uintx chIdx, boolean allowed);

FUNC(void, COMM_CODE) ComM_ChBusSmModeInd(uintx chIdx, ComM_ModeType comMode);

FUNC(void, COMM_CODE) ComM_ChNmModeInd(uintx chIdx, uint8 indMode);

void ComM_ChannelNmModeIndication(NetworkHandleType Channel, uint8 NmModeEvent);

FUNC(Std_ReturnType, COMM_CODE) ComM_ChRequstCommMode(uintx chIdx, uintx reqIdex, ComM_ModeType comMode);

FUNC(void, COMM_CODE) ComM_ChEcuMWakeup(uintx chIdx);

FUNC(void, COMM_CODE) ComM_ChPassiveWakeup(uintx chIdx);

FUNC(void, COMM_CODE) ComM_ChProcessTimer(uint8 chIdx);

#if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)
extern void ComM_ChLimitWakeUp(uintx chIdx, boolean stu);
#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
extern void ComM_ChLimitNoCom(uintx chIdx, boolean stu);
#endif

#if (COMM_DCM_INDICATION == STD_ON)
FUNC(void, COMM_CODE) ComM_ChDiagIndication(uintx chIdx, boolean flag);
#endif

#if ((COMM_USER_MODESWITCH_NOTIFY == STD_ON) && (COMM_USER_NUMBER > 0))
FUNC(void, COMM_CODE)
ComM_UserStateChangeNotify(P2CONST(ComM_UserConfigType, AUTOMATIC, COMM_APPL_CONST) usrCfgPtr, ComM_ModeType mode);
#endif

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
FUNC(Std_ReturnType, COMM_CODE)
ComM_ChRequstCommModeLimitCheck(uintx chIdx, uintx reqIdex, ComM_ModeType comMode);

extern void ComM_ChGetInhibitionStu(uintx chIdx, ComM_InhibitionStatusType* chStu);
#endif

END_C_DECLS

#endif /* COMM_CH_H_ */
