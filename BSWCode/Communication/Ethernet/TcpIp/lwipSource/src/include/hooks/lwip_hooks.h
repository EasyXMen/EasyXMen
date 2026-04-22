#ifndef LWIP_HOOKS_H_
#define LWIP_HOOKS_H_

#include "TcpIp_Cfg.h"
#include "lwip/prot/ethernet.h"
#if TCPIP_LWIP_HOOKS

#if (STD_ON == TCPIP_TCP_SYNISN_RECALC)
u32_t TcpIp_HookTcpIsn(const ip_addr_t* local_ip, u16_t local_port, const ip_addr_t* remote_ip, u16_t remote_port);
#endif /* (STD_ON == TCPIP_TCP_SYNISN_RECALC) */

#if (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL)
struct netif* TcpIp_HookIp4InputAccept(struct netif* netifPtr, const ip_addr_t* destIpPtr);

uint8 TcpIp_HookEthArpInputForus(const struct netif* netifPtr, const ip_addr_t* destAddrPtr, ip_addr_t* rltAddrPtr);

void TcpIp_HookGratuitousArp(struct netif* netifPtr);
const ip4_addr_t* TcpIp_HookEthArpGetGw(struct netif* netifPtr, const ip_addr_t* destAddrPtr);

#if (STD_ON == TCPIP_UDP_ENABLED)
uint8 TcpIp_UdpTransmitLocalIpCompare(const ip_addr_t* pcbLocalAddrPtr, const struct netif* netifPtr);
#endif /* (STD_ON == TCPIP_UDP_ENABLED) */

#endif /* (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL) */

s32_t TcpIp_HookVlanSet(
    const struct netif* netifPtr,
    const struct pbuf* pbufPtr,
    const struct eth_addr* srcPtr,
    const struct eth_addr* dstPtr,
    uint16 frameType);

#endif /* TCPIP_LWIP_HOOKS */

#endif /* LWIP_HOOKS_H_ */