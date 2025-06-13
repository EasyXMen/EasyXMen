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
************************************************************************************************************************
**                                                                                                                    **
**  FILENAME    : EthSwt_88Q5050.h                                                                                    **
**                                                                                                                    **
**  Created on  : 2021-10-13                                                                                          **
**  Author      : zhiqiang.huang                                                                                      **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : none                                                                                                **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/
#ifndef ETHSWT_88Q5050_H_
#define ETHSWT_88Q5050_H_
/***********************************************************************************************************************
**                                          REVISION   HISTORY                                                        **
***********************************************************************************************************************/
/* <VERSION>    <DATE>       <AUTHOR>        <REVISION LOG>
 *  V1.0.0    [20211013]  [zhiqiang.huang]   Initial version
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 ***********************************************************************************************************************/
#include "ComStack_Types.h"
/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 ***********************************************************************************************************************/
typedef enum
{
    ETHSWT_VTU_OP_NO = 0x0,
    ETHSWT_VTU_OP_FLUSH_ALL,
    ETHSWT_VTU_OP_FLUSH_RSV,
    ETHSWT_VTU_OP_VTU_LOAD_OR_PURGE,
    ETHSWT_VTU_OP_VTU_GET_NEXT,
    ETHSWT_VTU_OP_STU_LOAD_OR_PURGE,
    ETHSWT_VTU_OP_STU_GET_NEXT,
    ETHSWT_VTU_OP_STU_GET_OR_CLR_VIOL
} EthSwt_VTU_OperationType;

typedef struct
{
    EthSwt_VTU_OperationType op;
    /*VTU entry below*/
    boolean priOverride;
    /*
     * boolean     vidPolicy;
     */
    uint8 vidPri;
    uint8 sid;
    boolean valid;
    uint8 fid;
    uint16 vlanID;
    uint32 memberTag; /*bit0,1 for port0, bit2,3 for port1, and so on*/
} EthSwt_VtuOpInfoType;

typedef enum
{
    ETHSWT_ATU_OP_NO = 0x0,
    ETHSWT_ATU_OP_FLUSH_ALL,
    ETHSWT_ATU_OP_FLUSH_NONSTATIC,
    ETHSWT_ATU_OP_LOAD_OR_PURGE,
    ETHSWT_ATU_OP_GET_NEXT,
    ETHSWT_ATU_OP_FLASH_ALL_BY_FID,
    ETHSWT_ATU_OP_FLASH_ALL_NONSTATIC_VY_FID,
    ETHSWT_ATU_OP_STU_GET_OR_CLR_VIOL
} EthSwt_ATU_OperationType;

typedef struct
{
    uint8 macAddr[6];
    uint16 portVec;
} EthSwt_AtuMacAddressType;

typedef struct
{
    /*ATU entry below*/
    uint8 macPri;
    boolean lag;
    uint8 entryState;
    uint8 fid;
    EthSwt_AtuMacAddressType macAddress;
} EthSwt_AtuOpInfoType;

/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 ***********************************************************************************************************************/
#define ETHSWT_PORT_MAX             9u
#define ETHSWT_ATU_SIZE             10u

#define ETHSWT_802_1Q_MODE_DISABLE  0u
#define ETHSWT_802_1Q_MODE_FALLBACK 1u
#define ETHSWT_802_1Q_MODE_CHECK    2u
#define ETHSWT_802_1Q_MODE_SECURE   3u
#define ETHSWT_802_1Q_MODE          ETHSWT_802_1Q_MODE_CHECK
#if (ETHSWT_802_1Q_MODE > ETHSWT_802_1Q_MODE_FALLBACK)
#define ETHSWT_PORTBASED_VLAN_SUPPORT STD_OFF
#else
#define ETHSWT_PORTBASED_VLAN_SUPPORT STD_ON
#endif /*ETHSWT_802_1Q_MODE > ETHSWT_802_1Q_MODE_FALLBACK*/

#define EthSwt_ReadRegister  (void)Eth_17_GEthMac_ReadMii
#define EthSwt_WriteRegister (void)Eth_17_GEthMac_WriteMii

typedef uint16 EthSwt_RegType;
#define ETHSWT_REG_GROUP_GLB1 0x1B
#define ETHSWT_REG_GROUP_GLB2 0x1C
/*port registers*/
#define ETHSWT_REG_PORT_STAT                          0x00
#define ETHSWT_REG_PORT_STAT_MASK_PAUSE               0x3FFFu
#define ETHSWT_REG_PORT_STAT_OFST_PAUSE               14u
#define ETHSWT_REG_PORT_STAT_MASK_CMODE               0xFFF0u
#define ETHSWT_REG_PORT_STAT_OFST_CMODE               0u

#define ETHSWT_REG_PHY_CTRL                           0x01

#define ETHSWT_REG_JAM_CTRL                           0x02

#define ETHSWT_REG_SWITCH_ID                          0x03

#define ETHSWT_REG_PORT_CTRL                          0x04
#define ETHSWT_REG_PORT_CTRL_PORTSTATE_MASK           0xFFFCu
#define ETHSWT_REG_PORT_CTRL_PORTSTATE_OFST           0u

#define ETHSWT_REG_PORT_CTRL1                         0x05

#define ETHSWT_REG_PORT_BASED_VLAN_MAP                0x06
#define ETHSWT_REG_PORT_BASED_VLAN_MAP_VLANTABLE_MASK 0xFE00u
#define ETHSWT_REG_PORT_BASED_VLAN_MAP_VLANTABLE_OFST 0u

#define ETHSWT_REG_DEF_PORT_VID_PRI                   0x07
#define ETHSWT_REG_DEF_PORT_VID_PRI_DEFPRI_MASK       0x1FFFu
#define ETHSWT_REG_DEF_PORT_VID_PRI_DEFPRI_OFST       13u
#define ETHSWT_REG_DEF_PORT_VID_PRI_DEFVID_MASK       0xF000u
#define ETHSWT_REG_DEF_PORT_VID_PRI_DEFVID_OFST       0u
#define ETHSWT_REG_DEF_PORT_VID_PRI_DROPUNTAGGED_OFST 8u
#define ETHSWT_REG_DEF_PORT_VID_PRI_FORCEDEFVID_OFST  12u

#define ETHSWT_REG_PORT_CTRL2                         0x08
#define ETHSWT_REG_PORT_CTRL2_8021QMODE_MASK          0xF3FFu
#define ETHSWT_REG_PORT_CTRL2_8021QMODE_OFST          10u
#define ETHSWT_REG_PORT_CTRL2_EG_MONITOR_SRC          5u
#define ETHSWT_REG_PORT_CTRL2_IN_MONITOR_SRC          4u

#define ETHSWT_REG_EGR_RATE_CTRL                      0x09

#define ETHSWT_REG_EGR_RATE_CTRL2                     0x0A

#define ETHSWT_REG_PORT_ASSOC_VEC                     0x0B
#define ETHSWT_REG_PORT_ASSOC_VEC_LOCKEDPORT_OFST     13u

#define ETHSWT_REG_PORT_ATU_CTRL                      0x0C

#define ETHSWT_REG_ORIDE                              0x0D

#define ETHSWT_REG_POL_CTRL                           0x0E

#define ETHSWT_REG_PORT_ETYPE                         0x0F

/*global1 registers*/
#define ETHSWT_REG_GLB1_SWITCH_GLB_STAT                       0x00

#define ETHSWT_REG_GLB1_ATU_FID                               0x01
#define ETHSWT_REG_GLB1_ATU_FID_FID_MASK                      0xFF00u
#define ETHSWT_REG_GLB1_ATU_FID_FID_OFST                      0u

#define ETHSWT_REG_GLB1_VTU_FID                               0x02
#define ETHSWT_REG_GLB1_VTU_FID_VIDPOLICY_MASK                0xEFFFu
#define ETHSWT_REG_GLB1_VTU_FID_VIDPOLICY_OFST                12u
#define ETHSWT_REG_GLB1_VTU_FID_FID_MASK                      0xFF00u
#define ETHSWT_REG_GLB1_VTU_FID_FID_OFST                      0u

#define ETHSWT_REG_GLB1_VTU_SID                               0x03
#define ETHSWT_REG_GLB1_VTU_SID_SID_MASK                      0xFFB0u
#define ETHSWT_REG_GLB1_VTU_SID_SID_OFST                      0u

#define ETHSWT_REG_GLB1_SWITCH_GLB_CTRL                       0x04

#define ETHSWT_REG_GLB1_VTU_OP                                0x05
#define ETHSWT_REG_GLB1_VTU_OP_VTUBUSY_OFST                   15u
#define ETHSWT_REG_GLB1_VTU_OP_VTUOP_MASK                     0x8FFFu
#define ETHSWT_REG_GLB1_VTU_OP_VTUOP_OFST                     12u

#define ETHSWT_REG_GLB1_VTU_VID                               0x06
#define ETHSWT_REG_GLB1_VTU_VID_VALID_MASK                    0xEFFFu
#define ETHSWT_REG_GLB1_VTU_VID_VALID_OFST                    12u
#define ETHSWT_REG_GLB1_VTU_VID_VID_MASK                      0xF000u
#define ETHSWT_REG_GLB1_VTU_VID_VID_OFST                      0u

#define ETHSWT_REG_GLB1_VTUSTU_DATA_PORT_MASK                 0x03
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU                  0x07
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP3_MASK 0xCFFFu
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP3_OFST 12u
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP2_MASK 0xFCFFu
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP2_OFST 8u
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP1_MASK 0xFFCFu
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP1_OFST 4u
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP0_MASK 0xFFFCu
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_VTU_MEMBERTAGP0_OFST 0u

#define ETHSWT_REG_GLB1_VTUSTU_DATA_P0_3_STU                  0x07

#define ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU                  0x08
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP7_MASK 0xCFFFu
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP7_OFST 12u
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP6_MASK 0xFCFFu
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP6_OFST 8u
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP5_MASK 0xFFCFu
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP5_OFST 4u
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP4_MASK 0xFFFCu
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_VTU_MEMBERTAGP4_OFST 0u

#define ETHSWT_REG_GLB1_VTUSTU_DATA_P4_7_STU                  0x08

#define ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU                    0x09
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU_MEMBERTAGP8_MASK   0xFFFCu
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU_MEMBERTAGP8_OFST   0u
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU_VIDPRI_MASK        0x8FFFu
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU_VIDPRI_OFST        12u
#define ETHSWT_REG_GLB1_VTUSTU_DATA_P8_VTU_PRIOVERRIDE_OFST   15u

#define ETHSWT_REG_GLB1_VTUSTU_DATA_P8_STU                    0x09
#define ETHSWT_REG_GLB1_ATU_CTRL                              0x0A
#define ETHSWT_REG_GLB1_ATU_CTRL_AGETIME_MASK                 0xF00Fu
#define ETHSWT_REG_GLB1_ATU_CTRL_AGETIME_OFST                 4u

#define ETHSWT_REG_GLB1_ATU_OP                                0x0B
#define ETHSWT_REG_GLB1_ATU_OP_BUSY_OFST                      15u
#define ETHSWT_REG_GLB1_ATU_OP_OP_MASK                        0x8FFFu
#define ETHSWT_REG_GLB1_ATU_OP_OP_OFST                        12u

#define ETHSWT_REG_GLB1_ATU_DATA                              0x0C
#define ETHSWT_REG_GLB1_ATU_DATA_ENTRYSTATE_MASK              0xFFF0u
#define ETHSWT_REG_GLB1_ATU_DATA_ENTRYSTATE_OFST              0u
#define ETHSWT_REG_GLB1_ATU_DATA_PORTVEC_MASK                 0xE00Fu
#define ETHSWT_REG_GLB1_ATU_DATA_PORTVEC_OFST                 4u

#define ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES0_1                 0x0D
#define ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES2_3                 0x0E
#define ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES4_5                 0x0F
#define ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_MASK          0x00FFu
#define ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_HIGH_OFST          8u
#define ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_MASK           0xFF00u
#define ETHSWT_REG_GLB1_ATU_MAC_ADDR_BYTES_LOW_OFST           0u

#define ETHSWT_REG_GLB1_MON_MGMT_CTRL                         0x1A
#define ETHSWT_REG_GLB1_MON_MGMT_UPDATE_OFST                  15u
#define ETHSWT_REG_GLB1_MON_MGMT_POINTTER_MASK                0xC0FFu
#define ETHSWT_REG_GLB1_MON_MGMT_POINTTER_OFST                8u
#define ETHSWT_REG_GLB1_MON_MGMT_DATA_MASK                    0xFF00u
#define ETHSWT_REG_GLB1_MON_MGMT_DATA_OFST                    0u

#define ETHSWT_REG_GLB1_TOTAL_FREE_CNT                        0x1B
#define ETHSWT_REG_GLB1_GLB_CTRL2                             0x1C
#define ETHSWT_REG_GLB1_STAT_OP                               0x1D
#define ETHSWT_REG_GLB1_STAT_CNT_BYTES3_2                     0x1E
#define ETHSWT_REG_GLB1_STAT_CNT_BYTES1_0                     0x1F

/*global2 registers*/
#define ETHSWT_REG_GLB2_INTR_SRC             0x00
#define ETHSWT_REG_GLB2_INTR_MSK             0x01
#define ETHSWT_REG_GLB2_FC_DLY               0x04
#define ETHSWT_REG_GLB2_SWITCH_MGMT          0x05
#define ETHSWT_REG_GLB2_DEV_MAP_TBL          0x06
#define ETHSWT_REG_GLB2_LAG_MSK_TBL          0x07
#define ETHSWT_REG_GLB2_LAG_MAP              0x08
#define ETHSWT_REG_GLB2_INGR_RATE_CMD        0x09
#define ETHSWT_REG_GLB2_INGR_RATE_DATA       0x0A
#define ETHSWT_REG_GLB2_XCHIP_PORT_VLAN_ADDR 0x0B
#define ETHSWT_REG_GLB2_XCHIP_PORT_VLAN_DATA 0x0C
#define ETHSWT_REG_GLB2_SWITCH_MAC_WOL_WOF   0x0D
#define ETHSWT_REG_GLB2_ATU_STATS            0x0E
#define ETHSWT_REG_GLB2_PRI_ORIDE_TBL        0x0F
#define ETHSWT_REG_GLB2_AHB_MASTER_CTRL      0x11
#define ETHSWT_REG_GLB2_ENERGY_MGMT          0x12
#define ETHSWT_REG_GLB2_EEPROM_CMD           0x14
#define ETHSWT_REG_GLB2_EEPROM_ADDR          0x15
#define ETHSWT_REG_GLB2_AVB_CMD              0x16
#define ETHSWT_REG_GLB2_AVB_DATA             0x17
#define ETHSWT_REG_GLB2_SMI_PHY_CMD_ACCESS   0x18
#define ETHSWT_REG_GLB2_SMI_PHY_DATA_ACCESS  0x19
#define ETHSWT_REG_GLB2_SMI_PHY_CMD_SETUP    0x18
#define ETHSWT_REG_GLB2_SMI_PHY_DATA_SETUP   0x19
#define ETHSWT_REG_GLB2_SCRATCH_MISC         0x1A
#define ETHSWT_REG_GLB2_WD_CTRL              0x1B
#define ETHSWT_REG_GLB2_QOS_WEIGHTS          0x1C
#define ETHSWT_REG_GLB2_MISC                 0x1D
#define ETHSWT_REG_GLB2_CUT_THRU_CTRL        0x1F

#endif /* ifndef ETHSWT_88Q5050_H_ */
