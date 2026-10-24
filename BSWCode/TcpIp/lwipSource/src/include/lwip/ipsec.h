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
 **  @file               : ipsec.h
 **  @author             : xiankun.zhou
 **  @date               :
 **  @vendor             : isoft
 **  @description        : ipsec functuons
 **
 ***********************************************************************************************************************/

/*******************************************************************************
**                      REVISION   HISTORY                                    **
*******************************************************************************/
/* <VERSION>    <DATE>       <AUTHOR>        <REVISION LOG>
 *  V1.0.0    [20251212]  [xiankun.zhou]   Create file.
 ******************************************************************************/

#ifndef LWIP_HDR_PROT_IPSEC_H
#define LWIP_HDR_PROT_IPSEC_H

#include "lwip/opt.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
err_t ipsec_rx_process(struct pbuf *p);
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
err_t ipsec_tx_process(struct pbuf **p_ptr);
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

typedef uint8 TcpIp_IpSecPolicyType;
#define TCPIP_IPSEC_POLICY_PROTECT          ((TcpIp_IpSecPolicyType)(0x0u))
#define TCPIP_IPSEC_POLICY_OPTIONAL         ((TcpIp_IpSecPolicyType)(0x1u))
#define TCPIP_IPSEC_POLICY_BYPASS           ((TcpIp_IpSecPolicyType)(0x2u))
#define TCPIP_IPSEC_POLICY_DISCARD          ((TcpIp_IpSecPolicyType)(0x3u))

typedef uint8 TcpIp_IpSecHeaderType;
#define TCPIP_IPSEC_HDR_AH                  ((TcpIp_IpSecHeaderType)(0x0u))
#define TCPIP_IPSEC_HDR_ESP                 ((TcpIp_IpSecHeaderType)(0x1u))
#define TCPIP_IPSEC_HDR_NONE                ((TcpIp_IpSecHeaderType)(0x2u))

typedef uint8 TcpIp_IpSecModeType;
#define TCPIP_IPSEC_MODE_TRANSPORT ((TcpIp_IpSecModeType)(0x0u))
#define TCPIP_IPSEC_MODE_TUNNEL    ((TcpIp_IpSecModeType)(0x1u))

#define DIRECTION_INBOUND 0
#define DIRECTION_OUTBOUND 1

#define IPSEC_AH_FIXED_LEN 12  // next header(1), payload len(1), reserved(2), SPI(4), seq(4)
#define IPSEC_AUTH_KEY_LEN 32  // dummy key length for HMAC
#define IKE_ESP_AH_SPI_LEN 4u
#define IKE_INTEG_KEY_MAX_LEN 64u

 /* SPD and SAD organized together */
 typedef struct
 {
 	boolean InboundActive:1;
	boolean OutboundActive:1;
	TcpIp_IpSecPolicyType PolicyType;
	TcpIp_IpSecHeaderType HeaderType;
	uint8 Protocol;
	uint8 OutboundSpi[IKE_ESP_AH_SPI_LEN];
	uint8 InboundSpi[IKE_ESP_AH_SPI_LEN];
	uint8 IntegrityAlgorithmListId;
	uint8 EncryptionAlgorithmListId;
	uint8 IKi[IKE_INTEG_KEY_MAX_LEN];
	uint8 IKr[IKE_INTEG_KEY_MAX_LEN];
	uint8 IntegKeyLen;
	uint8 IntegOutputLen;
	TcpIp_IpSecModeType Mode;
	uint16 RemPortRangeStart;
	uint16 RemPortRangeEnd;
	uint16 LocalPortRangeStart;
	uint16 LocalPortRangeEnd;
	uint32 LocalIpAddrStart;
	uint32 LocalIpAddrEnd;
	uint32 RemIpAddrStart;
	uint32 RemIpAddrEnd;
	uint32 InboundSeqNum;
	uint32 OutboundSeqNum;
 } TcpIp_IpSecSAType;

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
 Std_ReturnType TcpIp_IPSecICVGenerate(TcpIp_IpSecSAType *spd_entry, uint8* keyPtr, uint32 keyLen,
	 uint8* dataPtr, uint32 dataLen, uint8* resultPtr, uint32* resultLenPtr);
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
 PACK_STRUCT_BEGIN
 struct ah_hdr {
	 u8_t  next_header;
	 u8_t  payload_len;
	 u16_t reserved;
	 u8_t  spi[IKE_ESP_AH_SPI_LEN];
	 u32_t seq_no;
 } PACK_STRUCT_STRUCT;
 PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif

#endif /* LWIP_HDR_PROT_IPSEC_H */
