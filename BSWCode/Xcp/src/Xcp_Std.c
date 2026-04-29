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
 **  @file               : Xcp_Std.c
 **  @author             : qinchun.yang
 **  @date               : 2024/11/17
 **  @vendor             : isoft
 **  @description        : Implementation of the XCP_Std command
 **
 ***********************************************************************************************************************/
/* =================================================== inclusions =================================================== */
#include "Xcp_Internal.h"
#if (XCP_ON_CAN_ENABLE == STD_ON)
#include "XcpOnCan_Cbk.h"
#endif

/* PRQA S 6070 EOF */ /* VL_MTR_Xcp_STCAL */

/* ===================================================== macros ===================================================== */
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
#define XCP_REQ_CAL_STORE     0x01u
#define XCP_REQ_DAQ_NO_RESUME 0x02u
#define XCP_REQ_DAQ_RESUME    0x04u
#define XCP_REQ_DAQ_CLEAR     0x08u
#endif /*XCP_SET_REQUEST == STD_ON*/

/* ================================================ type definitions ================================================ */
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

/* ========================================== internal function declarations ======================================== */
/*
 * Complex Command Handler
 */
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
#if (XCP_SEED_AND_UNLOCK == STD_ON)
XCP_LOCAL void Xcp_CopySeedIntoFrame(void);
XCP_LOCAL void Xcp_GetSeedHal(void);
XCP_LOCAL void Xcp_KeyHandler(uint8 len, uint8* normalExitPtr);

#endif /*XCP_SEED_AND_UNLOCK == STD_ON*/

#if (XCP_UPLOAD == STD_ON)
XCP_LOCAL void Xcp_UploadHal(void);
#endif
#if (XCP_SHORT_UPLOAD == STD_ON)
XCP_LOCAL void Xcp_ShortUploadHal(void);
#endif
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

/* ============================================ internal data definitions =========================================== */
#if (XCP_SEED_AND_UNLOCK == STD_ON)
#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
XCP_LOCAL uint8 Xcp_SeedBuffer[XCP_SEED_MAX_LENGTH];
XCP_LOCAL uint8 Xcp_KeyBuffer[XCP_KEY_MAX_LENGTH]; /*PRQA S 3218*/ /* VL_Xcp_3218 */
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

#endif /*XCP_SEED_AND_UNLOCK == STD_ON*/

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
/*
 * S T D   m a n d a t o r y    c o m m a n d
 */
/**
 * Establishes a connection to the XCP slave. This function checks the command
 * length and module initialization status. It sets the communication mode and
 * connection status, and prepares the response buffer with relevant information.
 * If any checks fail, it sets an appropriate error code and sends a response.
 * This function is intended for internal use.
 */
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Xcp_Connect(void)
{
    uint8 commModeBasic = ((uint8)(XCP_ADDR_GRANULARITY << 0x01u));
    uint8 connectMode;

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_2) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
        if (!Xcp_ModuleInit)
    {
        Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);
    }
    else
    {
#if (XCP_GET_COMM_MODE_INFO == STD_ON)
        commModeBasic |= XCP_CONST_128;
#endif
#if (XCP_SLAVE_BLOCK_MODE == STD_ON)
        commModeBasic |= XCP_CONST_64;
#endif
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
        commModeBasic |= 0x01u;
#endif

        connectMode = Xcp_CmdBuffer[XCP_CONNECT_MODE_OFFSET];

        if (connectMode <= XCP_CONNECT_MODE_USER)
        {
            Xcp_ChannelCommonData.ConnectionStatus = XCP_PRE_CONNECT;
            Xcp_RespBuffer[1u]                     = ((uint8)(XCP_RESOURCE));
            Xcp_RespBuffer[XCP_CONST_2]            = commModeBasic;
            Xcp_RespBuffer[XCP_CONST_3]            = XCP_MAX_CTO;
            Xcp_RespBuffer[XCP_CONST_6]            = XCP_PROTOCOL_VERSIOM_MAJOR;
            Xcp_RespBuffer[XCP_CONST_7]            = XCP_CAN_TRANSPORT_LAYER_VERION_MAJOR;
            Xcp_CopyU2ToU1Buffer(XCP_MAX_DTO, &Xcp_RespBuffer[XCP_CONST_4], (uint8)CPU_BYTE_ORDER);
            Xcp_ChannelCommonData.RespLength = XCP_CONST_8;
        }
        else
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        }
    }
    Xcp_SendResp();
    return;
}

/**
 * Disconnects from the XCP slave. This function disables the command processor
 * and, if applicable, the DAQ processor. It clears the status and sets the
 * connection status to disconnected. This function is intended for internal use.
 */
void Xcp_DisconnectHal(void)
{
/* Disable Command Proccessor */
#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
    /* Disable Daq Proccessor */
    Xcp_DAQReinit();
#endif
    Xcp_ClearStatus();

    Xcp_ChannelCommonData.ConnectionStatus = XCP_DISCONNECT;
}

/**
 * Disconnects from the XCP slave. This function checks the command length and
 * programming status. If the programming is active, it sets an appropriate error
 * code. Otherwise, it calls the `Xcp_DisconnectHal` function to perform the
 * disconnection and prepares the response. This function is intended for internal
 * use.
 */
void Xcp_Disconnect(void)
{
#if (XCP_ON_ETHERNET_ENABLE == STD_ON)
    uint8 normalExit = 1u;
#endif /*XCP_ON_ETHERNET_ENABLE == STD_ON*/
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != 1u) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
    {
        Xcp_DisconnectHal();
        Xcp_ChannelCommonData.RespLength = 0x01u;
#if (XCP_ON_ETHERNET_ENABLE == STD_ON)
        normalExit = 0u;
#endif /*XCP_ON_ETHERNET_ENABLE == STD_ON*/
    }
    Xcp_SendResp();
#if (XCP_ON_ETHERNET_ENABLE == STD_ON)
    /* First send the counter, and then clear the count value */
    if (0u == normalExit)
    {
        Xcp_ChannelCommonData.EthRxCounter = 0u;
        Xcp_ChannelCommonData.EthTxCounter = 0u;
    }
#endif /*XCP_ON_ETHERNET_ENABLE == STD_ON*/
    return;
}

/**
 * Retrieves the current status of the XCP session. This function checks the
 * command length and prepares the response buffer with the session status,
 * protection status, and configuration ID. If the command length is incorrect,
 * it sets an appropriate error code and sends a response. This function is
 * intended for internal use.
 */
void Xcp_GetStatus(void)
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != 1u) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
    {
        Xcp_RespBuffer[XCP_CONST_1] = Xcp_ChannelCommonData.SessionStatus;
        Xcp_RespBuffer[XCP_CONST_2] = Xcp_ChannelCommonData.ProtectionStatus;
#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
        Xcp_CopyU2ToU1Buffer(Xcp_ChannelDaqData.SessionCfgId, &Xcp_RespBuffer[XCP_CONST_4], (uint8)CPU_BYTE_ORDER);
#endif
        Xcp_ChannelCommonData.RespLength = XCP_CONST_6;
    }
    Xcp_SendResp();
    return;
}

/**
 * Synchronizes the XCP session. This function checks the command length and
 * the programming status. If the programming is active, it does nothing. Otherwise,
 * it initializes the command processor and sets a synchronization error code.
 * This function is intended for internal use.
 */
void Xcp_Synch(void)
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != 1u) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
    {
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        /* PGM handler */
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
        {
            /* Do Nothing */
        }
        else
#endif
        {
            Xcp_CommandInit();
        }
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNCH);
    }
    Xcp_SendResp();
    return;
}

/*
 * S T D   o p t i o n a l    c o m m a n d
 */
#if (XCP_GET_COMM_MODE_INFO == STD_ON)
/**
 * Retrieves communication mode information. This function checks the command
 * length and prepares the response buffer with details about the communication
 * mode, block size, minimum separation time, and queue size. It also includes
 * the XCP driver version number. If the command length is incorrect, it sets
 * an appropriate error code and sends a response. This function is intended for
 * internal use.
 */
void Xcp_GetCommModeInfo(void)
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != 1u) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
    {
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
        Xcp_RespBuffer[XCP_CONST_2] = 0x01u;
        Xcp_RespBuffer[XCP_CONST_4] = XCP_MAX_BS;
        Xcp_RespBuffer[XCP_CONST_5] = XCP_MIN_ST;
        Xcp_RespBuffer[XCP_CONST_6] = 0x00u;
#elif (XCP_INTERLEAVED_MODE == STD_ON)
        Xcp_RespBuffer[XCP_CONST_2] = XCP_CONST_2;
        Xcp_RespBuffer[XCP_CONST_4] = 0x00u;
        Xcp_RespBuffer[XCP_CONST_5] = 0x00u;
        Xcp_RespBuffer[XCP_CONST_6] = XCP_QUEUE_SIZE;
#else /* XCP_INTERLEAVED_MODE == STD_ON */
        Xcp_RespBuffer[XCP_CONST_2] = 0x00u;
        Xcp_RespBuffer[XCP_CONST_4] = 0x00u;
        Xcp_RespBuffer[XCP_CONST_5] = 0x00u;
        Xcp_RespBuffer[XCP_CONST_6] = 0x00u;
#endif
        /* XCP Driver Version Number */
        Xcp_RespBuffer[XCP_CONST_7] =
            ((uint8)((XCP_H_SW_MAJOR_VERSION & XCP_U8_MAX) << XCP_CONST_4)) | (XCP_H_SW_MINOR_VERSION & XCP_U8_MAX);
        Xcp_ChannelCommonData.RespLength = XCP_CONST_8;
    }
    Xcp_SendResp();
    return;
}
#endif /*XCP_GET_COMM_MODE_INFO == STD_ON*/

#if (STD_ON == XCP_GET_ID)
/**
 * Retrieves identification information. This function checks the command length
 * and the requested ID type. It searches for the matching ID in the configuration
 * and sets the memory transfer address (MTA) to the ID string. The response
 * buffer is prepared with the ID mode and length. If the requested ID is not
 * found or the command length is incorrect, it sets appropriate values in the
 * response buffer and sends a response. This function is intended for internal
 * use.
 */
void Xcp_GetId(void)
{
    uint8                 getIdReqIdType;
    uint8                 idNum;
    const Xcp_IdInfoType* localInfoCfgRef = Xcp_PConfig.Xcp_IdInfoPtr;
    const uint8           numOfId         = Xcp_PConfig.Xcp_IdInfoNum;

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_2) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
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
                /* PRQA S 0306,0309 ++ */ /* VL_Xcp_0306,VL_Xcp_0309 */
                Xcp_ChannelCommonData.MTA = (uint32)(localInfoCfgRef[idNum].ptr);
                /* PRQA S 0306,0309 -- */
                Xcp_ChannelCommonData.MTAExtension = 0x0u;
                Xcp_RespBuffer[1u]                 = localInfoCfgRef[idNum].mode;
                /* Xcp_RespBuffer[2-3] are Reserved */
                /* Put the length to RespBuffer[4]-[7] buffer */
                Xcp_CopyU4ToU1Buffer(
                    localInfoCfgRef[idNum].length,
                    &(Xcp_RespBuffer[XCP_CONST_4]),
                    (uint8)CPU_BYTE_ORDER);
                /* Number of Data Elements UPLOAD [AG] = (Length GET_ID [BYTE]) / AG */
                Xcp_ChannelCommonData.RespLength = XCP_CONST_8;
                break;
            } /* end of if (Xcp_GetId_Req_IdType == Xcp_IdInfo[i].type)*/
        }
        /* for id is not available */
        if (idNum == numOfId)
        {
            Xcp_RespBuffer[1u] = 0x0u;
            /* length = 0 */
            Xcp_RespBuffer[XCP_CONST_4]      = 0x0u;
            Xcp_RespBuffer[XCP_CONST_5]      = 0x0u;
            Xcp_RespBuffer[XCP_CONST_6]      = 0x0u;
            Xcp_RespBuffer[XCP_CONST_7]      = 0x0u;
            Xcp_ChannelCommonData.RespLength = XCP_CONST_8;
        }
    }
    Xcp_SendResp();
    return;
}
#endif /*STD_ON == XCP_GET_ID*/

#if (XCP_SET_REQUEST == STD_ON)
/**
 * Sets a request for calibration storage or DAQ list management. This function
 * checks the command length and processes the request based on the command
 * buffer. It updates the session status and, if applicable, the DAQ list mode.
 * If the command length is incorrect or the request is out of range, it sets
 * an appropriate error code and sends a response. This function is intended for
 * internal use.
 */
void Xcp_SetRequest(void)
{
#if (STD_ON == XCP_RESUME_SUPPORT)
    Xcp_DaqType* daqPtr;
    uint16       daqIdx = 0u;
#endif /*STD_ON == XCP_RESUME_SUPPORT*/

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_4) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
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
        switch (Xcp_CmdBuffer[0x01u])
        {
        case XCP_REQ_CAL_STORE:
            Xcp_ChannelCommonData.SessionStatus |= XCP_SESSION_STATE_STORE_CAL;
            break;
#if (XCP_PL_DAQ == (XCP_PL_DAQ & XCP_RESOURCE))
        case XCP_REQ_DAQ_NO_RESUME:
            Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[XCP_CONST_2], &Xcp_ChannelDaqData.SessionCfgId, (uint8)CPU_BYTE_ORDER);
            Xcp_ChannelCommonData.SessionStatus |= XCP_SESSION_STATE_STORE_DAQ;
            break;
#if (STD_ON == XCP_RESUME_SUPPORT)
        case XCP_REQ_DAQ_RESUME:
            Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[XCP_CONST_2], &Xcp_ChannelDaqData.SessionCfgId, (uint8)CPU_BYTE_ORDER);
            Xcp_ChannelCommonData.SessionStatus |= XCP_SESSION_STATE_STORE_DAQ;
            Xcp_ChannelCommonData.SessionStatus |= XCP_SESSION_STATE_RESUME;
            for (daqIdx = 0u; daqIdx < XCP_MAX_DAQ; daqIdx++)
            {
                daqPtr = &((Xcp_ChannelDaqData.DaqInfo)[daqIdx]);
                if (XCP_DAQ_MOD_SELE == (daqPtr->mode & XCP_DAQ_MOD_SELE))
                {
                    daqPtr->mode |= XCP_DAQ_MOD_RESUME;
                    daqPtr->mode &= (uint8)(~XCP_DAQ_MOD_SELE);
                }
            }
            break;
#endif /*STD_ON == XCP_RESUME_SUPPORT*/
        case XCP_REQ_DAQ_CLEAR:
            /*
             * clear daq
             * */
            Xcp_ChannelCommonData.SessionStatus |= XCP_SESSION_STATE_CLEAR_DAQ;
            break;
#endif
        default:
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);

            break;
        }
    }
    Xcp_SendResp();
    return;
}
#endif

#if (STD_ON == XCP_SEED_AND_UNLOCK)
/**
 * Copies the seed into the response frame buffer. This function checks the
 * seed length and copies the seed data into the response buffer. If the seed
 * length exceeds the maximum CTO (Communication Transfer Object) size, it
 * partially copies the seed and sets the key run status to indicate more data
 * is pending. Otherwise, it copies the entire seed and updates the key run
 * status accordingly. This function is intended for internal use.
 */
XCP_LOCAL void Xcp_CopySeedIntoFrame(void)
{
    uint8 pos;
    uint8 maxcto = XCP_MAX_CTO;
    /* copy seed into frame buffer */
    Xcp_RespBuffer[1u] = Xcp_ChannelCommonData.SeedandKeyLen;
    if (Xcp_ChannelCommonData.SeedandKeyLen > (maxcto - XCP_UNLOCK_DATA_OFFSET))
    {
        for (pos = XCP_GET_SEED_DATA_OFFSET; pos < maxcto; pos++)
        {
            Xcp_RespBuffer[pos] = Xcp_SeedBuffer[Xcp_ChannelCommonData.SeedandKeyPos];
            Xcp_ChannelCommonData.SeedandKeyPos++;
        }
        Xcp_ChannelCommonData.SeedandKeyLen -= (maxcto - XCP_UNLOCK_DATA_OFFSET);
        Xcp_ChannelCommonData.KeyRunStatus = XCP_SEED;
        Xcp_ChannelCommonData.RespLength   = maxcto;
    }
    else
    {
        for (pos = 0u; pos < Xcp_ChannelCommonData.SeedandKeyLen; pos++)
        {
            Xcp_RespBuffer[XCP_GET_SEED_DATA_OFFSET + pos] = Xcp_SeedBuffer[Xcp_ChannelCommonData.SeedandKeyPos];
            Xcp_ChannelCommonData.SeedandKeyPos++;
        }
        Xcp_ChannelCommonData.KeyRunStatus = XCP_PRE_KEY;
        Xcp_ChannelCommonData.RespLength   = Xcp_ChannelCommonData.SeedandKeyLen + XCP_UNLOCK_DATA_OFFSET;
    }
    return;
}

/**
 * Generates and retrieves a seed for unlocking protected resources. This function
 * checks the mode and resource type, and either generates a new seed or continues
 * sending a previously generated seed. If the resource is unprotected, it sets
 * the seed length to zero. If any checks fail, it sets an appropriate error code.
 * This function is intended for internal use.
 */
XCP_LOCAL void Xcp_GetSeedHal(void)
{
    uint8 tMode    = Xcp_CmdBuffer[XCP_CONST_1];
    uint8 resource = Xcp_CmdBuffer[XCP_CONST_2];

    switch (tMode)
    {
    /* (first part of) seed */
    case 0x0u:
    {
        /* check Resource*/
        if ((0u != resource)
            && ((XCP_PL_CAL == resource) || (XCP_PL_DAQ == resource) || (XCP_PL_PGM == resource)
                || (XCP_PL_STIM == resource)))
        {
            Xcp_ChannelCommonData.SeedandKeyRes = resource;
            if (resource == (Xcp_ChannelCommonData.ProtectionStatus & resource))
            {
                /* call seed calculate function */
                if ((Std_ReturnType)E_OK == Xcp_CreateSeed(&Xcp_ChannelCommonData.SeedLen, Xcp_SeedBuffer, resource))
                {
                    /* init Seed Buffer status*/
                    Xcp_ChannelCommonData.SeedandKeyPos = 0u;
                    Xcp_ChannelCommonData.SeedandKeyLen = Xcp_ChannelCommonData.SeedLen;
                    /* update frame buffer */
                    Xcp_CopySeedIntoFrame();
                }
                else
                {
                    Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);
                }
            }
            else
            {
                /* The resource is unprotected, the seed length is 0,
                 * and no unlock command necessary. */
                Xcp_RespBuffer[1]                = 0u;
                Xcp_ChannelCommonData.RespLength = XCP_CONST_2;
            }
        }
        /* resource error */
        else
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        }
        break;
    }
    /* continued frame */
    case 0x01u:
    {
        if (XCP_SEED == Xcp_ChannelCommonData.KeyRunStatus)
        {
            /* update frame buffer */
            Xcp_CopySeedIntoFrame();
        }
        /* resource consistency error
         * or sequence error
         */
        else
        {
            Xcp_ChannelCommonData.KeyRunStatus = XCP_PRE_SEED;
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        }
        break;
    }
    /* mode error */
    default:
    {
        Xcp_SetErrorCode(XCP_ERR_MODE_NOT_VALID);

        break;
    }
    }
    return;
}
/**
 * Retrieves a seed for unlocking protected resources. This function checks the
 * command length and programming status. If the programming is active or the
 * command length is incorrect, it sets an appropriate error code. Otherwise,
 * it calls the `Xcp_GetSeedHal` function to generate and retrieve the seed.
 * This function is intended for internal use.
 */
void Xcp_GetSeed(void)
{
/* length check */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_3) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
    {
        Xcp_GetSeedHal();
    }
    Xcp_SendResp();
    return;
}

/**
 * Handles the key verification process for unlocking protected resources. This
 * function checks whether the key is provided in multiple frames or a single
 * frame. It verifies the key against the seed and updates the protection status
 * if the key is valid. If the key is invalid, it disconnects and sets an access
 * locked error code. This function is intended for internal use.
 */
XCP_LOCAL void Xcp_KeyHandler(uint8 len, uint8* normalExitPtr)
{
    uint8              pos;
    uint8              resCto = XCP_MAX_CTO - XCP_UNLOCK_DATA_OFFSET;
    boolean            result;
    Xcp_SeedVerifyType seedVerify;
    Xcp_KeyVerifyType  keyVerify;

    /* key in multi frames */
    if (len > resCto)
    {
        for (pos = 0u; pos < resCto; pos++)
        {
            Xcp_KeyBuffer[Xcp_ChannelCommonData.SeedandKeyPos + pos] = Xcp_CmdBuffer[pos + XCP_UNLOCK_DATA_OFFSET];
        }
        Xcp_ChannelCommonData.SeedandKeyPos += pos;
        Xcp_ChannelCommonData.SeedandKeyLen -= resCto;
        Xcp_RespBuffer[1u]               = Xcp_ChannelCommonData.ProtectionStatus;
        Xcp_ChannelCommonData.RespLength = XCP_CONST_2;
    }
    /* key in this frame */
    else
    {
        /* copy last data frame */
        for (pos = 0u; pos < len; pos++)
        {
            Xcp_KeyBuffer[Xcp_ChannelCommonData.SeedandKeyPos + pos] = Xcp_CmdBuffer[pos + XCP_UNLOCK_DATA_OFFSET];
        }
        /* check validity */
        seedVerify.lengthS = Xcp_ChannelCommonData.SeedLen;
        seedVerify.seed    = Xcp_SeedBuffer;
        keyVerify.lengthK  = Xcp_ChannelCommonData.KeyLen;
        keyVerify.key      = Xcp_KeyBuffer;
        result             = Xcp_IsKeyRight(seedVerify, keyVerify, Xcp_ChannelCommonData.SeedandKeyRes);
        /* valid key */
        if (result)
        {
            Xcp_ChannelCommonData.ProtectionStatus &= (uint8)(~Xcp_ChannelCommonData.SeedandKeyRes);
            Xcp_RespBuffer[XCP_CONST_1]      = Xcp_ChannelCommonData.ProtectionStatus;
            Xcp_ChannelCommonData.RespLength = XCP_CONST_2;
        }
        /* unvalid key */
        else
        {
            Xcp_DisconnectHal();
            *normalExitPtr = 0u;
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        Xcp_ChannelCommonData.KeyRunStatus = XCP_PRE_SEED;
    }
    return;
}

/**
 * Unlocks protected resources by verifying the provided key. This function checks
 * the command length and programming status. It processes the key based on the
 * current key run status, verifying the key and updating the protection status
 * if valid. If the key is invalid or the sequence is incorrect, it sets an
 * appropriate error code and disconnects if necessary. This function is intended
 * for internal use.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Xcp_STMIF */
void Xcp_Unlock(void)
/* PRQA S 6030 -- */
{
    uint8 len        = Xcp_CmdBuffer[1u];
    uint8 normalExit = 1u;
/* length check */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != (len + XCP_CONST_2))
#if (XCP_MAX_CTO > 8u)
        && (Xcp_ChannelCommonData.CmdLength != (len + 4u))
#endif /*XCP_MAX_CTO > 8u*/
        && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
    {
        /* normal function */
        if (XCP_PRE_KEY == Xcp_ChannelCommonData.KeyRunStatus)
        {
            if (len > XCP_KEY_MAX_LENGTH)
            {
                Xcp_ChannelCommonData.KeyRunStatus = XCP_PRE_SEED;
                Xcp_DisconnectHal();
                normalExit = 0u;
                Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            }
            else
            {
                Xcp_ChannelCommonData.KeyLen        = len;
                Xcp_ChannelCommonData.SeedandKeyLen = len;
                Xcp_ChannelCommonData.SeedandKeyPos = 0u;
                Xcp_ChannelCommonData.KeyRunStatus  = XCP_KEY;
                /* call key handler */
                Xcp_KeyHandler(len, &normalExit);
            }
        }
        else if (XCP_KEY == Xcp_ChannelCommonData.KeyRunStatus)
        {
            if (len == Xcp_ChannelCommonData.SeedandKeyLen)
            {
                /* call key handler */
                Xcp_KeyHandler(len, &normalExit);
            }
            else
            {
                Xcp_ChannelCommonData.KeyRunStatus = XCP_PRE_SEED;
                Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
            }
        }
        /* err sequnce */
        else
        {
            Xcp_ChannelCommonData.KeyRunStatus = XCP_PRE_SEED;
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        }
    }
    Xcp_SendResp();

    /* First send the counter, and then clear the count value */
    if (0u == normalExit)
    {
#if (XCP_ON_ETHERNET_ENABLE == STD_ON)
        Xcp_ChannelCommonData.EthRxCounter = 0u;
        Xcp_ChannelCommonData.EthTxCounter = 0u;
#endif /*XCP_ON_ETHERNET_ENABLE == STD_ON*/
    }
    return;
}
#endif /* XCP_SEED_AND_UNLOCK == STD_ON */

#if (STD_ON == XCP_SET_MTA)
/**
 * Sets the Memory Transfer Address (MTA). This function checks the command length
 * and validates the address based on the specified memory segment. If the address
 * is valid, it sets the MTA and MTA extension. If the command length is incorrect
 * or the address is out of range, it sets an appropriate error code. This function
 * is intended for internal use.
 */
void Xcp_SetMta(void)
{
    uint8   extAddr = Xcp_CmdBuffer[XCP_CONST_3];
    uint32  Addr    = 0u;
    boolean calflag;
    boolean pgmflag;

    Xcp_CopyU1BufferToU4(&Xcp_CmdBuffer[XCP_CONST_4], &Addr, (uint8)CPU_BYTE_ORDER);

#if (0u != (XCP_PL_CAL & XCP_RESOURCE))
    calflag = Xcp_CheckAddress(Xcp_Mta2Ptr(extAddr, Addr), 0u, XCP_MEM_CAL_ALL);
#endif
#if (0u != (XCP_PL_PGM & XCP_RESOURCE))
    pgmflag = Xcp_CheckAddress(Xcp_Mta2Ptr(extAddr, Addr), 0u, XCP_MEM_PGM);
#endif

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    /* PRQA S 2995 ++ */ /* VL_Xcp_2995 */
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_8) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    /* PRQA S 2995 -- */
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
        if ((boolean)FALSE
#if (0u != (XCP_PL_CAL & XCP_RESOURCE))
            || (calflag) /* PRQA S 4558 */ /* VL_Xcp_4558 */
#endif
#if (0u != (XCP_PL_PGM & XCP_RESOURCE))
            || (pgmflag)
#endif
        )
    {
        Xcp_ChannelCommonData.MTAExtension = extAddr;
        Xcp_ChannelCommonData.MTA          = Addr;
    }
#if (0u != ((XCP_PL_CAL | XCP_PL_PGM) & XCP_RESOURCE))
    else
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
    }
#endif /*XCP_PL_CAL == XCP_PL_CAL&XCP_RESOURCE*/
    Xcp_SendResp();
    return;
}

/**
 * Convert the MTA (Memory Transfer Address) to an address.
 */
uint32 Xcp_Mta2Ptr(const uint8 u1AddExt, const uint32 u4Add)
{
    (void)u1AddExt;
    return u4Add;
}
#endif /*STD_ON == XCP_SET_MTA*/

#if (STD_ON == XCP_UPLOAD)
/**
 * Uploads data from the specified memory address. This function checks the
 * command length, protection status, and programming status. If the command
 * length is incorrect, the resource is protected, or the programming is active,
 * it sets an appropriate error code. Otherwise, it calls the `Xcp_UploadHal`
 * function to perform the upload. This function is intended for internal use.
 */
void Xcp_Upload(void)
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_2) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
    {
        if (Xcp_ChannelCommonData.ProtectionStatus != 0u)
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        else if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
        {
            Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
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

/**
 * Performs the actual data upload from the specified memory address. This function
 * checks the length of the data to be uploaded and ensures it is within valid
 * limits. It then copies the data from the source memory address to the response
 * buffer and updates the Memory Transfer Address (MTA). If the length exceeds
 * the upload size limit, it sets an appropriate error code. This function is
 * intended for internal use.
 */
XCP_LOCAL void Xcp_UploadHal(void)
{
    uint8             len = Xcp_CmdBuffer[1u];
    const Xcp_AGType* sourPtr;
    Xcp_AGType*       destPtr;
    uint8             pos;

    if ((0x00u == len)
#if (XCP_SLAVE_BLOCK_MODE == STD_OFF)
        || (len > XCP_UPLOAD_SIZE)
#endif
    )
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
    }
    else
    {
#if (XCP_SLAVE_BLOCK_MODE == STD_ON)
        if (len <= XCP_UPLOAD_SIZE)
#endif
        {
            /* get pointer from MTA */
            /* PRQA S 0306 ++ */ /* VL_Xcp_0306 */
            sourPtr = (Xcp_AGType*)Xcp_Mta2Ptr(Xcp_ChannelCommonData.MTAExtension, Xcp_ChannelCommonData.MTA);
            /* PRQA S 0306 -- */
            destPtr = (Xcp_AGType*)(&Xcp_RespBuffer[XCP_UPLOAD_DATA_OFFSET]);
            for (pos = 0u; pos < len; pos++)
            {
                destPtr[pos] = sourPtr[pos];
            }
            /* copy data into buffer */
            /* PRQA S 2985,1252 ++ */ /* VL_Xcp_2985,VL_Xcp_1252 */
            Xcp_UpdateMTA((uint32)len * XCP_AG);
            Xcp_ChannelCommonData.RespLength = (len * XCP_AG) + XCP_UPLOAD_DATA_OFFSET;
            /* PRQA S 2985,1252 -- */
        }
#if (XCP_SLAVE_BLOCK_MODE == STD_ON)
        else
        {
            Xcp_ChannelCommonData.BlockBufPos   = 0u;
            Xcp_ChannelCommonData.BlockBuflen   = len;
            Xcp_ChannelCommonData.CommandStatus = XCP_CMD_BLOCK_TRANSF;
        }
#endif
    }
    return;
}
#endif /*STD_ON == XCP_UPLOAD*/

#if (STD_ON == XCP_SHORT_UPLOAD)
/**
 * Performs a short data upload from the specified memory address. This function
 * checks the command length, programming status, and protection status. If the
 * command length is incorrect, the programming is active, or the resource is
 * protected, it sets an appropriate error code. Otherwise, it calls the
 * `Xcp_ShortUploadHal` function to perform the upload. This function is intended
 * for internal use.
 */
void Xcp_ShortUpload(void)
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC)
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        if (Xcp_ChannelCommonData.ProtectionStatus != 0u)
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
    }
    else
    {
        Xcp_ShortUploadHal();
    }
    Xcp_SendResp();
    return;
}

/**
 * Performs a short data upload from a specified memory address. This function
 * checks the length of the data to be uploaded and ensures it is within valid
 * limits. It sets the Memory Transfer Address (MTA), copies the data from the
 * source memory address to the response buffer, and updates the MTA. If the
 * length is invalid, it sets an appropriate error code. This function is intended
 * for internal use.
 */
XCP_LOCAL void Xcp_ShortUploadHal(void)
{
    uint8             len     = Xcp_CmdBuffer[XCP_CONST_1];
    uint8             extAddr = Xcp_CmdBuffer[XCP_CONST_3];
    uint32            Addr    = 0u;
    const Xcp_AGType* sourPtr;
    Xcp_AGType*       destPtr;
    uint8             pos;

    Xcp_CopyU1BufferToU4(&Xcp_CmdBuffer[XCP_CONST_4], &Addr, (uint8)CPU_BYTE_ORDER);

    if ((0x00u == len) || (len > XCP_SHORT_UPLOAD_SIZE))
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
    }
    else
    {
        /* Set MTA */
        Xcp_ChannelCommonData.MTAExtension = extAddr;
        Xcp_ChannelCommonData.MTA          = Addr;
        /* get pointer from MTA */
        /* PRQA S 0306 ++ */ /* VL_Xcp_0306 */
        sourPtr = (Xcp_AGType*)Xcp_Mta2Ptr(Xcp_ChannelCommonData.MTAExtension, Xcp_ChannelCommonData.MTA);
        /* PRQA S 0306 -- */
        destPtr = (Xcp_AGType*)(&Xcp_RespBuffer[XCP_UPLOAD_DATA_OFFSET]);
        /* copy data into buffer */
        for (pos = 0u; pos < len; pos++)
        {
            destPtr[pos] = sourPtr[pos];
        }
        /* PRQA S 2985,1252 ++ */ /* VL_Xcp_2985,VL_Xcp_1252 */
        Xcp_UpdateMTA((uint32)len * XCP_AG);
        Xcp_ChannelCommonData.RespLength = (len * XCP_AG) + XCP_UPLOAD_DATA_OFFSET;
        /* PRQA S 2985,1252 -- */
    }
    return;
}
#endif /*XCP_SHORT_UPLOAD == STD_ON*/

#if (XCP_BUILD_CHECKSUM == STD_ON)
/**
 * Computes a checksum for a specified memory block. This function checks the
 * command length, protection status, and block size. It calculates the checksum
 * using the specified start address and block size, and updates the Memory
 * Transfer Address (MTA). If the command length is incorrect, the resource is
 * protected, the block size exceeds the maximum allowed, or the address is
 * invalid, it sets an appropriate error code. This function is intended for
 * internal use.
 */
void Xcp_BuildChecksum(void)
{
    uint32 crcResult;
    uint32 blockSize;
    uint32 checksumStartAddress;
    uint32 maxChecksumBlockSize = XCP_MAX_CHECKSUM_SIZE;

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC)
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
        if (Xcp_ChannelCommonData.ProtectionStatus != 0u)
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
    }
    else
    {
        /* get block size from CTO buffer[4-7] */
        Xcp_CopyU1BufferToU4(&Xcp_CmdBuffer[XCP_CONST_4], &blockSize, (uint8)CPU_BYTE_ORDER);
        /* get bolck start address */
        checksumStartAddress = Xcp_Mta2Ptr(Xcp_ChannelCommonData.MTAExtension, Xcp_ChannelCommonData.MTA);

        if (blockSize > maxChecksumBlockSize)
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);

            /* Put the MAX_CHECKSUM_BLOCKSIZE to RespBuffer[4]-[7] buffer */
            Xcp_CopyU4ToU1Buffer(maxChecksumBlockSize, &(Xcp_RespBuffer[XCP_CONST_4]), (uint8)CPU_BYTE_ORDER);
            Xcp_ChannelCommonData.RespLength = XCP_CONST_8;
        }
#if (0u != (XCP_PL_CAL & XCP_RESOURCE))
        /* address check */
        else if ((boolean)FALSE == Xcp_CheckAddress(checksumStartAddress, blockSize, XCP_MEM_CAL_ALL))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
        }
#endif /*XCP_PL_CAL == XCP_PL_CAL&XCP_RESOURCE*/
        else
        {
            /* set checksum type */
            Xcp_RespBuffer[1u] = XCP_CHECKSUM_TYPE_CRC_16_CITT;
            /* call user interface  Ccp_ChecksumCompute to set the crcResult and retrun the size of cheksum*/
            /* PRQA S 2985 ++ */ /* VL_Xcp_2985 */
            Xcp_ChecksumCompute(checksumStartAddress, (blockSize * XCP_AG), &crcResult);
            /* PRQA S 2985 -- */
            Xcp_CopyU4ToU1Buffer(crcResult, &(Xcp_RespBuffer[XCP_CONST_4]), (uint8)CPU_BYTE_ORDER);
            /* PRQA S 2985 ++ */ /* VL_Xcp_2985 */
            Xcp_UpdateMTA(blockSize * XCP_AG);
            /* PRQA S 2985 -- */
            Xcp_ChannelCommonData.RespLength = XCP_CONST_8;
        }
    }
    Xcp_SendResp();
    return;
}
#endif /*XCP_BUILD_CHECKSUM == STD_ON*/

/* Transport Layer Command */
#if (XCP_GET_SLAVE_ID == STD_ON)
/**
 * Identifies the slave ID by echoing or inversely echoing a specific command.
 * This function checks the command length, programming status, and the mode
 * of identification. It responds with the appropriate echo or inverse echo
 * and sets the command status accordingly. If the command length is incorrect,
 * the programming is active, or the mode is invalid, it sets an appropriate
 * error code. This function is intended for internal use.
 */
void Xcp_GetSlaveId(void)
{
    uint8  getSlaveMode;
    uint32 canIdForStimCmd;
    /* PRQA S 3223 ++ */ /* VL_Xcp_3223 */
    XCP_LOCAL Xcp_GetSlaveIdStatusType Xcp_GetSlaveIdStatus = XCP_GETSLAVEID_IDLE;
    /* PRQA S 3223 -- */

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != 0x06u) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
    {
        getSlaveMode = Xcp_CmdBuffer[XCP_CONST_5];
        /* PRQA S 3120 ++ */                      /* VL_QAC_MagicNum */
        if ((0x58u == Xcp_CmdBuffer[XCP_CONST_2]) /**< 'X'*/
            && (0x43u == Xcp_CmdBuffer[3u])       /**< 'C'*/
            && (0x50u == Xcp_CmdBuffer[4u]))      /**< 'P'*/
                                                  /*PRQA S 3120 --*/
        {
            switch (getSlaveMode)
            {
            /* identify by echo */
            case 0u:
            {
                /* PRQA S 3120 ++ */                 /* VL_QAC_MagicNum */
                Xcp_RespBuffer[1u]          = 0x58u; /**< 'X'*/
                Xcp_RespBuffer[XCP_CONST_2] = 0x43u; /**< 'C'*/
                Xcp_RespBuffer[3u]          = 0x50u; /**< 'P'*/
                /* PRQA S 3120 -- */

                /* Get the Canid Used for STIM and CMD from Configuration */
                /*Not Support.*/
                canIdForStimCmd = 0u;

                /* Put the canid to RespBuffer[4]-[7] buffer in INTEL format */
                Xcp_CopyU4ToU1Buffer(canIdForStimCmd, &(Xcp_RespBuffer[XCP_CONST_4]), (uint8)LOW_BYTE_FIRST);

                /* response length is 8 */
                Xcp_ChannelCommonData.RespLength = XCP_CONST_8;

                /* Set The status to XCP_GETSLAVEID_POSTIVE_STATUS */
                Xcp_GetSlaveIdStatus = XCP_GETSLAVEID_POSTIVE_STATUS;
                break;
            }
            /* confirm by inverse echo */
            case 1u:
            {
                if (XCP_GETSLAVEID_POSTIVE_STATUS == Xcp_GetSlaveIdStatus)
                {
                    /* PRQA S 3120 ++ */                 /* VL_QAC_MagicNum */
                    Xcp_RespBuffer[1u]          = 0xA7u; /**< '~X'*/
                    Xcp_RespBuffer[XCP_CONST_2] = 0xBCu; /**< '~C'*/
                    Xcp_RespBuffer[3u]          = 0xAFu; /**< '~P'*/
                    /* PRQA S 3120 -- */

                    /* Get the Canid Used for STIM and CMD from Configuration */
                    /*Not Support.*/
                    canIdForStimCmd = 0u;

                    /* Put the canid to RespBuffer[4]-[7] buffer in INTEL format */
                    Xcp_CopyU4ToU1Buffer(canIdForStimCmd, &Xcp_RespBuffer[XCP_CONST_4], (uint8)LOW_BYTE_FIRST);

                    /* response length is 8 */
                    Xcp_ChannelCommonData.RespLength = XCP_CONST_8;

                    /* Set The status back to XCP_GETSLAVEID_IDLE */
                    Xcp_GetSlaveIdStatus = XCP_GETSLAVEID_IDLE;
                }
                else
                {
                    /* without a previous GET_SLAVE_ID(identify by echo),
                     * the slaves will silently ignore the command.
                     */
                    Xcp_ChannelCommonData.CommandStatus = XCP_CMD_IDLE;
                }
                break;
            }
            /* mode error */
            default:
            {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);

                break;
            }
            }
        }
        else
        {
            /* command value not correct */
            Xcp_ChannelCommonData.CommandStatus = XCP_CMD_IDLE;
        }
    }
    if ((Xcp_CommandStatusType)XCP_CMD_IDLE != Xcp_ChannelCommonData.CommandStatus)
    {
        Xcp_SendResp();
    }
    return;
}
#endif /* XCP_GET_SLAVE_ID == STD_ON */

#if (0u != ((XCP_PL_CAL | XCP_PL_PGM) & XCP_RESOURCE))
#if (STD_ON == XCP_CHECK_MEA_ADDR)
/**
 * Checks if the specified memory address and size are valid for measurement.
 * This function iterates through a set of predefined memory ranges and checks
 * if the given address and size fall within any of these ranges. If a valid
 * range is found, it returns `TRUE`; otherwise, it returns `FALSE`. This
 * function is intended for internal use.
 */
boolean Xcp_CheckMeaMem(uint32 addr, uint32 size)
{
    boolean result = FALSE;
    uint32  start;
    uint32  end;

    for (uint8 i = 0u; i < XCP_CHECK_MEASET_NUM; i++)
    {
        start = Xcp_MeaAddrCfgSet[i].MeaAddr;
        /* PRQA S 1252 ++ */ /* VL_Xcp_1252 */
        end = Xcp_MeaAddrCfgSet[i].MeaAddr + Xcp_MeaAddrCfgSet[i].MeaLen - 1u;
        /* PRQA S 1252 -- */
        if (Xcp_CheckMemVaild(addr, size, start, end))
        {
            result = TRUE;
            break;
        }
    }

    return result;
}
#endif
/**
 * Validates the specified memory address and size based on the memory type.
 * This function checks if the address and size are valid for calibration,
 * measurement, or programming memory, depending on the specified type. It
 * returns `TRUE` if the address and size are valid; otherwise, it returns
 * `FALSE`. This function is intended for internal use.
 */
boolean Xcp_CheckAddress(uint32 addr, uint32 size, Xcp_MemAddrType type)
{
    boolean result = FALSE;

    switch (type)
    {
#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
    case XCP_MEM_CAL_ALL:
    {
        result = Xcp_CheckCalMem(addr, size, XCP_MEM_CAL_ALL);
        break;
    }
    case XCP_MEM_CAL_ROM:
    {
        result = Xcp_CheckCalMem(addr, size, XCP_MEM_CAL_ROM);
        break;
    }
    case XCP_MEM_CAL_RAM:
    {
        result = Xcp_CheckCalMem(addr, size, XCP_MEM_CAL_RAM);
        break;
    }
#if (STD_ON == XCP_CHECK_MEA_ADDR)
    case XCP_MEM_MEA:
    {
        /*check measurement addr is valid in polling mode*/
        result = Xcp_CheckMeaMem(addr, size);
        break;
    }
#endif /*STD_ON == XCP_CHECK_MEA_ADDR*/
#endif /*XCP_PL_CAL == XCP_PL_CAL&XCP_RESOURCE*/

#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
    case XCP_MEM_PGM:
    {
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
/**
 * Updates the Memory Transfer Address (MTA) by the specified length. This
 * function increments the MTA by the given number of bytes. This function
 * is intended for internal use.
 */
void Xcp_UpdateMTA(uint32 u4Length)
{
    /* updata the MTA */
    Xcp_ChannelCommonData.MTA += u4Length;
    return;
}
#endif /*STD_ON == XCP_SET_MTA*/
/* PRQA S 1532 -- */
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

/* ========================================== internal function definitions ========================================= */
