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
**  @file               : ipsec.c
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
*  V1.0.0    [20251212]  [xiankun.zhou]   Create file, implement basic ipsec functions.
******************************************************************************/
#include "lwip/opt.h"

#ifdef LWIP_IPSEC_ENABLED

#if LWIP_IPV4

#include "lwip/icmp.h"
#include "lwip/udp.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/ipsec.h"
#include <string.h>

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
extern u16_t inet_chksum(const void *dataptr, u16_t len);
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
/**
 * @brief Strip the AH header from an inbound IP packet
 *
 * @param p		   pbuf containing the packet
 * @param iphdr	   pointer to the IP header in p->payload
 * @param ip_hdr_len  length of IP header in bytes
 * @param ah_total_len length of the entire AH header (including ICV)
 * @param next_header the protocol to restore in IP header
 * @return err_t ERR_OK if stripped successfully, otherwise ERR_VAL
 */
err_t ipsec_strip_ah_header(struct pbuf *p, struct ip_hdr *iphdr, u16_t ip_hdr_len, u16_t ah_total_len, u8_t next_header)
{
  u16_t remaining = p->tot_len - (ip_hdr_len + ah_total_len);
  if (remaining == 0)
  {
	 LWIP_DEBUGF(IP_DEBUG, ("[IPsec] No payload after AH, dropping\n"));
	 return ERR_VAL;
  }

  // Shift payload left to remove AH header
  memmove((u8_t *)p->payload + ip_hdr_len,
		   (u8_t *)p->payload + ip_hdr_len + ah_total_len,
		   remaining);

  // Update IP header fields
  IPH_PROTO_SET(iphdr, next_header);
  u16_t new_total_len = lwip_ntohs(IPH_LEN(iphdr)) - ah_total_len;
  IPH_LEN_SET(iphdr, lwip_htons(new_total_len));
  iphdr->_chksum = 0;
  iphdr->_chksum = inet_chksum(iphdr, ip_hdr_len);

  // Adjust pbuf lengths
  p->len -= ah_total_len;
  p->tot_len -= ah_total_len;

  return ERR_OK;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
static err_t ipsec_insert_ah_header(struct pbuf *p, struct ip_hdr *iphdr, u16_t old_tot_len,
									 u8_t* spi, u32_t seq_no, u16_t ah_total_len)
{
  u16_t ip_hlen   = IPH_HL_BYTES(iphdr);
  u16_t old_payload = old_tot_len - ip_hlen;
  struct ip_hdr *new_iphdr = (struct ip_hdr *)p->payload;

  // Shift original payload forward to make space for AH header
  memmove((u8_t*)p->payload + ip_hlen + ah_total_len,
		   (u8_t*)p->payload + ip_hlen,
		   old_payload);

  // Fill AH header
  struct ah_hdr *ah = (struct ah_hdr *)((u8_t*)p->payload + ip_hlen);
  ah->next_header	= IPH_PROTO(iphdr); // store original protocol
  ah->payload_len	= (ah_total_len >> 2) - 2;
  ah->reserved 	= 0;
  memcpy(ah->spi, spi, IKE_ESP_AH_SPI_LEN);
  ah->seq_no		= lwip_htonl(seq_no);

  // Update new IP header
  IPH_PROTO_SET(new_iphdr, IP_PROTO_AH);
  u16_t new_len = lwip_ntohs(IPH_LEN(iphdr)) + ah_total_len;
  IPH_LEN_SET(new_iphdr, lwip_htons(new_len));
  new_iphdr->_chksum = 0;
  new_iphdr->_chksum = inet_chksum(new_iphdr, ip_hlen);

  return ERR_OK;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "TcpIp_MemMap.h"
extern TcpIp_IpSecSAType TcpIp_IpSecSadSpdList[];
#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
TcpIp_IpSecSAType* ipsec_spd_match(u32_t remote_ip, u32_t local_ip, u8_t proto, u16_t remote_port, u16_t local_port)
{
  for (int i = 0; i < TCPIP_MAX_SA_NUMBER; i++) {
	 TcpIp_IpSecSAType* spd_entry = &TcpIp_IpSecSadSpdList[i];

	 if ((spd_entry->InboundActive == FALSE) || (spd_entry->OutboundActive == FALSE))
	   continue;

	 if (spd_entry->Protocol && (spd_entry->Protocol != proto))
	   continue;

	 if ((remote_port < spd_entry->RemPortRangeStart) || (remote_port > spd_entry->RemPortRangeEnd))
	   continue;

	 if ((local_port < spd_entry->LocalPortRangeStart) || (local_port > spd_entry->LocalPortRangeEnd))
	   continue;

	 if ((remote_ip < spd_entry->RemIpAddrStart) || (remote_ip > spd_entry->RemIpAddrEnd))
	   continue;

	 if ((local_ip < spd_entry->LocalIpAddrStart) || (local_ip > spd_entry->LocalIpAddrEnd))
	   continue;

	 return spd_entry;
  }

  return NULL;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
err_t ipsec_get_port(void *next_proto_hdr, u8_t proto, u16_t *src_port, u16_t *dst_port)
{
  switch (proto)
  {
	 case IP_PROTO_TCP:
	 {
	   struct tcp_hdr *tcphdr = (struct tcp_hdr *)next_proto_hdr;
	   *src_port = lwip_ntohs(tcphdr->src);
	   *dst_port = lwip_ntohs(tcphdr->dest);
	   break;
	 }

	 case IP_PROTO_UDP:
	 {
	   struct udp_hdr *udphdr = (struct udp_hdr *)next_proto_hdr;
	   *src_port = lwip_ntohs(udphdr->src);
	   *dst_port = lwip_ntohs(udphdr->dest);
	   break;
	 }

	 case IP_PROTO_ICMP:
	 {
	   struct icmp_echo_hdr *icmphdr = (struct icmp_echo_hdr *)next_proto_hdr;
	   *src_port = ((u16_t)icmphdr->type << 8) | icmphdr->code;
	   *dst_port = *src_port;
	   break;
	 }

	 default:
	   *src_port = 0;
	   *dst_port = 0;
	 return ERR_VAL;
  }

  return ERR_OK;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
err_t ipsec_rx_process(struct pbuf *p)
{
  struct ip_hdr *iphdr = (struct ip_hdr *)p->payload;
  u8_t proto = IPH_PROTO(iphdr);

  if (proto != IP_PROTO_AH) {
	 return ERR_OK; // Not AH, pass through
  }

  u8_t ip_hdr_len = IPH_HL_BYTES(iphdr);
  u8_t ip_hlen = IPH_HL(iphdr) << 2;
  const u8_t *ah_base = (u8_t *)p->payload + ip_hdr_len;

  struct ah_hdr *ah = (struct ah_hdr *)ah_base;
  u8_t ah_next_proto = ah->next_header;
  u16_t ah_total_len = (ah->payload_len + 2) << 2;
  u8_t *ah_next_proto_hdr = ah_base + ah_total_len;

  u32_t spi = lwip_ntohl((u32_t)ah->spi);
  u32_t seq = lwip_ntohl(ah->seq_no);
  u8_t *icv = ah_base + IPSEC_AH_FIXED_LEN;

  u16_t remote_port = 0, local_port = 0;
  ipsec_get_port(ah_next_proto_hdr, ah_next_proto, &remote_port, &local_port);

  TcpIp_IpSecSAType* spd_entry = ipsec_spd_match(
	 iphdr->src.addr, iphdr->dest.addr, ah_next_proto, remote_port, local_port);
  if (NULL == spd_entry)
  {
	 return ERR_OK;
  }

  switch (spd_entry->PolicyType)
  {
	 case TCPIP_IPSEC_POLICY_PROTECT:
	 {
	   break;
	 }

	 case TCPIP_IPSEC_POLICY_DISCARD:
	 {
	   return ERR_VAL;
	 }

	 case TCPIP_IPSEC_POLICY_OPTIONAL:
	 case TCPIP_IPSEC_POLICY_BYPASS:
	 {
	   return ERR_OK;
	 }

	 default:
	   return ERR_OK;
  }

  if ((0 != memcmp(spd_entry->OutboundSpi, ah->spi, IKE_ESP_AH_SPI_LEN))
	 //|| (spd_entry->InboundSeqNum != seq)
	 )
  {
	 return ERR_VAL;
  }
  spd_entry->InboundSeqNum++;

  // Copy packet data for HMAC
  struct pbuf * auth_data_pbuf = pbuf_clone(PBUF_RAW, PBUF_POOL, p);
  if (!auth_data_pbuf) {
	   return ERR_MEM;
  }

  /*
	RFC4302 3.3.3.1

	If a field may be modified during transit, the value of the field is
	set to zero for purposes of the ICV computation.  If a field is
	mutable, but its value at the (IPsec) receiver is predictable, then
	that value is inserted into the field for purposes of the ICV
	calculation.
  */

  /* set ICV to 0 */
  memset((u8_t*)auth_data_pbuf->payload + ip_hdr_len + IPSEC_AH_FIXED_LEN, 0, spd_entry->IntegOutputLen);

  struct ip_hdr *auth_ip = (struct ip_hdr *)auth_data_pbuf->payload;
  auth_ip->_chksum = 0;
  auth_ip->_ttl = 0;
  auth_ip->_tos = 0;
  auth_ip->_offset = 0;

  u32_t result_len = 32;
  u8_t verify_result[32] = {0};

  TcpIp_IPSecICVGenerate(spd_entry, spd_entry->IKr, spd_entry->IntegKeyLen, auth_data_pbuf->payload, auth_data_pbuf->tot_len, verify_result, &result_len);
  pbuf_free(auth_data_pbuf);

  if (0 != memcmp(verify_result, icv, spd_entry->IntegOutputLen))
  {
	   LWIP_DEBUGF(IP_DEBUG, ("[IPsec] AH integrity check FAILED\n"));
	   return ERR_VAL;
  }

  LWIP_DEBUGF(IP_DEBUG, ("[IPsec] AH integrity check PASSED\n"));

  // --- AH header stripping ---
  if (ipsec_strip_ah_header(p, iphdr, ip_hdr_len, ah_total_len, ah_next_proto) != ERR_OK) {
	 return ERR_VAL;
  }

  return ERR_OK;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
err_t ipsec_tx_process(struct pbuf **p_ptr)
{
  struct pbuf *p = *p_ptr;
  struct ip_hdr *iphdr = (struct ip_hdr *)p->payload;
  u8_t orig_proto = IPH_PROTO(iphdr);
  u16_t ip_hlen   = IPH_HL_BYTES(iphdr);

  // Extract upper layer ports
  u8_t *ah_next_proto_hdr = (u8_t*)p->payload + ip_hlen;
  u16_t remote_port = 0, local_port = 0;
  ipsec_get_port(ah_next_proto_hdr, orig_proto, &local_port, &remote_port);

  // Match SPD
  TcpIp_IpSecSAType *spd_entry = ipsec_spd_match(iphdr->dest.addr, iphdr->src.addr, orig_proto, remote_port, local_port);
  if (!spd_entry) {
	 return ERR_OK; // no IPsec policy applied
  }

  switch (spd_entry->PolicyType)
  {
	 case TCPIP_IPSEC_POLICY_PROTECT:
	 {
	   break;
	 }

	 case TCPIP_IPSEC_POLICY_DISCARD:
	 {
	   return ERR_VAL;
	 }

	 case TCPIP_IPSEC_POLICY_OPTIONAL:
	 case TCPIP_IPSEC_POLICY_BYPASS:
	 {
	   return ERR_OK;
	 }

	 default:
	   return ERR_OK;
  }

  u8_t spi[IKE_ESP_AH_SPI_LEN]    = {0};
  memcpy(spi, spd_entry->InboundSpi, IKE_ESP_AH_SPI_LEN);
  u32_t seq_no = spd_entry->OutboundSeqNum;
  spd_entry->OutboundSeqNum++;

  // Calculate total AH header length (aligned to 4 bytes)
  u16_t ah_total_len = (IPSEC_AH_FIXED_LEN + spd_entry->IntegOutputLen + 3) & ~3;

  // Clone packet for ICV calculation
  struct pbuf *auth_p = pbuf_alloc(PBUF_RAW, p->tot_len + ah_total_len, PBUF_POOL);
  if (!auth_p) {
	 return ERR_MEM;
  }
  pbuf_copy(auth_p, p);

  // cover origin p pointer
  *p_ptr = auth_p;
  pbuf_free(p);

  if (ipsec_insert_ah_header(auth_p, iphdr, p->tot_len, spi, seq_no, ah_total_len) != ERR_OK) {
	 return ERR_VAL;
  }

  u8_t *clone_ah_base = (u8_t*)auth_p->payload + ip_hlen;
  memset(clone_ah_base + IPSEC_AH_FIXED_LEN, 0, spd_entry->IntegOutputLen);

  struct ip_hdr *ciphdr = (struct ip_hdr *)auth_p->payload;
  struct ip_hdr iphdr_tmp = *ciphdr;  // bakup ip header
  ciphdr->_chksum = 0;
  ciphdr->_ttl    = 0;
  ciphdr->_tos    = 0;
  ciphdr->_offset = 0;

  u32_t result_len = spd_entry->IntegOutputLen;
  u8_t icv_buf[32];
  TcpIp_IPSecICVGenerate(spd_entry, spd_entry->IKi, spd_entry->IntegKeyLen, auth_p->payload, auth_p->tot_len, icv_buf, &result_len);

  // Copy ICV to real packet
  memcpy((u8_t*)auth_p->payload + ip_hlen + IPSEC_AH_FIXED_LEN, icv_buf, spd_entry->IntegOutputLen);
  *ciphdr = iphdr_tmp;

  return ERR_OK;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#endif /* LWIP_IPV4 */
#endif
