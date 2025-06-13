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
**                                                                            **
**  FILENAME    : Xcp_Std.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation of the XCP_Std command                       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*=======[I N C L U D E S]====================================================*/
#include "Xcp_Internal.h"
#if (XCP_ON_CAN_ENABLE == STD_ON)
#include "XcpOnCan_Cbk.h"
#endif
#include "Xcp_Interface.h"

/*=======[M A C R O S]========================================================*/
#define XCP_CONNECT_MODE_OFFSET 0x01u
#define XCP_CONNECT_MODE_NORMAL 0x00u
#define XCP_CONNECT_MODE_USER   0x01u

#if (XCP_SEED_AND_UNLOCK == STD_ON)
#define XCP_SEED_MAX_LENGTH      0x10u
#define XCP_KEY_MAX_LENGTH       0x20u
#define XCP_GET_SEED_DATA_OFFSET 0x02u
#define XCP_UNLOCK_DATA_OFFSET   0x02u
#endif /*XCP_SEED_AND_UNLOCK == STD_ON*/

/*
 * SET_REQUEST CMD
 * */

#if (XCP_SET_REQUEST == STD_ON)
#define XCP_REQ_CAL_STORE     0x01
#define XCP_REQ_DAQ_NO_RESUME 0x02
#define XCP_REQ_DAQ_RESUME    0x04
#define XCP_REQ_DAQ_CLEAR     0x08
#endif /*XCP_SET_REQUEST == STD_ON*/
/*=======[T Y P E   D E F I N I T I O N S]====================================*/
#if (XCP_ON_CAN_ENABLE == STD_ON)
#if (XCP_GET_SLAVE_ID == STD_ON)
/* Xcp On CAN Get Slave Id  status */
typedef enum
{
    XCP_GETSLAVEID_IDLE,
    XCP_GETSLAVEID_POSTIVE_STATUS
} Xcp_GetSlaveIdStatusType;
#endif
#endif
/*=======[E X T E R N A L   D A T A]==========================================*/

#if (XCP_SET_MTA == STD_ON)
#define XCP_START_SEC_VAR_INIT_UNSPECIFIED
#include "Xcp_MemMap.h"
VAR(Xcp_AddressType, XCP_VAR_INIT_UNSPECIFIED) Xcp_MTA = {0, 0};
#define XCP_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Xcp_MemMap.h"
#endif /*XCP_SET_MTA == STD_ON*/

#define XCP_START_SEC_VAR_SAVED_ZONE_16
#include "Xcp_MemMap.h"
/*Session configuration id,used for resume mode*/
VAR(uint16, XCP_VAR) Xcp_SessionCfgId;
#define XCP_STOP_SEC_VAR_SAVED_ZONE_16
#include "Xcp_MemMap.h"
/*=======[I N T E R N A L   D A T A]==========================================*/
#if (XCP_SEED_AND_UNLOCK == STD_ON)

#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
VAR(uint8, XCP_VAR_INIT_8) Xcp_SeedandKeyRes;
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
static VAR(uint8, XCP_VAR_INIT_8) Xcp_SeedandKeyLen;
static VAR(uint8, XCP_VAR_INIT_8) Xcp_SeedandKeyPos;
static VAR(uint8, XCP_VAR_INIT_8) Xcp_SeedLen;
static VAR(uint8, XCP_VAR_INIT_8) Xcp_KeyLen;
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_INIT_UNSPECIFIED
#include "Xcp_MemMap.h"
VAR(Xcp_SeedandKeyStatusType, XCP_VAR_INIT_UNSPECIFIED) Xcp_SeedandKeyStatus = XCP_PRE_SEED;
#define XCP_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
static VAR(uint8, XCP_VAR_NO_INIT_8) Xcp_SeedBuffer[XCP_SEED_MAX_LENGTH];
static VAR(uint8, XCP_VAR_NO_INIT_8) Xcp_KeyBuffer[XCP_KEY_MAX_LENGTH]; /*PRQA S 3218*/
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

#endif /*XCP_SEED_AND_UNLOCK == STD_ON*/
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/*
 * Complex Command Handler
 */
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
#if (XCP_SEED_AND_UNLOCK == STD_ON)
static FUNC(void, XCP_CODE) Xcp_CopySeedIntoFrame(void);
static FUNC(void, XCP_CODE) Xcp_GetSeedHal(void);
static FUNC(void, XCP_CODE) Xcp_KeyHandler(uint8 len);

#endif /*XCP_SEED_AND_UNLOCK == STD_ON*/

#if (XCP_UPLOAD == STD_ON)
static FUNC(void, XCP_CODE) Xcp_UploadHal(void);
#endif
#if (XCP_SHORT_UPLOAD == STD_ON)
static FUNC(void, XCP_CODE) Xcp_ShortUploadHal(void);
#endif
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

/* local */
/******************************************************************************/

/******************************************************************************/

/*
 * S T D   m a n d a t o r y    c o m m a n d
 */
/******************************************************************************/
/**
 * @brief               <Command Connect Handler>
 *
 * <This Function handle the connect command.>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
FUNC(void, XCP_CODE)
Xcp_Connect(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 commModeBasic = ((uint8)(XCP_ADDR_GRANULARITY << 0x01u));
    uint8 connectMode;

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 2u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        if (XCP_UINIT == Xcp_Status)
    {
        Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);
        Xcp_RespLength = 0x02u;
    }
    else
    {
#if (XCP_GET_COMM_MODE_INFO == STD_ON)
        commModeBasic |= 0x80u;
#endif
#if (XCP_SLAVE_BLOCK_MODE == STD_ON)
        commModeBasic |= 0x40u;
#endif
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
        commModeBasic |= 0x01u;
#endif

        connectMode = Xcp_CmdBuffer[XCP_CONNECT_MODE_OFFSET];

        if (connectMode <= XCP_CONNECT_MODE_USER)
        {
            Xcp_Status = XCP_PRE_CONNECT;
            Xcp_RespBuffer[1u] = ((uint8)(XCP_RESOURCE));
            Xcp_RespBuffer[2u] = commModeBasic;
            Xcp_RespBuffer[3u] = XCP_MAX_CTO;
            Xcp_RespBuffer[6u] = XCP_PROTOCOL_VERSIOM_MAJOR;
            Xcp_RespBuffer[7u] = XCP_CAN_TRANSPORT_LAYER_VERION_MAJOR;
            Xcp_CopyU2ToU1Buffer(XCP_MAX_DTO, &Xcp_RespBuffer[4u], CPU_BYTE_ORDER);
            Xcp_RespLength = 0x08u;
        }
        else
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            Xcp_RespLength = 0x02u;
        }
    }
    Xcp_SendResp();
    return;
}

/******************************************************************************/
/**
 * @brief               <Disconnect Handler>
 *
 * <This Function init. DAQ state and general variables and put stack state into
 *  Disconnect.>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_DisconnectHal(void)
{
#if (XCP_PAG_SUPPORT == STD_ON)
    uint8 segNum;
    uint8 maxSegNum;
#endif
/* Disable Command Proccessor */
#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
    /* Disable Daq Proccessor */
    Xcp_DAQReinit();
#endif
    Xcp_ClearStatus();

#if (XCP_PAG_SUPPORT == STD_ON)
    /* XCP send SET_CAL_PAGE,did not send the correct page, so when disconnect,
     * it should clear the state of cal page number to RAM */
    maxSegNum = Xcp_SegmentInfo.maxSegNum;
    for (segNum = 0; segNum < maxSegNum; segNum++)
    {
        Xcp_ActivPagNum[segNum] = XCP_RAM_PAGE_NUM;
    }
#endif
    Xcp_Status = XCP_DISCONNECT;
}

/******************************************************************************/
/**
 * @brief               <Command Disconnect Handler>
 *
 * <This Function handle the disconnect command.>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_Disconnect(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 1u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
    {
        Xcp_DisconnectHal();
        Xcp_RespLength = 0x01u;
    }
    Xcp_SendResp();
    return;
}

/******************************************************************************/
/*
 * @brief               <Command GetStatus Handler>
 *
 * < Get The current status > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_GetStatus(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 1u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
    {
        Xcp_RespBuffer[1u] = Xcp_SessionStatus;
        Xcp_RespBuffer[2u] = Xcp_ProtectionStatus;
        Xcp_CopyU2ToU1Buffer(Xcp_SessionCfgId, &Xcp_RespBuffer[4u], CPU_BYTE_ORDER);
        Xcp_RespLength = 0x06u;
    }
    Xcp_SendResp();
    return;
}

/******************************************************************************/
/*
 * @brief               <Command Synch Handler>
 *
 * < Get The current status > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_Synch(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 1u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
    {
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        /* PGM handler */
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
        {
            /* Do Nothing */
        }
        else
#endif
        {
            Xcp_CommandInit();
        }
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNCH);
        Xcp_RespLength = 0x02u;
    }
    Xcp_SendResp();
    return;
}

/*
 * S T D   o p t i o n a l    c o m m a n d
 */
#if (XCP_GET_COMM_MODE_INFO == STD_ON)
/******************************************************************************/
/*
 * @brief               <Command GetCommModeInfo Handler>
 *
 * < This command returns optional information on different Communication Modes
 *   supported by the slave> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_GetCommModeInfo(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 1u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
    {
        Xcp_RespBuffer[2u] = 0x00u;
        Xcp_RespBuffer[4u] = 0x00u;
        Xcp_RespBuffer[5u] = 0x00u;
        Xcp_RespBuffer[6u] = 0x00u;

#if (XCP_MASTER_BLOCK_MODE == STD_ON)
        Xcp_RespBuffer[2u] = 0x01u;
        Xcp_RespBuffer[4u] = XCP_MAX_BS;
        Xcp_RespBuffer[5u] = XCP_MIN_ST;
#elif (XCP_INTERLEAVED_MODE == STD_ON)
        Xcp_RespBuffer[2u] = 0x02u;
        Xcp_RespBuffer[6u] = XCP_QUEUE_SIZE;
#endif /*XCP_INTERLEAVED_MODE == STD_ON*/
        /* XCP Driver Version Number */
        Xcp_RespBuffer[7u] = ((uint8)((XCP_H_SW_MAJOR_VERSION & 0x0fu) << 4u)) | (XCP_H_SW_MINOR_VERSION & 0x0fu);
        Xcp_RespLength = 0x08u;
    }
    Xcp_SendResp();
    return;
}
#endif /*XCP_GET_COMM_MODE_INFO == STD_ON*/

#if (STD_ON == XCP_GET_ID)
/******************************************************************************/
/*
 * @brief               <Command GetId Handler>
 *
 * < This command is used for automatic session configuration and for slave
 *   device identification> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_GetId(void)
{
    uint8 getIdReqIdType;
    uint8 idNum;
    const Xcp_IdInfoType* localInfoCfgRef = Xcp_GlobalCfgPtr->Xcp_IdInfoPtr;
    const uint8 numOfId = Xcp_GlobalCfgPtr->Xcp_IdInfoNum;

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 2u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
    {
        /* get the mode from CommandRxObject[1] buffer */
        getIdReqIdType = Xcp_CmdBuffer[1u];

        for (idNum = 0u; idNum < numOfId; idNum++)
        {
            if (getIdReqIdType == localInfoCfgRef[idNum].type)
            {
                /* Set the MTA so that the master can upload the ID string. */
                Xcp_MTA.transferAddr = (uint32)(localInfoCfgRef[idNum].ptr);
                Xcp_MTA.extensionAddr = 0x0u;
                Xcp_RespBuffer[1u] = localInfoCfgRef[idNum].mode;
                /* Xcp_RespBuffer[2-3] are Reserved */
                /* Put the length to RespBuffer[4]-[7] buffer */
                Xcp_UploadInfoLen = ((localInfoCfgRef[idNum].length) / XCP_AG) + 1u;
                Xcp_CopyU4ToU1Buffer(localInfoCfgRef[idNum].length, &(Xcp_RespBuffer[4u]), CPU_BYTE_ORDER);
                /* Number of Data Elements UPLOAD [AG] = (Length GET_ID [BYTE]) / AG */
                Xcp_RespLength = 0x08u;
                break;
            } /* end of if (Xcp_GetId_Req_IdType == Xcp_IdInfo[i].type)*/
        }
        /* for id is not available */
        if (idNum == numOfId)
        {
            Xcp_RespBuffer[1u] = 0x0u;
            /* length = 0 */
            Xcp_RespBuffer[4u] = 0x0u;
            Xcp_RespBuffer[5u] = 0x0u;
            Xcp_RespBuffer[6u] = 0x0u;
            Xcp_RespBuffer[7u] = 0x0u;
            Xcp_RespLength = 0x08u;
        }
    }
    Xcp_SendResp();
    return;
}
#endif /*STD_ON == XCP_GET_ID*/

#if (XCP_SET_REQUEST == STD_ON)
/******************************************************************************/
/*
 * @brief               <Command SetRequest Handler>
 *
 * < This command is used to start a asychronous task for storing CAL or DAQ list>.
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_SetRequest(void)
{
#if (STD_ON == XCP_RESUME_SUPPORT)
    P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED_32) daqPtr;
    uint16 daqIdx = 0;
#endif /*STD_ON == XCP_RESUME_SUPPORT*/

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 4u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
    {
        /*
         * careful:
         * Before testing the SET_REQUEST command, you should first configure the DAQ information,
         * including (write_daq / set_daq_list_mode), and finally send start_stop_daq_list to
         * select the daq to save, and finally send the set_request (session id != 0)
         * */
        switch (Xcp_CmdBuffer[0x01])
        {
        case XCP_REQ_CAL_STORE:
            Xcp_SessionStatus |= XCP_SESSION_STATE_STORE_CAL;
            break;
        case XCP_REQ_DAQ_NO_RESUME:
            Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[2u], &Xcp_SessionCfgId, CPU_BYTE_ORDER);
            Xcp_SessionStatus |= XCP_SESSION_STATE_STORE_DAQ;
            break;
#if (STD_ON == XCP_RESUME_SUPPORT)
        case XCP_REQ_DAQ_RESUME:
            Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[2u], &Xcp_SessionCfgId, CPU_BYTE_ORDER);
            Xcp_SessionStatus |= XCP_SESSION_STATE_STORE_DAQ;
            for (daqIdx = 0; daqIdx < XCP_MAX_DAQ; daqIdx++)
            {
                daqPtr = &(Xcp_Daq[daqIdx]);
                if (XCP_DAQ_MOD_SELE == (daqPtr->mode & XCP_DAQ_MOD_SELE))
                {
                    daqPtr->mode |= XCP_DAQ_MOD_RESUME;
                    daqPtr->mode &= (~XCP_DAQ_MOD_SELE);
                }
            }
            break;
#endif /*STD_ON == XCP_RESUME_SUPPORT*/
        case XCP_REQ_DAQ_CLEAR:
            /*
             * clear daq
             * */
            Xcp_SessionStatus |= XCP_SESSION_STATE_CLEAR_DAQ;
            break;
        default:
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            Xcp_RespLength = 0x02u;
            break;
        }
    }
    Xcp_SendResp();
    return;
}

FUNC(void, XCP_CODE)
Xcp_SetRequestHandler(void)
{
    /*There are two ways to store to NVM:
     * 1.Utilize the hardware mechanism and realize it through MemMap mechanism,(SAVE_ZONE).
     * 2.Use NvmRam interface.
     * */
    if (XCP_SESSION_STATE_STORE_CAL == (Xcp_SessionStatus & XCP_SESSION_STATE_STORE_CAL))
    {
        Xcp_SessionStatus &= (~XCP_SESSION_STATE_STORE_CAL);
        Xcp_EvBuffer[XCP_PID_OFFSET] = XCP_EV_PID;
        Xcp_EvBuffer[XCP_EV_CODE_OFFEST] = XCP_EV_STORE_CAL;
        Xcp_EvLength = 2u;
        Xcp_SendEv();
    }
    else if (XCP_SESSION_STATE_STORE_DAQ == (Xcp_SessionStatus & XCP_SESSION_STATE_STORE_DAQ))
    {
        Xcp_SessionStatus &= (~XCP_SESSION_STATE_STORE_DAQ);
        Xcp_EvBuffer[XCP_PID_OFFSET] = XCP_EV_PID;
        Xcp_EvBuffer[XCP_EV_CODE_OFFEST] = XCP_EV_STORE_DAQ;
        Xcp_EvLength = 2u;
        Xcp_SendEv();
    }
    else if (XCP_SESSION_STATE_CLEAR_DAQ == (Xcp_SessionStatus & XCP_SESSION_STATE_CLEAR_DAQ))
    {
        Xcp_SessionStatus &= (~XCP_SESSION_STATE_CLEAR_DAQ);
        Xcp_EvBuffer[XCP_PID_OFFSET] = XCP_EV_PID;
        Xcp_EvBuffer[XCP_EV_CODE_OFFEST] = XCP_EV_CLEAR_DAQ;
        Xcp_EvLength = 2u;
        Xcp_SendEv();
    }
    else
    {
        /*Do nothing*/
    }
}

#endif

#if (STD_ON == XCP_SEED_AND_UNLOCK)
static FUNC(void, XCP_CODE) Xcp_CopySeedIntoFrame(void)
{
    uint8 pos;
    uint8 maxcto = XCP_MAX_CTO;
    /* copy seed into frame buffer */
    Xcp_RespBuffer[1u] = Xcp_SeedandKeyLen;
    if (Xcp_SeedandKeyLen > (maxcto - XCP_UNLOCK_DATA_OFFSET))
    {
        for (pos = XCP_GET_SEED_DATA_OFFSET; pos < maxcto; pos++)
        {
            Xcp_RespBuffer[pos] = Xcp_SeedBuffer[Xcp_SeedandKeyPos];
            Xcp_SeedandKeyPos++;
        }
        Xcp_SeedandKeyLen -= (maxcto - XCP_UNLOCK_DATA_OFFSET);
        Xcp_SeedandKeyStatus = XCP_SEED;
        Xcp_RespLength = maxcto;
    }
    else
    {
        for (pos = 0; pos < Xcp_SeedandKeyLen; pos++)
        {
            Xcp_RespBuffer[XCP_GET_SEED_DATA_OFFSET + pos] = Xcp_SeedBuffer[Xcp_SeedandKeyPos];
            Xcp_SeedandKeyPos++;
        }
        Xcp_SeedandKeyStatus = XCP_PRE_KEY;
        Xcp_RespLength = Xcp_SeedandKeyLen + XCP_UNLOCK_DATA_OFFSET;
    }
    return;
}

static FUNC(void, XCP_CODE) Xcp_GetSeedHal(void)
{
    uint8 tMode = Xcp_CmdBuffer[1u];
    uint8 resource = Xcp_CmdBuffer[2u];

    switch (tMode)
    {
    /* (first part of) seed */
    case 0x0u: {
        /* check Resource*/
        if ((0u != resource)
            && ((XCP_PL_CAL == resource) || (XCP_PL_DAQ == resource) || (XCP_PL_PGM == resource)
                || (XCP_PL_STIM == resource)))
        {
            Xcp_SeedandKeyRes = resource;
            if (resource == (Xcp_ProtectionStatus & resource))
            {
                /* call seed calculate function */
                if ((Std_ReturnType)E_OK == Xcp_CreateSeed(&Xcp_SeedLen, Xcp_SeedBuffer, resource))
                {
                    /* init Seed Buffer status*/
                    Xcp_SeedandKeyPos = 0;
                    Xcp_SeedandKeyLen = Xcp_SeedLen;
                    /* update frame buffer */
                    Xcp_CopySeedIntoFrame();
                }
                else
                {
                    Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);
                    Xcp_RespLength = 0x02u;
                }
            }
            else
            {
                /* The resource is unprotected, the seed length is 0,
                 * and no unlock command necessary. */
                Xcp_RespBuffer[1] = 0u;
                Xcp_RespLength = 0x02u;
            }
        }
        /* resource error */
        else
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            Xcp_RespLength = 0x02u;
        }
        break;
    }
    /* continued frame */
    case 0x01u: {
        if (XCP_SEED == Xcp_SeedandKeyStatus)
        {
            /* update frame buffer */
            Xcp_CopySeedIntoFrame();
        }
        /* resource consistency error
         * or sequence error
         */
        else
        {
            Xcp_SeedandKeyStatus = XCP_PRE_SEED;
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
            Xcp_RespLength = 0x02u;
        }
        break;
    }
    /* mode error */
    default: {
        Xcp_SetErrorCode(XCP_ERR_MODE_NOT_VALID);
        Xcp_RespLength = 0x02u;
        break;
    }
    }
    return;
}
/******************************************************************************/
/*
 * @brief               <Command GetSeed Handler>
 *
 * < > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_GetSeed(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{

/* length check */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 3u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
    {
        Xcp_GetSeedHal();
    }
    Xcp_SendResp();
    return;
}

/******************************************************************************/
/*
 * @brief               <Command GetSeed Handler>
 *
 * < This function copy key data from frame into buffer and check its validity > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
static FUNC(void, XCP_CODE) Xcp_KeyHandler(uint8 len)
{
    uint8 pos;
    uint8 resCto = XCP_MAX_CTO - XCP_UNLOCK_DATA_OFFSET;
    boolean result;
    Xcp_SeedVerifyType seedVerify;
    Xcp_KeyVerifyType keyVerify;

    /* key in multi frames */
    if (len > resCto)
    {
        for (pos = 0; pos < resCto; pos++)
        {
            Xcp_KeyBuffer[Xcp_SeedandKeyPos + pos] = Xcp_CmdBuffer[pos + XCP_UNLOCK_DATA_OFFSET];
        }
        Xcp_SeedandKeyPos += pos;
        Xcp_SeedandKeyLen -= resCto;
        Xcp_RespBuffer[1u] = Xcp_ProtectionStatus;
        Xcp_RespLength = 0x02;
    }
    /* key in this frame */
    else
    {
        /* copy last data frame */
        for (pos = 0; pos < len; pos++)
        {
            Xcp_KeyBuffer[Xcp_SeedandKeyPos + pos] = Xcp_CmdBuffer[pos + XCP_UNLOCK_DATA_OFFSET];
        }
        /* check validity */
        seedVerify.lengthS = Xcp_SeedLen;
        seedVerify.seed = Xcp_SeedBuffer;
        keyVerify.lengthK = Xcp_KeyLen;
        keyVerify.key = Xcp_KeyBuffer;
        result = Xcp_IsKeyRight(seedVerify, keyVerify, Xcp_SeedandKeyRes);
        /* valid key */
        if ((boolean)TRUE == result)
        {
            Xcp_ProtectionStatus &= (uint8)(~Xcp_SeedandKeyRes);
            Xcp_RespBuffer[1u] = Xcp_ProtectionStatus;
            Xcp_RespLength = 0x02;
        }
        /* unvalid key */
        else
        {
            Xcp_DisconnectHal();
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        Xcp_SeedandKeyStatus = XCP_PRE_SEED;
    }
    return;
}

/******************************************************************************/
/*
 * @brief               <Command Unlock>
 *
 * < > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_Unlock(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{

    uint8 len = Xcp_CmdBuffer[1u];
/* length check */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != (len + 2u))
#if (XCP_MAX_CTO > 8u)
        && (Xcp_CmdLength != (len + 4u))
#endif /*XCP_MAX_CTO > 8u*/
        && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
    {
        /* normal function */
        if (XCP_PRE_KEY == Xcp_SeedandKeyStatus)
        {
            if (len > XCP_KEY_MAX_LENGTH)
            {
                Xcp_SeedandKeyStatus = XCP_PRE_SEED;
                Xcp_DisconnectHal();
                Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
                Xcp_RespLength = 0x02u;
            }
            else
            {
                Xcp_KeyLen = len;
                Xcp_SeedandKeyLen = len;
                Xcp_SeedandKeyPos = 0;
                Xcp_SeedandKeyStatus = XCP_KEY;
                /* call key handler */
                Xcp_KeyHandler(len);
            }
        }
        else if (XCP_KEY == Xcp_SeedandKeyStatus)
        {
            if (len == Xcp_SeedandKeyLen)
            {
                /* call key handler */
                Xcp_KeyHandler(len);
            }
            else
            {
                Xcp_SeedandKeyStatus = XCP_PRE_SEED;
                Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
                Xcp_RespLength = 0x02u;
            }
        }
        /* err sequnce */
        else
        {
            Xcp_SeedandKeyStatus = XCP_PRE_SEED;
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
            Xcp_RespLength = 0x02u;
        }
    }
    Xcp_SendResp();
    return;
}
#endif /* XCP_SEED_AND_UNLOCK == STD_ON */

#if (STD_ON == XCP_SET_MTA)
/******************************************************************************/
/*
 * @brief               <Command SetMta>
 *
 * < > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_SetMta(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 extAddr = Xcp_CmdBuffer[3u];
    uint32 Addr = 0;
    boolean calflag;
    boolean pgmflag;

    Xcp_CopyU1BufferToU4(&Xcp_CmdBuffer[4u], &Addr, CPU_BYTE_ORDER);

#if (0 != (XCP_PL_CAL & XCP_RESOURCE))
    calflag = Xcp_CheckAddress(Xcp_Mta2Ptr(extAddr, Addr), 0, XCP_MEM_CAL_ALL);
#endif
#if (0 != (XCP_PL_PGM & XCP_RESOURCE))
    pgmflag = Xcp_CheckAddress(Xcp_Mta2Ptr(extAddr, Addr), 0, XCP_MEM_PGM);
#endif

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 8u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        if ((boolean)FALSE
#if (0 != (XCP_PL_CAL & XCP_RESOURCE))
            || ((boolean)TRUE == calflag) /* PRQA S 4558 */ /* MISRA Rule 10.1 */
#endif
#if (0 != (XCP_PL_PGM & XCP_RESOURCE))
            || ((boolean)TRUE == pgmflag)
#endif
        )
    {
        Xcp_MTA.extensionAddr = extAddr;
        Xcp_MTA.transferAddr = Addr;
#if ((XCP_GET_ID == STD_ON) || (XCP_GET_DAQ_EVENT_INFO == STD_ON) || (XCP_GET_SECTOR_INFO == STD_ON))
        Xcp_UploadInfoLen = 0x00u;
#endif
    }
#if (0 != ((XCP_PL_CAL | XCP_PL_PGM) & XCP_RESOURCE))
    else
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        Xcp_RespLength = 0x02u;
    }
#endif /*XCP_PL_CAL == XCP_PL_CAL&XCP_RESOURCE*/
    Xcp_SendResp();
    return;
}

FUNC(uint32, XCP_CODE) Xcp_Mta2Ptr(const uint8 u1AddExt, const uint32 u4Add)
{
    (void)u1AddExt;
    return u4Add;
}
#endif /*STD_ON == XCP_SET_MTA*/

#if (STD_ON == XCP_UPLOAD)
/******************************************************************************/
/*
 * @brief               <Command SetMta>
 *
 * < > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_Upload(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 2u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
    {
        if (Xcp_ProtectionStatus != 0u)
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        else if (Xcp_PgmStauts != XCP_PGM_IDLE)
        {
            Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
            Xcp_RespLength = 0x02u;
        }
#endif /*XCP_PL_PGM == XCP_PL_PGM & XCP_RESOURCE*/
        else
        {
            Xcp_UploadHal();
        }
    }
    Xcp_SendResp();
    return;
}

static FUNC(void, XCP_CODE) Xcp_UploadHal(void)
{
    uint8 len = Xcp_CmdBuffer[1u];
    const Xcp_AGType* sourPtr;
    Xcp_AGType* destPtr;
    uint8 pos;
#if (0 != (XCP_PL_CAL & XCP_RESOURCE))
    boolean calflag = Xcp_CheckAddress(
        Xcp_Mta2Ptr(Xcp_MTA.extensionAddr, Xcp_MTA.transferAddr),
        ((uint32)len * XCP_AG),
        XCP_MEM_CAL_ALL);
#if ((STD_ON == XCP_MEASUREMENT_POLLING_SUPPORT) && (XCP_POLL_MEA_BY_UP == XCP_MEASUREMENT_POLLING_BY))
    boolean pollingflag =
        Xcp_CheckAddress(Xcp_Mta2Ptr(Xcp_MTA.extensionAddr, Xcp_MTA.transferAddr), ((uint32)len * XCP_AG), XCP_MEM_MEA);
#endif /*STD_ON == XCP_MEASUREMENT_POLLING_SUPPORT) && (XCP_POLL_MEA_BY_UP == XCP_MEASUREMENT_POLLING_BY*/
#endif /*XCP_PL_CAL & XCP_RESOURCE*/
#if (0 != (XCP_PL_PGM & XCP_RESOURCE))
    boolean pgmflag =
        Xcp_CheckAddress(Xcp_Mta2Ptr(Xcp_MTA.extensionAddr, Xcp_MTA.transferAddr), ((uint32)len * XCP_AG), XCP_MEM_PGM);
#endif

    if ((0x00u == len)
#if (XCP_SLAVE_BLOCK_MODE == STD_OFF)
        || (len > XCP_UPLOAD_SIZE)
#endif
    )
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        Xcp_RespLength = 0x02u;
    }
    else if (
        (boolean)FALSE
#if (0 != (XCP_PL_CAL & XCP_RESOURCE))
        || ((boolean)TRUE == calflag) /* PRQA S 4558 */ /* MISRA Rule 10.1 */
#if ((STD_ON == XCP_MEASUREMENT_POLLING_SUPPORT) && (XCP_POLL_MEA_BY_UP == XCP_MEASUREMENT_POLLING_BY))
        || ((boolean)TRUE == pollingflag)
#endif /*STD_ON == XCP_MEASUREMENT_POLLING_SUPPORT) && (XCP_POLL_MEA_BY_UP == XCP_MEASUREMENT_POLLING_BY*/
#endif /*XCP_PL_CAL & XCP_RESOURCE*/
#if (0 != (XCP_PL_PGM & XCP_RESOURCE))
        || ((boolean)TRUE == pgmflag)
#endif /*XCP_PL_PGM & XCP_RESOURCE*/
#if ((XCP_GET_ID == STD_ON) || (XCP_GET_DAQ_EVENT_INFO == STD_ON) || (XCP_GET_SECTOR_INFO == STD_ON))
        || (len <= Xcp_UploadInfoLen)
#endif /*XCP_GET_ID == STD_ON || XCP_GET_DAQ_EVENT_INFO == STD_ON || XCP_GET_SECTOR_INFO == STD_ON*/
    )
    {
#if (XCP_SLAVE_BLOCK_MODE == STD_ON)
        if (len <= XCP_UPLOAD_SIZE)
#endif
        {
            /* get pointer from MTA */
            /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */
            sourPtr = (Xcp_AGType*)Xcp_Mta2Ptr(Xcp_MTA.extensionAddr, Xcp_MTA.transferAddr);
            /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
            destPtr = (Xcp_AGType*)(&Xcp_RespBuffer[XCP_UPLOAD_DATA_OFFSET]);
            for (pos = 0; pos < len; pos++)
            {
                destPtr[pos] = sourPtr[pos];
            }
            /* copy data into buffer */
            Xcp_UpdateMTA((uint32)len * XCP_AG);
            Xcp_RespLength = (len * XCP_AG) + XCP_UPLOAD_DATA_OFFSET;
        }
#if (XCP_SLAVE_BLOCK_MODE == STD_ON)
        else
        {
            Xcp_BlockBufferPos = 0;
            Xcp_BlockBufferLen = len;
            Xcp_CommandStatus = XCP_CMD_BLOCK_TRANSF;
        }
#endif
    }
#if (0 != ((XCP_PL_CAL | XCP_PL_PGM) & XCP_RESOURCE))
    else
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
        Xcp_RespLength = 0x02u;
    }
#endif /*XCP_PL_CAL == XCP_PL_CAL&XCP_RESOURCE*/
    return;
}
#endif /*STD_ON == XCP_UPLOAD*/

#if (STD_ON == XCP_SHORT_UPLOAD)
/******************************************************************************/
/*
 * @brief               <Command ShortUpload>
 *
 * < > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_ShortUpload(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if ((XCP_GET_ID == STD_ON) || (XCP_GET_DAQ_EVENT_INFO == STD_ON) || (XCP_GET_SECTOR_INFO == STD_ON))
    Xcp_UploadInfoLen = 0;
#endif
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if (Xcp_CmdLength != XCP_CAN_MAX_DLC)
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        if (Xcp_ProtectionStatus != 0u)
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        Xcp_RespLength = 0x02u;
    }
    else
    {
        Xcp_ShortUploadHal();
    }
    Xcp_SendResp();
    return;
}

static FUNC(void, XCP_CODE) Xcp_ShortUploadHal(void)
{
    uint8 len = Xcp_CmdBuffer[1u];
    uint8 extAddr = Xcp_CmdBuffer[3u];
    uint32 Addr = 0;
    const Xcp_AGType* sourPtr;
    Xcp_AGType* destPtr;
    uint8 pos;

    Xcp_CopyU1BufferToU4(&Xcp_CmdBuffer[4u], &Addr, CPU_BYTE_ORDER);

#if (0 != (XCP_PL_CAL & XCP_RESOURCE))
    boolean calflag = Xcp_CheckAddress(Xcp_Mta2Ptr(extAddr, Addr), ((uint32)len * XCP_AG), XCP_MEM_CAL_ALL);
#if ((STD_ON == XCP_MEASUREMENT_POLLING_SUPPORT) && (XCP_POLL_MEA_BY_SHORT_UP == XCP_MEASUREMENT_POLLING_BY))
    boolean pollingflag = Xcp_CheckAddress(Xcp_Mta2Ptr(extAddr, Addr), ((uint32)len * XCP_AG), XCP_MEM_MEA);
#endif /*STD_ON == XCP_MEASUREMENT_POLLING_SUPPORT) && (XCP_POLL_MEA_BY_SHORT_UP == XCP_MEASUREMENT_POLLING_BY*/
#endif /*XCP_PL_CAL & XCP_RESOURCE*/
#if (0 != (XCP_PL_PGM & XCP_RESOURCE))
    boolean pgmflag = Xcp_CheckAddress(Xcp_Mta2Ptr(extAddr, Addr), ((uint32)len * XCP_AG), XCP_MEM_PGM);
#endif
    if ((0x00u == len) || (len > XCP_UPLOAD_SIZE))
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        Xcp_RespLength = 0x02u;
    }
    else if (
        (boolean)FALSE
#if (0 != (XCP_PL_CAL & XCP_RESOURCE))
        || ((boolean)TRUE == calflag) /* PRQA S 4558 */ /* MISRA Rule 10.1 */
#if ((STD_ON == XCP_MEASUREMENT_POLLING_SUPPORT) && (XCP_POLL_MEA_BY_SHORT_UP == XCP_MEASUREMENT_POLLING_BY))
        || ((boolean)TRUE == pollingflag)
#endif /*STD_ON == XCP_MEASUREMENT_POLLING_SUPPORT) && (XCP_POLL_MEA_BY_SHORT_UP == XCP_MEASUREMENT_POLLING_BY*/
#endif /*XCP_PL_CAL & XCP_RESOURCE*/
#if (0 != (XCP_PL_PGM & XCP_RESOURCE))
        || ((boolean)TRUE == pgmflag)
#endif /*XCP_PL_PGM & XCP_RESOURCE*/
    )
    {
        /* Set MTA */
        Xcp_MTA.extensionAddr = extAddr;
        Xcp_MTA.transferAddr = Addr;
        /* get pointer from MTA */
        /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */
        sourPtr = (Xcp_AGType*)Xcp_Mta2Ptr(Xcp_MTA.extensionAddr, Xcp_MTA.transferAddr);
        /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
        destPtr = (Xcp_AGType*)(&Xcp_RespBuffer[XCP_UPLOAD_DATA_OFFSET]);
        /* copy data into buffer */
        for (pos = 0; pos < len; pos++)
        {
            destPtr[pos] = sourPtr[pos];
        }
        Xcp_UpdateMTA((uint32)len * XCP_AG);
        Xcp_RespLength = (len * XCP_AG) + XCP_UPLOAD_DATA_OFFSET;
    }
#if (0 != ((XCP_PL_CAL | XCP_PL_PGM) & XCP_RESOURCE))
    else
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
        Xcp_RespLength = 0x02u;
    }
#endif /*(XCP_PL_CAL | XCP_PL_PGM) & XCP_RESOURCE*/
    return;
}
#endif /*XCP_SHORT_UPLOAD == STD_ON*/

#if (XCP_BUILD_CHECKSUM == STD_ON)
/******************************************************************************/
/*
 * @brief               <Command BuildChecksum>
 *
 * < > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_BuildChecksum(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint32 crcResult;
    uint32 blockSize;
    uint32 checksumStartAddress;
    uint32 maxChecksumBlockSize = XCP_MAX_CHECKSUM_SIZE;

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if (Xcp_CmdLength != XCP_CAN_MAX_DLC)
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        if (Xcp_ProtectionStatus != 0u)
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        Xcp_RespLength = 0x02u;
    }
    else
    {
        /* get block size from CTO buffer[4-7] */
        Xcp_CopyU1BufferToU4(&Xcp_CmdBuffer[4u], &blockSize, CPU_BYTE_ORDER);
        /* get bolck start address */
        checksumStartAddress = Xcp_Mta2Ptr(Xcp_MTA.extensionAddr, Xcp_MTA.transferAddr);

        if (blockSize > maxChecksumBlockSize)
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);

            /* Put the MAX_CHECKSUM_BLOCKSIZE to RespBuffer[4]-[7] buffer */
            Xcp_CopyU4ToU1Buffer(maxChecksumBlockSize, &(Xcp_RespBuffer[4u]), CPU_BYTE_ORDER);
            Xcp_RespLength = 0x08u;
        }
#if (0u != (XCP_PL_CAL & XCP_RESOURCE))
        /* address check */
        else if ((boolean)FALSE == Xcp_CheckAddress(checksumStartAddress, blockSize, XCP_MEM_CAL_ALL))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
            Xcp_RespLength = 0x02u;
        }
#endif /*XCP_PL_CAL == XCP_PL_CAL&XCP_RESOURCE*/
        else
        {
            /* set checksum type */
            Xcp_RespBuffer[1u] = XCP_CHECKSUM_TYPE_CRC_16_CITT;
            /* call user interface  Ccp_ChecksumCompute to set the crcResult and retrun the size of cheksum*/
            Xcp_ChecksumCompute(checksumStartAddress, (blockSize * XCP_AG), &crcResult);
            Xcp_CopyU4ToU1Buffer(crcResult, &(Xcp_RespBuffer[4u]), CPU_BYTE_ORDER);
            Xcp_UpdateMTA(blockSize * XCP_AG);
            Xcp_RespLength = 0x08u;
        }
    }
    Xcp_SendResp();
    return;
}
#endif /*XCP_BUILD_CHECKSUM == STD_ON*/

/* Transport Layer Command */
#if (XCP_GET_SLAVE_ID == STD_ON)
/******************************************************************************/
/*
 * @brief               <Command GetSlaveId>
 *
 * < > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_GetSlaveId(void)
{
    uint8 getSlaveMode;
    uint32 canIdForStimCmd;
    static Xcp_GetSlaveIdStatusType Xcp_GetSlaveIdStatus = XCP_GETSLAVEID_IDLE;

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 0x06u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
    {
        getSlaveMode = Xcp_CmdBuffer[5u];

        if ((0x58u == Xcp_CmdBuffer[2u])     /**< 'X'*/
            && (0x43u == Xcp_CmdBuffer[3u])  /**< 'C'*/
            && (0x50u == Xcp_CmdBuffer[4u])) /**< 'P'*/
        {
            switch (getSlaveMode)
            {
            /* identify by echo */
            case 0u: {
                Xcp_RespBuffer[1u] = 0x58u; /**< 'X'*/
                Xcp_RespBuffer[2u] = 0x43u; /**< 'C'*/
                Xcp_RespBuffer[3u] = 0x50u; /**< 'P'*/

                /* Get the Canid Used for STIM and CMD from Configuration */
                /*Not Support.*/
                canIdForStimCmd = 0;

                /* Put the canid to RespBuffer[4]-[7] buffer in INTEL format */
                Xcp_CopyU4ToU1Buffer(canIdForStimCmd, &(Xcp_RespBuffer[4u]), LOW_BYTE_FIRST);

                /* response length is 8 */
                Xcp_RespLength = 0x08u;

                /* Set The status to XCP_GETSLAVEID_POSTIVE_STATUS */
                Xcp_GetSlaveIdStatus = XCP_GETSLAVEID_POSTIVE_STATUS;
                break;
            }
            /* confirm by inverse echo */
            case 1u: {
                if (XCP_GETSLAVEID_POSTIVE_STATUS == Xcp_GetSlaveIdStatus)
                {
                    Xcp_RespBuffer[1u] = 0xA7u; /**< '~X'*/
                    Xcp_RespBuffer[2u] = 0xBCu; /**< '~C'*/
                    Xcp_RespBuffer[3u] = 0xAFu; /**< '~P'*/

                    /* Get the Canid Used for STIM and CMD from Configuration */
                    /*Not Support.*/
                    canIdForStimCmd = 0;

                    /* Put the canid to RespBuffer[4]-[7] buffer in INTEL format */
                    Xcp_CopyU4ToU1Buffer(canIdForStimCmd, &Xcp_RespBuffer[4u], LOW_BYTE_FIRST);

                    /* response length is 8 */
                    Xcp_RespLength = 0x08u;

                    /* Set The status back to XCP_GETSLAVEID_IDLE */
                    Xcp_GetSlaveIdStatus = XCP_GETSLAVEID_IDLE;
                }
                else
                {
                    /* without a previous GET_SLAVE_ID(identify by echo),
                     * the slaves will silently ignore the command.
                     */
                    Xcp_CommandStatus = XCP_CMD_IDLE;
                }
                break;
            }
            /* mode error */
            default: {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
                Xcp_RespLength = 0x02u;
                break;
            }
            }
        }
        else
        {
            /* command value not correct */
            Xcp_CommandStatus = XCP_CMD_IDLE;
        }
    }
    if ((Xcp_CommandStatusType)XCP_CMD_IDLE != Xcp_CommandStatus)
    {
        Xcp_SendResp();
    }
    return;
}
#endif /* XCP_GET_SLAVE_ID == STD_ON */

#if (0 != ((XCP_PL_CAL | XCP_PL_PGM) & XCP_RESOURCE))
#if ((STD_ON == XCP_MEASUREMENT_POLLING_SUPPORT) && (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE)))
FUNC(boolean, XCP_CODE)
Xcp_CheckMeaMem(uint32 addr, uint32 size)
{
    boolean result = FALSE;
    uint32 start;
    uint32 end;

    start = Xcp_MeaStartAddr;
    end = Xcp_MeaStartAddr + Xcp_MeaLen - 1u;
    if (Xcp_CheckMemVaild(addr, size, start, end))
    {
        result = TRUE;
    }
    return result;
}
#endif

FUNC(boolean, XCP_CODE)
Xcp_CheckAddress(uint32 addr, uint32 size, Xcp_MemAddrType type)
{
    boolean result = FALSE;

    switch (type)
    {
#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
    case XCP_MEM_CAL_ALL: {
        result = Xcp_CheckCalMem(addr, size, XCP_MEM_CAL_ALL);
        break;
    }
    case XCP_MEM_CAL_ROM: {
        result = Xcp_CheckCalMem(addr, size, XCP_MEM_CAL_ROM);
        break;
    }
    case XCP_MEM_CAL_RAM: {
        result = Xcp_CheckCalMem(addr, size, XCP_MEM_CAL_RAM);
        break;
    }
#if (STD_ON == XCP_MEASUREMENT_POLLING_SUPPORT)
    case XCP_MEM_MEA: {
        /*check measurement addr is valid in polling mode*/
        result = Xcp_CheckMeaMem(addr, size);
        break;
    }
#endif /*STD_ON == XCP_MEASUREMENT_POLLING_SUPPORT*/
#endif /*XCP_PL_CAL == XCP_PL_CAL&XCP_RESOURCE*/

#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
    case XCP_MEM_PGM: {
        result = Xcp_CheckPgmMem(addr, size);
        break;
    }
#endif /*XCP_PL_PGM == XCP_PL_PGM & XCP_RESOURCE*/
    default:
        /*Do nothing.*/
        break;
    }
    return result;
}
#endif /*XCP_PL_CAL | XCP_PL_PGM & XCP_RESOURCE*/

#if (STD_ON == XCP_SET_MTA)
/******************************************************************************/
/*
 * @brief               <update MTA>
 *
 * < > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE)
Xcp_UpdateMTA(uint32 u4Length)
{
    /* updata the MTA */
    Xcp_MTA.transferAddr += u4Length;
#if ((XCP_GET_ID == STD_ON) || (XCP_GET_DAQ_EVENT_INFO == STD_ON) || (XCP_GET_SECTOR_INFO == STD_ON))
    if (0x00u != Xcp_UploadInfoLen)
    {
        if (Xcp_UploadInfoLen >= (u4Length / XCP_AG))
        {
            Xcp_UploadInfoLen -= (u4Length / XCP_AG);
        }
        else
        {
            Xcp_UploadInfoLen = 0;
        }
    }
#endif
    return;
}
#endif /*STD_ON == XCP_SET_MTA*/
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"
