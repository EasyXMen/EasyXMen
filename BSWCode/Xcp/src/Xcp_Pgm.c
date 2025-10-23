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
/*
 ************************************************************************************************************************
 **
 **  @file               : Xcp_Pgm.c
 **  @author             : qinchun.yang
 **  @date               : 2024/11/17
 **  @vendor             : isoft
 **  @description        : Implementation of the XCP_Pgm command
 **
 ***********************************************************************************************************************/
/* =================================================== inclusions =================================================== */
#include "Xcp_Internal.h"

#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
/* ===================================================== macros ===================================================== */
/* return code of interface function */
#define XCP_IF_OK     0x00u
#define XCP_IF_NOT_OK 0x01u
#define XCP_IF_BUSY   0x02u
/* mode of nvm/fls interface function */
#define XCP_FLS_ERASE 0x03u
#define XCP_FLS_WRITE 0x04u

/* SIZE Limitation */
#define XCP_PROGRAMMAX_SIZE ((XCP_MAX_CTO_PGM / XCP_AG) - 0x01u)

/* Xcp Access Mode Type */
#define XCP_ABSOLUTE_ACCESS   0x00u
#define XCP_FUNCTIONAL_ACCESS 0x01u

/* Limitation of flash driver */
#define XCP_FLS_DATA_BUFFER_SIZE (XCP_FLS_MIN_WRITE_SIZE + 6u)

#define XCP_FLASHAREA_ALL (XCP_FLASHAREA_CAL | XCP_FLASHAREA_CODE | XCP_FLASHAREA_NVRAM)

/* parameters of GET_PGM_PROCESSOR_INFO */
#define COMPRESSION_SUPPORTED 0x02u
#define COMPRESSION_REQUIRED  0x03u
#define ENCRYPTION_SUPPORTED  0x04u
#define ENCRYPTION_REQUIRED   0x05u
#define NON_SEQ_PGM_SUPPORTED 0x06u
#define NON_SEQ_PGM_REQUIRED  0x07u

#define PGM_PROPERTIES (((uint8)(1u << XCP_ABSOLUTE_ACCESS)) | ((uint8)(1u << XCP_FUNCTIONAL_ACCESS)))

/* Bitmasks for pending task */
#define XCP_CLEAR_TASK            0x01u
#define XCP_CLEAR_PENDING         0x02u
#define XCP_PROGRAM_TASK          0x04u
#define XCP_PROGRAM_PENDING       0x08u
#define XCP_BLOCK_PROGRAM_TASK    0x10u
#define XCP_BLOCK_PROGRAM_PENDING 0x20u

/* mode for get sector info */
#define XCP_GER_SECTOR_INFO_MOD_STAD 0u
#define XCP_GER_SECTOR_INFO_MOD_LEN  1u
#define XCP_GER_SECTOR_INFO_MOD_NAME 2u

#define XCP_PGM_FILLER 0xffu

/*Address Position & Size check wheather in ROM RP*/
/* PRQA S 3472 ++ */ /* VL_Xcp_3472 */
#define CHECK_XCP_FLASH_ROM_MEM(addr, size, sectorNum) \
    (((addr) + (size)) <= (Xcp_SectorInfo[sectorNum].progStartAddr + Xcp_SectorInfo[sectorNum].progDataSize))
/* PRQA S 3472 -- */

/* ================================================ type definitions ================================================ */
/* PRQA S 0722 ++ */ /* VL_Xcp_0722 */
typedef enum
{
    SEQ_OK = 0,
    SEQ_ERROR,
    SEQ_OUTRANGE
} Xcp_SeqCheckType;
/* PRQA S 0722 -- */

typedef uint16 Xcp_PgmBufferIndexType;

/* ========================================== internal function declarations ======================================== */
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
XCP_LOCAL Std_ReturnType Xcp_CheckPgmStatus(void);

XCP_LOCAL Std_ReturnType
    Xcp_FlsReq(uint8 Mode, Xcp_FlsAddressType u4Address, Xcp_FlsLengthType u4Size, const uint8* pu1DataPtr);

XCP_LOCAL uint8 Xcp_FlsGetStatus(void);

XCP_LOCAL Std_ReturnType Xcp_Program_ZeroHandle(void);

XCP_LOCAL Std_ReturnType Xcp_Program_NonZero(uint8 dataCount, uint8 pgmSize);

XCP_LOCAL Xcp_SeqCheckType

    Xcp_GetPgmStartAddr(uint32* startAddr, uint8 noOfBytes);

XCP_LOCAL Xcp_SeqCheckType Xcp_GetPgm_Functional_StartAddr(uint32* startAddr, uint8 noOfBytes, uint32 rangType);

XCP_LOCAL Xcp_SeqCheckType Xcp_GetPgmClearStartAddr(uint32 clearRange, uint32* startAddr, uint32* clearSize);

/* the next six functions used for pending handle */
XCP_LOCAL void Xcp_FlsClearPending(void);

XCP_LOCAL void Xcp_FlsClearTask(void);

XCP_LOCAL void Xcp_FlsProgramPending(void);

XCP_LOCAL void Xcp_FlsProgramTask(void);

#if (XCP_MASTER_BLOCK_MODE == STD_ON)
XCP_LOCAL void Xcp_FlsBlockProgramPending(void);

XCP_LOCAL void Xcp_FlsBlockProgramTask(void);
#endif /*XCP_MASTER_BLOCK_MODE == STD_ON*/

/*
 * Complex Command Handler
 */
XCP_LOCAL void Xcp_ProgramHal(void);
#if (XCP_PROGRAM_MAX == STD_ON)
XCP_LOCAL void Xcp_ProgramMaxHal(void);
#endif
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

/* ============================================ internal data definitions =========================================== */
#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
XCP_LOCAL uint8 Xcp_FlsBuffer[XCP_FLS_DATA_BUFFER_SIZE]; /* PRQA S 3218 */ /* VL_Xcp_3218 */
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

/* ============================================ external data definitions =========================================== */
Xcp_ChannelPgmType Xcp_ChannelPtgmData;

/* ========================================== external function definitions ========================================= */

#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"

/**
 * Checks the PGM (Programming) status. This function verifies the protection status
 * and the current programming status. If the protection is locked or the programming
 * status is idle, it sets an appropriate error code. Otherwise, it returns a success
 * status. This function is intended for internal use.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
XCP_LOCAL Std_ReturnType Xcp_CheckPgmStatus(void)
{
    Std_ReturnType ret = E_NOT_OK;

    /* check the protection status */
    if (XCP_PL_PGM == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_PGM))
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
    }
    else if (XCP_PGM_IDLE == Xcp_ChannelPtgmData.PgmStauts)
    {
        Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
    }
    else
    {
        ret = E_OK;
    }
    return ret;
}

/**
 * Checks the validity of the memory range for programming. This function iterates
 * through the defined sectors and checks if the given address and size fall within
 * any valid sector's range. If a valid sector is found, it returns true; otherwise,
 * it returns false. This function is intended for internal use.
 */
boolean Xcp_CheckPgmMem(uint32 addr, uint32 size)
{
    boolean result = FALSE;
    uint8   secNum;
    uint32  start;
    uint32  end;

    for (secNum = 0u; secNum < XCP_MAX_SECTOR; secNum++)
    {
        start = Xcp_SectorInfo[secNum].progStartAddr;
        end   = Xcp_SectorInfo[secNum].progStartAddr + Xcp_SectorInfo[secNum].progDataSize - 1uL;
        if (Xcp_CheckMemVaild(addr, size, start, end))
        {
            result = TRUE;
            break;
        }
    }
    return result;
}

/**
 * Handles flash requests. This function adjusts the provided address relative to
 * the base address and then calls the appropriate flash operation (write or erase)
 * based on the mode parameter. It returns the result of the flash operation.
 * This function is intended for internal use.
 */
XCP_LOCAL Std_ReturnType
    Xcp_FlsReq(uint8 Mode, Xcp_FlsAddressType u4Address, Xcp_FlsLengthType u4Size, const uint8* pu1DataPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    /* PRQA S 1338 ++ */ /* VL_Xcp_1338 */
    u4Address = u4Address - XCP_FLS_BASEADDR;
    /* PRQA S 1338 -- */
    switch (Mode)
    {
    case XCP_FLS_WRITE:
        ret = Xcp_FlsFuncPtr.flsWrite(u4Address, pu1DataPtr, u4Size);
        break;

    case XCP_FLS_ERASE:
        ret = Xcp_FlsFuncPtr.flsErase(u4Address, u4Size);
        break;

    default:
        /* Do nothing */
        break;
    }
    return ret;
}

/**
 * Gets the status of the flash operation. This function queries the job result
 * from the flash interface and converts it into a status code indicating whether
 * the operation is pending, completed successfully, or failed. This function is
 * intended for internal use.
 */
XCP_LOCAL uint8 Xcp_FlsGetStatus(void)
{
    MemIf_JobResultType flsRet;
    uint8               result;

    flsRet = Xcp_FlsFuncPtr.flsGetJobResult();
    switch (flsRet)
    {
    case MEMIF_JOB_PENDING:
        result = XCP_IF_BUSY;
        break;
    case MEMIF_JOB_OK:
        result = XCP_IF_OK;
        break;
    default:
        result = XCP_IF_NOT_OK;
        break;
    }
    return result;
}

/**
 * Handles the finalization of a programming sequence. This function pads the
 * remaining buffer space with filler values if necessary, updates the buffer
 * size to the minimum write size, and writes the data to the target address.
 * It also updates the programming status and flags. If the buffer is already
 * empty, it marks the programming as finished. This function is intended for
 * internal use.
 */

XCP_LOCAL Std_ReturnType Xcp_Program_ZeroHandle(void)
{
    Xcp_PgmBufferIndexType cnt;
    Std_ReturnType         programRet;

    if (0u != Xcp_ChannelPtgmData.PgmBufSize)
    {
        /* if remained data more than min write size,there should be a error */
        if (Xcp_ChannelPtgmData.PgmBufSize >= XCP_FLS_MIN_WRITE_SIZE)
        {
            programRet = E_NOT_OK;
        }
        else
        {
            for (cnt = 0u; cnt < (XCP_FLS_MIN_WRITE_SIZE - Xcp_ChannelPtgmData.PgmBufSize); cnt++)
            {
                (Xcp_ChannelPtgmData.PgmBufAlignPtr)[Xcp_ChannelPtgmData.PgmBufSize + cnt] = XCP_PGM_FILLER;
            }

            /* set fls buffer size to min write size for pending task */
            Xcp_ChannelPtgmData.PgmBufSize = XCP_FLS_MIN_WRITE_SIZE;

            /* set block seq number or MTA */
            if (XCP_FUNCTIONAL_ACCESS == Xcp_ChannelPtgmData.PgmAccessMode)
            {
                Xcp_ChannelPtgmData.PgmBlkSeqCnt++;
            }
            if (XCP_ABSOLUTE_ACCESS == Xcp_ChannelPtgmData.PgmAccessMode)
            {
                /* PRQA S 1258 ++ */ /* VL_Xcp_1258 */
                Xcp_UpdateMTA((uint32)XCP_FLS_MIN_WRITE_SIZE);
                /* PRQA S 1258 -- */
            }

            /* indicate that this time is the last write of this program sequence */
            Xcp_ChannelPtgmData.PgmLastWriteFlag = TRUE;

            if ((Std_ReturnType)E_OK
                == Xcp_FlsReq(
                    XCP_FLS_WRITE,
                    Xcp_ChannelPtgmData.PgmTargetAddr,
                    XCP_FLS_MIN_WRITE_SIZE,
                    (Xcp_ChannelPtgmData.PgmBufAlignPtr)))
            {
                Xcp_ChannelPtgmData.PgmPendingFlag |= XCP_PROGRAM_PENDING;
            }
            else
            {
                /* set the state to do flash program action */
                Xcp_ChannelPtgmData.PgmPendingFlag |= XCP_PROGRAM_TASK;
            }

            programRet = E_OK;
        }
    }
    else
    {
        /* Xcp_FlsBuffer empty and program finished */
        Xcp_ChannelPtgmData.PgmStauts = XCP_PGM_PROGRAMMED;
        programRet                    = E_OK;
    }
    return programRet;
}

/**
 * Retrieves the starting address for programming. This function determines the
 * starting address based on the access mode (functional or absolute). It checks
 * the selected flash area and the continuity of the address. If the sequence is
 * invalid or the address is out of range, it sets an appropriate error code.
 * This function is intended for internal use.
 */

XCP_LOCAL Xcp_SeqCheckType Xcp_GetPgmStartAddr(uint32* startAddr, uint8 noOfBytes)
{
    Xcp_SeqCheckType programSeqcheckRet   = SEQ_OK;
    boolean          programFlashAreaFind = FALSE;

    /* check the access mode */
    switch (Xcp_ChannelPtgmData.PgmAccessMode)
    {
    case XCP_FUNCTIONAL_ACCESS:
    {
        if (0uL != (Xcp_ChannelPtgmData.PgmAreaSelect & XCP_FLASHAREA_ALL))
        {
            programFlashAreaFind = TRUE;
            programSeqcheckRet =
                Xcp_GetPgm_Functional_StartAddr(startAddr, noOfBytes, Xcp_ChannelPtgmData.PgmAreaSelect);
        }
        break;
    }
    /* Absolute access mode ,then get the clear start address form MTA */
    case XCP_ABSOLUTE_ACCESS:
    {
        /*get address form MTA[0]*/
        *startAddr           = Xcp_Mta2Ptr(Xcp_ChannelCommonData.MTAExtension, Xcp_ChannelCommonData.MTA);
        programFlashAreaFind = TRUE;
        /* check the address */
        if ((0x00u != Xcp_ChannelPtgmData.PgmBufSize)
            && ((Xcp_ChannelPtgmData.PgmBufSize + Xcp_ChannelPtgmData.PgmTargetAddr) != (*startAddr)))
        {
            /* data remained before, but the address not continuous,
             * so clear the size to 0.
             */
            Xcp_ChannelPtgmData.PgmBufSize = 0u;
            programFlashAreaFind           = FALSE;
        }
        break;
    }
    default:
    {
        /*Do nothing*/
        break;
    }
    }
    if (!programFlashAreaFind)
    {
        programSeqcheckRet = SEQ_ERROR;
    }
    if (SEQ_OK != programSeqcheckRet)
    {
        if (SEQ_ERROR == programSeqcheckRet) /* clear sequence ocuured */
        {
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        }
        else /**< SEQ_OUTRANGE == programSeqcheckRet */
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
        }
    }
    return programSeqcheckRet;
}

/**
 * Retrieves the starting address for functional programming. This function
 * searches through the configured sectors to find the correct one based on
 * the range type and clears the start address. It checks the continuity of
 * the address and validates the memory range. If the sector is found and the
 * checks pass, it returns a success status; otherwise, it returns an error
 * status. This function is intended for internal use.
 */
XCP_LOCAL Xcp_SeqCheckType Xcp_GetPgm_Functional_StartAddr(uint32* startAddr, uint8 noOfBytes, uint32 rangType)
{
    uint8            sectorNum;
    boolean          rightSectorFind = FALSE;
    Xcp_SeqCheckType ret             = SEQ_ERROR;

    /* loop all config sectors to find the correct one for current programming */
    for (sectorNum = 0u; sectorNum < XCP_MAX_SECTOR; sectorNum++)
    {
        /* check the sector whether has the right range type */
        if ((Xcp_SectorInfo[sectorNum].rangeType == rangType)
            && (Xcp_ChannelPtgmData.PgmClrStartAddr == Xcp_SectorInfo[sectorNum].sectorclrStartAddr))
        {
            rightSectorFind = TRUE;
            *startAddr      = Xcp_SectorInfo[sectorNum].progStartAddr;

            /* check the address */
            if ((0x00u != Xcp_ChannelPtgmData.PgmBufSize)
                && ((Xcp_ChannelPtgmData.PgmBufSize + Xcp_ChannelPtgmData.PgmTargetAddr) != (*startAddr)))
            {
                /* data remained before, but the address not continuous,
                 * so clear the size to 0.
                 */
                Xcp_ChannelPtgmData.PgmBufSize = 0u;
                ret                            = SEQ_ERROR;
            }
            /* PRQA S 3469 ++ */ /* VL_Xcp_3469 */
            else if (CHECK_XCP_FLASH_ROM_MEM(*startAddr, noOfBytes, sectorNum))
            {
                /* PRQA S 3469 -- */
                ret = SEQ_OK;
                /* Clear the area range flag cause we already find it */
                Xcp_ChannelPtgmData.PgmAreaSelect &= ~(uint32)rangType;
            }
            else
            {
                ret = SEQ_OUTRANGE;
            }
        }
    }
    if (!rightSectorFind)
    {
        ret = SEQ_ERROR;
    }

    return ret;
}

/**
 * Programs non-zero data into the target memory. This function copies the data
 * from the command buffer to the programming buffer, updates the buffer size,
 * and increments the block sequence counter if in functional access mode. It
 * also updates the MTA (Memory Transfer Address) if in absolute access mode.
 * If the buffer size reaches the minimum write size, it initiates a flash
 * write operation. This function is intended for internal use.
 */
XCP_LOCAL Std_ReturnType Xcp_Program_NonZero(uint8 dataCount, uint8 pgmSize)
{
    uint8          dataNo;
    Std_ReturnType result = E_OK;

    if (dataCount <= pgmSize)
    {
        for (dataNo = 0u; dataNo < dataCount; dataNo++)
        {
            if (XCP_PROGRAMMAX_SIZE == pgmSize)
            {
                /* program_Max command copy the Cmdbuffer data to Xcp_FlsBuffer from XCP_AG position */
                (Xcp_ChannelPtgmData.PgmBufAlignPtr)[Xcp_ChannelPtgmData.PgmBufSize + dataNo] =
                    Xcp_CmdBuffer[XCP_AG + dataNo];
            }
            else
            {
#if (XCP_ADDR_GRANULARITY != XCP_AG_DWORD)
                (Xcp_ChannelPtgmData.PgmBufAlignPtr)[Xcp_ChannelPtgmData.PgmBufSize + dataNo] =
                    Xcp_CmdBuffer[XCP_CONST_2 + dataNo];
#else
                (Xcp_ChannelPtgmData.PgmBufAlignPtr)[Xcp_ChannelPtgmData.PgmBufSize + dataNo] =
                    Xcp_CmdBuffer[XCP_AG + dataNo];
#endif
            }
        }
        Xcp_ChannelPtgmData.PgmBufSize += dataCount;

        if (XCP_FUNCTIONAL_ACCESS == Xcp_ChannelPtgmData.PgmAccessMode)
        {
            Xcp_ChannelPtgmData.PgmBlkSeqCnt++;
        }
        else if (XCP_ABSOLUTE_ACCESS == Xcp_ChannelPtgmData.PgmAccessMode)
        {
            Xcp_UpdateMTA((uint32)dataCount);
        }
        else
        {
            /* Do Nothing */
        }

        /* check the buffer size , bigger than XCP_FLS_MIN_WRITE_SIZE, then do write */
        if (Xcp_ChannelPtgmData.PgmBufSize >= XCP_FLS_MIN_WRITE_SIZE)
        {
            if ((Std_ReturnType)E_OK
                == Xcp_FlsReq(
                    XCP_FLS_WRITE,
                    Xcp_ChannelPtgmData.PgmTargetAddr,
                    XCP_FLS_MIN_WRITE_SIZE,
                    (Xcp_ChannelPtgmData.PgmBufAlignPtr)))
            {
                Xcp_ChannelPtgmData.PgmPendingFlag |= XCP_PROGRAM_PENDING;
            }
            else
            {
                Xcp_ChannelPtgmData.PgmPendingFlag |= XCP_PROGRAM_TASK;
            }
        }
    }
    /* write data size bigger than pgmSize */
    else
    {
        result = E_NOT_OK;
    }
    return result;
}

/**
 * Retrieves the starting address and size for clearing memory. This function
 * determines the clear start address and size based on the access mode (functional
 * or absolute). For functional access, it selects the flash area to clear and
 * retrieves the start address and size from the sector information. For absolute
 * access, it checks the memory range and retrieves the start address and size
 * from the MTA (Memory Transfer Address). If the clear area is not found or
 * the access mode is invalid, it returns an error status. This function is
 * intended for internal use.
 */

XCP_LOCAL Xcp_SeqCheckType Xcp_GetPgmClearStartAddr(uint32 clearRange, uint32* startAddr, uint32* clearSize)
{
    uint8            sectorNum;
    Xcp_SeqCheckType result = SEQ_OK;

    /* functional access mode ,then calculate the clear start address */
    if (XCP_FUNCTIONAL_ACCESS == Xcp_ChannelPtgmData.PgmClrAccessMode)
    {
        /* select the flash area need to clear,
         * Provide the flag for PROGRAM used when in functional access mode
         */
        Xcp_ChannelPtgmData.PgmAreaSelect |= clearRange;

        for (sectorNum = 0u; sectorNum < XCP_MAX_SECTOR; sectorNum++)
        {
            /* get the clear start address by clear range */
            if ((uint32)Xcp_SectorInfo[sectorNum].rangeType == clearRange)
            {
                /* Get the info for clear */
                *startAddr = Xcp_SectorInfo[sectorNum].sectorclrStartAddr;
                *clearSize = Xcp_SectorInfo[sectorNum].sectorclrLength;
                break;
            }
        }
        /* Do not find the clear area*/
        if (sectorNum == XCP_MAX_SECTOR)
        {
            result = SEQ_ERROR;
        }
    }
    /* Absolute access mode ,then get the clear start address form MTA */
    else if (XCP_ABSOLUTE_ACCESS == Xcp_ChannelPtgmData.PgmClrAccessMode)
    {
        if (Xcp_CheckPgmMem(Xcp_Mta2Ptr(Xcp_ChannelCommonData.MTAExtension, Xcp_ChannelCommonData.MTA), clearRange))
        {
            *startAddr = Xcp_Mta2Ptr(Xcp_ChannelCommonData.MTAExtension, Xcp_ChannelCommonData.MTA);
            *clearSize = clearRange;
        }
        else
        {
            result = SEQ_ERROR;
        }
    }
    else
    {
        /* neither ABSOLUTE nor FUNCTIONAL mode */
        result = SEQ_OUTRANGE;
    }
    return result;
}

/**
 * Handles the completion of a flash clear operation. This function checks the
 * status of the flash operation. If the operation is successful, it updates the
 * pending flag, resets the buffer size, sets the programming status to cleared,
 * and sends a response. If the operation fails, it updates the pending flag,
 * resets the buffer size, sets an error code, and sends a response. This function
 * is intended for internal use.
 */
XCP_LOCAL void Xcp_FlsClearPending(void)
{
    uint8 result;
    result = Xcp_FlsGetStatus();
    switch (result)
    {
    case XCP_IF_OK:
        Xcp_ChannelPtgmData.PgmPendingFlag &= (uint8)(~XCP_CLEAR_PENDING);
        Xcp_ChannelPtgmData.PgmBufSize = 0u;
        /* set the program status as XCP_PGM_CLEARED */
        Xcp_ChannelPtgmData.PgmStauts = XCP_PGM_CLEARED;
        Xcp_SendResp();
        break;
    case XCP_IF_NOT_OK:
        Xcp_ChannelPtgmData.PgmPendingFlag &= (uint8)(~XCP_CLEAR_PENDING);
        Xcp_ChannelPtgmData.PgmBufSize = 0u;

        /* Do not set the program status because clear failed */

        Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);

        Xcp_SendResp();
        break;
    default:
        /*Do nothing*/
        break;
    }
    return;
}

/**
 * Initiates a flash clear operation. This function sends an erase request to
 * the flash interface. If the request is successful, it resets the retry count,
 * clears the task flag, and sets the pending flag. If the request fails, it
 * increments the retry count and, if the maximum retries are reached, sets an
 * error code and sends a response. This function is intended for internal use.
 */
XCP_LOCAL void Xcp_FlsClearTask(void)
{
    if ((Std_ReturnType)E_OK
        == Xcp_FlsReq(XCP_FLS_ERASE, Xcp_ChannelPtgmData.PgmClrStartAddr, Xcp_ChannelPtgmData.PgmClrSize, NULL_PTR))
    {
        Xcp_ChannelPtgmData.PgmRetryCnt = 0u;
        Xcp_ChannelPtgmData.PgmPendingFlag &= (uint8)(~XCP_CLEAR_TASK);
        Xcp_ChannelPtgmData.PgmPendingFlag |= XCP_CLEAR_PENDING;
    }
    else
    {
        Xcp_ChannelPtgmData.PgmRetryCnt++;
        if (Xcp_ChannelPtgmData.PgmRetryCnt >= XCP_PGM_REQRETRIE)
        {
            Xcp_ChannelPtgmData.PgmRetryCnt = 0u;
            Xcp_ChannelPtgmData.PgmPendingFlag &= (uint8)(~XCP_CLEAR_TASK);
            Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);

            Xcp_SendResp();
        }
        else
        {
            /* do nothing */
        }
    }
    return;
}

/**
 * Handles the completion of a pending flash programming operation. This function
 * checks the status of the flash operation. If the operation is successful, it
 * updates the pending flag, advances the target address, and shifts the remaining
 * data in the buffer. If the operation is the last write, it marks the programming
 * as completed and sends a response. If the operation fails, it resets the buffer
 * size, sets the programming status back to the start, sets an error code, and
 * sends a response. This function is intended for internal use.
 */
XCP_LOCAL void Xcp_FlsProgramPending(void)
{
    uint8                  result;
    Xcp_PgmBufferIndexType cnt;
    result = Xcp_FlsGetStatus();
    switch (result)
    {
    case XCP_IF_OK:
        Xcp_ChannelPtgmData.PgmPendingFlag &= (uint8)(~XCP_PROGRAM_PENDING);

        /* PRQA S 1252 ++ */ /* VL_Xcp_1252 */
        Xcp_ChannelPtgmData.PgmTargetAddr += XCP_FLS_MIN_WRITE_SIZE;
        /* PRQA S 1252 -- */
        Xcp_ChannelPtgmData.PgmBufSize -= XCP_FLS_MIN_WRITE_SIZE;

        if (Xcp_ChannelPtgmData.PgmLastWriteFlag)
        {
            Xcp_ChannelPtgmData.PgmStauts = XCP_PGM_PROGRAMMED;

            /* program completed, clear buffer size */
            Xcp_ChannelPtgmData.PgmBufSize = 0x0u;

            /* clear the last write flag */
            Xcp_ChannelPtgmData.PgmLastWriteFlag = FALSE;
        }
        /* Copy more than XCP_FLS_MIN_WRITE_SIZE bytes after the data */
        for (cnt = 0u; cnt < Xcp_ChannelPtgmData.PgmBufSize; cnt++)
        {
            (Xcp_ChannelPtgmData.PgmBufAlignPtr)[cnt] =
                (Xcp_ChannelPtgmData.PgmBufAlignPtr)[cnt + XCP_FLS_MIN_WRITE_SIZE];
        }
        if (Xcp_ChannelPtgmData.PgmBufSize >= XCP_FLS_MIN_WRITE_SIZE)
        {
            Xcp_ChannelPtgmData.PgmPendingFlag |= XCP_PROGRAM_TASK;
        }
        else
        {
            Xcp_SendResp();
        }
        break;
    case XCP_IF_NOT_OK:
        Xcp_ChannelPtgmData.PgmPendingFlag &= (uint8)(~XCP_PROGRAM_PENDING);
        Xcp_ChannelPtgmData.PgmBufSize = 0u;

        /* set the program status back to XCP_PGM_START because program failed, and Need to be re-erased */
        Xcp_ChannelPtgmData.PgmStauts = XCP_PGM_START;

        Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);

        Xcp_SendResp();
        break;
    default:
        /*Do nothing*/
        break;
    }
    return;
}

/**
 * Initiates a flash programming task. This function sends a write request to
 * the flash interface. If the request is successful, it resets the retry count,
 * clears the task flag, and sets the pending flag. If the request fails, it
 * increments the retry count and, if the maximum retries are reached, sets an
 * error code, resets the buffer size, and sends a response. This function is
 * intended for internal use.
 */
XCP_LOCAL void Xcp_FlsProgramTask(void)
{
    if ((Std_ReturnType)E_OK
        == Xcp_FlsReq(
            XCP_FLS_WRITE,
            Xcp_ChannelPtgmData.PgmTargetAddr,
            XCP_FLS_MIN_WRITE_SIZE,
            (Xcp_ChannelPtgmData.PgmBufAlignPtr)))
    {
        Xcp_ChannelPtgmData.PgmRetryCnt = 0u;
        Xcp_ChannelPtgmData.PgmPendingFlag &= (uint8)(~XCP_PROGRAM_TASK);
        Xcp_ChannelPtgmData.PgmPendingFlag |= XCP_PROGRAM_PENDING;
    }
    else
    {
        Xcp_ChannelPtgmData.PgmRetryCnt++;
        if (Xcp_ChannelPtgmData.PgmRetryCnt >= XCP_PGM_REQRETRIE)
        {
            Xcp_ChannelPtgmData.PgmRetryCnt = 0u;
            Xcp_ChannelPtgmData.PgmPendingFlag &= (uint8)(~XCP_PROGRAM_TASK);
            Xcp_ChannelPtgmData.PgmBufSize = 0u;
            Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);

            Xcp_SendResp();
        }
        else
        {
            /* counter no expired , do nothing */
        }
    }
    return;
}

#if (XCP_MASTER_BLOCK_MODE == STD_ON)
/**
 * Handles the completion of a block programming operation. This function checks
 * the status of the flash operation. If the operation is successful, it updates
 * the pending flag, advances the target address, and continues writing the next
 * block of data if there is more data to write. If the operation is the last
 * write, it sends a response. If the operation fails, it resets the buffer sizes,
 * sets the programming status back to the start, sets an error code, and sends
 * a response. This function is intended for internal use.
 */
XCP_LOCAL void Xcp_FlsBlockProgramPending(void)
{
    uint8                  result;
    Xcp_PgmBufferIndexType cnt;
    result = Xcp_FlsGetStatus();
    switch (result)
    {
    case XCP_IF_OK:
        Xcp_ChannelPtgmData.PgmPendingFlag &= (uint8)(~XCP_BLOCK_PROGRAM_PENDING);

        /* set the program status as XCP_PGM_PROGRAMMING */
        Xcp_ChannelPtgmData.PgmStauts = XCP_PGM_PROGRAMMING;
        /* PRQA S 1252 ++ */ /* VL_Xcp_1252 */
        Xcp_ChannelPtgmData.PgmTargetAddr += XCP_FLS_MIN_WRITE_SIZE;
        /* PRQA S 1252 -- */
        /* if remained data more than min write size ,then continue write*/
        if ((XCP_FLS_MIN_WRITE_SIZE + Xcp_ChannelCommonData.BlockBufPos) <= Xcp_ChannelCommonData.BlockBuflen)
        {
            for (cnt = 0u; cnt < XCP_FLS_MIN_WRITE_SIZE; cnt++)
            {
                /* PRQA S 2841 ++ */ /* VL_Xcp_2841 */
                (Xcp_ChannelPtgmData.PgmBufAlignPtr)[cnt] = Xcp_BlockBuffer[Xcp_ChannelCommonData.BlockBufPos + cnt];
                /* PRQA S 2841 -- */
            }
            /* set fls buffer size to min write size */
            Xcp_ChannelPtgmData.PgmBufSize = XCP_FLS_MIN_WRITE_SIZE;

            Xcp_ChannelCommonData.BlockBufPos += XCP_FLS_MIN_WRITE_SIZE;

            Xcp_ChannelPtgmData.PgmPendingFlag |= XCP_BLOCK_PROGRAM_TASK;
            /* PRQA S 1258 ++ */ /* VL_Xcp_1258 */
            Xcp_UpdateMTA((uint32)XCP_FLS_MIN_WRITE_SIZE);
            /* PRQA S 1258 ++ */ /* VL_Xcp_1258 */
        }
        else
        {
            cnt = 0u;
            while (Xcp_ChannelCommonData.BlockBuflen != Xcp_ChannelCommonData.BlockBufPos)
            {
                (Xcp_ChannelPtgmData.PgmBufAlignPtr)[cnt] = Xcp_BlockBuffer[Xcp_ChannelCommonData.BlockBufPos];
                Xcp_ChannelCommonData.BlockBufPos++;
                cnt++;
            }
            Xcp_ChannelPtgmData.PgmBufSize = cnt;
            Xcp_UpdateMTA((uint32)cnt);

            Xcp_SendResp();
        }

        break;
    case XCP_IF_NOT_OK:
        Xcp_ChannelPtgmData.PgmPendingFlag &= (uint8)(~XCP_BLOCK_PROGRAM_PENDING);

        /* set the program status back to XCP_PGM_START because program failed */
        Xcp_ChannelPtgmData.PgmStauts     = XCP_PGM_START;
        Xcp_ChannelCommonData.BlockBuflen = 0u;
        Xcp_ChannelCommonData.BlockBufPos = 0u;
        Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);

        Xcp_SendResp();
        break;
    default:
        /*Do nothing*/
        break;
    }
    return;
}

/**
 * Initiates a block programming task. This function sends a write request to
 * the flash interface. If the request is successful, it resets the retry count,
 * clears the task flag, and sets the pending flag. If the request fails, it
 * increments the retry count and, if the maximum retries are reached, resets
 * the buffer length, sets an error code, and sends a response. This function
 * is intended for internal use.
 */
XCP_LOCAL void Xcp_FlsBlockProgramTask(void)
{
    Std_ReturnType result;

    result = Xcp_FlsReq(
        XCP_FLS_WRITE,
        Xcp_ChannelPtgmData.PgmTargetAddr,
        XCP_FLS_MIN_WRITE_SIZE,
        (Xcp_ChannelPtgmData.PgmBufAlignPtr));

    if ((Std_ReturnType)E_OK == result)
    {
        Xcp_ChannelPtgmData.PgmRetryCnt = 0u;
        Xcp_ChannelPtgmData.PgmPendingFlag &= (uint8)(~XCP_BLOCK_PROGRAM_TASK);
        Xcp_ChannelPtgmData.PgmPendingFlag |= XCP_BLOCK_PROGRAM_PENDING;
    }
    else
    {
        Xcp_ChannelPtgmData.PgmRetryCnt++;
        if (Xcp_ChannelPtgmData.PgmRetryCnt >= XCP_PGM_REQRETRIE)
        {
            Xcp_ChannelPtgmData.PgmRetryCnt = 0u;
            Xcp_ChannelPtgmData.PgmPendingFlag &= (uint8)(~XCP_BLOCK_PROGRAM_TASK);
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
            Xcp_ChannelCommonData.BlockBuflen = 0u;
#endif
            Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);

            Xcp_SendResp();
        }
        else
        {
            /* counter no expired, do nothing */
        }
    }
    return;
}
#endif /*XCP_MASTER_BLOCK_MODE == STD_ON*/

/**
 * Initializes the programming module. This function clears all pending task flags,
 * initializes the flash buffer with a maximum value, aligns the buffer pointer
 * if alignment is enabled, and resets the buffer size, target address, last write
 * flag, and programming status. This function is intended for internal use.
 */

void Xcp_ProgramInit(void)

{
    Xcp_PgmBufferIndexType cnt;
    /*clear all pending task flag*/
    Xcp_ChannelPtgmData.PgmPendingFlag = 0x00u;

    for (cnt = 0u; cnt < XCP_FLS_DATA_BUFFER_SIZE; cnt++)
    {
        Xcp_FlsBuffer[cnt] = XCP_U8_MAX;
    }

    cnt = 0u;
#if (XCP_ACCESS_ALIGN_ENABLE == STD_ON)
    /* PRQA S 0306,0309 ++ */ /* VL_Xcp_0306,VL_Xcp_0309 */
    while (((uint32)(&Xcp_FlsBuffer[cnt]) % XCP_CONST_4L) != 0uL)
    /* PRQA S 0306,0309 -- */
    {
        cnt++;
    }
#endif
    (Xcp_ChannelPtgmData.PgmBufAlignPtr) = &Xcp_FlsBuffer[cnt];
    Xcp_ChannelPtgmData.PgmBufSize       = 0u;

    Xcp_ChannelPtgmData.PgmTargetAddr = 0u;

    /* Last write flag will be set while a program zero occurred */
    Xcp_ChannelPtgmData.PgmLastWriteFlag = FALSE;
    /*init the pgm statue to IDLE*/
    Xcp_ChannelPtgmData.PgmStauts = XCP_PGM_IDLE;
    return;
}

/**
 * Handles pending tasks for programming operations. This function checks the
 * pending task flag and calls the appropriate function to handle the specific
 * task, such as clearing or programming the flash. This function is intended
 * for internal use.
 */

void Xcp_PgmPendingTask(void)
{
    switch (Xcp_ChannelPtgmData.PgmPendingFlag)
    {
    case XCP_CLEAR_PENDING:
    {
        Xcp_FlsClearPending();
        break;
    }
    case XCP_CLEAR_TASK:
    {
        Xcp_FlsClearTask();
        break;
    }
    case XCP_PROGRAM_PENDING:
    {
        Xcp_FlsProgramPending();
        break;
    }
    case XCP_PROGRAM_TASK:
    {
        Xcp_FlsProgramTask();
        break;
    }
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
    case XCP_BLOCK_PROGRAM_PENDING:
    {
        Xcp_FlsBlockProgramPending();
        break;
    }
    case XCP_BLOCK_PROGRAM_TASK:
    {
        Xcp_FlsBlockProgramTask();
        break;
    }
#endif /* XCP_MASTER_BLOCK_MODE == STD_ON */
    default:
        /*Do nothing*/
        break;
    }
    return;
}

/**
 * Starts the programming process. This function checks the command length, DAQ
 * running status, and protection status. If all checks pass, it sets the response
 * buffer with programming capabilities and starts the programming process. If any
 * check fails, it sets an appropriate error code. This function is intended for
 * internal use.
 */
void Xcp_ProgramStart(void)
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != 1u) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
    {
        /* check whether DAQ Running at this moment */
        if (XCP_SESSION_STATE_DAQ_RUNNING == (Xcp_ChannelCommonData.SessionStatus & XCP_SESSION_STATE_DAQ_RUNNING))
        {
            Xcp_SetErrorCode(XCP_ERR_DAQ_ACTIVE);
        }
        /* check the protection status */
        else if (XCP_PL_PGM == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_PGM))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else
        {
            Xcp_RespBuffer[XCP_CONST_2] = 0u;
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
            Xcp_RespBuffer[XCP_CONST_2] |= XCP_CONST_1;
#endif
#if (XCP_INTERLEAVED_MODE == STD_ON)
            Xcp_RespBuffer[XCP_CONST_2] |= XCP_CONST_2;
#endif
#if (XCP_SLAVE_BLOCK_MODE == STD_ON)
            Xcp_RespBuffer[XCP_CONST_2] |= XCP_CONST_64;
#endif
            Xcp_RespBuffer[XCP_CONST_3] = XCP_MAX_CTO_PGM;
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
            Xcp_RespBuffer[XCP_CONST_4] = XCP_MAX_BS_PGM;
            Xcp_RespBuffer[XCP_CONST_5] = XCP_MIN_ST_PGM;
#else
            Xcp_RespBuffer[XCP_CONST_4] = 0u;
            Xcp_RespBuffer[XCP_CONST_5] = 0u;
#endif
#if (XCP_INTERLEAVED_MODE == STD_ON)
            Xcp_RespBuffer[XCP_CONST_6] = XCP_QUEUE_SIZE_PGM;
#endif
            Xcp_ChannelCommonData.RespLength = XCP_CONST_7;
            Xcp_ChannelPtgmData.PgmStauts    = XCP_PGM_START;
        }
    }
    Xcp_SendResp();
    return;
}

/**
 * Clears a specified range of the target memory. This function checks the command
 * syntax, programming status, and access mode. It calculates the start address
 * and size for the clear operation, sends an erase request to the flash interface,
 * and sets the pending flag accordingly. If the operation cannot proceed due to
 * errors, it sets an appropriate error code and sends a response. This function
 * is intended for internal use.
 */
void Xcp_ProgramClear(void)
{
    uint32           startAddr;
    uint32           programClearRange;
    Xcp_SeqCheckType seqResult;

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if (Xcp_ChannelCommonData.CmdLength != XCP_CONST_8)
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
    {
        if ((Std_ReturnType)E_OK == Xcp_CheckPgmStatus())
        { /* get the clear mode from CommandRxObject[1] buffer */
            Xcp_ChannelPtgmData.PgmClrAccessMode = Xcp_CmdBuffer[XCP_CONST_1];

            /* get the clear size from CommandRxObject[4]-[7] buffer */
            Xcp_CopyU1BufferToU4(&Xcp_CmdBuffer[XCP_CONST_4], &programClearRange, (uint8)CPU_BYTE_ORDER);
            seqResult = Xcp_GetPgmClearStartAddr(programClearRange, &startAddr, &Xcp_ChannelPtgmData.PgmClrSize);
            switch (seqResult)
            {
            case SEQ_OK:
            {
                Xcp_ChannelPtgmData.PgmClrStartAddr = startAddr;
                /* clear Flash it will be done in the pengding task*/
                if ((Std_ReturnType)E_OK
                    == Xcp_FlsReq(
                        XCP_FLS_ERASE,
                        Xcp_ChannelPtgmData.PgmClrStartAddr,
                        Xcp_ChannelPtgmData.PgmClrSize,
                        NULL_PTR))
                {
                    Xcp_ChannelPtgmData.PgmPendingFlag |= XCP_CLEAR_PENDING;
                }
                else
                {
                    Xcp_ChannelPtgmData.PgmPendingFlag |= XCP_CLEAR_TASK;
                }
                break;
            }
            case SEQ_OUTRANGE:
            {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);

                break;
            }
            case SEQ_ERROR:
            {
                Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);

                break;
            }
            default:
            {
                /* Do Nothing */
                break;
            }
            }
        }
    }
    if (0u == Xcp_ChannelPtgmData.PgmPendingFlag)
    {
        Xcp_SendResp();
    }
    return;
}

/**
 * Programs data into the target memory. This function checks the current programming
 * status and performs the necessary sequence checks to determine the start address.
 * It then programs the data, updates the programming status, and handles different
 * access modes (absolute or functional). If the operation cannot proceed due to
 * errors, it sets an appropriate error code. This function is intended for internal
 * use.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Xcp_STMIF */
XCP_LOCAL void Xcp_ProgramHal(void)
/* PRQA S 6030 -- */
{
    Std_ReturnType   result = E_OK;
    Xcp_SeqCheckType seqResult;
    uint8            programNumberOfData;
    uint8            NumOfBytes;
    uint32           programStartAddress = 0u;

    /* get the program size from CommandRxObject[1] buffer */
    programNumberOfData = Xcp_CmdBuffer[1u];
#if (XCP_ADDR_GRANULARITY != XCP_AG_DWORD)
    /* PRQA S 2985 ++ */ /* VL_Xcp_2985 */
    NumOfBytes = XCP_CONST_2 + (programNumberOfData * XCP_AG);
/* PRQA S 2985 -- */
#else
    NumOfBytes = XCP_CONST_4 + (programNumberOfData * XCP_AG);
#endif
    /* The cleanup operation must be performed before programming,so the Xcp_ChannelPtgmData.PgmStauts will be
     * XCP_PGM_CLEARED or XCP_PGM_PROGRAMMING */
    if (XCP_PGM_CLEARED == Xcp_ChannelPtgmData.PgmStauts)
    {
        /* do porgram sequence check and get program start address */
        seqResult = Xcp_GetPgmStartAddr(&programStartAddress, NumOfBytes);
        if (SEQ_OK == seqResult)
        {
            Xcp_ChannelPtgmData.PgmTargetAddr = programStartAddress;
            result                            = Xcp_Program_NonZero(programNumberOfData, XCP_PROGRAM_SIZE);
            if ((Std_ReturnType)E_OK == result)
            {
                /* set the program status as XCP_PGM_PROGRAMMING */
                Xcp_ChannelPtgmData.PgmStauts = XCP_PGM_PROGRAMMING;
            }
            else
            {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            }
        }
    }
    else if (XCP_PGM_PROGRAMMING == Xcp_ChannelPtgmData.PgmStauts)
    {
        switch (Xcp_ChannelPtgmData.PgmAccessMode)
        {
        case XCP_ABSOLUTE_ACCESS:
        {
            /*get address form MTA[0]*/
            programStartAddress = Xcp_Mta2Ptr(Xcp_ChannelCommonData.MTAExtension, Xcp_ChannelCommonData.MTA);
            /*if Xcp_ChannelPtgmData.PgmBufSize is 0, set Xcp_ChannelPtgmData.PgmTargetAddr for calibration
             * software(e.g. CANAPE) optimize */
            if (0x00u == Xcp_ChannelPtgmData.PgmBufSize)
            {
                Xcp_ChannelPtgmData.PgmTargetAddr = programStartAddress;
                result                            = Xcp_Program_NonZero(programNumberOfData, XCP_PROGRAM_SIZE);
            }
            else if ((Xcp_ChannelPtgmData.PgmBufSize + Xcp_ChannelPtgmData.PgmTargetAddr) != programStartAddress)
            {
                /* Xcp_ChannelPtgmData.PgmBufSize != 0, but the address not continuous,
                 * so clear the size to 0.
                 */
                Xcp_ChannelPtgmData.PgmBufSize = 0u;
                Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
            }
            else
            {
                result = Xcp_Program_NonZero(programNumberOfData, XCP_PROGRAM_SIZE);
            }
            break;
        }
        case XCP_FUNCTIONAL_ACCESS:
        {
            result = Xcp_Program_NonZero(programNumberOfData, XCP_PROGRAM_SIZE);
            break;
        }
        default:
        {
            result = E_NOT_OK;
            break;
        }
        }
        if ((Std_ReturnType)E_NOT_OK == result)
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        }
    }
    else
    {
        /* This address is not erased */
        Xcp_SetErrorCode(XCP_ERR_WRITE_PROTECTED);
    }
    return;
}

/**
 * Handles the programming of data into the target memory. This function checks
 * the command length, protection status, programming status, and data size.
 * It either handles the end of a memory segment or delegates the actual programming
 * to the `Xcp_ProgramHal` function. If any checks fail, it sets an appropriate
 * error code and sends a response. This function is intended for internal use.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Xcp_STMIF */
void Xcp_Program(void)
/* PRQA S 6030 -- */

{
    Std_ReturnType result = E_OK;
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if (
#if (XCP_ADDR_GRANULARITY != XCP_AG_DWORD)
        /* PRQA S 2985 ++ */ /* VL_Xcp_2985 */
        (Xcp_ChannelCommonData.CmdLength != (XCP_CONST_2 + (Xcp_CmdBuffer[1u] * XCP_AG)))
/* PRQA S 2985 -- */
#else
        (Xcp_ChannelCommonData.CmdLength != (4u + (Xcp_CmdBuffer[1u] * XCP_AG)))
#endif
        && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
        /* check the protection status */
        if (XCP_PL_PGM == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_PGM))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        /* check the program status */
        else if ((XCP_PGM_IDLE == Xcp_ChannelPtgmData.PgmStauts) || (XCP_PGM_START == Xcp_ChannelPtgmData.PgmStauts))
        {
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        }
        /* check out of range */
        else if (Xcp_CmdBuffer[1u] > XCP_PROGRAM_SIZE)
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        }
        else
        {
            switch (Xcp_CmdBuffer[1u])
            {
            case 0u:
            {
                /*The end of the memory segment is indicated,
                 * when the number of data elements is 0.*/
                result = Xcp_Program_ZeroHandle();
                break;
            }
            default:
            {
                Xcp_ProgramHal();
                break;
            }
            }
        }
    if ((Std_ReturnType)E_NOT_OK == result)
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    if (0u == Xcp_ChannelPtgmData.PgmPendingFlag)
    {
        Xcp_SendResp();
    }
    return;
}

/**
 * Resets the programming process. This function checks the command length, protection
 * status, and flash status. If all checks pass, it sets the programming status
 * to idle and disconnects the hardware abstraction layer. If any checks fail, it
 * sets an appropriate error code and sends a response. This function is intended
 * for internal use.
 */
void Xcp_ProgramReset(void)
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
    {
        /* check the protection status */
        if (XCP_PL_PGM == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_PGM))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        /* check the flash status */
        else if (XCP_IF_BUSY == Xcp_FlsGetStatus())
        {
            Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
        }
        else
        {
            /* set status to XCP_PGM_IDLE because it's the end of program sequence */
            Xcp_ChannelPtgmData.PgmStauts = XCP_PGM_IDLE;

            Xcp_DisconnectHal();
            Xcp_ChannelCommonData.RespLength = 0x01u;
#if (XCP_ON_ETHERNET_ENABLE == STD_ON)
            normalExit = 0u;
#endif /*XCP_ON_ETHERNET_ENABLE == STD_ON*/
        }
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

/* optional */
#if (XCP_GET_PGM_PROCESSOR_INFO == STD_ON)
/**
 * Retrieves the programming processor information. This function checks the command
 * length and protection status. If all checks pass, it sets the response buffer
 * with the maximum sector count and other properties. If any checks fail, it
 * sets an appropriate error code and sends a response. This function is intended
 * for internal use.
 */
void Xcp_GetPgmProcessorInfo(void)
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != 1u) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
    {
        /* check the protection status */
        if (XCP_PL_PGM == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_PGM))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else
        {
            Xcp_RespBuffer[0u]               = XCP_U8_MAX;
            Xcp_RespBuffer[XCP_CONST_1]      = PGM_PROPERTIES;
            Xcp_RespBuffer[XCP_CONST_2]      = XCP_MAX_SECTOR;
            Xcp_ChannelCommonData.RespLength = XCP_CONST_3;
        }
    }
    Xcp_SendResp();
    return;
}
#endif

#if (XCP_GET_SECTOR_INFO == STD_ON)
/**
 * Retrieves information about a specific sector. This function checks the command
 * length, protection status, and sector number. It then retrieves the requested
 * sector information based on the mode (start address, length, or name length)
 * and sets the response buffer accordingly. If any checks fail, it sets an
 * appropriate error code and sends a response. This function is intended for
 * internal use.
 */
void Xcp_GetSectorInfo(void)
{
    uint8 getSectorMode = Xcp_CmdBuffer[XCP_CONST_1];
    uint8 getsectorNum  = Xcp_CmdBuffer[XCP_CONST_2];

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_3) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
        /* check the protection status */
        if (XCP_PL_PGM == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_PGM))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        else if (getsectorNum >= XCP_MAX_SECTOR)
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        }
        else
        {
            switch (getSectorMode)
            {
            /* get start address for this SECTOR mode = 0u */
            case XCP_GER_SECTOR_INFO_MOD_STAD:
            {
                Xcp_RespBuffer[XCP_CONST_1] = Xcp_SectorInfo[getsectorNum].sectorclrSequenceNo;
                Xcp_RespBuffer[XCP_CONST_2] = Xcp_SectorInfo[getsectorNum].sectorPgmSequenceNo;
                Xcp_RespBuffer[XCP_CONST_3] = Xcp_SectorInfo[getsectorNum].sectorPgmMethod;

                /* if mode = 0,get the start address of the sector */
                Xcp_CopyU4ToU1Buffer(
                    Xcp_SectorInfo[getsectorNum].sectorclrStartAddr,
                    &(Xcp_RespBuffer[XCP_CONST_4]),
                    (uint8)CPU_BYTE_ORDER);
                Xcp_ChannelCommonData.RespLength = XCP_CONST_8;
                break;
            }
            /* get length of this SECTOR [BYTE] mode = 1u*/
            case XCP_GER_SECTOR_INFO_MOD_LEN:
            {
                Xcp_RespBuffer[XCP_CONST_1] = Xcp_SectorInfo[getsectorNum].sectorclrSequenceNo;
                Xcp_RespBuffer[XCP_CONST_2] = Xcp_SectorInfo[getsectorNum].sectorPgmSequenceNo;
                Xcp_RespBuffer[XCP_CONST_3] = Xcp_SectorInfo[getsectorNum].sectorPgmMethod;

                /* get the length of the sector */
                Xcp_CopyU4ToU1Buffer(
                    Xcp_SectorInfo[getsectorNum].sectorclrLength,
                    &(Xcp_RespBuffer[XCP_CONST_4]),
                    (uint8)CPU_BYTE_ORDER);
                Xcp_ChannelCommonData.RespLength = XCP_CONST_8;
                break;
            }
            /* get name length of this SECTOR */
            case XCP_GER_SECTOR_INFO_MOD_NAME:
            {
                /* command automatically sets the Memory Transfer Address (MTA) to
                 * the location from which the Master
                 */
                Xcp_RespBuffer[XCP_CONST_1] = Xcp_SectorInfo[getsectorNum].sectorNameLength;

                Xcp_ChannelCommonData.RespLength = XCP_CONST_2;

                /* Set the MTA so that the master can upload the ID string. */
                /* PRQA S 0306,0309 ++ */ /* VL_Xcp_0306,VL_Xcp_0309 */
                Xcp_ChannelCommonData.MTA = (uint32)(Xcp_SectorInfo[getsectorNum].sectorNamePtr);
                /* PRQA S 0306,0309 -- */
                Xcp_ChannelCommonData.MTAExtension = 0x0u;
                break;
            }
            default:
            {
                Xcp_SetErrorCode(XCP_ERR_MODE_NOT_VALID);

                break;
            }
            }
        }
    Xcp_SendResp();
    return;
}
#endif

#if (XCP_PROGRAM_PREPARE == STD_ON)
/**
 * Prepares the system for programming. This function checks the command length.
 * Currently, this function does not perform any actions but sends a response.
 * If the command length is incorrect, it sets an appropriate error code and
 * sends a response. This function is intended for internal use.
 */
void Xcp_ProgramPrepare(void)
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_6) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
    {
        /* Do Nothing current not supported*/
    }
    Xcp_SendResp();
    return;
}
#endif

#if (XCP_PROGRAM_FORMAT == STD_ON)
/**
 * Sets the programming format. This function checks the command length, protection
 * status, and programming status. It also validates the compression, encryption,
 * programming, and access methods. If all checks pass, it sets the access mode
 * for the programming operation. If any checks fail, it sets an appropriate error
 * code and sends a response. This function is intended for internal use.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Xcp_STMIF */
void Xcp_ProgramFormat(void)
/* PRQA S 6030 -- */
{
    /* get the Compression Method from CommandRxObject[1] buffer */
    uint8 pgmCompressionMethod = Xcp_CmdBuffer[XCP_CONST_1];
    /* get the Encryption_Method from CommandRxObject[2] buffer */
    uint8 pgmEncryptionMethod = Xcp_CmdBuffer[XCP_CONST_2];
    /* get the Programming method from CommandRxObject[3] buffer */
    uint8 pgmProgrammingMethod = Xcp_CmdBuffer[XCP_CONST_3];
    /* get the access method from CommandRxObject[4] buffer */
    uint8 pgmAccessMethod = Xcp_CmdBuffer[XCP_CONST_4];

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_ChannelCommonData.CmdLength != XCP_CONST_5) && (Xcp_ChannelCommonData.CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
        /* check the protection status */
        if (XCP_PL_PGM == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_PGM))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        }
        /* Program_format should be used under XCP_PGM_CLEARED status */
        else if (XCP_PGM_CLEARED != Xcp_ChannelPtgmData.PgmStauts)
        {
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        }
        /* only support default mode */
        else if ((0u != pgmCompressionMethod) || (0u != pgmEncryptionMethod) || (0u != pgmProgrammingMethod))
        {
            /* only support default mode */
            Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);
        }
        else
        {
            switch (pgmAccessMethod)
            {
            case XCP_ABSOLUTE_ACCESS:
            {
                Xcp_ChannelPtgmData.PgmAccessMode = XCP_ABSOLUTE_ACCESS;
                break;
            }
            case XCP_FUNCTIONAL_ACCESS:
            {
                /* Indicate the next progam uses functional mode */
                Xcp_ChannelPtgmData.PgmAccessMode = XCP_FUNCTIONAL_ACCESS;
                /* Initialise the block seq counter to 1 */
                Xcp_ChannelPtgmData.PgmBlkSeqCnt = 0x01u;
                break;
            }
            default:
            {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);

                break;
            }
            }
        }
    Xcp_SendResp();
    return;
}
#endif

#if (XCP_PROGRAM_NEXT == STD_ON)
/**
 * Programs the next block of data into the target memory. This function checks
 * the programming status and sequence, aligns the data buffer, and writes the
 * data to the flash if the buffer size meets the minimum write requirement.
 * It updates the memory transfer address and sequence counters as needed. If
 * any checks fail, it sets an appropriate error code and sends a response.
 * This function is intended for internal use.
 */
void Xcp_ProgramNext(void)
{
    Xcp_PgmBufferIndexType cnt;
    uint32                 programStartAddress = 0u;

    if (((Std_ReturnType)E_OK == Xcp_CheckPgmStatus()) && (XCP_PGM_PROGRAMMING != Xcp_ChannelPtgmData.PgmStauts))
    {
        if ((Xcp_SeqCheckType)SEQ_OK
            == Xcp_GetPgmStartAddr(&programStartAddress, (uint8)Xcp_ChannelCommonData.BlockBuflen))
        {
            /*if Xcp_ChannelPtgmData.PgmBufSize is 0, set Xcp_ChannelPtgmData.PgmTargetAddr for calibration
             * software(e.g. CANAPE) optimize */
            if (0x00u == Xcp_ChannelPtgmData.PgmBufSize)
            {
                Xcp_ChannelPtgmData.PgmTargetAddr = programStartAddress;
            }
            /* set the position to 0 */
            Xcp_ChannelCommonData.BlockBufPos = 0u;
            /* block buffer has enough bytes connect with flsBuffer bytes to achieve min write size */
            if (Xcp_ChannelCommonData.BlockBuflen >= (XCP_FLS_MIN_WRITE_SIZE - Xcp_ChannelPtgmData.PgmBufSize))
            {
                for (cnt = 0u; cnt < (XCP_FLS_MIN_WRITE_SIZE - Xcp_ChannelPtgmData.PgmBufSize); cnt++)
                {
                    (Xcp_ChannelPtgmData.PgmBufAlignPtr)[Xcp_ChannelPtgmData.PgmBufSize + cnt] = Xcp_BlockBuffer[cnt];
                }

                Xcp_ChannelCommonData.BlockBufPos += cnt;
                /* set fls buffer size to min write size */
                Xcp_ChannelPtgmData.PgmBufSize = XCP_FLS_MIN_WRITE_SIZE;

                /* remained data More than min write size, and can do a write action */
                if ((Std_ReturnType)E_OK
                    == Xcp_FlsReq(
                        XCP_FLS_WRITE,
                        Xcp_ChannelPtgmData.PgmTargetAddr,
                        XCP_FLS_MIN_WRITE_SIZE,
                        (Xcp_ChannelPtgmData.PgmBufAlignPtr)))
                {
                    Xcp_ChannelPtgmData.PgmPendingFlag |= XCP_BLOCK_PROGRAM_PENDING;
                }
                else
                {
                    /* set the state to do flash program action */
                    Xcp_ChannelPtgmData.PgmPendingFlag |= XCP_BLOCK_PROGRAM_TASK;
                }
            }
            else /*block buffer hasn't enough bytes*/
            {
                for (cnt = 0u; cnt < Xcp_ChannelCommonData.BlockBuflen; cnt++)
                {
                    (Xcp_ChannelPtgmData.PgmBufAlignPtr)[Xcp_ChannelPtgmData.PgmBufSize + cnt] = Xcp_BlockBuffer[cnt];
                }
                /* Fls Buffer increase,but not achieve to min write size */
                Xcp_ChannelPtgmData.PgmBufSize += cnt;
            }
            if (Xcp_ChannelPtgmData.PgmAccessMode == XCP_FUNCTIONAL_ACCESS)
            {
                Xcp_ChannelPtgmData.PgmBlkSeqCnt++;
            }
            else /*XCP_ABSOLUTE_ACCESS*/
            {
                Xcp_UpdateMTA((uint32)cnt);
            }
        }
    }
    else
    {
        Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
    }
    if (0u == Xcp_ChannelPtgmData.PgmPendingFlag)
    {
        Xcp_SendResp();
    }
    return;
}
#endif

#if (XCP_PROGRAM_MAX == STD_ON)

/**
 * Programs the maximum block of data into the target memory. This function checks
 * the programming status and sequence, retrieves the start address, and programs
 * the data. It handles different access modes (absolute or functional) and sets
 * the programming status accordingly. If any checks fail, it sets an appropriate
 * error code. This function is intended for internal use.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Xcp_STMIF */
XCP_LOCAL void Xcp_ProgramMaxHal(void)
/* PRQA S 6030 -- */
{
    Std_ReturnType   result = E_NOT_OK;
    Xcp_SeqCheckType seqResult;
    uint32           programStartAddress = 0u;

    if (XCP_PGM_CLEARED == Xcp_ChannelPtgmData.PgmStauts)
    {
        /* do porgram sequence check and get program start address */
        seqResult = Xcp_GetPgmStartAddr(&programStartAddress, XCP_PROGRAMMAX_SIZE);
        if (SEQ_OK == seqResult)
        {
            Xcp_ChannelPtgmData.PgmTargetAddr = programStartAddress;
            /*  program size of program_max is fixed */
            result = Xcp_Program_NonZero(XCP_PROGRAMMAX_SIZE, XCP_PROGRAMMAX_SIZE);
            if ((Std_ReturnType)E_OK == result)
            {
                /* set the program status as XCP_PGM_PROGRAMMING */
                Xcp_ChannelPtgmData.PgmStauts = XCP_PGM_PROGRAMMING;
            }
            else
            {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            }
        }
    }
    else if (XCP_PGM_PROGRAMMING == Xcp_ChannelPtgmData.PgmStauts)
    {
        switch (Xcp_ChannelPtgmData.PgmAccessMode)
        {
        case XCP_ABSOLUTE_ACCESS:
        {
            /*get address form MTA[0]*/
            programStartAddress = Xcp_Mta2Ptr(Xcp_ChannelCommonData.MTAExtension, Xcp_ChannelCommonData.MTA);
            /*if Xcp_ChannelPtgmData.PgmBufSize is 0, set Xcp_ChannelPtgmData.PgmTargetAddr for calibration
             * software(e.g. CANAPE) optimize */
            if (0x00u == Xcp_ChannelPtgmData.PgmBufSize)
            {
                Xcp_ChannelPtgmData.PgmTargetAddr = programStartAddress;
                result                            = Xcp_Program_NonZero(XCP_PROGRAMMAX_SIZE, XCP_PROGRAMMAX_SIZE);
            }
            else if ((Xcp_ChannelPtgmData.PgmBufSize + Xcp_ChannelPtgmData.PgmTargetAddr) != programStartAddress)
            {
                /* Xcp_ChannelPtgmData.PgmBufSize != 0, but the address not continuous,
                 * so clear the size to 0.
                 */
                Xcp_ChannelPtgmData.PgmBufSize = 0u;
                Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
            }
            else
            {
                result = Xcp_Program_NonZero(XCP_PROGRAMMAX_SIZE, XCP_PROGRAMMAX_SIZE);
            }
            break;
        }
        case XCP_FUNCTIONAL_ACCESS:
        {
            result = Xcp_Program_NonZero(XCP_PROGRAMMAX_SIZE, XCP_PROGRAMMAX_SIZE);
            break;
        }
        default:
        {
            result = E_NOT_OK;
            break;
        }
        }
        if ((Std_ReturnType)E_NOT_OK == result)
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
        }
    }
    else
    {
        /* This address is not erased */
        Xcp_SetErrorCode(XCP_ERR_WRITE_PROTECTED);
    }
    return;
}
/**
 * Programs the maximum block of data into the target memory. This function checks
 * the command length, protection status, and programming status. If all checks
 * pass, it delegates the actual programming to the `Xcp_ProgramMaxHal` function.
 * If any checks fail, it sets an appropriate error code and sends a response.
 * This function is intended for internal use.
 */
void Xcp_ProgramMax(void)
{
    /* check frame length */
    if (Xcp_ChannelCommonData.CmdLength != (XCP_PROGRAM_MAX_SIZE + 1u))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    /* check the protection status */
    else if (XCP_PL_PGM == (Xcp_ChannelCommonData.ProtectionStatus & XCP_PL_PGM))
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
    }
    /* check the program status */
    else if ((XCP_PGM_IDLE == Xcp_ChannelPtgmData.PgmStauts) || (XCP_PGM_START == Xcp_ChannelPtgmData.PgmStauts))
    {
        Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
    }
    else
    {
        Xcp_ProgramMaxHal();
    }
    if (0u == Xcp_ChannelPtgmData.PgmPendingFlag)
    {
        Xcp_SendResp();
    }
    return;
}
#endif

#if (XCP_PROGRAM_VERIFY == STD_ON)
/**
 * Verifies the programmed data in the target memory. This function checks the
 * command length, programming status, and verification mode. It retrieves the
 * verification type and value from the command buffer and performs the necessary
 * checks. If any checks fail, it sets an appropriate error code and sends a
 * response. This function is intended for internal use.
 */
void Xcp_ProgramVerify(void)
{
    uint8  verificationMode;
    uint16 verificationType;
    uint32 verificationValue;

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (XCP_ON_CAN_ENABLE == STD_ON))
    if (Xcp_ChannelCommonData.CmdLength < XCP_CONST_8)
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
    }
    else
#endif
    {
        /* get the mode from CommandRxObject[1] buffer */
        verificationMode = Xcp_CmdBuffer[1u];

        /* get the verification type from CommandRxObject[2] buffer */
        Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[XCP_CONST_2], &verificationType);

        /* get the verification value from CommandRxObject[4]-[7] buffer */
        Xcp_CopyU1BufferToU4(&Xcp_CmdBuffer[XCP_CONST_4], &verificationValue);

        /* check the program status */
        if (XCP_PGM_PROGRAMMED != Xcp_ChannelPtgmData.PgmStauts)
        {
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        }
        /* if request to start internal routine*/
        else if (0u == verificationMode)
        {
            Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);
        }
        else
        {
            /* verification_type and verification_value check */
        }
    }
    Xcp_SendResp();
    return;
}
#endif
/* PRQA S 1532 -- */
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#endif /*XCP_PL_PGM == XCP_PL_PGM&XCP_RESOURCE*/

/* ========================================== internal function definitions ========================================= */
