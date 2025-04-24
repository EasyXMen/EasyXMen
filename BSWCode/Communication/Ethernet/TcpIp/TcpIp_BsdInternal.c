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
************************************************************************************************************************
**                                                                                                                    **
**  FILENAME    : TcpIp_BsdInternal.c                                                                                 **
**                                                                                                                    **
**  Created on  : 2022/11/14                                                                                          **
**  Author      : fupeng.yu                                                                                           **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : internal function for BsdTcpIp                                                                      **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11  base on linux                                               **
**                                                                                                                    **
***********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 ***********************************************************************************************************************/
/* autosar include */
#include "TcpIp_PBcfg.h"
#if (STD_ON == TCPIP_BSDSOCKET_SUPPORT)
#include "TcpIp.h"
#include "TcpIp_Lcfg.h"
#include "TcpIp_BsdInternal.h"
/* system include */
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <pthread.h>
#if (STD_ON == TCIP_USED_MOUDLE_DET)
#include "Det.h"
#endif /* (STD_ON == TCIP_USED_MOUDLE_DET) */

/***********************************************************************************************************************
**                                              Version                                                               **
***********************************************************************************************************************/
#define TCPIP_BSD_C_AR_MAJOR_VERSION 4
#define TCPIP_BSD_C_AR_MINOR_VERSION 5
#define TCPIP_BSD_C_AR_PATCH_VERSION 0

/***********************************************************************************************************************
 *  VERSION CHECK
 ***********************************************************************************************************************/
/* check the version of TcpIp header files */
#if (                                                             \
    (TCPIP_BSD_C_AR_MAJOR_VERSION != TCPIP_H_AR_MAJOR_VERSION)    \
    || (TCPIP_BSD_C_AR_MINOR_VERSION != TCPIP_H_AR_MINOR_VERSION) \
    || (TCPIP_BSD_C_AR_PATCH_VERSION != TCPIP_H_AR_PATCH_VERSION))
#error "version numbers of TcpIp.h and TcpIp.c are inconsistent"
#endif

/* check the version of the configuration header file */
#if (                                                                 \
    (TCPIP_BSD_C_AR_MAJOR_VERSION != TCPIP_CFG_H_AR_MAJOR_VERSION)    \
    || (TCPIP_BSD_C_AR_MINOR_VERSION != TCPIP_CFG_H_AR_MINOR_VERSION) \
    || (TCPIP_BSD_C_AR_PATCH_VERSION != TCPIP_CFG_H_AR_PATCH_VERSION))
#error "version numbers of TcpIp_Cfg.h and TcpIp.c are inconsistent"
#endif
/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 ***********************************************************************************************************************/

#if !defined(TCPIP_SOCKET_NONBLOCK)
#define TCPIP_SOCKET_NONBLOCK 1
#endif /* !defined(TCPIP_SOCKET_NONBLOCK) */

#if !defined(TCPIP_FUNC_CHECK_OUTPUT)
#include "func_trace.h"
#define TCPIP_FUNC_CHECK_OUTPUT(...) FuncCheckOutput(__VA_ARGS__)
#endif /* !defined(TCPIP_FUNC_CHECK_OUTPUT) */

#if !defined(TCPIP_ENABLE_DEBUG)
#define TCPIP_ENABLE_DEBUG 0
#endif /* !defined(TCPIP_ENABLE_DEBUG) */

#if TCPIP_ENABLE_DEBUG
#define TCPIP_DEBUG(...)         \
    do                           \
    {                            \
        printf("TCPIP_DEBUG>>"); \
        printf(__VA_ARGS__);     \
    } while (0)
#else
#define TCPIP_DEBUG(...)
#endif /* TCPIP_ENABLE_DEBUG */

#if TCPIP_ENABLE_DEBUG_TIMER
#define TCPIP_DEBUG_TIMER()           \
    do                                \
    {                                 \
        printf("TCPIP_DEBUG_TIME>>"); \
        TcpIp_BsdPrintfTimer();       \
    } while (0)
#else
#define TCPIP_DEBUG_TIMER()
#endif /* TCPIP_ENABLE_DEBUG_TIMER */

#ifndef TCPIP_SUPPORT_MULTICAST
#define TCPIP_SUPPORT_MULTICAST (TCPIP_UDP_SOCKET_MAX > 0)
#endif /* TCPIP_SUPPORT_MULTICAST */

/* I/O multplexing method */
#define TCPIP_LINUX_EPOLL 1
#define TCPIP_LINUX_POLL  2

#if !defined(TCPIP_IO_MULTPLEXING_METHOD)
/* #define TCPIP_IO_MULTPLEXING_METHOD TCPIP_LINUX_EPOLL */
#define TCPIP_IO_MULTPLEXING_METHOD TCPIP_LINUX_POLL
#endif /* !defined(TCPIP_IO_MULTPLEXING_METHOD) */
#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL)
#include <sys/epoll.h>
#elif (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_POLL)
#include <poll.h>
#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL) */

#define TCPIP_MAX_BUF_LEN 1500u

/**In the scenario of multiple physical network ports, the IP address of each network port is unique.
 * linux: The network interface cannot be determined directly using SO BINDTODEVICE,shell open this function switch
 */
#ifndef TCPIP_MULTIP_DIRECT_MAP
#define TCPIP_MULTIP_DIRECT_MAP 0
#endif /* TCPIP_MULTIP_DIRECT_MAP */

#ifndef TCPIP_SPPORT_SCAN_NETIF_NUMBER
#define TCPIP_SPPORT_SCAN_NETIF_NUMBER 4u
#endif /* TCPIP_SPPORT_SCAN_NETIF_NUMBER */

#ifndef TCPIP_CTRL_NUM
#if (TCPIP_CONTROLLER_NUM > 1)
#define TCPIP_CTRL_NUM (TCPIP_CONTROLLER_NUM + TCPIP_SPPORT_SCAN_NETIF_NUMBER)
#else
#define TCPIP_CTRL_NUM 0
#endif /* (TCPIP_CONTROLLER_NUM > 1) */
#endif /* TCPIP_CTRL_NUM */

/**All socket communications use separate network interfaces. You need to enable this function
 * but this switch used SO_BINDTODEVICE socket option,on linux this option need high operation by call user
 */
#ifndef TCPIP_USED_FIXED_NETIF
#define TCPIP_USED_FIXED_NETIF 0
#endif /* TCPIP_USED_FIXED_NETIF */

/**Depends on the switch TCPIP USED FIXED NETIF,name of the network interface used for the configuration
 */
#ifndef TCPIP_USED_FIXED_NETIF_NAME
#define TCPIP_USED_FIXED_NETIF_NAME "eth0"
#endif /* TCPIP_USED_FIXED_NETIF_NAME */

/* qnx used raw sokcet transmit udp message,this message ip DF =1 */
#ifndef TCPIP_USED_RAW_SEND_UDP
#define TCPIP_USED_RAW_SEND_UDP 0
#endif /* TCPIP_USED_RAW_SEND_UDP */

#define TCPIP_AREA_TX  (0)
#define TCPIP_AREA_RX  (1)
#define TCPIP_AREA_ALL (2)

#ifndef SchM_Enter_TcpIp_ExclusiveArea
#define SchM_Enter_TcpIp_ExclusiveArea() \
    do                                   \
    {                                    \
        Schm_TcpIp_BsdLock();            \
    } while (0)
#define SchM_Enter_TcpIp_ExclusiveArea_Tx SchM_Enter_TcpIp_ExclusiveArea
#define SchM_Enter_TcpIp_ExclusiveArea_Rx SchM_Enter_TcpIp_ExclusiveArea
#endif

#ifndef SchM_Exit_TcpIp_ExclusiveArea
#define SchM_Exit_TcpIp_ExclusiveArea() \
    do                                  \
    {                                   \
        Schm_TcpIp_BsdUnlock();         \
    } while (0)
#define SchM_Exit_TcpIp_ExclusiveArea_Tx SchM_Exit_TcpIp_ExclusiveArea_Tx
#define SchM_Exit_TcpIp_ExclusiveArea_Rx SchM_Enter_TcpIp_ExclusiveArea_Rx
#endif

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

#if (TCPIP_LOCAL_ADR_NUM > 0)
#define LOCALADR_FLAG_REQUEST_IP (uint32)0x1u
#define LOCALADR_FLAG_IP_VAID    (uint32)0x2u
#define LOCALADR_FLAG_IP_ANY     (uint32)0x4u
typedef struct
{
    VAR(TcpIp_SockAddrType, TCPIP_VAR) netmask;
    VAR(TcpIp_SockAddrType, TCPIP_VAR) ipAdrSrc;
    VAR(TcpIp_SockAddrType, TCPIP_VAR) defaultrouter;
    VAR(uint32, TCPIP_VAR) flag;
} TcpIp_LocalAdrHandleType;
#endif /* TCPIP_LOCAL_ADR_NUM > 0 */

#if (TCPIP_SOCKET_NUM > 0u)
#if ((TCPIP_TCP_SOCKET_MAX > 0) || TCPIP_TCP_SOCKET_MAX)

#define TCPIP_SOCKET_IS_LISTEN              0x01
#define TCPIP_SOCKET_CONNECT_PENDING        0x02
#define TCPIP_SOCKET_IS_MULTICAST           0x04
#define TCPIP_SOCKET_IS_BINDANY             0x08

#define TCPIP_SOCKET_SET_FLAG(sockPtr, flg) ((sockPtr)->flag |= (flg))
#define TCPIP_SOCKET_CLR_FLAG(sockPtr, flg) ((sockPtr)->flag &= (uint32) ~(flg))
#define TCPIP_SOCKET_IS_FLAG(sockPtr, flg)  ((sockPtr)->flag & (flg))
#endif /* ((TCPIP_TCP_SOCKET_MAX > 0) || TCPIP_TCP_SOCKET_MAX) */

typedef struct TcpIp_SocketMangeDataTag
{
    /* file descriptor for the socket, error value is less than 0 */
    int socket_fd;
    TcpIp_DomainType domain;
#if TCPIP_SUPPORT_MULTICAST
    uint16 port;
#endif /* TCPIP_SUPPORT_MULTICAST */

    TcpIp_ProtocolType protocol;
#if (TCPIP_CTRL_NUM > 0)
    uint32 netIfIndex;
#endif /* (TCPIP_CTRL_NUM > 0) */
    uint32 flag;
#if TCPIP_SUPPORT_MULTICAST
    const TcpIp_LocalAdrHandleType* localAdtPtr;
#endif /* TCPIP_SUPPORT_MULTICAST */

    const TcpIp_SocketOwnerLCfgType* socketOwnerCfgPtr;
    struct TcpIp_SocketMangeDataTag* prev;
    struct TcpIp_SocketMangeDataTag* next;
#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_POLL)
    int pollEvtIndex;
#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_POLL) */
#if TCPIP_SUPPORT_MULTICAST
    /* multicast shall be link normal socket */
    struct TcpIp_SocketMangeDataTag* linkOther;
#endif /* TCPIP_SUPPORT_MULTICAST */
} TcpIp_SocketMangeDataType;
#endif /* (TCPIP_SOCKET_NUM > 0u) */

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
#if (STD_ON == TCPIP_INITIALIZATION_AVAILABLE)
typedef uint8 TcpIp_InitStateType;
#define TCPIP_STATE_UNINIT 0x0u
#define TCPIP_STATE_INIT   0x1u
TCPIP_LOCAL VAR(TcpIp_InitStateType, TCPIP_VAR_ZERO_INIT) TcpIp_InitState = TCPIP_STATE_UNINIT;

#endif /* (STD_ON == TCPIP_INITIALIZATION_AVAILABLE) */

TCPIP_LOCAL P2CONST(TcpIp_ConfigType, AUTOMATIC, TCPIP_VAR) TcpIp_PbCfgPtr;

TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_BsdChanegeSocketTcpNagle(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(TcpIp_ParamIdType, AUTOMATIC) parameterId,
    P2CONST(void, AUTOMATIC, TCPIP_APPL_CONST) paraPtr);

TCPIP_LOCAL CONST(TcpIp_SocketParaHandleType, TCPIP_VAR) TcpIp_SocketParaHandleTebla[] = {
    {TCPIP_PARAMID_FRAMEPRIO, NULL_PTR},
#if (STD_ON == TCPIP_TCP_ENABLED)
    {TCPIP_PARAMID_TCP_NAGLE, TcpIp_BsdChanegeSocketTcpNagle},
#endif /* (STD_ON == TCPIP_TCP_ENABLED) */
    {TCPIP_PARAMID_TTL, NULL_PTR},
#if (STD_ON == TCPIP_TCP_ENABLED)
    {TCPIP_PARAMID_TCP_RXWND_MAX, NULL_PTR},
    {TCPIP_PARAMID_TCP_KEEPALIVE, NULL_PTR},
    {TCPIP_PARAMID_TCP_KEEPALIVE_TIME, NULL_PTR},
    {TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX, NULL_PTR},
    {TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL, NULL_PTR},
    {TCPIP_PARAMID_TCP_OPTIONFILTER, NULL_PTR},
#endif /* (STD_ON == TCPIP_TCP_ENABLED) */
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
    {TCPIP_PARAMID_TLS_CONNECTION_ASSIGNMENT, NULL_PTR},
#endif /* (STD_ON == TCPIP_TCP_TLS_ENABLED) */
    {TCPIP_PARAMID_PATHMTU_ENABLE, NULL_PTR},
    {TCPIP_PARAMID_FLOWLABEL, NULL_PTR},
    {TCPIP_PARAMID_DSCP, NULL_PTR},
#if (STD_ON == TCPIP_UDP_ENABLED)
    {TCPIP_PARAMID_UDP_CHECKSUM, NULL_PTR},
#endif /* (STD_ON == TCPIP_UDP_ENABLED) */
    {TCPIP_PARAMID_VENDOR_SPECIFIC, NULL_PTR},
};

TcpIp_SocketMangeDataType* TcpIp_SocketMngHeaderPtr = NULL;

#if (TCPIP_TCP_SOCKET_MAX > 0)
TCPIP_LOCAL VAR(TcpIp_SocketIdType, TCPIP_VAR) Tcp_SocketUsedNum;
#endif /* (TCPIP_TCP_SOCKET_MAX > 0) */

#if (TCPIP_UDP_SOCKET_MAX > 0)
TCPIP_LOCAL VAR(TcpIp_SocketIdType, TCPIP_VAR) Udp_SocketUsedNum;
#endif /* (TCPIP_UDP_SOCKET_MAX > 0) */

#if (TCPIP_LOCAL_ADR_NUM > 0)
TCPIP_LOCAL VAR(TcpIp_LocalAdrHandleType, TCPIP_VAR) TcpIp_LocalAdrTable[TCPIP_LOCAL_ADR_NUM];
#endif /* (TCPIP_LOCAL_ADR_NUM > 0) */

#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL)
TCPIP_LOCAL int TcpIp_epfd = -1;
#define TCPIP_EPOLL_MAXEVENTS TCPIP_SOCKET_NUM
#define TCPIP_EPOLL_WAITTIME  (0)
TCPIP_LOCAL struct epoll_event* TcpIp_EpollEvntsPtr = NULL;
#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL) */

#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_POLL)
TCPIP_LOCAL struct pollfd TcpIp_PollEvent[TCPIP_SOCKET_NUM];
#define TCPIP_POLL_WAITTIME (0)
#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_POLL) */

#if (TCPIP_CTRL_NUM > 0)
TCPIP_LOCAL struct ifreq TcpIp_IfReq[TCPIP_CTRL_NUM];
TCPIP_LOCAL uint32 TcpIp_NumIf = 0;
#endif /* (TCPIP_CTRL_NUM > 0) */

TCPIP_LOCAL pthread_mutex_t* mutexPtr = NULL;
/***********************************************************************************************************************
 *  LOCAL FUNCTIONS PROTOTYPES
 **********************************************************************************************************************/
#if TCPIP_SUPPORT_MULTICAST
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_BsdUdpJoinMultCast(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(TcpIp_LocalAdrHandleType, AUTOMATIC, TCPIP_APPL_VAR) multAdtPtr,
    boolean isAdd);
#endif /* TCPIP_SUPPORT_MULTICAST */
/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) Schm_TcpIp_BsdLock(void)
{
    if (NULL == mutexPtr)
    {
        mutexPtr = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(mutexPtr, NULL);
        pthread_mutex_lock(mutexPtr);
    }
}
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) Schm_TcpIp_BsdUnlock(void)
{
    if (NULL != mutexPtr)
    {
        pthread_mutex_unlock(mutexPtr);
        pthread_mutex_destroy(mutexPtr);
        free(mutexPtr);
        mutexPtr = NULL;
    }
}
#if TCPIP_ENABLE_DEBUG_TIMER
#include <sys/time.h>
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_BsdPrintfTimer(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long tv_ms = tv.tv_sec * 1000u + tv.tv_usec / 1000u;
    printf("millisecond :%ld\n", tv_ms);
}
#endif /* TCPIP_ENABLE_DEBUG_TIMER */
#if (TCPIP_CTRL_NUM > 0)
TCPIP_LOCAL_INLINE FUNC(int, TCPIP_CODE)
    TcpIp_BsdSockCmpAddr(const struct sockaddr* sa1, const struct sockaddr* sa2, socklen_t salen)
{
    if (sa1->sa_family != sa2->sa_family)
    {
        return (-1);
    }

    if (sa1->sa_family == AF_INET)
    {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)

        return memcmp(
            &((struct sockaddr_in*)sa1)->sin_addr,
            &((struct sockaddr_in*)sa2)->sin_addr,
            sizeof(struct in_addr));
    }
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
    if (sa1->sa_family == AF_INET6)
    {
        return memcmp(
            &((struct sockaddr_in6*)sa1)->sin6_addr,
            &((struct sockaddr_in6*)sa2)->sin6_addr,
            sizeof(struct in6_addr));
    }
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    return (-1);
}

TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_BsdInitNetIf(void)
{
    struct ifconf ifc;
    ifc.ifc_len = sizeof(TcpIp_IfReq);
    ifc.ifc_buf = (caddr_t)TcpIp_IfReq;
    TcpIp_SocketIdType fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        close(fd);
        TCPIP_FUNC_CHECK_OUTPUT("create socket error:%d\n", errno);
        return;
    }
    int res = ioctl(fd, SIOCGIFCONF, (char*)&ifc);
    if (res)
    {
        close(fd);
        TCPIP_FUNC_CHECK_OUTPUT("get if config info failed:%d\n", errno);
        return;
    }
    TcpIp_NumIf = ifc.ifc_len / sizeof(struct ifreq);
    for (uint32 index = 0u; index < TcpIp_NumIf; index++)
    {
        TCPIP_DEBUG("interface index:%d name:%s\n", index, TcpIp_IfReq[index].ifr_name);
    }
    close(fd);
    TCPIP_DEBUG("system num is interface = %d\n", TcpIp_NumIf);
}

#if (STD_ON == TCPIP_UDP_ENABLED) && !(TCPIP_MULTIP_DIRECT_MAP)
TCPIP_LOCAL FUNC(ssize_t, TCPIP_CODE) TcpIp_BsdUdpTransmitBySpecNetIf(
    P2CONST(TcpIp_SocketMangeDataType, AUTOMATIC, TCPIP_APPL_CONST) sockMngPtr,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) dataPtr,
    VAR(size_t, AUTOMATIC) dataLen,
    P2VAR(struct sockaddr, AUTOMATIC, TCPIP_APPL_CONST) bsdSockAdrPtr,
    VAR(socklen_t, AUTOMATIC) socklen)
{
    struct msghdr msg = {0};
    msg.msg_name = bsdSockAdrPtr;
    msg.msg_namelen = socklen;
    struct iovec iov = {0};
    iov.iov_base = (void*)dataPtr;
    iov.iov_len = dataLen;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    uint32 netIfIndex = sockMngPtr->netIfIndex;
    size_t pktlen;
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)

    if (sockMngPtr->domain == TCPIP_AF_INET)
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
        pktlen = sizeof(struct in_pktinfo);

#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    }
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)

    else
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
        pktlen = sizeof(struct in6_pktinfo);

#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    }

    size_t cmsglen = CMSG_SPACE(pktlen);
    void* msgctl = malloc(cmsglen);
    if (msgctl == NULL)
    {
        TCPIP_FUNC_CHECK_OUTPUT("malloc error,socket id:%d\n", sockMngPtr->socket_fd);
        return -1;
    }
    memset(msgctl, 0, cmsglen);
    msg.msg_controllen = cmsglen;
    msg.msg_control = msgctl;
    struct cmsghdr* cmptr = CMSG_FIRSTHDR(&msg);
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)

    if (sockMngPtr->domain == TCPIP_AF_INET)
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
        cmptr->cmsg_level = IPPROTO_IP;
        cmptr->cmsg_type = IP_PKTINFO;
        cmptr->cmsg_len = CMSG_LEN(sizeof(struct in_pktinfo));

        struct in_pktinfo* pktptr = (struct in_pktinfo*)CMSG_DATA(cmptr);
        pktptr->ipi_ifindex = (int)netIfIndex;
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    }
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)

    else
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */

    {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
        cmptr->cmsg_level = IPPROTO_IPV6;
        cmptr->cmsg_type = IPV6_PKTINFO;
        cmptr->cmsg_len = CMSG_LEN(sizeof(in6_pktinfo));

        struct in6_pktinfo* pktptr = (struct in6_pktinfo*)CMSG_DATA(cmptr);
        pktptr->ipi6_ifindex = netIfIndex;
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    }
    ssize_t rln = sendmsg(sockMngPtr->socket_fd, &msg, 0);
    free(msgctl);
    return rln;
}
#endif /* (STD_ON == TCPIP_UDP_ENABLED) && !(TCPIP_MULTIP_DIRECT_MAP) */
#endif /* (TCPIP_CTRL_NUM > 0) */

#if (STD_ON == TCPIP_UDP_ENABLED) && (TCPIP_USED_RAW_SEND_UDP)
/** \brief only support qnx transmit DF =1 udp message */
TCPIP_LOCAL FUNC(ssize_t, TCPIP_CODE) TcpIp_BsdUdpTransmitByRawSocket(
    P2CONST(TcpIp_SocketMangeDataType, AUTOMATIC, TCPIP_APPL_CONST) sockMngPtr,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) dataPtr,
    VAR(size_t, AUTOMATIC) dataLen,
    P2VAR(struct sockaddr, AUTOMATIC, TCPIP_APPL_CONST) bsdSockAdrPtr,
    VAR(socklen_t, AUTOMATIC) socklen)
{
    ssize_t rln = -1;
    int raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (raw_socket < 0)
    {
        TCPIP_FUNC_CHECK_OUTPUT("raw socket create error\n");
        return rln;
    }
    int res;
#if TCPIP_USED_FIXED_NETIF
    struct ifreq interface;
    strncpy(interface.ifr_name, TCPIP_USED_FIXED_NETIF_NAME, sizeof(TCPIP_USED_FIXED_NETIF_NAME));
    res = setsockopt(raw_socket, SOL_SOCKET, SO_BINDTODEVICE, (char*)&interface, sizeof(interface));
    if (res < 0)
    {
        TCPIP_FUNC_CHECK_OUTPUT("SO_BINDTODEVICE return error%d\n", errno);
        close(raw_socket);
        return rln;
    }
#endif /* TCPIP_USED_FIXED_NETIF */
    /* open socket used ip header */
    int hold = 1;
    res = setsockopt(raw_socket, IPPROTO_IP, IP_HDRINCL, &hold, sizeof(hold));
    if (res < 0)
    {
        TCPIP_FUNC_CHECK_OUTPUT("IP_HDRINCL return error%d\n", errno);
        close(raw_socket);
        return rln;
    }
#if TCPIP_SUPPORT_MULTICAST && defined(TCPIP_MULTICAST_USED_UNICAST_INDEX)
    const TcpIp_LocalAddrType* localAdrCfgPtr = &(TcpIp_PbCfgPtr->LocalAdrPtr[TCPIP_MULTICAST_USED_UNICAST_INDEX]);
    const TcpIp_StaticIpAddressConfigType* localIpAdrPtr = localAdrCfgPtr->StaticIpAddressCfgPtr;
    struct in_addr multiCast = {0};
    if ((localIpAdrPtr != NULL_PTR) && (localIpAdrPtr->StaticIpAddressPtr != NULL_PTR))
    {
        multiCast.s_addr = localIpAdrPtr->StaticIpAddressPtr[0];
    }
    else
    {
        multiCast.s_addr = htonl(INADDR_ANY);
        TCPIP_FUNC_CHECK_OUTPUT("multicast not used any local addr\n");
    }
    res = setsockopt(raw_socket, IPPROTO_IP, IP_MULTICAST_IF, &multiCast, sizeof(multiCast));
    if (res < 0)
    {
        TCPIP_FUNC_CHECK_OUTPUT("SO_DONTROUTE return error%d\n", errno);
        close(raw_socket);
        return rln;
    }
#endif /* TCPIP_SUPPORT_MULTICAST && defined(TCPIP_MULTICAST_USED_UNICAST_INDEX) */
    /* malloc data used fill data */
    size_t tr_len = sizeof(struct ip) + sizeof(struct udphdr) + dataLen;
    uint8* dataTmpPtr = (uint8*)malloc(tr_len);
    if (dataTmpPtr == NULL)
    {
        TCPIP_FUNC_CHECK_OUTPUT("malloc error,socket id:%d\n", raw_socket);
        close(raw_socket);
        return rln;
    }
    struct ip* iphdrPtr = (struct ip*)dataTmpPtr;
    iphdrPtr->ip_v = IPVERSION;
    iphdrPtr->ip_hl = sizeof(struct ip) >> 2;
    iphdrPtr->ip_tos = 0;
    iphdrPtr->ip_id = 0;
    iphdrPtr->ip_off = IP_DF;
    iphdrPtr->ip_ttl = MAXTTL;
    iphdrPtr->ip_len = htons(sizeof(struct ip) + sizeof(struct udphdr) + dataLen);
    iphdrPtr->ip_p = IPPROTO_UDP;
    iphdrPtr->ip_sum = 0;
    /* source ip is any,open syetem shall fill value */
    iphdrPtr->ip_src.s_addr = htonl(INADDR_ANY);
    iphdrPtr->ip_dst = ((struct sockaddr_in*)bsdSockAdrPtr)->sin_addr;
    /* udp heeader */
    struct udphdr* udphdrPtr = (struct udphdr*)(dataTmpPtr + sizeof(struct ip));
    struct sockaddr_in bsdSockAddr;
    socklen_t soLen = sizeof(bsdSockAddr);
    /* raw socket udp source port used father socket port */
    res = getsockname(sockMngPtr->socket_fd, (struct sockaddr*)&bsdSockAddr, &soLen);
    if (res < 0)
    {
        TCPIP_FUNC_CHECK_OUTPUT("getsockname socket:%d return error%d\n", sockMngPtr->socket_fd, errno);
        close(raw_socket);
        free(dataTmpPtr);
        return rln;
    }
    udphdrPtr->uh_sport = bsdSockAddr.sin_port;
    udphdrPtr->uh_dport = ((struct sockaddr_in*)bsdSockAdrPtr)->sin_port;
    udphdrPtr->uh_ulen = htons(dataLen + sizeof(struct udphdr));
    /* udp checksum is zero,means the receiver do not care;if you need udp checksum,shall modify driver config */
    udphdrPtr->uh_sum = 0u;
    /* copy data to raw socket */
    memcpy(dataTmpPtr + sizeof(struct udphdr) + sizeof(struct ip), dataPtr, dataLen);
    /* transmit data */
    res = sendto(raw_socket, dataTmpPtr, tr_len, 0, bsdSockAdrPtr, sizeof(struct sockaddr));
    if (res != tr_len)
    {
        /* shall report error */
        TCPIP_FUNC_CHECK_OUTPUT(
            "raw socket:%d base socket:%d udp transmit error:%d\n",
            sockMngPtr->socket_fd,
            raw_socket,
            errno);
    }
    /* close this socket */
    close(raw_socket);
    free(dataTmpPtr);
    return dataLen;
}
#endif /* (STD_ON == TCPIP_UDP_ENABLED) && (TCPIP_USED_RAW_SEND_UDP) */

TCPIP_LOCAL FUNC(ssize_t, TCPIP_CODE) TcpIp_BsdUdpRxPreHandle(
    P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_VAR) socketIdPtr,
    P2CONST(TcpIp_SocketMangeDataType, AUTOMATIC, TCPIP_APPL_VAR) sockMngPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) bufPtr,
    VAR(size_t, AUTOMATIC) bufLen,
    P2VAR(struct sockaddr, AUTOMATIC, TCPIP_APPL_VAR) ipAddrPtr,
    VAR(socklen_t, AUTOMATIC) adrLen)
{
    struct msghdr msg;
    TcpIp_SocketIdType socketId = *socketIdPtr;
    struct iovec iov[1];
    /* The array length should be greater than CMSG_SPACE(sizeof(struct
     * in6_pktinfo)) */
    char cmbuf[128] = {0};
    msg.msg_name = ipAddrPtr;
    msg.msg_namelen = adrLen;
    iov[0].iov_base = bufPtr;
    iov[0].iov_len = bufLen;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_control = cmbuf;
    msg.msg_controllen = sizeof(cmbuf);
    ssize_t len = recvmsg(socketId, &msg, 0);

    if (len < 0)
    {
        TCPIP_FUNC_CHECK_OUTPUT("socket:%d recvmsg error:%d\n", socketId, errno);
        return len;
    }

    /* MSG_CTRUNC:
    Indicates that some control data was discarded due to lack of space in the
    buffer for ancillary data. */
    if (msg.msg_controllen < sizeof(struct cmsghdr) || (msg.msg_flags & MSG_CTRUNC))
    {
        TCPIP_FUNC_CHECK_OUTPUT("socket:%d msg len error:%d\n", socketId, errno);
        return len;
    }

#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
    struct in_pktinfo* pktptr = NULL;
#endif /* (TCPIP_SCALB ILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
    struct in6_pktinfo* in6_pktptr = NULL;
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) ||  (TCPIP_SCALABILITY_CLASS ==  TCPIP_SC3) */

    for (struct cmsghdr* cmptr = CMSG_FIRSTHDR(&msg); cmptr != NULL; cmptr = CMSG_NXTHDR(&msg, cmptr))
    {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
        if (cmptr->cmsg_level == IPPROTO_IP && cmptr->cmsg_type == IP_PKTINFO)
        {
            pktptr = (struct in_pktinfo*)CMSG_DATA(cmptr);
            TCPIP_DEBUG("udp socket:%d,rx dest ip:%x\n", socketId, pktptr->ipi_addr.s_addr);
            break;
        }

#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)

        if (cmptr->cmsg_level == IPPROTO_IPV6 && cmptr->cmsg_type == IPV6_PKTINFO)
        {
            in6_pktptr = (struct in6_pktinfo*)CMSG_DATA(cmptr);
            TCPIP_DEBUG("udp socket:%d,rx det ip:%x\n", socketId, pktptr->ipi6_addr.s6_addr32[0]);
            break;
        }

#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    }

#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)

    if (sockMngPtr->domain == TCPIP_AF_INET)
#endif /* (TCPIP_SCALABIL IT Y_CLASS == TCPIP_SC3) */
    {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
        if (pktptr == NULL)
        {
            TCPIP_FUNC_CHECK_OUTPUT("socket:%d recvmsg not find IP_PKTINFO\n", socketId);
            return len;
        }
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    }
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
    else
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
        if (in6_pktptr == NULL)
        {
            TCPIP_FUNC_CHECK_OUTPUT("socket:%d recvmsg not find IPV6_PKTINFO\n", socketId);
            return len;
        }
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) ||  (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    }
/* multicast only support sc1 */
#if TCPIP_SUPPORT_MULTICAST && (TCPIP_SCALABILITY_CLASS == TCPIP_SC1)
    /* check dest ip is multicast */
    int isMulticast = IN_MULTICAST(ntohl(pktptr->ipi_addr.s_addr));
    /* multicast socket */
    if (TCPIP_SOCKET_IS_FLAG(sockMngPtr, TCPIP_SOCKET_IS_MULTICAST) && (!isMulticast))
    {
        /* multicast socket only receiver multicast message,unicast shall be reject
         */
        TCPIP_DEBUG("normal message shall be reject in multicast socket:%d\n", *socketIdPtr);
        len = 0;
    }
    /* normal socket */
    else if ((sockMngPtr->linkOther != NULL) && (isMulticast))
    {
        /* redirect socket */
        *socketIdPtr = sockMngPtr->linkOther->socket_fd;
        TCPIP_DEBUG("this is multicast udp socket:%d\n", *socketIdPtr);
    }
    else
#endif /* TCPIP_SUPPORT_MULTICAST && (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) */
    {
#if (TCPIP_CTRL_NUM > 0)

        if (INADDR_BROADCAST == pktptr->ipi_addr.s_addr)
        {
            if (TcpIp_BsdSockCmpAddr(ipAddrPtr, &TcpIp_IfReq[sockMngPtr->netIfIndex].ifr_addr, adrLen) == 0)
            {
                TCPIP_DEBUG("refused to broadcast a loopback message,socket:%d\n", *socketIdPtr);
                len = 0;
            }
        }

#endif /* (TCPIP_CTRL_NUM > 0) */
    }

    return len;
}

TCPIP_LOCAL FUNC_P2VAR(TcpIp_SocketMangeDataType, AUTOMATIC, TCPIP_CODE)
    TcpIp_BsdFindSocketMngDataBySocketId(int socketId)
{
    TcpIp_SocketMangeDataType* pollPtr = TcpIp_SocketMngHeaderPtr;

    while (NULL != pollPtr)
    {
        if (pollPtr->socket_fd == socketId)
        {
            return pollPtr;
        }

        pollPtr = pollPtr->next;
    }

    return NULL;
}

TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_BsdFreeSocketMngData(const TcpIp_SocketMangeDataType* sockMngPtr)
{
    TCPIP_DEBUG("free socket mng data:%d\n", sockMngPtr->socket_fd);
    SchM_Enter_TcpIp_ExclusiveArea_Tx();
#if (TCPIP_TCP_SOCKET_MAX > 0)

    if ((sockMngPtr->protocol == TCPIP_IPPROTO_TCP) && (Tcp_SocketUsedNum > 0))
    {
        Tcp_SocketUsedNum--;
    }

#endif /* (TCPIP_TCP_SOCKET_MAX > 0) */
#if (TCPIP_UDP_SOCKET_MAX > 0)

    if ((sockMngPtr->protocol == TCPIP_IPPROTO_UDP) && (Udp_SocketUsedNum > 0))
    {
        Udp_SocketUsedNum--;
    }

#endif /* (TCPIP_UDP_SOCKET_MAX > 0) */

    /* prev handle */
    if (sockMngPtr->prev != NULL)
    {
        sockMngPtr->prev->next = sockMngPtr->next;
    }
    else
    {
        TcpIp_SocketMngHeaderPtr = sockMngPtr->next;
    }

    /* next handle */
    if (NULL != sockMngPtr->next)
    {
        sockMngPtr->next->prev = sockMngPtr->prev;
    }

#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_POLL)

    if (sockMngPtr->pollEvtIndex < TCPIP_SOCKET_NUM)
    {
        TcpIp_PollEvent[sockMngPtr->pollEvtIndex].fd = -1;
        TCPIP_DEBUG("clear poll index:%d\n", sockMngPtr->pollEvtIndex);
    }

#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_POLL) */
    SchM_Exit_TcpIp_ExclusiveArea_Tx();
    free((void*)sockMngPtr);
}

TCPIP_LOCAL FUNC_P2VAR(TcpIp_SocketMangeDataType, AUTOMATIC, TCPIP_CODE)
    TcpIp_BsdMallocSocketMngData(TcpIp_SocketIdType socketId)
{
    TcpIp_SocketMangeDataType* socketMngPtr = malloc(sizeof(TcpIp_SocketMangeDataType));

    if (NULL == socketMngPtr)
    {
        TCPIP_FUNC_CHECK_OUTPUT("malloc error,socket id:%d\n", socketId);
        return socketMngPtr;
    }

    TCPIP_DEBUG("malloc socket id:%d\n", socketId);
    memset(socketMngPtr, 0, sizeof(TcpIp_SocketMangeDataType));
    socketMngPtr->socket_fd = socketId;
    /* add list header */
    SchM_Enter_TcpIp_ExclusiveArea_Tx();
    TcpIp_SocketMangeDataType* itemPtr = TcpIp_SocketMngHeaderPtr;

    if (itemPtr != NULL)
    {
        itemPtr->prev = socketMngPtr;
    }
    else
    {
        TCPIP_DEBUG("first socket id:%d\n", socketId);
    }

    socketMngPtr->next = itemPtr;
    socketMngPtr->prev = NULL;
    TcpIp_SocketMngHeaderPtr = socketMngPtr;
#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_POLL)
    int i = 0;

    for (; i < TCPIP_SOCKET_NUM; i++)
    {
        if (TcpIp_PollEvent[i].fd < 0)
        {
            break;
        }
    }

    if (i < TCPIP_SOCKET_NUM)
    {
        TCPIP_DEBUG("set poll event index:%d\n", i);
        socketMngPtr->pollEvtIndex = i;
        TcpIp_PollEvent[i].fd = socketId;
        TcpIp_PollEvent[i].events = POLLIN | POLLERR;
    }
    else
    {
        TCPIP_FUNC_CHECK_OUTPUT("find valid poll error:%d\n", socketId);
        free(socketMngPtr);
        SchM_Exit_TcpIp_ExclusiveArea_Tx();
        return NULL;
    }

#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_POLL) */
#if TCPIP_SUPPORT_MULTICAST
    /* link other shall be set nullptr in init state */
    socketMngPtr->linkOther = NULL;
#endif /* TCPIP_SUPPORT_MULTICAST */
    SchM_Exit_TcpIp_ExclusiveArea_Tx();
    return socketMngPtr;
}
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_BsdCloseSocket(VAR(TcpIp_SocketIdType, AUTOMATIC) socketId)
{
    /* step1:del epoll */
    int res;
    Std_ReturnType ret = E_NOT_OK;
#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL)
    res = epoll_ctl(TcpIp_epfd, EPOLL_CTL_DEL, socketId, NULL);

    if (res < 0)
    {
        TCPIP_FUNC_CHECK_OUTPUT("close socket:%d delete epoll error:%d\n", socketId, errno);
        return ret;
    }

#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL) */
    /* step2:close socket */
    res = close(socketId);

    if (res < 0)
    {
        TCPIP_FUNC_CHECK_OUTPUT("close socket:%d return error:%d\n", socketId, errno);
        return ret;
    }

    /* step3:free socket */
    TcpIp_SocketMangeDataType* sockMngPtr = TcpIp_BsdFindSocketMngDataBySocketId(socketId);

    if (NULL_PTR != sockMngPtr)
    {
#if TCPIP_SUPPORT_MULTICAST
        if (sockMngPtr->linkOther != NULL)
        {
            if (TCPIP_SOCKET_IS_FLAG(sockMngPtr, TCPIP_SOCKET_IS_MULTICAST))
            {
                /* leave the multicast group */
                TcpIp_BsdUdpJoinMultCast(sockMngPtr->linkOther->socket_fd, sockMngPtr->localAdtPtr, FALSE);
                /* broken link,normal socket shall be reset linkother */
                sockMngPtr->linkOther->linkOther = NULL_PTR;
            }
            /* if normal socket shall be close,multicast socket shall be isolate */
        }
#endif /* TCPIP_SUPPORT_MULTICAST */
        TcpIp_BsdFreeSocketMngData(sockMngPtr);
    }

    ret = E_OK;
    return ret;
}

#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * @ingroup tcpip_internal
 * change specifies if the Nagle Algorithm  on the tcp socket. call by
 * TcpIp_ChangeParameter
 * @param socketId socket index
 * @param parameterId not used
 * @param paraPtr [uint8]Nagle switch state
 * @return E_OK,ok;another failed
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_BsdChanegeSocketTcpNagle(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(TcpIp_ParamIdType, AUTOMATIC) parameterId,
    P2CONST(void, AUTOMATIC, TCPIP_APPL_CONST) paraPtr)
{
    TCPIP_UNUSED_ARG(parameterId);
    Std_ReturnType ret = E_NOT_OK;
    /* nodelay is ture para is 0x0 */
    uint8 para = *(uint8*)paraPtr;
    int bsdRdFlag = 0;
    int bsdWdFlag = 0;
    socklen_t len = sizeof(bsdRdFlag);
    int res = getsockopt(socketId, IPPROTO_TCP, TCP_NODELAY, (void*)&bsdRdFlag, &len);

    if (0 == res)
    {
        bsdWdFlag = bsdRdFlag;

        /* default all tcp socket shall be open nagele  */
        /* bsdflag is true shall be disable ngale meas nodelay */
        if ((para == 0u) && (bsdRdFlag == 0))
        {
            /* request nodelay open, but socket nodelay is close  */
            bsdWdFlag = 1;
        }
        else if ((para != 0u) && (bsdRdFlag != 0))
        {
            /* request nodelay close, but socket nodelay is open  */
            bsdWdFlag = 0;
        }

        if (bsdWdFlag != bsdRdFlag)
        {
            res = setsockopt(socketId, IPPROTO_TCP, TCP_NODELAY, (void*)&bsdWdFlag, len);
            TCPIP_DEBUG("socket:%d,write TCP_NODELAY:%d\n", socketId, bsdWdFlag);
        }
    }

    if (0 == res)
    {
        ret = E_OK;
    }

    return ret;
}
#endif /* (STD_ON == TCPIP_TCP_ENABLED) */
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_BsdAddrChangeBswToBsd(
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) bswAddrPtr,
    struct sockaddr* bsdSockAdrPtr,
    socklen_t* sockLenPtr)
{
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)

    if (bswAddrPtr->domain == TCPIP_AF_INET)
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
        struct sockaddr_in s_localAdr = {0};
        s_localAdr.sin_family = AF_INET;
        /* addr not care byteorder */
        s_localAdr.sin_addr.s_addr = bswAddrPtr->addr[0];
        s_localAdr.sin_port = htons(bswAddrPtr->port);
        *sockLenPtr = sizeof(s_localAdr);
        *bsdSockAdrPtr = *((struct sockaddr*)&s_localAdr);
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    }

#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
    else
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
        struct sockaddr_in6 sv6_localAdr = {0};
        sv6_localAdr.sin6_family = AF_INET6;
        sv6_localAdr.sin6_port = htons(bswAddrPtr->port);

        for (uint32 i = 0; i < 4; i++)
        {
            sv6_localAdr.sin6_addr.__u6_addr32[i] = bswAddrPtr->addr[i];
        }

        *sockLenPtr = sizeof(sv6_localAdr);
        *bsdSockAdrPtr = *((struct sockaddr*)&sv6_localAdr);
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    }
}
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE)
    TcpIp_BsdAddrChangeBsdToBsw(const struct sockaddr* bsdSockAdrPtr, socklen_t socklen, TcpIp_SockAddrType* bswAddrPtr)
{
    TCPIP_UNUSED_ARG(socklen);
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)

    if (bsdSockAdrPtr->sa_family == AF_INET)
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
        const struct sockaddr_in* socketV4Ptr = (const struct sockaddr_in*)bsdSockAdrPtr;
        bswAddrPtr->domain = TCPIP_AF_INET;
        bswAddrPtr->addr[0] = socketV4Ptr->sin_addr.s_addr;
        bswAddrPtr->port = ntohs(socketV4Ptr->sin_port);
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    }

#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
    else if (bsdSockAdrPtr->sa_family == AF_INET6)
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
        const struct sockaddr_in6* socketV6Ptr = (const struct sockaddr_in6*)bsdSockAdrPtr;
        bswAddrPtr->domain = TCPIP_AF_INET6;
        bswAddrPtr->port = ntohs(socketV6Ptr->sin_port);

        for (int i = 0; i < 4; i++)
        {
            bswAddrPtr->addr[i] = socketV6Ptr->sin6_addr.__u6_addr32[i];
        }

#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    }

#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
    else
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    {
    }
}

TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_BsdNotifyEvent(
    P2CONST(TcpIp_SocketMangeDataType, AUTOMATIC, TCPIP_APPL_VAR) sockMngPtr,
    VAR(TcpIp_EventType, AUTOMATIC) evnet)
{
    TCPIP_DEBUG("tcpip_event socket:%d,event:%d\n", sockMngPtr->socket_fd, evnet);
    if ((NULL != sockMngPtr) && (NULL != sockMngPtr->socketOwnerCfgPtr)
        && (NULL != sockMngPtr->socketOwnerCfgPtr->Up_TcpIpEventFuncPtr))
    {
        sockMngPtr->socketOwnerCfgPtr->Up_TcpIpEventFuncPtr(sockMngPtr->socket_fd, evnet);
    }
}

#if (TCPIP_LOCAL_ADR_NUM > 0)

/**
 * @ingroup tcpip_internal
 * convert network mask from outside u8 to inside
 * @param inNetMaskNum outside network mask
 * @param domainType @ref TcpIp_DomainType
 * @param outNteMaskArry output point to memory inside encoding network mask
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_BsdNetMaskExtToInner(
    VAR(uint8, AUTOMATIC) inNetMaskNum,
    VAR(TcpIp_DomainType, AUTOMATIC) domainType,
    P2VAR(uint32, AUTOMATIC, TCPIP_APPL_VAR) outNteMaskArry) /* PRQA S 3432 */
{
    TCPIP_UNUSED_ARG(domainType);
#if (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)

    if (TCPIP_AF_INET == domainType)
#endif /* (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */
    {
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
        /* ipv4 32bit*/
        outNteMaskArry[0] = (uint32)(0xffffffffUL << (32u - inNetMaskNum));
#endif /* (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */
    }

#if (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
    else
#endif /* (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */
    {
#if (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
        /* ipv6 128bit*/
        /* (128u - inNetMaskNum)>96u?32-inNetMaskNum:0  */
        outNteMaskArry[3] = (uint32)(0xffffffffUL << ((inNetMaskNum < 32u) ? (32u - inNetMaskNum) : 0u));
        /* (128u - inNetMaskNum)>64u?(64-inNetMaskNum)%32:0  */
        outNteMaskArry[2] = (uint32)(0xffffffffUL << ((inNetMaskNum < 64u) ? (64u - inNetMaskNum) & 0x1f : 0u));
        /* (128u - inNetMaskNum)>96u?(96-inNetMaskNum)%32:0  */
        outNteMaskArry[1] = (uint32)(0xffffffffUL << ((inNetMaskNum < 64u) ? (64u - inNetMaskNum) & 0x1f : 0u));
        /* (128u - inNetMaskNum)%32  */
        outNteMaskArry[0] = (uint32)(0xffffffffUL << ((128u - inNetMaskNum) & 0x1f));
#endif /* (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */
    }
}
/**
 * @ingroup tcpip_internal
 * convert network mask from inside u8 to outside
 * @param domainType @ref TcpIp_DomainType
 * @param inNteMaskArry point to memory outside encoding network mask
 * @param outNetMaskNumPtr output point to memory outside encoding network mask
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_BsdNetMaskInnerToExt(
    VAR(TcpIp_DomainType, AUTOMATIC) domainType,
    P2CONST(uint32, AUTOMATIC, TCPIP_APPL_VAR) inNteMaskArry,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) outNetMaskNumPtr) /* PRQA S 3432 */
{
    uint8 cnt = 0u;
    TCPIP_UNUSED_ARG(domainType);
#if (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)

    if (TCPIP_AF_INET == domainType)
#endif /* (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */
    {
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
        uint32 vlu = inNteMaskArry[0];

        while (vlu > 0u)
        {
            cnt++;
            vlu <<= 1;
        }

#endif /* (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */
    }

#if (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
    else
#endif /* (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */
    {
#if (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
        uint32 vlu = inNteMaskArry[0];

        for (uintx i = 0; (i < 4) && (vlu > 0u); i++)
        {
            vlu = inNteMaskArry[i];

            while (vlu > 0)
            {
                cnt++;
                vlu <<= 1;
            }
        }

#endif /* (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */
    }

    *outNetMaskNumPtr = cnt;
}
/**
 * Up_LocalIpAddrAssignmentChg handle
 *
 * @param localAdrId local address id
 * @param ipAdrState @ref TcpIp_IpAddrStateType
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_BsdNotfiyUpperLayerIpAdrChange(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) localAdrId,
    VAR(TcpIp_IpAddrStateType, AUTOMATIC) ipAdrState)
{
#if defined(TCPIP_CHECK)

    if ((TcpIp_LCfg.SocketOwnerNum > 0u) && (TcpIp_LCfg.SocketOwnerLCfgPtr != NULL_PTR))
#endif /* defined(TCPIP_CHECK) */
    {
#if (STD_ON == TCPIP_MUILT_SOCKET_OWNERCFG)

        for (uint32 index = 0; index < TcpIp_LCfg.SocketOwnerNum; index++)
#else /* (STD_ON != TCPIP_MUILT_SOCKET_OWNERCFG) */
            uint32 index = 0;

#endif /* (STD_ON == TCPIP_MUILT_SOCKET_OWNERCFG) */
        {
            TcpIp_Up_LocalIpAddrAssignmentChgType localNotifyFuncPtr =
                TcpIp_LCfg.SocketOwnerLCfgPtr[index].Up_LocalIpAddrAssignmentChgFuncPtr;

            /* socket owner is essential spport LocalIpAddrAssignmentChg configurer
             * parameter */
            if (NULL_PTR != localNotifyFuncPtr)
            {
                localNotifyFuncPtr(localAdrId, ipAdrState);
            }
        }
    }

#if defined(TCPIP_CHECK)
    else
    {
        TCPIP_FUNC_CHECK_OUTPUT("link timer config not have socker owner\n");
    }

#endif /* defined(TCPIP_CHECK) */
}
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_BsdLocalIpAddrAssignHandle(
    uint32 localAdrIndex,
    VAR(TcpIp_AssignmentTriggerType, AUTOMATIC) assignTrigType,
    VAR(TcpIp_IpAddrAssignmentType, AUTOMATIC) assignType,
    P2CONST(TcpIp_StaticIpAddressConfigType, AUTOMATIC, TCPIP_APPL_CONST) ipAdrPtr)
{
    Std_ReturnType ret = E_OK;
    TcpIp_LocalAdrHandleType* lcaPtr = &TcpIp_LocalAdrTable[localAdrIndex];
    const TcpIp_LocalAddrType* localAdrCfgPtr = &(TcpIp_PbCfgPtr->LocalAdrPtr[localAdrIndex]);

    if ((localAdrCfgPtr->AddrAssignmentCfgPtr != NULL_PTR) && (localAdrCfgPtr->AddrAssignmentNum > 0u))
    {
#if (TCPIP_LOCALADDR_MUILT_ASSIGNMENT == STD_ON)

        for (uint32 index = 0u; index < localAdrCfgPtr->AddrAssignmentNum; index++)
#else /* (TCPIP_LOCALADDR_MUILT_ASSIGNMENT != STD_ON) */
            uint32 index = 0u;

#endif /* (TCPIP_LOCALADDR_MUILT_ASSIGNMENT == STD_ON) */
        {
            const TcpIp_AddrAssignmentType* addAssCfgPtr = &localAdrCfgPtr->AddrAssignmentCfgPtr[index];
            const TcpIp_StaticIpAddressConfigType* localIpAdrPtr = localAdrCfgPtr->StaticIpAddressCfgPtr;
            boolean continueFlg = FALSE;

            if ((TCPIP_AUTOMATIC == addAssCfgPtr->AssignmentTrigger) && (assignTrigType == TCPIP_AUTOMATIC))
            {
                continueFlg = TRUE;
            }

            if ((TCPIP_MANUAL == addAssCfgPtr->AssignmentTrigger) && (assignTrigType == TCPIP_MANUAL)
                && ((assignType == addAssCfgPtr->AssignmentMethod) || (assignType == TCPIP_IPADDR_ASSIGNMENT_ALL)))
            {
                if (localIpAdrPtr == NULL_PTR)
                {
                    localIpAdrPtr = ipAdrPtr;
                }

                continueFlg = TRUE;
            }

            if (TRUE == continueFlg)
            {
                if (TCPIP_IPADDR_ASSIGNMENT_STATIC == addAssCfgPtr->AssignmentMethod)
                {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)

                    if (localAdtCfgPtr->DomainType == TCPIP_AF_INET)
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
                    {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
                        lcaPtr->ipAdrSrc.domain = TCPIP_AF_INET;
                        lcaPtr->netmask.domain = TCPIP_AF_INET;
                        lcaPtr->defaultrouter.domain = TCPIP_AF_INET;
                        lcaPtr->flag |= LOCALADR_FLAG_IP_ANY;

                        if (NULL_PTR != localIpAdrPtr->StaticIpAddressPtr)
                        {
                            lcaPtr->ipAdrSrc.addr[0] = localIpAdrPtr->StaticIpAddressPtr[0];
                            lcaPtr->flag |= LOCALADR_FLAG_IP_VAID;
                            if (lcaPtr->ipAdrSrc.addr[0] != INADDR_ANY)
                            {
                                lcaPtr->flag &= ~LOCALADR_FLAG_IP_ANY;
                            }
                        }

                        if (NULL_PTR != localIpAdrPtr->NetmaskPtr)
                        {
                            lcaPtr->netmask.addr[0] = localIpAdrPtr->NetmaskPtr[0];
                        }

                        if (NULL_PTR != localIpAdrPtr->DefaultRouterPtr)
                        {
                            lcaPtr->defaultrouter.addr[0] = localIpAdrPtr->DefaultRouterPtr[0];
                        }

#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
                    }

#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
                    else
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
                    {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
                        lcaPtr->ipAdrSrc.domain = TCPIP_AF_INET6;
                        lcaPtr->netmask.domain = TCPIP_AF_INET6;
                        lcaPtr->defaultrouter.domain = TCPIP_AF_INET6;
                        lcaPtr->flag |= LOCALADR_FLAG_IP_ANY;
                        for (int i = 0; i < 4 : i++)
                        {
                            if (NULL_PTR != localIpAdrPtr > StaticIpAddressPtr)
                            {
                                lcaPtr->ipAdrSrc.addr[i] = localIpAdrPtr->StaticIpAddressPtr[i];
                                lcaPtr->flag |= LOCALADR_FLAG_IP_VAID;
                                if (lcaPtr->ipAdrSrc.addr[i] != INADDR_ANY)
                                {
                                    lcaPtr->flag &= ~LOCALADR_FLAG_IP_ANY;
                                }
                            }

                            if (NULL_PTR != localIpAdrPtr->NetmaskPtr)
                            {
                                lcaPtr->netmask.addr[i] = localIpAdrPtr->NetmaskPtr[i];
                            }

                            if (NULL_PTR != localIpAdrPtr->DefaultRouterPtr)
                            {
                                lcaPtr->defaultrouter.addr[i] = localIpAdrPtr->DefaultRouterPtr[i];
                            }
                        }

#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
                    }

                    ret = E_OK;
                }
                else
                {
                    /* not support DHCP or autoip */
                    TCPIP_FUNC_CHECK_OUTPUT(
                        "local addr :%u assiggment method %d is error\n",
                        localAdrIndex,
                        addAssCfgPtr->AssignmentMethod);
                    ret = E_NOT_OK;
                    return ret;
                }

                if ((lcaPtr->flag & LOCALADR_FLAG_IP_VAID) != 0u)
                {
                    TcpIp_BsdNotfiyUpperLayerIpAdrChange(localAdrIndex, TCPIP_IPADDR_STATE_ASSIGNED);
#if (TCPIP_LOCALADDR_MUILT_ASSIGNMENT == STD_ON)
                    break;
#endif /* (TCPIP_LOCALADDR_MUILT_ASSIGNMENT == STD_ON) */
                }
            }
        }
    }

    return ret;
}
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_BsdLocalIpAddrUnAssignHandle(uint32 localAdrIndex)
{
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_LocalAdrHandleType* lcaPtr = &TcpIp_LocalAdrTable[localAdrIndex];
    lcaPtr->flag &= ~LOCALADR_FLAG_IP_VAID;
    /* notify upper layer */
    TcpIp_BsdNotfiyUpperLayerIpAdrChange(localAdrIndex, TCPIP_IPADDR_STATE_UNASSIGNED);
    return E_OK;
}
#endif /* (TCPIP_LOCAL_ADR_NUM > 0) */

TCPIP_LOCAL FUNC(uint32, TCPIP_CODE) TcpIp_BsdTcpWriteData(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) dataPtr,
    VAR(uint32, AUTOMATIC) availableLength)
{
    size_t totLen = availableLength;
    const uint8* dataWrPtr = dataPtr;

    while (totLen > 0)
    {
        ssize_t wrLen = write(socketId, dataWrPtr, totLen);

        if (wrLen <= 0)
        {
            if (((wrLen < 0) && (errno == EINTR)) || (errno == EAGAIN))
            {
                wrLen = 0;
            }
            else
            {
                /* error */
                TCPIP_FUNC_CHECK_OUTPUT("socket:%d write data error:%d\n", socketId, errno);
                break;
            }
        }

        dataWrPtr += wrLen;
        totLen -= wrLen;
    }

    return totLen;
}
#if TCPIP_SUPPORT_MULTICAST

TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_BsdUdpJoinMultCast(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(TcpIp_LocalAdrHandleType, AUTOMATIC, TCPIP_APPL_VAR) multAdtPtr,
    boolean isAdd)
{
    int ret = 0;
#ifdef TCPIP_MULTICAST_USED_UNICAST_INDEX
    const TcpIp_LocalAddrType* localAdrCfgPtr = &(TcpIp_PbCfgPtr->LocalAdrPtr[TCPIP_MULTICAST_USED_UNICAST_INDEX]);
    const TcpIp_StaticIpAddressConfigType* localIpAdrPtr = localAdrCfgPtr->StaticIpAddressCfgPtr;
#endif /* TCPIP_MULTICAST_USED_UNICAST_INDEX */
    if (isAdd)
    {
        /* By default, the loopback mechanism is enabled for multicast */
        char loopEn = 0; /* 0 close loop;1 open loop */
        ret = setsockopt(socketId, IPPROTO_IP, IP_MULTICAST_LOOP, &loopEn, sizeof(loopEn));
        if (ret < 0)
        {
            TCPIP_FUNC_CHECK_OUTPUT("setsockopt IP_MULTICAST_LOOP error:%d\n", errno);
        }
    }
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)

    if (localAdtCfgPtr->DomainType == TCPIP_AF_INET)
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
        struct ip_mreq multiCast = {0};
        multiCast.imr_multiaddr.s_addr = multAdtPtr->ipAdrSrc.addr[0];
#ifdef TCPIP_MULTICAST_USED_UNICAST_INDEX
        if ((localIpAdrPtr != NULL_PTR) && (localIpAdrPtr->StaticIpAddressPtr != NULL_PTR))
        {
            multiCast.imr_interface.s_addr = localIpAdrPtr->StaticIpAddressPtr[0];
        }
        else
        {
            multiCast.imr_interface.s_addr = htonl(INADDR_ANY);
        }
#endif /* TCPIP_MULTICAST_USED_UNICAST_INDEX */
        if (TRUE == isAdd)
        {
            ret = setsockopt(socketId, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multiCast, sizeof(multiCast));
#ifdef TCPIP_MULTICAST_USED_UNICAST_INDEX
            /* qnx shall be set IP_MULTICAST_IF,when transmit multicast msg */
            ret = setsockopt(
                socketId,
                IPPROTO_IP,
                IP_MULTICAST_IF,
                (char*)&multiCast.imr_interface,
                sizeof(struct in_addr));
#endif /* TCPIP_MULTICAST_USED_UNICAST_INDEX */
        }
        else
        {
            ret = setsockopt(socketId, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&multiCast, sizeof(multiCast));
        }

#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    }

#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
    else
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
        struct ipv6_mreq multiCastV6 = {0};
        /* ipv6 select zero interface */
        /* multiCastV6.ipv6mr_interface = 0; */
        for (uint32 i = 0; i < 4; i++)
        {
            multiCastV6.ipv6mr_multiaddr.__u6_addr32[i] = htonl(localAdtPtr->ipAdrSrc.addr[i]);
        }

        if (TRUE == isAdd)
        {
            ret = setsockopt(socketId, IPPROTO_IP, IPV6_JION_GROUP, (char*)&multiCastV6, sizeof(multiCastV6));
            /* IPV6_MULTICAST_IF shall be set,but used parameters is net index */
        }
        else
        {
            ret = setsockopt(socketId, IPPROTO_IP, IPV6_LEAVE_GROUP, (char*)&multiCastV6, sizeof(multiCastV6));
        }

#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
    }
    if (ret != 0)
    {
        /* shall be report error */
        TCPIP_FUNC_CHECK_OUTPUT("socket:%d TcpIp_BsdUdpJoinMultCast error:%d\n", socketId, errno);
    }
}

TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_BsdMulticastRedirect(
    P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) socketIdPtr,
    P2CONST(TcpIp_LocalAdrHandleType, AUTOMATIC, TCPIP_APPL_DATA) localAdtPtr,
    P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) PortPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    uint16 destPort = *PortPtr;
    if (destPort == 0u)
    {
        TCPIP_FUNC_CHECK_OUTPUT("multicast bind not support any port\n");
        return ret;
    }
    TcpIp_SocketIdType socketId = *socketIdPtr;
    TcpIp_SocketMangeDataType* sockMngPtr = TcpIp_BsdFindSocketMngDataBySocketId(socketId);
    /* find same port in normal socket */
    TcpIp_SocketMangeDataType* normalSocMngPtr = TcpIp_SocketMngHeaderPtr;
    while (normalSocMngPtr != NULL)
    {
        if ((normalSocMngPtr->port == *PortPtr) && (!TCPIP_SOCKET_IS_FLAG(normalSocMngPtr, TCPIP_SOCKET_IS_MULTICAST)))
        {
            break;
        }
        normalSocMngPtr = normalSocMngPtr->next;
    }
    if (NULL == normalSocMngPtr)
    {
        /* if no regular socket is found, the socket is created in the default way
         */
        normalSocMngPtr = sockMngPtr;
    }
    /* normal socket options will be modified */
    TcpIp_BsdUdpJoinMultCast(normalSocMngPtr->socket_fd, localAdtPtr, TRUE);
    if (normalSocMngPtr != sockMngPtr)
    {
        TCPIP_DEBUG(
            "TcpIp_Bind multicast socket: %d in normal socket:%d,port:%d\n",
            socketId,
            normalSocMngPtr->socket_fd,
            *PortPtr);

        /* link two struct */
        normalSocMngPtr->linkOther = sockMngPtr;
        sockMngPtr->linkOther = normalSocMngPtr;
        sockMngPtr->port = normalSocMngPtr->port;
    }

    /* set flag TCPIP_SOCKET_IS_MULTICAST: */
    TCPIP_SOCKET_SET_FLAG(sockMngPtr, TCPIP_SOCKET_IS_MULTICAST);
    *socketIdPtr = normalSocMngPtr->socket_fd;
    ret = E_OK;
    return ret;
}
#endif /* TCPIP_SUPPORT_MULTICAST */

TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_BsdReadHandle(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    P2CONST(TcpIp_SocketMangeDataType, AUTOMATIC, TCPIP_APPL_VAR) sockMngPtr)
{
    uint8 buf[TCPIP_MAX_BUF_LEN];
    struct sockaddr bsdReAddr;
    socklen_t bsdReAddrLen = sizeof(bsdReAddr);
    ssize_t cnt;
#if (TCPIP_TCP_ENABLED == STD_ON)

    if (sockMngPtr->protocol == TCPIP_IPPROTO_TCP)
    {
        cnt = read(SocketId, buf, sizeof(buf));
    }

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */
#if (TCPIP_UDP_ENABLED == STD_ON)

    if (sockMngPtr->protocol == TCPIP_IPPROTO_UDP)
    {
#if TCPIP_SUPPORT_MULTICAST || (TCPIP_CTRL_NUM > 0)

        if (TCPIP_SOCKET_IS_FLAG(sockMngPtr, TCPIP_SOCKET_IS_BINDANY))
        {
            TcpIp_SocketIdType para_socketId = SocketId;
            cnt = TcpIp_BsdUdpRxPreHandle(&para_socketId, sockMngPtr, buf, sizeof(buf), &bsdReAddr, bsdReAddrLen);
#if TCPIP_SUPPORT_MULTICAST
            if ((para_socketId != SocketId)
                && ((sockMngPtr->linkOther != NULL) || TCPIP_SOCKET_IS_FLAG(sockMngPtr, TCPIP_SOCKET_IS_MULTICAST)))
            {
                SocketId = para_socketId;
                sockMngPtr = TcpIp_BsdFindSocketMngDataBySocketId(para_socketId);
            }
#endif /* TCPIP_SUPPORT_MULTICAST */
        }
        else
#endif /* TCPIP_SUPPORT_MULTICAST||(TCPIP_CTRL_NUM > 0) */

        {
            cnt = recvfrom(SocketId, buf, sizeof(buf), MSG_DONTWAIT, &bsdReAddr, &bsdReAddrLen);
        }
    }

#endif /* (TCPIP_UDP_ENABLED == STD_ON) */

    if (cnt > 0)
    {
#if (TCPIP_TCP_ENABLED == STD_ON)

        if (sockMngPtr->protocol == TCPIP_IPPROTO_TCP)
        {
            int res = getpeername(SocketId, &bsdReAddr, &bsdReAddrLen);

            if (res < 0)
            {
                /* shall be report error */
                TCPIP_FUNC_CHECK_OUTPUT("socket:%d getpeername error:%d\n", SocketId, errno);
                return;
            }
        }

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */
        TcpIp_SockAddrType bswReAdr = {0};
        TcpIp_BsdAddrChangeBsdToBsw(&bsdReAddr, bsdReAddrLen, &bswReAdr);
        TCPIP_DEBUG("tcpip socket:%d rx data from:%x,len:%ld\n", SocketId, bswReAdr.addr[0], cnt);
        TCPIP_DEBUG_TIMER();
        if ((NULL != sockMngPtr) && (NULL != sockMngPtr->socketOwnerCfgPtr)
            && (NULL != sockMngPtr->socketOwnerCfgPtr->Up_RxIndicationFuncPtr))
        {
            sockMngPtr->socketOwnerCfgPtr->Up_RxIndicationFuncPtr(SocketId, &bswReAdr, buf, (uint16)cnt);
        }
    }
    else if (cnt == 0)
    {
#if (TCPIP_TCP_SOCKET_MAX > 0)
        /* remote close this socket */
        if (sockMngPtr->protocol == TCPIP_IPPROTO_TCP)
        {
            TcpIp_BsdNotifyEvent(sockMngPtr, TCPIP_TCP_FIN_RECEIVED);
        }
#endif /* (TCPIP_TCP_SOCKET_MAX > 0) */
    }
    else if (errno == ECONNRESET)
    {
#if (TCPIP_TCP_SOCKET_MAX > 0)

        /* remote close this socket */
        if (sockMngPtr->protocol == TCPIP_IPPROTO_TCP)
        {
            TcpIp_BsdNotifyEvent(sockMngPtr, TCPIP_TCP_RESET);
        }
        else
#endif /* (TCPIP_TCP_SOCKET_MAX > 0) */
#if (TCPIP_UDP_SOCKET_MAX > 0)
        {
            TcpIp_BsdNotifyEvent(sockMngPtr, TCPIP_UDP_CLOSED);
        }

#endif /* (TCPIP_UDP_SOCKET_MAX > 0) */
        (void)TcpIp_BsdCloseSocket(SocketId);
    }
#if (TCPIP_TCP_SOCKET_MAX > 0)
    else if (
        ((errno == ETIMEDOUT) || (errno == ECONNREFUSED)) && (sockMngPtr->protocol == TCPIP_IPPROTO_TCP)
        && (TCPIP_SOCKET_IS_FLAG(sockMngPtr, TCPIP_SOCKET_CONNECT_PENDING)))
    {
        /* tcp client connect timeout */
        TcpIp_BsdNotifyEvent(sockMngPtr, TCPIP_TCP_RESET);
        (void)TcpIp_BsdCloseSocket(SocketId);
    }
#endif /* (TCPIP_TCP_SOCKET_MAX > 0) */

    else
    {
        /* errno != EAGAIN */
        TCPIP_FUNC_CHECK_OUTPUT("socket:%d recv error:%d\n", SocketId, errno);
    }
}

#if (TCPIP_TCP_SOCKET_MAX > 0)

TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_BsdTcpListenAcceptHandle(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    P2CONST(TcpIp_SocketMangeDataType, AUTOMATIC, TCPIP_APPL_VAR) sockMngPtr)
{
    struct sockaddr sockAddr;
    socklen_t addrLen = sizeof(sockAddr);
    int newSockId = accept(SocketId, &sockAddr, &addrLen);

    if (newSockId > 0)
    {
        TCPIP_DEBUG("new socket create:%d by server socket :%d\n", newSockId, SocketId);

        if (Tcp_SocketUsedNum >= TCPIP_TCP_SOCKET_MAX)
        {
            TCPIP_FUNC_CHECK_OUTPUT("accpet socket:%d shall be reject :%d\n", newSockId, Tcp_SocketUsedNum);
            close(newSockId);
            return;
        }

        TcpIp_SockAddrType remoteAddr;
        Std_ReturnType ret = E_NOT_OK;
        TcpIp_BsdAddrChangeBsdToBsw(&sockAddr, addrLen, &remoteAddr);

        if ((NULL != sockMngPtr) && (NULL != sockMngPtr->socketOwnerCfgPtr)
            && (NULL != sockMngPtr->socketOwnerCfgPtr->Up_TcpAcceptedFuncPtr))
        {
            ret = sockMngPtr->socketOwnerCfgPtr->Up_TcpAcceptedFuncPtr(SocketId, newSockId, &remoteAddr);
        }

        if (E_OK == ret)
        {
            TcpIp_SocketMangeDataType* newSockMngPtr = TcpIp_BsdMallocSocketMngData(newSockId);

            if (NULL != newSockMngPtr)
            {
                newSockMngPtr->domain = remoteAddr.domain;
                newSockMngPtr->protocol = sockMngPtr->protocol;
                newSockMngPtr->socketOwnerCfgPtr = sockMngPtr->socketOwnerCfgPtr;
#if TCPIP_SOCKET_NONBLOCK
                /* no block all socket */
                int socketFlgs = fcntl(newSockId, F_GETFL, 0);
                socketFlgs = fcntl(newSockId, F_SETFL, socketFlgs | O_NONBLOCK);
#endif /* TCPIP_SOCKET_NONBLOCK */
                ret = E_OK;
#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL)
                struct epoll_event env;
                env.events = EPOLLIN | EPOLLERR;
                env.data.fd = newSockId;
                int res = epoll_ctl(TcpIp_epfd, EPOLL_CTL_ADD, newSockId, &env);
                TCPIP_DEBUG("socket epoll add %d\n", newSockId);

                if (res < 0)
                {
                    TCPIP_FUNC_CHECK_OUTPUT("accpet socket:%d add epoll error:%d\n", newSockId, res);
                    TcpIp_BsdNotifyEvent(newSockMngPtr, TCPIP_TCP_RESET);
                    (void)TcpIp_BsdCloseSocket(newSockId);
                    ret = E_NOT_OK;
                }

#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL) */
            }
            else
            {
                TCPIP_FUNC_CHECK_OUTPUT("socket:%d,TcpIp_BsdMallocSocketMngData return null\n", newSockId);
                close(newSockId);
            }
        }
        else
        {
            TCPIP_DEBUG("socket:%d,Up_TcpAcceptedFuncPtr reject reason:%d \n", newSockId, ret);
            close(newSockId);
        }

        if (E_OK == ret)
        {
            SchM_Enter_TcpIp_ExclusiveArea_Tx();
            Tcp_SocketUsedNum++;
            SchM_Exit_TcpIp_ExclusiveArea_Tx();
        }
    }
}
#endif /* (TCPIP_TCP_SOCKET_MAX > 0) */

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**
 * @ingroup TcpIp_BsdInit
 * BSD tcpip Init
 */
FUNC(void, TCPIP_CODE)
TcpIp_BsdInit(P2CONST(TcpIp_ConfigType, AUTOMATIC, TCPIP_APPL_CONST) ConfigPtr)
{
#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL)
    /* epoll create */
    TcpIp_epfd = epoll_create(TCPIP_EPOLL_MAXEVENTS);

    if (TcpIp_epfd < 0)
    {
        TCPIP_FUNC_CHECK_OUTPUT("epoll create error\n");
        exit(-1);
    }

    TcpIp_EpollEvntsPtr = malloc(TCPIP_EPOLL_MAXEVENTS * sizeof(struct epoll_event));

    if (NULL_PTR == TcpIp_EpollEvntsPtr)
    {
        TCPIP_FUNC_CHECK_OUTPUT("epoll malloc event memory error\n");
        exit(-2);
    }

#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL) */
#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_POLL)

    for (int i = 0; i < TCPIP_SOCKET_NUM; i++)
    {
        TcpIp_PollEvent[i].fd = -1;
        TcpIp_PollEvent[i].events = 0;
    }

#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_POLL) */
    TcpIp_PbCfgPtr = ConfigPtr;
#if (TCPIP_CTRL_NUM > 0)
    TcpIp_BsdInitNetIf();
#endif /* (TCPIP_CTRL_NUM > 0) */
}

/**
 * @ingroup TcpIp_BsdDeInit
 * BSD tcpip DeInit
 */
FUNC(void, TCPIP_CODE)
TcpIp_BsdDeInit(void)
{
#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL)

    if (TcpIp_epfd > 0)
    {
        close(TcpIp_epfd);
    }

    TcpIp_epfd = -1;

    if (NULL != TcpIp_EpollEvntsPtr)
    {
        free(TcpIp_EpollEvntsPtr);
        TcpIp_EpollEvntsPtr = NULL;
    }

#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL) */
#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_POLL)

    for (int i = 0; i < TCPIP_SOCKET_NUM; i++)
    {
        TcpIp_PollEvent[i].fd = -1;
    }

#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_POLL) */
#if (STD_ON == TCPIP_INITIALIZATION_AVAILABLE)
    TcpIp_InitState = TCPIP_STATE_UNINIT;
#endif /* (STD_ON == TCPIP_INITIALIZATION_AVAILABLE) */
#if (TCPIP_TCP_SOCKET_MAX > 0)
    TCPIP_DEBUG("residue tcp socket num:%d\n", Tcp_SocketUsedNum);
#endif /* (TCPIP_TCP_SOCKET_MAX > 0) */
#if (TCPIP_UDP_SOCKET_MAX > 0)
    TCPIP_DEBUG("residue udp socket num:%d\n", Udp_SocketUsedNum);
#endif /* (TCPIP_UDP_SOCKET_MAX > 0) */
}
/**
 * @ingroup TcpIp_BsdCloseSocketHandle
 * close socket, Init
 * @param SocketId Socket handle identifying the local socket resource.
 * @param Abort TRUE: connection will immediately be terminated by sending a RST-Segment and releasing all related
 *        resources. FALSE: connection will be terminated after performing a regular connection termination handshake
 *        and releasing all related resources
 * @return E_OK is close successful, another is failed
 */
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdCloseSocketHandle(VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId, VAR(boolean, AUTOMATIC) Abort)
{
    Std_ReturnType ret = E_NOT_OK;
    TCPIP_DEBUG("TcpIp_Close: %d\n", SocketId);

    /* step3:notify event */
    TcpIp_SocketMangeDataType* sockMngPtr = TcpIp_BsdFindSocketMngDataBySocketId(SocketId);
    if (NULL_PTR != sockMngPtr)
    {
#if (TCPIP_TCP_SOCKET_MAX > 0u)

        if (sockMngPtr->protocol == TCPIP_IPPROTO_TCP)
        {
            TcpIp_BsdNotifyEvent(sockMngPtr, TCPIP_TCP_CLOSED);
        }
        else
#endif /* (TCPIP_TCP_SOCKET_MAX > 0u) */
        {
#if (TCPIP_UDP_SOCKET_MAX > 0u)
            TcpIp_BsdNotifyEvent(sockMngPtr, TCPIP_UDP_CLOSED);
#endif /* (TCPIP_UDP_SOCKET_MAX > 0u) */
        }
    }

    if (Abort == TRUE)
    {
        struct linger linopt = {0};
        /* shall used rst close this tcp socket */
        linopt.l_onoff = 1;
        int res = setsockopt(SocketId, SOL_SOCKET, SO_LINGER, &linopt, sizeof(linopt));
        if (res < 0)
        {
            TCPIP_FUNC_CHECK_OUTPUT("close socket:%d set option is rst return error:%d\n", SocketId, res);
        }
    }

    ret = TcpIp_BsdCloseSocket(SocketId);

    return ret;
}

#if ((TCPIP_LOCAL_ADR_NUM > 0u) && (TCPIP_SOCKET_NUM > 0u))
/**
 * @ingroup TcpIp_BsdBind
 * get local address inner ip infomation,call by TcpIp_Bind
 * @param socketId socket index
 * @param LocalAddrId local address index
 * @param portPtr socket local port,if TCPIP_PORT_ANY ,shall get automatically assignment port
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdBind(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) localAddrId,
    P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) portPtr) /* PRQA S 3432 */
{
    Std_ReturnType ret = E_NOT_OK;

    TCPIP_DEBUG("TcpIp_Bind: %d,port:%d\n", socketId, *portPtr);
#if TCPIP_SUPPORT_MULTICAST
    TcpIp_SocketIdType multNorSocketId = socketId;
#endif /* TCPIP_SUPPORT_MULTICAST */

    const TcpIp_LocalAdrHandleType* localAdtPtr = &TcpIp_LocalAdrTable[localAddrId];
    const TcpIp_LocalAddrType* localAdtCfgPtr = &TcpIp_PbCfgPtr->LocalAdrPtr[localAddrId];

    if ((localAdtPtr->flag & LOCALADR_FLAG_IP_VAID) == 0u)
    {
        TCPIP_FUNC_CHECK_OUTPUT("LocalAddrId not have valid ip address\n");
        return ret;
    }

    int res = -1;
    struct sockaddr bsdSockAddr;
    socklen_t t_bindLen = sizeof(bsdSockAddr);
    TcpIp_SockAddrType bswSockAddr;
#if TCPIP_USED_FIXED_NETIF
    struct ifreq interface;
    strncpy(interface.ifr_name, TCPIP_USED_FIXED_NETIF_NAME, sizeof(TCPIP_USED_FIXED_NETIF_NAME));
    res = setsockopt(socketId, SOL_SOCKET, SO_BINDTODEVICE, (char*)&interface, sizeof(interface));
    if (res < 0)
    {
        TCPIP_FUNC_CHECK_OUTPUT("SO_BINDTODEVICE return error%d\n", errno);
    }
#endif /* TCPIP_USED_FIXED_NETIF */

    if (localAdtCfgPtr->AddrType == TCPIP_UNICAST)
    {
        bswSockAddr = localAdtPtr->ipAdrSrc;
    }
    else if (localAdtCfgPtr->AddrType == TCPIP_MULTICAST)
    {

#if TCPIP_SUPPORT_MULTICAST
        ret = TcpIp_BsdMulticastRedirect(&multNorSocketId, localAdtPtr, portPtr);
        if (ret != E_OK)
        {
            TCPIP_FUNC_CHECK_OUTPUT("redirect multicast socket error\n");
            return ret;
        }

        if (multNorSocketId != socketId)
        {
            return ret;
        }
        /* on linux paltform,bind multicast ip addr shall used any,other paltforms
         * to be verified */
        /* INADDR_ANY */
        memset(&bswSockAddr.addr, 0, sizeof(bswSockAddr.addr));

#endif /* TCPIP_SUPPORT_MULTICAST */
    }
    else
    {
        /* nothing */
    }

    bswSockAddr.port = *portPtr;
    TcpIp_BsdAddrChangeBswToBsd(&bswSockAddr, &bsdSockAddr, &t_bindLen);
    /* multicast sockets use the same port as unicast sockets,
    However, the IP address of the unicast socket cannot be reset */
    res = bind(socketId, &bsdSockAddr, t_bindLen);

    if ((res >= 0) && (bswSockAddr.port == 0u))
    {
        t_bindLen = sizeof(bsdSockAddr);
        res = getsockname(socketId, &bsdSockAddr, &t_bindLen);

        if (res >= 0)
        {
            TcpIp_BsdAddrChangeBsdToBsw(&bsdSockAddr, t_bindLen, &bswSockAddr);
            *portPtr = bswSockAddr.port;
        }
    }

    if (res < 0)
    {
        TCPIP_FUNC_CHECK_OUTPUT("call bind return error:%d\n", errno);
        return ret;
    }

    ret = E_OK;
    TcpIp_SocketMangeDataType* sockMngPtr = TcpIp_BsdFindSocketMngDataBySocketId(socketId);

#if TCPIP_SUPPORT_MULTICAST
    sockMngPtr->localAdtPtr = localAdtPtr;
#endif /* TCPIP_SUPPORT_MULTICAST */

#if (TCPIP_CTRL_NUM > 0) || TCPIP_SUPPORT_MULTICAST
    if ((localAdtPtr->flag & LOCALADR_FLAG_IP_ANY) && (!TCPIP_SOCKET_IS_FLAG(sockMngPtr, TCPIP_SOCKET_IS_BINDANY)))
    {
        TCPIP_SOCKET_SET_FLAG(sockMngPtr, TCPIP_SOCKET_IS_BINDANY);
    }

    if (TCPIP_SOCKET_IS_FLAG(sockMngPtr, TCPIP_SOCKET_IS_BINDANY))
    {
#if (TCPIP_MULTIP_DIRECT_MAP && (TCPIP_CTRL_NUM > 0))
        res = setsockopt(
            socketId,
            SOL_SOCKET,
            SO_BINDTODEVICE,
            TcpIp_IfReq[localAdtCfgPtr->CtrlRef].ifr_name,
            sizeof(struct ifreq));

        if (res < 0)
        {
            TCPIP_FUNC_CHECK_OUTPUT("socket:%d setsockopt SO_BINDTODEVICE error:%d\n", socketId, errno);
            ret = E_NOT_OK;
        }

#endif /* (TCPIP_MULTIP_DIRECT_MAP && (TCPIP_CTRL_NUM > 0)) */
        /* option IP_PKTINFO used find receiver udp socket dest ip addr and transmit boardcast specify the IP */
        int enable = 1;
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
        if (localAdtCfgPtr->DomainType == TCPIP_AF_INET)
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
        {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
            res = setsockopt(socketId, IPPROTO_IP, IP_PKTINFO, &enable, sizeof(enable));
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
        }
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
        else
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
        {
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3)
            res = setsockopt(socketId, IPPROTO_IPV6, IPV6_PKTINFO, &enable, sizeof(enable));
#endif /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) || (TCPIP_SCALABILITY_CLASS == TCPIP_SC3) */
        }
        if (res < 0)
        {
            TCPIP_FUNC_CHECK_OUTPUT("socket:%d setsockopt IP_PKTINFO error:%d\n", socketId, errno);
            ret = E_NOT_OK;
        }
    }
#if (TCPIP_CTRL_NUM > 0)
    sockMngPtr->netIfIndex = localAdtCfgPtr->CtrlRef;
#endif /* (TCPIP_CTRL_NUM > 0) */
#endif /* (TCPIP_CTRL_NUM > 0) || TCPIP_SUPPORT_MULTICAST */
#if (TCPIP_UDP_SOCKET_MAX > 0u)

#if TCPIP_SUPPORT_MULTICAST
    sockMngPtr->port = *portPtr;
#endif /* TCPIP_SUPPORT_MULTICAST */

    if ((NULL_PTR != sockMngPtr) && (sockMngPtr->protocol == TCPIP_IPPROTO_UDP))
    {
        int so_broadcast = 1;
        res = setsockopt(socketId, SOL_SOCKET, SO_BROADCAST, (void*)&so_broadcast, sizeof(so_broadcast));

        if (res < 0)
        {
            TCPIP_FUNC_CHECK_OUTPUT("bind udp socket:%d open broadcast error:%d\n", socketId, errno);
            ret = E_NOT_OK;
        }

#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL)

        struct epoll_event env;
        env.events = EPOLLIN | EPOLLERR;
        env.data.fd = socketId;
        res = epoll_ctl(TcpIp_epfd, EPOLL_CTL_ADD, socketId, &env);
        TCPIP_DEBUG("socket epoll add %d\n", socketId);

        if (res < 0)
        {
            TCPIP_FUNC_CHECK_OUTPUT("bind udp socket:%d add epoll error:%d\n", socketId, errno);
            /* notify event */
            TcpIp_BsdNotifyEvent(sockMngPtr, TCPIP_UDP_CLOSED);
            (void)TcpIp_BsdCloseSocket(socketId);
            ret = E_NOT_OK;
        }

#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL) */
    }

#endif /* (TCPIP_UDP_SOCKET_MAX > 0u) */
    return ret;
}
#endif /* ((TCPIP_LOCAL_ADR_NUM > 0u)&&(TCPIP_SOCKET_NUM > 0u)) */

/**
 * @ingroup TcpIp_BsdTcpConnect
 * tcp transmit data,dataPtr is not nullptr call by TcpIp_TcpConnect
 * @param socketId socket index
 * @param remoteAddrPtr IP address and port of the remote host to connect to
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdTcpConnect(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) remoteAddrPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if (TCPIP_TCP_SOCKET_MAX > 0u)
    TcpIp_SocketMangeDataType* sockMngPtr = TcpIp_BsdFindSocketMngDataBySocketId(socketId);

    if (sockMngPtr == NULL)
    {
        TCPIP_FUNC_CHECK_OUTPUT("connect not find valid socket:%d\n", socketId);
        return ret;
    }
    TCPIP_DEBUG("socket:%d,connect remote port:%d\n", socketId, remoteAddrPtr->port);
    /* find valid socket */
    struct sockaddr bsdSockAdr = {0};
    socklen_t socklen = 0;
    TcpIp_BsdAddrChangeBswToBsd(remoteAddrPtr, &bsdSockAdr, &socklen);
    int res = connect(socketId, &bsdSockAdr, socklen);
    int needAddEpoll = FALSE;

    if (res == 0)
    {
        TCPIP_DEBUG("socket:%d,connect succeed\n", socketId);
        /* OK -- socket is ready for IO */
        if ((NULL != sockMngPtr->socketOwnerCfgPtr) && (NULL != sockMngPtr->socketOwnerCfgPtr->Up_TcpConnectedFuncPtr))
        {
            sockMngPtr->socketOwnerCfgPtr->Up_TcpConnectedFuncPtr(socketId);
        }
        ret = E_OK;
        needAddEpoll = FALSE;
    }
    else if (errno != EINPROGRESS)
    {
        TCPIP_DEBUG("socket:%d,connect error:%d\n", socketId, errno);
        /* error, fail somehow, close socket */
        /* notify event */
        /* TcpIp_BsdNotifyEvent(sockMngPtr, TCPIP_TCP_RESET);
        (void)TcpIp_BsdCloseSocket(SocketId); */
    }
    else
    {
        /* connection attempt is in progress */
        needAddEpoll = TRUE;
        TCPIP_SOCKET_SET_FLAG(sockMngPtr, TCPIP_SOCKET_CONNECT_PENDING);
    }

    if (needAddEpoll)
    {
        ret = E_OK;
#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL)
        struct epoll_event env;
        env.events = EPOLLIN | EPOLLOUT | EPOLLERR;
        env.data.fd = socketId;
        int res = epoll_ctl(TcpIp_epfd, EPOLL_CTL_ADD, socketId, &env);
        TCPIP_DEBUG("socket epoll add %d\n", socketId);

        if (res < 0)
        {
            TCPIP_FUNC_CHECK_OUTPUT("connect socket:%d add epoll error:%d\n", socketId, res);
            /* notify event */
            TcpIp_BsdNotifyEvent(sockMngPtr, TCPIP_TCP_RESET);
            (void)TcpIp_BsdCloseSocket(socketId);
            ret = E_NOT_OK;
        }
#elif (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_POLL)
            int i = 0;
            for (; i < TCPIP_SOCKET_NUM; i++)
            {
                if (TcpIp_PollEvent[i].fd == socketId)
                {
                    break;
                }
            }
            if (i < TCPIP_SOCKET_NUM)
            {
                TcpIp_PollEvent[i].events = POLLIN | POLLOUT;
            }
            else
            {
                ret = E_NOT_OK;
            }
#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL) */
    }

#endif /* (TCPIP_TCP_SOCKET_MAX > 0) */
    return ret;
}

/**
 * @ingroup TcpIp_BsdTcpListen
 * tcp transmit data,dataPtr is not nullptr call by TcpIp_TcpListen
 * @param socketId socket index
 * @param maxChannels Maximum number of new parallel connections established on this listen connection
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdTcpListen(VAR(TcpIp_SocketIdType, AUTOMATIC) socketId, VAR(uint8, AUTOMATIC) maxChannels)
{
    Std_ReturnType ret = E_NOT_OK;
#if (TCPIP_TCP_SOCKET_MAX > 0)
    TcpIp_SocketMangeDataType* sockMngPtr = TcpIp_BsdFindSocketMngDataBySocketId(socketId);

    if (sockMngPtr == NULL)
    {
        TCPIP_FUNC_CHECK_OUTPUT("listen not find valid socket:%d\n", socketId);
        return ret;
    }

    TCPIP_SOCKET_SET_FLAG(sockMngPtr, TCPIP_SOCKET_IS_LISTEN);
    int res = listen(socketId, (int)maxChannels);
    TCPIP_DEBUG("listen socket :%d\n", socketId);

    if (res < 0)
    {
        TCPIP_SOCKET_CLR_FLAG(sockMngPtr, TCPIP_SOCKET_IS_LISTEN);
        TCPIP_FUNC_CHECK_OUTPUT("listen socket:%d return error:%d\n", socketId, res);
        return ret;
    }

#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL)
    struct epoll_event env;
    env.events = EPOLLIN | EPOLLOUT | EPOLLERR;
    env.data.fd = socketId;
    res = epoll_ctl(TcpIp_epfd, EPOLL_CTL_ADD, socketId, &env);
    TCPIP_DEBUG("socket epoll add %d\n", socketId);

    if (res < 0)
    {
        TCPIP_FUNC_CHECK_OUTPUT("listen socket:%d add epoll error:%d\n", socketId, res);
        return ret;
    }

#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL) */
    ret = E_OK;
#endif /* (TCPIP_TCP_SOCKET_MAX > 0) */
    return ret;
}

#if (TCPIP_CONTROLLER_NUM > 0)
/**
 * @ingroup TcpIp_BsdRequestComMode
 * controller request state handle,call by TcpIp_RequestComMode
 * @param ctrlIdx inner controller index
 * @param state @ref TcpIp_StateType
 * @return E_OK ok,another is failed
 */
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdRequestComMode(VAR(uint8, AUTOMATIC) ctrlIdx, VAR(TcpIp_StateType, AUTOMATIC) state)
{
    Std_ReturnType ret = E_NOT_OK;
    if (ctrlIdx >= TcpIp_PbCfgPtr->ctrlNum)
    {
        return ret;
    }

#if (TCPIP_LOCAL_ADR_NUM > 0)
    uint32 loop = TcpIp_PbCfgPtr->CtrlPtr[ctrlIdx].TcpIpLocalAddrStartId;
    uint32 loopEnd = TcpIp_PbCfgPtr->CtrlPtr[ctrlIdx].TcpIpLocalAddrStopId;

    switch (state)
    {
    case TCPIP_STATE_ONLINE:

        /* pool this controller link all local address */
        while (loop <= loopEnd)
        {
            ret = TcpIp_BsdLocalIpAddrAssignHandle(loop, TCPIP_AUTOMATIC, TCPIP_IPADDR_ASSIGNMENT_STATIC, NULL_PTR);

            if (E_OK != ret)
            {
                break;
            }

            loop++;
        }

        /* pool this controller link all local address */
        break;

    case TCPIP_STATE_OFFLINE:

        /* controller ip address unassignment handle */
        while (loop <= loopEnd)
        {
            ret = TcpIp_BsdLocalIpAddrUnAssignHandle(loop);

            if (E_OK != ret)
            {
                break;
            }

            loop++;
        }

        break;

    default:
        break;
    }

#endif /* (TCPIP_LOCAL_ADR_NUM > 0) */
    return ret;
}
#endif /* (TCPIP_CONTROLLER_NUM > 0) */

/**
 * @ingroup TcpIp_BsdRequestIpAddrAssignment
 * initiated the local IP address assignment for the IP address specified by LocalAddrId
 * @param localAddrId IP address index specifying the IP address
 * @param type Type of IP address assignment
 * @param localIpAddrPtr Pointer to structure containing the IP address
 * @param netmask  Network mask of IPv4 address or address prefix of IPv6 address
 * @param defaultRouterPtr Pointer to structure containing the IP address of the default router
 * @return E_OK ok,another is failed
 */
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdRequestIpAddrAssignment(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) localAddrId,
    VAR(TcpIp_IpAddrAssignmentType, AUTOMATIC) type,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) localIpAddrPtr,
    VAR(uint8, AUTOMATIC) Netmask,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) defaultRouterPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (TCPIP_LOCAL_ADR_NUM > 0)

    if (NULL_PTR != localIpAddrPtr)
    {
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
        uint32 netMask[1] = {0};
#else  /* (TCPIP_SC1 != TCPIP_SCALABILITY_CLASS) */
            uint32 netMask[4] = {0};
#endif /* (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) */
        TcpIp_BsdNetMaskExtToInner(Netmask, TcpIp_PbCfgPtr->LocalAdrPtr[localAddrId].DomainType, netMask);
        /* function input parameter as local address var */
        TcpIp_StaticIpAddressConfigType ipAdr = {0};

        if (NULL_PTR != defaultRouterPtr)
        {
            ipAdr.DefaultRouterPtr = defaultRouterPtr->addr;
        }

        ipAdr.NetmaskPtr = netMask;
        ipAdr.StaticIpAddressPtr = localIpAddrPtr->addr;
        ret = TcpIp_BsdLocalIpAddrAssignHandle(localAddrId, TCPIP_MANUAL, type, &ipAdr);
    }
    else
    {
        ret = TcpIp_BsdLocalIpAddrAssignHandle(localAddrId, TCPIP_MANUAL, type, NULL_PTR);
    }

#endif /* (TCPIP_LOCAL_ADR_NUM > 0) */
    return ret;
}

/**
 * @ingroup TcpIp_BsdReleaseIpAddrAssignment
 * release the local IP address assignment for the IP address specified by LocalAddrId
 * @param localAddrId IP address index specifying the IP address
 * @return E_OK ok,another is failed
 */
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdReleaseIpAddrAssignment(VAR(TcpIp_LocalAddrIdType, AUTOMATIC) localAddrId)
{
    Std_ReturnType ret = E_NOT_OK;
#if (TCPIP_LOCAL_ADR_NUM > 0)
    ret = TcpIp_BsdLocalIpAddrUnAssignHandle(localAddrId);
#endif /* (TCPIP_LOCAL_ADR_NUM > 0) */
    return ret;
}

/**
 * @ingroup TcpIp_BsdChangeParameter
 * change a parameter of a socket
 * @param socketId socket identifier
 * @param parameterId Identifier of the parameter
 * @param parameterValue new parameter value
 * @return E_OK ok,another is failed
 */
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdChangeParameter(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(TcpIp_ParamIdType, AUTOMATIC) parameterId,
    P2CONST(void, AUTOMATIC, TCPIP_APPL_CONST) parameterValue)
{
    Std_ReturnType ret = E_OK;

    uintx limtLen = sizeof(TcpIp_SocketParaHandleTebla) / sizeof(TcpIp_SocketParaHandleTebla[0]);

    /* this designed shall be reduce QAC STCYC */
    for (uintx i = 0u; i < limtLen; i++)
    {
        const TcpIp_SocketParaHandleType* socketHandlePtr = &TcpIp_SocketParaHandleTebla[i];

        /* Matches only happen once */
        if (parameterId == socketHandlePtr->paramId)
        {
            if (socketHandlePtr->praHanldFuncPtr != NULL_PTR)
            {
                ret = socketHandlePtr->praHanldFuncPtr(socketId, parameterId, parameterValue);
            }

            break;
        }
    }

    return ret;
}
/**
 * @ingroup TcpIp_BsdGetIpAddr
 * get local address inner ip infomation,call by TcpIp_GetIpAddr
 * @param localAdrIndx local address index
 * @param ipAddrPtr Pointer to a struct where the IP address
 * @param netmaskPtr Network mask
 * @param defaultRouterPtr default router(gateway)
 * @return E_OK
 */
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdGetIpAddr(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) localAdrIndx,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr,        /* PRQA S 3432 */
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) netmaskPtr,                    /* PRQA S 3432 */
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) defaultRouterPtr) /* PRQA S 3432 */
{
    Std_ReturnType ret = E_NOT_OK;

    uint16 back_port = ipAddrPtr->port;
    *ipAddrPtr = TcpIp_LocalAdrTable[localAdrIndx].ipAdrSrc;
    ipAddrPtr->port = back_port;
    *defaultRouterPtr = TcpIp_LocalAdrTable[localAdrIndx].defaultrouter;
    TcpIp_BsdNetMaskInnerToExt(
        TcpIp_LocalAdrTable[localAdrIndx].netmask.domain,
        TcpIp_LocalAdrTable[localAdrIndx].netmask.addr,
        netmaskPtr);
    ret = E_OK;

    return ret;
}

#if (STD_ON == TCPIP_UDP_ENABLED)

/**
 * @ingroup TcpIp_BsdUdpTransmit
 * upd transmit data,dataPtr is not nullptr call by TcpIp_UdpTransmit
 * @param socketId socket index
 * @param dataPtr  Pointer to a linear buffer of TotalLength bytes containing the data to be transmitted.
 * @param remoteAddrPtr ip address and port of the remote host to transmit to
 * @param TotalLength indicates the payload size of the UDP datagram
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdUdpTransmit(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) dataPtr,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) remoteAddrPtr,
    VAR(uint16, AUTOMATIC) totalLength)
{
    Std_ReturnType ret = E_NOT_OK;

#if (TCPIP_UDP_SOCKET_MAX > 0)
    struct sockaddr bsdSockAdr = {0};
    socklen_t socklen = 0;
    TcpIp_BsdAddrChangeBswToBsd(remoteAddrPtr, &bsdSockAdr, &socklen);
    size_t trLen = totalLength;
    int sendFlg = 0;
    TcpIp_SocketMangeDataType* sockMngPtr = TcpIp_BsdFindSocketMngDataBySocketId(socketId);
    TCPIP_DEBUG("socket:%d udp transmit len:%d\n", socketId, totalLength);
    TCPIP_DEBUG_TIMER();
    if ((NULL_PTR != dataPtr) || (totalLength == 0u))
    {
        /* direct transmit */
        ssize_t res;
#if (TCPIP_CTRL_NUM > 0) && !(TCPIP_MULTIP_DIRECT_MAP)

        if (!TCPIP_SOCKET_IS_FLAG(sockMngPtr, TCPIP_SOCKET_IS_BINDANY))
#endif /* (TCPIP_CTRL_NUM > 0) && !(TCPIP_MULTIP_DIRECT_MAP) */
        {
#if TCPIP_USED_RAW_SEND_UDP
            res = TcpIp_BsdUdpTransmitByRawSocket(sockMngPtr, dataPtr, trLen, &bsdSockAdr, socklen);
#else
            res = sendto(socketId, dataPtr, trLen, sendFlg, &bsdSockAdr, socklen);
#endif /* TCPIP_USED_RAW_SEND_UDP */
        }

#if (TCPIP_CTRL_NUM > 0) && !(TCPIP_MULTIP_DIRECT_MAP)
        else
        {
            res = TcpIp_BsdUdpTransmitBySpecNetIf(sockMngPtr, dataPtr, trLen, &bsdSockAdr, socklen);
        }

#endif /* (TCPIP_CTRL_NUM > 0) && !(TCPIP_MULTIP_DIRECT_MAP) */

        if (res == trLen)
        {
            ret = E_OK;
        }
        else
        {
            /* shall report error */
            TCPIP_FUNC_CHECK_OUTPUT("socket:%d udp transmit error:%d\n", socketId, errno);
        }
    }
    else
    {
        /* shall copy data from upper layer */

        if ((NULL != sockMngPtr) && (NULL_PTR != sockMngPtr->socketOwnerCfgPtr)
            && (NULL_PTR != sockMngPtr->socketOwnerCfgPtr->Up_CopyTxDataFuncPtr))
        {
            uint8* buf = (uint8*)malloc(totalLength);
            BufReq_ReturnType bufRet = sockMngPtr->socketOwnerCfgPtr->Up_CopyTxDataFuncPtr(socketId, buf, totalLength);

            if (BUFREQ_OK == bufRet)
            {
                ssize_t res;
#if (TCPIP_CTRL_NUM > 0) && !(TCPIP_MULTIP_DIRECT_MAP)
                if (!TCPIP_SOCKET_IS_FLAG(sockMngPtr, TCPIP_SOCKET_IS_BINDANY))
#endif /* (TCPIP_CTRL_NUM > 0) && !(TCPIP_MULTIP_DIRECT_MAP) */
                {
#if TCPIP_USED_RAW_SEND_UDP
                    res = TcpIp_BsdUdpTransmitByRawSocket(sockMngPtr, dataPtr, trLen, &bsdSockAdr, socklen);
#else
                    res = sendto(socketId, dataPtr, trLen, sendFlg, &bsdSockAdr, socklen);
#endif /* TCPIP_USED_RAW_SEND_UDP */
                }
#if (TCPIP_CTRL_NUM > 0) && !(TCPIP_MULTIP_DIRECT_MAP)
                else
                {
                    res = TcpIp_BsdUdpTransmitBySpecNetIf(sockMngPtr, dataPtr, trLen, &bsdSockAdr, socklen);
                }
#endif /* (TCPIP_CTRL_NUM > 0) && !(TCPIP_MULTIP_DIRECT_MAP) */

                if (res == trLen)
                {
                    ret = E_OK;
                }
                else
                {
                    /* shall report error */
                    TCPIP_FUNC_CHECK_OUTPUT("socket:%d udp transmit error:%ld\n", socketId, res);
                }
            }

            free(buf);
        }
    }
#endif /* (TCPIP_UDP_SOCKET_MAX > 0) */

    return ret;
}
#endif /* (STD_ON == TCPIP_UDP_ENABLED) */

#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * @ingroup TcpIp_BsdTcpTransmit
 * tcp transmit data,dataPtr is not nullptr call by TcpIp_TcpTransmit
 * @param socketId socket index
 * @param dataPtr  Pointer to a linear buffer of TotalLength bytes containing the data to be transmitted.
 * @param availableLength Available data for transmission in bytes
 * @param forceRetrieve Indicates how the TCP/IP stack retrieves data from upper layer if dataPtr is a NULL_PTR
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdTcpTransmit(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) dataPtr,
    VAR(uint32, AUTOMATIC) availableLength,
    VAR(boolean, AUTOMATIC) forceRetrieve)
{
    Std_ReturnType ret = E_NOT_OK;

    TCPIP_DEBUG("socket:%d tcp usd %s transmit len:%d\n", socketId, (dataPtr != NULL) ? "IF" : "TP", availableLength);
    TCPIP_DEBUG_TIMER();
    if (0u < availableLength)
    {
        TcpIp_SocketMangeDataType* sockMngPtr = TcpIp_BsdFindSocketMngDataBySocketId(socketId);
        uint8* bufPtr = (uint8*)dataPtr;
        uint32 totLen = availableLength;

        if (NULL_PTR == bufPtr)
        {
            if ((NULL != sockMngPtr) && (NULL_PTR != sockMngPtr->socketOwnerCfgPtr)
                && (NULL_PTR != sockMngPtr->socketOwnerCfgPtr->Up_CopyTxDataFuncPtr))
            {
                bufPtr = (uint8*)malloc(availableLength);

                while (totLen > 0)
                {
                    uint16 copyLen = (totLen > 0xffffu) ? 0xffffu : totLen;
                    BufReq_ReturnType bufRet =
                        sockMngPtr->socketOwnerCfgPtr->Up_CopyTxDataFuncPtr(socketId, bufPtr, copyLen);

                    if (BUFREQ_E_BUSY == bufRet)
                    {
                        continue;
                    }
                    else if (BUFREQ_OK == bufRet)
                    {
                        totLen -= copyLen;
                    }
                    else
                    {
                        break;
                    }
                }

                if (totLen == 0u)
                {
                    ret = E_OK;
                }
            }
        }

        uint32 retLen = TcpIp_BsdTcpWriteData(socketId, bufPtr, availableLength);

        if (bufPtr != dataPtr)
        {
            free(bufPtr);
        }

        if (retLen == 0u)
        {
            ret = E_OK;
            /* transmit ok */
            /* call upper layer txconf */
            totLen = 0;
            TcpIp_Up_TxConfirmationType txConfFuncPtr = NULL_PTR;

            if ((NULL != sockMngPtr) && (NULL_PTR != sockMngPtr->socketOwnerCfgPtr)
                && (NULL_PTR != sockMngPtr->socketOwnerCfgPtr->Up_TxConfirmationFuncPtr))
            {
                txConfFuncPtr = sockMngPtr->socketOwnerCfgPtr->Up_TxConfirmationFuncPtr;
                totLen = availableLength;
            }

            while (totLen > 0)
            {
                uint16 txConfLen = (totLen > 0xffffu) ? 0xffffu : (uint16)totLen;
                txConfFuncPtr(socketId, txConfLen);
                totLen -= txConfLen;
            }
        }
    }

    return ret;
}

#endif /* (STD_ON == TCPIP_TCP_ENABLED) */

/**
 * @ingroup TcpIp_BsdMainFunction
 * Schedules the Bsd TCP/IP stack
 */
FUNC(void, TCPIP_CODE)
TcpIp_BsdMainFunction(void)
{
#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL)
    int nfds = epoll_wait(TcpIp_epfd, TcpIp_EpollEvntsPtr, TCPIP_EPOLL_MAXEVENTS, TCPIP_EPOLL_WAITTIME);

    if (nfds < 0)
    {
        TCPIP_FUNC_CHECK_OUTPUT("epoll_wait return error%d\n", nfds);
        return;
    }

    for (int i = 0; i < nfds; i++)
    {
        int socketId = TcpIp_EpollEvntsPtr[i].data.fd;
        TcpIp_SocketMangeDataType* sockMngPtr = TcpIp_BsdFindSocketMngDataBySocketId(socketId);

        if (NULL != sockMngPtr)
        {
#if (TCPIP_TCP_SOCKET_MAX > 0)

            if (TCPIP_SOCKET_IS_FLAG(sockMngPtr, TCPIP_SOCKET_IS_LISTEN))
            {
                /* new socket accept shall be handle */
                TcpIp_BsdTcpListenAcceptHandle(socketId, sockMngPtr);
            }
            else
#endif /* (TCPIP_TCP_SOCKET_MAX > 0) */
            {
                if (TcpIp_EpollEvntsPtr[i].events & EPOLLIN)
                {
                    TcpIp_BsdReadHandle(socketId, sockMngPtr);
                }
            }

            if (TCPIP_SOCKET_IS_FLAG(sockMngPtr, TCPIP_SOCKET_CONNECT_PENDING))
            {
                struct epoll_event env;
                env.events = EPOLLIN | EPOLLERR;
                env.data.fd = socketId;
                int res = epoll_ctl(TcpIp_epfd, EPOLL_CTL_MOD, socketId, &env);

                if (res < 0)
                {
                    TCPIP_FUNC_CHECK_OUTPUT("socket:%d,epoll_ctl modify return error%d\n", socketId, errno);
                    return;
                }

                struct sockaddr bsdReAddr;

                socklen_t bsdReAddrLen = sizeof(bsdReAddr);

                res = getpeername(socketId, &bsdReAddr, &bsdReAddrLen);

                if (res == 0)
                {
                    TCPIP_SOCKET_CLR_FLAG(sockMngPtr, TCPIP_SOCKET_CONNECT_PENDING);

                    /* OK -- socket is ready for IO */
                    if ((NULL != sockMngPtr->socketOwnerCfgPtr)
                        && (NULL != sockMngPtr->socketOwnerCfgPtr->Up_TcpConnectedFuncPtr))
                    {
                        sockMngPtr->socketOwnerCfgPtr->Up_TcpConnectedFuncPtr(socketId);
                    }
                }
            }

            if (TcpIp_EpollEvntsPtr[i].events & EPOLLERR)
            {
#if (TCPIP_TCP_SOCKET_MAX > 0)

                /* remote close this socket */
                if (sockMngPtr->protocol == TCPIP_IPPROTO_TCP)
                {
                    TcpIp_BsdNotifyEvent(sockMngPtr, TCPIP_TCP_RESET);
                }
                else
#endif /* (TCPIP_TCP_SOCKET_MAX > 0) */
#if (TCPIP_UDP_SOCKET_MAX > 0)
                {
                    TcpIp_BsdNotifyEvent(sockMngPtr, TCPIP_UDP_CLOSED);
                }

#endif /* (TCPIP_UDP_SOCKET_MAX > 0) */
                (void)TcpIp_BsdCloseSocket(socketId);
            }
        }
        else
        {
        }
    }

#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_EPOLL) */
#if (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_POLL)
    nfds_t pollSize = TCPIP_SOCKET_NUM;
    int nready = poll(TcpIp_PollEvent, pollSize, TCPIP_POLL_WAITTIME);

    if (nready < 0)
    {
        TCPIP_FUNC_CHECK_OUTPUT("poll return error%d\n", errno);
        return;
    }

    for (int i = 0; (i < pollSize) && (nready > 0); i++)
    {
        struct pollfd* pollEvtPtr = &TcpIp_PollEvent[i];
        int socketId = pollEvtPtr->fd;

        if ((socketId < 0) || (pollEvtPtr->revents == 0))
        {
            continue;
        }

        TCPIP_DEBUG("poll socket:%d,events:%d\n", socketId, pollEvtPtr->revents);
        TcpIp_SocketMangeDataType* sockMngPtr = TcpIp_BsdFindSocketMngDataBySocketId(socketId);

        if (NULL != sockMngPtr)
        {
#if (TCPIP_TCP_SOCKET_MAX > 0)

            if ((pollEvtPtr->revents & POLLIN) && (TCPIP_SOCKET_IS_FLAG(sockMngPtr, TCPIP_SOCKET_IS_LISTEN)))
            {
                /* new socket accept shall be handle */
                TcpIp_BsdTcpListenAcceptHandle(socketId, sockMngPtr);
            }
            else if ((pollEvtPtr->revents & POLLOUT) && TCPIP_SOCKET_IS_FLAG(sockMngPtr, TCPIP_SOCKET_CONNECT_PENDING))
            {
                struct sockaddr bsdReAddr;
                socklen_t bsdReAddrLen = sizeof(bsdReAddr);
                int res = getpeername(socketId, &bsdReAddr, &bsdReAddrLen);

                if (res == 0)
                {
                    TCPIP_DEBUG("socket:%d,connect succeed\n", socketId);
                    TCPIP_SOCKET_CLR_FLAG(sockMngPtr, TCPIP_SOCKET_CONNECT_PENDING);
                    pollEvtPtr->events = POLLIN | POLLERR;

                    /* OK -- socket is ready for IO */
                    if ((NULL != sockMngPtr->socketOwnerCfgPtr)
                        && (NULL != sockMngPtr->socketOwnerCfgPtr->Up_TcpConnectedFuncPtr))
                    {
                        sockMngPtr->socketOwnerCfgPtr->Up_TcpConnectedFuncPtr(socketId);
                    }
                }
                else
                {
                    TCPIP_DEBUG("socket:%d,connect failed errno:%d\n", socketId, errno);
                    TcpIp_BsdNotifyEvent(sockMngPtr, TCPIP_TCP_RESET);
                    (void)TcpIp_BsdCloseSocket(socketId);
                }
            }
            else
#endif /* (TCPIP_TCP_SOCKET_MAX > 0) */
            {
                if (pollEvtPtr->revents & (POLLIN | POLLERR))
                {
                    TcpIp_BsdReadHandle(socketId, sockMngPtr);
                }
            }

            nready--;
        }
    }

#endif /* (TCPIP_IO_MULTPLEXING_METHOD == TCPIP_LINUX_POLL) */
}

/**
 * @ingroup TcpIp_BsdGetSocket
 * upd transmit data,dataPtr is not nullptr call by TcpIp_UdpTransmit
 * @param domain TCPIP_AF_INET ipv4,TCPIP_AF_INET6 ipv6
 * @param protocol TCPIP_IPPROTO_TCP, TCPIP_IPPROTO_UDP
 * @param SocketIdPtr indicates SocketId
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdGetSocket(
    VAR(TcpIp_DomainType, AUTOMATIC) domain,
    VAR(TcpIp_ProtocolType, AUTOMATIC) protocol,
#if (STD_ON == TCPIP_MUILT_SOCKET_OWNERCFG)
    VAR(uint8, AUTOMATIC) socketOwnererId,
#endif /* (STD_ON == TCPIP_MUILT_SOCKET_OWNERCFG) */
    P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) socketIdPtr) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;

#if (TCPIP_SOCKET_NUM > 0u)
    int t_domian = (domain == TCPIP_AF_INET) ? AF_INET : AF_INET6;
    /* check config allow diff domain */
#if (TCPIP_SCALABILITY_CLASS == TCPIP_SC1)
    if (AF_INET != domain)
#elif (TCPIP_SCALABILITY_CLASS == TCPIP_SC2) /* TCPIP_SCALABILITY_CLASS == TCPIP_SC2 */
        if (AF_INET6 != domain)
#else                                        /* TCPIP_SCALABILITY_CLASS == TCPIP_SC3 */
        if ((AF_INET6 != domain) && (AF_INET != domain))
#endif                                       /* (TCPIP_SCALABILITY_CLASS == TCPIP_SC1) */
    {
        TCPIP_FUNC_CHECK_OUTPUT("domain check error\n");
        return ret;
    }

    int t_type = (protocol == TCPIP_IPPROTO_TCP) ? SOCK_STREAM : SOCK_DGRAM;
    SchM_Enter_TcpIp_ExclusiveArea_Tx();

    /* check config allow diff protocol */
    if (TCPIP_IPPROTO_UDP == protocol)
    {
#if (TCPIP_UDP_SOCKET_MAX > 0)
        if (Udp_SocketUsedNum >= TCPIP_UDP_SOCKET_MAX)
#endif /* (TCPIP_UDP_SOCKET_MAX > 0) */
        {
            TCPIP_FUNC_CHECK_OUTPUT("not create udp socket\n");
            SchM_Exit_TcpIp_ExclusiveArea_Tx();
            return ret;
        }

#if (TCPIP_UDP_SOCKET_MAX > 0)
        Udp_SocketUsedNum++;
#endif /* (TCPIP_UDP_SOCKET_MAX > 0) */
    }

    if (TCPIP_IPPROTO_TCP == protocol)
    {
#if (TCPIP_TCP_SOCKET_MAX > 0)
        if (Tcp_SocketUsedNum >= TCPIP_TCP_SOCKET_MAX)
#endif /* (TCPIP_TCP_SOCKET_MAX > 0 ) */
        {
            TCPIP_FUNC_CHECK_OUTPUT("not create tcp socket\n");
            SchM_Exit_TcpIp_ExclusiveArea_Tx();
            return ret;
        }

#if (TCPIP_TCP_SOCKET_MAX > 0)
        Tcp_SocketUsedNum++;
#endif /* (TCPIP_TCP_SOCKET_MAX > 0 ) */
    }

    SchM_Exit_TcpIp_ExclusiveArea_Tx();
    int t_protocol = 0;
    TcpIp_SocketIdType socket_fd = socket(t_domian, t_type, t_protocol);

    if (socket_fd < 0)
    {
        TCPIP_FUNC_CHECK_OUTPUT("get socket error\n");
        return ret;
    }

    TcpIp_SocketMangeDataType* socketMngPtr = TcpIp_BsdMallocSocketMngData(socket_fd);

    if (NULL == socketMngPtr)
    {
        close(socket_fd);
        return ret;
    }
    *socketIdPtr = socket_fd;
    /* init value */
    socketMngPtr->domain = domain;
    socketMngPtr->protocol = protocol;
#if (STD_ON == TCPIP_MUILT_SOCKET_OWNERCFG)
    socketMngPtr->socketOwnerCfgPtr = &TcpIp_LCfg.SocketOwnerLCfgPtr[socketOwnererId];
#else  /* (STD_ON != TCPIP_MUILT_SOCKET_OWNERCFG) */
        socketMngPtr->socketOwnerCfgPtr = &TcpIp_LCfg.SocketOwnerLCfgPtr[0];
#endif /* (STD_ON == TCPIP_MUILT_SOCKET_OWNERCFG) */
#if TCPIP_SOCKET_NONBLOCK
    /* no block all socket */
    int socketFlgs = fcntl(socket_fd, F_GETFL, 0);
    socketFlgs = fcntl(socket_fd, F_SETFL, socketFlgs | O_NONBLOCK);
#endif /* TCPIP_SOCKET_NONBLOCK */
    /* open socket option SO_REUSEADDR */
    int reuseAdrFlg = 1;
    int res = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuseAdrFlg, sizeof(reuseAdrFlg));
    if (res != 0)
    {
        TCPIP_FUNC_CHECK_OUTPUT("malloc socket:%d setsockopt err:%d\n", socket_fd, errno);
        return ret;
    }
#endif /* (TCPIP_SOCKET_NUM > 0u) */
    ret = E_OK;

    return ret;
}

#endif /* #if (STD_ON == TCPIP_BSDSOCKET_SUPPORT) */
