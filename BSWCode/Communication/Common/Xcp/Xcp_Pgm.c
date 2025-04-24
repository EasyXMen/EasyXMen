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
**  FILENAME    : Xcp_Pgm.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation of the XCP_Pgm command                       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*=======[I N C L U D E S]====================================================*/
#include "Xcp_Internal.h"

#if (XCP_PL_PGM == (XCP_PL_PGM & XCP_RESOURCE))
/*=======[M A C R O S]========================================================*/
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

#define XCP_FLASHAREA_ALL        (XCP_FLASHAREA_CAL | XCP_FLASHAREA_CODE | XCP_FLASHAREA_NVRAM)

/* parameters of GET_PGM_PROCESSOR_INFO */
#define COMPRESSION_SUPPORTED 0x02u
#define COMPRESSION_REQUIRED  0x03u
#define ENCRYPTION_SUPPORTED  0x04u
#define ENCRYPTION_REQUIRED   0x05u
#define NON_SEQ_PGM_SUPPORTED 0x06u
#define NON_SEQ_PGM_REQUIRED  0x07u

#define PGM_PROPERTIES        (((uint8)(1u << XCP_ABSOLUTE_ACCESS)) | ((uint8)(1u << XCP_FUNCTIONAL_ACCESS)))

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

/*Address Position & Size check wheather in ROM RP*/
/* PRQA S 3472 ++ */ /* MISRA Dir 4.9 */
#define CHECK_XCP_FLASH_ROM_MEM(addr, size, sectorNum)   \
    (((addr) >= Xcp_SectorInfo[sectorNum].progStartAddr) \
     && (((addr) + (size)) <= (Xcp_SectorInfo[sectorNum].progStartAddr + Xcp_SectorInfo[sectorNum].progDataSize)))
/* PRQA S 3472 -- */ /* MISRA Dir 4.9 */

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
typedef enum
{
    SEQ_OK = 0,
    SEQ_ERROR,
    SEQ_OUTRANGE
} Xcp_SeqCheckType;

typedef VAR(uint16, TYPEDEF) Xcp_PgmBufferIndexType;

/*=======[E X T E R N A L   D A T A]==========================================*/
#define XCP_START_SEC_VAR_INIT_UNSPECIFIED
#include "Xcp_MemMap.h"
VAR(Xcp_PgmStautsType, XCP_VAR) Xcp_PgmStauts = XCP_PGM_IDLE;
#define XCP_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Xcp_MemMap.h"
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
static VAR(uint8, XCP_VAR) Xcp_PgmPendingFlag;
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
static VAR(uint8, XCP_VAR) Xcp_FlsBuffer[XCP_FLS_DATA_BUFFER_SIZE]; /*PRQA S 3218*/
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_PTR
#include "Xcp_MemMap.h"
static P2VAR(uint8, AUTOMATIC, AUTOMATIC) Xcp_FlsBufAlignPtr; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
#define XCP_STOP_SEC_VAR_CLEARED_PTR
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
static VAR(Xcp_PgmBufferIndexType, XCP_VAR) Xcp_FlsBufferSize;
#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_32
#include "Xcp_MemMap.h"
static VAR(uint32, XCP_VAR) Xcp_FlsTargetAddr;
#define XCP_STOP_SEC_VAR_CLEARED_32
#include "Xcp_MemMap.h"

/* Clear and Program Access Mode(Absolute or Functional)
 * They may be different access mode.
 */
#define XCP_START_SEC_VAR_INIT_8
#include "Xcp_MemMap.h"
static VAR(uint8, XCP_VAR) Xcp_Clear_AccessMode = XCP_ABSOLUTE_ACCESS;
static VAR(uint8, XCP_VAR) Xcp_Program_AccessMode = XCP_ABSOLUTE_ACCESS;
#define XCP_STOP_SEC_VAR_INIT_8
#include "Xcp_MemMap.h"

/* indicate Xcp program_clear range and
 * also used in Program command when use functional access mode
 */
#define XCP_START_SEC_VAR_CLEARED_32
#include "Xcp_MemMap.h"
static VAR(uint32, XCP_VAR) XCP_FlashAreaSelect;
#define XCP_STOP_SEC_VAR_CLEARED_32
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_32
#include "Xcp_MemMap.h"
/* clear start address for program_clear command and pending task */
static VAR(Xcp_FlsAddressType, XCP_VAR) Xcp_ClearStartAddr;
#define XCP_STOP_SEC_VAR_CLEARED_32
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_32
#include "Xcp_MemMap.h"
/* clear size for program_clear command and pending task */
static VAR(Xcp_FlsLengthType, XCP_VAR) Xcp_ClearSize;
#define XCP_STOP_SEC_VAR_CLEARED_32
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_INIT_8
#include "Xcp_MemMap.h"
/* block sequence counter used for program in functional access mode */
static VAR(uint8, XCP_VAR) Xcp_Pgm_BlockSeqCounter = 0x01u;
#define XCP_STOP_SEC_VAR_INIT_8
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"
/* program_clear and program pending task retry counters */
static VAR(uint8, XCP_VAR) Xcp_Program_RetryCounters;
#define XCP_STOP_SEC_VAR_CLEARED_8
#include "Xcp_MemMap.h"

#define XCP_START_SEC_VAR_INIT_BOOLEAN
#include "Xcp_MemMap.h"
static VAR(boolean, XCP_VAR) Xcp_Pgm_LastWriteFlag = FALSE;
#define XCP_STOP_SEC_VAR_INIT_BOOLEAN
#include "Xcp_MemMap.h"
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
static FUNC(Std_ReturnType, XCP_CODE) Xcp_CheckPgmStatus(void);

static FUNC(Std_ReturnType, XCP_CODE)
    Xcp_FlsReq(uint8 Mode, Xcp_FlsAddressType u4Address, Xcp_FlsLengthType u4Size, const uint8* pu1DataPtr);

static FUNC(uint8, XCP_CODE) Xcp_FlsGetStatus(void);

static FUNC(Std_ReturnType, XCP_CODE) Xcp_Program_ZeroHandle(void);

static FUNC(Std_ReturnType, XCP_CODE) Xcp_Program_NonZero(uint8 dataCount, uint8 pgmSize);

static FUNC(Xcp_SeqCheckType, XCP_CODE)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    Xcp_GetPgmStartAddr(P2VAR(uint32, AUTOMATIC, XCP_VAR) startAddr, uint8 noOfBytes);

static FUNC(Xcp_SeqCheckType, XCP_CODE)
    Xcp_GetPgm_Functional_StartAddr(P2VAR(uint32, AUTOMATIC, XCP_VAR) startAddr, uint8 noOfBytes, uint32 rangType);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

static FUNC(Xcp_SeqCheckType, XCP_CODE)
    Xcp_GetPgmClearStartAddr(uint32 clearRange, uint32* startAddr, uint32* clearSize);

/* the next six functions used for pending handle */
static FUNC(void, XCP_CODE) Xcp_FlsClearPending(void);

static FUNC(void, XCP_CODE) Xcp_FlsClearTask(void);

static FUNC(void, XCP_CODE) Xcp_FlsProgramPending(void);

static FUNC(void, XCP_CODE) Xcp_FlsProgramTask(void);

#if (XCP_MASTER_BLOCK_MODE == STD_ON)
static FUNC(void, XCP_CODE) Xcp_FlsBlockProgramPending(void);

static FUNC(void, XCP_CODE) Xcp_FlsBlockProgramTask(void);
#endif /*XCP_MASTER_BLOCK_MODE == STD_ON*/

/*
 * Complex Command Handler
 */
static FUNC(void, XCP_CODE) Xcp_ProgramHal(void);
#if (XCP_PROGRAM_MAX == STD_ON)
static FUNC(void, XCP_CODE) Xcp_ProgramMaxHal(void);
#endif
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"
/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/* local */

#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
static FUNC(Std_ReturnType, XCP_CODE) Xcp_CheckPgmStatus(void)
{
    Std_ReturnType ret = E_NOT_OK;

    /* check the protection status */
    if (XCP_PL_PGM == (Xcp_ProtectionStatus & XCP_PL_PGM))
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        Xcp_RespLength = 0x02u;
    }
    else if (XCP_PGM_IDLE == Xcp_PgmStauts)
    {
        Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        Xcp_RespLength = 0x02u;
    }
    else
    {
        ret = E_OK;
    }
    return ret;
}

FUNC(boolean, XCP_CODE)
Xcp_CheckPgmMem(uint32 addr, uint32 size)
{
    boolean result = FALSE;
    uint8 secNum;
    uint32 start;
    uint32 end;

    for (secNum = 0; secNum < XCP_MAX_SECTOR; secNum++)
    {
        start = Xcp_SectorInfo[secNum].progStartAddr;
        end = Xcp_SectorInfo[secNum].progStartAddr + Xcp_SectorInfo[secNum].progDataSize - 1u;
        if (Xcp_CheckMemVaild(addr, size, start, end))
        {
            result = TRUE;
            break;
        }
    }
    return result;
}

static FUNC(Std_ReturnType, XCP_CODE)
    Xcp_FlsReq(uint8 Mode, Xcp_FlsAddressType u4Address, Xcp_FlsLengthType u4Size, const uint8* pu1DataPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    /* PRQA S 1338 ++ */ /* MISRA Rule 17.8 */
    u4Address = u4Address - XCP_FLS_BASEADDR;
    /* PRQA S 1338 -- */ /* MISRA Rule 17.8 */
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

static FUNC(uint8, XCP_CODE) Xcp_FlsGetStatus(void)
{
    MemIf_JobResultType flsRet;
    uint8 result;

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

/******************************************************************************/
/*
 * @brief               <Xcp_Program_ZeroHandle>
 *
 * <This function handle when program zero received> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/

static FUNC(Std_ReturnType, XCP_CODE) Xcp_Program_ZeroHandle(void)
{
    Xcp_PgmBufferIndexType cnt;
    Std_ReturnType programRet;

    if (0u != Xcp_FlsBufferSize)
    {
        /* if remained data more than min write size,there should be a error */
        if (Xcp_FlsBufferSize >= XCP_FLS_MIN_WRITE_SIZE)
        {
            programRet = E_NOT_OK;
        }
        else
        {
            for (cnt = 0u; cnt < (XCP_FLS_MIN_WRITE_SIZE - Xcp_FlsBufferSize); cnt++)
            {
                Xcp_FlsBufAlignPtr[Xcp_FlsBufferSize + cnt] = XCP_PGM_FILLER;
            }

            /* set fls buffer size to min write size for pending task */
            Xcp_FlsBufferSize = XCP_FLS_MIN_WRITE_SIZE;

            /* set block seq number or MTA */
            if (XCP_FUNCTIONAL_ACCESS == Xcp_Program_AccessMode)
            {
                Xcp_Pgm_BlockSeqCounter++;
            }
            if (XCP_ABSOLUTE_ACCESS == Xcp_Program_AccessMode)
            {
                Xcp_UpdateMTA((uint32)XCP_FLS_MIN_WRITE_SIZE);
            }

            /* indicate that this time is the last write of this program sequence */
            Xcp_Pgm_LastWriteFlag = TRUE;

            if ((Std_ReturnType)E_OK
                == Xcp_FlsReq(XCP_FLS_WRITE, Xcp_FlsTargetAddr, XCP_FLS_MIN_WRITE_SIZE, Xcp_FlsBufAlignPtr))
            {
                Xcp_PgmPendingFlag |= XCP_PROGRAM_PENDING;
            }
            else
            {
                /* set the state to do flash program action */
                Xcp_PgmPendingFlag |= XCP_PROGRAM_TASK;
            }

            programRet = E_OK;
        }
    }
    else
    {
        /* Xcp_FlsBuffer empty and program finished */
        Xcp_PgmStauts = XCP_PGM_PROGRAMMED;
        programRet = E_OK;
    }
    return programRet;
}

/******************************************************************************/
/*
 * @brief               <Xcp_GetPgmStartAddr>
 *
 * < this function do program sequence check do get the program start address> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Not Reentrant>
 * @param[in]           <noOfBytes>
 * @param[out]          <None>
 * @param[in/out]       <startAddr>
 * @return              <Xcp_SeqCheckType>
 */
/******************************************************************************/

static FUNC(Xcp_SeqCheckType, XCP_CODE)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    Xcp_GetPgmStartAddr(P2VAR(uint32, AUTOMATIC, XCP_VAR) startAddr, uint8 noOfBytes)
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
{
    Xcp_SeqCheckType programSeqcheckRet = SEQ_OK;
    boolean programFlashAreaFind = FALSE;

    /* check the access mode */
    switch (Xcp_Program_AccessMode)
    {
    case XCP_FUNCTIONAL_ACCESS: {
        if (0u != (XCP_FlashAreaSelect & XCP_FLASHAREA_ALL))
        {
            programFlashAreaFind = TRUE;
            programSeqcheckRet = Xcp_GetPgm_Functional_StartAddr(startAddr, noOfBytes, XCP_FlashAreaSelect);
        }
        break;
    }
    /* Absolute access mode ,then get the clear start address form MTA */
    case XCP_ABSOLUTE_ACCESS: {
        /*get address form MTA[0]*/
        *startAddr = Xcp_Mta2Ptr(Xcp_MTA.extensionAddr, Xcp_MTA.transferAddr);
        programFlashAreaFind = TRUE;
        /* check the address */
        if ((0x00u != Xcp_FlsBufferSize) && ((Xcp_FlsBufferSize + Xcp_FlsTargetAddr) != (*startAddr)))
        {
            /* data remained before, but the address not continuous,
             * so clear the size to 0.
             */
            Xcp_FlsBufferSize = 0;
            programFlashAreaFind = FALSE;
        }
        break;
    }
    default: {
        /*Do nothing*/
        break;
    }
    }
    if ((boolean)TRUE != programFlashAreaFind)
    {
        programSeqcheckRet = SEQ_ERROR;
    }
    return programSeqcheckRet;
}

/******************************************************************************/
/*
 * @brief               <Xcp_GetPgm_Functional_StartAddr>
 * < This function find the program start address when use functional access
 * mode >
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <pgmSeq, startAddr,noOfBytes,rangType>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <Xcp_SeqCheckType>
 */
/******************************************************************************/
static FUNC(Xcp_SeqCheckType, XCP_CODE)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    Xcp_GetPgm_Functional_StartAddr(P2VAR(uint32, AUTOMATIC, XCP_VAR) startAddr, uint8 noOfBytes, uint32 rangType)
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
{
    uint8 sectorNum;
    boolean rightSectorFind = FALSE;
    Xcp_SeqCheckType ret = SEQ_ERROR;

    /* loop all config sectors to find the correct one for current programming */
    for (sectorNum = 0u; sectorNum < XCP_MAX_SECTOR; sectorNum++)
    {
        /* check the sector whether has the right range type */
        if ((Xcp_SectorInfo[sectorNum].rangeType == rangType)
            && (Xcp_ClearStartAddr == Xcp_SectorInfo[sectorNum].sectorclrStartAddr))
        {
            rightSectorFind = TRUE;
            *startAddr = Xcp_SectorInfo[sectorNum].progStartAddr;

            /* check the address */
            if ((0x00u != Xcp_FlsBufferSize) && ((Xcp_FlsBufferSize + Xcp_FlsTargetAddr) != (*startAddr)))
            {
                /* data remained before, but the address not continuous,
                 * so clear the size to 0.
                 */
                Xcp_FlsBufferSize = 0;
                ret = SEQ_ERROR;
            }
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */
            else if (CHECK_XCP_FLASH_ROM_MEM(*startAddr, noOfBytes, sectorNum))
            {
                /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
                ret = SEQ_OK;
                /* Clear the area range flag cause we already find it */
                XCP_FlashAreaSelect &= ~(uint32)rangType;
            }
            else
            {
                ret = SEQ_OUTRANGE;
            }
        }
    }
    if ((boolean)TRUE != rightSectorFind)
    {
        ret = SEQ_ERROR;
    }

    return ret;
}

/******************************************************************************/
/*
 * @brief               <Xcp_Program_NonZero>
 *
 * < This function handled when not a program zero command received > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <startAddr, dataCount,pgmSize>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <Std_ReturnType>
 */
/******************************************************************************/
static FUNC(Std_ReturnType, XCP_CODE) Xcp_Program_NonZero(uint8 dataCount, uint8 pgmSize)
{
    uint8 dataNo;
    Std_ReturnType result = E_OK;

    if (dataCount <= pgmSize)
    {
        for (dataNo = 0u; dataNo < dataCount; dataNo++)
        {
            if (XCP_PROGRAMMAX_SIZE == pgmSize)
            {
                /* program_Max command copy the Cmdbuffer data to Xcp_FlsBuffer from XCP_AG position */
                Xcp_FlsBufAlignPtr[Xcp_FlsBufferSize + dataNo] = Xcp_CmdBuffer[XCP_AG + dataNo];
            }
            else
            {
#if (XCP_ADDR_GRANULARITY != XCP_AG_DWORD)
                Xcp_FlsBufAlignPtr[Xcp_FlsBufferSize + dataNo] = Xcp_CmdBuffer[2u + dataNo];
#else
                Xcp_FlsBufAlignPtr[Xcp_FlsBufferSize + dataNo] = Xcp_CmdBuffer[XCP_AG + dataNo];
#endif
            }
        }
        Xcp_FlsBufferSize += dataCount;

        if (XCP_FUNCTIONAL_ACCESS == Xcp_Program_AccessMode)
        {
            Xcp_Pgm_BlockSeqCounter++;
        }
        else if (XCP_ABSOLUTE_ACCESS == Xcp_Program_AccessMode)
        {
            Xcp_UpdateMTA((uint32)dataCount);
        }
        else
        {
            /* Do Nothing */
        }

        /* check the buffer size , bigger than XCP_FLS_MIN_WRITE_SIZE, then do write */
        if (Xcp_FlsBufferSize >= XCP_FLS_MIN_WRITE_SIZE)
        {
            if ((Std_ReturnType)E_OK
                == Xcp_FlsReq(XCP_FLS_WRITE, Xcp_FlsTargetAddr, XCP_FLS_MIN_WRITE_SIZE, Xcp_FlsBufAlignPtr))
            {
                Xcp_PgmPendingFlag |= XCP_PROGRAM_PENDING;
            }
            else
            {
                Xcp_PgmPendingFlag |= XCP_PROGRAM_TASK;
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

/******************************************************************************/
/*
 * @brief               <Xcp_GetPgmClearStartAddr>
 *
 * < Get the program_clear start address and size > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <clearRange, maxseqInClearedSectors,maxSeqInPgmedSectors>
 * @param[out]          <startAddr, clearSize>
 * @param[in/out]       <None>
 * @return              <Xcp_SeqCheckType>
 */
/******************************************************************************/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
static FUNC(Xcp_SeqCheckType, XCP_CODE) Xcp_GetPgmClearStartAddr(
    uint32 clearRange,
    P2VAR(uint32, AUTOMATIC, XCP_VAR) startAddr,
    P2VAR(uint32, AUTOMATIC, XCP_VAR) clearSize)
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
{
    uint8 sectorNum;
    Xcp_SeqCheckType result = SEQ_OK;

    /* functional access mode ,then calculate the clear start address */
    if (XCP_FUNCTIONAL_ACCESS == Xcp_Clear_AccessMode)
    {
        /* select the flash area need to clear,
         * Provide the flag for PROGRAM used when in functional access mode
         */
        XCP_FlashAreaSelect |= clearRange;

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
    else if (XCP_ABSOLUTE_ACCESS == Xcp_Clear_AccessMode)
    {
        if (TRUE == Xcp_CheckPgmMem(Xcp_Mta2Ptr(Xcp_MTA.extensionAddr, Xcp_MTA.transferAddr), clearRange))
        {
            *startAddr = Xcp_Mta2Ptr(Xcp_MTA.extensionAddr, Xcp_MTA.transferAddr);
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

/******************************************************************************/
/*
 * @brief               <Xcp_FlsClearPending>
 *
 * < This funtion do pending task when pendingFlag is clearPending > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
static FUNC(void, XCP_CODE) Xcp_FlsClearPending(void)
{
    uint8 result;
    result = Xcp_FlsGetStatus();
    switch (result)
    {
    case XCP_IF_OK:
        Xcp_PgmPendingFlag &= (uint8)(~XCP_CLEAR_PENDING);
        Xcp_FlsBufferSize = 0u;
        /* set the program status as XCP_PGM_CLEARED */
        Xcp_PgmStauts = XCP_PGM_CLEARED;
        Xcp_SendResp();
        break;
    case XCP_IF_NOT_OK:
        Xcp_PgmPendingFlag &= (uint8)(~XCP_CLEAR_PENDING);
        Xcp_FlsBufferSize = 0u;

        /* Do not set the program status because clear failed */

        Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);
        Xcp_RespLength = 0x02u;
        Xcp_SendResp();
        break;
    default:
        /*Do nothing*/
        break;
    }
    return;
}

/******************************************************************************/
/*
 * @brief               <Xcp_FlsClearTask>
 *
 * < This funtion do pending task when pendingFlag is clearTask > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
static FUNC(void, XCP_CODE) Xcp_FlsClearTask(void)
{
    if ((Std_ReturnType)E_OK == Xcp_FlsReq(XCP_FLS_ERASE, Xcp_ClearStartAddr, Xcp_ClearSize, NULL_PTR))
    {
        Xcp_Program_RetryCounters = 0u;
        Xcp_PgmPendingFlag &= (uint8)(~XCP_CLEAR_TASK);
        Xcp_PgmPendingFlag |= XCP_CLEAR_PENDING;
    }
    else
    {
        Xcp_Program_RetryCounters++;
        if (Xcp_Program_RetryCounters >= XCP_PGM_REQRETRIE)
        {
            Xcp_Program_RetryCounters = 0u;
            Xcp_PgmPendingFlag &= (uint8)(~XCP_CLEAR_TASK);
            Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);
            Xcp_RespLength = 0x02u;
            Xcp_SendResp();
        }
        else
        {
            /* do nothing */
        }
    }
    return;
}

/******************************************************************************/
/*
 * @brief               <Xcp_FlsProgramPending>
 *
 * < This funtion do pending task when pendingFlag is programPending > .
 * Service ID   :       <None>
 * Sync/Async   :       <None>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
static FUNC(void, XCP_CODE) Xcp_FlsProgramPending(void)
{
    uint8 result;
    Xcp_PgmBufferIndexType cnt;
    result = Xcp_FlsGetStatus();
    switch (result)
    {
    case XCP_IF_OK:
        Xcp_PgmPendingFlag &= (uint8)(~XCP_PROGRAM_PENDING);

        Xcp_FlsTargetAddr += XCP_FLS_MIN_WRITE_SIZE;
        Xcp_FlsBufferSize -= XCP_FLS_MIN_WRITE_SIZE;

        if ((boolean)TRUE == Xcp_Pgm_LastWriteFlag)
        {
            Xcp_PgmStauts = XCP_PGM_PROGRAMMED;

            /* program completed, clear buffer size */
            Xcp_FlsBufferSize = 0x0u;

            /* clear the last write flag */
            Xcp_Pgm_LastWriteFlag = FALSE;
        }
        /* Copy more than XCP_FLS_MIN_WRITE_SIZE bytes after the data */
        for (cnt = 0u; cnt < Xcp_FlsBufferSize; cnt++)
        {
            Xcp_FlsBufAlignPtr[cnt] = Xcp_FlsBufAlignPtr[cnt + XCP_FLS_MIN_WRITE_SIZE];
        }
        if (Xcp_FlsBufferSize >= XCP_FLS_MIN_WRITE_SIZE)
        {
            Xcp_PgmPendingFlag |= XCP_PROGRAM_TASK;
        }
        else
        {
            Xcp_SendResp();
        }
        break;
    case XCP_IF_NOT_OK:
        Xcp_PgmPendingFlag &= (uint8)(~XCP_PROGRAM_PENDING);
        Xcp_FlsBufferSize = 0;

        /* set the program status back to XCP_PGM_START because program failed, and Need to be re-erased */
        Xcp_PgmStauts = XCP_PGM_START;

        Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);
        Xcp_RespLength = 0x02u;
        Xcp_SendResp();
        break;
    default:
        /*Do nothing*/
        break;
    }
    return;
}

/******************************************************************************/
/*
 * @brief               <Xcp_FlsProgramTask>
 *
 * < This funtion do pending task when pendingFlag is programTask > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
static FUNC(void, XCP_CODE) Xcp_FlsProgramTask(void)
{
    if ((Std_ReturnType)E_OK
        == Xcp_FlsReq(XCP_FLS_WRITE, Xcp_FlsTargetAddr, XCP_FLS_MIN_WRITE_SIZE, Xcp_FlsBufAlignPtr))
    {
        Xcp_Program_RetryCounters = 0u;
        Xcp_PgmPendingFlag &= (uint8)(~XCP_PROGRAM_TASK);
        Xcp_PgmPendingFlag |= XCP_PROGRAM_PENDING;
    }
    else
    {
        Xcp_Program_RetryCounters++;
        if (Xcp_Program_RetryCounters >= XCP_PGM_REQRETRIE)
        {
            Xcp_Program_RetryCounters = 0u;
            Xcp_PgmPendingFlag &= (uint8)(~XCP_PROGRAM_TASK);
            Xcp_FlsBufferSize = 0u;
            Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);
            Xcp_RespLength = 0x02u;
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
/******************************************************************************/
/*
 * @brief               <Xcp_FlsBlockProgramPending>
 *
 * < This funtion do pending task when pendingFlag is BlockprogramPending > .
 * Service ID   :       <None>
 * Sync/Async   :       <None>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
static FUNC(void, XCP_CODE) Xcp_FlsBlockProgramPending(void)
{
    uint8 result;
    Xcp_PgmBufferIndexType cnt;
    result = Xcp_FlsGetStatus();
    switch (result)
    {
    case XCP_IF_OK:
        Xcp_PgmPendingFlag &= (uint8)(~XCP_BLOCK_PROGRAM_PENDING);

        /* set the program status as XCP_PGM_PROGRAMMING */
        Xcp_PgmStauts = XCP_PGM_PROGRAMMING;

        Xcp_FlsTargetAddr += XCP_FLS_MIN_WRITE_SIZE;
        /* if remained data more than min write size ,then continue write*/
        if ((XCP_FLS_MIN_WRITE_SIZE + Xcp_BlockBufferPos) <= Xcp_BlockBufferLen)
        {
            for (cnt = 0u; cnt < XCP_FLS_MIN_WRITE_SIZE; cnt++)
            {
                Xcp_FlsBufAlignPtr[cnt] = Xcp_BlockBuffer[Xcp_BlockBufferPos + cnt];
            }
            /* set fls buffer size to min write size */
            Xcp_FlsBufferSize = XCP_FLS_MIN_WRITE_SIZE;

            Xcp_BlockBufferPos += XCP_FLS_MIN_WRITE_SIZE;

            Xcp_PgmPendingFlag |= XCP_BLOCK_PROGRAM_TASK;
            Xcp_UpdateMTA((uint32)XCP_FLS_MIN_WRITE_SIZE);
        }
        else
        {
            cnt = 0u;
            while (Xcp_BlockBufferLen != Xcp_BlockBufferPos)
            {
                Xcp_FlsBufAlignPtr[cnt] = Xcp_BlockBuffer[Xcp_BlockBufferPos];
                Xcp_BlockBufferPos++;
                cnt++;
            }
            Xcp_FlsBufferSize = cnt;
            Xcp_UpdateMTA((uint32)cnt);

            Xcp_SendResp();
        }

        break;
    case XCP_IF_NOT_OK:
        Xcp_PgmPendingFlag &= (uint8)(~XCP_BLOCK_PROGRAM_PENDING);

        /* set the program status back to XCP_PGM_START because program failed */
        Xcp_PgmStauts = XCP_PGM_START;
        Xcp_BlockBufferLen = 0u;
        Xcp_BlockBufferPos = 0u;
        Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);
        Xcp_RespLength = 0x02u;
        Xcp_SendResp();
        break;
    default:
        /*Do nothing*/
        break;
    }
    return;
}

/******************************************************************************/
/*
 * @brief               <Xcp_FlsBlockProgramTask>
 *
 * < This funtion do pending task when pendingFlag is BlockprogramTask > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
static FUNC(void, XCP_CODE) Xcp_FlsBlockProgramTask(void)
{
    Std_ReturnType result;

    result = Xcp_FlsReq(XCP_FLS_WRITE, Xcp_FlsTargetAddr, XCP_FLS_MIN_WRITE_SIZE, Xcp_FlsBufAlignPtr);

    if ((Std_ReturnType)E_OK == result)
    {
        Xcp_Program_RetryCounters = 0;
        Xcp_PgmPendingFlag &= (uint8)(~XCP_BLOCK_PROGRAM_TASK);
        Xcp_PgmPendingFlag |= XCP_BLOCK_PROGRAM_PENDING;
    }
    else
    {
        Xcp_Program_RetryCounters++;
        if (Xcp_Program_RetryCounters >= XCP_PGM_REQRETRIE)
        {
            Xcp_Program_RetryCounters = 0u;
            Xcp_PgmPendingFlag &= (uint8)(~XCP_BLOCK_PROGRAM_TASK);
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
            Xcp_BlockBufferLen = 0u;
#endif
            Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);
            Xcp_RespLength = 0x02u;
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

/* gobal */
/******************************************************************************/
/*
 * @brief               <Xcp_ProgramInit>
 *
 * <This function handle the initialise of Program> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_ProgramInit(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Xcp_PgmBufferIndexType cnt;
    /*clear all pending task flag*/
    Xcp_PgmPendingFlag = 0x00u;

    for (cnt = 0u; cnt < XCP_FLS_DATA_BUFFER_SIZE; cnt++)
    {
        Xcp_FlsBuffer[cnt] = 0xff;
    }

    cnt = 0u;
    while (((uint32)(&Xcp_FlsBuffer[cnt]) % 4u) != 0u) /* PRQA S 0306 */ /* MISRA Rule 11.4 */
    {
        cnt++;
    }
    Xcp_FlsBufAlignPtr = &Xcp_FlsBuffer[cnt];
    Xcp_FlsBufferSize = 0u;

    Xcp_FlsTargetAddr = 0u;

    /* Last write flag will be set while a program zero occurred */
    Xcp_Pgm_LastWriteFlag = FALSE;
    /*init the pgm statue to IDLE*/
    Xcp_PgmStauts = XCP_PGM_IDLE;
    return;
}

/******************************************************************************/
/*
 * @brief               <Xcp_PgmPendingTask>
 *
 * <This function handle the initialise of Program> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_PgmPendingTask(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    switch (Xcp_PgmPendingFlag)
    {
    case XCP_CLEAR_PENDING: {
        Xcp_FlsClearPending();
        break;
    }
    case XCP_CLEAR_TASK: {
        Xcp_FlsClearTask();
        break;
    }
    case XCP_PROGRAM_PENDING: {
        Xcp_FlsProgramPending();
        break;
    }
    case XCP_PROGRAM_TASK: {
        Xcp_FlsProgramTask();
        break;
    }
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
    case XCP_BLOCK_PROGRAM_PENDING: {
        Xcp_FlsBlockProgramPending();
        break;
    }
    case XCP_BLOCK_PROGRAM_TASK: {
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

/* mandatory */
/******************************************************************************/
/*
 * @brief               <Xcp_ProgramStart>
 *
 * <start a program sequence> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_ProgramStart(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
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
        /* check whether DAQ Running at this moment */
        if (XCP_SESSION_STATE_DAQ_RUNNING == (Xcp_SessionStatus & XCP_SESSION_STATE_DAQ_RUNNING))
        {
            Xcp_SetErrorCode(XCP_ERR_DAQ_ACTIVE);
            Xcp_RespLength = 0x02u;
        }
        /* check the protection status */
        else if (XCP_PL_PGM == (Xcp_ProtectionStatus & XCP_PL_PGM))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            Xcp_RespBuffer[2u] = 0;
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
            Xcp_RespBuffer[2u] |= 0x01u;
#endif
#if (XCP_INTERLEAVED_MODE == STD_ON)
            Xcp_RespBuffer[2u] |= 0x02u;
#endif
#if (XCP_SLAVE_BLOCK_MODE == STD_ON)
            Xcp_RespBuffer[2u] |= 0x40u;
#endif
            Xcp_RespBuffer[3u] = XCP_MAX_CTO_PGM;
#if (XCP_MASTER_BLOCK_MODE == STD_ON)
            Xcp_RespBuffer[4u] = XCP_MAX_BS_PGM;
            Xcp_RespBuffer[5u] = XCP_MIN_ST_PGM;
#else
            Xcp_RespBuffer[4u] = 0;
            Xcp_RespBuffer[5u] = 0;
#endif
#if (XCP_INTERLEAVED_MODE == STD_ON)
            Xcp_RespBuffer[6u] = XCP_QUEUE_SIZE_PGM;
#endif
            Xcp_RespLength = 0x07u;
            Xcp_PgmStauts = XCP_PGM_START;
        }
    }
    Xcp_SendResp();
    return;
}

/******************************************************************************/
/*
 * @brief               <Xcp_ProgramClear>
 *
 * <xcp protocol to clear flash,should depend on the clear mode > .
 * Service ID   :       <SERVICE ID>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <void>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_ProgramClear(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint32 startAddr;
    uint32 programClearRange;
    Xcp_SeqCheckType seqResult;

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if (Xcp_CmdLength != 8u)
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
    {
        if ((Std_ReturnType)E_OK == Xcp_CheckPgmStatus())
        { /* get the clear mode from CommandRxObject[1] buffer */
            Xcp_Clear_AccessMode = Xcp_CmdBuffer[1u];

            /* get the clear size from CommandRxObject[4]-[7] buffer */
            Xcp_CopyU1BufferToU4(&Xcp_CmdBuffer[4u], &programClearRange, CPU_BYTE_ORDER);
            seqResult = Xcp_GetPgmClearStartAddr(programClearRange, &startAddr, &Xcp_ClearSize);
            switch (seqResult)
            {
            case SEQ_OK: {
                Xcp_ClearStartAddr = startAddr;
                /* clear Flash it will be done in the pengding task*/
                if ((Std_ReturnType)E_OK == Xcp_FlsReq(XCP_FLS_ERASE, Xcp_ClearStartAddr, Xcp_ClearSize, NULL_PTR))
                {
                    Xcp_PgmPendingFlag |= XCP_CLEAR_PENDING;
                }
                else
                {
                    Xcp_PgmPendingFlag |= XCP_CLEAR_TASK;
                    ;
                }
                break;
            }
            case SEQ_OUTRANGE: {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
                Xcp_RespLength = 0x02u;
                break;
            }
            case SEQ_ERROR: {
                Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
                Xcp_RespLength = 0x02u;
                break;
            }
            default: {
                /* Do Nothing */
                break;
            }
            }
        }
    }
    if (0u == Xcp_PgmPendingFlag)
    {
        Xcp_SendResp();
    }
    return;
}

static FUNC(void, XCP_CODE) Xcp_ProgramHal(void)
{
    Std_ReturnType result = E_OK;
    Xcp_SeqCheckType seqResult;
    uint8 programNumberOfData;
    uint8 NumOfBytes;
    uint32 programStartAddress = 0;

    /* get the program size from CommandRxObject[1] buffer */
    programNumberOfData = Xcp_CmdBuffer[1u];
#if (XCP_ADDR_GRANULARITY != XCP_AG_DWORD)
    NumOfBytes = 2u + (programNumberOfData * XCP_AG);
#else
    NumOfBytes = 4u + (programNumberOfData * XCP_AG);
#endif
    /* The cleanup operation must be performed before programming,so the Xcp_PgmStauts will be XCP_PGM_CLEARED or
     * XCP_PGM_PROGRAMMING */
    if (XCP_PGM_CLEARED == Xcp_PgmStauts)
    {
        /* do porgram sequence check and get program start address */
        seqResult = Xcp_GetPgmStartAddr(&programStartAddress, NumOfBytes);
        if (SEQ_OK == seqResult)
        {
            Xcp_FlsTargetAddr = programStartAddress;
            result = Xcp_Program_NonZero(programNumberOfData, XCP_PROGRAM_SIZE);
            if ((Std_ReturnType)E_OK == result)
            {
                /* set the program status as XCP_PGM_PROGRAMMING */
                Xcp_PgmStauts = XCP_PGM_PROGRAMMING;
            }
            else
            {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
                Xcp_RespLength = 0x02u;
            }
        }
        else if (SEQ_ERROR == seqResult) /* clear sequence ocuured */
        {
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
            Xcp_RespLength = 0x02u;
        }
        else /**< SEQ_OUTRANGE == seqResult */
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
            Xcp_RespLength = 0x02u;
        }
    }
    else if (XCP_PGM_PROGRAMMING == Xcp_PgmStauts)
    {
        switch (Xcp_Program_AccessMode)
        {
        case XCP_ABSOLUTE_ACCESS: {
            /*get address form MTA[0]*/
            programStartAddress = Xcp_Mta2Ptr(Xcp_MTA.extensionAddr, Xcp_MTA.transferAddr);
            /*if Xcp_FlsBufferSize is 0, set Xcp_FlsTargetAddr for calibration software(e.g. CANAPE) optimize */
            if (0x00u == Xcp_FlsBufferSize)
            {
                Xcp_FlsTargetAddr = programStartAddress;
                result = Xcp_Program_NonZero(programNumberOfData, XCP_PROGRAM_SIZE);
            }
            else if ((Xcp_FlsBufferSize + Xcp_FlsTargetAddr) != programStartAddress)
            {
                /* Xcp_FlsBufferSize != 0, but the address not continuous,
                 * so clear the size to 0.
                 */
                Xcp_FlsBufferSize = 0;
                Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
                Xcp_RespLength = 0x02u;
            }
            else
            {
                result = Xcp_Program_NonZero(programNumberOfData, XCP_PROGRAM_SIZE);
            }
            break;
        }
        case XCP_FUNCTIONAL_ACCESS: {
            result = Xcp_Program_NonZero(programNumberOfData, XCP_PROGRAM_SIZE);
            break;
        }
        default: {
            result = E_NOT_OK;
            break;
        }
        }
        if ((Std_ReturnType)E_NOT_OK == result)
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            Xcp_RespLength = 0x02u;
        }
    }
    else
    {
        /* This address is not erased */
        Xcp_SetErrorCode(XCP_ERR_WRITE_PROTECTED);
        Xcp_RespLength = 0x02u;
    }
    return;
}

/******************************************************************************/
/*
 * @brief               <Xcp_Program>
 *
 * <xcp prgram the data to flash,it contains block mode or non_block mode> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_Program(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType result = E_OK;
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
        /* check the protection status */
        if (XCP_PL_PGM == (Xcp_ProtectionStatus & XCP_PL_PGM))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        /* check the program status */
        else if ((XCP_PGM_IDLE == Xcp_PgmStauts) || (XCP_PGM_START == Xcp_PgmStauts))
        {
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
            Xcp_RespLength = 0x02u;
        }
        /* check out of range */
        else if (Xcp_CmdBuffer[1u] > XCP_PROGRAM_SIZE)
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            switch (Xcp_CmdBuffer[1u])
            {
            case 0u: {
                /*The end of the memory segment is indicated,
                 * when the number of data elements is 0.*/
                result = Xcp_Program_ZeroHandle();
                break;
            }
            default: {
                Xcp_ProgramHal();
                break;
            }
            }
        }
    if ((Std_ReturnType)E_NOT_OK == result)
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    if (0u == Xcp_PgmPendingFlag)
    {
        Xcp_SendResp();
    }
    return;
}

/******************************************************************************/
/*
 * @brief               <Xcp_ProgramReset>
 *
 * < when program finished , a program reset command shall be excuted for
    slave disconnect with master > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_ProgramReset(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
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
        /* check the protection status */
        if (XCP_PL_PGM == (Xcp_ProtectionStatus & XCP_PL_PGM))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        /* check the flash status */
        else if (XCP_IF_BUSY == Xcp_FlsGetStatus())
        {
            Xcp_SetErrorCode(XCP_ERR_PGM_ACTIVE);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            /* set status to XCP_PGM_IDLE because it's the end of program sequence */
            Xcp_PgmStauts = XCP_PGM_IDLE;

            Xcp_DisconnectHal();
            Xcp_RespLength = 0x01u;
        }
    }
    Xcp_SendResp();
    return;
}

/* optional */
#if (XCP_GET_PGM_PROCESSOR_INFO == STD_ON)
/******************************************************************************/
/*
 * @brief               <Xcp_Get_Pgm_Processor_Info>
 *
 * < optional command to get program processor info such as support block mode
    or not > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_GetPgmProcessorInfo(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
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
        /* check the protection status */
        if (XCP_PL_PGM == (Xcp_ProtectionStatus & XCP_PL_PGM))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            Xcp_RespBuffer[0u] = 0xFFu;
            Xcp_RespBuffer[1u] = PGM_PROPERTIES;
            Xcp_RespBuffer[2u] = XCP_MAX_SECTOR;
            Xcp_RespLength = 0x03u;
        }
    }
    Xcp_SendResp();
    return;
}
#endif

#if (XCP_GET_SECTOR_INFO == STD_ON)
/******************************************************************************/
/*
 * @brief               <Xcp_Get_Sector_Info>
 *
 * <This optional command is only helpful for the programming method
 *  absolute access mode> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_GetSectorInfo(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 getSectorMode = Xcp_CmdBuffer[1u];
    uint8 getsectorNum = Xcp_CmdBuffer[2u];

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 3u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check the protection status */
        if (XCP_PL_PGM == (Xcp_ProtectionStatus & XCP_PL_PGM))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        else if (getsectorNum >= XCP_MAX_SECTOR)
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            switch (getSectorMode)
            {
            /* get start address for this SECTOR mode = 0u */
            case XCP_GER_SECTOR_INFO_MOD_STAD: {
                Xcp_RespBuffer[1u] = Xcp_SectorInfo[getsectorNum].sectorclrSequenceNo;
                Xcp_RespBuffer[2u] = Xcp_SectorInfo[getsectorNum].sectorPgmSequenceNo;
                Xcp_RespBuffer[3u] = Xcp_SectorInfo[getsectorNum].sectorPgmMethod;

                /* if mode = 0,get the start address of the sector */
                Xcp_CopyU4ToU1Buffer(
                    Xcp_SectorInfo[getsectorNum].sectorclrStartAddr,
                    &(Xcp_RespBuffer[4u]),
                    CPU_BYTE_ORDER);
                Xcp_RespLength = 0x08u;
                break;
            }
            /* get length of this SECTOR [BYTE] mode = 1u*/
            case XCP_GER_SECTOR_INFO_MOD_LEN: {
                Xcp_RespBuffer[1u] = Xcp_SectorInfo[getsectorNum].sectorclrSequenceNo;
                Xcp_RespBuffer[2u] = Xcp_SectorInfo[getsectorNum].sectorPgmSequenceNo;
                Xcp_RespBuffer[3u] = Xcp_SectorInfo[getsectorNum].sectorPgmMethod;

                /* get the length of the sector */
                Xcp_CopyU4ToU1Buffer(
                    Xcp_SectorInfo[getsectorNum].sectorclrLength,
                    &(Xcp_RespBuffer[4u]),
                    CPU_BYTE_ORDER);
                Xcp_RespLength = 0x08u;
                break;
            }
            /* get name length of this SECTOR */
            case XCP_GER_SECTOR_INFO_MOD_NAME: {
                /* command automatically sets the Memory Transfer Address (MTA) to
                 * the location from which the Master
                 */
                Xcp_RespBuffer[1u] = Xcp_SectorInfo[getsectorNum].sectorNameLength;

                Xcp_RespLength = 0x02u;

                /* Set the MTA so that the master can upload the ID string. */
                /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */
                Xcp_MTA.transferAddr = (uint32)(Xcp_SectorInfo[getsectorNum].sectorNamePtr);
                /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
                Xcp_MTA.extensionAddr = 0x0u;
                Xcp_UploadInfoLen = (((uint32)Xcp_RespBuffer[1u] - 1u) / XCP_AG) + 1u;
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
#endif

#if (XCP_PROGRAM_PREPARE == STD_ON)
/******************************************************************************/
/*
 * @brief               <Xcp_ProgramPrepare>
 *
 * < xcp program prepare used for xcp bootloader function, not realized yet> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_ProgramPrepare(void)
{
#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 6u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
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
/******************************************************************************/
/*
 * @brief               <Xcp_ProgramFormat>
 *
 * <This optional command is used for data encryption and compression and defined
    the program method,access method > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_ProgramFormat(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    /* get the Compression Method from CommandRxObject[1] buffer */
    uint8 pgmCompressionMethod = Xcp_CmdBuffer[1u];
    /* get the Encryption_Method from CommandRxObject[2] buffer */
    uint8 pgmEncryptionMethod = Xcp_CmdBuffer[2u];
    /* get the Programming method from CommandRxObject[3] buffer */
    uint8 pgmProgrammingMethod = Xcp_CmdBuffer[3u];
    /* get the access method from CommandRxObject[4] buffer */
    uint8 pgmAccessMethod = Xcp_CmdBuffer[4u];

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (STD_ON == XCP_ON_CAN_ENABLE))
    if ((Xcp_CmdLength != 5u) && (Xcp_CmdLength != XCP_CAN_MAX_DLC))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
        /* check the protection status */
        if (XCP_PL_PGM == (Xcp_ProtectionStatus & XCP_PL_PGM))
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
            Xcp_RespLength = 0x02u;
        }
        /* Program_format should be used under XCP_PGM_CLEARED status */
        else if (XCP_PGM_CLEARED != Xcp_PgmStauts)
        {
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
            Xcp_RespLength = 0x02u;
        }
        /* only support default mode */
        else if ((0u != pgmCompressionMethod) || (0u != pgmEncryptionMethod) || (0u != pgmProgrammingMethod))
        {
            /* only support default mode */
            Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);
            Xcp_RespLength = 0x02u;
        }
        else
        {
            switch (pgmAccessMethod)
            {
            case XCP_ABSOLUTE_ACCESS: {
                Xcp_Program_AccessMode = XCP_ABSOLUTE_ACCESS;
                break;
            }
            case XCP_FUNCTIONAL_ACCESS: {
                /* Indicate the next progam uses functional mode */
                Xcp_Program_AccessMode = XCP_FUNCTIONAL_ACCESS;
                /* Initialise the block seq counter to 1 */
                Xcp_Pgm_BlockSeqCounter = 0x01u;
                break;
            }
            default: {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
                Xcp_RespLength = 0x02u;
                break;
            }
            }
        }
    Xcp_SendResp();
    return;
}
#endif

#if (XCP_PROGRAM_NEXT == STD_ON)
/******************************************************************************/
/*
 * @brief               <Xcp_ProgramNext>
 *
 * <This optional command used for block mode to program a block data to flash> .
 * Service ID   :       <SERVICE ID>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_ProgramNext(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    Std_ReturnType result = E_OK;
    Xcp_PgmBufferIndexType cnt;
    uint32 programStartAddress = 0u;

    if (((Std_ReturnType)E_OK == Xcp_CheckPgmStatus()) && (XCP_PGM_PROGRAMMING != Xcp_PgmStauts))
    {
        if ((Xcp_SeqCheckType)SEQ_OK == Xcp_GetPgmStartAddr(&programStartAddress, (uint8)Xcp_BlockBufferLen))
        {
            /*if Xcp_FlsBufferSize is 0, set Xcp_FlsTargetAddr for calibration software(e.g. CANAPE) optimize */
            if (0x00u == Xcp_FlsBufferSize)
            {
                Xcp_FlsTargetAddr = programStartAddress;
            }
        }
        else
        {
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
            result = E_NOT_OK;
            Xcp_RespLength = 0x02u;
        }
        if ((Std_ReturnType)E_OK == result)
        {
            /* set the position to 0 */
            Xcp_BlockBufferPos = 0u;
            /* block buffer has enough bytes connect with flsBuffer bytes to achieve min write size */
            if (Xcp_BlockBufferLen >= (XCP_FLS_MIN_WRITE_SIZE - Xcp_FlsBufferSize))
            {
                for (cnt = 0u; cnt < (XCP_FLS_MIN_WRITE_SIZE - Xcp_FlsBufferSize); cnt++)
                {
                    Xcp_FlsBufAlignPtr[Xcp_FlsBufferSize + cnt] = Xcp_BlockBuffer[cnt];
                }

                Xcp_BlockBufferPos += cnt;
                /* set fls buffer size to min write size */
                Xcp_FlsBufferSize = XCP_FLS_MIN_WRITE_SIZE;
                switch (Xcp_Program_AccessMode)
                {
                case XCP_FUNCTIONAL_ACCESS: {
                    Xcp_Pgm_BlockSeqCounter++;
                    break;
                }
                case XCP_ABSOLUTE_ACCESS: {
                    Xcp_UpdateMTA((uint32)cnt);
                    break;
                }
                default: {
                    /* Do Nothing */
                    break;
                }
                }
                /* remained data More than min write size, and can do a write action */
                if ((Std_ReturnType)E_OK
                    == Xcp_FlsReq(XCP_FLS_WRITE, Xcp_FlsTargetAddr, XCP_FLS_MIN_WRITE_SIZE, Xcp_FlsBufAlignPtr))
                {
                    Xcp_PgmPendingFlag |= XCP_BLOCK_PROGRAM_PENDING;
                }
                else
                {
                    /* set the state to do flash program action */
                    Xcp_PgmPendingFlag |= XCP_BLOCK_PROGRAM_TASK;
                }
            }
            else /**< block buffer hasn't enough bytes*/
            {
                for (cnt = 0u; cnt < Xcp_BlockBufferLen; cnt++)
                {
                    Xcp_FlsBufAlignPtr[Xcp_FlsBufferSize + cnt] = Xcp_BlockBuffer[cnt];
                }
                /* Fls Buffer increase,but not achieve to min write size */
                Xcp_FlsBufferSize += cnt;
                switch (Xcp_Program_AccessMode)
                {
                case XCP_FUNCTIONAL_ACCESS: {
                    Xcp_Pgm_BlockSeqCounter++;
                    break;
                }
                case XCP_ABSOLUTE_ACCESS: {
                    Xcp_UpdateMTA((uint32)cnt);
                    break;
                }
                default: {
                    /* Do Nothing */
                    break;
                }
                }
            }
        } /* end of E_OK == result */
    }
    else
    {
        Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        Xcp_RespLength = 0x02u;
    }
    if (0u == Xcp_PgmPendingFlag)
    {
        Xcp_SendResp();
    }
    return;
}
#endif

#if (XCP_PROGRAM_MAX == STD_ON)
static FUNC(void, XCP_CODE) Xcp_ProgramMaxHal(void)
{
    Std_ReturnType result = E_NOT_OK;
    Xcp_SeqCheckType seqResult;
    uint32 programStartAddress = 0u;

    if (XCP_PGM_CLEARED == Xcp_PgmStauts)
    {
        /* do porgram sequence check and get program start address */
        seqResult = Xcp_GetPgmStartAddr(&programStartAddress, XCP_PROGRAMMAX_SIZE);
        if (SEQ_OK == seqResult)
        {
            Xcp_FlsTargetAddr = programStartAddress;
            /*  program size of program_max is fixed */
            result = Xcp_Program_NonZero(XCP_PROGRAMMAX_SIZE, XCP_PROGRAMMAX_SIZE);
            if ((Std_ReturnType)E_OK == result)
            {
                /* set the program status as XCP_PGM_PROGRAMMING */
                Xcp_PgmStauts = XCP_PGM_PROGRAMMING;
            }
            else
            {
                Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
                Xcp_RespLength = 0x02u;
            }
        }
        else if (SEQ_ERROR == seqResult) /* clear sequence ocuured */
        {
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
            Xcp_RespLength = 0x02u;
        }
        else /**< SEQ_OUTRANGE == seqResult */
        {
            Xcp_SetErrorCode(XCP_ERR_ACCESS_DENIED);
            Xcp_RespLength = 0x02u;
        }
    }
    else if (XCP_PGM_PROGRAMMING == Xcp_PgmStauts)
    {
        switch (Xcp_Program_AccessMode)
        {
        case XCP_ABSOLUTE_ACCESS: {
            /*get address form MTA[0]*/
            programStartAddress = Xcp_Mta2Ptr(Xcp_MTA.extensionAddr, Xcp_MTA.transferAddr);
            /*if Xcp_FlsBufferSize is 0, set Xcp_FlsTargetAddr for calibration software(e.g. CANAPE) optimize */
            if (0x00u == Xcp_FlsBufferSize)
            {
                Xcp_FlsTargetAddr = programStartAddress;
                result = Xcp_Program_NonZero(XCP_PROGRAMMAX_SIZE, XCP_PROGRAMMAX_SIZE);
            }
            else if ((Xcp_FlsBufferSize + Xcp_FlsTargetAddr) != programStartAddress)
            {
                /* Xcp_FlsBufferSize != 0, but the address not continuous,
                 * so clear the size to 0.
                 */
                Xcp_FlsBufferSize = 0;
                Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
                Xcp_RespLength = 0x02u;
            }
            else
            {
                result = Xcp_Program_NonZero(XCP_PROGRAMMAX_SIZE, XCP_PROGRAMMAX_SIZE);
            }
            break;
        }
        case XCP_FUNCTIONAL_ACCESS: {
            result = Xcp_Program_NonZero(XCP_PROGRAMMAX_SIZE, XCP_PROGRAMMAX_SIZE);
            break;
        }
        default: {
            result = E_NOT_OK;
            break;
        }
        }
        if ((Std_ReturnType)E_NOT_OK == result)
        {
            Xcp_SetErrorCode(XCP_ERR_OUT_OF_RANGE);
            Xcp_RespLength = 0x02u;
        }
    }
    else
    {
        /* This address is not erased */
        Xcp_SetErrorCode(XCP_ERR_WRITE_PROTECTED);
        Xcp_RespLength = 0x02u;
    }
    return;
}
/******************************************************************************/
/*
 * @brief               <Xcp_ProgramMax>
 *
 * <This optional command can not be used in block mode, fixed size(MAX_CTO_PGM-1)
    shall be flashed into  NVM > .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_ProgramMax(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{

    /* check frame length */
    if (Xcp_CmdLength != (XCP_PROGRAM_MAX_SIZE + 1u))
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    /* check the protection status */
    else if (XCP_PL_PGM == (Xcp_ProtectionStatus & XCP_PL_PGM))
    {
        Xcp_SetErrorCode(XCP_ERR_ACCESS_LOCKED);
        Xcp_RespLength = 0x02u;
    }
    /* check the program status */
    else if ((XCP_PGM_IDLE == Xcp_PgmStauts) || (XCP_PGM_START == Xcp_PgmStauts))
    {
        Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
        Xcp_RespLength = 0x02u;
    }
    else
    {
        Xcp_ProgramMaxHal();
    }
    if (0u == Xcp_PgmPendingFlag)
    {
        Xcp_SendResp();
    }
    return;
}
#endif

#if (XCP_PROGRAM_VERIFY == STD_ON)
/******************************************************************************/
/*
 * @brief               <Xcp_ProgramVerify>
 *
 * <This optional command verify the flashed data whether correct or suitable> .
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 */
/******************************************************************************/
FUNC(void, XCP_CODE) Xcp_ProgramVerify(void)
{
    uint8 verificationMode;
    uint16 verificationType;
    uint32 verificationValue;

#if ((STD_OFF == XCP_CAN_MAX_DLC_REQUIRED) && (XCP_ON_CAN_ENABLE == STD_ON))
    if (Xcp_CmdLength < 8u)
    {
        Xcp_SetErrorCode(XCP_ERR_CMD_SYNTAX);
        Xcp_RespLength = 0x02u;
    }
    else
#endif
    {
        /* get the mode from CommandRxObject[1] buffer */
        verificationMode = Xcp_CmdBuffer[1u];

        /* get the verification type from CommandRxObject[2] buffer */
        Xcp_CopyU1BufferToU2(&Xcp_CmdBuffer[2u], &verificationType);

        /* get the verification value from CommandRxObject[4]-[7] buffer */
        Xcp_CopyU1BufferToU4(&Xcp_CmdBuffer[4u], &verificationValue);

        /* check the program status */
        if (XCP_PGM_PROGRAMMED != Xcp_PgmStauts)
        {
            Xcp_SetErrorCode(XCP_ERR_SEQUENCE);
            Xcp_RespLength = 0x02u;
        }
        /* if request to start internal routine*/
        else if (0u == verificationMode)
        {
            Xcp_SetErrorCode(XCP_ERR_RES_TEMP_NOT_A);
            Xcp_RespLength = 0x02u;
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
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#endif /*XCP_PL_PGM == XCP_PL_PGM&XCP_RESOURCE*/

/*=======[E N D   O F   F I L E]==============================================*/
