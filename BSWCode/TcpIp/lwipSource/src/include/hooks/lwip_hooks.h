#ifndef LWIP_HOOKS_H_
#define LWIP_HOOKS_H_

#include "TcpIp_Cfg.h"
#include "lwip/prot/ethernet.h"

#if TCPIP_LWIP_HOOKS

#if defined(LWIP_HOOK_IPERF_GET_TIME_100US)
extern uint32 TcpIp_HookGetTimeNow100us(void);
#endif

#if (defined TCPIP_TCP_SYNISN_RECALC) && (STD_ON == TCPIP_TCP_SYNISN_RECALC)
extern uint32
    TcpIp_HookTcpIsn(const ip_addr_t* local_ip, u16_t local_port, const ip_addr_t* remote_ip, u16_t remote_port);
#endif

#if (STD_ON == TCPIP_CTRL_MULTI_LOCALADDR)
extern struct netif* TcpIp_HookIp4InputAccept(struct netif* netifPtr, const ip_addr_t* destIpPtr);

extern uint8 TcpIp_HookEthArpInputForUs(const struct netif* netifPtr, const ip_addr_t* destIpPtr, ip_addr_t* srcIpPtr);

extern const ip4_addr_t* TcpIp_HookEthArpGetGw(struct netif* netifPtr, const ip_addr_t* destIpPtr);

extern void TcpIp_HookGratuitousArp(struct netif* netifPtr);

#if (STD_ON == TCPIP_UDP_ENABLED)
extern uint8 TcpIp_UdpTransmitLocalIpCompare(const ip_addr_t* localIpPtr, const struct netif* netifPtr);
#endif
#endif

#endif

#endif
