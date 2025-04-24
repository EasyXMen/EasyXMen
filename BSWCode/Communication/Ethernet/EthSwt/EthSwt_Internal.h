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
************************************************************************************************************************
**                                                                                                                    **
**  FILENAME    : EthSwt_Internal.h                                                                                   **
**                                                                                                                    **
**  Created on  : 2021-10-13                                                                                          **
**  Author      : zhiqiang.huang                                                                                      **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : none                                                                                                **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/

/***********************************************************************************************************************
**                                          REVISION   HISTORY                                                        **
***********************************************************************************************************************/
/* <VERSION>    <DATE>       <AUTHOR>        <REVISION LOG>
 *  V1.0.0    [20211013]  [zhiqiang.huang]   Initial version
 **********************************************************************************************************************/
#ifndef ETHSWT_INTERNAL_H_
#define ETHSWT_INTERNAL_H_
/***********************************************************************************************************************
 *  INCLUDES
 ***********************************************************************************************************************/
#include "istd_lib.h"
#include "EthSwt_Cfg.h"
#include "EthSwt_Types.h"
#include "EthSwt_PBCfg.h"
#include "EthSwt_88Q5050.h"
#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
#include "Det.h"
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
#include "EthSwt.h"
/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 ***********************************************************************************************************************/
#define ETHSWT_INVALID_PRIO    8u
#define ETHSWT_INVALIDE_VID    4095u
#define ETHSWT_MAC_LEN         6u

#define ETHSWT_MIRROR_NO_USE   0u
#define ETHSWT_MIRROR_IN_USE   1u
#define ETHSWT_MIRROR_TOBE_DEL 2u

/*register bit operations*/
#define ETHSWT_REG_SET_BIT(reg, ofst) (reg) |= ((uint16)0x01 << (ofst))
#define ETHSWT_REG_CLR_BIT(reg, ofst) (reg) &= (~((uint16)0x01 << (ofst)))
#define ETHSWT_REG_GET_BIT(reg, ofst) ((reg) >> (ofst)) & 0x0001u
#define ETHSWT_REG_RVS_BIT(reg, ofst) (reg) ^= (0x01 << (ofst))
#define ETHSWT_REG_WRT_BIT(reg, ofst, bit) \
    ((bit) ? (ETHSWT_REG_SET_BIT((reg), (ofst))) : (ETHSWT_REG_CLR_BIT((reg), (ofst))))

#define ETHSWT_REG_CLR_FIELD(reg, mask)        (reg) &= (mask)
#define ETHSWT_REG_WRT_FIELD(reg, val, ofst)   (reg) |= ((val) << (ofst))
#define ETHSWT_REG_READ_FIELD(temp, reg, mask) (temp) = (reg) & ~(mask)
#define ETHSWT_REG_SHIFT_FIELD(regTemp, ofst)  (regTemp) >> (ofst)

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 ***********************************************************************************************************************/

typedef struct
{
    EthSwt_PortMirrorStateType mirroState;
    EthSwt_PortMirrorCfgType* MirrorInfo;
} EthSwt_MirrorStoreType;

typedef struct
{
    uint8 swtIdx;
    uint8 mirroInnerState;
    NvM_BlockIdType blkId;
    EthSwt_MirrorStoreType MirrorStore;
} EthSwt_MirrorShadowBufType;

typedef struct
{
    NvM_BlockIdType blkId;
    EthSwt_AtuOpInfoType atuInfo[ETHSWT_ATU_SIZE]; /*TBD*/
} EthSwt_ArlTableBufType;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
static inline void ETHSWT_DET(uint8 api, uint8 error)
{
    (void)Det_ReportError((uint16)ETHSWT_MODULE_ID, (uint8)ETHSWT_INSTANCE, api, error);
}
#else
#define ETHSWT_DET(api, error)
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */

static inline void ETHSWT_RUNTIME_ERR(uint8 api, uint8 error)
{
    (void)Det_ReportError((uint16)ETHSWT_MODULE_ID, (uint8)ETHSWT_INSTANCE, api, error);
}
/***********************************************************************************************************************
 *  LOCAL FUNCTION declaration
 ***********************************************************************************************************************/

#if (STD_ON == ETHSWT_WRITE_PORT_MIRROR_CONFIGURATION_API)
FUNC(void, ETHSWT_CODE) EthSwt_MirroredPortRestore(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr);
FUNC(void, ETHSWT_CODE) EthSwt_MirroredPortStore(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr);
#endif /*STD_ON == ETHSWT_WRITE_PORT_MIRROR_CONFIGURATION_API*/

#if (STD_ON == ETHSWT_WRITE_PORT_MIRROR_CONFIGURATION_API)
Std_ReturnType EthSwt_WrtMirrCfgInternal(
    uint8 MirroredSwitchIdx,
    const EthSwt_PortMirrorCfgType* PortMirrorConfigurationPtr,
    boolean enable);
#endif /*STD_ON == ETHSWT_WRITE_PORT_MIRROR_CONFIGURATION_API*/

#if (STD_ON == ETHSWT_SET_SWITCH_PORT_MODE_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_SetSwitchPortModeInternal(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_ModeType PortMode);
#endif /* STD_ON == ETHSWT_SET_SWITCH_PORT_MODE_API */

#if (STD_ON == ETHSWT_SET_MAC_LEARNING_MODE_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_SetMacLearningModeInternal(uint8 SwitchIdx, uint8 SwitchPortIdx, EthSwt_MacLearningType MacLearningMode);
#endif /*STD_ON == ETHSWT_SET_MAC_LEARNING_MODE_API*/

#if (STD_ON == ETHSWT_GET_MAC_LEARNING_MODE_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_GetMacLearningModeInternal(uint8 SwitchIdx, uint8 SwitchPortIdx, EthSwt_MacLearningType* MacLearningMode);
#endif /*STD_ON == ETHSWT_GET_MAC_LEARNING_MODE_API*/

#if (STD_ON == ETHSWT_GET_SWITCH_PORT_MODE_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_GetSwitchPortModeInternal(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_ModeType* SwitchModePtr);
#endif /* STD_ON == ETHSWT_GET_SWITCH_PORT_MODE_API */

FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_ATULoadOrPurgeEntry(uint8 SwitchIdx, const EthSwt_AtuOpInfoType* atuInfo, EthSwt_ATU_OperationType op);

FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_ATUGetEntry(uint8 SwitchIdx, EthSwt_AtuOpInfoType* atuInfo, EthSwt_ATU_OperationType op);

FUNC(Std_ReturnType, ETHSWT_CODE) EthSwt_VTULoadOrPurgeEntry(uint8 SwitchIdx, const EthSwt_VtuOpInfoType* vtuInfo);

FUNC(Std_ReturnType, ETHSWT_CODE) EthSwt_VTUGetEntry(uint8 SwitchIdx, EthSwt_VtuOpInfoType* vtuInfo);

#if (STD_ON == ETHSWT_ENABLE_VLAN_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_EnableVlanInternal(uint8 SwitchIdx, uint8 SwitchPortIdx, uint16 VlanId, boolean Enable);
#endif /*STD_ON == ETHSWT_ENABLE_VLAN_API*/

FUNC(void, ETHSWT_CODE)
EthSwt_PortAllResetVLANTable(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr);

FUNC(void, ETHSWT_CODE) EthSwt_PortIngressInit(uint8 swtIdx, const EthSwt_PortConifgType* portCfgPtr);

FUNC(void, ETHSWT_CODE) EthSwt_PortInit(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr);

FUNC(void, ETHSWT_CODE) EthSwt_VTUVlanInit(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr);

FUNC(void, ETHSWT_CODE) EthSwt_VTUReadDatabase(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr);

FUNC(void, ETHSWT_CODE) EthSwt_ATUStaticMacInit(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr);

FUNC(void, ETHSWT_CODE) EthSwt_ATUReadDatabase(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr);

FUNC(void, ETHSWT_CODE) EthSwt_GLBInit(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr);

FUNC(void, ETHSWT_CODE) EthSwt_SwitchOffAllPorts(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr);

FUNC(void, ETHSWT_CODE) EthSwt_NvMBlkInit(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr);

FUNC(uint8, ETHSWT_CODE)
EthSwt_GetCfgPortIdx(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr, uint8 SwitchPortIdx);

extern VAR(EthSwt_MirrorShadowBufType, ETHSWT_VAR_ZERO_INIT) EthSwt_MirrorShadowBuf[ETHSWT_DEVICE_NUM];
extern VAR(EthSwt_PortMirrorCfgType, ETHSWT_VAR_ZERO_INIT) EthSwt_MirrorCfgBuf[ETHSWT_DEVICE_NUM];
#endif /*ifndef ETHSWT_INTERNAL_H_*/
