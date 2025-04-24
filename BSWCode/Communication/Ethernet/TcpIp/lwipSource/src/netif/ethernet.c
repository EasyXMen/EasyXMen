/**
 * @file
 * Ethernet common functions
 *
 * @defgroup ethernet Ethernet
 * @ingroup callbackstyle_api
 */

/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * Copyright (c) 2003-2004 Leon Woestenberg <leon.woestenberg@axon.tv>
 * Copyright (c) 2003-2004 Axon Digital Design B.V., The Netherlands.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 */

#include "lwip/opt.h"

#if LWIP_ARP || LWIP_ETHERNET

#include "netif/ethernet.h"
#include "lwip/def.h"
#include "lwip/stats.h"
#include "lwip/etharp.h"
#include "lwip/ip.h"
#include "lwip/snmp.h"
#include <string.h>

#include "EthIf.h"

#ifdef LWIP_HOOK_FILENAME
#include LWIP_HOOK_FILENAME
#endif

extern FUNC(uint8, TCPIP_CODE) TcpIp_GetControlIpFramePriority(VAR(uint8, AUTOMATIC) ctrlIdx);

const struct eth_addr ethbroadcast = {{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};
const struct eth_addr ethzero = {{0, 0, 0, 0, 0, 0}};

#if !defined(ETHERNET_CHECK)
#define ETHERNET_CHECK 1
#endif/* !defined(ETHERNET_CHECK) */
/**
 * @ingroup ethernet
 * Send an ethernet packet on the network using netif->linkoutput().
 * The ethernet header is filled in before sending.
 *
 * @see LWIP_HOOK_VLAN_SET
 *
 * @param netif the lwIP network interface on which to send the packet
 * @param p the packet to send. pbuf layer must be @ref PBUF_LINK.
 * @param src the source MAC address to be copied into the ethernet header
 * @param dst the destination MAC address to be copied into the ethernet header
 * @param eth_type ethernet type (@ref lwip_ieee_eth_type)
 * @return ERR_OK if the packet was sent, any other err_t on failure
 */
err_t
ethernet_output(struct netif* netif, struct pbuf* p,
                const struct eth_addr* src, const struct eth_addr* dst,
                u16_t eth_type)
{
    LWIP_UNUSED_ARG(src);
    err_t lwRet = ERR_OK;
    uint16 bufLen = p->tot_len;
    Eth_BufIdxType bufId = 0;
    uint8_t* dataBufPtr = NULL_PTR;
    Eth_FrameType frameType = eth_type;
    uint8 ethifCtrlIdx = (uint8)(netif->name[1]);
    uint8_t priority = 0;
#if ETHARP_SUPPORT_VLAN && (defined(LWIP_HOOK_VLAN_SET) || LWIP_VLAN_PCP)
    s32_t vlan_prio_vid = -1;
#ifdef LWIP_HOOK_VLAN_SET
    vlan_prio_vid = LWIP_HOOK_VLAN_SET(netif, p, src, dst, eth_type);
#endif
#if LWIP_VLAN_PCP
    if ((vlan_prio_vid < 0) && (netif->hints && (netif->hints->tci >= 0))) {
        vlan_prio_vid = (u16_t)netif->hints->tci;
    }
#endif
    if (vlan_prio_vid >= 0) {
        struct eth_vlan_hdr* vlanhdr;

        LWIP_ASSERT("prio_vid must be <= 0xFFFF", vlan_prio_vid <= 0xFFFF);

        if (pbuf_add_header(p, SIZEOF_VLAN_HDR) != 0) {
            return ERR_BUF;
        }
        vlanhdr = (struct eth_vlan_hdr*)((u8_t*)p->payload);
        vlanhdr->tpid = lwip_htons(frameType);
        vlanhdr->prio_vid = lwip_htons((u16_t)vlan_prio_vid);

        frameType = ETHTYPE_VLAN;
        bufLen = p->tot_len;
    }
#endif /* ETHARP_SUPPORT_VLAN && (defined(LWIP_HOOK_VLAN_SET) || LWIP_VLAN_PCP) */
    
#if defined( LWIP_GETFRAMEPRIORITY)
    priority = LWIP_GETFRAMEPRIORITY(netif->num);
#endif

#if ETHERNET_CHECK

    if (netif->name[0] != 'I')
    {
        return ERR_BUF;
    }

    BufReq_ReturnType bufRet = EthIf_ProvideTxBuffer(ethifCtrlIdx,
                                                     frameType, priority, &bufId, &dataBufPtr, &bufLen);

    if ((bufLen < p->tot_len) || (BUFREQ_OK != bufRet) || (NULL_PTR == dataBufPtr))
    {
        return ERR_BUF;
    }

    /* provide buf more than transmit need length,so shall be crop */
    bufLen = p->tot_len;
    uint16 copiedLen = pbuf_copy_partial(p, dataBufPtr, bufLen, ETH_PAD_SIZE);

    if (copiedLen != p->tot_len)
    {
        return ERR_BUF;
    }

    if (E_OK != EthIf_Transmit(ethifCtrlIdx, bufId, frameType, FALSE, bufLen, (uint8_t*)dst))
    {
        lwRet = ERR_WOULDBLOCK;
    }

#else/* ETHERNET_CHECK == 0 */
    (void)EthIf_ProvideTxBuffer(ethifCtrlIdx, frameType, priority, &bufId, &dataBufPtr, &bufLen);
    /* provide buf more than transmit need length,so shall be crop */
    bufLen = p->tot_len;
    pbuf_copy_partial(p, dataBufPtr, bufLen, ETH_PAD_SIZE);
    (void)EthIf_Transmit(ethifCtrlIdx, bufId, frameType, FALSE, bufLen, (uint8_t*)dst);
#endif/* ETHERNET_CHECK */
    return lwRet;
}


/**
 * @ingroup lwip_nosys
 * Process received ethernet frames. Using this function instead of directly
 * calling ip_input and passing ARP frames through etharp in ethernetif_input,
 * the ARP cache is protected from concurrent access.\n
 * Don't call directly, pass to netif_add() and call netif->input().
 *
 * @param p the received packet, p->payload pointing to the ethernet header
 * @param netif the network interface on which the packet was received
 *
 * @see LWIP_HOOK_UNKNOWN_ETH_PROTOCOL
 * @see ETHARP_SUPPORT_VLAN
 * @see LWIP_HOOK_VLAN_CHECK
 */
err_t
ethernet_input(struct pbuf* p, struct netif* netif)
{
    struct eth_hdr* ethhdr;
    u16_t type;
#if LWIP_ARP || ETHARP_SUPPORT_VLAN || LWIP_IPV6
    u16_t next_hdr_offset = SIZEOF_ETH_HDR;
#endif /* LWIP_ARP || ETHARP_SUPPORT_VLAN */
    LWIP_ASSERT_CORE_LOCKED();

    if (p->len <= SIZEOF_ETH_HDR)
    {
        /* a packet with only an ethernet header (or less) is not valid for us */
        ETHARP_STATS_INC(etharp.proterr);
        ETHARP_STATS_INC(etharp.drop);
        MIB2_STATS_NETIF_INC(netif, ifinerrors);
        goto free_and_return;
    }

    if (p->if_idx == NETIF_NO_INDEX)
    {
        p->if_idx = netif_get_index(netif);
    }

    /* points to packet payload, which starts with an Ethernet header */
    ethhdr = (struct eth_hdr*)p->payload;
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE,
                ("ethernet_input: dest:%"X8_F":%"X8_F":%"X8_F":%"X8_F":%"X8_F":%"X8_F", src:%"X8_F":%"X8_F"\
                :%"X8_F":%"X8_F":%"X8_F":%"X8_F", type:%"X16_F"\n",
                 (unsigned char)ethhdr->dest.addr[0], (unsigned char)ethhdr->dest.addr[1], (unsigned char)ethhdr->dest.addr[2],
                 (unsigned char)ethhdr->dest.addr[3], (unsigned char)ethhdr->dest.addr[4], (unsigned char)ethhdr->dest.addr[5],
                 (unsigned char)ethhdr->src.addr[0], (unsigned char)ethhdr->src.addr[1], (unsigned char)ethhdr->src.addr[2],
                 (unsigned char)ethhdr->src.addr[3], (unsigned char)ethhdr->src.addr[4], (unsigned char)ethhdr->src.addr[5],
                 lwip_htons(ethhdr->type)));
    type = ethhdr->type;
#if ETHARP_SUPPORT_VLAN

    if (type == PP_HTONS(ETHTYPE_VLAN))
    {
        struct eth_vlan_hdr* vlan = (struct eth_vlan_hdr*)(((char*)ethhdr) + SIZEOF_ETH_HDR);
        next_hdr_offset = SIZEOF_ETH_HDR + SIZEOF_VLAN_HDR;

        if (p->len <= SIZEOF_ETH_HDR + SIZEOF_VLAN_HDR)
        {
            /* a packet with only an ethernet/vlan header (or less) is not valid for us */
            ETHARP_STATS_INC(etharp.proterr);
            ETHARP_STATS_INC(etharp.drop);
            MIB2_STATS_NETIF_INC(netif, ifinerrors);
            goto free_and_return;
        }

#if defined(LWIP_HOOK_VLAN_CHECK) || defined(ETHARP_VLAN_CHECK) || defined(ETHARP_VLAN_CHECK_FN) /* if not, allow all VLANs */
#ifdef LWIP_HOOK_VLAN_CHECK

        if (!LWIP_HOOK_VLAN_CHECK(netif, ethhdr, vlan))
        {
#elif defined(ETHARP_VLAN_CHECK_FN)

        if (!ETHARP_VLAN_CHECK_FN(ethhdr, vlan))
        {
#elif defined(ETHARP_VLAN_CHECK)

        if (VLAN_ID(vlan) != ETHARP_VLAN_CHECK)
        {
#endif
            /* silently ignore this packet: not for our VLAN */
            pbuf_free(p);
            return ERR_OK;
        }

#endif /* defined(LWIP_HOOK_VLAN_CHECK) || defined(ETHARP_VLAN_CHECK) || defined(ETHARP_VLAN_CHECK_FN) */
        type = vlan->tpid;
    }

#endif /* ETHARP_SUPPORT_VLAN */
#if LWIP_ARP_FILTER_NETIF
    netif = LWIP_ARP_FILTER_NETIF_FN(p, netif, lwip_htons(type));
#endif /* LWIP_ARP_FILTER_NETIF*/

    if (ethhdr->dest.addr[0] & 1)
    {
        /* this might be a multicast or broadcast packet */
        if (ethhdr->dest.addr[0] == LL_IP4_MULTICAST_ADDR_0)
        {
#if LWIP_IPV4

            if ((ethhdr->dest.addr[1] == LL_IP4_MULTICAST_ADDR_1) &&
                (ethhdr->dest.addr[2] == LL_IP4_MULTICAST_ADDR_2))
            {
                /* mark the pbuf as link-layer multicast */
                p->flags |= PBUF_FLAG_LLMCAST;
            }

#endif /* LWIP_IPV4 */
        }

#if LWIP_IPV6
        else if ((ethhdr->dest.addr[0] == LL_IP6_MULTICAST_ADDR_0) &&
                 (ethhdr->dest.addr[1] == LL_IP6_MULTICAST_ADDR_1))
        {
            /* mark the pbuf as link-layer multicast */
            p->flags |= PBUF_FLAG_LLMCAST;
        }

#endif /* LWIP_IPV6 */
        else if (eth_addr_cmp(&ethhdr->dest, &ethbroadcast))
        {
            /* mark the pbuf as link-layer broadcast */
            p->flags |= PBUF_FLAG_LLBCAST;
        }
    }

    switch (type)
    {
#if LWIP_IPV4 && LWIP_ARP

        /* IP packet? */
        case PP_HTONS(ETHTYPE_IP):
            if (!(netif->flags & NETIF_FLAG_ETHARP))
            {
                goto free_and_return;
            }

            /* skip Ethernet header (min. size checked above) */
            if (pbuf_remove_header(p, next_hdr_offset))
            {
                LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING,
                            ("ethernet_input: IPv4 packet dropped, too short (%"U16_F"/%"U16_F")\n",
                             p->tot_len, next_hdr_offset));
                LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("Can't move over header in packet"));
                goto free_and_return;
            }
            else
            {
                /* pass to IP layer */
                ip4_input(p, netif);
            }

            break;

        case PP_HTONS(ETHTYPE_ARP):
            if (!(netif->flags & NETIF_FLAG_ETHARP))
            {
                goto free_and_return;
            }

            /* skip Ethernet header (min. size checked above) */
            if (pbuf_remove_header(p, next_hdr_offset))
            {
                LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING,
                            ("ethernet_input: ARP response packet dropped, too short (%"U16_F"/%"U16_F")\n",
                             p->tot_len, next_hdr_offset));
                LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("Can't move over header in packet"));
                ETHARP_STATS_INC(etharp.lenerr);
                ETHARP_STATS_INC(etharp.drop);
                goto free_and_return;
            }
            else
            {
                /* pass p to ARP module */
                etharp_input(p, netif);
            }

            break;
#endif /* LWIP_IPV4 && LWIP_ARP */
#if PPPOE_SUPPORT

        case PP_HTONS(ETHTYPE_PPPOEDISC): /* PPP Over Ethernet Discovery Stage */
            pppoe_disc_input(netif, p);
            break;

        case PP_HTONS(ETHTYPE_PPPOE): /* PPP Over Ethernet Session Stage */
            pppoe_data_input(netif, p);
            break;
#endif /* PPPOE_SUPPORT */
#if LWIP_IPV6

        case PP_HTONS(ETHTYPE_IPV6): /* IPv6 */

            /* skip Ethernet header */
            if ((p->len < next_hdr_offset) || pbuf_remove_header(p, next_hdr_offset))
            {
                LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING,
                            ("ethernet_input: IPv6 packet dropped, too short (%"U16_F"/%"U16_F")\n",
                             p->tot_len, next_hdr_offset));
                goto free_and_return;
            }
            else
            {
                /* pass to IPv6 layer */
                ip6_input(p, netif);
            }

            break;
#endif /* LWIP_IPV6 */

        default:
#ifdef LWIP_HOOK_UNKNOWN_ETH_PROTOCOL
            if (LWIP_HOOK_UNKNOWN_ETH_PROTOCOL(p, netif) == ERR_OK)
            {
                break;
            }

#endif
            ETHARP_STATS_INC(etharp.proterr);
            ETHARP_STATS_INC(etharp.drop);
            MIB2_STATS_NETIF_INC(netif, ifinunknownprotos);
            goto free_and_return;
    }

    /* This means the pbuf is freed or consumed,
       so the caller doesn't have to free it again */
    return ERR_OK;
free_and_return:
    pbuf_free(p);
    return ERR_OK;
}

#endif /* LWIP_ARP || LWIP_ETHERNET */