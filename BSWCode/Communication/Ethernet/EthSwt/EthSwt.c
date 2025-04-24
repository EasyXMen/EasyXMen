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
**  FILENAME    : EthSwt.c                                                                                            **
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
 *  V2.0.0    [20230206]  [zhiqiang.huang]   Fix version number
 *  V2.0.1    [20240702]  [zhiqiang.huang]   Modify the det judgment conditions for the EthSwt_MainFunction
 **********************************************************************************************************************/

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:EthSwt<br>
  RuleSorce:puhua-rule2024-2.rcf 2.3.5

    \li PRQA S 2877 MISRA Dir 4.1 .<br>
    Reason:ETHTRCV_TRCVS_NUM follows configuration changes.

    \li PRQA S 3218 MISRA Rule 8.9 .<br>
    Reason:That a variable is used by more than one function is a matter of configuration.
 */

/***********************************************************************************************************************
 *  INCLUDES
 ***********************************************************************************************************************/
#include "EthSwt_Internal.h"
#include "EthSwt.h"
/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA
 ***********************************************************************************************************************/
#define ETHSWT_START_SEC_VAR_CLEARED_8
#include "EthSwt_MemMap.h"
static VAR(EthSwt_StateType, ETHSWT_VAR_ZERO_INIT) EthSwt_InitState = ETHSWT_STATE_UNINIT;
#define ETHSWT_STOP_SEC_VAR_CLEARED_8
#include "EthSwt_MemMap.h"

#define ETHSWT_START_SEC_VAR_CLEARED_PTR
#include "EthSwt_MemMap.h"
/* PRQA S 3218 ++ */ /* MISRA Rule 8.9 */
static P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_VAR) EthSwt_PbCfgPtr = NULL_PTR;
/* PRQA S 3218 -- */ /* MISRA Rule 8.9 */
#define ETHSWT_STOP_SEC_VAR_CLEARED_PTR
#include "EthSwt_MemMap.h"

/***********************************************************************************************************************
**                                              Version                                                               **
***********************************************************************************************************************/
#define ETHSWT_C_AR_MAJOR_VERSION 4u
#define ETHSWT_C_AR_MINOR_VERSION 5u
#define ETHSWT_C_AR_PATCH_VERSION 0u
#define ETHSWT_C_SW_MAJOR_VERSION 2u
#define ETHSWT_C_SW_MINOR_VERSION 0u
#define ETHSWT_C_SW_PATCH_VERSION 1u
/***********************************************************************************************************************
 *  VERSION CHECK
 ***********************************************************************************************************************/
/* check the version of EthSwt header files */
#if (                                                           \
    (ETHSWT_C_AR_MAJOR_VERSION != ETHSWT_H_AR_MAJOR_VERSION)    \
    || (ETHSWT_C_AR_MINOR_VERSION != ETHSWT_H_AR_MINOR_VERSION) \
    || (ETHSWT_C_AR_PATCH_VERSION != ETHSWT_H_AR_PATCH_VERSION))
#error "version numbers of EthSwt.h and EthSwt.c are inconsistent"
#endif

/* check the version of the configuration header file */
#if (                                                               \
    (ETHSWT_C_AR_MAJOR_VERSION != ETHSWT_CFG_H_AR_MAJOR_VERSION)    \
    || (ETHSWT_C_AR_MINOR_VERSION != ETHSWT_CFG_H_AR_MINOR_VERSION) \
    || (ETHSWT_C_AR_PATCH_VERSION != ETHSWT_CFG_H_AR_PATCH_VERSION) \
    || (ETHSWT_C_SW_MAJOR_VERSION != ETHSWT_CFG_H_SW_MAJOR_VERSION) \
    || (ETHSWT_C_SW_MINOR_VERSION != ETHSWT_CFG_H_SW_MINOR_VERSION))
#error "version numbers of EthSwt_Cfg.h and EthSwt.c are inconsistent"
#endif

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
#define ETHSWT_START_SEC_CODE
#include "EthSwt_MemMap.h"
/***********************************************************************************************************************
 * Service ID[hex]:     0x01
 * Sync/Async:          Synchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     ConfigPtr Pointer to the configuration data of the EthSwt module
 * Parameters(inout):   None
 * Parameters (out):    None
 * Return value:        None
 * Description:         Initializes the Ethernet Switch Driver.
 ***********************************************************************************************************************/
FUNC(void, ETHSWT_CODE) EthSwt_Init(P2CONST(EthSwt_ConfigType, AUTOMATIC, ETHSWT_APPL_CONST) CfgPtr)
{
    uint8 idx;
#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    /*SWS_EthSwt_00009*/
    if (NULL_PTR == CfgPtr)
    {
        ETHSWT_DET(ETHSWT_SID_INIT, ETHSWT_E_INIT_FAILED);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        EthSwt_InitState = ETHSWT_STATE_INIT;
        EthSwt_PbCfgPtr = CfgPtr;

        for (idx = 0u; idx < ETHSWT_DEVICE_NUM; idx++) /* PRQA S 2877,1862 */ /* MISRA Dir 4.1,Rule 10.4 */
        {
#if (STD_ON == ETHSWT_ARL_NVM_SUPPORT || STD_ON == ETHSWT_MIRROR_NVM_SUPPORT)
            EthSwt_NvMBlkInit(&EthSwt_PbCfgPtr[idx]);
#endif /* STD_ON == ETHSWT_ARL_NVM_SUPPORT || STD_ON == ETHSWT_MIRROR_NVM_SUPPORT */
            EthSwt_SwitchOffAllPorts(&EthSwt_PbCfgPtr[idx]);
            /*check mirror*/
#if (STD_ON == ETHSWT_MIRROR_NVM_SUPPORT)
            EthSwt_MirroredPortRestore(&EthSwt_PbCfgPtr[idx]);
#endif /* STD_ON == ETHSWT_MIRROR_NVM_SUPPORT */
            /*switch off nonused port*/
            EthSwt_ATUStaticMacInit(&EthSwt_PbCfgPtr[idx]);
#if (STD_ON == ETHSWT_VLAN_USED)
            EthSwt_VTUVlanInit(&EthSwt_PbCfgPtr[idx]);
#endif /* STD_ON == ETHSWT_VLAN_USED */
            EthSwt_PortInit(&EthSwt_PbCfgPtr[idx]);
            EthSwt_InitState = ETHSWT_STATE_PORTINIT_COMPLETED;
            EthSwt_GLBInit(&EthSwt_PbCfgPtr[idx]);
        }

        EthSwt_InitState = ETHSWT_STATE_ACTIVE;
    }
}

/***********************************************************************************************************************
 *
 * Service ID[hex]:     0x18
 * Sync/Async:          Synchronous
 * Reentrancy:          Reentrant
 * Parameters (in):     None
 * Parameters(inout):   None
 * Parameters (out):    versioninfo Pointer to where to store the version information of this module.
 * Return value:        None
 * Description:         Returns the version information.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_VERSION_INFO_API)
FUNC(void, ETHSWT_CODE) EthSwt_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, ETHSWT_APPL_DATA) versioninfo)
{
#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (NULL_PTR == versioninfo)
    {
        ETHSWT_DET(ETHSWT_SID_GETVERSIONINFO, ETHSWT_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        versioninfo->moduleID = ETHSWT_MODULE_ID;
        versioninfo->vendorID = ETHSWT_VENDOR_ID;
        versioninfo->sw_major_version = ETHSWT_C_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = ETHSWT_C_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = ETHSWT_C_SW_PATCH_VERSION;
    }
}
#endif /* STD_ON == ETHSWT_VERSION_INFO_API */

/***********************************************************************************************************************
 * Service ID[hex]:     0x03
 * Sync/Async:          Synchronous /Asynchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 *                      SwitchPortIdx: Index of the port at the addressed switch
 *                      PortMode: ETH_MODE_DOWN: Disable the addressed Ethernet switch port at the given Ethernet switch
 *                      ETH_MODE_ACTIVE: Enable the addressed Ethernet switch port at the given Ethernet switch
 * Parameters(inout):   None
 * Parameters (out):    None.
 * Return value:        E_OK: success
 *                      E_NOT_OK: The indexed switch port could not be set to Port Mode, or the function is called in
 *                      state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT.
 * Description:         Enables/disables the indexed switch port.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_SET_SWITCH_PORT_MODE_API)
FUNC(Std_ReturnType, ETHSWT_CODE) EthSwt_SetSwitchPortMode(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_ModeType PortMode)
{
    Std_ReturnType retVal = E_NOT_OK;
    uint8 cfgPortIdx;
    P2CONST(EthSwt_EthTrcvType, ETHSWT_APPL_DATA, ETHSWT_VAR) ethTrcv;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_SETSWITCHPORTMODE, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM <= SwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_SETSWITCHPORTMODE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (0u == (EthSwt_PbCfgPtr[SwitchIdx].SwtPortMask & ((uint16)0x01u << (uint16)SwitchPortIdx)))
    {
        ETHSWT_DET(ETHSWT_SID_SETSWITCHPORTMODE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        retVal = EthSwt_SetSwitchPortModeInternal(SwitchIdx, SwitchPortIdx, PortMode);
        cfgPortIdx = EthSwt_GetCfgPortIdx(&EthSwt_PbCfgPtr[SwitchIdx], SwitchPortIdx);
        ethTrcv = EthSwt_PbCfgPtr[SwitchIdx].PortCfgPtr[cfgPortIdx].TrcvRef;
        if ((NULL_PTR != ethTrcv) && ((NULL_PTR != ethTrcv->DrvApi)) && (E_OK == retVal))
        {
            if ((ethTrcv->DrvApi->SetTransceiverMode != NULL_PTR))
            {
                retVal = ethTrcv->DrvApi->SetTransceiverMode(ethTrcv->TrcvIdx, PortMode);
            }
#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
            else
            {
                ETHSWT_DET(ETHSWT_SID_SETSWITCHPORTMODE, ETHSWT_E_INV_API);
            }
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
        }
    }
    return retVal;
}
#endif /* STD_ON == ETHSWT_SET_SWITCH_PORT_MODE_API */

/***********************************************************************************************************************
 * Service ID[hex]:     0x04
 * Sync/Async:          Synchronous /Asynchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 *                      SwitchPortIdx: Index of the port at the addressed switch
 * Parameters(inout):   None.
 * Parameters(out):     SwitchModePtr: ETH_MODE_DOWN: The Ethernet switch port of the given Ethernet switch is disabled.
 *                      ETH_MODE_ACTIVE: The Ethernet switch port of the given Ethernet switch is enabled
 * Return value:        E_OK: success E_NOT_OK: The mode of the indexed switch port could not be obtained, or the
 *                      function is called in state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT.
 * Description:         Obtains the mode of the indexed switch port.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_GET_SWITCH_PORT_MODE_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_GetSwitchPortMode(uint8 SwitchIdx, uint8 SwitchPortIdx, Eth_ModeType* SwitchModePtr)
{
    Std_ReturnType retVal = E_NOT_OK;
    uint8 cfgPortIdx;
    P2CONST(EthSwt_EthTrcvType, ETHSWT_APPL_DATA, ETHSWT_VAR) ethTrcv;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_GETDUPLEXMODE, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM <= SwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_GETSWITCHPORTMODE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (0u == (EthSwt_PbCfgPtr[SwitchIdx].SwtPortMask & ((uint16)0x01u << (uint16)SwitchPortIdx)))
    {
        ETHSWT_DET(ETHSWT_SID_GETSWITCHPORTMODE, ETHSWT_E_INV_SWITCH_IDX);
    }

    else if (NULL_PTR == SwitchModePtr)
    {
        ETHSWT_DET(ETHSWT_SID_GETSWITCHPORTMODE, ETHSWT_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        retVal = EthSwt_GetSwitchPortModeInternal(SwitchIdx, SwitchPortIdx, SwitchModePtr);
        cfgPortIdx = EthSwt_GetCfgPortIdx(&EthSwt_PbCfgPtr[SwitchIdx], SwitchPortIdx);
        ethTrcv = EthSwt_PbCfgPtr[SwitchIdx].PortCfgPtr[cfgPortIdx].TrcvRef;
        if ((NULL_PTR != ethTrcv) && ((NULL_PTR != ethTrcv->DrvApi)) && (E_OK == retVal))
        {
            if ((ethTrcv->DrvApi->GetTransceiverMode != NULL_PTR))
            {
                retVal = ethTrcv->DrvApi->GetTransceiverMode(ethTrcv->TrcvIdx, SwitchModePtr);
            }
#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
            else
            {
                ETHSWT_DET(ETHSWT_SID_GETSWITCHPORTMODE, ETHSWT_E_INV_API);
            }
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
        }
    }
    return retVal;
}
#endif /* STD_ON == ETHSWT_GET_SWITCH_PORT_MODE_API */

/***********************************************************************************************************************
 * Service ID[hex]:     0x08
 * Sync/Async:          Synchronous /Asynchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 *                      SwitchPortIdx: Index of the port at the addressed switch
 * Parameters(inout):   None
 * Parameters (out):    DuplexModePtr: ETHTRCV_DUPLEX_MODE_HALF: half duplex connections ETHTRCV_DUPLEXMODE_FULL: full
 *                      duplex connection switch port of the given Ethernet switch is enabled.
 * Return value:        E_OK: success
 *                      E_NOT_OK: duplex mode of the indexed switch port could not be obtained, or the function is
 *                      called in state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT.
 * Description:         Obtains the duplex mode of the indexed switch port.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_GET_DUPLEX_MODE_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_GetDuplexMode(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_DuplexModeType* DuplexModePtr)
{
    Std_ReturnType retVal = E_NOT_OK;
    uint8 cfgPortIdx;
    P2CONST(EthSwt_EthTrcvType, ETHSWT_APPL_DATA, ETHSWT_VAR) ethTrcv;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_GETDUPLEXMODE, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM <= SwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_GETDUPLEXMODE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (0u == (EthSwt_PbCfgPtr[SwitchIdx].SwtPortMask & ((uint16)0x01u << (uint16)SwitchPortIdx)))
    {
        ETHSWT_DET(ETHSWT_SID_GETDUPLEXMODE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (NULL_PTR == DuplexModePtr)
    {
        ETHSWT_DET(ETHSWT_SID_GETDUPLEXMODE, ETHSWT_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        cfgPortIdx = EthSwt_GetCfgPortIdx(&EthSwt_PbCfgPtr[SwitchIdx], SwitchPortIdx);
        ethTrcv = EthSwt_PbCfgPtr[SwitchIdx].PortCfgPtr[cfgPortIdx].TrcvRef;
        if (ethTrcv->DrvApi->GetDuplexMode != NULL_PTR)
        {
            retVal = ethTrcv->DrvApi->GetDuplexMode(ethTrcv->TrcvIdx, DuplexModePtr);
        }
    }
    return retVal;
}
#endif /* STD_ON == ETHSWT_GET_DUPLEX_MODE_API */

/***********************************************************************************************************************
 * Service ID[hex]:     0x06
 * Sync/Async:          Synchronous /Asynchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 *                      SwitchPortIdx: Index of the port at the addressed switch
 * Parameters(inout):   None
 * Parameters (out):    LinkStatePtr: ETHTRCV_LINK_STATE_DOWN: Switch port is disconnected
 *                      ETHTRCV_LINK_STATE_ACTIVE: Switch port is connected.
 * Return value:        E_OK: success
                        E_NOT_OK: Link state of the indexed switch port could not be obtained, or the function is
                        called in state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT.
 * Description:         Obtains the link state of the indexed switch port.
***********************************************************************************************************************/
#if (STD_ON == ETHSWT_GET_LINK_STATE_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_GetLinkState(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_LinkStateType* LinkStatePtr)
{
    Std_ReturnType retVal = E_NOT_OK;
    uint8 cfgPortIdx;
    P2CONST(EthSwt_EthTrcvType, ETHSWT_APPL_DATA, ETHSWT_VAR) ethTrcv;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_GETLINKSTATE, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM <= SwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_GETLINKSTATE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (0u == (EthSwt_PbCfgPtr[SwitchIdx].SwtPortMask & ((uint16)0x01u << (uint16)SwitchPortIdx)))
    {
        ETHSWT_DET(ETHSWT_SID_GETLINKSTATE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (NULL_PTR == LinkStatePtr)
    {
        ETHSWT_DET(ETHSWT_SID_GETLINKSTATE, ETHSWT_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        retVal = E_OK;
        cfgPortIdx = EthSwt_GetCfgPortIdx(&EthSwt_PbCfgPtr[SwitchIdx], SwitchPortIdx);
        ethTrcv = EthSwt_PbCfgPtr[SwitchIdx].PortCfgPtr[cfgPortIdx].TrcvRef;
        if ((NULL_PTR != ethTrcv) && ((NULL_PTR != ethTrcv->DrvApi)) && (E_OK == retVal))
        {
            if ((ethTrcv->DrvApi->GetLinkState != NULL_PTR))
            {
                retVal = ethTrcv->DrvApi->GetLinkState(ethTrcv->TrcvIdx, LinkStatePtr);
            }
#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
            else
            {
                ETHSWT_DET(ETHSWT_SID_GETLINKSTATE, ETHSWT_E_INV_API);
            }
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
        }
    }
    return retVal;
}
#endif /* STD_ON == ETHSWT_GET_LINK_STATE_API */

/***********************************************************************************************************************
 * Service ID[hex]:     0x07
 * Sync/Async:          Synchronous /Asynchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 *                      SwitchPortIdx: Index of the port at the addressed switch
 * Parameters(inout):   None
 * Parameters (out):    BaudRatePtr: ETHTRCV_BAUD_RATE_10MBIT: 10MBit connection
 *                      ETHTRCV_BAUD_RATE_100MBIT: 100MBit connection
 *                      ETHTRCV_BAUD_RATE_1000MBIT: 1000MBit connection
 *                      ETHTRCV_BAUD_RATE_2500MBIT: 2500MBit connection.
 * Return value:        E_OK: success
 *                      E_NOT_OK: Baud rate of the indexed switch port could not be obtained, or the function is called
 *                      in state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT.
 * Description:         Obtains the baud rate of the indexed switch port.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_GET_BAUD_RATE_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_GetBaudRate(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_BaudRateType* BaudRatePtr)
{
    Std_ReturnType retVal = E_NOT_OK;
    uint8 cfgPortIdx;
    P2CONST(EthSwt_EthTrcvType, ETHSWT_APPL_DATA, ETHSWT_VAR) ethTrcv;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_GETBAUDRATE, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM <= SwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_GETBAUDRATE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (0u == (EthSwt_PbCfgPtr[SwitchIdx].SwtPortMask & ((uint16)0x01u << (uint16)SwitchPortIdx)))
    {
        ETHSWT_DET(ETHSWT_SID_GETBAUDRATE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (NULL_PTR == BaudRatePtr)
    {
        ETHSWT_DET(ETHSWT_SID_GETBAUDRATE, ETHSWT_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        cfgPortIdx = EthSwt_GetCfgPortIdx(&EthSwt_PbCfgPtr[SwitchIdx], SwitchPortIdx);
        ethTrcv = EthSwt_PbCfgPtr[SwitchIdx].PortCfgPtr[cfgPortIdx].TrcvRef;
        if (ethTrcv->DrvApi->GetBaudRate != NULL_PTR)
        {
            retVal = ethTrcv->DrvApi->GetBaudRate(ethTrcv->TrcvIdx, BaudRatePtr);
        }
    }
    return retVal;
}
#endif /* STD_ON == ETHSWT_GET_BAUD_RATE_API */

/***********************************************************************************************************************
 * Service ID[hex]:     0x05
 * Sync/Async:          Synchronous /Asynchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 *                      SwitchPortIdx: Index of the port at the addressed switch
 * Parameters(inout):   None
 * Parameters (out):    None.
 * Return value:        E_OK: success
 *                      E_NOT_OK: Automatic negotiation could not be started for the indexed switch port, or the
 *                      function is called in state ETHSWT_STATE_UNINIT or ETHSWT_STATE_INIT.
 * Description:         Starts the auto-negotiation of the indexed switch port.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_START_SWITCH_PORT_AUTO_NEGOTIATION_API)
FUNC(Std_ReturnType, ETHSWT_CODE) EthSwt_StartSwitchPortAutoNegotiation(uint8 SwitchIdx, uint8 SwitchPortIdx)
{
    Std_ReturnType retVal = E_NOT_OK;
    uint8 cfgPortIdx;
    P2CONST(EthSwt_EthTrcvType, ETHSWT_APPL_DATA, ETHSWT_VAR) ethTrcv;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_STARTSWITCHPORTAUTONEGOTIATION, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM <= SwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_STARTSWITCHPORTAUTONEGOTIATION, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (0u == (EthSwt_PbCfgPtr[SwitchIdx].SwtPortMask & ((uint16)0x01u << (uint16)SwitchPortIdx)))
    {
        ETHSWT_DET(ETHSWT_SID_STARTSWITCHPORTAUTONEGOTIATION, ETHSWT_E_INV_SWITCH_IDX);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        cfgPortIdx = EthSwt_GetCfgPortIdx(&EthSwt_PbCfgPtr[SwitchIdx], SwitchPortIdx);
        ethTrcv = EthSwt_PbCfgPtr[SwitchIdx].PortCfgPtr[cfgPortIdx].TrcvRef;
        if (ethTrcv->DrvApi->StartAutoNegotiation != NULL_PTR)
        {
            retVal = ethTrcv->DrvApi->StartAutoNegotiation(ethTrcv->TrcvIdx);
        }
    }

    return retVal;
}
#endif /* STD_ON == ETHSWT_START_SWITCH_PORT_AUTO_NEGOTIATION_API */

/***********************************************************************************************************************
 * Service ID[hex]:     0x4c
 * Sync/Async:          Synchronous
 * Reentrancy:          Reentrant
 * Parameters (in):     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 * Parameters(inout):   None
 * Parameters (out):    None.
 * Return value:        E_OK: request to check for a wake-up is accepted.
 *                      E_NOT_OK: request to check for a wake-up is not accepted.
 * Description:         API is called by EthIf. The Ethernet switch driver request to check for a wake-up at all
 *                      Ethernet switch ports which reference an EthTrcv. For those Ethernet switch ports the call
 *                      is forwarded to the referenced EthTrcv. The function could be called in context of an
 *                      interrupt service routine or on task leve.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_CHECK_WAKEUP_API)
FUNC(Std_ReturnType, ETHSWT_CODE) EthSwt_CheckWakeup(uint8 SwitchIdx)
{
    Std_ReturnType retVal = E_NOT_OK;
    uint8 cfgPortIdx;
    P2CONST(EthSwt_EthTrcvType, ETHSWT_APPL_DATA, ETHSWT_VAR) ethTrcv;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_CHECKWAKEUP, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM <= SwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_CHECKWAKEUP, ETHSWT_E_INV_SWITCH_IDX);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        if (ETHSWT_STATE_ACTIVE != EthSwt_InitState)
        {
            ETHSWT_RUNTIME_ERR(ETHSWT_SID_CHECKWAKEUP, ETHSWT_INIT_NOT_COMPLETED);
        }
        else
        {
            for (cfgPortIdx = 0u; cfgPortIdx < EthSwt_PbCfgPtr[SwitchIdx].SwtPortNbr; cfgPortIdx++)
            {
                ethTrcv = EthSwt_PbCfgPtr[SwitchIdx].PortCfgPtr[cfgPortIdx].TrcvRef;
                if (ethTrcv->DrvApi->CheckWakeup != NULL_PTR)
                {
                    retVal = ethTrcv->DrvApi->CheckWakeup(ethTrcv->TrcvIdx);
                }
                if (E_OK != retVal)
                {
                    break;
                }
            }
        }
    }

    return retVal;
}
#endif /* STD_ON == ETHSWT_CHECK_WAKEUP_API */

/***********************************************************************************************************************
 * Service ID[hex]:     0x4b
 * Sync/Async:          Synchronous
 * Reentrancy:          Reentrant
 * Parameters (in):     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver.
 *                      SwitchPortIdx: Index of the Ethernet switch port index in the context of the Ethernet
 *                      switch driver.
 * Parameters(inout):   None
 * Parameters (out):    Reason:Pointer to structure of least recent wakeup event, which was detected by the Ethernet
 *                      switch port.
 * Return value:        E_OK: Ethernet switch port wake up reason request has been accepted.
 *                      E_NOT_OK: Ethernet switch port wake up reason request has not been accepted.
 * Description:         This function obtains the wake up reasons of the the indexed Ethernet switch port by calling
 *                      Eth Trcv_GetBusWuReason(...) of the referenced EthTrcv.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_GET_SWITCH_WAKEUP_REASON_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_GetSwitchPortWakeupReason(uint8 SwitchIdx, uint8 SwitchPortIdx, EthTrcv_WakeupReasonType* Reason)
{
    Std_ReturnType retVal = E_NOT_OK;
    uint8 cfgPortIdx;
    P2CONST(EthSwt_EthTrcvType, ETHSWT_APPL_DATA, ETHSWT_VAR) ethTrcv;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_GETSWITCHPORTWAKEUPREASON, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM <= SwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_GETSWITCHPORTWAKEUPREASON, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (0u == (EthSwt_PbCfgPtr[SwitchIdx].SwtPortMask & ((uint16)0x01u << (uint16)SwitchPortIdx)))
    {
        ETHSWT_DET(ETHSWT_SID_GETSWITCHPORTWAKEUPREASON, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (NULL_PTR == Reason)
    {
        ETHSWT_DET(ETHSWT_SID_GETSWITCHPORTWAKEUPREASON, ETHSWT_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        cfgPortIdx = EthSwt_GetCfgPortIdx(&EthSwt_PbCfgPtr[SwitchIdx], SwitchPortIdx);
        ethTrcv = EthSwt_PbCfgPtr[SwitchIdx].PortCfgPtr[cfgPortIdx].TrcvRef;
        if (ETHSWT_STATE_ACTIVE != EthSwt_InitState)
        {
            ETHSWT_RUNTIME_ERR(ETHSWT_SID_GETSWITCHPORTWAKEUPREASON, ETHSWT_INIT_NOT_COMPLETED);
        }
        else
        {
            if (ethTrcv->DrvApi->GetBusWuReason != NULL_PTR)
            {
                retVal = ethTrcv->DrvApi->GetBusWuReason(ethTrcv->TrcvIdx, Reason);
            }
        }
    }

    return retVal;
}
#endif /* STD_ON == ETHSWT_GET_SWITCH_WAKEUP_REASON_API */

/***********************************************************************************************************************
 * Service ID[hex]:     0x09
 * Sync/Async:          Synchronous /Asynchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 *                      MacAddrPtr: MAC-address for which a switch port is searched over which the node with this
 *                      MAC-address can be reached
 * Parameters(inout):   None
 * Parameters (out):    PortIdxPtr: Pointer to the port index.
 * Return value:        E_OK: success
                        E_NOT_OK: multiple ports were found
 * Description:         Obtains the port over which this MAC-address at the indexed switch can be reached. The result
 *                      might be used for a DHCP-server which will need the port/MAC-resolution.If for the PortIdxPtr
 *                      the maximal possible value (255) is returned the given MAC address cannot be reached via a port
 *                      of this switch. If multiple ports were found the API returns E_NOT_OK.
***********************************************************************************************************************/
#if (STD_ON == ETHSWT_GET_PORT_MAC_ADDR_API)
FUNC(Std_ReturnType, ETHSWT_CODE) EthSwt_GetPortMacAddr(uint8 SwitchIdx, const uint8* MacAddrPtr, uint8* PortIdxPtr)
{
    Std_ReturnType retVal = E_NOT_OK;
    EthSwt_AtuOpInfoType atuInfo;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_GETPORTMACADDR, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM <= SwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_GETPORTMACADDR, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if ((NULL_PTR == MacAddrPtr) || (NULL_PTR == PortIdxPtr))
    {
        ETHSWT_DET(ETHSWT_SID_GETPORTMACADDR, ETHSWT_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        if (ETHSWT_STATE_ACTIVE != EthSwt_InitState)
        {
            ETHSWT_RUNTIME_ERR(ETHSWT_SID_GETPORTMACADDR, ETHSWT_INIT_NOT_COMPLETED);
        }
        else
        {
            atuInfo.fid = 0u;
            ILib_memcpy((uint8*)&atuInfo.macAddress.macAddr[0], (const uint8*)MacAddrPtr, ETHSWT_MAC_LEN);
            atuInfo.macAddress.macAddr[5] = atuInfo.macAddress.macAddr[5] - 1u;
            (void)EthSwt_ATUGetEntry(SwitchIdx, &atuInfo, ETHSWT_ATU_OP_GET_NEXT);
            *PortIdxPtr = (uint8)(atuInfo.macAddress.portVec & (uint16)0x00ff);

            retVal = E_OK;
        }
    }

    return retVal;
}
#endif /* STD_ON == ETHSWT_GET_PORT_MAC_ADDR_API */

/***********************************************************************************************************************
 * Service ID[hex]:     0x12
 * Sync/Async:          Synchronous /Asynchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 *                      SwitchPortIdx: Index of the port at the addressed switch
 *                      VlanId: VLAN-ID to a preconfigured configuration on the given ingress port
 *                      Enable: 1 = VLAN-configuration enabled 0 = VLAN-configuration disabled(frames with given
 *                      VLAN-ID will be dropped)
 * Parameters(inout):   None
 * Parameters (out):    None.
 * Return value:        E_OK: success
 *                      E_NOT_OK: buffer level could not be obtained
 * Description:         Enables or disables a pre-configured VLAN at a certain port of a switch.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_ENABLE_VLAN_API)
FUNC(Std_ReturnType, ETHSWT_CODE) EthSwt_EnableVlan(uint8 SwitchIdx, uint8 SwitchPortIdx, uint16 VlanId, boolean Enable)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_ENABLEVLAN, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM <= SwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_ENABLEVLAN, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (0u == (EthSwt_PbCfgPtr[SwitchIdx].SwtPortMask & ((uint16)0x01u << (uint16)SwitchPortIdx)))
    {
        ETHSWT_DET(ETHSWT_SID_ENABLEVLAN, ETHSWT_E_INV_SWITCH_IDX);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        if (ETHSWT_STATE_ACTIVE != EthSwt_InitState)
        {
            ETHSWT_RUNTIME_ERR(ETHSWT_SID_ENABLEVLAN, ETHSWT_INIT_NOT_COMPLETED);
        }
        else
        {
            /*TBD*/
            retVal = EthSwt_EnableVlanInternal(SwitchIdx, SwitchPortIdx, VlanId, Enable);
        }
    }

    return retVal;
}
#endif /*STD_ON == ETHSWT_ENABLE_VLAN_API*/

/***********************************************************************************************************************
 * Service ID[hex]:     0x15
 * Sync/Async:          Synchronous /Asynchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 *                      SwitchPortIdx: Index of the port at the addressed switch
 *                      MacLearningMode: Defines whether MAC addresses shall be learned and if they shall be learned
 *                      in software or hardware.
 * Parameters(inout):   None
 * Parameters (out):    None.
 * Return value:        E_OK: success
 *                      E_NOT_OK: configuration could be persistently reset
 * Description:         Sets the MAC learning mode in one of the tree modes:
 *                      1.) HW learning enabled, 2.) Hardware learning disabled, 3.) Software learning enabled.
 *                      Note: This feature is hardware dependent, i.e. the switch hardware needs to support the
 *                      different learning modes.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_SET_MAC_LEARNING_MODE_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_SetMacLearningMode(uint8 SwitchIdx, uint8 SwitchPortIdx, EthSwt_MacLearningType MacLearningMode)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_SETMACLEARNINGMODE, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM <= SwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_SETMACLEARNINGMODE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (0u == (EthSwt_PbCfgPtr[SwitchIdx].SwtPortMask & ((uint16)0x01u << (uint16)SwitchPortIdx)))
    {
        ETHSWT_DET(ETHSWT_SID_SETMACLEARNINGMODE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        if (ETHSWT_STATE_ACTIVE != EthSwt_InitState)
        {
            ETHSWT_RUNTIME_ERR(ETHSWT_SID_SETMACLEARNINGMODE, ETHSWT_INIT_NOT_COMPLETED);
        }
        else
        {
            retVal = EthSwt_SetMacLearningModeInternal(SwitchIdx, SwitchPortIdx, MacLearningMode);
        }
    }
    return retVal;
}
#endif /*STD_ON == ETHSWT_SET_MAC_LEARNING_MODE_API*/

/***********************************************************************************************************************
 * Service ID[hex]:     0x16
 * Sync/Async:          Synchronous /Asynchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 *                      SwitchPortIdx: Index of the port at the addressed switch
 * Parameters(inout):   None
 * Parameters (out):    MacLearningMode: Defines whether MAC addresses shall be learned and if they shall be learned in
 *                      software or hardware.
 * Return value:        E_OK: success
 *                      E_NOT_OK: configuration could be persistently reset
 * Description:         Returns the MAC learning mode, i.e.
 *                      1.) HW learning enabled, 2.) Hardware learning disabled, 3.) Software learning enabled.
 *                      Note: This feature is hardware dependent, i.e. the switch hardware needs to support the
 *                      different learning modes.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_GET_MAC_LEARNING_MODE_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_GetMacLearningMode(uint8 SwitchIdx, uint8 SwitchPortIdx, EthSwt_MacLearningType* MacLearningMode)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_GETMACLEARNINGMODE, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM <= SwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_GETMACLEARNINGMODE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (0u == (EthSwt_PbCfgPtr[SwitchIdx].SwtPortMask & ((uint16)0x01u << (uint16)SwitchPortIdx)))
    {
        ETHSWT_DET(ETHSWT_SID_GETMACLEARNINGMODE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (NULL_PTR == MacLearningMode)
    {
        ETHSWT_DET(ETHSWT_SID_GETMACLEARNINGMODE, ETHSWT_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        if (ETHSWT_STATE_ACTIVE != EthSwt_InitState)
        {
            ETHSWT_RUNTIME_ERR(ETHSWT_SID_GETMACLEARNINGMODE, ETHSWT_INIT_NOT_COMPLETED);
        }
        else
        {
            retVal = EthSwt_GetMacLearningModeInternal(SwitchIdx, SwitchPortIdx, MacLearningMode);
        }
    }

    return retVal;
}
#endif /*STD_ON == ETHSWT_GET_MAC_LEARNING_MODE_API*/

/***********************************************************************************************************************
 * Service ID[hex]:     0x32
 * Sync/Async:          Synchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 *                      mode: True Forewarding enabled, False Forwarding disabled
 * Parameters(inout):   None
 * Parameters (out):    None.
 * Return value:        E_OK: stopping of frame forwarding succeeded,
 *                      E_NOT_OK: stopping of frame forwarding not succeeded.
 * Description:         Configures switch to start or stop forwarding for all ports.
 *                      This API call may be used during switch configuration verification.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_SET_FORWARDING_MODE_API)
FUNC(Std_ReturnType, ETHSWT_CODE) EthSwt_SetForwardingMode(uint8 SwitchIdx, boolean mode)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_SETFORWARDINGMODE, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM <= SwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_SETFORWARDINGMODE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        if (ETHSWT_STATE_ACTIVE != EthSwt_InitState)
        {
            ETHSWT_RUNTIME_ERR(ETHSWT_SID_SETFORWARDINGMODE, ETHSWT_INIT_NOT_COMPLETED);
        }
        else
        {
            /*used for test mode*/
            (void)mode;
            retVal = E_OK;
        }
    }

    return retVal;
}
#endif /*STD_ON == ETHSWT_SET_FORWARDING_MODE_API*/

/***********************************************************************************************************************
 * Service ID[hex]:     0x3c
 * Sync/Async:          Synchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 *                      PortIdx: Index of the port at the addressed switch
 *                      EthSwt_SetPortTxMode
 * Parameters(inout):   None
 * Parameters (out):    None.
 * Return value:        E_OK: the port Tx mode for the indexed Ethernet switch port was activated successfully.
 *                      E_NOT_OK: the port Tx mode for the indexed Ethernet switch port was not activated successfully.
 *                      (i.e. indexed Ethernet switch port is not available)
 * Description:         Activates a given transmission mode of the indexed Ethernet switch port.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_SET_PORT_TX_MODE_API)
FUNC(Std_ReturnType, ETHSWT_CODE) EthSwt_SetPortTxMode(uint8 SwitchIdx, uint8 PortIdx, EthTrcv_PhyTxModeType Mode)
{
    Std_ReturnType retVal = E_NOT_OK;
    uint8 cfgPortIdx;
    P2CONST(EthSwt_EthTrcvType, ETHSWT_APPL_DATA, ETHSWT_VAR) ethTrcv;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_SETPORTTXMODE, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM <= SwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_SETPORTTXMODE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (0u == (EthSwt_PbCfgPtr[SwitchIdx].SwtPortMask & ((uint16)0x01u << (uint16)PortIdx)))
    {
        ETHSWT_DET(ETHSWT_SID_SETPORTTXMODE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        cfgPortIdx = EthSwt_GetCfgPortIdx(&EthSwt_PbCfgPtr[SwitchIdx], PortIdx);
        ethTrcv = EthSwt_PbCfgPtr[SwitchIdx].PortCfgPtr[cfgPortIdx].TrcvRef;
        if (ETHSWT_STATE_ACTIVE != EthSwt_InitState)
        {
            ETHSWT_RUNTIME_ERR(ETHSWT_SID_SETPORTTXMODE, ETHSWT_INIT_NOT_COMPLETED);
        }
        else
        {
            if (ethTrcv->DrvApi->SetPhyTxMode != NULL_PTR)
            {
                retVal = ethTrcv->DrvApi->SetPhyTxMode(ethTrcv->TrcvIdx, Mode);
            }
        }
    }

    return retVal;
}
#endif /*STD_ON == ETHSWT_SET_PORT_TX_MODE_API*/

/***********************************************************************************************************************
 * Service ID[hex]:     0x36
 * Sync/Async:          Synchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     MirroredSwitchIdx: Index of the switch within the context of the Ethernet Switch Driver, where
 *                      the Ethernet switch port is located, that has to be mirrored.
 *                      PortMirrorConfigurationPtr: Pointer of the port configuration, which shall be stored in a
 *                      shadow buffer in the Ethernet switch driver.
 * Parameters(inout):   None
 * Parameters (out):    None.
 * Return value:        E_OK: the port mirror configuration for the indexed Ethernet switch port was written.
 *                      E_NOT_OK: the port mirror configuration for the indexed Ethernet switch port was not written.
 *                      (i.e. indexed ethernet switch is not available)
 *                      ETHSWT_PORT_MIRRORING_CONFIGURATION_NOT_SUPPORTED: port mirroring configuration is not
 *                      supported by Ethernet switch driver or by the Ethernet switch hardware.
 * Description:         Store the given port mirror configuration in a shadow buffer in the Ethernet switch driver for
 *                      the given MirroredSwitchIdx.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_WRITE_PORT_MIRROR_CONFIGURATION_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_WritePortMirrorConfiguration(uint8 MirroredSwitchIdx, const EthSwt_PortMirrorCfgType* PortMirrorConfigurationPtr)
{
    Std_ReturnType retVal = E_NOT_OK;
#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_WRITEPORTMIRRORCONFIGURATION, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM < MirroredSwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_WRITEPORTMIRRORCONFIGURATION, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (NULL_PTR == PortMirrorConfigurationPtr)
    {
        ETHSWT_DET(ETHSWT_SID_WRITEPORTMIRRORCONFIGURATION, ETHSWT_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        if (ETHSWT_STATE_ACTIVE != EthSwt_InitState)
        {
            ETHSWT_RUNTIME_ERR(ETHSWT_SID_WRITEPORTMIRRORCONFIGURATION, ETHSWT_INIT_NOT_COMPLETED);
        }
        else
        {
            retVal = EthSwt_WrtMirrCfgInternal(MirroredSwitchIdx, PortMirrorConfigurationPtr, FALSE);
        }
    }

    return retVal;
}
#endif /*STD_ON == ETHSWT_WRITE_PORT_MIRROR_CONFIGURATION_API*/

/***********************************************************************************************************************
 * Service ID[hex]:     0x37
 * Sync/Async:          Asynchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     MirroredSwitchIdx: Index of the switch within the context of the Ethernet Switch Driver, where
 *                      the Ethernet switch port is located, that has to be mirrored.
 *                      PortMirrorConfigurationPtr: Pointer to the memory where the port configuration shall be stored.
 * Parameters(inout):   None
 * Parameters (out):    None.
 * Return value:        E_OK: the port mirror configuration for the indexed Ethernet switch port was red successfully.
 *                      E_NOT_OK: the port mirror configuration for the indexed Ethernet switch was not red
 *                      successfully. (i.e. indexed Ethernet switch is not available)
 * Description:         Obtain the port mirror configuration of the given Ethernet switch.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_READ_PORT_MIRROR_CONFIGURATION_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_ReadPortMirrorConfiguration(uint8 MirroredSwitchIdx, EthSwt_PortMirrorCfgType* PortMirrorConfigurationPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_READPORTMIRRORCONFIGURATION, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM < MirroredSwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_READPORTMIRRORCONFIGURATION, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (NULL_PTR == PortMirrorConfigurationPtr)
    {
        ETHSWT_DET(ETHSWT_SID_READPORTMIRRORCONFIGURATION, ETHSWT_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        if (ETHSWT_STATE_ACTIVE != EthSwt_InitState)
        {
            ETHSWT_RUNTIME_ERR(ETHSWT_SID_READPORTMIRRORCONFIGURATION, ETHSWT_INIT_NOT_COMPLETED);
        }
        else
        {
            if (ETHSWT_MIRROR_NO_USE != EthSwt_MirrorShadowBuf[MirroredSwitchIdx].mirroInnerState)
            {
                *PortMirrorConfigurationPtr = *EthSwt_MirrorShadowBuf[MirroredSwitchIdx].MirrorStore.MirrorInfo;
                retVal = E_OK;
            }
        }
    }

    return retVal;
}
#endif /*STD_ON == ETHSWT_READ_PORT_MIRROR_CONFIGURATION_API*/

/***********************************************************************************************************************
 * Service ID[hex]:     0x4A
 * Sync/Async:          Synchronous
 * Reentrancy:          Reentrant for different MirroredSwitchIdx. Non reentrant for the same SwitchIdx.
 * Parameters (in):     MirroredSwitchIdx: Index of the switch within the context of the Ethernet Switch Driver.
 * Parameters(inout):   None
 * Parameters (out):    None.
 * Return value:        E_OK: Port mirror configuration was deleted successfully.
 *                      E_NOT_OK: Port mirror configuration was not deleted successfully. (e.g. the port mirroring
 *                      is enabled).
 * Description:         Delete the stored port mirror configuration of the given MirroredSwitchIdx. If no port mirror
                        configuration was found for the given MirroredSwitchIdx, the return value shall be E_OK.
***********************************************************************************************************************/
#if (STD_ON == ETHSWT_DELETE_PORT_MIRROR_CONFIGURATION_API)
FUNC(Std_ReturnType, ETHSWT_CODE) EthSwt_DeletePortMirrorConfiguration(uint8 MirroredSwitchIdx)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_DELETEPORTMIRRORCONFIGURATION, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM < MirroredSwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_DELETEPORTMIRRORCONFIGURATION, ETHSWT_E_INV_SWITCH_IDX);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        if (ETHSWT_STATE_ACTIVE != EthSwt_InitState)
        {
            ETHSWT_RUNTIME_ERR(ETHSWT_SID_DELETEPORTMIRRORCONFIGURATION, ETHSWT_INIT_NOT_COMPLETED);
        }
        else
        {
            if (PORT_MIRROR_DISABLED == EthSwt_MirrorShadowBuf[MirroredSwitchIdx].MirrorStore.mirroState)
            {
                EthSwt_MirrorShadowBuf[MirroredSwitchIdx].mirroInnerState = ETHSWT_MIRROR_TOBE_DEL;
                retVal = E_OK;
            }
        }
    }

    return retVal;
}
#endif /*STD_ON == ETHSWT_DELETE_PORT_MIRROR_CONFIGURATION_API*/

/***********************************************************************************************************************
 * Service ID[hex]:     0x38
 * Sync/Async:          Synchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     SwitchIdx: Index of the switch within the context of the Ethernet Switch Driver
 *                      SwitchPortIdx: Index of the port at the addressed switch
 * Parameters(inout):   None
 * Parameters (out):    PortMirrorStatePtr: Pointer to the memory where the port mirroring state
 *                      (either PORT_MIRRORING_ENABLED or PORT_MIRRORING_ DISABLED)of the given Ethernet switch port
 *                      shall be stored.
 * Return value:        E_OK: the port mirroring state for the indexed Ethernet switch port returned successfully.
 *                      E_NOT_OK: the port mirror configuration for the indexed Ethernet switch returned not
 *                      successfully. (i.e. indexed ethernet switch port is not available)
 * Description:         Obtain the current status of the port mirroring for the indexed Ethernet switch port.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_GET_PORT_MIRROR_STATE_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_GetPortMirrorState(uint8 SwitchIdx, uint8 PortIdx, EthSwt_PortMirrorStateType* PortMirrorStatePtr)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_GETPORTMIRRORSTATE, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM <= SwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_GETPORTMIRRORSTATE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (0u == (EthSwt_PbCfgPtr[SwitchIdx].SwtPortMask & ((uint16)0x01u << (uint16)PortIdx)))
    {
        ETHSWT_DET(ETHSWT_SID_GETPORTMIRRORSTATE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else if (NULL_PTR == PortMirrorStatePtr)
    {
        ETHSWT_DET(ETHSWT_SID_GETPORTMIRRORSTATE, ETHSWT_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        if (ETHSWT_STATE_ACTIVE != EthSwt_InitState)
        {
            ETHSWT_RUNTIME_ERR(ETHSWT_SID_GETPORTMIRRORSTATE, ETHSWT_INIT_NOT_COMPLETED);
        }
        else
        {
            *PortMirrorStatePtr = EthSwt_MirrorShadowBuf[SwitchIdx].MirrorStore.mirroState;
            retVal = E_OK;
        }
    }
    return retVal;
}
#endif /*STD_ON == ETHSWT_GET_PORT_MIRROR_STATE_API*/

/***********************************************************************************************************************
 * Service ID[hex]:     0x39
 * Sync/Async:          Synchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     MirroredSwitchIdx: Index of the Ethernet switch within the context of the Ethernet Switch
 *                      Driver, where the port mirroring configuration is located that has to be enabled and
 *                      disabled, repectively.
 *                      PortMirrorState: Contain the requested port mirroring state either
 *                      PORT_MIRRORING_ENABLED or PORT_MIRRORING_DISABLED
 * Parameters(inout):   None
 * Parameters (out):    None.
 * Return value:        E_OK: the requested port mirroring state for the indexed Ethernet switch port was set
 *                      successfully.
 *                      E_NOT_OK: the requested port mirroring state for the indexed Ethernet switch was not set
 *                      successfully. (i.e. indexed Ethernet switch is not available, no port mirrior configuration
 *                      is available)
 * Description:         Request to set the given port mirroring state of the port mirror configuration for the given
 *                      Ethernet switch.
 ***********************************************************************************************************************/
#if (STD_ON == ETHSWT_SET_PORT_MIRROR_STATE_API)
FUNC(Std_ReturnType, ETHSWT_CODE)
EthSwt_SetPortMirrorState(uint8 MirroredSwitchIdx, EthSwt_PortMirrorStateType PortMirrorState)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_SETPORTMIRRORSTATE, ETHSWT_E_UNINIT);
    }
    else if (ETHSWT_DEVICE_NUM < MirroredSwitchIdx)
    {
        ETHSWT_DET(ETHSWT_SID_SETPORTMIRRORSTATE, ETHSWT_E_INV_SWITCH_IDX);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        if (ETHSWT_STATE_ACTIVE != EthSwt_InitState)
        {
            ETHSWT_RUNTIME_ERR(ETHSWT_SID_SETPORTMIRRORSTATE, ETHSWT_INIT_NOT_COMPLETED);
        }
        else
        {
            EthSwt_MirrorShadowBuf[MirroredSwitchIdx].MirrorStore.mirroState = PortMirrorState;
            if (PORT_MIRROR_ENABLED == PortMirrorState)
            {
                EthSwt_MirrorShadowBuf[MirroredSwitchIdx].mirroInnerState = ETHSWT_MIRROR_IN_USE;
            }
            retVal = E_OK;
        }
    }
    return retVal;
}
#endif /*STD_ON == ETHSWT_SET_PORT_MIRROR_STATE_API*/

/***********************************************************************************************************************
 * Service ID[hex]:     0x1c
 * Sync/Async:          Synchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     None
 * Parameters(inout):   None
 * Parameters (out):    None.
 * Return value:        None
 * Description:         Service to support asynchronous behavior of API calls.
 ***********************************************************************************************************************/
FUNC(void, ETHSWT_CODE) EthSwt_MainFunction(void)
{
    if (ETHSWT_STATE_INIT <= EthSwt_InitState)
    {
        /*ETHSWT_MAIN_FUNCTION_PERIOD*/
        /*SWS_EthSwt_00398*/
    }
}

/***********************************************************************************************************************
 * Service ID[hex]:     0x46
 * Sync/Async:          Synchronous
 * Reentrancy:          Non Reentrant
 * Parameters (in):     None
 * Parameters(inout):   None
 * Parameters (out):    None.
 * Return value:        None
 * Description:         Returns the version information.
 ***********************************************************************************************************************/
FUNC(void, ETHSWT_CODE) EthSwt_BackgroundTask(void)
{
#if (STD_ON == ETHSWT_DEV_ERROR_DETECT)
    if (ETHSWT_STATE_INIT > EthSwt_InitState)
    {
        ETHSWT_DET(ETHSWT_SID_BACKGROUNDTASK, ETHSWT_E_UNINIT);
    }
    else
#endif /* STD_ON == ETHSWT_DEV_ERROR_DETECT */
    {
        /*excute some initialization that take too long time*/
    }
}
#define ETHSWT_STOP_SEC_CODE
#include "EthSwt_MemMap.h"
