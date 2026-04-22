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
 ***********************************************************************************************************************
 **
 **  @file               : Xcp_Cal.c
 **  @author             : qinchun.yang
 **  @date               : 2024/11/17
 **  @vendor             : isoft
 **  @description        : Implementation of the XCP_Cal command
 **
 ***********************************************************************************************************************/
/* =================================================== inclusions =================================================== */
#include "Xcp_Internal.h"

#if (XCP_PL_CAL == (XCP_PL_CAL & XCP_RESOURCE))
/* ===================================================== macros ===================================================== */
#if (XCP_MODIFY_BITS == STD_ON)
#define XCP_MODIFY_BITS_MAX_S 32u
#endif
#define XCP_SET_PAG_MODE_ALL 0x83u
#define XCP_SET_PAG_MODE_ECU 0x01u
#define XCP_SET_PAG_MODE_XCP 0x02u

#define XCP_GET_PAG_MODE_ECU 0x01u
#define XCP_GET_PAG_MODE_XCP 0x02u

/* ========================================== internal function declarations ======================================== */
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
XCP_LOCAL void Xcp_DownloadHal(void);
#if (STD_ON == XCP_PAG_SUPPORT)
XCP_LOCAL void Xcp_SwitchToWP(void);
XCP_LOCAL void Xcp_SwitchToRP(void);
XCP_LOCAL void Xcp_SetCalPageHal(void);
#endif /*STD_ON == XCP_PAG_SUPPORT*/
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

/* ============================================ internal data definitions =========================================== */
#if (STD_OFF == XCP_SWITCH_PAG_BY_USER)
#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
XCP_LOCAL uint8 Xcp_ActivPagNum[XCP_MAX_SEGMENT];
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
#endif

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
/**
 * Initializes the XCP calibration segments. This function copies data from the
 * read-only pages (RP) to the writeable pages (WP) and initializes the buffer
 * for each segment. It sets the active page number for each segment to the
 * RAM page number. If user-defined page switching is enabled, it calls the
 * application-specific initialization function.
 */
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Xcp_CALInit(void)
/* PRQA S 1532 -- */
{
#if (STD_ON == XCP_PAG_SUPPORT)
#if (STD_OFF == XCP_SWITCH_PAG_BY_USER)
    /* init the RAM page */
    for (uint8 segNum = 0u; segNum < Xcp_SegmentInfo.maxSegNum; segNum++)
    {
        /*Copy data from RP to WP.*/
        Xcp_BufferCopy(
            /*PRQA S 0326 ++*/ /* VL_Xcp_0326 */
            (const void*)Xcp_SegmentInfo.segInfoPtr[segNum]
                .rpStartAddr,
            (void*)Xcp_SegmentInfo.segInfoPtr[segNum].wpStartAddr,
            /*PRQA S 0326 --*/
            Xcp_SegmentInfo.segInfoPtr[segNum].size);
        /* Alloc an array to copy the data in the RAM */
        Xcp_BufferCopy(
            /*PRQA S 0326 ++*/ /* VL_Xcp_0326 */
            (const void*)Xcp_SegmentInfo.segInfoPtr[segNum]
                .wpStartAddr,
            (void*)Xcp_SegmentInfo.segInfoPtr[segNum].bufferPtr,
            Xcp_SegmentInfo.segInfoPtr[segNum].size);
        /*PRQA S 0326 --*/
        Xcp_ActivPagNum[segNum] = XCP_RAM_PAGE_NUM;
    }
#else
    XcpAppl_CALInit();
#endif /*STD_OFF == XCP_SWITCH_PAG_BY_USER*/
#endif /*STD_ON == XCP_PAG_SUPPORT*/
}

/**
 * Validates the memory address and size for calibration. This function checks if
 * the provided address and size fall within the valid ROM or RAM segments based
 * on the specified memory type. It returns a boolean indicating whether the
 * memory range is valid.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
boolean Xcp_CheckCalMem(uint32 addr, uint32 size, Xcp_MemAddrType type)
/* PRQA S 1532 -- */
{
    uint8                      segmentNum;
    boolean                    result = FALSE;
    const Xcp_SegmentInfoType* segInfoPtr;

    if ((XCP_MEM_CAL_ALL == type) || (XCP_MEM_CAL_ROM == type))
    {
        for (segmentNum = 0u; segmentNum < Xcp_SegmentInfo.maxSegNum; segmentNum++)
        {
            segInfoPtr = &Xcp_SegmentInfo.segInfoPtr[segmentNum];
            /*Check if the address in the range of Rom*/
            if (Xcp_CheckMemVaild(addr, size, segInfoPtr->rpStartAddr, segInfoPtr->rpEndAddr))
            {
                result = TRUE;
                break;
            }
        }
    }
    if ((!result) && ((XCP_MEM_CAL_ALL == type) || (XCP_MEM_CAL_RAM == type)))
    {
        for (segmentNum = 0u; segmentNum < Xcp_SegmentInfo.maxSegNum; segmentNum++)
        {
            segInfoPtr = &Xcp_SegmentInfo.segInfoPtr[segmentNum];
            /*Check if the address in the range of Ram*/
            if (Xcp_CheckMemVaild(addr, size, segInfoPtr->wpStartAddr, segInfoPtr->wpEndAddr))
            {
                result = TRUE;
                break;
            }
        }
    }

    return result;
}

/**
 * Handles the XCP `DOWNLOAD` command. This function validates the frame length,
 * checks the program active status, and ensures the memory is not protected.
 * It also verifies that the data length is within the allowed range. If all
 * checks pass, it calls the `Xcp_DownloadHal` function to perform the download.
 * Finally, it sends a response indicating the result.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Xcp_Download(void)
/* PRQA S 1532 -- */
{
/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    /* PRQA S 2985 ++ */ /* VL_Xcp_2985 */
    if (
#if (XCP_ADDR_GRANULARITY != XCP_AG_DWORD)
        (Xcp_ChannelCommonData.CmdLength != (XCP_CONST_2 + (Xcp_CmdBuffer[1u] * XCP_AG)))
#else
        (Xcp_ChannelCommonData.CmdLength != (4u + (Xcp_CmdBuffer[1u] * XCP_AG)))
#endif
        && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    /* PRQA S 2985 -- */
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_CAL == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_CAL))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else if ((Xcp_CmdBuffer[1u] > XCP_DOWNLOAD_SIZE) || (0u == Xcp_CmdBuffer[1u]))
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        }
        else
        {
            Xcp_DownloadHal();
        }
    Xcp_SendResp();
    return;
}

/**
 * Performs the actual data download in XCP. This function checks if the destination
 * address is valid and writable. If the address is valid, it copies the data from
 * the command buffer to the destination address. It then updates the Memory Transfer
 * Address (MTA). If the address is not valid or is write-protected, it sets an
 * appropriate error code.
 */
XCP_LOCAL void Xcp_DownloadHal(void)
{
    const Xcp_AGType* sourPtr;
    Xcp_AGType*       destPtr;
    uint8             pos;
    uint8             numOfElements = Xcp_CmdBuffer[1u];
    uint32            u4DestAddr    = Xcp_Mta2Ptr(Xcp_ChannelCommonData.MTAExtension, Xcp_ChannelCommonData.MTA);

    /* PRQA S 2985,1252 ++ */ /* VL_Xcp_2985 */
    if (!Xcp_CheckAddress(u4DestAddr, ((uint32)numOfElements * XCP_AG), XCP_MEM_CAL_RAM))
    {
        if (Xcp_CheckAddress(u4DestAddr, ((uint32)numOfElements * XCP_AG), XCP_MEM_CAL_ROM))
        {
            Xcp_SetErrorCode(XCP_ERR_WRITE_PROTECTED);
        }
        else
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
        }
    }
    /* PRQA S 2985,1252 -- */
    else
    {
        /* write the data of CRO to destination address. */
        sourPtr = (Xcp_AGType*)&Xcp_CmdBuffer[XCP_DOWNLOAD_DATA_OFFSET];
        destPtr = (Xcp_AGType*)u4DestAddr; /* PRQA S 0306 */ /* VL_Xcp_0306 */
        for (pos = 0u; pos < numOfElements; pos++)
        {
            destPtr[pos] = sourPtr[pos];
        }
        /*updata the MTA*/
        /* PRQA S 2985,1252 ++ */ /* VL_Xcp_2985 */
        Xcp_UpdateMTA((uint32)numOfElements * XCP_AG);
        /* PRQA S 2985,1252 -- */
    }
    return;
}

#if (XCP_MAX_CTO > 8u)
/**
 * Handles the XCP `SHORT_DOWNLOAD` command. This function checks the protection
 * status and ensures the data length is within the allowed range. It verifies
 * that the destination address is valid and writable. If all checks pass, it
 * copies the data from the command buffer to the destination address. If any
 * check fails, it sets an appropriate error code and sends a response.
 */
void Xcp_ShortDownload(void)
{
    const Xcp_AGType* sourPtr;
    Xcp_AGType*       destPtr;
    uint32            u4DestAddr;
    uint8             numOfData;
    uint8             pos = 0;

    /* check protection status */
    if (XCP_PL_CAL == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_CAL))
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
    }
    else if ((Xcp_CmdBuffer[1u] > XCP_SHORT_DOWNLOAD_SIZE) || (0u == Xcp_CmdBuffer[1u]))
    {
        Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
    }
    else
    {
        Xcp_CopyU1BufferToU4(&Xcp_CmdBuffer[4u], &u4DestAddr, (uint8)CPU_BYTE_ORDER);
        numOfData = Xcp_CmdBuffer[1];
        if ((boolean)FALSE == Xcp_CheckAddress(u4DestAddr, ((uint32)numOfData * XCP_AG), XCP_MEM_CAL_RAM))
        {
            Xcp_SetErrorCode(XCP_ERR_WRITE_PROTECTED);
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
/**
 * Handles the XCP `DOWNLOAD_NEXT` command. This function checks the program active
 * status and protection status, and verifies that the destination address is valid
 * and writable. If all checks pass, it writes the data from the block buffer to
 * the destination address and updates the Memory Transfer Address (MTA). If any
 * check fails, it sets an appropriate error code and sends a response.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Xcp_DownloadNext(void)
/* PRQA S 1532 -- */
{
    const Xcp_AGType* sourPtr;
    Xcp_AGType*       destPtr;
    uint8             pos;
    uint32            u4DestAddr = Xcp_Mta2Ptr(Xcp_ChannelCommonData.MTAExtension, Xcp_ChannelCommonData.MTA);

/* check frame length has been checked before */
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
    if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_CAL == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_CAL))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        /* PRQA S 2985 ++ */ /* VL_QAC_2985_2986 */
        else if (
            (boolean)FALSE
            == Xcp_CheckAddress(u4DestAddr, ((uint32)Xcp_ChannelCommonData.BlockBuflen * XCP_AG), XCP_MEM_CAL_RAM))
        /* PRQA S 2985 -- */
        {
            /* PRQA S 2985 ++ */ /* VL_QAC_2985_2986 */
            if ((boolean)TRUE
                == Xcp_CheckAddress(u4DestAddr, ((uint32)Xcp_ChannelCommonData.BlockBuflen * XCP_AG), XCP_MEM_CAL_ROM))
            /* PRQA S 2985 -- */
            {
                Xcp_SetErrorCode(XCP_ERR_WRITE_PROTECTED);
            }
            else
            {
                Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
            }
        }
        else
        {
            /* write the data in BlockBuffer to destination address. */
            sourPtr = &Xcp_BlockBuffer[0u];
            destPtr = (Xcp_AGType*)u4DestAddr; /* PRQA S 0306 */ /* VL_Xcp_0306 */
            for (pos = 0u; pos < Xcp_ChannelCommonData.BlockBuflen; pos++)
            {
                destPtr[pos] = sourPtr[pos];
            }
            /*updata the MTA*/
            /* PRQA S 2985 ++ */ /* VL_QAC_2985_2986 */
            Xcp_UpdateMTA((uint32)Xcp_ChannelCommonData.BlockBuflen * XCP_AG);
            /* PRQA S 2985 -- */
        }
    Xcp_SendResp();
    return;
}
#endif
#if (XCP_DOWNLOAD_MAX == STD_ON)
/**
 * Handles the XCP `DOWNLOAD_MAX` command. This function checks the frame length,
 * program active status, and protection status. It verifies that the destination
 * address is valid and writable. If all checks pass, it writes the maximum amount
 * of data from the command buffer to the destination address and updates the
 * Memory Transfer Address (MTA). If any check fails, it sets an appropriate error
 * code and sends a response.
 */
/* PRQA S 1532,6030 ++ */ /* VL_QAC_OneFunRef,VL_MTR_Xcp_STMIF */
void Xcp_DownloadMax(void)
/* PRQA S 1532,6030 -- */
{
    const Xcp_AGType* sourPtr;
    Xcp_AGType*       destPtr;
    uint8             pos;
    uint32            u4DestAddr = Xcp_Mta2Ptr(Xcp_ChannelCommonData.MTAExtension, Xcp_ChannelCommonData.MTA);

    /* check frame length */
    if (Xcp_ChannelCommonData.CmdLength != (XCP_DOWNLOAD_MAX_SIZE + 1u))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
    else if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
    {
        /* check protection status */
        if (XCP_PL_CAL == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_CAL))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else if ((boolean)FALSE == Xcp_CheckAddress(u4DestAddr, (XCP_DOWNLOADMAX_SIZE * XCP_AG), XCP_MEM_CAL_RAM))
        {
            if ((boolean)TRUE == Xcp_CheckAddress(u4DestAddr, (XCP_DOWNLOADMAX_SIZE * XCP_AG), XCP_MEM_CAL_ROM))
            {
                Xcp_SetErrorCode(XCP_ERR_WRITE_PROTECTED);
            }
            else
            {
                Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
            }
        }
        else
        {
            /* write the data of CRO to destination address. */
            sourPtr = (Xcp_AGType*)&Xcp_CmdBuffer[XCP_AG];
            destPtr = (Xcp_AGType*)u4DestAddr; /* PRQA S 0306 */ /* VL_Xcp_0306 */
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
/**
 * Handles the XCP `MODIFY_BITS` command. This function checks the frame length,
 * program active status, and protection status. It verifies that the source
 * address is valid and writable. If all checks pass, it modifies the bits at
 * the specified address using the provided AND and XOR masks and shift value.
 * If any check fails, it sets an appropriate error code and sends a response.
 */
void Xcp_ModifyBits(void)
{
    uint8  shiftValue   = Xcp_CmdBuffer[1u];
    uint16 andMask      = 0u;
    uint16 xorMask      = 0u;
    uint32 u4SourceAddr = Xcp_Mta2Ptr(Xcp_ChannelCommonData.MTAExtension, Xcp_ChannelCommonData.MTA);

/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != 0x06u) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_CAL == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_CAL))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else if ((boolean)FALSE == Xcp_CheckAddress(u4SourceAddr, sizeof(uint32), XCP_MEM_CAL_RAM))
        {
            if ((boolean)TRUE == Xcp_CheckAddress(u4SourceAddr, sizeof(uint32), XCP_MEM_CAL_ROM))
            {
                Xcp_SetErrorCode(XCP_ERR_WRITE_PROTECTED);
            }
            else
            {
                Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
            }
        }
        else if (shiftValue >= XCP_MODIFY_BITS_MAX_S)
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        }
        else
        {
            /* get infomation from the frame */
            Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[XCP_CONST_2], &andMask, (uint8)CPU_BYTE_ORDER);
            Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[XCP_CONST_4], &xorMask, (uint8)CPU_BYTE_ORDER);
            /* do modify */
            /*PRQA S 4391,4397,4399,0306 ++*/ /* VL_Xcp_4391,VL_Xcp_4397,VL_Xcp_4399,VL_Xcp_0306 */
            *(uint32*)u4SourceAddr &= (~((uint32)((~andMask) << shiftValue)));
            *(uint32*)u4SourceAddr ^= ((uint32)(xorMask << shiftValue));
            /*PRQA S 4391,4397,4399,0306 --*/

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
#if (STD_OFF == XCP_SWITCH_PAG_BY_USER)
/**
 * Switches the active page to the writeable page (WP) for all segments. This
 * function iterates through each segment, copies the data from the buffer to
 * the WP, and sets the active page number to the RAM page number.
 */
XCP_LOCAL void Xcp_SwitchToWP(void)
{
    uint8 segNum;
    for (segNum = 0u; segNum < Xcp_SegmentInfo.maxSegNum; segNum++)
    {
        Xcp_BufferCopy(
            /*PRQA S 0326 ++*/ /* VL_Xcp_0326 */
            (const void*)Xcp_SegmentInfo.segInfoPtr[segNum]
                .bufferPtr,
            (void*)Xcp_SegmentInfo.segInfoPtr[segNum].wpStartAddr,
            +
            /*PRQA S 0326 --*/
            Xcp_SegmentInfo.segInfoPtr[segNum].size);
        Xcp_ActivPagNum[segNum] = XCP_RAM_PAGE_NUM;
    }
    return;
}

/**
 * Switches the active page to the read-only page (RP) for all segments. This
 * function iterates through each segment, copies the data from the WP to the
 * buffer, then copies the data from the RP to the WP, and sets the active page
 * number to the flash page number.
 */
XCP_LOCAL void Xcp_SwitchToRP(void)
{
    uint8 segNum;
    for (segNum = 0u; segNum < Xcp_SegmentInfo.maxSegNum; segNum++)
    {
        Xcp_BufferCopy(
            /*PRQA S 0326 ++*/ /* VL_Xcp_0326 */
            (const void*)Xcp_SegmentInfo.segInfoPtr[segNum]
                .wpStartAddr,
            (void*)Xcp_SegmentInfo.segInfoPtr[segNum].bufferPtr,
            /*PRQA S 0326 --*/
            Xcp_SegmentInfo.segInfoPtr[segNum].size);
        Xcp_BufferCopy(
            /*PRQA S 0326 ++*/ /* VL_Xcp_0326 */
            (const void*)Xcp_SegmentInfo.segInfoPtr[segNum]
                .rpStartAddr,
            (void*)Xcp_SegmentInfo.segInfoPtr[segNum].wpStartAddr,
            /*PRQA S 0326 --*/
            Xcp_SegmentInfo.segInfoPtr[segNum].size);
        Xcp_ActivPagNum[segNum] = XCP_FLASH_PAGE_NUM;
    }
    return;
}
#endif /*STD_OFF == XCP_SWITCH_PAG_BY_USER*/

/**
 * Handles the XCP `SET_CAL_PAGE` command. This function checks the frame length,
 * program active status, and protection status. If all checks pass, it calls
 * the `Xcp_SetCalPageHal` function to switch the calibration page. If any check
 * fails, it sets an appropriate error code and sends a response.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Xcp_SetCalPage(void)
/* PRQA S 1532 -- */
{
/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_4) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_CAL == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_CAL))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
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

/**
 * Sets the calibration page for a specific segment. This function checks the
 * mode, segment number, and page number. If the mode and segment are valid,
 * it switches the active page to the specified page (either flash or RAM).
 * If user-defined page switching is enabled, it calls the application-specific
 * function to set the calibration page. If any check fails, it sets an appropriate
 * error code.
 */
XCP_LOCAL void Xcp_SetCalPageHal(void)
{
    uint8 mode   = Xcp_CmdBuffer[XCP_CONST_1];
    uint8 segNum = Xcp_CmdBuffer[XCP_CONST_2];
    uint8 pagNum = Xcp_CmdBuffer[XCP_CONST_3];

    if (mode != XCP_SET_PAG_MODE_ALL)
    {
        Xcp_SetErrorCode(XCP_ERR_MODE_NOT_VALID);
    }
    else if (segNum >= Xcp_SegmentInfo.maxSegNum)
    {
        Xcp_SetErrorCode(XCP_ERR_SEGMENT_NOT_VALID);
    }
    else
    {
#if (STD_OFF == XCP_SWITCH_PAG_BY_USER)
        if (Xcp_ActivPagNum[segNum] != pagNum)
        {
            switch (pagNum)
            {
            case XCP_FLASH_PAGE_NUM:
            {
                Xcp_SwitchToRP();
                break;
            }
            case XCP_RAM_PAGE_NUM:
            {
                Xcp_SwitchToWP();
                break;
            }
            default:
            {
                Xcp_SetErrorCode(XCP_ERR_PAGE_NOT_VALID);

                break;
            }
            }
        }
#else
        XcpAppl_SetCalPage(mode, segNum, pagNum);
#endif /*STD_OFF == XCP_SWITCH_PAG_BY_USER*/
    }
    return;
}

/**
 * Handles the XCP `GET_CAL_PAGE` command. This function checks the frame length,
 * program active status, and protection status. It verifies the segment number
 * and access mode. If all checks pass, it retrieves the active page number for
 * the specified segment and prepares the response. If user-defined page switching
 * is enabled, it calls the application-specific function to get the calibration
 * page. If any check fails, it sets an appropriate error code and sends a response.
 */
/* PRQA S 1532,6030 ++ */ /* VL_QAC_OneFunRef,VL_MTR_Xcp_STMIF */
void Xcp_GetCalPage(void)
/* PRQA S 1532,6030 -- */
{
    uint8 accessMode = Xcp_CmdBuffer[XCP_CONST_1];
    uint8 segNum     = Xcp_CmdBuffer[XCP_CONST_2];
/* check frame length */
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_3) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
/* check pgm active status */
#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
        if (Xcp_ChannelPtgmData.PgmStauts != XCP_PGM_IDLE)
    {
        Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
    }
    else
#endif
        /* check protection status */
        if (XCP_PL_CAL == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_CAL))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else if (segNum >= Xcp_SegmentInfo.maxSegNum)
        {
            Xcp_SetErrorCode(XCP_ERR_SEGMENT_NOT_VALID);
        }
        else
        {
#if (STD_OFF == XCP_SWITCH_PAG_BY_USER)
            switch (accessMode)
            {
            case XCP_GET_PAG_MODE_ECU:
            case XCP_GET_PAG_MODE_XCP:
            {
                Xcp_RespBuffer[XCP_CONST_3]      = Xcp_ActivPagNum[segNum];
                Xcp_ChannelCommonData.RespLength = XCP_CONST_4;
                break;
            }
            default:
            {
                Xcp_SetErrorCode(XCP_ERR_MODE_NOT_VALID);

                break;
            }
            }
#else
        XcpAppl_GetCalPage(accessMode, segNum, &Xcp_RespBuffer[3u]);
#endif /*STD_OFF == XCP_SWITCH_PAG_BY_USER*/
        }
    Xcp_SendResp();
    return;
}
#endif /*STD_ON == XCP_PAG_SUPPORT*/

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#endif /*XCP_PL_CAL == XCP_PL_CAL & XCP_RESOURCE*/

/* ========================================== internal function definitions ========================================= */
