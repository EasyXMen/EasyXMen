
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
 **  @file        : LinIf_Internal.c
 **  @author      : HuRongbo
 **  @date        :
 **  @vendor      : isoft
 **  @description : Define LinIf module internal function
 **
 ***********************************************************************************************************************/
/* PRQA S 6510,0553 EOF */ /* VL_MTR_LinIf_CONF,VL_LinIf_0553 */
#include "LinIf_Internal.h"

#if (STD_ON == LINIF_BUS_MIRROR_SUPPORT)
#include "Mirror.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"
void LinIf_ReportRxFrameToMirror(
    NetworkHandleType      ch,
    const LinIf_FrameType* frame,
    const uint8*           linSduPtr,
    Lin_StatusType         status)
{
    NetworkHandleType commChannel           = LinIf_GetComMNetwork(ch);
    uint8             sduBuf[LINIF_SDU_LEN] = {0u};

    if (status == LIN_RX_OK)
    {
        IStdLib_MemCpy(sduBuf, linSduPtr, frame->Length);
        PduInfoType pduInfo = {.SduDataPtr = sduBuf, .SduLength = frame->Length};
        Mirror_ReportLinFrame(commChannel, frame->ProtectedId, &pduInfo, status);
    }
    else
    {
        Mirror_ReportLinFrame(commChannel, frame->ProtectedId, NULL_PTR, status);
    }
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

void LinIf_ReportTxFrameToMirror(NetworkHandleType ch, LinIf_MirrorBufferType* mirrorBuffer, Lin_StatusType status)
{
    NetworkHandleType commChannel = LinIf_GetComMNetwork(ch);

    if (status == LIN_TX_OK)
    {
        PduInfoType pduInfo = {.SduDataPtr = mirrorBuffer->TxFrameDataBuffer, .SduLength = mirrorBuffer->TxDataLen};
        Mirror_ReportLinFrame(commChannel, mirrorBuffer->FramePid, &pduInfo, status);
    }
    else
    {
        Mirror_ReportLinFrame(commChannel, mirrorBuffer->FramePid, NULL_PTR, status);
    }
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

void LinIf_SaveTxDataToMirror(LinIf_MirrorBufferType* mirrorBufferPtr, const LinIf_FrameType* framePtr, uint8* sduPtr)
{
    mirrorBufferPtr->FramePid  = framePtr->ProtectedId;
    mirrorBufferPtr->TxDataLen = framePtr->Length;
    IStdLib_MemCpy(mirrorBufferPtr->TxFrameDataBuffer, sduPtr, framePtr->Length);
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"

/* @req <SWS_LinIf_00869> */
void LinIf_ReportLinErrorToMirror(NetworkHandleType ch, const LinIf_FrameType* framePtr, Lin_SlaveErrorType error)
{
    NetworkHandleType commChannel = LinIf_GetComMNetwork(ch);
    Lin_StatusType    status      = LIN_NOT_OK;

    if (error == LIN_ERR_HEADER || error > LIN_ERR_INC_RESP)
    {
        return;
    }

    switch (error)
    {
    case LIN_ERR_RESP_CHKSUM:
    case LIN_ERR_INC_RESP:
        status = LIN_RX_ERROR;
        break;

    case LIN_ERR_NO_RESP:
        status = LIN_RX_NO_RESPONSE;
        break;

    case LIN_ERR_RESP_DATABIT:
        status = LIN_TX_ERROR;
        break;

    case LIN_ERR_RESP_STOPBIT:
        if (LINIF_RX_PDU == framePtr->PduDirection->PduDirectionId)
        {
            status = LIN_RX_ERROR;
        }
        else if (LINIF_TX_PDU == framePtr->PduDirection->PduDirectionId)
        {
            status = LIN_TX_ERROR;
        }
        else
        {
            /* do nothing */
        }
        break;
    default:
        /*do nothing */
        break;
    }

    if (LIN_NOT_OK != status)
    {
        Mirror_ReportLinFrame(commChannel, framePtr->ProtectedId, NULL_PTR, status);
    }
}
#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"

#endif
