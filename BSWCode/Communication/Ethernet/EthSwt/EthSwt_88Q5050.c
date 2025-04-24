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
**  FILENAME    : EthSwt_88Q5050.c                                                                                    **
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

/***********************************************************************************************************************
 *  INCLUDES
 ***********************************************************************************************************************/
#include "EthSwt_Cfg.h"
#include "EthSwt.h"
#include "EthSwt_88Q5050.h"
#include "EthSwt_Internal.h"
#include "Eth_17_GEthMac.h"
/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 *  LOCAL DATA
 ***********************************************************************************************************************/
#define ETHSWT_START_SEC_VAR_CLEARED_PTR
#include "EthSwt_MemMap.h"
VAR(EthSwt_MirrorShadowBufType, ETHSWT_VAR_ZERO_INIT) EthSwt_MirrorShadowBuf[ETHSWT_DEVICE_NUM]; /*TBD*/
VAR(EthSwt_PortMirrorCfgType, ETHSWT_VAR_ZERO_INIT) EthSwt_MirrorCfgBuf[ETHSWT_DEVICE_NUM];      /*TBD*/
#if (STD_ON == ETHSWT_ARL_NVM_SUPPORT)
static VAR(EthSwt_ArlTableBufType, ETHSWT_VAR_ZERO_INIT) EthSwt_ArlTabShadowBuf[ETHSWT_DEVICE_NUM]; /*TBD*/
#endif /*STD_ON == ETHSWT_ARL_NVM_SUPPORT*/
#define ETHSWT_STOP_SEC_VAR_CLEARED_PTR
#include "EthSwt_MemMap.h"
/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 ***********************************************************************************************************************/
#define ETHSWT_START_SEC_CODE
#include "EthSwt_MemMap.h"

#if (STD_ON == ETHSWT_MIRROR_NVM_SUPPORT)
FUNC(void, ETHSWT_CODE) EthSwt_MirroredPortRestore(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr)
{
    NvM_RequestResultType NvMResult;
    NvM_BlockIdType bldId = EthSwt_MirrorShadowBuf[SwtCfgPtr->SwtIdx].blkId;
    EthSwt_MirrorStoreType* mirrorStorePtr = &EthSwt_MirrorShadowBuf[SwtCfgPtr->SwtIdx].MirrorStore;

    (void)NvM_GetErrorStatus(EthSwt_MirrorShadowBuf[SwtCfgPtr->SwtIdx].blkId, &NvMResult);

    if (NVM_REQ_OK == NvMResult)
    {
        (void)ILib_memcpy(
            mirrorStorePtr,
            NvM_BlockDescriptor[bldId - 1u].NvmRamBlockDataAddress,
            sizeof(EthSwt_MirrorStoreType));
        if (PORT_MIRROR_ENABLED == mirrorStorePtr->mirroState)
        {
            (void)EthSwt_WrtMirrCfgInternal(SwtCfgPtr->SwtIdx, mirrorStorePtr->MirrorInfo, TRUE);
        }
    }
}

FUNC(void, ETHSWT_CODE) EthSwt_MirroredPortStore(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr)
{
    NvM_RequestResultType NvMResult;
    NvM_BlockIdType bldId = EthSwt_MirrorShadowBuf[SwtCfgPtr->SwtIdx].blkId;
    EthSwt_MirrorStoreType* mirrorStorePtr = &EthSwt_MirrorShadowBuf[SwtCfgPtr->SwtIdx].MirrorStore;

    if (ETHSWT_MIRROR_IN_USE == EthSwt_MirrorShadowBuf[SwtCfgPtr->SwtIdx].mirroInnerState)
    {
        (void)ILib_memcpy(
            NvM_BlockDescriptor[bldId - 1u].NvmRamBlockDataAddress,
            mirrorStorePtr,
            sizeof(EthSwt_MirrorStoreType));
        (void)NvM_GetErrorStatus(bldId, &NvMResult);
        if (NvMResult == NVM_REQ_PENDING)
        {
            (void)NvM_CancelJobs(bldId);
        }
        (void)NvM_WriteBlock(bldId, NULL_PTR);
    }
}
#endif /*STD_ON == ETHSWT_MIRROR_NVM_SUPPORT*/

#if (STD_ON == ETHSWT_WRITE_PORT_MIRROR_CONFIGURATION_API)
Std_ReturnType EthSwt_WrtMirrCfgInternal(
    uint8 MirroredSwitchIdx,
    const EthSwt_PortMirrorCfgType* PortMirrorConfigurationPtr,
    boolean enable)
{
    Std_ReturnType retVal = E_NOT_OK;
    EthSwt_RegType regVal;
    uint8 portIdx;

    if ((PortMirrorConfigurationPtr->TrafficDirectionIngressBitMask & 0x1FF) == 0x1FF
        && (PortMirrorConfigurationPtr->TrafficDirectionEgressBitMask & 0x1FF) == 0x1FF)
    {
        /*invalid*/
    }
    else
    {
        for (portIdx = 0u; portIdx < ETHSWT_PORT_MAX; portIdx++)
        {
            if (0 == ((PortMirrorConfigurationPtr->TrafficDirectionIngressBitMask >> portIdx) & 0x1))
            {
                EthSwt_ReadRegister(MirroredSwitchIdx, portIdx, ETHSWT_REG_PORT_CTRL2, &regVal);
                ETHSWT_REG_SET_BIT(regVal, ETHSWT_REG_PORT_CTRL2_EG_MONITOR_SRC);
                EthSwt_WriteRegister(MirroredSwitchIdx, portIdx, ETHSWT_REG_PORT_CTRL2, regVal);
            }
            if (0 == ((PortMirrorConfigurationPtr->TrafficDirectionEgressBitMask >> portIdx) & 0x1))
            {
                EthSwt_ReadRegister(MirroredSwitchIdx, portIdx, ETHSWT_REG_PORT_CTRL2, &regVal);
                ETHSWT_REG_SET_BIT(regVal, ETHSWT_REG_PORT_CTRL2_IN_MONITOR_SRC);
                EthSwt_WriteRegister(MirroredSwitchIdx, portIdx, ETHSWT_REG_PORT_CTRL2, regVal);
            }
        }
        /*TBD*/
        /*these param not support:
        srcMacAddrFilter,0x00..
        dstMacAddrFilter,0x00..
        VlanIdFilter,0xffff
        MirroringPacketDivider,0x0
        MirroringMode,0x0
        ReTaggingVlanId,0xffff
        DoubleTaggingVlanId,0xffff*/
        EthSwt_ReadRegister(MirroredSwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_MON_MGMT_CTRL, &regVal);
        /*set pointer,0x22:MirrorDest*/
        ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_MON_MGMT_POINTTER_MASK);
        ETHSWT_REG_WRT_FIELD(regVal, 0x22, ETHSWT_REG_GLB1_MON_MGMT_POINTTER_OFST);
        /*set data*/
        ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_MON_MGMT_DATA_MASK);
        ETHSWT_REG_WRT_FIELD(regVal, PortMirrorConfigurationPtr->CapturePortIdx, ETHSWT_REG_GLB1_MON_MGMT_DATA_OFST);
        /*load data*/
        ETHSWT_REG_SET_BIT(regVal, ETHSWT_REG_GLB1_MON_MGMT_UPDATE_OFST);
        EthSwt_WriteRegister(MirroredSwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_MON_MGMT_CTRL, regVal);

        if (TRUE == enable)
        {
            EthSwt_MirrorShadowBuf[MirroredSwitchIdx].MirrorStore.mirroState = PORT_MIRROR_ENABLED;
        }
        else
        {
            EthSwt_MirrorShadowBuf[MirroredSwitchIdx].MirrorStore.mirroState = PORT_MIRROR_DISABLED;
        }
        EthSwt_MirrorShadowBuf[MirroredSwitchIdx].mirroInnerState = ETHSWT_MIRROR_IN_USE;
        EthSwt_MirrorShadowBuf[MirroredSwitchIdx].MirrorStore.MirrorInfo =
            (EthSwt_PortMirrorCfgType*)PortMirrorConfigurationPtr;

        retVal = E_OK;
    }

    return retVal;
}
#endif /*STD_ON == ETHSWT_WRITE_PORT_MIRROR_CONFIGURATION_API*/

#if (STD_ON == ETHSWT_SET_SWITCH_PORT_MODE_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_SetSwitchPortModeInternal(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_ModeType PortMode)
{
    Std_ReturnType retVal = E_NOT_OK;
    EthSwt_RegType regVal;
    uint8 _88Q5050_mode;

    if (ETH_MODE_ACTIVE == PortMode)
    {
        _88Q5050_mode = 3u; /*forwarding*/
    }
    else
    {
        _88Q5050_mode = (uint8)PortMode;
    }

    EthSwt_ReadRegister(SwitchIdx, SwitchPortIdx, ETHSWT_REG_PORT_CTRL, &regVal);
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_PORT_CTRL_PORTSTATE_MASK);
    ETHSWT_REG_WRT_FIELD(regVal, (uint16)_88Q5050_mode, ETHSWT_REG_PORT_CTRL_PORTSTATE_OFST);
    EthSwt_WriteRegister(SwitchIdx, SwitchPortIdx, ETHSWT_REG_PORT_CTRL, regVal);

    retVal = E_OK;

    return retVal;
}
#endif /* STD_ON == ETHSWT_SET_SWITCH_PORT_MODE_API */

#if (STD_ON == ETHSWT_SET_MAC_LEARNING_MODE_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_SetMacLearningModeInternal(uint8 SwitchIdx, uint8 SwitchPortIdx, EthSwt_MacLearningType MacLearningMode)
{
    Std_ReturnType retVal = E_NOT_OK;
    EthSwt_RegType regVal;
    boolean mode = TRUE;
    if (MacLearningMode & ETHSWT_MACLEARNING_HWENABLED == 0x01) /*hardware enabled*/
    {
        mode = FALSE;
    }

    EthSwt_ReadRegister(SwitchIdx, SwitchPortIdx, ETHSWT_REG_PORT_ASSOC_VEC, &regVal);

    if (mode)
    {
        ETHSWT_REG_SET_BIT(regVal, ETHSWT_REG_PORT_ASSOC_VEC_LOCKEDPORT_OFST);
    }
    else
    {
        ETHSWT_REG_CLR_BIT(regVal, ETHSWT_REG_PORT_ASSOC_VEC_LOCKEDPORT_OFST);
    }
    EthSwt_WriteRegister(SwitchIdx, SwitchPortIdx, ETHSWT_REG_PORT_ASSOC_VEC, regVal);

    retVal = E_OK;

    return retVal;
}
#endif /*STD_ON == ETHSWT_SET_MAC_LEARNING_MODE_API*/

#if (STD_ON == ETHSWT_GET_MAC_LEARNING_MODE_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_GetMacLearningModeInternal(uint8 SwitchIdx, uint8 SwitchPortIdx, EthSwt_MacLearningType* MacLearningMode)
{
    Std_ReturnType retVal = E_NOT_OK;
    EthSwt_RegType regVal;
    uint8 learningMode;

    EthSwt_ReadRegister(SwitchIdx, SwitchPortIdx, ETHSWT_REG_PORT_ASSOC_VEC, &regVal);
    learningMode = ETHSWT_REG_GET_BIT(regVal, ETHSWT_REG_PORT_ASSOC_VEC_LOCKEDPORT_OFST);

    if (learningMode & 0x01 == 0x01) /*hardware disabled*/
    {
        *MacLearningMode = ETHSWT_MACLEARNING_HWDISABLED;
    }
    else
    {
        *MacLearningMode = ETHSWT_MACLEARNING_HWENABLED;
    }

    retVal = E_OK;

    return retVal;
}
#endif /*STD_ON == ETHSWT_GET_MAC_LEARNING_MODE_API*/

#if (STD_ON == ETHSWT_GET_SWITCH_PORT_MODE_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_GetSwitchPortModeInternal(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_ModeType* SwitchModePtr)
{
    Std_ReturnType retVal = E_NOT_OK;
    EthSwt_RegType regVal;
    EthSwt_RegType regTemp;

    EthSwt_ReadRegister(SwitchIdx, SwitchPortIdx, ETHSWT_REG_PORT_CTRL, &regVal);
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_PORT_CTRL_PORTSTATE_MASK);

    *SwitchModePtr = (regTemp) ? ETH_MODE_ACTIVE : ETH_MODE_DOWN;

    retVal = E_OK;

    return retVal;
}
#endif /* STD_ON == ETHSWT_GET_SWITCH_PORT_MODE_API */

FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_ATULoadOrPurgeEntry(uint8 SwitchIdx, const EthSwt_AtuOpInfoType* atuInfo, EthSwt_ATU_OperationType op)
{
    EthSwt_RegType regVal;
    Std_ReturnType retVal = E_OK;

    /*1. set ATU FID*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_FID, &regVal);
    /*1.1 set fid*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_FID_FID_MASK);

    ETHSWT_REG_WRT_FIELD(regVal, (uint16)atuInfo->fid, ETHSWT_REG_GLB1_ATU_FID_FID_OFST);

    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_FID, regVal);
    /*2. set ATU Data*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_DATA, &regVal);
    /*2.1 entry state*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_DATA_ENTRYSTATE_MASK);

    ETHSWT_REG_WRT_FIELD(regVal, (uint16)atuInfo->entryState, ETHSWT_REG_GLB1_ATU_DATA_ENTRYSTATE_OFST);

    /*2.2 portVec*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_DATA_PORTVEC_MASK);
    ETHSWT_REG_WRT_FIELD(regVal, atuInfo->macAddress.portVec, ETHSWT_REG_GLB1_ATU_DATA_PORTVEC_OFST);
    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_DATA, regVal);
    /*3. set ATU MAC*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES0_1, &regVal);
    /*3.1 byte 0, 1*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_MASK);
    ETHSWT_REG_WRT_FIELD(regVal, (uint16)atuInfo->macAddress.macAddr[0], ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_OFST);
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_MASK);

    ETHSWT_REG_WRT_FIELD(regVal, (uint16)atuInfo->macAddress.macAddr[1], ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_OFST);
    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES0_1, regVal);
    /*3.3 byte 2, 3*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES2_3, &regVal);
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_MASK);

    ETHSWT_REG_WRT_FIELD(regVal, (uint16)atuInfo->macAddress.macAddr[2], ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_OFST);

    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_MASK);

    ETHSWT_REG_WRT_FIELD(regVal, (uint16)atuInfo->macAddress.macAddr[3], ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_OFST);

    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES2_3, regVal);
    /*3.5 byte 4, 5*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES4_5, &regVal);
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_MASK);
    ETHSWT_REG_WRT_FIELD(regVal, (uint16)atuInfo->macAddress.macAddr[4], ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_OFST);
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_MASK);

    ETHSWT_REG_WRT_FIELD(regVal, (uint16)atuInfo->macAddress.macAddr[5], ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_OFST);

    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES4_5, regVal);
    /*4. set operation*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_OP, &regVal);
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_OP_OP_MASK);
    ETHSWT_REG_WRT_FIELD(regVal, (uint16)op, ETHSWT_REG_GLB1_ATU_OP_OP_OFST);
    /*==========================start operation============================*/
    ETHSWT_REG_SET_BIT(regVal, ETHSWT_REG_GLB1_ATU_OP_BUSY_OFST);
    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_OP, regVal);

    return retVal;
}

FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_ATUGetEntry(uint8 SwitchIdx, EthSwt_AtuOpInfoType* atuInfo, EthSwt_ATU_OperationType op)
{
    EthSwt_RegType regVal;
    EthSwt_RegType regTemp;
    Std_ReturnType retVal = E_OK;

    /*1. set ATU FID*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_FID, &regVal);
    /*1.1 set fid*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_FID_FID_MASK);

    ETHSWT_REG_WRT_FIELD(regVal, (uint16)atuInfo->fid, ETHSWT_REG_GLB1_ATU_FID_FID_OFST);

    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_FID, regVal);
    /*2. set ATU MAC*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES0_1, &regVal);
    /*2.1 byte 0, 1*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_MASK);
    ETHSWT_REG_WRT_FIELD(regVal, (uint16)atuInfo->macAddress.macAddr[0], ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_OFST);
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_MASK);

    ETHSWT_REG_WRT_FIELD(regVal, (uint16)atuInfo->macAddress.macAddr[1], ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_OFST);

    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES0_1, regVal);
    /*2.2 byte 2, 3*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES2_3, &regVal);
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_MASK);
    ETHSWT_REG_WRT_FIELD(regVal, (uint16)atuInfo->macAddress.macAddr[2], ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_OFST);
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_MASK);

    ETHSWT_REG_WRT_FIELD(regVal, (uint16)atuInfo->macAddress.macAddr[3], ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_OFST);

    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES2_3, regVal);
    /*2.3 byte 4, 5*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES4_5, &regVal);
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_MASK);
    ETHSWT_REG_WRT_FIELD(regVal, (uint16)atuInfo->macAddress.macAddr[4], ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_OFST);
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_MASK);

    ETHSWT_REG_WRT_FIELD(regVal, (uint16)atuInfo->macAddress.macAddr[5], ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_OFST);

    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES4_5, regVal);
    /*3. set ATU Operation*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_OP, &regVal);
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_OP_OP_MASK);
    ETHSWT_REG_WRT_FIELD(regVal, (uint16)op, ETHSWT_REG_GLB1_ATU_OP_OP_OFST);
    /*==========================start operation============================*/
    ETHSWT_REG_SET_BIT(regVal, ETHSWT_REG_GLB1_ATU_OP_BUSY_OFST);

    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_OP, regVal);
    do
    {
        /*wait the write procedure done*/
        EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_OP, &regVal);
        regTemp = ETHSWT_REG_GET_BIT(regVal, ETHSWT_REG_GLB1_ATU_OP_BUSY_OFST);
    } while (regTemp != 0u);

    /*4. read portVec*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_DATA, &regVal);
    /*4.1 get portVec*/
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_ATU_DATA_PORTVEC_MASK);
    atuInfo->macAddress.portVec = ETHSWT_REG_SHIFT_FIELD(regTemp, ETHSWT_REG_GLB1_ATU_DATA_PORTVEC_OFST);
    /*4.2 get entryState*/
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_ATU_DATA_ENTRYSTATE_MASK);

    atuInfo->entryState = (uint8)(ETHSWT_REG_SHIFT_FIELD(regTemp, ETHSWT_REG_GLB1_ATU_DATA_ENTRYSTATE_OFST));

    /*5. get atu mac*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES0_1, &regVal);
    /*5.1 byte 0, 1*/
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_MASK);
    atuInfo->macAddress.macAddr[0] =
        (uint8)(ETHSWT_REG_SHIFT_FIELD(regTemp, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_OFST));

    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_MASK);

    atuInfo->macAddress.macAddr[1] =
        (uint8)(ETHSWT_REG_SHIFT_FIELD(regTemp, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_OFST));

    /*5.2 byte 2, 3*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES2_3, &regVal);
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_MASK);
    atuInfo->macAddress.macAddr[2] =
        (uint8)(ETHSWT_REG_SHIFT_FIELD(regTemp, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_OFST));
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_MASK);

    atuInfo->macAddress.macAddr[3] =
        (uint8)(ETHSWT_REG_SHIFT_FIELD(regTemp, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_OFST));
    /*5.3 byte 4, 5*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES4_5, &regVal);
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_MASK);
    atuInfo->macAddress.macAddr[4] =
        (uint8)(ETHSWT_REG_SHIFT_FIELD(regTemp, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_OFST));
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_MASK);

    atuInfo->macAddress.macAddr[5] =
        (uint8)(ETHSWT_REG_SHIFT_FIELD(regTemp, ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_OFST));
    return retVal;
}

FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_VTULoadOrPurgeEntry(uint8 SwitchIdx, const EthSwt_VtuOpInfoType* vtuInfo)
{
    EthSwt_RegType regVal;
    uint32 memberTag = vtuInfo->memberTag;
    Std_ReturnType retVal = E_OK;

    /*1. set VTU FID*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTU_FID, &regVal);
    /*1.1 set fid*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTU_FID_FID_MASK);

    ETHSWT_REG_WRT_FIELD(regVal, (uint16)vtuInfo->fid, ETHSWT_REG_GLB1_VTU_FID_FID_OFST);

    /*1.2 set vidPolicy(mirror)*/
    /*
     * ETHSWT_REG_SET_BIT(regVal, ETHSWT_REG_GLB1_VTU_FID_VIDPOLICY_OFST);
     */
    /*
     *EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTU_FID, regVal);
     */

    /*2. set VTU SID*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTU_SID, &regVal);
    /*2.1 set vtu sid*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTU_SID_SID_MASK);

    ETHSWT_REG_WRT_FIELD(regVal, (uint16)vtuInfo->sid, ETHSWT_REG_GLB1_VTU_SID_SID_OFST);

    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTU_SID, regVal);

    /*3. set VTU VID*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTU_VID, &regVal);
    /*3.1 set valid*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTU_VID_VALID_MASK);
    ETHSWT_REG_WRT_FIELD(regVal, (uint16)(vtuInfo->valid ? 1u : 0u), ETHSWT_REG_GLB1_VTU_VID_VALID_OFST);
    /*3.2 set vtu vid*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTU_VID_VID_MASK);

    ETHSWT_REG_WRT_FIELD(regVal, vtuInfo->vlanID, ETHSWT_REG_GLB1_VTU_VID_VID_OFST);

    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTU_VID, regVal);

    /*4. set VTU Data(3 registers)*/
    /*4.1 VTU Data p0-3*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU, &regVal);
    /*4.1.1 member tag*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP0_MASK);

    ETHSWT_REG_WRT_FIELD(
        regVal,
        (uint16)(memberTag & (uint32)ETHSWT_REG_GLB1_VTUSTU_DATA_PORT_MASK),
        ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP0_OFST);

    memberTag = memberTag >> 2u; /*get next port's info*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP1_MASK);

    ETHSWT_REG_WRT_FIELD(
        regVal,
        (uint16)(memberTag & (uint32)ETHSWT_REG_GLB1_VTUSTU_DATA_PORT_MASK),
        ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP1_OFST);

    memberTag = memberTag >> 2u; /*get next port's info*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP2_MASK);

    ETHSWT_REG_WRT_FIELD(
        regVal,
        (uint16)(memberTag & (uint32)ETHSWT_REG_GLB1_VTUSTU_DATA_PORT_MASK),
        ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP2_OFST);

    memberTag = memberTag >> 2u; /*get next port's info*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP3_MASK);

    ETHSWT_REG_WRT_FIELD(
        regVal,
        (uint16)(memberTag & (uint32)ETHSWT_REG_GLB1_VTUSTU_DATA_PORT_MASK),
        ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP3_OFST);

    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU, regVal);
    /*4.2 VTU Data p4-7*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU, &regVal);
    /*4.2.1 member tag*/
    memberTag = memberTag >> 2u; /*get next port's info*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP4_MASK);

    ETHSWT_REG_WRT_FIELD(
        regVal,
        (uint16)(memberTag & (uint32)ETHSWT_REG_GLB1_VTUSTU_DATA_PORT_MASK),
        ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP4_OFST);

    memberTag = memberTag >> 2u; /*get next port's info*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP5_MASK);

    ETHSWT_REG_WRT_FIELD(
        regVal,
        (uint16)(memberTag & (uint32)ETHSWT_REG_GLB1_VTUSTU_DATA_PORT_MASK),
        ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP5_OFST);

    memberTag = memberTag >> 2u; /*get next port's info*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP6_MASK);

    ETHSWT_REG_WRT_FIELD(
        regVal,
        (uint16)(memberTag & (uint32)ETHSWT_REG_GLB1_VTUSTU_DATA_PORT_MASK),
        ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP6_OFST);

    memberTag = memberTag >> 2u; /*get next port's info*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP7_MASK);

    ETHSWT_REG_WRT_FIELD(
        regVal,
        (uint16)(memberTag & (uint32)ETHSWT_REG_GLB1_VTUSTU_DATA_PORT_MASK),
        ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP7_OFST);

    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU, regVal);
    /*4.3 VTU Data p8*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU, &regVal);
    /*4.3.1 member tag*/
    memberTag = memberTag >> 2u; /*get next port's info*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU_MEMBERTAGP8_MASK);

    ETHSWT_REG_WRT_FIELD(
        regVal,
        (uint16)(memberTag & (uint32)ETHSWT_REG_GLB1_VTUSTU_DATA_PORT_MASK),
        ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU_MEMBERTAGP8_OFST);

    /*4.3.2 vid priority override*/
    if (vtuInfo->priOverride)
    {
        ETHSWT_REG_SET_BIT(regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU_PRIOVERRIDE_OFST);
    }
    else
    {
        ETHSWT_REG_CLR_BIT(regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU_PRIOVERRIDE_OFST);
    }

    /*4.3.3 vid priority*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU_VIDPRI_MASK);
    ETHSWT_REG_WRT_FIELD(regVal, (uint16)vtuInfo->vidPri, ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU_VIDPRI_OFST);
    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU, regVal);
    /*5. set VTU Operation, start opertion*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTU_OP, &regVal);
    /*5.1 set vtu OP*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTU_OP_VTUOP_MASK);
    ETHSWT_REG_WRT_FIELD(regVal, (uint16)vtuInfo->op, ETHSWT_REG_GLB1_VTU_OP_VTUOP_OFST);
    /*==========================start operation============================*/

    ETHSWT_REG_SET_BIT(regVal, ETHSWT_REG_GLB1_VTU_OP_VTUBUSY_OFST);

    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTU_OP, regVal);

    return retVal;
}

FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_VTUGetEntry(uint8 SwitchIdx, EthSwt_VtuOpInfoType* vtuInfo)
{
    EthSwt_RegType regVal;
    EthSwt_RegType regTemp;
    Std_ReturnType retVal = E_OK;

    /*1. set VTU VID*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTU_VID, &regVal);
    /*1.1 set vtu vid*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTU_VID_VID_MASK);

    ETHSWT_REG_WRT_FIELD(regVal, vtuInfo->vlanID, ETHSWT_REG_GLB1_VTU_VID_VID_OFST);

    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTU_VID, regVal);

    /*2. set VTU Operation, start opertion*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTU_OP, &regVal);
    /*2.1 set vtu OP*/
    ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_VTU_OP_VTUOP_MASK);

    ETHSWT_REG_WRT_FIELD(regVal, (uint16)vtuInfo->op, ETHSWT_REG_GLB1_VTU_OP_VTUOP_OFST);

    /*==========================start operation============================*/

    ETHSWT_REG_SET_BIT(regVal, ETHSWT_REG_GLB1_VTU_OP_VTUBUSY_OFST);

    EthSwt_WriteRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTU_OP, regVal);
    do
    {
        /*wait the write procedure done*/
        EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTU_OP, &regVal);
        regTemp = ETHSWT_REG_GET_BIT(regVal, ETHSWT_REG_GLB1_VTU_OP_VTUBUSY_OFST);
    } while (regTemp != 0u);

    /*read vtu vid*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTU_VID, &regVal);
    /*get next vid*/
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_VTU_VID_VID_MASK);

    vtuInfo->vlanID = ETHSWT_REG_SHIFT_FIELD(regTemp, ETHSWT_REG_GLB1_VTU_VID_VID_OFST);

    /*get valid*/
    regTemp = ETHSWT_REG_GET_BIT(regVal, ETHSWT_REG_GLB1_VTU_VID_VALID_OFST);
    vtuInfo->valid = (regTemp != 0u);

    /*read vtu data0 P0-3*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU, &regVal);
    /*get membertag P0-3*/
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP0_MASK);

    vtuInfo->memberTag =
        (ETHSWT_REG_SHIFT_FIELD((uint32)regTemp, ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP0_OFST));

    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP1_MASK);
    vtuInfo->memberTag |=
        (ETHSWT_REG_SHIFT_FIELD((uint32)regTemp, ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP1_OFST)) << 2u;
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP2_MASK);
    vtuInfo->memberTag |=
        (ETHSWT_REG_SHIFT_FIELD((uint32)regTemp, ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP2_OFST)) << 4u;
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP3_MASK);
    vtuInfo->memberTag |=
        (ETHSWT_REG_SHIFT_FIELD((uint32)regTemp, ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP3_OFST)) << 6u;

    /*read vtu data1 P4-7*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU, &regVal);
    /*get membertag P4-7*/
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP4_MASK);

    vtuInfo->memberTag |=
        (ETHSWT_REG_SHIFT_FIELD((uint32)regTemp, ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP4_OFST)) << 8u;

    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP5_MASK);
    vtuInfo->memberTag |=
        (ETHSWT_REG_SHIFT_FIELD((uint32)regTemp, ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP5_OFST)) << 10u;
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP6_MASK);
    vtuInfo->memberTag |=
        (ETHSWT_REG_SHIFT_FIELD((uint32)regTemp, ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP6_OFST)) << 12u;
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP7_MASK);
    vtuInfo->memberTag |=
        (ETHSWT_REG_SHIFT_FIELD((uint32)regTemp, ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP7_OFST)) << 14u;

    /*read vtu data2 P8*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU, &regVal);
    /*get membertag P8*/
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU_MEMBERTAGP8_MASK);

    vtuInfo->memberTag |= (ETHSWT_REG_SHIFT_FIELD((uint32)regTemp, ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU_MEMBERTAGP8_OFST))
                          << 16u;

    /*get VIDPriOverride*/
    regTemp = ETHSWT_REG_GET_BIT(regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU_PRIOVERRIDE_OFST);
    vtuInfo->priOverride = (regTemp != 0u);

    /*get VIDPri*/
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU_VIDPRI_MASK);
    vtuInfo->vidPri = (uint8)(ETHSWT_REG_SHIFT_FIELD(regTemp, ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU_VIDPRI_OFST));
    /*read vtu fid*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTU_FID, &regVal);
    /*get fid*/
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_VTU_FID_FID_MASK);

    vtuInfo->fid = (uint8)(ETHSWT_REG_SHIFT_FIELD(regTemp, ETHSWT_REG_GLB1_VTU_FID_FID_OFST));

    /*get vidPolicy*/
    /*
     * vtuInfo->vidPolicy = ETHSWT_REG_GET_BIT(regVal, ETHSWT_REG_GLB1_VTU_FID_VIDPOLICY_OFST);
     */

    /*read vtu sid*/
    EthSwt_ReadRegister(SwitchIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_VTU_SID, &regVal);
    /*get fid*/
    ETHSWT_REG_READ_FIELD(regTemp, regVal, ETHSWT_REG_GLB1_VTU_SID_SID_MASK);

    vtuInfo->sid = (uint8)(ETHSWT_REG_SHIFT_FIELD(regTemp, ETHSWT_REG_GLB1_VTU_SID_SID_OFST));

    return retVal;
}

#if (STD_ON == ETHSWT_ENABLE_VLAN_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_EnableVlanInternal(uint8 SwitchIdx, uint8 SwitchPortIdx, uint16 VlanId, boolean Enable)
{
#if 0
    Std_ReturnType retVal = E_NOT_OK;
    EthSwt_VtuOpInfoType vtuInfo;
    uint16  index;
    uint8 portOfst = SwitchPortIdx << 1u;
    const EthSwt_PortConifgType* portCfgPtr = EthSwt_Config[SwitchIdx].PortCfgPtr;

    vtuInfo.vlanID = (VlanId - 1) & ETHSWT_INVALIDE_VID;
    vtuInfo.op = ETHSWT_VTU_OP_VTU_GET_NEXT;
    EthSwt_VTUGetEntry(SwitchIdx, &vtuInfo);

    for (index = 0u; index < EthSwt_Config[SwitchIdx].SwtVlanEntryNbr; i++)
    {
        /* code */
    }

    if ((vtuInfo.memberTag >> portOfst) & ETHSWT_NOT_SENT != ETHSWT_NOT_SENT)
    {
        if ((vtuInfo.valid == 1) &&
            ((vtuInfo.memberTag >> portOfst) & 0x3 != 0x3) &&
            (Enable == FALSE))
        {
            vtuInfo.memberTag = vtuInfo.memberTag & (~(0x3 << portOfst));

            vtuInfo.fid = 0u;
            vtuInfo.sid = 0u;
            vtuInfo.op  = ETHSWT_VTU_OP_VTU_LOAD_OR_PURGE;
            vtuInfo.valid = 0u;/*pure*/
            vtuInfo.vlanID      = SwtCfgPtr->VlanEntries[index].vlanID;
            vtuInfo.memberTag   = SwtCfgPtr->VlanEntries[index].memberTag;
            vtuInfo.vidPri      = SwtCfgPtr->VlanEntries[index].vlanDefPrio;
            EthSwt_VTULoadOrPurgeEntry(SwtCfgPtr->SwtIdx, &vtuInfo);

        }
        else if ((vtuInfo.valid == 1) &&
                ((vtuInfo.memberTag >> portOfst) & 0x3 == 0x3) &&
                (Enable == FALSE))
        {
            vtuInfo.memberTag = vtuInfo.memberTag | (0x1 << portOfst);
        }
        else
        {
            /*do nothing*/
        }

        vtuInfo.fid = 0u;
        vtuInfo.sid = 0u;
        vtuInfo.op  = ETHSWT_VTU_OP_VTU_LOAD_OR_PURGE;
        vtuInfo.valid = 1u;/*re load*/
        vtuInfo.vlanID      = SwtCfgPtr->VlanEntries[index].vlanID;
        vtuInfo.memberTag   = SwtCfgPtr->VlanEntries[index].memberTag;
        vtuInfo.vidPri      = SwtCfgPtr->VlanEntries[index].vlanDefPrio;
        EthSwt_VTULoadOrPurgeEntry(SwtCfgPtr->SwtIdx, &vtuInfo);

    }
    else
    {
        /*do nothing*/
    }
#endif
    return E_OK;
}
#endif /*STD_ON == ETHSWT_ENABLE_VLAN_API*/
#if (STD_ON == ETHSWT_PORTBASED_VLAN_SUPPORT)
FUNC(void, ETHSWT_CODE) EthSwt_PortAllResetVLANTable(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr)
{
    EthSwt_PortConifgType portCfgPtr;
    EthSwt_RegType regVal;
    uint16 vlanMap;
    uint8 index;

    for (index = 0u; index < SwtCfgPtr->SwtPortNbr; index++)
    {
        portCfgPtr = SwtCfgPtr->PortCfgPtr[index];
        vlanMap = (~(0x1 << portCfgPtr.PortIdx)) & 0x01FF;

        EthSwt_ReadRegister(SwtCfgPtr->SwtIdx, portCfgPtr.PortIdx, ETHSWT_REG_PORT_BASED_VLAN_MAP, &regVal);
        ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_PORT_BASED_VLAN_MAP_VLANTABLE_MASK);
        ETHSWT_REG_WRT_FIELD(regVal, vlanMap, ETHSWT_REG_PORT_BASED_VLAN_MAP_VLANTABLE_OFST);
        EthSwt_WriteRegister(SwtCfgPtr->SwtIdx, portCfgPtr.PortIdx, ETHSWT_REG_PORT_BASED_VLAN_MAP, regVal);
    }
}
#endif /*STD_ON == ETHSWT_PORTBASED_VLAN_SUPPORT*/
FUNC(void, ETHSWT_CODE)
EthSwt_PortIngressInit(uint8 swtIdx, const EthSwt_PortConifgType* portCfgPtr)
{
    uint8 DefPrior;
    uint16 DefVID;
    boolean dropUntagged;
    boolean forceVID = FALSE;
    /*forceVID is the same with modifVID in 88q5050*/
    EthSwt_RegType regVal;

    DefPrior = portCfgPtr->PortIngress->DefPrior;
    DefVID = portCfgPtr->PortIngress->DefVlan;
    dropUntagged = portCfgPtr->PortIngress->DropUntagged;

    if (ETHSWT_INVALIDE_VID != portCfgPtr->PortIngress->VlanModification)
    {
        forceVID = TRUE;
    }

    EthSwt_ReadRegister(swtIdx, portCfgPtr->PortIdx, ETHSWT_REG_DEF_PORT_VID_PRI, &regVal);
    /*if dropUntagged = TRUE, prio and vid are not need*/
    if (TRUE == dropUntagged)
    {
        ETHSWT_REG_SET_BIT(regVal, ETHSWT_REG_DEF_PORT_VID_PRI_DROPUNTAGGED_OFST);
    }
    else
    {
        /*set default priority*/
        if (ETHSWT_INVALID_PRIO != DefPrior)
        {
            ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_DEF_PORT_VID_PRI_DEFPRI_MASK);
            ETHSWT_REG_WRT_FIELD(regVal, (uint16)DefPrior, ETHSWT_REG_DEF_PORT_VID_PRI_DEFPRI_OFST);
        }

        /*set default VID*/
        if (ETHSWT_INVALIDE_VID != DefVID)
        {
            ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_DEF_PORT_VID_PRI_DEFVID_MASK);

            ETHSWT_REG_WRT_FIELD(regVal, (uint16)DefVID, ETHSWT_REG_DEF_PORT_VID_PRI_DEFVID_OFST);

            if (forceVID)
            {
                ETHSWT_REG_SET_BIT(regVal, ETHSWT_REG_DEF_PORT_VID_PRI_FORCEDEFVID_OFST);
            }
            else
            {
                ETHSWT_REG_CLR_BIT(regVal, ETHSWT_REG_DEF_PORT_VID_PRI_FORCEDEFVID_OFST);
            }
        }
    }
    /*set forceVID, assigned to untagged or replace tagged*/
    EthSwt_WriteRegister(swtIdx, portCfgPtr->PortIdx, ETHSWT_REG_DEF_PORT_VID_PRI, regVal);
}

FUNC(void, ETHSWT_CODE)
EthSwt_PortInit(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr)
{
    EthSwt_RegType regVal;
    uint8 index;
    uint16 vlanMap;
    const EthSwt_PortConifgType* portCfgPtr;

    for (index = 0u; index < SwtCfgPtr->SwtPortNbr; index++)
    {
        portCfgPtr = &SwtCfgPtr->PortCfgPtr[index];
        /*access ETHSWT_REG_DEF_PORT_VID_PRI*/
        EthSwt_PortIngressInit(SwtCfgPtr->SwtIdx, portCfgPtr);
        /*init port-based VLAN*/
#if (STD_ON == ETHSWT_PORTBASED_VLAN_SUPPORT)
        EthSwt_PortAllResetVLANTable(SwtCfgPtr);
#endif /*STD_ON == ETHSWT_PORTBASED_VLAN_SUPPORT*/
#if (ETHSWT_802_1Q_MODE != ETHSWT_802_1Q_MODE_DISABLE)
        /*init 802.1Qmode VLAN*/
        EthSwt_ReadRegister(SwtCfgPtr->SwtIdx, portCfgPtr->PortIdx, ETHSWT_REG_PORT_CTRL2, &regVal);
        ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_PORT_CTRL2_8021QMODE_MASK);

        EthSwt_RegType tempVal = (uint16)ETHSWT_802_1Q_MODE;
        ETHSWT_REG_WRT_FIELD(regVal, tempVal, ETHSWT_REG_PORT_CTRL2_8021QMODE_OFST);

        EthSwt_WriteRegister(SwtCfgPtr->SwtIdx, portCfgPtr->PortIdx, ETHSWT_REG_PORT_CTRL2, regVal);
#endif /*ETHSWT_802_1Q_MODE != ETHSWT_802_1Q_MODE_DISABLE*/
        /*egress mode:  Default to Unmodified mode – frames are transmitted*/

        /*1000 or 2000M, force duplex, add delay to rx/tx clock*/
        EthSwt_ReadRegister(SwtCfgPtr->SwtIdx, portCfgPtr->PortIdx, ETHSWT_REG_PHY_CTRL, &regVal);
        ETHSWT_REG_CLR_FIELD(regVal, 0xffffu);
        ETHSWT_REG_WRT_FIELD(regVal, 0xC03Eu, 0);
        EthSwt_WriteRegister(SwtCfgPtr->SwtIdx, portCfgPtr->PortIdx, ETHSWT_REG_PHY_CTRL, regVal);

#if (STD_ON == ETHSWT_SET_SWITCH_PORT_MODE_API)
        /*switch on the configured ports to active*/
        (void)EthSwt_SetSwitchPortModeInternal(SwtCfgPtr->SwtIdx, portCfgPtr->PortIdx, ETH_MODE_ACTIVE);
#endif /* STD_ON == ETHSWT_SET_SWITCH_PORT_MODE_API */
    }
}

FUNC(void, ETHSWT_CODE) EthSwt_VTUVlanInit(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr)
{
    EthSwt_VtuOpInfoType vtuInfo;
    uint16 index;

    for (index = 0u; index < SwtCfgPtr->SwtVlanEntryNbr; index++)
    {
        vtuInfo.fid = 0u;
        vtuInfo.sid = 0u;
        vtuInfo.op = ETHSWT_VTU_OP_VTU_LOAD_OR_PURGE;
        vtuInfo.valid = TRUE; /*load operation*/
        vtuInfo.vlanID = SwtCfgPtr->VlanEntries[index].vlanID;
        vtuInfo.memberTag = SwtCfgPtr->VlanEntries[index].memberTag;
        vtuInfo.vidPri = SwtCfgPtr->VlanEntries[index].vlanDefPrio;
        (void)EthSwt_VTULoadOrPurgeEntry(SwtCfgPtr->SwtIdx, &vtuInfo);
    }
}

FUNC(void, ETHSWT_CODE) EthSwt_VTUReadDatabase(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr)
{
    EthSwt_VtuOpInfoType vtuInfo;
    uint16 fisrtVid = ETHSWT_INVALIDE_VID; /*use 0x0FFF to find the lowest VID*/

    vtuInfo.vlanID = fisrtVid;
    vtuInfo.op = ETHSWT_VTU_OP_VTU_GET_NEXT;

    do
    {
        (void)EthSwt_VTUGetEntry(SwtCfgPtr->SwtIdx, &vtuInfo);
    } while (vtuInfo.vlanID != ETHSWT_INVALIDE_VID);
}

FUNC(void, ETHSWT_CODE)
EthSwt_ATUStaticMacInit(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr)
{
    EthSwt_AtuOpInfoType atuInfo;
    uint16 index;

    /*init predefine mac*/
    for (index = 0u; index < SwtCfgPtr->StaticMacNbr; index++)
    {
        atuInfo.fid = 0u;
        atuInfo.entryState = 0xE; /*static entry*/
        (void)ILib_memcpy(&atuInfo.macAddress.macAddr[0], SwtCfgPtr->PredefMacAddr[index].macAddr, 6u);
        atuInfo.macAddress.portVec = SwtCfgPtr->PredefMacAddr[index].portVec;

        (void)EthSwt_ATULoadOrPurgeEntry(SwtCfgPtr->SwtIdx, &atuInfo, ETHSWT_ATU_OP_LOAD_OR_PURGE);
    }
}

FUNC(void, ETHSWT_CODE) EthSwt_ATUReadDatabase(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr)
{
    EthSwt_AtuOpInfoType atuInfo;
    uint8 learnedAddrIdx = 0u;
    /* Use an address of all ones to find the first or lowest MAC address.
    Use the last address to find the next address*/
    atuInfo.macAddress.macAddr[0] = 0xff;
    atuInfo.macAddress.macAddr[1] = 0xff;
    atuInfo.macAddress.macAddr[2] = 0xff;
    atuInfo.macAddress.macAddr[3] = 0xff;
    atuInfo.macAddress.macAddr[4] = 0xff;
    atuInfo.macAddress.macAddr[5] = 0xff;
    atuInfo.fid = 0u;

    do
    {
        (void)EthSwt_ATUGetEntry(SwtCfgPtr->SwtIdx, &atuInfo, ETHSWT_ATU_OP_GET_NEXT);
        /*Save learned entry, where EntryState = the Age of the entry*/
#if (STD_ON == ETHSWT_ARL_NVM_SUPPORT)
        if ((0x00 < atuInfo.entryState) && (0x08 > atuInfo.entryState))
        {
            EthSwt_ArlTabShadowBuf[SwtCfgPtr->SwtIdx].atuInfo[learnedAddrIdx++] = atuInfo;
            if (learnedAddrIdx >= ETHSWT_ATU_SIZE)
            {
                break; /*avoid overflow*/
            }
        }
#endif /*STD_ON == ETHSWT_ARL_NVM_SUPPORT*/
    } while (atuInfo.entryState != 0x00u);
}

FUNC(void, ETHSWT_CODE)
EthSwt_GLBInit(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr)
{
    EthSwt_RegType regVal;
    EthSwt_RegType _88Q5050_EntryTimeout;

    /*set age time, 3.75s(0x1)~956s(0xff),0:disable, remain forever*/
    _88Q5050_EntryTimeout = SwtCfgPtr->ArlTableEntryTimeout;
    if (0x00u != _88Q5050_EntryTimeout)
    {
        EthSwt_ReadRegister(SwtCfgPtr->SwtIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_CTRL, &regVal);
        ETHSWT_REG_CLR_FIELD(regVal, ETHSWT_REG_GLB1_ATU_CTRL_AGETIME_MASK);
        ETHSWT_REG_WRT_FIELD(regVal, _88Q5050_EntryTimeout, ETHSWT_REG_GLB1_ATU_CTRL_AGETIME_OFST);
        EthSwt_WriteRegister(SwtCfgPtr->SwtIdx, ETHSWT_REG_GROUP_GLB1, ETHSWT_REG_GLB1_ATU_CTRL, regVal);
    }
}

FUNC(void, ETHSWT_CODE)
EthSwt_SwitchOffAllPorts(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr)
{
    uint8 index;

    /*SWS_EthSwt_00374, SWS_EthSwt_00375*/
    for (index = 0u; index < ETHSWT_PORT_MAX; index++)
    {
#if (STD_ON == ETHSWT_SET_SWITCH_PORT_MODE_API)
        EthSwt_SetSwitchPortModeInternal(SwtCfgPtr->SwtIdx, index, ETH_MODE_DOWN);
#endif /* STD_ON == ETHSWT_SET_SWITCH_PORT_MODE_API */
    }
#if (STD_OFF == ETHSWT_SET_SWITCH_PORT_MODE_API)
    (void)(SwtCfgPtr);
#endif /* STD_OFF == ETHSWT_SET_SWITCH_PORT_MODE_API */
}
#if (STD_ON == ETHSWT_ARL_NVM_SUPPORT || STD_ON == ETHSWT_MIRROR_NVM_SUPPORT)
FUNC(void, ETHSWT_CODE) EthSwt_NvMBlkInit(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr)
{
#if (STD_ON == ETHSWT_ARL_NVM_SUPPORT)
    EthSwt_ArlTabShadowBuf[SwtCfgPtr->SwtIdx].blkId = SwtCfgPtr->ArlTableNvmBlkId;
#endif /*STD_ON == ETHSWT_ARL_NVM_SUPPORT*/

#if (STD_ON == ETHSWT_MIRROR_NVM_SUPPORT)
    EthSwt_MirrorShadowBuf[SwtCfgPtr->SwtIdx].blkId = SwtCfgPtr->CfgNvmBlkId;
    EthSwt_MirrorShadowBuf[SwtCfgPtr->SwtIdx].MirrorStore.MirrorInfo = &EthSwt_MirrorCfgBuf[SwtCfgPtr->SwtIdx];
#endif /*STD_ON == ETHSWT_MIRROR_NVM_SUPPORT*/
}
#endif /*STD_ON == ETHSWT_ARL_NVM_SUPPORT || STD_ON == ETHSWT_MIRROR_NVM_SUPPORT*/

FUNC(uint8, ETHSWT_CODE)
EthSwt_GetCfgPortIdx(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) SwtCfgPtr, uint8 SwitchPortIdx)
{
    uint8 index;
    uint8 retPortIdx = 0u;

    for (index = 0u; index < ETHSWT_PORT_MAX; index++)
    {
        if (SwitchPortIdx == SwtCfgPtr->PortCfgPtr[index].PortIdx)
        {
            retPortIdx = index;
            break;
        }
    }

    return retPortIdx;
}
#define ETHSWT_STOP_SEC_CODE
#include "EthSwt_MemMap.h"
