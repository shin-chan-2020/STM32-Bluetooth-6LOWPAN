
//lwipopts.h

//Note that this port was for a machine that — unlike many embedded systems on which lwIP runs — was not memory constrained. Hence, many of the settings here are much higher than will be the case for other lwIP ports. Unfortunately, these settings are highly specific to the particular machine and configuration. Your best bet is to read the comments in the main lwIP opts.h file, take a stab at settings appropriate to your system, and ask on the lwip-users mailing list if you need help in understanding what they all mean.
#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__


/**
 * NO_SYS==1: Provides VERY minimal functionality. Otherwise,
 * use lwIP facilities.
 */

#define NO_SYS                      1 /* NO_SYS==1: Use lwIP without OS-awareness */
#define MEM_LIBC_MALLOC             1 /* 0: use lwip internal allocator*/ //
#define MEMP_MEM_MALLOC             1 /* MEMP_MEM_MALLOC==0: Use lwip pool allocator*/ //
#define MEM_ALIGNMENT               4
#define MEM_SIZE                    (5 * 1024 * 1024) /* MEM_SIZE: the size of the heap memory */ //
#define MEMP_NUM_PBUF               100 /* If the application sends a lot of data out of ROM (or other static memory), this should be set high.*/
#define MEMP_NUM_RAW_PCB            0 //  added
//#define MEMP_NUM_UDP_PCB          4 /*One per active UDP "connection"*/
#define MEMP_NUM_UDP_PCB            2 //  added
//#define MEMP_NUM_TCP_PCB          4 /* the number of simultaneously active TCP connections */
#define MEMP_NUM_TCP_PCB            1 //  added
//#define MEMP_NUM_TCP_PCB_LISTEN   4 /* the number of listening TCP connections */
#define MEMP_NUM_TCP_PCB_LISTEN     1 //  added
//#define MEMP_NUM_TCP_SEG         12 /*the number of simultaneously queued TCP segments*/
//#define MEMP_NUM_TCP_SEG         32 /*the number of simultaneously queued TCP segments*/ //  : big bug
#define MEMP_NUM_TCP_SEG            0 //  added
#define MEMP_NUM_FRAG_PBUF          0 //  added
#define MEMP_NUM_TCPIP_MSG_API      0 //  added
#define MEMP_NUM_TCPIP_MSG_INPKT    0 //  added
#define MEMP_NUM_SNMP_NODE          0 //  added
#define MEMP_NUM_SNMP_ROOTNODE      0 //  added
#define MEMP_NUM_SNMP_VARBIND       0 //  added
#define MEMP_NUM_SNMP_VALUE         0 //  added

#define MEMP_NUM_REASSDATA          5  /* the number of IP packets simultaneously queued for reassembly */
#define MEMP_NUM_ARP_QUEUE          10 /* the number of simultaneously queued outgoing packets that are waiting for an ARP request (to resolve their destination address) to finish. */
//#define PBUF_POOL_SIZE            10 /* the number of buffers in the pbuf pool */
#define PBUF_POOL_SIZE              32 // added

#define LWIP_ARP                    0  //
#define ARP_TABLE_SIZE              0  // added
#define IP_REASSEMBLY               0  // added
#define IP_FRAG                     0  // added

//#define IP_REASS_MAX_PBUFS       10
#define IP_REASS_MAX_PBUFS          0  // added
#define IP_FRAG_USES_STATIC_BUF     0

#define IP_DEFAULT_TTL              255
#define IP_SOF_BROADCAST            0
#define IP_SOF_BROADCAST_RECV       0
#define LWIP_ICMP                   1
#define LWIP_BROADCAST_PING         0  /* LWIP_BROADCAST_PING==0: respond to unicast pings only (default). Not broadcast pings*/
#define LWIP_MULTICAST_PING         0  /* LWIP_MULTICAST_PING==0: respond to unicast pings only (default). Not multicast pings*/
//#define LWIP_RAW                  1  /* LWIP_RAW==0: Do NOT Enable application layer to hook into the IP layer itself */
#define LWIP_RAW                    1  /* added it should be 0, for ping app it should be 1 */
#define TCP_WND                     (2 * TCP_MSS) /* TCP receive window. */
//#define TCP_MSS                   1460  /* TCP Maximum segment size. *//* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */
#define TCP_MSS                     536   //
#define TCP_SND_BUF                 (4 * TCP_MSS) /* TCP sender buffer space (bytes). */
//#define TCP_LISTEN_BACKLOG        0  /* Enable the backlog option for tcp listen pcb. */
#define TCP_LISTEN_BACKLOG          1  // added

//#define LWIP_NETIF_STATUS_CALLBACK  0  /* Support a callback function whenever an interface changes its up/down status */
#define LWIP_NETIF_STATUS_CALLBACK    1 // added


#define LWIP_NETIF_LINK_CALLBACK    1  /* Support a callback function from an interface whenever the link changes (i.e., link down)*/
//#define LWIP_NETIF_HWADDRHINT     0  /* Do not Cache link-layer-address hints (e.g. table  * indices) in struct netif*/
#define LWIP_NETIF_HWADDRHINT       1  // added

#define LWIP_NETCONN                0  /* LWIP_NETCONN==0: Do NOT Enable Netconn API*/
#define LWIP_SOCKET                 0  /* LWIP_SOCKET==0: Do NOT Enable Socket API (require to use sockets.c)*/
#define LWIP_STATS_DISPLAY          0  /* LWIP_STATS_DISPLAY==0: Do NOT Compile in the statistics output functions.*/
#define MEM_STATS                   0  /* MEM_STATS==0: Do NOT Enable mem.c stats. */
#define SYS_STATS                   0  /* SYS_STATS==0: Do NOT Enable system stats (sem and mbox counts, etc).*/
#define MEMP_STATS                  0  /* MEMP_STATS==0: Do NOT Enable memp.c pool stats. */
#define LINK_STATS                  0  /* LINK_STATS==1: Do NOT Enable link stats. */
#define ETHARP_TRUST_IP_MAC         0
#define ETH_PAD_SIZE                2  /* ETH_PAD_SIZE: number of bytes added before the ethernet header to ensure alignment of payload after that header */
#define LWIP_CHKSUM_ALGORITHM       2

#define LWIP_CHECKSUM_ON_COPY       1  // added
#define LWIP_ND6_MAX_MULTICAST_SOLICIT 5 //1  // added
#define LWIP_ND6_MAX_NEIGHBOR_ADVERTISEMENT 1 // added
#define LWIP_ND6_RETRANS_TIMER       5000     // added

#define LWIP_TCP_KEEPALIVE          1  /* Keep the TCP link active. Important for MQTT/TLS */

// Keepalive values, compliant with RFC 1122. Don't change this unless you know what you're doing
#define TCP_KEEPIDLE_DEFAULT        10000UL // Default KEEPALIVE timer in milliseconds
#define TCP_KEEPINTVL_DEFAULT       2000UL  // Default Time between KEEPALIVE probes in milliseconds
#define TCP_KEEPCNT_DEFAULT         9U      // Default Counter for KEEPALIVE probes

/* this is for bare metal (NO_SYS = 1) and MEM_LIBC_MALLOC = 0 system where
   you use mahine specific memory functions.

   in our case though (NO_SYS = 1) we use MEM_LIBC_MALLOC = 1,
   which means we plan to use c lib mem function.

  so we commenting out below lines.

  if you want to use it in rtos environment and want to use rtos memory
  alloc function instead of c lib, then use map these functions to your os calls.

*/

/*
#include "mch.h"

#define mem_init()
#define mem_free                    mch_free
#define mem_malloc                  mch_malloc
#define mem_calloc(c, n)            mch_zalloc((c) * (n))
#define mem_realloc(p, sz)          (p)
*/
#define LWIP_DEBUG                  1

#define ETHARP_DEBUG                LWIP_DBG_OFF
#define NETIF_DEBUG                 LWIP_DBG_OFF
#define PBUF_DEBUG                  LWIP_DBG_OFF
#define API_LIB_DEBUG               LWIP_DBG_OFF
#define API_MSG_DEBUG               LWIP_DBG_OFF
#define SOCKETS_DEBUG               LWIP_DBG_OFF
#define ICMP_DEBUG                  LWIP_DBG_OFF
#define INET_DEBUG                  LWIP_DBG_OFF
#define IP_DEBUG                    LWIP_DBG_OFF
#define IP_REASS_DEBUG              LWIP_DBG_OFF
#define RAW_DEBUG                   LWIP_DBG_OFF
#define MEM_DEBUG                   LWIP_DBG_OFF
#define MEMP_DEBUG                  LWIP_DBG_OFF
#define SYS_DEBUG                   LWIP_DBG_OFF
#define TCP_DEBUG                   LWIP_DBG_OFF
#define TCP_INPUT_DEBUG             LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG            LWIP_DBG_OFF
#define TCP_RTO_DEBUG               LWIP_DBG_OFF
#define TCP_CWND_DEBUG              LWIP_DBG_OFF
#define TCP_WND_DEBUG               LWIP_DBG_OFF
#define TCP_FR_DEBUG                LWIP_DBG_OFF
#define TCP_QLEN_DEBUG              LWIP_DBG_OFF
#define TCP_RST_DEBUG               LWIP_DBG_OFF
#define UDP_DEBUG                   LWIP_DBG_OFF
#define TCPIP_DEBUG                 LWIP_DBG_OFF
#define PPP_DEBUG                   LWIP_DBG_OFF
#define SLIP_DEBUG                  LWIP_DBG_OFF
#define DHCP_DEBUG                  LWIP_DBG_OFF

#define LWIP_TCPIP_CORE_LOCKING     0 //  added

/* ---------- IPv4 options ---------- */
#define LWIP_TCP                    1 //  added
#define LWIP_ICMP6                  1 //  added
#define LWIP_IPV6_REASS             1 //  not same as in public sample
#define LWIP_ND6_TCP_REACHABILITY_HINTS 0 // added
#define LWIP_IPV6_MLD               0 //  added /* Multicast Listener Discovery  */
#define LWIP_STATS                  0 //  added
#define PPP_IPV6_SUPPORT            0 //  added
#define LWIP_PPP_API                0 //  added
#define PPP_SUPPORT                 0 //  added
#define LWIP_DNS                    0 //  added
#define LWIP_SUPPORT_CUSTOM_PBUF    1 //  added
#define LWIP_ND6_QUEUEING           0 //  added
#define TCP_TMR_INTERVAL           50 //  added
#define SO_REUSE                    1 //  added


#define LWIP_IPV4                   0 //  added
#define LWIP_IPV6                   1 //  added
#define LWIP_IPV6_AUTOCONFIG        1 //  added //http://lwip.100.n7.nabble.com/STABLE-1-4-1-IPv6-Initialization-Examples-or-Guidance-td20750.html
#define LWIP_ND6_NUM_NEIGHBORS      2 //  10 :  reduced to help debugging
#define LWIP_ND6_NUM_DESTINATIONS   2 //  10 :  reduced to help debugging
#define LWIP_ND6_NUM_ROUTERS        1 //   3 :  reduced to help debugging
#define LWIP_ND6_NUM_PREFIXES       3 //   5 :  reduced to help debugging. it can be problematic, need to understand it better
#define LWIP_SINGLE_NETIF           1
#define LWIP_IPV6_DUP_DETECT_ATTEMPTS   0

#define LWIP_LOWPAN6_DEBUG                    LWIP_DBG_OFF
#define LWIP_LOWPAN6_IP_COMPRESSED_DEBUG      LWIP_DBG_OFF
#define LWIP_LOWPAN6_DECOMPRESSION_DEBUG      LWIP_DBG_OFF
#define LWIP_RFC7668_IP_UNCOMPRESSED_DEBUG    LWIP_DBG_OFF
#define IP6_DEBUG                             LWIP_DBG_OFF
#define ND6_DEBUG                             LWIP_DBG_OFF /* added */
#define ICMP6_DEBUG                           LWIP_DBG_OFF /* added */
#define CHECKSUM_CHECK_ICMP6                  0
#define LWIP_RFC7668_LINUX_WORKAROUND_PUBLIC_ADDRESS 0

#define LOWPAN6_ISR_DEBUG                     LWIP_DBG_OFF /* created this */
#define LOWPAN6_SPI_DEBUG                     LWIP_DBG_OFF /* created this */
#define LOWPAN6_PHY_DEBUG                     LWIP_DBG_OFF /* created this : Network physical layer*/


#endif /* __LWIPOPTS_H__ */
