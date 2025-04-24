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
**                                                                            **
**  FILENAME    : Xcp_Cal.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation of the XCP_Cal command                       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*=======[I N C L U D E S]====================================================*/
#include "Xcp_Internal.h"
#if (STD_ON == XCP_PAG_SUPPORT)
#include "Xcp_Interface.h"
#endif

#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
/*=======[M A C R O S]========================================================*/
#if (XCP_MODIFY_BITS == STD_ON)
#define XCP_MODIFY_BITS_MAX_S 32u
#endif
#define XCP_SET_PAG_MODE_ALL 0x83u
#define XCP_SET_PAG_MODE_ECU 0x01u
#define XCP_SET_PAG_MODE_XCP 0x02u

#define XCP_GET_PAG_MODE_ECU 0x01u
#define XCP_GET_PAG_MODE_XCP 0x02u

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
static FUNC(void, XCP_CODE) Xcp_DownloadHal(void);
#if (STD_ON == XCP_PAG_SUPPORT)
static FUNC(void, XCP_CODE) Xcp_SwitchToWP(void);
static FUNC(void, XCP_CODE) Xcp_SwitchToRP(void);
static FUNC(void, XCP_CODE) Xcp_SetCalPageHal(void);
#endif /*STD_ON == XCP_PAG_SUPPORT*/
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"
/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/******************************************************************************/
/*
 * @brief               <Calibration Initialization>
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
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
FUNC(void, XCP_CODE)
Xcp_CALInit(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 segNum;
#if (STD_ON == XCP_PAG_SUPPORT)
    Xcp_InitCalMem();
#endif /*STD_ON == XCP_PAG_SUPPORT*/
    /* init the RAM page */
    for (segNum = 0; segNum < Xcp_SegmentInfo.maxSegNum; segNum++)
    {
        SchM_Enter_Xcp_Exclusive_Common();
        Xcp_BufferCopy(
            Xcp_SegmentInfo.pageInfoPtr[segNum].romStart,
            Xcp_SegmentInfo.pageInfoPtr[segNum].ramStart,
            Xcp_SegmentInfo.pageInfoPtr[segNum].size);
        SchM_Exit_Xcp_Exclusive_Common();
#if (STD_ON == XCP_PAG_SUPPORT)
        /* alloc an array to copy the data in the RAM */
        Xcp_BufferCopy(
            Xcp_SegmentInfo.pageInfoPtr[segNum].ramStart,
            Xcp_SegmentInfo.pageInfoPtr[segNum].bufferPtr,
            Xcp_SegmentInfo.pageInfoPtr[segNum].size);
        Xcp_ActivPagNum[segNum] = XCP_RAM_PAGE_NUM;
#endif /*STD_ON == XCP_PAG_SUPPORT*/
    }
    return;
}

/*Check calibration memory.*/
FUNC(boolean, XCP_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Xcp_CheckCalMem(uint32 addr, uint32 size, Xcp_MemAddrType type)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    uint8 segmentNum;
    boolean result = FALSE;
    const Xcp_PageInfoType* pageInfoPtr;

    if ((XCP_MEM_CAL_ALL == type) || (XCP_MEM_CAL_ROM == type))
    {
        for (segmentNum = 0; segmentNum < Xcp_SegmentInfo.maxSegNum; segmentNum++)
        {
            pageInfoPtr = &Xcp_SegmentInfo.pageInfoPtr[segmentNum];
            /*Check if the address in the range of Rom*/
            if (Xcp_CheckMemVaild(addr, size, pageInfoPtr->romStart, pageInfoPtr->romEnd))
            {
                result = TRUE;
                break;
            }
        }
    }
    if ((!result) && ((XCP_MEM_CAL_ALL == type) || (XCP_MEM_CAL_RAM == type)))
    {
        for (segmentNum = 0; segmentNum < Xcp_SegmentInfo.maxSegNum; segmentNum++)
        {
            pageInfoPtr = &Xcp_SegmentInfo.pageInfoPtr[segmentNum];
            /*Check if the address in the range of Ram*/
            if (Xcp_CheckMemVaild(addr, size, pageInfoPtr->ramStart, pageInfoPtr->ramEnd))
            {
                result = TRUE;
                break;
            }
        }
    }

    return result;
}

/******************************************************************************/
/*
 * @brief               <Command Download Handler>
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
FUNC(void, XCP_CODE) Xcp_Download(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if (
#if (XCP_ADDR_GRANULARITY != XCP_AG_DWORD)
        (Xcp_CmdLength != (2u + (Xcp_CmdBuffer[1u] * XCP_AG)))
#else
        (Xcp_CmdLength != (4u + (Xcp_CmdBuffer[1u] * XCP_AG)))
#endif
        && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_CAL == (Xcp_ProtectionStatus & XCP_PL_CAL))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else if ((Xcp_CmdBuffer[1u] > XCP_DOWNLOAD_SIZE) || (0u == Xcp_CmdBuffer[1u]))
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            Xcp_DownloadHal();
        }
    Xcp_SendResp();
    return;
}
static FUNC(void, XCP_CODE) Xcp_DownloadHal(void)
{
    P2CONST(Xcp_AGType, AUTOMATIC, XCP_VAR) sourPtr;
    P2VAR(Xcp_AGType, AUTOMATIC, XCP_VAR) destPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    uint8 pos;
    uint8 numOfElements = Xcp_CmdBuffer[1u];
    uint32 u4DestAddr = Xcp_Mta2Ptr(Xcp_MTA.extensionAddr, Xcp_MTA.transferAddr);

    if ((boolean)FALSE == Xcp_CheckAddress(u4DestAddr, ((uint32)numOfElements * XCP_AG), XCP_MEM_CAL_RAM))
    {
        if ((boolean)TRUE == Xcp_CheckAddress(u4DestAddr, ((uint32)numOfElements * XCP_AG), XCP_MEM_CAL_ROM))
        {
            Xcp_SetErrorCode(XCP_ERR_WRITE_PROTECTED);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
            Xcp_RespLength = 0x02u;
        }
    }
    else
    {
        /* write the data of CRO to destination address. */
        sourPtr = (Xcp_AGType*)&Xcp_CmdBuffer[XCP_DOWNLOAD_DATA_OFFSET];
        destPtr = (Xcp_AGType*)u4DestAddr; /* PRQA S 0306 */ /* MISRA Rule 11.4 */
        for (pos = 0u; pos < numOfElements; pos++)
        {
            destPtr[pos] = sourPtr[pos];
        }
        /*updata the MTA*/
        Xcp_UpdateMTA((uint32)numOfElements * XCP_AG);
    }
    return;
}

#if (XCP_MAX_CTO > 8u)
FUNC(void, XCP_CODE)
Xcp_ShortDownload(void)
{
    P2CONST(Xcp_AGType, AUTOMATIC, XCP_VAR) sourPtr;
    P2VAR(Xcp_AGType, AUTOMATIC, XCP_VAR) destPtr;
    uint32 u4DestAddr;
    uint8 numOfData;
    uint8 pos = 0;

    /* check protection status */
    if (XCP_PL_CAL == (Xcp_ProtectionStatus & XCP_PL_CAL))
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        Xcp_RespLength = 0x02u;
    }
    else if ((Xcp_CmdBuffer[1u] > XCP_SHORT_DOWNLOAD_SIZE) || (0u == Xcp_CmdBuffer[1u]))
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        Xcp_RespLength = 0x02u;
    }
    else
    {
        Xcp_CopyU1BufferToU4(&Xcp_CmdBuffer[4u], &u4DestAddr, CPU_BYTE_ORDER);
        numOfData = Xcp_CmdBuffer[1];
        if ((boolean)FALSE == Xcp_CheckAddress(u4DestAddr, ((uint32)numOfData * XCP_AG), XCP_MEM_CAL_RAM))
        {
            Xcp_SetErrorCode(XCP_ERR_WRITE_PROTECTED);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            sourPtr = (Xcp_AGType*)&Xcp_CmdBuffer[8u];
            destPtr = (Xcp_AGType*)u4DestAddr;
            for (pos = 0u; pos < numOfData; pos++)
            {
                destPtr[pos] = sourPtr[pos];
            }
        }
    }
    Xcp_SendResp();
    return;
}
#endif /*XCP_MAX_CTO > 8u*/
/*
 * CAL/PAG Optional Function
 */
#if (XCP_DOWNLOAD_NEXT == STD_ON)
/******************************************************************************/
/*
 * @brief               <Command DownloadNext Handler>
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
Xcp_DownloadNext(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    P2CONST(Xcp_AGType, AUTOMATIC, XCP_VAR) sourPtr;
    P2VAR(Xcp_AGType, AUTOMATIC, XCP_VAR) destPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    uint8 pos;
    uint32 u4DestAddr = Xcp_Mta2Ptr(Xcp_MTA.extensionAddr, Xcp_MTA.transferAddr);

/* check frame length has been checked before */
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
    if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_CAL == (Xcp_ProtectionStatus & XCP_PL_CAL))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else if ((boolean)FALSE == Xcp_CheckAddress(u4DestAddr, ((uint32)Xcp_BlockBufferLen * XCP_AG), XCP_MEM_CAL_RAM))
        {
            if ((boolean)TRUE == Xcp_CheckAddress(u4DestAddr, ((uint32)Xcp_BlockBufferLen * XCP_AG), XCP_MEM_CAL_ROM))
            {
                Xcp_SetErrorCode(XCP_ERR_WRITE_PROTECTED);
                Xcp_RespLength = 0x02u;
            }
            else
            {
                Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
                Xcp_RespLength = 0x02u;
            }
        }
        else
        {
            /* write the data in BlockBuffer to destination address. */
            sourPtr = &Xcp_BlockBuffer[0u];
            destPtr = (Xcp_AGType*)u4DestAddr; /* PRQA S 0306 */ /* MISRA Rule 11.4 */
            for (pos = 0u; pos < Xcp_BlockBufferLen; pos++)
            {
                destPtr[pos] = sourPtr[pos];
            }
            /*updata the MTA*/
            Xcp_UpdateMTA((uint32)Xcp_BlockBufferLen * XCP_AG);
        }
    Xcp_SendResp();
    return;
}
#endif
#if (XCP_DOWNLOAD_MAX == STD_ON)
/******************************************************************************/
/*
 * @brief               <Command DownloadMax Handler>
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
FUNC(void, XCP_CODE) Xcp_DownloadMax(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    P2CONST(Xcp_AGType, AUTOMATIC, XCP_VAR) sourPtr;
    P2VAR(Xcp_AGType, AUTOMATIC, XCP_VAR) destPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    uint8 pos;
    uint32 u4DestAddr = Xcp_Mta2Ptr(Xcp_MTA.extensionAddr, Xcp_MTA.transferAddr);

    /* check frame length */
    if (Xcp_CmdLength != (XCP_DOWNLOAD_MAX_SIZE + 1u))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
    else if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
    {
        /* check protection status */
        if (XCP_PL_CAL == (Xcp_ProtectionStatus & XCP_PL_CAL))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else if ((boolean)FALSE == Xcp_CheckAddress(u4DestAddr, (XCP_DOWNLOADMAX_SIZE * XCP_AG), XCP_MEM_CAL_RAM))
        {
            if ((boolean)TRUE == Xcp_CheckAddress(u4DestAddr, (XCP_DOWNLOADMAX_SIZE * XCP_AG), XCP_MEM_CAL_ROM))
            {
                Xcp_SetErrorCode(XCP_ERR_WRITE_PROTECTED);
                Xcp_RespLength = 0x02u;
            }
            else
            {
                Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
                Xcp_RespLength = 0x02u;
            }
        }
        else
        {
            /* write the data of CRO to destination address. */
            sourPtr = (Xcp_AGType*)&Xcp_CmdBuffer[XCP_AG];
            destPtr = (Xcp_AGType*)u4DestAddr; /* PRQA S 0306 */ /* MISRA Rule 11.4 */
            for (pos = 0u; pos < XCP_DOWNLOADMAX_SIZE; pos++)
            {
                destPtr[pos] = sourPtr[pos];
            }
            /*updata the MTA*/
            Xcp_UpdateMTA(XCP_DOWNLOADMAX_SIZE * XCP_AG);
        }
    }
    Xcp_SendResp();
    return;
}
#endif

/*
 * SHORT_DOWNLOAD CMD:don't support in CAN
 * */

#if (XCP_MODIFY_BITS == STD_ON)
/******************************************************************************/
/*
 * @brief               <Command ModifyBits Handler>
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
FUNC(void, XCP_CODE) Xcp_ModifyBits(void)
{
    uint8 shiftValue = Xcp_CmdBuffer[1u];
    uint16 andMask = 0;
    uint16 xorMask = 0;
    uint32 u4SourceAddr = Xcp_Mta2Ptr(Xcp_MTA.extensionAddr, Xcp_MTA.transferAddr);

/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 0x06u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_CAL == (Xcp_ProtectionStatus & XCP_PL_CAL))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else if ((boolean)FALSE == Xcp_CheckAddress(u4SourceAddr, sizeof(uint32), XCP_MEM_CAL_RAM))
        {
            if ((boolean)TRUE == Xcp_CheckAddress(u4SourceAddr, sizeof(uint32), XCP_MEM_CAL_ROM))
            {
                Xcp_SetErrorCode(XCP_ERR_WRITE_PROTECTED);
                Xcp_RespLength = 0x02u;
            }
            else
            {
                Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
                Xcp_RespLength = 0x02u;
            }
        }
        else if (shiftValue >= XCP_MODIFY_BITS_MAX_S)
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            /* get infomation from the frame */
            Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[2], &andMask, CPU_BYTE_ORDER);
            Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[4], &xorMask, CPU_BYTE_ORDER);
            /* do modify */
            *(uint32*)u4SourceAddr &= (~((uint32)((~andMask) << shiftValue)));
            *(uint32*)u4SourceAddr ^= ((uint32)(xorMask << shiftValue));
            /* Do not updata the MTA*/
        }
    Xcp_SendResp();
    return;
}
#endif

/*
 * PAG
 */
#if (STD_ON == XCP_PAG_SUPPORT)
/******************************************************************************/
/*
 * @brief               <Switching Page to RAM>
 *
 * <Switch Page to RAM by using Buffer> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
static FUNC(void, XCP_CODE) Xcp_SwitchToWP(void)
{
    uint8 segNum;
    for (segNum = 0; segNum < Xcp_SegmentInfo.maxSegNum; segNum++)
    {
        Xcp_BufferCopy(
            Xcp_SegmentInfo.pageInfoPtr[segNum].bufferPtr,
            Xcp_SegmentInfo.pageInfoPtr[segNum].ramStart,
            Xcp_SegmentInfo.pageInfoPtr[segNum].size);
        Xcp_ActivPagNum[segNum] = XCP_RAM_PAGE_NUM;
    }
    return;
}

/******************************************************************************/
/*
 * @brief               <Switching Page to FLASH>
 *
 * <Switch Page to FLASH by using Buffer> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
static FUNC(void, XCP_CODE) Xcp_SwitchToRP(void)
{
    uint8 segNum;
    for (segNum = 0; segNum < Xcp_SegmentInfo.maxSegNum; segNum++)
    {
        Xcp_BufferCopy(
            Xcp_SegmentInfo.pageInfoPtr[segNum].ramStart,
            Xcp_SegmentInfo.pageInfoPtr[segNum].bufferPtr,
            Xcp_SegmentInfo.pageInfoPtr[segNum].size);
        Xcp_BufferCopy(
            Xcp_SegmentInfo.pageInfoPtr[segNum].romStart,
            Xcp_SegmentInfo.pageInfoPtr[segNum].ramStart,
            Xcp_SegmentInfo.pageInfoPtr[segNum].size);
        Xcp_ActivPagNum[segNum] = XCP_FLASH_PAGE_NUM;
    }
    return;
}

#if (STD_ON == XCP_SET_CAL_PAGE)
/******************************************************************************/
/*
 * @brief               <Command SetCalPage Handler>
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
Xcp_SetCalPage(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 0x04u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_CAL == (Xcp_ProtectionStatus & XCP_PL_CAL))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        /* according to the solution of CANape, page switching shall
         * be excuted only in 0x83 mode.
         */
        else
        {
            Xcp_SetCalPageHal();
        }
    Xcp_SendResp();
    return;
}

static FUNC(void, XCP_CODE) Xcp_SetCalPageHal(void)
{
    uint8 mode = Xcp_CmdBuffer[1u];
    uint8 segNum = Xcp_CmdBuffer[2u];
    uint8 pagNum = Xcp_CmdBuffer[3u];

    if (mode != XCP_SET_PAG_MODE_ALL)
    {
        Xcp_SetErrorCode(XCP_ERR_MODE_NOT_VALID);
        Xcp_RespLength = 0x02u;
    }
    else if (segNum >= Xcp_SegmentInfo.maxSegNum)
    {
        Xcp_SetErrorCode(XCP_ERR_SEGMENT_NOT_VALID);
        Xcp_RespLength = 0x02u;
    }
    else
    {
        if (Xcp_ActivPagNum[segNum] != pagNum)
        {
            switch (pagNum)
            {
            case XCP_FLASH_PAGE_NUM: {
                Xcp_SwitchToRP();
                break;
            }
            case XCP_RAM_PAGE_NUM: {
                Xcp_SwitchToWP();
                break;
            }
            default: {
                Xcp_SetErrorCode(XCP_ERR_PAGE_NOT_VALID);
                Xcp_RespLength = 0x02u;
                break;
            }
            }
        }
    }
    return;
}

#endif /*STD_ON == XCP_SET_CAL_PAGE*/
#if (XCP_GET_CAL_PAGE == STD_ON)
/******************************************************************************/
/*
 * @brief               <Command getCalPage Handler>
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
FUNC(void, XCP_CODE) Xcp_GetCalPage(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 accessMode = Xcp_CmdBuffer[1u];
    uint8 segNum = Xcp_CmdBuffer[2u];
/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 0x03u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_CAL == (Xcp_ProtectionStatus & XCP_PL_CAL))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else if (segNum >= Xcp_SegmentInfo.maxSegNum)
        {
            Xcp_SetErrorCode(XCP_ERR_SEGMENT_NOT_VALID);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            switch (accessMode)
            {
            case XCP_GET_PAG_MODE_ECU:
            case XCP_GET_PAG_MODE_XCP: {
                Xcp_RespBuffer[3u] = Xcp_ActivPagNum[segNum];
                Xcp_RespLength = 4;
                break;
            }
            default: {
                Xcp_SetErrorCode(XCP_ERR_MODE_NOT_VALID);
                Xcp_RespLength = 0x02u;
                break;
            }
            }
        }
    Xcp_SendResp();
    return;
}
#endif /*XCP_GET_CAL_PAGE == STD_ON*/
#endif /*STD_ON == XCP_PAG_SUPPORT*/

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#endif /*XCP_PL_CAL == XCP_PL_CAL & XCP_RESOURCE*/
