

//Main function, etc. (mch_main.c)

#include "mch.h"
#include "lwip/inet.h"
#include "lwip/tcp.h"
//#include "lwip/ip_frag.h" //Anup : no file found
#include "lwip/netif.h"
#include "lwip/init.h"
#include "lwip/stats.h"
//#include "netif/etharp.h"
#include "lwip/ip6_addr.h"

#include "lwip/priv/tcp_priv.h" // Anup for TCP_TMR_INTERVAL
#include "link_layer.h"  // Anup for tBDAddr
#include "netif/lowpan6_ble.h" // Anup
#include <assert.h>
#include "netif/lowpan6_common.h"

typedef uint32_t mch_timestamp; //Anup
typedef struct ip6_addr ip_addr; //Anup
ip_addr mch_myip_addr;

#define ARP_TMR_INTERVAL  1 // Anup
#define IP_TMR_INTERVAL 1000  // Anup

#define MCH_ARP_TIMER_INTERVAL      (ARP_TMR_INTERVAL * 1000)
#define MCH_TCP_TIMER_INTERVAL      (TCP_TMR_INTERVAL * 1000)
#define MCH_IPREASS_TIMER_INTERVAL  (IP_TMR_INTERVAL * 1000)

#define MCH_IPADDR_BASE "192.168.0.2"
#define MCH_IPADDR_GW   "192.168.0.1"
#define MCH_IPADDR_NETMASK "255.255.255.0"

static mch_timestamp ts_etharp;
static mch_timestamp ts_tcp;
static mch_timestamp ts_ipreass;

// Our network interface structure
struct netif mchdrv_netif;


uint8_t iid_addr[8];

extern tBDAddr bdaddr;
extern tBDAddr brouter_bdaddr;

// Functions from my netif driver
// Probe function (find the device, return driver private data)
extern int mchdrv_probe(struct mch_pci_dev *, void **, uint8_t *);
// Init function
extern int mchdrv_attach(struct netif *);
// Poll for received frames
extern void mchdrv_poll(struct netif *);

err_t rfc7668_if_init(struct netif *netif);
void mch_timestamp_get(mch_timestamp * now);
mch_timestamp mch_timestamp_diff(mch_timestamp * pre, mch_timestamp * now);
//int rfc7668_input_push(void * datapointer, int datasize);
int rfc7668_input_push(struct pbuf *p);
void rfc7668_create_ip6_linklocal_address(struct netif *netif, u8_t from_mac_48bit);
#define fail_unless assert

extern struct lowpan6_link_addr rfc7668_local_addr;


int mch_net_init(void)
{
  //  static uint8_t bdaddr_be[6] = {0x00, 0x80, 0xE1, 0x00,0x00, 0xAA}; /* Local Address */
  static uint8_t bdaddr_be[6] = {0xb8, 0x27, 0xeb, 0x4f,0x42, 0xbe}; /* Local Address : big endian format*/
  void * mchdrvnet_priv;
  int err = -1;

  lwip_init();

  if (netif_add_noaddr(&mchdrv_netif, NULL, rfc7668_if_init, rfc7668_input) == NULL) {
    mch_printf("mch_net_init: netif_add (rfc7668_if_init) failed\n");
    return -1;
  }
  //  if (netif_add(&mchdrv_netif, NULL, NULL, NULL,NULL,rfc7668_if_init, rfc7668_input) == NULL) {
  //                  mch_printf("mch_net_init: netif_add (rfc7668_if_init) failed\n");
  //                  return -1;
  //                }
  /* convert BT address to EUI64 addr */
  printf("%-20s","ble addr: ");
  for(int i = 0; i < 6; i++)
  {
    printf("%X:", bdaddr_be[i]);
  }
  printf("\b \n");

  ble_addr_to_eui64(iid_addr, bdaddr_be, 0); // Not Public addr
  //  ble_addr_to_eui64(iid_addr, bdaddr_be, 1); // Public addr

  printf("%-20s","ble eui64 addr: ");
  for(int i = 0; i < 8; i++)
  {
    printf("%X:", iid_addr[i]);
  }
  printf("\b \n");
  // This call is important as it sets rfc7668_local_addr which is used inside rfc7668_input() and rfc7668_compress
  rfc7668_set_local_addr_eui64(&mchdrv_netif, iid_addr, sizeof(iid_addr)); // sets rfc7668_local_addr

  memcpy(mchdrv_netif.hwaddr, bdaddr_be,sizeof(tBDAddr));
  mchdrv_netif.hwaddr_len = sizeof(tBDAddr);

  /* create link-local addr based interface ID */
  netif_create_ip6_linklocal_address(&mchdrv_netif, 1); /* 0: not MAC 48 address*/

  if (mchdrv_netif.ip6_addr[0].addr[0] == 0)
  {
    printf(" Failed to get IP address! Please check cable and/or network configuration.\n");
    Error_Handler();
  }
  else
  {
//    static char str[40];
//    ip6addr_ntoa_r(&mchdrv_netif.ip6_addr[0],str,40 );
//    printf("%-20s%s\n","Ip6 : ",str);
//
//    printf("%-20s","ble eui64 addr: ");
//    for(int i = 0; i < rfc7668_local_addr.addr_len ; i++)
//    {
//      printf("%X:", rfc7668_local_addr.addr[i]);
//    }
//    printf("\b \n");

  }

  /*
Using IP6_ADDR_VALID instead of IP6_ADDR_TENTATIVE didn't work and I
don't know why from looking at the source code, do you have a clue ?

The initial state you should set for your address depends on whether you have DAD enabled or not.

If DAD is enabled (usual case), you should set it to IP6_ADDR_TENTATIVE.

If DAD is disabled, you should set it to IP6_ADDR_PREFERRED.

I think that in a PPP link DAD is actually not required, and probably wouldn't work because it requires multicast support. So I would set the state to IP6_ADDR_PREFERRED and it should work.

I'm not sure why IP6_ADDR_VALID wouldn't work, although generally you want it to be either PREFERRED or DEPRECATED, which is VALID with an extra flag.

What are you seeing that makes you say "it didn't work"?
 */

  //netif_ip6_addr_set_state((&mchdrv_netif), 0, IP6_ADDR_TENTATIVE); //http://lwip.100.n7.nabble.com/STABLE-1-4-1-IPv6-Initialization-Examples-or-Guidance-td20750.html
  fail_unless(IP_IS_V6(&mchdrv_netif.ip6_addr[0]));


  //mchdrv_netif.ip6_addr_state[0] = IP6_ADDR_VALID;

  netif_set_default(&mchdrv_netif);
  //  netif_set_up(&mchdrv_netif);

  // Initialize timer values
  mch_timestamp_get(&ts_etharp);
  mch_timestamp_get(&ts_tcp);
  mch_timestamp_get(&ts_ipreass);


  return 0;
}

//
//
// Regular polling mechanism.  This should be called each time through
// the main application loop (after each interrupt, regardless of source).
//
// It handles any received packets, permits NIC device driver house-keeping
// and invokes timer-based TCP/IP functions (TCP retransmissions, delayed
// acks, IP reassembly timeouts, ARP timeouts, etc.)
//
void mch_net_poll(void)
{
  mch_timestamp now;

  // Call network interface to process incoming packets and do housekeeping
  //mchdrv_poll(&mchdrv_netif);

  // Process lwip network-related timers.
  mch_timestamp_get(&now);
  if (mch_timestamp_diff(&ts_etharp, &now) >= MCH_ARP_TIMER_INTERVAL) {
    //etharp_tmr();
    ts_etharp = now;
  }
  if (mch_timestamp_diff(&ts_tcp, &now) >= MCH_TCP_TIMER_INTERVAL) {
    tcp_tmr();
    ts_tcp = now;
  }
  if (mch_timestamp_diff(&ts_ipreass, &now) >= MCH_IPREASS_TIMER_INTERVAL) {
    ip6_reass_tmr();
    ts_ipreass = now;
  }
}

//
// Convert address from string to internal format.
// Return 0 on success; else non-zero
//
//int mch_net_aton(char * str_addr, struct ip_addr * net_addr)
//{
//    struct in_addr a;
//    int i = inet_aton(str_addr, &net_addr->addr);
//    if (!i)
//        return -1;
//    return 0;
//}

//
// Main entry point
//
int lwip_main(void)
{
  //    [snip other non-lwip initializations]
  //mch_timestamp_init();       // Initialize timestamp generator
  while (mch_net_init() != 0) {HAL_Delay(500);};
  //  while (1) {
  //        [snip other non-lwip functions]
  //    mch_wait_for_interrupt();   // Awakened by network, timer or other interrupt
  //    mch_net_poll();             // Poll network stack
  //  }
}

void mch_timestamp_get(mch_timestamp * now)
{
  *now = sys_now();
}

mch_timestamp mch_timestamp_diff(mch_timestamp * pre, mch_timestamp * now)
{
  return (*now - *pre);
}

/*
This optional function does a "fast" critical region protection and returns
the previous protection level. This function is only called during very short
critical regions. An embedded system which supports ISR-based drivers might
want to implement this function by disabling interrupts. Task-based systems
might want to implement this by using a mutex or disabling tasking. This
function should support recursive calls from the same task or interrupt. In
other words, sys_arch_protect() could be called while already protected. In
that case the return value indicates that it is already protected.

sys_arch_protect() is only required if your port is supporting an operating
system.

Note: This function is based on FreeRTOS API, because no equivalent CMSIS-RTOS
API is available
*/
sys_prot_t sys_arch_protect(void)
{
  return (sys_prot_t)1;
}


/*
This optional function does a "fast" set of critical region protection to the
value specified by pval. See the documentation for sys_arch_protect() for
more information. This function is only required if your port is supporting
an operating system.

Note: This function is based on FreeRTOS API, because no equivalent CMSIS-RTOS
API is available
*/
void sys_arch_unprotect(sys_prot_t pval)
{
  ( void ) pval;
}

//int rfc7668_input_push(void * datapointer, int datasize)
//{
//  struct pbuf *p = NULL;
//
//  /* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
//  p = pbuf_alloc(PBUF_RAW, datasize, PBUF_POOL);
//
//  if (p != NULL)
//  {
//
//    //copy the data from datapointer to p
//    // pending
//    memcpy(p->payload, datapointer, datasize);
//    //pass the data uo the ip layer
//
//    rfc7668_input(p, &mchdrv_netif);
//
//    //pbuf_free(p);
//  }
//}

int rfc7668_input_push(struct pbuf *p)
{

  if (p != NULL)
  {
    return rfc7668_input(p, &mchdrv_netif);
  }
}

int peer_btadder_to_llip6addr(void)
{
  int err = -1;
  uint8_t brouter_eui64_addr[8];

  /* convert BT address to EUI64 addr */
  //  ble_addr_to_eui64(brouter_eui64_addr, brouter_bdaddr, 0);
  //
  //  rfc7668_set_peer_addr_eui64(NULL, brouter_eui64_addr, sizeof(brouter_eui64_addr));

  rfc7668_set_peer_addr_mac48(NULL, brouter_bdaddr, sizeof(brouter_bdaddr), 0);

  return 0;
}

void
rfc7668_create_ip6_linklocal_address(struct netif *netif, u8_t from_mac_48bit)
{
  u8_t i, addr_index;

  LWIP_ASSERT_CORE_LOCKED();

  LWIP_ASSERT("rfc7668_create_ip6_linklocal_address: invalid netif", netif != NULL);
  //iid_addr[0]^= 0x02;
  /* Link-local prefix. */
  ip_2_ip6(&netif->ip6_addr[0])->addr[0] = PP_HTONL(0xfe800000ul);
  ip_2_ip6(&netif->ip6_addr[0])->addr[1] = 0;

  /* Generate interface ID. */
  ip_2_ip6(&netif->ip6_addr[0])->addr[2] = (iid_addr[3] << 24 ) | (iid_addr[2] << 16) | (iid_addr[1] << 8)|(iid_addr[0]);
  ip_2_ip6(&netif->ip6_addr[0])->addr[3] = (iid_addr[7] << 24 ) | (iid_addr[6] << 16) | (iid_addr[5] << 8)|(iid_addr[4]);
  /* Set a link-local zone. Even though the zone is implied by the owning
  * netif, setting the zone anyway has two important conceptual advantages:
  * 1) it avoids the need for a ton of exceptions in internal code, allowing
  *    e.g. ip6_addr_cmp() to be used on local addresses;
  * 2) the properly zoned address is visible externally, e.g. when any outside
  *    code enumerates available addresses or uses one to bind a socket.
  * Any external code unaware of address scoping is likely to just ignore the
  * zone field, so this should not create any compatibility problems. */
  ip6_addr_assign_zone(ip_2_ip6(&netif->ip6_addr[0]), IP6_UNICAST, netif);

  /* Set address state. */
#if LWIP_IPV6_DUP_DETECT_ATTEMPTS
  /* Will perform duplicate address detection (DAD). */
  netif_ip6_addr_set_state(netif, 0, IP6_ADDR_TENTATIVE);
#else
  /* Consider address valid. */
  netif_ip6_addr_set_state(netif, 0, IP6_ADDR_PREFERRED);
#endif /* LWIP_IPV6_AUTOCONFIG */
}


int peer_link_addr_to_ip6addr(ip_addr_t *ip6_addr,struct lowpan6_link_addr  *hwaddr)
{
  ip_2_ip6(ip6_addr)->addr[0] = PP_HTONL(0xfe800000ul);
  ip_2_ip6(ip6_addr)->addr[1] = 0;

  /* Generate interface ID. */
  ip_2_ip6(ip6_addr)->addr[2] = (hwaddr->addr[3] << 24 ) | (hwaddr->addr[2] << 16) | (hwaddr->addr[1] << 8)|(hwaddr->addr[0]);
  ip_2_ip6(ip6_addr)->addr[3] = (hwaddr->addr[7] << 24 ) | (hwaddr->addr[6] << 16) | (hwaddr->addr[5] << 8)|(hwaddr->addr[4]);
}